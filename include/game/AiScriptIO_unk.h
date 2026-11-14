#ifndef GAME_AISCRIPTIO_UNK_H
#define GAME_AISCRIPTIO_UNK_H

#include "math/Vec.h"

class CharaTask;

// inferred: input and output blocks of the AI scripts, shared by game/AiTarget.cpp (AIManager fills the input
// and reads the output) and AI/AI_archer_attack01.cpp (the scripts). Only data: AI_archer_attack01 does not
// include game/AiTarget.h (its inlines change the register allocation of ~11 scripts).
// inferred: input block of the AI scripts (lbl_807C68C0, 0x180 bytes, returned by fn_8001C260; defined in
// AI/AI_archer_attack01.cpp). Filled by AIManager::fn_80017128. Names invented.
struct AiScriptInput_unk {
    CharaTask* mChara;      // 0x00
    int mState;             // 0x04: CharaC58_unk::m00
    int mDone;              // 0x08: CharaC58_unk::mDone_unk
    int mFrame;             // 0x0C: CharaC58_unk::m04
    atn::Vector3 mPos;      // 0x10
    float mTargetDist;      // 0x1C: distance to mTarget minus its radius (9999: none)
    CharaTask* mTarget;     // 0x20: CharaC58_unk::mC4
    float mFollowDist;      // 0x24 (9999: none)
    CharaTask* mFollow;     // 0x28: CharaC58_unk::mCC
    int mHasGimmick;        // 0x2C
    int m30[3];             // 0x30: CharaRes_unk::m58-m60
    int m3C[3];             // 0x3C: CharaRes_unk::m30-m38 (times 3 in hard mode for enemies)
    int m48[4];             // 0x48: CharaRes_unk::m44, m48, m3C, m40
    int mLife;              // 0x58: percent of m04 over mParamB.m84
    int mMana;              // 0x5C: percent of m08 over mParamB.m88
    int m60[4];             // 0x60
    int m70[3];
    int m7C[9];             // 0x7C: CharaRes_unk::m64-m84 (floats)
    int mA0[2];
    int mA8[24];            // 0xA8: CharaC58_unk::m140 (floats)
    int m108;
    int m10C[16];           // 0x10C: AIManager::m9C
    int m14C[9];
    int m170[4];            // 0x170: CharaC58_unk::m1A0
};
// inferred: output block of the AI scripts (lbl_807C6A40, 0xF0 bytes, returned by fn_8001C26C). Names invented.
struct AiScriptOutput_unk {
    int mValid;    // 0x00: > 0 when the script ran
    int mState;    // 0x04
    int m08;
    int mPart;     // 0x0C: part to hold (-1: none)
    int mC0;       // 0x10
    int m14;
    int m18[24];   // 0x18: back to CharaC58_unk::m140
    int m78;
    int m7C[16];   // 0x7C: back to AIManager::m9C
    int mBC[9];
    int mE0[4];    // 0xE0: back to CharaC58_unk::m1A0 (AI_archer_attack01 keeps a CharaTask* in mE0[0])
};

#endif
