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
    void fn_8030D0D8(); // inferred: starts the landing (state 8) towards the seat pair m14E8
    void fn_8030CF84(int warp); // inferred: back to the flying state (7); warp to the start of the route

    int m14B0;
    unsigned char m14B4[0x14E8 - 0x14B4];
    int m14E8;                // inferred: index of the seat pair used to land (m1538)
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
    unsigned char m152C[0x1538 - 0x152C];
    // inferred: route place ids of the two seats where the gargoyle lands (names invented)
    struct Seat_unk {
        int m00;
        int m04;
    };
    Seat_unk m1538[10];
    int m1588;                // inferred: route point indices of m1524/m1528
    int m158C;
    float m1590;
    atn::Vector3 m1594;       // inferred: route point of m158C
    atn::Vector3 m15A0;       // inferred: position when the landing started
    atn::Vector3 m15AC;       // inferred: control point in front of the gargoyle
    atn::Vector3 m15B8;       // inferred: seat positions
    atn::Vector3 m15C4;
    float m15D0;
    atn::EfpHandle_unk m15D4; // inferred: "ef_uc037.efp"
    int m15E0;                // inferred: "DamageCalcMode" (0..2)
    atn::Vector3 m15E4;       // inferred: position of the "neck" node
    unsigned char m15F0[0x15F4 - 0x15F0];
    atn::EfpHandle_unk m15F4; // inferred: "ef_cm008.efp"
    ResHandle m1600;          // inferred: debug menu group
    int m1604;                // inferred: "DebugDisp"
    float m1608;              // inferred: "BulletRotX"
    float m160C;              // inferred: "BulletRotY"
};

#endif
