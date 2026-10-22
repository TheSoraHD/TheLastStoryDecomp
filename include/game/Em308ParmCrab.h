#ifndef GAME_EM308PARMCRAB_H
#define GAME_EM308PARMCRAB_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Binary.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"
#include "std/vector.h"

// inferred: the destructor tests `this` once more than for a plain std::vector (as ChaserRouteList_unk in
// game/ChaserGuard.h); name invented
struct CrabList_unk : public std::vector<int> {};

// inferred from __vt__13Em308ParmCrab (0x80787A00). The factory allocates 0x1620 bytes
// (SpecialEnemyBase.cpp). Members keep their offsets (names unknown).
class Em308ParmCrab : public EnemyTask {
public:
    Em308ParmCrab(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em308ParmCrab();
    virtual float vf90();

    unsigned char m14B0[0x14D8 - 0x14B0];
    atn::SoundRef m14D8;
    atn::EfpHandle_unk m14DC;
    unsigned char m14E8[0x151C - 0x14E8];
    atn::SoundRef m151C;
    atn::EfpHandle_unk m1520;
    unsigned char m152C[0x1558 - 0x152C];
    CrabList_unk m1558;
    unsigned char m1564[0x15AC - 0x1564];
    CrabList_unk m15AC;
    unsigned char m15B8[0x15F4 - 0x15B8];
    atn::EfpHandle_unk m15F4;
    atn::EfpHandle_unk m1600;
    atn::LH_Binary m160C;     // inferred: data file
    ResHandle m1614;          // inferred: debug menu group
    int m1618;
    unsigned char m161C[0x1620 - 0x161C];
};

#endif
