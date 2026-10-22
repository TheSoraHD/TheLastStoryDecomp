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
    virtual void vf54();

    unsigned char m14B0[0x15D4 - 0x14B0];
    atn::EfpHandle_unk m15D4;
    unsigned char m15E0[0x15F4 - 0x15E0];
    atn::EfpHandle_unk m15F4;
    ResHandle m1600;          // inferred: debug menu group
    unsigned char m1604[0x1610 - 0x1604];
};

#endif
