#ifndef STD_VECTOR_STRING_H
#define STD_VECTOR_STRING_H

// inferred: std::vector<std::string> (weak copies in game/MotionCommand_unk.cpp: push_back 0x801207D4,
// destructor 0x80120700). When push_back reallocates, the strings are moved to the new storage as raw
// bytes (memcpy + memset, as for pointer elements) and the old storage and the temporary buffer are
// then cleared element by element. Names invented.

#include "std/string.h"
#include "std/vector.h"

namespace std {

template <>
struct __vec_split_base_unk<string> {
    __vec_split_base_unk(void* alloc) {
        unsigned int* w = (unsigned int*)this;
        w[0] = 0;
        w[1] = 0;
        w[2] = 0;
        mAlloc = alloc;
    }
    ~__vec_split_base_unk() {
        if (mData != 0) {
            unsigned int n = mSize;
            string* p = mData + n;
            unsigned int& size = mSize;
            size -= n;
            for (; n != 0; n--) {
                --p;
                p->~string();
            }
            ::operator delete(mData);
        }
    }
    string* mData;
    unsigned int mSize;
    unsigned int mCap;
    void* mAlloc;
};

template <>
struct __vec_split_unk<string> : public __vec_split_base_unk<string> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<string>(alloc), mStart(0) {}
    string* begin() { return this->mData + mStart; }
    void push_back(const string& x) {
        new (begin() + this->mSize) string(x);
        this->mSize++;
    }
    // inferred: moves [first, last) in front of the stored elements as raw bytes
    void __push_front_unk(string* first, string* last) {
        int n = last - first;
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(string));
        memset(first, 0, n * sizeof(string));
        this->mSize += n;
    }
    void clear() {
        string* b = begin();
        string* p = b + this->mSize;
        while (p > b) {
            --p;
            p->~string();
        }
        this->mSize = 0;
    }
    unsigned int mStart;
};

template <>
inline void vector<string>::__swap_out_unk(__vec_split_unk<string>& buf) {
    buf.__push_front_unk(this->mData, this->mData + this->mSize);
    __vec_swap_unk(this->mCap, buf.mCap);
    __vec_swap_unk(this->mData, buf.mData);
    __vec_swap_unk(this->mSize, buf.mSize);
    buf.clear();
}

} // namespace std

#endif
