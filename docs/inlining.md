# MWCC inlining rules (Wii/1.3, `-O4,p -inline auto -ipa file`)

Measured with synthetic code (`tools/inline_bench.py`) and checked against game functions. Every
Wii version tested behaves the same (0x4201_127, 1.0RC1, 1.0a, 1.0, 1.1, 1.3, 1.5, 1.6, 1.7) and so
does GC/3.0a5.2. A "statement" below is one simple assignment such as `p[i] = p[i + 1] * k + p[i + 2];`.

## R1. Return by value of a class with a destructor: never inlined

An inline function that returns by value an object whose class has a non-trivial destructor is
always called, even with five statements. POD returns are inlined normally.
Examples: `tokenizer::begin()`/`end()` and `token_iterator::operator++(int)`.

## R2. By-value parameter of a class with a destructor: never inlined

The same applies when a parameter is passed by value and its class has a destructor.
Examples: `LoadSeTableTask::Create` and the `LoadSeTableTask` constructor (both take a
`boost::function` by value), and the `token_iterator` constructor (`char_separator` by value).

Functions under R1/R2 do not count as inline candidates for R3.

## R3. Inlining budget per caller (only with `-ipa file`)

The decision is taken per calling function and is **all or nothing** over its direct inline
candidates: either all of them are inlined or none is.

- A single candidate is always inlined, whatever its size (tested up to 3000 statements).
- The cost depends on order. The first candidate costs its size, and every later one costs about
  1.3 times its size. With little code of its own, the caller accepts about 200 units:

  | Candidates (in call order) | Result |
  |---|---|
  | 150, 5 | inlined |
  | 170, 1 | inlined |
  | 1, 170 | called (1 + 1.3 × 170 > 200) |
  | 150, 100 | called |

- The budget grows with the caller's own code. Every candidate was inlined once the caller had at
  least this many statements of its own:

  | Candidates | Own statements needed |
  |---|---|
  | 150 + 100 | 120 (100 is not enough) |
  | 200 + 200 | 200 |
  | 300 + 300 | 300 |
  | 100 × 4 | 220 |

- Decisions are made bottom-up. An inline function that rejects its own candidates is inlined into
  its caller with its calls left as they are, and those calls are not reconsidered there.
- Nothing is exempt: blocks, loops, templates, members, constructors, forwarders and one-line
  accessors (`return x;`) all count.
- The pragmas `inline_max_size`, `inline_max_total_size`, `inline_depth` and `always_inline`, as well
  as `__inline`, are accepted but change nothing under IPA. Neither do the `-inline` or `-O` variants.

## R4. Empty destructors are elided

An inline empty destructor, or an empty destructor defined in the same file, disappears even when
the caller is over budget. Other empty functions are still called.

## Without `-ipa file`

There is no budget: every inline function is inlined. However, non-inline templates (e.g.
`operator==` for strings, `boost::function::operator()`) are not inlined.

## Practical consequences for matching

- If a game function calls a small inline member out of line, the usual cause is that its caller is
  over budget (R3), or the callee falls under R1/R2. It is not a sign that the member was defined out
  of line.
- In an over-budget function, every inline call becomes a weak copy, emitted after its first user.
  The same inline function can be inlined into a small function of the same file. Example:
  the `atn::Csv` constructor (0x8004203C), called by large functions and inlined by small ones in
  game/Em203GreaterDaemonRed.cpp.
- Check R1/R2 before blaming the budget: a by-value `boost::function` or a returned iterator is
  enough to force the call.
- Confirmed in game code:
  - EnvSeBase fn_803614E8 inlines all of the tokenizer except `begin`/`end`/`++` (R1);
  - LastCocoonEgg fn_8045374C, which is over budget, contains no inline expansion at all (R3).

## Open case: LoadSeTableTask::vf0C (0x803EEEDC)

The original inlines `FixedString_unk::operator=`, `std::string(const char*)`, `~basic_string`, the
`boost::function` copy and destructor, and the iterator `*` and `!=`. It calls out of line the
`atn::Csv` and `gmk::SeTable` constructors, the `char_separator` and tokenizer constructors and
destructors, `~token_iterator` (three times), `operator==` (six times) and `operator()`.

Such a partial result never happens with the rules above, and `!=` is inlined while the
`~token_iterator` of the same temporary is called. Either the source has a structure not yet found
(e.g. the inlined members sit inside another function inlined raw), or there is another mechanism.
