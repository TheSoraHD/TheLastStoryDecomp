// std::basic_ios<char>::~basic_ios with its vtable and RTTI (no MSL C++ iostream source is
// available; inferred from 0x800E0AC0 and the data at 0x807799E0). This is the tail of the game
// unit before it (the atn::Video code at 0x800DD1EC-0x800E0AC0 writes to a std::basic_ostream).
// The specialization with an out-of-line destructor (key function) emits exactly the weak
// function, the vtable and both RTTI records, so the unit links on its own.

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
class basic_ios;

template <>
class basic_ios<char, char_traits<char> > : public ios_base {
public:
    virtual ~basic_ios();
};

basic_ios<char, char_traits<char> >::~basic_ios() {}

} // namespace std
