#ifndef GAME_CHASERNPC_H
#define GAME_CHASERNPC_H

#include "game/EnemyTask.h"

// inferred: list of ints; the destructor of ChaserNpc tests `this` once more than for a plain
// std::vector, so the vector is wrapped in one more class (name invented)
struct ChaserList_unk : public std::vector<int> {};

// inferred from __vt__9ChaserNpc (0x8077CAE0): the character chased by the player
class ChaserNpc : public EnemyTask {
public:
    static ChaserNpc* fn_8018059C(); // inferred: returns sInstance
    ChaserNpc(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~ChaserNpc();
    virtual void vf10();
    virtual void vf14();

    void fn_80180AE0(); // inferred: per-frame update of the state machine (and debug display)
    void fn_8018120C(); // inferred: stops (state 1) and resets the motion layers
    void fn_80181284(); // inferred: starts running (state 3)
    void fn_8018128C(); // inferred: state 3 (RUN)
    void fn_801814F0(); // inferred: state 2 (WALK)
    void fn_8018161C(int state); // inferred: changes the state
    void fn_80181A64(); // inferred: chooses the next place, away from the player
    void fn_80181D94(); // inferred: moves along the planned route
    bool fn_80182134(); // inferred: the player is seen

    int m14B0;            // inferred: state (index into the state names)
    ChaserList_unk m14B4; // inferred: places read from the parameter text
    int m14C0;            // inferred: current place (index into m14B4)
    float m14C4;          // inferred: speed
    int m14C8;            // inferred: frames spent running
    int m14CC;            // inferred: frames the player has been out of sight
    int m14D0;            // inferred: frames before the NPC can be caught again
    atn::Matrix3x4 m14D4; // inferred: matrix while caught (follows the player)

    static ChaserNpc* sInstance; // inferred name (0x8087F090)
};

#endif
