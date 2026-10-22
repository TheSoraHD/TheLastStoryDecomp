#ifndef GMK_BANANA_H
#define GMK_BANANA_H

#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: model that falls with a random velocity and spin when it receives message 3, then
// returns to its place. The name is in the DOL (__vt__Q23gmk6Banana).
class Banana : public GimmickCommon {
public:
    Banana(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mModel(1, 32), mDesc(desc), mTimer(0) {
        m54 = 0;
    }
    static GimmickCommon* fn_804166B0(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Banana() {}
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

    Model_unk mModel;       // 0xF4
    GimmickDesc_unk* mDesc; // 0x4C4
    int mTimer;             // 0x4C8
    atn::Vector3 mVel;      // 0x4CC
    atn::Vector3 mRotVel;   // 0x4D8
    int m4E4;               // inferred: unused (the object is 0x4E8 bytes)
};

} // namespace gmk

#endif
