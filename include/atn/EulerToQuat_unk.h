#ifndef ATN_EULERTOQUAT_UNK_H
#define ATN_EULERTOQUAT_UNK_H

#include "math/Vec.h"
#include "nw4r/math/triangular.h"

namespace atn {

// inferred: rotation from euler angles (same as Quaternion_unk::fn_801498F0 of game/CharaTask.cpp); name
// invented
inline Quaternion_unk EulerToQuat_unk(const Vector3& rot) {
    Quaternion_unk q;
    float rx = rot.x;
    float ry = rot.y;
    float rz = rot.z;
    float cx, cy, cz;
    float sx, sy, sz;
    nw4r::math::SinCosRad(&sx, &cx, rx / 2.0f);
    nw4r::math::SinCosRad(&sy, &cy, ry / 2.0f);
    nw4r::math::SinCosRad(&sz, &cz, rz / 2.0f);
    float cycz = cy * cz;
    float sysz = sy * sz;
    q.w = cx * cycz + sx * sysz;
    q.x = sx * cycz - cx * sysz;
    q.y = cz * (cx * sy) + sz * (sx * cy);
    q.z = sz * (cx * cy) - cz * (sx * sy);
    return q;
}

} // namespace atn

#endif
