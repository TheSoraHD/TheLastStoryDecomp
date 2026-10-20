#include "atn/D2A_Frame.h"

// inferred: 2D renderer instance (class unknown) and its textured quad drawing method
struct Renderer2D_unk {
    unsigned char m00[0x10];
    unsigned char m10[1]; // inferred: white texture
};
extern "C" Renderer2D_unk* lbl_8087EEB0;
extern "C" void fn_8005E3E4(Renderer2D_unk* renderer, unsigned int color, void* tex, float x, float y, float z,
                            float w, float h, float u0, float v0, float u1, float v1, int arg, int arg13);

namespace atn {

// inferred: converts a [0,1] float channel to 0..255 (inline, name invented; same as D2A_Line)
inline unsigned int fn_ToU8(float f) {
    return (f >= 1.0f) ? 255 : (f <= 0.0f) ? 0 : (unsigned int)(f * 255.0f + 0.5f);
}

// inferred: RGBA color (names invented)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(const Color4f_unk& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
    float r, g, b, a;
};

// inferred: outline parameters filled by D2A_Common::fn_801ED928 (offset and color)
struct D2A_Outline_unk {
    float x;
    float y;
    Color4f_unk mColor;
};

D2A_Frame::D2A_Frame(D2A_Data* owner, unsigned short id) : D2A_Common(owner, id) {
    mType = 2;
}

D2A_Frame::~D2A_Frame() {}

// NONMATCHING: 99.85% - only the float registers of the alpha test and the first color channel
// differ (f3/f5/f2 in the original, f2/f4/f5 here)
void D2A_Frame::vf18() {
    if (!fn_801EDC78()) {
        return;
    }
    D2A_DrawRect rect;
    D2A_DrawColor color;
    D2A_DrawExtra extra;
    D2A_Outline_unk outline;
    fn_801ED928(&rect, &color, &extra, &outline);
    if (color.a <= 0.0f) {
        return;
    }
    Color4f_unk oc(outline.mColor);
    oc.a *= color.a;
    unsigned int r = fn_ToU8(color.r);
    unsigned int g = fn_ToU8(color.g);
    unsigned int b = fn_ToU8(color.b);
    unsigned int a = fn_ToU8(color.a);
    unsigned int c = (a << 24) | (r << 16) | (g << 8) | b;
    r = fn_ToU8(oc.r);
    g = fn_ToU8(oc.g);
    b = fn_ToU8(oc.b);
    a = fn_ToU8(oc.a);
    unsigned int o = (a << 24) | (r << 16) | (g << 8) | b;
    void* tex = lbl_8087EEB0->m10;
    fn_8005E3E4(lbl_8087EEB0, c, tex, rect.x, rect.y, rect.z, rect.w, rect.h, 0.0f, 0.0f, 1.0f,
                1.0f, extra.m04, 1);
    if (m0E == 1) {
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z,
                    rect.w, rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
    } else if (m0E == 2) {
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
    } else if (m0E == 3) {
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w,
                    rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z,
                    rect.w, rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x + outline.x, rect.y - outline.y, 0.001f + rect.z,
                    rect.w, rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x - outline.x, rect.y - outline.y, 0.001f + rect.z,
                    rect.w, rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
        fn_8005E3E4(lbl_8087EEB0, o, tex, rect.x - outline.x, rect.y + outline.y, 0.001f + rect.z,
                    rect.w, rect.h, 0.0f, 0.0f, 1.0f, 1.0f, extra.m04, 1);
    }
}

} // namespace atn
