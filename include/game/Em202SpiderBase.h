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

// inferred from __vt__15Em202SpiderBase. The factory allocates 0x15E0 bytes (SpecialEnemyBase.cpp).
// Members keep their offsets (names unknown).
class Em202SpiderBase : public EnemyTask {
public:
    Em202SpiderBase(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em202SpiderBase();
    virtual void vf7C(int arg);

    void fn_80287788(); // inferred: death

    int m14B0;
    unsigned char m14B4[0x14D8 - 0x14B4];
    atn::SoundRef m14D8;
    atn::EfpHandle_unk m14DC;
    unsigned char m14E8[0x14F4 - 0x14E8];
    HitInfo_unk* m14F4;       // inferred: skill 0x3F7
    unsigned char m14F8[0x151C - 0x14F8];
    atn::SoundRef m151C;
    atn::EfpHandle_unk m1520;
    unsigned char m152C[0x1570 - 0x152C];
    SpiderBaseList_unk m1570;
    unsigned char m157C[0x158C - 0x157C];
    int m158C;
    int m1590;
    int m1594;
    unsigned char m1598[0x15A4 - 0x1598];
    atn::EfpHandle_unk m15A4;
    atn::EfpHandle_unk m15B0;
    atn::LH_Binary m15BC;     // inferred: data file
    unsigned char m15C4[0x15D4 - 0x15C4];
    ResHandle m15D4;          // inferred: debug menu group
    unsigned char m15D8[0x15E0 - 0x15D8];
};

#endif
