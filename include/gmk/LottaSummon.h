#ifndef GMK_LOTTASUMMON_H
#define GMK_LOTTASUMMON_H

#include "atn/EfpHandle_unk.h"
#include "game/GimmickCommon.h"

class NpcTask;

namespace gmk {

// inferred: summoning point of Lotta (an npc taken from the npc manager is placed on the point, and
// the players near it are pushed away). The name is in the DOL (__vt__Q23gmk11LottaSummon,
// 0x148 bytes).
class LottaSummon : public GimmickCommon {
public:
    LottaSummon(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mDesc(0), mNpc(0), mEnabled(1), mNear(0), mTimer(0) {
        m54 = 1;
    }
    static GimmickCommon* fn_8043A770(atn::Task* parent, int id, int arg); // inferred: factory
    static GimmickCommon* fn_8043A850(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~LottaSummon();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    atn::EfpFile mEfp[5]; // 0xF4: [1..4] "EFP" in order; [1]/[2] states 1/2, [3] state 3, [4] summon
    GimmickDesc_unk* mDesc;     // 0x130
    NpcTask* mNpc;              // 0x134: inferred: summoned npc
    int mEnabled;               // 0x138: inferred: copy of mDesc->m7C
    int mNear;                  // 0x13C: inferred: the player is near (state 2)
    int mTimer;                 // 0x140: inferred: frames until the players are pushed away
    int m144;                   // inferred: unused
};

} // namespace gmk

#endif
