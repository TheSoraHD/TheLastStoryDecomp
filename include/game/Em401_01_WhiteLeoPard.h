#ifndef GAME_EM401_01_WHITELEOPARD_H
#define GAME_EM401_01_WHITELEOPARD_H

#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__21Em401_01_WhiteLeoPard (0x80788A10): white leopard. The factory allocates 0x15A8
// bytes (SpecialEnemyBase.cpp, case 47). Members keep their offsets (names unknown).
class Em401_01_WhiteLeoPard : public EnemyTask {
public:
    Em401_01_WhiteLeoPard(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em401_01_WhiteLeoPard();

    unsigned char m14B0[0x14C0 - 0x14B0];
    atn::Vector3 m14C0;
    unsigned char m14CC[0x14F4 - 0x14CC];
    float m14F4;
    float m14F8;
    float m14FC;
    float m1500;
    float m1504;
    int m1508;
    int m150C;
    int m1510;
    int m1514;
    float m1518;
    float m151C;
    float m1520;
    float m1524;
    float m1528;
    float m152C;
    unsigned char m1530[0x155C - 0x1530];
    int m155C;
    int m1560;
    int m1564;
    int m1568;
    int m156C;
    unsigned char m1570[0x158C - 0x1570];
    ResHandle m158C; // inferred: debug menu group
    int m1590;       // inferred: "DebugDisp"
    int m1594;
    float m1598;
    int m159C;
    int m15A0;
    int m15A4;
};

#endif
