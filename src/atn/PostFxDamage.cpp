// atn/PostFxDamage.cpp: damage post effect (rotating blur of a reduced copy of the frame, drawn through
// the "slime_edge" texture with a tint).
#include "atn/PostFx.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"
#include "atn/DrawHelper.h"

extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
extern "C" void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
extern "C" void GXSetTevKAlphaSel(int stage, int sel);

// inferred: render parameters (lbl_8087EFA8, fields named by offset; see atn/PostFxBloom.cpp)
struct DamageColor_unk {
    DamageColor_unk(const DamageColor_unk& c) {
        float r_ = c.r;
        float g_ = c.g;
        float b_ = c.b;
        float a_ = c.a;
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    float r, g, b, a;
};
struct DamageParams_unk {
    unsigned char m00[0x378];
    int m378;            // 0x378: inferred: debug display
    DamageColor_unk m37C; // 0x37C: inferred: tint
    float m38C;          // 0x38C: inferred: blur rotation
    float m390;          // 0x390: inferred: blur length
    float m394;          // 0x394: inferred: zoom of the edge texture
};
extern "C" DamageParams_unk* lbl_8087EFA8;

extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path); // inferred: loads a texture and waits
// inferred: 2D texture matrix (atn/TextureDraw_unk.cpp)
extern "C" void fn_800D5C84(float m[3][4], float sx, float sy, float c, float s, float tx, float ty);

// inferred: one blur matrix: scaled by (1 + sx, 1 + sy) and rotated by `angle` around the center
inline void SetBlurMtx_unk(float m[3][4], float sx, float sy, float angle) {
    fn_800D5C84(m, 1.0f + sx, 1.0f + sy, sin(angle), cos(angle), 0.5f * sx, 0.5f * sy);
}

// NONMATCHING: 97.93% - stack slots/order of the K colors and registers of the TEV loop (~8 variants)
// inferred: rotating/zooming blur pass: draws `src` five times with growing scale and rotation and copies
// the result to `dst` (same TEV setup as fn_800A981C of atn/PostFxBloom.cpp)
extern "C" void fn_800AB774(GXTexObj* src, void* dst, int width, int height, int format, const float* weights,
                            float angle, float length) {
    float defWeights[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    GXInvalidateTexAll();
    fn_80075F58(lbl_8087EEE0, 0, 0, width, height);
    if (weights == 0) {
        weights = defWeights;
    }
    int ksel[5] = {0x10, 0x14, 0x18, 0x1C, 0x11};
    float sum = 0.0f;
    for (int i = 0; i < 5; i++) {
        sum += weights[i];
    }
    float inv = 1.0f / sum;
    int c0 = 255.0f * weights[0] * inv;
    GXColor k1 = {0, 0, 0, 0};
    GXColor k0 = {(unsigned char)(255.0f * weights[0] * inv), (unsigned char)(255.0f * weights[1] * inv),
                  (unsigned char)(255.0f * weights[2] * inv), (unsigned char)(255.0f * weights[3] * inv)};
    k1.r = (unsigned char)(255.0f * weights[4] * inv);
    // inferred: the rounding error of the five weights goes to the first one
    GXColor k = k0;
    k.r += (unsigned char)(-c0 * 5 + 255);
    GXSetTevKColor(0, k);
    GXSetTevKColor(1, k1);
    GXSetNumTevStages(5);
    GXSetNumChans(1);
    GXSetNumIndStages(0);
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetNumTexGens(5);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    GXSetTexCoordGen2(2, 1, 4, 0x24, 0, 0x7D);
    GXSetTexCoordGen2(3, 1, 4, 0x27, 0, 0x7D);
    GXSetTexCoordGen2(4, 1, 4, 0x2A, 0, 0x7D);
    float sx = length * (1.0f / width);
    float sy = length * (1.0f / height);
    atn::Matrix3x4 m0;
    m0.SetIdentity();
    atn::Matrix3x4 m1;
    SetBlurMtx_unk(m1.m, sx, sy, angle);
    atn::Matrix3x4 m2;
    SetBlurMtx_unk(m2.m, 2.0f * sx, 2.0f * sy, 2.0f * angle);
    atn::Matrix3x4 m3;
    SetBlurMtx_unk(m3.m, 3.0f * sx, 3.0f * sy, -1.0f * angle);
    atn::Matrix3x4 m4;
    SetBlurMtx_unk(m4.m, 4.0f * sx, 4.0f * sy, -2.0f * angle);
    GXLoadTexMtxImm(m0.m, 0x1E, 1);
    GXLoadTexMtxImm(m1.m, 0x21, 1);
    GXLoadTexMtxImm(m2.m, 0x24, 1);
    GXLoadTexMtxImm(m3.m, 0x27, 1);
    GXLoadTexMtxImm(m4.m, 0x2A, 1);
    fn_800763FC(lbl_8087EEE0, src, 0);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    for (int i = 0; i < 5; i++) {
        if (i > 0) {
            GXSetTevColorIn(i, 0xF, 8, 0xE, 0);
        } else {
            GXSetTevColorIn(i, 0xF, 8, 0xE, 0xF);
        }
        GXSetTevKColorSel(i, ksel[i]);
        GXSetTevColorOp(i, 0, 0, 0, i == 4, 0);
        GXSetTevSwapMode(i, 0, 0);
        GXSetTevOrder(i, i, 0, 0xFF);
        GXSetTevDirect(i);
    }
    GXSetBlendMode(0, 4, 1, 3);
    fn_800BFB70(lbl_8087EFB4);
    GXSetTexCopySrc(0, 0, width, height);
    GXSetTexCopyDst(width, height, format, 0);
    GXCopyTex(dst, 0);
    GXPixModeSync();
    GXInvalidateTexAll();
}

// inferred: scale shift of the TEV stage for a color up to 4.0
inline int Shift_unk(float mx) {
    if (mx > 2.0f) {
        return 2;
    }
    int s = 0;
    if (mx > 1.0f) {
        s = 1;
    }
    return s;
}

namespace atn {

class PostFxDamage : public IPostFx {
public:
    PostFxDamage();
    virtual ~PostFxDamage() {}
    virtual void vf10(void* camera);
};

PostFxDamage::PostFxDamage() {}

// NONMATCHING: 98.11% - registers of the sizes/images at the start (w/h in r23/r22 in the original), the shift
// of the tint is computed with mfcr instead of branches and the K color slots are swapped (~12 variants)
void PostFxDamage::vf10(void* camera) {
    static int sLoaded;
    static Texture_unk sEdge;
    if (sLoaded == 0) {
        fn_800D594C(&sEdge, "slime_edge.texture");
        sLoaded = 1;
    }
    int fullW = lbl_8087EEE0->mWidth;
    int fullH = lbl_8087EEE0->mHeight;
    int w = fullW / 8;
    int h = fullH / 8;
    void* image0 = lbl_8087EF8C->fn_800A96AC(w, h, 6);
    static GXTexObj sBlur;
    GXInitTexObj(&sBlur, image0, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&sBlur, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    int halfW = fullW / 2;
    int halfH = fullH / 2;
    void* image1 = lbl_8087EF8C->fn_800A96AC(halfW, halfH, 6);
    GXTexObj half;
    GXInitTexObj(&half, image1, halfW, halfH, 6, 0, 0, 0);
    GXInitTexObjLOD(&half, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    int w2 = w * 2;
    int h2 = h * 2;
    void* image2 = lbl_8087EF8C->fn_800A96AC(w2, h2, 6);
    GXTexObj save;
    GXInitTexObj(&save, image2, w2, h2, 6, 0, 0, 0);
    // inferred: saves the corner of the frame buffer that the passes draw over
    GXSetTexCopySrc(0, 0, (unsigned short)w * 2, (unsigned short)h * 2);
    GXSetTexCopyDst((unsigned short)w * 2, (unsigned short)h * 2, 6, 0);
    GXCopyTex(image2, 0);
    // inferred: half size copy of the frame buffer
    GXSetTexCopySrc(0, 0, fullW, fullH);
    GXSetTexCopyDst((unsigned short)fullW / 2, (unsigned short)fullH / 2, 6, 1);
    GXCopyTex(image1, 0);
    GXPixModeSync();
    // inferred: scales it down to w x h
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetBlendMode(0, 1, 0, 3);
    fn_800763FC(lbl_8087EEE0, &half, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    fn_80075F58(lbl_8087EEE0, 0, 0, w2, h2);
    fn_800BFB70(lbl_8087EFB4);
    GXSetTexCopySrc(0, 0, (unsigned short)w * 2, (unsigned short)h * 2);
    GXSetTexCopyDst(w, h, 6, 1);
    GXCopyTex(image0, 1);
    GXPixModeSync();
    fn_800AB774(&sBlur, image0, w, h, 6, 0, lbl_8087EFA8->m38C, lbl_8087EFA8->m390);
    // inferred: restores the saved corner
    GXSetBlendMode(0, 1, 0, 3);
    GXSetNumTexGens(1);
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    fn_800763FC(lbl_8087EEE0, &save, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevOp(0, 0);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTevDirect(0);
    fn_80075F58(lbl_8087EEE0, 0, 0, w2, h2);
    fn_800BFB70(lbl_8087EFB4);
    // inferred: draws the blurred frame through the edge texture with the tint
    GXSetNumTexGens(2);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetNumIndStages(0);
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetChanCtrl(5, 0, 1, 1, 0, 0, 2);
    fn_800763FC(lbl_8087EEE0, &sEdge.mObj, 0);
    fn_800763FC(lbl_8087EEE0, &sBlur, 1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    float scale = 1.0f + lbl_8087EFA8->m394;
    float m[3][4];
    m[0][1] = m[0][2] = m[1][0] = m[1][2] = m[2][0] = m[2][1] = m[2][3] = 0.0f;
    m[2][2] = 1.0f;
    m[1][1] = m[0][0] = scale;
    m[1][3] = m[0][3] = -((scale - 1.0f) * 0.5);
    GXLoadTexMtxImm(m, 0x1E, 1);
    DamageColor_unk c = lbl_8087EFA8->m37C;
    float mx = c.r > (c.b > c.g ? c.b : c.g) ? c.r : (c.b > c.g ? c.b : c.g);
    int shift = Shift_unk(mx);

    float inv = 1.0f / (1 << shift);
    c.a = 1.0f;
    c.r *= inv;
    c.g *= inv;
    c.b *= inv;
    GXColor k = {(unsigned char)(255.0f * c.r), (unsigned char)(255.0f * c.g), (unsigned char)(255.0f * c.b),
                 (unsigned char)(255.0f * c.a)};
    GXSetTevKColor(0, k);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevColorIn(0, 0xF, 8, 0xE, 0xF);
    GXSetTevColorOp(0, 0, 0, shift, 1, 0);
    GXSetTevOrder(0, 1, 1, 0xFF);
    GXSetTevDirect(0);
    GXSetTevColorIn(1, 0xF, 0xF, 0xF, 0);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevKAlphaSel(1, 0x1C);
    GXSetTevAlphaIn(1, 6, 7, 4, 7);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevOrder(1, 0, 0, 0xFF);
    GXSetTevDirect(1);
    GXSetBlendMode(1, 4, 5, 3);
    fn_80075DEC(lbl_8087EEE0);
    fn_800BFB70(lbl_8087EFB4);
    GXSetBlendMode(0, 4, 1, 3);
    if (lbl_8087EFA8->m378 != 0) {
        lbl_8087EEB0->fn_8005DF90(-1, &sBlur, 0, 16.0f, 16.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
        lbl_8087EEB0->fn_8005DFC8(-1, &sEdge, 0, 16.0f, 32.0f + fullH / 3.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
    }
}

} // namespace atn
