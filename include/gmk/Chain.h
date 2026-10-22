#ifndef GMK_CHAIN_H
#define GMK_CHAIN_H

#include "atn/EffectModel_unk.h"
#include "game/CharaTask.h"
#include "game/GimmickCommon.h"

namespace gmk {

// inferred: joint of the chain model (0x5C bytes, built with new[] from the "joint%d" nodes)
struct ChainJoint_unk {
    ChainJoint_unk() : mNode(0), m04(0.0f, 0.0f, 0.0f), m10(0.0f, 0.0f, 0.0f), m4C(0.0f, 0.0f, 0.0f) {}
    int mNode;          // inferred: node index
    atn::Vector3 m04;
    atn::Vector3 m10;
    atn::Matrix3x4 m1C; // inferred: node matrix
    atn::Vector3 m4C;
    float m58;          // inferred: length to the next joint
};

// inferred: array allocated with new[] (heap 3) that is replaced by a new one (name invented)
struct ChainJointArray_unk {
    ChainJointArray_unk() : mData(0) {}
    ~ChainJointArray_unk() { delete[] mData; }
    ChainJoint_unk* mData;
};

// inferred: chain model ("it087_00.model") hanging from a node of a character (mDesc->m20 kind,
// m24 id, m28 node). The name is in the DOL (__vt__Q23gmk5Chain, 0x348 bytes).
class Chain : public GimmickCommon {
public:
    Chain(atn::Task* parent, GimmickDesc_unk* desc);
    static GimmickCommon* fn_8043B778(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory (returns 0)
    virtual ~Chain();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf3C(void* lens);
    virtual int vf40(GimmickMsg_unk* msg);

    void fn_8043B9E0(); // inferred: builds the joints; not decompiled yet
    void fn_8043C508(); // inferred: update
    void fn_8043CA58(); // inferred: relaxes the joints (20 passes of length constraints)
    void fn_8043CF58(); // inferred: orients the joints along the chain

    GimmickDesc_unk* mDesc; // 0xF4
    EffectModel_unk mModel; // 0xF8
    CharaTask* mTarget;     // 0x30C
    int mNode;              // 0x310: inferred: node of the target
    atn::Vector3 m314;      // inferred: position of the first joint (the node of the target)
    atn::Vector3 m320;      // inferred: position of the last joint
    ChainJointArray_unk mJoints;  // 0x32C
    ChainJointArray_unk mJoints2; // 0x330
    int mNumJoints;         // 0x334
    float mLength;          // 0x338
    float m33C;
    float m340;             // inferred: 0.98 (damping?)
    int m344;
};

} // namespace gmk

#endif
