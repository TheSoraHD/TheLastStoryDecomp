#ifndef GAME_BATTLETARGET_UNK_H
#define GAME_BATTLETARGET_UNK_H

#include "math/Vec.h"

class CharaTask;

// inferred: own header so that the float constants of the inline constructor do not take an @N in every unit
// that includes game/BattleTask.h
// inferred: target found by BattleTask::fn_801004FC / fn_8010089C (BattleCharaTask builds it on the stack)
struct BattleTarget_unk {
    BattleTarget_unk() : mChara(0), m04(0), mIndex(0), mPos(0.0f, 0.0f, 0.0f) {}
    CharaTask* mChara;
    int m04;
    int mIndex;        // inferred: hit sphere of mChara
    atn::Vector3 mPos; // inferred: center of the sphere
};

#endif
