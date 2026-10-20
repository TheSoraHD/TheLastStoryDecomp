#ifndef GMK_FIREWORKS_H
#define GMK_FIREWORKS_H

#include "atn/EfpHandle_unk.h"
#include "gmk/CharaLight.h"
#include "game/MotionCmdElemD_unk.h"

namespace gmk {

// inferred: fireworks launched at random around the gimmick; each explosion plays an effect and a
// sound and flashes the light of the room. The name is in the DOL (__vt__Q23gmk9Fireworks,
// 0x330 bytes).
class Fireworks : public GimmickCommon {
public:
    // inferred: flash of the light ("EFPLIGHT": delay, frames, size)
    struct Flash_unk {
        Flash_unk() : mDelay(0), mFrames(30), mSize(2500.0f) {}
        int mDelay;
        int mFrames;
        float mSize;
    };
    // inferred: fixed vector of the active flashes (names invented)
    struct FlashList_unk {
        FlashList_unk() : mCount(0) {}
        void Erase_unk(Flash_unk* it) {
            for (unsigned int i = it - mData; i < mCount - 1; i++) {
                mData[i] = mData[i + 1];
            }
            mCount = mCount - 1;
        }
        unsigned int Size_unk() const { return mCount; }
        Flash_unk& operator[](int i) { return mData[i]; }
        Flash_unk* End_unk() { return &mData[mCount]; }
        void PushBack_unk(const Flash_unk& flash) {
            if (Size_unk() < 8) {
                new (End_unk()) Flash_unk(flash);
                mCount++;
            }
        }
        unsigned int mCount;
        Flash_unk mData[8];
    };

    Fireworks(atn::Task* parent, int id, int arg);
    static GimmickCommon* fn_8041CD54(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~Fireworks();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void vf28();
    virtual int vf40(GimmickMsg_unk* msg);

    atn::EfpHandle_unk mEfp[6];   // 0xF4: "EFP"
    MotionCmdElemD_unk mSe[6];    // 0x13C: "SE": inferred: sound names
    atn::Vector3 mOffset;         // 0x1FC: "EXPLODE"
    atn::Vector3 mRange;          // 0x208: "EXPLODE"
    int mInterval;                // 0x214: "EXPLODE": inferred: frames before the explosion
    int mTimer;                   // 0x218
    int mLimit;                   // 0x21C: "LAUNCH": inferred: number of launches (<= 0: no limit)
    int mCount;                   // 0x220
    int mWait;                    // 0x224: "LAUNCH": inferred: frames between launches
    int mWaitRand;                // 0x228: "LAUNCH"
    int mWaitTimer;               // 0x22C
    Flash_unk mPresets[6];        // 0x230: "EFPLIGHT"
    FlashList_unk mFlashes;       // 0x278
    CharaLightData_unk mLight;    // 0x2DC
    int m32C;                     // inferred: unused
};

} // namespace gmk

#endif
