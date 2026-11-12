#ifndef ATN_SINGLETONNOSTATIC_H
#define ATN_SINGLETONNOSTATIC_H

namespace atn {

// inferred: empty base of the singletons that keep their instance outside the class (RTTI only, e.g.
// MP::ManagerMP at 0x807916B8)
template <class T>
class SingletonNoStatic {
public:
    // inferred: empty; deleting an instance keeps the null test of the delete (Event processors,
    // fn_8054F380)
    ~SingletonNoStatic() {}
};

} // namespace atn

#endif
