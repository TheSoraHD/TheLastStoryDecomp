#ifndef STD_SSTREAM_H
#define STD_SSTREAM_H

// Partial reconstruction of MSL C++ std::basic_stringbuf (no MSL C++ source is available); vtable
// 0x80779A08 (char). Data member names are invented.

#include "std/streambuf.h"

namespace std {

template <class C, class T = char_traits<C>, class A = allocator<C> >
class basic_stringbuf : public basic_streambuf<C, T> {
public:
    typedef typename basic_streambuf<C, T>::int_type int_type;
    typedef typename basic_streambuf<C, T>::pos_type pos_type;
    typedef typename basic_streambuf<C, T>::off_type off_type;

    virtual ~basic_stringbuf();

protected:
    virtual pos_type seekoff(off_type off, ios_base::seekdir way, ios_base::openmode which);
    virtual pos_type seekpos(pos_type sp, ios_base::openmode which);
    virtual int_type underflow();
    virtual int_type pbackfail(int_type c);
    virtual int_type overflow(int_type c);

public:
    unsigned char mMode;         // 0x24: inferred: ios_base::openmode bits kept in a byte
    char m25[3];
    C* mHigh;                    // 0x28: inferred: high-water mark of the put area
    basic_string<C, T, A> mStr;  // 0x2C
};

} // namespace std

#endif
