#include "game/Em316LastLaser.h"
#include "game/CharaStatus.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/HitInfo_unk.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>
#include "atn/ColliBase.h"

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
// inferred: std::string concatenation (weak copy 0x8006AFF8)
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: sets bits of CharaParam_unk::m9C (weak copy 0x802660FC, first emitted by game/Em201ZanGurgFinal)
extern "C" inline void fn_802660FC(CharaParam_unk* param, unsigned int flag) {
    param->m9C |= flag;
}

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" int fn_80239D58(EffectMgr_unk* mgr, void* owner, int id); // inferred: the effect is playing
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
// inferred: effect/bullet manager of the characters (class unknown)
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" int fn_800F8548(BulletMgr_unk* mgr); // inferred: new bullet id

extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0;   extern "C" void* lbl_8087EE98; // inferred: collision world
extern "C" int fn_8004ED34(void* world, atn::ColliResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg); // inferred: ray test
extern "C" int atoi(const char* s);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: color as four floats (as in game/Em315Coffin.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
// inferred: game state (lbl_8087EFA8): time of a frame at 0x3A4; settings (lbl_8087F0A8): frames per second
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4;
};
extern "C" GameState_unk* lbl_8087EFA8;
struct LaserSettings_unk {
    unsigned char m00[0x30];
    int mFrameRate;
};
extern "C" LaserSettings_unk* lbl_8087F0A8;

// inferred: plays `efp` on `model` with default placement (as CoffinEffect_unk of game/Em315Coffin.cpp)
inline void LastLaserEffect_unk(atn::EfpFile* efp, atn::Model* model) {
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

inline void Em316LastLaser::SetAppear_unk(const atn::Vector3& pos, const atn::Vector3& offset) {
    m14D4 = pos;
    m14E0 = pos + offset;
    mPos = pos;
}

inline void Em316LastLaser::SetState_unk(int state) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = state;
    fn_8016E970(3);
}

inline void LaserList_unk::Erase_unk(LaserPoint_unk** it) {
    memmove(it, it + 1, (&this->mData[this->mSize] - it - 1) * sizeof(LaserPoint_unk*));
    this->mSize--;
}

// inferred: removes every point of the trail and stops its effects and its sound
inline void LaserTrace::Reset_unk() {
    LaserPoint_unk** it = m28.begin();
    m04 = 0;
    m34 = 0;
    while (it != m28.end()) {
        fn_80239DAC(lbl_8087F3C0, *it, 0, 0);
        delete *it;
        m28.Erase_unk(it);
    }
    if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
        fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    }
    fn_800CB5C8(&m38, 0, 0);
}

Em316LastLaser::Em316LastLaser(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14D4(0.0f, 0.0f, 0.0f),
      m14E0(0.0f, 0.0f, 0.0f), m14EC(0.25f), m1500(0.0f, 0.0f, 0.0f), m150C(0.0f, 0.0f, 0.0f),
      m1518(0.0f, 0.0f, 0.0f), m1530(90), m15C0(1), m15C4(0), m15C8(4), m15CC(8) {
    fn_802660FC(&GetStatus_unk()->mParamB, 0x10);
    std::string dir("database/chara/special/");
    std::string name("db_em601_00_00.csv");
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
    m1574.LoadRequest("ef_sk043f_01.eff");
    m1580.LoadRequest("ef_sk043c_01.eff");
    m158C.LoadRequest("ef_sk043a.efp");
    m1598.LoadRequest("ef_sk043d.efp");
    m15A4.LoadRequest("ef_sk043e.efp");
    m15B0.LoadRequest("ef_uc066.efp");
    m14C8 = lbl_807C7030;
}

// NONMATCHING: 97.2% - same register swap as ~LaserTrace in the inlined Reset_unk
Em316LastLaser::~Em316LastLaser() {
    m1538.Reset_unk();
}

// NONMATCHING: 97.91% - scheduling of the flag words at the end (the original loads 0x958 before m54C and
// stores m54C, 0x958, m18 in that order); ~10 variants (statement order, locals, permutations)
int Em316LastLaser::vf0C() {
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0 && m1574.IsLoading() == 0 && m1580.IsLoading() == 0 &&
        m158C.IsLoading() == 0 && m1598.IsLoading() == 0 && m15A4.IsLoading() == 0 &&
        m15B0.IsLoading() == 0) {
        void* group = m15BC.Load("Bit");
        fn_80087994(group, "DebugDisp", &m15C0, 0, 0);
        fn_80087994(group, "DebugLog", &m15C4, 0, 0);
        fn_8008771C(group, "AppearMoveRate", &m14EC, 0, 0, 0.01f, 0.99f, 0.1f);
        fn_800874C8(group, "ActionMode", &m58C, 1, 100, 1, 0, 0);
        fn_80087E9C(group, "Pos", &mPos, 0, 0, 1.0f, 100.0f, 1.0f);
        if (m14B0.fn_80470580() != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                if (strcmp(csv.fn_8005B9CC(), "LaserID") == 0) {
                    m1534 = fn_80219E6C(atoi(csv.fn_8005B9CC()));
                }
            } while (csv.fn_8005BBF8());
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0xC21D;
        mStatus.m18 |= 0x3880000;
        m54C |= 0x200;
        fn_802660FC(&mStatus.mParamB, 0x2A);
        m1538.m08 = &m1574;
        m1538.m0C = &m1580;
        m14C8 = mPos;
        return 1;
    }
    return 0;
}

// inferred: node position (as in game/Em205Ogre.cpp; name invented)
inline atn::Vector3 GetNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: m = m * rotation(r) z, y, x (as in game/Em403WarLion.cpp; name invented)
inline atn::Matrix3x4& MakeRotationZYX_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    return m;
}

// inferred: stops the sound of the trail when it is off and removes the points whose time is over
inline void LaserTrace::Update_unk() {
    if (m04 == 0) {
        if (fn_80239D58(lbl_8087F3C0, this, 0) != 0) {
            fn_80239DAC(lbl_8087F3C0, this, 0, 0);
        }
        fn_800CB5C8(&m38, 10, 0);
    }
    LaserPoint_unk** it = m28.begin();
    while (it != m28.end()) {
        (*it)->m4C--;
        if ((*it)->m4C <= 0) {
            fn_80239DAC(lbl_8087F3C0, *it, 0, 0);
            delete *it;
            m28.Erase_unk(it);
        } else {
            it++;
        }
    }
}

// inferred: squared length (as in game/CrowdSimulation.cpp; name invented)
inline float LengthSq_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: the player can be aimed at (name invented)
inline bool CanTarget_unk(CharaTask* chara) {
    return chara->IsFree_unk() && !chara->IsGrappled_unk();
}

// inferred: distance from `pos` to the "spine1" node of the target (computed and not used; name invented). A static
// function of the file: its literal is in @stringBase0 (an inline member would put it in @STRING@)
static inline float SpineDist_unk(const atn::Vector3& pos, CharaTask* target) {
    MtxPtr mtx = target->mMotion.GetNodeMtx_unk("spine1");
    atn::Vector3 p(0.0f, 0.0f, 0.0f);
    if (mtx != 0) {
        p = GetNodePos_unk(mtx);
    } else {
        p = target->mPos;
        p.y += 10.0f;
    }
    return (pos - p).Length_unk();
}

// inferred: position of a character by value (name invented)
inline atn::Vector3 CharaPos_unk(CharaTask* chara) {
    return chara->mPos;
}

// inferred: collision capsule standing on the position (own version of CharaTask::vf20; name invented)
inline void Em316LastLaser::UpdateColli_unk() {
    m614 = mPos + m5A4;
    m614.y += m5B0;
    m620 = m5B0;
    atn::Vector3 a(mPos);
    atn::Vector3 b(mPos);
    b.y += 0.1f;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = m5B0;
}

// NONMATCHING: 95.3% - stack slot of the copy of the target position (the original creates it before the
// temporaries of GetNormalized/ToRotation), r28-r31 assignment and the string of "spine1" (@STRING@ of the
// inline helper instead of @stringBase0)
// inferred: update: picks the nearest free player, runs the state and casts the laser forward
void Em316LastLaser::vf10() {
    mC58.mC8 = (int)mC58.mC4;
    if (m58C - 7U > 2) {
        CharaTask* best = 0;
        float bestDist = 1e16f;
        for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
            if (CanTarget_unk(p)) {
                float d = LengthSq_unk(p->mPos - mPos);
                if (d < bestDist) {
                    best = p;
                    bestDist = d;
                }
            }
        }
        m14B8 = best;
        m14A8 |= 0x04000000;
    } else {
        m14A8 &= ~0x04000000;
    }
    m1454 = 0;
    mC58.mC4 = m14B8;
    m14C0++;
    if (mC58.mC0 == 0 || m14B8 == 0) {
        fn_8016E970(3);
        if (m1538.m04 != 0) {
            m1538.Reset_unk();
        }
        fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
        fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
        fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
    } else if (m14B8 != 0) {
        m1530--;
        switch (m58C) {
        case 2:
            vfEC();
            break;
        case 6:
            if (m14C0 >= 60) {
                fn_8031D2BC();
                m1530 = rand() % 90 + 15;
            }
            break;
        case 7:
            if (m14C0 >= 60) {
                fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
                fn_8031D350();
            }
            break;
        case 8:
            fn_8031CA30();
            break;
        case 9:
            if (m14C0 > 50) {
                fn_8031D2BC();
                fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
            }
            break;
        default:
            if (m14B8 != 0) {
                mRot = (CharaPos_unk(m14B8) - mPos).GetNormalize().ToRotation_unk();
            }
            if (CharaTask* target = m14B8) {
                SpineDist_unk(mPos, target);
                if (m1530 < 0) {
                    fn_8031D350();
                }
            }
            break;
        }
    }
    fn_8014C540();
    fn_80145334();
    UpdateColli_unk();
    if (m1538.m04 != 0) {
        atn::Vector3 dir(0.0f, 0.0f, 10000.0f);
        atn::Matrix3x4 m;
        MakeRotationZYX_unk(m, mRot);
        PSMTXMultVec(m.m, &dir, &dir);
        atn::Vector3 to = mPos + dir;
        atn::ColliResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &mPos, &to, 0x80000004, &mColli, 0) != 0) {
            to = result.m10;
            m1538.fn_8031E8BC(to, result.m28);
        }
    }
    m1538.Update_unk();
}

void Em316LastLaser::vf14() {
    fn_80149A30();
}

void Em316LastLaser::vf3C(HitInfo_unk* hit) {
    fn_80151448((HitMsg_unk*)hit);
    vf40(hit);
}

// NONMATCHING: 99.66% - r4/r5 of the two globals of the gravity (lbl_8087EFA8 / the double constant);
// ~18 variants of the gravity expression
void Em316LastLaser::vf40(HitInfo_unk* hit) {
    if ((mStatus.m0C & 0x20) == 0x20) {
        atn::Vector3 epos;
        atn::Vector3 erot;
        Color4f_unk color;
        SetState_unk(2);
        fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
        atn::Vector3 d(m6B8);
        PSVECNormalize(&d, &d);
        m14F0 = d * 2.0f;
        m14F0.y *= 0.5f;
        if (m14F0.y > 0.0f) {
            m14F0.y = 0.0f;
        }
        float dt = lbl_8087EFA8->m3A4;
        int fps = lbl_8087F0A8->mFrameRate;
        float g = -98.1f / (float)(fps * fps);
        m14FC = 1.5f * (g * dt);
        fn_800EB7A0();
        fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
        fn_80232B7C(this, 0x66);
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
        fn_8023A8B4(lbl_8087F3C0, &m15A4, &mMotion, 0, epos, erot, color, -1, -1, 1, 1.0f);
        m1538.m04 = 0;
    }
}

// inferred: same body as vf80, inlined by vfEC (the original inlines vf80 there); name invented
inline void Em316LastLaser::Appear_unk(const atn::Vector3& pos, const atn::Vector3& offset) {
    atn::Vector3 epos;
    atn::Vector3 erot;
    Color4f_unk color;
    SetState_unk(6);
    SetAppear_unk(pos, offset);
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
    fn_8023A8B4(lbl_8087F3C0, &m15B0, &mMotion, 0, epos, erot, color, -1, -1, 1, 1.0f);
}

void Em316LastLaser::vf80(const atn::Vector3& pos, const atn::Vector3& offset) {
    atn::Vector3 epos;
    atn::Vector3 erot;
    Color4f_unk color;
    SetState_unk(6);
    SetAppear_unk(pos, offset);
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
    fn_8023A8B4(lbl_8087F3C0, &m15B0, &mMotion, 0, epos, erot, color, -1, -1, 1, 1.0f);
}


// NONMATCHING: 62.6% - the original inlines vf80 here and keeps the addresses of both arguments (m14C8 and the
// zero vector) in saved registers from before the state reset; with a copy of the body as an inline MWCC
// rematerializes them after the calls (~8 variants: references, pointers, devirtualized call)
void Em316LastLaser::vfEC() {
    if (m14C0 >= 120) {
        mStatus.fn_8012D8B8();
        Appear_unk(m14C8, lbl_807C7030);
    } else {
        m14F0.y += m14FC;
    }
    mPos += m14F0;
}

// inferred: inline in the original (weak copy 0x8013C3B4, defined out of line in game/CharaTask.cpp)
inline atn::Vector3 atn::Vector3::operator-() const {
    return atn::Vector3(-x, -y, -z);
}
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800FDB20(BulletMgr_unk* mgr, CharaTask* owner, CharaTask* target, int arg, HitInfo_unk* data, int id,
                            const atn::Vector3* pos, const atn::Vector3* dir, int time, float scale);
extern "C" void fn_80101434(BulletMgr_unk* mgr, const atn::Vector3* pos, const atn::Vector3* rot, int a, int b);

// inferred: state 9: the laser sweeps from m150C along m1524 for 90 frames and hurts the character it hits (skill
// 0x76E); then back to the wait with a new delay
void Em316LastLaser::fn_8031CA30() {
    if (m14C0 > 90) {
        fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
        SetState_unk(9);
        fn_80232B7C(this, 0x66);
        LastLaserEffect_unk(&m15A4, (atn::Model*)&mMotion);
        m1538.m04 = 0;
        m1530 = rand() % 90 + 15;
    } else {
        m150C += m1524 * 1.0f;
        m1500 = m150C - mPos;
        m1500.Normalize_unk();
        mRot = m1500.ToRotation_unk();
        atn::Vector3 dir(0.0f, 0.0f, 10000.0f);
        atn::Matrix3x4 m;
        MakeRotationZYX_unk(m, mRot);
        PSMTXMultVec(m.m, &dir, &dir);
        atn::Vector3 to = mPos + dir;
        atn::ColliResult_unk result;
        if (fn_8004ED34(lbl_8087EE98, &result, &mPos, &to, 0x40000000, &mColli, 0) != 0 && result.m38 != 0 &&
            (result.m38->mFlags & 2) == 2) {
            HitInfo_unk* skill = fn_80219E6C(0x76E);
            CharaTask* chara = (CharaTask*)result.m38->m0C;
            if (!chara->fn_801533C8(m590, skill)) {
                atn::Vector3 rot = (-dir).ToRotation_unk();
                fn_800FDB20(lbl_8087F048, this, chara, 0, skill, m590, &result.m10, &m1500, 15, 0.1f);
                fn_80101434(lbl_8087F048, &result.m10, &rot, skill->mC4, 0);
                fn_800C344C("SE_BTCOM_006", &result.m10, 0, -1, 1.0f);
            }
        }
    }
}

void Em316LastLaser::fn_8031D27C() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
}

void Em316LastLaser::fn_8031D2BC() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x65, 0);
    fn_80239DAC(lbl_8087F3C0, this, 0x66, 0);
}

// inferred: weak copies of other files called out of line by the functions over the inline budget below (as in
// game/Em316LaserSphere.cpp and game/Em201ZanGurgFinal.cpp)
// inferred: asserts of the debug build, empty in release: with them fn_8031D350 and LaserTrace::fn_8031E8BC go over
// the inline budget as the original (docs/inlining.md R3)
#define LASER_ASSERT_UNK(x) ((void)0)
#define LASER_ASSERT10_UNK(x)                                                                                  \
    LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x);      \
    LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x); LASER_ASSERT_UNK(x)
#define LASER_ASSERT100_UNK(x)                                                                                 \
    LASER_ASSERT10_UNK(x); LASER_ASSERT10_UNK(x); LASER_ASSERT10_UNK(x); LASER_ASSERT10_UNK(x);                  \
    LASER_ASSERT10_UNK(x); LASER_ASSERT10_UNK(x); LASER_ASSERT10_UNK(x); LASER_ASSERT10_UNK(x);                  \
    LASER_ASSERT10_UNK(x); LASER_ASSERT10_UNK(x)
extern "C" inline void fn_802657FC(CharaTask* chara, int state) {
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    LASER_ASSERT100_UNK(chara != 0);
    chara->m58C = state;
}
extern "C" inline EffectMgr_unk* fn_800F7FA0() {
    return lbl_8087F3C0;
}
extern "C" inline void fn_800F7FA8(EffectMgr_unk* mgr, void* owner, int arg) {
    LASER_ASSERT100_UNK(mgr != 0);
    LASER_ASSERT100_UNK(mgr != 0);
    LASER_ASSERT100_UNK(mgr != 0);
    LASER_ASSERT100_UNK(mgr != 0);
    LASER_ASSERT100_UNK(mgr != 0);
    LASER_ASSERT100_UNK(mgr != 0);
    fn_80232B7C(owner, arg);
}
// inferred: plays an effect on the model (as in game/Em201ZanGurgFinal.cpp)
extern "C" inline void fn_8026607C(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, int arg) {
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
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, arg, 1.0f);
}
inline void* fn_801404F8() {
    return lbl_8087EE98;
}
extern "C" void fn_8023A680(EffectMgr_unk* mgr, void* res, int a, int b, void* mtx, const atn::Vector3* pos,
                            const atn::Vector3* rot, const float* color, float scale, int e, int f, int g);
extern "C" float fn_801A03E8(const atn::Vector3& a, const atn::Vector3& b); // inferred: dot product
extern "C" void fn_800CB6E4(atn::SoundRef* sound, const atn::Vector3* pos); // inferred: moves the sound
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
// inferred: translation matrix by value, rotation and scale (weak copies of another file, as in
// game/BattleTask.cpp)
extern "C" inline atn::Matrix3x4 fn_800F80A8(const atn::Vector3& pos) {
    atn::Matrix3x4 m;
    PSMTXTrans(m.m, pos.x, pos.y, pos.z);
    return m;
}
extern "C" inline void fn_800F80B8(MtxPtr mtx, const atn::Vector3& rot) {
    ((atn::Matrix3x4*)mtx)->RotateZYX_unk(rot);
}
extern "C" inline void fn_800F8290(MtxPtr mtx, float x, float y, float z) {
    Mtx s;
    PSMTXScale(s, x, y, z);
    Mtx t;
    PSMTXConcat(mtx, s, t);
    __MtxCopy(mtx, t);
}

static void fn_8031D6DC(LaserTrace* t, const atn::Vector3& pos, const atn::Vector3& normal);
static inline void fn_8031E078(LaserTrace* t, const atn::Vector3& pos);

// inferred: state 8: aims at the target from 40 units in front of it and starts the laser where the ray hits
void Em316LastLaser::fn_8031D350() {
    fn_8031D27C();
    fn_802657FC(this, 8);
    fn_8016E970(3);
    fn_800F7FA8(fn_800F7FA0(), this, 0x65);
    fn_8026607C(fn_800F7FA0(), &m1598, (atn::Model*)&mMotion, 1);
    atn::Vector3 dir = *m14B8->fn_8013C38C() - mPos;
    dir.Normalize_unk();
    m1524 = dir;
    m1524.y = 0.0f;
    m1524.Normalize_unk();
    m150C = *m14B8->fn_8013C38C() - m1524 * 40.0f;
    dir = (m150C - mPos).GetNormalize();
    atn::Vector3 to = mPos + dir * 1000.0f;
    atn::ColliResult_unk result;
    if (fn_8004ED34(fn_801404F8(), &result, &mPos, &to, 0x80000004, &mColli, 0) != 0) {
        fn_8031D6DC(&m1538, result.m10, result.m28);
        mRot = (result.m10 - mPos).GetNormalize().ToRotation_unk();
    } else {
        fn_8031D2BC();
    }
}

LaserTrace::LaserTrace()
    : m04(0), m08(0), m10(0.0f, 0.0f, 0.0f), m1C(0.0f, 1.0f, 0.0f), m34(0) {
    m28.__shrink_unk(0);
}

// NONMATCHING: 91.7% - registers: the original keeps the iterator in r29 and `this` in r30 (here r31/r29);
// tried the statement order, a for loop and a local for the deleted point
LaserTrace::~LaserTrace() {
    Reset_unk();
}

// inferred: restarts the trail at `pos` (static of the file, see fn_8031E078; called by fn_8031D350)
// NONMATCHING: 98.4% - as fn_8031E078 (frame 0x10 smaller, offsets of "" in @stringBase0 until fn_8031CA30 is written)
static void fn_8031D6DC(LaserTrace* t, const atn::Vector3& pos, const atn::Vector3& normal) {
    t->Reset_unk();
    t->m04 = 1;
    t->m10 = pos;
    t->m1C = normal;
    fn_8031E078(t, pos);
    t->m34 = t->m28.back();
    t->m38 = fn_800C344C("SE_DG040_009", &pos, 0, -1, 1.0f);
}

LaserPoint_unk** LaserList_unk::Back_unk() {
    return &(*this)[size() - 1];
}

// inferred: new segment from the end of the trail (m10) to `pos`, its effect placed at `pos` (name keeps the
// address). Static functions of the file in the original: local symbols without a name at 0x8031D6DC/0x8031E078
// and their literals in @stringBase0 (an inline member would put them in @STRING@). Inlined in fn_8031D6DC, called
// by fn_8031E8BC
// NONMATCHING: 98.9% - the original frame is 0x10 larger (one more temporary of 12 bytes above the difference) and
// "" sits at 0xE0 of @stringBase0 (strings of fn_8031CA30, not written yet); tried a named difference
static inline void fn_8031E078(LaserTrace* t, const atn::Vector3& pos) {
    LaserPoint_unk* p = new ((MemoryTag)1, "", "", 0) LaserPoint_unk;
    p->m30 = t->m10;
    atn::Vector3 dir = pos - t->m10;
    p->m3C = dir;
    p->m48 = p->m3C.Length_unk();
    if (!(atn::Abs_unk(p->m48) < 1.1920929e-7f)) {
        p->m3C.Normalize_unk();
    }
    p->m4C = 30;
    p->m00 = fn_800F80A8(pos);
    fn_800F80B8(p->m00.m, p->m3C.ToRotation_unk());
    fn_800F8290(p->m00.m, 1.0f, 1.0f, p->m48 / 10.0f);
    t->m28.push_back(p);
    if (t->m08 != 0) {
        fn_80232B7C(p, 0);
        fn_8023A680(lbl_8087F3C0, t->m08, -1, 5, p, 0, 0, 0, 1.0f, 0, -1, 1);
    }
}


// inferred: the end of the laser moved to `pos`: the last segment is stretched, or a new one is started when the
// laser turned or the segment is long enough; over the inline budget as the original
void LaserTrace::fn_8031E8BC(const atn::Vector3& pos, const atn::Vector3& normal) {
    if (m04 == 0) {
        if (fn_80239D58(fn_800F7FA0(), this, 0) != 0) {
            fn_80239DAC(fn_800F7FA0(), this, 0, 0);
        }
        return;
    }
    if (fn_80239D58(fn_800F7FA0(), this, 0) == 0) {
        fn_800F7FA8(fn_800F7FA0(), this, 0);
        fn_8023A680(fn_800F7FA0(), m0C, -1, 6, &m10, 0, 0, 0, 1.0f, 0, -1, 1);
    }
    atn::Vector3 d = pos - m34->m30;
    float len = d.Length_unk();
    if (len < 0.0001f) {
        return;
    }
    int split = 0;
    if (fn_801A03E8(d.GetNormalize(), normal) > 0.0001f) {
        split = 1;
    }
    if (m34->m48 > 0.0f && fn_801A03E8(d.GetNormalize(), m34->m3C) < 0.99f) {
        split = 1;
    }
    if (m34->m48 >= 12.0f) {
        split = 1;
    }
    if (split == 0) {
        if (m34->m48 <= 0.0f) {
            m34->m3C = d.GetNormalize();
        }
        m34->m48 = len;
        m34->m00 = fn_800F80A8(m34->m30);
        fn_800F80B8(m34->m00.m, m34->m3C.ToRotation_unk());
        fn_800F8290(m34->m00.m, 1.0f, 1.0f, m34->m48 / 10.0f);
        m34->m4C = 30;
    } else {
        fn_8031E078(this, pos);
        m34 = *m28.Back_unk();
    }
    fn_800CB6E4(&m38, &pos);
    m10 = pos;
    m1C = normal;
}
