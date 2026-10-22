#include "game/Em308ParmCrab.h"
#include "game/HitInfo_unk.h"

// inferred: effect/bullet manager (lbl_8087F048); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound
extern "C" void fn_80097CCC(Model_unk* model, int track, float value); // inferred: stops a track
extern "C" void fn_800EB7A0(CharaTask* chara); // inferred: BattleCharaTask (death effect?)

Em308ParmCrab::~Em308ParmCrab() {}

float Em308ParmCrab::vf90() {
    return 3.5f;
}

void Em308ParmCrab::fn_802FC188() {
    fn_80239DAC(lbl_8087F3C0, this, 0, 0);
    fn_800CB5C8(&m151C, 0, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    vf48();
    fn_80178A6C();
    fn_8015495C(0, 1, 1, 1);
    fn_8016DA4C();
    m14B0 = 0;
    m15CC = 0;
    m15D4 = m14F4 != 0 ? m14F4->m68 : 90;
    m590 = fn_800F8548(lbl_8087F048);
    fn_800CB5C8(&m151C, 15, 0);
    fn_80239DAC(lbl_8087F3C0, this, 200, 0);
    m58C = 2;
    fn_8016E970(9);
    fn_80097CCC(&mMotion, 0, 0.0f);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
    fn_800EB7A0(this);
}
