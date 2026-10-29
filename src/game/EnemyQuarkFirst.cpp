#include "game/EnemyQuarkFirst.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/CharaStatus.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: field (lbl_8087F430): camera follow state cleared by vf10 (names invented)
struct Field_unk {
    unsigned char m000[0x4D8];
    int m4D8;
    unsigned char m4DC[0x8A0 - 0x4DC];
    CharaTask* m8A0; // inferred: character the camera follows (as in game/Em302Kraken.cpp)
    unsigned char m8A4[0x97C - 0x8A4];
    unsigned char m97C;
};
extern "C" Field_unk* lbl_8087F430;
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: skill effect manager (lbl_8087F9E8); fn_8059C2A8 shows the skill of a character
struct SkillEffect_unk;
extern "C" SkillEffect_unk* lbl_8087F9E8;
extern "C" void fn_8059C2A8(SkillEffect_unk* mgr, CharaTask* chara, HitInfo_unk* skill);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: collision world and its ray test (see game/ChaserNpc.cpp)
extern "C" void* lbl_8087EE98;
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x34];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7);
// inferred: debug menu entries (as in game/Em315Coffin.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, float min, float max, float step,
                            int arg6, int arg7);
// inferred: constructor of chr::ControlAttackLastOne (chr/ControlAttackBase.cpp, 0x38 bytes; it stores
// __vt__Q23chr20ControlAttackLastOne); only called here and by BattleCharaTask
extern "C" chr::ControlBase* fn_8018E438(void* self, CharaTask* chara, const atn::Vector3* dir, CharaTask* target,
                                         int type, HitInfo_unk* skill);

EnemyQuarkFirst::EnemyQuarkFirst(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14D8(2), m14DC(1), m14E0(300.0f), m14E8(60.0f), m1504(40),
      m1508(200), m150C(80), m1510(0), m1518(0), m151C(0), m1540(0), m156C(), m1570(0), m1574(0),
      m1578(0), m157C(0), m1580(0) {
    mFlag22 = 1;
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        sprintf(path, "/database/chara/special/%s", name);
    } else {
        sprintf(path, "/database/chara/special/db_enemy_quark_first.csv");
    }
    m14B4.vf0C(path);
    m1528.LoadRequest("ef_bs007a.efp");
    m1534.LoadRequest("ef_bu047a.efp");
    mFlagB11 = 1;
    mFlag24 = 0;
}

int EnemyQuarkFirst::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1528.IsLoading() != 0 || m1534.IsLoading() != 0) {
        return 0;
    }
    fn_80334FDC((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    mStatus.m18 |= 0x140;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0x209;
    mStatus.m18 |= 0x3800000;
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    vfFC();
    mStatus.fn_80134674(2);
    mStatus.fn_80134674(3);
    mStatus.m2D0[3] = CharaStatusPair_unk(m151C->m04);
    mStatus.m2FC = 2.0f;
    return 1;
}

void EnemyQuarkFirst::fn_80334FDC(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "Manamia") == 0) {
            CharaManager<PlayerTask>* players = lbl_8087F8A0;
            m1580 = players->Find_unk(fn_800DC12C(csv.fn_8005B9CC()));
        } else if (strcmp(key, "NormalAttack") == 0) {
            m1510 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "CounterAttack") == 0) {
            m1514 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "PikeSkill") == 0) {
            m1518 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "JumpAttack") == 0) {
            m151C = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "StanceSkill") == 0) {
            m1520 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
            m1524 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "SecondScriptId") == 0) {
            EnemyManager* enemies = EnemyManager::sInstance;
            m1540 = enemies->Find_unk(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "WaitFrame") == 0) {
            m1504 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "CounterWaitFrame") == 0) {
            m150C = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackDistance") == 0) {
            m14E8 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "ChaseCancelFrame") == 0) {
            m1508 = fn_800DC12C(csv.fn_8005B9CC());
        } else if (strcmp(key, "FullGuardActNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1544, &csv);
        } else if (strcmp(key, "FullGuardHitNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m154C, &csv);
        } else if (strcmp(key, "FullGuardBreakNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1554, &csv);
        } else if (strcmp(key, "PowerHitActNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m155C, &csv);
        } else if (strcmp(key, "PowerHitHitNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1564, &csv);
        }
    } while (csv.fn_8005BBF8());
}

void EnemyQuarkFirst::vf10() {
    float rate = mStatus.m04 / (float)mStatus.mParamB.m84;
    if (rate <= 0.4f) {
        m14D4 = 2;
    } else if (rate <= 0.6f) {
        m14D4 = 1;
    } else {
        m14D4 = 0;
    }
    fn_8014C0B4(0, 0x146);
    fn_8014C0B4(1, 0x147);
    mStatus.m2FC = 2.0f;
    if (m1580 != 0) {
        m1580->mStatus.m2FC = 1.5f;
    }
    if (mC58.mC4 == 0) {
        mC58.mC4 = lbl_8087F8A0->mHead;
    }
    if (m55C == 6) {
        switch (m560) {
        case 10:
            if (m58C != 1) {
                m598 = 0;
                m59D = 0;
                m59C = 0;
                m59F = 1;
                m590 = fn_800F8548(lbl_8087F048);
                SetHit_unk(fn_80219E6C(0xE5));
                ((int*)mControl.get())[7] = 1;
                m58C = 1;
            }
            break;
        case 0x8D:
            if (m58C != 1) {
                m58C = 1;
            }
            break;
        case 0x80:
        case 0x14:
            if (m58C != 0) {
                vf100(0);
            }
            break;
        }
    }
    vf104();
    if ((unsigned int)(m58C - 6) <= 1 || m58C == 0 || m58C == 10) {
        if (mC58.mC4 != 0) {
            mLook.fn_80129978(&mC58.mC4->mMotion, "neck", lbl_807C7030, 0.3f);
        }
    } else {
        mLook.fn_80129A48(0.3f);
    }
    if (m157C != 0 && m58C != 8 && m58C != 2) {
        Field_unk* field = lbl_8087F430;
        field->m8A0 = 0;
        field->m4D8 = 0;
        field->m97C = 0;
        m157C = 0;
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
}

int EnemyQuarkFirst::vf10C(HitMsg_unk* msg) {
    if (m58C == 2 || m58C == 10 || (mStatus.m0C & 0x20) == 0x20) {
        return 0;
    }
    int done = 0;
    if (m14D8 == 1) {
        if ((msg->mData->m90.mFlags & 0x80000) == 0x80000) {
            m14D8 = 2;
            m14E0 = 300.0f;
            fn_8059C2A8(lbl_8087F9E8, this, m1524);
            if (fn_805A49E4((SpecialFlag_unk*)&m1554) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m1554, 1);
            }
            done = 1;
        } else {
            if (m58C != 9) {
                msg->m48 = 1;
                done = 1;
                msg->m10 *= 0.01f;
            }
            if (fn_805A49E4((SpecialFlag_unk*)&m154C) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m154C, 1);
            }
        }
    }
    if (!done) {
        msg->m10 *= 0.0f;
    }
    mStatus.fn_80134674(2);
    mStatus.fn_80134674(3);
    return 1;
}

void EnemyQuarkFirst::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
        float k = 5.0f;
        atn::Vector3 push(msg->m28.x * k, 0.0f, msg->m28.z * k);
        m6B8 = push;
        return;
    }
    if (m55C != 6) {
        return;
    }
    if (m560 == 0x17) {
        fn_8015E4B0(m6B8, -1);
    }
    if (m560 == 10 && m58C != 1) {
        m590 = fn_800F8548(lbl_8087F048);
        m594 = 8;
        m59C = 0;
        m59D = 0;
        m59F = 1;
        m59E = 0;
        m5A0 = 1.0f;
        m14E4 = mC58.mC4;
        m598 = 0;
        void* p = ::operator new(0x38, 3, "", "", 0);
        if (p != 0) {
            atn::Vector3 dir(-msg->m28.x, -msg->m28.y, -msg->m28.z);
            p = fn_8018E438(p, this, &dir, m14E4, 10, m1514);
        }
        fn_80178208((chr::ControlBase*)p);
        mMotion.mTracks[0].m08 = 12.0f; // inferred: frame
    }
    if ((unsigned int)(m560 - 0x16) <= 1 || m560 == 10) {
        vf100(0);
    }
}

void EnemyQuarkFirst::vfFC() {
    fn_8016E970(2);
    vf100(0);
}


// NONMATCHING: vf108 (state machine, 5976 bytes) not written yet

extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: game state (lbl_8087EFA8): time scale at 0x3A4 (as in game/Em301LShade.cpp)
struct QuarkGame_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" QuarkGame_unk* lbl_8087EFA8;
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag

// inferred: hit parameters (lbl_807C6B90, as in game/Em302KrakenMp.cpp); only the default is used
struct HitParam_unk {
    int m00[6];
};
extern "C" HitParam_unk lbl_807C6B90; // inferred: default parameters
// inferred: damage result of fn_8003EA3C (0x20 bytes, as in game/Em302KrakenMp.cpp); names invented
struct QuarkDamage_unk {
    QuarkDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1), m18(-1) { mFlags = 0; }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags : 4;
    unsigned int m1C_0 : 28;
};
extern "C" void fn_8003EA3C(QuarkDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const HitParam_unk* param, int arg, int arg2);
extern "C" void* lbl_8087F498; // inferred: camera shake manager
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects

// inferred: turns towards m14E4 by `rate` of the difference (inlined in the states); name invented
inline void EnemyQuarkFirst::TurnTo_unk(float rate) {
    atn::Vector3 dir = m14E4->mPos - mPos;
    if (!((float)__fabs(fn_805F9920(&dir)) < 1.1920929e-7f)) {
        PSVECNormalize(&dir, &dir);
    }
    atn::Vector3 rot = dir.ToRotation_unk();
    rot.z = 0.0f;
    float d = rot.y - mRot.y;
    float y;
    if (d > 3.1415927f) {
        y = rate * (d - 6.2831855f) + mRot.y;
    } else if (d < -3.1415927f) {
        y = rate * (6.2831855f + d) + mRot.y;
    } else {
        y = rate * d + mRot.y;
    }
    atn::WrapAngle_unk(y - mRot.y);
    mRot.y = y;
    atn::WrapAngle_unk(y);
}

// inferred: plays motion `motion` on track 0 (inlined in the states); name invented
inline void EnemyQuarkFirst::PlayMotion_unk(int motion, int loop) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, motion, loop, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 95.34% - draft: the stack slots of the rotation temporaries and the inlined turn
// (TurnTo_unk) differ, and the .sdata2 pool is not in the original order (pi comes after 0.1 here)
void EnemyQuarkFirst::vf100(int state) {
    m14BC = 0;
    m14C0 = 0;
    m14C4 = 0;
    m14C8 = 0;
    mFlag31 = 0;
    mFlag30 = 0;
    mFlag29 = 0;
    mFlag28 = 0;
    mFlag27 = 1;
    m14D0 = 0;
    m58C = state;
    m14E4 = 0;
    switch (state) {
    case 0:
        mFlag30 = 1;
        break;
    case 6:
        mFlag5 = m14D8 == 1;
        PlayMotion_unk(0x146, 1);
        break;
    case 7:
        mFlag5 = 0;
        m14E4 = mC58.mC4;
        mC58.m0C.fn_80128930(m14E4, &mPos, 0);
        mC58.m0C.mRadius = 30.0f;
        mC58.m0C.fn_801255C8();
        mFlag30 = 1;
        break;
    case 1:
        mFlag5 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m594 = 8;
        m598 = 0;
        m59C = 0;
        m59D = 0;
        m59F = 0;
        m5A0 = 1.0f;
        m59E = 0;
        m14E4 = mC58.mC4;
        if (!m14E4->mFlag5 && m14E4->vfC4()) {
            atn::Vector3 dir(m14E4->mPos.x - mPos.x, 0.0f, m14E4->mPos.z - mPos.z);
            dir.Normalize_unk();
            void* p = ::operator new(0x38, 3, "", "", 0);
            if (p != 0) {
                p = fn_8018E438(p, this, &dir, m14E4, 9, m1514);
            }
            fn_80178208((chr::ControlBase*)p);
        } else {
            fn_8015783C(m1510, m14E4->mPos, 0.0f);
        }
        m598 = 0;
        break;
    case 8:
        m14E4 = mC58.mC4;
        mFlag5 = 0;
        if (m14E4 != 0) {
            TurnTo_unk(1.0f);
        }
        fn_8016D74C(m151C, 0, (int)&m14E4->mPos);
        break;
    case 9: {
        int mode = m14DC;
        m14D8 = mode;
        m14E0 = 600.0f;
        m14DC = (mode + 1) % 2;
        if (mode == 1) {
            m14E0 = 300.0f;
        }
        fn_8059C2A8(lbl_8087F9E8, this, m1520);
        fn_8059C2A8(lbl_8087F9E8, this, m1524);
        QuarkDamage_unk result;
        fn_8003EA3C(&result, (&m1520)[m14D8], this, this, 0, &lbl_807C6B90, 0, 0);
        if (m14D8 == 0) {
            if (fn_805A49E4((SpecialFlag_unk*)&m155C) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m155C, 1);
            }
        } else if (m14D8 == 1) {
            mStatus.fn_8013322C(0x80000);
            fn_80168F3C();
            if (fn_805A49E4((SpecialFlag_unk*)&m1544) == 0) {
                fn_805A4A1C((SpecialFlag_unk*)&m1544, 1);
            }
        }
        mFlag5 = 0;
        PlayMotion_unk(0x173, 0);
        break;
    }
    case 10:
        m14E4 = mC58.mC4;
        mC58.m0C.fn_80128930(m14E4, &mPos, 0);
        mC58.m0C.mRadius = 30.0f;
        mC58.m0C.fn_801255C8();
        mFlag30 = 1;
        if (m58C == 10) {
            fn_80232B7C(this, 0);
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
            fn_8023A8B4(lbl_8087F3C0, &m1528, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        }
        break;
    case 11:
        m590 = fn_800F8548(lbl_8087F048);
        m594 = 8;
        m598 = 0;
        m59C = 0;
        m59D = 0;
        m59F = 0;
        m59E = 0;
        m5A0 = 1.0f;
        mFlag5 = 0;
        m14E4 = mC58.mC4;
        if (m14E4 != 0) {
            TurnTo_unk(1.0f);
        }
        PlayMotion_unk(0x14A, 0);
        mMotion.mTracks[0].m08 = 0.0f;
        break;
    case 2:
        vf44();
        fn_8016E970(2);
        PlayMotion_unk(0x36, 1);
        mMotion.mTracks[0].m0C = 1.0f;
        if (lbl_8087F498 != 0) {
            fn_803EA77C(lbl_8087F498, this, 2, 0, 1.0f, 600.0f);
        }
        break;
    }
}

// inferred: a free place around the target to jump to (inlined in vf104): one of eight directions at
// 12 units, tested with a ray; name invented
inline int EnemyQuarkFirst::FindJumpPlace_unk() {
    atn::Vector3 target = mC58.mC4->mPos;
    for (int i = 0; i < 8; i++) {
        atn::Vector3 d(0.0f, 0.0f, 12.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', 3.1415927f * (float)((i + 2) % 8) / 4.0f);
        PSMTXMultVec(m, &d, &d);
        m14EC = atn::Vector3(target.x + d.x, target.y + d.y, target.z + d.z);
        atn::Vector3 from = target;
        from.y += 3.0f;
        atn::Vector3 to = m14EC;
        to.y += 3.0f;
        if (fn_8004ED34(lbl_8087EE98, 0, &from, &to, fn_80179D44() | 0x80000000, 0, 0) == 0) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: ~99% - r23/r24 and r26/r27 swapped in the search of a place around the target (the
// collision world against the loop counter) and r3/r4 of the target distance; 12 variants
void EnemyQuarkFirst::vf104() {
    m14C0++;
    float dt = lbl_8087EFA8->m3A4;
    m14E0 -= dt;
    if (m14E0 <= 0.0f && m55C == 2 && ((unsigned int)(m58C - 6) <= 1 || m58C == 0)) {
        vf100(9);
    }
    atn::Vector3 dir;
    if (mC58.mC4 != 0) {
        dir = mC58.mC4->mPos - mPos;
    }
    switch (m58C) {
    case 0:
        if (vfF8() != 0 && m55C == 2) {
            if (m14D8 == 1) {
                vf100(6);
            } else if (mStatus.m228 >= 1.0f) {
                m14EC.x = 0.0f;
                m14EC.y = 0.0f;
                m14EC.z = 0.0f;
                m1578 = 8;
                vf100(10);
            } else {
                vf100(7);
            }
        }
        vf108();
        break;
    case 6:
        if (mFlag31) {
            if (m14D8 == 1) {
                vf100(6);
            } else {
                vf100(7);
            }
        }
        vf108();
        break;
    case 7:
        if (mC58.mC4 != m14E4) {
            vf100(7);
        } else if (mControl.get() == 0) {
            if (mFlag31) {
                if (fn_805F9920(&dir) < m14E8 * m14E8) {
                    vf100(1);
                } else {
                    vf100(7);
                }
            } else if (m14C0 > m1508) {
                mC58.m0C.fn_80127D8C(0, 0, 0);
                int r = rand() % 100;
                if (fn_803383E4() != 0) {
                    if (r < 60) {
                        vf100(11);
                    } else {
                        m1578 = 7;
                        vf100(10);
                    }
                } else if (FindJumpPlace_unk()) {
                    m1578 = 7;
                    vf100(10);
                } else {
                    vf100(6);
                }
            }
        }
        vf108();
        break;
    case 1:
    case 8:
        if (mFlag31) {
            if (m58C == 10) {
                fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            }
            vf100(6);
            fn_8016E970(2);
        }
        vf108();
        break;
    case 9:
        if (mFlag31) {
            if (m14D8 == 1) {
                vf100(6);
            } else {
                vf100(7);
            }
        }
        vf108();
        break;
    case 10:
        if (mFlag31) {
            if (m58C == 10) {
                fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            }
            vf100(m1578);
        }
        vf108();
        break;
    case 11:
        if (mFlag31) {
            vf100(6);
        }
        vf108();
        break;
    case 2:
        if (mFlag31) {
            fn_80370AE4(lbl_8087F430, 0xCF, 1);
            fn_801765D8();
        }
        vf108();
        break;
    default:
        vf100(0);
        break;
    }
}

int EnemyQuarkFirst::fn_803383E4() {
    CharaTask* target = mC58.mC4;
    atn::Vector3 dir = target->mPos - mPos;
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
            m14F8 = mPos;
            m14EC = to;
            m14EC.y = mPos.y;
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 99.54% - only the offsets in @stringBase0: "SE_DG040_105" (used by vf108, not written
// yet) comes before these strings in the pool
void EnemyQuarkFirst::vf30(void* menu) {
    void* group = m156C.Load("EnemyQuarkFirst");
    fn_805A5220(group);
    fn_80087994(group, "DebugDisp", &m1570, 0, 0);
    fn_80087994(group, "DebugLog", &m1574, 0, 0);
    fn_800874C8(group, "ActionMode", &m58C, 0, 100000, 1, 0, 0);
    fn_800874C8(group, "Phase", &m14D4, 0, 2, 1, 0, 0);
    fn_800874C8(group, "WaitFrame", &m1504, 0, 500, 1, 0, 0);
    fn_800874C8(group, "ChaseCancelFrame", &m1508, 0, 500, 1, 0, 0);
    fn_8008771C(group, "AttackDistance", &m14E8, 0.0f, 500.0f, 1.0f, 0, 0);
    fn_8008771C(group, "HP", &mStatus.m04, 0.0f, 50000.0f, 1.0f, 0, 0);
}

int EnemyQuarkFirst::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    if (m14D8 == 1 && !IsDown_unk()) {
        if (m55C != 6) {
            return 2;
        }
        if (m560 == 10) {
            return 3;
        }
        return 1;
    }
    return 0;
}

int EnemyQuarkFirst::vfA8(int kind) {
    if (kind == 1 && m14D8 == 1 && m58C != 9 && m58C != 1 && !IsDown_unk()) {
        return 3;
    }
    return -1;
}

EnemyQuarkFirst::~EnemyQuarkFirst() {}
