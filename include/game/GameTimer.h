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
    void fn_803CE688(int value, int countDown); // inferred: starts the timer (counts down from value or up to it)
    void fn_803CE6BC(int pause);                // inferred: pauses (state 2) / resumes (state 1)
    void fn_803CE6FC();                         // inferred: stops the timer
    void fn_803CE708(int n);                    // inferred: adds n (clamped when counting up)
    void fn_803CE738(int n);                    // inferred: subtracts n (clamped when counting down)
    void fn_803CE768();                         // inferred: loads the timer from the game variables 0x385/0x386
    void fn_803CE7E0();                         // inferred: stores the timer into the game variables 0x385/0x386

    int mState;     // 0x48: inferred: 1 = running
    int mValue;     // 0x4C
    int mLimit;     // 0x50
    int mCountDown; // 0x54

    static GameTimer* sInstance; // inferred name (0x8087F488)
};

#endif
