#include "game/LastCocoonEgg.h"
#include "game/PopupTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/GameTask.h"
#include <boost/tokenizer.hpp>
#include "std/string_members.h"
#include "std/string.h"
#include "atn/Keyboard.h"
#include "atn/Vector4_unk.h"
#include "macros.h"

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, void* model, int arg3, const atn::Vector3& pos,
                            const atn::Vector3& rot, const Color4f_unk& color, int arg7, int arg8, int arg9,
                            float scale);
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)               // inferred: popup text manager
// inferred: camera shake request of the field (lbl_8087F430); as in gmk/ShakeCamera.cpp
struct EggField_unk {
    unsigned char m00[0x96C];
    int m96C;
    int m970;   // inferred: frames
    float m974; // inferred: x amplitude
    float m978; // inferred: y amplitude
    // inferred: starts a camera shake (name invented)
    void Shake_unk(int frame, float x, float y) {
        m96C = m96C % 2;
        m970 = frame;
        m974 = x;
        m978 = y;
    }
};
extern "C" EggField_unk* lbl_8087F430;

extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" std::string fn_8006AFF8(const std::string& a, const std::string& b); // inferred: a + b
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: sets flags of the parameters (weak copy of other files, as in game/Em201ZanGurgFinal.cpp)
extern "C" inline void fn_802660FC(CharaParam_unk* param, unsigned int flags) {
    param->m9C |= flags;
}


LastCocoonEgg::LastCocoonEgg(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B8(0), m14BC(0), m14C0(0), m14C4(0), m14C8(0), m14CC(1), m14D0(1.0f),
      m14D4(0.05f), m14D8(0.0f), m14DC(1.0f), m14E0(0), m14E4(0), m14EC(0, 0x23A), m190C(0), m1910(0),
      m1914(0.0f, 0.0f, 0.0f), m194C(2), m1980(0), m1984(0), m2558(0), m255C(0.1f), m2A44(0), m2A48(40.0f),
      m2A4C(1) {
    mFlag22 = 1;
    fn_802660FC(&GetStatus_unk()->mParamB, 0x10);
    fn_802660FC(&GetStatus_unk()->mParamB, 0x2);
    fn_802660FC(&GetStatus_unk()->mParamB, 0x8);
    fn_802660FC(&GetStatus_unk()->mParamB, 0x20);
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
    m14EC.fn_8008B978(1);
    m14EC.fn_8008AD4C("/data/model/em314_00_bs00.model", 0);
    m14EC.fn_80097A88(0x13F, "/data/motion/em314_atn00a_00.motion");
    m14EC.fn_80097A88(0x140, "/data/motion/em314_atn00b_00.motion");
    m14EC.fn_80097A88(0x141, "/data/motion/em314_atn00c_00.motion");
    m18EC = lbl_807C7030;
    m18F8 = lbl_807C7030;
    m18BC.SetIdentity();
    m1904 = 0;
    m1908 = 0;
    m19A0.clear();
    m1920 = 0;
    m1924 = 30;
    m1928.LoadRequest("ef_bs039a.efp");
    m1934.LoadRequest("ef_bs039d.efp");
    m1940.LoadRequest("ef_bs039e.efp");
    m1950.LoadRequest("ef_bs040a.efp");
    m195C.LoadRequest("ef_bs040c.efp");
    m1988.LoadRequest("ef_bs034c.efp");
    m1994.LoadRequest("ef_bs034b_01.eff");
    m252C.LoadRequest("ef_bs045a.efp");
    m2538.LoadRequest("ef_bs045e.efp");
}

LastCocoonEgg::~LastCocoonEgg() {}

// ---- vf10: update of the egg (over the inlining budget: the accessors are weak copies) ----
extern "C" double floor(double x);
// inferred: hit point ratio (weak copy 0x8029F3AC of game/Em203GreaterDaemonRedMp.cpp)
extern "C" inline float fn_8029F3AC(CharaStatus* status) {
    if (status->mParamB.m84 > 0) {
        return status->m04 / status->mParamB.m84;
    }
    return 0.0f;
}
// inferred: weak copy 0x80454514 (name invented)
extern "C" __declspec(noinline) inline float fn_80454514(float x) {
    return floor(x);
}

// NONMATCHING: draft - the original is over the inlining budget (its accessors and Keyboard::Press are called)
void LastCocoonEgg::vf10() {
    fn_80455E20();
    if (m2A44 != 0) {
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4D) && m14E4 == 0) {
            fn_804570F4(m14B8);
        }
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4B)) {
            fn_80456E1C(m14B8);
        }
        if (atn::Keyboard::Order()->Press(atn::Keyboard::PORT0_unk, 0x4E)) {
            fn_80456F04();
        }
    }
    m14C0++;
    m14E8++;
    if (mC58.mC0 == 0 || m14B8 == 0) {
        fn_8016E970(3);
        m14E0 = 0;
        m14C0 = 0;
        m14E8 = 0;
        if (mC58.mC0 == 0) {
            mStatus.m04 = 1.0f;
        }
    } else {
        if (m1920 == 0 && m1924 >= 0) {
            fn_80455D80();
        }
        if (m1980 == 0 && fn_8029F3AC(&mStatus) >= 1.0f) {
            fn_80456F94();
        }
        m14E0 = 1;
        if (m14B8 != 0) {
            switch (m58C) {
            case 6:
                fn_80456194();
                break;
            case 7:
                fn_804562E4();
                break;
            case 8:
                fn_80456DB8();
                break;
            default:
                fn_8045617C();
                if (m55C == 3) {
                    fn_80455F2C();
                }
                break;
            }
        }
        switch (m14E4) {
        case 1:
            fn_804576D4();
            break;
        case 2:
            fn_80457C48();
            break;
        case 3:
            fn_80457E98();
            break;
        default:
            if (m58C != 8) {
                if (m1910 > 0 && --m1910 == 0) {
                    m190C = 0;
                }
                for (unsigned int i = 0; i < m19A0.size(); i++) {
                    atn::Vector3 d = *m19A0[i].m00->fn_8013C38C() - mPos;
                    if (d.LengthSq_unk() < 2500.0f && m19A0[i].m00 != m190C) {
                        fn_804570F4(m19A0[i].m00);
                        m190C = m19A0[i].m00;
                        m1910 = 120;
                        break;
                    }
                }
            }
            break;
        }
        if (m58C != 8) {
            m14D0 = 1.0f + fn_8029F3AC(&mStatus);
            m14D0 = fn_80454514(2.0f * m14D0) * 0.5f;
        }
        fn_80455638();
        m2548.fn_80454538();
    }
    fn_8014C540();
    fn_80454F08();
    fn_804560A4();
    fn_80454D4C();
    fn_8045531C();
}

void LastCocoonEgg::vf3C(HitInfo_unk* hit) {
    vf40(hit);
}

// inferred: plays an effect on the model with the default color and scale (name invented)
// inferred: same with the vectors passed by the caller (names invented)
static inline void PlayEfp2_unk(atn::EfpFile* efp, void* model, const atn::Vector3& pos, const atn::Vector3& rot) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
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
// inferred: same, other layout of the locals (names invented)
static inline void PlayEfp3_unk(atn::EfpFile* efp, void* model) {
    Color4f_unk color;
    atn::Vector3 rot;
    atn::Vector3 pos;
    rot.Set_unk(0.0f, 0.0f, 0.0f);
    pos.Set_unk(0.0f, 0.0f, 0.0f);
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
static inline void PlayEfp4_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, void* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    rot.Set_unk(0.0f, 0.0f, 0.0f);
    pos.Set_unk(0.0f, 0.0f, 0.0f);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
// inferred: the hit is guarded (no damage, reaction 1)
static inline void Guard_unk(HitMsg_unk* msg) {
    msg->m68 = 0;
    msg->m64 = 1;
    msg->m90 = 1;
    msg->m94 |= 8;
}

void LastCocoonEgg::vf40(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m1980 != 0) {
        Guard_unk(msg);
        return;
    }
    for (unsigned int i = 0; i < m19A0.size(); i++) {
        if (msg->mAttacker == m19A0[i].m00) {
            m19A0[i].m0C++;
        }
    }
    if (--m1924 < 0) {
        m14BC = 0;
        m14C0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 8;
        fn_8016E970(3);
        fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
        fn_80232B7C(0, 0);
        PlayEfp4_unk(lbl_8087F3C0, &m1940, &mMotion);
        m2558 = 3;
        m1980 = 1;
        m255C = 0.3f;
        m14D0 = 4.0f;
        m14D4 = 0.05f;
        m2548.Shake_unk(0x58, 2.0f, 2.0f);
        lbl_8087F430->Shake_unk(60, 1.5f, 1.5f);
        atn::Vector3 pos = fn_80148B0C(0)->m04.mCenter;
        pos += atn::Vector3(0.0f, 15.0f, 0.0f);
        lbl_8087F8A8->fn_8054D794(0, L"Shield Break!!", &pos, 0, 0, 1, 0, 0, 0, 1, -1, -1);
    } else {
        fn_80232B7C(0, 0);
        PlayEfp3_unk(&m1934, &mMotion);
        Guard_unk(msg);
    }
}

atn::Vector3 LastCocoonEgg::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else {
        result = m624.mData[0].m04.mCenter;
    }
    return result;
}

// inferred: column of a matrix (name invented)
inline atn::Vector3 EggColumn_unk(const atn::Matrix3x4& mtx, int i) {
    return atn::Vector3(mtx.m[0][i], mtx.m[1][i], mtx.m[2][i]);
}

// inferred: largest scale axis of a matrix from its column lengths (as in CharaTask's model update; name
// invented)
inline float EggMaxScale_unk(const atn::Matrix3x4& mtx) {
    float sz = EggColumn_unk(mtx, 2).Length_unk();
    float sy = EggColumn_unk(mtx, 1).Length_unk();
    float sx = EggColumn_unk(mtx, 0).Length_unk();
    atn::Vector3 s(sx, sy, sz);
    return MAX(s.x, MAX(s.y, s.z));
}

// inferred: updates the matrices of the debug model m14EC from m18BC (largest scale axis from the column
// lengths, as CharaTask's model update)
void LastCocoonEgg::fn_80454D4C() {
    if (m14E4 != 0) {
        const atn::Matrix3x4& mtx = m18BC;
        __MtxCopy(m14EC.mMtx.m, mtx.m);
        m14EC.m54 = EggMaxScale_unk(mtx);
        m14EC.m20C_15_8 = m14EC.m50;
        m14EC.m50 = 0;
        m14EC.fn_80097E80(1);
        m14EC.CalcMatrixStack(atn::ModelNodeCallback_unk());
    }
}

void LastCocoonEgg::fn_80455D80() {
    fn_80232B7C(this, 1000);
    PlayEfp_unk(&m1928, &mMotion);
    m1920 = 1;
}

void LastCocoonEgg::fn_80455E20() {
    float best = 0.0f;
    CharaTask* target = 0;
    mC58.mC8 = (int)mC58.mC4;
    for (unsigned int i = 0; i < m19A0.size(); i++) {
        atn::Vector3 d = m19A0[i].m00->mPos - mPos;
        float dist = PSVECSquareMag(&d);
        if (dist > best && !m19A0[i].m00->mStatus.IsState_unk(0x20000)) {
            best = dist;
            target = m19A0[i].m00;
        }
    }
    if (target == 0) {
        target = lbl_8087F8A0->mHead;
    }
    m1454 = 0;
    m14B8 = target;
    mC58.mC4 = target;
}

// inferred: the shield breaks (as vf40 without the popup text)
// NONMATCHING: pool offset of the string (the constructor and its literals are missing)
// inferred: after 45 frames, repeats the attack of fn_80456E1C twice and then starts the shake of fn_80456F04
void LastCocoonEgg::fn_80455F2C() {
    if (m14C0 > 45) {
        if (m194C < 2) {
            m194C++;
            fn_80456E1C(m14B8);
        } else {
            m194C = 0;
            fn_80456F04();
        }
    }
}

// NONMATCHING: 80% - float registers and the order of the copies of the collision center
void LastCocoonEgg::fn_804560A4() {
    float radius = m5B0 * m14D0;
    m614 = mPos + m5A4;
    m620 = radius;
    atn::Vector3 a = m614;
    atn::Vector3 b = m614;
    float h = m14D0 * m5B4 - 2.0f * radius;
    b.y += h > 0.0f ? h : 0.0f;
    mColli.m3C = a;
    mColli.m48 = b;
    mColli.m54 = radius;
}

void LastCocoonEgg::fn_8045617C() {
    if (m55C != 6) {
        fn_8016E970(3);
    }
}

void LastCocoonEgg::fn_80456194() {
    if (m14BC == 0) {
        if (m14C0 > 20) {
            m14C0 = 0;
            m14BC = 1;
            m1968 = m14B8->mPos;
            m1974 = m14B8->mRot;
            m1974.z = 0.0f;
            m1974.x = 0.0f;
            fn_8023A8B4(lbl_8087F3C0, &m195C, 0, 0, m1968, m1974, Color4f_unk(1.0f, 1.0f, 1.0f, 1.0f), -1, -1, 1,
                        1.0f);
        }
    } else if (m14C0 > 15) {
        Effect_unk* mgr = lbl_8087F048;
        fn_800FAB80(mgr, this, fn_80219E6C(0x5AA), m590, &m1968, &m1974, 0, 30, -1, -1, 0.0f, 1.0f);
        m14BC = 0;
        m14C0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 0;
        fn_8016E970(3);
        m2558 = 0;
        m14CC = 1;
    }
}

void LastCocoonEgg::fn_80456DB8() {
    if (m14BC == 0) {
        if (m14C0 > 90) {
            m14C0 = 0;
            m14BC = 1;
            m2558 = 2;
            m255C = 0.05f;
        }
    } else if (m14BC == 1) {
        if (m14C0 > 60) {
            ((GameTask*)lbl_8087F430)->fn_80370AE4(0x5A, 1);
        }
    }
}

// NONMATCHING: pool offset of the string (the constructor and its literals are missing)
// inferred: starts the state 6 (sound and effect m1950); the target is not read
void LastCocoonEgg::fn_80456E1C(CharaTask* target) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(3);
    fn_800C344C("SE_EM206_027", &mPos, 0, -1, 1.0f);
    PlayEfp_unk(&m1950, &mMotion);
}

void LastCocoonEgg::fn_80456F04() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 7;
    fn_8016E970(3);
    m2548.Shake_unk(50, 1.5f * m14D0, 1.5f * m14D0);
    m2558 = 1;
}

void LastCocoonEgg::fn_80456F94() {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 8;
    fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, this, 1000, 0);
    fn_80232B7C(0, 0);
    PlayEfp2_unk(&m1940, &mMotion, atn::Vector3(0.0f, 0.0f, 0.0f), atn::Vector3(0.0f, 0.0f, 0.0f));
    m1980 = 1;
    m2558 = 3;
    m255C = 0.3f;
    m14D0 = 4.0f;
    m14D4 = 0.05f;
    m2548.Shake_unk(0x58, 2.0f, 2.0f);
    lbl_8087F430->Shake_unk(60, 1.5f, 1.5f);
}

// NONMATCHING: registers of the inlined CharaTask::CanAct_unk (the character in r8 in the original)
// inferred: debug action 2: throws the caught character (or plays the motion 0x141 while it is held)
void LastCocoonEgg::fn_80457C48() {
    CharaTask* caught = m1908;
    if (caught != 0 && caught->CanAct_unk() && caught->m55C == 6 && caught->m560 == 17) {
        if (m14E8 > 90) {
            m14E8 = 0;
            m14E4 = 3;
            m14EC.m34C = 1;
            m14EC.mTracks[0].m20 = 1.0f;
            m14EC.mTracks[0].m0C = 1.0f;
            m14EC.fn_80097C08(0, 0x141, 0, 1, 1, 0.0f, 0.1f);
        }
        return;
    }
    if (caught != 0) {
        if (caught->CanAct_unk()) {
            atn::Vector3 dir(caught->mPos.x - mPos.x, 0.0f, caught->mPos.z - mPos.z);
            dir.Normalize_unk();
            dir *= 8.0f;
            m1908->SetM6B8_unk(dir);
            m1908->fn_8015AC48(-1);
        } else {
            caught->mF1C = 0;
        }
        m1908 = 0;
    }
    m14E8 = 0;
    m14E4 = 0;
}

extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

void LastCocoonEgg::fn_8045817C() {
    void* group = m2A40.Load("LastCocoonEgg");
    fn_80087994(group, "DebugDisp", &m2A44, 0, 0);
    fn_80087994(group, "GrowScaling", &m2A4C, 0, 0);
    fn_8008771C(group, "ThrowPower", &m2A48, 0, 0, 1.0f, 10000.0f, 10.0f);
}

// inferred: base color of the glow of fn_8045531C (static object, initialized in __sinit)
static atn::Vector4_unk sGlowColor_unk(0.8f, 0.8f, 0.8f, 1.0f);
