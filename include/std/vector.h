#ifndef STD_VECTOR_H
#define STD_VECTOR_H

// Partial reconstruction of the MSL C++ vector used by the game.
// Layout inferred from atn::FileList (0x8006BBCC): data pointer, size, capacity.
// The destructor is inlined through three levels (three null checks of `this` at 0x8006BCE4),
// so the storage lives in a base class two levels below vector. Class names are invented.

#include "std/msl_common.h"
#include "std/new.h"

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
        // inferred: through a reference, so that `mSize - mSize` is not folded to 0 for elements
        // without a destructor (ShopColor::vf38, 0x80587624)
        unsigned int& size = mSize;
        size -= n;
        for (; n != 0; n--) {
            --p;
            p->~T();
        }
    }

    // inferred: used by atn::FileList::FileList; destroys the elements after `newSize`
    void __shrink_unk(unsigned int newSize) {
        T* p = mData + mSize;
        unsigned int n = mSize - newSize;
        // inferred: through a reference, so that with an empty loop `mSize - mSize` is not folded
        // to 0 (atn::LoadMaterial::~LoadMaterial, 0x80474224)
        unsigned int& size = mSize;
        size -= n;
        for (; n != 0; n--) {
            --p;
            p->~T();
        }
    }

    // inferred: moves [first, last) behind the stored elements as raw bytes (reserve of
    // std::wstring elements in atn::D2A_Message::vf18, 0x801F2A8C)
    void __append_raw_unk(T* first, T* last) {
        unsigned int n = last - first;
        memcpy(mData + mSize, first, n * sizeof(T));
        memset(first, 0, n * sizeof(T));
        mSize += n;
    }

    T* mData;
    unsigned int mSize;
    unsigned int mCap;
};

// inferred: no destroy loop for float elements (weak copy 0x8000D844, resize 0x8032BE88)
template <>
inline void __vec_storage_unk<float>::__shrink_unk(unsigned int newSize) {
    unsigned int n = mSize - newSize;
    unsigned int& size = mSize;
    size -= n;
}

template <class T>
struct __vec_imp_unk : public __vec_storage_unk<T> {
    ~__vec_imp_unk() {}
    // inferred: generic elements are copy-constructed in place (0x80476ED4)
    void __push_fast_unk(unsigned int n, const T& x) {
        new (this->mData + n) T(x);
        this->mSize++;
    }
    // inferred: appends n value-initialised elements in place (resize, weak copy 0x8032B314)
    void __append_n_unk(unsigned int n) {
        T x = T();
        T* p = this->mData + this->mSize;
        for (; n != 0; n--, p++) {
            new (p) T(x);
            this->mSize++;
        }
    }
};

// inferred: pointer elements are stored after the size is incremented (0x800DB3EC)
template <class T>
struct __vec_imp_unk<T*> : public __vec_storage_unk<T*> {
    ~__vec_imp_unk() {}
    void __push_fast_unk(unsigned int n, T* const& x) {
        this->mSize = n + 1;
        ((void**)this->mData)[this->mSize - 1] = x;
    }
    void __append_n_unk(unsigned int n) {
        memset(this->mData + this->mSize, 0, n * sizeof(T*));
        this->mSize += n;
    }
};

// inferred: temporary buffer used when push_back reallocates (0x800DB3EC): storage, number of
// elements, capacity, allocator reference and index of the first element. Names invented. The
// destructor lives in a base class: the original tests the buffer address for null before
// destroying it (addic. r3, r1, 0x14), and keeps its fields in memory.
template <class T>
struct __vec_split_base_unk {
    // inferred: the fields are cleared through a word pointer (as in basic_string::__zero): the
    // stores then count as stores through a pointer, and the capacity of the vector, whose address
    // the buffer keeps, is reloaded after them (0x80476ED4)
    __vec_split_base_unk(void* alloc) {
        unsigned int* w = (unsigned int*)this;
        w[0] = 0;
        w[1] = 0;
        w[2] = 0;
        mAlloc = alloc;
    }
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

// inferred: generic elements are copy-constructed one by one (0x80476ED4, 0x104-byte elements)
template <class T>
struct __vec_split_unk : public __vec_split_base_unk<T> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<T>(alloc), mStart(0) {}
    T* begin() { return this->mData + mStart; }
    void push_back(const T& x) {
        new (begin() + this->mSize) T(x);
        this->mSize++;
    }
    // inferred: appends n copies of x (resize, weak copy 0x8032B314)
    void __append_n_unk(unsigned int n, const T& x) {
        T* p = begin() + this->mSize;
        for (; n != 0; n--, p++) {
            new (p) T(x);
            this->mSize++;
        }
    }
    // inferred: moves [first, last) in front of the stored elements, last one first
    void __push_front_unk(T* first, T* last) {
        T* p = begin();
        while (last > first) {
            new (--p) T(*--last);
            mStart--;
            this->mSize++;
        }
    }
    // inferred: same loop for the reallocation of resize (0x8032B314 keeps the two decrements apart)
    void __push_front_resize_unk(T* first, T* last) {
        T* p = begin();
        while (last > first) {
            --p;
            --last;
            new (p) T(*last);
            mStart--;
            this->mSize++;
        }
    }
    unsigned int mStart;
};

// inferred: pointer elements are stored directly and moved with memcpy (weak copy 0x800DB3EC)
template <class T>
struct __vec_split_unk<T*> : public __vec_split_base_unk<T*> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<T*>(alloc), mStart(0) {}
    T** begin() { return this->mData + mStart; }
    // inferred: appends n null pointers (resize, 0x801FFA04)
    void __append_n_unk(unsigned int n, T* const&) {
        memset(begin() + this->mSize, 0, n * sizeof(T*));
        this->mSize += n;
    }
    void push_back(T* const& x) {
        begin()[this->mSize] = x;
        this->mSize++;
    }
    // inferred: moves [first, last) in front of the stored elements
    void __push_front_resize_unk(T** first, T** last) { __push_front_unk(first, last); }
    void __push_front_unk(T** first, T** last) {
        // inferred: a byte difference; `last - first` would fold to the size (0x800DB3EC computes
        // (data + size) - data)
        unsigned int n = ((char*)last - (char*)first) / (int)sizeof(T*);
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(T*));
        memset(first, 0, n * sizeof(T*));
        this->mSize += n;
    }
    unsigned int mStart;
};

// inferred: int elements take the same path as pointers (ChaserNpc::ChaserNpc, 0x801805A4)
template <>
struct __vec_imp_unk<int> : public __vec_storage_unk<int> {
    ~__vec_imp_unk() {}
    void __push_fast_unk(unsigned int n, const int& x) {
        this->mSize = n + 1;
        this->mData[this->mSize - 1] = x;
    }
};

template <>
struct __vec_split_unk<int> : public __vec_split_base_unk<int> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<int>(alloc), mStart(0) {}
    int* begin() { return this->mData + mStart; }
    void push_back(const int& x) {
        begin()[this->mSize] = x;
        this->mSize++;
    }
    void __push_front_resize_unk(int* first, int* last) { __push_front_unk(first, last); }
    void __push_front_unk(int* first, int* last) {
        unsigned int n = ((char*)last - (char*)first) / (int)sizeof(int);
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(int));
        memset(first, 0, n * sizeof(int));
        this->mSize += n;
    }
    unsigned int mStart;
};

// inferred: float elements are moved as raw words like int (resize 0x8032BE88, constructor 0x8032B6F8)
template <>
struct __vec_imp_unk<float> : public __vec_storage_unk<float> {
    ~__vec_imp_unk() {}
    void __push_fast_unk(unsigned int n, const float& x) {
        this->mSize = n + 1;
        this->mData[this->mSize - 1] = x;
    }
    void __append_n_unk(unsigned int n) {
        memset(this->mData + this->mSize, 0, n * sizeof(float));
        this->mSize += n;
    }
};

template <>
struct __vec_split_unk<float> : public __vec_split_base_unk<float> {
    __vec_split_unk(void* alloc) : __vec_split_base_unk<float>(alloc), mStart(0) {}
    float* begin() { return this->mData + mStart; }
    void push_back(const float& x) {
        begin()[this->mSize] = x;
        this->mSize++;
    }
    void __append_n_unk(unsigned int n, const float&) {
        memset(begin() + this->mSize, 0, n * sizeof(float));
        this->mSize += n;
    }
    void __push_front_resize_unk(float* first, float* last) { __push_front_unk(first, last); }
    void __push_front_unk(float* first, float* last) {
        unsigned int n = ((char*)last - (char*)first) / (int)sizeof(float);
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(float));
        memset(first, 0, n * sizeof(float));
        this->mSize += n;
    }
    unsigned int mStart;
};

// inferred: temporary buffer of vector<T*>::reserve (0x800C3834): {data, size, capacity}; the
// elements are moved as raw bytes and the size is cleared before the storage is freed
template <class T>
struct __vec_raw_buf_unk {
    __vec_raw_buf_unk() : mData(0), mSize(0), mCap(0) {}
    ~__vec_raw_buf_unk() {
        if (mData != 0) {
            mSize = 0;
            ::operator delete(mData);
        }
    }
    void __append_raw_unk(T* first, T* last) {
        unsigned int n = ((char*)last - (char*)first) / (int)sizeof(T);
        memcpy(mData + mSize, first, n * sizeof(T));
        memset(first, 0, n * sizeof(T));
        mSize += n;
    }
    T* mData;
    unsigned int mSize;
    unsigned int mCap;
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
    // inferred: weak copy 0x8032B6F8 (float elements)
    explicit vector(size_type n);
    void __init_n_unk(size_type n);
    template <class Iterator> vector(Iterator first, Iterator last);
    // inferred: weak copies 0x80513D34 and 0x80513EC4 (game/NewCampPartsGmk.cpp)
    vector(const vector& x) { __init_unk(x.mData, x.mData + x.mSize); }
    ~vector() {}

    T* begin() { return this->mData; }
    T* end() { return this->mData + this->mSize; }
    size_type size() const { return this->mSize; }
    size_type capacity() const { return this->mCap; }
    // inferred: weak copy 0x80120C20 (game/MotionCommand_unk.cpp)
    bool empty() const { return this->mSize == 0; }
    T& operator[](size_type i) { return this->mData[i]; }
    size_type max_size() const { return size_type(-1) / sizeof(T); }
    // inferred: weak copies of size 0x2F4 for pointer elements (0x800DB3EC, 0x80244CCC, ...),
    // inlined in atn::LoadMaterial::vf0C (0x804742F4). Defined out of the class and split into
    // small out-of-class helpers: a function that is not declared inline is still inlined by
    // `-inline auto` when it has at most about 13 statements, and its literals go to the file pool
    // (the three "vector length error" messages share one address, as in the original).
    // NONMATCHING (weak copy 0x800DB3EC, compiled standalone): only the fast path differs, the
    // original loads the size before the capacity (and keeps the capacity in r5); tried accessors,
    // the comparison reversed, if/else forms and a helper for the length check.
    void push_back(const T& x);
    // inferred: inlined in atn::D2A_Message::vf18 (0x801F2A8C, std::wstring elements); pointer
    // elements take another buffer (atn::SequenceHolder::SequenceHolder, 0x800C3834)
    void reserve(size_type n) { __reserve_unk(n, (T*)0); }
    // inferred: inlined in the d2anime loader (0x801FFA04, float* elements): new elements are
    // zeroed, removed ones are dropped through __shrink_unk
    void resize(size_type n) {
        if (n > this->mSize) {
            __append_unk(n - this->mSize);
        } else if (n < this->mSize) {
            this->__shrink_unk(n);
        }
    }
    // inferred: reserve with a length check, elements moved as raw bytes (atn/Xsca_unk 0x805BD4B8,
    // 8-byte entries); the check is out of the class so that its message goes to the file pool
    void __reserve_checked_unk(size_type n) {
        if (n > this->mCap) {
            __vec_raw_buf_unk<T> buf;
            __check_length_unk(n);
            buf.mData = __allocate(n);
            buf.mCap = n;
            buf.__append_raw_unk(this->mData, this->mData + this->mSize);
            this->mSize = 0;
            __vec_swap_unk(this->mCap, buf.mCap);
            __vec_swap_unk(this->mData, buf.mData);
            __vec_swap_unk(this->mSize, buf.mSize);
        }
    }
    void __check_length_unk(size_type n);
    template <class U>
    void __reserve_unk(size_type n, U**) {
        if (this->mCap < n) {
            __vec_raw_buf_unk<T> buf;
            buf.mData = __allocate(n);
            buf.mCap = n;
            buf.__append_raw_unk(this->mData, this->mData + this->mSize);
            this->mSize = 0;
            __vec_swap_unk(this->mCap, buf.mCap);
            __vec_swap_unk(this->mData, buf.mData);
            __vec_swap_unk(this->mSize, buf.mSize);
        }
    }
    template <class U>
    void __reserve_unk(size_type n, U*) {
        if (this->mCap < n) {
            __vec_storage_unk<T> buf;
            buf.mData = __allocate(n);
            buf.mCap = n;
            buf.__append_raw_unk(this->mData, this->mData + this->mSize);
            __vec_swap_unk(this->mCap, buf.mCap);
            __vec_swap_unk(this->mData, buf.mData);
            __vec_swap_unk(this->mSize, buf.mSize);
        }
    }

    // inferred: insertion of one element (weak copy 0x800C507C, 8-byte elements of atn/Shader_unk);
    // the out-of-line helper takes a flag by reference.
    // NONMATCHING (0x800C507C, 66%): the original lays out the length check of the slow path before
    // the in-place path, and its copy loops differ.
    T* insert(T* pos, const T& x) {
        bool flag = false;
        return __insert_one_unk(pos, x, flag);
    }
    T* __insert_one_unk(T* pos, const T& x, const bool& flag);

private:
    // inferred: growth of resize (0x801FFA04, pointer elements); the length check is defined out
    // of the class (its "vector length error" goes to the file pool) and auto-inlined
    void __append_unk(size_type n) {
        if (n > this->mCap || this->mSize > this->mCap - n) {
            // inferred: the sum goes through a helper (0x8032B314 adds size + n in that order)
            __check_grow_unk(__vec_sum_unk(this, n) - this->mCap);
            __recommend_cap_unk(size() + n);
        } else {
            this->__append_n_unk(n);
            return;
        }
        __vec_split_unk<T> buf(&this->mCap);
        size_type cap = __grow_cap_unk(this->mSize + n - this->mCap);
        buf.mData = __allocate(cap);
        buf.mCap = cap;
        buf.mStart = this->mSize;
        buf.__append_n_unk(n, T());
        __swap_out_resize_unk(buf);
    }
    // inferred: reallocation of resize; float elements reach it through a separate inline level
    // (0x8032BE88 keeps the capacity of the dead __recommend_cap_unk in another register)
    void __append_realloc_unk(size_type n) {
        __vec_split_unk<T> buf(&this->mCap);
        size_type cap = __grow_cap_unk(this->mSize + n - this->mCap);
        buf.mData = __allocate(cap);
        buf.mCap = cap;
        buf.mStart = this->mSize;
        buf.__append_n_unk(n, T());
        __swap_out_unk(buf);
    }
    friend size_type __vec_sum_unk(const vector* v, size_type n) { return v->mSize + n; }
    void __check_grow_unk(size_type n);
    void __check_append_unk(size_type n);
    void __init_unk(const T* first, const T* last);
    size_type __grow_cap_unk(size_type n);
    // inferred: growth without the length checks; push_back computes it once for one element and drops the
    // result (dead code that stays when push_back is inlined into a large function, CrowdManager 0x801EB188)
    size_type __recommend_unk(size_type n) {
        size_type cap = this->mCap;
        if (cap < max_size() / 3) {
            cap += __vec_max_unk(size_type((cap + 1) * 3 / 5), n);
        } else if (cap < max_size() / 3 * 2) {
            cap += __vec_max_unk(size_type((cap + 1) / 2), n);
        } else {
            cap = max_size();
        }
        return cap;
    }
    // inferred: same as __recommend_unk, reading the capacity through capacity() (dead code of resize,
    // 0x8032B314)
    size_type __recommend_cap_unk(size_type n) {
        size_type cap = capacity();
        if (cap < max_size() / 3) {
            cap += __vec_max_unk(size_type((cap + 1) * 3 / 5), n);
        } else if (cap < max_size() / 3 * 2) {
            cap += __vec_max_unk(size_type((cap + 1) / 2), n);
        } else {
            cap = max_size();
        }
        return cap;
    }
    // inferred: allocation of the temporary buffer (split off so that push_back stays within the
    // auto-inline limit)
    void __vec_set_buf_unk(__vec_split_unk<T>& buf, size_type cap) {
        buf.mData = __allocate(cap);
        buf.mCap = cap;
        buf.mStart = this->mSize;
    }
    void __swap_out_resize_unk(__vec_split_unk<T>& buf) {
        buf.__push_front_resize_unk(this->mData, this->mData + this->mSize);
        this->mSize = 0;
        __vec_swap_unk(this->mCap, buf.mCap);
        __vec_swap_unk(this->mData, buf.mData);
        __vec_swap_unk(this->mSize, buf.mSize);
    }
    void __swap_out_unk(__vec_split_unk<T>& buf) {
        buf.__push_front_unk(this->mData, this->mData + this->mSize);
        this->mSize = 0;
        __vec_swap_unk(this->mCap, buf.mCap);
        __vec_swap_unk(this->mData, buf.mData);
        __vec_swap_unk(this->mSize, buf.mSize);
    }
public:
    // inferred: also used directly by the weak copy 0x8032B6F8 (game/Em403WarLion.cpp)
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
    size_type n = this->mSize;
    if (n < this->mCap) {
        this->__push_fast_unk(n, x);
        return;
    }
    if (max_size() - this->mCap < 1) {
        __msl_error("vector length error");
    }
    __recommend_unk(1);
    __vec_split_unk<T> buf(&this->mCap);
    __vec_set_buf_unk(buf, __grow_cap_unk(this->mSize + 1 - this->mCap));
    buf.push_back(x);
    __swap_out_unk(buf);
}

template <class T>
T* vector<T>::__insert_one_unk(T* pos, const T& x, const bool& flag) {
    if (this->mSize < this->mCap) {
        T* last = this->mData + this->mSize;
        const T* px = &x;
        if (pos <= px && px < last) {
            px++;
        }
        this->mSize++;
        T* dst = last + 1;
        while (last > pos) {
            *--dst = *--last;
        }
        *pos = *px;
        return pos;
    }
    if (max_size() - this->mCap < 1) {
        __msl_error("vector length error");
    }
    __recommend_unk(1);
    T* old = this->mData;
    size_type idx = pos - old;
    size_type cap = __grow_cap_unk(1);
    T* p = __allocate(cap);
    this->mData = p;
    this->mCap = cap;
    p[idx] = x;
    if (old != 0) {
        T* src = old;
        T* end = old + idx;
        T* dst = this->mData;
        while (src < end) {
            *dst++ = *src++;
        }
        dst++;
        end = old + this->mSize;
        while (src < end) {
            *dst++ = *src++;
        }
        ::operator delete(old);
    }
    this->mSize++;
    return this->mData + idx;
}

template <class T>
void vector<T>::__check_length_unk(size_type n) {
    if (n > max_size()) {
        __msl_error("vector length error");
    }
}

template <class T>
void vector<T>::__check_grow_unk(size_type n) {
    if (n > max_size() - capacity()) {
        __msl_error("vector length error");
    }
}

template <class T>
void vector<T>::__check_append_unk(size_type n) {
    if (max_size() - this->mCap < this->mSize + n) {
        __msl_error("vector length error");
    }
}

// inferred: vector of n value-initialised elements (weak copy 0x8032B6F8, float elements)
template <class T>
vector<T>::vector(size_type n) {
    __init_n_unk(n);
}

// inferred: body of vector(n); out of the class so that its message goes to the file pool
template <class T>
void vector<T>::__init_n_unk(size_type n) {
    if (n != 0) {
        if (n > max_size()) {
            __msl_error("vector length error");
        }
        this->mData = __allocate(n);
        this->mCap = n;
        this->__append_n_unk(n);
    }
}

// inferred: copy of a range into an empty vector (names invented)
template <class T>
void vector<T>::__init_unk(const T* first, const T* last) {
    size_type n = last - first;
    if (n != 0) {
        if (n > max_size()) {
            __msl_error("vector length error");
        }
        this->mData = __allocate(n);
        this->mCap = n;
        T* p = this->mData + this->mSize;
        for (; first != last; ++first, ++p) {
            new (p) T(*first);
            ++this->mSize;
        }
    }
}

template <class T>
typename vector<T>::size_type vector<T>::__grow_cap_unk(size_type n) {
    if (n > max_size() - this->mCap) {
        __msl_error("vector length error");
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
        __msl_error("vector length error");
    }
    return cap;
}

// inferred: resize of float elements (weak copy 0x8032BE88)
template <>
inline void vector<float>::__append_unk(size_type n) {
    if (n > this->mCap || this->mSize > this->mCap - n) {
        __check_grow_unk(__vec_sum_unk(this, n) - this->mCap);
        __recommend_cap_unk(size() + n);
    } else {
        this->__append_n_unk(n);
        return;
    }
    __append_realloc_unk(n);
}

} // namespace std

#endif
