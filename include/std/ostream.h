#ifndef STD_OSTREAM_H
#define STD_OSTREAM_H

// Partial reconstruction of MSL C++ std::basic_ostream (no MSL C++ source is available). The
// stream has basic_ios as virtual base: the virtual-base pointer at 0x00, the vtable at 0x04
// (0x80779AA0, char: only the destructor) and basic_ios at 0x08 (thunk @8@60@ 0x800E2A10).

#include "std/streambuf.h"

namespace std {

template <class C, class T = char_traits<C> >
class basic_ostream : virtual public basic_ios<C, T> {
public:
    // inferred layout: two flags and the stream (0x800E14A4)
    class sentry {
    public:
        explicit sentry(basic_ostream& os);
        ~sentry();

        bool mOk;
        bool mNested_unk; // inferred: set on the sentry of the tied stream
        basic_ostream* mOs;
    };

    virtual ~basic_ostream();
    basic_ostream& flush();
};

} // namespace std

#endif
