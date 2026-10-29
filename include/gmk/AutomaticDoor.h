#ifndef GMK_AUTOMATICDOOR_H
#define GMK_AUTOMATICDOOR_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "atn/Texture_unk.h"
#include "game/GimmickCommon.h"
#include "gmk/WaterLevel.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

namespace gmk {

// inferred: motions and effects of a door state ("PARAM_CLOSE", "PARAM_OPEN", "PARAM_OPENHALF"
// with the suffixes "_MOTION" and "_EFF"); 0x30 bytes, names invented
struct DoorParam_unk {
    int mMotions[3];
    atn::EfpFile mEfp[3];
};

// inferred: fixed string of 0x20 chars (as in gmk::MotionObject; names invented)
struct DoorString_unk {
    DoorString_unk() { mStr[0] = 0; }
    void Set_unk(const char* s) {
        if (s != mStr) {
            memcpy(mStr, s, strlen(s) + 1);
        }
    }
    char mStr[0x20];
};

// inferred: door that opens and closes with motions; two collisions (closed and open), an optional
// projected texture ("PROJECT_EFFECT") and reflection plane. The name is in the DOL
// (__vt__Q23gmk13AutomaticDoor, 0x6F8 bytes).
class AutomaticDoor : public GimmickCommon {
public:
    AutomaticDoor(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_803F471C(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~AutomaticDoor();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void Start();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf64(int arg);
    virtual void vf70(int state);
    void fn_803F53E0(); // inferred: projects the texture on the model
    void fn_803F6448(int from, int to, int loop, float speed, float frame); // inferred: plays a motion
    virtual void vf3C(void* lens); // 0x803F69C8

    atn::Model mModel;             // 0xF4: "MODEL"
    atn::ColliObject mColliClose; // 0x4C4: "COLLISION_BEFORE"
    atn::ColliObject mColliOpen;  // 0x54C: "COLLISION_AFTER"
    float mCloseFrame;            // 0x5D4
    float mOpenFrame;             // 0x5D8
    int mHasClose;                // 0x5DC
    int mHasOpen;                 // 0x5E0
    int mPlaying;                 // 0x5E4
    int mOpen;                    // 0x5E8: inferred: returned by vf64(0)
    int mKnob;                    // 0x5EC: "KNOB_OFFSET" (vf64(9))
    int mReflect;                 // 0x5F0: "REFLECT_PLANE"
    int mReflectNode;             // 0x5F4
    Plane_unk mPlane;             // 0x5F8
    int mImmediate;               // 0x608: "COLLI_IMEDIATE"
    int mImmediateNow;            // 0x60C
    DoorParam_unk mParams[3];     // 0x610: close, open, open half
    atn::Texture_unk mTexture;    // 0x6A0: "PROJECT_EFFECT"
    int mProject;                 // 0x6D0
    DoorString_unk mBoundNode;    // 0x6D4: "BOUND_CENTER_NODE"
    int m6F4; // inferred: unused (the object is 0x6F8 bytes)
};

} // namespace gmk

#endif
