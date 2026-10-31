#ifndef ATN_COLOR4F_UNK_H
#define ATN_COLOR4F_UNK_H

#include "atn/D2A_Common.h"

namespace atn {

// inferred: [0,1] float channel to 0..255
inline unsigned int ColorToU8_unk(float f) {
    if (f >= 1.0f) {
        return 255;
    }
    if (f <= 0.0f) {
        return 0;
    }
    return (unsigned int)(f * 255.0f + 0.5f);
}

// inferred: RGBA color of the engine (0.0-1.0; constructor 0x800D8814, ToU32 0x800F8348). It derives from
// D2A_DrawColor: game/GameConfig_unk.cpp assigns a Color4f_unk with the implicit
// D2A_DrawColor::operator= (0x80042108). Other files still declare their own view of it.
struct Color4f_unk : public D2A_DrawColor {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    void Set_unk(float r_, float g_, float b_, float a_) { // inferred: weak copy of another file
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    unsigned int ToU32() const {
        unsigned int ur = ColorToU8_unk(r);
        unsigned int ug = ColorToU8_unk(g);
        unsigned int ub = ColorToU8_unk(b);
        unsigned int ua = ColorToU8_unk(a);
        return (ua << 24) | (ur << 16) | (ug << 8) | ub;
    }
};

} // namespace atn

#endif
