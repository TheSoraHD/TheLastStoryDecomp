// Camera of the field (GameCamera, GameTask + 0x6C). Only a few members are written; the constructor and
// the update (0x8037F9AC) are still missing, so the unit is not linked.
#include "game/GameCamera.h"
#include "game/CharaManager.h"
#include "game/GameConfig_unk.h"
#include "game/PlayerTask.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" GameConfig_unk* lbl_8087F0A8;

void GameCamera::fn_8037EF30(int a, int b, float f) {
    fn_8037EF48(lbl_8087F8A0->mHead, a, b, f);
}

void GameCamera::fn_8037F690(const atn::Vector3* pos) {
    m840 = *pos;
    m838 = 1;
    m83C = 0;
    m84C = mTarget;
    m85C = mFovy;
    m860 = 0;
}

void GameCamera::fn_8037F6D8(const atn::Vector3* pos) {
    m840 = *pos;
    m858 = 1;
    m838 = 1;
    m83C = 0;
    m84C = mTarget;
    m85C = mFovy;
    m860 = 0;
}

void GameCamera::fn_8037F724(const atn::Vector3* pos) {
    m840 = *pos;
    m860 = 0;
}

void GameCamera::fn_803897EC(const CharaTask* chara) {
    m8F0 = chara->mMotion.mTracks[0].m08 < 67.0f;
    fn_8037F9AC(chara);
    m8F0 = 0;
}

void GameCamera::fn_80389838(int mode) {
    if (m7FC == mode) {
        return;
    }
    m800 = m7FC;
    m7FC = mode;
    m808 = 0;
    m838 = 0;
    m880 = 0;
    mA0C[0] = 0.0f;
    mA0C[1] = 0.0f;
    mA0C[2] = 0.0f;
    mA0C[3] = 0.0f;
    mA0C[4] = 0.0f;
    lbl_8087F0A8->mInput.m10 = 3;
}

void GameCamera::fn_8038A1B8(const CharaTask* chara) {
    m8EC = 1;
    m800 = m7FC;
    m834 = 0;
    m7F4 = 0;
    fn_80382A1C(chara);
}

void GameCamera::fn_8038A1D8() {
    if (m800 == 9) {
        m8EC = 1;
    }
    m800 = m7FC;
    m834 = 0;
}

void GameCamera::fn_80390374() {
    fn_80389838(m804);
}

// NONMATCHING: 43% - the original loads every member before the first store
void GameCamera::fn_80392C94(GameCameraState_unk* state) {
    state->m0C = m80C;
    state->mMode = m7FC;
    state->mPrevMode = m800;
    state->m08 = m808;
    state->m18 = m818;
    state->m24 = m830;
}

void GameCamera::fn_80392CE0(const GameCameraState_unk* state) {
    m80C = state->m0C;
    m7FC = state->mMode;
    m800 = state->mPrevMode;
    m808 = state->m08;
    m818 = state->m18;
    m830 = state->m24;
}

void GameCamera::fn_803933A4() {
    m834 = 0;
    m910 = 0;
    m914 = 0;
    m858 = 0;
    m838 = 0;
    m46C = 0;
    m900 = m900 % 2;
    m904 = 0;
    m908 = 0.0f;
    m90C = 0.0f;
    mC20 = 0;
    mC84 = 0;
}
