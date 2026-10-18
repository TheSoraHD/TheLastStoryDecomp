# Boost 1.34.1 (subset)

Headers of Boost 1.34.1 used by the game (`boost::function`, `boost::bind`, `boost::mem_fn`),
taken from the `boostorg/*` repositories at tag `boost-1.34.1`. Only the files the compiler opens for
`function.hpp`, `bind.hpp` and `mem_fn.hpp` are kept. Licence: `LICENSE_1_0.txt` (Boost Software
License 1.0). Files in Latin-1 were converted to UTF-8 (needed by sjiswrap); nothing else changed.

Why this version: the game's function managers use the operations `clone`, `destroy`,
`check_functor_type` and `get_functor_type` (no `move`, added in 1.37), the vtable is a guarded
function-local static (1.34-1.36), and a standalone compile of `boost::function2<atn::Scene*,
atn::Sphere, void*>` reproduces the manager at 0x80491280 and the invoker at 0x80491234 exactly.
The type names used by `check_functor_type` are in the DOL (e.g. `"atn::Scene * (*)(atn::Sphere, void *)"`).

`msl_shim/` holds minimal stand-ins for the MSL C++ headers Boost includes that are not in
`libs/PowerPC_EABI_Support/include/stl` (`<memory>`, `<stdexcept>`, `<typeinfo>`, ...). They are
not reconstructions of MSL: only enough to compile Boost. `std::type_info` follows the CodeWarrior
layout (`{name, bases}`, compared with `strcmp`).
