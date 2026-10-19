#include "atn/Model_unk.h"
#include "atn/Memory.h"
#include "atn/Sphere.h"

extern "C" double fabs(double x);

namespace atn {
class ModelInstance;
}

// inferred: camera lens (only what is used here; see game/CharaTask.cpp for other fields)
struct InstLens_unk {
    unsigned char m000[0x120];
    int m120;          // inferred: shadow enabled
    atn::Matrix3x4 m124; // inferred: shadow projection
    atn::Vector3 m154; // inferred: shadow plane normal
    float m160;        // inferred: shadow plane distance
};

// inferred: global camera (lbl_8087EFB4); names invented
struct InstCamera_unk {
    unsigned char m000[0x15C];
    unsigned char m15C[0x204 - 0x15C]; // inferred: view parameters for the LOD
    unsigned char m204[0x2F8 - 0x204]; // inferred: frustum
    int m2F8;                          // inferred: current pass
    InstLens_unk* m2FC;                // inferred: default lens
    unsigned char m300[0x54960 - 0x300];
    int m54960; // inferred: set when a model needs the screen copy
};
extern "C" InstCamera_unk* lbl_8087EFB4;

struct InstGame_unk {
    unsigned char m000[0x1D0];
    int m1D0; // inferred: occlusion test enabled
    unsigned char m1D4[0x208 - 0x1D4];
    float m208[8]; // inferred: LOD distances of the options
};
extern "C" InstGame_unk* lbl_8087EFA8;

// inferred: LOD entry of a model (0x18 bytes)
struct InstLod_unk {
    unsigned char m00[0x08];
    float mDistance; // 0x08
    unsigned char m0C[0x10 - 0x0C];
    void* mData; // 0x10
    unsigned char m14[0x18 - 0x14];
};

// inferred: callbacks called by the model while it is drawn (0x1F0 in the model)
struct InstCallback_unk {
    InstCallback_unk() : mFunc1(0), mFunc2(0), mInstance(0), mModel(0) {}
    InstCallback_unk(void (*f1)(atn::ModelInstance*, Model_unk*), void (*f2)(atn::ModelInstance*, Model_unk*),
                     atn::ModelInstance* instance, Model_unk* model)
        : mFunc1(f1), mFunc2(f2), mInstance(instance), mModel(model) {}
    void (*mFunc1)(atn::ModelInstance*, Model_unk*);
    void (*mFunc2)(atn::ModelInstance*, Model_unk*);
    // inferred: name invented
    void Clear_unk() { *this = InstCallback_unk(); }
    atn::ModelInstance* mInstance;
    Model_unk* mModel;
};

// inferred: view of the model fields used here (offsets in atn::Model)
struct InstModel_unk {
    virtual ~InstModel_unk();
    virtual void vf0C();
    virtual void vf10(struct InstCamera_unk* camera); // inferred: draw

    unsigned int mFlags; // 0x04
    unsigned char m08[0x38 - 0x08];
    unsigned int mNodeCount; // 0x38
    atn::Matrix3x4* mNodeMtx; // 0x3C
    unsigned char m40[0x50 - 0x40];
    int mLod; // 0x50
    unsigned char m54[0x98 - 0x54];
    float mDrawOrder; // 0x98
    atn::Sphere mSphere; // 0x9C
    float mScale;        // 0xAC
    unsigned char mB0[0x168 - 0xB0];
    InstLod_unk mLods[1]; // 0x168
    unsigned char m180[0x1DC - 0x180];
    InstLens_unk* mLens; // 0x1DC
    unsigned char m1E0[0x1F0 - 0x1E0];
    InstCallback_unk mCallback; // 0x1F0
    unsigned char m200[0x20C - 0x200];
    int mType : 8;              // 0x20C
    int mLodCount : 8;
    unsigned int m20C_15_0 : 16;
    unsigned char m210[0x218 - 0x210];
    int mInstances; // 0x218
};

// inferred: sphere without the copy constructor of atn::Sphere
struct InstSphere2_unk {
    atn::Vector3 mCenter;
    float mRadius;
};

// inferred: axis-aligned box (min, max)
struct InstBox_unk {
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};

// inferred: model data shared by the instances: a header followed by the model (names invented)
struct InstData_unk {
    unsigned char m00[0x10];
    InstModel_unk mModel;
};

extern "C" int fn_8009FFCC(InstData_unk* data); // inferred: nonzero while loading
// inferred: sphere passed by value (plain copy of the center)
struct InstSphere_unk {
    InstSphere_unk(const atn::Sphere& s) : mCenter(s.mCenter), mRadius(s.mRadius) {}
    atn::Vector3 mCenter;
    float mRadius;
};
extern "C" void* fn_800C122C(InstCamera_unk* camera, InstSphere_unk sphere); // inferred: screen data of a sphere
extern "C" int fn_8004FF58(void* frustum, const atn::Sphere* sphere);    // inferred: sphere visibility test
extern "C" int fn_800C0A50(InstCamera_unk* camera, InstBox_unk* box);   // inferred: occlusion test
extern "C" void fn_800BDB58(InstCamera_unk* camera, void* render, float order, int pass); // inferred: queues a render
extern "C" void fn_80091684(InstModel_unk* model, void* view, void* lod);
extern "C" void fn_805F9420(const Mtx m, const atn::Vector3* src, atn::Vector3* dst, unsigned int count); // PSMTXMultVecArray
extern "C" InstBox_unk fn_80070B60(const InstBox_unk* box, float scale); // inferred: scales a box
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

namespace atn {

// inferred: renderer interface (see atn/VideoTHP.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C(); // 0x80046218
    virtual void vf10(InstCamera_unk* camera) = 0;
    virtual int vf14() = 0;
    virtual void vf18(InstCamera_unk* camera) = 0;
    virtual void vf1C(); // 0x800461FC
};

// inferred from __vt__Q23atn13ModelInstance (0x80778958): instance of a shared model
class ModelInstance : public IRender {
public:
    ModelInstance(InstData_unk* data);
    virtual ~ModelInstance();
    virtual int vf0C() { return 1; }
    virtual void vf10(InstCamera_unk* camera); // inferred: draw
    virtual int vf14() { return 1; }
    virtual void vf18(InstCamera_unk* camera) {
        if (mData->mModel.mType == 8 && mVisible && lbl_8087EFA8->m1D0 != 0) {
            mVisible = fn_800C0A50(camera, &mBox);
        }
    }
    virtual void vf1C() {}

    void fn_8009E6EC(); // inferred: culling and queueing
    void fn_8009EB20(); // inferred: LOD selection
    void fn_8009EE30(const Matrix3x4& mtx); // inferred: sets the matrix
    void fn_8009F4AC(InstModel_unk* model); // inferred: applies the instance matrices to the model

    static void fn_8009F444(ModelInstance* instance, InstModel_unk* model);
    static void fn_8009F448(ModelInstance* instance, InstModel_unk* model);

    // inferred: lens of the model and screen data of the instance (inline, name invented)
    void SetupLens_unk(InstModel_unk* model) {
        InstLens_unk* lens = model->mLens;
        void* screen = m64;
        if (lens == 0) {
            lens = lbl_8087EFB4->m2FC;
        }
        mLens = lens;
        if (screen == 0) {
            screen = fn_800C122C(lbl_8087EFB4, InstSphere_unk(mSphere));
            m64 = screen;
        }
        model->mLens = (InstLens_unk*)screen;
    }

    InstData_unk* mData; // 0x04
    Sphere mSphere;      // 0x08
    InstBox_unk mBox;    // 0x18
    Matrix3x4 mMtx;      // 0x30
    int mLod;            // 0x60
    void* m64;           // inferred: screen data
    InstLens_unk* mLens; // 0x68
    int mFlags;          // 0x6C: inferred: flags added to the model while drawn
    float m70;           // inferred: UV scroll speed
    Vector2_unk m74;     // inferred: UV scroll
    void* m7C;           // inferred: UV scroll owner
    Matrix3x4* m80;      // inferred: node matrices given by the owner
    union {
        unsigned char m84;
        struct {
            unsigned char mVisible : 1; // 0x84
            unsigned char mFlag40 : 1;
            unsigned char mShadowVisible : 1;
            unsigned char m84_4 : 5;
        };
    };
};

} // namespace atn

// inferred: matrices saved while an instance draws the shared model
static atn::Matrix3x4* lbl_8087EF40;

namespace atn {

ModelInstance::ModelInstance(InstData_unk* data) : mData(data) {
    mMtx.SetIdentity();
    mLod = 0;
    m64 = 0;
    mFlags = 0;
    m70 = 1.0f;
    m74.x = 0.0f;
    m74.y = 0.0f;
    m7C = 0;
    m80 = 0;
    if (lbl_8087EF40 == 0) {
        lbl_8087EF40 = new (6, "", "", 0) Matrix3x4[0x100];
    }
    m84 = 0;
    mData->mModel.mInstances++;
}

// NONMATCHING: 86.96% - the original stores the vtable pointer first
ModelInstance::~ModelInstance() {
    mData->mModel.mInstances--;
}

// inferred: shadow test of the model flags (inline, name invented)
inline int HasShadow_unk(InstModel_unk* model) {
    unsigned int flags = model->mFlags;
    int shadow = false;
    int cast = true;
    if ((flags & 0x400) != 0x400 && (flags & 0x800) != 0x800) {
        cast = false;
    }
    if (cast) {
        InstLens_unk* lens = model->mLens;
        int enabled = true;
        if (lens == 0) {
            lens = lbl_8087EFB4->m2FC;
        }
        if (lens->m120 == 0 && (flags & 0x1000) != 0x1000) {
            enabled = false;
        }
        if (enabled) {
            shadow = true;
        }
    }
    return shadow;
}

// NONMATCHING: 97.06% - register choices in the inlined shadow tests and the plane test
void ModelInstance::fn_8009E6EC() {
    Sphere* sphere = &mSphere;
    InstModel_unk* model = &mData->mModel;
    SetupLens_unk(model);
    mData->mModel.mFlags &= 0x42030;
    int flags = mFlags;
    mData->mModel.mFlags |= flags;
    mVisible = 1;
    if (fn_8004FF58(lbl_8087EFB4->m204, sphere) == 0) {
        mVisible = 0;
    }
    mShadowVisible = 0;
    InstSphere2_unk shadow;
    if (HasShadow_unk(&mData->mModel) && (mData->mModel.mFlags & 0x400) == 0x400) {
        InstLens_unk* lens = mData->mModel.mLens;
        if (lens == 0) {
            lens = lbl_8087EFB4->m2FC;
        }
        Vector3 center(sphere->mCenter);
        PSMTXMultVec(lens->m124.m, &center, &center);
        shadow.mCenter = center;
        shadow.mRadius = sphere->mRadius;
        if (fn_8004FF58(lbl_8087EFB4->m204, (Sphere*)&shadow) != 0) {
            mShadowVisible = 1;
        }
    }
    mFlag40 = 0;
    if (!mVisible && !mShadowVisible) {
        return;
    }
    fn_800BDB58(lbl_8087EFB4, this, 0.0f, 2);
    if ((mData->mModel.mFlags & 4) == 4 && mVisible) {
        fn_800BDB58(lbl_8087EFB4, this, mData->mModel.mDrawOrder, 3);
    }
    if (HasShadow_unk(&mData->mModel)) {
        if ((mData->mModel.mFlags & 0x400) == 0x400) {
            if (mShadowVisible) {
                InstLens_unk* lens = mData->mModel.mLens;
                if (lens == 0) {
                    lens = lbl_8087EFB4->m2FC;
                }
                if (lens->m160 + PSVECDotProduct(&lens->m154, &shadow.mCenter) < shadow.mRadius) {
                    fn_800BDB58(lbl_8087EFB4, this, mData->mModel.mDrawOrder, 4);
                }
            }
        } else if (mVisible) {
            fn_800BDB58(lbl_8087EFB4, this, mData->mModel.mDrawOrder, 4);
        }
    }
    if (mVisible) {
        if ((mData->mModel.mFlags & 0x10) == 0x10) {
            fn_800BDB58(lbl_8087EFB4, this, mData->mModel.mDrawOrder, 8);
        }
        if ((mData->mModel.mFlags & 0x20) == 0x20) {
            fn_800BDB58(lbl_8087EFB4, this, mData->mModel.mDrawOrder, 6);
        }
    }
    if ((mData->mModel.mFlags & 0x2000) == 0x2000) {
        fn_800BDB58(lbl_8087EFB4, this, mData->mModel.mDrawOrder, 9);
    }
    if ((mData->mModel.mFlags & 0x40000) == 0x40000) {
        lbl_8087EFB4->m54960 = 1;
    }
    if (mFlag40) {
        fn_800BDB58(lbl_8087EFB4, this, 0.0f, 0);
    }
}

void ModelInstance::fn_8009EB20() {
    if (mData->mModel.mLodCount <= 1) {
        return;
    }
    InstCamera_unk* camera = lbl_8087EFB4;
    Vector3* eye = (Vector3*)(camera->m000 + 0x10C);
    Vector3 d(mSphere.mCenter.x - eye->x, mSphere.mCenter.y - eye->y, mSphere.mCenter.z - eye->z);
    float dist = PSVECMag(&d) - mSphere.mRadius;
    float zero = (0.0f > dist) ? 0.0f : dist;
    for (mLod = mData->mModel.mLodCount - 1; mLod != 0; mLod--) {
        float lod = ((InstLod_unk*)((unsigned char*)mData->mModel.mLods + mLod * 0x18))->mDistance;
        if ((float)__fabs(lod) < 1.1920929e-7f) {
            lod = lbl_8087EFA8->m208[mLod];
        }
        if (lod <= zero) {
            break;
        }
    }
}

// NONMATCHING: 97.38% - stack slots of the callback temporaries (reverse order) and this/camera
// registers swapped
void ModelInstance::vf10(InstCamera_unk* camera) {
    InstModel_unk* model = &mData->mModel;
    switch (camera->m2F8) {
    case 0:
        if (!mFlag40) {
            return;
        }
        break;
    case 4:
        if (!mShadowVisible) {
            return;
        }
        break;
    case 6:
    case 8:
        if (!mVisible) {
            return;
        }
        break;
    }
    if (model->mType == 8) {
        int flags = mFlags;
        model->mFlags |= flags;
        *(Matrix3x4*)(model->m08) = mMtx;
        model->mCallback = InstCallback_unk((void (*)(ModelInstance*, Model_unk*))fn_8009F444,
                                            (void (*)(ModelInstance*, Model_unk*))fn_8009F448, this,
                                            (Model_unk*)model);
        model->mLod = mLod;
        int count = model->mLodCount;
        if (model->mLod >= count) {
            model->mLod = count - 1;
        }
        if (model == &mData->mModel) {
            SetupLens_unk(model);
        }
    }
    model->vf10(camera);
    model->mCallback.Clear_unk();
    if (model == &mData->mModel) {
        model->mLens = mLens;
    }
    int flags = mFlags;
    model->mFlags &= ~flags;
}

// NONMATCHING: 49% - the original takes the smallest and largest corners with paired-single selects
// (inline asm in the original headers) and builds the corners in a different order
void ModelInstance::fn_8009EE30(const Matrix3x4& mtx) {
    mMtx = mtx;
    double sz = PSVECMag(&Vector3(mMtx.m[0][2], mMtx.m[1][2], mMtx.m[2][2]));
    double sy = PSVECMag(&Vector3(mMtx.m[0][1], mMtx.m[1][1], mMtx.m[2][1]));
    Vector3 scale(PSVECMag(&Vector3(mMtx.m[0][0], mMtx.m[1][0], mMtx.m[2][0])), sy, sz);
    float maxYZ = ((float)sy > (float)sz) ? (float)sy : (float)sz;
    float maxScale;
    if (scale.x > maxYZ) {
        maxScale = scale.x;
    } else if (scale.y > scale.z) {
        maxScale = scale.y;
    } else {
        maxScale = scale.z;
    }
    if (fn_8009FFCC(mData) != 0) {
        return;
    }
    mSphere = mData->mModel.mSphere;
    PSMTXMultVec(mMtx.m, &mSphere.mCenter, &mSphere.mCenter);
    mSphere.mRadius *= maxScale * mData->mModel.mScale;
    InstBox_unk* box = *(InstBox_unk**)((unsigned char*)&mData->mModel + 0x16C);
    if (box == 0) {
        return;
    }
    InstBox_unk local = *(InstBox_unk*)((unsigned char*)box + 0x10);
    Vector3 corners[8];
    corners[0] = Vector3(local.mMin.x, local.mMin.y, local.mMin.z);
    corners[1] = Vector3(local.mMax.x, local.mMin.y, local.mMin.z);
    corners[2] = Vector3(local.mMin.x, local.mMax.y, local.mMin.z);
    corners[3] = Vector3(local.mMax.x, local.mMax.y, local.mMin.z);
    corners[4] = Vector3(local.mMin.x, local.mMin.y, local.mMax.z);
    corners[5] = Vector3(local.mMax.x, local.mMin.y, local.mMax.z);
    corners[6] = Vector3(local.mMin.x, local.mMax.y, local.mMax.z);
    corners[7] = Vector3(local.mMax.x, local.mMax.y, local.mMax.z);
    fn_805F9420(mMtx.m, corners, corners, 8);
    InstBox_unk out;
    out.mMin = corners[0];
    out.mMax = corners[0];
    for (int i = 1; i < 8; i++) {
        if (corners[i].x < out.mMin.x) out.mMin.x = corners[i].x;
        if (corners[i].y < out.mMin.y) out.mMin.y = corners[i].y;
        if (corners[i].z < out.mMin.z) out.mMin.z = corners[i].z;
        if (corners[i].x > out.mMax.x) out.mMax.x = corners[i].x;
        if (corners[i].y > out.mMax.y) out.mMax.y = corners[i].y;
        if (corners[i].z > out.mMax.z) out.mMax.z = corners[i].z;
    }
    mBox = out;
    mBox = fn_80070B60(&mBox, mData->mModel.mScale);
}

void ModelInstance::fn_8009F444(ModelInstance* instance, InstModel_unk* model) {
    instance->fn_8009F4AC(model);
}

// NONMATCHING: 98.80% - r7/r8 swapped (counter and node array)
void ModelInstance::fn_8009F448(ModelInstance* instance, InstModel_unk* model) {
    Matrix3x4* nodes = model->mNodeMtx;
    for (unsigned int i = 0; i < model->mNodeCount; i++) {
        nodes[i] = lbl_8087EF40[i];
    }
}

// NONMATCHING: 99.58% - r28/r29 swapped (node array and offset)
void ModelInstance::fn_8009F4AC(InstModel_unk* model) {
    Matrix3x4 base(mMtx);
    if ((mFlags & 0x400) == 0x400 && lbl_8087EFB4->m2F8 == 4) {
        Matrix3x4 m;
        PSMTXConcat(lbl_8087EFB4->m2FC->m124.m, base.m, m.m);
        base = m;
    }
    Matrix3x4* nodes = model->mNodeMtx;
    for (unsigned int i = 0; i < model->mNodeCount; i++) {
        lbl_8087EF40[i] = nodes[i];
        Matrix3x4* src;
        Matrix3x4 tmp;
        if (m80 != 0) {
            src = &m80[i];
        } else {
            PSMTXConcat(base.m, nodes[i].m, tmp.m);
            src = &tmp;
        }
        nodes[i] = *src;
    }
    fn_80091684(model, lbl_8087EFB4->m15C, ((InstLod_unk*)((unsigned char*)model->mLods + model->mLod * 0x18))->mData);
    if (model == &mData->mModel && m7C != 0) {
        ((Model_unk*)model)->fn_800954DC(m7C, m74, m70);
    }
}

} // namespace atn
