#ifndef GMK_SKELTONSMIRROR_H
#define GMK_SKELTONSMIRROR_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: mirror gimmick with a model ("MODEL"/"MODEL_LOD"), a collision object
// ("COLLI_OBJECT") and an effect ("EFP"); vf40 messages 1-4 toggle the collision and the effect.
// The name is in the DOL (__vt__Q23gmk14SkeltonsMirror).
class SkeltonsMirror : public GimmickCommon {
public:
    SkeltonsMirror(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mModel(1, 32), mDesc(desc) {
        m54 = 0;
    }
    static GimmickCommon* fn_80414AC4(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~SkeltonsMirror();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();

    Model_unk mModel;         // 0xF4
    atn::ColliObject mColli;  // 0x4C4
    atn::EfpHandle_unk mEfp;  // 0x54C
    GimmickDesc_unk* mDesc;   // 0x558
    int m55C;                 // inferred: unused (the object is 0x560 bytes)
};

} // namespace gmk

#endif
