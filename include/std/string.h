#ifndef STD_STRING_H
#define STD_STRING_H

// Partial reconstruction of the MSL C++ basic_string used by the game (no MSL C++ source is
// available). See docs/msl_cpp.md. Layout (same as libc++ on big-endian targets):
//   long mode : word0 = capacity | 0x80000000, word1 = size, word2 = data pointer
//   short mode: byte0 = size (bit 7 clear), characters start at the first aligned slot after it

#include "std/msl_common.h"
#include "std/iterator.h"

namespace std {

template <class T>
class basic_string {
public:
    typedef unsigned long size_type;
    enum { __min_cap = (12 - 1) / sizeof(T) };

    // Inline members (inferred); game functions still call them out of line (weak copies:
    // size() 0x8000EC88, operator[] 0x8000ECA8, ~basic_string() 0x80013D60; wchar_t:
    // clear() 0x801F18BC, size() 0x801F1970, operator[] 0x801F1990, data() 0x801F19B4), while the
    // members below inline them (tmp in 0x80013F78). Checked with standalone compiles.
    size_type size() const { return !is_long() ? (mShort.mSize & 0x7F) : mLong.mSize; }
    const T* data() const { return !is_long() ? mShort.mData : mLong.mData; }
    T& operator[](size_type i) { return (!is_long() ? mShort.mData : mLong.mData)[i]; }
    void clear() {
        if (!is_long()) {
            mShort.mData[0] = 0;
            mShortFlags.mSize = 0;
        } else {
            mLong.mData[0] = 0;
            mLong.mSize = 0;
        }
    }
    size_type max_size() const { return 0x7FFFFFFE; }
    ~basic_string() {
        if (is_long()) {
            ::operator delete(mLong.mData);
        }
    }

    // 0x80013DC4 (char): same instructions as the original (only the string pool offset differs
    // in a test file), checked with a standalone compile.
    void reserve(size_type res_arg);
    // inferred: range replace used by the constructors/assign (0x80013F78, char). The last
    // argument is an iterator-tag object passed by value; the tags derive from each other
    // (random_access : bidirectional : forward : input), so Tag() is not zero-initialised.
    // NONMATCHING (standalone compile with -ipa file, 54 differing lines of 182): the original
    // does not reuse n2 for the memmove of the new characters (it recomputes j2 - j1), keeps
    // new_data + pos in a register of its own (one more saved register) and does not fold
    // tmp.size() for the empty temporary (it tests word 0 and reloads byte 0).
    template <class Tag>
    basic_string& __replace_unk(size_type pos, size_type n1, const T* j1, const T* j2, Tag tag);

    // inferred: inlined at 0x80512944: size() is evaluated in assign, the iterator tag copy and
    // strlen in the inner replace
    basic_string& replace(size_type pos, size_type n1, const T* s) {
        return __replace_unk(pos, n1, s, s + strlen(s), random_access_iterator_tag());
    }
    basic_string& assign(const T* s) { return replace(0, size(), s); }

    static const size_type npos = (size_type)-1;

    basic_string() { mLong.mCap = 0; mLong.mSize = 0; mLong.mData = 0; }
    // inferred: inlined at 0x80476220 (strlen, reserve, then the range replace)
    basic_string(const T* s) {
        mLong.mCap = 0;
        mLong.mSize = 0;
        mLong.mData = 0;
        size_type n = strlen(s);
        reserve(n);
        __replace_unk(0, 0, s, s + n, random_access_iterator_tag());
    }
    basic_string(const basic_string& str, size_type pos, size_type n = npos); // 0x80069BF4 (char)

    // inferred: inlined at 0x80476220: two short strings are copied word by word
    basic_string& operator=(const basic_string& str) {
        if (!is_long() && !str.is_long()) {
            mLong = str.mLong;
        } else {
            size_type sz = is_long() ? mLong.mSize : (mShort.mSize & 0x7F);
            const T* p;
            size_type n;
            if (!str.is_long()) {
                p = str.mShort.mData;
                n = str.mShort.mSize & 0x7F;
            } else {
                p = str.mLong.mData;
                n = str.mLong.mSize;
            }
            __replace_unk(0, sz, p, p + n, random_access_iterator_tag());
        }
        return *this;
    }
    basic_string& assign(const T* first, const T* last) {
        return __replace_unk(0, size(), first, last, random_access_iterator_tag());
    }
    basic_string& erase(size_type pos = 0, size_type n = npos) { return replace(pos, n, (const T*)0, (const T*)0); }
    basic_string& replace(size_type pos, size_type n1, const T* j1, const T* j2) {
        return __replace_unk(pos, n1, j1, j2, random_access_iterator_tag());
    }
    basic_string substr(size_type pos = 0, size_type n = npos) const { return basic_string(*this, pos, n); }
    const T* c_str() const { return data(); }

    // inferred: inlined at 0x80476220
    size_type rfind(T c, size_type pos = npos) const {
        const T* p;
        size_type sz;
        if (!is_long()) {
            p = mShort.mData;
            sz = mShort.mSize & 0x7F;
        } else {
            p = mLong.mData;
            sz = mLong.mSize;
        }
        if (sz != 0) {
            if (sz - 1 < pos) {
                pos = sz - 1;
            }
            for (const T* q = p + pos;; --q) {
                if (*q == c) {
                    return q - p;
                }
                if (q <= p) {
                    break;
                }
            }
        }
        return npos;
    }

private:
    static void __assign(T& c1, const T& c2) { c1 = c2; }
    static const size_type& __min(const size_type& a, const size_type& b) { return (b < a) ? b : a; }
    bool is_long() const { return mLong.mCap >> 31; }
    static T* __allocate(size_type n) {
        T* p = (T*)::operator new(n * sizeof(T));
        if (p == 0) {
            __msl_error("Memory allocation failure");
        }
        return p;
    }

    struct Long {
        unsigned long mCap;
        unsigned long mSize;
        T* mData;
    };
    struct Short {
        unsigned char mSize;
        T mData[__min_cap];
    };
    struct Flags {
        unsigned long mIsLong : 1;
        unsigned long mCap : 31;
        unsigned long mSize;
        T* mData;
    };
    struct ShortFlags {
        unsigned char mIsLong : 1;
        unsigned char mSize : 7;
    };
    union {
        Long mLong;
        Short mShort;
        Flags mFlags;
        ShortFlags mShortFlags;
    };
};

typedef basic_string<char> string;

} // namespace std

#endif
