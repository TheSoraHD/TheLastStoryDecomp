#ifndef ATN_VECTOR4_UNK_H
#define ATN_VECTOR4_UNK_H

#include "math/Vec.h"

namespace atn {

// inferred: 4 float vector copied with two paired singles (uv rectangles of the button icons, rows
// of the color matrix). Weak copies 0x803D6FEC, 0x803D7000 and 0x803D7014 (game/GameUI.cpp). Name
// invented.
struct Vector4_unk {
    Vector4_unk() {}
    Vector4_unk(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    Vector4_unk(const Vector4_unk& v) { __Vec4Copy(&x, &v.x); }
    Vector4_unk& operator=(const Vector4_unk& v) {
        __Vec4Copy(&x, &v.x);
        return *this;
    }
    float x, y, z, w;
};

} // namespace atn

#endif
