// atn/WPad_unk.cpp: Wii Remote manager of the engine (lbl_8087F018; file name invented). Reads the
// KPAD samples of the four remotes and turns them into per-button states for atn/Pad_unk.cpp.
#include "atn/WPad_unk.h"
#include "atn/MemoryManager.h"
#include "atn/Memory.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);
extern "C" double sqrt(double x);
extern "C" double fabs(double x);

// inferred: RVL SDK (KPAD/WPAD) functions, names unknown
extern "C" void fn_8065F530(void* (*alloc)(unsigned int), int (*free)(void*)); // inferred: WPADRegisterAllocator
extern "C" void fn_80657AF0();                                                 // inferred: KPADInit
extern "C" void fn_80658170(int chan, void (*cb)(int, int));                   // inferred: KPADSetConnectCallback
extern "C" void fn_80658220(int chan);                                         // inferred: KPADDisableDPD
extern "C" void fn_80658240(int chan);                                         // inferred: KPADEnableDPD
extern "C" void fn_806540A0(int chan);
extern "C" int fn_806572D0(int chan, KPADStatus_unk* samples, int count);     // inferred: KPADRead
extern "C" void fn_80660C50(unsigned char sensitivity);                       // inferred: WPADSetDpdSensitivity
extern "C" void fn_80660C90(int chan);
extern "C" void WPADDisconnect(int chan);
extern "C" int fn_80661010(int chan, void* info);                             // inferred: WPADGetInfo
extern "C" void WPADControlMotor(int chan, unsigned int command);

// inferred: engine heap

// inferred: render manager (include/atn/PostFx.h); m3C/m40 are the screen size
struct ScreenSize_unk {
    unsigned char m00[0x3C];
    int m3C;
    int m40;
};
extern "C" ScreenSize_unk* lbl_8087EEE0;

extern "C" {

WPad_unk* lbl_8087F018;

// inferred: connection callback
void fn_800DDA34(int chan, int reason) {
    WPad_unk* wpad = lbl_8087F018;
    if (wpad == 0) {
        return;
    }
    if (reason == 0) {
        wpad->mConnected[chan] = 1;
    } else {
        wpad->mConnected[chan] = 0;
    }
}

// inferred: creates the manager
void fn_800DDA70() {
    if (lbl_8087F018 == 0) {
        lbl_8087F018 = new ((MemoryTag)1, "", "", 0) WPad_unk;
    }
}

// inferred: allocator of the WPAD library
void* fn_800DDAC4(unsigned int size) {
    return atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)1, "", "", 0, 0);
}

int fn_800DDB14(void* ptr) {
    atn::MemoryManager::Instance()->Free(ptr);
    return 1;
}

} // extern "C"

// NONMATCHING: 99% - the counter of the fn_806540A0 loop is r31 in the original (r30 here); tried shared
// counters, pointer loops and declaration orders.
WPad_unk::WPad_unk() {
    mRepeatDelay = 15;
    mRepeatInterval = 5;
    mPointerEnabled = 0;
    m40BC = 0;
    mSensitivity = 5;
    fn_8065F530(fn_800DDAC4, fn_800DDB14);
    fn_80657AF0();
    for (int j = 0; j < 4; j++) {
        fn_80658170(j, fn_800DDA34);
        fn_80658220(j);
    }
    fn_80660C50(mSensitivity);
    memset(mFlags, 0, sizeof(mFlags));
    memset(mHold, 0, sizeof(mHold));
    memset(mTrig, 0, sizeof(mTrig));
    memset(mRelease, 0, sizeof(mRelease));
    memset(mRepeat, 0, sizeof(mRepeat));
    memset(mCount, 0, sizeof(mCount));
    memset(mAnalog, 0, sizeof(mAnalog));
    memset(mPrevAnalog, 0, sizeof(mPrevAnalog));
    memset(mAnalogButton, 0, sizeof(mAnalogButton));
    memset(mRumble, 0, sizeof(mRumble));
    memset(mConnected, 0, sizeof(mConnected));
    m40D0[0] = 1;
    m40D0[1] = 1;
    m40D0[2] = 1;
    m40D0[3] = 1;
    memset(m40E0, 0, sizeof(m40E0));
    memset(mStatus, 0, sizeof(mStatus));
    for (int k = 0; k < 4; k++) {
        fn_806540A0(k);
    }
    KPADStatus_unk* p = mStatus[0];
    for (int i = 0; i < 4; i++, p += 16) {
        fn_806572D0(i, p, 16);
    }
}

extern "C" void fn_800DEB04(WPad_unk* wpad, int port, int axis, int negative, int positive);

#define WPAD_MIN(a, b) ((a) > (b) ? (b) : (a))
#define WPAD_MAX(a, b) ((a) > (b) ? (a) : (b))

// NONMATCHING: ~75% - per-frame update; same blocks as the original but different registers for the
// per-port pointers (the original keeps one base pointer per array: this + port * 4, * 0x64, * 0x2C,
// * 0x50, * 8) and float constants.
extern "C" void fn_800DDD18(WPad_unk* wpad) {
    bool connected[4];
    connected[0] = wpad->mConnected[0];
    connected[1] = wpad->mConnected[1];
    connected[2] = wpad->mConnected[2];
    connected[3] = wpad->mConnected[3];
    memset(wpad->mTrig, 0, sizeof(wpad->mTrig));
    memset(wpad->mRelease, 0, sizeof(wpad->mRelease));
    memset(wpad->mRepeat, 0, sizeof(wpad->mRepeat));
    memcpy(wpad->mPrevAnalog, wpad->mAnalog, sizeof(wpad->mAnalog));
    for (int i = 0; i < 4; i++) {
        fn_806572D0(i, wpad->mStatus[i], 16);
    }
    for (int port = 0; port < 4; port++) {
        KPADStatus_unk* status = wpad->mStatus[port];
        wpad->mFlags[port].mBit0 = 0;
        if (!connected[port] && status->mErr == 0) {
            if (port == 0) {
                for (int i = 0; i < 25; i++) {
                    wpad->Release_unk(port, i);
                }
                for (int i = 0; i < 11; i++) {
                    wpad->mAnalog[port][i] = 0.0f;
                }
                for (int i = 0; i < 20; i++) {
                    wpad->mAnalogButton[port][i] = 0.0f;
                }
            }
            continue;
        }
        if (status->mDevType == 2) {
            wpad->mFlags[port].mBit0 = 1;
            wpad->mAnalog[port][0] = status->mEx.mCl.mLStick[0];
            wpad->mAnalog[port][1] = status->mEx.mCl.mLStick[1];
            wpad->mAnalog[port][2] = status->mEx.mCl.mRStick[0];
            wpad->mAnalog[port][3] = status->mEx.mCl.mRStick[1];
            wpad->mAnalog[port][4] = 0.0f;
            wpad->mAnalog[port][5] = 0.0f;
            wpad->mAnalog[port][6] = 0.0f;
            wpad->mAnalog[port][7] = 0.0f;
            wpad->mAnalog[port][8] = 0.0f;
            wpad->mAnalog[port][9] = 0.0f;
            for (int i = 0; i < 20; i++) {
                unsigned int mask;
                switch (i) {
                case 0:
                    mask = 0x2;
                    break;
                case 1:
                    mask = 0x8000;
                    break;
                case 2:
                    mask = 0x4000;
                    break;
                case 3:
                    mask = 0x1;
                    break;
                case 4:
                    mask = 0x8;
                    break;
                case 8:
                    mask = 0x400;
                    break;
                case 9:
                    mask = 0x1000;
                    break;
                case 10:
                    mask = 0x40;
                    break;
                case 11:
                    mask = 0x10;
                    break;
                case 12:
                    mask = 0x20;
                    break;
                case 13:
                    mask = 0x80;
                    break;
                case 15:
                    mask = 0x800;
                    break;
                case 16:
                    mask = 0x2000;
                    break;
                case 17:
                    mask = 0x200;
                    break;
                case 18:
                    mask = 0x80;
                    break;
                case 19:
                    mask = 0x4;
                    break;
                default:
                    continue;
                }
                if (i == 15) {
                    if ((status->mEx.mCl.mHold & mask) == mask || (status->mHold & (1 << i)) == (1 << i)) {
                        wpad->Press_unk(port, i);
                    } else {
                        wpad->Release_unk(port, i);
                    }
                } else if ((status->mEx.mCl.mHold & mask) == mask) {
                    wpad->Press_unk(port, i);
                } else {
                    wpad->Release_unk(port, i);
                }
            }
            wpad->mAnalogButton[port][16] = (wpad->mHold[port] & 0x10000) == 0x10000 ? 1.0f : 0.0f;
            wpad->mAnalogButton[port][17] = (wpad->mHold[port] & 0x20000) == 0x20000 ? 1.0f : 0.0f;
            wpad->mAnalogButton[port][18] = (wpad->mHold[port] & 0x40000) == 0x40000 ? 1.0f : 0.0f;
            wpad->mAnalogButton[port][19] = (wpad->mHold[port] & 0x80000) == 0x80000 ? 1.0f : 0.0f;
            if (wpad->mAnalogButton[port][16] > 0.5f) {
                wpad->Press_unk(port, 16);
                wpad->Press_unk(port, 14);
            } else {
                wpad->Release_unk(port, 16);
                wpad->Release_unk(port, 14);
            }
            if (wpad->mAnalogButton[port][17] > 0.5f) {
                wpad->Press_unk(port, 17);
            } else {
                wpad->Release_unk(port, 17);
            }
        } else {
            wpad->mFlags[port].mBit1 = 0;
            if (status->mDevType == 1) {
                wpad->mAnalog[port][0] = status->mEx.mFs.mStick[0];
                wpad->mAnalog[port][1] = status->mEx.mFs.mStick[1];
                wpad->mAnalog[port][7] = status->mEx.mFs.mAcc[0];
                wpad->mAnalog[port][8] = status->mEx.mFs.mAcc[1];
                wpad->mAnalog[port][9] = status->mEx.mFs.mAcc[2];
            } else {
                wpad->mAnalog[port][0] = 0.0f;
                wpad->mAnalog[port][1] = 0.0f;
                wpad->mAnalog[port][7] = 0.0f;
                wpad->mAnalog[port][8] = 0.0f;
                wpad->mAnalog[port][9] = 0.0f;
            }
            wpad->mAnalog[port][2] = 0.0f;
            wpad->mAnalog[port][3] = 0.0f;
            wpad->mAnalog[port][4] = status->mAcc[0];
            wpad->mAnalog[port][5] = status->mAcc[1];
            wpad->mAnalog[port][6] = status->mAcc[2];
            if (status->mDevType == 1 || wpad->m40E0[port] == 0) {
                for (int i = 0; i < 20; i++) {
                    if (i >= 16) {
                        wpad->Release_unk(port, i);
                    } else if ((status->mHold & (1 << i)) == (1 << i)) {
                        wpad->Press_unk(port, i);
                    } else {
                        wpad->Release_unk(port, i);
                    }
                }
            } else if ((status->mHold & 0x8000) == 0x8000) {
                wpad->Press_unk(port, 15);
            } else {
                wpad->Release_unk(port, 15);
            }
            wpad->mAnalogButton[port][9] = (wpad->mHold[port] & 0x200) == 0x200 ? 1.0f : 0.0f;
            wpad->mAnalogButton[port][8] = (wpad->mHold[port] & 0x100) == 0x100 ? 1.0f : 0.0f;
        }
        for (int i = 0; i < 4; i++) {
            float v = (float)__fabs(wpad->mAnalog[port][i]) - 0.1f;
            if (0.0f > v) {
                v = 0.0f;
            }
            float sign = wpad->mAnalog[port][i] > 0.0f ? 1.0f : -1.0f;
            wpad->mAnalog[port][i] = sign * (v / 0.9f);
        }
        fn_800DEB04(wpad, port, 0, 20, 21);
        fn_800DEB04(wpad, port, 1, 22, 23);
        if (status[port].mDpdValid >= 2) {
            float width = lbl_8087EEE0->m3C;
            float height = lbl_8087EEE0->m40;
            wpad->mPointer[port].x = WPAD_MAX(0.0f, WPAD_MIN(width * (1.1f * status->mPos[0] + 1.0f) * 0.5f, width));
            wpad->mPointer[port].y = WPAD_MAX(0.0f, WPAD_MIN(height * (1.1f * status->mPos[1] + 1.0f) * 0.5f, height));
        } else {
            wpad->mPointer[port].x = -999.0f;
        }
        if (wpad->mRumble[port] > 0 && --wpad->mRumble[port] <= 0) {
            WPADControlMotor(port, 0);
        }
    }
    for (int i = 0; i < 4; i++) {
        if (wpad->m40D0[i] == 0 && wpad->mConnected[i] != 0) {
            WPADDisconnect(i);
        }
    }
}

extern "C" {

// inferred: turns a stick axis into two direction buttons
void fn_800DEB04(WPad_unk* wpad, int port, int axis, int negative, int positive) {
    float v = wpad->mAnalog[port][axis];
    if (v < -0.5f) {
        if ((wpad->mHold[port] & negative) == negative || v < -0.8f) {
            wpad->Press_unk(port, negative);
            wpad->Release_unk(port, positive);
        } else {
            wpad->Release_unk(port, negative);
            wpad->Release_unk(port, positive);
        }
    } else if (v > 0.5f) {
        if ((wpad->mHold[port] & positive) == positive || v > 0.8f) {
            wpad->Release_unk(port, negative);
            wpad->Press_unk(port, positive);
        } else {
            wpad->Release_unk(port, negative);
            wpad->Release_unk(port, positive);
        }
    } else {
        wpad->Release_unk(port, negative);
        wpad->Release_unk(port, positive);
    }
}

// inferred: button triggered
int fn_800DEEDC(WPad_unk* wpad, int port, int button) {
    return (wpad->mTrig[port] & (1 << button)) == (1 << button);
}

// inferred: button released
int fn_800DEF04(WPad_unk* wpad, int port, int button) {
    return (wpad->mRelease[port] & (1 << button)) == (1 << button);
}

// inferred: button repeated
int fn_800DEF2C(WPad_unk* wpad, int port, int button) {
    return (wpad->mRepeat[port] & (1 << button)) == (1 << button);
}

// inferred: button held
int fn_800DEF54(WPad_unk* wpad, int port, int button) {
    return (wpad->mHold[port] & (1 << button)) == (1 << button);
}

// inferred: analog value of a button
float fn_800DEF7C(WPad_unk* wpad, int port, int button) {
    return wpad->mAnalogButton[port][button];
}

// inferred: stick axis
float fn_800DEF94(WPad_unk* wpad, int port, int axis) {
    return wpad->mAnalog[port][axis];
}

// inferred: 2D vector kept in memory (name invented)
struct StickVec_unk {
    void Set(float x_, float y_) {
        x = x_;
        y = y_;
    }
    float x, y;
};

// inferred: stick flicked past `threshold` (axes 0-3; 0-1 left stick, 2-3 right stick)
int fn_800DEFAC(WPad_unk* wpad, int port, int axis, float threshold) {
    if (0 <= axis && axis <= 3) {
        float scale = 3.0f;
        StickVec_unk cur;
        StickVec_unk prev;
        if ((unsigned int)axis <= 1) {
            cur.Set(scale * wpad->mAnalog[port][0], scale * wpad->mAnalog[port][1]);
            prev.Set(scale * wpad->mPrevAnalog[port][0], scale * wpad->mPrevAnalog[port][1]);
        } else {
            cur.Set(scale * wpad->mAnalog[port][2], scale * wpad->mAnalog[port][3]);
            prev.Set(scale * wpad->mPrevAnalog[port][2], scale * wpad->mPrevAnalog[port][3]);
        }
        StickVec_unk d;
        d.y = cur.y - prev.y;
        d.x = cur.x - prev.x;
        if ((float)sqrt(d.x * d.x + d.y * d.y) > threshold) {
            return 1;
        }
    } else {
        float v = wpad->mAnalog[port][axis];
        if ((float)__fabs((double)v) > threshold && v * wpad->mPrevAnalog[port][axis] < 0.0f) {
            return 1;
        }
    }
    return 0;
}

// inferred: enables the pointer
void fn_800DF0F0(WPad_unk* wpad, int enable) {
    wpad->mPointerEnabled = enable;
    if (enable != 0) {
        for (int i = 0; i < 4; i++) {
            fn_80658240(i);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            fn_80658220(i);
        }
    }
}

// inferred: pointer position on the screen
atn::Vector2_unk fn_800DF154(WPad_unk* wpad, int port) {
    return wpad->mPointer[port];
}

// inferred: sets the pointer sensitivity
void fn_800DF16C(WPad_unk* wpad, int sensitivity) {
    if (wpad->mSensitivity != sensitivity) {
        wpad->mSensitivity = sensitivity;
        for (int i = 0; i < 4; i++) {
            fn_80660C90(i);
        }
        fn_80660C50(wpad->mSensitivity);
    }
}

// inferred: battery level of the first remote (4 when it is not connected)
int fn_800DF1D0(WPad_unk* wpad) {
    if (wpad->mConnected[0] == 0 && wpad->mStatus[0][0].mErr == 0) {
        return 4;
    }
    unsigned char info[0x18];
    if (fn_80661010(0, info) == 0) {
        return info[0x14];
    }
    return 4;
}

} // extern "C"
