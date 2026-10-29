#include "game/SetConfig.h"
#include "atn/SoundManager.h"
#include "gmk/AttractHatePoint.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x194];
    int m194; // inferred: the gimmick is disabled
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: field object (class unknown)
struct HateField_unk {
    unsigned char m00[0x5694];
    int m5694; // inferred: messages other than 1 are ignored while set
};
extern "C" HateField_unk* lbl_8087F430;
extern "C" int fn_80370174(HateField_unk* field, int id);
extern "C" void fn_80370320(HateField_unk* field, int id, int arg2, int arg3);
extern "C" int fn_80375184(HateField_unk* field);
extern "C" void fn_80376730(HateField_unk* field);
// inferred: request of the battle object (class unknown)
struct HateBattle_unk {
    unsigned char m000[0x7C0];
    int m7C0;
    int m7C4;
    int m7C8;
    int m7CC;
    int m7D0;
    int m7D4;
    int m7D8;
};
extern "C" HateBattle_unk* lbl_8087F490;

namespace gmk {

// inferred: sounds of the states 2 and 3
static const char* const sSounds[] = {"SE_DGCOM_007", "SE_BTCOM_021"};

GimmickCommon* AttractHatePoint::fn_80421AB8(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        return new (12, "", "", 0) AttractHatePoint(parent, desc);
    }
    return 0;
}

AttractHatePoint::~AttractHatePoint() {
    fn_800CB5C8(&mSound, 0, 0);
}

int AttractHatePoint::vf1C() {
    if (mEfpOn.fn_80237874() != 0 || mEfpAttract.fn_80237874() != 0 || mEfpWait.fn_80237874() != 0) {
        return 1;
    }
    return 0;
}

void AttractHatePoint::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EFP") == 0) {
            const char* kind = csv.fn_8005B9CC();
            if (strcmp(kind, "ON") == 0) {
                mEfpOn.fn_8023780C(csv.fn_8005B9CC());
            } else if (strcmp(kind, "ATTRACT") == 0) {
                mEfpAttract.fn_8023780C(csv.fn_8005B9CC());
            } else if (strcmp(kind, "WAIT") == 0) {
                mEfpWait.fn_8023780C(csv.fn_8005B9CC());
            }
        }
    } while (csv.fn_8005BBF8() != 0);
}

int AttractHatePoint::vf0C() {
    if (fn_803EC91C() == 0) {
        m54 = 0;
        return 1;
    }
    return 0;
}

void AttractHatePoint::vf28() {
    m6C = mDesc->mPos;
    m78.y = mDesc->mRotY;
    mRadius = mDesc->m40;
    GimmickMsg_unk msg(1);
    vf40(&msg);
}

void AttractHatePoint::vf10() {
    if (lbl_8087F0A8->m194 != 0) {
        return;
    }
    mInRange = 0;
    if (m54 == 0) {
        vf28();
    }
    if (m54 == 1) {
        return;
    }
    PlayerTask* player = lbl_8087F8A0->mHead;
    if (m54 == 2) {
        atn::Vector3 d = player->mPos - m6C;
        if (fn_805F9920(&d) < mRadius * mRadius) {
            if (!player->mFlag6) {
                if (!player->HasC48_unk() && !player->mFlag31) {
                    HateBattle_unk* battle = lbl_8087F490;
                    battle->m7C0 = 15;
                    battle->m7C4 = -1;
                    battle->m7C8 = -1;
                    battle->m7CC = 0;
                    battle->m7D0 = 0;
                    battle->m7D4 = 0;
                    battle->m7D8 = 0;
                    mInRange = 1;
                }
            }
            if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0xA2) == 0) {
                fn_80370320(lbl_8087F430, 0xA2, 1, 0);
            }
        }
    }
}

void AttractHatePoint::vf14() {}

int AttractHatePoint::vf40(GimmickMsg_unk* msg) {
    if (lbl_8087F0A8->m194 != 0) {
        return 0;
    }
    if (msg == 0) {
        return 0;
    }
    int type = msg->mType;
    if (type != 1 && lbl_8087F430 != 0 && lbl_8087F430->m5694 != 0) {
        return 0;
    }
    switch (type) {
    case 1:
        m54 = type;
        if (fn_80239D58(lbl_8087F3C0, this, 2) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 2, 1);
        }
        if (fn_80239D58(lbl_8087F3C0, this, 4) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 4, 1);
        }
        fn_800CB5C8(&mSound, 15, 0);
        break;
    case 2:
        m54 = type;
        if (fn_80239D58(lbl_8087F3C0, this, 2) == 0) {
            lbl_8087F3C0->mB8 = 1;
            fn_80232B7C(this, 2);
            fn_8023A8B4(lbl_8087F3C0, &mEfpOn, 0, 0, m6C, m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                        1.0f);
            lbl_8087F3C0->mB8 = 0;
        }
        if (fn_80239D58(lbl_8087F3C0, this, 4) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 4, 1);
        }
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->m34C8 = 0;
        }
        fn_800CB5C8(&mSound, 15, 0);
        mSound = fn_800C344C(sSounds[0], &m6C, 0, -1, 0.3f);
        if (lbl_8087EFE8 != 0) {
            lbl_8087EFE8->m34C8 = 1;
        }
        break;
    case 3:
        if (mInRange != 0) {
            if (lbl_8087F430 != 0 && fn_80375184(lbl_8087F430) == 0 && SetConfig::sInstance->mAutoSave != 0) {
                fn_80376730(lbl_8087F430);
            }
            m54 = msg->mType;
            fn_800C344C(sSounds[1], &m6C, 0, -1, 1.0f);
            fn_80232B7C(0, 0);
            fn_8023A8B4(lbl_8087F3C0, &mEfpAttract, 0, 0, m6C, m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                        1.0f);
        }
        break;
    case 4:
        m54 = type;
        if (fn_80239D58(lbl_8087F3C0, this, 2) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 2, 1);
        }
        if (fn_80239D58(lbl_8087F3C0, this, 4) == 0) {
            lbl_8087F3C0->mB8 = 1;
            fn_80232B7C(this, 4);
            fn_8023A8B4(lbl_8087F3C0, &mEfpWait, 0, 0, m6C, m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                        1.0f);
            lbl_8087F3C0->mB8 = 0;
        }
        fn_800CB5C8(&mSound, 15, 0);
        break;
    }
    return m54;
}

int AttractHatePoint::vf60() {
    if (lbl_8087F0A8->m194 != 0) {
        return 0;
    }
    int ret = 0;
    if (m54 >= 2 && mDesc->m7C != 0) {
        ret = 1;
    }
    return ret;
}

void AttractHatePoint::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

void AttractHatePoint::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 99, 1, 0, 0);
    fn_8008771C(group, "check_radius", &mRadius, 0, 0, 0.0f, 1000.0f, 0.1f);
}

} // namespace gmk
