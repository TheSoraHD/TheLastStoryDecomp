#ifndef STD_STREAMBUF_H
#define STD_STREAMBUF_H

// Partial reconstruction of MSL C++ std::basic_streambuf (no MSL C++ source is available). The
// virtual functions follow the vtable 0x80779A58 (char); names and signatures from the USA
// LastWorld_tools.rso. Data member names are invented (the six area pointers in the standard
// order, then the locale).

#include "std/string.h"
#include "std/ios_base.h"
#include "std/locale.h"

namespace std {

template <class C, class T = char_traits<C> >
class basic_streambuf {
public:
    typedef C char_type;
    typedef typename T::int_type int_type;
    typedef fpos<int> pos_type;
    typedef streamoff off_type;

    virtual ~basic_streambuf();
    int pubsync() { return sync(); }

protected:
    virtual void imbue(const locale& loc);
    virtual basic_streambuf* setbuf(char_type* s, streamsize n);
    virtual pos_type seekoff(off_type off, ios_base::seekdir way, ios_base::openmode which);
    virtual pos_type seekpos(pos_type sp, ios_base::openmode which);
    virtual int sync();
    virtual streamsize showmanyc();
    virtual streamsize xsgetn(char_type* s, streamsize n);
    virtual int_type underflow();
    virtual int_type uflow();
    virtual int_type pbackfail(int_type c);
    virtual streamsize xsputn(const char_type* s, streamsize n);
    virtual int_type overflow(int_type c);

public:
    char_type* mEback; // 0x04
    char_type* mGptr;  // 0x08
    char_type* mEgptr; // 0x0C
    char_type* mPbase; // 0x10
    char_type* mPptr;  // 0x14
    char_type* mEpptr; // 0x18
    locale mLoc;       // 0x1C
};

} // namespace std

#endif
