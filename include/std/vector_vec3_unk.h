#ifndef STD_VECTOR_VEC3_UNK_H
#define STD_VECTOR_VEC3_UNK_H

#include "std/vector.h"
#include "math/Vec.h"

// inferred: asserts of the debug build, empty in release (they only count for the inline budget)
#define MSL_VEC3_ASSERT_UNK(x) ((void)0)

// inferred: std::vector<atn::Vector3>::push_back (weak copy 0x8032AC1C, first used by game/Em403WarLion.cpp and
// also called by Event/Manager, gmk/CharaPathMove and game/LastCocoonQuadruped). The release-mode assertions
// only count for the inlining budget (R3): Em403WarLion::fn_8032A9F4 is over it in the original and calls
// every member of the vector (docs/inlining.md). Included only by the units that call it.
// NONMATCHING: 97.47% - the capacity register and the stack slots of the growth temporaries (known push_back
// family, docs/mwcc-blockers-q1.md)
template <>
inline void std::vector<atn::Vector3>::push_back(const atn::Vector3& x) {
    MSL_VEC3_ASSERT_UNK(this != 0); MSL_VEC3_ASSERT_UNK(&x != 0); MSL_VEC3_ASSERT_UNK(this->mSize <= this->mCap);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1); MSL_VEC3_ASSERT_UNK(1);
    size_type n = this->mSize;
    if (n < this->mCap) {
        this->__push_fast_unk(n, x);
        return;
    }
    __push_back_slow_unk(x);
}


#endif
