#ifndef GMK_AUGUR_H
#define GMK_AUGUR_H

#include "game/GimmickCommon.h"

class CharaTask;
class NpcTask;

namespace gmk {

// inferred: fortune teller (an npc of the npc manager, desc->m20) that calls an npc or the player
// in front of it, plays a scene and sends it away. The name is in the DOL (__vt__Q23gmk5Augur,
// 0x150 bytes).
class Augur : public GimmickCommon {
public:
    Augur(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_8041EB80(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Augur();
    virtual int vf0C();
    virtual void vf10();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();

    void fn_8041EE28();                      // inferred: finds the fortune teller
    CharaTask* fn_8041F434(CharaTask* last); // inferred: chooses the next visitor
    void fn_8041F6A4(int state);             // inferred: changes the scene state

    // inferred: the visitor is released (names invented)
    void ReleaseTarget_unk();

    GimmickDesc_unk* mDesc;  // 0xF4
    int mState;              // 0xF8
    int mTimer;              // 0xFC
    int mRoute;              // 0x100: inferred: route point in front of the gimmick
    atn::Vector3 mFront;     // 0x104: inferred: position in front of the gimmick
    atn::Vector3 mFrontRot;  // 0x110
    CharaTask* mAugur;       // 0x11C
    CharaTask* mTarget;      // 0x120: inferred: visitor
    CharaTask* mLast;        // 0x124
    int mLastId;             // 0x128
    atn::Vector3 mMove;      // 0x12C: inferred: movement per frame of the visitor
    atn::Vector3 mTurn;      // 0x138: inferred: rotation per frame of the visitor
    int m144;
    int m148;
    int m14C;
};

} // namespace gmk

#endif
