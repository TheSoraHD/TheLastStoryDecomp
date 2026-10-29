#include "atn/GXDecl.h"
#include "atn/Camera_unk.h"
#include "math/Vec.h"
#include "atn/RenderSetting_unk.h"

namespace atn {

// inferred: axis aligned box (min, max), as in atn/Geometry_unk.cpp
struct AABB_unk {
    AABB_unk() {}
    AABB_unk(const Vector3& min, const Vector3& max) : mMin(min), mMax(max) {}
    AABB_unk Merge_unk(const AABB_unk& o) const; // 0x80070C98: union of both boxes
    Vector3 mMin;
    Vector3 mMax;
};

} // namespace atn

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" double ceil(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void C_MTXLookAt(Mtx m, const atn::Vector3* eye, const atn::Vector3* up, const atn::Vector3* target);
extern "C" void fn_805F9420(const Mtx m, const atn::Vector3* src, atn::Vector3* dst,
                            unsigned long n); // inferred: PSMTXMultVecArray
extern "C" void GXSetViewport(float x, float y, float w, float h, float n, float f);
extern "C" void GXSetColorUpdate(unsigned char enable);

// inferred: registers a debug toggle `name` bound to `flag` (see atn/Layer3D.h)
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4);

// render settings (atn/RenderSetting_unk.cpp)
extern "C" RenderSetting_unk* lbl_8087EFA8;

// inferred: render system (lbl_8087EFB4): current camera at 0x104, shadow camera at 0x318
struct ShadowRenderSystem_unk {
    // inferred: three per-pass tables, each with 20 entries (pass IDs 0..0x13).
    void* mPassEnabled[0x14]; // 0x000
    void* mPassQueueHeads[0x14]; // 0x050
    int mPassQueueCounts[0x14]; // 0x0A0
    void* mQueueBuffer; // 0x0F0
    int mQueueCapacity; // 0x0F4
    int mQueueUsed; // 0x0F8
    unsigned char m0FC[0x104 - 0x0FC];
    Camera_unk mCamera; // 0x104
    int mCurrentPass; // 0x2F8
    void* m2FC; // inferred: frame-scoped light/lens override
    unsigned char m300[0x318 - 0x300];
    Camera_unk mShadowCamera; // 0x318
    unsigned char m50C[0x79C - 0x50C];
    void* m79C; // inferred: depth copy buffer
};
extern "C" ShadowRenderSystem_unk* lbl_8087EFB4;

// inferred: light settings returned by fn_800C1424 (direction at 0x254 when m234 is set)
struct ShadowLight_unk {
    unsigned char m000[0x234];
    int m234;
    unsigned char m238[0x254 - 0x238];
    float m254;
    float m258;
    float m25C;
};
extern "C" ShadowLight_unk* fn_800C1424(ShadowRenderSystem_unk* rs);
extern "C" void fn_800BDB58(ShadowRenderSystem_unk* rs, void* render, float order, int pass); // inferred: queues a render

// inferred: render manager (lbl_8087EEE0)
struct ShadowRenderMgr_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
    unsigned char m44[0x48 - 0x44];
    unsigned int mClearColor; // 0x48: ARGB
};
extern "C" ShadowRenderMgr_unk* lbl_8087EEE0;
extern "C" void fn_80071E04(ShadowRenderMgr_unk* rs);
extern "C" void fn_80071D74(ShadowRenderMgr_unk* rs, const atn::Matrix44_unk& proj);
extern "C" void fn_80076760(ShadowRenderMgr_unk* rs, int arg1, int arg2);
extern "C" void fn_80075DEC(ShadowRenderMgr_unk* rs);
extern "C" unsigned char lbl_8087EFB0; // inferred: z pre-pass active

// inferred: debug drawing
extern "C" void* lbl_8087EEB0;
extern "C" void fn_8005DF90(void* dbg, int color, GXTexObj* obj, int arg, float x, float y, float z, float w,
                            float h); // inferred: draws a texture on screen
extern "C" void fn_80063764(void* draw, const atn::Vector3* a, const atn::Vector3* b, unsigned int color,
                            float width); // inferred: line
extern "C" void fn_800638B0(void* draw, float a, float b, const atn::Vector3* from, const atn::Vector3* to,
                            unsigned int color); // inferred: arrow

// inferred: box helpers of atn/Geometry_unk.cpp
extern "C" int fn_8006FAB0(const atn::AABB_unk* box, float* t, const atn::Vector3* origin, const atn::Vector3* dir);
extern "C" float fn_8006FDD8(const atn::AABB_unk* box);
extern "C" void fn_8006FE08(const atn::AABB_unk* box, unsigned int color);

// inferred: texture matrix of a projection (atn/TextureDraw_unk.cpp)
extern "C" void fn_800D5E18(float m[3][4], float t, float b, float l, float r, float scaleS, float scaleT,
                            float transS, float transT);

namespace atn {

class PostFxManager {
public:
    void* fn_800A96AC(int width, int height, int format); // inferred: temporary texture
};

} // namespace atn

extern "C" atn::PostFxManager* lbl_8087EF8C;

// inferred: object of the render list; vf14 returns nonzero (and its bounds) when it casts a shadow
struct ShadowObj_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual int vf14(const atn::Matrix3x4* view, const atn::Vector3* pos, const Frustum_unk* frustum,
                     atn::AABB_unk* bounds);
};
struct ShadowNode_unk {
    ShadowObj_unk* mObj;
    float m04;
    ShadowNode_unk* mNext;
};

// inferred: float color (as in game/ColliAttrManager.cpp); names invented
struct ShadowColorF_unk {
    ShadowColorF_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    ShadowColorF_unk(unsigned int argb) {
        r = (1.0f / 255.0f) * (float)((argb >> 16) & 0xFF);
        g = (1.0f / 255.0f) * (float)((argb >> 8) & 0xFF);
        b = (1.0f / 255.0f) * (float)(argb & 0xFF);
        a = (1.0f / 255.0f) * (float)(argb >> 24);
    }
    GXColor ToGX_unk() const {
        GXColor c;
        c.r = r * 255.0f;
        c.g = g * 255.0f;
        c.b = b * 255.0f;
        c.a = a * 255.0f;
        return c;
    }
    float r, g, b, a;
};

// inferred: product of two matrices by value; name invented
inline atn::Matrix3x4 MtxMul_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

// inferred: matrix from view space depth to [0, 1] (row 0) and to [0, scale] (row 1); name invented
inline void DepthMtx_unk(atn::Matrix3x4* m, float n, float f, float scale) {
    float range = f - n;
    memset(m, 0, sizeof(*m));
    m->m[2][3] = 1.0f;
    m->m[0][2] = -1.0f / range;
    m->m[0][3] = -n / range;
    m->m[1][2] = m->m[0][2] * scale;
    m->m[1][3] = m->m[0][3] * scale;
}

// inferred: ceilf of MSL (float wrapper of ceil)
inline float Ceil_unk(float x) {
    return ceil(x);
}

// inferred: rounds down / up to a multiple of step; names invented
inline int SnapDown_unk(int x, float step) {
    if (x < 0) {
        return x - (-x % (int)step != 0 ? (int)step - -x % (int)step : 0);
    }
    return x - x % (int)step;
}
inline int SnapUp_unk(int x, float step) {
    if (x < 0) {
        return x + -x % (int)step;
    }
    return x + (x % (int)step != 0 ? (int)step - x % (int)step : 0);
}

// inferred: box of the 8 corners of `box` transformed by `m` (as in game/MapPartsLocatorSet.cpp)
inline atn::AABB_unk TransformBox_unk(const Mtx m, const atn::AABB_unk* box) {
    float maxZ = box->mMax.z;
    float maxY = box->mMax.y;
    float maxX = box->mMax.x;
    float minX = box->mMin.x;
    float minY = box->mMin.y;
    float minZ = box->mMin.z;
    atn::Vector3 corners[8];
    corners[0] = atn::Vector3(maxX, maxY, maxZ);
    corners[1] = atn::Vector3(minX, maxY, maxZ);
    corners[2] = atn::Vector3(maxX, minY, maxZ);
    corners[3] = atn::Vector3(minX, minY, maxZ);
    corners[4] = atn::Vector3(maxX, maxY, minZ);
    corners[5] = atn::Vector3(minX, maxY, minZ);
    corners[6] = atn::Vector3(maxX, minY, minZ);
    corners[7] = atn::Vector3(minX, minY, minZ);
    fn_805F9420(m, corners, corners, 8);
    atn::Vector3 max(corners[0]);
    atn::Vector3 min(corners[0]);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[1].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[2].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[3].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[4].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[5].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[6].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[7].x);
    return atn::AABB_unk(min, max);
}

namespace atn {

// inferred: local copies of atn::IRenderLayer / atn::Layer3D (atn/Layer3D.h): this layer shows that
// vf10 takes the render list
class IRenderLayer {
public:
    IRenderLayer(const char* name, int arg0C = -1, int arg10 = -1) : m04(1), mName(name), m0C(arg0C), m10(arg10) {}
    virtual ~IRenderLayer() {}
    virtual void vf0C(void* menu) { fn_80087994(menu, mName, &m04, 0, 0); }
    virtual void vf10(void* arg) = 0;
    virtual void vf14(void* arg) = 0;
    virtual void vf18() {}
    virtual void vf1C() = 0;
    virtual void vf20() = 0;

    int m04;
    const char* mName; // 0x08
    int m0C;
    int m10;
};

class Layer3D : public IRenderLayer {
public:
    Layer3D(const char* name, int arg0C = -1, int arg10 = -1) : IRenderLayer(name, arg0C, arg10), m14(1), m18(0) {}
    virtual ~Layer3D() {}
    virtual void vf10(void* arg) {}
    virtual void vf14(void* arg);
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();

    int m14;
    unsigned int m18;
    void* m1C[16];
};

// inferred: shadow map layer (depth from the light camera)
class LayerShadow : public Layer3D {
public:
    LayerShadow();
    virtual ~LayerShadow();
    virtual void vf10(void* arg);
    virtual void vf1C();
    virtual void vf20();

    AABB_unk fn_800B6C70(float step, const AABB_unk& box);
    Camera_unk fn_800B7090(const AABB_unk& box, const Vector3& dir);
    // inferred: copies the frame buffer at half size into mImage; name invented
    void CopyHalf_unk(unsigned short w, unsigned short h) {
        GXSetTexCopySrc(0, 0, w, h);
        GXSetTexCopyDst(w / 2, h / 2, 4, 1);
        GXCopyTex(mImage, 1);
    }

    AABB_unk mBox;            // 0x5C: inferred: shadow receiver box
    Camera_unk mCamera;       // 0x74: inferred: view camera of the frame
    Camera_unk mLightCamera;  // 0x268
    void* mImage;             // 0x45C
    GXTexObj mTex;            // 0x460
    GXTexObj mDepthTex;       // 0x480
    Matrix3x4 mDepthMtx;      // 0x4A0: inferred: view space to light depth
    Matrix3x4 mTexMtx;        // 0x4D0: inferred: view space to shadow texture
};

LayerShadow::LayerShadow() : Layer3D("Shadow") {}

// inferred: box snapped outwards to a grid of `step` units
// NONMATCHING: 99.88% - the last two (int)step conversions get new stack slots instead of reusing 0x38/0x30
AABB_unk LayerShadow::fn_800B6C70(float step, const AABB_unk& box) {
    int minX = (int)Ceil_unk(box.mMin.x);
    int minY = (int)Ceil_unk(box.mMin.y);
    int minZ = (int)Ceil_unk(box.mMin.z);
    int maxX = (int)Ceil_unk(box.mMax.x);
    int maxY = (int)Ceil_unk(box.mMax.y);
    int maxZ = (int)Ceil_unk(box.mMax.z);
    minX = SnapDown_unk(minX, step);
    minY = SnapDown_unk(minY, step);
    minZ = SnapDown_unk(minZ, step);
    maxX = SnapUp_unk(maxX, step);
    maxY = SnapUp_unk(maxY, step);
    maxZ = SnapUp_unk(maxZ, step);
    AABB_unk r;
    r.mMin.x = minX;
    r.mMin.y = minY;
    r.mMin.z = minZ;
    r.mMax.x = maxX;
    r.mMax.y = maxY;
    r.mMax.z = maxZ;
    return r;
}

// inferred: orthographic light camera looking along `dir` that covers `box`
// NONMATCHING: 82.01% - the original has one more saved GPR and 0x10 more stack (corner temporaries
// start at 0x30), so most registers and stack offsets differ
Camera_unk LayerShadow::fn_800B7090(const AABB_unk& box, const Vector3& dir) {
    Vector3 center = (box.mMax + box.mMin) * 0.5f;
    float t = 0.0f;
    fn_8006FAB0(&box, &t, &center, &dir);
    Camera_unk cam(lbl_8087EFB4->mCamera);
    cam.mProjType = 1;
    Vector3 eye = center + dir * (2.0f * t);
    Vector3 target(center);
    Vector3 up(0.0f, 1.0f, 0.0f);
    if ((float)__fabs(PSVECDotProduct(&dir, &Vector3(0.0f, 1.0f, 0.0f))) > 0.99f) {
        up = Vector3(0.0f, 0.0f, 1.0f);
        if ((float)__fabs(PSVECDotProduct(&dir, &Vector3(0.0f, 0.0f, 1.0f))) > 0.99f) {
            up = Vector3(1.0f, 0.0f, 0.0f);
        }
    }
    cam.mUp = up;
    cam.mPos = eye;
    cam.mTarget = target;
    Mtx view;
    C_MTXLookAt(view, &eye, &up, &target);
    AABB_unk b = TransformBox_unk(view, &box);
    cam.mTop = b.mMax.y;
    cam.mBottom = b.mMin.y;
    cam.mLeft = b.mMin.x;
    cam.mRight = b.mMax.x;
    Vector3 d = (cam.mTarget - cam.mPos).GetNormalize();
    cam.mPos = eye + d * -b.mMax.z;
    cam.mNear = 0.0f;
    cam.mFar = 0.0f + (b.mMax.z - b.mMin.z);
    cam.fn_8004B378();
    return cam;
}

// NONMATCHING: 22.90% - over the inline budget: the camera copies, the frustum copy and the vector
// helpers are called out of line (weak copies), while the original inlines all of them
void LayerShadow::vf10(void* arg) {
    if (arg == 0) {
        fn_800BDB58(lbl_8087EFB4, 0, 0.0f, 3);
    }
    mCamera = lbl_8087EFB4->mCamera;
    RenderSetting_unk* s = lbl_8087EFA8;
    Vector3 dir(-s->m104.mDir.x, -s->m104.mDir.y, -s->m104.mDir.z);
    ShadowLight_unk* light = fn_800C1424(lbl_8087EFB4);
    if (light->m234 != 0) {
        dir = Vector3(-light->m254, -light->m258, -light->m25C);
    }
    if (dir.IsZero_unk()) {
        dir = Vector3(1.0f, -1.0f, 1.0f).GetNormalize();
    }
    dir.Normalize_unk();
    AABB_unk total(Vector3(1e16f, 1e16f, 1e16f), Vector3(-1e16f, -1e16f, -1e16f));
    Frustum_unk frustum(mCamera.mFrustum);
    Matrix3x4 view(mCamera.mView);
    Vector3 pos(mCamera.mPos);
    for (ShadowNode_unk* node = (ShadowNode_unk*)arg; node != 0; node = node->mNext) {
        AABB_unk b(Vector3(1e16f, 1e16f, 1e16f), Vector3(-1e16f, -1e16f, -1e16f));
        if (node->mObj != 0 && node->mObj->vf14(&view, &pos, &frustum, &b)) {
            if (lbl_8087EFA8->m12C != 0) {
                fn_8006FE08(&b, 0xFF0000FF);
            }
            total = total.Merge_unk(b);
        } else {
            node->mObj = 0;
        }
    }
    AABB_unk fbox(frustum.mMin, frustum.mMax);
    s = lbl_8087EFA8;
    if (s->m140 != 0) {
        if (s->m138 != 0) {
            if (fn_8006FDD8(&total) > 1.0f) {
                mBox = total;
            } else {
                mBox = fbox;
            }
        } else {
            mBox.mMin = Vector3(s->m174, s->m16C, s->m164);
            mBox.mMax = Vector3(lbl_8087EFA8->m170, lbl_8087EFA8->m168, lbl_8087EFA8->m160);
        }
    }
    if (lbl_8087EFA8->m17C != 0) {
        mBox = fn_800B6C70((int)lbl_8087EFA8->m180, mBox);
    }
    float margin = lbl_8087EFA8->m190;
    Vector3 m(margin, margin, margin);
    mBox.mMin -= m;
    mBox.mMax += m;
    mLightCamera = fn_800B7090(mBox, dir);
    lbl_8087EFB4->mShadowCamera = mLightCamera;
    if (lbl_8087EFA8->m12C != 0) {
        fn_8006FE08(&mBox, 0xFFFFFF00);
        fn_80063764(lbl_8087EEB0, &mLightCamera.mPos, &mLightCamera.mTarget, 0xFFFF0000, 0.0f);
        Vector3 to = mLightCamera.mPos + (mLightCamera.mTarget - mLightCamera.mPos) * 0.25f;
        fn_800638B0(lbl_8087EEB0, 0.0f, 2.0f, &mLightCamera.mPos, &to, 0xFFFF0000);
        float scale = 1.0f;
        float n = -mLightCamera.mNear;
        float f = -mLightCamera.mFar;
        float top = mLightCamera.mTop;
        float bottom = mLightCamera.mBottom;
        float left = mLightCamera.mLeft;
        float right = mLightCamera.mRight;
        Vector3 corners[8] = {
            Vector3(left, top, n),
            Vector3(right, top, n),
            Vector3(right, bottom, n),
            Vector3(left, bottom, n),
            Vector3(scale * left, scale * top, f),
            Vector3(scale * right, scale * top, f),
            Vector3(scale * right, scale * bottom, f),
            Vector3(scale * left, scale * bottom, f),
        };
        for (int i = 0; i < 8; i++) {
            PSMTXMultVec(mLightCamera.mInvView.m, &corners[i], &corners[i]);
        }
        fn_80063764(lbl_8087EEB0, &corners[0], &corners[1], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[1], &corners[2], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[2], &corners[3], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[3], &corners[0], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[4], &corners[5], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[5], &corners[6], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[6], &corners[7], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[7], &corners[4], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[0], &corners[4], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[1], &corners[5], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[2], &corners[6], 0xFF00FF00, 0.0f);
        fn_80063764(lbl_8087EEB0, &corners[3], &corners[7], 0xFF00FF00, 0.0f);
    }
}

// NONMATCHING: 86.69% - registers of the frustum copy inside the camera copy (base and loop pointers
// swapped) and scheduling of the clear color conversion
void LayerShadow::vf1C() {
    RenderSetting_unk* s = lbl_8087EFA8;
    int w = s->m158;
    int h = s->m15C;
    lbl_8087EFB0 = 1;
    mImage = lbl_8087EF8C->fn_800A96AC(w, h, 4);
    GXInitTexObj(&mTex, mImage, w, h, 4, 0, 0, 0);
    GXInitTexObjLOD(&mTex, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXInitTexObj(&mDepthTex, lbl_8087EFB4->m79C, w, h, 3, 0, 0, 0);
    GXInitTexObjLOD(&mDepthTex, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    lbl_8087EFB4->mCamera = mLightCamera;
    ShadowRenderMgr_unk* rs = lbl_8087EEE0;
    fn_80071E04(rs);
    fn_80076760(rs, 6, 2);
    fn_80071D74(rs, mLightCamera.mProj);
    fn_80076760(rs, 8, 1);
    fn_80076760(rs, 9, 2);
    fn_80076760(rs, 10, 1);
    GXSetViewport(0.0f, 0.0f, lbl_8087EFA8->m158, lbl_8087EFA8->m15C, 0.0f, 1.0f);
    ShadowColorF_unk clear(lbl_8087EEE0->mClearColor);
    ShadowColorF_unk black(0.0f, 0.0f, 0.0f, 1.0f);
    GXColor black8 = black.ToGX_unk();
    GXColor clear8 = clear.ToGX_unk();
    GXSetCopyClear(black8, 0xFFFFFF);
    CopyHalf_unk(w, h);
    GXSetCopyClear(clear8, 0xFFFFFF);
}

// NONMATCHING: 93.95% - registers of the frustum copy inside the camera copy, operand order of the
// debug sizes and load order of the arguments of fn_800D5E18
void LayerShadow::vf20() {
    RenderSetting_unk* s = lbl_8087EFA8;
    int w = s->m158;
    int h = s->m15C;
    if (s->m134 == 0) {
        void* depth = lbl_8087EFB4->m79C;
        GXSetTexCopySrc(0, 0, w, h);
        GXSetTexCopyDst(w, h, 0x13, 0);
        GXCopyTex(depth, 0);
        GXInitTexObj(&mDepthTex, depth, w, h, 3, 0, 0, 0);
    }
    CopyHalf_unk(w, h);
    GXInitTexObj(&mTex, mImage, w / 2, h / 2, 4, 0, 0, 0);
    GXPixModeSync();
    lbl_8087EFB0 = 0;
    fn_80075DEC(lbl_8087EEE0);
    GXSetColorUpdate(1);
    lbl_8087EFB4->mCamera = mCamera;
    if (lbl_8087EFA8->m12C != 0) {
        ShadowRenderMgr_unk* rs = lbl_8087EEE0;
        int height = rs->mHeight;
        int width = rs->mWidth;
        fn_8005DF90(lbl_8087EEB0, -1, &mDepthTex, 0, (float)(width - width / 4) - 16.0f,
                    (float)(height - height / 4) - 32.0f, 0.0f, (float)width * 0.25f, (float)height * 0.25f);
        fn_8005DF90(lbl_8087EEB0, -1, &mTex, 0, 16.0f, (float)(height - height / 4) - 32.0f, 0.0f,
                    (float)width * 0.25f, (float)height * 0.25f);
    }
    Matrix3x4 tex;
    tex.SetIdentity();
    fn_800D5E18(tex.m, mLightCamera.mTop, mLightCamera.mBottom, mLightCamera.mLeft, mLightCamera.mRight, 0.5f,
                -0.5f, 0.5f, 0.5f);
    Matrix3x4 depth;
    DepthMtx_unk(&depth, mLightCamera.mNear, mLightCamera.mFar, 256.0f);
    mDepthMtx = MtxMul_unk(depth, MtxMul_unk(mLightCamera.mView, mCamera.mInvView));
    mTexMtx = MtxMul_unk(tex, MtxMul_unk(mLightCamera.mView, mCamera.mInvView));
}

LayerShadow::~LayerShadow() {}

} // namespace atn
