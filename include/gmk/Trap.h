#ifndef GMK_TRAP_H
#define GMK_TRAP_H

#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

class CharaTask;
extern "C" void* memset(void* dst, int c, unsigned long n);

namespace gmk {

// inferred: trap that catches the player (message 2), follows the node "PARENT_NODE" of its model
// and releases it after the button 0x22 was pressed enough times. The name is in the DOL
// (__vt__Q23gmk4Trap, 0x5D8 bytes).
class Trap : public GimmickCommon {
public:
    Trap(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mModel(8, 32), mDesc(desc), mCount(0), mDamage(1.0f),
          mPlayer(0) {
        memset(mNode, 0, sizeof(mNode));
        m54 = 0;
    }
    static GimmickCommon* fn_80410A68(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Trap();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);

    atn::Model mModel;       // 0xF4
    GimmickDesc_unk* mDesc; // 0x4C4
    int mCount;             // 0x4C8: inferred: presses left to escape
    float mDamage;          // 0x4CC: inferred: passed to CharaTask::fn_8016A814 (desc->m40 / 100)
    CharaTask* mPlayer;     // 0x4D0
    char mNode[0x100];      // 0x4D4: "PARENT_NODE"
    int m5D4;               // inferred: unused (the object is 0x5D8 bytes)
};

} // namespace gmk

#endif
