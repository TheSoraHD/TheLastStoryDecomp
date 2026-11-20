#ifndef EVENT_EVENTSPVECTOR_UNK_H
#define EVENT_EVENTSPVECTOR_UNK_H

// inferred: vectors of boost::shared_ptr of the event tree (channels of Event::Section, actions of
// Event::Channel, labels and sections of Event::Cut, cuts of Event::Sequence). Their push_back differs from
// the generic one of std/vector.h: the original keeps max_size() in a register and divides it at run time
// (a local of the growth), keeps an unreachable copy of the fast path after the length check of expand_by,
// moves the old elements comparing first < last and destroys the buffer element by element three inline
// levels below its destructor. The destroy loop of their destructors is also one level below
// __vec_storage_unk::__shrink_unk (Event::Channel::~Channel 0x8053AD50, Event::Cut::~Cut). Each user .cpp
// includes this header first and specializes vector<T>::__push_unk and __vec_storage_unk<T>::__shrink_unk
// for its element types with the helpers below (an explicit specialization here would take @N labels in
// every unit that includes it). Names invented.

#include <boost/shared_ptr.hpp>
#include "std/vector.h"

namespace std {

template <class T>
struct __vec_sp_unk;

// inferred: base of the reallocation buffer: destroys what is left (nothing after the move) one by one
template <class T>
struct __vec_split_gbase_unk<boost::shared_ptr<T> > {
    typedef boost::shared_ptr<T> E;
    __vec_split_gbase_unk(void* alloc) : mData(0), mSize(0), mCap(0), mAlloc(alloc) {}
    ~__vec_split_gbase_unk() {
        if (mData != 0) {
            __shrink_unk(0);
            ::operator delete(mData);
        }
    }
    void __shrink_unk(unsigned int newSize) { __vec_sp_unk<E>::__shrink_buf_unk(mData, mSize, newSize); }
    E* mData;
    unsigned int mSize;
    unsigned int mCap;
    void* mAlloc;
};

template <class T>
struct __vec_split_unk<boost::shared_ptr<T> > : public __vec_split_gbase_unk<boost::shared_ptr<T> > {
    typedef boost::shared_ptr<T> E;
    __vec_split_unk(void* alloc) : __vec_split_gbase_unk<E>(alloc), mStart(0) {}
    ~__vec_split_unk() { __destroy_front_unk(); }
    void __destroy_front_unk() {
        __vec_sp_unk<E>::__destroy_front_unk(begin(), begin() + this->mSize);
        this->mSize = 0;
    }
    enum { __clear_src_unk = 0 };
    E* begin() { return this->mData + mStart; }
    void __append_copies_unk(unsigned int n, const E& x) {
        E* p = mStart + this->mData + this->mSize;
        for (; n != 0; n--, p++) {
            new (p) E(x);
            this->mSize++;
        }
    }
    unsigned int mStart;
};

template <class T>
struct __vec_sp_unk {
    static void __push_slow_unk(vector<T>& v, const T& x);
    static bool __expand_by_unk(vector<T>& v, unsigned int n, unsigned int c);
    static void __realloc_unk(vector<T>& v, unsigned int n, const T& x);
    static void __alloc_split_unk(vector<T>& v, __vec_split_unk<T>& buf, unsigned int n);
    static unsigned int __grow_cap_unk(vector<T>& v, unsigned int n);
    static unsigned int __grow_cap2_unk(vector<T>& v, unsigned int n);
    static void __push_front_unk(__vec_split_unk<T>& buf, T* first, T* last);
    static void __push_front_loop_unk(__vec_split_unk<T>& buf, T* first, T* last);
    // inferred: destroy loop of the vector (from __vec_storage_unk<T>::__shrink_unk)
    static void __shrink_loop_unk(__vec_storage_unk<T>& v, unsigned int newSize) {
        unsigned int n = v.mSize - newSize;
        T* p = v.mData + v.mSize;
        unsigned int& size = v.mSize;
        size -= n;
        for (; n != 0; n--) {
            --p;
            p->~T();
        }
    }
    // inferred: destroy loop of the reallocation buffer, three levels below its destructor; the end pointer
    // is computed by the caller and the count in the loop function (register order of the loop)
    static void __shrink_buf_unk(T* data, unsigned int& size, unsigned int newSize) { __shrink_buf2_unk(data, size, newSize); }
    static void __shrink_buf2_unk(T* data, unsigned int& size, unsigned int newSize) {
        __destroy_back_unk(data + size, size, newSize);
    }
    static void __destroy_back_unk(T* p, unsigned int& size, unsigned int newSize) {
        unsigned int n = size - newSize;
        size -= n;
        for (; n != 0; n--) {
            --p;
            p->~T();
        }
    }
    static void __destroy_front_unk(T* first, T* last) { __destroy_range_unk(first, last); }
    static void __destroy_range_unk(T* first, T* last) {
        while (last > first) {
            (--last)->~T();
        }
    }
};

// inferred: fast path, then expand_by (whose growth is dead code and which always fails: the copy of the
// fast path after it is unreachable) and the reallocation
template <class T>
void __vec_sp_unk<T>::__push_slow_unk(vector<T>& v, const T& x) {
    if (__expand_by_unk(v, 1, v.mCap)) {
        v.__push_fast_unk(v.mSize, x);
    } else {
        __realloc_unk(v, 1, x);
    }
}

template <class T>
bool __vec_sp_unk<T>::__expand_by_unk(vector<T>& v, unsigned int n, unsigned int c) {
    const unsigned int ms = v.max_size();
    if (n > ms - c) {
        __msl_error("vector length error");
    }
    unsigned int cap = c;
    const unsigned int third = ms / 3;
    if (cap < third) {
        cap += __vec_max_unk((unsigned int)((cap + 1) * 3 / 5), n);
    } else if (cap < third * 2) {
        cap += __vec_max_unk((unsigned int)((cap + 1) / 2), n);
    } else {
        cap = ms;
    }
    return false;
}

template <class T>
void __vec_sp_unk<T>::__realloc_unk(vector<T>& v, unsigned int n, const T& x) {
    __vec_split_unk<T> buf(&v.__alloc_unk());
    __alloc_split_unk(v, buf, n);
    buf.__append_copies_unk(n, x);
    __push_front_unk(buf, v.mData, v.mData + v.mSize);
    __vec_swap_unk(v.mCap, buf.mCap);
    __vec_swap_unk(v.mData, buf.mData);
    __vec_swap_unk(v.mSize, buf.mSize);
}

template <class T>
void __vec_sp_unk<T>::__alloc_split_unk(vector<T>& v, __vec_split_unk<T>& buf, unsigned int n) {
    unsigned int cap = __grow_cap_unk(v, v.mSize + n - v.mCap);
    buf.mData = __vec_alloc_sel_unk<T>::alloc(cap);
    buf.mCap = cap;
    buf.mStart = v.mSize;
}

template <class T>
unsigned int __vec_sp_unk<T>::__grow_cap_unk(vector<T>& v, unsigned int n) {
    return __grow_cap2_unk(v, n);
}

template <class T>
unsigned int __vec_sp_unk<T>::__grow_cap2_unk(vector<T>& v, unsigned int n) {
    const unsigned int ms = v.max_size();
    unsigned int cap = v.mCap;
    if (n > ms - cap) {
        __msl_error("vector length error");
    }
    const unsigned int third = ms / 3;
    if (cap < third) {
        cap += __vec_max_unk((unsigned int)((cap + 1) * 3 / 5), n);
    } else if (cap < third * 2) {
        cap += __vec_max_unk((unsigned int)((cap + 1) / 2), n);
    } else {
        cap = ms;
    }
    if (cap > v.max_size()) {
        __msl_error("vector length error");
    }
    return cap;
}

template <class T>
void __vec_sp_unk<T>::__push_front_unk(__vec_split_unk<T>& buf, T* first, T* last) {
    __push_front_loop_unk(buf, first, last);
}

template <class T>
void __vec_sp_unk<T>::__push_front_loop_unk(__vec_split_unk<T>& buf, T* first, T* last) {
    T* p = buf.begin();
    while (first < last) {
        --p;
        --last;
        new (p) T(*last);
        buf.mStart--;
        buf.mSize++;
    }
}

} // namespace std

#endif
