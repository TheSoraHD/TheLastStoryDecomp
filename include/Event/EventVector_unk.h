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

template <>
inline void vector<Event::Actor>::__insert_realloc_unk(size_type n, const Event::Actor& x) {
    __vec_event_unk<Event::Actor>::__realloc_unk(*this, n, x);
}

template <>
inline void vector<Event::Path>::__insert_realloc_unk(size_type n, const Event::Path& x) {
    __vec_event_unk<Event::Path>::__realloc_unk(*this, n, x);
}

} // namespace std

#endif
