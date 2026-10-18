#ifndef STD_VECTOR_H
#define STD_VECTOR_H

// Partial reconstruction of the MSL C++ vector used by the game.
// Layout inferred from atn::FileList (0x8006BBCC): data pointer, size, capacity.
// The destructor is inlined through three levels (three null checks of `this` at 0x8006BCE4),
// so the storage lives in a base class two levels below vector. Class names are invented.

#include "std/msl_common.h"

namespace std {

template <class T>
struct __vec_storage_unk {
    __vec_storage_unk() : mData(0), mSize(0), mCap(0) {}
    ~__vec_storage_unk() {
        if (mData != 0) {
            // inferred: not through clear(); with -ipa file the extra inline level swaps the loop
            // registers in atn::FileList::~FileList (0x8006BCE4)
            __shrink_unk(0);
            ::operator delete(mData);
        }
    }
    void clear() {
        unsigned int n = mSize;
        T* p = mData + n;
        mSize -= n;
        for (; n != 0; n--) {
            --p;
            p->~T();
        }
    }

    // inferred: used by atn::FileList::FileList; destroys the elements after `newSize`
    void __shrink_unk(unsigned int newSize) {
        T* p = mData + mSize;
        unsigned int n = mSize - newSize;
        mSize -= n;
        for (; n != 0; n--) {
            --p;
            p->~T();
        }
    }

    T* mData;
    unsigned int mSize;
    unsigned int mCap;
};

template <class T>
struct __vec_imp_unk : public __vec_storage_unk<T> {
    ~__vec_imp_unk() {}
};

// inferred: temporary buffer used when push_back reallocates (0x800DB3EC): storage, number of
// elements, capacity, allocator reference and index of the first element. Names invented. The
// destructor lives in a base class: the original tests the buffer address for null before
// destroying it (addic. r3, r1, 0x14), and keeps its fields in memory.
template <class T>
struct __vec_split_base_unk {
    __vec_split_base_unk(void* alloc) : mData(0), mSize(0), mCap(0), mAlloc(alloc) {}
    ~__vec_split_base_unk() {
        if (mData != 0) {
            mSize = 0;
            ::operator delete(mData);
        }
    }
    T* mData;
    unsigned int mSize;
    unsigned int mCap;
    void* mAlloc;
};

template <class T>
struct __vec_split_unk : public __vec_split_base_unk<T> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<T>(alloc), mStart(0) {}
    T* begin() { return this->mData + mStart; }
    void push_back(const T& x) {
        begin()[this->mSize] = x;
        this->mSize++;
    }
    // inferred: moves [first, last) in front of the stored elements
    void __push_front_unk(T* first, T* last) {
        unsigned int n = last - first;
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(T));
        memset(first, 0, n * sizeof(T));
        this->mSize += n;
    }
    unsigned int mStart;
};

template <class T>
inline void __vec_swap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <class T>
inline const T& __vec_max_unk(const T& a, const T& b) {
    return (a < b) ? b : a;
}

template <class T>
class vector : public __vec_imp_unk<T> {
public:
    typedef unsigned int size_type;
    vector() {}
    // declared only (boost::offset_separator, never used by the game)
    vector(size_type n, const T& value);
    template <class Iterator> vector(Iterator first, Iterator last);
    ~vector() {}

    T* begin() { return this->mData; }
    T* end() { return this->mData + this->mSize; }
    size_type max_size() const { return 0x3FFFFFFF; }
    // inferred: weak copies of size 0x2F4 for pointer elements (0x800DB3EC, 0x80244CCC, ...).
    // NONMATCHING (standalone compile, 62 differing lines of 189): the fast path loads the
    // capacity before the size, the two growth temporaries swap stack slots, the new capacity
    // gets its own register, and the original does not fold (data + size) - data before the memcpy.
    // Defined out of the class: its literals are in the file string pool, not in an @STRING@ pool.
    void push_back(const T& x);

private:
    static T* __allocate(size_type n) {
        T* p = (T*)::operator new(n * sizeof(T));
        if (p == 0) {
            __msl_error("Memory allocation failure");
        }
        return p;
    }
};

template <class T>
void vector<T>::push_back(const T& x) {
    if (this->mSize < this->mCap) {
        this->mSize++;
        this->mData[this->mSize - 1] = x;
        return;
    }
    if (max_size() - this->mCap < 1) {
        __msl_error("vector length_error");
    }
    __vec_split_unk<T> buf(&this->mCap);
    size_type n = this->mSize + 1 - this->mCap;
    if (n > max_size() - this->mCap) {
        __msl_error("vector length_error");
    }
    size_type cap = this->mCap;
    if (cap < max_size() / 3) {
        cap += __vec_max_unk(size_type((cap + 1) * 3 / 5), n);
    } else if (cap < max_size() / 3 * 2) {
        cap += __vec_max_unk(size_type((cap + 1) / 2), n);
    } else {
        cap = max_size();
    }
    if (cap > max_size()) {
        __msl_error("vector length_error");
    }
    buf.mData = __allocate(cap);
    buf.mCap = cap;
    buf.mStart = this->mSize;
    buf.push_back(x);
    buf.__push_front_unk(this->mData, this->mData + this->mSize);
    this->mSize = 0;
    __vec_swap_unk(this->mCap, buf.mCap);
    __vec_swap_unk(this->mData, buf.mData);
    __vec_swap_unk(this->mSize, buf.mSize);
}

} // namespace std

#endif
