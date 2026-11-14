#ifndef GAME_BATTLECALC_UNK_H
#define GAME_BATTLECALC_UNK_H

#include "math/Vec.h"

class CharaTask;
struct HitInfo_unk;

// inferred: layout from the stores of fn_800E2A24 only; field meanings unknown
// (BattleTask keeps one at 0x38A5C, filled by BattleTask::fn_800FDE60)
struct BattleCalcParam_unk;
// inferred: resets the block (game/BattleCalc_unk.cpp)
extern "C" void fn_800E2A24(BattleCalcParam_unk*);

struct BattleCalcParam_unk {
    // inferred: inlined in the constructor of BattleTask (0x800F1430)
    BattleCalcParam_unk() : m64(0), m68(0), m6C(0), m70(0), m74(0), m78(-1) {
        m80 &= 0x0FFFFFFF;
        m7C = -1;
        fn_800E2A24(this);
    }
    int m00;          // inferred: effect id (BattleTask::fn_800FDE60)
    CharaTask* m04;   // inferred: character
    HitInfo_unk* m08; // inferred: bullet data
    unsigned int m0C; // inferred: flags
    atn::Vector3 m10;
    atn::Vector3 m1C;
    atn::Vector3 m28;
    int m34;
    int m38;
    int m3C;
    int m40;
    int m44;
    int m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    float m5C;
    float m60;
    int m64;
    int m68;
    int m6C;
    int m70;
    int m74;
    int m78;
    int m7C;
    unsigned int m80; // inferred: top 4 bits are a separate field
    int m84;
    int m88;
    int m8C;
    int m90;
    int m94;
};

#endif
