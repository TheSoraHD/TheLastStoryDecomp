#include "game/Em309GhoulSimple.h"
#include "atn/Csv.h"
#include "chr/ControlStickByQuark.h"
#include <boost/bind.hpp>
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/PlayerTask.h"
#include "std/new.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" int rand();
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data by id
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: collision world (lbl_8087EE98) and its tests (as in game/CharaTask.cpp)
struct GhoulColl_unk {
    GhoulColl_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28; // inferred: hit normal
    int m34;
    int m38;
    unsigned int m3C; // inferred: attribute of the hit (bit 5: wall)
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004D388(void* world, GhoulColl_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" int fn_8004ED34(void* world, GhoulColl_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" void PSMTXRotAxisRad(Mtx m, const atn::Vector3* axis, float rad);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int arg, float angle); // inferred: fires a bullet (as in game/Em205Ogre.cpp)
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11);
// inferred: effect manager (class unknown, as in game/Em309Ghoul.cpp)
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
struct GhoulColor_unk {
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const GhoulColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: field (lbl_8087F430), route tables at 0x10D8
struct GhoulField_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" GhoulField_unk* lbl_8087F430;
// inferred: per-player weights of an effect (floats at +0x128); names invented
struct GhoulWeights_unk {
    unsigned char m00[0x128];
    float mWeight[4];
};
extern "C" GhoulWeights_unk* fn_80103F60(Effect_unk* mgr, CharaTask* owner);
extern "C" int fn_80108378(GhoulWeights_unk* w, CharaTask* chara); // inferred: index of `chara`

inline GhoulStr_unk& GhoulStr_unk::operator=(const char* str) {
    if (str != mStr) {
        memcpy(mStr, str, strlen(str) + 1);
    }
    return *this;
}

// inferred: name invented
inline bool Em309GhoulSimple::IsDownState_unk() {
    bool ok = true;
    if ((mStatus.m0C & 0x80000) != 0x80000 && (mStatus.m0C & 0x1000000) != 0x1000000) {
        ok = false;
    }
    return ok;
}

Em309GhoulSimple::Em309GhoulSimple(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14D8(0.0f, 0.0f, 0.0f), m14E4(40.0f), m14E8(0), m14EC(0), m14F0(0),
      m14F4(0), m14F8(0), m14FC(0), m1500(0), m1504(0), m1508(0), m150C(0), m1510(0), m1514(0), m1518(0),
      m151C(0), m1520(0), m1524(0), m1528(90), m1538(10), m153C(3.0f), m1540(0.0f, 0.0f, 0.0f),
      m154C(0.0f, 0.0f, 0.0f), m1570(0), m15B8(-1.0f), m1648(0),
      m164C(0), m1654(0) {
    char name[0x100];
    char path[0x100];
    if (fn_805A3D68(name, info->m2C)) {
        // inferred: the pool keeps a second terminator after this string and the next one
        sprintf(path, "/database/chara/special/%s\0", name);
    } else {
        sprintf(path, "/database/chara/special/db_em309_01_00.csv\0");
    }
    m14B4.vf0C(path);
    m152C.fn_8023780C("/effect/ef_sk030b.efp");
    m15BC = "/effect/ef_sk036e.efp";
    m14E8 = 0x64A;
    m14F0 = 90;
    m14F8 = 0x64B;
    m1500 = 150;
    m1508 = 0x64C;
    m1510 = 120;
    m150C = 30;
    m1518 = 0x64D;
    m1520 = 0;
    mFlag22 = 1;
}

Em309GhoulSimple::~Em309GhoulSimple() {}

int Em309GhoulSimple::vf0C() {
    if (m164C == 0) {
        if (m14B4.fn_80473F50() != 0) {
            return 0;
        }
        fn_80306078((const char*)m14B4.fn_80470580(), m14B4.fn_8047059C());
        if (m15B8 > 0.0f && m1574.mCount != 0) {
            m163C.fn_8023780C(m15BC.mStr);
        }
        m164C++;
    } else {
        if (fn_8013655C() != 0) {
            return 0;
        }
        if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
            return 0;
        }
        if (m152C.fn_80237874() != 0) {
            return 0;
        }
        if (m163C.fn_80237874() != 0) {
            return 0;
        }
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        if (m15B8 > 0.0f && m1574.mCount != 0) {
            m1648 = 1;
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x4011;
        mStatus.m18 |= 0x3000000;
        mStatus.m18 |= 0x8004;
        vfFC();
        return 1;
    }
    return 0;
}

void Em309GhoulSimple::fn_80306078(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "AttackNormalSkillId") == 0) {
            m14E8 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackNormalCoolTime") == 0) {
            m14F0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLongSkillId") == 0) {
            m1508 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLongChargeTime") == 0) {
            m150C = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackLonglCoolTime") == 0) {
            m1510 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackRollingSkillId") == 0) {
            m1518 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackDistance") == 0) {
            m14E4 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "WaitFrame") == 0) {
            m1528 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "SometimeAttackLong") == 0) {
            m1538 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "HeadAttackStartFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1558, &csv);
        } else if (strcmp(key, "HeadAttackNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1560, &csv);
        } else if (strcmp(key, "SummonHp") == 0) {
            m15B8 = fn_800DC288(csv.fn_8005B9CC()) / 100.0f;
        } else if (strcmp(key, "SummonEnemy") == 0) {
            while (true) {
                int id = atoi(csv.fn_8005B9CC());
                if (id == 0) {
                    break;
                }
                m1574.PushBack_unk(GhoulPair_unk(id, 0));
            }
        } else if (strcmp(key, "SummonWarpPoint") == 0) {
            GhoulPair_unk* p = m1574.mData;
            for (unsigned int i = 0; i < m1574.mCount; p++, i++) {
                p->m04 = atoi(csv.fn_8005B9CC());
            }
        } else if (strcmp(key, "SummonNotifyFlag") == 0) {
            fn_805A4980((SpecialFlag_unk*)&m1568, &csv);
        } else if (strcmp(key, "SummonEfp") == 0) {
            m15BC = csv.fn_8005B9CC();
        } else if (strcmp(key, "Speed") == 0) {
            m153C = fn_800DC288(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

// NONMATCHING: 99.57% - volatile registers of the inlined CanAct_unk (r5/r0/r4 in the original, r5/r4/r6 here;
// the same inline matches in Em205Ogre/Em315Coffin); 9 variants
void Em309GhoulSimple::vf10() {
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        vf104();
    }
    if (m14F4 > 0) {
        m14F4--;
    }
    if (m1504 > 0) {
        m1504--;
    }
    if (m1514 > 0) {
        m1514--;
    }
    if (m1524 > 0) {
        m1524--;
    }
    fn_8014C540();
    atn::Vector3 pos = mPos;
    atn::Vector3 rot = mRot;
    if (m58C == 12) {
        if (m14BC == 0) {
            float frame = mMotion.mTracks[0].m08;
            mPos.y += m5B0 * (frame / mMotion.fn_80097D7C(0));
        } else {
            mPos.y += m5B0;
            m154C.x += -0.5235988f;
            mRot.x = m154C.x;
        }
    }
    if ((mFlags & 1) != 1 && !CharaTask::mFlag25) {
        fn_80145334();
    }
    if (m58C == 12) {
        mPos = pos;
        mRot = rot;
    }
    if (m1570 != 0) {
        GhoulWeights_unk* w = fn_80103F60(lbl_8087F048, this);
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            int i = fn_80108378(w, p);
            w->mWeight[i] = p->mType == 0 ? 1.0f : 0.3f;
        }
    }
}

void Em309GhoulSimple::vf14() {
    fn_80149A30();
}

int Em309GhoulSimple::vf10C(HitMsg_unk* msg) {
    if (mFlag29) {
        return 0;
    }
    if (m58C == 13) {
        if (msg->mData != 0 && msg->mData->m04 == 0x658) {
            msg->m10 *= 0.0f;
            msg->m0C |= 0x80;
            return 1;
        }
        return 0;
    }
    msg->m10 *= 0.0f;
    if (!IsDownState_unk()) {
        msg->m0C |= 8;
    }
    return 1;
}

void Em309GhoulSimple::vfFC() {
    fn_8016E970(2);
    vf100(6);
}

// inferred: names invented (as in game/Em205Ogre.cpp)
inline void SafeNormalize_unk(atn::Vector3* v) {
    if ((float)__fabs(fn_805F9920(v)) < 1.1920929e-7f) {
        return;
    }
    PSVECNormalize(v, v);
}

// inferred: negated vector (name invented)
inline atn::Vector3 GhoulNeg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}

// inferred: hit by skill 0xCB: the ghoul turns its back to the knockback direction and plays state 12;
// a hit by skill 0x658 during the head attack (13) raises "HeadAttackNotifyFlag"
void Em309GhoulSimple::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf100(2);
        return;
    }
    if (msg->mData != 0 && msg->mData->m04 == 0xCB) {
        atn::Vector3 dir = msg->m10;
        dir.y = 0.0f;
        if (!((float)__fabs(fn_805F9920(&dir)) < 1.1920929e-7f)) {
            PSVECNormalize(&dir, &dir);
            mRot = dir.ToRotation_unk();
            mRot.y += 3.1415927f;
        }
        vfFC();
        vf100(12);
    }
    if (m58C == 13 && msg->mData != 0 && msg->mData->m04 == 0x658) {
        fn_805A4A1C((SpecialFlag_unk*)&m1560, 1);
        m1570 = 1;
    }
}

// NONMATCHING: 99.96% - stack slots of the direction temporaries of states 9-11 and 12 (~15 variants)
void Em309GhoulSimple::vf100(int state) {
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
        m14D4 = mC58.mC4;
        mC58.m0C.fn_80128930(m14D4, &mPos, 0);
        mC58.m0C.mRadius = 0.9f * m14E4;
        mC58.m0C.fn_801255C8();
        mFlag30 = 1;
        break;
    case 9:
    case 10:
    case 11:
        m14D4 = mC58.mC4;
        if (m14D4 != 0) {
            atn::Vector3 d = m14D4->mPos - mPos;
            m14D8 = d;
            m14D8.y = 0.0f;
            SafeNormalize_unk(&m14D8);
        } else {
            m14D8.x = 0.0f;
            m14D8.y = 0.0f;
            m14D8.z = 1.0f;
            Mtx m;
            float ry = mRot.y;
            PSMTXRotRad(m, 'y', ry);
            PSMTXMultVec(m, &m14D8, &m14D8);
        }
        if (m58C == 9) {
            fn_805A4F1C(0x140, 0, 1, 1.0f, 0.0f);
        } else if (m58C == 10) {
            fn_805A4F1C(0x143, 0, 1, 1.0f, 0.0f);
        } else {
            fn_805A4F1C(0x145, 0, 1, 1.0f, 0.0f);
        }
        m14C8 = fn_800F8548(lbl_8087F048);
        mFlag28 = 1;
        break;
    case 12: {
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m;
        float ry = mRot.y;
        PSMTXRotRad(m, 'y', ry);
        PSMTXMultVec(m, &front, &front);
        m1540 = GhoulNeg_unk(front);

        m154C.x = 0.0f;
        m154C.y = 0.0f;
        m154C.z = 0.0f;
        fn_805A4F1C(0x14A, 0, 1, 1.0f, 0.0f);
        m14C8 = fn_800F8548(lbl_8087F048);
        mFlag28 = 1;
        break;
    }
    case 13:
        fn_805A4F1C(0x3D, 0, 1, 1.0f, 0.0f);
        mFlag28 = 1;
        break;
    case 14:
        fn_805A4F1C(0x150, 0, 1, 1.0f, 0.0f);
        mFlag28 = 1;
        break;
    case 2:
        if ((mStatus.m0C & 0x20) == 0x20 && prev != 2) {
            vf44();
        }
        fn_805A4F1C(0x2E, 0, 1, 1.0f, 0.0f);
        break;
    }
}

// inferred: hit point ratio (name invented)
inline float GhoulHpRatio_unk(CharaTask* c) {
    int max = c->mStatus.mParamB.m84;
    return max > 0 ? c->mStatus.m04 / max : 0.0f;
}

void Em309GhoulSimple::vf104() {
    m14C0++;
    if (CanAct_unk() && (m58C == 7 || m58C == 8)) {
        if (!vfF8()) {
            vf100(6);
        } else if (m1648 != 0 && GhoulHpRatio_unk(this) <= m15B8) {
            vf100(14);
        }
    }
    switch (m58C) {
    case 6:
        if (vfF8()) {
            vf100(8);
        }
        vf108();
        break;
    case 7:
        if (mFlag31) {
            if (m1538 > rand() % 100 && m1514 <= 0) {
                vf100(11);
            } else {
                vf100(8);
            }
        }
        vf108();
        break;
    case 8: {
        CharaTask* target = m14D4;
        if (mC58.mC4 != target) {
            if (m1538 > rand() % 100 && m1514 <= 0) {
                vf100(11);
            } else {
                vf100(8);
            }
        } else {
            int state = target->m55C;
            int sub = target->m560;
            fn_805A45F8(50.0f, 360.0f, 0.0f);
            if (m14C0 % 10 == 0 && m1514 <= 0 && state == 6 && (sub == 0x1D || sub == 0xD)) {
                vf100(11);
            } else if (mFlag31) {
                atn::Vector3 d = m14D4->mPos - mPos;
                if (fn_805F9920(&d) < m14E4 * m14E4) {
                    if (m14F4 <= 0) {
                        vf100(9);
                    }
                } else {
                    vf100(8);
                }
            }
        }
        vf108();
        break;
    }
    case 9:
    case 10:
    case 11:
    case 13:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 12:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 14:
        if (mFlag31) {
            vf100(7);
        }
        vf108();
        break;
    case 2:
        fn_805A40B8();
        break;
    default:
        vf100(6);
        break;
    }
}

// inferred: unit vector rotated around y (name invented, as Front_unk in game/Em203GreaterDaemon.cpp)
inline atn::Vector3 GhoulFront_unk(const float& angle) {
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(angle);
    PSMTXMultVec(m.m, &front, &front);
    return front;
}

#define GHOUL_MAX0_UNK(x) ((x) > 0.0f ? (x) : 0.0f)

// inferred: the status flags that slow the ghoul down (bits 3/9 of the status, 0x800000, 0x1000000); name invented
inline bool GhoulSlow1_unk(const unsigned int& flags) {
    return (flags & 8) == 8 || (flags & 0x200) == 0x200;
}
inline bool GhoulSlow2_unk(const unsigned int& flags) {
    return GhoulSlow1_unk(flags) || (flags & 0x800000) == 0x800000;
}
inline bool GhoulSlowed_unk(const unsigned int& flags) {
    return GhoulSlow2_unk(flags) || (flags & 0x1000000) == 0x1000000;
}


// NONMATCHING: 99.61% - stack layout of the state locals and FPR choice of the frame/speed (f29/f30/f31);
// the code is the same (~10 variants on the helpers and the temporaries)
// inferred: runs the current state (6 wait, 7 rest, 8 chase, 9-10 normal/long attack, 11 rolling attack, 12
// knocked back, 13 thrust, 14 summon) and moves towards the rotation `rot`
void Em309GhoulSimple::vf108() {
    float dist = 0.0f;
    float speed = !GhoulSlowed_unk(mStatus.m0C) ? m153C : 0.8f;
    atn::Vector3 rot = mRot;
    int turn = 0;
    if (m55C == 2) {
    if (!(m58C - 11U <= 2) && m58C != 2) {
        mMotion.mTracks[0].m0C = !GhoulSlowed_unk(mStatus.m0C) ? 2.0f : 0.6f;
    } else {
        mMotion.mTracks[0].m0C = 1.0f;
    }
    switch (m58C) {
    case 7: {
        int wait = !GhoulSlowed_unk(mStatus.m0C) ? m1528 / 8 : m1528;
        if (m14C0 > wait) {
            mFlag31 = 1;
        }
        break;
    }
    case 8: {
        mC58.m0C.fn_80126214();
        if (mC58.m0C.m2C != 0) {
            mFlag31 = 1;
        }
        atn::Vector3 dir(mC58.m0C.m58);
        dist = fn_805F9920(&dir);
        if (dist > 0.0001f) {
            rot = dir.GetNormalize().ToRotation_unk();
        } else {
            rot = mRot;
        }
        break;
    }
    case 9:
    case 10: {
        rot = m14D8.ToRotation_unk();
        float frame = mMotion.mTracks[0].m08;
        if (m14BC == 0) {
            if (m58C == 9) {
                if (frame >= 36.0f) {
                    int id = m14C8;
                    HitInfo_unk* skill = fn_80219E6C(m14E8);
                    fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, id, 30, -1, 0.0f);
                    m14BC++;
                }
            } else if (m58C == 10 && frame >= 40.0f) {
                int id = m14C8;
                HitInfo_unk* skill = fn_80219E6C(m14F8);
                fn_800F8C6C(lbl_8087F048, 0, 0, this, skill, id, 30, -1, 0.0f);
                m14BC++;
            }
        } else if (frame >= mMotion.fn_80097D7C(0)) {
            mFlag31 = 1;
        }
        break;
    }
    case 11: {
        rot = m14D8.ToRotation_unk();
        float frame = mMotion.mTracks[0].m08;
        if (m14BC == 0) {
            if (frame >= mMotion.fn_80097D7C(0)) {
                mMotion.fn_80097C08(0, 0x146, 1, 0, 1, 0.0f, 0.1f);
                m14C0 = 0;
                m14BC++;
            }
        } else if (m14BC == 1) {
            if (m14C0 > 30) {
                mMotion.fn_80097C08(0, 0x147, 0, 0, 1, 0.0f, 0.1f);
                fn_80232B7C(&m152C, 0);
                GhoulColor_unk color;
                atn::Vector3 erot;
                atn::Vector3 epos;
                erot.x = 0.0f;
                erot.y = 0.0f;
                erot.z = 0.0f;
                epos.x = 0.0f;
                epos.y = 0.0f;
                epos.z = 0.0f;
                color.r = 1.0f;
                color.g = 1.0f;
                color.b = 1.0f;
                color.a = 1.0f;
                fn_8023A8B4(lbl_8087F3C0, &m152C, &mMotion, 0, epos, erot, color, -1, -1, 1, 1.0f);
                m14BC++;
            }
        } else if (m14BC == 2) {
            if (frame >= 40.0f) {
                m14BC++;
            }
        } else {
            if (m14C0 % 2 == 0) {
                float t = GHOUL_MAX0_UNK((frame - 40.0f) / 28.0f) < 1.0f ? GHOUL_MAX0_UNK((frame - 40.0f) / 28.0f)
                                                                           : 1.0f;
                float range = 330.0f * t;
                atn::Vector3 pos = mPos;
                MtxPtr left = mMotion.GetNodeMtx_unk("lefthand");
                MtxPtr right = mMotion.GetNodeMtx_unk("righthand");
                if (left != 0 && right != 0) {
                    atn::Vector3 l(left[0][3], left[1][3], left[2][3]);
                    atn::Vector3 r(right[0][3], right[1][3], right[2][3]);
                    pos = (l + r) * 0.5f;
                }
                atn::Vector3 step(m14D8.x * range, m14D8.y * range, m14D8.z * range);
                HitInfo_unk* skill = fn_80219E6C(m1508);
                for (int i = 0; i < 5; i++) {
                    atn::Vector3 at = pos + step;
                    pos.y = 2.0f * ((float)i * skill->m58) + pos.y;
                    fn_800FAB80(lbl_8087F048, this, skill, m14C8, &at, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
                }
                Mtx m1;
                PSMTXRotRad(m1, 'y', -0.2617994f);
                PSMTXMultVec(m1, &step, &step);
                for (int i = 0; i < 5; i++) {
                    atn::Vector3 at = pos + step;
                    pos.y = 2.0f * ((float)i * skill->m58) + pos.y;
                    fn_800FAB80(lbl_8087F048, this, skill, m14C8, &at, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
                }
                Mtx m2;
                PSMTXRotRad(m2, 'y', 0.5235988f);
                PSMTXMultVec(m2, &step, &step);
                for (int i = 0; i < 5; i++) {
                    atn::Vector3 at = pos + step;
                    pos.y = 2.0f * ((float)i * skill->m58) + pos.y;
                    fn_800FAB80(lbl_8087F048, this, skill, m14C8, &at, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
                }
            }
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag31 = 1;
            }
        }
        break;
    }
    case 12: {
        rot = m1540.ToRotation_unk();
        float frame = mMotion.mTracks[0].m08;
        if (m14BC == 0) {
            if (frame >= mMotion.fn_80097D7C(0)) {
                mMotion.fn_80097C08(0, 0x14B, 1, 0, 1, 0.0f, 0.1f);
                m14BC++;
            }
        } else if (m14C0 > 300) {
            mFlag31 = 1;
        } else {
            Sphere_unk sphere = fn_80176548(mPos);
            GhoulColl_unk result;
            atn::Vector3 move(m1540.x * 10.0f, -5.0f, m1540.z * 10.0f);
            if (fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80020000, 0, 0, sphere.mRadius)) {
                mPos = result.m10;
                if ((result.m3C & 0x20) == 0x20) {
                    fn_800C344C("SE_EM309_016", &result.m10, 0, -1, 1.0f);
                    atn::Vector3 to = sphere.mCenter + m1540 * 50.0f;
                    GhoulColl_unk wall;
                    atn::Vector3 dir;
                    if (fn_8004ED34(lbl_8087EE98, &wall, &sphere.mCenter, &to, 0x80000000, 0, 0)) {
                        dir = m1540;
                        dir.y = 0.0f;
                        Mtx m;
                        PSMTXRotAxisRad(m, &wall.m28, 3.1415927f);
                        PSMTXMultVec(m, &dir, &dir);
                        dir = GhoulNeg_unk(dir);
                        PSVECNormalize(&dir, &dir);
                    } else {
                        atn::Vector3 d = result.m10 - (sphere.mCenter + m1540);
                        dir = d;
                        dir.y = 0.0f;
                        PSVECNormalize(&dir, &dir);
                    }
                    m1540 = dir;
                }
            } else {
                mPos = result.m10;
            }
            atn::Vector3 back = GhoulNeg_unk(m1540).ToRotation_unk();
            float d = atn::WrapAngle_unk(back.y - mRot.y) * 0.6f;
            mRot.y += d;
            mPos.y = mPos.y - sphere.mRadius;
        }
        if (m14BC == 1 && m14C0 % 6 == 0) {
            Effect_unk* mgr = lbl_8087F048;
            fn_800FAB80(mgr, this, fn_80219E6C(m1518), m14C8, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        }
        return;
    }
    case 13: {
        float frame = mMotion.mTracks[0].m08;
        if (frame <= 25.0f) {
            float d = atn::WrapAngle_unk(m154C.y - mRot.y) * 0.6f;
            mRot.y += d;
        } else {
            mRot.y = m154C.y;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag31 = 1;
            }
        }
        return;
    }
    case 14: {
        float frame = mMotion.mTracks[0].m08;
        if (m14BC == 0) {
            if (frame >= 40.0f) {
                for (unsigned int i = 0; i < m1574.mCount; i++) {
                    GhoulPair_unk* pair = &m1574.mData[i];
                    EnemyTask* enemy = EnemyManager::sInstance != 0 ? EnemyManager::sInstance->Find_unk(pair->m00) : 0;
                    RouteTable_unk* table = lbl_8087F430 != 0 ? lbl_8087F430->m10D8 : 0;
                    RoutePlace_unk* place = table != 0 ? table->FindPlace_unk(pair->m04) : 0;
                    if (enemy != 0 && place != 0) {
                        atn::Vector3 pos = place->mPos;
                        Mtx m;
                        PSMTXRotRad(m, 'y', place->m14);
                        atn::Vector3 dir;
                        dir = atn::Vector3(0.0f, 0.0f, 1.0f);
                        PSMTXMultVec(m, &dir, &dir);
                        enemy->fn_80176ACC();
                        enemy->fn_800D246C(0);
                        enemy->m58C = 0;
                        enemy->mStatus.fn_8012D8B8();
                        enemy->fn_8015495C(0, 1, 1, 1);
                        enemy->mC58.mC0 = 1;
                        enemy->m54C &= ~0x2000;
                        enemy->vf80(pos, dir);
                        fn_80232B7C(pair, i);
                        GhoulColor_unk color;
                        atn::Vector3 erot;
                        atn::Vector3 epos;
                        erot.x = 0.0f;
                        erot.y = 0.0f;
                        erot.z = 0.0f;
                        epos.x = 0.0f;
                        epos.y = 0.0f;
                        epos.z = 0.0f;
                        color.r = 1.0f;
                        color.g = 1.0f;
                        color.b = 1.0f;
                        color.a = 1.0f;
                        fn_8023A8B4(lbl_8087F3C0, &m163C, &enemy->mMotion, 0, epos, erot, color, -1, -1, 1, 1.0f);
                    }
                }
                fn_805A4A1C((SpecialFlag_unk*)&m1568, 1);
                m14BC++;
            }
        } else if (frame >= mMotion.fn_80097D7C(0)) {
            m1648 = 0;
            mFlag31 = 1;
        }
        break;
    }
    }
    } else {
        fn_805A4254();
        return;
    }
    if (turn) {
        fn_801426A4(&rot, dist, speed);
    } else {
        fn_8013CB68(&rot, 0, dist, m568 * speed);
    }
}

// inferred: node position (name invented, as in game/Em205Ogre.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// NONMATCHING: 99.77% - r4/r5 of the copy of the spine position (__VecCopy family, documented blocker)
void Em309GhoulSimple::vf20() {
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 top = m614;
    atn::Vector3 head;
    int spine = mMotion.fn_80092814("spine1", 0);
    if (spine > -1) {
        head = GetNodePos_unk(mMotion.mNodeMtx[spine]);
    } else {
        head = top;
        head.y += m5B4 - 2.0f * m620;
    }
    float r = m620;
    mColli.m3C = top;
    mColli.m48 = head;
    mColli.m54 = r;

}

// inferred: two damage spheres on the body (the second one is moved to the head by vf54)
void Em309GhoulSimple::vf50() {
    Item14_unk item;
    m624.clear();
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    item.m04.mCenter.y += item.m04.mRadius;
    m624.push_back(item);
    m624.push_back(item);
    mFlagB29 = 0;
}

void Em309GhoulSimple::vf54() {
    if (m624.mSize < 2) {
        return;
    }
    m624.mData[0].m04.mRadius = mColli.m54;
    m624.mData[0].m04.mCenter = mColli.m3C;
    m624.mData[1].m04.mRadius = 0.5f * mColli.m54;
    m624.mData[1].m04.mCenter = mColli.m48;
    m624.mData[1].m04.mCenter.y += 0.5f * mColli.m54;
}

atn::Vector3 Em309GhoulSimple::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (fn_805A49E4((SpecialFlag_unk*)&m1558) > 0 && m624.mSize >= 2) {
        result = m624.mData[1].m04.mCenter;
    } else {
        result = mPos;
    }
    return result;
}


// inferred: the head (second damage sphere) can be thrust at while the ghoul is down (m55C 2, not in state
// 12): point 0.8 of the sphere radius in front of it
// NONMATCHING: 99.52% - r4/r5 of the center address and of the sum temporary (__VecCopy family); ~10 variants
int Em309GhoulSimple::vfB0(atn::Vector3* out) {
    if (m55C != 2 || m58C == 12 || m624.mSize < 2) {
        return 0;
    }
    float r = mColli.m54;
    *out = m624.mData[1].m04.mCenter + GhoulFront_unk(mRot.y) * r * 0.8f;
    return 1;
}

// inferred: a character thrusts at the head: the ghoul restarts (vfFC) into state 13 facing the character,
// and the character aims at the "neck" node (chr::ControlThrustForGhoul)
boost::function<void()> Em309GhoulSimple::vfB4(CharaTask* chara) {
    vfFC();
    vf100(13);
    m154C = chara->mPos - mPos;
    m154C.y = 0.0f;
    m154C.RotZ_unk();
    atn::Vector3 target;
    vfB0(&target);
    chara->SetHit_unk(fn_80219E6C(0x658));
    chr::ControlThrustForGhoul* next = new (0, "", "", 0) chr::ControlThrustForGhoul(chara, this, "neck");
    return boost::bind(&CharaTask::fn_80178208, chara, next);
}

void Em309GhoulSimple::vf30(void* menu) {
    char name[0x100];
    sprintf(name, "Em309 GhoulLight[%d]\0", m58);
    void* group;
    if (m1650.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = m1650.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_805A5220(group);
    {
        void* sub = fn_8008937C(group, "AttackNormal");
        fn_800874C8(sub, "SkillID", &m14E8, 0, 999999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m14F0, 0, 999999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "AttackLong");
        fn_800874C8(sub, "SkillID", &m14E8, 0, 999999, 1, 0, 0);
        fn_800874C8(sub, "ChargeTime", &m14EC, 0, 999999, 1, 0, 0);
        fn_800874C8(sub, "CoolTime", &m14F0, 0, 999999, 1, 0, 0);
    }
    {
        void* sub = fn_8008937C(group, "AttackRolling");
        fn_800874C8(sub, "SkillID", &m14E8, 0, 999999, 1, 0, 0);
    }
    fn_8008771C(group, "AttackDistance", &m14E4, 0, 0, 0.0f, 9999.0f, 1.0f);
    fn_800874C8(group, "WaitFrame", &m1528, 0, 999, 1, 0, 0);
    fn_800874C8(group, "SometimeAttackLong", &m1538, 0, 999, 1, 0, 0);
}

float Em309GhoulSimple::vf90() {
    return 3.0f;
}

