#ifndef GAME_LASTCOCOONCHRYSALIS_H
#define GAME_LASTCOCOONCHRYSALIS_H

#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

class PlayerTask;

// inferred: enemy of the last cocoon fight driven by the event flags 200, 230 and 231 (names
// invented; its members at 0x14B4/0x14B8 are set by LastCocoonChrysalis::vf10)
class LastCocoonPart_unk : public EnemyTask {
public:
    int m14B0;
    CharaTask* m14B4;
    CharaTask* m14B8;
};

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
    LastCocoonPart_unk* m14CC;
    int m14D0;                 // inferred: id of m14D4 (2)
    PlayerTask* m14D4;
    int m14D8;                 // inferred: id of m14DC (5)
    LastCocoonPart_unk* m14DC;
    ResHandle m14E0;           // inferred: debug menu group
    int m14E4;                 // inferred: "DebugDisp"
};

#endif
