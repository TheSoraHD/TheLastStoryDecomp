#include "game/LastCocoonEgg.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"

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
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" void* lbl_8087F8A8;
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)               // inferred: popup text manager
extern "C" void fn_8054D794(void* obj, int arg1, const wchar_t* text, const atn::Vector3* pos, int arg4, int arg5,
                            int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12);
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
extern "C" void fn_80370AE4(EggField_unk* field, int index, int value); // inferred: sets an event flag

LastCocoonEgg::~LastCocoonEgg() {}

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
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
// inferred: the hit is guarded (no damage, reaction 1)
static inline void Guard_unk(HitMsg_unk* msg) {
    msg->m68 = 0;
    msg->m64 = 1;
    msg->m90 = 1;
    msg->m94 |= 8;
}

// NONMATCHING: 97% - stack slots and store order of the zero vectors and the color of the two effects
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
        PlayEfp_unk(&m1940, &mMotion);
        m2558 = 3;
        m1980 = 1;
        m255C = 0.3f;
        m14D0 = 4.0f;
        m14D4 = 0.05f;
        m2548.Shake_unk(0x58, 2.0f, 2.0f);
        lbl_8087F430->Shake_unk(60, 1.5f, 1.5f);
        atn::Vector3 pos = fn_80148B0C(0)->m04.mCenter;
        pos += atn::Vector3(0.0f, 15.0f, 0.0f);
        fn_8054D794(lbl_8087F8A8, 0, L"Shield Break!!", &pos, 0, 0, 1, 0, 0, 0, 1, -1, -1);
    } else {
        fn_80232B7C(0, 0);
        PlayEfp_unk(&m1934, &mMotion);
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

// NONMATCHING: 98% - the original stores the second zero vector first (as in vf40)
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
            fn_80370AE4(lbl_8087F430, 0x5A, 1);
        }
    }
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
