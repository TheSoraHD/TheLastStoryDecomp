#ifndef GAME_LASTCOCOONEGG_H
#define GAME_LASTCOCOONEGG_H

#include "game/EnemyTask.h"

// inferred from __vt__13LastCocoonEgg (0x8078F630); the enemy factory (fn_805A5374) allocates 0x2A50 bytes.
// Only the constructor is declared until the unit is decompiled.
class LastCocoonEgg : public EnemyTask {
public:
    LastCocoonEgg(atn::Task* parent, EnemyInfo_unk* info); // 0x80452F14

    unsigned char m14B0[0x2A50 - 0x14B0]; // inferred: members not decompiled yet
};

#endif
