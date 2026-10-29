#ifndef ATN_FIXEDARRAY_H
#define ATN_FIXEDARRAY_H

#include "std/new.h"

namespace atn {

// inferred: fixed-capacity array {count, elements[N]} used by game tasks (name unknown).
// push_back constructs the element in place (null check on the slot address, e.g. 0x805B9C64).
template <class T, int N>
class FixedArray_unk {
public:
    FixedArray_unk() : mSize(0) {}
    unsigned int size() const { return mSize; }
    void clear() { mSize = 0; }
    T& operator[](int i) { return mData[i]; }
    void push_back(const T& value) {
        new (&mData[mSize]) T(value);
        mSize++;
    }
    T* begin() { return mData; }
    T* end() { return mData + mSize; }
    // inferred: removes an element, moving the following ones (atn/SoundManager.cpp)
    void erase(T* it) {
        unsigned int i = it - mData;
        for (; i < mSize - 1; i++) {
            mData[i] = mData[i + 1];
        }
        mSize = mSize - 1;
    }

    unsigned int mSize;
    T mData[N];
};

} // namespace atn

#endif
