#ifndef ATN_COLOR_H
#define ATN_COLOR_H

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

// RGBA color of the engine (0.0-1.0). Real name from the RSO (SetColor__Q23atn5LightFRCQ23atn5Color,
// SetClearColor__Q23atn12RenderConfigFRCQ23atn5Color). Default constructor 0x80041B8C (weak),
// constructor 0x800D8814, operator= 0x80042108, ToU32 0x800F8348.
struct Color {
    Color() {}
    Color(float r_, float g_, float b_, float a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    // inferred: weak copy 0x800D8BB4
    Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
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
    float r, g, b, a;
};

// inferred: older names of atn::Color in this project
typedef Color D2A_DrawColor;
typedef Color Color4f_unk;

} // namespace atn

#endif
