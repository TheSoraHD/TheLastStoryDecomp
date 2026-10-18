#ifndef GMK_INTERACTIONSPOT_H
#define GMK_INTERACTIONSPOT_H

#include "game/CharaTask.h"
#include "game/GimmickCommon.h"

struct Spot_unk;

namespace gmk {

// inferred: point where two characters meet and interact (picked by fn_8042917C among the NPCs,
// placed on two linked spots of the field). The name is in the DOL
// (__vt__Q23gmk15InteractionSpot).
class InteractionSpot : public GimmickCommon {
public:
    InteractionSpot(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), m10C(0) {
        m54 = 0;
        m68 = 1;
        for (int i = 0; i < 2; i++) {
            mChara[i] = 0;
            mPrev[i] = 0;
        }
    }
    static GimmickCommon* fn_80428B0C(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~InteractionSpot() {}
    virtual int vf0C();
    virtual void vf10(); // 0x80428DE8: not decompiled yet
    virtual void vf14();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();

    CharaTask* fn_8042917C(CharaTask* other); // inferred: picks a character; not decompiled yet
    void fn_8042952C(int state);              // inferred: sets the state (11 releases the characters)

    GimmickDesc_unk* mDesc; // 0xF4
    CharaTask* mChara[2];   // 0xF8
    CharaTask* mPrev[2];    // 0x100
    int mState;             // 0x108
    int m10C;               // inferred: handle returned by fn_803C1560
    int mTimer;             // 0x110
    Spot_unk* mSpot[2];     // 0x114
    int mSide;              // 0x11C: inferred: index of the nearer spot
};

} // namespace gmk

#endif
