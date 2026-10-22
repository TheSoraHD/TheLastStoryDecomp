#include "game/Em316LastLaser.h"
#include "game/CharaStatus.h"
#include "atn/Csv.h"
#include "game/HitInfo_unk.h"
#include "math/Vec.h"
#include "std/string.h"
#include "std/string_members.h"
#include <boost/tokenizer.hpp>

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
extern "C" int atoi(const char* s);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
// inferred: color as four floats (as in game/Em315Coffin.cpp)
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
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
inline void LastLaserEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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
    m1574.fn_80237654("ef_sk043f_01.eff");
    m1580.fn_80237654("ef_sk043c_01.eff");
    m158C.fn_8023780C("ef_sk043a.efp");
    m1598.fn_8023780C("ef_sk043d.efp");
    m15A4.fn_8023780C("ef_sk043e.efp");
    m15B0.fn_8023780C("ef_uc066.efp");
    m14C8 = lbl_807C7030;
}

// NONMATCHING: 97.2% - same register swap as ~LaserTrace in the inlined Reset_unk
Em316LastLaser::~Em316LastLaser() {
    m1538.Reset_unk();
}

// NONMATCHING: 97.91% - scheduling of the flag words at the end (the original loads 0x958 before m54C and
// stores m54C, 0x958, m18 in that order); ~10 variants (statement order, locals, permutations)
int Em316LastLaser::vf0C() {
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0 && m1574.fn_802376D0() == 0 && m1580.fn_802376D0() == 0 &&
        m158C.fn_80237874() == 0 && m1598.fn_80237874() == 0 && m15A4.fn_80237874() == 0 &&
        m15B0.fn_80237874() == 0) {
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
        fn_800EB7A0(this);
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

LaserTrace::LaserTrace()
    : m04(0), m08(0), m10(0.0f), m14(0.0f), m18(0.0f), m1C(0.0f), m20(1.0f), m24(0.0f), m34(0) {
    m28.__shrink_unk(0);
}

// NONMATCHING: 91.7% - registers: the original keeps the iterator in r29 and `this` in r30 (here r31/r29);
// tried the statement order, a for loop and a local for the deleted point
LaserTrace::~LaserTrace() {
    Reset_unk();
}

LaserPoint_unk** LaserList_unk::Back_unk() {
    return &(*this)[size() - 1];
}
