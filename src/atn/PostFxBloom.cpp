// atn/PostFxBloom.cpp: efecto de post-proceso de resplandor (bloom).
#include "atn/PostFx.h"
#include "math/Vec.h"

extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);

// inferred: render parameters (lbl_8087EFA8, fields named by offset; see atn/LinearAllocator.cpp)
struct BloomColor_unk {
    float r, g, b, a;
};
struct BloomParams_unk {
    unsigned char m00[0x58];
    int m58;      // 0x58: inferred: debug display
    int m5C;      // 0x5C: inferred: size shift
    int m60;
    int m64;      // 0x64: inferred: number of additive passes
    float m68;    // 0x68: inferred: intensity
    float m6C;    // 0x6C: inferred: blur length X
    float m70;    // 0x70: inferred: blur length Y
    float m74;    // 0x74: inferred: blur angle
    float m78[5]; // 0x78: inferred: blur weights
    BloomColor_unk m8C[3]; // 0x8C: inferred: color of each threshold pass
    int mBC[3];   // 0xBC: inferred: TEV bias of each pass
    int mC8[3];   // 0xC8: inferred: TEV scale of each pass
};
extern "C" BloomParams_unk* lbl_8087EFA8;
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_800BFC18(PostFxFrameBuffer_unk* fb, BloomColor_unk color, float a, float b);
extern "C" void fn_8005DF90(void* dbg, int color, GXTexObj* obj, int arg, float x, float y, float z, float w,
                            float h);

// inferred: translation matrix by value (names invented)
static inline atn::Matrix3x4 MakeTrans_unk(const atn::Vector3& v) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, v.x, v.y, v.z);
    return m;
}

// NONMATCHING: 96.87% - register allocation (k1 kept in a register across the first GXSetTevKColor,
// loop counters in r25/r26)
// inferred: directional blur pass: draws `src` five times with offsets of (dx, dy) and copies the
// result to `dst`
extern "C" void fn_800A981C(GXTexObj* src, void* dst, int width, int height, int format, const float* weights,
                            int forward, float dx, float dy) {
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
    GXColor k0 = {(unsigned char)(255.0f * weights[0] * inv), (unsigned char)(255.0f * weights[1] * inv),
                  (unsigned char)(255.0f * weights[2] * inv), (unsigned char)(255.0f * weights[3] * inv)};
    GXColor k1 = {0, 0, 0, 0};
    k1.r = (unsigned char)(255.0f * weights[4] * inv);
    GXSetTevKColor(0, k0);
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
    dx /= width;
    dy /= height;
    if (forward != 0) {
        atn::Matrix3x4 m0;
        m0.SetIdentity();
        atn::Matrix3x4 m1 = MakeTrans_unk(atn::Vector3(dx, dy, 0.0f));
        atn::Matrix3x4 m2 = MakeTrans_unk(atn::Vector3(2.0f * dx, 2.0f * dy, 0.0f));
        atn::Matrix3x4 m3 = MakeTrans_unk(atn::Vector3(3.0f * dx, 3.0f * dy, 0.0f));
        atn::Matrix3x4 m4 = MakeTrans_unk(atn::Vector3(4.0f * dx, 4.0f * dy, 0.0f));
        GXLoadTexMtxImm(m0.m, 0x1E, 1);
        GXLoadTexMtxImm(m1.m, 0x21, 1);
        GXLoadTexMtxImm(m2.m, 0x24, 1);
        GXLoadTexMtxImm(m3.m, 0x27, 1);
        GXLoadTexMtxImm(m4.m, 0x2A, 1);
    } else {
        atn::Matrix3x4 m0;
        m0.SetIdentity();
        atn::Matrix3x4 m1 = MakeTrans_unk(atn::Vector3(-dx, -dy, 0.0f));
        atn::Matrix3x4 m2 = MakeTrans_unk(atn::Vector3(-2.0f * dx, -2.0f * dy, 0.0f));
        atn::Matrix3x4 m3 = MakeTrans_unk(atn::Vector3(dx, dy, 0.0f));
        atn::Matrix3x4 m4 = MakeTrans_unk(atn::Vector3(2.0f * dx, 2.0f * dy, 0.0f));
        GXLoadTexMtxImm(m0.m, 0x1E, 1);
        GXLoadTexMtxImm(m1.m, 0x21, 1);
        GXLoadTexMtxImm(m2.m, 0x24, 1);
        GXLoadTexMtxImm(m3.m, 0x27, 1);
        GXLoadTexMtxImm(m4.m, 0x2A, 1);
    }
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

namespace atn {

class PostFxBloom : public IPostFx {
public:
    PostFxBloom();
    virtual ~PostFxBloom() {}
    virtual void vf10(void* camera);

    GXTexObj m04;
    GXTexObj m24;
};

PostFxBloom::PostFxBloom() {}

// NONMATCHING: 85.65% - register allocation and the u16 width/height expressions of the copies; the
// original keeps more values in f28-f31 (blur factors and the additive color)
void PostFxBloom::vf10(void* camera) {
    int fullW = lbl_8087EEE0->mWidth;
    int fullH = lbl_8087EEE0->mHeight;
    int div = 1 << lbl_8087EFA8->m5C;
    int w = fullW / div;
    int h = fullH / div;
    void* image0 = lbl_8087EF8C->fn_800A96AC(w, h, 6);
    GXInitTexObj(&m04, image0, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&m04, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    void* image1 = lbl_8087EF8C->fn_800A96AC(w, h, 6);
    GXInitTexObj(&m24, image1, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&m24, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    int halfW = fullW / 2;
    int halfH = fullH / 2;
    GXTexObj half;
    void* image2 = lbl_8087EF8C->fn_800A96AC(halfW, halfH, 6);
    GXInitTexObj(&half, image2, halfW, halfH, 6, 0, 0, 0);
    GXInitTexObjLOD(&half, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    int w2 = w * 2;
    int h2 = h * 2;
    GXTexObj save;
    void* image3 = lbl_8087EF8C->fn_800A96AC(w2, h2, 6);
    GXInitTexObj(&save, image3, w2, h2, 6, 0, 0, 0);
    // inferred: saves the corner of the frame buffer that the passes draw over
    GXSetTexCopySrc(0, 0, w2, h2);
    GXSetTexCopyDst(w2, h2, 6, 0);
    GXCopyTex(image3, 0);
    // inferred: half size copy of the frame buffer
    GXSetTexCopySrc(0, 0, fullW, fullH);
    GXSetTexCopyDst((unsigned short)fullW / 2, (unsigned short)fullH / 2, 6, 1);
    GXCopyTex(image2, 0);
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
    GXSetTexCopySrc(0, 0, w2, h2);
    GXSetTexCopyDst(w, h, 6, 1);
    GXCopyTex(image0, 1);
    GXPixModeSync();
    // inferred: brightness threshold passes
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetBlendMode(0, 1, 0, 3);
    fn_800763FC(lbl_8087EEE0, &m04, 0);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    fn_80075F58(lbl_8087EEE0, 0, 0, w, h);
    fn_800BFB70(lbl_8087EFB4);
    for (int i = 0; i < 3; i++) {
        GXSetTevColorIn(0, 0xF, 8, 0xE, 0xF);
        GXSetTevColorOp(0, 0, lbl_8087EFA8->mBC[i], lbl_8087EFA8->mC8[i], 1, 0);
        GXSetTevSwapMode(0, 0, 0);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTevKColorSel(0, 0xC);
        GXSetTevDirect(0);
        BloomColor_unk& c = lbl_8087EFA8->m8C[i];
        GXColor color = {(unsigned char)(255.0f * c.r), (unsigned char)(255.0f * c.g), (unsigned char)(255.0f * c.b),
                         (unsigned char)(255.0f * c.a)};
        GXSetTevKColor(0, color);
        GXSetBlendMode(1, 2, 0, 3);
        fn_800BFB70(lbl_8087EFB4);
    }
    GXSetBlendMode(0, 1, 0, 3);
    GXSetTexCopySrc(0, 0, w, h);
    GXSetTexCopyDst(w, h, 6, 0);
    GXCopyTex(image1, 0);
    GXPixModeSync();
    // inferred: blur in two directions
    fn_800A981C(&m24, image0, w, h, 6, lbl_8087EFA8->m78, 0, cos(lbl_8087EFA8->m74) * lbl_8087EFA8->m6C,
                -sin(lbl_8087EFA8->m74) * lbl_8087EFA8->m6C);
    fn_800A981C(&m04, image0, w, h, 6, lbl_8087EFA8->m78, 0, -sin(lbl_8087EFA8->m74) * lbl_8087EFA8->m70,
                cos(lbl_8087EFA8->m74) * lbl_8087EFA8->m70);
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
    fn_80075DEC(lbl_8087EEE0);
    // inferred: adds the bloom to the frame buffer
    GXSetBlendMode(1, 1, 1, 3);
    float intensity = lbl_8087EFA8->m68;
    BloomColor_unk color = {intensity, intensity, intensity, 1.0f};
    fn_800763FC(lbl_8087EEE0, &m04, 0);
    for (int i = 1; i <= lbl_8087EFA8->m64; i++) {
        fn_800BFC18(lbl_8087EFB4, color, 0.0f, 0.0f);
    }
    GXSetBlendMode(0, 1, 0, 3);
    if (lbl_8087EFA8->m58 != 0) {
        fn_8005DF90(lbl_8087EEB0, -1, &m24, 0, 16.0f, 16.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
        fn_8005DF90(lbl_8087EEB0, -1, &m04, 0, 16.0f, 32.0f + fullH / 3.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
    }
}

} // namespace atn
