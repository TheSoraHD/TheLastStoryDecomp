#ifndef GAME_HITINFO_UNK_H
#define GAME_HITINFO_UNK_H

#include "math/Vec.h"

// inferred: hit/damage description passed to CharaTask::vf3C and vf40 (0x801513D0) and to
// CharaStatus::fn_8012E244; name invented.
struct HitInfo_unk {
    unsigned char m00[0x10];
    atn::Vector3 m10; // inferred: knockback direction
    unsigned char m1C[0x68 - 0x1C];
    int m68;
    unsigned char m6C[0x74 - 0x6C];
    unsigned char m74[0x90 - 0x74]; // inferred: passed to fn_80040588
    int m90;
    int m94;
    int m98;  // inferred: percentages applied to mParamB (fn_8012E244)
    int m9C;
    int mA0;
    int mA4;
    unsigned char mA8[0xAC - 0xA8];
    int mAC; // inferred: flags
};

#endif
