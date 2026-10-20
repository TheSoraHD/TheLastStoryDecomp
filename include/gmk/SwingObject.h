#ifndef GMK_SWINGOBJECT_H
#define GMK_SWINGOBJECT_H

#include "atn/EffectModel_unk.h"
#include "atn/EfpHandle_unk.h"
#include "game/GimmickCommon.h"

extern "C" int rand();

namespace atn {
struct NodeChunk;
}

namespace gmk {

// inferred: "PARAM" line of a swinging object (debug menu names); 0x24 bytes
struct SwingParam_unk {
    SwingParam_unk() : mWindVec(0.0f), mTwist(0.5f), mWindRand(0.0f) {}
    float mWindPower; // "wind_power"
    float mWindVec;   // "wind_vec"
    float mDegMin;    // "degree_min"
    float mDegMax;    // "degree_max"
    float mMiddle;    // "middle/bottom (%)"
    float mLag;       // "node_time_lag"
    float mSpeed;     // "time_speed"
    float mTwist;     // "twist_power"
    float mWindRand;  // "wind_power_rand"
};

// inferred: swing state shared by gmk::SwingObject and the swinging map parts (names invented)
struct SwingCore_unk {
    SwingCore_unk() : mMaxWind(45.0f), mWind(0.0f) {
        mPhase = (float)(rand() % 5 + 1);
        mAngle1 = 0.0f;
        mAngle2 = 0.0f;
        mTwistAngle = 0.0f;
        mTime = (float)(rand() % 20 + 1) / 10.0f;
        mAxis1 = 2;
        mAxis2 = 1;
        mAngle0 = 0.0f;
    }

    SwingParam_unk mParams[2];  // 0x00: normal and second state
    SwingParam_unk* mCur;       // 0x48
    int mNodes[3];              // 0x4C: "NODE" (hashes)
    float mMaxWind;             // 0x58
    float mWind;                // 0x5C
    float mPhase;               // 0x60
    float mAngle1;              // 0x64
    float mAngle2;              // 0x68
    float mTwistAngle;          // 0x6C
    float mTime;                // 0x70
    int mAxis1;                 // 0x74: "AXIS" 1 ("axis1")
    int mAxis2;                 // 0x78: "AXIS" 2
    float mAngle0;              // 0x7C
};

// inferred: object that swings in the wind (flags, lanterns): a model whose three nodes ("NODE") are
// turned by a node callback. The name is in the DOL (__vt__Q23gmk11SwingObject, 0x3A8 bytes).
class SwingObject : public GimmickCommon {
public:
    SwingObject(atn::Task* parent, int id, int arg) : GimmickCommon(parent, id, arg), mModel(1), mHidden(false) {
        m54 = 0;
    }
    static GimmickCommon* fn_803FE390(atn::Task* parent, int id, int arg);           // inferred: factory
    static GimmickCommon* fn_803FE570(atn::Task* parent, GimmickDesc_unk* desc);    // inferred: factory
    virtual ~SwingObject();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual void vf24();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf6C(void* menu);
    void fn_803FEF78(atn::Matrix3x4* mtx, const atn::NodeChunk& node); // inferred: node callback
    void fn_803FFC50(); // inferred: selects the parameters of the state
    virtual void vf3C(void* lens); // 0x80400714

    EffectModel_unk mModel;     // 0xF4: "MODEL"
    MtxPtr mNodeMtx[3];         // 0x308
    SwingCore_unk mCore;        // 0x314
    bool mHidden;               // 0x394
    atn::EfpHandle_unk mEfp;    // 0x398: "EFP"
    int m3A4; // inferred: unused (the object is 0x3A8 bytes)
};

} // namespace gmk

#endif
