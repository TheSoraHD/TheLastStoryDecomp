#ifndef CHR_FUNCPAIR_UNK_H
#define CHR_FUNCPAIR_UNK_H

#include <boost/function.hpp>

// inferred: two actions run one after the other, turned into a boost::function (name invented).
// Its constructor (0x8019C8A8), the boost::function constructors (0x8019C94C, 0x8019CA80) and the
// rest of its functor_manager (up to 0x8019D634) are weak copies first emitted in
// chr/ControlStickByQuark.cpp; chr::ControlRevival::vf10 calls them. The arguments are taken by
// value: with references MWCC inlines the constructor, the original calls it.
struct FuncPair_unk {
    FuncPair_unk(boost::function<void()> a, boost::function<void()> b) : m00(a), m14(b) {}
    void operator()() {
        m00();
        m14();
    }
    boost::function<void()> m00;
    boost::function<void()> m14;
};

#endif
