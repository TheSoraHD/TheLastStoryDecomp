#ifndef ATN_STATIC_STRING_FWD_H
#define ATN_STATIC_STRING_FWD_H

// declaration of atn::static_string with its default arguments (definition in atn/static_string.h)
namespace std {
template <class T> struct char_traits;
}

namespace atn {
template <int N, class C = char, class Tr = std::char_traits<C> >
class static_string;
}

#endif
