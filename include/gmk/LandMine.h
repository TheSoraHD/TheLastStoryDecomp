#ifndef GMK_LANDMINE_H
#define GMK_LANDMINE_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: mine that fires a bullet when the player comes near ("EFP", "EFP_WAIT", "MODEL",
// "COLLI_RADIUS"). The name is in the DOL (__vt__Q23gmk8LandMine).
class LandMine : public GimmickCommon {
public:
    LandMine(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mModel(8, 32), mDesc(desc), mBulletId(0), mRadius(30.0f),
          mTimer(0), mHasModel(0), mColliRadius(0.001f) {
        mColli.m20 = 0x800F8; // inferred: collision mask
        m54 = 0;
    }
    static GimmickCommon* fn_80420ADC(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~LandMine() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf44(GimmickHitMsg_unk* msg);
    virtual atn::Vector3 vf54();
    virtual int vf60();

    Model_unk mModel;         // 0xF4
    GimmickDesc_unk* mDesc;   // 0x4C4
    atn::EfpHandle_unk mEfp;  // 0x4C8: "EFP"
    int mBulletId;            // 0x4D4
    atn::EfpHandle_unk mEfpWait; // 0x4D8: "EFP_WAIT"
    float mRadius;            // 0x4E4: inferred: trigger distance
    int mTimer;               // 0x4E8
    int mHasModel;            // 0x4EC
    float mColliRadius;       // 0x4F0: "COLLI_RADIUS"
    atn::ColliSphere mColli;  // 0x4F4
};

} // namespace gmk

#endif
