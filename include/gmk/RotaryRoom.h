#ifndef GMK_ROTARYROOM_H
#define GMK_ROTARYROOM_H

#include "atn/Array_unk.h"
#include "atn/ColliBase.h"
#include "game/GimmickCommon.h"

struct RotaryGroup_unk;

namespace gmk {

// inferred: rotated object (0x50 bytes, built with new[] in vf0C; names invented)
struct RotaryEntry_unk {
    RotaryEntry_unk() : mType(0), m08(0.0f, 0.0f, 0.0f), m14(0.0f, 0.0f, 0.0f) { mMtx.SetIdentity(); }
    int mType;
    void* mObject;
    atn::Vector3 m08;
    atn::Vector3 m14;
    atn::Matrix3x4 mMtx; // 0x20
};

// inferred: collision of one state ("STATE": index, path); names invented
struct RotaryColli_unk {
    RotaryColli_unk() : mTree(0), mHash(0), m08(0), m0C(false) {}
    atn::ColliTree* mTree;
    unsigned int mHash;
    int m08;
    bool m0C;
};

// inferred: room that rotates its map group ("MAP") between 24 orientations; the state picks
// the orientation (1-24) or rotates by a quarter or half turn (1000-1003). The name is in the DOL
// (__vt__Q23gmk10RotaryRoom, 0x2B0 bytes).
class RotaryRoom : public GimmickCommon {
public:
    RotaryRoom(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mGroup(0), mRot(0.0f, 0.0f, 0.0f), mRotOffset(0.0f, 0.0f, 0.0f),
          mFrames(180), mFrame(0) {
        m54 = 0;
    }
    static GimmickCommon* fn_80423F64(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~RotaryRoom();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual void vf6C(void* menu);
    virtual void vf70(int state);

    void fn_80424AD4(); // not decompiled yet
    void fn_80424FB4(float t); // inferred: places the room at t of the rotation; not decompiled yet
    void fn_80425498(); // not decompiled yet
    void fn_80425878(int on); // inferred: switches the parts of type 5

    RotaryGroup_unk* mGroup;  // 0xF4
    atn::Vector3 mRot;        // 0xF8
    atn::Vector3 mRotOffset;  // 0x104
    int mFrames;              // 0x110: "rotOffsetFrame_"
    int mFrame;               // 0x114
    atn::Buffer_unk<RotaryEntry_unk> mEntries; // 0x118
    RotaryColli_unk mColli[25]; // 0x120
};

} // namespace gmk

#endif
