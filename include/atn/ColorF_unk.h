#ifndef ATN_COLORF_UNK_H
#define ATN_COLORF_UNK_H

// inferred: color as four floats and the material color (weak copies 0x8000DD14 and 0x8000DD98 in
// Event/Puppeteer_CharaTask, also called by game/Em316LaserSphere.cpp); names invented

#include "atn/Material_unk.h"

struct ColorF_unk {
    // inferred: weak copy 0x8000DD14 (Event/Puppeteer_CharaTask)
    ColorF_unk(const GXColor& c) {
        r = c.r / 255.0f;
        g = c.g / 255.0f;
        b = c.b / 255.0f;
        a = c.a / 255.0f;
    }
    // inferred: member-wise copy (lfs/stfs in the in-place copy of vector<ColorF_unk>::push_back, 0x8000DE44)
    ColorF_unk(const ColorF_unk& o) : r(o.r), g(o.g), b(o.b), a(o.a) {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

// inferred: color of a material (weak copy 0x8000DD98)
extern "C" inline GXColor fn_8000DD98(Material_unk* material) {
    return material->mColor;
}

#endif
