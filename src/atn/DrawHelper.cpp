#include "atn/Memory.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"

#include "atn/DrawHelper.h"

extern "C" void fn_8006C858(atn::DrawHelper* self); // inferred: releases the queued commands
// The string "" and the constants 0.0f and 1.0f used below are outside the split of this unit
// (string pool 0x80731340 and .sdata2 0x808809A0 of the code that follows), so they are
// referenced by address.
extern "C" const char lbl_80731340[];
extern "C" const float lbl_808809A0; // 0.0f
extern "C" const float lbl_808809A4; // 1.0f

namespace atn {

inline DrawHelper::DrawHelper() : mA0(0x12) {
    // inferred: Matrix3x4::SetIdentity with the constants of the original file
    mA4.m[0][1] = mA4.m[0][2] = mA4.m[0][3] = mA4.m[1][0] = mA4.m[1][2] = mA4.m[1][3] = mA4.m[2][0] =
        mA4.m[2][1] = mA4.m[2][3] = lbl_808809A0;
    mA4.m[0][0] = mA4.m[1][1] = mA4.m[2][2] = lbl_808809A4;
}

void DrawHelper::fn_8005DD60() {
    if (lbl_8087EEB0 == 0) {
        lbl_8087EEB0 = new ((MemoryTag)6, lbl_80731340, lbl_80731340, 0) DrawHelper;
    }
}

DrawHelper::~DrawHelper() {
    fn_8006C858(this);
}

} // namespace atn
