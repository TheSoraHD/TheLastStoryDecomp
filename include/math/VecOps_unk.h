#ifndef MATH_VECOPS_UNK_H
#define MATH_VECOPS_UNK_H

// Small vector helpers that are not members of atn::Vector3 (names invented).

#include "math/Vec.h"

// inferred: scalar times vector (weak copy 0x803FAE8C)
inline atn::Vector3 operator*(float f, const atn::Vector3& v) {
    return atn::Vector3(v * f);
}

#endif
