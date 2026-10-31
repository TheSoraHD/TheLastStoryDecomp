#include "chr/ControlLookAround.h"

extern "C" int rand();

namespace chr {

ControlLookAround::ControlLookAround(Chara_unk* chara, int time) : ControlBase(chara), mTime(time) {
    chara->m560 = 0x60;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, 0x45, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m08 = 40.0f * ((rand() % 1001) / 1000.0f);
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
    if (--mTime <= 0) {
        return 1;
    }
    mChara->vf34(mChara->mRot, 0, 0.0f, 1.0f);
    return 0;
}

ControlLookAroundEgg::ControlLookAroundEgg(Chara_unk* chara, int time) : ControlBase(chara), mTime(time) {
    Motion_unk* motion = &chara->mMotion;
    chara->m58C = 5;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, 0x142, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m08 = 40.0f * ((rand() % 1001) / 1000.0f);
}

int ControlLookAroundEgg::vf0C() {
    mChara->vf34(mChara->mRot, 0, 0.0f, 1.0f);
    return 0;
}

} // namespace chr
