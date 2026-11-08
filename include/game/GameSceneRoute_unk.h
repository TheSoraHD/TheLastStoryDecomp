#ifndef GAME_GAMESCENEROUTE_UNK_H
#define GAME_GAMESCENEROUTE_UNK_H

#include "math/Vec.h"

// inferred: hide route along a link of the scene (0x40 bytes; filled by GameScene::fn_803CAED0, passed to
// CharaTask::fn_80153698 for a ledge); names invented
struct GameSceneRoute_unk {
    // inferred: name invented
    atn::Vector3 GetDir_unk() const {
        atn::Vector3 dir;
        dir = mTo - mFrom;
        PSVECNormalize(&dir, &dir);
        return dir;
    }

    atn::Vector3 mDir;   // 0x00
    atn::Vector3 mSide;  // 0x0C
    atn::Vector3 mFrom;  // 0x18
    atn::Vector3 mTo;    // 0x24
    unsigned int mFlags; // 0x30: inferred: 1 start on, 2 end on, 4 wide
    int mType;       // 0x34
    int mFromIndex;  // 0x38: inferred: route links
    int mToIndex;    // 0x3C
};

#endif
