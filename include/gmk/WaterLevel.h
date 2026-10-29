#ifndef GMK_WATERLEVEL_H
#define GMK_WATERLEVEL_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"

// inferred: plane (normal and distance)
struct Plane_unk {
    atn::Vector3 mNormal;
    float mDist;
};

namespace gmk {

// inferred: water surface: a collision box ("COLLI_BOX") that follows a node of the model and an
// optional reflection plane ("REFLECT_PLANE" normal, distance and node) given to the camera. The
// name is in the DOL (__vt__Q23gmk10WaterLevel, 0x890 bytes).
class WaterLevel : public GimmickCommon {
public:
    WaterLevel(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mModel(8, 32), mBoxOffset(0.0f, 0.0f, 0.0f), mBoxRot(0.0f, 0.0f, 0.0f),
          mBoxSize(0.0f, 0.0f, 0.0f), mReflect(0), mNode(-1) {
        m54 = 0;
    }
    static GimmickCommon* fn_8042E36C(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~WaterLevel();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual atn::Model* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    atn::Model mModel;          // 0xF4
    atn::Vector3 mBoxPos;      // 0x4C4
    atn::ColliBox mColli;      // 0x4D0
    atn::Vector3 mBoxOffset;   // 0x848: "COLLI_BOX"
    atn::Vector3 mBoxRot;      // 0x854
    atn::Vector3 mBoxSize;     // 0x860
    atn::EfpHandle_unk mEfp;   // 0x86C: "EFP"
    int mReflect;              // 0x878
    int mNode;                 // 0x87C: inferred: node of the reflection plane
    Plane_unk mPlane;          // 0x880: "REFLECT_PLANE"
};

} // namespace gmk

#endif
