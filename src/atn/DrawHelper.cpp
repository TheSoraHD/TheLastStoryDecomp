#include "atn/Memory.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"

namespace atn {

// inferred: array released with delete[] {data, size, ?} (names invented; the destructor is
// inlined into DrawHelper::~DrawHelper)
template <class T>
class DrawArray_unk {
public:
    DrawArray_unk() : mData(0), mSize(0), m08(0) {}
    ~DrawArray_unk() {
        if (mData != 0) {
            delete[] mData;
            mData = 0;
            mSize = 0;
        }
    }

    T* mData;
    int mSize;
    int m08;
};

// inferred: debug drawing helper (0x504 bytes, singleton lbl_8087EEB0)
class DrawHelper {
public:
    DrawHelper();
    virtual ~DrawHelper();

    static void fn_8005DD60(); // inferred: creates the singleton

    DrawArray_unk<unsigned char> m04; // inferred: element type unknown
    Texture_unk m10;
    Texture_unk m40;
    Texture_unk m70;
    int mA0;
    Matrix3x4 mA4;
    unsigned char mD4[0x504 - 0xD4];
};

} // namespace atn

extern "C" atn::DrawHelper* lbl_8087EEB0; // inferred: DrawHelper singleton
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
        lbl_8087EEB0 = new (6, lbl_80731340, lbl_80731340, 0) DrawHelper;
    }
}

DrawHelper::~DrawHelper() {
    fn_8006C858(this);
}

} // namespace atn
