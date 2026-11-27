#ifndef GAME_CAMERACONFIG_UNK_H
#define GAME_CAMERACONFIG_UNK_H

#include "game/SetConfig.h"

// inferred: camera part of SetConfig (from mAttackTurnSpeed); names invented. Event/Path.cpp and
// game/GameCamera_unk.cpp reach it through GetCameraConfig_unk
struct CameraConfig_unk {
    // inferred: weak copies 0x801156CC, 0x801156E0 and 0x801156F8
    int IsInvertX_unk() { return (m10 & 4) == 4; }
    float GetRotScale_unk() { return 0.3f * (m0C - 1.0f) + 1.0f; }
    int IsInvertY_unk() { return (m10 & 8) == 8; }
    float m00; // inferred: SetConfig::mAttackTurnSpeed
    float m04;
    float m08;
    float m0C; // inferred: SetConfig::mCameraRotSpeed
    unsigned int m10;
};
// inferred: weak copies 0x801156C4 and 0x801156D8
inline SetConfig* GetSetConfig_unk() {
    return SetConfig::sInstance;
}
inline CameraConfig_unk* GetCameraConfig_unk(SetConfig* config) {
    return (CameraConfig_unk*)&config->mAttackTurnSpeed;
}

#endif
