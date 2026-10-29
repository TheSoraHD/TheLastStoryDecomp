#include "gmk/WaterLevel.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" double fabs(double x);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(atn::Model* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" void PSMTXTranspose(const Mtx src, Mtx dst);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: screen data of a sphere (result of fn_800C122C); names invented
struct ScreenInfo_unk {
    unsigned char m000[0x120];
    int m120;
};
struct Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" ScreenInfo_unk* fn_800C122C(Camera_unk* camera, const atn::Model::Sphere_unk* sphere);
extern "C" void fn_800C2C20(ScreenInfo_unk* info, const Plane_unk* plane); // inferred: sets the reflection plane

// inferred: normalizes unless the vector is (almost) zero; name invented
inline void SafeNormalize_unk(atn::Vector3* v) {
    if ((float)__fabs(fn_805F9920(v)) < 1.1920929e-7f) {
        return;
    }
    PSVECNormalize(v, v);
}

namespace gmk {

// inferred: the box update is an inline (its locals go to the bottom of the frame); name invented
inline void UpdateBox_unk(WaterLevel* self) {
    MtxPtr node = self->mModel.GetNodeMtx_unk(self->mNode);
    atn::Vector3 pos = self->mBoxPos;
    if (node != 0) {
        pos = ((atn::Matrix3x4*)node)->GetTrans_unk();
    }
    pos.x += self->mBoxOffset.x;
    pos.y += self->mBoxOffset.y;
    pos.z += self->mBoxOffset.z;
    self->mColli.fn_800575BC(&pos, &self->m78, &self->mBoxSize);
}

GimmickCommon* WaterLevel::fn_8042E36C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) WaterLevel(parent, id, arg);
    }
    return 0;
}

WaterLevel::~WaterLevel() {}

int WaterLevel::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        mColli.m0C = (int)this;
        mColli.m1C = 0x1A;
        return 1;
    }
    return 0;
}

void WaterLevel::Start() {
    vf2C();
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    mBoxPos = m6C;
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

void WaterLevel::vf10() {
    if (m54 == 0) {
        Start();
    }
    if (vf38() != 0) {
        fn_803ED5D0(vf38());
    }
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    UpdateBox_unk(this);
    mColli.mFlags |= 1;
    if (mReflect != 0 && mNode != -1) {
        Camera_unk* camera = lbl_8087EFB4;
        ScreenInfo_unk* info = fn_800C122C(camera, &mModel.fn_80094F98());
        if (info != 0) {
            Plane_unk plane = mPlane;
            MtxPtr mtx = mModel.GetNodeMtx_unk(mNode);
            atn::Matrix3x4 m(*(atn::Matrix3x4*)mtx);
            PSMTXInverse(m.m, m.m);
            PSMTXTranspose(m.m, m.m);
            SafeNormalize_unk(&plane.mNormal);
            PSMTXMultVec(m.m, &plane.mNormal, &plane.mNormal);
            SafeNormalize_unk(&plane.mNormal);
            atn::Vector3 trans = ((atn::Matrix3x4*)mtx)->GetTrans_unk();
            plane.mDist = -PSVECDotProduct(&plane.mNormal, &trans);
            info->m120 = 0;
            fn_800C2C20(info, &plane);
        }
    }
}

void WaterLevel::vf14() {
    if (lbl_8087F0A8->m18 != 0 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int WaterLevel::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    return mEfp.fn_80237874() != 0;
}

void WaterLevel::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    char path[0x100];
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), path);
        }
    } while (csv.fn_8005BBF8() != 0);
}

void WaterLevel::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "COLLI_BOX") == 0) {
            mBoxOffset.x = fn_800DC288(csv.fn_8005B9CC());
            mBoxOffset.y = fn_800DC288(csv.fn_8005B9CC());
            mBoxOffset.z = fn_800DC288(csv.fn_8005B9CC());
            mBoxRot.x = fn_800DC288(csv.fn_8005B9CC());
            mBoxRot.y = fn_800DC288(csv.fn_8005B9CC());
            mBoxRot.z = fn_800DC288(csv.fn_8005B9CC());
            mBoxSize.x = fn_800DC288(csv.fn_8005B9CC());
            mBoxSize.y = fn_800DC288(csv.fn_8005B9CC());
            mBoxSize.z = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(field, "REFLECT_PLANE") == 0) {
            mReflect = 1;
            mPlane.mNormal.x = fn_800DC288(csv.fn_8005B9CC());
            mPlane.mNormal.y = fn_800DC288(csv.fn_8005B9CC());
            mPlane.mNormal.z = fn_800DC288(csv.fn_8005B9CC());
            mPlane.mDist = fn_800DC288(csv.fn_8005B9CC());
            mNode = mModel.fn_80092814(csv.fn_8005B9CC(), 0);
        }
    } while (csv.fn_8005BBF8() != 0);
}

int WaterLevel::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    m54 = msg->mType;
    if ((unsigned int)m54 > 3) {
        m54 = 0;
    }
    if (m54 == 2) {
        mModel.m34C = 1;
        mModel.mTracks[0].m20 = 1.0f;
        mModel.mTracks[0].m0C = 1.0f;
        mModel.fn_80097C08(0, 0, 0, 0, 1, 0.0f, 0.1f);
        Color4f_unk color;
        atn::Vector3 rot;
        atn::Vector3 pos;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &mEfp, &mModel, 0, pos, rot, color, -1, -1, 1, 1.0f);
    }
    return m54;
}

void WaterLevel::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

void WaterLevel::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80088724(group, "rot", &m78, 0, 0, -3.1415927f, 3.1415927f, 0.05235988f);
    fn_80087E9C(group, "scl", &m90, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    fn_800874C8(group, "locData", &m58, 0, 99, 1, 0, 0);
    mB0.fn_803F11F8(group);
}

atn::Model* WaterLevel::vf38() {
    return &mModel;
}

} // namespace gmk
