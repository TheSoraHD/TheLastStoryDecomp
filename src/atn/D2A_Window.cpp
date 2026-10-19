#include "atn/D2A_Window.h"

// inferred: texture lookup and 2D renderer (classes unknown, automatic names)
extern "C" void* lbl_8087F138;
extern "C" void* lbl_8087EEB0;
extern "C" void* fn_801F1098(void* textures, int id, int index);
extern "C" void fn_80061E2C(void* renderer, float x, float y, float z, float w, float h, unsigned int color,
                            void* tex0, void* tex1, int arg, float offsetX, float offsetY, float scaleX,
                            float scaleY);

namespace atn {

class D2A_Window;

// inferred: [0,1] float channel to 0..255 (name invented). Written with ifs: with this cost the
// inline candidates of D2A_Window::vf18 go over budget and are all called, as in the original.
inline unsigned int FloatToU8_unk(float f) {
    if (f >= 1.0f) {
        return 255;
    }
    if (f <= 0.0f) {
        return 0;
    }
    return (unsigned int)(f * 255.0f + 0.5f);
}

// inferred: RGBA color (copy 0x800D8BB4, ToU32 0x800F8348; both weak copies of other files)
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(const Color4f_unk& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
    unsigned int ToU32() const {
        unsigned int ur = FloatToU8_unk(r);
        unsigned int ug = FloatToU8_unk(g);
        unsigned int ub = FloatToU8_unk(b);
        unsigned int ua = FloatToU8_unk(a);
        return (ua << 24) | (ur << 16) | (ug << 8) | ub;
    }
    float r, g, b, a;
};

typedef void (*D2A_DrawCallback_unk)(D2A_Window* window, D2A_DrawColor* color, Color4f_unk* outlineColor);

// inferred: owner of the element; only the draw callback is used here
struct D2A_Owner_unk {
    D2A_DrawCallback_unk GetDrawCallback() const { return mA0; }

    unsigned char m00[0xA0];
    D2A_DrawCallback_unk mA0;
};

// inferred: color conversion of the draw color (same body as Color4f_unk::ToU32)
inline unsigned int fn_ColorToU32(const D2A_DrawColor& c) {
    unsigned int r = (c.r >= 1.0f) ? 255 : (c.r <= 0.0f) ? 0 : (unsigned int)(c.r * 255.0f + 0.5f);
    unsigned int g = (c.g >= 1.0f) ? 255 : (c.g <= 0.0f) ? 0 : (unsigned int)(c.g * 255.0f + 0.5f);
    unsigned int b = (c.b >= 1.0f) ? 255 : (c.b <= 0.0f) ? 0 : (unsigned int)(c.b * 255.0f + 0.5f);
    unsigned int a = (c.a >= 1.0f) ? 255 : (c.a <= 0.0f) ? 0 : (unsigned int)(c.a * 255.0f + 0.5f);
    return (a << 24) | (r << 16) | (g << 8) | b;
}

// inferred: outline parameters filled by D2A_Common::fn_801ED928 (offset and color)
struct D2A_Outline_unk {
    float x;           // 0x00
    float y;           // 0x04
    Color4f_unk mColor; // 0x08
};

D2A_Window::D2A_Window(D2A_Data* owner, unsigned short id) : D2A_Common(owner, id), m130(Vector2_unk(1.0f, 1.0f)) {
    mType = 3;
    m12C = 0;
    m138 = Vector2_unk(0.0f, 0.0f);
}

D2A_Window::~D2A_Window() {}

void D2A_Window::vf18() {
    if (!fn_801EDC78()) {
        return;
    }
    D2A_DrawRect rect;
    D2A_DrawColor color;
    D2A_DrawExtra extra;
    D2A_Outline_unk outline;
    fn_801ED928(&rect, &color, &extra, &outline);
    Color4f_unk outlineColor(outline.mColor);
    if (mFC->GetDrawCallback() != 0) {
        if (m0E == 0) {
            mFC->GetDrawCallback()(this, &color, 0);
        } else {
            mFC->GetDrawCallback()(this, &color, &outlineColor);
        }
    }
    if (color.a <= 0.0f) {
        return;
    }
    outlineColor.a *= color.a;
    fn_801FF8F0(m12C, fn_ColorToU32(color), rect.x, rect.y, rect.z, rect.w, rect.h, extra.m04);
    if (m0E == 1) {
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w,
                    rect.h, extra.m04);
    } else if (m0E == 2) {
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
    } else if (m0E == 3) {
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x + outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x, rect.y + outline.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x - outline.x, rect.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x, rect.y - outline.y, 0.001f + rect.z, rect.w, rect.h,
                    extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x + outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w,
                    rect.h, extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x + outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w,
                    rect.h, extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x - outline.x, rect.y - outline.y, 0.001f + rect.z, rect.w,
                    rect.h, extra.m04);
        fn_801FF8F0(m12C, outlineColor.ToU32(), rect.x - outline.x, rect.y + outline.y, 0.001f + rect.z, rect.w,
                    rect.h, extra.m04);
    }
}

void D2A_Window::fn_801FF8F0(int id, unsigned int color, float x, float y, float z, float w, float h,
                                    int arg) {
    void* tex0 = fn_801F1098(lbl_8087F138, id, 0);
    void* tex1 = fn_801F1098(lbl_8087F138, id, 1);
    void* renderer = lbl_8087EEB0;
    if (tex0 != 0 && tex1 != 0) {
        fn_80061E2C(renderer, x, y, z, w, h, color, tex0, tex1, arg, m138.x, m138.y, m130.x, m130.y);
    }
}

} // namespace atn
