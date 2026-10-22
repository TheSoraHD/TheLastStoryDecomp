#include "game/Em303KrakenFootMp.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "game/CharaStatus.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (as in game/Em209BigGolem.cpp)
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: node position (name invented, as in game/Em209BigGolem.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/CharaC58_unk.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" int rand();
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: attack hit (as in game/Em303KrakenFoot.cpp)

Em303KrakenFootMp::Em303KrakenFootMp(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14DC(0), m14E0(0), m14E4(0), m14EC(20.0f), m14F0(0), m14F4(0), m14F8(0),
      m14FC(0), m1500(0), m1504(0), m1508(0), m150C(1), m1520(0) {
    mStatus.m18 |= 0x40;
    mStatus.m18 |= 0x1000000;
    mStatus.mParamB.m9C |= 0x200;
    char path[0x100];
    const char* name = strstr(info->m2C, "db_name=");
    if (name != 0) {
        sprintf(path, "/database/chara/special/%s\0", name + 8);
    } else {
        sprintf(path, "/database/chara/special/db_em303_00_00.csv\0");
    }
    m14D4.vf0C(path);
    m1510.fn_8023780C("/effect/ef_bd006.efp");
}

void Em303KrakenFootMp::fn_802E5B30(int value) {
    m14F0 = value;
}

// inferred: helpers of the state functions (names invented, as in game/Em303KrakenFoot.cpp)
inline void Em303KrakenFootMp::SetState_unk(int state) {
    m14DC = 0;
    m14E0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}
inline void Em303KrakenFootMp::PlayMotion_unk(int motion) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, motion, 0, 1, 1, 0.0f, 0.1f);
}

inline void Em303KrakenFootMp::FaceTarget_unk() {
    CharaTask* target = (CharaTask*)m14F4;
    atn::Vector3 d;
    float z = target->mPos.z - mPos.z;
    d.x = target->mPos.x - mPos.x;
    d.z = z;
    d.y = 0.0f;
    float y = d.ToRotationZ_unk().y;
    if (!(mMotion.mTracks[0].m08 < 10.0f)) {
        mRot.y = y;
    }
}

// inferred: back to the idle state (name invented)
inline void Em303KrakenFootMp::Reset_unk() {
    m14DC = 0;
    m14E0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
}

// NONMATCHING: 99.6% - the flags 0x2A (status) and 0x200 (m54C): the original loads the status word first
// and stores it first; either order of the two statements gets one of them wrong (8 variants)
int Em303KrakenFootMp::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14D4.fn_80473F50() != 0) {
        return 0;
    }
    if (m1510.fn_80237874() != 0) {
        return 0;
    }
    fn_80087994(m151C.Load("Em303KrakenFootMp"), "DebugDisp", &m1520, 0, 0);
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    mStatus.m18 |= 0x1C0;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC21D;
    mStatus.m18 |= 0x3800000;
    fn_802E5CD0((const char*)m14D4.fn_80470580(), m14D4.fn_8047059C());
    m54C |= 0x200;
    mStatus.mParamB.m9C |= 0x2A;
    mRoute.mA0 = 1;
    m14DC = 0;
    m14E0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    return 1;
}

void Em303KrakenFootMp::fn_802E5CD0(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "SwingSkillId") == 0) {
            m14F8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "PoundSkillId") == 0) {
            m14FC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "YOffset") == 0) {
            m14EC = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "BasePoint") == 0) {
            m1504 = lbl_8087F430->m10D8->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
        }
    } while (csv.fn_8005BBF8());
}

// inferred: capsule around the node "reference" (name invented, as in game/Em303KrakenFoot.cpp)
void Em303KrakenFootMp::UpdateColli_unk() {
    atn::Vector3 base = mPos;
    MtxPtr ref = mMotion.GetNodeMtx_unk("reference");
    if (ref != 0) {
        base = GetNodePos_unk(ref);
    }
    m620 = m5B0;
    m614 = base + m5A4;
    m614.y += m620;
    atn::Vector3 a = m614;
    atn::Vector3 top = a;
    top.y += m5B4 - 2.0f * m620;
    mColli.m3C = a;
    mColli.m48 = top;
    mColli.m54 = m620;
}

// NONMATCHING: 99.23% - registers of the two inlined CanAct_unk tests (r0/r4/r6 rotated, as in
// Em303KrakenFoot::vf40) and r3/r4 of the copy of the "reference" node position (__VecCopy family)
void Em303KrakenFootMp::vf10() {
    bool act = CanAct_unk();
    if (act) {
        m1508 = 0;
    }
    if (!mFlag2) {
        if (CanAct_unk() && m58C != 0 && vfF8() == 0) {
            vf100(0);
        }
        vf104();
    } else {
        if ((mStatus.m0C & 0x20) == 0x20 && m58C != 2) {
            vf44();
        }
        if (m58C == 2) {
            vfEC();
        }
    }
    fn_8014C540();
    fn_80145334();
    UpdateColli_unk();
}

void Em303KrakenFootMp::vf14() {
    if (m58C == 12) {
        return;
    }
    fn_80149A30();
}

void Em303KrakenFootMp::vfFC() {
    Reset_unk();
}

// inferred: starts a state (7: back to the base point, 8/9: attacks towards m14F4, 2: damage)
void Em303KrakenFootMp::vf100(int state) {
    if (mC58.mC4 == 0) {
        return;
    }
    switch (state) {
    case 7:
        m14DC = 0;
        m14E0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 7;
        fn_8016E970(3);
        fn_8016E4C4(1);
        m1508 = 0;
        mColli.mFlags |= 1;
        vf98();
        if (m1504 != 0) {
            mPos = m1504->mPos;
            mRot.y = m1504->m14;
        }
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x2D, 0, 1, 1, 1.0f, 0.1f);
        break;
    case 8: {
        SetState_unk(8);
        PlayMotion_unk(0x142);
        FaceTarget_unk();
        break;
    }
    case 9: {
        SetState_unk(9);
        PlayMotion_unk(0x14D);
        FaceTarget_unk();
        break;
    }
    case 2:
        fn_802E8084(2);
        break;
    default:
        Reset_unk();
        break;
    }
}

// inferred: state machine of the foot
// NONMATCHING: 99.82% - stack slots of the temporaries of the two inlined movement blocks (written out with a
// goto; with the helper Move_unk they get other FPRs, 99.0%)
void Em303KrakenFootMp::vf104() {
    m14F4 = (int)mC58.mC4;
    if (m14F4 == 0) {
        m14F4 = (int)lbl_8087F8A0->mHead;
        mC58.mC4 = (CharaTask*)m14F4;
    }
    m14E0++;
    if (mC58.mC0 == 0 || m14F4 == 0) {
        switch (m58C) {
        case 2:
            vfEC();
            break;
        case 12:
            mPos.y = -10000.0f;
            break;
        default:
            fn_8016E970(3);
            break;
        }
    } else {
        float frame;
        switch (m58C) {
        case 7:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                Reset_unk();
                m14E4 = 0;
            }
            break;
        case 8:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                if (m14E4 == 0) {
                    SetState_unk(10);
                    m1500 = 10;
                } else {
                    Reset_unk();
                }
            } else if (mMotion.mTracks[0].m08 >= 19.0f && mMotion.mTracks[0].m08 <= 23.0f) {
                int id = m590;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, m14F8, id, 30, -1, 0.0f);
            }
            break;
        case 9:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                Reset_unk();
            } else if (mMotion.mTracks[0].m08 >= 24.0f && mMotion.mTracks[0].m08 <= 28.0f) {
                int id = m590;
                HitInfo_unk* data = m14FC;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
            }
            break;
        case 10:
            if (m55C != 6) {
                fn_8016E970(3);
            }
            {
                float speed = 0.0f;
                float rate = 1.0f;
                atn::Vector3 rot = mRot;
                if (m55C == 7) {
                    mRoute.fn_80126214();
                    atn::Vector3 v = mRoute.m58;
                    speed = v.Length_unk();
                    if (speed > 0.0001f) {
                        rot = v.GetNormalized_unk().ToRotation_unk();
                    }
                } else if (m55C == 6) {
                    fn_8013A258();
                    goto moved1;
                }
                fn_8013CB68(&rot, 0, speed, rate * m568);
            }
            moved1:
            if (m14E0 >= m1500) {
                Reset_unk();
            }
            break;
        case 2:
            vfEC();
            break;
        case 12:
            mPos.y = -10000.0f;
            break;
        default:
            if (m55C != 6) {
                if (m55C == 7) {
                    if (m14E4 == 1 && (CharaTask*)m14F4 != mRoute.mTarget) {
                        fn_8016E970(3);
                    }
                } else {
                    switch (m14E4) {
                    case 1:
                        fn_80170A20((CharaTask*)m14F4, 0, -1.0f);
                        break;
                    case 3:
                        if (m1504 != 0) {
                            fn_8017039C(m1504->mId, 0);
                        }
                        break;
                    }
                }
            }
            {
                float speed = 0.0f;
                float rate = 1.0f;
                atn::Vector3 rot = mRot;
                if (m55C == 7) {
                    mRoute.fn_80126214();
                    atn::Vector3 v = mRoute.m58;
                    speed = v.Length_unk();
                    if (speed > 0.0001f) {
                        rot = v.GetNormalized_unk().ToRotation_unk();
                    }
                } else if (m55C == 6) {
                    fn_8013A258();
                    goto moved2;
                }
                fn_8013CB68(&rot, 0, speed, rate * m568);
            }
            moved2:
            fn_802E7740();
            break;
        }
    }
    mPos.y = m14EC;
}

void Em303KrakenFootMp::vf108() {}

void Em303KrakenFootMp::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C != 7 && msg->m44 != 1) {
        fn_80151448(msg);
        if (!mFlag2) {
            vf40(hit);
        } else {
            vf118(hit);
        }
    }
}

void Em303KrakenFootMp::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    int heavy = 0;
    if (msg->mData->m01 == 0) {
        heavy = 1;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (!m1508) {
            if (heavy) {
                fn_802E8084(1);
            } else {
                fn_802E8084(2);
            }
        }
    } else {
        m14DC = 0;
        m14E0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 11;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x33, 0, 1, 1, 0.0f, 0.1f);
    }
}

void Em303KrakenFootMp::vf54() {
    m624.clear();
    Item14_unk item;
    item.m04.mRadius = m5B0;
    item.m04.mCenter = mPos + m5A4;
    bool up = (mStatus.mParamB.m9C & 0x40U) == 0x40U;
    if (up) {
        item.m04.mCenter.y -= item.m04.mRadius;
    } else {
        item.m04.mCenter.y += item.m04.mRadius;
    }
    m624.push_back(item);
}

// inferred: action state machine (m14E4): attacks the target when it is near, else walks to it and
// back to the base point
void Em303KrakenFootMp::fn_802E7740() {
    switch (m14E4) {
    case 0:
        if (m14E0 > 30) {
            atn::Vector3 d = ((CharaTask*)m14F4)->mPos - mPos;
            d.y = 0.0f;
            if (d.Length_unk() < 30.0f) {
                SetState_unk(8);
                PlayMotion_unk(0x142);
                FaceTarget_unk();
            } else {
                m14E4 = 1;
                if (rand() % 2 == 0) {
                    m14E8 = 8;
                } else {
                    m14E8 = 9;
                }
            }
        }
        break;
    case 1: {
        CharaTask* target = (CharaTask*)m14F4;
        if (target != 0) {
            float dist = (target->mPos - mPos).Length_unk();
            switch (m14E8) {
            case 8:
                if (dist < 60.0f) {
                    SetState_unk(8);
                    PlayMotion_unk(0x142);
                    FaceTarget_unk();
                    m14E4 = 2;
                }
                break;
            case 9:
                if (dist < 60.0f) {
                    SetState_unk(9);
                    PlayMotion_unk(0x14D);
                    FaceTarget_unk();
                    m14E4 = 2;
                }
                break;
            }
        }
        break;
    }
    case 2:
        m14E4 = 3;
        break;
    case 3:
        if (m1504 != 0) {
            atn::Vector3 d = m1504->mPos - mPos;
            d.y = 0.0f;
            if (d.Length_unk() < 5.0f) {
                SetState_unk(10);
                m1500 = 5;
                m14E4 = 0;
            }
        } else {
            SetState_unk(10);
            m1500 = 5;
            m14E4 = 0;
        }
        break;
    }
}

void Em303KrakenFootMp::fn_802E8084(int kind) {
    m150C = kind;
    m1508 = 1;
    m14DC = 0;
    m14E0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_8016E970(6);
    m560 = 23;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    switch (kind) {
    case 1: {
        mMotion.fn_80097C08(0, 0x2F, 0, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(0, 0);
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
        fn_8023A8B4(lbl_8087F3C0, &m1510, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
        break;
    }
    case 2:
        mMotion.fn_80097C08(0, 0x30, 0, 0, 1, 0.0f, 0.1f);
        break;
    case 3:
        mMotion.mTracks[0].m0C = 0.5f;
        mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
        break;
    }
}

void Em303KrakenFootMp::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
        } else {
            mFlag15 = 1;
            fn_801765D8();
        }
    } else {
        m1434 = 30;
    }
}

void Em303KrakenFootMp::vf24() {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 1.0f;
}

void Em303KrakenFootMp::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    m14DC = 0;
    m14E0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 7;
    fn_8016E970(3);
    fn_8016E4C4(1);
    m1508 = 0;
    mColli.mFlags |= 1;
    vf98();
    if (m1504 != 0) {
        mPos = m1504->mPos;
        mRot.y = m1504->m14;
    }
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2D, 0, 1, 1, 1.0f, 0.1f);
}

void Em303KrakenFootMp::vf110(SpecialState_unk* state) {
    state->m00 = m58C;
    state->m04 = m14BC;
    *(int*)&state->m08 = m150C;
}

void Em303KrakenFootMp::vf114(const SpecialState_unk* state) {
    int s = state->m00;
    m150C = *(const int*)&state->m08;
    if (s == 2 && !m1508) {
        vf44();
        fn_802E8084(m150C);
    } else {
        m58C = s;
        m14BC = state->m04;
    }
}

Em303KrakenFootMp::~Em303KrakenFootMp() {}
