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

LastCocoonImago::~LastCocoonImago() {}
