#include "game/Em305Vampire.h"
#include "math/Vec.h"

extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect

// NONMATCHING: ~95% - only the offset of "op_02_cloud" in the string pool (the pool is in the auto split
// after the unit and starts with the strings of functions not written yet)
void Em305Vampire::vf14() {
    if (m1528 != 0) {
        mMotion.m40 = m15C4;
        mMotion.fn_8009373C("op_02_cloud", 0);
    } else {
        Model_unk::Float4_unk white;
        white.m00 = 1.0f;
        white.m04 = 1.0f;
        white.m08 = 1.0f;
        white.m0C = 1.0f;
        mMotion.m40 = white;
        mMotion.fn_8009373C("op_02_cloud", 1);
    }
    fn_80149A30();
}

void Em305Vampire::fn_802EABE4() {
    if (m55C - 6U > 1) {
        fn_8016E970(3);
        if (m1500 == 1) {
            fn_8017039C(*(int*)m14E8[m1560], 0);
        } else {
            fn_80170A20(m14F4, 0, -1.0f);
        }
    }
    fn_802EA490();
}

void Em305Vampire::fn_802EB97C() {
    if (m14FC >= m15B8) {
        fn_80239DAC(lbl_8087F3C0, this, 100, 0);
        fn_802EBABC();
    }
}

void Em305Vampire::vfEC() {
    float frame = mMotion.mTracks[0].m08;
    if (frame >= mMotion.fn_80097D7C(0)) {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        vf9C(1);
        if (m1434 > 0) {
            m1434--;
        } else {
            mFlag15 = 1;
            fn_801765D8();
            fn_800EE360(this);
        }
    } else {
        m1434 = 0;
    }
}

void Em305Vampire::fn_802EBABC() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 0;
    fn_8016E970(3);
}

void Em305Vampire::fn_802EBB14() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 6;
    fn_8016E970(6);
    m560 = 4;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x145, 0, 1, 1, 0.0f, 0.1f);
}

void Em305Vampire::vf108() {}
void Em305Vampire::vf104() {}
void Em305Vampire::vf100(int state) {}
void Em305Vampire::vfFC() {}
