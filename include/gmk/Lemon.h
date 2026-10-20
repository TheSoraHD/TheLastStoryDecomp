#ifndef GMK_LEMON_H
#define GMK_LEMON_H

#include "game/GimmickCommon.h"

// inferred: shared data of a model instance (atn/ModelInstance.cpp); only what is used here
struct InstData_unk {
    unsigned char m00[0xBC];
    float mBC; // inferred: set to 2.0f by gmk::Lemon
};

namespace atn {

// inferred: view of atn::ModelInstance (the class is defined in atn/ModelInstance.cpp); 0x88 bytes
class ModelInstance {
public:
    ModelInstance(InstData_unk* data);
    virtual ~ModelInstance(); // inferred: virtual (gmk::Leaf deletes its instances through the vtable)
    void fn_8009E6EC(int arg); // inferred: culling and queueing (callers pass 0 or 1 in r4)
    void fn_8009EB20();        // inferred: LOD selection
    void fn_8009EE30(const Matrix3x4& mtx); // inferred: sets the matrix

    InstData_unk* mData; // 0x04
    unsigned char m08[0x30 - 0x08];
    Matrix3x4 mMtx;      // 0x30
    unsigned char m60[0x64 - 0x60];
    void* m64; // inferred: screen data
    unsigned char m68[0x88 - 0x68];
};

} // namespace atn

// inferred: result of the collision tests of lbl_8087EE98 (0x50 bytes, see game/CharaTask.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position after the sweep
    atn::Vector3 m1C; // inferred: normal
    atn::Vector3 m28; // inferred: normal of the hit (gmk::Cannonball)
    void* m34;
    void* m38;
    unsigned int m3C;
    int m40;
    atn::Vector3 m44;
};

namespace gmk {

// inferred: building model (gmXXX_XXX.building) that is knocked away by message 3 and bounces on
// the collision. The name is in the DOL (__vt__Q23gmk5Lemon).
class Lemon : public GimmickCommon {
public:
    Lemon(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_8041524C(atn::Task* parent, int id, int arg); // inferred: factory
    static GimmickCommon* fn_8041537C(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~Lemon();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);
    void fn_80415F6C(); // inferred: moves while flying
    int fn_80416268(CollResult_unk* out, const atn::Vector3* pos, const atn::Vector3* vel, float radius);

    atn::ModelInstance mInst; // 0xF4
    atn::Vector3 mVel;        // 0x17C
    float mRadius;            // 0x188: inferred: "PARAM" 1
    float mBounce;            // 0x18C: inferred: "PARAM" 2
    float mFriction;          // 0x190: inferred: "PARAM" 3
    int m194;                 // inferred: "PARAM" 4
    int m198;
    int m19C;                 // inferred: unused (the object is 0x1A0 bytes)
};

} // namespace gmk

#endif
