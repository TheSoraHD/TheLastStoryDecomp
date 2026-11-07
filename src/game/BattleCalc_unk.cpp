// inferred file name (splits-auto): the only function resets a battle parameter block that
// BattleTask keeps at 0x3_8A5C (called before every attack/damage computation).
#include "game/BattleCalc_unk.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

extern "C" void fn_800E2A24(BattleCalcParam_unk* p) {
    p->m00 = 0;
    p->m04 = 0;
    p->m08 = 0;
    p->m0C = 0;
    p->m1C = lbl_807C7030;
    p->m28 = lbl_807C7030;
    p->m10 = lbl_807C7030;
    p->m34 = 0;
    p->m38 = 0;
    p->m3C = 3;
    p->m40 = -1;
    p->m44 = 4;
    p->m48 = 0;
    p->m4C = 1;
    p->m50 = 0;
    p->m54 = -1;
    p->m58 = -1;
    p->m5C = 1.0f;
    p->m60 = 1.0f;
    p->m64 = 0;
    p->m68 = 0;
    p->m6C = 0;
    p->m70 = 0;
    p->m74 = 0;
    p->m78 = -1;
    p->m80 &= 0x0FFFFFFF;
    p->m7C = -1;
    p->m84 = 3;
    p->m88 = 2;
    p->m8C = 0;
    p->m90 = 0;
    p->m94 = 0;
}
