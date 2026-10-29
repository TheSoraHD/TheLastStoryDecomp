#ifndef GMK_FLAMINGARROW_H
#define GMK_FLAMINGARROW_H

#include "atn/EfpHandle_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: flying arrow of gmk::FlamingArrow (0x3EC bytes; constructor and destructor out of
// line for the array). Name invented.
struct FlamingArrowShot_unk {
    FlamingArrowShot_unk(); // 0x80417098
    ~FlamingArrowShot_unk(); // 0x804170D8

    atn::Model mModel;   // 0x00: "MODEL_ARROW"
    int mState;         // 0x3D0: inferred: 0 free, 1 flying, 2 stuck, 3 fading
    atn::Vector3 mPos;  // 0x3D4
    atn::Vector3 mVel;  // 0x3E0
};

// inferred: shoots flaming arrows at a route place or at a random character every desc->m28 frames
// (skill desc->m20). The name is in the DOL (__vt__Q23gmk12FlamingArrow, 0x2490 bytes).
class FlamingArrow : public GimmickCommon {
public:
    FlamingArrow(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mTimer(0), mModel(8, 1) {
        m54 = 0;
    }
    static GimmickCommon* fn_80416F74(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~FlamingArrow();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();

    FlamingArrowShot_unk* fn_8041762C(); // inferred: free arrow

    GimmickDesc_unk* mDesc;                // 0xF4
    int mTimer;                            // 0xF8
    atn::Model mModel;                      // 0xFC: "MODEL_BASE"
    FlamingArrowShot_unk mArrows[8];       // 0x4CC
    atn::EfpHandle_unk mEfpShoot;          // 0x242C: "EFP_SHOOT"
    atn::EfpHandle_unk mEfpArrow;          // 0x2438: "EFP_ARROW"
    atn::EfpHandle_unk mEfpBurnout[3];     // 0x2444: "EFP_BURNOUT"
    atn::EfpHandle_unk mEfpHit[3];         // 0x2468: "EFF_HIT"
    int m248C;
};

} // namespace gmk

#endif
