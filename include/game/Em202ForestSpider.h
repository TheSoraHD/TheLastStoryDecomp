#ifndef GAME_EM202FORESTSPIDER_H
#define GAME_EM202FORESTSPIDER_H

#include "game/EnemyTask.h"

// inferred from __vt__17Em202ForestSpider (0x807850E0); the enemy factory (fn_805A5374) allocates 0x1968 bytes.
// Only the constructor is declared until the unit is decompiled.
class Em202ForestSpider : public EnemyTask {
public:
    Em202ForestSpider(atn::Task* parent, EnemyInfo_unk* info); // 0x8027278C

    unsigned char m14B0[0x1968 - 0x14B0]; // inferred: members not decompiled yet
};

#endif
