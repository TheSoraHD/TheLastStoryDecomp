#ifndef GMK_DOORSEAL_H
#define GMK_DOORSEAL_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: seal of a door: "MODEL_BEFORE" while sealed (state 1), "MODEL_AFTER" plays its motion
// when it breaks (states 3-5), with a collision object and an effect. The name is in the DOL
// (__vt__Q23gmk8DoorSeal, 0x928 bytes).
class DoorSeal : public GimmickCommon {
public:
    DoorSeal(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mBefore(8, 32), mAfter(8, 32) {
        m54 = 0;
    }
    static GimmickCommon* fn_8042335C(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~DoorSeal();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf70(int state);

    atn::Model mBefore;        // 0xF4: "MODEL_BEFORE"
    atn::Model mAfter;         // 0x4C4: "MODEL_AFTER"
    atn::ColliObject mColli;  // 0x894: "COLLI_OBJECT"
    atn::EfpFile mEfp;  // 0x91C: "EFP"
};

} // namespace gmk

#endif
