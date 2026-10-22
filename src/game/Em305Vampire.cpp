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
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects
extern "C" void fn_80108F38(Effect_unk* mgr, const atn::Vector3* pos, int arg);

// inferred: plays an effect on the model (same as in Em305VampireMp.cpp)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
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

extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound

// inferred: multiplayer session (lbl_8087F610), as in Em305VampireMp.cpp
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DA48C(MpSession_unk* session, int arg);
extern "C" void fn_804DA4A0(MpSession_unk* session, int arg);

Em305Vampire::~Em305Vampire() {
    fn_800CB5C8(&m15A0, 5, 0);
    fn_800CB5C8(&m15A4, 5, 0);
}

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

void Em305Vampire::fn_802EBF40() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 13;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 0.2f;
    mMotion.fn_80097C08(0, 0x14D, 0, 0, 1, 0.0f, 0.1f);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    vf9C(0);
    fn_80232B7C(0, 0);
    PlayEffect_unk(&m1588, &mMotion);
    if (lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 4);
        fn_804DA4A0(lbl_8087F610, 0);
    }
}

void Em305Vampire::fn_802EC098() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 14;
    m152C = 0.0f;
    m1530 = 0;
    m1534 = 0;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x66, 0, 0, 1, 0.0f, 0.1f);
    vf98();
    if (lbl_8087F610 != 0) {
        fn_804DA48C(lbl_8087F610, 5);
        fn_804DA4A0(lbl_8087F610, 1);
    }
}

void Em305Vampire::fn_802EC160() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 15;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 0.0f;
    mMotion.fn_80097C08(0, 2, 1, 1, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 100);
    PlayEffect_unk(&m1594, &mMotion);
    Effect_unk* mgr = lbl_8087F048;
    atn::Vector3 pos = fn_801781B0();
    fn_80108F38(mgr, &pos, 0x80);
}

void Em305Vampire::fn_802EC280() {
    m14F8 = 0;
    m14FC = 0;
    m590 = fn_800F8548(lbl_8087F048);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m58C = 2;
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_80239DAC(lbl_8087F3C0, this, 100, 1);
    fn_80239DAC(lbl_8087F3C0, this, 101, 1);
    fn_800CB5C8(&m15A0, 5, 0);
    fn_800CB5C8(&m15A4, 5, 0);
    fn_800EB7A0(this);
}

void Em305Vampire::vf108() {}
void Em305Vampire::vf104() {}
void Em305Vampire::vf100(int state) {}
void Em305Vampire::vfFC() {}
