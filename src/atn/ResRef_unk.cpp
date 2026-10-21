// inferred: separate file between atn/FileSystem_unk and atn/LH_Binary (0x80470364-0x80470580): it calls
// Loader::fn_8046F460, PfsArchive_unk::fn_80470198/804701F8/80470264 out of line and "%sh" is a
// .rodata object after the FileSystem_unk string pool. Name invented.
#include "atn/PfsArchive_unk.h"

extern "C" int sprintf(char* s, const char* format, ...);
struct DebugMessages_unk { void fn_800698DC(const char* path); }; // inferred: see atn/FileUtil_unk.cpp
extern "C" DebugMessages_unk* lbl_8087EEB8;                                 // inferred: listener of failed loads

void ResRef_unk::fn_80470364(const char* name, int a, int b, int c) {
    if (mRes != 0) {
        mRes->mRefCount--;
    }
    mRes = 0;
    mRes = lbl_8087F518->fn_8046F460(name);
    if (mRes == 0 && b != 0) {
        char index[0x100];
        sprintf(index, "%sh", name);
        mRes = fn_8046EF08(lbl_8087F518, name, index, c);
    }
    if (mRes != 0) {
        mRes->fn_804701F8();
        mRes->m70 = a;
        mRes->m74 = b;
    } else {
        lbl_8087EEB8->fn_800698DC(name);
    }
}

void ResRef_unk::fn_8047043C(const char* name, int a, int b) {
    if (mRes != 0) {
        mRes->mRefCount--;
    }
    mRes = 0;
    mRes = lbl_8087F518->fn_8046F460(name);
    if (mRes == 0 && b != 0) {
        char index[0x100];
        sprintf(index, "%sh", name);
        mRes = fn_8046EF08(lbl_8087F518, name, index, 1);
    }
    if (mRes != 0) {
        mRes->fn_80470198();
        mRes->m70 = a;
        mRes->m74 = b;
    } else {
        lbl_8087EEB8->fn_800698DC(name);
    }
}

void ResRef_unk::fn_80470528() {
    if (mRes != 0) {
        mRes->mRefCount--;
    }
    mRes = 0;
}

int ResRef_unk::fn_8047054C() {
    if (mRes != 0) {
        return mRes->fn_80470264();
    }
    return 0;
}

namespace atn {

void* PfsHandle_unk::fn_80470564() {
    if (mCore != 0) {
        return ((LoadCore*)mCore)->m54;
    }
    return 0;
}

} // namespace atn
