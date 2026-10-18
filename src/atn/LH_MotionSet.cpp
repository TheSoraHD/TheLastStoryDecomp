#include "atn/LH_MotionSet.h"
#include "atn/Array_unk.h"
#include "atn/LH_Motion.h"
#include "atn/LoadManager.h"

// inferred: reference to a shared resource of the load manager (functions 0x80470364/0x80470528)
struct ResRef_unk {
    ResRef_unk() : mRes(0), m04(0) {}
    ~ResRef_unk() { fn_80470528(); }
    void fn_80470364(const char* name, int a, int b, int c); // inferred: starts loading `name`
    void fn_80470528();                                      // inferred: drops the reference

    void* mRes;
    void* m04;
};

// inferred: motion of the set (0x10 bytes)
struct MotionSetEntry_unk {
    ~MotionSetEntry_unk();

    int mId;                // inferred: motion id given by the callback
    atn::LH_Motion mMotion; // 0x04
};

// inferred: defined at the top of the file (the copy at 0x80476D00 comes first)
MotionSetEntry_unk::~MotionSetEntry_unk() {}

namespace atn {

// inferred from __vt__Q23atn13LoadMotionSet (0x8078FEC8)
class LoadMotionSet : public LoadCore {
public:
    LoadMotionSet(const char* path) : LoadCore(path), mCallback(0), mParsed(0) {}
    virtual ~LoadMotionSet();
    virtual void vf0C();
    // NONMATCHING: not decompiled yet (0x80476ED4: reads the CSV of the set; it inlines
    // vector::push_back, see std/vector.h)
    void fn_80476ED4();

    Buffer_unk<MotionSetEntry_unk> mEntries; // 0x5C
    void* mCallback;                         // 0x64
    int mParsed;                             // 0x68
    ResRef_unk m6C;
};

LoadMotionSet::~LoadMotionSet() {}

void LoadMotionSet::vf0C() {
    if (mState == 2) {
        if (mParsed != 0) {
            if (mRefCount == 0) {
                fn_80473104(5);
            } else {
                int done = 1;
                for (unsigned int i = 0; i < mEntries.mSize; i++) {
                    if (mEntries.mData[i].mMotion.fn_800A0448() != 0) {
                        done = 0;
                    }
                }
                if (done) {
                    m6C.fn_80470528();
                    fn_80473104(3);
                }
            }
        } else {
            fn_80476ED4();
            mParsed = 1;
        }
    }
}

void LH_MotionSet::vf0C(const char* path) {
    fn_80473F88();
    LoadMotionSet* core = lbl_8087F518->Get_unk<LoadMotionSet>(path, 1);
    mCore = core;
    core->mCallback = mCallback;
}

} // namespace atn
