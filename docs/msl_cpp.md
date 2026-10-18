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
reference-counted buffer (control block with vtable `lbl_80775B98`, `use_count`, `weak_count`, pointer).

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

Open problem: the original keeps the buffer in memory (it reloads its fields after every store and
call); our compile keeps them in registers and needs one more saved register.
