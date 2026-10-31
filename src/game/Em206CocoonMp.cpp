#include "game/Em206CocoonMp.h"
#include "game/CharaStatus.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/GameBgm.h"
#include "math/Vec.h"
#include "atn/DrawHelper.h"
#include "chr/ControlBase.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b); // inferred: a + b
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4); // debug menu
// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: shoots a bullet
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, HitInfo_unk* data, int id, int time,
                            int c, float f);
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, void* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3* pos, int arg);
extern "C" HitInfo_unk* fn_80219E6C(int id); // inferred: skill data
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: color as four floats (as in game/CharaTask.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
// inferred: field (lbl_8087F430); class unknown
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);             // inferred: reads an event flag
extern "C" void fn_80370AE4(Field_unk* field, int index, int value); // inferred: sets an event flag
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);
extern "C" void PSMTXRotRad(Mtx m, char axis, float rad);
// inferred: frame time (lbl_8087EFA8, as FrameTime_unk in game/CharaStatus.cpp)
struct FrameTime_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" FrameTime_unk* lbl_8087EFA8;
extern "C" void PSMTXMultVec(const Mtx m, const atn::Vector3* src, atn::Vector3* dst);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: multiplayer session (lbl_8087F610, as in game/Em204MulueMp.cpp)
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DA48C(MpSession_unk* session, int arg);

// inferred: damage spheres of the body (table lbl_80746964; names invented)
struct CocoonSphereDef_unk {
    const char* mNode;     // inferred: center (or start of the segment)
    const char* mNode2;    // inferred: end of the segment ("" = only mNode)
    const char* mMaterial;
    int m0C;
    int m10;
    float mRadius;
    int m18;
};
static const CocoonSphereDef_unk sSpheres_unk[3] = {
    {"reference", "", "", -1, -1, 17.5f, 0},
    {"head", "", "em206_body_l", 100, 200, 8.4f, 1000},
    {"spine1", "", "", -1, -1, 21.0f, 1000},
};

// inferred: collision capsules of the body (table lbl_80746A70; names invented)
struct CocoonCapsuleDef_unk {
    const char* mNode;  // inferred: start of the segment
    const char* mNode2; // inferred: end of the segment
    float mRadius;
};
static const CocoonCapsuleDef_unk sCapsules_unk[11] = {
    {"neck", "ef_eye", 5.6f},          {"spine", "neck", 14.0f},
    {"rightleg", "rightfoot", 7.0f},   {"rightupleg", "rightleg", 7.0f},
    {"leftleg", "leftfoot", 7.0f},     {"leftupleg", "leftleg", 7.0f},
    {"righthand", "rightmiddle3", 7.0f}, {"rightarm", "righthand", 7.0f},
    {"lefthand", "leftmiddle1", 7.0f}, {"leftarm", "lefthand", 7.0f},
    {"tail1", "tail3", 10.5f},
};

// inferred: state of the multiplayer copy (SpecialState_unk seen with the layout of this class; name invented)
struct CocoonState_unk {
    int m00;          // inferred: state (m58C)
    int m04;          // inferred: step (m14BC)
    short m08;        // inferred: m1900
    short m0A;        // inferred: m1904
    signed char m0C;  // inferred: m14F0
    signed char m0D;  // inferred: m1908
    short m0E;        // inferred: m1520
};

// inferred: damage record (as KrakenDamage_unk of game/Em302Kraken.cpp, starting at 0; names invented)
struct CocoonDamage_unk {
    CocoonDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        m1C = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int m1C : 4;
    unsigned int m1C_rest : 28;
};
// inferred: default hit parameters (HitParam_unk of game/CharaTask.cpp)
struct CocoonHitParam_unk {
    int m00;
    float m04;
    float m08;
    int m0C;
    float m10;
    int m14;
};
extern "C" CocoonHitParam_unk lbl_807C6B90;
extern "C" void fn_8003EA3C(CocoonDamage_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const void* param, int arg, int arg2); // inferred: computes the damage

// inferred: effect `efp` on a model with no offset (name invented, as in game/Em206EggMonster.cpp)
static inline void PlayEffectM_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: linear interpolation with the offset added last (name invented)
inline atn::Vector3 LerpR_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: node position (as in game/Em403WarLion.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: starts state `state` (names invented)
inline void Em206CocoonMp::SetState_unk(int state) {
    m14DC = 0;
    m14E0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
}

inline void Em206CocoonMp::PlayMotion_unk(int motion, int loop, float speed) {
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = speed;
    mMotion.fn_80097C08(0, motion, loop, 0, 1, 0.0f, 0.025f);
}

// inferred: death (state 2)
inline void Em206CocoonMp::Die_unk() {
    SetState_unk(2);
    fn_8016E970(6);
    m560 = 0x17;
    PlayMotion_unk(0x2E, 0, 0.5f);
    fn_80232B7C(0, 0);
    PlayEffectM_unk(lbl_8087F3C0, &m191C, &mMotion);
    m574 = lbl_807C7030;
    fn_80370AE4(lbl_8087F430, 0x65, 1);
    Effect_unk* mgr = lbl_8087F048;
    CharaTask* player = lbl_8087F8A0->mHead;
    int id = fn_800F8548(mgr);
    fn_800FAB80(mgr, player, fn_80219E6C(0x5C3), id, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
    m1914 = 1;
}

// NONMATCHING: written for the order of the strings (std::string, tokenizer and the ResHandle constructor are
// called out of line in the original, as in game/LastCocoonChrysalis.cpp)
Em206CocoonMp::Em206CocoonMp(atn::Task* parent, EnemyInfo_unk* info)
    : SpecialEnemyBase(parent, info), m14DC(0), m14E0(0), m14E4(0), m14EC(0), m14F0(0), m14F4(0), m1520(0),
      m1524(0), m1528(5), m1900(0), m1904(900), m1908(0), m190C(1.0f), m1910(0x4650), m1914(0), m1918(0),
      m192C(0), m1930(40.0f) {
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
    m191C.LoadRequest("/effect/ef_uc015.efp");
    m14D4.vf0C(path.c_str());
    m14A8 |= 0x80000000;
}

int Em206CocoonMp::vf0C() {
    if (fn_8013655C() == 0 && m14D4.fn_80473F50() == 0 && m191C.IsLoading() == 0) {
        mStatus.m18 |= 0xC1D1;
        mStatus.m18 |= 0x1010000;
        mStatus.m18 |= 0x208;
        mStatus.m18 |= 0x2800000;
        mStatus.m18 |= 4;
        fn_80087994(m1928.Load("Em206CocoonMp"), "DebugDisp", &m192C, 0, 0);
        m56C = 0.05f;
        m5B0 = 17.5f;
        m5B4 = 84.0f;
        for (int i = 0; i < 11; i++) {
            m1538[i].mFlags |= 3;
            m1538[i].m0C = (int)this;
            m1538[i].mFlags &= ~0x80000000;
        }
        vf20();
        return 1;
    }
    return 0;
}

void Em206CocoonMp::vf10() {
    if (m1520 != m1524) {
        fn_80370AE4(lbl_8087F430, 10, 1);
        m1524 = m1520;
    }
    if (!CharaTask::mFlag2 || !CanAct_unk()) {
        if (m1904 > 0) {
            m1904--;
        }
        if (CanAct_unk() && m58C != 0 && !vfF8()) {
            vf100(0);
        }
        m14F4++;
        m1910--;
        vf104();
    }
    fn_8014C540();
    fn_80145334();
    fn_802BDA04();
}

void Em206CocoonMp::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    bool down = true;
    msg->m10 *= 0.0f;
    unsigned int flags = mStatus.m0C;
    if ((flags & 0x80000) != 0x80000 && (flags & 0x1000000) != 0x1000000) {
        down = false;
    }
    if (!down && !msg->mData->m90.Check(0x80000)) {
        msg->m0C |= 8;
    }
    if (*(int*)&msg->m3C[4] == 1) {
        if (msg->m44 == 0) {
            *(int*)&msg->m3C[4] = 0;
        } else {
            msg->m0C |= 0x80000000;
            msg->m0C &= ~8;
        }
    }
    if (m14F0 != 0 && m58C == 15) {
        msg->m0C &= ~8;
    }
    fn_80151448(msg);
    if (!CharaTask::mFlag2) {
        vf40(hit);
    } else {
        vf118(hit);
    }
}

// NONMATCHING: 99% - register allocation of the inlined death (Die_unk)
void Em206CocoonMp::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (mStatus.IsState_unk(0x20)) {
        if (m1914 == 0) {
            Die_unk();
        }
    } else if (m14F0 != 0 && m58C - 13U <= 1 && ((unsigned int)msg->mData->m90.mFlags & 0x4000) == 0x4000) {
        SetState_unk(15);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.fn_80097C08(0, 0x16A, 0, 0, 1, 0.0f, 0.025f);
        mMotion.mTracks[0].m0C = 0.1f;
        Effect_unk* mgr = lbl_8087F048;
        atn::Vector3 pos = vfAC();
        fn_80108F38(mgr, &pos, 0x4000);
    }
}

atn::Vector3 Em206CocoonMp::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (m1900 != 0) {
        result = m624.mData[1].m04.mCenter;
    }
    return result;
}

void Em206CocoonMp::vfFC() {
    SetState_unk(0);
    fn_8016E970(3);
}

// NONMATCHING: 99% - register allocation of the inlined death (Die_unk) and of the copies of the direction
// inferred: starts the action chosen by fn_802BD480 (the argument is not used)
void Em206CocoonMp::vf100(int state) {
    CharaTask* target = mC58.mC4;
    if (target == 0) {
        return;
    }
    atn::Vector3 pos = target->mPos;
    atn::Vector3 d = pos - mPos;
    PSVECNormalize(&d, &d);
    switch (m58C) {
    case 6:
        SetState_unk(6);
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x33, 0, 1, 1, 1.0f, 0.025f);
        break;
    case 7:
        SetState_unk(7);
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x33, 0, 1, 1, 0.0f, 0.025f);
        break;
    case 8: {
        atn::Vector3 dir = d;
        SetState_unk(8);
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x156, 0, 1, 1, 0.0f, 0.025f);
        m152C = dir;
        break;
    }
    case 9: {
        atn::Vector3 dir = d;
        SetState_unk(9);
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14D, 0, 1, 1, 0.0f, 0.025f);
        m152C = dir;
        break;
    }
    case 10:
        SetState_unk(10);
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x13F, 0, 1, 1, 0.0f, 0.025f);
        break;
    case 11:
        SetState_unk(11);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x145, 0, 0, 1, 0.0f, 0.025f);
        break;
    case 2:
        Die_unk();
        break;
    default:
        SetState_unk(0);
        fn_8016E970(3);
        if (m55C != 6) {
            fn_802BD480();
        }
        break;
    }
}

void Em206CocoonMp::vf108() {}

// NONMATCHING: 99.9% - stack slots of the front vector and of the center
// inferred: creates the damage spheres of the table sSpheres_unk
void Em206CocoonMp::vf50() {
    Item14_unk item;
    for (int i = 0; i < 3; i++) {
        MtxPtr a = mMotion.GetNodeMtx_unk(sSpheres_unk[i].mNode);
        MtxPtr b = mMotion.GetNodeMtx_unk(sSpheres_unk[i].mNode2);
        if (a != 0) {
            if (b != 0) {
                item.m04.mCenter = LerpR_unk(GetNodePos_unk(a), GetNodePos_unk(b), 0.6f);
            } else {
                item.m04.mCenter = GetNodePos_unk(a);
            }
            item.m04.mRadius = sSpheres_unk[i].mRadius;
            m624.push_back(item);
        }
    }
    mFlagB29 = 0;
}

void Em206CocoonMp::vf54() {
    if (m624.mSize != 0) {
        for (int i = 0; i < 3; i++) {
            if (i == 0) {
                atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &fwd, &fwd);
                atn::Vector3 off = fwd * 3.0f;
                m624.mData[i].m04.mCenter = mPos - off;
                m624.mData[i].m04.mCenter.y += 0.5f * sSpheres_unk[i].mRadius;
            } else {
                MtxPtr a = mMotion.GetNodeMtx_unk(sSpheres_unk[i].mNode);
                MtxPtr b = mMotion.GetNodeMtx_unk(sSpheres_unk[i].mNode2);
                if (a != 0) {
                    if (b != 0) {
                        m624.mData[i].m04.mCenter = LerpR_unk(GetNodePos_unk(a), GetNodePos_unk(b), 0.6f);
                    } else {
                        m624.mData[i].m04.mCenter = GetNodePos_unk(a);
                    }
                }
            }
        }
    }
}

void Em206CocoonMp::vf24() {
    if (m55C == 6 && mControl && mControl->vf3C() == 0) {
        return;
    }
    if (m14E8 != 0) {
        PlayMotion_unk(0x14, 1, 1.5f);
    } else {
        PlayMotion_unk(2, 1, 1.5f);
    }
}

// NONMATCHING: 99.36% - registers of the hoisted vector addresses swapped in pairs; copy crossing into m148C
// inferred: updates the collision of the body and of the capsules (masks 0xE0 in ice mode, 4 in state 10)
void Em206CocoonMp::vf20() {
    m620 = m5B0;
    m614 = mPos + m5A4;
    m614.y += m620;
    atn::Vector3 a = m614;
    atn::Vector3 top = a;
    top.y += 15.0f;
    mColli.m3C = a;
    mColli.m48 = top;
    mColli.m54 = m620;
    int mask = m14F0 != 0 ? 0xE0 : 0;
    if (m58C == 10) {
        mask |= 4;
    }
    mColli.m20 = mask;
    for (int i = 0; i < 11; i++) {
        float radius = sCapsules_unk[i].mRadius;
        a = GetNodePos_unk(mMotion.GetNodeMtx_unk(sCapsules_unk[i].mNode));
        top = GetNodePos_unk(mMotion.GetNodeMtx_unk(sCapsules_unk[i].mNode2));
        m1538[i].m3C = a;
        m1538[i].m48 = top;
        m1538[i].m54 = radius;
        m1538[i].m20 = mask;
    }
    m148C = GetNodePos_unk(mMotion.GetNodeMtx_unk("head"));
    m1498 = 13.0f;
}

// inferred: walks towards the character it follows (control 7); twice as fast unless slowed
void Em206CocoonMp::fn_802BD118() {
    float speed = 0.0f;
    float scale = 1.0f;
    atn::Vector3 rot = mRot;
    bool slow = true;
    if ((mStatus.m0C & 8) != 8 && (mStatus.m0C & 0x200) != 0x200) {
        slow = false;
    }
    if (!slow) {
        scale *= 2.0f;
    }
    if (m55C == 7) {
        CharaTask* t = mC58.mC4;
        atn::Vector3 dir = (t->mPos - mPos).GetNormalize();
        if ((mC58.mC4->mPos - mPos).Length_unk() < 50.0f) {
            speed = 0.0f;
        } else {
            speed = dir.Length_unk();
        }
        rot = dir.ToRotation_unk();
    } else if (m55C == 6) {
        fn_8013A258();
        return;
    }
    fn_802BC4EC(rot, speed, m568 * scale);
}

void Em206CocoonMp::vf38(int arg) {
    for (int i = 0; i < 11; i++) {
        if (arg != 0) {
            m1538[i].mFlags |= 1;
        } else {
            m1538[i].mFlags &= ~1;
        }
    }
}

void Em206CocoonMp::fn_802BDFA0() {
    if (m55C != 6) {
        fn_8016E970(3);
        fn_80170A20(mC58.mC4, 0, -1.0f);
    }
    fn_802BD118();
}

void Em206CocoonMp::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        for (int i = 0; i < 11; i++) {
            m1538[i].mFlags &= ~1;
        }
        mFlag15 = 1;
        fn_801765D8();
    }
}

void Em206CocoonMp::fn_802BE0FC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
        fn_8016E970(3);
    }
}

void Em206CocoonMp::fn_802BE180() {
    mMotion.mTracks[0].m0C = mStatus.fn_8012DB04();
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
        fn_8016E970(3);
    } else if (mMotion.mTracks[0].m08 >= 57.0f && mMotion.mTracks[0].m08 <= 65.0f) {
        HitInfo_unk* data = fn_80219E6C(0x5BF);
        fn_80144710();
        fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
        if (m192C != 0) {
            lbl_8087EEB0->fn_80063D3C(mPos, mStatus.mParamB.m28 + data->m40, 0.0f, 0xFFFF0000);
        }
    }
}

// inferred: state 9 (charges forward, turning towards m152C, and hits from frame 35 to 46)
void Em206CocoonMp::fn_802BE2A0() {
    mMotion.mTracks[0].m0C = mStatus.fn_8012DB04();
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
        fn_8016E970(3);
    }
    if (mMotion.mTracks[0].m08 >= 28.0f && mMotion.mTracks[0].m08 <= 58.0f) {
        float k = mMotion.mTracks[0].m0C * lbl_8087EFA8->m3A4;
        atn::Vector3 fwd(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &fwd, &fwd);
        float yaw = m152C.ToRotation_unk().y;
        float turn = k * (0.15f * (yaw - mRot.y));
        mRot.y = atn::WrapAngle_unk(mRot.y + turn);
        if (mMotion.mTracks[0].m08 >= 35.0f && mMotion.mTracks[0].m08 <= 46.0f) {
            HitInfo_unk* data = fn_80219E6C(0x5BE);
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, data, m590, 30, -1, 0.0f);
        }
        fn_802BC4EC(fwd.ToRotation_unk(), 1.0f, 1.5f * (3.75f * k));
    }
}

// NONMATCHING: 99% - stack slots of the direction (and pool offsets until the rest of the unit is written)
// inferred: state 10 (throws a bullet from the right hand to the hips of the target)
void Em206CocoonMp::fn_802BE7EC() {
    if (m14DC == 0) {
        if (mMotion.mTracks[0].m08 >= 104.0f) {
            m14DC = 1;
            m12CC = 0.4f;
            atn::Vector3 from = GetNodePos_unk(mMotion.GetNodeMtx_unk("righthand"));
            atn::Model* model = &mC58.mC4->mMotion;
            atn::Vector3 to = GetNodePos_unk(model->GetNodeMtx_unk("hips"));
            atn::Vector3 d = to - from;
            atn::Vector3 dir = d.GetNormalize();
            Effect_unk* mgr = lbl_8087F048;
            fn_800F8574(mgr, this, fn_80219E6C(0x5C5), &from, &dir, 0, 0, 0, 0.0f, 1.0f);
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            SetState_unk(0);
            fn_8016E970(3);
        }
    }
}

void Em206CocoonMp::fn_802BE9C8() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
        fn_8016E970(3);
    }
    if (mMotion.mTracks[0].m08 >= 55.1f && m1904 == 0) {
        if (fn_80370A78(lbl_8087F430, 0x96) == 0) {
            fn_80370AE4(lbl_8087F430, 0x96, 1);
        }
        m1520 = m1520 == 0;
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, fn_80219E6C(0x57C), m590, &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
        m1904 = 900;
    }
}

// NONMATCHING: 98.5% - stack slot of the temporary of m14F8
// inferred: state 12 (curls up with motion 0x169, then starts state 13: rolls towards the target)
void Em206CocoonMp::fn_802BEAEC() {
    if (mMotion.mTracks[0].m00 == 0x169) {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            SetState_unk(13);
            m1504 = 0;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, 0x167, 1, 0, 1, 0.0f, 0.025f);
            mMotion.mTracks[0].m0C = 1.8f;
            m14F8 = mC58.mC4->mPos - mPos;
            m14F8.y = 0.0f;
            if (m14F8.LengthSq_unk() > 0.0001f) {
                m14F8.Normalize_unk();
            } else {
                m14F8.x = 0.0f;
                m14F8.y = 0.0f;
                m14F8.z = 1.0f;
            }
            mRot = m14F8.ToRotation_unk();
        }
    } else if (mMotion.mTracks[0].m08 <= 2.0f) {
        mMotion.fn_80097C08(0, 0x169, 0, 0, 1, 0.0f, 0.1f);
        mMotion.mTracks[0].m0C = 1.0f;
    }
}

// inferred: state 16 (jumps from m1508 to m1514, then lands)
void Em206CocoonMp::fn_802C0458() {
    if (mMotion.mTracks[0].m00 == 0x168) {
        float t = MIN(1.0f, mMotion.mTracks[0].m08 / 22.0f);
        atn::Vector3 pos = LerpR_unk(m1508, m1514, t);
        mPos = pos;
        mPos.y += -160.0f * ((t - 0.5f) * (t - 0.5f)) + 40.0f;
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            mMotion.fn_80097C08(0, 0x169, 0, 0, 1, 0.0f, 0.1f);
        }
        if (m14DC == 0 && mMotion.mTracks[0].m08 >= 21.0f) {
            Effect_unk* mgr = lbl_8087F048;
            fn_800FAB80(mgr, this, fn_80219E6C(0x5C1), fn_800F8548(mgr), &m1514, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
            m14DC++;
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            SetState_unk(17);
            m14F0 = 0;
            m14F4 = 0;
            mMotion.m34C = 1;
            mMotion.mTracks[0].m20 = 1.0f;
            mMotion.fn_80097C08(0, 0x165, 0, 0, 1, 0.0f, 0.025f);
            mMotion.mTracks[0].m0C = 1.0f;
            if (lbl_8087F610 != 0) {
                fn_804DA48C(lbl_8087F610, 2);
            }
        }
    }
}

void Em206CocoonMp::fn_802C06B4() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
        fn_8016E970(3);
    }
}

void Em206CocoonMp::fn_802C0738() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetState_unk(0);
        fn_8016E970(3);
    }
}

void Em206CocoonMp::vf110(SpecialState_unk* state) {
    CocoonState_unk* s = (CocoonState_unk*)state;
    s->m00 = m58C;
    s->m04 = m14BC;
    s->m08 = m1900;
    s->m0A = m1904;
    s->m0C = m14F0;
    s->m0D = m1908;
    s->m0E = m1520;
}

// NONMATCHING: 99% - register allocation of the inlined death (Die_unk)
void Em206CocoonMp::vf114(const SpecialState_unk* state) {
    const CocoonState_unk* s = (const CocoonState_unk*)state;
    int next = s->m00;
    if (next == 2 && m1914 == 0) {
        vf44();
        Die_unk();
        return;
    }
    if (lbl_8087F610 != 0) {
        if ((next == 12 && m58C != 12) || (next == 17 && m58C != 17)) {
            fn_804DA48C(lbl_8087F610, 2);
        }
        if (s->m0D != 0 && m1908 == 0) {
            fn_804DA48C(lbl_8087F610, 5);
            CocoonDamage_unk damage;
            fn_8003EA3C(&damage, fn_80219E6C(0x5C2), this, this, 0, &lbl_807C6B90, 0, 0);
            if (GameBgm::sInstance != 0) {
                GameBgm::sInstance->fn_8037D4C0(0x8B8, 15, 1, 0, 1.0f);
            }
        }
        if (s->m00 == 15 && m58C != 15) {
            Effect_unk* mgr = lbl_8087F048;
            atn::Vector3 pos = vfAC();
            fn_80108F38(mgr, &pos, 0x4000);
        }
    }
    m58C = s->m00;
    m14BC = s->m04;
    m1900 = s->m08;
    m1904 = s->m0A;
    m14F0 = s->m0C;
    m1908 = s->m0D;
    m1520 = s->m0E;
}

int Em206CocoonMp::vfE4() {
    if (m58C == 10) {
        float frame = mMotion.mTracks[0].m08;
        if (80.0f <= frame && frame <= 105.0f) {
            return 1;
        }
    }
    return 0;
}

float Em206CocoonMp::vf90() {
    return 5.0f;
}

Em206CocoonMp::~Em206CocoonMp() {}
