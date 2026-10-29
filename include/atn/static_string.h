#ifndef ATN_STATIC_STRING_H
#define ATN_STATIC_STRING_H

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

#include "atn/static_string_fwd.h"

namespace atn {

// atn::static_string<N, C, Tr> (name and template arguments from the RSO, e.g.
// SimpleVector<static_string<32,char,char_traits<char>>>): fixed string of N characters. For N = 32
// the constructor (0x80042740) and destructor (0x80041C0C) are out of line (weak copies of
// game/AreaEffConfig.cpp); the assignment is inlined (strlen + memcpy when the buffers differ).
// Member names other than the operators are invented.
template <int N, class C, class Tr>
class static_string {
public:
    static_string();
    ~static_string();
    // inferred: weak copy 0x80043130 (game/AreaEffConfig.cpp)
    static_string(const C* str) { *this = str; }
    static_string& operator=(const C* str) {
        if (str != mStr) {
            memcpy(mStr, str, strlen(str) + 1);
        }
        return *this;
    }
    // inferred: weak copy 0x8004276C (game/AreaEffConfig.cpp); the plain copy constructor copies
    // the N bytes
    static_string& operator=(const static_string& o) { return *this = o.mStr; }
    // inferred: inlined in AreaEffConfig::fn_80043014 (strlen of the stored string, then the
    // characters are compared one by one)
    bool operator==(const C* str) const {
        unsigned long n = strlen(mStr);
        return str[n] == '\0' && __Equal_unk(mStr, mStr + n, str);
    }
    static bool __Equal_unk(const C* first, const C* last, const C* first2) {
        for (; first != last; ++first, ++first2) {
            if (*first != *first2) {
                return false;
            }
        }
        return true;
    }
    C mStr[N];
};

} // namespace atn

#endif
