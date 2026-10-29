#ifndef ATN_ARRAY_UNK_H
#define ATN_ARRAY_UNK_H

#include "atn/Memory.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

namespace atn {

// atn::SimpleVector (name and operator[](int) from the RSO; the other method names are invented):
// growable array {size, capacity, data} allocated with new[]. Its
// members are defined in the class body: their literals are @STRING@ objects in .sdata
// (0x8087DF10/0x8087DF14 for the new[] of GimmickManager::fn_803EE7FC).
template <class T>
class SimpleVector {
public:
    SimpleVector() : mSize(0), mCapacity(0), mData(0) {}
    ~SimpleVector() {
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
        T* data = new ((MemoryTag)heap, "", "", 0) T[n];
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
        T* data = (n != 0) ? new ((MemoryTag)0, "", "", 0) T[n] : 0;
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
    // inferred: same from the given heap; the weak copies of game/MotionCommand_unk.cpp
    // (0x801204A8, 0x80120C30, 0x80120D44, 0x80120E48) take a third argument they never read
    void Resize_unk(unsigned int n, int heap, int unused) {
        T* data = (n != 0) ? new ((MemoryTag)heap, "", "", 0) T[n] : 0;
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
    void Assign_unk(const SimpleVector& o) {
        clear();
        Resize_unk(o.mSize);
        for (unsigned int i = 0; i < mSize; i++) {
            mData[i] = o[i];
        }
    }
    // inferred: the copy of gmk::RandomItemDrop_unk (inlined in gmk::RandomItem::vf20) calls
    // Assign_unk through the assignment of this member
    SimpleVector& operator=(const SimpleVector& o) {
        Assign_unk(o);
        return *this;
    }
    T& operator[](int i) { return mData[i]; }
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

// atn::SimpleArray (name from the RSO: SimpleArray<T>, SetMotionParts takes a
// SimpleArray<CharaFaceControl::MotionPart>); array allocated once with new[] {size, data};
// member names invented
template <class T>
class SimpleArray {
public:
    SimpleArray() : mSize(0), mData(0) {}
    // inferred: weak copy 0x80514218 (inlined in the copy of a record of game/NewCampPartsGmk.cpp)
    SimpleArray(const SimpleArray& o) : mSize(0), mData(0) {
        Resize_unk(o.mSize, 0);
        for (unsigned int i = 0; i < mSize; i++) {
            mData[i] = o.mData[i];
        }
    }
    ~SimpleArray() {
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
            mData = new ((MemoryTag)1, "", "", 0) T[mSize];
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
            mData = new ((MemoryTag)heap, "", "", 0) T[mSize];
        } else {
            mData = 0;
        }
    }
    // inferred: Resize_unk from heap 0 (weak copies 0x8004269C and 0x80042E28 in
    // game/AreaEffConfig.cpp; name invented)
    void Alloc0_unk(int n) { Resize_unk(n, 0); }
    // inferred: weak copy inside AreaEffConfig's row assignment (0x80042EEC)
    SimpleArray& operator=(const SimpleArray& o) {
        Resize_unk(o.mSize, 0);
        for (unsigned int i = 0; i < mSize; i++) {
            mData[i] = o.mData[i];
        }
        return *this;
    }
    // inferred: as Resize_unk(n, heap); the third argument is the name of the owner, unused in release
    // (weak copies 0x803F0280-0x803F0538, game/GimmickStateCommand.cpp)
    void Resize_unk(int n, int heap, const char* name) {
        if (mData != 0) {
            delete[] mData;
        }
        mSize = n;
        if (mSize != 0) {
            mData = new ((MemoryTag)heap, "", "", 0) T[mSize];
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
