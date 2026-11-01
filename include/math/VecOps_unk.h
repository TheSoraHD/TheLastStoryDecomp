#ifndef MATH_VECOPS_UNK_H
#define MATH_VECOPS_UNK_H

// Small vector helpers that are not members of atn::Vector3 (names invented).

#include "math/Vec.h"

// inferred: scalar times vector (weak copy 0x803FAE8C)
inline atn::Vector3 operator*(float f, const atn::Vector3& v) {
    return atn::Vector3(v * f);
}

// inferred: converts three angles from degrees to radians in place (weak copy 0x80243EF8, atn/EffectSequencer_unk)
inline void fn_DegToRad_unk(float* v) {
    float x = v[0];
    float y = v[1];
    float z = v[2];
    x = 0.017453292f * x;
    y = 0.017453292f * y;
    z = 0.017453292f * z;
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

#endif
