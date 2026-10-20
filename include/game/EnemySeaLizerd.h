#ifndef GAME_ENEMYSEALIZERD_H
#define GAME_ENEMYSEALIZERD_H

#include "game/EnemyTask.h"

// inferred from __vt__14EnemySeaLizerd (0x807896B8); no destructor declared (the implicit one is
// emitted last, 0x8035598C)
class EnemySeaLizerd : public EnemyTask {
public:
    EnemySeaLizerd(atn::Task* parent, EnemyInfo_unk* info);
    virtual int vf0C();
    virtual void vf10();
    virtual void vf28();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual bool vfC4();
    virtual int vfE4();

    void fn_80354910(); // inferred: reads the parameters from the csv file
    void fn_80354DA4(); // inferred: starts the throw (state 7) if the target is in sight
    void fn_80354EE4(); // inferred: update of the throw
    void fn_80355390(); // inferred: starts the jump next to the target (state 6)

    atn::LH_Binary m14B0; // inferred: "/database/chara/special/db_em_sea_lizerd.csv"
    int m14B8;            // inferred: throw timer
    int m14BC;            // inferred: "ThrowWaitTime"
    CharaTask* m14C0;     // inferred: throw target
    int m14C4;            // inferred: damage count
    int m14C8;            // inferred: "SwimDamageCount"
    int m14CC;            // inferred: wait timer
    int m14D0;            // inferred: the bullet was thrown
    int m14D4;            // inferred: "ThrowSkillId"
    atn::Vector3 m14D8;   // inferred: end of the jump
    atn::Vector3 m14E4;   // inferred: start of the jump
    int m14F0;            // inferred: "AppearSkillId"
};

#endif
