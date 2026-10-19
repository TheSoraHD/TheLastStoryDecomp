#include "atn/Texture_unk.h"
#include "std/vector.h"

// inferred: GX/OS functions not in atn/GXDecl.h (enums as int)
extern "C" {
int GXGetTexObjFmt(const GXTexObj* obj);
unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format, unsigned char mipmap,
                                unsigned char maxLod);
void GXInvalidateTexAll();
void GXFlush();
void GXSetTevKColor(int id, GXColor color);
void GXSetTevKColorSel(int stage, int sel);
void GXSetTevKAlphaSel(int stage, int sel);
void GXSetTevSwapModeTable(int table, int red, int green, int blue, int alpha);
void GXSetTevSwapMode(int stage, int rasSel, int texSel);
void GXSetTevColorIn(int stage, int a, int b, int c, int d);
void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
void GXSetTevColorOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void DCInvalidateRange(void* addr, unsigned int size);
void DCFlushRange(void* addr, unsigned int size);
}

// inferred: render state and camera (automatic names, as in other atn files)
struct RenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
struct Camera_unk {
    unsigned char m000[0x79C];
    void* m79C; // inferred: copy texture buffer
};
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void fn_80071E04(RenderState_unk* rs);
extern "C" void fn_80075F58(RenderState_unk* rs, int x, int y, int w, int h); // inferred: set viewport
extern "C" void fn_80075DEC(RenderState_unk* rs);
extern "C" void fn_800763FC(RenderState_unk* rs, atn::Texture_unk* texture, int map); // inferred: loads a texture
extern "C" void fn_800BDB58(Camera_unk* camera, void* render, float alpha, int arg);  // inferred: queues a render
extern "C" void fn_800BFB70(Camera_unk* camera); // inferred: draws a full screen quad
extern "C" void fn_800C0FBC(void* image, int format, int width, int height); // inferred: untiles an image
extern "C" void fn_800D6E48(void* src, void* dst, int width, int height);    // inferred: tiles to RGBA8
extern "C" void fn_806140C0();

namespace atn {

// inferred: RGBA color (name invented)
struct CompositeColor_unk {
    CompositeColor_unk() {}
    CompositeColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    CompositeColor_unk operator*(float f) const { return CompositeColor_unk(r * f, g * f, b * f, a * f); }
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

// inferred: renderer interface (see atn/VideoTHP.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C(); // 0x80046218
    virtual void vf10(Camera_unk* camera) = 0;
    virtual int vf14() = 0;
    virtual void vf18() = 0;
    virtual void vf1C(); // 0x800461FC
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
    virtual ~TextureCompositeCopy() {}
    virtual void vf0C();

    Texture_unk* m04;
    CompositeColor_unk m08;
};

// inferred from __vt__Q23atn25TextureCompositeCopyAlpha
class TextureCompositeCopyAlpha : public TextureCompositeCommand {
public:
    virtual ~TextureCompositeCopyAlpha() {}
    virtual void vf0C();

    Texture_unk* m04;
    CompositeColor_unk m08;
};

// inferred: list of commands (one more destructor level than std::vector in the original)
struct TextureCompositeCommandList_unk : public std::vector<TextureCompositeCommand*> {};

// inferred from __vt__Q23atn16TextureComposite: renders its commands into a texture (with mipmaps)
class TextureComposite : public IRender {
public:
    TextureComposite();
    virtual ~TextureComposite();
    virtual void vf10(Camera_unk* camera);
    virtual int vf14() { return 1; }
    virtual void vf18() {}

    void Request_unk(Texture_unk* texture, int format);

    int m04; // inferred: done
    int m08; // inferred: copy format (-1: RGBA8)
    Texture_unk* m0C;
    Texture_unk* m10; // inferred: 4-bit alpha texture
    TextureCompositeCommandList_unk mCommands; // 0x14
};

TextureComposite::TextureComposite() : m04(1), m0C(0), m10(0) {}

TextureComposite::~TextureComposite() {}

void TextureComposite::Request_unk(Texture_unk* texture, int format) {
    m08 = format;
    m0C = texture;
    m04 = 0;
    fn_800BDB58(lbl_8087EFB4, this, 0.0f, 1);
}

// inferred: number of mipmap levels of a texture (inline, name invented)
// inferred: the original reads the image through another view of the texture (the two loads of
// 0x28 are not merged); the view is a guess
struct TextureView_unk {
    unsigned char m00[0x28];
    unsigned char* mImage;
    unsigned char m2C[3];
    signed char mMips;
};
inline int GetMipCount_unk(Texture_unk* tex) {
    TextureView_unk* view = (TextureView_unk*)tex;
    if (view->mImage != 0) {
        return view->mMips;
    }
    return tex->m20.fn_80478198();
}

// NONMATCHING: 97.72% - the original loads the image pointer twice (test and variable), which
// shifts the saved registers by one; tried other views of the texture and statement orders
void TextureComposite::vf10(Camera_unk* camera) {
    fn_80071E04(lbl_8087EEE0);
    Texture_unk* tex = m0C;
    unsigned char* image = (unsigned char*)tex->mImage;
    int mips = GetMipCount_unk(tex);
    int w = GXGetTexObjWidth(&m0C->mObj);
    int h = GXGetTexObjHeight(&m0C->mObj);
    int format = m08;
    unsigned char* dst = image;
    if (format == -1) {
        format = 6;
    }
    int texFormat = GXGetTexObjFmt(&m0C->mObj);
    int direct = GXGetTexObjFmt(&m0C->mObj) == format;
    if (format == 0x20 && GXGetTexObjFmt(&m0C->mObj) == 0) {
        direct = 1;
    } else if (format == 0x28 && GXGetTexObjFmt(&m0C->mObj) == 1) {
        direct = 1;
    }
    unsigned char* buffer = (unsigned char*)camera->m79C;
    unsigned int bufferSize =
        GXGetTexBufferSize(lbl_8087EEE0->mWidth, lbl_8087EEE0->mHeight, 0x13, 0, 0);
    memset(buffer, 0, bufferSize);
    DCInvalidateRange(buffer, bufferSize);
    unsigned char* levels[16];
    for (int i = 0; i < mips; i++) {
        if (!direct) {
            texFormat = 6;
            int size = GXGetTexBufferSize(w, h, 6, 0, 0);
            levels[i] = buffer;
            dst = buffer;
            buffer += (size + 0x1F) / 0x20 * 0x20;
            DCInvalidateRange(dst, size);
        }
        for (unsigned int j = 0; j < mCommands.size(); j++) {
            GXInvalidateTexAll();
            GXPixModeSync();
            if (i == 0) {
                fn_80075F58(lbl_8087EEE0, 0, 0, w, h);
            } else {
                fn_80075F58(lbl_8087EEE0, 0, 0, w * 2, h * 2);
            }
            mCommands[j]->vf0C();
        }
        if (i == 0) {
            GXSetTexCopySrc(0, 0, w, h);
            GXSetTexCopyDst(w, h, format, 0);
        } else {
            GXSetTexCopySrc(0, 0, (unsigned short)w * 2, (unsigned short)h * 2);
            GXSetTexCopyDst(w, h, format, 1);
        }
        GXCopyTex(dst, 1);
        GXInvalidateTexAll();
        GXPixModeSync();
        GXFlush();
        fn_806140C0();
        dst += GXGetTexBufferSize(w, h, texFormat, 0, 0);
        w >>= 1;
        h >>= 1;
    }
    if (!direct) {
        int lw = GXGetTexObjWidth(&m0C->mObj);
        int lh = GXGetTexObjHeight(&m0C->mObj);
        unsigned int alphaOffset = 0;
        for (int i = 0; i < mips; i++) {
            unsigned char* level = levels[i];
            GXGetTexBufferSize(lw, lh, texFormat, 0, 0);
            unsigned int size = GXGetTexBufferSize(lw, lh, GXGetTexObjFmt(&m0C->mObj), 0, 0);
            fn_800C0FBC(level, 6, lw, lh);
            if (m10 != 0) {
                int aw = GXGetTexObjWidth(&m10->mObj) >> i;
                int ah = GXGetTexObjHeight(&m10->mObj) >> i;
                unsigned int alphaSize = GXGetTexBufferSize(aw, ah, GXGetTexObjFmt(&m10->mObj), 0, 0);
                unsigned char* alpha = (unsigned char*)m10->mImage + alphaOffset;
                alphaOffset += alphaSize;
                fn_800C0FBC(alpha, GXGetTexObjFmt(&m10->mObj), aw, ah);
                for (int y = 0; y < lh; y++) {
                    unsigned char* src = alpha + y * (aw / 2);
                    unsigned char* out = level + y * (lw * 4);
                    for (int x = 0; x < lw; x++) {
                        int v = src[x / 2];
                        if ((x & 1) == 0) {
                            v >>= 4;
                        }
                        v &= 0xF;
                        out[x * 4] = v | (v << 4);
                    }
                }
            }
            fn_800D6E48(level, image, lw, lh);
            lw >>= 1;
            lh >>= 1;
            image += size;
        }
    }
    memset(camera->m79C, 0, bufferSize);
    DCFlushRange(camera->m79C, bufferSize);
    fn_80075DEC(lbl_8087EEE0);
    for (unsigned int j = 0; j < mCommands.size(); j++) {
        delete mCommands[j];
    }
    m04 = 1;
    mCommands.__shrink_unk(0);
}

// NONMATCHING (data only): the original pools 255.0f before 0.25f in .sdata2
void TextureCompositeMaskIntensitySingle::vf0C() {
    GXSetNumTexGens(3);
    GXSetNumChans(0);
    GXSetNumTevStages(5);
    GXSetNumIndStages(0);
    fn_800763FC(lbl_8087EEE0, m08, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    fn_800763FC(lbl_8087EEE0, m04, 1);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    fn_800763FC(lbl_8087EEE0, m0C, 2);
    GXSetTexCoordGen2(2, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevKColor(0, (m10 * 0.25f).ToGX());
    GXSetTevKColor(1, m20.ToGX());
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 0, 0, 0);
    GXSetTevSwapModeTable(2, 1, 1, 1, 1);
    GXSetTevSwapModeTable(3, 2, 2, 2, 2);

    GXSetTevSwapMode(0, 1, 1);
    GXSetTevKColorSel(0, 0x11);
    GXSetTevColorIn(0, 0xF, 0xE, 8, 0xF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetTevKAlphaSel(0, 0x11);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 6, 4, 7);

    GXSetTevSwapMode(1, 2, 2);
    GXSetTevKColorSel(1, 0x15);
    GXSetTevColorIn(1, 0xF, 0xE, 8, 0);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevOrder(1, 0, 0, 0xFF);
    GXSetTevDirect(1);
    GXSetTevKAlphaSel(1, 0x15);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(1, 7, 6, 4, 0);

    GXSetTevSwapMode(2, 3, 3);
    GXSetTevKColorSel(2, 0x19);
    GXSetTevColorIn(2, 0xF, 0xE, 8, 0);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevOrder(2, 0, 0, 0xFF);
    GXSetTevDirect(2);
    GXSetTevKAlphaSel(2, 0x19);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(2, 7, 6, 4, 0);

    GXSetTevSwapMode(3, 0, 0);
    GXSetTevColorIn(3, 0xF, 8, 0, 0xF);
    GXSetTevColorOp(3, 0, 0, 0, 1, 0);
    GXSetTevOrder(3, 2, 2, 0xFF);
    GXSetTevAlphaOp(3, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(3, 7, 7, 7, 0);
    GXSetTevDirect(3);

    GXSetTevSwapMode(4, 0, 0);
    GXSetTevKColorSel(4, 0xC);
    GXSetTevColorIn(4, 0xF, 0xE, 0, 0xF);
    GXSetTevColorOp(4, 0, 0, 2, 1, 0);
    GXSetTevOrder(4, 0xFF, 0xFF, 0xFF);
    GXSetTevAlphaOp(4, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(4, 7, 7, 7, 0);
    GXSetTevDirect(4);

    GXSetZMode(0, 3, 0);
    GXSetBlendMode(1, 4, 5, 3);
    fn_800BFB70(lbl_8087EFB4);
}

void TextureCompositeCopy::vf0C() {
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
    GXSetTevSwapModeTable(2, 0, 1, 2, 3);
    GXSetTevSwapModeTable(3, 0, 1, 2, 3);
    fn_800763FC(lbl_8087EEE0, m04, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevKColor(0, m08.ToGX());
    GXSetTevKColorSel(0, 0xC);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevColorIn(0, 0xF, 8, 0xE, 0xF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetZMode(0, 3, 0);
    GXSetBlendMode(0, 4, 5, 3);
    fn_800BFB70(lbl_8087EFB4);
}

void TextureCompositeCopyAlpha::vf0C() {
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetTevSwapModeTable(0, 3, 3, 3, 3);
    fn_800763FC(lbl_8087EEE0, m04, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevKColor(0, m08.ToGX());
    GXSetTevKColorSel(0, 0xC);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevColorIn(0, 0xF, 8, 0xE, 0xF);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaIn(0, 7, 4, 6, 7);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetZMode(0, 3, 0);
    GXSetBlendMode(0, 4, 5, 3);
    fn_800BFB70(lbl_8087EFB4);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
}

} // namespace atn
