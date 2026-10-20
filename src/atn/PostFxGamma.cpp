// atn/PostFxGamma.cpp: gamma (tone) post effect: averages the luminance of the last frames on a 1x1
// texture and maps the frame through a gamma ramp chosen by that luminance.
#include "atn/PostFx.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"

extern "C" double pow(double x, double y);
extern "C" unsigned int GXGetTexObjFmt(const GXTexObj* obj);
extern "C" void GXSetIndTexOrder(int stage, int coord, int map);
extern "C" void GXSetIndTexCoordScale(int stage, int scaleS, int scaleT);
extern "C" void GXSetIndTexMtx(int id, const float offset[2][3], signed char scaleExp);
extern "C" void GXSetTevIndirect(int stage, int indStage, int format, int biasSel, int matrixSel, int wrapS, int wrapT,
                                 unsigned char addPrev, unsigned char utcLod, int alphaSel);

// inferred: parameters of the effect (0x34 bytes at lbl_8087EFA8+0x3DC; constructor 0x800BC2E0 in
// atn/RenderSetting_unk). Names invented.
struct GammaParams_unk {
    GammaParams_unk(); // 0x800BC2E0
    int mDebug;        // 0x00: inferred: debug display
    int mEnable;       // 0x04
    float mDepth;      // 0x08: inferred: > 0 draws the depth texture before the copy
    float mGamma[8];   // 0x0C: exponent of each row of the ramp
    float mIndX;       // 0x2C: inferred: indirect offset of the ramp lookup
    float mIndY;       // 0x30: inferred: indirect offset of the final lookup
};

// inferred: render parameters (lbl_8087EFA8, fields named by offset; see atn/PostFxBloom.cpp)
struct GammaRenderParams_unk {
    unsigned char m000[0x3DC];
    GammaParams_unk mGamma; // 0x3DC
};
extern "C" GammaRenderParams_unk* lbl_8087EFA8;

// inferred: frame buffer object (lbl_8087EFB4); only the member used here
struct GammaFrameBuffer_unk {
    unsigned char m000[0x74C];
    GXTexObj m74C; // 0x74C: inferred: depth texture
};

extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_8005DF90(void* dbg, int color, GXTexObj* obj, int arg, float x, float y, float z, float w,
                            float h);
extern "C" void fn_800C0508(PostFxFrameBuffer_unk* fb, int arg);
extern "C" void fn_800C0D4C(void* image, int format, int width, int height); // inferred: converts to texture tiles

// inferred: scales an indirect matrix until its values are below 1 (exp gets the scale exponent)
extern "C" void fn_800ACFC8(float m[2][3], int* exp);

// inferred: limits of the ramp values (writable globals in .sdata; names invented)
float gGammaRampMin_unk = 0.0f;
float gGammaRampMax_unk = 1.0f;

#define GAMMA_MIN(a, b) ((a) < (b) ? (a) : (b))
#define GAMMA_MAX(a, b) ((a) > (b) ? (a) : (b))

// inferred: integer log2 (name invented)
inline int Log2_unk(int v) {
    int r = 0;
    const int b[] = {0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000};
    const int s[] = {1, 2, 4, 8, 16};
    for (int i = 4; i >= 0; i--) {
        if (v & b[i]) {
            v >>= s[i];
            r |= s[i];
        }
    }
    return r;
}

// inferred: largest power of two not above v (name invented)
inline int Pow2Floor_unk(int v) {
    return 1 << Log2_unk(v);
}

// inferred: float color (names invented)
struct GammaColor_unk {
    GammaColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

inline GXColor ToGXColor_unk(const GammaColor_unk& c) {
    GXColor r;
    r.r = 255.0f * c.r;
    r.g = 255.0f * c.g;
    r.b = 255.0f * c.b;
    r.a = 255.0f * c.a;
    return r;
}

// inferred: max that returns a reference (std::max)
inline const float& MaxRef_unk(const float& a, const float& b) {
    return a < b ? b : a;
}

extern "C" void fn_800ACFC8(float m[2][3], int* exp) {
    *exp = 0;
    int first = 1;
    int again;
    do {
        float mx = m[0][0];
        again = 0;
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < 3; j++) {
                mx = MaxRef_unk(mx, m[i][j]);
            }
        }
        if (atn::Abs_unk(mx) >= 1.0f) {
            again = 1;
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    m[i][j] *= 0.5f;
                }
            }
            if (!first) {
                (*exp)++;
            }
            first = 0;
        }
    } while (again);
}

namespace atn {

class PostFxGamma : public IPostFx {
public:
    PostFxGamma();
    virtual ~PostFxGamma() {}
    virtual void vf10(void* camera);
    void fn_800AD340();
    void fn_800AD484();
    void fn_800AD648();

    GammaParams_unk mParams;  // 0x004
    GXTexObj mTex[8];         // 0x038: inferred: copies of the frame, smaller each time
    Texture_unk mAverage;     // 0x138: inferred: 1x1 average luminance
    int mNumFrames;           // 0x168: inferred: frames in mFrames
    int mFrame;               // 0x16C: inferred: next entry of mFrames
    Texture_unk mFrames[8];   // 0x170: inferred: luminance of the last frames
    Texture_unk mLut;         // 0x2F0: inferred: 64x1 lookup table of this frame
    Texture_unk mRamp;        // 0x320: inferred: 64x8 gamma ramps
};

PostFxGamma::PostFxGamma() : mNumFrames(0), mFrame(0) {
    mAverage.fn_800D5B58(1, 1, 1, 1, "AverageLum");
    GXInitTexObjLOD(&mAverage.mObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    for (int i = 0; i < 8; i++) {
        mFrames[i].fn_800D5B58(1, 1, 1, 1, "LumFrame");
        GXInitTexObjLOD(&mFrames[i].mObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    }
    mLut.fn_800D5B58(0x40, 1, 1, 1, "GammaLut");
    GXInitTexObjLOD(&mLut.mObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    mRamp.fn_800D5B58(0x40, 8, 1, 1, "GammaRamp");
    GXInitTexObjLOD(&mRamp.mObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    mParams = lbl_8087EFA8->mGamma;
    fn_800AD484();
}

// inferred: rebuilds the ramps when the parameters change
// NONMATCHING: 81% - the original keeps the flag in one register and a dead induction variable
void PostFxGamma::fn_800AD340() {
    GammaRenderParams_unk* rp = lbl_8087EFA8;
    const GammaParams_unk& p = rp->mGamma;
    int changed = 0;
    for (int i = 0; i < 8; i++) {
        changed = changed || mParams.mGamma[i] != p.mGamma[i];
    }
    if (changed) {
        mParams = rp->mGamma;
        fn_800AD484();
    }
}

// inferred: builds the ramp texture (one row per exponent)
// NONMATCHING: 99.19% - r30/r31 swapped (exponent pointer and int->float constant)
void PostFxGamma::fn_800AD484() {
    unsigned char* data = (unsigned char*)mRamp.mImage;
    for (int y = 0; y < GXGetTexObjHeight(&mRamp.mObj); y++) {
        unsigned char* row = data + y * 0x40;
        for (int x = 0; x < GXGetTexObjWidth(&mRamp.mObj); x++) {
            float v = (float)x / 63.0f;
            float r = v / GAMMA_MAX((float)pow(v, mParams.mGamma[y]), 0.001f) / 4.0f;
            float c = r < gGammaRampMin_unk ? gGammaRampMin_unk : (r > gGammaRampMax_unk ? gGammaRampMax_unk : r);
            row[x] = 255.0f * c;
        }
    }
    fn_800C0D4C(data, GXGetTexObjFmt(&mRamp.mObj), GXGetTexObjWidth(&mRamp.mObj), GXGetTexObjHeight(&mRamp.mObj));
}

// inferred: average of the luminance of the last frames into mAverage
void PostFxGamma::fn_800AD648() {
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetNumTexGens(8);
    GXSetNumChans(0);
    GXSetNumTevStages(8);
    GXSetNumIndStages(0);
    GammaColor_unk color(0.125f, 0.125f, 0.125f, 0.125f);
    GXSetTevKColor(0, ToGXColor_unk(color));
    for (int i = 0; i < 8; i++) {
        fn_800763FC(lbl_8087EEE0, &mFrames[(i + mFrame) % 8].mObj, i);
        if (i > 0) {
            GXSetTevColorIn(i, 0xF, 0xE, 8, 0);
        } else {
            GXSetTevColorIn(i, 0xF, 0xE, 8, 0xF);
        }
        GXSetTevKColorSel(i, 0xC);
        GXSetTevColorOp(i, 0, 0, 0, 1, 0);
        GXSetTevOrder(i, 0, i, 0xFF);
        GXSetTevDirect(i);
    }
    fn_80075F58(lbl_8087EEE0, 0, 0, 1, 1);
    fn_800BFB70(lbl_8087EFB4);
    GXPixModeSync();
    GXSetTexCopySrc(0, 0, 1, 1);
    GXSetTexCopyDst(1, 1, 0x28, 0);
    GXCopyTex(mAverage.mImage, 1);
}

// NONMATCHING: 97.09% - register allocation of the log2 helpers and of the downsampling loop (the original
// does not reuse cw * 2 across the viewport and the copy)
void PostFxGamma::vf10(void* camera) {
    if (lbl_8087EFA8->mGamma.mEnable == 0) {
        return;
    }
    GXPixModeSync();
    fn_800C0508(lbl_8087EFB4, 0);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 1, 2, 3);
    GXSetTevSwapModeTable(2, 0, 1, 2, 3);
    GXSetTevSwapModeTable(3, 0, 1, 2, 3);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevSwapMode(2, 0, 0);
    GXSetTevSwapMode(3, 0, 0);
    int fullW = lbl_8087EEE0->mWidth;
    int fullH = lbl_8087EEE0->mHeight;
    GXTexObj* tex;
    int count = 0;
    GXSetBlendMode(0, 4, 1, 3);
    int w = fullW / 2;
    int h = fullH / 2;
    if (mParams.mDepth > 0.0f) {
        GXSetNumTexGens(1);
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetNumIndStages(0);
        GXSetTevOp(0, 3);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetTevDirect(0);
        fn_800763FC(lbl_8087EEE0, &((GammaFrameBuffer_unk*)lbl_8087EFB4)->m74C, 0);
        fn_80075F58(lbl_8087EEE0, 0, 0, w * 2, h * 2);
        fn_800BFB70(lbl_8087EFB4);
        GXPixModeSync();
    }
    GXSetTexCopySrc(0, 0, w * 2, h * 2);
    GXSetTexCopyDst(w, h, 1, 1);
    void* images[8];
    images[0] = lbl_8087EF8C->fn_800A96AC(w, h, 1);
    tex = mTex;
    GXInitTexObj(tex, images[0], w, h, 1, 0, 0, 0);
    GXInitTexObjLOD(tex, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXCopyTex(images[0], 1);

    int size = GAMMA_MIN(Pow2Floor_unk(w), Pow2Floor_unk(h));
    int sw = size;
    int sh = size;
    GXTexObj* obj = tex;
    void** image = images;
    while (sw >= GXGetTexObjWidth(&mAverage.mObj)) {
        int cw = GAMMA_MIN(sw, sh);
        image++;
        obj++;
        count++;
        GXPixModeSync();
        GXInvalidateTexAll();
        GXSetNumTexGens(1);
        GXSetNumChans(0);
        GXSetNumTevStages(1);
        GXSetNumIndStages(0);
        GXSetTevOp(0, 3);
        GXSetTevOrder(0, 0, 0, 0xFF);
        GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
        GXSetTevDirect(0);
        fn_80075F58(lbl_8087EEE0, 0, 0, cw * 2, sh * 2);
        fn_800BFB70(lbl_8087EFB4);
        GXPixModeSync();
        GXSetTexCopySrc(0, 0, cw * 2, sh * 2);
        GXSetTexCopyDst(cw, sh, 0x28, 1);
        if (GXGetTexObjWidth(&mAverage.mObj) >= cw) {
            GXCopyTex(mFrames[mFrame++].mImage, 1);
            mNumFrames = GAMMA_MIN(mFrame, 8);
            mFrame = mFrame % 8;
            break;
        }
        *image = lbl_8087EF8C->fn_800A96AC(cw, sh, 1);
        GXCopyTex(*image, 1);
        GXInitTexObj(obj, *image, cw, sh, 1, 0, 0, 0);
        GXInitTexObjLOD(obj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        fn_800763FC(lbl_8087EEE0, obj, 0);
        sw = GAMMA_MAX(1, cw >> 2);
        sh = GAMMA_MAX(1, sh >> 2);
    }
    GXPixModeSync();
    GXInvalidateTexAll();
    fn_800AD648();

    GXSetNumTexGens(2);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetBlendMode(0, 1, 0, 3);
    fn_800763FC(lbl_8087EEE0, &mRamp.mObj, 0);
    fn_800763FC(lbl_8087EEE0, &mAverage.mObj, 1);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    Matrix3x4 m;
    m.SetIdentity();
    m.m[1][1] = 0.0f;
    GXLoadTexMtxImm(m.m, 0x1E, 1);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(0, 1, 1);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndirect(0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
    float ind[2][3];
    ind[0][0] = 0.0f;
    ind[0][1] = 0.0f;
    ind[0][2] = 0.0f;
    ind[1][0] = mParams.mIndX;
    ind[1][1] = 0.0f;
    ind[1][2] = 0.0f;
    int exp = 0;
    fn_800ACFC8(ind, &exp);
    GXSetIndTexMtx(1, ind, exp);
    fn_80075F58(lbl_8087EEE0, 0, 0, 0x40, 1);
    fn_800BFB70(lbl_8087EFB4);
    GXPixModeSync();
    GXSetTexCopySrc(0, 0, 0x40, 1);
    GXSetTexCopyDst(0x40, 1, 0x28, 0);
    GXCopyTex(mLut.mImage, 1);
    GXPixModeSync();
    GXInvalidateTexAll();

    GXSetNumTexGens(3);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetBlendMode(0, 1, 0, 3);
    fn_800763FC(lbl_8087EEE0, &((GammaFrameBuffer_unk*)lbl_8087EFB4)->m74C, 0);
    fn_800763FC(lbl_8087EEE0, &mLut.mObj, 1);
    fn_800763FC(lbl_8087EEE0, mTex, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(2, 1, 4, 0x3C, 0, 0x7D);
    Matrix3x4 m2;
    m2.SetIdentity();
    m2.m[0][0] = 0.0f;
    m2.m[1][1] = 0.0f;
    GXLoadTexMtxImm(m2.m, 0x1E, 1);
    GXSetTevOp(0, 3);
    GXSetTevDirect(0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevColorIn(1, 0xF, 0, 8, 0xF);
    GXSetTevColorOp(1, 0, 0, 2, 1, 0);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(0, 2, 2);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndirect(1, 0, 0, 0, 1, 0, 0, 0, 0, 0);
    float ind2[2][3];
    ind2[0][0] = mParams.mIndY;
    ind2[0][1] = 0.0f;
    ind2[0][2] = 0.0f;
    ind2[1][0] = 0.0f;
    ind2[1][1] = 0.0f;
    ind2[1][2] = 0.0f;
    int exp2 = 0;
    fn_800ACFC8(ind2, &exp2);
    GXSetIndTexMtx(1, ind2, exp2);
    fn_80075DEC(lbl_8087EEE0);
    GXSetBlendMode(0, 4, 1, 3);
    fn_800BFB70(lbl_8087EFB4);

    if (lbl_8087EFA8->mGamma.mDebug != 0) {
        float x = 8.0f;
        for (int i = 0; i < count; i++) {
            fn_8005DF90(lbl_8087EEB0, -1, tex, 0, x, 16.0f, 0.0f, 64.0f, 64.0f);
            x += 72.0f;
            tex++;
        }
        fn_8005DF90(lbl_8087EEB0, -1, &mAverage.mObj, 0, 8.0f, (float)fullH - 96.0f, 0.0f, 64.0f, 64.0f);
        fn_8005DF90(lbl_8087EEB0, -1, &mRamp.mObj, 0, 80.0f, (float)fullH - 96.0f, 0.0f, 64.0f, 64.0f);
        fn_8005DF90(lbl_8087EEB0, -1, &mLut.mObj, 0, 152.0f, (float)fullH - 96.0f, 0.0f, 64.0f, 8.0f);
    }
}

} // namespace atn
