#ifndef GMK_WAITOBJECT_H
#define GMK_WAITOBJECT_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/CharaManager.h"
#include "game/GimmickCommon.h"
#include <boost/scoped_ptr.hpp>

extern "C" void* memset(void* dst, int c, unsigned long n);

namespace gmk {

// inferred: data of one state ("STATE"/"COLLI"; 0x28 bytes, seven of them). Names invented.
struct WaitState_unk {
    WaitState_unk()
        : m00(0), m04(0.0f), m08(1), m09(1), m0C(0.0f), mFlags(0), mColli(0), mMove(0.0f, 0.0f, 0.0f), m24(0.0f) {}
    int m00;
    float m04;
    unsigned char m08;
    unsigned char m09;
    float m0C;
    unsigned int mFlags; // 0x10: inferred: bit 1 = no debug draw
    atn::ColliBase* mColli; // 0x14
    atn::Vector3 mMove; // 0x18: inferred: offset of TRANS/TRANSINV
    float m24;          // inferred: frames of the move
};

// inferred: trigger of a state ("TRIGGER"; 0x1C bytes, up to 16). Names invented.
struct WaitTrigger_unk {
    WaitTrigger_unk() : mState(0), mNext(0), mType(0) { memset(m08, 0, sizeof(m08)); }
    signed char mState; // 0x00: state the trigger belongs to
    signed char mNext;  // 0x01: state it switches to
    int mType;          // 0x04: inferred: 4 attention point, 5 collision target
    unsigned char m08[0x14]; // inferred: m08 is the radius (float)
};

// inferred: object with a model, a collision object, effects and states ("MODEL", "MOTION",
// "COLLI_OBJECT", "EFP", "STATE", "TRIGGER"...). The name is in the DOL
// (__vt__Q23gmk10WaitObject, 0x818 bytes).
class WaitObject : public GimmickCommon {
public:
    WaitObject(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_80418EAC(atn::Task* parent, int id, int arg);         // inferred: factory
    static GimmickCommon* fn_80418F30(atn::Task* parent, GimmickDesc_unk* desc);   // inferred: factory
    virtual ~WaitObject();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf44(GimmickHitMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual void vf4C(CharaTask* chara);
    virtual int vf50();
    virtual atn::Vector3 vf54();
    virtual float vf58();
    virtual int vf5C();
    virtual int vf60();
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    inline void PlayEfp_unk(); // inferred: name invented
    int fn_8041AAC0(); // inferred: index of the trigger of the current state that fires (-1 if none)
    // inferred: a character of the manager is in the area of the trigger (weak copies 0x8041B28C
    // npc, 0x8041B4D8 enemy, 0x8041B724 player)
    template <class T>
    int InArea_unk(CharaManager<T>* manager, WaitTrigger_unk* trigger);

    // inferred: collision of a state (name invented)
    atn::ColliBase* GetColli_unk(int state) { return mStates[state].mColli; }

    atn::Model mModel;                         // 0xF4
    boost::scoped_ptr<atn::ColliObject> mColli; // 0x4C4: "COLLI_OBJECT"
    atn::EfpHandle_unk mEfp[7];               // 0x4C8: "EFP"
    WaitState_unk mStates[7];                 // 0x51C
    unsigned int mNumTriggers;                // 0x634
    WaitTrigger_unk mTriggers[16];            // 0x638
    int m7F8;
    atn::Vector3 m7FC; // inferred: initial position
    atn::Vector3 m808; // inferred: initial rotation
    GimmickDesc_unk* mDesc; // 0x814
};

} // namespace gmk

#endif
