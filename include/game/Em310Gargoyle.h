#ifndef GAME_EM310GARGOYLE_H
#define GAME_EM310GARGOYLE_H

#include "atn/EfpHandle_unk.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__13Em310Gargoyle (0x807880D0). The factory allocates 0x1610 bytes
// (SpecialEnemyBase.cpp). Members keep their offsets (names unknown).
class Em310Gargoyle : public EnemyTask {
public:
    Em310Gargoyle(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em310Gargoyle();
    virtual int vf0C();
    virtual void vf20();
    virtual void vf3C(HitInfo_unk* hit);
    virtual void vf40(HitInfo_unk* hit);
    virtual void vf54();
    void fn_8030CF84(int warp); // inferred: back to the flying state (7); warp to the start of the route

    unsigned char m14B0[0x14EC - 0x14B0];
    int m14EC;
    unsigned char m14F0[0x14F1 - 0x14F0];
    unsigned char m14F1;      // inferred: set once when the gargoyle is defeated
    unsigned char m14F2[0x14F4 - 0x14F2];
    int m14F4[3];             // inferred: ids of the three riders ("ride1".."ride3")
    EnemyTask* m1500[3];      // inferred: the riders found by id
    unsigned char m150C;      // inferred: cleared when the gargoyle lands on its route
    unsigned char m150D;
    unsigned char m150E[0x1518 - 0x150E];
    int m1518;                // inferred: field counter set when both riders are down
    unsigned char m151C[0x1524 - 0x151C];
    int m1524;                // inferred: route point ids (start / end)
    int m1528;
    unsigned char m152C[0x1588 - 0x152C];
    int m1588;                // inferred: route point indices of m1524/m1528
    int m158C;
    float m1590;
    unsigned char m1594[0x15D0 - 0x1594];
    float m15D0;
    atn::EfpHandle_unk m15D4; // inferred: "ef_uc037.efp"
    int m15E0;                // inferred: "DamageCalcMode" (0..2)
    unsigned char m15E4[0x15F4 - 0x15E4];
    atn::EfpHandle_unk m15F4; // inferred: "ef_cm008.efp"
    ResHandle m1600;          // inferred: debug menu group
    int m1604;                // inferred: "DebugDisp"
    float m1608;              // inferred: "BulletRotX"
    float m160C;              // inferred: "BulletRotY"
};

#endif
