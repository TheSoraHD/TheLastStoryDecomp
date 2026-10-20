// Weak copy of std::basic_ios<char>::~basic_ios with its vtable and RTTI (no MSL C++ iostream
// source is available; inferred from 0x800E0AC0 and the data at 0x807799E0). These are the tail
// of the game unit before it (the atn::Video code at 0x800DD1EC-0x800E0AC0 writes to a
// std::basic_ostream), so this file only reproduces the function and is not linked.

namespace std {

template <class T>
struct char_traits;

template <>
struct char_traits<char> {};

// inferred: destructor at 0x8068B50C (stores __vt__Q23std8ios_base)
class ios_base {
public:
    virtual ~ios_base();
};

template <class T, class Traits>
class basic_ios : public ios_base {
public:
    virtual ~basic_ios() {}
};

} // namespace std

// inferred: any construction of a stream emits the vtable and the destructor
void* fn_char_traits_char_unk() {
    return new std::basic_ios<char, std::char_traits<char> >;
}
