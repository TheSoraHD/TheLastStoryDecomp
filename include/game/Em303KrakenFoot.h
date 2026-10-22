#ifndef GAME_EM303KRAKENFOOT_H
#define GAME_EM303KRAKENFOOT_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

struct RoutePlace_unk;

// inferred from __vt__15Em303KrakenFoot (0x80787120): leg of the kraken (Em302Kraken calls
// fn_802E2328/fn_802E2360/fn_802E2468). Data file "/database/chara/special/" + db_name (default
// "db_em303_00_00.csv").
class Em303KrakenFoot : public EnemyTask {
public:
    Em303KrakenFoot(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em303KrakenFoot();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf24();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf54();
    virtual void vfEC();

    void fn_802E2640(const char* text, int length); // inferred: reads the data file
    void fn_802E3604(); // inferred: chooses the target
    void fn_802E37DC(); // inferred: movement
    void fn_802E3B24(); // inferred: action state machine (m14C0)
    void fn_802E3D60();
    void fn_802E3EE8(); // inferred: state 8 (catch)
    void fn_802E470C(); // inferred: state 11 // inferred: starts state 7 (called by the kraken)
    void fn_802E3E38(int arg); // inferred: starts state 8 (m14CC)
    void fn_802E4D6C(); // inferred: starts the attack of state 12 (motion 0x142)
    void fn_802E500C(); // inferred: starts the attack of state 13 (motion 0x14D)
    void fn_802E52AC(); // inferred: starts the attack of state 14 (motion 0x145)
    void fn_802E554C(int arg);
    void fn_802E55B0(int kind); // inferred: damage reaction (called by the kraken)
    inline void Release_unk();
    inline void SetState_unk(int state);   // inferred: names invented
    inline void PlayMotion_unk(int motion);
    void UpdateColli_unk(); // inferred: inline (name invented)
    inline void FaceTarget_unk(); // inferred: releases the caught character (name invented)

    atn::LH_Binary m14B0;     // inferred: data file
    int m14B8;
    int m14BC;
    int m14C0;
    int m14C4;
    float m14C8;
    int m14CC;                // inferred: target character (stored as an int by fn_802E3E38)
    CharaTask* m14D0;         // inferred: caught character (released by vf40/fn_802E55B0)
    void* m14D4;              // inferred: node "catch_point"
    HitInfo_unk* m14D8;       // inferred: "SwingSkillId"
    HitInfo_unk* m14DC;       // inferred: "PoundSkillId"
    int m14E0;                // inferred: "HoldDamageInterval"
    float m14E4;              // inferred: "CatchDistance"
    RoutePlace_unk* m14E8;    // inferred: "BasePoint"
    int m14EC;
    int m14F0;                // inferred: "CatchNotifyFlag" (SpecialFlag_unk: global flag, index)
    int m14F4;
    atn::EfpHandle_unk m14F8; // inferred: "/effect/ef_bd006.efp"
    int m1504;                // inferred: the kraken (CharaTask*, set by fn_802E2468)
    ResHandle m1508;          // inferred: debug menu group
    int m150C;                // inferred: "DebugDisp"
};

extern "C" int fn_802E2328(Em303KrakenFoot* foot);
extern "C" float fn_802E2360(Em303KrakenFoot* foot);
extern "C" void fn_802E2468(Em303KrakenFoot* foot, int value);

#endif
