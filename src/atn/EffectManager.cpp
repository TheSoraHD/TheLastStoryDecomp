#include "atn/Task.h"
#include "atn/Memory.h"
#include "atn/Array_unk.h"
#include "math/Vec.h"
#include "std/vector.h"

// The real file (0x80239798-0x8024373C, fused with the former std/char_traits_char_8023C080 and
// atn/EffectManager_cont units) goes on after 0x8023B9D0 with the std:: template code it instantiates
// (iostream facets, up to 0x802411B8) and the EfpInfoList loader, and ends with the this-adjusting
// thunks of EffectManager for its IRender base (0x8024372C/0x80243734, emitted by MWCC at the end).

struct Camera_unk;
namespace atn {
struct Model;
}
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void fn_800BDB58(Camera_unk* camera, void* render, float alpha, int layer); // inferred: queues a render
extern "C" int fn_800928B0(void* nodes, int hash, int arg);                          // inferred: node index by hash
extern "C" char* strstr(const char* s, const char* sub);
extern "C" const char* fn_80473F18(void* name); // inferred: resource name
// inferred: render parameters (fields named by offset)
struct EffectParams_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
extern "C" EffectParams_unk* lbl_8087EFA8;

// inferred: current owner/group of the effects being created (statics of the effect system)
extern "C" unsigned int fn_80232B88();
extern "C" int fn_80232B90();
extern "C" void fn_80232B7C(unsigned int owner, int group);
extern "C" int fn_802346B4();
extern "C" void fn_802346AC(int handle);
struct EffectRes_unk;
extern "C" void fn_80232AC4(EffectRes_unk* res); // inferred: marks the resource as used

// inferred: RGBA color
struct EffectColor_unk {
    EffectColor_unk() {}
    EffectColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

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

// inferred: receives the effect events (replay)
class EffectListener_unk {
public:
    virtual void vf08(EffectEvent_unk* event) = 0;
};

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

// inferred: the effect system kept by the manager (0x88 bytes; its members are in other files)
// inferred: two empty levels over std::vector (five null checks of `this` in the destructor)
struct EffectPtrVectorBase_unk : public std::vector<void*> {};
struct EffectPtrVector_unk : public EffectPtrVectorBase_unk {};
// inferred: element with a constructor (its new[] has a cookie)
struct EffectSystemElem_unk {
    EffectSystemElem_unk();
    unsigned char m00[4];
};
struct EffectSystemPool_unk {
    EffectSystemPool_unk() : m00(0), m04(0) {}
    int m00;
    int m04;
    atn::SimpleArray<EffectSystemElem_unk> m08;
    EffectPtrVector_unk m10;
};

class EffectSystem_unk {
public:
    EffectSystem_unk()
        : m00(0), m04(0), m08(0), mEnabled(0), m10(0), m14(0), m18(0), m1C(0), m20(0), m24(0), m28(0), m48(0.0f),
          m4C(0), m50(0), m54(0), m58(0), m5C(0), m60(0), mFlags(0), m68(0), mLayer(0), m70(0), mUpdating(0),
          m78(0), m7C(0), mListener(0), mReplaying(0) {}
    void fn_8022B850(int a, int b, int c); // inferred: init
    void fn_8022BEAC();
    void fn_8022C2A0();                    // inferred: release
    void fn_8022C3F0();                    // inferred: update
    void* fn_8022FAC4(void* res, int a, int b, int c, const atn::Vector3* pos, const atn::Vector3* rot,
                      const EffectColor_unk* color, int d, int e, float scale);
    int fn_80232ACC(void* owner);
    int fn_80232B98(unsigned int owner, int group);
    void fn_80232BEC(unsigned int owner, int group, int count);
    int fn_80232D04(unsigned int owner, int group, int flag);
    void fn_80232E48();
    void fn_80232ED4();
    void fn_80232F84(int arg);
    void fn_80232FF4(unsigned int owner, int group, unsigned int flags);
    void fn_802330A4(unsigned int owner, int group, unsigned int flags);
    void fn_80233158(unsigned int owner, int group, const EffectColor_unk* color);
    void fn_802331B0(unsigned int owner, int group, const atn::Vector3* vec);
    void fn_80233260(unsigned int owner, int group, float rate);
    int fn_802346BC(int handle);
    void fn_802346F0(int handle, unsigned int flags);
    void fn_80234764(int handle, const atn::Vector3* vec);
    void fn_802347DC(int handle, int arg);
    void fn_802348E4(void* effect, unsigned int flags);
    void fn_80234958(void* effect, int flag);
    void fn_8023686C(float rate);
    void fn_802368B0(void* arg);

    int m00;
    int m04;
    int m08;
    int mEnabled; // 0x0C
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    int m28;
    EffectSystemPool_unk m2C;
    float m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    int m5C;
    int m60;
    unsigned int mFlags;          // 0x64
    int m68;
    int mLayer;                   // 0x6C
    int m70;
    int mUpdating;                // 0x74
    int m78;
    int m7C;
    EffectListener_unk* mListener; // 0x80
    int mReplaying;               // 0x84
};

namespace atn {

// inferred: renderer interface (see atn/AABBTree.cpp)
class IRender {
public:
    virtual ~IRender() {}
    virtual int vf0C() { return 2; }
    virtual void vf10(Camera_unk* camera) = 0;
    virtual int vf14(void* arg1, void* arg2, void* frustum) { return 1; }
    virtual void vf18() {}
    virtual void vf1C() {}
};

// inferred: plays the effects (efp resources) of the game
class EffectManager : public Task, public IRender {
public:
    EffectManager(Task* parent, int a, int b, int c, unsigned int max);
    virtual ~EffectManager();
    virtual void vf10();
    virtual void vf14();
    virtual void vf10(Camera_unk* camera);

    static EffectManager* fn_80239798(Task* parent, int a, int b, int c, unsigned int max);
    void fn_80239C14();
    void fn_80239C20();
    void fn_8023A184(unsigned int owner, int group, const EffectColor_unk* color);
    void fn_8023A1F4(unsigned int owner, int group, const atn::Vector3* vec);
    void fn_8023A254(unsigned int owner, int group, float rate);
    void fn_8023A2A8(void* owner);
    void fn_8023A340(EffectRes_unk* res);
    void fn_8023A5A8(int clear);
    void fn_8023A60C();
    void fn_8023A614(int arg);
    void fn_8023A664(void* arg);
    void fn_8023A678();
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
        EffectListener_unk* listener = mSystem.mListener;
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

namespace atn {

EffectManager* EffectManager::fn_80239798(Task* parent, int a, int b, int c, unsigned int max) {
    if (lbl_8087F3C0 == 0 && parent != 0) {
        lbl_8087F3C0 = new ((MemoryTag)7, "", "", 0) EffectManager(parent, a, b, c, max);
    }
    return lbl_8087F3C0;
}

EffectManager::EffectManager(Task* parent, int a, int b, int c, unsigned int max)
    : Task(parent), mSerial(1), mPaused(0), mFlags(0), mCheckTrace(0), mDrawArg(0) {
    ((void)0); // inferred: keeps the constructor out of fn_80239798, as in the original
    mSystem.fn_8022B850(a, b, c);
    mMax = max;
    mEntries.reserve(max, 7);
}

EffectManager::~EffectManager() {
    mSystem.fn_8022C2A0();
    lbl_8087F3C0 = 0;
}

void EffectManager::vf10() {
    fn_8023AE34(0, 0, -1);
    mSystem.fn_8023686C(1.0f);
}

void EffectManager::fn_80239C14() {
    mSystem.fn_8023686C(1.0f);
}

void EffectManager::fn_80239C20() {
    if (mSystem.mUpdating == 0) {
        mSystem.mUpdating = 1;
        mSystem.fn_8022C3F0();
        mSystem.mUpdating = 0;
    }
}

void EffectManager::vf14() {
    mSystem.fn_8022C3F0();
    fn_800BDB58(lbl_8087EFB4, static_cast<IRender*>(this), 0.0f, 0xB);
    if ((mSystem.mFlags & 1) == 1) {
        fn_800BDB58(lbl_8087EFB4, static_cast<IRender*>(this), 0.0f, 6);
    }
    if ((mSystem.mFlags & 8) == 8) {
        fn_800BDB58(lbl_8087EFB4, static_cast<IRender*>(this), -10000000.0f, 4);
    }
}

void EffectManager::vf10(Camera_unk* camera) {
    mSystem.fn_802368B0(mDrawArg);
    mDrawArg = 0;
}

} // namespace atn

using atn::EffectManager;

extern "C" int fn_80239D58(EffectManager* mgr, unsigned int owner, int group) {
    for (EffectEntry_unk* it = mgr->mEntries.mData; it != mgr->mEntries.mData + mgr->mEntries.mSize; it++) {
        if (it->mOwner == owner && (it->mGroup == group || group == -2)) {
            return 1;
        }
    }
    return mgr->mSystem.fn_80232B98(owner, group);
}

// NONMATCHING: 97% - the original reloads the end of the array in the erase (ours reuses the loop test)
extern "C" void fn_80239DAC(EffectManager* mgr, unsigned int owner, int group, int flag) {
    int found = 0;
    EffectEntry_unk* it = mgr->mEntries.mData;
    while (it != mgr->mEntries.mData + mgr->mEntries.mSize) {
        if (it->mOwner == owner && (it->mGroup == group || group == -2)) {
            if (flag == 0) {
                it = mgr->Erase_unk(it);
            } else {
                it->mFrame = -1;
                it++;
            }
            found = 1;
        } else {
            it++;
        }
    }
    if (mgr->mSystem.mEnabled != 0) {
        int r = mgr->mSystem.fn_80232D04(owner, group, flag);
        found = (found != 0 || r > 0) ? 1 : 0;
    }
    EffectListener_unk* listener = mgr->mSystem.mListener;
    if (listener != 0 && mgr->mSystem.mReplaying == 0 && found != 0) {
        EffectEvent_unk event;
        event.mType = 1;
        event.mRes = 0;
        event.mOwner = owner;
        event.mGroup = group;
        event.mFlags = 0;
        event.m5C = flag;
        event.m58 = 0;
        event.mHandle = 0;
        event.mLayer = mgr->mSystem.mLayer;
        listener->vf08(&event);
    }
}

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
extern "C" void fn_8023A02C(EffectManager* mgr, unsigned int owner, int group, unsigned int flags) {
    EffectEntry_unk* e;
    for (int i = 0; i < mgr->mEntries.mSize; i++) {
        e = &mgr->mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mFlags |= flags;
        }
    }
    if (mgr->mSystem.mEnabled != 0) {
        mgr->mSystem.fn_80232FF4(owner, group, flags);
    }
}

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
extern "C" void fn_8023A098(EffectManager* mgr, unsigned int owner, int group, unsigned int flags) {
    EffectEntry_unk* e;
    unsigned int mask = ~flags;
    for (int i = 0; i < mgr->mEntries.mSize; i++) {
        e = &mgr->mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mFlags &= mask;
        }
    }
    if (mgr->mSystem.mEnabled != 0) {
        mgr->mSystem.fn_802330A4(owner, group, flags);
    }
}

extern "C" void fn_8023A108(EffectManager* mgr, unsigned int owner, int group, int count) {
    for (int i = 0; i < count; i++) {
        mgr->fn_8023AE34(owner, group, -1);
    }
    if (mgr->mSystem.mEnabled != 0) {
        mgr->mSystem.fn_80232BEC(owner, group, count);
    }
}

namespace atn {

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
void EffectManager::fn_8023A184(unsigned int owner, int group, const EffectColor_unk* color) {
    EffectEntry_unk* e;
    for (int i = 0; i < mEntries.mSize; i++) {
        e = &mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mColor.r = color->r;
            e->mColor.g = color->g;
            e->mColor.b = color->b;
            e->mColor.a = color->a;
        }
    }
    mSystem.fn_80233158(owner, group, color);
}

void EffectManager::fn_8023A1F4(unsigned int owner, int group, const atn::Vector3* vec) {
    EffectEntry_unk* e;
    for (int i = 0; i < mEntries.mSize; i++) {
        e = &mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->m34 = *vec;
        }
    }
    mSystem.fn_802331B0(owner, group, vec);
}

// inferred: int index converted to the unsigned operator[] (the index is incremented before the offset)
void EffectManager::fn_8023A254(unsigned int owner, int group, float rate) {
    EffectEntry_unk* e;
    for (int i = 0; i < mEntries.mSize; i++) {
        e = &mEntries[(unsigned int)i];
        if (e->mOwner == owner && (e->mGroup == group || group == -2)) {
            e->mRate = rate;
        }
    }
    mSystem.fn_80233260(owner, group, rate);
}

void EffectManager::fn_8023A2A8(void* owner) {
    mSystem.fn_80232ACC(owner);
    NotifyStop_unk(owner, 0, 0, 0, 0, 0, 0);
}

void EffectManager::fn_8023A340(EffectRes_unk* res) {
    EffectEntry_unk* it = mEntries.mData;
    while (it != mEntries.mData + mEntries.mSize) {
        if (it->mRes == res) {
            EffectEmitters_unk* emitters = res->mData;
            for (unsigned int i = 0; i < emitters->mCount; i++) {
                if (it->mEmitter < 0 || it->mEmitter == i) {
                    if (emitters->mEmitters[i].m00 != 0) {
                        int handle = it->mHandle + i;
                        StopEmitter_unk(handle, 0);
                    }
                }
            }
            it = Erase_unk(it);
        } else {
            it++;
        }
    }
}

void EffectManager::fn_8023A5A8(int clear) {
    if (clear != 0) {
        mEntries.clear();
    }
    mSystem.fn_80232E48();
}

void EffectManager::fn_8023A60C() {
    mSystem.fn_80232ED4();
}

void EffectManager::fn_8023A614(int arg) {
    fn_8023AE34(0, 0, arg);
    mSystem.fn_80232F84(arg);
}

void EffectManager::fn_8023A664(void* arg) {
    mDrawArg = arg;
    vf14();
}

void EffectManager::fn_8023A678() {
    mSystem.fn_8022BEAC();
}

} // namespace atn

// NONMATCHING: 99% - the original loads res->mData before storing the stack arguments; its "ef_mr" is at
// offset 0xD of the pool of the whole file (outside this split)
// note: the last integer argument is not used
extern "C" void* fn_8023A680(EffectManager* mgr, EffectRes_unk* res, int a, int b, int c, const atn::Vector3* pos,
                             const atn::Vector3* rot, const EffectColor_unk* color, int d, int e, int f, float scale) {
    if (res->mData == 0) {
        return 0;
    }
    fn_80232AC4(res);
    void* effect = mgr->mSystem.fn_8022FAC4(res->mData, a, b, c, pos, rot, color, d, e, scale);
    if (mgr->mCheckTrace != 0) {
        unsigned int flags = 0;
        if (strstr(fn_80473F18(&res->m04), "ef_mr") != 0) {
            flags |= 0x18;
        }
        if (flags != 0) {
            mgr->mSystem.fn_802348E4(effect, flags);
        }
    }
    if (mgr->mSystem.mListener != 0 && mgr->mSystem.mReplaying == 0) {
        EffectEvent_unk event;
        event.mType = 0;
        event.mRes = res;
        event.mOwner = fn_80232B88();
        event.mGroup = fn_80232B90();
        event.mFlags = mgr->mFlags;
        event.m58 = fn_802346B4();
        event.mHandle = (int)effect;
        event.mModel = (void*)a;
        event.m0C = b;
        event.m10 = c;
        event.mPos = (pos != 0) ? *pos : atn::Vector3(0.0f, 0.0f, 0.0f);
        event.mRot = (rot != 0) ? *rot : atn::Vector3(0.0f, 0.0f, 0.0f);
        event.mScale = scale;
        event.mColor = (color != 0) ? *color : EffectColor_unk(1.0f, 1.0f, 1.0f, 1.0f);
        event.m40 = d;
        event.m44 = e;
        event.mLayer = mgr->mSystem.mLayer;
        mgr->mSystem.mListener->vf08(&event);
    }
    return effect;
}

extern "C" int fn_8023A8B4(EffectManager* mgr, EffectRes_unk* res, void* model, void* node, const atn::Vector3& pos,
                           const atn::Vector3& rot, const EffectColor_unk& color, int emitter, int arg8, int arg9,
                           float scale) {
    if (res->mData == 0) {
        return 0;
    }
    if (mgr->mEntries.mSize >= mgr->mMax) {
        return 0;
    }
    mgr->mSerial++;
    if (mgr->mSerial >= 0x10000) {
        mgr->mSerial = 1;
    }
    EffectEntry_unk entry;
    entry.mRes = res;
    entry.mOwner = fn_80232B88();
    entry.mGroup = fn_80232B90();
    entry.mFlags = mgr->mFlags;
    if (mgr->mSystem.m78 != 0) {
        entry.mFlags |= 0x20;
    }
    if (mgr->mSystem.m7C != 0) {
        entry.mFlags |= 0x100;
    }
    if (node != 0) {
        entry.mTarget = node;
        entry.mFlags |= 0x10000;
    } else {
        entry.mTarget = model;
    }
    entry.mFrame = 0;
    entry.mPos = pos;
    entry.mRot = rot;
    entry.mScale = scale;
    entry.m34.x = 0.0f;
    entry.m34.y = 0.0f;
    entry.m34.z = 0.0f;
    entry.mHandle = mgr->mSerial << 16;
    entry.mEmitter = emitter;
    entry.m46 = arg8;
    entry.mLayer = mgr->mSystem.mLayer;
    entry.m4A = mgr->mSystem.m70;
    entry.mColor = color;
    entry.mRate = 1.0f;
    entry.mTime = 0.0f;
    mgr->mEntries.push_back(entry);
    return mgr->mSerial;
}

namespace atn {

// NONMATCHING: 74% - advances the entries and emits their particles; written from the original, the
// register allocation and the matrix copies differ
void EffectManager::fn_8023AE34(unsigned int owner, int group, int arg) {
    if (mPaused != 0) {
        return;
    }
    EffectEntry_unk* it = mEntries.mData;
    while (it != mEntries.mData + mEntries.mSize) {
        if ((it->mFlags & 2) == 2) {
            it++;
            continue;
        }
        if (it->mFrame < 0) {
            if (mSystem.fn_802346BC(it->mHandle) == 0) {
                it = Erase_unk(it);
            } else {
                it++;
            }
            continue;
        }
        if (owner != 0 && !(it->mOwner == owner && (it->mGroup == group || group == -2))) {
            it++;
            continue;
        }
        if (arg >= 0 && it->mLayer != arg) {
            it++;
            continue;
        }
        it->mTime = it->mRate * lbl_8087EFA8->mTimeScale + it->mTime;
        while (it->mTime >= 1.0f) {
            EffectEmitters_unk* emitters = it->mRes->mData;
            for (unsigned int i = 0; i < emitters->mCount; i++) {
                if (it->mEmitter >= 0 && it->mEmitter != i) {
                    continue;
                }
                EffectEmitter_unk* em = &emitters->mEmitters[i];
                if (em->m00 == 0) {
                    continue;
                }
                int handle = it->mHandle + i;
                if (em->m04 == it->mFrame) {
                    void* target = it->mTarget;
                    int mode = em->m38;
                    if (target != 0) {
                        if ((it->mFlags & 0x10000) == 0x10000) {
                            if (mode == 3) {
                                target = (unsigned char*)target + 0x30;
                                mode = 5;
                            } else if (mode == 4) {
                                int index = fn_800928B0(*(unsigned char**)((unsigned char*)target + 4) + 0x10, em->m3C, 0);
                                void* nodes;
                                if (index >= 0 && (nodes = *(void**)((unsigned char*)it->mTarget + 0x80)) != 0) {
                                    target = (unsigned char*)nodes + index * 0x30;
                                    mode = 5;
                                } else {
                                    mode = 0;
                                }
                            }
                        } else if (mode == 4) {
                            int index = fn_800928B0(target, em->m3C, 0);
                            void* node = (index >= 0) ? *(unsigned char**)((unsigned char*)target + 0x3C) + index * 0x30 : 0;
                            if (node != 0) {
                                target = node;
                                mode = 5;
                            } else {
                                mode = 0;
                            }
                        }
                    } else if (mode > 1) {
                        mode = 0;
                    }
                    atn::Vector3 offset = em->m0C;
                    atn::Vector3 rot = em->m18;
                    EffectColor_unk color;
                    color.r = em->m24.x * it->mColor.r;
                    color.g = em->m24.y * it->mColor.g;
                    color.b = em->m24.z * it->mColor.b;
                    color.a = em->m30 * it->mColor.a;
                    float scale = em->m34;
                    float m[3][4];
                    float tmp[3][4];
                    float mtx[3][4];
                    m[0][0] = 1.0f;
                    m[0][1] = 0.0f;
                    m[0][2] = 0.0f;
                    m[0][3] = 0.0f;
                    m[1][0] = 0.0f;
                    m[1][1] = 1.0f;
                    m[1][2] = 0.0f;
                    m[1][3] = 0.0f;
                    m[2][0] = 0.0f;
                    m[2][1] = 0.0f;
                    m[2][2] = 1.0f;
                    m[2][3] = 0.0f;
                    if (it->mRot.z != 0.0f) {
                        PSMTXRotRad(tmp, 'z', it->mRot.z);
                        PSMTXConcat(m, tmp, mtx);
                        for (int k = 0; k < 3; k++) {
                            for (int l = 0; l < 4; l++) {
                                m[k][l] = mtx[k][l];
                            }
                        }
                    }
                    if (it->mRot.y != 0.0f) {
                        PSMTXRotRad(tmp, 'y', it->mRot.y);
                        PSMTXConcat(m, tmp, mtx);
                        for (int k = 0; k < 3; k++) {
                            for (int l = 0; l < 4; l++) {
                                m[k][l] = mtx[k][l];
                            }
                        }
                    }
                    if (it->mRot.x != 0.0f) {
                        PSMTXRotRad(tmp, 'x', it->mRot.x);
                        PSMTXConcat(m, tmp, mtx);
                        for (int k = 0; k < 3; k++) {
                            for (int l = 0; l < 4; l++) {
                                m[k][l] = mtx[k][l];
                            }
                        }
                    }
                    PSMTXMultVec(m, &offset, &offset);
                    offset.x *= it->mScale;
                    offset.y *= it->mScale;
                    offset.z *= it->mScale;
                    offset.x += it->mPos.x;
                    offset.y += it->mPos.y;
                    offset.z += it->mPos.z;
                    rot.x += it->mRot.x;
                    rot.y += it->mRot.y;
                    rot.z += it->mRot.z;
                    scale *= it->mScale;
                    mSystem.mLayer = it->mLayer;
                    mSystem.m70 = it->m4A;
                    fn_80232B7C(it->mOwner, it->mGroup);
                    fn_802346AC(handle);
                    fn_8023A680(this, (EffectRes_unk*)&em->m48, it->m46, mode, (int)target, &offset, &rot, &color,
                                em->m40, em->m44, 1, scale);
                    mSystem.fn_80234764(handle, &it->m34);
                    mSystem.fn_802346F0(handle, it->mFlags);
                    fn_80232B7C(0, 0);
                    fn_802346AC(0);
                    mSystem.m70 = 0;
                    mSystem.mLayer = 0;
                } else if (em->m08 != 0 && em->m08 == it->mFrame) {
                    StopEmitter_unk(handle, 1);
                }
            }
            it->mFrame++;
            it->mTime -= 1.0f;
        }
        int length = it->mRes->mData->mLength;
        if (it->mFrame >= length && length != 0 && mSystem.fn_802346BC(it->mHandle) == 0) {
            it = Erase_unk(it);
        } else {
            it++;
        }
    }
}

void EffectManager::fn_8023B71C(EffectManager* mgr, int handle) {
    fn_802346AC(handle);
}

// NONMATCHING: 99.7% - registers of the inlined fn_8023A02C loop
// Replays a recorded event.
int EffectManager::fn_8023B724(EffectEvent_unk* event) {
    int result = 0;
    switch (event->mType) {
    case 0:
        fn_80232B7C(event->mOwner, event->mGroup);
        fn_802346AC(event->m58);
        result = (int)fn_8023A680(this, (EffectRes_unk*)event->mRes, (int)event->mModel, event->m0C, event->m10,
                                  &event->mPos, &event->mRot, &event->mColor, event->m40, event->m44, 1, event->mScale);
        fn_8023A02C(this, event->mOwner, event->mGroup, event->mFlags);
        break;
    case 1:
        if (event->mRes != 0) {
            fn_8023A2A8(event->mRes);
        } else if (event->m58 != 0) {
            StopEmitter_unk(event->m58, event->m5C);
        } else if (event->mHandle != 0) {
            StopEffect_unk((void*)event->mHandle, event->m5C);
        } else {
            fn_80239DAC(this, event->mOwner, event->mGroup, event->m5C);
        }
        break;
    }
    return result;
}

} // namespace atn
