#ifndef GAME_EM206EGGMONSTER_H
#define GAME_EM206EGGMONSTER_H

#include "game/EnemyTask.h"

// inferred from __vt__15Em206EggMonster (0x807866F0); the enemy factory (fn_805A5374) allocates 0x1718 bytes.
// Only the constructor is declared until the unit is decompiled.
class Em206EggMonster : public EnemyTask {
public:
    Em206EggMonster(atn::Task* parent, void* data); // 0x802C0C38

    unsigned char m14B0[0x1718 - 0x14B0]; // inferred: members not decompiled yet
};

#endif
