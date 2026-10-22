#include "game/Em005DoppelGanger.h"

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xEC];
    unsigned int mEC; // inferred: flags (bit 3 set while a doppelganger exists)
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" int lbl_8087F3D8; // inferred: cleared by vf14 (file static)

// inferred: offsets of the hit sphere (static constructors at 0x80254530)
static atn::Vector3 sSphereOffset_unk(0.0f, 0.0f, 1.0f);
static atn::Vector3 sOffsetB_unk(0.0f, 0.0f, 26.81f);
static atn::Vector3 sOffsetC_unk(-130.0f, 0.0f, 26.81f);

// inferred: through an inline taking `this`; with the test written in the destructor body the vtable
// store of the original is dropped (name invented)
inline void ClearDoppel_unk(Em005DoppelGanger* self) {
    if (lbl_8087F3C0 != 0) {
        lbl_8087F3C0->mEC &= ~8;
    }
}

Em005DoppelGanger::~Em005DoppelGanger() {
    ClearDoppel_unk(this);
}

void Em005DoppelGanger::vf54() {
    CharaTask::vf54();
    if (m624.mSize != 0) {
        if (m14BC == 0) {
            m624.mData[0].m04.mRadius = 5.0f;
            m624.mData[0].m04.mCenter += sSphereOffset_unk * -3.0f;
        } else {
            m624.mData[0].m04.mRadius = 5.0f;
        }
    }
}

void Em005DoppelGanger::vf14() {
    lbl_8087F3D8 = 0;
    fn_80149A30();
}

void Em005DoppelGanger::vf44() {
    if (m14BC == 0) {
        mStatus.m04 = 1.0f;
        mStatus.fn_8013322C(0x20);
    } else {
        BattleCharaTask::vf44();
    }
}
