#ifndef STD_STRING_H
#define STD_STRING_H

// Partial reconstruction of the MSL C++ basic_string used by the game (no MSL C++ source is
// available). See docs/msl_cpp.md. Layout (same as libc++ on big-endian targets):
//   long mode : word0 = capacity | 0x80000000, word1 = size, word2 = data pointer
//   short mode: byte0 = size (bit 7 clear), characters start at the first aligned slot after it

#include "std/msl_common.h"
#include "std/iterator.h"
#include <memory>

namespace std {

// inferred: the string iterators are a class holding a pointer, not plain pointers:
// boost::tokenizer::begin (0x800EC654) passes them to the token_iterator constructor by address.
// Name invented.
template <class T>
class __string_iterator_unk {
public:
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef long difference_type;
    typedef T* pointer;
    typedef T& reference;
    __string_iterator_unk() {}
    explicit __string_iterator_unk(T* p) : mPtr(p) {}
    T& operator*() const { return *mPtr; }
    T* operator->() const { return mPtr; }
    __string_iterator_unk& operator++() {
        ++mPtr;
        return *this;
    }
    __string_iterator_unk operator++(int) {
        __string_iterator_unk tmp(*this);
        ++mPtr;
        return tmp;
    }
    T* base() const { return mPtr; }
    friend bool operator==(const __string_iterator_unk& a, const __string_iterator_unk& b) { return a.mPtr == b.mPtr; }
    friend bool operator!=(const __string_iterator_unk& a, const __string_iterator_unk& b) { return a.mPtr != b.mPtr; }

    T* mPtr;
};

// Standard character traits (only what the string and boost::char_separator use)
template <class T>
struct char_traits {
    typedef T char_type;
    typedef int int_type;
};

template <class T, class Traits = char_traits<T>, class Alloc = allocator<T> >
class basic_string {
public:
    typedef Traits traits_type;
    typedef T value_type;
    typedef Alloc allocator_type;
    typedef unsigned long size_type;
    typedef long difference_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef __string_iterator_unk<T> iterator;
    typedef __string_iterator_unk<const T> const_iterator;
    enum { __min_cap = (12 - 1) / sizeof(T) };

    // Inline members (inferred); game functions still call them out of line (weak copies:
    // size() 0x8000EC88, operator[] 0x8000ECA8, ~basic_string() 0x80013D60; wchar_t:
    // clear() 0x801F18BC, size() 0x801F1970, operator[] 0x801F1990, data() 0x801F19B4), while the
    // members below inline them (tmp in 0x80013F78). Checked with standalone compiles.
    size_type size() const { return !is_long() ? (mShort.mSize & 0x7F) : mLong.mSize; }
    const T* data() const { return !is_long() ? (const T*)this + 1 : mLong.mData; }
    T& operator[](size_type i) { return (!is_long() ? (T*)this + 1 : mLong.mData)[i]; }
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
    // inferred: weak copy 0x80042218 (char, game/AreaEffConfig.cpp)
    bool empty() const { return size() == 0; }
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
    basic_string& __replace_unk(size_type pos, size_type n1, const T* j1, const T* j2, random_access_iterator_tag tag);

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
        __zero();
        size_type n = strlen(s);
        reserve(n);
        __replace_unk(0, 0, s, s + n, random_access_iterator_tag());
    }
    basic_string(const basic_string& str, size_type pos, size_type n = npos); // 0x80069BF4 (char)
    // inferred: inlined in boost::tokenizer::begin (0x800EC654): a short string is copied word by
    // word, a long one through reserve and the range replace
    basic_string(const basic_string& str) {
        if (!str.is_long()) {
            // inferred: copied as a block (0x800EC7F0 stores word 1 before word 0)
            mRaw = str.mRaw;
        } else {
            __zero();
            reserve(str.mLong.mSize);
            __replace_unk(0, 0, str.mLong.mData, str.mLong.mData + str.mLong.mSize, random_access_iterator_tag());
        }
    }

    // inferred: inlined at 0x80476220: two short strings are copied word by word
    basic_string& operator=(const basic_string& str) {
        if (!is_long() && !str.is_long()) {
            mRaw = str.mRaw;
        } else {
            size_type sz = is_long() ? mLong.mSize : (mShort.mSize & 0x7F);
            const T* p;
            size_type n;
            if (!str.is_long()) {
                p = (const T*)&str + 1;
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
    size_type length() const { return size(); }
    const_iterator begin() const { return const_iterator(data()); }
    // inferred: data and size from one test (boost::tokenizer constructor, 0x800EC2C4)
    const_iterator end() const {
        const T* p;
        size_type sz;
        if (!is_long()) {
            p = (const T*)this + 1;
            sz = mShort.mSize & 0x7F;
        } else {
            p = mLong.mData;
            sz = mLong.mSize;
        }
        return const_iterator(p + sz);
    }
    // inferred: boost::char_separator (0x800EC7F0) assigns with replace(0, size(), first, last)
    template <class Iterator>
    basic_string& assign(Iterator first, Iterator last) {
        return replace(0, size(), first, last);
    }
    template <class Iterator>
    basic_string& replace(size_type pos, size_type n1, Iterator j1, Iterator j2) {
        typename iterator_traits<Iterator>::iterator_category tag;
        return __replace_unk(pos, n1, &*j1, &*j2, tag);
    }
    basic_string& operator=(const T* s) { return assign(s); }
    // inferred: inlined in CharaTask::CharaTask (0x80135CF0): size() first, then strlen in replace
    basic_string& append(const T* s) { return replace(size(), 0, s); }
    basic_string& operator+=(const T* s) { return append(s); }
    // inferred: inlined in BookTask::fn_8046AE7C (0x8046AE7C): size() first, then the data and
    // size of `str` from one test
    basic_string& append(const basic_string& str) {
        size_type sz = size();
        const T* p;
        size_type n;
        if (!str.is_long()) {
            p = (const T*)&str + 1;
            n = str.mShort.mSize & 0x7F;
        } else {
            p = str.mLong.mData;
            n = str.mLong.mSize;
        }
        return __replace_unk(sz, 0, p, p + n, random_access_iterator_tag());
    }
    basic_string& operator+=(const basic_string& str) { return append(str); }
    // inferred: one character (atn/WordWrap_unk 0x800DFB40: size() then the range replace of &c)
    basic_string& operator+=(T c) { return __replace_unk(size(), 0, &c, &c + 1, random_access_iterator_tag()); }
    // inferred: inlined in operator==(const basic_string&, const T*) (0x8000EB8C, char)
    // inferred: matches the weak copy 0x8000EB8C: n2 copied to a local that stays in a register
    // (n2 itself is passed by address to __min), the first __min takes the size first, and the
    // tail returns directly
    int compare(size_type pos1, size_type n1, const T* s, size_type n2) const {
        size_type len = n2;
        const T* p;
        size_type sz;
        if (!is_long()) {
            p = (const T*)this + 1;
            sz = mShort.mSize & 0x7F;
        } else {
            p = mLong.mData;
            sz = mLong.mSize;
        }
        if (pos1 > sz) {
            __msl_error("basic_string: out_of_range");
        }
        size_type rlen = __min(sz - pos1, n1);
        int r = memcmp(p + pos1, s, __min(rlen, n2) * sizeof(T));
        if (r == 0) {
            if (rlen < len) {
                return -1;
            } else if (rlen == len) {
                return 0;
            } else {
                return 1;
            }
        }
        return r;
    }
    int compare(const T* s) const { return compare(0, size(), s, strlen(s)); }

    // inferred: inlined in boost::char_separator (0x800EC7F0)
    size_type find(T c, size_type pos = 0) const {
        const T* p;
        size_type sz;
        if (!is_long()) {
            p = (const T*)this + 1;
            sz = mShort.mSize & 0x7F;
        } else {
            p = mLong.mData;
            sz = mLong.mSize;
        }
        if (pos < sz) {
            const T* e = p + sz;
            for (const T* q = p + pos; q < e; ++q) {
                if (c == *q) {
                    return q - p;
                }
            }
        }
        return npos;
    }

    size_type find(const T* s, size_type pos, size_type n) const; // 0x8006A004 (char)
    // inferred: inlined in CharaTask::fn_801371AC
    size_type find(const T* s, size_type pos = 0) const { return find(s, pos, strlen(s)); }

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
                if (c == *q) {
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
    // inferred: read through a word pointer, so the long bit is reloaded after stores that may
    // alias (inlined compare in EquipItemData::EquipItemData, 0x80363284)
    bool is_long() const { return *(const unsigned long*)this >> 31; }
    // inferred: clears the three words through a word pointer (the object address then stays in
    // a register for the calls that follow, 0x804732DC)
    void __zero() {
        unsigned long (&a)[3] = mRaw.mWords;
        for (unsigned int i = 0; i < 3; i++) {
            a[i] = 0;
        }
    }
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
    struct Raw {
        unsigned long mWords[3];
    };
    union {
        Raw mRaw;
        Long mLong;
        Short mShort;
        Flags mFlags;
        ShortFlags mShortFlags;
    };
};

// inferred: called out of line (weak copy 0x8000EB8C, char)
template <class T, class Tr, class A>
bool operator==(const basic_string<T, Tr, A>& lhs, const T* rhs) {
    return lhs.compare(rhs) == 0;
}

typedef basic_string<char> string;

} // namespace std

#endif
