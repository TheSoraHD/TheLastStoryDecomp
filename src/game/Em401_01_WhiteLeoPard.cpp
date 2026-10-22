#include "game/Em401_01_WhiteLeoPard.h"
#include "math/Vec.h"

// NONMATCHING: 99.9% - the load of info->m20 is scheduled into the prologue in the original (only through a
// const pointer, as in CopyRobotTask; the symbol takes a non-const EnemyInfo_unk*, as in EnemyTrista)
Em401_01_WhiteLeoPard::Em401_01_WhiteLeoPard(atn::Task* parent, EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14C0(0.0f, 0.0f, 0.0f), m155C(0), m1560(0), m1564(0), m1568(9999), m156C(0),
      m1590(0), m1594(1), m1598(1000.0f), m159C(50), m15A0(50), m15A4(1000) {
    mFlag22 = 1;
    mStatus.mParamB.m9C |= 0x10;
    m14F4 = 0.0f;
    m14F8 = 0.0f;
    m14FC = 30.0f;
    m1500 = 0.0f;
    m1504 = 0.0f;
    m1508 = 120;
    m150C = 30;
    m1510 = 0;
    m1514 = 0;
    m1518 = 300.0f;
    m151C = 150.0f;
    m1520 = 120.0f;
    m1524 = 30.0f;
    m1528 = 50.0f;
    m152C = 10000.0f;
    m14A8 |= 0x80000000;
}

Em401_01_WhiteLeoPard::~Em401_01_WhiteLeoPard() {}
