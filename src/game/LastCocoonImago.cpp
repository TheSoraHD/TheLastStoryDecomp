#include "game/LastCocoonImago.h"
#include "chr/ControlBase.h"
#include "game/GameMessageList.h"

// inferred: effect/bullet manager (lbl_8087F048); only the member used here (name invented)
struct Effect_unk {
    unsigned char m00000[0x3E2E8];
    void* m3E2E8; // inferred: sphere of the current target (compared with m18FC)
};
extern "C" Effect_unk* lbl_8087F048;
// inferred: field (lbl_8087F430); only the member used here
struct Field_unk {
    unsigned char m0000[0x54F4];
    unsigned char m54F4[4]; // inferred: name table (fn_803B3B38)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);

atn::Vector3 LastCocoonImago::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (m18D0 != 0 && m58C != 12) {
        result = m624.mData[m18C8].m04.mCenter;
    }
    return result;
}

// inferred: name of the character, unless the current target is its sphere
int LastCocoonImago::vfB8(wchar_t* name) {
    if (lbl_8087F048 != 0 && lbl_8087F048->m3E2E8 != &m18FC && m5C != 0 && m5C->mC4 > 0 && lbl_8087F430 != 0) {
        GameMessage* entry = ((GameMessageList*)lbl_8087F430->m54F4)->fn_803B3B38(m5C->mC4);
        if (entry != 0) {
            wcscpy(name, entry->mText);
            return 1;
        }
    }
    return 0;
}

void LastCocoonImago::vf24() {
    if (m55C == 6 && mControl && !mControl->vf3C()) {
        return;
    }
    if (m197C) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x3B, 1, 0, 1, 0.0f, 0.025f);
    } else if (m14C8 != 0) {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x14, 1, 0, 1, 0.0f, 0.025f);
    } else {
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.025f);
    }
}

void LastCocoonImago::vf38(int enable) {
    for (int i = 0; i < 11; i++) {
        if (enable) {
            mCapsules[i].mFlags |= 1;
        } else {
            mCapsules[i].mFlags &= ~1;
        }
    }
}

// inferred: frames 80-105 of the motion of state 11
int LastCocoonImago::vfE4() {
    if (m58C == 11) {
        float frame = mMotion.mTracks[0].m08;
        if (80.0f <= frame && frame <= 105.0f) {
            return 1;
        }
    }
    return 0;
}

float LastCocoonImago::vfDC() {
    return 0.7f;
}

float LastCocoonImago::vfD4() {
    return 2.2f;
}

float LastCocoonImago::vf90() {
    return 5.0f;
}

LastCocoonImago::~LastCocoonImago() {}
