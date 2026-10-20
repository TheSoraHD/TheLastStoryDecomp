#include "atn/D3Anime.h"
#include "atn/Memory.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" double sqrt(double x);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);

// inferred: d2anime task functions (automatic names)
extern "C" atn::D2ATask_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg);
extern "C" void fn_801F4484(atn::D2ATask_unk* task);
extern "C" void fn_801F465C(atn::D2ATask_unk* task, int arg);

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
    Sphere_unk() {}
    Vector3 mCenter;
    float mRadius;
};
extern "C" int fn_8004FF58(Frustum_unk* frustum, const Sphere_unk* sphere); // inferred: sphere visibility test
struct Frustum_unk {
    bool IsCulled_unk(const Sphere_unk& sphere) { return fn_8004FF58(this, &sphere) == 0; }
};

D3Anime* D3Anime::Create(Task* parent, const char* name, int load) {
    if (parent != 0) {
        return new (8, "", "", 0) D3Anime(parent, name, load);
    }
    return 0;
}

D3Anime::D3Anime(Task* parent, const char* name, int load)
    : Task(parent), mTask(0), mPos(0.0f, -10000.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f) {
    mState = 0;
    mVisible = 1;
    mFlag9 = 0;
    mFlag10 = 1;
    mEnabled = 1;
    memset(mName, 0, sizeof(mName));
    mRot.SetIdentity();
    mMtx.SetIdentity();
    strcpy(mName, name);
    if (load) {
        Load(0);
    }
}

D3Anime::~D3Anime() {}

int D3Anime::Load(int arg) {
    mState = 1;
    mTask = fn_801F3FF8(this, mName, arg);
    mTask->mFC &= ~0x40000000;
    mTask->fn_800D246C(1);
    return 1;
}

D2ATask_unk* D3Anime::GetTask() const {
    if (mState == 1) {
        return 0;
    }
    return mTask;
}

int D3Anime::vf0C() {
    return UpdateLoad_unk() == 0;
}

void D3Anime::vf10() {
    if (UpdateLoad_unk()) {
        return;
    }
}

// NONMATCHING: 99.53% - only r4/r5 swapped for the busy flag and the reloaded task pointer in the
// inlined load check (vf10 with the same inline matches); tried other forms of the inline/test
void D3Anime::vf14() {
    if (!mEnabled) {
        return;
    }
    if (UpdateLoad_unk() != 0) {
        return;
    }
    Matrix3x4& mtx = mMtx;
    mtx = mRot;
    mtx.m[0][3] = mPos.x;
    mtx.m[1][3] = mPos.y;
    mtx.m[2][3] = mPos.z;
    Mtx m;
    Mtx scale;
    PSMTXScale(scale, mScale.x, mScale.y, mScale.z);
    PSMTXConcat(mtx.m, scale, m);
    __MtxCopy(mtx.m, m);
    if (mTask != 0) {
        fn_801F4484(mTask);
    }
    if (mVisible) {
        // inferred: the radius is a folded constant (sqrt(180000.0)), so it was written with literals
        Sphere_unk sphere;
        sphere.mCenter = mPos + Vector3(300.0f, 300.0f, 0.0f);
        Frustum_unk* frustum = lbl_8087EFB4->GetFrustum_unk();
        sphere.mRadius = sqrt(300.0f * 300.0f + 300.0f * 300.0f + 0.0f * 0.0f);
        if (!frustum->IsCulled_unk(sphere)) {
            Draw();
        }
    }
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

// NONMATCHING: 99.29% - only volatile register numbers differ (camera/render state pointers and the
// int->float constants at the top, the renderer pointer and the matrix addresses at the end)
void D3Anime::Draw() {
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
        fn_801F465C(mTask, 1);
    }
    lbl_8087EEB0->mA0 = mode;
    lbl_8087EEB0->mA4 = savedModel;
    lbl_8087EEB0->mD4 = savedView;
}

} // namespace atn
