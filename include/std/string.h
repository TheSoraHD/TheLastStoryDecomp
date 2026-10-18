#ifndef STD_STRING_H
#define STD_STRING_H

// Partial reconstruction of the MSL C++ basic_string used by the game (no MSL C++ source is
// available). See docs/msl_cpp.md. Layout (same as libc++ on big-endian targets):
//   long mode : word0 = capacity | 0x80000000, word1 = size, word2 = data pointer
//   short mode: byte0 = size (bit 7 clear), characters start at the first aligned slot after it

void* operator new(unsigned long size);
void operator delete(void* p);

struct _FILE_unk {
    unsigned char m00[0x50];
};
extern "C" _FILE_unk __files[];
extern "C" int fprintf(_FILE_unk* file, const char* format, ...);
// inferred: abort is declared noreturn (0x80013F78 reuses sz - rlen computed only on the
// non-error path of an || condition)
extern "C" __declspec(noreturn) void abort();
extern "C" void* memmove(void* dst, const void* src, unsigned long n);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: MSL reports errors with fprintf(stderr, ...) + abort() when exceptions are off
#define __msl_error(msg) (fprintf(&__files[2], msg), abort())

namespace std {

template <class T>
class basic_string {
public:
    typedef unsigned long size_type;
    enum { __min_cap = (12 - 1) / sizeof(T) };

    // Inline members; large game functions still call them out of line (weak copies:
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
    // NONMATCHING (standalone compile, 61 differing lines of 182): the original does not reuse
    // n2 for the memmove of the new characters (it recomputes j2 - j1) and does not fold
    // tmp.size() for the empty temporary.
    template <class Tag>
    basic_string& __replace_unk(size_type pos, size_type n1, const T* j1, const T* j2, Tag tag);

    basic_string() { mLong.mCap = 0; mLong.mSize = 0; mLong.mData = 0; }

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

template <class T>
void basic_string<T>::reserve(size_type res_arg) {
    if (res_arg > max_size()) {
        __msl_error("basic_string::reserve length_error");
    }
    size_type cap;
    size_type sz;
    bool was_long = mFlags.mIsLong;
    if (!was_long) {
        sz = mShortFlags.mSize;
        cap = __min_cap;
    } else {
        sz = mLong.mSize;
        cap = mFlags.mCap;
    }
    if (res_arg < sz) {
        res_arg = sz;
    }
    res_arg = (res_arg + 1 <= __min_cap) ? (size_type)__min_cap : (res_arg + 16) & ~15;
    if (res_arg != cap) {
        T* new_data;
        T* p;
        bool now_long;
        if (res_arg == __min_cap) {
            now_long = false;
            new_data = mShort.mData;
            p = mLong.mData;
        } else {
            if (res_arg > cap) {
                new_data = __allocate(res_arg);
            } else {
                new_data = __allocate(res_arg);
                if (new_data == 0) {
                    return;
                }
            }
            now_long = true;
            p = was_long ? mLong.mData : mShort.mData;
        }
        memmove(new_data, p, sz * sizeof(T));
        new_data[sz] = 0;
        if (was_long) {
            ::operator delete(p);
        }
        mFlags.mIsLong = now_long;
        if (!now_long) {
            mShortFlags.mSize = sz;
        } else {
            mLong.mData = new_data;
            mLong.mSize = sz;
            mFlags.mCap = res_arg;
        }
    }
}

template <class T>
template <class Tag>
basic_string<T>& basic_string<T>::__replace_unk(size_type pos, size_type n1, const T* j1, const T* j2, Tag tag) {
    bool was_long = mFlags.mIsLong;
    size_type new_sz;
    T* p;
    size_type sz;
    size_type cap;
    if (!was_long) {
        p = mShort.mData;
        cap = __min_cap;
        sz = mShortFlags.mSize;
    } else {
        p = mLong.mData;
        cap = mFlags.mCap;
        sz = mLong.mSize;
    }
    if (pos > sz) {
        __msl_error("basic_string: out_of_range");
    }
    size_type rlen = __min(n1, sz - pos);
    size_type n2 = j2 - j1;
    if (n2 > max_size() || sz - rlen > max_size() - n2) {
        __msl_error("basic_string: length_error");
    }
    new_sz = n2 + (sz - rlen);
    size_type tail = sz - (pos + rlen);
    if (new_sz >= cap) {
        cap = (cap + 15) & ~15;
        while (cap < new_sz + 1) {
            cap = (cap * 2 + 15) & ~15;
        }
        T* new_data = __allocate(cap);
        if (pos != 0) {
            memcpy(new_data, p, pos * sizeof(T));
        }
        T* d = new_data + pos;
        memmove(d, j1, (j2 - j1) * sizeof(T));
        if (tail != 0) {
            memcpy(d + n2, p + pos + rlen, tail * sizeof(T));
        }
        __assign(new_data[new_sz], T());
        if (was_long) {
            ::operator delete(p);
        } else {
            mFlags.mIsLong = 1;
        }
        mLong.mData = new_data;
        mLong.mSize = new_sz;
        mFlags.mCap = cap;
    } else {
        basic_string tmp;
        if (tail != 0 && p + pos + n2 < j2 && j2 <= p + sz) {
            tmp.__replace_unk(0, tmp.size(), j1, j2, Tag());
            j1 = tmp.data();
        }
        if (tail != 0) {
            memmove(p + pos + n2, p + pos + rlen, tail * sizeof(T));
        }
        memmove(p + pos, j1, n2 * sizeof(T));
        __assign(p[new_sz], T());
        if (!was_long) {
            mShortFlags.mSize = new_sz;
        } else {
            mLong.mSize = new_sz;
        }
    }
    return *this;
}

typedef basic_string<char> string;

} // namespace std

#endif
