#include "atn/LoadHandle.h"
#include "atn/LoadCore.h"
#include "atn/LoadManager.h"
#include <boost/function.hpp>

extern "C" void OSSleepTicks(long long ticks);

// inferred: list of callbacks run while a load is waited for (global at 0x8087EE94, also run by
// atn::AudioDevice 0x80046228 and main). Name and layout invented: count at 0x0, array at 0x8.
struct LoadCallbacks_unk {
    unsigned int Size_unk() const { return mCount; } // inferred: name invented
    boost::function<void()>& At_unk(unsigned int i) const { return mData[i]; } // inferred: name invented
    unsigned int mCount;
    unsigned int m04;
    boost::function<void()>* mData;
};

extern "C" LoadCallbacks_unk* lbl_8087EE94;

namespace atn {

LoadHandle::LoadHandle() : mCore(0) {}

LoadHandle::~LoadHandle() {
    if (mCore != 0) {
        ((LoadCore*)mCore)->fn_804730E4();
    }
}

int LoadHandle::fn_80473EFC() {
    if (mCore != 0) {
        return ((LoadCore*)mCore)->mState;
    }
    return 0;
}

const char* LoadHandle::fn_80473F18() {
    if (mCore != 0) {
        return ((LoadCore*)mCore)->mPath;
    }
    return 0;
}

unsigned int LoadHandle::fn_80473F34() {
    if (mCore != 0) {
        return ((LoadCore*)mCore)->m48;
    }
    return 0;
}

int LoadHandle::fn_80473F50() {
    if (mCore != 0) {
        return ((LoadCore*)mCore)->GetState_unk() == 1 || ((LoadCore*)mCore)->GetState_unk() == 2;
    }
    return 0;
}

void LoadHandle::fn_80473F88() {
    if (mCore != 0) {
        ((LoadCore*)mCore)->fn_804730E4();
        mCore = 0;
    }
}

// NONMATCHING: 99.56% - only register allocation of the callback loop: the original has the list in r21,
// the element in r22 and the counter in r23 (here r22/r23/r21). `this` (r20) and the offset (r19) match
// with the Size_unk/At_unk accessors and an int counter. Tried: const list, At_unk(int), const
// element, the counter declared before the list, a reference to the element, (mData + i)->operator();
// before: the loop in an inline helper, the wait in an inline member, early return + do/while, a
// `busy` local, locals declared at the top.
void LoadHandle::fn_80473FCC(const char* path) {
    vf0C(path);
    if (mCore != 0) {
        for (;;) {
            lbl_8087F518->vf10();
            LoadCallbacks_unk* callbacks = lbl_8087EE94;
            if (callbacks != 0) {
                for (int i = 0; i < callbacks->Size_unk(); i++) {
                    callbacks->At_unk(i)();
                }
            }
            ((LoadCore*)mCore)->vf0C();
            if (fn_80473F50() == 0) {
                break;
            }
            OSSleepTicks((*(unsigned int*)0x800000F8 / 4) / 1000);
        }
    }
}

void LoadHandle::operator=(const LoadHandle& o) {
    fn_80473F88();
    mCore = o.mCore;
    if (mCore != 0) {
        ((LoadCore*)mCore)->fn_804730D4();
    }
}

} // namespace atn
