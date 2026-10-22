#include "game/EnemyTasha.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" int rand();
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: debug menu entries (as in game/EnemyTrista.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
// inferred: work area of the damage computation (as in game/EnemyTask.cpp)
extern "C" char lbl_807C6B90[0x18];
// inferred: damage of `hit` from `attacker` to `target` (game/DamageCalc_unk.cpp)
extern "C" int fn_8003F538(HitInfo_unk* hit, CharaStatus* attacker, CharaStatus* target, int flags, void* work);

// inferred: effect manager (class unknown), global instance at 0x8087F3C0; mB8 is a mode used while an
// effect is created (as in game/Arms.cpp)
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
Effect_unk* fn_8013A194();
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in game/EnemyJill.cpp)
// inferred: the bullet of `owner` that hits `chara` (index, negative if none)
extern "C" int fn_800FBA9C(Effect_unk* mgr, CharaTask* chara, CharaTask* owner, HitInfo_unk* hit, int arg,
                           float f);
// inferred: skill effect manager (lbl_8087F9E8; weak copy 0x80356FFC is its accessor)
struct SkillEffectManager;
extern "C" SkillEffectManager* lbl_8087F9E8;
extern "C" void fn_8059C2A8(SkillEffectManager* mgr, CharaTask* owner, HitInfo_unk* skill); // inferred: stops the skill

// inferred: collision world and its ray test (as in game/EnemyTrista.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    unsigned char m10[0x34 - 0x10];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test

// inferred: reaction control created by vf6C (class unknown, 0x24 bytes; as in game/EnemyTask.cpp)
struct EnemyReaction_unk {
    EnemyReaction_unk(CharaTask* chara, const atn::Vector3* dir, int kind); // 0x801AFAC0
    unsigned char m00[0x24];
};

// NONMATCHING: draft
EnemyTasha::EnemyTasha(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14DC(0), m14E0(3), m14E4(1), m14E8(300.0f), m14F0(0.0f),
      m14F4(0.0f), m14F8(0.0f), m14FC(60.0f), m1518(40), m151C(20), m1520(200), m1524(40.0f), m1528(50.0f),
      m152C(60.0f), m1530(120.0f), m1540(80), m1544(0), m1545(0), m1548(60.0f), m154C(0), m1550(0), m1554(0),
      m1558(20.0f), m155C(0), m1560(0), m1564(0), m1568(0), m156C(0), m1570(0), m1574(0), m1584(0), m162C(-1),
      m1630(-1), m16B8(0), m16BC(0), m16C0(1.2f), m16C4(-1) {
    mFlag22 = 1;
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_enemy_tasha.csv\0");
    }
    m14B4.vf0C(path);
    m1634.fn_8023780C("/effect/ef_bs007a.efp");
    m1640.fn_8023780C("/effect/ef_sp007d.efp");
    m164C.fn_8023780C("/effect/ef_cm006.efp");
    m1658.fn_8023780C("/effect/ef_ev027.efp");
}

int EnemyTasha::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1640.fn_80237874() || m1634.fn_80237874() || m164C.fn_80237874() || m1658.fn_80237874()) {
        return 0;
    }
    mStatus.m18 |= 0x100;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0x1;
    mStatus.m18 |= 0x1000000;
    fn_80355DD8((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    vfFC();
    return 1;
}

void EnemyTasha::fn_80355DD8(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "NormalAttack") == 0) {
            m155C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "CrossAttack") == 0) {
            m1560 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "JumpAttack") == 0) {
            m1564 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "CounterAttack") == 0) {
            m1568 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "SwordDanceAttackBegin") == 0) {
            m156C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "SwordDanceAttackMiddle") == 0) {
            m1570 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "SwordDanceAttackFinish") == 0) {
            m1574 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "EventDamageSkillID") == 0) {
            m1584 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "StanceSkill") == 0) {
            m1578[0] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
            m1578[1] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
            m1578[2] = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WaitFrame") == 0) {
            m1518 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "CounterWaitFrame") == 0) {
            m1540 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackDistance") == 0) {
            m14FC = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "JumpAttackDistance") == 0) {
            m1524 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "WarpRadius") == 0) {
            m1528 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "WarpAngleMin") == 0) {
            m152C = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "WarpAngleMax") == 0) {
            m1530 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackChanceFrame") == 0) {
            m151C = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(key, "ChaseCancelFrame") == 0) {
            m1520 = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(key, "Event") == 0) {
            TashaEvent_unk e;
            e.m00.mIndex = atoi(csv.fn_8005B9CC());
            e.m08.mIndex = atoi(csv.fn_8005B9CC());
            e.m00.mGlobal = 0;
            e.m08.mGlobal = 0;
            e.m10 = fn_800DC288(csv.fn_8005B9CC());
            m1588.PushBack_unk(e);
        } else if (strcmp(key, "ProvokeNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1664, &csv);
        } else if (strcmp(key, "FullGuardActNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m166C, &csv);
        } else if (strcmp(key, "FullGuardHitNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1674, &csv);
        } else if (strcmp(key, "FullGuardBreakNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m167C, &csv);
        } else if (strcmp(key, "HasteActNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1684, &csv);
        } else if (strcmp(key, "HasteHitNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m168C, &csv);
        } else if (strcmp(key, "HasteBreakNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1694, &csv);
        } else if (strcmp(key, "HasteStateNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m169C, &csv);
        } else if (strcmp(key, "PowerHitActNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m16A4, &csv);
        } else if (strcmp(key, "PowerHitHitNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m16AC, &csv);
        }
    } while (csv.fn_8005BBF8());
}

// inferred: stops the effect of the current state (names invented)
inline void EnemyTasha::StopEffect_unk() {
    switch (m58C) {
    case 11:
        fn_80239DAC(lbl_8087F3C0, this, 1000, 1);
        break;
    case 14:
        fn_80239DAC(lbl_8087F3C0, this, 5, 1);
        break;
    case 17:
        fn_80239DAC(lbl_8087F3C0, this, 0, 1);
        break;
    }
}

// inferred: hit point ratio (as CharaTask::GetHpRate)
inline float EnemyTasha::HpLimit_unk(int index) {
    return (float)mStatus.mParamB.m84 * (m1588.mData[index].m10 - 0.01f);
}

// inferred: fires the event damage skill in front of the target (as EnemyTrista::FireEvent_unk)
inline void EnemyTasha::FireEvent_unk() {
    if (m1584 == 0) {
        return;
    }
    CharaTask* target = mC58.mC4;
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', target->mRot.y);
    PSMTXMultVec(m, &dir, &dir);
    Effect_unk* mgr = lbl_8087F048;
    atn::Vector3 pos = mC58.mC4->mPos + dir;
    fn_800FAB80(mgr, this, m1584, fn_800F8548(mgr), &pos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
}

inline bool EnemyTasha::IsHaste_unk() {
    if (m14E0 == 2 && m58C != 14) {
        return true;
    }
    return false;
}

// inferred: the target is attacking with a bullet that reaches this enemy
inline bool EnemyTasha::IsCountered_unk(CharaTask* target) {
    if (target->IsActive_unk() && target->m55C == 6 && (target->vf78() || target->fn_80158CA4())) {
        if (fn_800FBA9C(lbl_8087F048, target, this, target->m638, -1, 0.0f) >= 0) {
            return true;
        }
    }
    return false;
}

// NONMATCHING: 99.97% - r29/r30 of the effect manager in the event damage (the original keeps the event
// pointer alive in r30)
void EnemyTasha::vf10() {
    if (mC58.mC0 == 0) {
        m54C |= 0x2000;
    } else {
        m54C &= ~0x2000;
    }
    if (mC58.mC0 == 0) {
        CharaTask* leader = lbl_8087F8A0 != 0 ? (CharaTask*)lbl_8087F8A0->mHead : 0;
        if (leader != 0 && leader->m55C == 5) {
            fn_805A4A1C((SpecialFlag_unk*)&m1664, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m166C, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m1674, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m167C, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m1684, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m168C, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m1694, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m169C, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m16A4, 0);
            fn_805A4A1C((SpecialFlag_unk*)&m16AC, 0);
            m14D4 = 0;
            m14E0 = 3;
            m1630 = -1;
        }
    }
    if (CanAct_unk() && m58C != 6 && mC58.mC0 == 0) {
        vf100(6);
    }
    if (m162C >= 0) {
        TashaEvent_unk* e = &m1588.mData[m162C];
        if (fn_805A49E4((SpecialFlag_unk*)&e->m00) != 1) {
            if (fn_805A49E4((SpecialFlag_unk*)&e->m08) == 1) {
                m1630 = m162C;
                m14D4++;
                mStatus.m04 -= 0.01f * mStatus.mParamB.m84;
            } else {
                mStatus.m04 += 0.05f * mStatus.mParamB.m84;
                FireEvent_unk();
            }
            m162C = -1;
            vf100(6);
            m14E0 = 3;
            m14E8 = 0.0f;
        }
    }
    if (m14DC != m55C) {
        m14DC = m55C;
        if (m55C == 6 && m564 == 2) {
            if (m14D8 == 14) {
                fn_80239DAC(lbl_8087F3C0, this, 5, 1);
            }
            if (mMotion.mTracks[0].m00 == 70) {
                m1545 = 1;
            }
        }
    }
    if (m1545 != 0 && !mControl) {
        fn_8015ECC4();
        float y = mRot.y;
        y += 0.863938f;
        mRot.y = atn::WrapAngle_unk(y);
        m1545 = 0;
    }
    CharaTask* leader = (CharaTask*)lbl_8087F8A0->mHead;
    mC58.mC4 = leader;
    m14EC = leader;
    if (IsHaste_unk()) {
        if (IsCountered_unk(mC58.mC4)) {
            StopEffect_unk();
            vf100(17);
        }
    }
    vf104();
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
    if (fn_8035AB78()) {
        unsigned int i = 0;
        TashaEvent_unk* e = m1588.mData;
        for (; i < m1588.mCount; i++) {
            if (fn_805A49E4((SpecialFlag_unk*)&e->m00) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&e->m00, 1);
                m162C = i;
                break;
            }
            e++;
        }
    }
    if (m14E0 == 2) {
        fn_805A4A1C((SpecialFlag_unk*)&m169C, 1);
    } else {
        fn_805A4A1C((SpecialFlag_unk*)&m169C, 0);
    }
    if (m1550 > 0) {
        m1550--;
    }
}

// inferred: the attack states 11-13 and 15 (name invented)
// inferred: state accessor (name invented)
inline int GetStateOf_unk(SpecialEnemyBase* e) {
    return e->m58C;
}

inline bool EnemyTasha::IsAttacking_unk() {
    if (m58C - 11U <= 2 || m58C == 15) {
        return true;
    }
    return false;
}

int EnemyTasha::vf10C(HitMsg_unk* msg) {
    if (m58C == 14) {
        return 0;
    }
    CharaTask* attacker = msg->mAttacker;
    atn::Vector3 rot = attacker->mPos - mPos;
    rot = rot.GetNormalized_unk().ToRotation_unk();
    atn::WrapAngle_unk(mRot.y - rot.y);
    if (mC58.mC4 == 0) {
        mC58.mC4 = msg->mAttacker;
    }
    switch (m14E0) {
    case 1:
        if ((msg->mData->m90 & 0x80000) == 0x80000) {
            m14E0 = 3;
            m14E8 = 300.0f;
            fn_8059C2A8(lbl_8087F9E8, this, m1578[1]);
            if (fn_805A49E4((SpecialFlag_unk*)&m167C) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m167C, 1);
            }
        } else {
            msg->m48 = 1;
            if (fn_805A49E4((SpecialFlag_unk*)&m1674) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m1674, 1);
            }
        }
        break;
    case 2: {
        HitInfo_unk* data = msg->mData;
        if (data != 0 && data->m04 - 0x1F7U <= 2) {
            return 0;
        }
        unsigned int f = data->m90;
        if ((f & 8) == 8 || (f & 0x200) == 0x200 || (f & 0x800000) == 0x800000) {
            m14E0 = 3;
            m14E8 = 300.0f;
            fn_8059C2A8(lbl_8087F9E8, this, m1578[2]);
            if (fn_805A49E4((SpecialFlag_unk*)&m1694) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m1694, 1);
            }
        } else {
            StopEffect_unk();
            vf100(17);
            return 0;
        }
        break;
    }
    }
    // inferred: the original reloads the state after the range test (accessor)
    if ((IsAttacking_unk() && !(GetStateOf_unk(this) == 13 && m14BC == 1) && msg->mAttacker->m560 != 10 &&
         GetStateOf_unk(this) != 14) ||
        m1545 != 0) {
        msg->m10 *= 0.0f;
    }
    if (m58C == 16) {
        msg->m10 *= 0.0f;
    }
    return 1;
}

extern "C" SkillEffectManager* fn_80356FFC() {
    return lbl_8087F9E8;
}

// inferred: keeps the hit points above the threshold of the next event (name invented)
inline void EnemyTasha::ClampHp_unk() {
    int next = m1630 + 1;
    if (next < (int)m1588.mCount) {
        float min = (float)mStatus.mParamB.m84 * (m1588.mData[next].m10 - 0.01f);
        if (mStatus.m04 < min) {
            mStatus.m04 = min;
        }
    }
}

void EnemyTasha::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (m14D4 < 2) {
            ClampHp_unk();
            mStatus.fn_8013322C(0x20);
        } else {
            vf100(2);
            atn::Vector3 dir = msg->m28 * 6.0f;
            fn_8015E7A0(dir, -1, 0);
        }
    } else {
        if (!msg->m10.IsZero_unk() || m1544 != 0) {
            m1550 = 60;
            switch (m14E0) {
            case 3:
            case 0:
                if (++m1554 >= 3) {
                    m1554 = 0;
                    vf100(16);
                    fn_8016E970(2);
                } else {
                    vf100(6);
                }
                break;
            case 1:
                if (++m1554 >= 1 && msg->m44 == 0) {
                    msg->mAttacker->vf7C(1);
                    m1554 = 0;
                    m1544 = 1;
                }
                vf100(m1544 != 0 ? 15 : 6);
                m1544 = 0;
                break;
            default:
                m1554 = 0;
                vf100(6);
                break;
            }
            if (m55C == 6 && m560 == 23) {
                atn::Vector3 dir = m6B8 * 0.5f;
                fn_8015E4B0(dir, -1);
            }
        }
    }
    ClampHp_unk();
}

void EnemyTasha::fn_80357330() {
    CharaTask* leader = (CharaTask*)lbl_8087F8A0->mHead;
    mC58.mC4 = leader;
    m14EC = leader;
}

// inferred: angle from `from` toward `to` by `t` along the shortest way (as in game/EnemyJill.cpp)
inline float LerpAngle_unk(float to, float from, float t) {
    float d = to - from;
    if (d > 3.1415927f) {
        return t * (d - 6.2831855f) + from;
    } else if (d < -3.1415927f) {
        return t * (6.2831855f + d) + from;
    }
    return t * d + from;
}

int EnemyTasha::fn_8035738C(float rate, float limit) {
    if (mC58.mC4 != 0) {
        atn::Vector3 dir = mC58.mC4->mPos - mPos;
        if (!(atn::Abs_unk(fn_805F9920(&dir)) < 1.1920929e-7f)) {
            PSVECNormalize(&dir, &dir);
        }
        atn::Vector3 rot = dir.ToRotation_unk();
        float y = LerpAngle_unk(rot.y, mRot.y, rate);
        float d = atn::WrapAngle_unk(y - mRot.y);
        mRot.y = y;
        atn::WrapAngle_unk(mRot.y);
        if (atn::Abs_unk(57.295776f * d) >= limit) {
            return 1;
        }
    }
    return 0;
}

// inferred: front direction for a rotation around y (name invented, as in game/EnemyTrista.cpp)
inline atn::Vector3 FrontDir_unk(float rotY) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', rotY);
    PSMTXMultVec(m, &dir, &dir);
    return dir;
}

// NONMATCHING: 97.18% - FPR numbering of the loop constants and stack slots of the front/rotation temporaries
void EnemyTasha::fn_803576F8(float radius) {
    atn::Vector3 front(0.0f, 0.0f, 0.0f);
    atn::Vector3 center = mC58.mC4->mPos;
    atn::Vector3 diff = center - mPos;
    int count = 0;
    while (true) {
        if (count++ == 10) {
            m1534 = mPos + FrontDir_unk(mRot.y);
            return;
        }
        float sign = rand() % 2 == 0 ? 1.0f : -1.0f;
        float angle = sign * atn::DegToRad_unk(m152C + (m1530 - m152C) * ((float)(rand() % 1001) / 1000.0f));
        if (!(atn::Abs_unk(fn_805F9920(&diff)) < 1.1920929e-7f)) {
            front = diff.GetNormalized_unk();
        }
        Mtx m;
        PSMTXRotRad(m, 'y', angle);
        PSMTXMultVec(m, &front, &front);
        m1534 = center + front * radius;
        void* colli = lbl_8087EE98;
        if (fn_8004ED34(colli, 0, &center, &m1534, fn_80179D44() | 0x80000000, 0, 0) == 0) {
            return;
        }
    }
}

void EnemyTasha::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

// inferred: damage result of fn_8003EA3C (as DamageResult_unk of game/Em002Skelton.cpp; 0x80357F04 is
// its inline constructor); names invented
struct TashaDamage_unk {
    TashaDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlags31_28 = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags31_28 : 4;
    unsigned int mFlags27_0 : 28;
};

// inferred: state block of SpecialEnemyBase reset by vf100 (0x80357ED8 is its inline reset); names
// invented
struct TashaStep_unk {
    void Reset_unk() {
        m00 = 0;
        m04 = 0;
        m08 = 0;
        m0C = 0;
        mFlags31_28 = 0;
        mFlag27 = 1;
            m14 = 0;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    unsigned int mFlags31_28 : 4;
    unsigned int mFlag27 : 1;
    unsigned int mFlags26_0 : 27;
    int m14;
};

// inferred: sets the frame of motion track `track` (weak copy of a Model_unk inline, also called by
// game/GameTask and game/TutorialTask; defined after vf100 to keep its position)
extern "C" void fn_80357F38(Model_unk* model, int track, float frame);

void EnemyTasha::fn_8035ACB8(int motion, int arg, float speed) {
    mMotion.mTracks[0].m0C = speed;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, motion, arg, 0, 1, 0.0f, 0.1f);
}

int EnemyTasha::fn_8035A9F4() {
    CharaTask* target = mC58.mC4;
    atn::Vector3 dir(target->mPos.x - mPos.x, target->mPos.y, target->mPos.z - mPos.z);
    dir.y = 0.0f;
    float dist = PSVECMag(&dir);
    if (dist > 12.0f) {
        PSVECNormalize(&dir, &dir);
        CollResult_unk result;
        atn::Vector3 from = mPos;
        from.y += 5.0f;
        atn::Vector3 to = from;
        to += dir * (dist - 12.0f);
        if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) == 0) {
            m150C = mPos;
            m1500 = to;
            m1500.y = mPos.y;
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 90.79% - the original keeps the byte offset of the event as its own induction variable and
// adds `this` again in every iteration (r29/r30/r31); ~10 forms of the loop
int EnemyTasha::fn_8035AB78() {
    if (m162C >= 0) {
        return 0;
    }
    if (mC58.mC4 == 0) {
        return 0;
    }
    if (mC58.mC4->m55C != 6 || !mC58.mC4->vf78()) {
        return 0;
    }
    if (mC58.mC4->m638 == 0) {
        return 0;
    }
    float ratio = mStatus.m04 / (float)mStatus.mParamB.m84;
    for (int i = m1588.mCount - 1; i >= 0; i--) {
        if (fn_805A49E4((SpecialFlag_unk*)&m1588[i].m00) == 0) {
            if (ratio <= m1588[i].m10) {
                return 1;
            }
        }
    }
    return 0;
}

// inferred: identity rotated by z, y and x (as ChaserRotZYX_unk of game/ChaserNpc.cpp)
inline void TashaRotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// inferred: plays an effect on the model (as PlayEffect_unk of game/EnemyTrista.cpp)
inline void TashaPlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

void EnemyTasha::fn_8035ACF0(int state) {
    switch (state) {
    case 11: {
        atn::Vector3 ofs(0.0f, 0.0f, 10.0f);
        atn::Matrix3x4 m;
        TashaRotZYX_unk(m, mRot);
        PSMTXMultVec(m.m, &ofs, &ofs);
        fn_80232B7C(this, 1000);
        atn::Vector3 pos = mPos + ofs;
        Color4f_unk color;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &m164C, 0, 0, pos, mRot, color, -1, -1, 1, 1.5f);
        break;
    }
    case 14:
        fn_80232B7C(this, 5);
        lbl_8087F3C0->mB8 = 2;
        TashaPlayEffect_unk(&m1640, &mMotion);
        lbl_8087F3C0->mB8 = 0;
        break;
    case 17:
        fn_80232B7C(this, 0);
        TashaPlayEffect_unk(&m1634, &mMotion);
        break;
    }
}

void EnemyTasha::vf30(void* menu) {
    void* group = m16B4.Load("EnemyTasha");
    fn_805A5220(group);
    fn_80087994(group, "DebugDisp", &m16B8, 0, 0);
    fn_80087994(group, "DebugLog", &m16BC, 0, 0);
    fn_800874C8(group, "ActionMode", &m58C, 0, 100000, 1, 0, 0);
    fn_800874C8(group, "AttackTest", &m16C4, -1, 1000, 1, 0, 0);
    fn_800874C8(group, "Phase", &m14D4, 0, 2, 1, 0, 0);
    fn_800874C8(group, "WaitFrame", &m1518, 0, 500, 1, 0, 0);
    fn_800874C8(group, "ChaseCancelFrame", &m1520, 0, 500, 1, 0, 0);
    fn_800874C8(group, "AttackChanceFrame", &m151C, 0, 500, 1, 0, 0);
    fn_8008771C(group, "AttackDistance", &m14FC, 0, 0, 0.0f, 500.0f, 1.0f);
    fn_8008771C(group, "BackstepDisRate", &m16C0, 0, 0, 0.0f, 500.0f, 0.1f);
    fn_8008771C(group, "HP", &mStatus.m04, 0, 0, 0.0f, 50000.0f, 1.0f);
}

// inferred: state setter (weak copy 0x802657FC of game/Em201ZanGurgFinal.cpp)
extern "C" void fn_802657FC(CharaTask* chara, int state);
// inferred: parameters and result of fn_8003EA3C (as in game/Em002Skelton.cpp)
extern "C" void fn_8003EA3C(TashaDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const void* param, int arg, int arg2); // inferred: computes the damage
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track

void EnemyTasha::vf100(int state) {
    // inferred: m14BC-m14D0 of SpecialEnemyBase reset as one block (weak copy 0x80357ED8)
    ((TashaStep_unk*)&m14BC)->Reset_unk();
    m14D8 = state;
    fn_802657FC(this, state);
    switch (m58C) {
    case 6:
        mFlag30 = 1;
        break;
    case 7:
        fn_8013C460(1);
        fn_8035ACB8(0x1C7, 1, 0.0f);
        break;
    case 8:
        fn_8013C460(0);
        fn_80357330();
        mC58.m0C.fn_80128930(m14EC, &mPos, 0);
        mC58.m0C.mRadius = 30.0f;
        mC58.m0C.fn_801255C8();
        mFlag30 = 1;
        break;
    case 9:
        fn_8013C460(0);
        fn_8035738C(1.0f, 0.0f);
        fn_8035ACB8(0x156, 0, 1.0f);
        break;
    case 10:
        fn_8013C460(0);
        fn_8035738C(1.0f, 0.0f);
        fn_8035ACB8(0x13F, 0, 1.0f);
        break;
    case 11:
        fn_8013C460(0);
        fn_8035738C(1.0f, 0.0f);
        fn_8035ACB8(0x140, 0, 0.8f);
        break;
    case 12:
        fn_8013C460(0);
        fn_8035738C(1.0f, 0.0f);
        m598 = 0;
        fn_8035ACB8(0x157, 0, 1.0f);
        break;
    case 14: {
        m14E0 = m14E4;
        m14E8 = 600.0f;
        m14E4 = (m14E4 + 1) % 3;
        fn_8059C2A8(fn_80356FFC(), this, m1578[0]);
        fn_8059C2A8(fn_80356FFC(), this, m1578[1]);
        fn_8059C2A8(fn_80356FFC(), this, m1578[2]);
        TashaDamage_unk result;
        fn_8003EA3C(&result, m1578[m14E0], this, this, 0, lbl_807C6B90, 0, 0);
        if (m14E0 == 0) {
            if (fn_805A49E4((SpecialFlag_unk*)&m16A4) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m16A4, 1);
            }
        } else if (m14E0 == 1) {
            if (fn_805A49E4((SpecialFlag_unk*)&m166C) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m166C, 1);
            }
        } else if (m14E0 == 2) {
            if (fn_805A49E4((SpecialFlag_unk*)&m1684) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m1684, 1);
            }
        }
        fn_8013C460(0);
        fn_8035ACB8(0x173, 0, 1.0f);
        break;
    }
    case 15:
        fn_8016E970(2);
        fn_8013C460(0);
        fn_8035738C(1.0f, 0.0f);
        m590 = fn_800F8548(fn_8013A194());
        fn_8035ACB8(0x1D2, 0, 1.0f);
        break;
    case 13:
        fn_8013C460(0);
        fn_8035738C(0.3f, 0.0f);
        m598 = 0;
        fn_8035ACB8(0x15C, 0, 1.0f);
        break;
    case 16:
        if (m1550 > 0) {
            m1558 = 20.0f * m16C0;
        } else {
            m1558 = 20.0f;
        }
        fn_8035738C(1.0f, 0.0f);
        fn_8035ACB8(0x1EA, 0, 0.6f);
        fn_803576F8(30.0f);
        mRot.y = (m1534 - mPos).ToRotation_unk().y;
        break;
    case 17:
        if (fn_805A49E4((SpecialFlag_unk*)&m168C) == 0) {
            fn_805A4A1C((SpecialFlag_unk*)&m168C, 1);
        }
        fn_80097CCC(&mMotion, 0, 0.0f);
        fn_8035ACB8(0x1EA, 0, 1.0f);
        fn_8035ACF0(m58C);
        fn_803576F8(m1528);
        break;
    case 18:
        fn_8013C460(0);
        fn_8035738C(1.0f, 0.0f);
        fn_8035ACB8(0x14A, 0, 1.0f);
        fn_80357F38(&mMotion, 0, 10.0f);
        break;
    case 2:
        vf44();
        break;
    }
}

extern "C" void fn_80357F38(Model_unk* model, int track, float frame) {
    model->mTracks[track].m08 = frame;
}

int EnemyTasha::vf68(CharaTask* attacker) {
    int ok = 1;
    if (m55C == 6) {
        switch (m560) {
        case 23:
        case 27:
        case 28:
        case 35:
        case 36:
        case 59:
        case 113:
        case 123:
        case 124:
        case 125:
        case 126:
        case 127:
        case 142:
            ok = 0;
            break;
        }
    }
    if (m14E0 == 2) {
        ok = 0;
    }
    if (ok) {
        HitInfo_unk* hit = fn_80219E6C(200);
        int damage = fn_8003F538(hit, &attacker->mStatus, &mStatus, 0x80, lbl_807C6B90);
        if (mStatus.m04 - damage <= 0.0f) {
            return 10;
        }
    }
    return 0;
}

void EnemyTasha::vf6C(int kind, CharaTask* other) {
    atn::Vector3 dir(mPos.x - other->mPos.x, mPos.y, mPos.z - other->mPos.z);
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);
    if (kind >= 9 && kind <= 12) {
        fn_80178208((chr::ControlBase*)new (3, "", "", 0) EnemyReaction_unk(this, &dir, kind));
        m58C = 6;
    }
}

EnemyTasha::~EnemyTasha() {}
