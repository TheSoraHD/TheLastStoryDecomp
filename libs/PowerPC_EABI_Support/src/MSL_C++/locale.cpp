// MSL C++ locale.cpp (name inferred): the locale tables, locale::classic/__global and the ctype/codecvt
// facets. inferred: own translation unit 0x80691E50-0x806952C4 (second string pool 0x80766688, its __sinit
// sets the guards of the facet ids).
#include "msl_iostream_unk.h"

namespace std {

// inferred: the mutex of the locale tables, built on first use and dropped at exit (pointer 0x808803A0,
// storage 0x80880398, flag 0x808803A4)
static double __locale_mutex_storage_unk;
static Metrowerks::recursive_mutex* __locale_mutex_ptr_unk;
static bool __locale_mutex_ready_unk;

static void __destroy_locale_mutex_unk() {
    if (__locale_mutex_ptr_unk != 0) {
        __locale_mutex_ptr_unk->~recursive_mutex();
        __locale_mutex_ptr_unk = 0;
    }
}

inline Metrowerks::recursive_mutex& __locale_mutex_get_unk() {
    if (__locale_mutex_ptr_unk == 0) {
        __locale_mutex_ptr_unk = new (&__locale_mutex_storage_unk) Metrowerks::recursive_mutex;
        atexit(__destroy_locale_mutex_unk);
    }
    return *__locale_mutex_ptr_unk;
}

inline Metrowerks::recursive_mutex& __locale_mutex_unk() {
    if (!__locale_mutex_ready_unk) {
        __locale_mutex_get_unk();
    }
    __locale_mutex_ready_unk = true;
    return __locale_mutex_get_unk();
}

// NONMATCHING: 94% - the original strength-reduces the facet loop (offset + counter); -opt nostrength keeps i*4
__locale_imp::~__locale_imp() throw() {
    Metrowerks::scoped_lock<Metrowerks::recursive_mutex> lock(__locale_mutex_unk());
    for (size_t i = 0, n = facets_.size(); i < n; ++i) {
        locale::facet* f = facets_[i];
        if (f != 0) {
            delete f->__release_unk();
        }
    }
}

// inferred: the classic locale, built once under the locale mutex (guard 0x808803B0, object 0x808803B8)
inline const locale& locale::classic() {
    Metrowerks::scoped_lock<Metrowerks::recursive_mutex> lock(__locale_mutex_unk());
    static locale classic_(new __locale_imp("C"));
    return classic_;
}

// inferred: guard 0x808803A5, object 0x808803A8
// NONMATCHING: 80% - classic_ construction and the "C" literal pool (see docs/msl-iostream-20261107.md)
locale& locale::__global() throw() {
    static locale global_(classic());
    return global_;
}

ctype<char>::ctype(const mask* tab, bool del, size_t refs)
    : locale::facet(refs), table_(tab), lower_(lbl_80765538), upper_(lbl_80765638), del_(del) {
    if (table_ == 0) {
        table_ = lbl_80765338;
        del_ = false;
    }
}

ctype<char>::~ctype() {
    if (del_) {
        delete[] table_;
    }
}

char ctype<char>::do_toupper(char c) const {
    return upper_[(unsigned char)c];
}

// NONMATCHING: 98% - only a loop alignment nop (depends on the offset in the object)
const char* ctype<char>::do_toupper(char* low, const char* high) {
    for (; low < high; ++low) {
        *low = ctype<char>::do_toupper(*low);
    }
    return high;
}

char ctype<char>::do_tolower(char c) const {
    return lower_[(unsigned char)c];
}

// NONMATCHING: 98% - only a loop alignment nop (depends on the offset in the object)
const char* ctype<char>::do_tolower(char* low, const char* high) {
    for (; low < high; ++low) {
        *low = ctype<char>::do_tolower(*low);
    }
    return high;
}

const char* ctype<char>::do_widen(const char* low, const char* high, char* to) const {
    memcpy(to, low, high - low);
    return high;
}

const char* ctype<char>::do_narrow(const char* low, const char* high, char dfault, char* to) const {
    memcpy(to, low, high - low);
    return high;
}

codecvt<char, char, int>::codecvt(size_t refs) : locale::facet(refs) {}

codecvt<wchar_t, char, int>::codecvt(size_t refs) : locale::facet(refs) {}

// inferred: the wide characters are written as their two bytes, high byte first
codecvt_base::result codecvt<wchar_t, char, int>::do_out(int& state, const wchar_t* from, const wchar_t* from_end,
                                                         const wchar_t*& from_next, char* to, char* to_end,
                                                         char*& to_next) const {
    from_next = from;
    to_next = to;
    for (; from_next < from_end && to_next < to_end - 1; ++from_next, to_next += 2) {
        const char* p = (const char*)from_next;
        to_next[0] = p[0];
        to_next[1] = p[1];
    }
    if (from_next < from_end) {
        return partial;
    }
    return ok;
}

codecvt_base::result codecvt<wchar_t, char, int>::do_in(int& state, const char* from, const char* from_end,
                                                        const char*& from_next, wchar_t* to, wchar_t* to_end,
                                                        wchar_t*& to_next) const {
    from_next = from;
    to_next = to;
    for (; from_next < from_end - 1 && to_next < to_end; from_next += 2, ++to_next) {
        char* p = (char*)to_next;
        p[0] = from_next[0];
        p[1] = from_next[1];
    }
    if (from_next < from_end) {
        return partial;
    }
    return ok;
}

int codecvt<wchar_t, char, int>::do_length(int& state, const char* from, const char* end, size_t max) const {
    return 2 * min((int)((size_t)(end - from) / 2), (int)max);
}

int codecvt<char, char, int>::do_length(int& state, const char* from, const char* end, size_t max) const {
    return min((int)(end - from), (int)max);
}

char ctype<wchar_t>::do_narrow(wchar_t c, char dfault) const {
    if (c > 0xFF) {
        return dfault;
    }
    return (char)c;
}

// NONMATCHING: 92.5% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_is(const wchar_t* low, const wchar_t* high, mask* vec) const {
    for (; low < high; ++low, ++vec) {
        *vec = table_.find(*low);
    }
    return high;
}

// NONMATCHING: 88% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_scan_is(mask m, const wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        if (table_.find(*low) & m) {
            break;
        }
    }
    return low;
}

// NONMATCHING: 88% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_scan_not(mask m, const wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        if (!(table_.find(*low) & m)) {
            break;
        }
    }
    return low;
}

// NONMATCHING: 89.6% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_toupper(wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        *low = upper_.find(*low);
    }
    return high;
}

// NONMATCHING: 89.6% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_tolower(wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        *low = lower_.find(*low);
    }
    return high;
}

const char* ctype<wchar_t>::do_widen(const char* low, const char* high, wchar_t* to) const {
    for (; low < high; ++low, ++to) {
        *to = ctype<wchar_t>::do_widen(*low);
    }
    return high;
}

const wchar_t* ctype<wchar_t>::do_narrow(const wchar_t* low, const wchar_t* high, char dfault, char* to) const {
    while (low < high) {
        *to++ = do_narrow(*low++, dfault);
    }
    return high;
}

} // namespace std
