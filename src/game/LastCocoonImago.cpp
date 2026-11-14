#include "game/LastCocoonImago.h"
#include "chr/ControlBase.h"
#include "game/GameMessageList.h"
#include "game/GimmickManager.h"
#include "game/GameTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/GameUI.h"
#include "game/GameConfig_unk.h"
#include "chr/ControlStickByQuark.h"
#include <boost/tokenizer.hpp>
#include "std/string_members.h"
#include "std/string.h"
#include "macros.h"
#include "atn/DrawHelper.h"
#include "gmk/Lemon.h"
#include "atn/Camera_unk.h"
#include "atn/RenderManager.h"

// inferred: effect/bullet manager (lbl_8087F048); only the member used here (name invented)
struct Effect_unk {
    unsigned char m00000[0x3E2E8];
    void* m3E2E8; // inferred: sphere of the current target (compared with m18FC)
};
extern "C" Effect_unk* lbl_8087F048;
// inferred: field (lbl_8087F430); only the member used here
struct Field_unk {
    unsigned char m0000[0x6C];
    atn::Camera m006C; // inferred: camera of the field (as in game/EnemyQuarkStand.cpp)
    unsigned char m0260[0x10D8 - 0x260];
    RouteTable_unk* m10D8; // inferred: route table of the map
    unsigned char m10DC[0x54F4 - 0x10DC];
    unsigned char m54F4[4]; // inferred: name table (fn_803B3B38)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" atn::Vector3 lbl_807C7030;        // inferred: zero vector
extern "C" int fn_80051B14(const atn::ColliSphereShape_unk* s, const atn::ColliCapsuleShape_unk* c); // inferred: hit test

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b); // inferred: a + b

// NONMATCHING: the original is over the inlining budget (docs/inlining.md R3) and calls every inline
// (LH_Binary, Vector3, LH_Xsca and ResHandle constructors, the strings, the tokenizer); this one stays under it
// and inlines them (same problem as the constructor of LastCocoonChrysalis, docs/close-one-func.md)
LastCocoonImago::LastCocoonImago(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14D0(5), m18C8(1), m18CC(1),
      m18D0(0), m18EC(0), m18F0(0), m1960(0), m1978(0), m197C(0), m1984(0), m1988(40.0f) {
    mFlag22 = 1;
    std::string dir("database/chara/special/");
    std::string name("db_last_cocoon.csv");
    std::string params(info->m2C);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(params, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "db_name=", 8) == 0) {
            name = it->c_str() + 8;
        }
    }
    std::string path = fn_8006AFF8(dir, name);
    m14B0.vf0C(path.c_str());
    m14F0.LoadRequest("ef_sk023a.efp");
    m18F8 = 0;
    m1948 = lbl_807C7030;
    m1954 = 30.0f;
    m1958 = 0;
    m195C = 0;
    m1964.vf0C("camera/battle/cm000_em206_00.xsca");
    m196C.LoadRequest("ef_uc030.efp");
    mFlagC31 = 1;
}

extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
// inferred: RouteTable_unk::FindPlace_unk with the array read inside the loop (as in
// game/LastCocoonQuadruped.cpp)
inline RoutePlace_unk* FindImagoPlace_unk(RouteTable_unk* table, int id) {
    for (unsigned int i = 0; i < table->mNumPlaces; i++) {
        RoutePlace_unk* places = table->mPlaces;
        if (id == places[i].mId) {
            return &places[i];
        }
    }
    return 0;
}

int LastCocoonImago::vf0C() {
    if (fn_8013655C() == 0 && m14F0.IsLoading() == 0 && m14B0.fn_80473F50() == 0 && m1964.fn_80473F50() == 0 &&
        m196C.IsLoading() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC219;
        mStatus.m18 |= 0x3800000;
        fn_80087994(m1980.Load("LastCocoonImago"), "DebugDisp", &m1984, 0, 0);
        if (m14B0.fn_80470580() != 0 && lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
            m18F4 = FindImagoPlace_unk(lbl_8087F430->m10D8, 2);
        }
        m56C = 0.02f;
        m5B0 = 25.0f;
        m5B4 = 120.0f;
        for (int i = 0; i < 11; i++) {
            mCapsules[i].mFlags |= 3;
            mCapsules[i].m0C = (int)this;
            mCapsules[i].mFlags &= ~0x80000000;
        }
        m18FC.mFlags |= 3;
        m18FC.m0C = (int)this;
        vf20();
        ResetAttacks_unk();
        return 1;
    }
    return 0;
}

// inferred: effect manager (class unknown), global instance at 0x8087F3C0 (as in game/LastCocoonEgg.cpp)
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, void* model, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const Color4f_unk& color, int arg7, int arg8, int arg9,
                            float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" double sqrt(double x);
// inferred: plays an effect on the model with the default color and scale (as in game/LastCocoonEgg.cpp)
static inline void PlayEfp_unk(atn::EfpFile* efp, void* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    rot.Set_unk(0.0f, 0.0f, 0.0f);
    pos.Set_unk(0.0f, 0.0f, 0.0f);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

extern "C" double cos(double x);
// inferred: starts the mode `mode` with the motion `motion` (names invented)
inline void LastCocoonImago::PlayMode_unk(int mode, int motion) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = mode;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, motion, 0, 1, 1, 0.0f, 0.025f);
}
// inferred: mode 10 (lunge, motion 0x14D) along `dir` (name invented)
inline void LastCocoonImago::Lunge_unk(const atn::Vector3& dir) {
    atn::Vector3 v = dir;
    PlayMode_unk(10, 0x14D);
    m14D4 = v;
}
// inferred: forward direction of the imago (name invented)
inline atn::Vector3 LastCocoonImago::Front_unk() {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 rot;
    rot.fn_8013A13C(mRot.y);
    PSMTXMultVec(rot.m, &v, &v);
    return v;
}
// inferred: mode 11 (motion 0x13F): effect m14F0 and the capsules of the body hit `m14B8` (name invented)
inline void LastCocoonImago::Charge_unk() {
    PlayMode_unk(11, 0x13F);
    fn_80232B7C(this, 1000);
    PlayEfp_unk(&m14F0, &mMotion);
    for (int i = 0; i < 11; i++) {
        mCapsules[i].m20 = 4;
    }
    m14FC = m14B8;
}


// inferred: back to the mode 0 (name invented)
inline void LastCocoonImago::EndMode_unk() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
}
// inferred: roar of the mode 6 (motion 0x145 from the frame 30; name invented)
inline void LastCocoonImago::Roar_unk() {
    if (m14BC == 0) {
        if (mMotion.mTracks[0].m08 >= 30.0f) {
            m14BC = 1;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.025f);
        }
    } else if (m14BC == 1) {
        float frame = mMotion.mTracks[0].m08;
        if (frame > 60.0f) {
            ((GameTask*)lbl_8087F430)->fn_80370AE4(0x5A, 1);
        } else if (frame >= mMotion.fn_80097D7C(0)) {
            EndMode_unk();
        }
    }
}

extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data
struct HitResult_unk;
extern "C" int fn_800F8C6C(Effect_unk* mgr, HitResult_unk* results, int count, CharaTask* chara, HitInfo_unk* data,
                           int id, int time, int hits, float angle); // inferred: deals a hit, returns the hits
struct HomingParam_unk;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet
extern "C" GameConfig_unk* lbl_8087F0A8; // inferred: game settings
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// NONMATCHING: 95% - not iterated (registers and the order of some blocks)
// inferred: update: finds the gimmicks 0x6D9C (hang point) and 0x6DD8, keeps the target, runs the mode,
// the arrows of the weak points, the glow, and the fall onto the gimmick m1960
void LastCocoonImago::vf10() {
    if (m18F8 == 0) {
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
            if (g->mId == 0x6D9C && g->m4C == 100) {
                m18F8 = g;
                m1948 = g->m6C;
                break;
            }
        }
    }
    if (m1960 == 0) {
        for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
            if (g->mId == 0x6DD8 && g->m4C == 1) {
                m1960 = g;
                break;
            }
        }
    }
    m14B8 = mC58.mC4;
    if (m14B8 == 0) {
        m14B8 = lbl_8087F8A0->mHead;
        mC58.mC4 = m14B8;
    }
    m14C0++;
    if (mC58.mC0 == 0 || m14B8 == 0) {
        fn_8016E970(3);
        if (m58C == 6) {
            Roar_unk();
        }
    } else {
        switch (m58C) {
        case 6:
            Roar_unk();
            break;
        case 7:
            fn_8045E548();
            break;
        case 8:
            if (mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0)) {
                EndMode_unk();
            }
            fn_8045ABB4(m14E0, 0.0f, m568);
            break;
        case 9: {
            mMotion.mTracks[0].m0C = mStatus.fn_8012DB04();
            float frame = mMotion.mTracks[0].m08;
            if (frame >= mMotion.fn_80097D7C(0)) {
                EndMode_unk();
            } else if (mMotion.mTracks[0].m08 >= 57.0f && mMotion.mTracks[0].m08 <= 65.0f) {
                HitInfo_unk* bullet = fn_80219E6C(0x5B8);
                fn_80144710();
                fn_800F8C6C(lbl_8087F048, 0, 0, this, bullet, m590, 30, -1, 0.0f);
                if (m1984 != 0) {
                    lbl_8087EEB0->fn_80063D3C(mPos, mStatus.mParamB.m28 + bullet->m40, 0.0f, 0xFFFF0000);
                }
            }
            break;
        }
        case 10:
            fn_8045E780();
            break;
        case 11:
            if (m14BC == 0) {
                if (mMotion.mTracks[0].m08 >= 104.0f) {
                    m14BC = 1;
                    m12CC = 0.4f;
                    atn::Vector3 hand = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("righthand"))->GetTrans_unk();
                    atn::Vector3 hips =
                        ((atn::Matrix3x4*)m14FC->mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
                    atn::Vector3 dir = hips - hand;
                    dir.Normalize_unk();
                    Effect_unk* mgr = lbl_8087F048;
                    fn_800F8574(mgr, this, fn_80219E6C(0x5B7), &hand, &dir, 0, 0x24, 0, 0.0f, 1.0f);
                }
            } else {
                if (mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0)) {
                    for (int i = 0; i < 11; i++) {
                        mCapsules[i].m20 = 0;
                    }
                    EndMode_unk();
                }
            }
            break;
        case 12:
            fn_8045D3A4();
            break;
        case 13:
            if (mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0)) {
                Charge_unk();
            }
            break;
        case 14:
            fn_8045DF34();
            break;
        case 15: {
            atn::Vector3 g = m1960->m6C;
            float y = mPos.y;
            mPos = (g - mPos) * 0.1f + mPos;
            mPos.y = y;
            m574.y += -98.1f / (float)(lbl_8087F0A8->m030 * lbl_8087F0A8->m030);
            mPos.y += m574.y;
            if (mPos.y < -20.0f) {
                mPos.y = -20.0f;
                m574.y = 0.0f;
            }
            if (mMotion.mTracks[0].m08 >= mMotion.fn_80097D7C(0)) {
                EndMode_unk();
            }
            break;
        }
        case 2:
            vfEC();
            break;
        default:
            if (m55C != 6) {
                fn_8016E970(3);
                fn_80170A20(m14B8, -1.0f, 0);
            }
            fn_8045B82C();
            if (m55C != 6) {
                fn_8045BFFC();
            }
            break;
        }
        switch (m18CC) {
        case 1:
            if (m18D0 == 0) {
                fn_8045EDEC(m18C8);
            }
            break;
        case 2:
            for (int i = 0; i < 2; i++) {
                fn_8045EDEC(i);
            }
            break;
        }
        fn_8045CB24();
        if (m18EC == 0 && ((GameTask*)lbl_8087F430)->fn_80370A78(0xCE) == 2) {
            m18EC = 1;
        }
        if (m195C > 0 && ++m195C > 150) {
            if (((GameTask*)lbl_8087F430)->fn_80370A78(0xD1) == 0) {
                ((GameTask*)lbl_8087F430)->fn_80370AE4(0xD1, 1);
            }
        }
    }
    if (m197C != 0 && m58C != 15) {
        mPos.y = -20.0f;
    }
    fn_8014C540();
    fn_80145334();
    fn_8045F4CC();
    if (m197C == 0 && m18F4 != 0) {
        atn::Vector3 d(m18F4->mPos.x - mPos.x, 0.0f, m18F4->mPos.z - mPos.z);
        if (d.LengthSq_unk() < 225.0f && m1960->m54 == 2) {
            GimmickMsg_unk gm;
            gm.mType = 3;
            m1960->vf40(&gm);
            PlayMode_unk(15, 0x38);
            m197C = 1;
            m574.y = 0.0f;
        }
    }
}

void LastCocoonImago::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 12 && msg->mData->m04 != 0x5BC) {
        return;
    }
    // inferred: HitMsg_unk+0x40 is the index of the part hit (as in game/Em205Ogre.cpp)
    if (*(int*)&msg->m3C[4] == 3) {
        if (m18F8->m54 == 3 || m18F8->m54 == 4) {
            return;
        }
        if (++m1958 > 3) {
            GimmickMsg_unk gm;
            atn::Vector3 d = mPos - m1948;
            if ((float)sqrt(d.x * d.x + d.z * d.z) < 30.0f) {
                gm.mType = 3;
                m14BC = 0;
                m14C0 = 0;
                m590 = fn_800F8548(lbl_8087F048);
                m58C = 14;
                fn_8016E970(3);
                mMotion.m34C = 1;
                mMotion.mTracks[0].m20 = 1.0f;
                mMotion.mTracks[0].m0C = 1.0f;
                mMotion.fn_80097C08(0, 0x2F, 0, 0, 1, 0.0f, 0.1f);
                m574 = lbl_807C7030;
            } else {
                gm.mType = 4;
                m195C = 1;
            }
            m18F8->vf40(&gm);
            m18FC.mFlags &= ~1;
        } else {
            GimmickMsg_unk gm;
            gm.mType = m1958 % 2 == 0 ? 2 : 6;
            m18F8->vf40(&gm);
            if (((GameTask*)lbl_8087F430)->fn_80370A78(0xD3) == 0) {
                ((GameTask*)lbl_8087F430)->fn_80370AE4(0xD3, 1);
            }
        }
        return;
    }
    msg->m10 *= 0.0f;
    if (m58C == 14 || m58C == 12) {
        msg->m0C |= 0x80;
    } else if (!IsDown_unk() && !msg->mData->m90.Check(0x80000)) {
        msg->m0C |= 8;
    }
    if (*(int*)&msg->m3C[4] == 1) {
        msg->m0C |= 0x80000000;
        msg->m0C &= ~8;
        fn_80232B7C(0, 0);
        PlayEfp_unk(&m196C, &mMotion);
    }
    fn_80151448(msg);
    vf40(hit);
}

// inferred: hit point ratio (weak copy 0x8029F3AC of game/Em203GreaterDaemonRedMp.cpp)
extern "C" inline float fn_8029F3AC(CharaStatus* status) {
    if (status->mParamB.m84 > 0) {
        return status->m04 / status->mParamB.m84;
    }
    return 0.0f;
}
// inferred: starts a knockback mode (7 or 8) pushed along `dir` (name invented)
inline void LastCocoonImago::Knockback_unk(const atn::Vector3& dir, int mode, int motion) {
    atn::Vector3 v = dir;
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = mode;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, motion, 0, 1, 1, 0.0f, 0.025f);
    atn::Vector3 n = v;
    n.Normalize_unk();
    atn::Vector3 push = n;
    push.y *= 0.3f;
    SetM6B8_unk(push);
    atn::Vector3 back(-n.x, -n.y, -n.z);
    m14E0 = back.ToRotation_unk();
}

// NONMATCHING: 97% - stack slots of the two knockbacks (the inline keeps the mode in a register)
// inferred: hit reaction: the attack 0x1791 pushes it (towards the place m18F4 while m18EC is set), the
// hits of part 1 with a power of 3 or more knock it back; asks for help at 70 % of the hit points
void LastCocoonImago::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 12) {
        return;
    }
    if (m58C == 14) {
        if ((mStatus.m0C & 0x20) == 0x20) {
            fn_8045EB60();
        }
        return;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        fn_8045EB60();
        return;
    }
    if (msg->mData->m04 == 0x1791) {
        atn::Vector3 dir = msg->m28 * 0.5f;
        if (m18EC != 0) {
            if (((GameTask*)lbl_8087F430)->fn_80370A78(0xCF) == 0) {
                ((GameTask*)lbl_8087F430)->fn_80370AE4(0xCF, 1);
            }
            dir = m18F4->mPos - mPos;
            dir.y = 0.0f;
            dir.Normalize_unk();
            dir *= 0.5f;
        }
        if (m197C == 0) {
            Knockback_unk(dir, 7, 0x14A);
        }
    } else if (*(int*)&msg->m3C[4] == 1) {
        if (msg->mData->m3C >= 3 && m58C != 7) {
            atn::Vector3 dir(msg->m28.x * 5.0f, 0.0f, msg->m28.z * 5.0f);
            if (m18EC != 0) {
                dir *= 0.1f;
            }
            if (m197C == 0) {
                Knockback_unk(dir, 8, 0x33);
            }
        }
        m18D4[1].mCount = 15;
    }
    if (m18EC == 0 && fn_8029F3AC(&mStatus) < 0.7f) {
        if (((GameTask*)lbl_8087F430)->fn_80370A78(0xCE) == 0) {
            ((GameTask*)lbl_8087F430)->fn_80370AE4(0xCE, 1);
        }
    }
}

atn::Vector3 LastCocoonImago::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (m18D0 != 0 && m58C != 12) {
        result = m624.mData[m18C8].m04.mCenter;
    }
    return result;
}

// inferred: the three attacks of the imago: nodes, two values, a radius and the wait before the first one
// (names invented)
struct ImagoAttack_unk {
    const char* mNode0;
    const char* mNode1;
    const char* mNode2;
    int m0C;
    int m10;
    float mRadius;
    int mWait;
};
static const ImagoAttack_unk sImagoAttacks_unk[3] = {
    {"reference", "", "", -1, -1, 25.0f, 0},
    {"head", "", "em206_body_l", 100, 200, 12.0f, 1000},
    {"spine1", "", "", -1, -1, 30.0f, 1000},
};

// inferred: hit capsules of the body: two nodes and a radius (vf20; names invented)
struct ImagoCapsule_unk {
    const char* mFrom;
    const char* mTo;
    float mRadius;
};
static const ImagoCapsule_unk sImagoCapsules_unk[11] = {
    {"neck", "ef_eye", 8.0f},
    {"spine", "neck", 20.0f},
    {"rightleg", "rightfoot", 10.0f},
    {"rightupleg", "rightleg", 10.0f},
    {"leftleg", "leftfoot", 10.0f},
    {"leftupleg", "leftleg", 10.0f},
    {"righthand", "rightmiddle3", 10.0f},
    {"rightarm", "righthand", 10.0f},
    {"lefthand", "leftmiddle1", 10.0f},
    {"leftarm", "lefthand", 10.0f},
    {"tail1", "tail3", 15.0f},
};

inline void LastCocoonImago::ResetAttacks_unk() {
    for (int i = 0; i < 3; i++) {
        m18D4[i].mWait = sImagoAttacks_unk[i].mWait;
        m18D4[i].mCount = 0;
    }
}

inline void LastCocoonImago::SetMode_unk(int mode) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = mode;
    fn_8016E970(3);
}

// inferred: name of the character, unless the current target is its sphere
int LastCocoonImago::vfB8(wchar_t* name) {
    if (lbl_8087F048 != 0 && lbl_8087F048->m3E2E8 != &m18FC && m5C != 0 && m5C->mC4 > 0 && lbl_8087F430 != 0) {
        GameMessage* entry = ((GameMessageList*)lbl_8087F430->m54F4)->fn_803B3B38(m5C->mC4);
        if (entry != 0) {
            wcscpy(name, entry->mText);
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 98% - stack slots of the temporaries (the zero vector of the Item14_unk constructor)
// inferred: damage spheres: the three attack nodes (between the two nodes when both exist) and m1948
void LastCocoonImago::vf50() {
    Item14_unk item;
    for (int i = 0; i < 3; i++) {
        MtxPtr a = mMotion.GetNodeMtx_unk(sImagoAttacks_unk[i].mNode0);
        MtxPtr b = mMotion.GetNodeMtx_unk(sImagoAttacks_unk[i].mNode1);
        if (a != 0) {
            if (b != 0) {
                atn::Vector3 pb = ((atn::Matrix3x4*)b)->GetTrans_unk();
                atn::Vector3 pa = ((atn::Matrix3x4*)a)->GetTrans_unk();
                item.m04.mCenter = (pb - pa) * 0.6f + pa;
            } else {
                item.m04.mCenter = ((atn::Matrix3x4*)a)->GetTrans_unk();
            }
            item.m04.mRadius = sImagoAttacks_unk[i].mRadius;
            m624.push_back(item);
        }
    }
    item.m04.mCenter = m1948;
    item.m04.mRadius = m1954;
    m624.push_back(item);
}

// NONMATCHING: 98.5% - stack slots of the temporaries of the two branches (and the load order of the nodes)
void LastCocoonImago::vf54() {
    if (m624.mSize != 0) {
        for (int i = 0; i < 3; i++) {
            if (i == 0) {
                atn::Vector3 dir;
                dir.x = 0.0f;
                dir.y = 0.0f;
                dir.z = 1.0f;
                atn::Matrix3x4 rot;
                rot.fn_8013A13C(mRot.y);
                PSMTXMultVec(rot.m, &dir, &dir);
                m624.mData[i].m04.mCenter = mPos - dir * 3.0f;
                m624.mData[i].m04.mCenter.y += 0.5f * sImagoAttacks_unk[i].mRadius;
            } else {
                MtxPtr a = mMotion.GetNodeMtx_unk(sImagoAttacks_unk[i].mNode0);
                MtxPtr b = mMotion.GetNodeMtx_unk(sImagoAttacks_unk[i].mNode1);
                if (a != 0) {
                    if (b != 0) {
                        atn::Vector3 pb = ((atn::Matrix3x4*)b)->GetTrans_unk();
                        atn::Vector3 pa = ((atn::Matrix3x4*)a)->GetTrans_unk();
                        m624.mData[i].m04.mCenter = pa + (pb - pa) * 0.6f;
                    } else {
                        m624.mData[i].m04.mCenter = ((atn::Matrix3x4*)a)->GetTrans_unk();
                    }
                }
            }
        }
        m624.mData[3].m04.mCenter = m1948;
    }
}

extern "C" int fn_8004D388(void* world, atn::ColliResult_unk* result, CharaTask::Sphere_unk* sphere, atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test
extern "C" void* lbl_8087EE98; // inferred: collision world
// inferred: game state (as in game/LastCocoonQuadruped.cpp); names invented
struct ImagoGameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" ImagoGameState_unk* lbl_8087EFA8;

// NONMATCHING: 96% - float registers (dt, speed, move) and stack slots
// inferred: turns towards `rot` and walks (when `move` is over a third) at `speed`, with gravity, the push
// m6B8 and the collision sweep; leans with the turn (m584)
void LastCocoonImago::fn_8045ABB4(const atn::Vector3& target, float move, float speed) {
    if (m197C != 0) {
        move = 0.0f;
    }
    atn::Vector3 pos = mPos;
    atn::Vector3 rot = mRot;
    m14C8 = 0;
    float d = atn::WrapAngle_unk(target.y - rot.y);
    float dt = lbl_8087EFA8->m3A4;
    float step = d * (m56C * (1.0f + 0.75f * atn::Abs_unk(d / 3.1415927f)));
    if (step < 0.034906585f) {
        m14C8 = 1;
    }
    rot.y += step;
    atn::Vector3 vel(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(rot.y);
    PSMTXMultVec(m.m, &vel, &vel);
    if (move > 0.33333334f) {
        vel *= 0.33333334f * speed;
        m14C8 = 1;
    } else {
        vel *= 0.0f;
    }
    if (m58C != 7 && m58C != 8) {
        vf24();
    }
    int fps = lbl_8087F0A8->m030;
    vel.y += 2.0f * (dt * (-98.1f / (float)(fps * fps)));
    vel.x += m6B8.x * MIN(1.0f, dt);
    vel.z += m6B8.z * MIN(1.0f, dt);
    vel.y += m6B8.y;
    atn::Vector3 friction(m6B8.x * MIN(1.0f, 0.19999999f * dt), m6B8.y * MIN(1.0f, 0.19999999f * dt),
                          m6B8.z * MIN(1.0f, 0.19999999f * dt));
    m6B8.x -= friction.x;
    m6B8.z -= friction.z;
    m6B8.y = 0.0f;
    if (m6B8.Length_unk() < 0.1f) {
        m6B8.x = 0.0f;
        m6B8.y = 0.0f;
        m6B8.z = 0.0f;
    }
    if (vel.y < -12.0f) {
        vel.y = -12.0f;
    }
    atn::ColliResult_unk result;
    atn::Vector3 oldPos = pos;
    Sphere_unk sphere;
    sphere.mCenter = m614;
    sphere.mRadius = m620;
    fn_8004D388(lbl_8087EE98, &result, &sphere, &vel, fn_80179D44() | 0x80000000, &mColli, 0, sphere.mRadius);
    pos = result.m10;
    pos -= m5A4;
    pos.y -= sphere.mRadius;
    m634 = result.m34 != 0 ? *(int*)result.m34 : -1;
    atn::Vector3 moved = vel + (pos - oldPos);
    float lean = 0.0f;
    if (PSVECSquareMag(&moved) > 0.001f && move > 0.001f) {
        lean = atn::WrapAngle_unk(moved.GetNormalize().ToRotation_unk().y - mRot.y);
    }
    lean *= 2.0f;
    m584 = MAX(MIN(m584 - 0.5f * lean, 1.0471976f), -1.0471976f) * 0.9f;
    mPos = pos;
    mRot = rot;
    mRot.x = atn::WrapAngle_unk(mRot.x);
    mRot.y = atn::WrapAngle_unk(mRot.y);
    mRot.z = atn::WrapAngle_unk(mRot.z);
    m574 = atn::Vector3(0.0f, 0.0f, 0.0f);
    m630 = 0;
    m610 = 0;
}

void LastCocoonImago::vf24() {
    if (m55C == 6 && mControl && !mControl->vf3C()) {
        return;
    }
    if (m197C) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x3B, 1, 0, 1, 0.0f, 0.025f);
    } else if (m14C8 != 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.025f);
    } else {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.025f);
    }
}

// NONMATCHING: 98.8% - stack slots of the two points and the register of m614.z
// inferred: places the collision capsule, the hit capsules of the body and the sphere
void LastCocoonImago::vf20() {
    m614 = mPos + m5A4;
    m620 = m5B0;
    m614.y += m620;
    atn::Vector3 a = m614;
    atn::Vector3 b = a;
    b.y += m5B4 - 2.0f * m620;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m620;
    for (int i = 0; i < 11; i++) {
        float r = sImagoCapsules_unk[i].mRadius;
        a = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sImagoCapsules_unk[i].mFrom))->GetTrans_unk();
        b = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk(sImagoCapsules_unk[i].mTo))->GetTrans_unk();
        mCapsules[i].m3C = a;
        mCapsules[i].m48 = b;
        mCapsules[i].m54 = r;
    }
    m18FC.mSphere = atn::ColliSphereShape_unk(m1948, m1954);
    m148C = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    m1498 = 13.0f;
}

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// NONMATCHING: 97% - the dead store of toPlace.y and registers
// inferred: walk of the mode 7: towards the place m18F4 while the leader is the target and the place is
// nearer (and ahead), otherwise towards the target; stops near it
void LastCocoonImago::fn_8045B82C() {
    float move = 0.0f;
    atn::Vector3 rot = mRot;
    float speed = 1.0f;
    bool fast = (mStatus.m0C & 8) == 8 || (mStatus.m0C & 0x200) == 0x200;
    if (!fast) {
        speed *= 2.0f;
    }
    if (m55C == 7) {
        int done = 0;
        if (m18EC != 0 && m14B8 == lbl_8087F8A0->mHead) {
            atn::Vector3 toPlace = m18F4->mPos - mPos;
            atn::Vector3 toTarget(m14B8->mPos.x - mPos.x, 0.0f, m14B8->mPos.z - mPos.z);
            toPlace.y = 0.0f;
            if (toPlace.LengthSq_unk() < toTarget.LengthSq_unk()) {
                if (toPlace.LengthSq_unk() > 25.0f) {
                    toPlace.Normalize_unk();
                    toTarget.Normalize_unk();
                    if (PSVECDotProduct(&toPlace, &toTarget) > 0.70710677f) {
                        done = 1;
                        move = 1.0f;
                        rot = toPlace.ToRotation_unk();
                    }
                } else {
                    move = 0.0f;
                    rot = toPlace.ToRotation_unk();
                    done = 1;
                }
            }
        }
        if (done == 0) {
            atn::Vector3 dir = m14B8->mPos - mPos;
            dir.Normalize_unk();
            atn::Vector3 d = m14B8->mPos - mPos;
            move = d.Length_unk() < 60.0f ? 0.0f : 1.0f;
            rot = dir.ToRotation_unk();
        }
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_8045ABB4(rot, move, m568 * speed);
}

// NONMATCHING: 99.8% - frame 0x20 bytes larger (stack slots of the temporaries)
// inferred: choice of the attack: lunge to the place m18F4 when the gimmick m1960 is ready, otherwise after
// a wait the charge (mode 11), the lunge (mode 10) or the bite (mode 9, motion 0x156) by distance and angle
void LastCocoonImago::fn_8045BFFC() {
    if (m14B8 == 0) {
        return;
    }
    if (m197C == 0 && m1960 != 0 && m1960->m54 == 1) {
        atn::Vector3 d(m18F4->mPos.x - mPos.x, 0.0f, m18F4->mPos.z - mPos.z);
        if (d.LengthSq_unk() < 324.0f) {
            atn::Vector3 v = Front_unk();
            PlayMode_unk(10, 0x14D);
            m14D4 = v;
            return;
        }
    }
    bool fast = (mStatus.m0C & 8) == 8 || (mStatus.m0C & 0x200) == 0x200;
    int wait = 120;
    if (!fast) {
        wait = 60;
    }
    if (m18EC != 0) {
        wait *= 2;
    }
    if (m14C0 <= wait) {
        return;
    }
    if (m14D0 >= 5 || m14C0 > wait * 2) {
        if (m197C != 0) {
            Charge_unk();
        } else if (m18EC != 0) {
            if (m18F0 != 0) {
                atn::Vector3 v = Front_unk();
                PlayMode_unk(10, 0x14D);
                m14D4 = v;
                if (((GameTask*)lbl_8087F430)->fn_80370A78(0xD2) == 0) {
                    ((GameTask*)lbl_8087F430)->fn_80370AE4(0xD2, 1);
                }
            } else {
                Charge_unk();
            }
        } else {
            Charge_unk();
        }
        m14D0 = 0;
        return;
    }
    atn::Vector3 target = m14B8->mPos;
    atn::Vector3 dir = target - mPos;
    float dist = dir.Length_unk();
    dir.Normalize_unk();
    atn::Vector3 front = Front_unk();
    float dot = PSVECDotProduct(&front, &dir);
    if (dot >= (float)cos(60.0)) {
        if (dist < 60.0f) {
            atn::Vector3 v = dir;
            PlayMode_unk(9, 0x156);
            m14D4 = v;
            m14D0++;
        } else if (dist < 120.0f) {
            if (m197C == 0) {
                Lunge_unk(dir);
            }
            m14D0++;
        }
    }
}

// NONMATCHING: 98.2% - stack slots of the rotation and r30/r31 of dir and of the copy
// inferred: placed at pos facing dir, in mode 6 (motion 2)
void LastCocoonImago::vf80(const atn::Vector3& pos, const atn::Vector3& dir) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 2, 0, 1, 1, 1.0f, 0.025f);
    mPos = pos;
    mMotion.mTracks[0].m08 = 0.0f;
    atn::Vector3 rot = dir;
    rot.fn_80139F60();
    mRot = rot;
    fn_80144710();
    mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
}

extern "C" double sin(double x);
// inferred: pulsing glow of the body (frame counter m1978, `speed` degrees per frame); name invented
static inline float ImagoGlow_unk(const int& frame, float speed) {
    return 0.6f * MIN(1.0f, 0.5f * (float)sin(0.017453292f * (speed * frame)) + 0.5f) + 0.2f;
}

// NONMATCHING: 95% - the three channels: ours multiplies the first one and copies it (they start equal)
// inferred: colours of the body parts: pulsing glow, faded in and out by the frame of the motion in the
// modes 9, 10 and 12
void LastCocoonImago::fn_8045CB24() {
    float r = ImagoGlow_unk(m1978, 4.0f);
    float g = r;
    float b = r;
    switch (m58C) {
    case 9:
        if (mMotion.mTracks[0].m08 < 20.0f) {
            float k = 1.0f - mMotion.mTracks[0].m08 / 20.0f;
            r *= k;
            g *= k;
            b *= k;
        } else if (mMotion.mTracks[0].m08 < 86.0f) {
            if (mMotion.mTracks[0].m08 < 71.0f) {
                g = MIN(1.0f, MAX(0.0f, (mMotion.mTracks[0].m08 - 30.0f) / 20.0f));
            } else {
                g = MIN(1.0f, MAX(0.0f, 1.0f - (mMotion.mTracks[0].m08 - 71.0f) / 15.0f));
            }
            r = g;
            m1978 = 0x43;
            b = 0.0f;
        } else {
            float k = MIN(1.0f, (mMotion.mTracks[0].m08 - 76.0f) / 30.0f);
            r *= k;
            m1978 = 0x43;
            g *= k;
            b *= k;
        }
        break;
    case 10:
        if (mMotion.mTracks[0].m08 < 20.0f) {
            float k = 1.0f - mMotion.mTracks[0].m08 / 20.0f;
            r *= k;
            g *= k;
            b *= k;
        } else if (mMotion.mTracks[0].m08 < 65.0f) {
            if (mMotion.mTracks[0].m08 < 50.0f) {
                g = MIN(1.0f, MAX(0.0f, (mMotion.mTracks[0].m08 - 20.0f) / 20.0f));
            } else {
                g = MIN(1.0f, MAX(0.0f, 1.0f - (mMotion.mTracks[0].m08 - 50.0f) / 15.0f));
            }
            r = g;
            m1978 = 0x43;
            b = g;
        } else {
            float k = MIN(1.0f, (mMotion.mTracks[0].m08 - 65.0f) / 15.0f);
            r *= k;
            m1978 = 0x43;
            g *= k;
            b *= k;
        }
        break;
    case 12:
        if (mMotion.mTracks[0].m08 > 140.0f) {
            r = ImagoGlow_unk(m1978, 64.0f);
            g = r;
            b = r;
            float end = mMotion.fn_80097D7C(0) - 120.0f;
            if (mMotion.mTracks[0].m08 > end) {
                float k = MAX(0.0f, 1.0f - (mMotion.mTracks[0].m08 - end) / 90.0f);
                r *= k;
                g *= k;
                b *= k;
            }
        } else {
            r = ImagoGlow_unk(m1978, 16.0f);
            g = r;
            b = r;
        }
        break;
    case 7:
    case 8:
    case 14:
        r = ImagoGlow_unk(m1978, 16.0f);
        g = r;
        b = r;
        break;
    }
    Color4f_unk one;
    Color4f_unk cb;
    Color4f_unk cg;
    Color4f_unk cr;
    cr.r = r;
    cr.g = r;
    cr.b = r;
    cr.a = 1.0f;
    cg.r = g;
    cg.g = g;
    cg.b = g;
    cg.a = 1.0f;
    cb.r = b;
    cb.g = b;
    cb.b = b;
    cb.a = 1.0f;
    one.r = 1.0f;
    one.g = 1.0f;
    one.b = 1.0f;
    one.a = 1.0f;
    mMotion.fn_80094958("em206_body_l", &cr, &one, &one);
    mMotion.fn_80094958("em206_tail_l", &cr, &one, &one);
    mMotion.fn_80094958("em206_rarm_l", &cg, &one, &one);
    mMotion.fn_80094958("em206_larm_l", &cb, &one, &one);
    m1978++;
}

void LastCocoonImago::vf38(int enable) {
    for (int i = 0; i < 11; i++) {
        if (enable) {
            mCapsules[i].mFlags |= 1;
        } else {
            mCapsules[i].mFlags &= ~1;
        }
    }
}

void LastCocoonImago::fn_8045D224() {
    if (m18F8->m54 == 5 && m18F8->vf38() != 0) {
        // inferred: the model is read again before each access (a local per access keeps the constants
        // out of the saved registers)
        atn::Model* model = m18F8->vf38();
        model->mTracks[0].m0C = 1.0f;
        model = m18F8->vf38();
        if (model->mTracks[0].m08 < 201.0f) {
            model = m18F8->vf38();
            model->mTracks[0].m08 = 201.0f;
        }
    }
    if (m14C0 == 120) {
        LastCocoonImago::vf38(0);
        if (((GameTask*)lbl_8087F430)->fn_80370A78(0xCD) == 0) {
            ((GameTask*)lbl_8087F430)->fn_80370AE4(0xCD, 1);
        }
    } else {
        mMotion.fn_80097D7C(0);
    }
}

extern "C" void fn_80392D2C(atn::Camera* camera, const atn::Camera* from, float rate); // inferred: blends back
extern "C" double tan(double x);
extern "C" double atan(double x);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: xsca camera curves (as in game/EnemyQuarkStand.cpp); only the members called here
struct XsfCurveSet_unk {
    float fn_805BF204(float time, int channel);
    void fn_805BF410(float time, float* out, int x, int y, int z);
};
struct Xsca_unk {
    XsfCurveSet_unk* fn_805BDCBC(unsigned int index);
};
// inferred: rotation matrix y * x * z of a vector (as QuarkRotationYXZv_unk of game/EnemyQuarkStand.cpp)
inline atn::Matrix3x4& ImagoRotationYXZ_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.y != 0.0f) {
        Mtx a;
        Mtx t;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx a;
        Mtx t;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.z != 0.0f) {
        Mtx a;
        Mtx t;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    return m;
}
inline void ImagoRotateYXZ_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    Mtx t;
    atn::Matrix3x4 rot;
    PSMTXConcat(m.m, ImagoRotationYXZ_unk(rot, r).m, t);
    __MtxCopy(m.m, t);
}
// inferred: game state (lbl_8087EFA8): flag at 0x240 set while the camera event of mode 12 runs (name invented)
struct ImagoEventState_unk {
    unsigned char m000[0x240];
    int m240;
};

// inferred: mode 12: the camera follows the xsca curves "cm000_em206_00" around the imago until the motion ends
// (then the capsules are disabled), effects and sounds at fixed frames, and from frame 200 the field camera
// blends back and the imago goes to mode 14 (motions 0x2F, 0x30) and tells the gimmick m18F8 (message 3)
// NONMATCHING: 95.2% - registers of the implicit copies of atn::Camera, the aspect kept in f31 across tan() and
// the slots of the rotation temporaries (as EnemyQuarkStand::fn_8034DE90)
void LastCocoonImago::fn_8045D3A4() {
    XsfCurveSet_unk* curve = ((Xsca_unk*)m1964.fn_804786F8())->fn_805BDCBC(0);
    atn::Vector3 eye;
    atn::Vector3 target;
    curve->fn_805BF410(mMotion.mTracks[0].m08, &eye.x, 1, 2, 3);
    curve->fn_805BF410(mMotion.mTracks[0].m08, &target.x, 7, 8, 9);
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, mPos.x, mPos.y, mPos.z);
    ImagoRotateYXZ_unk(m, atn::Vector3(0.0f, mRot.y, 0.0f));
    PSMTXMultVec(m.m, &eye, &eye);
    PSMTXMultVec(m.m, &target, &target);
    atn::Camera camera = lbl_8087F430->m006C;
    camera.mPos = eye;
    camera.mTarget = target;
    float fov = curve->fn_805BF204(mMotion.mTracks[0].m08, 0);
    camera.mFovy = 2.0f * (float)atan((float)tan(0.5f * (0.017453292f * fov)) / camera.mAspect);
    camera.fn_8004B378();
    float frame = mMotion.mTracks[0].m08;
    if (frame < mMotion.fn_80097D7C(0)) {
        atn::RenderManager::instance_->mCamera = camera;
    } else {
        LastCocoonImago::vf38(0);
    }
    frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0) - 30.0f && ((GameTask*)lbl_8087F430)->fn_80370A78(0xCD) == 0) {
        ((GameTask*)lbl_8087F430)->fn_80370AE4(0xCD, 1);
    }
    if ((m14BC == 0 && mMotion.mTracks[0].m08 > 139.0f) || (m14BC == 1 && mMotion.mTracks[0].m08 > 144.0f) ||
        (m14BC == 2 && mMotion.mTracks[0].m08 > 150.0f) || (m14BC == 3 && mMotion.mTracks[0].m08 > 155.0f) ||
        (m14BC == 4 && mMotion.mTracks[0].m08 > 170.0f) || (m14BC == 5 && mMotion.mTracks[0].m08 > 190.0f) ||
        (m14BC == 6 && mMotion.mTracks[0].m08 > 220.0f)) {
        m14BC++;
        fn_80232B7C(0, 0);
        PlayEfp_unk(&m196C, &mMotion);
        if (m14BC % 2 == 0) {
            fn_800C344C("SE_EM206_011", &mPos, 0, -1, 1.0f);
        }
    }
    frame = mMotion.mTracks[0].m08;
    if (frame >= 140.0f && frame < mMotion.fn_80097D7C(0) - 10.0f) {
        ((ImagoEventState_unk*)lbl_8087EFA8)->m240 = 1;
    } else {
        ((ImagoEventState_unk*)lbl_8087EFA8)->m240 = 0;
    }
    if (mMotion.mTracks[0].m08 >= 200.0f && (mStatus.m0C & 0x20) != 0x20) {
        fn_80392D2C(&lbl_8087F430->m006C, &camera, 0.033333335f);
        m14BC = 0;
        m14C0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 14;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x2F, 0, 0, 1, 0.0f, 0.1f);
        m574 = lbl_807C7030;
        mMotion.fn_80097C08(0, 0x30, 1, 0, 1, 0.0f, 0.025f);
        m14BC = 2;
        GimmickMsg_unk msg(3);
        m18F8->vf40(&msg);
        ((ImagoEventState_unk*)lbl_8087EFA8)->m240 = 0;
    }
}

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" GameUI* lbl_8087F490;
extern "C" GameConfig_unk* lbl_8087F0A8; // inferred: game settings (m48C: input)
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
// inferred: button hint of the UI with its defaults (as MulueUIHint_unk in game/Em204Mulue.cpp)
struct ImagoUIHint_unk : public GameUIElem1C_unk {
    ImagoUIHint_unk(int a, int b, int c) {
        m00 = a;
        m04 = b;
        m08 = c;
        m0C = 0;
        m10 = 0;
        m14 = 0;
        m18 = 1;
    }
};

// NONMATCHING: 97% - stack slots and scheduling of the temporaries of the first lerp of the gimmick position
// inferred: mode 11: carries the gimmick m18F8 (its position follows the imago at the height m1948.y),
// shoots the skill 0x5BD from frame 85 and then waits for the player to thrust (chr::ControlThrustForCocoon)
void LastCocoonImago::fn_8045DF34() {
    if (m14BC == 0) {
        if (mMotion.mTracks[0].m08 < 60.0f) {
            atn::Vector3 pos = m18F8->m6C;
            pos = (mPos - pos) * 0.05f + pos;
            pos.y = m1948.y;
            m18F8->m6C = pos;
            m1948 = pos;
            atn::Vector3 rot = m18F8->m78;
            float turn = 0.05f * atn::WrapAngle_unk(mRot.y - rot.y);
            rot.y = atn::WrapAngle_unk(rot.y + turn);
            m18F8->m78 = rot;
        } else {
            atn::Vector3 pos = m18F8->m6C;
            pos = mPos;
            pos.y = m1948.y;
            m18F8->m6C = pos;
            m1948 = pos;
            atn::Vector3 rot = m18F8->m78;
            rot = mRot;
            m18F8->m78 = rot;
            if (mMotion.mTracks[0].m08 >= 85.0f) {
                m14BC++;
                HitInfo_unk* bullet = fn_80219E6C(0x5BD);
                atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                atn::Matrix3x4 m;
                m.fn_8013A13C(mRot.y);
                PSMTXMultVec(m.m, &dir, &dir);
                atn::Vector3 at = mPos + dir;
                CharaTask* player = lbl_8087F8A0->mHead;
                Effect_unk* mgr = lbl_8087F048;
                fn_800FAB80(mgr, player, bullet, fn_800F8548(mgr), &at, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            }
        }
    } else if (m14BC == 1) {
        atn::Vector3 pos = m18F8->m6C;
        pos = mPos;
        pos.y = m1948.y;
        m18F8->m6C = pos;
        m1948 = pos;
        atn::Vector3 rot = m18F8->m78;
        rot = mRot;
        m18F8->m78 = rot;
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14BC++;
            mMotion.fn_80097C08(0, 0x30, 1, 0, 1, 1.0f, 0.1f);
            if (((GameTask*)lbl_8087F430)->fn_80370A78(0xD0) == 0) {
                ((GameTask*)lbl_8087F430)->fn_80370AE4(0xD0, 1);
            }
        }
    } else {
        m18F8->vf38()->mTracks[0].m08 = m18F8->vf38()->fn_80097D7C(0);
        PlayerTask* player = lbl_8087F8A0->mHead;
        atn::Vector3 d = player->mPos - mPos;
        d.y = 0.0f;
        if (PSVECMag(&d) < 80.0f) {
            int ok = player->fn_8016DDB0(0);
            if (player->m55C == 6 && player->m560 == 4) {
                ok = 1;
            }
            PSVECNormalize(&d, &d);
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            atn::Matrix3x4 m;
            m.fn_8013A13C(mRot.y);
            PSMTXMultVec(m.m, &front, &front);
            if (PSVECDotProduct(&d, &front) < 0.0f) {
                ok = 0;
            }
            if (ok) {
                GameUI* ui = lbl_8087F490;
                if (ui != 0) {
                    ui->m764 = ImagoUIHint_unk(6, 0x11, -1);
                }
                if (lbl_8087F0A8->mInput.fn_801231D0(0)) {
                    player->fn_80178208(new ((MemoryTag)3, "", "", 0) chr::ControlThrustForCocoon(player, this, 0x5BC));
                    fn_8045ECC4();
                    if (lbl_8087F490 != 0) {
                        lbl_8087F490->fn_803E3384();
                    }
                }
            }
        }
    }
}

void LastCocoonImago::fn_8045E548() {
    if (m14BC == 0) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            m14EC = 210;
            m14BC = 1;
            mMotion.fn_80097C08(0, 0x14B, 1, 1, 1, 0.0f, 0.025f);
        }
    } else if (m14BC == 1) {
        if (--m14EC < 0) {
            ResetAttacks_unk();
            m14BC = 2;
            mMotion.fn_80097C08(0, 0x14C, 0, 1, 1, 0.0f, 0.025f);
        }
    } else if (m14BC == 2) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            atn::Vector3 dir;
            dir.x = 0.0f;
            dir.y = 0.0f;
            dir.z = 1.0f;
            atn::Matrix3x4 rot;
            rot.fn_8013A13C(mRot.y);
            PSMTXMultVec(rot.m, &dir, &dir);
            dir *= 15.0f;
            atn::Vector3 pos = dir;
            m14BC = 0;
            m14C0 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 10;
            fn_8016E970(3);
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.mTracks[0].m0C = 1.0f;
            mMotion.fn_80097C08(0, 0x14D, 0, 1, 1, 0.0f, 0.025f);
            m14D4 = pos;
            return;
        }
    }
    fn_8045ABB4(m14E0, 0.0f, m568);
}

extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data
struct HitResult_unk;
extern "C" int fn_800F8C6C(Effect_unk* mgr, HitResult_unk* results, int count, CharaTask* chara, HitInfo_unk* data,
                           int id, int time, int hits, float angle); // inferred: deals a hit, returns the hits

void LastCocoonImago::fn_8045E780() {
    mMotion.mTracks[0].m0C = mStatus.fn_8012DB04();
    float end = mMotion.mTracks[0].m08;
    if (end >= mMotion.fn_80097D7C(0)) {
        m14BC = 0;
        m14C0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 0;
        fn_8016E970(3);
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= 28.0f && frame <= 58.0f) {
        float t = mMotion.mTracks[0].m0C * lbl_8087EFA8->m3A4;
        atn::Vector3 dir;
        dir.x = 0.0f;
        dir.y = 0.0f;
        dir.z = 1.0f;
        atn::Matrix3x4 m;
        m.fn_8013A13C(mRot.y);
        PSMTXMultVec(m.m, &dir, &dir);
        mPos += dir * 3.75f * t;
        float turn = t * (0.15f * (m14D4.ToRotation_unk().y - mRot.y));
        mRot.y = atn::WrapAngle_unk(mRot.y + turn);
        frame = mMotion.mTracks[0].m08;
        if (frame >= 35.0f && frame <= 46.0f) {
            HitInfo_unk* bullet = fn_80219E6C(0x5B6);
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, bullet, m590, 30, -1, 0.0f);
        }
    }
}

__declspec(noinline) void LastCocoonImago::fn_8045EB60() {
    int fromHang = m58C == 14;
    SetMode_unk(2);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.fn_80097C08(0, fromHang ? 0x31 : 0x37, 0, 0, 1, 0.0f, 0.025f);
    mMotion.mTracks[0].m0C = 0.5f;
    if (fromHang) {
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.mTracks[0].m08 = 200.0f;
        GimmickMsg_unk msg;
        msg.mType = 5;
        m18F8->vf40(&msg);
    }
    m574 = lbl_807C7030;
}

// called with bl by fn_8045DF34 (MWCC would inline it there)
__declspec(noinline) void LastCocoonImago::fn_8045ECC4() {
    SetMode_unk(12);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x31, 0, 0, 1, 0.0f, 0.1f);
    m574 = lbl_807C7030;
    GimmickMsg_unk msg;
    msg.mType = 5;
    m18F8->vf40(&msg);
}

extern "C" int fn_8004ECC0(void* world, atn::Vector3* hit, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" int fn_80051B70(const atn::Vector3* line, const CharaTask::Sphere_unk* sphere); // inferred: segment hits
                                                                                             // sphere
// inferred: the field camera looks around (as in gmk/WindyItem.cpp; name invented)
inline bool ImagoLookMode_unk(int mode) {
    return mode == 3 || mode == 1 || mode == 4;
}
// inferred: the sphere can be seen from `eye` (as CanSee_unk of gmk/WindyItem.cpp, other mask; name invented)
inline int ImagoCanSee_unk(const atn::Vector3& eye, const CharaTask::Sphere_unk& sphere) {
    atn::Vector3 line[2];
    line[0] = eye;
    line[1] = sphere.mCenter;
    atn::Vector3 d = line[1] - line[0];
    float len = PSVECMag(&d);
    float radius = sphere.mRadius;
    if (!((float)__fabs(len) < 1.1920929e-7f)) {
        line[1] = line[1] - line[0];
        PSVECNormalize(&line[1], &line[1]);
        line[1] = line[1] * (len - radius) + line[0];
    }
    return fn_8004ECC0(lbl_8087EE98, 0, &line[0], &line[1], 0x80000000, 0, 0) == 0;
}

// inferred: arrow of the weak point `index` (sphere of m624): when the field camera looks around and the sphere is
// in front of it and visible, the UI shows the target (and the hint of the attack once); otherwise, when it is
// near (800) and visible, the UI shows it too
// NONMATCHING: 73.6% - the original stores -1/0 at the start in seven dead stack words (an 0xA8-byte object at
// 0xF0 with -1 at +4 and +0x24 and 0 at +0x44..+0xA4, and -1 at 0x1E0: locals of an inline not identified) and
// keeps the copies of the segment and of the sphere in other slots; 3 variants
void LastCocoonImago::fn_8045EDEC(int index) {
    GameTask* field = (GameTask*)lbl_8087F430;
    CharaTask::Sphere_unk ball = m624.mData[index].m04;
    if (field->mCamera.m7FC == 3 || field->mCamera.m7FC == 1 || field->mCamera.m7FC == 4) {
        atn::Vector3 line[2];
        line[0] = field->mCamera.mPos;
        line[1] = field->mCamera.mTarget;
        line[1] -= line[0];
        PSVECNormalize(&line[1], &line[1]);
        line[1] = line[1] * 800.0f + line[0];
        CharaTask::Sphere_unk item;
        item.mCenter = ball.mCenter;
        item.mRadius = ball.mRadius * 1.4f;
        atn::Vector3 toItem = item.mCenter - field->mCamera.mPos;
        float dist = PSVECMag(&toItem);
        atn::Vector3 lo(-ball.mRadius, -ball.mRadius, 0.0f);
        atn::Vector3 hi(ball.mRadius, ball.mRadius, 0.0f);
        atn::Matrix3x4 view = field->mCamera.mInvView;
        view.m[0][3] = 0.0f;
        view.m[1][3] = 0.0f;
        view.m[2][3] = 0.0f;
        PSMTXMultVec(view.m, &lo, &lo);
        PSMTXMultVec(view.m, &hi, &hi);
        atn::Vector3 p1 = ball.mCenter + lo;
        p1 = atn::RenderManager::instance_->GetScreenPosition(p1);
        atn::Vector3 p2 = ball.mCenter + hi;
        p2 = atn::RenderManager::instance_->GetScreenPosition(p2);
        atn::Vector2_unk size(0.5f * (float)__fabs(p1.x - p2.x), 0.5f * (float)__fabs(p1.y - p2.y));
        atn::Vector2_unk center(0.5f * (p1.x + p2.x), 0.5f * (p1.y + p2.y));
        int shown = 0;
        if (fn_80051B70(line, &item) != 0 && ImagoCanSee_unk(line[0], item)) {
            shown = 1;
            lbl_8087F490->fn_803E3050(ball.mCenter, atn::Vector3(center.x, center.y, p1.z), size, dist);
            int held;
            if (lbl_8087F0A8->m0B0 != 0) {
                held = lbl_8087F0A8->mInput.fn_801240B4(0x24);
            } else {
                held = 1;
            }
            if (held != 0 && m18D0 == 0 && index < 3) {
                ((GameTask*)lbl_8087F430)->fn_80370AE4(sImagoAttacks_unk[index].m10, 1);
                m18D0 = 1;
            }
        }
        if (shown == 0 && dist < 800.0f && ImagoCanSee_unk(line[0], item)) {
            lbl_8087F490->fn_803E3050(ball.mCenter, atn::Vector3(center.x, center.y, p1.z), size, dist);
        }
    }
}

// inferred: the sphere of a ColliSphere read as one shape (name invented)
inline const atn::ColliSphereShape_unk& GetShape_unk(const atn::ColliSphere& c) {
    return *(const atn::ColliSphereShape_unk*)&c.mSphere;
}
// inferred: the segment and radius of a capsule (m3C-m57) read as one shape (name invented)
inline const atn::ColliCapsuleShape_unk& GetShape_unk(const atn::ColliCapsule& c) {
    return *(const atn::ColliCapsuleShape_unk*)&c.m3C;
}

void LastCocoonImago::fn_8045F4CC() {
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (g->m50 == 1 && g->vf5C() != 0) {
            atn::ColliSphere* colli = (atn::ColliSphere*)g->vf50();
            if (colli != 0) {
                atn::ColliSphereShape_unk s = GetShape_unk(*colli);
                int hit = 0;
                atn::ColliCapsuleShape_unk c = GetShape_unk(mColli);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                c = GetShape_unk(mCapsules[6]);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                c = GetShape_unk(mCapsules[8]);
                if (fn_80051B14(&s, &c)) {
                    hit = 1;
                }
                if (hit) {
                    GimmickMsg_unk msg;
                    msg.mType = 4;
                    g->vf40(&msg);
                }
            }
        }
    }
}

// inferred: frames 80-105 of the motion of state 11
int LastCocoonImago::vfE4() {
    if (m58C == 11) {
        float frame = mMotion.mTracks[0].m08;
        if (80.0f <= frame && frame <= 105.0f) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 98% - offsets in the string pool (lbl_80754DE0, the strings of the unwritten functions are missing)
// inferred: the four points of the body (right shoulder, spine, left shoulder, hips)
void LastCocoonImago::vfCC(atn::Vector3* out, const atn::Vector3& dir) {
    out[0] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("rightshoulder"))->GetTrans_unk();
    out[1] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("spine1"))->GetTrans_unk();
    out[2] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("leftshoulder"))->GetTrans_unk();
    out[3] = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("hips"))->GetTrans_unk();
}

// NONMATCHING: 99.4% - offset of "head" in the string pool (see vfCC)
atn::Vector3 LastCocoonImago::vfD8(CharaTask* target) {
    atn::Vector3 pos = ((atn::Matrix3x4*)mMotion.GetNodeMtx_unk("head"))->GetTrans_unk();
    pos.y += 10.0f;
    return pos;
}

float LastCocoonImago::vfDC() {
    return 0.7f;
}

float LastCocoonImago::vfD4() {
    return 2.2f;
}

float LastCocoonImago::vf90() {
    return 5.0f;
}

LastCocoonImago::~LastCocoonImago() {}
