#ifndef STD_STRING_H
#define STD_STRING_H

// Partial reconstruction of the MSL C++ basic_string used by the game (no MSL C++ source is
// available). Layout inferred from the inline accessors at 0x8000EC88 / 0x801F1970
// (size), 0x8000ECA8 / 0x801F1990 (operator[]), 0x801F18BC (clear) and 0x801F1908 (dtor):
//   long mode : word0 = capacity | 0x80000000, word1 = size, word2 = data pointer
//   short mode: byte0 = size (bit 7 clear), characters start at the first aligned slot after it
// This is the same SSO layout as libc++ on big-endian targets.

namespace std {

template <class T>
class basic_string {
public:
    unsigned int size() const { return is_long() ? mLong.mSize : (mShort.mSize & 0x7F); }
    const T* data() const { return is_long() ? mLong.mData : mShort.mData; }
    T& operator[](unsigned int i) { return (is_long() ? mLong.mData : mShort.mData)[i]; }
    ~basic_string() {
        if (is_long()) {
            delete mLong.mData;
        }
    }

private:
    bool is_long() const { return mLong.mCap >> 31; }

    struct Long {
        unsigned int mCap;
        unsigned int mSize;
        T* mData;
    };
    struct Short {
        unsigned char mSize;
        T mData[(12 - 1) / sizeof(T)];
    };
    union {
        Long mLong;
        Short mShort;
    };
};

typedef basic_string<char> string;

} // namespace std

#endif
