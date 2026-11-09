#ifndef ATN_EFFECTMANAGER_H
#define ATN_EFFECTMANAGER_H

#include "atn/RenderManager.h"

#include "atn/Task.h"
#include "atn/Model_unk.h"
#include "atn/Memory.h"
#include "atn/Array_unk.h"
#include "math/Vec.h"
#include "std/vector.h"
#include "atn/EffectSystem_unk.h"
#include "atn/EffectManagerCallback.h"

namespace atn {
struct Camera;
}
typedef atn::Camera Camera_unk;
namespace atn {
struct Model;
}
extern "C" char* strstr(const char* s, const char* sub);
extern "C" const char* fn_80473F18(void* name); // inferred: resource name

// inferred: current owner/group of the effects being created (statics of the effect system)
extern "C" unsigned int fn_80232B88();
extern "C" int fn_80232B90();
extern "C" void fn_80232B7C(unsigned int owner, int group);
extern "C" int fn_802346B4();
extern "C" void fn_802346AC(int handle);
struct EffectRes_unk;
extern "C" void fn_80232AC4(EffectRes_unk* res); // inferred: marks the resource as used



// inferred: resource of an efp file: emitters (0x54 bytes each)
struct EffectEmitter_unk {
    int m00;
    int m04;
    int m08;
    atn::Vector3 m0C;
    atn::Vector3 m18;
    atn::Vector3 m24;
    float m30;
    float m34;
    int m38;
    int m3C;
    int m40;
    int m44;
    unsigned char m48[0x54 - 0x48];
};
struct EffectEmitters_unk {
    unsigned int mCount;
    EffectEmitter_unk* mEmitters;
    int m08;
    int m0C;
    int mLength; // 0x10
};
// inferred: handle on a loaded efp resource
struct EffectRes_unk {
    EffectEmitters_unk* mData;
    unsigned char m04[4]; // inferred: resource name object
};

// inferred: one effect started by Play (0x64 bytes; names invented)
struct EffectEntry_unk {
    EffectEntry_unk() {}
    bool IsMatch_unk(unsigned int owner, int group) const { return mOwner == owner && (mGroup == group || group == -2); }

    EffectRes_unk* mRes;    // 0x00
    void* mTarget;          // 0x04: model, or node with flag 0x10000
    unsigned int mOwner;    // 0x08
    int mGroup;             // 0x0C
    unsigned int mFlags;    // 0x10
    int mFrame;             // 0x14
    atn::Vector3 mPos;      // 0x18
    atn::Vector3 mRot;      // 0x24
    float mScale;           // 0x30
    atn::Vector3 m34;
    int mHandle;            // 0x40
    short mEmitter;         // 0x44
    short m46;
    short mLayer;           // 0x48
    short m4A;
    EffectColor_unk mColor; // 0x4C
    float mRate;            // 0x5C
    float mTime;            // 0x60
};

namespace atn {


// inferred: plays the effects (efp resources) of the game
class EffectManager : public Task, public IRender {
public:
    EffectManager(Task* parent, int a, int b, int c, unsigned int max);
    virtual ~EffectManager();
    virtual void vf10();
    virtual void vf14();
    virtual void vf10(RenderManager* view);

    static EffectManager* fn_80239798(Task* parent, int a, int b, int c, unsigned int max);
    void fn_80239C14();
    void fn_80239C20();
    void fn_8023A184(unsigned int owner, int group, const EffectColor_unk* color);
    void fn_8023A1F4(unsigned int owner, int group, const atn::Vector3* vec);
    void fn_8023A254(unsigned int owner, int group, float rate);
    void fn_8023A2A8(void* owner);
    void fn_8023A340(EffectRes_unk* res);
    void fn_8023A5A8(int clear);
    void fn_8023A60C(int layer);
    inline void SetPaused_unk(int paused); // inferred: weak copy 0x80373084 (game/GameTask.cpp); name invented
    void fn_8023A614(int arg);
    void fn_8023A664(void* arg);
    void fn_8023A678(unsigned int max); // inferred: the argument goes to EffectSystem_unk::fn_8022BEAC (GameTask passes 0x80 or 0x120)
    void fn_8023AE34(unsigned int owner, int group, int arg);
    static void fn_8023B71C(EffectManager* mgr, int handle);
    int fn_8023B724(EffectEvent_unk* event);

    // inferred: removes an entry, returns the next one (names invented)
    EffectEntry_unk* Erase_unk(EffectEntry_unk* it) {
        if (it == mEntries.mData + mEntries.mSize) {
            return mEntries.mData + mEntries.mSize;
        }
        int index = it - mEntries.mData;
        for (unsigned int i = index; i < mEntries.mSize - 1; i++) {
            mEntries.mData[i] = mEntries.mData[i + 1];
        }
        mEntries.mSize = mEntries.mSize - 1;
        return mEntries.mData + index;
    }
    EffectEntry_unk* GetEntry_unk(unsigned int i) { return &mEntries.mData[i]; }
    // inferred: records a stop event for the replay (names invented)
    void NotifyStop_unk(void* res, unsigned int owner, int group, unsigned int flags, int flag, int handle,
                        int effect) {
        EffectManagerCallback* listener = mSystem.mListener;
        if (listener != 0 && mSystem.mReplaying == 0) {
            EffectEvent_unk event;
            event.mType = 1;
            event.mRes = res;
            event.mOwner = owner;
            event.mGroup = group;
            event.mFlags = flags;
            event.m5C = flag;
            event.m58 = handle;
            event.mHandle = effect;
            event.mLayer = mSystem.mLayer;
            listener->vf08(&event);
        }
    }
    // inferred: stops one emitter of an entry (names invented)
    void StopEmitter_unk(int handle, int flag) {
        mSystem.fn_802347DC(handle, flag);
        NotifyStop_unk(0, 0, 0, 0, flag, handle, 0);
    }
    // inferred: stops an effect started by fn_8023A680 (names invented)
    void StopEffect_unk(void* effect, int flag) {
        mSystem.fn_80234958(effect, flag);
        NotifyStop_unk(0, 0, 0, 0, flag, 0, (int)effect);
    }
    EffectEntry_unk* End_unk() { return mEntries.mData + mEntries.mSize; }
    bool IsMatch_unk(const EffectEntry_unk& e, unsigned int owner, int group) const {
        return e.mOwner == owner && (e.mGroup == group || group == -2);
    }

    EffectSystem_unk mSystem;                // 0x4C
    unsigned int mMax;                       // 0xD4
    SimpleVector<EffectEntry_unk> mEntries;    // 0xD8
    unsigned int mSerial;                    // 0xE4
    int mPaused;                             // 0xE8
    unsigned int mFlags;                     // 0xEC
    int mCheckTrace;                         // 0xF0
    void* mDrawArg;                          // 0xF4
};

} // namespace atn

extern "C" atn::EffectManager* lbl_8087F3C0; // inferred: instance
extern "C" void* fn_8023A680(atn::EffectManager* mgr, EffectRes_unk* res, int a, int b, int c, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, int d, int e, int f, float scale);
extern "C" void fn_8023A02C(atn::EffectManager* mgr, unsigned int owner, int group, unsigned int flags);
extern "C" void fn_80239DAC(atn::EffectManager* mgr, unsigned int owner, int group, int flag);

#endif
