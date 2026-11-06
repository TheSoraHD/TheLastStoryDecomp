#include "game/LastCocoonImago.h"

atn::Vector3 LastCocoonImago::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (m18D0 != 0 && m58C != 12) {
        result = m624.mData[m18C8].m04.mCenter;
    }
    return result;
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
