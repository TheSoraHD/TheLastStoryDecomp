#ifndef GMK_SAVEPOINT_H
#define GMK_SAVEPOINT_H

#include "atn/EffectModel_unk.h"
#include "atn/EfpHandle_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: save point (the player near it can save: state 0 idle, 1 shown, 2 the player is in
// range). The name is in the DOL (__vt__Q23gmk9SavePoint, 0x348 bytes).
class SavePoint : public GimmickCommon {
public:
    SavePoint(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mModel(9), mShown(1), mRadius(30.0f), mAlpha(0.0f), mDesc(0), mEnabled(1),
          mSePlayed(0) {
        m54 = 0;
        mE8 = 6;
        mEC = 15;
    }
    static GimmickCommon* fn_80435CE4(atn::Task* parent, int id, int arg); // inferred: factory
    static GimmickCommon* fn_80435DEC(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~SavePoint();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual void vf4C();
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    EffectModel_unk mModel;     // 0xF4: "MODEL"
    atn::EfpHandle_unk mEfp[3]; // 0x308: "EFP" (one per state)
    int mShown;                 // 0x32C
    float mRadius;              // 0x330: inferred: range of state 2
    float mAlpha;               // 0x334: inferred: decreases while the player is out of range
    GimmickDesc_unk* mDesc;     // 0x338
    int mEnabled;               // 0x33C
    int mSePlayed;              // 0x340
    int m344;                   // inferred: unused
};

} // namespace gmk

#endif
