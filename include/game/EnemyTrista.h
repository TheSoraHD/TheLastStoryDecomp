#ifndef GAME_ENEMYTRISTA_H
#define GAME_ENEMYTRISTA_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/CharaStatus.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred: line "Event" of the data file: two event flags and a hit point ratio (names invented)
struct TristaEvent_unk {
    int m00; // inferred: flag set when the event starts
    int m04; // inferred: flag that ends it
    float m08; // inferred: hit point ratio that triggers it
};

// inferred from __vt__11EnemyTrista (0x80789B88): Trista (game file enemy_trista.cpp, see the name of
// its local class ControlShingan). No destructor is declared: the implicit one is emitted at the end
// of the file (0x8035DE10).
class EnemyTrista : public EnemyTask {
public:
    EnemyTrista(atn::Task* parent, EnemyInfo_unk* info);
    virtual int vf0C();
    virtual void vf10();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf44();
    virtual int vfA8(int kind);

    void fn_8035C1B4();                          // inferred: reads the data file
    void fn_8035C8CC();                          // inferred: default state
    void fn_8035CE58();                          // inferred: starts state 7 (attack)
    void fn_8035CF78();                          // inferred: state 7
    void fn_8035D294();                          // inferred: state 8 (high jump attack, camera)
    int fn_8035D4E0(HitMsg_unk* msg);            // inferred: finds where to jump back (m14C4)
    int fn_8035DA94();                           // inferred: nonzero when an event can start
    void fn_8035DC58(int on, float time);        // inferred: guard mode on/off
    // inferred: names invented (inlined)
    inline void StartEvent_unk();
    inline void ClampHp_unk();
    inline void ResetCamera_unk();
    inline void FireEvent_unk();

    atn::LH_Binary m14B0;  // inferred: "database/chara/special/db_enemy_trista.csv"
    int m14B8;             // inferred: frame counter
    int m14BC;
    int m14C0;
    atn::Vector3 m14C4;    // inferred: jump target
    int m14D0;             // inferred: running event (-1: none)
    int m14D4;             // inferred: last event
    int m14D8;             // inferred: camera set by state 8
    int m14DC;
    int m14E0;             // inferred: guard mode
    float m14E4;           // inferred: guard time left
    HitInfo_unk* m14E8;    // inferred: "NormalAttackID"
    HitInfo_unk* m14EC;    // inferred: "GuardCounterID"
    HitInfo_unk* m14F0;    // inferred: "HighJumpAttackID"
    HitInfo_unk* m14F4;    // inferred: "EventDamageSkillID"
    atn::EfpHandle_unk m14F8; // inferred: "/effect/ef_sp007a.efp"
    atn::EfpHandle_unk m1504; // inferred: "/effect/ef_sp007d.efp"
    atn::EfpHandle_unk m1510; // inferred: "/effect/ef_sp007e.efp"
    FixedArray_unk<HitInfo_unk*, 8> m151C;     // inferred: skills that break the guard
    FixedArray_unk<TristaEvent_unk, 8> m1540;  // inferred: "Event"
    int m15A4;             // inferred: "WaitFrame"
    int m15A8;             // inferred: "DamageSubtractWait"
    float m15AC;           // inferred: "AttackChanceFrame"
    int m15B0;             // inferred: "ChaseCancelFrame"
    ResHandle m15B4;       // inferred: debug menu group "EnemyTrista"
};

#endif
