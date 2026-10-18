#ifndef GAME_PLAYERTASK_H
#define GAME_PLAYERTASK_H

#include "game/BattleCharaTask.h"

// inferred from __vt__10PlayerTask (0x80794968)
class PlayerTask : public BattleCharaTask {
public:
    PlayerTask(atn::Task* parent, int type, void* data); // 0x8054A52C
    virtual ~PlayerTask();                               // 0x8054A5C4
    // inline: weak copy emitted after FriendArganan (0x8036545C)
    virtual int vfF4() { return 0; }
};

#endif
