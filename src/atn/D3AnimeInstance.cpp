#include "atn/D3AnimeInstance.h"
#include "atn/Memory.h"
#include "atn/DrawHelper.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" double sqrt(double x);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);

// inferred: d2anime task functions (automatic names)
extern "C" atn::D2AInstance_unk* fn_801F64D0(atn::Task* parent, const char* path);
extern "C" void fn_801F69A8(atn::D2AInstance_unk* task);
extern "C" void fn_801F6A38(atn::D2AInstance_unk* task, int arg);

namespace atn {
struct Frustum_unk;
}

// inferred: globals of unknown classes
struct RenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
struct Camera_unk {
    unsigned char m000[0x15C];
    atn::Matrix3x4 m15C; // inferred: view matrix
    unsigned char m18C[0x204 - 0x18C];
    atn::Frustum_unk* GetFrustum_unk() { return (atn::Frustum_unk*)m204; }
    unsigned char m204[1]; // inferred: frustum
};
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" Camera_unk* lbl_8087EFB4;

namespace atn {

// inferred: bounding sphere (names invented)
struct Sphere_unk {
    Sphere_unk() {}
    Vector3 mCenter;
    float mRadius;
};
extern "C" int fn_8004FF58(Frustum_unk* frustum, const Sphere_unk* sphere); // inferred: sphere visibility test
struct Frustum_unk {
    bool IsCulled_unk(const Sphere_unk& sphere) { return fn_8004FF58(this, &sphere) == 0; }
};

D3AnimeInstance* D3AnimeInstance::Create(Task* parent, const char* name, int load) {
    if (parent != 0) {
        return new ((MemoryTag)8, "", "", 0) D3AnimeInstance(parent, name, load);
    }
    return 0;
}

D3AnimeInstance::D3AnimeInstance(Task* parent, const char* name, int load)
    : Task(parent), mTask(0), mPos(0.0f, -10000.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f) {
    mState = 0;
    mVisible = 1;
    mFlag9 = 0;
    mFlag10 = 1;
    memset(mName, 0, sizeof(mName));
    mRot.SetIdentity();
    mMtx.SetIdentity();
    strcpy(mName, name);
    if (load) {
        Load();
    }
}

D3AnimeInstance::~D3AnimeInstance() {}

int D3AnimeInstance::Load() {
    mState = 1;
    mTask = fn_801F64D0(this, mName);
    mTask->m4E = 0;
    mTask->fn_800D246C(1);
    return 1;
}

D2AInstance_unk* D3AnimeInstance::GetTask() const {
    if (mState == 1) {
        return 0;
    }
    return mTask;
}

int D3AnimeInstance::vf0C() {
    return UpdateLoad_unk() == 0;
}

void D3AnimeInstance::vf10() {
    if (UpdateLoad_unk()) {
        return;
    }
}

// inferred: same as UpdateMtx with the two work matrices declared in the other order (inline)
inline void CalcMtx_unk(Matrix3x4& mtx, const Matrix3x4& rot, const Vector3& pos, const Vector3& scl) {
    mtx = rot;
    mtx.m[0][3] = pos.x;
    mtx.m[1][3] = pos.y;
    mtx.m[2][3] = pos.z;
    Mtx m;
    Mtx scale;
    PSMTXScale(scale, scl.x, scl.y, scl.z);
    PSMTXConcat(mtx.m, scale, m);
    __MtxCopy(mtx.m, m);
}

// inferred: mtx = mtx * scale(v) (inline, name invented)
inline void ScaleMtx_unk(Matrix3x4& mtx, const Vector3& v) {
    Mtx m;
    Mtx scale;
    PSMTXScale(scale, v.x, v.y, v.z);
    PSMTXConcat(mtx.m, scale, m);
    __MtxCopy(mtx.m, m);
}

// inferred: column lengths (scale) of a matrix (inline, names invented)
inline float GetScaleX_unk(const Matrix3x4& m) { return PSVECMag(&Vector3(m.m[0][0], m.m[1][0], m.m[2][0])); }
inline float GetScaleY_unk(const Matrix3x4& m) { return PSVECMag(&Vector3(m.m[0][1], m.m[1][1], m.m[2][1])); }
inline float GetScaleZ_unk(const Matrix3x4& m) { return PSVECMag(&Vector3(m.m[0][2], m.m[1][2], m.m[2][2])); }

// inferred: 1 / x, or 0 when x is too small (inline, name invented)
inline float SafeInv_unk(float x) {
    if (Abs_unk(x) < 1.1920929e-7f) {
        return 0.0f;
    }
    return 1.0f / x;
}

// inferred: visibility test of the billboard (inline, name invented)
inline bool IsVisible_unk(const Vector3& pos) {
    Sphere_unk sphere;
    sphere.mCenter = pos + Vector3(300.0f, 300.0f, 0.0f);
    Frustum_unk* frustum = lbl_8087EFB4->GetFrustum_unk();
    sphere.mRadius = sqrt(300.0f * 300.0f + 300.0f * 300.0f + 0.0f * 0.0f);
    return !frustum->IsCulled_unk(sphere);
}

// NONMATCHING: 99.72% - only r4/r5 swapped for the busy flag and the reloaded task pointer in the
// inlined load check (same as D3Anime::vf14)
void D3AnimeInstance::vf14() {
    if (UpdateLoad_unk() != 0) {
        return;
    }
    CalcMtx_unk(mMtx, mRot, mPos, mScale);
    if (mTask != 0) {
        fn_801F69A8(mTask);
    }
    if (mVisible) {
        if (IsVisible_unk(mPos)) {
            Draw();
        }
    }
}

// NONMATCHING: 93.82% - same code and stack; float register numbers differ (position, the scale
// reloads and the fabs/compare values of the inverse scale)
void D3AnimeInstance::SetMatrix(const Matrix3x4& m) {
    Vector3 inv;
    float sz = GetScaleZ_unk(m);
    float sy = GetScaleY_unk(m);
    float sx = GetScaleX_unk(m);
    mScale = Vector3(sx, sy, sz);
    mPos = Vector3(m.m[0][3], m.m[1][3], m.m[2][3]);
    mRot = m;
    mRot.m[0][3] = 0.0f;
    mRot.m[1][3] = 0.0f;
    mRot.m[2][3] = 0.0f;
    inv.x = SafeInv_unk(mScale.x);
    inv.y = SafeInv_unk(mScale.y);
    inv.z = SafeInv_unk(mScale.z);
    ScaleMtx_unk(mRot, inv);
}

void D3AnimeInstance::UpdateMtx() {
    Matrix3x4& mtx = mMtx;
    mtx = mRot;
    mMtx.m[0][3] = mPos.x;
    mMtx.m[1][3] = mPos.y;
    mMtx.m[2][3] = mPos.z;
    Mtx scale;
    PSMTXScale(scale, mScale.x, mScale.y, mScale.z);
    Mtx m;
    PSMTXConcat(mtx.m, scale, m);
    __MtxCopy(mtx.m, m);
}

// inferred: matrix helpers of the engine (names invented)
inline Matrix3x4 MakeScale_unk(const Vector3& v) {
    Matrix3x4 r;
    PSMTXScale(r.m, v.x, v.y, v.z);
    return r;
}
inline Matrix3x4 MakeTrans_unk(const Vector3& v) {
    Matrix3x4 r;
    PSMTXTrans(r.m, v.x, v.y, v.z);
    return r;
}
inline Matrix3x4 operator*(const Matrix3x4& a, const Matrix3x4& b) {
    Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}
inline Vector3 operator*(const Matrix3x4& m, const Vector3& v) {
    Vector3 r(v);
    PSMTXMultVec(m.m, &r, &r);
    return r;
}
// inferred: scale matrix from components (the vector temporary is built inside)
inline Matrix3x4 MakeScale_unk(float x, float y, float z) {
    return MakeScale_unk(Vector3(x, y, z));
}
// inferred: m = m * trans(x, y, z); the translation is declared before the product (stack order)
inline void Translate_unk(Matrix3x4& m, float x, float y, float z) {
    Matrix3x4 t = MakeTrans_unk(Vector3(x, y, z));
    m = m * t;
}

// NONMATCHING: 99.03% - only volatile register numbers differ (camera/render state pointers and the
// int->float constants at the top, the renderer pointer and the matrix addresses at the end)
void D3AnimeInstance::Draw() {
    const Matrix3x4& camView = lbl_8087EFB4->m15C;
    int iw = lbl_8087EEE0->mWidth;
    int ih = lbl_8087EEE0->mHeight;
    Matrix3x4 savedView(lbl_8087EEB0->mD4);
    Matrix3x4 view(camView);
    lbl_8087EEB0->mD4 = view;
    Matrix3x4 savedModel(lbl_8087EEB0->mA4);
    int mode = lbl_8087EEB0->mA0;
    float w = iw;
    float h = ih;
    // inferred: the draw matrix is built twice (z = -0.001f, then 0.0f) and only the second is used
    Matrix3x4 m;
    if (mFlag9) {
        m = MakeScale_unk(1.0f, -1.0f, 1.0f);
        Translate_unk(m, 0.5f * -w, 0.5f * -h, -0.001f);
    } else {
        m = MakeScale_unk(1.0f, -1.0f, 1.0f);
    }
    PSMTXConcat(mMtx.m, m.m, m.m);
    if (mFlag9) {
        m = MakeScale_unk(1.0f, -1.0f, 1.0f);
        Translate_unk(m, 0.5f * -w, 0.5f * -h, 0.0f);
    } else {
        m = MakeScale_unk(1.0f, -1.0f, 1.0f);
    }
    PSMTXConcat(mMtx.m, m.m, m.m);
    Vector3 quad[4];
    quad[0] = m * Vector3(0.0f, 0.0f, 0.0f);
    quad[1] = m * Vector3(1.0f, 0.0f, 0.0f);
    quad[2] = m * Vector3(0.0f, 1.0f, 0.0f);
    quad[3] = m * Vector3(1.0f, 1.0f, 0.0f);
    lbl_8087EEB0->mA0 = 0xD;
    lbl_8087EEB0->mA4 = m;
    if (mTask != 0) {
        fn_801F6A38(mTask, 1);
    }
    lbl_8087EEB0->mA0 = mode;
    lbl_8087EEB0->mA4 = savedModel;
    lbl_8087EEB0->mD4 = savedView;
}

} // namespace atn
