#ifndef GMK_BULLETPOINT_H
#define GMK_BULLETPOINT_H

#include "game/GimmickCommon.h"

namespace gmk {

// inferred: spawns bullets towards a random character every few frames. The name is in the DOL
// (__vt__Q23gmk11BulletPoint).
class BulletPoint : public GimmickCommon {
public:
    BulletPoint(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mTimer(0) {
        m54 = 0;
        m68 = 3;
    }
    static GimmickCommon* fn_8041B970(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~BulletPoint() {}
    virtual int vf0C();
    virtual void vf10(); // 0x8041BAC0: not decompiled yet
    virtual void vf14();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();

    GimmickDesc_unk* mDesc; // 0xF4
    int mTimer;             // 0xF8: inferred: frames since the last bullet
    int mFC;                // inferred: unused (the object is 0x100 bytes)
};

} // namespace gmk

#endif
