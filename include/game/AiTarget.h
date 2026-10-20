#ifndef GAME_AITARGET_H
#define GAME_AITARGET_H

#include "math/Vec.h"

class CharaTask;

// inferred: object with a position at +0x10 aimed at by AiTarget type 2 (class unknown)
struct AiTargetObj_unk {
    char pad00[0x10];
    atn::Vector3 m10;
};

// inferred from __vt__8AiTarget (0x80775B18) and the code of game/TargetPos_unk.cpp
// (AIManager keeps two of them at +0x424/+0x458). Size 0x34.
class AiTarget {
public:
    virtual ~AiTarget();

    void fn_8003E918();               // inferred: reset (type 4 = none)
    atn::Vector3 fn_8003E950() const; // inferred: aimed position (target + offset)

    int m04;
    int m08;          // inferred: type (0 chara, 2 object, 3 fixed position, 4 none)
    CharaTask* m0C;   // inferred: target (AiTargetObj_unk* for type 2)
    atn::Vector3 m10; // inferred: fixed position (type 3)
    atn::Vector3 m1C; // inferred: offset added to the target position
    int m28;
    int m2C;
    int m30;
};

#endif
