#include "gmk/PlayEffect.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound

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
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, Model_unk* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
extern "C" void fn_8023A02C(EffectMgr_unk* mgr, void* owner, int arg2, int arg3);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x74];
    int mDrawGimmickState; // 0x74
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: copies a string unless it is the destination itself (name invented)
inline void CopyString_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}

namespace gmk {

GimmickCommon* PlayEffect::fn_803F69E4(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) PlayEffect(parent, id, arg);
    }
    return 0;
}

PlayEffect::~PlayEffect() {
    fn_800CB5C8(&mSound, 0, 0);
    if (mModel != 0) {
        delete mModel;
    }
}

int PlayEffect::vf0C() {
    return fn_803EC91C() == 0;
}

void PlayEffect::vf10() {
    if (m54 == 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        fn_800CB5C8(&mSound, 10, 0);
        mOn = 0;
        Start();
    }
    if (mModel != 0) {
        fn_803ED0D4(mModel, 1);
    }
    switch (m54) {
    case 1:
        if (mOn != 0) {
            fn_803F7198(1);
        }
        break;
    case 2:
    case 3:
        if (mOn == 0) {
            fn_803F7198(0);
        }
        break;
    }
}

void PlayEffect::vf14() {
    if (lbl_8087F0A8->mDrawGimmickState != 0) {
        fn_803EDCF4();
    }
}

int PlayEffect::vf1C() {
    for (int i = 0; i < 2; i++) {
        if (mEffects[i].mRes.fn_802376D0() != 0 || mEffects[i].mEfp.fn_80237874() != 0) {
            return 1;
        }
    }
    if (mModel != 0 && mModel->fn_8008B140() != 0) {
        return 1;
    }
    return 0;
}

void PlayEffect::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            mModel = new (12, "", "", 0) Model_unk(8, 32);
            fn_8008AD4C(mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(mModel, &csv);
        } else if (strcmp(field, "EFF") == 0) {
            for (int i = 0; i < 2; i++) {
                if (strlen(csv.fn_8005B9CC()) != 0) {
                    mEffects[i].mRes.fn_80237654(csv.mField);
                }
            }
        } else if (strcmp(field, "EFP") == 0) {
            for (int i = 0; i < 2; i++) {
                if (strlen(csv.fn_8005B9CC()) != 0) {
                    mEffects[i].mEfp.fn_8023780C(csv.mField);
                }
            }
        } else if (strcmp(field, "SE") == 0) {
            for (int i = 0; i < 2; i++) {
                CopyString_unk(mEffects[i].mSe, csv.fn_8005B9CC());
            }
        } else if (strcmp(field, "FLAG") == 0) {
            for (const char* flag = csv.fn_8005B9CC(); flag[0] != '\0'; flag = csv.fn_8005B9CC()) {
                if (strcmp(flag, "EFF_REFLECT") == 0) {
                    mFlags |= 1;
                }
            }
        } else if (strcmp(field, "INIT") == 0) {
            mInitState = strcmp(csv.fn_8005B9CC(), "ON") == 0 ? 3 : 1;
        }
    } while (csv.fn_8005BBF8() != 0);
}

void PlayEffect::Start() {
    vf2C();
    GimmickMsg_unk msg;
    msg.mType = m58 == 0 ? mInitState : 3;
    vf40(&msg);
}

int PlayEffect::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    return m54 = msg->mType;
}

void PlayEffect::fn_803F7198(int index) {
    fn_80239DAC(lbl_8087F3C0, this, 0, 1);
    fn_800CB5C8(&mSound, 10, 0);
    Effect_unk* effect = &mEffects[index];
    if (index == 0) {
        lbl_8087F3C0->mB8 = 1;
    }
    fn_80232B7C(this, 0);
    if (effect->mRes.mResource != 0) {
        if (mModel != 0) {
            fn_8023A680(lbl_8087F3C0, &effect->mRes, -1, 4, mModel, 0, 0, 0, 0, -1, 1, 1.0f);
        } else {
            fn_8023A680(lbl_8087F3C0, &effect->mRes, -1, 0, 0, &m6C, &m78, 0, 0, -1, 1, 1.0f);
        }
    }
    Model_unk* model = mModel;
    if (model != 0) {
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
        fn_8023A8B4(lbl_8087F3C0, &effect->mEfp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
    } else {
        fn_8023A8B4(lbl_8087F3C0, &effect->mEfp, 0, 0, m6C, m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                    1.0f);
    }
    if ((mFlags & 1) == 1) {
        fn_8023A02C(lbl_8087F3C0, this, 0, 8);
    }
    if (index == 0) {
        lbl_8087F3C0->mB8 = 0;
    }
    atn::SoundRef sound = fn_800C344C(effect->mSe, &m6C, 0, -1, 1.0f);
    if (index == 0) {
        mSound = sound;
    }
    mOn = index == 0;
}

void PlayEffect::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

} // namespace gmk
