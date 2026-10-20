#ifndef GMK_MOTIONOBJECT_H
#define GMK_MOTIONOBJECT_H

#include "atn/Array_unk.h"
#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

class CharaTask;

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

namespace gmk {

// inferred: fixed string of 0x20 chars (names invented)
struct String32_unk {
    String32_unk() { mStr[0] = 0; }
    void Set_unk(const char* s) {
        if (s != mStr) {
            memcpy(mStr, s, strlen(s) + 1);
        }
    }
    char mStr[0x20];
};

// inferred: "STATE_SETTING" of a state: message sent when the state ends and its length in frames
// (names invented)
struct MotionState_unk {
    MotionState_unk() : mNext(-1), mFrames(0) {}
    int mNext;
    int mFrames;
};

// inferred: "MOTION" of a model (0x14 bytes; names invented)
struct MotionPlay_unk {
    MotionPlay_unk() : mMotion(-1), mBlend(0.0f), mLoop(false), mReverse(false), mFrame(0.0f), mSpeed(1.0f) {}
    int mMotion;
    float mBlend;
    bool mLoop;
    bool mReverse;
    float mFrame;
    float mSpeed;
};

// inferred: "TARGET_COLLI": a sphere placed at an offset turned by the gimmick rotation (0x5C bytes;
// the vtable is the one of atn::ColliSphere; names invented)
struct MotionTarget_unk {
    atn::ColliSphere mSphere;
    atn::Vector3 mOffset; // 0x4C
    float mRadius;        // 0x58
};

// inferred: collisions of a model (names invented)
struct MotionColli_unk {
    MotionColli_unk() : mObject(0), mNode(0), mTarget(0) {}
    ~MotionColli_unk() {
        if (mObject != 0) {
            delete mObject;
            mObject = 0;
        }
        if (mTarget != 0) {
            delete mTarget;
            mTarget = 0;
        }
    }
    atn::ColliObject* mObject; // "COLLI_OBJECT"
    MtxPtr mNode;              // "BOUND_CENTER_NODE" of "PLAYER_ACTION"
    MotionTarget_unk* mTarget; // "TARGET_COLLI"
};

// inferred: "EFP" of a state with its flags (1 EFF_CLEAR_IMMEDIATE, 2 EFF_REFLECT; names invented)
struct MotionEfp_unk {
    MotionEfp_unk() : mFlags(0) {}
    atn::EfpHandle_unk mEfp;
    unsigned int mFlags;
};

// inferred: "PLAYER_ACTION": the object can be opened (0x24 bytes; names invented)
struct MotionAction_unk {
    MotionAction_unk() : mEnabled(0) {}
    int mEnabled;
    float m04[4];
    atn::Vector3 mPos;   // 0x14: inferred: position of the character (model space)
    float mRotY;         // 0x20
};

// inferred: object with up to eight models, one per state, each with its motion, collisions and
// effects. The name is in the DOL (__vt__Q23gmk12MotionObject, 0x2348 bytes).
class MotionObject : public GimmickCommon {
public:
    MotionObject(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_80412AFC(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~MotionObject();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf44(GimmickHitMsg_unk* msg);
    virtual int vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag);
    virtual int vf50();
    virtual atn::Vector3 vf54();
    virtual float vf58();
    virtual int vf5C();
    virtual void vf70(int state);
    void fn_804137F4(); // inferred: places the collisions
    void fn_804139D8(); // inferred: updates the material fades and node switches
    virtual void vf4C(CharaTask* chara); // 0x8041482C

    int mPrevState;                   // 0xF4
    int mFrame;                       // 0xF8
    Model_unk mModels[8];             // 0xFC
    int mHasModel[8];                 // 0x1F7C
    MotionPlay_unk mPlay[8];          // 0x1F9C
    MotionColli_unk mColli[8];        // 0x203C
    MotionEfp_unk mEfp[8];            // 0x209C
    MotionState_unk mStates[8];       // 0x211C
    int mStateDone;                   // 0x215C
    MotionAction_unk mActions[8];     // 0x2160
    int mInitState;                   // 0x2280: "INIT_STATE"
    String32_unk mBoundNode;          // 0x2284: "BOUND_CENTER_NODE"
    int mActionEnabled[8];            // 0x22A4
    atn::Buffer_unk<GimmickMaterialFade_unk> mFades[8];    // 0x22C4: "MATALPHA"
    atn::Buffer_unk<GimmickMotionSwitch_unk> mSwitches[8]; // 0x2304: "NODEVIS"
    int m2344; // inferred: unused (the object is 0x2348 bytes)
};

} // namespace gmk

#endif
