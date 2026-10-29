// Metrowerks::bufferedbuf<char>::showmanyc (MSL C++, name from the USA LastWorld_tools.rso): the
// characters left in the get area. Only this function of the class is in this split.
#include "std/streambuf.h"

namespace Metrowerks {

// inferred: MSL stream buffer of the file streams (vtable 0x807BBCC8); only showmanyc declared
template <class C, class T>
class bufferedbuf : public std::basic_streambuf<C, T> {
protected:
    virtual std::streamsize showmanyc();
};

template <class C, class T>
std::streamsize bufferedbuf<C, T>::showmanyc() {
    return this->mEgptr - this->mGptr;
}

} // namespace Metrowerks

template std::streamsize Metrowerks::bufferedbuf<char, std::char_traits<char> >::showmanyc();
