#ifndef GAME_BOOTTASK_H
#define GAME_BOOTTASK_H

#include "atn/Task.h"

// inferred: from RTTI "BootTask" (vtable 0x80779ED8, game/Bullet.cpp): first sequence of the game,
// opens the sound archive and starts the sequence named in the settings
class BootTask : public atn::Task {
public:
    BootTask(atn::Task* parent) : atn::Task(parent), mState(0) {}
    virtual ~BootTask();
    virtual int vf0C();
    virtual void vf10();

    int mState; // 0x48: inferred: 0 waiting for the loader, 1 loading, 3 done
    int m4C;
};

atn::Task* fn_8010EB58(atn::Task* const parent); // inferred: creator (registered as "BootTask")

#endif
