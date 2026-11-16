#ifndef STD_STRING_REPLACE_UNK_H
#define STD_STRING_REPLACE_UNK_H

// basic_string::replace(pos, n1, str). Defined out of the class and only in the files that use it: its named
// parameters and locals would renumber the @N labels of every file that includes std/string.h.

#include "std/string.h"

namespace std {

// inferred: data and size of `str` from one test (game/GameChoiceWindow fn_803B3D2C, wchar_t)
template <class T, class Tr, class A>
inline basic_string<T, Tr, A>& basic_string<T, Tr, A>::replace(size_type pos, size_type n1, const basic_string& str) {
    const T* p;
    size_type n;
    if (!str.is_long()) {
        p = (const T*)&str + 1;
        n = str.mShort.mSize & 0x7F;
    } else {
        p = str.mLong.mData;
        n = str.mLong.mSize;
    }
    return do_replace(pos, n1, p, p + n, random_access_iterator_tag());
}

} // namespace std

#endif
