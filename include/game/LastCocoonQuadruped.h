#ifndef GAME_LASTCOCOONQUADRUPED_H
#define GAME_LASTCOCOONQUADRUPED_H

#include "game/EnemyTask.h"

// inferred from __vt__19LastCocoonQuadruped (0x8078F980); the enemy factory (fn_805A5374) allocates 0x18F8 bytes.
// Only the constructor is declared until the unit is decompiled.
class LastCocoonQuadruped : public EnemyTask {
public:
    LastCocoonQuadruped(atn::Task* parent, EnemyInfo_unk* info); // 0x8045FAB0

    unsigned char m14B0[0x18F8 - 0x14B0]; // inferred: members not decompiled yet
};

#endif
