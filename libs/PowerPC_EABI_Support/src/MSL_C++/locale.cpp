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

// NONMATCHING: 99.98% - stack slots of the two inlined placement news of the locale mutex are swapped
__locale_imp::~__locale_imp() throw() {
    Metrowerks::scoped_lock<Metrowerks::recursive_mutex> lock(__locale_mutex_unk());
    size_t n = facets_.size();
    for (size_t i = 0; i < n; ++i) {
        locale::facet* f = facets_[i];
        if (f != 0) {
            delete f->__release_unk();
        }
    }
}

// NONMATCHING: 91% - the original has eight one-byte temporaries (stb 0) in the inlined resize (unknown origin)
void __locale_imp::replace(locale::facet* f, size_t id) {
    ++f->refs_;
    if (id >= facets_.size()) {
        try {
            facets_.resize(id + 1);
        } catch (...) {
            delete f->__release_unk();
            throw;
        }
    }
    locale::facet*& p = facets_[id];
    if (p != 0) {
        delete p->__release_unk();
    }
    p = f;
}

// inferred: the classic locale, built once under the locale mutex (guard 0x808803B0, object 0x808803B8)
const locale& locale::classic() {
    Metrowerks::scoped_lock<Metrowerks::recursive_mutex> lock(__locale_mutex_unk());
    static locale classic_(new __locale_imp("C"));
    return classic_;
}

// inferred: guard 0x808803A5, object 0x808803A8
// NONMATCHING: 85% - the temporary shared_ptr of classic_ stays in memory (the original keeps it in registers)
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

const char* ctype<char>::do_toupper(char* low, const char* high) {
    for (; low < high; ++low) {
        *low = ctype<char>::do_toupper(*low);
    }
    return high;
}

char ctype<char>::do_tolower(char c) const {
    return lower_[(unsigned char)c];
}

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

// inferred: the classification and case tables of the "C" locale as ranges (constants in .sdata 0x8087EC80)
ctype<wchar_t>::ctype(size_t refs) : locale::facet(refs) {
    __range_map_unk<mask>::insert(table_, L'\x00', L'\x08', cntrl);
    __range_map_unk<mask>::insert(table_, L'\t', mask(space | cntrl | blank));
    __range_map_unk<mask>::insert(table_, L'\n', L'\r', mask(space | cntrl));
    __range_map_unk<mask>::insert(table_, L'\x0E', L'\x1F', cntrl);
    __range_map_unk<mask>::insert(table_, L' ', mask(space | print | blank));
    __range_map_unk<mask>::insert(table_, L'!', L'/', mask(punct | print | graph));
    __range_map_unk<mask>::insert(table_, L'0', L'9', mask(xdigit | print | graph | digit));
    __range_map_unk<mask>::insert(table_, L':', L'@', mask(punct | print | graph));
    __range_map_unk<mask>::insert(table_, L'A', L'F', mask(xdigit | upper | print | graph | alpha));
    __range_map_unk<mask>::insert(table_, L'G', L'Z', mask(upper | print | graph | alpha));
    __range_map_unk<mask>::insert(table_, L'[', L'`', mask(punct | print | graph));
    __range_map_unk<mask>::insert(table_, L'a', L'f', mask(xdigit | lower | print | graph | alpha));
    __range_map_unk<mask>::insert(table_, L'g', L'z', mask(lower | print | graph | alpha));
    __range_map_unk<mask>::insert(table_, L'{', L'~', mask(punct | print | graph));
    __range_map_unk<mask>::insert(table_, L'\x7F', cntrl);
    __range_map_unk<wchar_t>::insert(lower_, numeric_limits<wchar_t>::min(), numeric_limits<wchar_t>::max(), numeric_limits<wchar_t>::min(),
                  numeric_limits<wchar_t>::max());
    __range_map_unk<wchar_t>::insert(lower_, L'A', L'Z', L'a', L'z');
    __range_map_unk<wchar_t>::insert(upper_, numeric_limits<wchar_t>::min(), numeric_limits<wchar_t>::max(), numeric_limits<wchar_t>::min(),
                  numeric_limits<wchar_t>::max());
    __range_map_unk<wchar_t>::insert(upper_, L'a', L'z', L'A', L'Z');
}

ctype<wchar_t>::~ctype() {}

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
        *vec = __range_map_unk<mask>::find(table_, *low);
    }
    return high;
}

// NONMATCHING: 88% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_scan_is(mask m, const wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        if (__range_map_unk<mask>::find(table_, *low) & m) {
            break;
        }
    }
    return low;
}

// NONMATCHING: 88% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_scan_not(mask m, const wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        if (!(__range_map_unk<mask>::find(table_, *low) & m)) {
            break;
        }
    }
    return low;
}

// NONMATCHING: 89.6% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_toupper(wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        *low = __range_map_unk<wchar_t>::find(upper_, *low);
    }
    return high;
}

// NONMATCHING: 89.6% - the original recomputes the table end after the search
const wchar_t* ctype<wchar_t>::do_tolower(wchar_t* low, const wchar_t* high) const {
    for (; low < high; ++low) {
        *low = __range_map_unk<wchar_t>::find(lower_, *low);
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

// inferred: the facet ids (0x808803C0-0x808803CC, .sbss); their (empty) initialization sets the guards
// 0x808803D0-0x808803D3 in __sinit_locale_cpp
size_t locale::id::__id_count_;
locale::id ctype<char>::id;
locale::id ctype<wchar_t>::id;
locale::id codecvt<char, char, int>::id;
locale::id codecvt<wchar_t, char, int>::id;

} // namespace std
