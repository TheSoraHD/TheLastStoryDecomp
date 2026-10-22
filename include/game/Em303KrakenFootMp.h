#ifndef GAME_EM303KRAKENFOOTMP_H
#define GAME_EM303KRAKENFOOTMP_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "game/ResHandle.h"
#include "game/SpecialEnemyBase.h"

struct RoutePlace_unk;

// inferred from __vt__17Em303KrakenFootMp (0x80787290): leg of the kraken (multiplayer). Data file
// "/database/chara/special/" + db_name (default "db_em303_00_00.csv"). The factory allocates 0x1528 bytes
// (SpecialEnemyBase.cpp).
class Em303KrakenFootMp : public SpecialEnemyBase {
public:
    Em303KrakenFootMp(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em303KrakenFootMp();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf24();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf54();
    virtual void vf80(const atn::Vector3& pos, const atn::Vector3& offset);
    virtual void vfEC();
    virtual void vfFC();
    virtual void vf100(int state);
    virtual void vf104();
    virtual void vf108();
    virtual void vf110(SpecialState_unk* state);
    virtual void vf114(const SpecialState_unk* state);

    void fn_802E5B30(int value); // inferred: setter of m14F0 (called by the kraken)
    void fn_802E5CD0(const char* text, int length); // inferred: reads the data file
    void fn_802E8084(int kind);
    inline void Reset_unk(); // inferred: starts the damage reaction

    atn::LH_Binary m14D4;    // inferred: data file
    int m14DC;
    int m14E0;
    int m14E4;
    int m14E8;
    float m14EC;             // inferred: "YOffset"
    int m14F0;
    int m14F4;
    HitInfo_unk* m14F8;      // inferred: "SwingSkillId"
    HitInfo_unk* m14FC;      // inferred: "PoundSkillId"
    int m1500;
    RoutePlace_unk* m1504;   // inferred: "BasePoint"
    unsigned char m1508;
    int m150C;
    atn::EfpHandle_unk m1510; // inferred: "/effect/ef_bd006.efp"
    ResHandle m151C;          // inferred: debug menu group
    int m1520;                // inferred: "DebugDisp"
    int m1524;
};

#endif
