#ifndef EVENT_EVENTVECTOR_UNK_H
#define EVENT_EVENTVECTOR_UNK_H

// inferred: reallocation of push_back for the vectors of Event::Actor and Event::Path. The original calls the
// growth (0x80544650/0x80544748), the storage of the buffer with the length check of the new capacity
// (0x80545C3C/0x80545B90), the copies and the move of the old elements (0x80545084/0x805455CC) one by one
// from the slow path (DataList_unk<Actor>::Add_unk 0x80543108, ParseACID_unk 0x8057B4A0, push_back<Path>
// 0x8057DB14), while the generic code of std/vector.h allocates the buffer in one helper. Only included by
// Event/Sequence.cpp and Event/EventParser_unk.cpp (the only users of these two push_back): a template body
// in std/vector.h takes @N labels in every unit that includes it. Names invented.

#include "Event/Path.h"
#include "Event/Actor.h"
#include "Event/Sequence.h"
#include "std/vector.h"

namespace std {

template <class T>
struct __vec_event_unk {
    static unsigned int __grow_by_unk(vector<T>& v, unsigned int n);
    // inferred: the first argument (the new size) is not used
    static void __reserve_unk(__vec_split_unk<T>& buf, unsigned int, unsigned int cap);
    static void __push_front_unk(__vec_split_unk<T>& buf, T* first, T* last);
    static void __push_front_loop_unk(__vec_split_unk<T>& buf, T* first, T* last);
    static void __realloc_unk(vector<T>& v, unsigned int n, const T& x);
};

// inferred: growth without the length check of the new capacity
template <class T>
unsigned int __vec_event_unk<T>::__grow_by_unk(vector<T>& v, unsigned int n) {
    if (n > v.max_size() - v.mCap) {
        __msl_error("vector length error");
    }
    unsigned int cap = v.mCap;
    if (cap < v.max_size() / 3) {
        cap += __vec_max_unk((unsigned int)((cap + 1) * 3 / 5), n);
    } else if (cap < v.max_size() / 3 * 2) {
        cap += __vec_max_unk((unsigned int)((cap + 1) / 2), n);
    } else {
        cap = v.max_size();
    }
    return cap;
}

// inferred: the assertions only count for the inlining budget (R5 of docs/inlining.md: without them the
// function is small enough to be inlined into the slow path)
template <class T>
void __vec_event_unk<T>::__reserve_unk(__vec_split_unk<T>& buf, unsigned int, unsigned int cap) {
    MSL_VEC_ASSERT_UNK(0);
    MSL_VEC_ASSERT_UNK(0);
    MSL_VEC_ASSERT_UNK(0);
    MSL_VEC_ASSERT_UNK(0);
    if (cap > (unsigned int)-1 / sizeof(T)) {
        __msl_error("vector length error");
    }
    buf.mData = __vec_alloc_sel_unk<T>::alloc(cap);
    buf.mCap = cap;
}

// inferred: moves the old elements in front of the copies, last one first (comparing first < last)
template <class T>
void __vec_event_unk<T>::__push_front_unk(__vec_split_unk<T>& buf, T* first, T* last) {
    __push_front_loop_unk(buf, first, last);
}

template <class T>
void __vec_event_unk<T>::__push_front_loop_unk(__vec_split_unk<T>& buf, T* first, T* last) {
    T* p = buf.begin();
    while (first < last) {
        --p;
        --last;
        new (p) T(*last);
        buf.mStart--;
        buf.mSize++;
    }
}

template <class T>
void __vec_event_unk<T>::__realloc_unk(vector<T>& v, unsigned int n, const T& x) {
    __vec_split_unk<T> buf(&v.mCap);
    unsigned int cap = __grow_by_unk(v, v.mSize + n - v.mCap);
    __reserve_unk(buf, v.mSize + n, cap);
    buf.mStart = v.mSize;
    buf.__append_copies_unk(n, x);
    __push_front_unk(buf, v.mData, v.mData + v.mSize);
    __vec_swap_unk(v.mCap, buf.mCap);
    __vec_swap_unk(v.mData, buf.mData);
    __vec_swap_unk(v.mSize, buf.mSize);
}

// inferred: the destructor of the buffer counts the elements before taking the end pointer (weak copies
// 0x80544840 Event::Actor and 0x80545290 Event::Path; the generic order of std/vector.h suits the other element
// types, Event/EventParser_unk ParseMOID_unk...)
template <>
inline __vec_split_gbase_unk<Event::Actor>::~__vec_split_gbase_unk() {
    if (mData != 0) {
        unsigned int n = mSize;
        Event::Actor* p = mData + mSize;
        mSize = 0;
        for (; n != 0; n--) {
            --p;
            p->~Actor();
        }
        ::operator delete(mData);
    }
}

template <>
inline __vec_split_gbase_unk<Event::Path>::~__vec_split_gbase_unk() {
    if (mData != 0) {
        unsigned int n = mSize;
        Event::Path* p = mData + mSize;
        mSize = 0;
        for (; n != 0; n--) {
            --p;
            p->~Path();
        }
        ::operator delete(mData);
    }
}

// inferred: clear of the effect entries takes the end pointer before the count (weak copy 0x8053E588)
template <>
inline void __vec_storage_unk<Event::EffectEntry24_unk>::clear() {
    Event::EffectEntry24_unk* p = mData + mSize;
    unsigned int n = mSize;
    unsigned int& size = mSize;
    size -= n;
    for (; n != 0; n--) {
        --p;
        p->~EffectEntry24_unk();
    }
}

template <>
inline void vector<Event::Actor>::__insert_realloc_unk(size_type n, const Event::Actor& x) {
    __vec_event_unk<Event::Actor>::__realloc_unk(*this, n, x);
}

template <>
inline void vector<Event::Path>::__insert_realloc_unk(size_type n, const Event::Path& x) {
    __vec_event_unk<Event::Path>::__realloc_unk(*this, n, x);
}

// inferred: fast path of push_back of elements with a copy constructor: the original reads the size and the
// capacity directly and calls the construct of the allocator (weak copies 0x80542684 EffectEntry24_unk, called
// by Sequence::fn_8054204C, and 0x8057DC38 Path, called by ParsePATH_unk)
template <>
inline void vector<Event::EffectEntry24_unk>::push_back(const Event::EffectEntry24_unk& x) {
    if (this->mSize < this->mCap) {
        this->__alloc_unk().construct(this->mData + this->mSize, x);
        ++this->mSize;
    } else {
        __push_back_slow_unk(x);
    }
}

template <>
inline void vector<Event::Path>::push_back(const Event::Path& x) {
    if (this->mSize < this->mCap) {
        this->__alloc_unk().construct(this->mData + this->mSize, x);
        ++this->mSize;
    } else {
        __push_back_slow_unk(x);
    }
}

// inferred: the copy of vector<ColorF_unk> (weak copy 0x80544398) takes the range of the source through two
// inline accessors (the generic copy of std/vector.h reads mData and mSize directly; changing it there moves the
// registers of vector<atn::Light> in game/NewCampPartsGmk.cpp). Names invented.
template <class T>
inline const T* __vec_cbegin_unk(const vector<T>& v) { return v.mData; }
template <class T>
inline const T* __vec_cend_unk(const vector<T>& v) { return v.mData + v.mSize; }

template <>
inline vector<ColorF_unk>::vector(const vector<ColorF_unk>& x) {
    MSL_VEC_ASSERT10_UNK;
    MSL_VEC_ASSERT10_UNK;
    __init_unk(__vec_cbegin_unk(x), __vec_cend_unk(x));
}

// inferred: same form for vector<Event::ActorPair_unk> (weak copy 0x80543F60)
template <>
inline vector<Event::ActorPair_unk>::vector(const vector<Event::ActorPair_unk>& x) {
    MSL_VEC_ASSERT10_UNK;
    MSL_VEC_ASSERT10_UNK;
    __init_unk(__vec_cbegin_unk(x), __vec_cend_unk(x));
}

} // namespace std

#endif
