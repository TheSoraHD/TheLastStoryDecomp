#ifndef GMK_REINFORCEPOINT_H
#define GMK_REINFORCEPOINT_H

#include "game/GimmickCommon.h"

class CharaTask;

namespace gmk {

// inferred: point where a character (enemy or player, desc->m20/m24) is brought back after it
// falls (CharaTask flag 15), counting its knock-outs. The name is in the DOL
// (__vt__Q23gmk14ReinforcePoint, 0x110 bytes).
class ReinforcePoint : public GimmickCommon {
public:
    ReinforcePoint(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mChara(0), mTimer(0), mRespawn(0), mCount(0),
          mDown(0), mFrame(0) {
        m54 = 0;
        m68 = 1;
    }
    static GimmickCommon* fn_803F9420(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~ReinforcePoint() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual int vf64(int arg);

    GimmickDesc_unk* mDesc; // 0xF4
    CharaTask* mChara;      // 0xF8
    int mTimer;             // 0xFC: inferred: frames before the character comes back (desc->m2C)
    int mRespawn;           // 0x100: inferred: bring the character back now
    int mCount;             // 0x104: inferred: knock-outs (vf64(0))
    int mDown;              // 0x108: inferred: the current knock-out was counted
    int mFrame;             // 0x10C: inferred: frames active (compared with desc->m34)
};

} // namespace gmk

#endif
