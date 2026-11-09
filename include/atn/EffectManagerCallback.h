#ifndef ATN_EFFECTMANAGERCALLBACK_H
#define ATN_EFFECTMANAGERCALLBACK_H

#include "math/Vec.h"
#include "atn/EffectSystem_unk.h"

// inferred: event sent to the effect listener (0x64 bytes; names invented)
struct EffectEvent_unk {
    int mType;              // 0x00: 0 play, 1 stop
    void* mRes;             // 0x04
    void* mModel;           // 0x08
    int m0C;
    int m10;
    atn::Vector3 mPos;      // 0x14
    atn::Vector3 mRot;      // 0x20
    float mScale;           // 0x2C
    EffectColor_unk mColor; // 0x30
    int m40;
    int m44;
    int mHandle;            // 0x48
    unsigned int mOwner;    // 0x4C
    int mGroup;             // 0x50
    unsigned int mFlags;    // 0x54
    int m58;
    int m5C;
    int mLayer;             // 0x60
};

namespace atn {
// RTTI name (vtable 0x80795834): receives the effect events (the replay, ReplayBuffer)
class EffectManagerCallback {
public:
    virtual void vf08(EffectEvent_unk* event) = 0;
};
} // namespace atn

#endif
