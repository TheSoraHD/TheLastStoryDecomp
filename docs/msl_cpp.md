# MSL C++ containers used by the game (reconstruction notes)

No MSL C++ source is available, so `include/std/string.h` and `include/std/vector.h` are
partial reconstructions. Only what has been checked against the DOL is written there.

## std::basic_string (12 bytes, short-string optimisation)

Inline accessors emitted as weak functions show the layout (same as libc++ on big-endian):

| Function | What it shows |
|---|---|
| `0x8000EC88`, `0x801F1970` | `size()`: long ? `word1` : `byte0 & 0x7F` |
| `0x8000ECA8` | `operator[]` (char): long ? `word2` : `this + 1` |
| `0x801F1990`, `0x801F19B4` | `operator[]` / `data()` (wchar_t): long ? `word2` : `this + 2` |
| `0x801F18BC` | `clear()` (wchar_t): short: keep bit 7 of byte0, write `L'\0'`; long: `data[0] = 0`, `size = 0` |
| `0x801F1908` | destructor: if long, `operator delete(word2)` (wchar_t copy has one more inlined level than the char one at `0x80013D60`) |

- long mode: `word0 = capacity | 0x80000000`, `word1 = size`, `word2 = data`
- short mode: `byte0 = size` (bit 7 clear), characters from the first aligned slot after it

`std::runtime_error` (used by `boost::bad_function_call`) does not use `basic_string`: it holds a
`std::tr1::shared_ptr<char>` to a `new char[]` copy of the message. Reconstructed in
`include/std/stdexcept.h` (used by the Boost shim): the inlined throw in
`boost::function0::operator()` (CharaTask::fn_8013A258, 0x8013A380) compiles to the same
instructions. Details that matter:
- `shared_ptr(Y* p, D d)` keeps `p` in a holder object while the control block is allocated
  (`new` of 0x10 bytes: vtable, use count 1, weak count 1, pointer); the holder's destructor
  survives as dead code (`b`; `bl __dla`) after `release()`.
- The pointer and the control block live in a base class with the destructor: the destructor of
  `runtime_error` tests the member address twice (`addic.; beq; beq`) before `release()`
  (0x806952C4, out of line).
- `new char[strlen(s) + 1]` gives `slwi; subf` (n * 2 - n) before `__nwa__FUl` (0x80084A78).
- The deleter (`std::detail::default_delete<char[]>`, empty) shares the address of the pointer
  (`get_deleter` returns `this + 0xC`, 0x8001A34C); `dispose` is `delete[] p` (0x8001A39C).
- The message of `bad_function_call` is an `@STRING@` pool of its inline constructor (in `.data`,
  0x80775BC8).

## std::vector (12 bytes)

`atn::FileList` (`0x8006BBCC`, `0x8006BC64`, both 100% in objdiff) shows:

- layout: `T* data`, `size`, `capacity`
- `clear()`: `n = size; p = data + n; size -= n;` then destroy backwards (`p->~T()` with null check)
- destructor: inlined through three levels (three `this` null checks); the storage class does
  `if (data) { clear(); operator delete(data); }`
- the `FileList` constructor calls a destroy loop where the end pointer is computed before the
  count (different register order than `clear()`); `__shrink_unk` stands in for it.

The class names `__vec_storage_unk` / `__vec_imp_unk` are invented; only the nesting depth is known.

These accessors are inline members (inferred): game functions still call them out of line (and call
`~basic_string(this, -1)` for locals) — presumably because of the inliner's size limits, not verified — while small members such as the range replace inline them.
Taking a member pointer in a test file forces the weak copy; all of the above then compile to the
same instructions (`size` and `operator[]` test the short case first).

`assign(const char*)` is inlined in two levels (0x80512944): `assign(s)` evaluates `size()` and calls
an inline `replace(0, n, s)`, which creates the iterator tag, calls `strlen` and then the range replace.

## Out-of-line members found so far

| Address (char) | Member | Status |
|---|---|---|
| `0x80013DC4` | `reserve(size_type)` | reconstructed, same instructions (standalone test compile) |
| `0x80013F78` | range replace `(pos, n1, first, last, tag)` | 61 of 182 lines differ (standalone, Wii/1.3): `j2 - j1` recomputed for the memmove, `tmp.size()` not folded |

Error reporting with exceptions off: `fprintf(stderr, msg); abort();` with the messages
`"basic_string::reserve length_error"`, `"basic_string: out_of_range"`,
`"basic_string: length_error"` and, in the allocator, `"Memory allocation failure"` (that one is
in an `@STRING@` pool of an inline function, in `.data`).

`traits::assign(c, T())` binds a constant temporary to a reference: the compiler materialises it as
a 1-byte object in `.sdata` (`lbl_8087D6C8`, `lbl_8087D6C9`), one per call site.

`abort` must be declared noreturn: the replace reuses `sz - rlen` computed in the second operand of an
`||` whose other path ends in `abort()`. The iterator tag is not zero-initialised when passed as
`Tag()`, which fits the MSL tags deriving from each other (an empty derived struct is not a POD).

Growth policy: short capacity 11 chars; `reserve` rounds `n + 16` down to a multiple of 16;
`replace` starts from `(cap + 15) & ~15` and doubles (`(cap * 2 + 15) & ~15`) until it fits.

## vector::push_back (draft)

Weak copies of 0x2F4 bytes for pointer elements (`0x800DB3EC`, `0x80244CCC`, `0x8043E1F0`, ...) and the
same code inlined in many constructors (334 uses of the growth constants). Reconstructed so far in
`include/std/vector.h`:

- fast path: `size++; data[size - 1] = x;`
- `"vector length_error"` checks, `max_size() = 0x3FFFFFFF`
- temporary buffer on the stack `{data, size, cap, &vector.cap (allocator), start}`, zeroed first
- growth: `cap < max/3` → `cap + max((cap + 1) * 3 / 5, n)`; `cap < 2*max/3` → `cap + max((cap + 1) / 2, n)`;
  otherwise `max`; `max` takes references (the operands live on the stack)
- the new element goes to `begin()[size]` with `begin() = data + start`, the old ones are copied in
  front with `memcpy`, then the old storage is cleared with `memset(old, 0, n * sizeof(T))`
- `size = 0`, then capacity, data and size are swapped with the buffer, whose destructor frees the old storage

The buffer keeps its fields in memory and its destruction tests the buffer address for null
(`addic. r3, r1, 0x14`): its destructor is in a base class (`__vec_split_base_unk`). `push_back` is
defined outside the class (its `"vector length_error"` literals are in the file string pool, after
the `""` of `new (..., "", "", 0)`), the allocation helper is in-class (its `"Memory allocation
failure"` is in an `@STRING@` pool). 62 of 189 lines still differ (standalone compile, `-ipa file`):
load order of size/capacity in the fast path, stack slots of the two growth temporaries, a separate
register for the new capacity, and `(data + size) - data` not folded before the `memcpy`.

### Element kinds (2026-10-30, branch `vector-push-back`)

- MSL dispatches on is_POD: MWCC has `__builtin_is_pod(T)` (no `__is_pod`). POD elements take the
  pointer path in `push_back` (size incremented first, then the element assigned, no placement-new null
  test) and are moved with separate decrements; non-POD elements keep placement new. The per-type copies
  of that path in `ShopColor.h`/`LibraryTask.h` are gone; `U16Table_unk` and `Xsca_unk` still need theirs.
- Pointer, int and float elements share ONE allocation function: its "Memory allocation failure"
  literal is a single weak `@STRING@` object (0x80775A88) referenced by 48 files. Weak `@STRING@`
  objects of inline members are merged by name by the linker, so a literal shared across files means a
  shared function (`__vec_word_alloc_unk::__allocate`, symbol renamed). Other element types keep one
  literal per `vector<T>::__allocate` (e.g. `vector<Vector3>`, 6 files). `n * sizeof(void*)` (not `n * 4`)
  so that `vector<float>(n)` reuses the product for its memset (0x8032B6F8).
- Same pattern in `std::list`: lists of pointers share 0x80779810 (17 files), now through
  `__list_ptr_alloc_unk::__allocate` (symbol renamed). 14 ups and 2 accepted drops (user decision
  2026-10-30): `Loader::fn_8046EF08` 98.36 -> 98.13 and `Get_unk<LoadCore>` 98.62 -> 98.49, both the
  list-end/node register residue of `Loader::Get_unk` (`Get_unk<LoadColliTree>` 96.75 -> 96.96).
- Open: in loops the original gives size r4 / cap r3 (`addi r4,r4,1; ...; add; stw x,-4`). Probe
  `pb/t1.cpp` (`while (id != -1) { v.push_back(id); id = next(); }`) reproduces it in isolation: addressing
  the element with the new size (`mData[mSize - 1]`, as the original) always gives size r3; addressing it
  with the old `n` (`mData[n]`, `&mData[n]`, `n++` + `[n - 1]`) gives size r4 but `addi r0; stw; stwx`.
  Accessors, cap locals, `cap > n`, if/else, the slow path in another inline, without `__recommend`:
  no change. ~35 more forms on 2026-10-30.

## std::list (partial, `include/std/list.h`)

Seen in the loader manager template `atn::LoadManager_unk::Get_unk` (`include/atn/LoadManager.h`,
weak copies in every `atn/LH_*.cpp`, e.g. 0x80472DCC):

- layout: `size`, then the sentinel node `{prev, next}`; nodes are `{prev, next, value}`
- `insert(pos, x)`: allocate the node (`operator new` + `"Memory allocation failure"`), keep it in a
  holder `{owner, node}` on the stack, construct the value with placement new, link it before
  `pos`, clear the holder, `++size`
- the holder stays in memory and its destructor is left as dead code after the link (`b` over a
  `bl __dl__`). Accessing it through a pointer reproduces that; with direct member access the
  compiler removes it, and with the destructor in a base class it adds a `this` check that the
  original does not have.

The search before the insertion is a `find_if(begin(), end(), pred)`-like loop: with `last` as a
separate variable the loop has the original's shape (increment first, `it != last` test, then the
predicate), and `end()` is recomputed after it.

### Position of insert and the list end register (2026-10-30, branch `list-get-unk`)

- `insert` takes its position as a `const_iterator` with a user-written copy constructor; `push_back`
  builds it in place (`insert(const_iterator(&mTail), x)`). The position then lives in a temporary and
  the list end gets a lower saved register than the new node (0x8046EF08, the `Get_unk` copies).
- `insert(end(), x)` converts a plain `iterator` and keeps the list end in a higher register than the
  node: that is the original of `fn_8046F014` (0x8046F014). Both forms exist in the game.
- The plain `iterator` stays trivially copyable: a copy constructor there fixes the push_back but moves
  every loop (`fn_800D8624` 100 -> 91, five other drops).
- The same register effect in loops: iterating with `const_iterator` (`const std::list<T>& l = m; for (it = l.begin(); ...)`)
  matches `Loader::fn_8046F4D4`/`fn_8046F544`; `clear()` erases through the const `begin()/end()` and
  `erase(first, last)` takes `const_iterator` (`Loader::~Loader`). Not enough for `fn_8046F460` (98.3%) nor
  `PacketPack::fn_8050DEA8` (99.7%, the inline `__Deactivate_unk` lookup modifies the element).
- The holder is released before the link (`linked = h->mNode; h->mNode = 0; link`); the store of 0 is
  scheduled after the link, and the volatile registers of the link match (`li r4,0; lwz r5`).

## Error reporting

`__msl_error(msg)` is an inline function (`fprintf(stderr, msg); abort();`), not a macro: the
message is loaded before the stream at every call site (0x800DB3EC, 0x80013F78, 0x80069BF4).

## Where the members live

- Members defined in the class body are inline; their literals go to `@STRING@<function>` pools.
- Template members defined outside the class put their literals into the file string pool.
- `reserve`, the range replace and the substring constructor are never instantiated in game
  objects (no copy and no error strings in their pools, e.g. `atn/LH_Motion.cpp`): they are declared
  in `std/string.h` and defined in `std/string_members.h`, which only test files include. Their
  symbols in `symbols.txt` use our mangled names so the linker can resolve the calls.
- The iterator tags are in `std/iterator.h`; Boost gets them through `libs/boost/msl_shim/iterator`.

## Inlined members seen in game code (`atn::LoadMotion::LoadMotion`, 0x80476220)

- `basic_string(const char*)`: zero the three words, `strlen`, `reserve(n)`, range replace
  `(0, 0, s, s + n)`.
- `rfind(c)`: data and size read in one branch on the long bit (short case first), then
  `if (sz - 1 < pos) pos = sz - 1` and a backwards loop.
- `operator=(const basic_string&)`: two short strings are copied word by word; otherwise
  `replace(0, size(), str.data(), str.data() + str.size())`, where this `size()` tests the long
  case first.
- `erase(pos, n)` goes through an inline `replace(pos, n, first, last)` (one more level: the tag
  copy takes the higher stack slot).
- `c_str()` returns `data()`; the substring is built by calling the substring constructor directly.

Solved (2026-10-18): while the string is constructed from a `const char*`, the original keeps its
address in a saved register (`addi r29, r1, 0x2c` ... `mr r3, r29`) because the constructor clears
the three words through a word array (`__zero()`, as in libc++: `unsigned long (&a)[3] = ...;
for (...) a[i] = 0;`). The cast of `this` to a word pointer makes the address a value of its own,
which is then reused for `reserve`, the range replace and any later use of the temporary. Writing
`mLong.mCap = 0; ...` lets the compiler fold the address into each instruction. With this
`atn::LoadMotion::LoadMotion` (0x80476220) matches. Likewise `data()` returns `(const T*)this + 1`
for a short string (and `operator=` takes the source data the same way): the address is then
hoisted out of loops into a saved register (`LoadEffect::vf0C`, 0x804732DC).

## Template parameters and members added for boost::tokenizer

`basic_string` now has the standard parameters `<T, char_traits<T>, allocator<T> >` (mangled
`basic_string<c,Q23std14char_traits<c>,Q23std12allocator<c>>`; the symbols were renamed, the binary
does not change). Checked against the tokenizer weak copies (see `libs/boost/README.md`):

- the iterators are a class holding a pointer (`__string_iterator_unk`, name invented):
  `tokenizer::begin` (0x800EC654) passes them by address to the token_iterator constructor;
- copy constructor: a short string is copied as a 3-word block (0x800EC7F0 stores word 1 before
  word 0), a long one with `reserve` and the range replace;
- `end()`, `find(c, pos)` and `compare` take data and size from one test; the short data pointer
  is `(const T*)this + 1` (with `mShort.mData` the address is not hoisted out of the find loop);
- `find` compares `c == *q` over `[p + pos, p + sz)`;
- `assign(first, last)` evaluates `size()` and calls an inline `replace(0, n, first, last)`, which
  creates the iterator tag as a named local.

## Why game functions call small inline members out of line

See docs/inlining.md: an over-budget caller inlines nothing (IPA budget), and functions that return
or take by value an object with a destructor are never inlined. This explains the out-of-line calls
to `size()`, `~basic_string`, the `atn::Csv` constructor and the tokenizer members.

## Changes of 2026-10-18 (game/EquipItemData.cpp, linked)

- `operator==(const basic_string&, const T*)` (weak copy 0x8000EB8C) now matches: `compare` keeps a
  copy of `n2` in a local, calls `__min(sz - pos1, n1)` and returns from the comparison of the
  lengths directly.
- `is_long()` reads the first word through a `const unsigned long*`: after an inlined `compare`
  the long bit is reloaded, as in the original.
- `do_replace` (real name from the USA RSO; was `__replace_unk`) is an ordinary member taking a `random_access_iterator_tag` (not a member
  template): its weak copy is then emitted at the end of the object, after `reserve`, and the
  error messages land in the file pool in the original order ("reserve length_error",
  "out_of_range", "length_error"). Game files whose pool has these messages include
  `std/string_members.h` (EquipItemData, GimmickManager); the weak copies are dropped by the
  linker.
- `operator+(const basic_string&, const T*)` and `operator+(const T*, const basic_string&)` are
  namespace templates (RSO: `__pl<c,...>__3std...`), declared friends of the class; weak copies
  0x8006D008 (atn/Font_unk) and 0x8006AF38 (atn/FileList).
- Streams (2026-10-29, rso-names-qA): `include/std/ios_base.h` (`ios_base`, `basic_ios`, `fpos`),
  `locale.h`, `streambuf.h`, `sstream.h` (`basic_stringbuf`), `ostream.h` (`basic_ostream` with
  virtual base `basic_ios`, `sentry`). The weak copies of std/allocator_char are explicit
  instantiations of members (`template int std::basic_streambuf<char>::sync();`): MWCC emits each
  one weak at that point of the file, in source order.
