// Camera of the field (GameCamera, GameTask + 0x6C). Only a few members are written; the constructor and
// the update (0x8037F9AC) are still missing, so the unit is not linked.
#include "game/GameCamera.h"
#include "game/CharaManager.h"
#include "game/GameConfig_unk.h"
#include "game/PlayerTask.h"
#include "atn/RenderSetting_unk.h"
#include "atn/RenderManager.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" RenderSetting_unk* lbl_8087EFA8;

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

// inferred: member function of a camera mode
typedef void (GameCamera::*GameCameraMode_unk)(const CharaTask* chara);

// inferred: first frame of a mode (the mode changed) (21 modes; name invented)
static const GameCameraMode_unk sModeStart_unk[21] = {
    &GameCamera::fn_80389A00, &GameCamera::fn_8038A200, &GameCamera::fn_8038A5F0,
    &GameCamera::fn_8038B148, &GameCamera::fn_8038B650, &GameCamera::fn_8038BD78,
    &GameCamera::fn_8038C070, &GameCamera::fn_8038C5B0, &GameCamera::fn_8038A1D8,
    &GameCamera::fn_8038A1B8, &GameCamera::fn_8038C928, &GameCamera::fn_8038CD94,
    &GameCamera::fn_8038D1C0, &GameCamera::fn_8038D4D4, &GameCamera::fn_8038D854,
    &GameCamera::fn_8038DBE4, &GameCamera::fn_80389A00, &GameCamera::fn_80389A00,
    &GameCamera::fn_8038DDE0, &GameCamera::fn_8038DDE0, &GameCamera::fn_8038DF5C,
};

// inferred: update of a mode (21 modes; name invented)
static const GameCameraMode_unk sModeUpdate_unk[21] = {
    &GameCamera::fn_8037F9AC, &GameCamera::fn_803830A8, &GameCamera::fn_8038378C,
    &GameCamera::fn_80384188, &GameCamera::fn_80385B48, &GameCamera::fn_80387548,
    &GameCamera::fn_80387910, &GameCamera::fn_803884B8, &GameCamera::fn_8037F9AC,
    &GameCamera::fn_80382A1C, &GameCamera::fn_80388A88, &GameCamera::fn_80388C44,
    &GameCamera::fn_80389060, &GameCamera::fn_80389210, &GameCamera::fn_803893A0,
    &GameCamera::fn_8038952C, &GameCamera::fn_8037F9AC, &GameCamera::fn_8037F9AC,
    &GameCamera::fn_80389678, &GameCamera::fn_80389678, &GameCamera::fn_803897EC,
};

void GameCamera::fn_8037F744() {
    PlayerTask* leader = lbl_8087F8A0->mHead;
    mNear = lbl_8087EFA8->m1C0;
    mFar = lbl_8087EFA8->m1C4;
    if (m800 != m7FC) {
        (this->*sModeStart_unk[m7FC])(leader);
    } else {
        (this->*sModeUpdate_unk[m7FC])(leader);
    }
    if (m900 < m904) {
        m900++;
    }
    if (m910 != 0) {
        m914 = m914 + 1 < m918 ? m914 + 1 : m918;
    } else {
        int n = m914 - 2;
        m914 = n > 0 ? n : 0;
    }
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

void GameCamera::fn_8038A1D8(const CharaTask* chara) {
    if (m800 == 9) {
        m8EC = 1;
    }
    m800 = m7FC;
    m834 = 0;
}

void GameCamera::fn_80390374() {
    fn_80389838(m804);
}

// inferred: const (the original loads every member before the first store)
void GameCamera::fn_803928C0() {
    m1F4.mPos = m1F4.mPos + m93C;
    m1F4.mTarget = m1F4.mTarget + m948;
    float fovy = m1F4.mFovy;
    m1F4.mFovy = m954 + fovy;
}

// inferred: copies a camera (also called by game/TutorialTask)
extern "C" void fn_80392A04(atn::Camera* dst, const atn::Camera* src);

// NONMATCHING: the original is over the inlining budget and calls fn_80392A04 and RenderManager::SetCamera_unk;
// here both are inlined
void GameCamera::fn_80392964(int a, int b, int c, int d) {
    fn_80392A04(&m1F4, this);
    if (c != 0) {
        fn_80392510();
    }
    if (d != 0) {
        fn_80392FE8();
    }
    if (a != 0) {
        fn_803918EC();
    }
    if (b != 0) {
        fn_803920C8();
    }
    fn_803928C0();
    m1F4.fn_8004B378();
    fn_8008B964()->SetCamera_unk(m1F4);
}

// NONMATCHING: 98% - registers and order of the copy of the frustum planes
extern "C" void fn_80392A04(atn::Camera* dst, const atn::Camera* src) {
    *dst = *src;
}

void GameCamera::fn_80392C94(GameCameraState_unk* state) const {
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
