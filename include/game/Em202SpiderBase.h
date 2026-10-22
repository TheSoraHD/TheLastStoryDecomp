#ifndef GAME_EM202SPIDERBASE_H
#define GAME_EM202SPIDERBASE_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as ChaserRouteList_unk in
// game/ChaserGuard.h); name invented
struct SpiderBaseList_unk : public std::vector<int> {};

// inferred: color of the materials "em202_body"/"em202_tail" (r, g, b, a; starts white); name invented
struct SpiderBaseColor_unk {
    SpiderBaseColor_unk() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
    float r, g, b, a;
};

// inferred from __vt__15Em202SpiderBase. The factory allocates 0x15E0 bytes (SpecialEnemyBase.cpp).
// Members keep their offsets (names unknown).
class Em202SpiderBase : public EnemyTask {
public:
    Em202SpiderBase(atn::Task* parent, EnemyInfo_unk* info); // 0x80282AE8
    virtual ~Em202SpiderBase();
    virtual void vf7C(int arg);

    void fn_80287788(); // inferred: death

    int m14B0;
    int m14B4;
    unsigned char m14B8[0x14D8 - 0x14B8];
    atn::SoundRef m14D8;
    atn::EfpHandle_unk m14DC; // inferred: "/effect/ef_uc007.efp"
    int m14E8;                // inferred: 60
    int m14EC;                // inferred: 15
    float m14F0;              // inferred: 70.0f
    HitInfo_unk* m14F4;       // inferred: skill 0x3F7 ("NeedleID")
    float m14F8;              // inferred: 180.0f
    unsigned char m14FC[0x151C - 0x14FC];
    atn::SoundRef m151C;
    atn::EfpHandle_unk m1520; // inferred: "/effect/ef_sk002d.efp"
    float m152C;              // inferred: 30.0f
    HitInfo_unk* m1530;       // inferred: skill 0x3F8 ("TackleID")
    float m1534;              // inferred: pi
    unsigned char m1538[0x153C - 0x1538];
    int m153C;
    float m1540;              // inferred: 0.0f
    float m1544;              // inferred: 0.0f
    unsigned char m1548[0x1554 - 0x1548];
    int m1554;
    unsigned char m1558[0x1570 - 0x1558];
    SpiderBaseList_unk m1570;
    int m157C;
    int m1580;
    int m1584;
    unsigned char m1588[0x158C - 0x1588];
    int m158C;
    int m1590;                // inferred: 90
    int m1594;                // inferred: 90
    unsigned char m1598[0x159C - 0x1598];
    int m159C;
    float m15A0;              // inferred: 1.0f
    atn::EfpHandle_unk m15A4; // inferred: "/effect/ef_uc001.efp"
    atn::EfpHandle_unk m15B0; // inferred: "/effect/ef_sc003_01b.efp"
    atn::LH_Binary m15BC;     // inferred: data file
    SpiderBaseColor_unk m15C4; // inferred: color of the materials (vf14)
    ResHandle m15D4;          // inferred: debug menu group
    int m15D8;
    unsigned char m15DC[0x15E0 - 0x15DC];
};

#endif
