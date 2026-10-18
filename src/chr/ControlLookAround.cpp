#include "chr/ControlLookAround.h"

extern "C" int rand();

namespace chr {

ControlLookAround::ControlLookAround(Chara_unk* chara, int time) : ControlBase(chara, time) {
    chara->m560 = 0x60;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->m238 = 1.0f;
    motion->fn_80097C08(0, 0x45, 1, 0, 1, 0.0f, 0.1f);
    motion->m234 = 40.0f * ((rand() % 1001) / 1000.0f);
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    mChara->fn_8016DA4C();
    mChara->mFlag5 = 0;
}

int ControlLookAround::vf0C() {
    if (--m08 <= 0) {
        return 1;
    }
    mChara->vf34(mChara->m534, 0, 0.0f, 1.0f);
    return 0;
}

ControlLookAroundEgg::ControlLookAroundEgg(Chara_unk* chara, int time) : ControlBase(chara, time) {
    Motion_unk* motion = &chara->mMotion;
    chara->m58C = 5;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->m238 = 1.0f;
    motion->fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
    motion->m234 = 40.0f * ((rand() % 1001) / 1000.0f);
}

int ControlLookAroundEgg::vf0C() {
    mChara->vf34(mChara->m534, 0, 0.0f, 1.0f);
    return 0;
}

} // namespace chr
