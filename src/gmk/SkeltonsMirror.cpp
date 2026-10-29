#include "gmk/SkeltonsMirror.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(Model_unk* model, const char* path, int arg, float lod); // inferred: adds a LOD model

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);

namespace gmk {

GimmickCommon* SkeltonsMirror::fn_80414AC4(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) SkeltonsMirror(parent, desc);
    }
    return 0;
}

SkeltonsMirror::~SkeltonsMirror() {}

int SkeltonsMirror::vf1C() {
    if (mModel.fn_8008B140() != 0 || mColli.fn_800580BC() != 0 || mEfp.fn_80237874() != 0) {
        return 1;
    }
    return 0;
}

void SkeltonsMirror::vf20() {
    char path[256];
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void SkeltonsMirror::vf24() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        csv.fn_8005B9CC();
    } while (csv.fn_8005BBF8() != 0);
}

int SkeltonsMirror::vf0C() {
    return fn_803EC91C() == 0;
}

void SkeltonsMirror::Start() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    m78.x = 0.0f;
    m78.z = 0.0f;
    if (vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
    m54 = 1;
    mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
    mColli.mFlags &= ~1;
}

void SkeltonsMirror::vf10() {
    if (m54 == 0) {
        Start();
    }
    if ((unsigned int)m54 > 2 && vf38() != 0) {
        fn_803ED0D4(vf38(), 1);
    }
}

void SkeltonsMirror::vf14() {
    if ((unsigned int)m54 > 2) {
        mModel.fn_8008CD60();
    }
}

int SkeltonsMirror::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
        fn_80239DAC(lbl_8087F3C0, this, 1, 0);
        mColli.mFlags &= ~1;
        break;
    case 2:
        mColli.mFlags &= ~1;
        fn_80239DAC(lbl_8087F3C0, this, 1, 0);
        break;
    case 3:
        fn_80232B7C(this, 1);
        mColli.mFlags |= 1;
        break;
    case 4:
        fn_80232B7C(this, 1);
        {
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
        break;
    }
    return m54 = msg->mType;
}

int SkeltonsMirror::vf60() {
    int ret = 0;
    if ((unsigned int)(m54 - 2) <= 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

Model_unk* SkeltonsMirror::vf38() {
    return &mModel;
}

} // namespace gmk
