#include "game/GameTimer.h"
#include "atn/Memory.h"

// inferred: timer display object and its setters (class unknown)
extern "C" void* lbl_8087F430;
extern "C" void fn_803761E4(void* obj, int visible);
extern "C" void fn_803761F4(void* obj, int value);
extern "C" int fn_803761EC(void* obj);                                 // inferred: getter paired with fn_803761F4
extern "C" int fn_80370174(void* obj, int id);                         // inferred: reads a game variable
extern "C" void fn_80370320(void* obj, int id, int value, int arg3); // inferred: writes a game variable

GameTimer* GameTimer::fn_803CE4B0(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (1, "", "", 0) GameTimer(parent);
    }
    return sInstance;
}

GameTimer::~GameTimer() {
    sInstance = 0;
}

int GameTimer::vf0C() {
    return fn_800D3FA4() != 0;
}

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

void GameTimer::vf14() {}

void GameTimer::fn_803CE688(int value, int countDown) {
    mCountDown = countDown;
    if (countDown != 0) {
        mValue = value;
        mLimit = 0;
    } else {
        mValue = 0;
        mLimit = value;
    }
    mState = 1;
}

void GameTimer::fn_803CE6BC(int pause) {
    if (pause != 0 && mState == 1) {
        mState = 2;
    } else if (pause == 0 && mState == 2) {
        mState = 1;
    }
}

void GameTimer::fn_803CE6FC() {
    mState = 0;
}

void GameTimer::fn_803CE708(int n) {
    mValue += n;
    if (mCountDown == 0) {
        mValue = (mValue < mLimit) ? mValue : mLimit;
    }
}

void GameTimer::fn_803CE738(int n) {
    mValue -= n;
    if (mCountDown != 0) {
        mValue = (mValue > mLimit) ? mValue : mLimit;
    }
}

void GameTimer::fn_803CE768() {
    if (lbl_8087F430 != 0) {
        mValue = fn_803761EC(lbl_8087F430);
        mLimit = fn_80370174(lbl_8087F430, 0x385);
        unsigned int flags = fn_80370174(lbl_8087F430, 0x386);
        mState = flags >> 16;
        mCountDown = (flags & 0xFFFF) != 0;
    }
}

void GameTimer::fn_803CE7E0() {
    if (lbl_8087F430 != 0) {
        fn_803761F4(lbl_8087F430, mValue);
        fn_80370320(lbl_8087F430, 0x385, mLimit, 0);
        fn_80370320(lbl_8087F430, 0x386, (mState << 16) | (mCountDown & 0xFFFF), 0);
    }
}
