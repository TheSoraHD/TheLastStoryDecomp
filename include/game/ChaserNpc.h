#ifndef GAME_CHASERNPC_H
#define GAME_CHASERNPC_H

#include "game/EnemyTask.h"

// inferred: list of ints; the destructor of ChaserNpc tests `this` once more than for a plain
// std::vector, so the vector is wrapped in one more class (name invented)
struct ChaserList_unk : public std::vector<int> {};

// inferred from __vt__9ChaserNpc (0x8077CAE0)
class ChaserNpc : public EnemyTask {
public:
    ChaserNpc(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~ChaserNpc();
    virtual void vf10();
    virtual void vf14();

    int m14B0; // inferred: state (0 and 4 skip the player check in vf10)
    ChaserList_unk m14B4; // inferred: values read from the parameter text
    int m14C0;
    float m14C4;
    int m14C8;
    int m14CC;
    int m14D0;
};

#endif
