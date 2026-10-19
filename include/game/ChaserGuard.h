#ifndef GAME_CHASERGUARD_H
#define GAME_CHASERGUARD_H

#include "game/EnemyTask.h"

// inferred: the destructor of ChaserGuard tests `this` once more than for a plain std::vector,
// so the vector is wrapped in one more class (name invented)
struct ChaserRouteList_unk : public std::vector<int> {};

// inferred from __vt__11ChaserGuard (0x8077C968)
class ChaserGuard : public EnemyTask {
public:
    ChaserGuard(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~ChaserGuard();
    virtual void vf10();
    virtual void vf14();

    void fn_8017D13C(); // inferred: per-frame update of the state machine (and debug display)
    void fn_8017E650(); // inferred: state 1 (CHASE)
    void fn_8017E9D8(); // inferred: state 3 (CAUGHT)
    void fn_8017F118(); // inferred: state 4 (LOST)
    void fn_8017F348(); // inferred: state 5 (SEARCH): walks the route
    void fn_8017F4A0(int state); // inferred: changes the state
    bool fn_8017FBEC(); // inferred: the player is seen (sets m14B8/m14C4)
    void fn_8017FED0(); // inferred: moves along the planned route
    // inferred: inlined helpers (names invented)
    void SwingHead_unk(const struct ChaseParam_unk* param);
    bool IsPlayerNear_unk();

    int m14B0;          // inferred: state (index into the state names)
    CharaTask* m14B4;   // inferred: caught character
    atn::Vector3 m14B8; // inferred: last position where the player was seen
    atn::Vector3 m14C4; // inferred: direction from there
    int m14D0;          // inferred: frame counters of the states
    int m14D4;
    int m14D8;
    atn::Vector3 m14DC; // inferred: spawn position
    float m14E8;        // inferred: spawn rotation
    ChaserRouteList_unk m14EC; // inferred: route points read from the parameter text
    unsigned int m14F8; // inferred: current route point
    int m14FC;
    float m1500;        // inferred: head swing direction (1 or -1)
    int m1504;          // inferred: set while chasing the player
    int m1508;          // inferred: frames spent chasing
    CharaTask* m150C;   // inferred: character caught by another guard
    atn::Vector3 m1510; // inferred: place to walk to after the catch
};

#endif
