#ifndef STD_LOCALE_FACETS_UNK_H
#define STD_LOCALE_FACETS_UNK_H

// Partial reconstruction of the MSL C++ numeric facets (no MSL C++ source is available): only what the weak
// copies of atn/EffectManager (0x8023B9D0-0x80243700) need. Layouts and vtable slots are inferred from those
// copies: numpunct<char> (0x30 bytes: the two characters at 0x08/0x09 and the grouping, true and false names
// at 0x0C/0x18/0x24), num_put (8 bytes, do_put slots 0x0C-0x28) and the stream iterator (one pointer).

#include "std/string.h"
#include "std/locale.h"
#include "std/streambuf.h"
#include "std/ostream.h"
#include "std/vector.h"

namespace std {

// inferred: reference-counted base of the facets (the deleting destructors of the facets only test the
// count argument)
class locale::facet {
protected:
    explicit facet(size_t refs = 0) : refs_(refs) {}
    virtual ~facet() {}

    size_t refs_;
};

// inferred: index of a facet in the locale tables, given on first use from a global count (0x8087F360 area;
// the ids are zero-initialized and their guards are set by the __sinit of the file)
class locale::id {
public:
    id() {}
    size_t get() {
        if (id_ == 0) {
            id_ = ++__id_count_;
        }
        return id_;
    }

    size_t id_;
    static size_t __id_count_;
};

// inferred: index of the facets of a locale; replace (0x80691xxx, MSL locale.cpp) installs a facet in a slot
struct __locale_imp {
    void replace(locale::facet* f, size_t id);

    vector<locale::facet*> facets_; // 0x00
};

// inferred: output iterator over a stream buffer (one word); a null buffer is a failed iterator
template <class C, class T = char_traits<C> >
class ostreambuf_iterator {
public:
    ostreambuf_iterator(basic_ostream<C, T>& s) : sbuf_(s.rdbuf()) {}
    ostreambuf_iterator& operator=(C c) {
        if (sbuf_ != 0 && sbuf_->sputc(c) == -1) {
            sbuf_ = 0;
        }
        return *this;
    }
    ostreambuf_iterator& operator*() { return *this; }
    ostreambuf_iterator& operator++() { return *this; }
    bool failed() const { return sbuf_ == 0; }

    basic_streambuf<C, T>* sbuf_;
};

// inferred: the facet of a locale, created and installed on first use
template <class Facet>
inline const Facet& use_facet(const locale& loc) {
    size_t i = Facet::id.get();
    const locale::facet* f;
    if (i >= loc.mImp.mPtr->facets_.size() || (f = loc.mImp.mPtr->facets_[i]) == 0) {
        __locale_imp* imp = loc.mImp.mPtr;
        imp->replace(new Facet, Facet::id.get());
        f = loc.mImp.mPtr->facets_[i];
    }
    return *(const Facet*)f;
}

template <class C, class O = ostreambuf_iterator<C> >
class num_put : public locale::facet {
public:
    typedef C char_type;
    typedef O iter_type;

    explicit num_put(size_t refs = 0) : locale::facet(refs) {}
    iter_type put(iter_type out, ios_base& str, char_type fill, long v) const { return do_put(out, str, fill, v); }
    iter_type put(iter_type out, ios_base& str, char_type fill, unsigned long v) const {
        return do_put(out, str, fill, v);
    }

    static locale::id id;

protected:
    virtual ~num_put();
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, bool v) const;
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, long v) const;
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, unsigned long v) const;
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, long long v) const;
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, unsigned long long v) const;
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, double v) const;
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, long double v) const;
    virtual iter_type do_put(iter_type out, ios_base& str, char_type fill, const void* v) const;
};

template <class C, class O>
locale::id num_put<C, O>::id;

template <class C, class O>
num_put<C, O>::~num_put() {}

// inferred: forwards to the long double one (0x8023F5A4)
template <class C, class O>
typename num_put<C, O>::iter_type num_put<C, O>::do_put(iter_type out, ios_base& str, char_type fill, double v) const {
    return do_put(out, str, fill, (long double)v);
}

// inferred: a narrow literal as a string of the character type (weak copy 0x80243514)
inline string __literal(const char* s, char) {
    return string(s);
}

// inferred: the ios_base flags used by num_put (only used here; dec 0x02, hex 0x08, oct 0x40)
enum {
    __boolalpha_unk = 0x01,
    __hex_unk = 0x08,
    __internal_unk = 0x10,
    __left_unk = 0x20,
    __oct_unk = 0x40,
    __basefield_unk = 0x4A,
    __adjustfield_unk = 0xB0,
    __uppercase_unk = 0x4000
};

template <class C>
class ctype;

// inferred: the char ctype facet (constructor 0x8069xxxx in MSL locale.cpp); vtable slots from the calls of
// the digit helpers (do_widen at 0x1C)
template <>
class ctype<char> : public locale::facet {
public:
    explicit ctype(const unsigned short* tab = 0, bool del = false, size_t refs = 0);
    char widen(char c) const { return do_widen(c); }

    static locale::id id;

protected:
    virtual ~ctype();
    virtual char do_toupper(char c) const;
    virtual const char* do_toupper(char* low, const char* high) const;
    virtual char do_tolower(char c) const;
    virtual const char* do_tolower(char* low, const char* high) const;
    virtual char do_widen(char c) const;
};

// inferred: std::reverse as inlined in the digit helpers
template <class I>
inline void __reverse_unk(I first, I last) {
    if (first != last) {
        while (first < --last) {
            char t = *first;
            *first = *last;
            *last = t;
            ++first;
        }
    }
}

template <class C>
class numpunct : public locale::facet {
public:
    typedef C char_type;
    typedef basic_string<C> string_type;

    // inferred: inlined in num_put::do_put(bool) (0x8023C0C0); the names are literals of .data
    explicit numpunct(size_t refs = 0) : locale::facet(refs), decimal_point_('.'), thousands_sep_(',') {
        truename_ = __literal("true", C());
        falsename_ = __literal("false", C());
    }

    char_type decimal_point() const;
    char_type thousands_sep() const;
    string_type grouping() const;
    string_type truename() const;
    string_type falsename() const;

    static locale::id id;

protected:
    virtual ~numpunct();
    virtual char_type do_decimal_point() const;
    virtual char_type do_thousands_sep() const;
    virtual string_type do_grouping() const;
    virtual string_type do_truename() const;
    virtual string_type do_falsename() const;

    char_type decimal_point_;  // 0x08
    char_type thousands_sep_;  // 0x09
    string grouping_;          // 0x0C
    string_type truename_;     // 0x18
    string_type falsename_;    // 0x24
};

template <class C>
locale::id numpunct<C>::id;


template <class C>
typename numpunct<C>::string_type numpunct<C>::truename() const {
    return do_truename();
}
template <class C>
typename numpunct<C>::string_type numpunct<C>::falsename() const {
    return do_falsename();
}
// NONMATCHING: 55%% - the short copy of the string is scheduled differently (the original saves r30/r31
// first and copies word 1 before word 0; ctype<char>::widen 0x80241C20 has this file's order)
template <class C>
typename numpunct<C>::string_type numpunct<C>::do_truename() const {
    return truename_;
}
// NONMATCHING: 55%% - the short copy of the string is scheduled differently (the original saves r30/r31
// first and copies word 1 before word 0; ctype<char>::widen 0x80241C20 has this file's order)
template <class C>
typename numpunct<C>::string_type numpunct<C>::do_falsename() const {
    return falsename_;
}
template <class C>
typename numpunct<C>::char_type numpunct<C>::do_decimal_point() const {
    return decimal_point_;
}
template <class C>
typename numpunct<C>::string_type numpunct<C>::grouping() const {
    return do_grouping();
}
// NONMATCHING: 55%% - the short copy of the string is scheduled differently (the original saves r30/r31
// first and copies word 1 before word 0; ctype<char>::widen 0x80241C20 has this file's order)
template <class C>
typename numpunct<C>::string_type numpunct<C>::do_grouping() const {
    return grouping_;
}
template <class C>
typename numpunct<C>::char_type numpunct<C>::do_thousands_sep() const {
    return thousands_sep_;
}
template <class C>
numpunct<C>::~numpunct() {}

// NONMATCHING: 88% - stack slots of the two name temporaries of the inlined numpunct constructor swapped,
// registers of the facet and the table
// inferred: inserts the thousands separator in the integer part of a formatted float (the digits before the
// decimal point dp, or the whole string), following the grouping (weak copy 0x802428D8)
template <class C, class S>
void __put_grouping_float(C dp, C sep, const S& grouping, S& s) {
    if (grouping.size() != 0 && s.size() > 1) {
        typename S::size_type pos = s.find(dp);
        if (pos == S::npos) {
            pos = s.size();
        }
        typename S::size_type gi = 0;
        unsigned char g = grouping.data()[0];
        unsigned int cnt = 0;
        while (--pos != 0) {
            if (++cnt == g) {
                s.replace(pos, 0, 1, sep);
                if (++gi < grouping.size()) {
                    g = grouping[gi];
                }
                cnt = 0;
            }
        }
    }
}

// NONMATCHING: 89.5% (unsigned long long), 94.4% (unsigned long) - one saved register less (registers of the
// grouping counters), stack slots of the name temporaries of the inlined numpunct constructor swapped
// inferred: digits of an unsigned integer in the base of the stream flags (8, 10 or 16, upper case letters
// with uppercase), grouped with the numpunct of the stream unless noGroup, written backwards then reversed;
// returns their count (weak copies 0x80242A8C for unsigned long long and 0x80242FF4 for unsigned long; name
// invented)
template <class C, class U>
int __put_digits_unk(ios_base& str, U v, C* buf, const ctype<C>& ct, bool noGroup) {
    locale loc = str.getloc();
    if (v == 0) {
        *buf = ct.widen('0');
        return 1;
    }
    const numpunct<C>& np = use_facet<numpunct<C> >(loc);
    unsigned int base = 10;
    switch (str.flags() & __basefield_unk) {
    case __oct_unk:
        base = 8;
        break;
    case __hex_unk:
        base = 16;
        break;
    }
    C* p = buf;
    string g = np.grouping();
    unsigned int gi = 0;
    unsigned char cnt = 0;
    unsigned char gsize = 0;
    bool group = g.size() != 0;
    if (group) {
        gsize = g[0];
        if (gsize == 0) {
            group = false;
        }
    }
    if (noGroup) {
        group = false;
    }
    C sep = group ? np.thousands_sep() : C();
    bool upper = (str.flags() & __uppercase_unk) != 0;
    while (v != 0) {
        unsigned int d = v % base;
        C c;
        if (d < 10) {
            c = ct.widen((char)('0' + d));
        } else if (upper) {
            c = ct.widen((char)('A' - 10 + d));
        } else {
            c = ct.widen((char)('a' - 10 + d));
        }
        v /= base;
        *p++ = c;
        if (v != 0 && group && ++cnt == gsize) {
            *p++ = sep;
            if (++gi < g.size()) {
                gsize = g[gi];
                if (gsize == 0) {
                    group = false;
                }
            }
            cnt = 0;
        }
    }
    __reverse_unk(buf, p);
    return p - buf;
}

// inferred: bool output (0x8023C0C0): without boolalpha as a long; else the true or false name of the
// numpunct of the stream, padded with the fill character to the width (left, internal or right), which is
// cleared
template <class C, class O>
typename num_put<C, O>::iter_type num_put<C, O>::do_put(iter_type out, ios_base& str, char_type fill, bool v) const {
    if ((str.flags() & __boolalpha_unk) != 0) {
        const numpunct<C>& np = use_facet<numpunct<C> >(str.getloc());
        basic_string<C> s = v ? np.truename() : np.falsename();
        int n = s.size();
        const C* p = s.data();
        int pad = 0;
        if (str.mWidth > n) {
            pad = str.mWidth - n;
        }
        unsigned short adjust = str.flags() & __adjustfield_unk;
        if (adjust != __left_unk && adjust != __internal_unk) {
            for (int i = 0; i < pad; i++) {
                *out = fill;
            }
        }
        if (adjust == __internal_unk) {
            for (int i = 0; i < pad; i++) {
                *out = fill;
            }
        }
        for (int i = 0; i < n; i++) {
            *out = *p++;
        }
        if (adjust == __left_unk) {
            for (int i = 0; i < pad; i++) {
                *out = fill;
            }
        }
        str.mWidth = 0;
        return out;
    }
    return do_put(out, str, fill, (long)v);
}

// inferred: integer output (weak copy 0x8023BA10): octal and hexadecimal go through the unsigned long put;
// a failed put sets badbit and failbit
// NONMATCHING: 27% - the original inlines the sentry constructor and destructor (and the flush of the tied stream
// down to an out of line sentry), here they are called (std/ostream.h, defined in atn/WordWrap_unk)
template <class C, class T>
basic_ostream<C, T>& basic_ostream<C, T>::operator<<(int n) {
    sentry s(*this);
    if (s.mOk) {
        typedef num_put<C, ostreambuf_iterator<C, T> > facet_type;
        const facet_type& np = use_facet<facet_type>(this->getloc());
        bool failed;
        unsigned short base = this->flags() & __basefield_unk;
        if (base == __oct_unk || base == __hex_unk) {
            failed = np.put(ostreambuf_iterator<C, T>(*this), *this, this->m38[0], (unsigned long)n).failed();
        } else {
            failed = np.put(ostreambuf_iterator<C, T>(*this), *this, this->m38[0], (long)n).failed();
        }
        if (failed) {
            this->clear((ios_base::iostate)(this->mState | ios_base::badbit | ios_base::failbit));
        }
    }
    return *this;
}

} // namespace std

#endif
