#ifndef GAME_GAMETIMER_H
#define GAME_GAMETIMER_H

#include "atn/Task.h"

class GameTimer : public atn::Task {
public:
    GameTimer(atn::Task* parent) : atn::Task(parent), mState(0), mValue(0), mLimit(0), mCountDown(1) {}
    virtual ~GameTimer();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    // inferred: creates the singleton under `parent` if it does not exist yet
    static GameTimer* fn_803CE4B0(atn::Task* parent);

    int mState;     // 0x48: inferred: 1 = running
    int mValue;     // 0x4C
    int mLimit;     // 0x50
    int mCountDown; // 0x54

    static GameTimer* sInstance; // inferred name (0x8087F488)
};

#endif
