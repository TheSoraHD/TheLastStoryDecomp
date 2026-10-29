#ifndef ATN_AABB_UNK_H
#define ATN_AABB_UNK_H

#include "math/Vec.h"

namespace atn {

// inferred: axis aligned box (min, max); the box utilities are in atn/Geometry_unk.cpp and the
// box tests of the collision shapes in atn/ColliTest_unk.cpp
struct AABB_unk {
    AABB_unk() {}
    AABB_unk(const Vector3& min, const Vector3& max) : mMin(min), mMax(max) {}
    AABB_unk Merge_unk(const AABB_unk& o) const; // 0x80070C98: union of both boxes
    Vector3 mMin;
    Vector3 mMax;
};

} // namespace atn

#endif
