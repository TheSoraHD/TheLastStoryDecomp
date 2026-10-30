#ifndef GAME_LASTCOCOONIMAGO_H
#define GAME_LASTCOCOONIMAGO_H

#include "game/EnemyTask.h"

// inferred from __vt__15LastCocoonImago (0x8078F7F0); the enemy factory (fn_805A5374) allocates 0x1990 bytes.
// Only the constructor is declared until the unit is decompiled.
class LastCocoonImago : public EnemyTask {
public:
    LastCocoonImago(atn::Task* parent, EnemyInfo_unk* info); // 0x80458270

    unsigned char m14B0[0x1990 - 0x14B0]; // inferred: members not decompiled yet
};

#endif
