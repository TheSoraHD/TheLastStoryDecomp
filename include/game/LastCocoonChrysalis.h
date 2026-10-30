#ifndef GAME_LASTCOCOONCHRYSALIS_H
#define GAME_LASTCOCOONCHRYSALIS_H

#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/CopyRobotTask.h"
#include "game/ResHandle.h"

class PlayerTask;

// inferred from __vt__19LastCocoonChrysalis (0x8078F500): controller of the last cocoon battle
class LastCocoonChrysalis : public EnemyTask {
public:
    LastCocoonChrysalis(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~LastCocoonChrysalis();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);

    atn::LH_Binary m14B0; // inferred: "database/chara/special/" + db_name (default "db_last_cocoon.csv")
    int m14B8;
    int m14BC;
    int m14C0;                 // inferred: frame counter
    int m14C4;
    int m14C8;                 // inferred: id of m14CC (6)
    CopyRobotTask* m14CC;      // inferred: fn_801CCD00 and the members at 0x14B4/0x14B8
    int m14D0;                 // inferred: id of m14D4 (2)
    PlayerTask* m14D4;
    int m14D8;                 // inferred: id of m14DC (5)
    EnemyTask* m14DC;
    ResHandle m14E0;           // inferred: debug menu group
    int m14E4;                 // inferred: "DebugDisp"
};

#endif
