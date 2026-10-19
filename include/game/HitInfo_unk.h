#ifndef GAME_HITINFO_UNK_H
#define GAME_HITINFO_UNK_H

#include "math/Vec.h"

// inferred: values at HitInfo_unk::m74 (names invented)
struct HitRange_unk {
    float m00; // inferred: > 0 sets the hit flag 0x20 (CharaTask::fn_80179730)
    float m04; // inferred: flag 0x40
    float m08;
    float m0C; // inferred: flag 0x400
    float m10; // inferred: flag 0x200
    unsigned char m14[0x1C - 0x14];
};

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
    int m5C; // inferred: duration of the hit (CharaTask::fn_801784BC)
    unsigned char m60[0x68 - 0x60];
    int m68;
    int m6C; // inferred: id of a second bullet (CharaTask::fn_801784BC); 2 and 5 are tested by fn_80178BFC
    int m70; // inferred: id of a sub bullet (CharaTask::fn_8014C540)
    HitRange_unk m74; // inferred: passed to fn_80040588
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
    int mC4; // inferred: copied to the hit message (CharaTask::fn_80151448)

    // inferred: the percentages m98-mA4 as an array (CharaTask::fn_80179730 loops over them); name
    // invented
    const int* GetRates_unk() const { return &m98; }
};

#endif
