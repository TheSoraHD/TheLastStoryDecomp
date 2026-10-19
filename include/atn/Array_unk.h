#ifndef ATN_ARRAY_UNK_H
#define ATN_ARRAY_UNK_H

#include "atn/Memory.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

namespace atn {

// inferred: growable array {size, capacity, data} allocated with new[] (names invented). Its
// members are defined in the class body: their literals are @STRING@ objects in .sdata
// (0x8087DF10/0x8087DF14 for the new[] of GimmickManager::fn_803EE7FC).
template <class T>
class Vector_unk {
public:
    Vector_unk() : mSize(0), mCapacity(0), mData(0) {}
    ~Vector_unk() {
        if (mData != 0) {
            delete[] mData;
        }
    }
    void clear() {
        mSize = 0;
        mCapacity = 0;
        if (mData != 0) {
            delete[] mData;
            mData = 0;
        }
    }
    // inferred: the heap is a parameter (CharaTask::vf50 reserves from heap 3)
    void reserve(unsigned int n, int heap = 0) {
        if (mCapacity > n) {
            return;
        }
        T* data = new (heap, "", "", 0) T[n];
        if (mData != 0) {
            // inferred: the ternary gives the register order of the original (0x803EE7FC)
            unsigned int count = (n > mSize) ? mSize : n;
            for (unsigned int i = 0; i < count; i++) {
                data[i] = mData[i];
            }
            delete[] mData;
        }
        mData = data;
        mCapacity = n;
    }
    // inferred: sets size and capacity to n, keeping the first elements (names invented;
    // CharaTask::fn_8017B1C4)
    void Resize_unk(unsigned int n) {
        T* data = (n != 0) ? new (0, "", "", 0) T[n] : 0;
        if (mData != 0) {
            unsigned int count = (n > mSize) ? mSize : n;
            for (unsigned int i = 0; i < count; i++) {
                data[i] = mData[i];
            }
            delete[] mData;
        }
        mData = data;
        mSize = n;
        mCapacity = n;
    }
    // inferred: copy of another vector (name invented; CharaTask::fn_8017B1C4)
    void Assign_unk(const Vector_unk& o) {
        clear();
        Resize_unk(o.mSize);
        for (unsigned int i = 0; i < mSize; i++) {
            (*this)[i] = o[i];
        }
    }
    T& operator[](unsigned int i) { return mData[i]; }
    const T& operator[](int i) const { return mData[i]; }
    void push_back(const T& value) {
        if (mData == 0 || mCapacity == 0) {
            reserve(8);
        } else if (mSize >= mCapacity) {
            reserve(mSize * 2);
        }
        mData[mSize] = value;
        mSize++;
    }

    unsigned int mSize;
    unsigned int mCapacity;
    T* mData;
};

// inferred: array allocated once with new[] {size, data} (names invented)
template <class T>
class Buffer_unk {
public:
    Buffer_unk() : mSize(0), mData(0) {}
    // inferred: weak copy 0x80514218 (inlined in the copy of a record of game/NewCampPartsGmk.cpp)
    Buffer_unk(const Buffer_unk& o) : mSize(0), mData(0) {
        Resize_unk(o.mSize, 0);
        for (unsigned int i = 0; i < mSize; i++) {
            mData[i] = o.mData[i];
        }
    }
    ~Buffer_unk() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mSize = 0;
    }
    // inferred: same body as the destructor, called by the destructor of CampSaveLoad (inlined at
    // 0x801177E8) before the member is destroyed (name invented)
    void Free_unk() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mSize = 0;
    }
    void Alloc(int n) {
        if (mData != 0) {
            delete[] mData;
        }
        mSize = n;
        // inferred: "!= 0", not "> 0": the test is folded but the store of the dead branch stays
        // (GimmickManager ctor, 0x803EE024)
        if (mSize != 0) {
            mData = new (1, "", "", 0) T[mSize];
        } else {
            mData = 0;
        }
        memset(mData, 0, mSize * sizeof(T));
    }
    // inferred: as Alloc, but from the given heap and without clearing (atn::LoadScene 0x804778DC
    // uses heap 0, atn::LoadModel 0x804749C8 heap 6)
    void Resize_unk(int n, int heap) {
        if (mData != 0) {
            delete[] mData;
        }
        mSize = n;
        if (mSize != 0) {
            mData = new (heap, "", "", 0) T[mSize];
        } else {
            mData = 0;
        }
    }
    T& operator[](int i) { return mData[i]; }

    int mSize;
    T* mData;
};

} // namespace atn

#endif
