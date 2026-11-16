#ifndef STD_STRING_FIND_LAST_OF_H
#define STD_STRING_FIND_LAST_OF_H

// basic_string::find_last_of(c, pos), as in MSL: forwards to rfind. Defined out of the class and only
// in the files that use it: its named parameters would renumber the @N labels of every file that
// includes std/string.h.

#include "std/string.h"

namespace std {

// inferred: the extra inline level over rfind swaps the registers of the position and of the data
// pointer (Event/Sequence fn_8053F818, atn/FileList fn_8006AC08, fn_8006AD24, fn_8006B2D8); atn/LH_Motion
// (0x80476220) calls rfind directly
template <class T, class Tr, class A>
inline typename basic_string<T, Tr, A>::size_type basic_string<T, Tr, A>::find_last_of(T c, size_type pos) const {
    return rfind(c, pos);
}

} // namespace std

#endif
