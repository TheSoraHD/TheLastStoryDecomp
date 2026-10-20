#include "atn/D3AnimeInstance.h"
#include "atn/Memory.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" double sqrt(double x);
extern "C" double fabs(double x);
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
struct Renderer2D_unk {
    unsigned char m00[0xA0];
    int mA0;               // 0xA0: inferred: mode
    atn::Matrix3x4 mA4;    // 0xA4: inferred: model matrix
    atn::Matrix3x4 mD4;    // 0xD4: inferred: view matrix
};
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
extern "C" Renderer2D_unk* lbl_8087EEB0;
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" Camera_unk* lbl_8087EFB4;

namespace atn {

// inferred: bounding sphere (names invented)
struct Sphere_unk {
    // inferred: sphere around the box [pos, pos + 2 * ext]
    Sphere_unk(const Vector3& pos, Vector3 ext) : mCenter(pos + ext) {
        mRadius = sqrt(ext.x * ext.x + ext.y * ext.y + ext.z * ext.z);
    }
    Sphere_unk(const Vector3& center, float radius) : mCenter(center), mRadius(radius) {}
    Vector3 mCenter;
    float mRadius;
};
extern "C" int fn_8004FF58(Frustum_unk* frustum, const Sphere_unk* sphere); // inferred: sphere visibility test
struct Frustum_unk {
    bool IsCulled_unk(const Sphere_unk& sphere) { return fn_8004FF58(this, &sphere) == 0; }
};

D3AnimeInstance* D3AnimeInstance::Create(Task* parent, const char* name, int load) {
    if (parent != 0) {
        return new (8, "", "", 0) D3AnimeInstance(parent, name, load);
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

// NONMATCHING: the original keeps the dead result of the inlined load check (li r5)
void D3AnimeInstance::vf10() {
    UpdateLoad_unk();
}

// NONMATCHING: 92% - the original folds the sphere radius to sqrt(180000.0) and computes the
// center before it; ours computes the radius from the extent at run time
// NONMATCHING: the original folds the sphere radius to sqrt(180000.0), computes the center before
// it and orders the stack differently
void D3AnimeInstance::vf14() {
    if (UpdateLoad_unk() != 0) {
        return;
    }
    UpdateMtx();
    if (mTask != 0) {
        fn_801F69A8(mTask);
    }
    if (mVisible) {
        Frustum_unk* frustum = lbl_8087EFB4->GetFrustum_unk();
        Sphere_unk sphere(mPos, Vector3(300.0f, 300.0f, 0.0f));
        if (!frustum->IsCulled_unk(sphere)) {
            Draw();
        }
    }
}

// NONMATCHING: same calls; float register use, stack and double/float conversions differ
void D3AnimeInstance::SetMatrix(const Matrix3x4& m) {
    double sz = PSVECMag(&Vector3(m.m[0][2], m.m[1][2], m.m[2][2]));
    float sy = PSVECMag(&Vector3(m.m[0][1], m.m[1][1], m.m[2][1]));
    Vector3 scale(PSVECMag(&Vector3(m.m[0][0], m.m[1][0], m.m[2][0])), sy, sz);
    mScale = scale;
    Vector3 pos(m.m[0][3], m.m[1][3], m.m[2][3]);
    mPos = pos;
    mRot = m;
    mRot.m[0][3] = 0.0f;
    mRot.m[1][3] = 0.0f;
    mRot.m[2][3] = 0.0f;
    Vector3 inv((float)fabs(mScale.x) < 1.1920929e-7f ? 0.0f : 1.0f / mScale.x,
                (float)fabs(mScale.y) < 1.1920929e-7f ? 0.0f : 1.0f / mScale.y,
                (float)fabs(mScale.z) < 1.1920929e-7f ? 0.0f : 1.0f / mScale.z);
    Mtx m2;
    Mtx s;
    PSMTXScale(s, inv.x, inv.y, inv.z);
    PSMTXConcat(mRot.m, s, m2);
    __MtxCopy(mRot.m, m2);
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
inline void SetView_unk(Renderer2D_unk* r, Matrix3x4 m) {
    r->mD4 = m;
}

inline Matrix3x4 D3AnimeInstance::GetDrawMtx_unk(float w, float h, float z) {
    Matrix3x4 m;
    if (mFlag9) {
        m = MakeScale_unk(Vector3(1.0f, -1.0f, 1.0f));
        m = m * MakeTrans_unk(Vector3(0.5f * -w, 0.5f * -h, z));
    } else {
        m = MakeScale_unk(Vector3(1.0f, -1.0f, 1.0f));
    }
    PSMTXConcat(mMtx.m, m.m, m.m);
    return m;
}

// NONMATCHING: same calls and stores as D3Anime::Draw; stack layout and register use differ
void D3AnimeInstance::Draw() {
    Matrix3x4 savedView(lbl_8087EEB0->mD4);
    SetView_unk(lbl_8087EEB0, lbl_8087EFB4->m15C);
    Matrix3x4 savedModel(lbl_8087EEB0->mA4);
    int mode = lbl_8087EEB0->mA0;
    float w = lbl_8087EEE0->mWidth;
    float h = lbl_8087EEE0->mHeight;
    Matrix3x4 m = GetDrawMtx_unk(w, h, -0.001f);
    m = GetDrawMtx_unk(w, h, 0.0f);
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
