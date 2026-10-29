#include "game/GameLoadBackground.h"
#include "atn/Memory.h"

extern "C" char* strcpy(char* dst, const char* src);


GameLoadBackground* GameLoadBackground::fn_803B33F8(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameLoadBackground(parent);
    }
    return 0;
}

void GameLoadBackground::vf10() {
    int work[2]; // inferred: output buffer, layout unknown
    if (m154) {
        if (lbl_8087EF68 == 0 || fn_800A4228(lbl_8087EF68, work) == 0) {
            fn_803B8270();
        }
    }
}

void GameLoadBackground::fn_803B3530(const char* name, int arg1, int arg2, int arg3) {
    int work[2]; // inferred: output buffer, layout unknown
    m158 = arg1;
    m14C = arg2;
    strcpy(mName, name);
    fn_803B8200(0);
    fn_800A49C4(lbl_8087EF68, name, arg1, arg2, arg3, work);
}
