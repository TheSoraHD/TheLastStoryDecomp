#ifndef STD_ISTREAM_H
#define STD_ISTREAM_H

// Partial reconstruction of MSL C++ std::basic_istream and std::basic_iostream (no MSL C++ source is
// available), as inlined in boost::lexical_cast<std::string, int> (game/GameEventScript 0x80393BEC) and
// in the weak destructors after it (0x803940E8-0x80394208, thunks 0x803B27AC-0x803B27F8). Layouts:
// basic_istream has the virtual-base pointer at 0x00, the extracted count at 0x04, its vtable at 0x08
// (0x8078B2E0) and basic_ios at 0x0C; basic_iostream puts the basic_ostream part at 0x0C and basic_ios
// at 0x14. Data member names are invented.

#include "std/ostream.h"
#include "std/locale_facets_unk.h"
#include "std/sstream.h"

namespace std {

inline void ios_base::unsetf(fmtflags mask) {
    mFlags &= ~mask;
}

inline streamsize ios_base::precision(streamsize prec) {
    streamsize old = mPrecision;
    mPrecision = prec;
    return old;
}

inline locale::locale() : mImp(__global().mImp) {
    if (mImp.mCntrl != 0) {
        ++mImp.mCntrl->mUseCount;
    }
}

template <class C, class T>
inline basic_streambuf<C, T>::basic_streambuf()
    : mEback(0), mGptr(0), mEgptr(0), mPbase(0), mPptr(0), mEpptr(0) {}

// inferred: widen through the ctype facet of the stream locale (the facet is created on first use)
template <class C, class T>
inline C basic_ios<C, T>::widen(char c) const {
    return use_facet<ctype<C> >(this->getloc()).widen(c);
}

// inferred: ios_base::init, no tied stream and a space as fill character
template <class C, class T>
inline void basic_ios<C, T>::init(basic_streambuf<C, T>* sb) {
    ios_base::init(sb);
    mTie = 0;
    mFill = widen(' ');
}

// inferred: the destructors are inlined in ~basic_stringstream (0x80394208); atn/WordWrap_unk defines
// its own out-of-line copies
template <class C, class T>
inline basic_ostream<C, T>::~basic_ostream() {}

template <class C, class T>
inline basic_streambuf<C, T>::~basic_streambuf() {}

template <class C, class T, class A>
inline basic_stringbuf<C, T, A>::~basic_stringbuf() {}

template <class C, class T = char_traits<C> >
class basic_istream : virtual public basic_ios<C, T> {
public:
    explicit basic_istream(basic_streambuf<C, T>* sb) : mGcount(0) { this->init(sb); }
    streamsize mGcount; // 0x04
    virtual ~basic_istream() {}
};

template <class C, class T = char_traits<C> >
class basic_iostream : public basic_istream<C, T>, public basic_ostream<C, T> {
public:
    // inferred: basic_ios is initialized once, by the basic_istream part
    explicit basic_iostream(basic_streambuf<C, T>* sb) : basic_istream<C, T>(sb) {}
    virtual ~basic_iostream() {}
};

// inferred: the areas start empty on the string
template <class C, class T, class A>
inline basic_stringbuf<C, T, A>::basic_stringbuf(ios_base::openmode which) : mMode(which) {
    C* p = (C*)mStr.data();
    mHigh = p;
    if (mMode & ios_base::out) {
        this->mPptr = p;
        this->mPbase = p;
        this->mEpptr = p;
    }
    if (mMode & ios_base::in) {
        this->mEback = p;
        this->mGptr = p;
        this->mEgptr = p;
    }
}

// inferred: the written part (up to the high-water mark) or the readable part
// NONMATCHING: 88.53% - the original uses a range constructor of basic_string (reserve(last - first) and the
// range replace; with it 98.97%, first/last swapped between r30 and r31): not declared in std/string.h
// to keep that header (included by every unit) unchanged
template <class C, class T, class A>
basic_string<C, T, A> basic_stringbuf<C, T, A>::str() const {
    if (mMode & ios_base::out) {
        if (mHigh < this->mPptr) {
            mHigh = this->mPptr;
        }
        return basic_string<C, T, A>(this->mPbase, mHigh - this->mPbase);
    }
    return basic_string<C, T, A>(this->mEback, this->mEgptr - this->mEback);
}

// inferred: MSL <sstream> string stream (vtable 0x8078B080): the basic_iostream part at 0x00, the
// buffer at 0x14 and basic_ios at 0x4C. Kept here and not in std/sstream.h because atn/WordWrap_unk
// (which includes that one) defines ~basic_ostream itself.
template <class C, class T = char_traits<C>, class A = allocator<C> >
class basic_stringstream : public basic_iostream<C, T> {
public:
    explicit basic_stringstream(ios_base::openmode which = ios_base::openmode(ios_base::in | ios_base::out))
        : basic_iostream<C, T>(&mBuf), mBuf(which) {}
    virtual ~basic_stringstream() {}
    basic_string<C, T, A> str() const { return mBuf.str(); }

    basic_stringbuf<C, T, A> mBuf; // 0x14
};

typedef basic_stringstream<char> stringstream;

} // namespace std

#endif
