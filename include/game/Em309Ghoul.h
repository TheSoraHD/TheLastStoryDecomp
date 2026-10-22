#ifndef GAME_EM309GHOUL_H
#define GAME_EM309GHOUL_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Xsca.h"
#include "atn/ResLoader_unk.h"
#include "atn/SoundRef.h"
#include "game/EnemyTask.h"
#include "game/ResHandle.h"

// inferred from __vt__10Em309Ghoul: ghoul. The factory allocates 0x1700 bytes (SpecialEnemyBase.cpp).
// Members keep their offsets (names unknown).
class Em309Ghoul : public EnemyTask {
public:
    Em309Ghoul(atn::Task* parent, EnemyInfo_unk* info);
    virtual ~Em309Ghoul();
    virtual atn::Vector3 vf64(int center);

    unsigned char m14B0[0x1528 - 0x14B0];
    atn::EfpHandle_unk m1528;
    atn::EfpHandle_unk m1534;
    atn::EfpHandle_unk m1540;
    ResLoader_unk m154C;
    atn::EfpHandle_unk m1558;
    atn::EfpHandle_unk m1564;
    atn::EfpHandle_unk m1570;
    atn::EfpHandle_unk m157C;
    ResLoader_unk m1588;
    atn::EfpHandle_unk m1594;
    atn::EfpHandle_unk m15A0;
    atn::SoundRef m15AC;
    unsigned char m15B0[0x1674 - 0x15B0];
    atn::LH_Xsca m1674[2];
    unsigned char m1684[0x168C - 0x1684];
    ResHandle m168C;          // inferred: debug menu group
    unsigned char m1690[0x1700 - 0x1690];
};

#endif
