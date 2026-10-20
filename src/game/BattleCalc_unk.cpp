// inferred file name (splits-auto): the only function resets a battle parameter block that
// BattleTask keeps at 0x3_8A5C (called before every attack/damage computation).
#include "math/Vec.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector

// inferred: layout from the stores of fn_800E2A24 only; field meanings unknown
struct BattleCalcParam_unk {
    int m00;
    int m04;
    int m08;
    int m0C;
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
