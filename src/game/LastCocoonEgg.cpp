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
#include "atn/Material_unk.h"
#include "macros.h"
#include "game/BattleTask.h"

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
// inferred: named Field_unk as in game/LastCocoonImago.cpp (fn_8013C504__9Field_unkFv)
struct Field_unk {
    unsigned char m00[0x96C];
    int m96C;
    int m970;   // inferred: frames
    float m974; // inferred: x amplitude
    float m978; // inferred: y amplitude
    unsigned char m97C[0x10D8 - 0x97C];
    RouteTable_unk* m10D8; // inferred: route table of the map
    RouteTable_unk* fn_8013C504(); // inferred: route table (game/LastCocoonQuadruped)
    // inferred: starts a camera shake (name invented)
    void Shake_unk(int frame, float x, float y) {
        m96C = m96C % 2;
        m970 = frame;
        m974 = x;
        m978 = y;
    }
};
extern "C" Field_unk* lbl_8087F430;

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

extern "C" Field_unk* fn_80121F00();
// inferred: weak copies of other files (as in game/TacView.cpp and game/EnemyQuarkSecond.cpp)
extern "C" inline void fn_80339F74(BattleCharaTask* chara, int value) {
    chara->m1454 = value;
}
extern "C" inline CharaManager<PlayerTask>* fn_8000D9E8() {
    return lbl_8087F8A0;
}
extern "C" inline CharaTask* fn_802A36B0(CharaManager<PlayerTask>* mgr) {
    return mgr->mHead;
}
extern "C" inline CharaTask* fn_802A4094(CharaTask* chara) {
    return ((PlayerTask*)chara)->mNext;
}
extern "C" RenderSetting_unk* fn_802F0990();                // inferred: returns lbl_8087EFA8
extern "C" atn::Color fn_800F8014(unsigned int rgba);       // inferred: colour of a 32-bit value (game/BattleTask)
// inferred: as in game/GameTask.cpp
inline ColorMatrixParam_unk* RenderSetting_unk::GetColorMatrix_unk() {
    return &m324;
}
// inferred: bloom settings of the render settings (weak copy 0x80453DB4; name of the original unknown)
extern "C" inline BloomParam_unk* fn_80453DB4(RenderSetting_unk* rs) {
    return &rs->m054;
}
// NONMATCHING: the original is over the inlining budget (every inline call is a weak copy: fn_80453C88,
// fn_80453DB4, fn_80453DBC, push_back fn_80453E20, Vector4_unk, fn_8045817C); ours inlines them all. Adding
// candidates (a helper per post effect) tips it over, but then the helper is called too.
int LastCocoonEgg::vf0C() {
    if (fn_8013655C() == 0 && m14B0.fn_80473F50() == 0 && m14EC.fn_8008B140() == 0 && m1928.IsLoading() == 0 &&
        m1934.IsLoading() == 0 && m1940.IsLoading() == 0 && m1950.IsLoading() == 0 && m195C.IsLoading() == 0 &&
        m1988.IsLoading() == 0 && m1994.IsLoading() == 0 && m252C.IsLoading() == 0 && m2538.IsLoading() == 0) {
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x10000;
        mStatus.m18 |= 0x400015;
        mStatus.m18 |= 0x388C208;
        fn_8045817C();
        if (m14B0.fn_80470580() != 0 && fn_80121F00() != 0 && fn_80121F00()->fn_8013C504() != 0) {
            atn::Csv csv((const char*)m14B0.fn_80470580(), m14B0.fn_8047059C());
            do {
                csv.fn_8005B9CC();
            } while (csv.fn_8005BBF8());
        }
        m1904 = m14EC.GetNodeMtx_unk("catch_point");
        mStatus.m04 = 1.0f;
        fn_80339F74(this, 0);
        LastCocoonEggPart_unk part;
        part.m04 = 0.0f;
        part.m08 = 0;
        part.m0C = 0;
        for (CharaTask* p = fn_802A36B0(fn_8000D9E8()); p != 0; p = fn_802A4094(p)) {
            part.m00 = p;
            part.m10 = 0;
            switch (fn_80219558(p->GetCharaNo_unk())) {
            case 0:
                part.m14 = 0xB4;
                break;
            case 3:
                part.m14 = 0xB5;
                break;
            case 4:
                part.m14 = 0xB6;
                break;
            case 5:
                part.m14 = 0xB8;
                break;
            case 6:
                part.m14 = 0xB7;
                break;
            default:
                part.m14 = 0xB4;
                break;
            }
            m19A0.push_back(part);
        }
        m2560.mBloom = *fn_80453DB4(fn_802F0990());
        m2560.mColorMatrix = *fn_802F0990()->GetColorMatrix_unk();
        m2630.mBloom = m2560.mBloom;
        m2630.mColorMatrix = m2560.mColorMatrix;
        for (int i = 0; i < 4; i++) {
            m2700[i].mBloom = m2560.mBloom;
            m2700[i].mColorMatrix = m2560.mColorMatrix;
        }
        for (int j = 0; j < 3; j++) {
            m2700[0].mBloom.mScale[j] = 2;
        }
        m2700[0].mBloom.mColor[0] = fn_800F8014(0xFF234794);
        m2700[0].mColorMatrix.mRows[0] = atn::Vector4_unk(0.9f, 0.1f, 0.1f, 0.0f);
        m2700[0].mColorMatrix.mRows[1] = atn::Vector4_unk(0.1f, 0.9f, 0.1f, 0.0f);
        m2700[0].mColorMatrix.mRows[2] = atn::Vector4_unk(0.1f, 0.1f, 1.0f, 0.0f);
        for (int j = 0; j < 3; j++) {
            m2700[1].mBloom.mScale[j] = 2;
        }
        m2700[1].mBloom.mColor[0] = fn_800F8014(0xFF800000);
        m2700[1].mColorMatrix.mRows[0] = atn::Vector4_unk(2.0f, 0.1f, 0.1f, 0.0f);
        m2700[1].mColorMatrix.mRows[1] = atn::Vector4_unk(0.1f, 0.7f, 0.1f, 0.0f);
        m2700[1].mColorMatrix.mRows[2] = atn::Vector4_unk(0.1f, 0.1f, 0.7f, 0.0f);
        for (int j = 0; j < 3; j++) {
            m2700[2].mBloom.mScale[j] = 2;
        }
        m2700[2].mBloom.mColor[0] = fn_800F8014(0xFFFFFFFF);
        m2700[2].mColorMatrix.mRows[0] = atn::Vector4_unk(0.9f, 0.1f, 0.1f, 0.0f);
        m2700[2].mColorMatrix.mRows[1] = atn::Vector4_unk(0.1f, 0.9f, 0.1f, 0.0f);
        m2700[2].mColorMatrix.mRows[2] = atn::Vector4_unk(0.1f, 0.1f, 1.0f, 0.0f);
        for (int j = 0; j < 3; j++) {
            m2700[3].mBloom.mScale[j] = 2;
        }
        m2700[3].mBloom.mColor[0] = fn_800F8014(0xFF004000);
        m2700[3].mColorMatrix.mRows[0] = atn::Vector4_unk(0.9f, 0.1f, 0.1f, 0.0f);
        m2700[3].mColorMatrix.mRows[1] = atn::Vector4_unk(0.1f, 0.9f, 0.1f, 0.0f);
        m2700[3].mColorMatrix.mRows[2] = atn::Vector4_unk(0.1f, 0.1f, 1.0f, 0.0f);
        return 1;
    }
    return 0;
}

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

extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings
// inferred: colour and colour matrix row arithmetic of the post effect blend of vf14 (names invented)
inline atn::Color operator-(const atn::Color& a, const atn::Color& b) {
    return atn::Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
}
inline atn::Color operator*(const atn::Color& a, float s) {
    return atn::Color(a.r * s, a.g * s, a.b * s, a.a * s);
}
inline atn::Color operator+(const atn::Color& a, const atn::Color& b) {
    return atn::Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
}
inline atn::Vector4_unk EggSub4_unk(const atn::Vector4_unk& a, const atn::Vector4_unk& b) {
    return atn::Vector4_unk(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}
inline atn::Vector4_unk EggMul4_unk(const atn::Vector4_unk& a, float s) {
    return atn::Vector4_unk(a.x * s, a.y * s, a.z * s, a.w * s);
}
inline atn::Vector4_unk EggAdd4_unk(const atn::Vector4_unk& a, const atn::Vector4_unk& b) {
    return atn::Vector4_unk(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

// NONMATCHING: 62% - scheduling and stack slots of the colour matrix rows (the bloom part matches)
// inferred: blends the current post effect (m2630) towards the one of the phase m2558 and applies it
void LastCocoonEgg::vf14() {
    fn_80149A30();
    if (mC58.mC0 == 1) {
        float t;
        m2630.mBloom.mEnabled = 1;
        t = m255C;
        m2630.mBloom.mColor[0] = (m2700[m2558].mBloom.mColor[0] - m2630.mBloom.mColor[0]) * t + m2630.mBloom.mColor[0];
        lbl_8087EFA8->m054 = m2630.mBloom;
        m2630.mColorMatrix.mEnabled = 1;
        m2630.mColorMatrix.mRows[0] = EggAdd4_unk(
            EggMul4_unk(EggSub4_unk(m2700[m2558].mColorMatrix.mRows[0], m2630.mColorMatrix.mRows[0]), m255C),
            m2630.mColorMatrix.mRows[0]);
        m2630.mColorMatrix.mRows[1] = EggAdd4_unk(
            EggMul4_unk(EggSub4_unk(m2700[m2558].mColorMatrix.mRows[1], m2630.mColorMatrix.mRows[1]), m255C),
            m2630.mColorMatrix.mRows[1]);
        m2630.mColorMatrix.mRows[2] = EggAdd4_unk(
            EggMul4_unk(EggSub4_unk(m2700[m2558].mColorMatrix.mRows[2], m2630.mColorMatrix.mRows[2]), m255C),
            m2630.mColorMatrix.mRows[2]);
        lbl_8087EFA8->m324 = m2630.mColorMatrix;
    }
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

extern "C" double sin(double x);

// inferred: offset of the shake: random in x/z, alternating sign every frame and fading with the frame
// count (name invented)
inline atn::Vector3 EggShakeOffset_unk(const LastCocoonEggShake_unk& s) {
    if (s.m00 >= s.mFrames) {
        return lbl_807C7030;
    }
    float z = 0.7f * s.mY + 0.3f * s.mY * ((rand() % 1001) / 1000.0f);
    float x = 2.0f * s.mX * ((rand() % 1001) / 1000.0f) - s.mX;
    atn::Vector3 v(x, 0.0f, z);
    if (s.m00 % 2 == 0) {
        v = atn::Vector3(-v.x, -v.y, -v.z);
    }
    v *= (float)s.m00 / (float)s.mFrames;
    return v;
}

// NONMATCHING: 98.7% - registers of the place search and stack slots of the shake offset
// inferred: floats over the place 601 with a sine bob, adds the shake m2548 (alternating sign every frame,
// fading with its frame count) and eases the scale m540 towards m14D0
void LastCocoonEgg::fn_80454F08() {
    if (mC58.mC0 != 0) {
        if (m14CC != 0) {
            m14C8++;
        }
        if (m14C8 >= 240) {
            m14C8 = 0;
        }
        float bob = 3.0f * (float)sin(6.2831855f * (m14C8 / 240.0f));
        RoutePlace_unk* place = lbl_8087F430->m10D8->FindPlace_unk(601);
        if (place != 0) {
            mPos = place->mPos;
        } else {
            mPos = lbl_807C7030;
        }
        mPos.y = 15.0f + bob;
        mPos += EggShakeOffset_unk(m2548);
        if (m2A4C != 0) {
            m540 = (atn::Vector3(m14D0, m14D0, m14D0) - m540) * m14D4 + m540;
        }
    }
    fn_80144710();
    mMotion.m20C_15_8 = mMotion.m50;
    mMotion.m50 = 0;
    mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
}

// inferred: color as four floats (as LeoColor_unk in game/Em401_01_WhiteLeoPard.cpp); name invented
struct EggColor_unk {
    EggColor_unk() {}
    EggColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    EggColor_unk(GXColor c) : r(c.r / 255.0f), g(c.g / 255.0f), b(c.b / 255.0f), a(c.a / 255.0f) {}
    EggColor_unk operator-(const EggColor_unk& o) const { return EggColor_unk(r - o.r, g - o.g, b - o.b, a - o.a); }
    EggColor_unk operator+(const EggColor_unk& o) const { return EggColor_unk(r + o.r, g + o.g, b + o.b, a + o.a); }
    EggColor_unk operator*(float s) const { return EggColor_unk(r * s, g * s, b * s, a * s); }
    float r, g, b, a;
};
inline EggColor_unk operator*(const atn::Vector4_unk& v, float s) {
    return EggColor_unk(v.x * s, v.y * s, v.z * s, v.w * s);
}
// inferred: name invented
inline EggColor_unk EggLerp_unk(const EggColor_unk& a, const EggColor_unk& b, float t) {
    return (b - a) * t + a;
}

// inferred: base color of the glow of fn_8045531C (static object, initialized in __sinit)
static atn::Vector4_unk sGlowColor_unk(0.8f, 0.8f, 0.8f, 1.0f);

// NONMATCHING: 99.5% - offset of "light" in the string pool (the strings of the unwritten functions are
// missing) and the float register of the glow factor
// inferred: color of the material "light": brightened in mode 8, else pulsing every 120 units of m14D8
void LastCocoonEgg::fn_8045531C() {
    Material_unk* material = mMotion.fn_80094B6C("light");
    if (material != 0) {
        EggColor_unk color(0.0f, 0.0f, 0.0f, 1.0f);
        if (m14E0 != 0) {
            if (m58C == 8) {
                EggColor_unk base(material->mColor);
                color = EggLerp_unk(base, EggColor_unk(1.0f, 1.0f, 1.0f, 1.0f), 0.1f);
            } else {
                m14D8 += m14DC;
                if (m14D8 > 120.0f) {
                    m14D8 -= 120.0f;
                }
                float s = (float)sin(6.2831855f * (m14D8 / 120.0f));
                color = sGlowColor_unk * (0.5f * s + 0.5f);
            }
            color.a = 1.0f;
        }
        mMotion.fn_80094958("light", &color, &EggColor_unk(material->m20), &EggColor_unk(material->m1C));
    }
}

// inferred: damage record shown by fn_80108C10 (as DaemonDamage_unk of game/Em203GreaterDaemon.cpp)
struct EggDamage_unk {
    EggDamage_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
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
extern "C" void fn_80108C10(Effect_unk* effect, EggDamage_unk* damage, const atn::Vector3& pos, CharaTask* attacker,
                            CharaTask* chara, int b, int c);
// inferred: plays an effect bound to a matrix (kind 5; as in game/EnemyQuarkSecond.cpp)
extern "C" void fn_8023A680(EffectMgr_unk* mgr, atn::EffFile* res, int arg2, int kind, const void* target,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
// inferred: party member that can be targeted (as IsBurstTarget_unk of game/EnemyQuarkSecond.cpp)
inline bool EggIsTarget_unk(CharaTask* c) {
    bool active;
    bool free;
    bool r;
    r = false;
    active = false;
    free = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        free = true;
    }
    if (free && (c->mStatus.m0C & 0x20) != 0x20) {
        active = true;
    }
    if (active) {
        bool grappled = false;
        if (c->m55C == 6 && c->m560 == 28) {
            grappled = true;
        }
        if (!grappled) {
            r = true;
        }
    }
    return r;
}
// inferred: first free orb (name invented)
inline LastCocoonEggSeg_unk* LastCocoonEgg_FreeSeg_unk(LastCocoonEgg* egg) {
    for (int i = 0; i < 32; i++) {
        if (egg->m19AC[i].m00 == 0) {
            return &egg->m19AC[i];
        }
    }
    return 0;
}
// inferred: position of a node (name invented)
inline atn::Vector3 EggNodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

// inferred: life drain: every 40 frames that an attacker keeps bullets on it, the egg drains 50 hit points per
// bullet (leaving at least one) into an orb that flies from the attacker's spine back to the egg and heals it
// on arrival; also tells the game task (event m14) the first time an attacker gets the state 0x20000
// NONMATCHING: 98.7% - registers (the constants of the loops, the flags of the target test) and stack slots of the
// position temporaries; the offset of "spine" in @stringBase0 (strings of fn_804562E4 not written); 3 variants
void LastCocoonEgg::fn_80455638() {
    for (unsigned int i = 0; i < m19A0.size(); i++) {
        LastCocoonEggPart_unk& part = m19A0[i];
        int state = (part.m00->mStatus.m0C >> 17) & 1;
        if (part.m10 == 0 && state != 0) {
            ((GameTask*)lbl_8087F430)->fn_80370AE4(part.m14, 1);
        }
        part.m10 = state;
        part.m04 = 0.0f;
        if (EggIsTarget_unk(part.m00)) {
            part.m04 = (float)((BattleTask*)lbl_8087F048)->fn_80101340(part.m00);
        }
        if (part.m04 <= 0.0f) {
            part.m08 = 0;
        } else if (++part.m08 % 40 == 0) {
            EggDamage_unk damage;
            LastCocoonEggSeg_unk* seg = LastCocoonEgg_FreeSeg_unk(this);
            if (seg != 0) {
                float amount = MIN(50.0f * part.m04, part.m00->mStatus.m04 - 1.0f);
                damage.m00 = 1;
                damage.m04 = (int)amount;
                part.m00->mStatus.fn_8012DF7C(damage.m04, 0, 0);
                fn_80108C10(lbl_8087F048, &damage, part.m00->mPos + atn::Vector3(0.0f, 15.0f, 0.0f), this,
                            part.m00, 0, 0);
                PlayEfp_unk(&m1988, &part.m00->mMotion);
                seg->m14 = EggNodePos_unk(part.m00->mMotion.GetNodeMtx_unk("spine"));
                seg->m20 = mPos;
                seg->m20.y += 10.0f * m14D0;
                seg->m04 = seg->m14;
                seg->m10 = amount;
                seg->m2C.SetIdentity();
                seg->m2C.fn_801479E4(seg->m04);
                seg->m00 = 1;
                fn_80232B7C(seg, 0);
                fn_8023A680(lbl_8087F3C0, &m1994, -1, 5, &seg->m2C, 0, 0, 0, 0, -1, 1, 1.0f);
                fn_800C344C("SE_EM206_024", &mPos, 0, -1, 1.0f);
            }
        }
    }
    for (int i = 0; i < 32; i++) {
        LastCocoonEggSeg_unk* seg = &m19AC[i];
        if (seg->m00 != 0) {
            atn::Vector3 dir(seg->m20.x - seg->m14.x, 0.0f, seg->m20.z - seg->m14.z);
            PSVECNormalize(&dir, &dir);
            atn::Vector3 d(seg->m20.x - seg->m04.x, 0.0f, seg->m20.z - seg->m04.z);
            if (PSVECSquareMag(&d) < 2.25f) {
                dir *= PSVECMag(&d);
            } else {
                dir *= 1.5f;
            }
            seg->m04 += dir;
            seg->m04.y = (seg->m20.y - seg->m04.y) * 0.2f + seg->m04.y;
            seg->m2C.SetIdentity();
            seg->m2C.fn_801479E4(seg->m04);
            atn::Vector3 diff(seg->m04.x - seg->m20.x, seg->m04.y - seg->m20.y, seg->m04.z - seg->m20.z);
            if (PSVECSquareMag(&diff) < 1.0f) {
                fn_80239DAC(lbl_8087F3C0, seg, 0, 0);
                EggDamage_unk damage;
                damage.m00 = 1;
                damage.m04 = (int)-seg->m10;
                atn::Vector3 pos = fn_80148B0C(0)->m04.mCenter;
                pos += atn::Vector3(0.0f, 15.0f, 0.0f);
                mStatus.fn_8012DF7C(damage.m04, 0, 0);
                fn_80108C10(lbl_8087F048, &damage, pos, this, this, 0, 0);
                seg->m00 = 0;
            }
        }
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

extern "C" int rand();
extern "C" int fn_80053BD0(atn::ColliResult_unk* r, const atn::ColliCapsuleShape_unk* c,
                           const atn::ColliCapsuleShape_unk* capsule); // inferred: segment against capsule
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, void* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/EnemyQuarkSecond.cpp)
// inferred: segment of a shot (a capsule shape without radius; as Segment_unk of chr/ControlPushedDown.cpp)
struct EggSegment_unk {
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};
// inferred: fires the bullet 0x5AB from 10 above the egg along `dir`; it starts where the 30-long segment first
// meets the egg's own capsule (name invented)
inline void EggShoot_unk(LastCocoonEgg* egg, atn::Vector3 dir) {
    atn::Vector3 from = egg->mPos + atn::Vector3(0.0f, 10.0f, 0.0f);
    EggSegment_unk seg;
    seg.mStart = from + dir * 30.0f;
    seg.mEnd = from;
    atn::ColliResult_unk result;
    if (fn_80053BD0(&result, (const atn::ColliCapsuleShape_unk*)&seg,
                    (const atn::ColliCapsuleShape_unk*)&egg->mColli.m3C) != 0) {
        from = result.m04;
    }
    Effect_unk* mgr = lbl_8087F048;
    fn_800F8574(mgr, egg, fn_80219E6C(0x5AB), &from, &dir, 0, 0x24, 0, 0.0f, 1.0f);
}
// inferred: rotation matrix z, y, x with the angle compared on the right (as in game/EnemyQuarkSecond.cpp)
inline void EggRotate2_unk(Mtx m, char axis, float rad) {
    if (0.0f != rad) {
        Mtx r;
        PSMTXRotRad(r, axis, rad);
        Mtx t;
        PSMTXConcat(m, r, t);
        __MtxCopy(m, t);
    }
}

// inferred: mode 7: charges for 60 frames (effect m252C), then for 45 frames (effect m2538, sound) and shoots: one
// bullet at each targetable party member ("hips") and six fans of random bullets (count of the skill 0x5AC) around
// the egg; back to mode 0
// NONMATCHING: 87.2% - registers of the loop constants and stack slots of the shot locals (the original keeps
// the segment, the result and the start in other slots and reloads the start through a copy); 4 variants
void LastCocoonEgg::fn_804562E4() {
    if (m14BC == 0) {
        if (m14C0 < 60) {
            m14DC = 9.0f * ((float)m14C0 / 60.0f) + 1.0f;
        } else {
            m14C0 = 0;
            m14BC = 1;
            m14CC = 0;
            fn_80232B7C(this, 0x3E9);
            PlayEfp_unk(&m252C, &mMotion);
        }
    } else if (m14BC == 1) {
        if (m14C0 < 45) {
            m14DC = 1.0f;
        } else {
            fn_80239DAC(lbl_8087F3C0, this, 0x3E9, 0);
            PlayEfp_unk(&m2538, &mMotion);
            fn_800C344C("SE_EM206_023", &mPos, 0, -1, 1.0f);
            m12CC = 1.0f;
            for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = (PlayerTask*)p->mNext) {
                if (EggIsTarget_unk(p)) {
                    atn::Vector3 dir = EggNodePos_unk(p->mMotion.GetNodeMtx_unk("hips")) -
                                       (mPos + atn::Vector3(0.0f, 10.0f, 0.0f));
                    PSVECNormalize(&dir, &dir);
                    EggShoot_unk(this, dir);
                }
            }
            for (int i = 0; i < 6; i++) {
                float angle = 6.2831855f * (float)i / 6.0f;
                atn::Vector3 front(0.0f, 0.0f, 1.0f);
                Mtx r;
                PSMTXRotRad(r, 'y', mRot.y);
                PSMTXMultVec(r, &front, &front);
                Mtx r2;
                PSMTXRotRad(r2, 'y', angle);
                PSMTXMultVec(r2, &front, &front);
                atn::Vector3 rot = front.ToRotation_unk();
                atn::Matrix3x4 m;
                m.SetIdentity();
                EggRotate2_unk(m.m, 'z', rot.z);
                EggRotate2_unk(m.m, 'y', rot.y);
                EggRotate2_unk(m.m, 'x', rot.x);
                HitInfo_unk* data = fn_80219E6C(0x5AC);
                for (int j = 0; j < data->m48; j++) {
                    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                    float spread = 0.017453292f * mStatus.mParamB.m2C;
                    Mtx ry;
                    PSMTXRotRad(ry, 'y', spread * ((float)(rand() % 1001) / 1000.0f));
                    PSMTXMultVec(ry, &dir, &dir);
                    Mtx rz;
                    PSMTXRotRad(rz, 'z', 6.2831855f * ((float)(rand() % 1001) / 1000.0f));
                    PSMTXMultVec(rz, &dir, &dir);
                    PSMTXMultVec(m.m, &dir, &dir);
                    EggShoot_unk(this, dir);
                }
            }
            m12CC = 0.0f;
            m14BC = 0;
            m14C0 = 0;
            m590 = fn_800F8548(lbl_8087F048);
            m58C = 0;
            fn_8016E970(3);
            m2558 = 0;
            m14CC = 1;
        }
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

extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
// inferred: *mtx = *mtx * rot(axis, rad) when the angle is not zero (name invented)
static inline void EggRotate_unk(atn::Matrix3x4& mtx, char axis, float rad) {
    if (0.0f != rad) {
        atn::Matrix3x4::RotateAxis_unk(mtx, axis, rad);
    }
}

// NONMATCHING: 91% - not iterated
// inferred: catches `target`: plays the motion 0x13F from the frame 21 and places the character 30 units in
// front of the egg, facing it (the debug action 1 then holds it)
void LastCocoonEgg::fn_804570F4(CharaTask* target) {
    if (target != 0) {
        m14E8 = 0;
        m14E4 = 1;
        m14EC.m34C = 1;
        m14EC.mTracks[0].m20 = 1.0f;
        m14EC.mTracks[0].m0C = 0.7f;
        m14EC.fn_80097C08(0, 0x13F, 0, 1, 1, 0.0f, 0.1f);
        m14EC.mTracks[0].m08 = 21.0f;
        m1908 = target;
        if (target != 0 && m1904 != 0) {
            atn::Vector3 dir(target->mPos.x - mPos.x, 0.0f, target->mPos.z - mPos.z);
            dir.Normalize_unk();
            m18EC = m1908->mPos - dir * 30.0f;
            m18F8 = dir.ToRotation_unk();
            atn::Matrix3x4 trans;
            PSMTXTrans(trans.m, m18EC.x, m18EC.y, m18EC.z);
            m18BC = trans;
            atn::Matrix3x4 rot;
            rot.SetIdentity();
            EggRotate_unk(rot, 'y', m18F8.y);
            EggRotate_unk(rot, 'x', m18F8.x);
            EggRotate_unk(rot, 'z', m18F8.z);
            atn::Matrix3x4 t;
            PSMTXConcat(m18BC.m, rot.m, t.m);
            m18BC = t;
            m1914 = m1908->mPos;
            m1914 += dir * m14D0 * 2.0f;
            m1914.y += 30.0f;
            m1908->fn_801595BC((int)m1904, 10, 0x1D8, -1);
        }
    }
}

// NONMATCHING: 93% - registers of the inlined CharaTask::CanAct_unk (as fn_80457C48)
// inferred: debug action 1: holds the caught character at the node of the egg (m18BC follows m1914 and
// the angles m18F8); at the end of the motion 0x13F/0x140 it moves to the action 2
void LastCocoonEgg::fn_804576D4() {
    CharaTask* caught = m1908;
    if (caught == 0 || !caught->CanAct_unk() || caught->m55C != 6 || caught->m560 != 17) {
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
    } else {
        float frame = m14EC.mTracks[0].m08;
        if (frame >= m14EC.fn_80097D7C(0)) {
            m14E8 = 0;
            m14E4 = 2;
            m14EC.m34C = 1;
            m14EC.mTracks[0].m20 = 1.0f;
            m14EC.mTracks[0].m0C = 0.7f;
            m14EC.fn_80097C08(0, 0x140, 1, 1, 1, 0.0f, 0.1f);
            if (m1908 != 0) {
                m1908->mMotion.fn_80097C08(0, 0x1D8, 1, 0, 1, 0.0f, 0.1f);
            }
        } else {
            m18EC = (m1914 - m18EC) * 0.15f + m18EC;
            m18BC.SetIdentity();
            atn::Matrix3x4 trans;
            PSMTXTrans(trans.m, m18EC.x, m18EC.y, m18EC.z);
            m18BC = trans;
            atn::Matrix3x4 rot;
            rot.SetIdentity();
            EggRotate_unk(rot, 'y', m18F8.y);
            EggRotate_unk(rot, 'x', m18F8.x);
            EggRotate_unk(rot, 'z', m18F8.z);
            atn::Matrix3x4 t;
            PSMTXConcat(m18BC.m, rot.m, t.m);
            m18BC = t;
        }
    }
}

// NONMATCHING: 98% - registers of the inlined CharaTask::CanAct_unk (the character in r8 in the original)
// inferred: debug action 2: throws the caught character (or plays the motion 0x141 while it is held)
void LastCocoonEgg::fn_80457C48() {
    CharaTask* caught = m1908;
    if (caught == 0 || !caught->CanAct_unk() || caught->m55C != 6 || caught->m560 != 17) {
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
    } else {
        if (m14E8 > 90) {
            m14E8 = 0;
            m14E4 = 3;
            m14EC.m34C = 1;
            m14EC.mTracks[0].m20 = 1.0f;
            m14EC.mTracks[0].m0C = 1.0f;
            m14EC.fn_80097C08(0, 0x141, 0, 1, 1, 0.0f, 0.1f);
        }
    }
}

// NONMATCHING: 93% - registers of the inlined CharaTask::CanAct_unk (as fn_80457C48)
// inferred: debug action 3: throws the caught character forward at the frame 24 of the motion 0x141
void LastCocoonEgg::fn_80457E98() {
    CharaTask* caught = m1908;
    if (caught == 0 || !caught->CanAct_unk() || caught->m55C != 6 || caught->m560 != 17) {
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
    } else {
        float frame = m14EC.mTracks[0].m08;
        if (frame >= m14EC.fn_80097D7C(0)) {
            m14E8 = 0;
            m14E4 = 0;
        }
        if ((float)__fabs(m14EC.mTracks[0].m08 - 24.0f) < 1.1920929e-7f && m1908 != 0) {
            float power = m2A48;
            atn::Vector3 dir;
            atn::Vector3 v(0.0f, 0.0f, 1.0f);
            atn::Matrix3x4 rot;
            rot.fn_8013A13C(mRot.y);
            PSMTXMultVec(rot.m, &v, &v);
            dir = v * power;
            m1908->fn_8015AC48(-1);
            m1908->fn_8015E7A0(dir, 0x39, 0);
            m1908 = 0;
        }
    }
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

