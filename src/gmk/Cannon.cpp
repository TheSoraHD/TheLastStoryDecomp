#include "gmk/Cannon.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(atn::Model* model, int id, const char* path); // inferred: loads a motion
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_80232B7C(void* owner, int arg);

// inferred: effect manager (atn::EffectManager, global instance at 0x8087F3C0)
namespace atn {
class EffectManager {
public:
    void fn_8023A1F4(unsigned int owner, int group, const atn::Vector3* vec); // inferred: sets a direction
    unsigned char m00[0xB8];
    int mB8;
};
} // namespace atn
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" atn::EffectManager* lbl_8087F3C0;
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(atn::EffectManager* mgr, void* owner, int id, int arg); // inferred: stops an effect
// inferred: debug menu
// lbl_8087EF10 (debug menu root) is declared in the headers
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

// inferred: plays an effect at the model (as in gmk/Cannonball.cpp; name invented)
inline void PlayEfp_unk(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
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
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

namespace gmk {

GimmickCommon* Cannon::fn_80408324(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) Cannon(parent, id, arg);
    }
    return 0;
}

Cannon::Cannon(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mModelCrush(8, 32), mShootWait(-1), mShootWaitRand(0),
      mTimer(0), m918(30), m91C(0.5f), m920(100.0f), mWindDirection(0.3f, 0.0f, 0.0f) {
    m54 = 0;
}

Cannon::~Cannon() {}

int Cannon::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        fn_803EDB18(&mModelCrush);
        return 1;
    }
    return 0;
}

void Cannon::vf10() {
    fn_803ED0D4(&mModel, 1);
    fn_803ED0D4(&mModelCrush, 1);
    if (m54 == 0) {
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        Start();
    } else if (m54 == 2) {
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        GimmickMsg_unk msg(3);
        if (mId % 1000 != 0) {
            msg.mType = 4;
        }
        vf40(&msg);
        fn_803ED0D4(&mModel, 1);
        fn_803ED0D4(&mModelCrush, 1);
    }
    switch (m54) {
    case 3: {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        float frame = mModel.mTracks[0].m08;
        if (frame >= mModel.fn_80097D7C(0)) {
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
        break;
    }
    case 4:
        if (mTimer > 0 && --mTimer <= 0) {
            GimmickMsg_unk msg(5);
            vf40(&msg);
        }
        break;
    case 5: {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
        float frame = mModel.mTracks[0].m08;
        if (frame >= mModel.fn_80097D7C(0)) {
            GimmickMsg_unk msg(4);
            vf40(&msg);
        }
        break;
    }
    case 6: {
        GimmickMsg_unk msg(7);
        vf40(&msg);
        break;
    }
    }
}

void Cannon::vf14() {
    if (vf38() != 0) {
        fn_803ED774(vf38());
    }
}

int Cannon::vf1C() {
    int busy = 0;
    if (mModel.fn_8008B140() != 0 || mModelCrush.fn_8008B140() != 0) {
        busy = 1;
    }
    if (mEfpShoot.fn_80237874() != 0 || mEfpCrush.fn_80237874() != 0) {
        busy = 1;
    }
    return busy;
}

void Cannon::vf20() {
    char motion[256];
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            if (m58 != 0) {
                csv.fn_8005B9CC();
            }
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "MODEL_CRUSH") == 0) {
            fn_8008AD4C(&mModelCrush, csv.fn_8005B9CC(), 0);
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(motion, csv.fn_8005B9CC());
            fn_80097A88(&mModel, fn_802180A8(csv.fn_8005B9CC()), motion);
        } else if (strcmp(field, "EFP_SHOOT") == 0) {
            if (m58 != 0) {
                csv.fn_8005B9CC();
            }
            mEfpShoot.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP_CRUSH") == 0) {
            mEfpCrush.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "PARAM") == 0) {
            mShootWait = atoi(csv.fn_8005B9CC());
            mShootWaitRand = atoi(csv.fn_8005B9CC());
        } else if (strcmp(field, "SE_PARAM") == 0) {
            mSeReady = csv.fn_8005B9CC();
            mSeShoot = csv.fn_8005B9CC();
            mSeCrush = csv.fn_8005B9CC();
        }
    } while (csv.fn_8005BBF8() != 0);
}

void Cannon::Start() {
    vf2C();
    fn_803ED0D4(&mModel, 1);
    fn_803ED0D4(&mModelCrush, 1);
    GimmickMsg_unk msg(1);
    if (m58 == 1) {
        msg.mType = 4;
    } else if (m58 == 2) {
        msg.mType = 7;
    }
    vf40(&msg);
}

int Cannon::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
        mModel.fn_80097C08(0, 0, 0, 1, 1, 1.0f, 0.1f);
        fn_803ED5D0(&mModel);
        mModel.mTracks[0].m08 = 0.0f;
        break;
    case 3:
        mModel.fn_80097C08(0, 0, 0, 1, 1, 0.0f, 0.1f);
        fn_800C344C(mSeReady.c_str(), &m6C, 0, -1, 1.0f);
        break;
    case 4:
        mTimer = mShootWait;
        if (mShootWaitRand > 0) {
            mTimer += rand() % mShootWaitRand - mShootWaitRand / 2;
        }
        break;
    case 5:
        if (m58 != 0) {
            mModel.fn_80097C08(0, 2, 0, 1, 1, 0.0f, 0.1f);
        } else {
            mModel.fn_80097C08(0, 1, 0, 1, 1, 0.0f, 0.1f);
        }
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        fn_80232B7C(this, 1);
        PlayEfp_unk(lbl_8087F3C0, &mEfpShoot, &mModel);
        lbl_8087F3C0->fn_8023A1F4((unsigned int)this, 1, &mWindDirection);
        fn_800C344C(mSeShoot.c_str(), &m6C, 0, -1, 1.0f);
        break;
    case 6:
        lbl_8087F3C0->mB8 = 1;
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        fn_80232B7C(this, 1);
        PlayEfp_unk(lbl_8087F3C0, &mEfpCrush, &mModelCrush);
        lbl_8087F3C0->mB8 = 0;
        fn_800C344C(mSeCrush.c_str(), &m6C, 0, -1, 1.0f);
        break;
    case 7:
        if (m54 != 6) {
            lbl_8087F3C0->mB8 = 1;
            fn_80239DAC(lbl_8087F3C0, this, 1, 1);
            fn_80232B7C(this, 1);
            PlayEfp_unk(lbl_8087F3C0, &mEfpCrush, &mModelCrush);
            lbl_8087F3C0->mB8 = 0;
        }
        break;
    }
    m54 = msg->mType;
    return 1;
}

void Cannon::vf70(int state) {
    GimmickMsg_unk msg(4);
    switch (state) {
    case 0:
    case 1:
    case 2:
        msg.mType = 1;
        break;
    case 3:
    case 4:
    case 5:
        msg.mType = 4;
        break;
    case 6:
    case 7:
        msg.mType = 7;
        break;
    }
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

void Cannon::vf6C(void* menu) {
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
    fn_800874C8(group, "shootWait", &mShootWait, -1, 9999, 1, 0, 0);
    fn_800874C8(group, "shootWaitRand", &mShootWaitRand, 0, 9999, 1, 0, 0);
    fn_80087E9C(group, "windDirection", &mWindDirection, 0, 0, -9999.0f, 9999.0f, 1.0f);
}

atn::Model* Cannon::vf38() {
    return m54 < 6 ? &mModel : &mModelCrush;
}

} // namespace gmk
