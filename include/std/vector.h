#ifndef STD_VECTOR_H
#define STD_VECTOR_H

// Partial reconstruction of the MSL C++ vector used by the game.
// Layout inferred from atn::FileList (0x8006BBCC): data pointer, size, capacity.
// The destructor is inlined through three levels (three null checks of `this` at 0x8006BCE4),
// so the storage lives in a base class two levels below vector. Class names are invented.

void operator delete(void* p);

namespace std {

template <class T>
struct __vec_storage_unk {
    __vec_storage_unk() : mData(0), mSize(0), mCap(0) {}
    ~__vec_storage_unk() {
        if (mData != 0) {
            clear();
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

template <class T>
class vector : public __vec_imp_unk<T> {
public:
    ~vector() {}
};

} // namespace std

#endif
