#ifndef GAME_SPECIALENEMYBASE_H
#define GAME_SPECIALENEMYBASE_H

#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"

// inferred: state of a special enemy copied by vf110 and restored by vf114 (multiplayer sync; the
// base class copies m00/m04, Em305VampireMp the rest); names invented
struct SpecialState_unk {
    int m00;          // inferred: state (m58C)
    int m04;          // inferred: step (m14BC)
    short m08;
    signed char m0A;
    signed char m0B;
    int m0C;
};

// inferred from __vt__16SpecialEnemyBase (0x80797368): base of the special enemies driven by a
// state machine (vf100 sets the state m58C, vf104 runs it). Its members are out of line
// (0x805A3C54-0x805A5078, not decompiled); names keep the offsets.
class SpecialEnemyBase : public EnemyTask {
public:
    SpecialEnemyBase(atn::Task* parent, EnemyInfo_unk* info); // 0x805A3C54
    virtual ~SpecialEnemyBase();                              // 0x805A3CFC
    // inferred: weak copy 0x8025589C (game/Em006DarkMaster.cpp)
    virtual void vf24() {
        if (mFlag30) {
            CharaTask::vf24();
        }
    }
    virtual void vf3C(HitInfo_unk* hit); // 0x805A4014
    virtual void vf40(HitInfo_unk* hit); // 0x804F2C40
    virtual int vfF8();                  // 0x805A3F58: inferred: nonzero while a target is in range
    virtual void vfFC() = 0;             // inferred: start
    virtual void vf100(int state) = 0;   // inferred: sets the state (m58C)
    virtual void vf104() = 0;            // inferred: runs the state machine
    virtual void vf108() = 0;            // inferred: runs the current state
    virtual int vf10C(struct HitMsg_unk* msg); // 0x805A3FC8: inferred: filters a hit (1 = ignored)
    virtual void vf110(SpecialState_unk* state);       // 0x805A4F58: inferred: saves the state
    virtual void vf114(const SpecialState_unk* state); // 0x805A4F6C: inferred: restores the state
    virtual void vf118(HitInfo_unk* hit);              // 0x805A4F80

    // inferred: reads "key=value" parameters; nonzero when `params` names a data file (written to `name`)
    int fn_805A3D68(char* name, const char* params);
    void fn_805A40B8(); // inferred: state 2 (dead)
    void fn_805A4254(); // inferred: update while not in control mode 2
    void fn_805A4F1C(int motion, int arg2, int arg3, float blend, float speed); // inferred: plays a motion

    int m14B0;
    atn::LH_Binary m14B4; // inferred: data file of the enemy
    int m14BC;            // inferred: step of the current state
    int m14C0;            // inferred: frame counter
    int m14C4;
    int m14C8;
    // 0x14CC: flags (names invented)
    unsigned int mFlag31 : 1; // inferred: the current state has ended
    unsigned int mFlag30 : 1; // inferred: CharaTask::vf24 runs only while set
    unsigned int mFlag29 : 1;
    unsigned int mFlag28 : 1;
    unsigned int mFlag27 : 1;
    unsigned int mFlag26_0 : 27;
    int m14D0;
};

#endif
