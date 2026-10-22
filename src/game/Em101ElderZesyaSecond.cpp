#include "game/Em101ElderZesyaSecond.h"
#include "game/HitInfo_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: field (lbl_8087F430): camera shake at 0x96C (as in game/GimmickStateCommand.cpp)
struct Field_unk {
    unsigned char m000[0x96C];
    int m96C; // inferred: camera shake
    int m970;
    float m974;
    float m978;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);

// inferred: sound effects of the barrier
static const char* const sSounds_unk[] = {"SE_SP007_001", "SE_SP007_003"};

// inferred: plays an effect on the model (locals in the order of game/Em305Vampire.cpp)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// NONMATCHING: 99.74% - r26/r27 of the second std::string, r25/r26 of the strlen of the parameters and of
// the old m121C (same as Em101LittleZesya / Em101ElderZesya / Em201ZanGurg)
Em101ElderZesyaSecond::Em101ElderZesyaSecond(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14CC(0), m14D0(0), m14D4(0), m14D8(0), m14DC(0), m14E0(0), m1504(0),
      m1554(0), m15CC(1), m15D0(1), m15D4(0), m15D8(0), m15DC(0), m15E4(0) {
    mFlag22 = 1;
    m14BC = 1e16f;
    m14C0.clear();
    m14B8 = 0;
    std::string dir("database/chara/special/");
    std::string name("db_em101_03_00.csv");
    atn::Csv csv(info->m2C, strlen(info->m2C));
    do {
        const char* s = csv.fn_8005B9CC();
        if (s == 0 || strcmp(s, "") == 0) {
            break;
        }
        if (strncmp(s, "db_name=", 8) == 0) {
            name = s + 8;
        }
    } while (csv.fn_8005BBF8());
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    m1518.fn_8023780C("cm002_00.efp");
    m1524.fn_8023780C("ef_sk026a.efp");
    m1530.fn_8023780C("ef_bs007a.efp");
    m153C.fn_8023780C("ef_bs066e.efp");
    m155C.fn_8023780C("ef_bs067a.efp");
    m1568.fn_8023780C("ef_bs067b.efp");
    m1574.fn_8023780C("ef_ui002b.efp");
    m121C.reset(new (1, "", "", 0) atn::EfpHandle_unk());
    m121C->fn_8023780C("ef_bu013.efp");
    m14FC = 0x5E6;
    mFlagB5 = 1;
    m1500 = 0x5EA;
    mFlagB11 = 1;
}

Em101ElderZesyaSecond::~Em101ElderZesyaSecond() {}

void Em101ElderZesyaSecond::vf14() {
    fn_80149A30();
}

void Em101ElderZesyaSecond::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C - 10U <= 1) {
        msg->m90 = 1;
        msg->m84 = 2;
        msg->m68 = 0;
        msg->m8C = 0;
        return;
    }
    if (m1554 != 0) {
        if (msg->mAttacker != 0 && msg->mData != 0) {
            int ok = 0;
            if (m15D4 == 0 && msg->mData->m74.m0C > 0.0f) {
                ok = 1;
            }
            if (m15D4 == 1 && msg->mData->m74.m00 > 0.0f) {
                ok = 1;
            }
            if (ok != 0) {
                if (m15D0 != 0) {
                    if (fn_80370A78(lbl_8087F430, 0xE6) == 0) {
                        fn_80370AE4(lbl_8087F430, 0xE6, 1);
                    }
                    m15D0 = 0;
                }
                fn_8025C624();
                return;
            }
        }
        if (m15CC != 0) {
            fn_80370AE4(lbl_8087F430, 0xE7, 1);
            m15CC = 0;
        }
        msg->m64 = 1;
        msg->m90 = 1;
        msg->m84 = 1;
        msg->m68 = 0;
        msg->m8C = 0;
        CharaTask* attacker = msg->mAttacker;
        if (attacker != 0 && msg->m44 == 0 && attacker->m55C == 6 && attacker->m560 - 4U <= 1) {
            attacker->vf7C(0);
        }
        return;
    }
    if (msg->mAttacker != 0 && msg->mAttacker->mType != 0) {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em101ElderZesyaSecond::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    m15D8++;
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8, -1);
        reset:
            m58C = 0;
            m14D4 = 0;
            m14D8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
            m1504 = m14B8;
            break;
        }
        if (m560 == 30 && msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
            fn_8015E4B0(msg->m28 * 2.0f, -1);
        }
    }
    if (mStatus.IsState_unk(0x20)) {
        if (fn_80370A78(lbl_8087F430, 0x26) > 0 && msg->mAttacker != 0 && msg->mAttacker->mType == 0) {
            vf48();
            fn_80178A6C();
            fn_8015495C(0, 1, 1, 1);
            fn_8016DA4C();
            m58C = 2;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
            fn_800EB7A0(this);
        } else {
            mStatus.fn_8013322C(0x20);
            mStatus.m04 = 1.0f;
            fn_8015E4B0(msg->m28 * 2.0f, -1);
        }
    }
}

void Em101ElderZesyaSecond::fn_8025C624() {
    m1554 = 0;
    fn_80239DAC(lbl_8087F3C0, this, 1001, 0);
    fn_80239DAC(lbl_8087F3C0, this, 1002, 0);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1574, &mMotion);
    fn_800C344C(sSounds_unk[1], &mPos, 0, -1, 1.0f);
    Field_unk* field = lbl_8087F430;
    field->m96C %= 2;
    field->m970 = 5;
    field->m974 = 1.0f;
    field->m978 = 2.0f;
    m15DC++;
    if (m15DC < 2) {
        m15D4 = 0;
    } else if (m15DC < 4) {
        m15D4 = 1;
    } else {
        m15D4 = (m15D4 + 1) % 2;
    }
}

int Em101ElderZesyaSecond::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (m1554 != 0 && hit != 0) {
        int ok = 0;
        if (m15D4 == 0 && hit->m74.m0C > 0.0f) {
            ok = 1;
        }
        if (m15D4 == 1 && hit->m74.m00 > 0.0f) {
            ok = 1;
        }
        if (ok == 0) {
            return 4;
        }
    }
    return 0;
}
