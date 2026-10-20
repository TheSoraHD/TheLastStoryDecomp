#include "game/Em201ZanGurg.h"
#include "atn/Csv.h"
#include "game/CharaManager.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int rand();
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" atn::Vector3 lbl_807C7030;              // inferred: zero vector
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, int a, int b, int c, float f1, float f2);
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
// inferred: voice player (lbl_8087F498, as in chr/ControlCliffhang.cpp)
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
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
extern "C" void fn_800EB7A0(CharaTask* chara);     // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800EE360(CharaTask* chara);     // inferred: BattleCharaTask (enemies only)
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: field (lbl_8087F430); fn_80370AE4 sets an event flag
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);

// inferred: four floats passed to the material edit (as in game/CharaTask.cpp); name invented
struct Color4_unk {
    Color4_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r;
    float g;
    float b;
    float a;
};

// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}
// inferred: names invented
inline void MaterialEdit_unk(Model_unk* model, const char* name, const Color4_unk& a, const Color4_unk& b,
                             const Color4_unk& c) {
    model->fn_80094958(name, &a, &b, &c);
}
inline float AngleDiffDeg_unk(float a, float b) {
    float d = fmod(57.295776f * (a - b), 360.0);
    if (d > 180.0f) {
        d -= 360.0f;
    }
    if (d < -180.0f) {
        d += 360.0f;
    }
    return d;
}
inline void PlayMotion_unk(Model_unk* model, int motion, int arg3, int arg4) {
    model->m34C = 1;
    model->mTracks[0].m20 = 1.0f;
    model->mTracks[0].m0C = 1.0f;
    model->fn_80097C08(0, motion, arg3, arg4, 1, 0.0f, 0.1f);
}
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

Em201ZanGurg::Em201ZanGurg(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m16CC(0), m16D0(0),
      m16D4(1500), m16D8(1502), m1708(0) {
    std::string dir("database/chara/special/");
    std::string name("db_ch3006_02_00.csv");
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
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m16DC = 0;
    m16E4 = 10;
    m16F8.fn_8023780C("/effect/ef_sk026a.efp");
}

Em201ZanGurg::~Em201ZanGurg() {}

int Em201ZanGurg::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m16F8.fn_80237874() == 0 &&
        m14B0.fn_80473F50() == 0) {
        fn_80262E6C((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3800000;
        m56C = 0.1f;
        mFlag24 = 0;
        MaterialEdit_unk(&mMotion, "em201_arm_l", Color4_unk(0.49803925f, 0.49803925f, 0.49803925f, 0.0f),
                         Color4_unk(1.0f, 1.0f, 1.0f, 0.0f), Color4_unk(1.0f, 1.0f, 1.0f, 0.0f));
        if (m16CC == 1) {
            m16E0 = 60;
        } else {
            m16E0 = 150;
        }
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

void Em201ZanGurg::fn_80262E6C(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "AIType") == 0) {
            m16CC = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "AttackSkillId") == 0) {
            m16D4 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "MagicSkillId") == 0) {
            m16D8 = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}

// inferred: inlined in vf10 (default state); name invented
inline void Em201ZanGurg::SelectAttack_unk() {
    if (m14B8 != 0) {
        atn::Vector3 pos = m14B8->mPos;
        atn::Vector3 d = pos - mPos;
        float dist = d.Length_unk();
        atn::Vector3 rot = d.GetNormalized_unk().ToRotation_unk();
        AngleDiffDeg_unk(rot.y, mRot.y);
        if (m14C4 == 0 && m16E8 != 0) {
            if (dist < 30.0f) {
                m58C = 8;
                fn_8016E970(6);
                m560 = 4;
                m590 = fn_800F8548(lbl_8087F048);
                PlayMotion_unk(&mMotion, 0x141, 0, 0);
            } else {
                m58C = 9;
                fn_8016E970(6);
                m560 = 0x1D;
                m590 = fn_800F8548(lbl_8087F048);
                PlayMotion_unk(&mMotion, 0x145, 1, 0);
                mFB8 = 0.0f;
                fn_80232B7C(this, 0);
                PlayEffect_unk(&m16F8, &mMotion);
            }
            m16DC = m16E0;
        }
    }
}

void Em201ZanGurg::vf10() {
    m14B8 = (CharaTask*)lbl_8087F8A0->mHead;
    mLook.fn_80129978(&m14B8->mMotion, "neck", lbl_807C7030, 0.3f);
    m14C0++;
    if (mC58.mC0 == 0 || m14B8 == 0) {
        fn_8016E970(3);
        m16E8 = 0;
        m16DC = m16E0;
    } else if (m14B8 != 0) {
        switch (m58C) {
        case 7:
            fn_802640FC();
            break;
        case 8:
            fn_8026442C();
            break;
        case 2: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                mFlag25 = 1;
                mColli.mFlags &= ~1;
                mFlag15 = 1;
                fn_801765D8();
                fn_800EE360(this);
            }
            break;
        }
        case 9:
            fn_8026474C();
            break;
        case 10:
            fn_802648CC();
            break;
        case 11: {
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                m58C = 0;
                fn_8016E970(3);
                m16E8 = 0;
            }
            break;
        }
        case 12:
            fn_80264C88();
            break;
        default:
            fn_80264024();
            SelectAttack_unk();
            break;
        }
    }
    m5B0 = 8.0f;
    fn_8014C540();
    fn_80145334();
}

void Em201ZanGurg::vf14() {
    fn_80149A30();
}

// inferred: inlined in vf3C (state 12); name invented
inline void Em201ZanGurg::StartCounter_unk(CharaTask* attacker) {
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 12;
    fn_8016E970(6);
    m560 = 10;
    atn::Vector3 rot = (attacker->mPos - mPos).ToRotation_unk();
    mRot.y = rot.y;
    PlayMotion_unk(&mMotion, 0x1D2, 0, 0);
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, this, 0, 0, 1.0f, 10.0f);
    }
}

void Em201ZanGurg::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    msg->m10 *= 0.0f;
    if (m16CC == 1) {
        mStatus.mParamB.m08 -= (float)(rand() % 50 + 300);
    } else {
        if (msg->mAttacker != 0) {
            if (msg->m44 == 0) {
                msg->mAttacker->vf7C(1);
                StartCounter_unk(msg->mAttacker);
            }
        } else {
            msg->m8C = 1;
        }
        msg->m64 = 1;
        msg->m90 = 1;
        msg->m84 = 1;
        msg->m88 = msg->mData->mC4;
        msg->m68 = 0;
        msg->m94 |= 0x8008;
        m6D0.PushBack_unk(CharaPair8_unk(msg->m34, msg->m38));
        return;
    }
    fn_80151448(msg);
    vf40(hit);
    m16DC -= 20;
    if (m16CC == 1) {
        mStatus.mParamB.m08 = 5000.0f;
    }
}

// inferred: inlined in vf40 (state 11); name invented
inline void Em201ZanGurg::StartKnockback_unk(const atn::Vector3& dir) {
    m58C = 11;
    fn_8016E970(6);
    atn::Vector3 rot = dir.ToRotation_unk();
    mRot.y = rot.y;
    PlayMotion_unk(&mMotion, 0x35, 0, 0);
    fn_8016DA4C();
}

void Em201ZanGurg::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (msg->m68 != 0 && m16CC == 1) {
        StartKnockback_unk(atn::Vector3(-msg->m28.x, -msg->m28.y, -msg->m28.z));
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        vf48();
        fn_80178A6C();
        fn_8015495C(0, 1, 1, 1);
        fn_8016DA4C();
        m58C = 2;
        PlayMotion_unk(&mMotion, 0x2E, 0, 0);
        fn_800EB7A0(this);
    }
}

void Em201ZanGurg::fn_80263CF0() {
    float speed = 0.0f;
    atn::Vector3 rot = mRot;
    atn::Vector3 pos = m14B8->mPos;
    atn::Vector3 d = pos - mPos;
    d.Length_unk();
    if (m55C == 7) {
        mRoute.fn_80126214();
        atn::Vector3 v = mRoute.m58;
        speed = v.Length_unk();
        if (speed > 0.0001f) {
            rot = v.GetNormalized_unk().ToRotation_unk();
        }
    } else if (m55C == 6) {
        if (m560 == 4) {
            m580 = 0.0f;
            m584 = 0.0f;
        } else {
            fn_8013A258();
            return;
        }
    }
    fn_8013CB68(&rot, 1, speed, m568);
}

void Em201ZanGurg::fn_80264024() {
    int state = m55C;
    switch (state) {
    case 6:
    case 7:
        break;
    case 3:
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        if (m16CC == 0) {
            mMotion.fn_80097C08(0, 0x14F, 1, 0, 1, 0.0f, 0.1f);
        } else {
            mMotion.fn_80097C08(0, 3, 1, 0, 1, 0.0f, 0.1f);
        }
        m16DC--;
        if (m16DC <= 0) {
            m16E8 = 1;
        }
        return;
    default:
        fn_8016E970(3);
        break;
    }
    fn_80263CF0();
}

void Em201ZanGurg::fn_802640FC() {
    if (m16DC < 0) {
        atn::Vector3 pos = m14B8->mPos;
    atn::Vector3 d = pos - mPos;
        float dist = d.Length_unk();
        atn::Vector3 rot = d.GetNormalized_unk().ToRotation_unk();
        AngleDiffDeg_unk(rot.y, mRot.y);
        if (rand() % (100 / m16E4) == 0 || dist > 30.0f) {
            m58C = 0;
            fn_8016E970(3);
            m16E8 = 1;
        } else {
            m16DC = m16E0;
        }
    } else {
        m16DC--;
    }
}

void Em201ZanGurg::fn_8026442C() {
    if (fn_80158BB4()) {
        m58C = 0;
        fn_8016E970(3);
        m16E8 = 0;
    }
    if (mMotion.mTracks[0].m08 >= 15.0f && mMotion.mTracks[0].m08 <= 18.0f) {
        HitInfo_unk* data = fn_80219E6C(m16D4);
        fn_80144710();
        int id = m590;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
    }
    if (mMotion.mTracks[0].m08 >= 1.0f && mMotion.mTracks[0].m08 <= 14.0f) {
        atn::Vector3 d = m14B8->mPos - mPos;
        d.Normalize_unk();
        d.fn_80139F60();
        mRot.y = atn::WrapAngle_unk(mRot.y + 0.1f * (d.y - mRot.y));
    }
}

void Em201ZanGurg::fn_8026474C() {
    m638 = fn_80219E6C(m16D8);
    mFBC = m638->mC0;
    if (mFB8 < m638->mC0) {
        mFB8 += 1.0f;
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 20.0f) {
            mMotion.mTracks[0].m0C = 0.1f;
        }
        if (mFB8 == m638->mC0) {
            m58C = 10;
            m560 = 30;
            float frame2 = mMotion.mTracks[0].m08;
            mMotion.fn_80097C08(0, 0x146, 0, 0, 1, 0.0f, 0.1f);
            mMotion.mTracks[0].m08 = frame2;
            mMotion.mTracks[0].m0C = 0.5f;
            m16EC = m14B8->mPos;
            fn_80239DAC(lbl_8087F3C0, this, 0, 1);
            if (lbl_8087F498 != 0) {
                fn_803EA77C(lbl_8087F498, this, 0x19, 0, 1.0f, 10.0f);
            }
        }
    }
}

void Em201ZanGurg::fn_802648CC() {
    m638 = fn_80219E6C(m16D8);
    if (mMotion.mTracks[0].m08 >= 30.0f && mMotion.mTracks[0].m08 <= 37.0f) {
        mMotion.mTracks[0].m0C = 1.0f;
        atn::Vector3 d = m16EC - mPos;
        d.Normalize_unk();
        d.fn_80139F60();
        mRot.y = atn::WrapAngle_unk(mRot.y + 0.3f * (d.y - mRot.y));
    }
    if (mFB8 > 0.0f && mMotion.mTracks[0].m08 >= 32.0f) {
        atn::Vector3 pos = GetNodePos_unk(mMotion.GetNodeMtx_unk("rightwepoint"));
        atn::Vector3 dir = m16EC - pos;
        dir.Normalize_unk();
        fn_800F8574(lbl_8087F048, this, m638, &pos, &dir, 0, 0, 0, 0.0f, 1.0f);
        mFB8 = 0.0f;
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m58C = 0;
        fn_8016E970(3);
        m16E8 = 0;
        mFB8 = 0.0f;
    }
}

void Em201ZanGurg::fn_80264C88() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        m58C = 0;
        fn_8016E970(3);
    } else if (mMotion.mTracks[0].m08 >= 19.0f && mMotion.mTracks[0].m08 <= 21.0f) {
        HitInfo_unk* data = fn_80219E6C(m16D4);
        fn_80144710();
        m59F = 1;
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
        m59F = 0;
    } else if (atn::Abs_unk(mMotion.mTracks[0].m08 - 10.0f) < 1.1920929e-7f) {
        if (++m16D0 > 2) {
            fn_80370AE4(lbl_8087F430, 0x66, 1);
        }
    }
}

// inferred: debug menu of the enemy; removed by the linker, only its strings are left in the pool
// (after "rightwepoint"). The group name "em305" is the one in the pool.
void Em201ZanGurg::fn_DebugMenu_unk() {
    void* group = m1704.Load("em305");
    fn_80087994(group, "DebugDisp", &m1708, 0, 0);
    fn_80087994(group, "WaitTime", &m16E0, 0, 0);
    fn_80087994(group, "AttackFrequcy", &m16E4, 0, 0);
}
