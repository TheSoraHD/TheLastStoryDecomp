#ifndef GMK_LEMONBUCKET_H
#define GMK_LEMONBUCKET_H

#include "atn/Array_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: bucket of lemons: "MODEL_BEFORE" while full (state 1) and "MODEL_AFTER" plays its
// motion when it tips over (states 2-4); at the "SCATTER" frame it throws its gmk::Lemon gimmicks
// (created by the lemon factory with id 30000) from the node "lemon". The name is in the DOL
// (__vt__Q23gmk11LemonBucket, 0x8D8 bytes). The destructor is implicit (emitted last).
class LemonBucket : public GimmickCommon {
public:
    LemonBucket(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_8042F7EC(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual void vf4C(CharaTask* chara);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);
    void fn_8042FDBC(); // inferred: scatters the lemons

    // inferred: hides every lemon (name invented)
    void HideLemons_unk() {
        for (unsigned int i = 0; i < mLemons.mSize; i++) {
            mLemons[i]->mFlags |= 4;
        }
    }

    GimmickDesc_unk* mDesc;   // 0xF4
    atn::Vector3 mDir;        // 0xF8: inferred: throw direction (0, 0, 1) turned by the rotation
    Model_unk mModel;         // 0x104: "MODEL_BEFORE"
    Model_unk mModel2;        // 0x4D4: "MODEL_AFTER"
    atn::Buffer_unk<GimmickMaterialFade_unk> mFades;    // 0x8A4: "MATALPHA"
    atn::Buffer_unk<GimmickMotionSwitch_unk> mSwitches; // 0x8AC: "NODEVIS"
    atn::Buffer_unk<GimmickCommon*> mLemons;            // 0x8B4
    float mPosRand;           // 0x8BC: "positionRand"
    float mRotRand;           // 0x8C0: "rotationRand"
    float mSpeedRand;         // 0x8C4: "speedRand"
    float mSpeedBase;         // 0x8C8: "speedBase"
    int mScattered;           // 0x8CC
    float mScatterFrame;      // 0x8D0: "SCATTER" 1
    float mRange;             // 0x8D4: "SCATTER" 2: inferred: update distance
};

} // namespace gmk

#endif
