#ifndef GAME_ENEMYLIZERDBLADE_H
#define GAME_ENEMYLIZERDBLADE_H

#include "game/EnemyTask.h"

// inferred from __vt__16EnemyLizerdBlade (0x80788FC0); no destructor declared (the implicit one is
// emitted last, 0x80334ACC)
class EnemyLizerdBlade : public EnemyTask {
public:
    EnemyLizerdBlade(atn::Task* parent, EnemyInfo_unk* info);
    virtual int vf0C();
    virtual void vf10();
    virtual void vf24();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual bool vfC4();

    void fn_803338D8(); // inferred: reads the parameters from the csv file
    void fn_80333C40(); // inferred: starts the tackle (state 7) if the target is in sight
    void fn_80333D68(); // inferred: update of the tackle
    void fn_803342EC(); // inferred: starts the jump attack (state 6) if the target is close
    void fn_80334460(); // inferred: update of the jump attack

    atn::LH_Binary m14B0; // inferred: "/database/chara/special/db_em_lizerd_blade.csv"
    int m14B8;            // inferred: step of the attack
    int m14BC;            // inferred: jump attack timer
    int m14C0;            // inferred: "PressWaitTime"
    int m14C4;            // inferred: "PressSkillId"
    int m14C8;            // inferred: tackle timer
    int m14CC;            // inferred: "TackleWaitTime"
    int m14D0;            // inferred: "TackleSkillId"
    float m14D4;          // inferred: time of the tackle
    CharaTask* m14D8;     // inferred: target of the attack
    atn::Vector3 m14DC;   // inferred: start of the jump
    atn::Vector3 m14E8;   // inferred: end of the jump
};

#endif
