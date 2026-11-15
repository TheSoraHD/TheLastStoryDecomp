#ifndef STD_VECTOR_ASSIGN_H
#define STD_VECTOR_ASSIGN_H

#include "std/vector.h"

namespace std {

// inferred: min of MSL (by reference) used by the assignment below
template <class T>
inline const T& __vec_assign_min_unk(const T& a, const T& b) {
    return (b < a) ? b : a;
}

// inferred: growth of the capacity `cap` by at least `n` (as __recommend_unk, with the capacity given)
inline unsigned int __vec_grow_by_unk(unsigned int cap, unsigned int n, unsigned int max) {
    if (cap < max / 3) {
        cap += __vec_max_unk((unsigned int)((cap + 1) * 3 / 5), n);
    } else if (cap < max / 3 * 2) {
        cap += __vec_max_unk((unsigned int)((cap + 1) / 2), n);
    } else {
        cap = max;
    }
    return cap;
}

// NONMATCHING: 95.1% (atn::Light) and 92.1% (atn::Camera) - registers (the original keeps `need` in r31 and
// stores it before the length check) and the offset of the "vector length error" literal (the original emits
// these copies right after NewCampView_unk::fn_8052E1A0, their literal before "angle"; here at the end of the
// file: defined in the class body they would come in place, but a body there renumbers the @N of every unit)
// inferred: assignment of a range (MSL assign for forward iterators): a new storage when the capacity is too
// small, else the elements are assigned and the rest destroyed or constructed
template <class T>
inline vector<T>& vector<T>::operator=(const vector& x) {
    if (this != &x) {
        const T* first = x.mData;
        const T* last = x.mData + x.mSize;
        size_type need;
        size_type n = last - first;
        size_type cap = this->mCap;
        if (n > cap) {
            need = n - cap;
            __check_grow_unk(need);
            cap = __vec_grow_by_unk(cap, need, max_size());
        } else {
            const T* mid = first + __vec_assign_min_unk(n, this->mSize);
            T* p = this->mData;
            for (; first < mid; ++first, ++p) {
                *p = *first;
            }
            if (n < this->mSize) {
                this->__shrink_unk(n);
            } else if (this->mSize < n) {
                __construct_at_end_unk(mid, last);
            }
            return *this;
        }
        this->clear();
        if (this->mData != 0) {
            ::operator delete(this->mData);
            this->mData = 0;
            this->mCap = 0;
        }
        __check_length_unk(cap);
        this->mData = __allocate(cap);
        this->mCap = cap;
        __construct_at_end_unk(first, last);
    }
    return *this;
}

} // namespace std

#endif
