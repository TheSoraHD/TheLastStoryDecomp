// inferred file name (splits-auto): power/reset handling of the game (power and reset callbacks,
// fade out and shutdown, return to the Wii menu, idle timer), plus five small helpers of GameTask
// and the field that sit before it without data (they may belong to game/ReplayBuffer).
#include "atn/FadeCommon.h"
#include "atn/SequenceControl.h"
#include "atn/Memory.h"

// inferred: view of HomeButtonMenu (game/HomeButtonMenu.cpp), only the members used here
class HomeButtonMenu {
public:
    unsigned char m00[0x84];
    int m84; // inferred: state (0 closed, 1 open, 2 leaving the game)
    int m88; // inferred: menu allowed
    int m8C;
    int m90; // inferred: show the icon when the menu is not allowed

    static HomeButtonMenu* sInstance;
};

extern "C" void OSSetPowerCallback(void (*callback)());
extern "C" void fn_805F69E0(void (*callback)()); // inferred: OSSetResetCallback
extern "C" void OSReturnToMenu();
extern "C" void OSShutdownSystem();
extern "C" void fn_805F38A0(int arg);   // inferred: OSRestart
extern "C" void fn_805F3C50();          // inferred: restart to the disc channel
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

// inferred: game flow manager (lbl_8087F420), only the members used here
struct GameFlow_unk {
    unsigned char m00[0x18];
    int m18;
    int m1C;
    int m20; // inferred: stop request
    unsigned char m24[0x34 - 0x24];
    int m34;

    // inferred: name invented
    int IsBusy_unk() {
        int busy = 0;
        if (m18 != 0 || m1C != 0 || m34 == 0) {
            busy = 1;
        }
        return busy;
    }
};
extern "C" GameFlow_unk* lbl_8087F420;

// inferred: game settings (lbl_8087F0A8), only the member read here
struct GameSettings_unk {
    unsigned char m000[0x194];
    int m194; // inferred: idle timeout enabled
};
extern "C" GameSettings_unk* lbl_8087F0A8;

// inferred: the system state (0x30 bytes, singleton lbl_8087F9A0); names invented
struct GameSystem_unk {
    int mPower;      // 0x00: inferred: power button pressed (restart)
    int mReset;      // 0x04: inferred: reset button pressed
    int mWait;       // 0x08: inferred: wait for the save to end
    int mToMenu;     // 0x0C
    int mToDisc;     // 0x10
    int mShutdown;   // 0x14
    atn::FadeColor* mFade; // 0x18
    int mIdle;       // 0x1C: inferred: frames without input
    int mNoFade;     // 0x20
    int mLock;       // 0x24: inferred: requests are delayed while set
    int mPowerReq;   // 0x28
    int mResetReq;   // 0x2C
};
extern "C" GameSystem_unk* lbl_8087F9A0;
extern "C" void fn_805714DC(GameSystem_unk* sys, int wait);

// inferred: big object of GameTask (0x5BDB4 bytes or more), only the members used here
struct GameTaskData_unk {
    unsigned char m00000[0x04];
    int m00004;
    unsigned char m00008[0x4517C - 0x8];
    int m4517C; // inferred: state (45 = ...)
    unsigned char m45180[0x5BDA8 - 0x45180];
    int m5BDA8;
    int m5BDAC;
    int m5BDB0;
};

extern "C" void fn_80570A14(GameTaskData_unk* obj, int value) {
    obj->m5BDA8 = value;
    if (value != 0) {
        obj->m5BDAC = obj->m00004;
    } else {
        obj->m4517C = 0;
    }
}

extern "C" void fn_80570A40(GameTaskData_unk* obj, int value) {
    obj->m5BDB0 = value;
}

extern "C" int fn_80570A4C(GameTaskData_unk* obj) {
    return obj->m4517C == 45;
}

extern "C" void fn_80570A64(GameTaskData_unk* obj) {
    obj->m4517C = 0;
}

// inferred: list of effects of the field (field->m5590): count at 0x52184, pointers at 0x52188
struct EfpEntry_unk {
    unsigned char m00[0x08];
    void* mOwner; // 0x08
};
struct EfpList_unk {
    unsigned char m00000[0x52184];
    unsigned int mCount;      // 0x52184
    EfpEntry_unk* mItems[1];  // 0x52188
};
struct EfpOwner_unk {
    unsigned char m00[0x48];
    unsigned char m48[0x54 - 0x48];
};
struct EfpOwners_unk {
    unsigned int mCount;
    EfpOwner_unk* mItems;
};
struct EfpHandle_unk {
    EfpOwners_unk* mOwners;
};

// inferred: removes from the list every entry that belongs to one of the owners of `efp`
// NONMATCHING: 89.4% - register allocation: the original keeps the outer counter in r31 and the
// erase index in a volatile register (tried counters declared outside, int/unsigned, an erase inline).
extern "C" void fn_80570A74(EfpList_unk* list, EfpHandle_unk* efp) {
    EfpOwners_unk* owners = efp->mOwners;
    if (owners != 0) {
        for (unsigned int i = 0; i < owners->mCount; i++) {
            void* owner = owners->mItems[i].m48;
            EfpEntry_unk** p = list->mItems;
            while (p != list->mItems + list->mCount) {
                if ((*p)->mOwner == owner) {
                    for (unsigned int j = p - list->mItems; j < list->mCount - 1; j++) {
                        list->mItems[j] = list->mItems[j + 1];
                    }
                    list->mCount = list->mCount - 1;
                } else {
                    p++;
                }
            }
        }
    }
}

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
    GameSystem_unk* sys = lbl_8087F9A0;
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
    GameSystem_unk* sys = lbl_8087F9A0;
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
        GameSystem_unk* sys = new (1, "", "", 0) GameSystem_unk;
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
            OSSetPowerCallback(fn_80570B48);
            fn_805F69E0(fn_80570BD0);
        }
        lbl_8087F9A0 = sys;
    }
}

// inferred: fades the screen out before the request (name invented)
inline void GameSystem_StartFade_unk(GameSystem_unk* sys) {
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
    OSSetPowerCallback(0);
    fn_805F69E0(0);
}

// inferred: per-frame update
// NONMATCHING: 99.9% - in the fade block the original loads mFade into r4 and the HOME button menu
// into r3 (swapped here); tried a local, a helper for the menu state.
extern "C" void fn_80570CF8(GameSystem_unk* sys) {
    int out;
    if (sys->mLock == 0) {
        if (sys->mPowerReq != 0) {
            sys->mPowerReq = 0;
            fn_805714DC(sys, 0);
        }
        if (sys->mResetReq != 0) {
            sys->mResetReq = 0;
            if (sys->mLock != 0) {
                sys->mResetReq = 1;
            } else {
                sys->mReset = 1;
                StopGame_unk();
            }
        }
    }
    if (sys->mShutdown != 0) {
        if (sys->mPower != 0 && sys->mWait != 0 && fn_800A4228(lbl_8087EF68, &out) == 0) {
            fn_805F38A0(0);
        }
        return;
    }
    if (sys->mNoFade != 0 || sys->mFade != 0) {
        if (sys->mFade != 0 && HomeButtonMenu::sInstance != 0 && HomeButtonMenu::sInstance->m84 == 1) {
            sys->mFade->mFlags |= 0x22;
            sys->mFade->Update();
        }
        if (sys->mNoFade == 0) {
            if (sys->mFade == 0) {
                return;
            }
            if (sys->mFade->mFrame <= sys->mFade->mDuration) {
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
        if (sys->mPower != 0) {
            fn_805F38A0(0);
        } else if (sys->mToMenu != 0) {
            OSReturnToMenu();
        } else if (sys->mToDisc != 0) {
            fn_805F3C50();
        } else {
            OSShutdownSystem();
        }
        return;
    }
    if (sys->mPower != 0) {
        if (HomeButtonMenu::sInstance != 0 && HomeButtonMenu::sInstance->m84 >= 1 && HomeButtonMenu::sInstance->m84 <= 2) {
            if (sys->mWait != 0) {
                if (fn_800A4228(lbl_8087EF68, &out) == 0) {
                    fn_805F38A0(0);
                }
            } else {
                HBMStartBlackOut();
                sys->mShutdown = 1;
                OSSetPowerCallback(0);
                fn_805F69E0(0);
                StopGame_unk();
            }
        } else {
            GameSystem_StartFade_unk(sys);
        }
    } else if (sys->mToMenu != 0) {
        if (sys->mWait != 0) {
            if (fn_800A4228(lbl_8087EF68, &out) == 0) {
                OSReturnToMenu();
            }
        } else {
            GameSystem_StartFade_unk(sys);
        }
    } else if (sys->mToDisc != 0) {
        GameSystem_StartFade_unk(sys);
    } else if (sys->mReset != 0) {
        GameSystem_StartFade_unk(sys);
    }
    if (lbl_8087EF70 != 0) {
        sys->mIdle++;
        if (fn_800A5920(lbl_8087EF70, 0, 1) || fn_800A59F0(lbl_8087EF70, 0)) {
            sys->mIdle = 0;
        }
        if (lbl_8087F0A8->m194 != 0 && sys->mIdle > 5400) {
            fn_805714DC(sys, 0);
        }
    }
}

// inferred: restart request (power)
extern "C" void fn_805714DC(GameSystem_unk* sys, int wait) {
    if (sys->mLock != 0) {
        sys->mPowerReq = 1;
    } else {
        sys->mPower = 1;
        sys->mWait = wait;
        StopGame_unk();
    }
}

// inferred: return to the Wii menu
extern "C" void fn_8057155C(GameSystem_unk* sys, int wait) {
    sys->mToMenu = 1;
    sys->mWait = wait;
    StopGame_unk();
}

// inferred: restart to the disc channel
extern "C" void fn_805715C4(GameSystem_unk* sys, int wait) {
    sys->mToDisc = 1;
    sys->mWait = wait;
    StopGame_unk();
}
