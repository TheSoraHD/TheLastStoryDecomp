#include "game/Em101LittleZesyaSecond.h"

Em101LittleZesyaSecond::~Em101LittleZesyaSecond() {}

int Em101LittleZesyaSecond::vf0C() {
    if (fn_8013655C() == 0 && (m1438 == 0 || (m1438->mFlags & 2) == 2) && m1520.fn_80237874() == 0 &&
        m152C.fn_80237874() == 0 && m1538.fn_80237874() == 0 && m14B0.fn_80473F50() == 0) {
        m58C = 0;
        fn_8016E970(2);
        fn_80260324();
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        mStatus.m18 |= 0x1C0;
        mStatus.m18 |= 0x1C21D;
        mStatus.m18 |= 0x3800000;
        mStatus.fn_80134674(2);
        mStatus.fn_80134674(3);
        return 1;
    }
    return 0;
}

int Em101LittleZesyaSecond::vfA8(int kind) {
    if (kind == 1 && mFlag5) {
        bool down = mStatus.IsState_unk(0x80000) || mStatus.IsState_unk(0x1000000);
        if (!down) {
            return 3;
        }
    }
    return -1;
}
