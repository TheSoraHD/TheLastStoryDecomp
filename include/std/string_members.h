#ifndef STD_STRING_MEMBERS_H
#define STD_STRING_MEMBERS_H

// Out-of-line members of std::basic_string. Game files only include std/string.h and call these
// out of line: their objects have no copies of them (the error messages are not in their string
// pools, e.g. atn/LH_Motion.cpp), so the members were compiled once elsewhere (inferred: an
// explicit instantiation in MSL C++). Exceptions: game/GimmickManager.cpp (pool 0x80751F60) and
// game/EquipItemData.cpp have their messages in their pools, so they include this file (the weak
// copies are dropped by the linker).

#include "std/string.h"

namespace std {

// 0x80069BF4 (char). NONMATCHING (standalone compile, 40 differing lines of 85): the original
// spills n to the highest stack slot and copies it before the second __min (as if passed to an
// inner function); ours spills it to the lowest slot.
template <class T, class Tr, class A>
basic_string<T, Tr, A>::basic_string(const basic_string& str, size_type pos, size_type n) {
    mLong.mCap = 0;
    mLong.mSize = 0;
    mLong.mData = 0;
    size_type sz = str.size();
    reserve(pos <= sz ? __min(n, sz - pos) : 0);
    const T* p;
    size_type len;
    if (!str.is_long()) {
        p = str.mShort.mData;
        len = str.mShort.mSize & 0x7F;
    } else {
        p = str.mLong.mData;
        len = str.mLong.mSize;
    }
    if (pos > len) {
        __msl_error("basic_string: out_of_range");
    }
    size_type rlen = __min(n, len - pos);
    p += pos;
    __replace_unk(0, 0, p, p + rlen, random_access_iterator_tag());
}

template <class T, class Tr, class A>
void basic_string<T, Tr, A>::reserve(size_type res_arg) {
    if (res_arg > max_size()) {
        __msl_error("basic_string::reserve length_error");
    }
    size_type cap;
    size_type sz;
    bool was_long = mFlags.mIsLong;
    if (!was_long) {
        sz = mShortFlags.mSize;
        cap = __min_cap;
    } else {
        sz = mLong.mSize;
        cap = mFlags.mCap;
    }
    if (res_arg < sz) {
        res_arg = sz;
    }
    res_arg = (res_arg + 1 <= __min_cap) ? (size_type)__min_cap : (res_arg + 16) & ~15;
    if (res_arg != cap) {
        T* new_data;
        T* p;
        bool now_long;
        if (res_arg == __min_cap) {
            now_long = false;
            new_data = mShort.mData;
            p = mLong.mData;
        } else {
            if (res_arg > cap) {
                new_data = __allocate(res_arg);
            } else {
                new_data = __allocate(res_arg);
                if (new_data == 0) {
                    return;
                }
            }
            now_long = true;
            p = was_long ? mLong.mData : mShort.mData;
        }
        memmove(new_data, p, sz * sizeof(T));
        new_data[sz] = 0;
        if (was_long) {
            ::operator delete(p);
        }
        mFlags.mIsLong = now_long;
        if (!now_long) {
            mShortFlags.mSize = sz;
        } else {
            mLong.mData = new_data;
            mLong.mSize = sz;
            mFlags.mCap = res_arg;
        }
    }
}

template <class T, class Tr, class A>
basic_string<T, Tr, A>& basic_string<T, Tr, A>::__replace_unk(size_type pos, size_type n1, const T* j1, const T* j2, random_access_iterator_tag tag) {
    bool was_long = mFlags.mIsLong;
    size_type new_sz;
    T* p;
    size_type sz;
    size_type cap;
    if (!was_long) {
        p = mShort.mData;
        cap = __min_cap;
        sz = mShortFlags.mSize;
    } else {
        p = mLong.mData;
        cap = mFlags.mCap;
        sz = mLong.mSize;
    }
    if (pos > sz) {
        __msl_error("basic_string: out_of_range");
    }
    size_type rlen = __min(n1, sz - pos);
    size_type n2 = j2 - j1;
    if (n2 > max_size() || sz - rlen > max_size() - n2) {
        __msl_error("basic_string: length_error");
    }
    new_sz = n2 + (sz - rlen);
    size_type tail = sz - (pos + rlen);
    if (new_sz >= cap) {
        cap = (cap + 15) & ~15;
        while (cap < new_sz + 1) {
            cap = (cap * 2 + 15) & ~15;
        }
        T* new_data = __allocate(cap);
        if (pos != 0) {
            memcpy(new_data, p, pos * sizeof(T));
        }
        T* d = new_data + pos;
        memmove(d, j1, (j2 - j1) * sizeof(T));
        if (tail != 0) {
            memcpy(d + n2, p + pos + rlen, tail * sizeof(T));
        }
        __assign(new_data[new_sz], T());
        if (was_long) {
            ::operator delete(p);
        } else {
            mFlags.mIsLong = 1;
        }
        mLong.mData = new_data;
        mLong.mSize = new_sz;
        mFlags.mCap = cap;
    } else {
        basic_string tmp;
        if (tail != 0 && p + pos + n2 < j2 && j2 <= p + sz) {
            tmp.__replace_unk(0, tmp.size(), j1, j2, random_access_iterator_tag());
            j1 = tmp.data();
        }
        if (tail != 0) {
            memmove(p + pos + n2, p + pos + rlen, tail * sizeof(T));
        }
        memmove(p + pos, j1, n2 * sizeof(T));
        __assign(p[new_sz], T());
        if (!was_long) {
            mShortFlags.mSize = new_sz;
        } else {
            mLong.mSize = new_sz;
        }
    }
    return *this;
}

} // namespace std

#endif
