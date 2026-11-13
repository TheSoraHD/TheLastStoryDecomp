#ifndef STD_LOCALE_FACETS_UNK_H
#define STD_LOCALE_FACETS_UNK_H

// Partial reconstruction of the MSL C++ numeric facets (no MSL C++ source is available): only what the weak
// copies of atn/EffectManager (0x8023B9D0-0x80243700) need. Layouts and vtable slots are inferred from those
// copies: numpunct<char> (0x30 bytes: the two characters at 0x08/0x09 and the grouping, true and false names
// at 0x0C/0x18/0x24), num_put (8 bytes, do_put slots 0x0C-0x28) and the stream iterator (one pointer).

#include "std/string.h"
#include "std/locale.h"
#include "std/streambuf.h"

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

    size_t id_;
    static size_t __id_count_;
};

// inferred: output iterator over a stream buffer (one word)
template <class C, class T = char_traits<C> >
class ostreambuf_iterator {
public:
    basic_streambuf<C, T>* sbuf_;
};

template <class C, class O = ostreambuf_iterator<C> >
class num_put : public locale::facet {
public:
    typedef C char_type;
    typedef O iter_type;

    explicit num_put(size_t refs = 0) : locale::facet(refs) {}

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

template <class C>
class numpunct : public locale::facet {
public:
    typedef C char_type;
    typedef basic_string<C> string_type;

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

// inferred: a narrow literal as a string of the character type (weak copy 0x80243514)
inline string __literal(const char* s, char) {
    return string(s);
}

} // namespace std

#endif
