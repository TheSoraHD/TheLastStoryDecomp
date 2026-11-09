// inferred file name (splits-auto): power/reset handling of the game (power and reset callbacks,
// fade out and shutdown, return to the Wii menu, idle timer). The ReplayBuffer members before it are at the
// end of game/ReplayBuffer.cpp.
#include "atn/FadeCommon.h"
#include "atn/SequenceControl.h"
#include "atn/Memory.h"

#include "game/HomeButtonMenu.h"
#include "game/ResetShutdown.h"

extern "C" void OSSetResetCallback(void (*callback)());
extern "C" void OSSetPowerCallback(void (*callback)());
extern "C" void OSReturnToMenu();
extern "C" void OSShutdownSystem();
extern "C" void OSRestart(int arg);   // inferred: OSRestart
extern "C" void OSReturnToDataManager();          // inferred: restart to the disc channel
extern "C" void HBMStartBlackOut();     // inferred: HBM library (stops the HOME button menu sound)
extern "C" void* VISetPreRetraceCallback(void* callback);
extern "C" void* VISetPostRetraceCallback(void* callback);
extern "C" void* GXSetBreakPtCallback(void* callback);
extern "C" void VISetBlack(int black);
extern "C" void VIFlush();
extern "C" void VIWaitForRetrace();

extern "C" void* lbl_8087EF68; // inferred: save manager
extern "C" int fn_800A4228(void* obj, int* out); // inferred: nonzero while saving
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" bool fn_800A5920(void* pad, int port, int all); // inferred: a button was pressed
extern "C" bool fn_800A59F0(void* pad, int port);          // inferred: a stick was moved
extern "C" atn::Task* lbl_8087F530; // inferred: root task (parent of the fade)

#include "game/ErrorManager.h"

// inferred: game settings (lbl_8087F0A8), only the member read here
struct GameSettings_unk {
    unsigned char m000[0x194];
    int m194; // inferred: idle timeout enabled
};
extern "C" GameSettings_unk* lbl_8087F0A8;


// inferred: stops the game while the request is handled (name invented)
inline void StopGame_unk() {
    if (lbl_8087F420 != 0) {
        lbl_8087F420->m20 = 1;
    }
    if (atn::SequenceControl::sInstance != 0) {
        atn::SequenceControl::sInstance->fn_800D246C(1);
    }
    if (HomeButtonMenu::sInstance != 0) {
        HomeButtonMenu::sInstance->m88 = 0;
        HomeButtonMenu::sInstance->m90 = 0;
    }
}

// inferred: power button callback
extern "C" void fn_80570B48() {
    ResetShutdown* sys = lbl_8087F9A0;
    if (sys->mLock != 0) {
        sys->mPowerReq = 1;
    } else {
        sys->mPower = 1;
        sys->mWait = 0;
        StopGame_unk();
    }
}

// inferred: reset button callback
extern "C" void fn_80570BD0() {
    ResetShutdown* sys = lbl_8087F9A0;
    if (sys->mLock != 0) {
        sys->mResetReq = 1;
    } else {
        sys->mReset = 1;
        StopGame_unk();
    }
}

// inferred: creates the singleton and installs the callbacks
extern "C" void fn_80570C50() {
    if (lbl_8087F9A0 == 0) {
        ResetShutdown* sys = new ((MemoryTag)1, "", "", 0) ResetShutdown;
        if (sys != 0) {
            sys->mPower = 0;
            sys->mReset = 0;
            sys->mWait = 0;
            sys->mToMenu = 0;
            sys->mToDisc = 0;
            sys->mShutdown = 0;
            sys->mFade = 0;
            sys->mIdle = 0;
            sys->mNoFade = 0;
            sys->mLock = 0;
            sys->mPowerReq = 0;
            sys->mResetReq = 0;
            OSSetResetCallback(fn_80570B48);
            OSSetPowerCallback(fn_80570BD0);
        }
        lbl_8087F9A0 = sys;
    }
}

// inferred: fades the screen out before the request (name invented)
inline void GameSystem_StartFade_unk(ResetShutdown* sys) {
    if (lbl_8087F530 != 0 && lbl_8087F420 != 0 && !lbl_8087F420->IsBusy_unk()) {
        sys->mFade = atn::FadeColor::Create(lbl_8087F530, 20, 0, 0xFF000000);
        sys->mFade->mWait = 300000;
        sys->mFade->mZ = -1000000.0f;
        sys->mFade->m78 = 1;
        sys->mFade->mActive = 1;
        StopGame_unk();
    } else {
        sys->mNoFade = 1;
    }
    OSSetResetCallback(0);
    OSSetPowerCallback(0);
}

// inferred: per-frame update
// NONMATCHING: 99.9% - in the fade block the original loads mFade into r4 and the HOME button menu
// into r3 (swapped here); tried a local for the fade or the menu, nested ifs, helpers for the menu state
// (bool/int/accessor), `|= 2 | 0x20` and `mFlags = mFlags | 0x22`.
void ResetShutdown::fn_80570CF8() {
    int out;
    if (mLock == 0) {
        if (mPowerReq != 0) {
            mPowerReq = 0;
            fn_805714DC(0);
        }
        if (mResetReq != 0) {
            mResetReq = 0;
            if (mLock != 0) {
                mResetReq = 1;
            } else {
                mReset = 1;
                StopGame_unk();
            }
        }
    }
    if (mShutdown != 0) {
        if (mPower != 0 && mWait != 0 && fn_800A4228(lbl_8087EF68, &out) == 0) {
            OSRestart(0);
        }
        return;
    }
    if (mNoFade != 0 || mFade != 0) {
        if (mFade != 0 && HomeButtonMenu::sInstance != 0 && HomeButtonMenu::sInstance->m84 == 1) {
            mFade->mFlags |= 0x22;
            mFade->Update();
        }
        if (mNoFade == 0) {
            if (mFade == 0) {
                return;
            }
            if (mFade->mFrame <= mFade->mDuration) {
                return;
            }
        }
        if (lbl_8087EF68 != 0 && fn_800A4228(lbl_8087EF68, &out) != 0) {
            return;
        }
        VISetPreRetraceCallback(0);
        VISetPostRetraceCallback(0);
        GXSetBreakPtCallback(0);
        VISetBlack(1);
        VIFlush();
        VIWaitForRetrace();
        if (mPower != 0) {
            OSRestart(0);
        } else if (mToMenu != 0) {
            OSReturnToMenu();
        } else if (mToDisc != 0) {
            OSReturnToDataManager();
        } else {
            OSShutdownSystem();
        }
        return;
    }
    if (mPower != 0) {
        if (HomeButtonMenu::sInstance != 0 && HomeButtonMenu::sInstance->m84 >= 1 && HomeButtonMenu::sInstance->m84 <= 2) {
            if (mWait != 0) {
                if (fn_800A4228(lbl_8087EF68, &out) == 0) {
                    OSRestart(0);
                }
            } else {
                HBMStartBlackOut();
                mShutdown = 1;
                OSSetResetCallback(0);
                OSSetPowerCallback(0);
                StopGame_unk();
            }
        } else {
            GameSystem_StartFade_unk(this);
        }
    } else if (mToMenu != 0) {
        if (mWait != 0) {
            if (fn_800A4228(lbl_8087EF68, &out) == 0) {
                OSReturnToMenu();
            }
        } else {
            GameSystem_StartFade_unk(this);
        }
    } else if (mToDisc != 0) {
        GameSystem_StartFade_unk(this);
    } else if (mReset != 0) {
        GameSystem_StartFade_unk(this);
    }
    if (lbl_8087EF70 != 0) {
        mIdle++;
        if (fn_800A5920(lbl_8087EF70, 0, 1) || fn_800A59F0(lbl_8087EF70, 0)) {
            mIdle = 0;
        }
        if (lbl_8087F0A8->m194 != 0 && mIdle > 5400) {
            fn_805714DC(0);
        }
    }
}

// inferred: restart request (power)
void ResetShutdown::fn_805714DC(int wait) {
    if (mLock != 0) {
        mPowerReq = 1;
    } else {
        mPower = 1;
        mWait = wait;
        StopGame_unk();
    }
}

// inferred: return to the Wii menu
void ResetShutdown::fn_8057155C(int wait) {
    mToMenu = 1;
    mWait = wait;
    StopGame_unk();
}

// inferred: restart to the disc channel
void ResetShutdown::fn_805715C4(int wait) {
    mToDisc = 1;
    mWait = wait;
    StopGame_unk();
}
