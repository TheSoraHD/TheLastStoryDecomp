#ifndef ATN_WPAD_UNK_H
#define ATN_WPAD_UNK_H

// Wii Remote manager of the engine (lbl_8087F018, src/atn/WPad_unk.cpp). All names invented.
// It reads the KPAD samples of the four remotes every frame and keeps per-button state words
// (bit i = button i of the game: 0-15 remote/classic buttons, 16-19 classic triggers and extras,
// 20-23 directions of the sticks) and analog values. atn/Pad_unk.cpp is the game-side interface.
#include "math/Vec.h"

// inferred: KPADStatus of this SDK version (0xF0 bytes)
struct KPADStatus_unk {
    unsigned int mHold;      // 0x00
    unsigned int mTrig;      // 0x04
    unsigned int mRelease;   // 0x08
    float mAcc[3];           // 0x0C
    float m18;
    float m1C;
    float mPos[2];           // 0x20: pointer position (-1..1)
    unsigned char m28[0x5C - 0x28];
    unsigned char mDevType;  // 0x5C: 1 = nunchuk, 2 = classic controller
    signed char mErr;        // 0x5D
    signed char mDpdValid;   // 0x5E
    unsigned char m5F;
    union {
        struct {
            float mStick[2]; // 0x60
            float mAcc[3];   // 0x68
        } mFs;
        struct {
            unsigned int mHold; // 0x60
            unsigned int mTrig;
            unsigned int mRelease;
            float mLStick[2];   // 0x6C
            float mRStick[2];   // 0x74
        } mCl;
    } mEx;
    unsigned char m7C[0xF0 - 0x7C];
};

// inferred: flags of a remote; bit 0: classic controller, bit 1: nunchuk?
struct WPadFlags2_unk {
    unsigned int m_31_2 : 30;
    unsigned int mBit1 : 1;
    unsigned int mBit0 : 1;
};

struct WPad_unk {
    WPad_unk();

    // inferred: button state helpers (inlined everywhere in the original)
    void Press_unk(int port, int button) {
        unsigned int mask = 1 << button;
        if ((mHold[port] & mask) != mask) {
            mTrig[port] |= mask;
            mRepeat[port] |= mask;
            mCount[port][button] = 0;
        }
        if (mCount[port][button] > mRepeatDelay && mCount[port][button] % mRepeatInterval == 0) {
            mRepeat[port] |= mask;
        }
        mCount[port][button]++;
        mHold[port] |= mask;
    }
    void Release_unk(int port, int button) {
        unsigned int mask = 1 << button;
        if ((mHold[port] & mask) == mask) {
            mRelease[port] |= mask;
        }
        mCount[port][button] = -1;
        mHold[port] &= ~mask;
    }

    KPADStatus_unk mStatus[4][16];  // 0x0000
    WPadFlags2_unk mFlags[4];       // 0x3C00
    int mRepeatDelay;               // 0x3C10
    int mRepeatInterval;            // 0x3C14
    unsigned int mHold[4];          // 0x3C18
    unsigned int mTrig[4];          // 0x3C28
    unsigned int mRelease[4];       // 0x3C38
    unsigned int mRepeat[4];        // 0x3C48
    int mCount[4][25];              // 0x3C58: frames held, -1 when released
    float mAnalog[4][11];           // 0x3DE8: sticks (0-3), acceleration (4-6), nunchuk acceleration (7-9)
    float mPrevAnalog[4][11];       // 0x3E98
    float mAnalogButton[4][20];     // 0x3F48
    atn::Vector2_unk mPointer[4];   // 0x4088
    int mPointerEnabled;            // 0x40A8
    int mRumble[4];                 // 0x40AC: frames left
    int m40BC;
    int mConnected[4];              // 0x40C0
    int m40D0[4];
    int m40E0[4];
    int mSensitivity;               // 0x40F0
    int m40F4;
};

#endif
