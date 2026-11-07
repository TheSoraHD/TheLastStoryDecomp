// game/BattleTask.cpp (0x800F13BC-0x8010D98C): battle manager (BattleTask, HomingBullet). Only the
// effect start/stop functions are reconstructed so far.
#include "game/BattleTask.h"
#include "atn/EffectManager.h"
#include "atn/EffectSystem_unk.h"
#include "math/Vec.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const EffectColor_unk& color, int arg7,
                            int arg8, int arg9, float scale);

// inferred: plays an EFP effect at the model, without offset (as gmk/Coffin.cpp)
inline void PlayEfp_unk(atn::EfpFile* efp, atn::Model* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    EffectColor_unk color;
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

void BattleTask::fn_80105950(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363C0[0], model);
}

void BattleTask::fn_80105A08(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 0);
    PlayEfp_unk(&mEfp363C0[1], model);
}

void BattleTask::fn_80105AB0(void* owner, atn::Model* model, const EffectColor_unk& color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363C0[2], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105B4C(void* owner, atn::Model* model, const EffectColor_unk* color) {
    fn_80232B7C((unsigned int)owner, 0);
    fn_8023A8B4(lbl_8087F3C0, &mEfp363C0[3], model, 0, lbl_807C7030, lbl_807C7030, *color, -1, -1, 1, 1.0f);
}

void BattleTask::fn_80105F3C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 4, 1);
}

void BattleTask::fn_8010607C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 3, 1);
}

void BattleTask::fn_8010652C(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_8010653C(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 6);
    PlayEfp_unk(&mEfp38BC0[12], model);
}

void BattleTask::fn_801065E4(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 6, 1);
}

void BattleTask::fn_80106680(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 8);
    PlayEfp_unk(&mEfp38BC0[14], model);
}

void BattleTask::fn_801067D0(CharaTask* chara, atn::Model* model) {
    fn_80232B7C(0, 0);
    PlayEfp_unk(&mEfp363C0[45], model);
}

void BattleTask::fn_80107798(void* owner, int flag) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 20, flag);
}

void BattleTask::fn_801077A8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 21);
    PlayEfp_unk(&mEfp38BC0[0], model);
}

void BattleTask::fn_80107850(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 21, 1);
}

void BattleTask::fn_80107860(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 22);
    PlayEfp_unk(&mEfp38BC0[2], model);
}

void BattleTask::fn_80107908(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 23);
    PlayEfp_unk(&mEfp38BC0[3], model);
}

void BattleTask::fn_801079B0(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 23, 1);
}

void BattleTask::fn_801079C0(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 24);
    PlayEfp_unk(&mEfp38BC0[5], model);
}

void BattleTask::fn_80107A68(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 24, 1);
}

void BattleTask::fn_80107A78(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 25);
    PlayEfp_unk(&mEfp38BC0[1], model);
}

void BattleTask::fn_80107B20(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 25, 1);
}

void BattleTask::fn_80107B30(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 26);
    PlayEfp_unk(&mEfp38BC0[6], model);
}

void BattleTask::fn_80107BD8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 26, 1);
}

void BattleTask::fn_80107BE8(void* owner, atn::Model* model) {
    lbl_8087F3C0->mSystem.mLayer = 2;
    fn_80232B7C((unsigned int)owner, 27);
    PlayEfp_unk(&mEfp38BC0[7], model);
    lbl_8087F3C0->mSystem.mLayer = 0;
}

void BattleTask::fn_80107CEC(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 28);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38BC0[8], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107D94(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 28, 1);
}

void BattleTask::fn_80107E58(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 29, 1);
}

void BattleTask::fn_80107E68(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 30);
    PlayEfp_unk(&mEfp38BC0[4], model);
}

void BattleTask::fn_80107F10(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 30, 1);
}

void BattleTask::fn_80107F20(void* owner, atn::Model* model, float scale) {
    fn_80232B7C((unsigned int)owner, 31);
    EffectColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    fn_8023A8B4(lbl_8087F3C0, &mEfp38BC0[10], model, 0, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, scale);
}

void BattleTask::fn_80107FC8(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 31, 1);
}

void BattleTask::fn_80107FD8(void* owner, atn::Model* model) {
    fn_80232B7C((unsigned int)owner, 32);
    PlayEfp_unk(&mEfp38BC0[11], model);
}

void BattleTask::fn_80108080(void* owner) {
    fn_80239DAC(lbl_8087F3C0, (unsigned int)owner, 32, 1);
}
