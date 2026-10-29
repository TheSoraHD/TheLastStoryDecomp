#include "gmk/ItemSlotAnime.h"
#include "atn/Task.h"

namespace gmk {

int ItemSlotAnime::fn_8044E610() {
    return mState == 2 && m20 != 0 && m24 > 0;
}

int ItemSlotAnime::fn_80450A5C() {
    return m20 != 0 ? *reinterpret_cast<int*>(static_cast<unsigned char*>(m20) + 4) : 0;
}

} // namespace gmk

extern "C" atn::Task* lbl_8087F4F8;

extern "C" void fn_80450B28() {
    if (lbl_8087F4F8 != 0) {
        lbl_8087F4F8->mFlags |= 4;
    }
}

extern "C" void fn_80450B44() {
    if (lbl_8087F4F8 != 0) {
        lbl_8087F4F8->mFlags &= ~4;
    }
}

extern "C" int fn_80450B60(int value) {
    unsigned int index = value - 0x2711;
    if (index > 7) {
        if (value != 0x271A) {
            goto rejected;
        }
    }
    return 1;
rejected:
    return 0;
}
