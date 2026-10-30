#ifndef GAME_QUADRUPEDTASK_H
#define GAME_QUADRUPEDTASK_H

#include "game/EnemyTask.h"

// inferred from __vt__13QuadrupedTask (0x807956F0); the enemy factory (fn_805A5374) allocates 0x1568 bytes.
// Only the constructor is declared until the unit is decompiled.
class QuadrupedTask : public EnemyTask {
public:
    QuadrupedTask(atn::Task* parent, void* data); // 0x80566420

    unsigned char m14B0[0x1568 - 0x14B0]; // inferred: members not decompiled yet
};

#endif
