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
    m84C = m014;
    m85C = m050;
    m860 = 0;
}

void GameCamera::fn_8037F6D8(const atn::Vector3* pos) {
    m840 = *pos;
    m858 = 1;
    m838 = 1;
    m83C = 0;
    m84C = m014;
    m85C = m050;
    m860 = 0;
}

void GameCamera::fn_8037F724(const atn::Vector3* pos) {
    m840 = *pos;
    m860 = 0;
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
