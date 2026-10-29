#ifndef GMK_LEAF_H
#define GMK_LEAF_H

#include "atn/Array_unk.h"
#include "game/GimmickCommon.h"
#include "gmk/Lemon.h"

class CharaTask;

// inferred: one leaf of gmk::Leaf (0x30 bytes); names invented
struct LeafEntry_unk {
    LeafEntry_unk() : mInst(0) {}
    atn::ModelInstance* mInst; // 0x00
    atn::Vector3 mPos;         // 0x04: inferred: offset from the gimmick position
    atn::Vector3 mRot;         // 0x10
    float mDirX;               // 0x1C: inferred: push direction
    float mDirZ;               // 0x20
    float mSpeed;              // 0x24
    unsigned char m28[0x30 - 0x28];
};

namespace gmk {

// inferred: leaves blown by the wind ("PARAM": type, number of leaves, wind speed, flow length and
// size; "MODEL_INSTANCE": model of a leaf). Type 0 and 1 blow along "winddir", type 2 lets the
// characters push them. The name is in the DOL (__vt__Q23gmk4Leaf).
class Leaf : public GimmickCommon {
public:
    Leaf(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mWindDir(0.0f, 0.0f, 1.0f), mWindVel(0.3f), mWindCur(0.3f),
          mFlowLength(180.0f), mNumLeaf(10), m118(0), m120(0.0f), m124(0.0f), m128(0) {
        mState = 0;
    }
    static GimmickCommon* fn_80400728(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~Leaf();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf20();
    virtual float vf58();
    virtual void vf6C(void* menu);
    void fn_80400D08();                  // inferred: places the leaves
    void fn_8040161C();                  // inferred: state of type 0: blown by the wind
    void fn_80401BF0();                  // inferred: state of type 2: pushed by the characters
    void fn_80401E50(CharaTask* chara); // inferred: pushes the leaves near a character
    void fn_804023E0();                  // inferred: moves the pushed leaves
    void fn_80402764();                  // inferred: state of type 1

    unsigned int mState;       // 0xF4: inferred: 0 until the leaves are placed
    int mType;                 // 0xF8: "PARAM" 1
    atn::Vector3 mWindDir;     // 0xFC: "winddir"
    float mWindVel;            // 0x108: "PARAM" 3
    float mWindCur;            // 0x10C: inferred: current wind speed
    float mFlowLength;         // 0x110: "PARAM" 4
    int mNumLeaf;              // 0x114: "PARAM" 2
    unsigned char m118;
    float mSize;               // 0x11C: "PARAM" 5
    float m120;                // inferred: gust speed
    float m124;                // inferred: gust target
    int m128;                  // inferred: gust frames
    atn::SimpleArray<LeafEntry_unk> mLeaves; // 0x12C
    int m134;                  // inferred: unused (the object is 0x138 bytes)
};

} // namespace gmk

#endif
