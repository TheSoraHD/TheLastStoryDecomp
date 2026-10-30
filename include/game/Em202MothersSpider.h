#ifndef GAME_EM202MOTHERSSPIDER_H
#define GAME_EM202MOTHERSSPIDER_H

#include "game/Em202ForestSpider.h"

// inferred from __vt__18Em202MothersSpider (0x807852E8); the enemy factory (fn_805A5374) allocates 0x19C8 bytes.
// Only the constructor is declared until the unit is decompiled.
class Em202MothersSpider : public Em202ForestSpider {
public:
    Em202MothersSpider(atn::Task* parent, EnemyInfo_unk* info); // 0x80279860

    unsigned char m1968[0x19C8 - 0x1968]; // inferred: members not decompiled yet
};

#endif
