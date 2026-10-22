#ifndef GAME_EM202SPIDER_H
#define GAME_EM202SPIDER_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "std/vector.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as ChaserRouteList_unk in
// game/ChaserGuard.h); name invented
struct SpiderList_unk : public std::vector<int> {};

// inferred from __vt__11Em202Spider. Members keep their offsets (names unknown).
class Em202Spider : public EnemyTask {
public:
    Em202Spider(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em202Spider();
    virtual void vf7C(int arg);

    void fn_80281ED8(); // inferred: starts state 7 (moves to the place m14B8)

    atn::LH_Binary m14B0;     // inferred: data file
    int m14B8;                // inferred: place id
    unsigned char m14BC[0x14C0 - 0x14BC];
    SpiderList_unk m14C0;
    SpiderList_unk m14CC;
    int m14D8;
    unsigned char m14DC[0x1504 - 0x14DC];
    int m1504;
    unsigned char m1508[0x151C - 0x1508];
    int m151C;
    int m1520;
    unsigned char m1524[0x1550 - 0x1524];
    atn::EfpHandle_unk m1550;
    ResLoader_unk m155C;
    atn::EfpHandle_unk m1568;
    atn::EfpHandle_unk m1574;
    atn::EfpHandle_unk m1580;
    atn::EfpHandle_unk m158C;
    unsigned char m1598[0x15A8 - 0x1598];
    atn::SoundRef m15A8;
};

#endif
