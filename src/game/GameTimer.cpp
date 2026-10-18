#include "game/GameTimer.h"
#include "atn/Memory.h"

// inferred: timer display object and its setters (class unknown)
extern "C" void* lbl_8087F430;
extern "C" void fn_803761E4(void* obj, int visible);
extern "C" void fn_803761F4(void* obj, int value);

void GameTimer::vf14() {}

void GameTimer::vf10() {
    if (mState == 1) {
        if (mCountDown != 0) {
            mValue = (mValue - 1 > mLimit) ? mValue - 1 : mLimit;
        } else {
            mValue = (mValue + 1 < mLimit) ? mValue + 1 : mLimit;
        }
    }
    if (lbl_8087F430 != 0) {
        fn_803761E4(lbl_8087F430, mState != 0);
        fn_803761F4(lbl_8087F430, mValue);
    }
}

int GameTimer::vf0C() {
    return fn_800D3FA4() != 0;
}

GameTimer::~GameTimer() {
    sInstance = 0;
}

GameTimer* GameTimer::fn_803CE4B0(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (1, "", "", 0) GameTimer(parent);
    }
    return sInstance;
}
