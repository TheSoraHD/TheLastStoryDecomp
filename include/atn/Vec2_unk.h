#ifndef ATN_VEC2_UNK_H
#define ATN_VEC2_UNK_H

#include "math/Vec.h"

namespace atn {
// inferred: 2D vector of the dialog layout (weak copies 0x800F84BC and 0x800F833C of another file; a
// different class from Vector2_unk, whose copies at 0x805030FC are in game/SoftKeyboardU.cpp; also the
// 2D vectors of Event::Manager, which call the same weak copies). Name invented.
struct Vec2_unk {
    Vec2_unk() {}
    Vec2_unk(float x_, float y_) : x(x_), y(y_) {}
    // inferred: copied with paired singles (weak copy 0x804827CC, Event::Manager::vf10)
    Vec2_unk(const Vec2_unk& v) { __Vec2Copy(&x, &v.x); }
    Vec2_unk& operator=(const Vec2_unk& v) {
        __Vec2Copy(&x, &v.x);
        return *this;
    }
    float x, y;
};
} // namespace atn

#endif
