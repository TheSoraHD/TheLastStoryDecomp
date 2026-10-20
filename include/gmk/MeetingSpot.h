#ifndef GMK_MEETINGSPOT_H
#define GMK_MEETINGSPOT_H

#include "game/CharaTask.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: spot where two NPCs meet and play a motion together (picked by fn_803FD0A0, the
// motions depend on mDesc->m2C). Twin of gmk::InteractionSpot. The name is in the DOL
// (__vt__Q23gmk11MeetingSpot).
class MeetingSpot : public GimmickCommon {
public:
    MeetingSpot(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), m10C(0) {
        m54 = 0;
        m68 = 1;
        for (int i = 0; i < 2; i++) {
            mChara[i] = 0;
            mPrev[i] = 0;
        }
    }
    static GimmickCommon* fn_803FC8D8(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~MeetingSpot() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();

    inline void PlayMotion_unk(int i, int motion, int loop, float blend = 0.0f); // inferred
    inline bool MatchId_unk(CharaTask* chara); // inferred
    CharaTask* fn_803FD0A0(CharaTask* other); // inferred: picks the farthest NPC within 80 units
    void fn_803FD328(int state);              // inferred: sets the state (10 releases the characters)

    GimmickDesc_unk* mDesc; // 0xF4
    CharaTask* mChara[2];   // 0xF8
    CharaTask* mPrev[2];    // 0x100
    int mState;             // 0x108
    int m10C;               // inferred: route point returned by fn_803C1560
    int mTimer;             // 0x110
    int m114;               // inferred: unused (the object is 0x118 bytes)
};

} // namespace gmk

#endif
