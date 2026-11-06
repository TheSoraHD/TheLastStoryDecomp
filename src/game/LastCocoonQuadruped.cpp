#include "game/LastCocoonQuadruped.h"

struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_80107A68(Effect_unk* effect, chr::Motion_unk* motion); // inferred: as in game/CharaTask.cpp
// inferred: field (lbl_8087F430): camera shake and event flags (as in gmk/ShakeCamera.cpp)
struct QuadField_unk {
    void Shake_unk(int frame, float x, float y) {
        m96C = m96C % 2;
        m970 = frame;
        m974 = x;
        m978 = y;
    }
    unsigned char m00[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
};
extern "C" QuadField_unk* lbl_8087F430;
extern "C" void fn_80370AE4(QuadField_unk* field, int index, int value); // inferred: sets an event flag

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
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
// inferred: plays an effect on the model with the default color and scale (as in game/LastCocoonEgg.cpp)
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

inline void LastCocoonQuadruped::SetMode_unk(int mode) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    if (lbl_8087F430 != 0) {
        fn_80370AE4(lbl_8087F430, 0x8C, 0);
    }
    m58C = mode;
    fn_8016E970(3);
}

void LastCocoonQuadruped::vf14() {
    fn_80149A30();
}

void LastCocoonQuadruped::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m14C4 != 7 && m58C != 8) {
        if (m14C4 == 6 || m14C4 == 1 || (m58C >= 17 && m58C <= 19)) {
            msg->m10 *= 0.0f;
        }
        fn_80151448(msg);
        vf40(hit);
    }
}

atn::Vector3 LastCocoonQuadruped::vf64(int center) {
    atn::Vector3 result = mPos;
    result = m624.mData[0].m04.mCenter;
    return result;
}

void LastCocoonQuadruped::fn_804654F0() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        fn_80466974();
    }
}

void LastCocoonQuadruped::fn_80466868() {
    if ((float)__fabs(mMotion.mTracks[0].m08 - 3.0f) < 1.1920929e-7f) {
        lbl_8087F430->Shake_unk(60, 0.3f, 0.3f);
    }
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        SetMode_unk(0);
        if (m1800 != 0) {
            fn_80107A68(lbl_8087F048, &mMotion);
            m1800 = 0;
        }
    }
}

void LastCocoonQuadruped::fn_80466974() {
    SetMode_unk(0);
    if (m1800 != 0) {
        fn_80107A68(lbl_8087F048, &mMotion);
        m1800 = 0;
    }
}

void LastCocoonQuadruped::fn_80466AE4() {
    SetMode_unk(7);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 2.0f;
    mMotion.fn_80097C08(0, 0x156, 0, 0, 1, 0.0f, 0.1f);
}

// NONMATCHING: 98% - the original stores the second zero vector first (as LastCocoonEgg fn_80455D80)
void LastCocoonQuadruped::fn_80466B8C() {
    SetMode_unk(8);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x13F, 0, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 0x65);
    PlayEfp_unk(&m1790, &mMotion);
}

int LastCocoonQuadruped::vfE4() {
    int ret = 0;
    if (m58C == 16 && m14BC == 1) {
        ret = 1;
    }
    return ret;
}

float LastCocoonQuadruped::vf90() {
    return 3.0f;
}
