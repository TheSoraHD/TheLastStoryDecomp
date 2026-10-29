// atn/PostFxGodRay.cpp: light shaft post effect (radial blur of the bright parts of the frame from the
// screen position of the light).
#include "atn/PostFx.h"
#include "atn/Texture_unk.h"
#include "atn/LoadHandle.h"
#include "atn/Sphere.h"
#include "math/Vec.h"

extern "C" void GXSetIndTexOrder(int stage, int coord, int map);
extern "C" void GXSetIndTexCoordScale(int stage, int scaleS, int scaleT);
extern "C" void GXSetIndTexMtx(int id, const float offset[2][3], signed char scaleExp);
extern "C" void GXSetTevIndWarp(int stage, int indStage, unsigned char signedOffset, unsigned char replaceMode,
                                int matrix);
extern "C" unsigned int GXGetTexObjFmt(const GXTexObj* obj);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void PSMTXTranspose(const Mtx src, Mtx dst);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

// inferred: texture class seen as another owner elsewhere (see gmk/AutomaticDoor.cpp)
struct GimmickUvOwner_unk {
    int fn_800D59B8(); // inferred: nonzero while loading
};
extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path); // inferred: loads a texture file

// inferred: float color (names invented)
struct GodRayColor_unk {
    GodRayColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: parameters of the effect (at lbl_8087EFA8+0x264, or from the scene). Names invented.
struct GodRayParams_unk {
    int mEnable;          // 0x00
    int mDebug;           // 0x04
    int mShift;           // 0x08: inferred: size of the work textures (frame size >> mShift)
    int mPasses;          // 0x0C
    int mUseDir;          // 0x10: inferred: light at the camera + direction * 800
    int mUseDepth;        // 0x14
    int mType;            // 0x18: 0 = point, 1 = direction, 2 = screen position
    float m1C;            // 0x1C: inferred: radial blur parameters
    float m20;
    float m24;
    GodRayColor_unk mColor; // 0x28
    atn::Vector3 mPos;    // 0x38
    float mRadius;        // 0x44
};

// inferred: render parameters (lbl_8087EFA8, fields named by offset; see atn/PostFxBloom.cpp)
struct GodRayRenderParams_unk {
    unsigned char m000[0x264];
    GodRayParams_unk mGodRay; // 0x264
};
extern "C" GodRayRenderParams_unk* lbl_8087EFA8;

// inferred: scene settings that can override the parameters (see atn/PostFxRain.cpp)
struct GodRayOverride_unk {
    unsigned char m000[0x194];
    int m194;
    GodRayParams_unk m198;
};

// inferred: frame buffer object (lbl_8087EFB4); only the members used here
struct GodRayFrameBuffer_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mEye;    // 0x10C
    atn::Vector3 mTarget; // 0x118
    unsigned char m124[0x15C - 0x124];
    atn::Matrix3x4 mView; // 0x15C
    unsigned char m18C[0x74C - 0x18C];
    GXTexObj m74C; // 0x74C: inferred: depth texture
    unsigned char m76C[0x77C - 0x76C];
    GXTexObj m77C; // 0x77C: inferred: color texture
};

extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_8005DF90(void* dbg, int color, GXTexObj* obj, int arg, float x, float y, float z, float w,
                            float h);
extern "C" void fn_800610A4(void* dbg, unsigned int color, float x, float y, float z, float w, float h); // inferred: rect
extern "C" void fn_800C0508(PostFxFrameBuffer_unk* fb, int arg);
extern "C" GodRayOverride_unk* fn_800C1424(PostFxFrameBuffer_unk* fb);
extern "C" void fn_800BFAC8(atn::Vector3* out, PostFxFrameBuffer_unk* fb, const atn::Vector3* pos); // inferred: to screen
extern "C" void fn_800C0D4C(void* image, int format, int width, int height); // inferred: converts to texture tiles
extern "C" void fn_800A981C(GXTexObj* src, void* dst, int width, int height, int format, const float* weights,
                            int forward, float dx, float dy);

using namespace atn;

// inferred: limits of the debug rectangle and the indirect matrix of the final pass (writable globals in
// .sdata; names invented)
float gGodRayRectMinX_unk = 0.0f;
float gGodRayRectMinY_unk = 0.0f;
float gGodRayRectMaxX_unk = 0.0f;
float gGodRayRectMaxY_unk = 0.0f;
float gGodRayIndScale_unk = 0.5f;
int gGodRayIndExp_unk = 1;

// inferred: max that returns a reference (std::max)
inline const float& MaxRef_unk(const float& a, const float& b) {
    return a < b ? b : a;
}

// inferred: scales an indirect matrix until its values are in [-1, 1) (name invented)
inline void NormalizeIndMtx_unk(float m[2][3], int& exp) {
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
        if (mx >= 1.0f || mx < -1.0f) {
            again = 1;
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 3; j++) {
                    m[i][j] *= 0.5f;
                }
            }
            if (!first) {
                exp++;
            }
            first = 0;
        }
    } while (again);
}

// inferred: the texture is ready (name invented)
inline bool IsLoaded_unk(Texture_unk& tex) {
    return tex.mImage != 0 ||
           (((GimmickUvOwner_unk*)&tex)->fn_800D59B8() == 0 && ((LoadHandle*)&tex.m20)->fn_80473EFC() == 3);
}

// inferred: one radial blur pass of src into dst around (cx, cy)
// NONMATCHING: 98.75% - FPR/GPR allocation and the stack slot of the K color temporary
extern "C" void fn_800AE568(GXTexObj* src, void* dst, int w, int h, int fmt, float cx, float cy, float dx, float dy,
                            float scale) {
    static Texture_unk sRadial;
    if (!IsLoaded_unk(sRadial)) {
        fn_800D594C(&sRadial, "common/radial.texture");
    }
    GXInvalidateTexAll();
    float iw = 1.0f / w;
    float ih = 1.0f / h;
    GXSetNumTevStages(4);
    GXSetNumChans(1);
    GXSetNumIndStages(3);
    GXSetIndTexOrder(0, 1, 1);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndWarp(1, 0, 1, 0, 1);
    GXSetIndTexOrder(1, 1, 1);
    GXSetIndTexCoordScale(1, 0, 0);
    GXSetTevIndWarp(2, 1, 1, 0, 2);
    GXSetIndTexOrder(2, 1, 1);
    GXSetIndTexCoordScale(2, 0, 0);
    GXSetTevIndWarp(3, 2, 1, 0, 3);
    float ox = iw * dx;
    float oy = ih * dy;
    for (int i = 0; i < 3; i++) {
        float m[2][3];
        m[0][0] = (i + 1) * ox;
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[1][0] = 0.0f;
        m[1][1] = (i + 1) * oy;
        m[1][2] = 0.0f;
        int exp = 0;
        NormalizeIndMtx_unk(m, exp);
        GXSetIndTexMtx(i + 1, m, exp);
    }
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetNumTexGens(2);
    GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x21, 0, 0x7D);
    Matrix3x4 id;
    id.SetIdentity();
    Matrix3x4 sm;
    Vector3 s(scale, scale, 1.0f);
    PSMTXScale(sm.m, s.x, s.y, s.z);
    sm.m[0][3] = 0.5f - cx * scale;
    sm.m[1][3] = 0.5f - cy * scale;
    sm.m[2][3] = 0.0f;
    GXLoadTexMtxImm(id.m, 0x1E, 1);
    GXLoadTexMtxImm(sm.m, 0x21, 1);
    fn_800763FC(lbl_8087EEE0, src, 0);
    fn_800763FC(lbl_8087EEE0, &sRadial.mObj, 1);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    static const GXColor kColor = {0x3F, 0x3F, 0x3F, 0x3F};
    GXSetTevKColor(0, kColor);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevSwapMode(0, 0, 0);
    GXSetTevColorIn(0, 0xF, 8, 0xE, 0xF);
    GXSetTevColorOp(0, 0, 0, 0, 0, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetTevKColorSel(1, 0xC);
    GXSetTevSwapMode(1, 0, 0);
    GXSetTevColorIn(1, 0xF, 8, 0xE, 0);
    GXSetTevColorOp(1, 0, 0, 0, 0, 0);
    GXSetTevOrder(1, 0, 0, 0xFF);
    GXSetTevKColorSel(2, 0xC);
    GXSetTevSwapMode(2, 0, 0);
    GXSetTevColorIn(2, 0xF, 8, 0xE, 0);
    GXSetTevColorOp(2, 0, 0, 0, 0, 0);
    GXSetTevOrder(2, 0, 0, 0xFF);
    GXSetTevKColorSel(3, 0xC);
    GXSetTevSwapMode(3, 0, 0);
    GXSetTevColorIn(3, 0xF, 8, 0xE, 0);
    GXSetTevColorOp(3, 0, 0, 0, 0, 0);
    GXSetTevOrder(3, 0, 0, 0xFF);
    GXSetBlendMode(0, 4, 1, 3);
    fn_80075F58(lbl_8087EEE0, 0, 0, w, h);
    fn_800BFB70(lbl_8087EFB4);
    GXSetTexCopySrc(0, 0, w, h);
    GXSetTexCopyDst(w, h, fmt, 0);
    GXCopyTex(dst, 0);
    GXPixModeSync();
    GXInvalidateTexAll();
    GXSetTevDirect(1);
    GXSetTevDirect(2);
    GXSetTevDirect(3);
}

// inferred: clears the ramp texture (the last texel is white); the float is not used
// NONMATCHING: 92.5% - register of the width and the width/height conversions before fn_800C0D4C
extern "C" void fn_800AECD4(Texture_unk* tex, float value) {
    unsigned char* data = (unsigned char*)tex->mImage;
    int h = GXGetTexObjHeight(&tex->mObj);
    int w = GXGetTexObjWidth(&tex->mObj);
    int off = 0;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            data[off + x] = 0;
        }
        off += w;
    }
    data[w * h - 1] = 0xFF;
    fn_800C0D4C(tex->mImage, GXGetTexObjFmt(&tex->mObj), GXGetTexObjWidth(&tex->mObj), GXGetTexObjHeight(&tex->mObj));
    GXInitTexObj(&tex->mObj, tex->mImage, GXGetTexObjWidth(&tex->mObj), GXGetTexObjHeight(&tex->mObj),
                 GXGetTexObjFmt(&tex->mObj), 0, 0, 0);
    GXInitTexObjLOD(&tex->mObj, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
}


inline GXColor ToGXColor_unk(const GodRayColor_unk& c) {
    GXColor r;
    r.r = 255.0f * c.r;
    r.g = 255.0f * c.g;
    r.b = 255.0f * c.b;
    r.a = 255.0f * c.a;
    return r;
}

// inferred: name invented
inline bool IsZero2_unk(const Vector3& v) {
    return 0.0f == v.x && 0.0f == v.y && 0.0f == v.z;
}

namespace atn {

class PostFxGodRay : public IPostFx {
public:
    PostFxGodRay();
    virtual ~PostFxGodRay() {}
    virtual void vf10(void* camera);
    int fn_800AEF4C();
    void fn_800AEF68();

    GodRayParams_unk* mParams; // 0x04
    GXTexObj m08;              // 0x08
    GodRayColor_unk mColor;    // 0x28: inferred: current color (follows mParams->mColor)
    Texture_unk mRamp;         // 0x38: inferred: depth ramp
    float m68;
    Vector2_unk mScreen;       // 0x6C: inferred: screen position of the light (0-1)
    float mIntensity;          // 0x74
};

PostFxGodRay::PostFxGodRay()
    : mParams(0), mColor(0.0f, 0.0f, 0.0f, 0.0f), m68(0.0f), mScreen(0.5f, 0.5f), mIntensity(0.0f) {
    mRamp.fn_800D5B58(0x100, 0x100, 1, 1, "DoFDepthRamp");
    fn_800AECD4(&mRamp, m68);
    mParams = &lbl_8087EFA8->mGodRay;
}

int PostFxGodRay::fn_800AEF4C() {
    return mParams != 0 ? mParams->mEnable : 0;
}

// NONMATCHING: 97.59% - stack slots of the case 0 temporaries and FPR of the screen size
void PostFxGodRay::fn_800AEF68() {
    mParams = &lbl_8087EFA8->mGodRay;
    GodRayOverride_unk* o = fn_800C1424(lbl_8087EFB4);
    if (o->m194 != 0) {
        mParams = &o->m198;
    }
    float fw = lbl_8087EEE0->mWidth;
    float fh = lbl_8087EEE0->mHeight;
    GodRayParams_unk* p = mParams;
    GodRayFrameBuffer_unk* fb;
    switch (p->mType) {
    case 0: {
        Vector3 pos(p->mPos);
        if (p->mUseDir) {
            Vector3 dir(p->mPos);
            if (IsZero2_unk(dir)) {
                dir = Vector3(0.0f, 1.0f, 0.0f);
            }
            pos = ((GodRayFrameBuffer_unk*)lbl_8087EFB4)->mEye + dir.GetNormalize() * 800.0f;
        }
        Sphere sph;
        sph.mCenter = pos;
        sph.mRadius = p->mRadius;
        Vector3 scr;
        fn_800BFAC8(&scr, lbl_8087EFB4, &pos);
        mScreen = Vector2_unk(scr.x / fw, scr.y / fh);
        fb = (GodRayFrameBuffer_unk*)lbl_8087EFB4;
        Vector3 a = (pos - fb->mEye).GetNormalize();
        fb = (GodRayFrameBuffer_unk*)lbl_8087EFB4;
        Vector3 b = (fb->mTarget - fb->mEye).GetNormalize();
        Vector3 c = b.GetNormalize();
        float d = PSVECDotProduct(&a, &c);
        if (d < 0.01) {
            mIntensity = 0.0f;
        } else {
            mIntensity = d;
            if (p->mDebug != 0) {
                float x0 = scr.x - sph.mRadius;
                x0 = x0 < gGodRayRectMinX_unk ? gGodRayRectMinX_unk : (x0 > fw ? fw : x0);
                float y0 = scr.y - sph.mRadius;
                y0 = y0 < gGodRayRectMinY_unk ? gGodRayRectMinY_unk : (y0 > fh ? fh : y0);
                float x1 = scr.x + sph.mRadius;
                x1 = x1 < gGodRayRectMaxX_unk ? gGodRayRectMaxX_unk : (x1 > fw ? fw : x1);
                float y1 = scr.y + sph.mRadius;
                y1 = y1 < gGodRayRectMaxY_unk ? gGodRayRectMaxY_unk : (y1 > fh ? fh : y1);
                fn_800610A4(lbl_8087EEB0, 0xFFFF0000, x0, y0, 0.0f, x1 - x0, y1 - y0);
            }
        }
        break;
    }
    case 1: {
        mIntensity = 1.0f;
        Vector3 dir(p->mPos);
        if (IsZero2_unk(dir)) {
            dir = Vector3(0.0f, 1.0f, 0.0f);
        }
        dir.Normalize_unk();
        Matrix3x4 m = ((GodRayFrameBuffer_unk*)lbl_8087EFB4)->mView.GetRotation_unk();
        PSMTXInverse(m.m, m.m);
        PSMTXTranspose(m.m, m.m);
        Vector3 v(dir);
        PSMTXMultVec(m.m, &v, &v);
        mScreen = Vector2_unk(v.x, v.y);
        break;
    }
    case 2: {
        mIntensity = 1.0f;
        Vector3 dir(p->mPos);
        if (IsZero2_unk(dir)) {
            dir = Vector3(0.5f, 0.5f, 0.0f);
        }
        mScreen = Vector2_unk(dir.x, dir.y);
        break;
    }
    }
}

// NONMATCHING: 99.84% - FPR allocation of the color lerp
void PostFxGodRay::vf10(void* camera) {
    int fullW;
    int fullH;
    GXSetZMode(0, 3, 0);
    GodRayParams_unk* p = mParams;
    void* image0;
    if (mIntensity < 0.05f) {
        return;
    }
    GXInvalidateTexAll();
    int w;
    int h;
    int fmt = 1;
    int copyFmt = 0x28;
    fullW = lbl_8087EEE0->mWidth;
    fullH = lbl_8087EEE0->mHeight;
    w = fullW >> p->mShift;
    h = fullH >> p->mShift;
    if (p->mUseDepth) {
        fmt = 4;
        copyFmt = 4;
    }
    image0 = lbl_8087EF8C->fn_800A96AC(w, h, fmt);
    GXInitTexObj(&m08, image0, w, h, fmt, 0, 0, 0);
    GXInitTexObjLOD(&m08, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    void* image1 = lbl_8087EF8C->fn_800A96AC(w, h, fmt);
    GXTexObj tmp;
    GXInitTexObj(&tmp, image1, w, h, fmt, 0, 0, 0);
    GXInitTexObjLOD(&tmp, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    fn_800C0508(lbl_8087EFB4, 1);
    if (p->mUseDepth) {
        GXSetNumTevStages(2);
        GXSetNumTexGens(3);
        GXSetNumChans(1);
        GXSetNumIndStages(1);
        fn_800763FC(lbl_8087EEE0, &((GodRayFrameBuffer_unk*)lbl_8087EFB4)->m74C, 2);
        GXSetTexCoordGen2(2, 1, 4, 0x3C, 0, 0x7D);
    } else {
        GXSetNumTevStages(1);
        GXSetNumTexGens(2);
        GXSetNumChans(1);
        GXSetNumIndStages(1);
    }
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    fn_800763FC(lbl_8087EEE0, &mRamp.mObj, 0);
    fn_800763FC(lbl_8087EEE0, &((GodRayFrameBuffer_unk*)lbl_8087EFB4)->m77C, 1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    GXSetIndTexOrder(0, 1, 1);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndWarp(0, 0, 0, 1, 1);
    float ind[2][3];
    ind[0][0] = gGodRayIndScale_unk;
    ind[0][1] = 0.0f;
    ind[0][2] = 0.0f;
    ind[1][0] = 0.0f;
    ind[1][1] = gGodRayIndScale_unk;
    ind[1][2] = 0.0f;
    GXSetIndTexMtx(1, ind, gGodRayIndExp_unk);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevSwapMode(0, 0, 0);
    if (p->mUseDepth) {
        GXSetTevColorIn(1, 0xF, 8, 0, 0xF);
        GXSetTevColorOp(1, 0, 0, 0, 1, 0);
        GXSetTevOrder(1, 2, 2, 0xFF);
        GXSetTevSwapMode(1, 0, 0);
    }
    GXSetBlendMode(0, 4, 1, 3);
    if (p->mShift > 0) {
        fn_80075F58(lbl_8087EEE0, 0, 0, w * 2, h * 2);
    } else {
        fn_80075F58(lbl_8087EEE0, 0, 0, w, h);
    }
    fn_800BFB70(lbl_8087EFB4);
    if (p->mShift > 0) {
        GXSetTexCopySrc(0, 0, w * 2, h * 2);
        GXSetTexCopyDst(w, h, copyFmt, 1);
    } else {
        GXSetTexCopySrc(0, 0, w, h);
        GXSetTexCopyDst(w, h, copyFmt, 0);
    }
    GXCopyTex(image0, 1);
    GXPixModeSync();
    fn_80075DEC(lbl_8087EEE0);
    fn_800A981C(&m08, image1, w, h, copyFmt, 0, 0, 1.0f, 0.0f);
    fn_800A981C(&tmp, image0, w, h, copyFmt, 0, 0, 0.0f, 1.0f);
    if (p->mType == 0 || p->mType == 2) {
        for (int i = 0; i < p->mPasses; i++) {
            fn_800AE568(&m08, image0, w, h, copyFmt, mScreen.x, mScreen.y, p->m1C, p->m20, p->m24);
        }
    } else if (p->mType == 1) {
        for (int i = 0; i < p->mPasses; i++) {
            fn_800A981C(&m08, image0, w, h, copyFmt, 0, 1, mScreen.x * p->m1C, mScreen.y * p->m20);
        }
    }
    GXSetNumTexGens(2);
    GXSetNumChans(0);
    GXSetNumTevStages(2);
    GXSetNumIndStages(0);
    float d0 = p->mColor.r - mColor.r;
    float d1 = p->mColor.g - mColor.g;
    float d2 = p->mColor.b - mColor.b;
    float d3 = p->mColor.a - mColor.a;
    float rate = 0.06666667f;
    float eps = 0.0019607844f;
    if (Abs_unk(d0) > eps) {
        mColor.r = d0 * rate + mColor.r;
    } else {
        mColor.r = p->mColor.r;
    }
    if (Abs_unk(d1) > eps) {
        mColor.g = d1 * rate + mColor.g;
    } else {
        mColor.g = p->mColor.g;
    }
    if (Abs_unk(d2) > eps) {
        mColor.b = d2 * rate + mColor.b;
    } else {
        mColor.b = p->mColor.b;
    }
    if (Abs_unk(d3) > eps) {
        mColor.a = d3 * rate + mColor.a;
    } else {
        mColor.a = p->mColor.a;
    }
    GXSetTevKColor(0, ToGXColor_unk(GodRayColor_unk(mColor.r * mIntensity, mColor.g * mIntensity,
                                                     mColor.b * mIntensity, mColor.a * mIntensity)));
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevColorIn(0, 0xF, 0xF, 0xF, 8);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevDirect(0);
    GXSetTevKColorSel(1, 0xC);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevColorIn(1, 0xF, 8, 0xE, 0);
    GXSetTevOrder(1, 1, 1, 0xFF);
    GXSetTevDirect(1);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    fn_800763FC(lbl_8087EEE0, &((GodRayFrameBuffer_unk*)lbl_8087EFB4)->m74C, 0);
    fn_800763FC(lbl_8087EEE0, &m08, 1);
    GXSetBlendMode(0, 4, 1, 3);
    fn_80075F58(lbl_8087EEE0, 0, 0, fullW, fullH);
    fn_800BFB70(lbl_8087EFB4);
    if (lbl_8087EFA8->mGodRay.mDebug != 0) {
        fn_8005DF90(lbl_8087EEB0, -1, &((GodRayFrameBuffer_unk*)lbl_8087EFB4)->m77C, 0, 16.0f, 16.0f, 0.0f,
                    (float)fullW / 3.0f, (float)fullH / 3.0f);
        fn_8005DF90(lbl_8087EEB0, -1, &m08, 0, 16.0f, 32.0f + (float)fullH / 3.0f, 0.0f, (float)fullW / 3.0f,
                    (float)fullH / 3.0f);
    }
}

} // namespace atn
