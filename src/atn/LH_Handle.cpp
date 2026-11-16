#include "atn/LoadHandle.h"
#include "atn/LoadCore.h"
#include "atn/LoadManager.h"
#include <boost/function.hpp>
#include "atn/Array_unk.h"

extern "C" void OSSleepTicks(long long ticks);

// inferred: list of callbacks run while a load is waited for (global at 0x8087EE94, also run by
// atn::AudioDevice 0x80046228 and main): the same atn::SimpleVector as in those units.
typedef atn::SimpleVector<boost::function<void()> > FrameCallbacks_unk;
extern "C" FrameCallbacks_unk* lbl_8087EE94;

// inferred: calls every per-frame callback (same inline as game/main.cpp and atn/AudioDevice.cpp)
inline void CallAll_unk(FrameCallbacks_unk* callbacks) {
    for (unsigned int i = 0; i < callbacks->size(); i++) {
        (*callbacks)[i]();
    }
}

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

void LoadHandle::fn_80473FCC(const char* path) {
    vf0C(path);
    if (mCore != 0) {
        for (;;) {
            lbl_8087F518->vf10();
            FrameCallbacks_unk* callbacks = lbl_8087EE94;
            if (callbacks != 0) {
                CallAll_unk(callbacks);
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
