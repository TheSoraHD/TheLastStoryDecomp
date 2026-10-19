#ifndef GMK_ATTRACTHATEPOINT_H
#define GMK_ATTRACTHATEPOINT_H

#include "atn/EfpHandle_unk.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: point that attracts the enemies (state 2 shows "ON", the player within
// "check_radius" triggers state 3 "ATTRACT"). The name is in the DOL
// (__vt__Q23gmk16AttractHatePoint, 0x128 bytes).
class AttractHatePoint : public GimmickCommon {
public:
    AttractHatePoint(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mInRange(0) {
        m54 = 0;
    }
    static GimmickCommon* fn_80421AB8(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~AttractHatePoint();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    GimmickDesc_unk* mDesc;       // 0xF4
    atn::EfpHandle_unk mEfpOn;    // 0xF8: "EFP ON"
    atn::SoundRef mSound;         // 0x104
    atn::EfpHandle_unk mEfpAttract; // 0x108: "EFP ATTRACT"
    atn::EfpHandle_unk mEfpWait;  // 0x114: "EFP WAIT"
    float mRadius;                // 0x120: "check_radius"
    int mInRange;                 // 0x124: inferred: the player is in range this frame
};

} // namespace gmk

#endif
