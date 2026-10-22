#include "game/Em101ElderZesya.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
// inferred: debug menu entries (as in gmk/SwingObject.cpp)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80088FAC(void* group, const char* name, ZesyaColor_unk* value, int arg3, int arg4, float min,
                            float max, float step);

struct Gimmick_unk {
    unsigned char m00[0x04];
    atn::Vector3 mPos;
    unsigned char m10[0x14 - 0x10];
    float mRotY_unk;
};

inline const atn::Vector3& Em101ElderZesya::Barrier_unk::GetPos_unk() const {
    return mDesc->mPos;
}

// inferred: difference on the ground plane (name invented)
inline atn::Vector3 FlatDiff_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return atn::Vector3(a.x - b.x, 0.0f, a.z - b.z);
}

struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80102EAC(Effect_unk* mgr, int id, CharaTask* attacker, int arg, int arg2);

// inferred: collision world and its ray test (see game/ChaserNpc.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep (as in gmk/Lemon.h)
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);

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

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
extern "C" double sqrt(double x);

// inferred: barrier sizes (radius, height); static vector of the file built by the __sinit (0x8025A03C)
static atn::Vector3 sChantOffset_unk(35.0f, 80.0f, 1.0f);

// NONMATCHING: 99.6% - registers r25/r26 of the strlen of the parameters and of the debug menu group
// (same as the second std::string of Em201ZanGurg::Em201ZanGurg)
Em101ElderZesya::Em101ElderZesya(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14C8(0), m14CC(0), m14D0(0), m14D4(0), m14F0(0), m14F4(0),
      m14F8(0), m14FC(1), m1500(0), m1510(0), m15EC(0), m15F0(0), m15F4(0), m164C(0), m1650(0),
      m1654(0.0f, 0.0f, 0.0f), m1660(0), m1668(0.0f, 0.0f, 0.0f), m1674(0.0f, 0.0f, 0.0f), m1680(0),
      m1684(0.0f, 0.0f, 0.0f), m1690(0.0f), m1694(0.0f, 0.0f, 0.0f), m16A4(0), m16A8(0), m16AC(1), m16B0(120),
      m16B4(9.0f), m16BC(0), m16C0(ZesyaColor_unk(0.98039216f, 0.3137255f, 0.23529412f, 1.0f)),
      m16D0(ZesyaColor_unk(0.078431375f, 0.11764706f, 0.9411765f, 1.0f)),
      m16E0(ZesyaColor_unk(1.0f, 1.0f, 1.0f, 1.0f)),
      m16F0(ZesyaColor_unk(0.23529412f, 0.39215687f, 0.27450982f, 1.0f)),
      m1700(ZesyaColor_unk(0.9019608f, 0.8235294f, 0.19607843f, 1.0f)) {
    mFlag22 = 1;
    mFlagB11 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_em101_00_00.csv");
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
    m14B4.vf0C(path.c_str());
    void* group = m16B8.Load("Em101ElderZesya");
    fn_80087994(group, "DebugDisp", &m16BC, 0, 0);
    fn_800874C8(group, "WaitFrame", &m16B0, 0, 1000000, 1, 0, 0);
    fn_8008771C(group, "AttackChanceFrame", &m16B4, 0, 0, 0.0f, 100000.0f, 1.0f);
    fn_800874C8(group, "ComboWaitTime", &m14FC, 0, 100000, 1, 0, 0);
    fn_800874C8(group, "bossActPhase_", &m14CC, 3, 5, 1, 0, 0);
    fn_80088FAC(group, "FireBarrier", &m16C0, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088FAC(group, "IceBarrier", &m16D0, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088FAC(group, "ForestBarrier", &m16F0, 0, 0, 0.0f, 1.0f, 0.01f);
    fn_80088FAC(group, "HolyBarrier", &m1700, 0, 0, 0.0f, 1.0f, 0.01f);
    m15F8.fn_8023780C("ef_bs007a.efp");
    m1604.fn_8023780C("ef_bs066e.efp");
    m1610.fn_8023780C("ef_bs066c.efp");
    m161C.fn_8023780C("ef_sk053a.efp");
    m1628.fn_8023780C("ef_sk053b.efp");
    m1634.fn_8023780C("ef_sk053c.efp");
    m1640.fn_8023780C("ef_sk054d.efp");
    mFlagB5 = 1;
}

Em101ElderZesya::Barrier_unk::Barrier_unk() {
    m04.mState = 2;
    m04.mKind = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            m04.mObjects[i][j] = 0;
        }
    }
    mDesc = 0;
}

Em101ElderZesya::Barrier_unk::~Barrier_unk() {}

Em101ElderZesya::~Em101ElderZesya() {}

int Em101ElderZesya::vf0C() {
    if (m14B0 == 0) {
        if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m14B4.fn_80473F50() == 0 &&
            m15F8.fn_80237874() == 0 && m1604.fn_80237874() == 0 && m1610.fn_80237874() == 0 &&
            m161C.fn_80237874() == 0 && m1628.fn_80237874() == 0 && m1634.fn_80237874() == 0) {
        mStatus.m18 |= 0x1C0;
        m58C = 0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        fn_8016E970(2);
        fn_80256384();
        m14C8 = 100000;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        m14B0 = 1;
        }
    } else if (m14B0 == 1) {
        for (unsigned int i = 0; i < 4; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    if (m151C[i].m04.mObjects[j][k] != 0 && m151C[i].m04.mObjects[j][k]->fn_800580BC() != 0) {
                        return 0;
                    }
                }
            }
        }
        m14B0 = 2;
        return 1;
    }
    return 0;
}

atn::Vector3 Em101ElderZesya::vf64(int center) {
    atn::Vector3 result = mPos;
    Barrier_unk* barrier = &m151C[m1680];
    if (barrier->m04.mState != 2 && barrier->mDesc != 0) {
        result = barrier->GetPos_unk();
        result.y += 0.8f * sChantOffset_unk.y;

    } else {
        result = m624.mData[0].m04.mCenter;
    }
    return result;
}

// inferred: the third argument is a position here
int Em101ElderZesya::vf4C(void* attacker, HitInfo_unk* hit, int arg, const atn::Vector3* dir) {
    const atn::Vector3* pos = (const atn::Vector3*)arg;
    Barrier_unk* barrier = &m151C[m1680];
    if (barrier->m04.mState != 2 && barrier->mDesc != 0) {
        atn::Vector3 d = *pos - barrier->mDesc->mPos;
        d.y = 0.0f;
        if (PSVECMag(&d) < sChantOffset_unk.x) {
            if (((float*)&hit->m74)[barrier->m04.mKind] < 0.5f) {
                return 4;
            }
        }
    }
    return 0;
}


// NONMATCHING: 99.9% - the original reloads m624.mData before the copy of the center (one lwz more,
// same open case as CharaTask::vf54)
// inferred: damage sphere of the body; while a barrier is up a second sphere covers it
void Em101ElderZesya::vf54() {
    if (m624.mSize != 0) {
        Barrier_unk* barrier = &m151C[m1680];
        Item14_unk* item = m624.mData;
        item->m04.mRadius = m5B0;
        item->m04.mCenter = mPos + m5A4;
        if (((unsigned int)mStatus.mParamB.m9C & 0x40) == 0x40) {
            m624.mData[0].m04.mCenter.y -= m624.mData[0].m04.mRadius;
        } else {
            m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
        }
        if (barrier->m04.mState != 2) {
            m624.mData[0].m04.mCenter.y -= 10000.0f;
            mFlagB29 = 0;
        } else {
            mFlagB29 = 1;
        }
        if (barrier->mDesc != 0) {
            m624.mData[1].m04.mRadius = 30.0f;
            m624.mData[1].m04.mCenter = barrier->mDesc->mPos;
            m624.mData[1].m04.mCenter.y += 30.0f;
            m624.mData[1].m04.mCenter.y += -10.0f;
            atn::Vector3 off(0.0f, 0.0f, -1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', barrier->mDesc->mRotY_unk);
            PSMTXMultVec(m, &off, &off);
            m624.mData[1].m04.mCenter += off;
            m624.mData[1].m00 = 0;
            if (barrier->m04.mState == 2) {
                m624.mData[1].m04.mCenter.y += 10000.0f;
                m624.mData[1].m00 |= 8;
            }
        }
    }
}

// inferred: warp point of the current barrier (name invented)
inline void Em101ElderZesya::GetWarp_unk(atn::Vector3* pos, float* rot) {
    Gimmick_unk* desc = 0;
    if (m1680 < 4) {
        desc = m151C[m1680].m00;
    }
    if (desc != 0) {
        *pos = desc->mPos;
        *rot = desc->mRotY_unk;
    }
}

void Em101ElderZesya::fn_8025941C() {
    m14C8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m14D0 = 0;
    m1660 = 0;
    m58C = 13;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1EA, 0, 0, 1, 0.0f, 0.1f);
    GetWarp_unk(&m1684, &m1690);
    CollResult_unk result;
    mPos = m1684;
    atn::Vector3 to = atn::Vector3(0.0f, -100.0f, 0.0f) + m1684;
    if (fn_8004ED34(lbl_8087EE98, &result, &m1684, &to, 0x80000000, &mColli, 0) != 0) {
        mPos = result.m10;
    }
    mRot.y = m1690;
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m15F8, &mMotion);
}

// inferred: hit handling: in states 9, 12 and 13 the hits are ignored; while a barrier is up a hit on the
// barrier (m40 != 0) breaks it if the attack matches its kind
void Em101ElderZesya::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 12 || m58C == 13 || m58C == 9) {
        msg->m10 *= 0.0f;
        msg->m64 = 3;
        msg->m90 = 0;
        msg->m84 = 2;
        msg->m88 = msg->mData->mC4;
        msg->m68 = 0;
        return;
    }
    if (*(int*)&msg->m3C[4] != 0) {
        int index = m1680;
        Barrier_unk* barrier = &m151C[index];
        if (barrier->m04.mState != 0) {
            return;
        }
        if (msg->m44 == 0) {
            msg->mAttacker->vf7C(1);
            msg->m64 = 4;
            msg->m90 = 0;
            msg->m68 = 0;
        } else if (msg->m44 == 2) {
            if (((float*)&msg->mData->m74)[barrier->m04.mKind] > 0.5f) {
                fn_80259AB4(index, 2);
                m15F4 = 1;
                m15EC = 0;
                if (fn_80370A78(lbl_8087F430, 200) == 0) {
                    fn_80370AE4(lbl_8087F430, 200, 1);
                }
            }
        }
        return;
    }
    CharaTask::vf3C(hit);
    m14C8 += 30;
    m14F0 += msg->m68;
    if ((float)m14F0 / (float)mStatus.mParamB.m84 > 0.1f) {
        m14F8 = 1;
        m14F0 = 0;
    }
    float hp = mStatus.m04;
    if (hp / (float)mStatus.mParamB.m84 < 0.1f) {
        fn_80370AE4(lbl_8087F430, 210, 1);
        for (unsigned int i = 0; i < 4; i++) {
            fn_80259AB4(i, 2);
        }
    }
    if (msg->m68 > 0) {
        fn_80102EAC(lbl_8087F048, m16A0, msg->mAttacker, 100, -1);
    }
    if (m55C == 6) {
        switch (m560) {
        case 22:
            goto reset;
        case 23:
            fn_8015E4B0(m6B8, -1);
        case 20:
        reset:
            m58C = 0;
            m14C8 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m14D0 = 0;
            m1660 = 0;
            break;
        }
    }
}

// inferred: names invented
inline int PartyCount_unk() {
    return lbl_8087F8A0->mCount;
}
inline atn::Vector3 PartyCenter_unk() {
    atn::Vector3 center = lbl_807C7030;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        center += p->mPos;
    }
    center *= 1.0f / (float)PartyCount_unk();
    return center;
}

// NONMATCHING: 98.8% - r3/r4 of the vector copies (the zero vector and the chosen point; __VecCopy family,
// docs/mwcc-blockers-q3.md) and the order of the two increments of the loop
// inferred: picks the chant point nearest to the center of the party (m1694)
void Em101ElderZesya::fn_802598DC() {
    atn::Vector3 center = lbl_807C7030;
    for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
        center += p->mPos;
    }
    center *= 1.0f / (float)PartyCount_unk();
    int best = -1;
    float dist = 1e16f;
    for (unsigned int i = 0; i < m14BC.mSize; i++) {
        atn::Vector3 d = center - m14BC.mData[i];
        float len = sqrt(d.x * d.x + d.z * d.z);
        if (dist > len) {
            dist = len;
            best = i;
        }
    }
    m1694 = m14BC.mData[best];
}
