#ifndef ATN_TEXTURECOMPOSITE_H
#define ATN_TEXTURECOMPOSITE_H

#include "atn/Texture_unk.h"
#include "atn/Color.h"
#include "atn/IRender.h"
#include "std/vector.h"

namespace atn {

// inferred: RGBA color of the commands (name invented)
struct CompositeColor_unk {
    CompositeColor_unk() {}
    CompositeColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    CompositeColor_unk operator*(float f) const { return CompositeColor_unk(r * f, g * f, b * f, a * f); }
    GXColor ToGX() const; // atn/TextureComposite.cpp
    float r, g, b, a;
};

// inferred from __vt__Q23atn23TextureCompositeCommand: one pass drawn into the composite
class TextureCompositeCommand {
public:
    virtual ~TextureCompositeCommand() {}
    virtual void vf0C() = 0; // inferred: draw
};

// note: the original emits the weak destructors of the commands in the order Mask, Copy, CopyAlpha
// and their vtables in the reverse order; this declaration order gives the vtables
// inferred from __vt__Q23atn35TextureCompositeMaskIntensitySingle
class TextureCompositeMaskIntensitySingle : public TextureCompositeCommand {
public:
    TextureCompositeMaskIntensitySingle(Texture_unk* a, Texture_unk* b, Texture_unk* c, Color4f_unk color0,
                                        Color4f_unk color1)
        : m04(a), m08(c), m0C(b) {
        m10.r = color0.r;
        m10.g = color0.g;
        m10.b = color0.b;
        m10.a = color0.a;
        m20.r = color1.r;
        m20.g = color1.g;
        m20.b = color1.b;
        m20.a = color1.a;
    }
    virtual ~TextureCompositeMaskIntensitySingle() {}
    virtual void vf0C();

    Texture_unk* m04;
    Texture_unk* m08;
    Texture_unk* m0C;
    CompositeColor_unk m10;
    CompositeColor_unk m20;
};

// inferred from __vt__Q23atn20TextureCompositeCopy
class TextureCompositeCopy : public TextureCompositeCommand {
public:
    TextureCompositeCopy(Texture_unk* tex, const Color4f_unk& color) : m04(tex) {
        m08.r = color.r;
        m08.g = color.g;
        m08.b = color.b;
        m08.a = color.a;
    }
    virtual ~TextureCompositeCopy() {}
    virtual void vf0C();

    Texture_unk* m04;
    CompositeColor_unk m08;
};

// inferred from __vt__Q23atn25TextureCompositeCopyAlpha
class TextureCompositeCopyAlpha : public TextureCompositeCommand {
public:
    TextureCompositeCopyAlpha(Texture_unk* tex, const Color4f_unk& color) : m04(tex) {
        m08.r = color.r;
        m08.g = color.g;
        m08.b = color.b;
        m08.a = color.a;
    }
    virtual ~TextureCompositeCopyAlpha() {}
    virtual void vf0C();

    Texture_unk* m04;
    CompositeColor_unk m08;
};

// inferred: list of commands (one more destructor level than std::vector in the original)
typedef std::vector<TextureCompositeCommand*> TextureCompositeCommandList_unk;

// inferred from __vt__Q23atn16TextureComposite: renders its commands into a texture (with mipmaps)
class TextureComposite : public IRender {
public:
    TextureComposite();
    virtual ~TextureComposite();
    virtual void vf10(RenderManager* view);
    virtual int vf14(const Matrix3x4* view, const Vector3* eye, const Frustum_unk* frustum, AABB_unk* box) {
        return 1;
    }
    virtual void vf18(RenderManager* view) {}

    void Request_unk(Texture_unk* texture, int format);
    void fn_800D8C0C(Texture_unk* alpha) { m10 = alpha; }
    void fn_800D8894(TextureCompositeCommand* command) { mCommands.push_back(command); } // weak copy 0x800D8894

    int m04; // inferred: done
    int m08; // inferred: copy format (-1: RGBA8)
    Texture_unk* m0C;
    Texture_unk* m10; // inferred: 4-bit alpha texture
    TextureCompositeCommandList_unk mCommands; // 0x14
};

} // namespace atn

#endif
