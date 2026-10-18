#ifndef CHR_CONTROLJUMPAT_H
#define CHR_CONTROLJUMPAT_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: these classes declare no destructor (implicit destructors at 0x801B72B0-0x801B73B0)

// inferred: jump at a target (motion 0x146); then grapples it
class ControlJumpAt : public ControlBase {
public:
    ControlJumpAt(Chara_unk* chara, const atn::Vector3& start, Chara_unk* target); // 0x801B529C
    virtual int vf0C();                     // 0x801B5370
    virtual boost::function<void()> vf10(); // 0x801B54E8

    atn::Vector3 mStart;         // 0x08
    Chara_unk* mTarget; // 0x14
};

// inferred: holding the target (motion 0x147)
class ControlGrapple : public ControlBase {
public:
    ControlGrapple(Chara_unk* chara, Chara_unk* target) : ControlBase(chara), mTarget(target) {
        chara->m560 = 0x7C;
        mChara->m5C0 |= 1;
        Motion_unk* motion = &mChara->mMotion;
        motion->m34C = 1;
        motion->mTracks[0].m20 = 1.0f;
        motion->fn_80097C08(0, 0x147, 1, 0, 1, 0.0f, 0.1f);
        motion->mTracks[0].m0C = 1.0f;
    }
    virtual int vf0C();                     // 0x801B5894
    virtual boost::function<void()> vf10(); // 0x801B5A2C

    Chara_unk* mTarget; // 0x08
};

// inferred: held by mHolder (motion 0x1DA); button presses count mCount down
class ControlGrappled : public ControlBase {
public:
    ControlGrappled(Chara_unk* chara, Chara_unk* holder); // 0x801B618C
    virtual int vf0C();                     // 0x801B6278
    virtual boost::function<void()> vf10(); // 0x801B6494

    Chara_unk* mHolder; // 0x08
    int mCount;         // 0x0C
};

// inferred: holder throwing the grappled character away (motion 0x148)
class ControlGrappleThrown : public ControlBase {
public:
    ControlGrappleThrown(Chara_unk* chara, Chara_unk* target) : ControlBase(chara), mTarget(target) {
        chara->m560 = 0x7E;
        Motion_unk* motion = &mChara->mMotion;
        motion->m34C = 1;
        motion->mTracks[0].m20 = 1.0f;
        motion->fn_80097C08(0, 0x148, 0, 0, 1, 0.0f, 0.1f);
        motion->mTracks[0].m0C = 1.0f;
    }
    virtual int vf0C(); // 0x801B6E70

    Chara_unk* mTarget; // 0x08
};

// inferred: thrown by the holder (motion 0x1DB)
class ControlGrappledThrow : public ControlBase {
public:
    ControlGrappledThrow(Chara_unk* chara, Chara_unk* holder); // 0x801B7184
    virtual int vf0C();                                        // 0x801B7218

    Chara_unk* mHolder; // 0x08
};

} // namespace chr

#endif
