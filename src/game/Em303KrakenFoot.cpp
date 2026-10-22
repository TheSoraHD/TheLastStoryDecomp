#include "game/Em303KrakenFoot.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaStatus.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: bullet/skill data by id
// inferred: field (lbl_8087F430): route tables at 0x10D8 (as in game/CharaC58_unk.cpp)
struct Field_unk {
    unsigned char m0000[0x10D8];
    RouteTable_unk* m10D8;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
// inferred: next action of a leg chosen by the kraken {kind, argument} (Em302Kraken, without source)
struct FootAction_unk {
    int mKind;
    int mArg;
};
extern "C" FootAction_unk* fn_802DC8B0(void* kraken);
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: ray test result (as in game/Em101LittleZesya.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04;
    atn::Vector3 m10; // inferred: hit position
    unsigned char m1C[0x28 - 0x1C];
    atn::Vector3 m28; // inferred: offset added to the hit position
    int m34;
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (as in game/Em303KrakenFootMp.cpp)
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
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f); // inferred: attack hit (as in game/Em201ZanGurg.cpp)
// inferred: node position (name invented, as in game/Em303KrakenFootMp.cpp)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

Em303KrakenFoot::Em303KrakenFoot(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C8(20.0f), m14CC(0), m14D0(0), m14D8(0),
      m14DC(0), m14E0(0), m14E4(0.0f), m14E8(0), m14EC(0), m14F0(-1), m14F4(0), m1504(0), m150C(0) {
    mStatus.mParamB.m9C |= 0x200;
    char path[0x100];
    const char* name = strstr(info->m2C, "db_name=");
    if (name != 0) {
        sprintf(path, "/database/chara/special/%s\0", name + 8);
    } else {
        sprintf(path, "/database/chara/special/db_em303_00_00.csv\0");
    }
    m14B0.vf0C(path);
    m14F8.fn_8023780C("/effect/ef_bd006.efp");
}

Em303KrakenFoot::~Em303KrakenFoot() {}

extern "C" int fn_802E2328(Em303KrakenFoot* foot) {
    if ((foot->mStatus.m0C & 0x20) == 0x20) {
        return 1;
    }
    bool r = false;
    if (foot->m14C0 != 1 && foot->m14C0 != 2) {
        r = true;
    }
    return r;

}

// inferred: names invented
inline bool InRange_unk(float v, float lo, float hi) {
    return lo < v && v < hi;
}
inline float GetFrame_unk(Em303KrakenFoot* foot) {
    return foot->mMotion.mTracks[0].m08;
}

extern "C" float fn_802E2360(Em303KrakenFoot* foot) {
    float result = 0.0f;
    atn::Vector3 d = lbl_8087F8A0->mHead->mPos - foot->mPos;
    float rate = (60.0f - PSVECMag(&d)) / 60.0f;
    if (rate > 0.0f) {
        int s = foot->m58C;
        if (s == 2 && foot->mMotion.mTracks[0].m00 == 0x2F && 10.0f < GetFrame_unk(foot) &&
            GetFrame_unk(foot) < 20.0f) {

            result += 0.5f * rate;
        }
        if (s == 13 && foot->mMotion.mTracks[0].m08 >= 25.0f && foot->mMotion.mTracks[0].m08 <= 34.0f) {
            result += 0.5f * rate;
        }
    }
    return result;
}

extern "C" void fn_802E2468(Em303KrakenFoot* foot, int value) {
    foot->m1504 = value;
}

// NONMATCHING: 99.7% - the flags 0x2A (status) / 0x200 (m54C): load and store order (as in
// Em303KrakenFootMp::vf0C)
int Em303KrakenFoot::vf0C() {
    if (fn_8013655C() != 0) {
        return 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        return 0;
    }
    if (m14B0.fn_80473F50() != 0) {
        return 0;
    }
    if (m14F8.fn_80237874() != 0) {
        return 0;
    }
    fn_80087994(m1508.Load("Em303KrakenFoot"), "DebugDisp", &m150C, 0, 0);
    if (m1438 != 0) {
        m1438->fn_800D246C(0);
        m1438->mFlags |= 4;
    }
    mStatus.m18 |= 0x1C0;
    mStatus.m18 |= 0x10000;
    mStatus.m18 |= 0xC21D;
    mStatus.m18 |= 0x3800000;
    fn_802E2640((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
    m14D4 = mMotion.GetNodeMtx_unk("catch_point");
    m54C |= 0x200;
    mStatus.mParamB.m9C |= 0x2A;
    mRoute.mA0 = 1;
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    return 1;
}

void Em303KrakenFoot::fn_802E2640(const char* text, int length) {
    atn::Csv csv(text, length);
    do {
        const char* key = csv.fn_8005B9CC();
        if (strcmp(key, "SwingSkillId") == 0) {
            m14D8 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "PoundSkillId") == 0) {
            m14DC = fn_80219E6C(atoi(csv.fn_8005B9CC()));
        } else if (strcmp(key, "HoldDamageInterval") == 0) {
            m14E0 = atoi(csv.fn_8005B9CC());
        } else if (strcmp(key, "CatchDistance") == 0) {
            m14E4 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "YOffset") == 0) {
            m14C8 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(key, "BasePoint") == 0) {
            m14E8 = lbl_8087F430->m10D8->FindPlace_unk(fn_800DC12C(csv.fn_8005B9CC()));
        } else if (strcmp(key, "CatchNotifyFlag") == 0) {
            m14F0 = *csv.fn_8005B9CC() == 'G';
            m14F4 = atoi(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8());
}



// inferred: capsule around the node "reference" (name invented)
void Em303KrakenFoot::UpdateColli_unk() {
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

// NONMATCHING: 99.63% - only r3/r4 of the copy of the "reference" node position in UpdateColli_unk
// (__VecCopy family, documented blocker; 5 forms tried)
void Em303KrakenFoot::vf10() {
    fn_802E3604();
    m14BC++;
    if (mC58.mC0 == 0 || m14CC == 0) {
        fn_8016E970(3);
    } else {
        float frame;
        switch (m58C) {
        case 7:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
                m14C0 = 0;
            }
            break;
        case 8:
            fn_802E3EE8();
            break;
        case 9:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
            }
            break;
        case 11:
            fn_802E470C();
            break;
        case 10:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
            }
            break;
        case 12:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                if (m14C0 == 0) {
                    SetState_unk(15);
                    m14EC = 10;
                } else {
                    SetState_unk(0);
                }
            } else if (mMotion.mTracks[0].m08 >= 19.0f && mMotion.mTracks[0].m08 <= 23.0f) {
                int id = m590;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, m14D8, id, 30, -1, 0.0f);
            }
            break;
        case 13:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
            } else if (mMotion.mTracks[0].m08 >= 24.0f && mMotion.mTracks[0].m08 <= 28.0f) {
                int id = m590;
                HitInfo_unk* data = m14DC;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
            }
            break;
        case 14:
            frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                SetState_unk(0);
            } else if (mMotion.mTracks[0].m08 >= 19.0f && mMotion.mTracks[0].m08 <= 22.0f) {
                int id = m590;
                HitInfo_unk* data = m14DC;
                fn_800F8C6C(lbl_8087F048, 0, 0, this, data, id, 30, -1, 0.0f);
            }
            break;
        case 15:
            if (m55C != 6) {
                fn_8016E970(3);
            }
            fn_802E37DC();
            if (m14BC >= m14EC) {
                SetState_unk(0);
            }
            break;
        case 2:
            vfEC();
            break;
        default:
            if (m55C != 6) {
                if (m55C == 7) {
                    if (m14C0 == 1 && (CharaTask*)m14CC != mRoute.mTarget) {
                        fn_8016E970(3);
                    }
                } else {
                    switch (m14C0) {
                    case 1:
                        fn_80170A20((CharaTask*)m14CC, 0, -1.0f);
                        break;
                    case 3:
                        if (m14E8 != 0) {
                            fn_8017039C(m14E8->mId, 0);
                        }
                        break;
                    }
                }
            }
            fn_802E37DC();
            fn_802E3B24();
            break;
        }
    }
    mPos.y = m14C8;
    fn_8014C540();
    fn_80145334();
    UpdateColli_unk();
}

void Em303KrakenFoot::vf14() {
    fn_80149A30();
}

void Em303KrakenFoot::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 7 || m58C == 10) {
        return;
    }
    if (msg->m44 == 1 && msg->mData != 0 && msg->mData->m04 != 0x1FB && msg->mData->m04 != 0x1FC) {
        return;
    }
    fn_80151448(msg);
    vf40(hit);
}

// inferred: CharaTask::CanAct_unk written out (names invented)
// inferred: helpers of the state functions (names invented)
inline void Em303KrakenFoot::SetState_unk(int state) {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}
inline void Em303KrakenFoot::PlayMotion_unk(int motion) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, motion, 0, 1, 1, 0.0f, 0.1f);
}

inline bool FootActive_unk(CharaTask* c) {
    bool r = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        r = true;
    }
    return r;
}
inline bool FootFree_unk(CharaTask* c) {
    bool r = false;
    if (FootActive_unk(c) && !c->mStatus.IsState_unk(0x20)) {
        r = true;
    }
    return r;
}
inline bool FootGrappled_unk(CharaTask* c) {
    return c->m55C == 6 && c->m560 == 28;
}
inline bool FootCanAct_unk(CharaTask* c) {
    bool r = false;
    if (FootFree_unk(c) && !FootGrappled_unk(c)) {
        r = true;
    }
    return r;
}

// inferred: releases the caught character and puts it back on the ground (name invented)
inline void Em303KrakenFoot::Release_unk() {
    if (m14D0 != 0) {
        if (FootCanAct_unk(m14D0)) {
            m14D0->fn_8015AC48(-1);
        }
        if (m14D0 != 0) {
            m14D0->mF1C = 0;
            atn::Vector3 to = m14D0->mPos;
            atn::Vector3 from = mPos;
            from.y = to.y;
            CollResult_unk result;
            if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0) {
                result.m10 += result.m28;
                m14D0->mPos = result.m10;
            }
            m14D0 = 0;
        }
    }
}

// NONMATCHING: 99.33% - registers of the inlined CanAct_unk of the caught character (r0/r4/r6 rotated; same
// family as Em314Ivy::vf104 and Em309GhoulSimple::vf10; ~12 forms of the inline tried)
void Em303KrakenFoot::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    int heavy = 0;
    if (msg->mData->m01 == 0) {
        heavy = 1;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (heavy) {
            fn_802E55B0(1);
        } else {
            fn_802E55B0(2);
        }
    } else {
        m14B8 = 0;
        m14BC = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 16;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x33, 0, 1, 1, 0.0f, 0.1f);
        if (m14D0 != 0) {
            if (FootCanAct_unk(m14D0)) {
                m14D0->fn_8015AC48(-1);
            }
            if (m14D0 != 0) {
                m14D0->mF1C = 0;
                atn::Vector3 to = m14D0->mPos;
                atn::Vector3 from = mPos;
                from.y = to.y;
                CollResult_unk result;
                if (fn_8004ED34(lbl_8087EE98, &result, &from, &to, 0x80000000, 0, 0) != 0) {
                    result.m10 += result.m28;
                    m14D0->mPos = result.m10;
                }
                m14D0 = 0;
            }
        }
    }
}

void Em303KrakenFoot::vf54() {
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

// inferred: chooses the target (m14CC): the leader while he is free, else the nearest player (only when
// m14C0 is 0)
void Em303KrakenFoot::fn_802E3604() {
    mC58.mC8 = (int)mC58.mC4;
    if (m58C - 8U <= 3) {
        m1454 = 0;
    } else {
        PlayerTask* leader = lbl_8087F8A0->mHead;
        if (leader->mFlag28 && !(leader->m55C == 6 && leader->m560 == 17)) {
            m1454 = 0;
            m14CC = (int)leader;
        } else if (m14C0 == 0) {
            m1454 = 0;
            float best = 1e16f;
            PlayerTask* p;
            PlayerTask* found = 0;
            for (p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                if (p->CanAct_unk() && (p->m54C & 0x2000) != 0x2000) {
                    atn::Vector3 d = p->mPos - mPos;
                    float dist = fn_805F9920(&d);
                    if (dist < best) {
                        found = p;
                        best = dist;
                    }
                }
            }
            m14CC = (int)found;
        } else {
            m1454 = 1;
            m14CC = (int)mC58.mC4;
        }
    }
    mC58.mC4 = (CharaTask*)m14CC;
}

// inferred: movement along the route; in state 11 it stops near the kraken (m1504)
void Em303KrakenFoot::fn_802E37DC() {
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
        if (m58C == 11) {
            CharaTask* kraken = (CharaTask*)m1504;
            atn::Vector3 d = kraken->mPos - mPos;
            d.y = 0.0f;
            if (d.Length_unk() < 70.0f) {
                speed = 0.0f;
                rate = speed;
                rot = mRot;
            }
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8013CB68(&rot, 0, speed, rate * m568);
}

// inferred: action state machine (m14C0): asks the kraken for the next action and starts it near the target
void Em303KrakenFoot::fn_802E3B24() {
    switch (m14C0) {
    case 0: {
        int idle = 1;
        if (m1504 != 0) {
            FootAction_unk* action = fn_802DC8B0((void*)m1504);
            if (action != 0) {
                if (action->mKind == 15) {
                    fn_802E554C(action->mArg);
                    m14C0 = 0;
                } else {
                    m14C4 = action->mKind;
                    idle = 0;
                    m14C0 = 1;
                }
            }
        }
        if (idle && m14BC > 30) {
            atn::Vector3 d = *((CharaTask*)m14CC)->fn_8013C38C() - mPos;
            d.y = 0.0f;
            if (d.Length_unk() < 30.0f) {
                fn_802E4D6C();
            }
        }
        break;
    }
    case 1: {
        CharaTask* target = (CharaTask*)m14CC;
        if (target != 0) {
            float dist = (*target->fn_8013C38C() - mPos).Length_unk();
            switch (m14C4) {
            case 12:
                if (dist < 60.0f) {
                    fn_802E4D6C();
                    m14C0 = 2;
                }
                break;
            case 13:
                if (dist < 60.0f) {
                    fn_802E500C();
                    m14C0 = 2;
                }
                break;
            case 14:
                if (dist < 60.0f) {
                    fn_802E52AC();
                    m14C0 = 2;
                }
                break;
            case 8:
                if (dist < m14E4) {
                    fn_802E3E38(m14CC);
                    m14C0 = 2;
                }
                break;
            }
        }
        break;
    }
    case 2:
        m14C0 = 3;
        break;
    case 3:
        if (m14E8 != 0) {
            atn::Vector3 d = m14E8->mPos - mPos;
            d.y = 0.0f;
            if (d.Length_unk() < 5.0f) {
                fn_802E554C(5);
                m14C0 = 0;
            }
        } else {
            fn_802E554C(5);
            m14C0 = 0;
        }
        break;
    }
}

void Em303KrakenFoot::fn_802E3D60() {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 7;
    fn_8016E970(3);
    fn_8016E4C4(1);
    mColli.mFlags |= 1;
    vf98();
    RoutePlace_unk* base = m14E8;
    if (base != 0) {
        mPos = base->mPos;
        mRot.y = base->m14;
    }
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2D, 0, 1, 1, 1.0f, 0.1f);
}

void Em303KrakenFoot::fn_802E3E38(int arg) {
    if (arg != 0) {
        m14B8 = 0;
        m14BC = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 8;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x13F, 0, 1, 1, 0.0f, 0.1f);
        m14CC = arg;
        m14D0 = 0;
    }
}

void Em303KrakenFoot::fn_802E4D6C() {
    SetState_unk(12);
    PlayMotion_unk(0x142);
    CharaTask* target = (CharaTask*)m14CC;
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

void Em303KrakenFoot::fn_802E500C() {
    SetState_unk(13);
    PlayMotion_unk(0x14D);
    CharaTask* target = (CharaTask*)m14CC;
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

void Em303KrakenFoot::fn_802E52AC() {
    SetState_unk(14);
    PlayMotion_unk(0x145);
    CharaTask* target = (CharaTask*)m14CC;
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

void Em303KrakenFoot::fn_802E554C(int arg) {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    m14EC = arg;
}

// NONMATCHING: 99.60% - registers of the inlined CanAct_unk (see vf40)
void Em303KrakenFoot::fn_802E55B0(int kind) {
    m14B8 = 0;
    m14BC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    fn_8016E970(3);
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
        EffectMgr_unk* mgr = lbl_8087F3C0;
        fn_8023A8B4(mgr, &m14F8, &mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
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
    Release_unk();
}

void Em303KrakenFoot::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            m1434--;
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 30;
    }
}

void Em303KrakenFoot::vf24() {
    if (m58C == 11) {
        return;
    }
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.1f);
    mMotion.mTracks[0].m0C = 1.0f;
}

