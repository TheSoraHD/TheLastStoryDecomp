#include "game/Em312DarkServant.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/Em006DarkMaster.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int atoi(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;  // inferred: player manager
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_800EAECC(CharaTask* chara); // inferred: BattleCharaTask
extern "C" int rand();
extern "C" float fn_80011220(float x); // inferred: fabs (weak copy of another helper)
// inferred: effect manager methods (as in game/CharaTask.cpp)
struct Effect_unk;
Effect_unk* fn_8013A194();
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_802F0964(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, int arg7, int arg8,
                            float arg9); // inferred: fires a bullet (weak copy, game/Em306GurgBeast.cpp)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

Em312DarkServant::Em312DarkServant(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D4(0), m14D8(1), m14DC(0), m14E0(0), m14E4(50.0f), m14E8(40.0f), m1570(900),
      m1574(1200), m1578(3), m157C(3) {
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em312_00_00.csv\0");
    }
    m14B4.vf0C(path);
    m1580.LoadRequest("/effect/ef_bs007a.efp");
    m158C.LoadRequest("/effect/ef_bs027a.efp");
    m14EC[0].mSkillId = 0x4E2;
    m14EC[0].mCoolTime = 240;
    m14EC[1].mSkillId = 0x4E3;
    m14EC[1].mCoolTime = 120;
    m14EC[2].mSkillId = 0x4E4;
    m14EC[2].mCoolTime = 0;
    m14EC[3].mSkillId = 0x4E5;
    m14EC[3].mCoolTime = 0;
    m152C[0].m00 = 0x13F;
    m152C[0].m04 = 54.0f;
    m152C[0].m08 = -1;
    m152C[0].m0C = 2;
    m152C[1].m00 = 0x142;
    m152C[1].m04 = 52.0f;
    m152C[1].m08 = -1;
    m152C[1].m0C = 1;
    m152C[2].m00 = 0x145;
    m152C[2].m04 = 15.0f;
    m152C[2].m08 = -1;
    m152C[2].m0C = 2;
    m152C[3].m00 = 0x14A;
    m152C[3].m04 = 38.0f;
    m152C[3].m08 = -1;
    m152C[3].m0C = 2;
}

Em312DarkServant::~Em312DarkServant() {}

inline void Em312DarkServant::KnockDown_unk() {
    m14E0 = m157C;
    if (m58C != 13) {
        vf100(13);
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        if ((mStatus.m0C & 0x4000) != 0x4000) {
            mStatus.fn_8013310C(0x4000, 0);
        }
    }
}

int Em312DarkServant::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1580.IsLoading() || m158C.IsLoading()) {
        return 0;
    }
    fn_8030DEB0((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
    mStatus.m18 |= 0x100;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC218;
    mStatus.m18 |= 0x2800000;
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    const char* master = strstr(((EnemyInfo_unk*)m14B0)->m2C, "master=");
    if (master != 0) {
        EnemyManager* mgr = EnemyManager::sInstance;
        if (mgr != 0) {
            EnemyTask* e = mgr->Find_unk(atoi(master + 7));
            if (e->m146C == 6) {
                m14D4 = e;
            }
        }
    }
    vfFC();
    return 1;
}

void Em312DarkServant::fn_8030DEB0(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "MoveRadius") == 0) {
            m14E4 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLength") == 0) {
            m14E8 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "BreathIntervalInit") == 0) {
            m1570 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "BreathInterval") == 0) {
            m1574 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackCountMax") == 0) {
            m1578 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "DamageCountMax") == 0) {
            m157C = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "VertSkillId") == 0) {
            m14EC[0].mSkillId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "VertIntervalFrame") == 0) {
            m14EC[0].mCoolTime = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HorzSkillId") == 0) {
            m14EC[1].mSkillId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HorzIntervalFrame") == 0) {
            m14EC[1].mCoolTime = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackAllSkillId") == 0) {
            m14EC[2].mSkillId = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "BreathSkillId") == 0) {
            m14EC[3].mSkillId = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

void Em312DarkServant::vf10() {
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        vf104();
    }
    fn_8014C540();
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
}

void Em312DarkServant::vf14() {
    if (m14D8 == 0) {
        return;
    }
    fn_80149A30();
}

int Em312DarkServant::vf10C(HitMsg_unk* msg) {
    switch (m58C) {
    case 9:
    case 10:
    case 11:
        msg->m10 *= 0.0f;
        break;
    case 12:
        if (m14C0 >= 25) {
            msg->m10 *= 0.0f;
            break;
        }
    default:
        if (PSVECSquareMag(&msg->m10) > 0.2f) {
            PSVECNormalize(&msg->m10, &msg->m10);
            msg->m10 *= 0.2f;
        }
        break;
    }
    return m14D8;
}

void Em312DarkServant::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
        return;
    }
    if (msg->m44 != 1) {
        m14E0++;
    }
    if ((mStatus.m0C & 0x4000) == 0x4000) {
        m14E0 = m157C;
    }
    // inferred: the original reloads both counters here (volatile reads)
    if (*(volatile int*)&m14E0 >= *(volatile int*)&m157C) {
        KnockDown_unk();

    } else if (m58C == 12 && m14C0 < 25) {
        vf100(6);
    }
}

// inferred: name invented
inline bool HasFlag54C_unk(CharaTask* c, unsigned int flag) {
    return (c->m54C & flag) == flag;
}

extern "C" int fn_8030E490(EnemyTask* servant) {
    return ((Em312DarkServant*)servant)->IsDown_unk();
}

extern "C" void fn_8030E4AC(EnemyTask* servant) {
    ((Em312DarkServant*)servant)->KnockDown_unk();
}

extern "C" void fn_8030E530(EnemyTask* servant) {
    Em312DarkServant* s = (Em312DarkServant*)servant;
    s->m14E0 = 0;
    if ((s->mStatus.m0C & 0x4000) == 0x4000) {
        s->mStatus.fn_8012F440(0x4000, 0, 1, 0, 0, 0, 1);
    }
    // inferred: the original reloads m54C after the test (volatile read through a cast)
    if (s->m14D8 != 0 && (*(volatile unsigned int*)&s->m54C & 0x2000) == 0x2000) {
        s->m14A8 |= 0x04000000;
        s->m54C &= ~0x2000;
    }

}

// NONMATCHING: ~85% - register numbering of the recursion inlined three levels deep (zero and counters;
// recursive inline, documented blocker)
void Em312DarkServant::fn_8030E5C8() {
    if (m14DC != 0) {
        m14DC = 0;
        Em006DarkMaster* master = (Em006DarkMaster*)m14D4;
        if (master != 0) {
            FixedArray_unk<EnemyTask*, 8>* list = &master->m14D8;
            for (unsigned int i = 0; i < list->mCount; i++) {
                ((Em312DarkServant*)list->mData[i])->fn_8030E5C8();
            }
        }
    }
}

void Em312DarkServant::vfFC() {
    fn_8016E970(2);
    vf100(6);
    m156C = m1570;
}

// NONMATCHING: 98.43% - saved registers of the recursion of fn_8030E5C8 inlined three levels deep (documented
// blocker)
void Em312DarkServant::vf100(int state) {
    if (CharaTask::mFlag2) {
        return;
    }
    int prev = m58C;
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
    switch (state) {
    case 6:
        mFlag30 = 1;
        break;
    case 7:
        mFlag30 = 1;
        break;
    case 8:
        mC58.m0C.fn_80128930(mC58.mC4, &mPos, 0);
        mC58.m0C.fn_801255C8();
        mFlag30 = 1;
        break;
    case 9:
    case 10:
    case 11:
    case 12:
        fn_805A4F1C(m152C[state - 9].m00, 0, 1, 1.0f, 0.0f);
        m14EC[m58C - 9].m0C = m14EC[m58C - 9].mCoolTime;
        if (m58C == 11) {
            fn_8030E5C8();
        } else {
            if (m58C == 12) {
                m156C = m1574;
            }
            if (m14D8 != 0) {
                m14DC++;
            }
        }
        mFlag30 = 0;
        break;
    case 13:
        fn_805A4F1C(0x1E1, 0, 1, 1.0f, 0.0f);
        mFlag30 = 0;
        if (m14D8 != 0 && (*(volatile unsigned int*)&m54C & 0x2000) != 0x2000) {
            m14A8 &= ~0x04000000;
            m54C |= 0x2000;
        }
        break;
    case 2:
        if ((mStatus.m0C & 0x20) == 0x20 && prev != 2) {
            vf44();
        }
        mFlag30 = 0;
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
        break;
    }
    if (prev != 7 && m58C == 7) {
        mFC0 = mC58.mC4;
    } else if (prev == 7 && m58C != 7) {
        mFC0 = 0;
    }
}

// inferred: hit point ratio of a character, 1 without maximum (name invented)
inline float ServantHpRatio_unk(CharaTask* c) {
    int max = c->mStatus.mParamB.m84;
    return max > 0 ? c->mStatus.m04 / max : 1.0f;
}

// inferred: plays the effect m1580 at `pos` (name invented)
inline void ServantEffect_unk(Em312DarkServant* s, const atn::Vector3& pos, float scale) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, &s->m1580, &s->mMotion, 0, pos, lbl_807C7030, color, -1, -1, 1, scale);
}

void Em312DarkServant::vf104() {
    m14C0++;
    if (CanAct_unk()) {
        for (int i = 0; i < 4; i++) {
            if (m14EC[i].m0C > 0) {
                m14EC[i].m0C--;
            }
        }
        if (m14D8 != 0 && m156C > 0) {
            m156C--;
        }
        fn_8030F380();
        if (m14D8 == 0 && !fn_80239D58(lbl_8087F3C0, this, 0)) {
            fn_80232B7C(this, 0);
            float scale = 0.75f + (rand() % 1001) / 1000.0f;
            ServantEffect_unk(this, atn::Vector3(0.0f, -(0.5f * m5B4 * ((rand() % 1001) / 1000.0f)), 0.0f), scale);
        }
        if (m58C != 9 && m14D8 == 0 && m14D4 != 0) {
            mC58.mC4 = m14D4->mC58.mC4;
            mC58.mC8 = (int)mC58.mC4;
        }
        if (m14D4 != 0 && (m14D4->mStatus.m0C & 0x20) == 0x20) {
            vf100(2);
        }
    }
    if (CanAct_unk() && m58C != 6 && m58C != 11) {
        if (!vfF8()) {
            vf100(6);
        } else if (mC58.mC4 != (CharaTask*)mC58.mC8) {
            vf100(6);
        }
    }
    switch (m58C) {
    case 6:
        if (m55C == 2 && vfF8()) {
            if (*(volatile int*)&m14E0 >= *(volatile int*)&m157C) {
                vf100(13);
            } else if (m14D4 != 0 && ServantHpRatio_unk(m14D4) < 0.5f) {
                vf100(7);
            } else {
                vf100(8);
            }
        }
        vf108();
        break;
    case 7: {
        if (*(volatile int*)&m14E0 >= *(volatile int*)&m157C) {
            vf100(13);
            break;
        }
        atn::Vector3 d = mC58.mC4->mPos - mPos;
        if (mFlag31 || m14BC == 0 || PSVECSquareMag(&d) < m14E8 * m14E8) {
            if (m156C <= 0 && (mStatus.m0C & 4) != 4) {
                vf100(12);
            } else if (m14DC >= m1578 && m14D8 != 0) {
                vf100(11);
            } else if (fn_8030FEAC(m14E8) >= 2 && m14EC[1].m0C <= 0) {
                vf100(10);
            } else if (m14EC[0].m0C <= 0) {
                vf100(9);
            }
        }
        vf108();
        break;
    }
    case 8: {
        if (*(volatile int*)&m14E0 >= *(volatile int*)&m157C) {
            vf100(13);
            break;
        }
        atn::Vector3 d = mC58.mC4->mPos - mPos;
        if (mFlag31 || PSVECSquareMag(&d) < m14E8 * m14E8) {
            if (m156C <= 0 && (mStatus.m0C & 4) != 4) {
                vf100(12);
            } else if (m14DC >= m1578 && m14D8 != 0) {
                vf100(11);
            } else if (fn_8030FEAC(m14E8) >= 2 && m14EC[1].m0C <= 0) {
                vf100(10);
            } else if (m14EC[0].m0C <= 0) {
                vf100(9);
            }
        } else if (m14D4 != 0 && ServantHpRatio_unk(m14D4) < 0.5f) {
            atn::Vector3 d2 = m14D4->mPos - mPos;
            if (PSVECSquareMag(&d2) > m14E4 * m14E4) {
                vf100(7);
            }
        }
        vf108();
        break;
    }
    case 9:
    case 10:
    case 11:
    case 12:
        if (mFlag31) {
            vf100(6);
        }
        vf108();
        break;
    case 13:
        if (mFlag31) {
            vf100(6);
        }
        vf108();
        break;
    case 2:
        mColli.mFlags &= ~1;
        CharaTask::mFlag25 = 1;
        fn_800EAECC(this);
        break;
    default:
        vf100(6);
        break;
    }
}

void Em312DarkServant::vf108() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mRot;
    int follow = 0;
    if (m55C == 2) {
        switch (m58C) {
        case 6:
            mFlag31 = 1;
            break;
        case 7: {
            follow = 1;
            float near = 0.8f * m14E4;
            float far = 1.2f * m14E4;
            if (m14BC == 0) {
                atn::Vector3 dir;
                if (mC58.mC4 != 0) {
                    dir = *mC58.mC4->fn_8013C38C() - *m14D4->fn_8013C38C();
                } else {
                    dir = mPos - *m14D4->fn_8013C38C();
                }
                far = dir.Length_unk();
                if (far > 0.0f) {
                    dir.Normalize_unk();
                }
                float len = far / 2.0f;
                if (len > near) {
                    len = near;
                }
                atn::Vector3 goal = *m14D4->fn_8013C38C() + dir * len;
                goal.y = mPos.y;
                atn::Vector3 d = goal - mPos;
                speed = d.Length_unk();
                if (speed > 0.0f) {
                    d.Normalize_unk();
                }
                if (speed < 0.1f) {
                    speed = 0.0f;
                    rot = mRot;
                } else if (speed < 10.0f) {
                    speed = 0.0f;
                    rot = d.ToRotation_unk();
                } else if (speed < 40.0f) {
                    rot = d.ToRotation_unk();
                } else if (speed < 70.0f) {
                    rot = d.ToRotation_unk();
                    rate = 3.0f;
                } else {
                    m14BC = 1;
                    mC58.m0C.fn_80128930(m14D4, &mPos, 0);
                    mC58.m0C.fn_801255C8();
                }
            } else if (m14BC == 1) {
                mC58.m0C.fn_80126214();
                atn::Vector3 dir(*mC58.m0C.fn_80139F58());
                if (dir.LengthSq_unk() > 0.0f) {
                    speed = dir.Length_unk();
                    dir.Normalize_unk();
                    rot = dir.ToRotation_unk();
                }
                if (mC58.m0C.fn_8013C3F4() || (mPos - mC58.mDest).LengthSq_unk() < far * far) {
                    m14BC = 0;
                }
                rate = 1.5f;
            }
            if (m14D4 == 0 || !m14D4->CanAct_unk()) {
                m14BC = -1;
                mFlag31 = 1;
            }
            break;
        }
        case 8: {
            mC58.m0C.fn_80126214();
            atn::Vector3 dir(*mC58.m0C.fn_80139F58());
            speed = dir.Length_unk();
            if (speed > 0.0001f) {
                rot = dir.GetNormalize().ToRotation_unk();
            }
            if (mC58.m0C.fn_8013C3F4()) {
                mFlag31 = 1;
            }
            break;
        }
        case 9:
        case 10:
        case 12:
            if (mC58.mC4 != 0) {
                float frame = mMotion.GetMotionFrame(0);
                int idx = m58C - 9;
                ServantAttack_unk* atk = &m14EC[idx];
                ServantRange_unk* range = &m152C[idx];
                atn::Vector3 dir = *mC58.mC4->fn_8013C38C() - mPos;
                if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                }
                if (range->m0C == 0 || m14C0 < 15) {
                    rot = dir.ToRotation_unk();
                }
                if (m14BC == 0) {
                    if (range->m0C == 1) {
                        rot = dir.ToRotation_unk();
                    }
                    if (frame >= range->m04) {
                        int id = range->m08 >= 0 ? range->m08 : fn_800F8548(fn_8013A194());
                        HitInfo_unk* bullet = fn_80219E6C(atk->mSkillId);
                        fn_802F0964(fn_8013A194(), this, bullet, id, 30, -1, 0.0f);
                        m14BC++;
                    }
                } else {
                    float len = mMotion.fn_80097D7C(0);
                    if (mMotion.GetMotionFrame(0) >= len) {
                        mFlag31 = 1;
                    }
                }
            } else {
                mFlag31 = 1;
            }
            break;
        case 11: {
            float frame = mMotion.GetMotionFrame(0);
            int idx = m58C - 9;
            ServantAttack_unk* atk = &m14EC[idx];
            ServantRange_unk* range = &m152C[idx];
            atn::Vector3 dir;
            if (mC58.mC4 != 0) {
                // inferred: the original computes the difference but never stores it (dir stays unset)
                *mC58.mC4->fn_8013C38C() - mPos;
                if (!(fn_80011220(dir.LengthSq_unk()) < 1.1920929e-7f)) {
                    dir.Normalize_unk();
                }
            } else {
                dir = fn_8014052C();
            }
            if (range->m0C == 0 || m14C0 < 15) {
                rot = dir.ToRotation_unk();
            }
            if (m14BC == 0) {
                if (range->m0C == 1) {
                    rot = dir.ToRotation_unk();
                }
                if (frame >= mMotion.fn_80097D7C(0)) {
                    m14BC++;
                    mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
                    fn_8030FFF0();
                }
            } else if (m14BC == 1) {
                if (range->m0C == 1) {
                    rot = dir.ToRotation_unk();
                }
                if (++m14D0 >= 60) {
                    m14BC++;
                    mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
                    fn_80310084();
                }
            } else if (m14BC == 2) {
                if (range->m0C == 1) {
                    rot = dir.ToRotation_unk();
                }
                if (frame >= range->m04) {
                    int id = range->m08 >= 0 ? range->m08 : fn_800F8548(fn_8013A194());
                    HitInfo_unk* bullet = fn_80219E6C(atk->mSkillId);
                    fn_802F0964(fn_8013A194(), this, bullet, id, 30, -1, 0.0f);
                    m14BC++;
                }
            } else {
                float len = mMotion.fn_80097D7C(0);
                if (mMotion.GetMotionFrame(0) >= len) {
                    mFlag31 = 1;
                }
            }
            break;
        }
        case 13: {
            float frame = mMotion.GetMotionFrame(0);
            if (m14BC == 0) {
                if (frame >= mMotion.fn_80097D7C(0)) {
                    m14BC++;
                    mMotion.fn_80097C08(0, 0x1E2, 1, 0, 1, 0.0f, 0.1f);
                }
            } else if (m14BC == 1) {
                if (!fn_8030E490(this)) {
                    m14BC++;
                    mMotion.fn_80097C08(0, 0x1E3, 0, 1, 1, 0.0f, 0.1f);
                }
            } else if (frame >= mMotion.fn_80097D7C(0)) {
                m14BC++;
                mFlag31 = 1;
            }
            break;
        }
        }
    } else {
        fn_805A4254();
        return;
    }
    if (follow && fn_8013C478()) {
        fn_80140588(&rot, speed, m568 * rate);
    } else {
        fn_8013CB68(rot, 0, speed, m568 * rate);
    }
}

void Em312DarkServant::fn_8030F380() {
    int on = 0;
    PlayerTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
    if (player != 0) {
        on = player->mFlag28;
    }
    if (on == 0 && m152C[2].m00 == mMotion.mTracks[0].m00 && mMotion.mTracks[0].m08 >= 36.0f) {
        on = 1;
    }
    if (on != 0 && m14D8 == 0) {
        m14D8 = 1;
        if (m58C != 13) {
            m14A8 |= 0x04000000;
            m54C &= ~0x2000;
        }
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
        fn_8023A8B4(lbl_8087F3C0, &m1580, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        fn_800C344C("SE_DG020_008", &mPos, 0, -1, 1.0f);
    } else if (on == 0 && m14D8 != 0) {
        m14D8 = 0;
        m14A8 &= ~0x04000000;
        m54C |= 0x2000;
        if (m58C == 11 && mMotion.mTracks[0].m08 < 36.0f) {
            vf100(6);
        }
        fn_80239DAC(lbl_8087F3C0, this, 1, 1);
    }
}

int Em312DarkServant::fn_8030FEAC(float radius) {
    int count = 0;
    for (CharaTask* c = lbl_8087F8A0->mHead; c != 0; c = ((PlayerTask*)c)->mNext) {
        if (!fn_8016F3D0(c, 1)) {
            atn::Vector3 d = c->mPos - mPos;
            if (PSVECSquareMag(&d) < radius * radius) {
                count++;
            }
        }
    }
    for (CharaTask* c = EnemyManager::sInstance->mHead; c != 0; c = ((EnemyTask*)c)->mNext) {
        if (!fn_8016F3D0(c, 1)) {
            atn::Vector3 d = c->mPos - mPos;
            if (PSVECSquareMag(&d) < radius * radius) {
                count++;
            }
        }
    }
    return count;
}

void Em312DarkServant::fn_8030FFF0() {
    fn_80232B7C(this, 1);
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
    fn_8023A8B4(lbl_8087F3C0, &m158C, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

void Em312DarkServant::fn_80310084() {
    fn_80239DAC(lbl_8087F3C0, this, 1, 1);
}

void Em312DarkServant::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em312 DarkServant[%d]\0", m58);
    void* group;
    if (m54.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m54.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    fn_8008771C(group, "MoveRadius", &m14E4, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_8008771C(group, "AttackLength", &m14E8, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_800874C8(group, "BreathIntervalInit", &m1570, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "BreathInterval", &m1574, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "AttackCountMax", &m1578, 0, 9999, 1, 0, 0);
    fn_800874C8(group, "DamageCountMax", &m157C, 0, 9999, 1, 0, 0);
    {
        void* sub = fn_8008937C(group, "VertAttack");
        fn_800874C8(sub, "SkillId", &m14EC[0].mSkillId, 0, 99999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m14EC[0].mCoolTime, 0, 99999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "HorzAttack");
        fn_800874C8(sub, "SkillId", &m14EC[1].mSkillId, 0, 99999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m14EC[1].mCoolTime, 0, 99999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "AllAttack");
        fn_800874C8(sub, "SkillId", &m14EC[2].mSkillId, 0, 99999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "BreathAttack");
        fn_800874C8(sub, "SkillId", &m14EC[3].mSkillId, 0, 99999, 1, 0, 0);
    }
}

