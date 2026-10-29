#ifndef GMK_HEALPOINT_H
#define GMK_HEALPOINT_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "game/GimmickCommon.h"

class SkillEffect;
struct HitInfo_unk;

namespace gmk {

// inferred: healing point: while the player is in state 2 a skill effect ("SKILL") is cast on the
// player; when it ends, it is cast again after 5 frames. The name is in the DOL
// (__vt__Q23gmk9HealPoint, 0x568 bytes).
class HealPoint : public GimmickCommon {
public:
    HealPoint(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mModel(8, 32), mSkill(0), mEffect(0) {
        m54 = 0;
    }
    HealPoint(atn::Task* parent, GimmickDesc_unk* desc)
        : GimmickCommon(parent, desc->mId, desc->mArg), mModel(8, 32), mSkill(0), mEffect(0), mWait(0) {
        m54 = 0;
        m6C = desc->mPos;
        m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
    }
    static GimmickCommon* fn_8040EC30(atn::Task* parent, int id, int arg);           // inferred: factory
    static GimmickCommon* fn_8040ED00(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~HealPoint();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();

    void fn_8040FAA4(SkillEffect* effect); // inferred: the skill effect ended
    // inferred: names invented
    void StartEffect_unk();
    void StopEffect_unk();

    atn::Model mModel;         // 0xF4: "MODEL"
    atn::ColliObject mColli;  // 0x4C4: "COLLI_OBJECT"
    atn::EfpHandle_unk mEfp;  // 0x54C: "EFP"
    HitInfo_unk* mSkill;      // 0x558: "SKILL": inferred: skill data (fn_80219E6C)
    SkillEffect* mEffect;     // 0x55C
    int mWait;                // 0x560
    int m564;
};

} // namespace gmk

#endif
