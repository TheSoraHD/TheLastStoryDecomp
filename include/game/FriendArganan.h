#ifndef GAME_FRIENDARGANAN_H
#define GAME_FRIENDARGANAN_H

#include "game/PlayerTask.h"

// inferred from __vt__13FriendArganan (0x80789ED8)
class FriendArganan : public PlayerTask {
public:
    FriendArganan(atn::Task* parent, int type, void* data);
    virtual ~FriendArganan();
    virtual void vf3C(HitInfo_unk* hit);
};

#endif
