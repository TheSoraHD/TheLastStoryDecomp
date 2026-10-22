#include "game/Em202Spider.h"

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id

Em202Spider::~Em202Spider() {}

void Em202Spider::vf7C(int arg) {
    m14D8 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 11;
    fn_8016E970(6);
    m560 = 0x16;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1DD, 0, 0, 1, 0.0f, 0.05f);
}

void Em202Spider::fn_80281ED8() {
    if (m14B8 > 0) {
        m14D8 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        m58C = 7;
        m1504 = m14B8;
        fn_8017039C(m14B8, 2);
        m1520 = 0;
        m151C = 1;
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 2.0f;
        mMotion.fn_80097C08(0, 0x14, 0, 0, 1, 0.0f, 0.1f);
    }
}
