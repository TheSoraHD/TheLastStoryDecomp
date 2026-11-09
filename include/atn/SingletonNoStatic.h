#pragma once

namespace atn {

// inferred: empty base of the singletons that keep their instance outside the class (RTTI only, e.g.
// MP::ManagerMP at 0x807916B8)
template <class T>
class SingletonNoStatic {};

} // namespace atn
