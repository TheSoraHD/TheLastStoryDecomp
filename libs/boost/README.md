# Boost (subset, 2007 snapshot)

Headers of Boost used by the game (`boost::function`, `boost::bind`, `boost::mem_fn`), taken from
the `boostorg/*` repositories. Only the files the compiler opens for `function.hpp`, `bind.hpp` and
`mem_fn.hpp` are kept. Licence: `LICENSE_1_0.txt` (Boost Software License 1.0). Non-ASCII
characters (in comments) were replaced by ASCII equivalents, as sjiswrap rejects them; nothing else
changed.

- `boost/function.hpp` and `boost/function/*`: `boostorg/function` at commit `a7b9940`
  (2007-08-29, "Handle GCC's -fno-exceptions properly").
- `boost/bind.hpp`, `boost/bind/arg.hpp`, `boost/bind/bind_template.hpp` and
  `boost/bind/placeholders.hpp`: Boost 1.34.1. In 1.35.0 `boost::arg<I>` has a user-declared
  constructor, so the `static boost::arg<1> _1;` placeholders (the `__MWERKS__` branch) need dynamic
  initialisation and every file including `bind.hpp` gets an empty `__sinit_*` and a `.ctors`
  entry, which the game's units with `boost::bind` (e.g. 0x801C7390) do not have. In 1.34.1
  `arg<I>` is an empty class.
- `boost/tokenizer.hpp`, `boost/token_iterator.hpp`, `boost/token_functions.hpp` and what they
  open (`boost/iterator/*`, more of `boost/mpl/*` and `boost/type_traits/*`, `boost/implicit_cast.hpp`,
  ...): tag `boost-1.35.0` of `boostorg/tokenizer`, `iterator`, `mpl`, `type_traits`, `utility`,
  `detail`, `static_assert`, `conversion` (1.34.1 differs only in comments and in `at_end()`, which the
  game does not use).
- Everything else: tag `boost-1.35.0`.

## boost::tokenizer

`boost::tokenizer<boost::char_separator<char> >` over `std::string` is used by about 26 game files.
Its members are called out of line; the weak copies live in game/BattleCharaTask.cpp
(0x800EC2C4-0x800EE180), 0x80205BE8 (`operator++(int)`) and 0x8000EB8C (`std::string == const
char*`). Compiled from these headers (standalone, Wii/1.3 with the game flags):

| Address | Function | Result |
|---|---|---|
| 0x800EC2C4 | `tokenizer(const std::string&, const char_separator&)` | same instructions |
| 0x800EC440 | `char_separator(const char*, const char*, empty_token_policy)` | same instructions |
| 0x800EC534 | `~char_separator` | same instructions |
| 0x800EC5BC | `~tokenizer` | same instructions |
| 0x800ED42C | `~token_iterator` | same instructions |
| 0x800EC654, 0x800EDFE8 | `begin()`, `end()` | 8 lines differ (one store base register) |
| 0x800EC7F0 | `token_iterator(f, begin, end)` | register numbers differ, 4 instructions moved |
| 0x800ED4E0, 0x80205BE8 | `operator++()`, `operator++(int)` | register numbers differ, 6 instructions moved |
| 0x8000EB8C | `operator==(const std::string&, const char*)` | 31 instructions differ |

What they show about MSL is in `docs/msl_cpp.md` (string iterators are a class, copy constructor,
`find`, `end`, two-level `assign`).

## Why this Boost.Function version

No released version fits the DOL; the window between two trunk commits of 2007 does:

- The vtable used by `assign_to` is a guarded function-local static initialised at run time
  (guard byte at `0x8087F0E8`, vtable at `0x807C7C40`, in `0x801B2A80`). Boost 1.34.x and the trunk
  before `de27ae9` (2007-09-13) do this; 1.35.0 uses a constant-initialised static instead
  (`static const vtable_type stored_vtable = {...}`), which compiles without a guard.
- The game is compiled with exceptions off, and MWCC rejects `try`/`throw` in any instantiated
  function. The `try` blocks in `operator=` and `swap` are wrapped in `#ifndef BOOST_NO_EXCEPTIONS`
  only from `a7b9940` (2007-08-29) on; 1.34.1 and 1.36.0 do not have it.
- The managers use the operations `clone`, `destroy`, `check_functor_type` and
  `get_functor_type` (no `move`, added in 1.37).

With this snapshot the invoker (`0x801B2CC8`) and the manager (`0x801B2CF8`) of the
`boost::bind` stored by `chr::ControlDraggedStart::vf10` match exactly, and the setter at
`0x8049103C` (`self_type(f).swap(*this)`) matches as with 1.35.0. The type names used by
`check_functor_type` are in the DOL (e.g. `"atn::Scene * (*)(atn::Sphere, void *)"`).

Whether the developers used that exact trunk revision or a release they patched cannot be told
from the binary; only the code it produces is checked.

## msl_shim

`msl_shim/` holds minimal stand-ins for the MSL C++ headers Boost includes that are not in
`libs/PowerPC_EABI_Support/include/stl` (`<memory>`, `<stdexcept>`, `<typeinfo>`, ...). They are
not reconstructions of MSL: only enough to compile Boost. `std::type_info` follows the CodeWarrior
layout (`{name, bases}`, compared with `strcmp`). `std::allocator::allocate` reports failure with
`fprintf` + `abort` like `basic_string` (0x801B2D3C: "Memory allocation failure").
