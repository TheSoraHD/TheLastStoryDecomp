#include "atn/LoadCore.h"
#include "atn/LoadManager.h"
#include <string.h>

struct DebugMessages_unk { void fn_800698DC(const char* path); }; // inferred: see atn/FileUtil_unk.cpp
extern "C" DebugMessages_unk* lbl_8087EEB8;                               // inferred: listener of failed loads

namespace atn {

// inferred: copy of the path only when it is not the same buffer
inline void __CopyPath_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}

LoadCore::LoadCore(const char* path) {
    mState = 1;
    m05 = 0;
    __CopyPath_unk(mPath, path);
    mRefCount = 1;
    m50 = 0;
    m54 = 0;
    m58 = 0;
    m48 = fn_800DC6B4(mPath);
}

LoadCore::~LoadCore() {
    fn_80473130();
}

void LoadCore::fn_804730D4() {
    mRefCount++;
}

void LoadCore::fn_804730E4() {
    if (--mRefCount == 0) {
        lbl_8087F518->m3EDC = 1;
    }
}

void LoadCore::fn_80473104(unsigned char state) {
    mState = state;
    if (mState == 4 && lbl_8087EEB8 != 0) {
        lbl_8087EEB8->fn_800698DC(mPath);
    }
}

void LoadCore::fn_80473130() {
    if (m54 != 0 && m58 == 0) {
        fn_8046DD20(lbl_8087F518, m54);
    }
    m54 = 0;
    m50 = 0;
}

void LoadCore::vf0C() {
    if (mState == 2) {
        fn_80473104(3);
    }
}

} // namespace atn
