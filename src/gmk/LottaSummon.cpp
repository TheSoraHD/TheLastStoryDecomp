#include "gmk/LottaSummon.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/NpcManager.h"
#include "game/NpcTask.h"
#include "game/PlayerTask.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: next character of the player list (0x14AC)
struct CharaListNode_unk {
    unsigned char m0000[0x14AC];
    CharaTask* mNext;
};

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" void* lbl_8087F048; // inferred: effect manager of the characters
extern "C" void fn_8010653C(void* mgr, CharaTask* chara, void* motion);

// inferred: field object (class unknown)
struct Field_unk {
    void fn_803750E4(int id);
    unsigned char m00[0x10D8];
    void* m10D8; // inferred: route tables
};
extern "C" Field_unk* lbl_8087F430;
// inferred: finds the place where `chara` stands in the route tables
extern "C" int fn_803CCA84(void* routes, CharaTask* chara, atn::Vector3* pos, atn::Vector3* rot);

// inferred: request of an event (names invented)
struct EventRequest_unk {
    EventRequest_unk(int type) : m04(-1), m08(-1), m0C(0), m10(0), m14(0), m18(0) { mType = type; }
    int mType;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
};
struct EventMgr_unk {
    void Request_unk(int type) { m7C0 = EventRequest_unk(type); }
    unsigned char m000[0x7C0];
    EventRequest_unk m7C0;
};
extern "C" EventMgr_unk* lbl_8087F490;

extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80088724(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);

// inferred: plays an effect at the gimmick (name invented)
inline void PlayEfp_unk(GimmickCommon* owner, atn::EfpFile* efp, int arg) {
    fn_80232B7C(owner, arg);
    lbl_8087F3C0->mB8 = 1;
    fn_8023A8B4(lbl_8087F3C0, efp, 0, 0, owner->m6C, owner->m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                1.0f);
    lbl_8087F3C0->mB8 = 0;
}
inline void PlayEfp2_unk(GimmickCommon* owner, atn::EfpFile* efp, int arg) {
    fn_80232B7C(owner, arg);
    lbl_8087F3C0->mB8 = 1;
    EffectMgr_unk* mgr = lbl_8087F3C0;
    fn_8023A8B4(mgr, efp, 0, 0, owner->m6C, owner->m78, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1, 1.0f);
    lbl_8087F3C0->mB8 = 0;
}

// inferred: names invented
inline void SetPos_unk(CharaTask* chara, const atn::Vector3& pos) { chara->mPos = pos; }
inline void SetRot_unk(CharaTask* chara, const atn::Vector3& rot) { chara->mRot = rot; }

// inferred: direction behind a rotation around y (name invented)
inline atn::Vector3 Back_unk(CharaTask* chara) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', chara->mRot.y);
    PSMTXMultVec(mtx, &dir, &dir);
    return atn::Vector3(-dir.x, -dir.y, -dir.z);
}

// inferred: name invented
inline float LenSq_unk(atn::Vector3 v) {
    return fn_805F9920(&v);
}

namespace gmk {

GimmickCommon* LottaSummon::fn_8043A770(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) LottaSummon(parent, id, arg);
    }
    return 0;
}

GimmickCommon* LottaSummon::fn_8043A850(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        LottaSummon* gimmick = new ((MemoryTag)12, "", "", 0) LottaSummon(parent, desc->mId, desc->mArg);
        if (gimmick != 0) {
            gimmick->m6C = desc->mPos;
            gimmick->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            gimmick->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
            gimmick->mDesc = desc;
        }
        return gimmick;
    }
    return 0;
}

LottaSummon::~LottaSummon() {
    if (lbl_8087F3C0 != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
}

int LottaSummon::vf0C() {
    return fn_803EC91C() == 0;
}

void LottaSummon::Start() {
    vf2C();
    GimmickMsg_unk msg(2);
    if (m58 == 1) {
        msg.mType = 1;
    }
    vf40(&msg);
}

// NONMATCHING: 99.9% - r29/r30 swapped for the hoisted addresses of the pushed direction and its
// temporary in the player loop. Tried: back/d declaration orders, a named copy of Back_unk(),
// other return forms of Back_unk (named, operator-, * -1), inverted condition, Normalize_unk,
// Assign_unk.
void LottaSummon::vf10() {
    if (mDesc != 0) {
        int enabled = mDesc->m7C;
        if (enabled == 0 && mEnabled != 0 && lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        if (enabled != 0 && mEnabled == 0) {
            GimmickMsg_unk msg(m54);
            vf40(&msg);
        }
        mEnabled = enabled;
    }
    if (mEnabled == 0) {
        return;
    }
    switch (m54) {
    case 1:
    case 2:
        if (mNpc != 0) {
            int control = mNpc->m55C;
            int kind = mNpc->m560;
            if (!(control == 6 && kind == 2)) {
                void* routes = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
                atn::Vector3 pos;
                atn::Vector3 rot;
                if (routes != 0 && fn_803CCA84(routes, mNpc, &pos, &rot) != 0) {
                    SetPos_unk(mNpc, pos);
                    SetRot_unk(mNpc, rot);
                }
                mNpc = 0;
            }
        }
        if (m54 == 2) {
            mNear = 0;
            PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
            if (player != 0) {
                if (LenSq_unk(player->mPos - m6C) < 256.0f) {
                    if (lbl_8087F430 != 0) {
                        lbl_8087F430->fn_803750E4(0x1BD);
                    }
                    if (!player->mFlag6 && !player->HasC48_unk() && !player->mFlag31) {
                        lbl_8087F490->Request_unk(15);
                        mNear = 1;
                    }
                }
            }
        }
        break;
    case 3:
        if (mNpc != 0) {
            int control = mNpc->m55C;
            int kind = mNpc->m560;
            if (!(control == 6 && kind == 2)) {
                m54 = 4;
            }
        }
        if (mTimer > 0 && --mTimer <= 0 && lbl_8087F8A0 != 0) {
            for (CharaTask* chara = lbl_8087F8A0->mHead; chara != 0; chara = ((CharaListNode_unk*)chara)->mNext) {
                if (chara->CanAct_unk()) {
                    atn::Vector3 d = chara->mPos - m6C;
                    float len = fn_805F9920(&d);
                    if (!(len >= 256.0f)) {
                        if (!((float)__fabs(len) < 1.1920929e-7f) && len > 4.0f) {
                            PSVECNormalize(&d, &d);
                        } else {
                            d = Back_unk(chara);
                        }
                        chara->fn_8016B8D4(d, 1, 0, 0);
                    }
                }
            }
        }
        break;
    }
}

void LottaSummon::vf14() {}

int LottaSummon::vf1C() {
    for (int i = 0; i < 5; i++) {
        if (mEfp[i].IsLoading() != 0) {
            return 1;
        }
    }
    return 0;
}

void LottaSummon::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    int count = 1;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "EFP") == 0 && count < 5) {
            mEfp[count++].LoadRequest(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
}

void LottaSummon::vf24() {}

// inferred: cases 3 and 4 use a second copy of the effect helper that keeps the manager in a local
// (the manager is then loaded before the arguments)
int LottaSummon::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 1:
    case 2: {
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        atn::EfpFile* efp = &mEfp[msg->mType];
        if (lbl_8087F3C0 != 0) {
            PlayEfp_unk(this, efp, 0);
        }
        if (mNpc != 0) {
            mNpc->fn_80155A88(m6C - atn::Vector3(0.0f, 50.0f, 0.0f), 0);
            if (lbl_8087F048 != 0) {
                fn_8010653C(lbl_8087F048, mNpc, &mNpc->mMotion);
            }
        }
        break;
    }
    case 3: {
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        atn::EfpFile* efp = &mEfp[msg->mType];
        EffectMgr_unk* mgr = lbl_8087F3C0;
        if (mgr != 0) {
            PlayEfp2_unk(this, efp, 1);
        }
        if (lbl_8087F3C0 != 0) {
            PlayEfp2_unk(this, &mEfp[4], 0);
        }
        if (mDesc != 0) {
            mNpc = NpcManager::sInstance != 0 ? NpcManager::sInstance->Find_unk(mDesc->m20) : 0;
            if (mNpc != 0) {
                mNpc->mPos = m6C - atn::Vector3(0.0f, 50.0f, 0.0f);
                mNpc->mRot = m78;
                mNpc->mMotion.fn_80097C08(0, 0x153, 1, 0, 1, 0.0f, 0.1f);
                mNpc->fn_80155A88(m6C, 0);
                if (lbl_8087F048 != 0) {
                    fn_8010653C(lbl_8087F048, mNpc, &mNpc->mMotion);
                }
                mTimer = 5;
            }
        }
        break;
    }
    case 4: {
        if (lbl_8087F3C0 != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        }
        atn::EfpFile* efp = &mEfp[msg->mType];
        EffectMgr_unk* mgr = lbl_8087F3C0;
        if (mgr != 0) {
            PlayEfp2_unk(this, efp, 1);
        }
        if (mNpc != 0) {
            mNpc->mPos = m6C;
            mNpc->mRot = m78;
        }
        break;
    }
    }
    return m54 = msg->mType;
}

void LottaSummon::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
}

void LottaSummon::vf6C(void* menu) {
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
}

int LottaSummon::vf60() {
    int ret = 0;
    if (m54 == 2 && mEnabled != 0 && mNear != 0) {
        ret = 1;
    }
    return ret;
}

} // namespace gmk
