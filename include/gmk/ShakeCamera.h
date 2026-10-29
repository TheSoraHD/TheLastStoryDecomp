#ifndef GMK_SHAKECAMERA_H
#define GMK_SHAKECAMERA_H

#include "game/GimmickCommon.h"

namespace gmk {

// inferred: shakes the camera at random intervals and plays a sound. The member names come
// from the debug menu (vf6C). The name is in the DOL (__vt__Q23gmk11ShakeCamera).
class ShakeCamera : public GimmickCommon {
public:
    ShakeCamera(atn::Task* parent, GimmickDesc_unk* desc) : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc) {
        m54 = 0;
        m68 = 1;
    }
    static GimmickCommon* fn_80421454(atn::Task* parent, GimmickDesc_unk* desc); // inferred: factory
    virtual ~ShakeCamera() {}
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void Start();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf60();
    virtual void vf6C(void* menu);

    GimmickDesc_unk* mDesc; // 0xF4
    int mWait;              // 0xF8: inferred: frames until the next shake
    float mShakeX;          // 0xFC
    float mShakeY;          // 0x100
    int mShakeFrame;        // 0x104
    int mWaitFrame;         // 0x108
    int mWaitFrameMin;      // 0x10C
    float mRateMax;         // 0x110
    float mRateMin;         // 0x114
};

} // namespace gmk

#endif
