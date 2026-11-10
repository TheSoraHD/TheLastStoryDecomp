#ifndef MATH_VECASSIGN_UNK_H
#define MATH_VECASSIGN_UNK_H

#include "math/Vec.h"

namespace atn {
// inferred: Vector3 copy through one more inline level: gives the dst/src registers of the
// original in the __VecCopy (e.g. atn/Camera_unk, atn/DrawHelper); name invented. Not the
// member Assign_unk (z-first copy).
inline void VecAssign_unk(Vector3& dst, const Vector3& src) {
    dst = src;
}

} // namespace atn

#endif
