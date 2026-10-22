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

    atn::LH_Binary m14B0;     // inferred: data file
    unsigned char m14B8[0x14C0 - 0x14B8];
    SpiderList_unk m14C0;
    SpiderList_unk m14CC;
    unsigned char m14D8[0x1550 - 0x14D8];
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
