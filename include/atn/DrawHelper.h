#ifndef ATN_DRAWHELPER_H
#define ATN_DRAWHELPER_H

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

#endif
