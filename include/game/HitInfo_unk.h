#ifndef GAME_HITINFO_UNK_H
#define GAME_HITINFO_UNK_H

#include "math/Vec.h"

// inferred: hit/damage description passed to CharaTask::vf3C and vf40 (0x801513D0) and to
// CharaStatus::fn_8012E244; name invented.
struct HitInfo_unk {
    unsigned char m00;
    signed char m01; // inferred: kind (2 is tested by CharaStatus::fn_8012DF7C)
    unsigned char m02; // inferred: 4 = the effect is centred on the target (CharaTask::fn_8015F568)
    unsigned char m03;
    int m04; // inferred: id (0x1789 tested by CharaTask::vf4C; shown by fn_80149A30 "CIRCLE [%d:%s]")
    const char* m08;
    unsigned char m0C[0x10 - 0x0C];
    atn::Vector3 m10; // inferred: knockback direction
    unsigned char m1C[0x58 - 0x1C];
    float m58; // inferred: passed to chr::ControlChant (CharaTask::fn_8015F568)
    unsigned char m5C[0x68 - 0x5C];
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
    unsigned char mB0[0xC0 - 0xB0];
    int mC0; // inferred: chant time (CharaTask::fn_8015F568)
};

#endif
