// Weak copies of std::runtime_error, boost::bad_function_call and the std::tr1::shared_ptr
// deleter (include/std/stdexcept.h), emitted at the end of a game translation unit that calls an
// empty boost::function0 (inferred: the data follows game/AiTarget.cpp directly; the code between
// 0x800185B4 and 0x8001A268 is not split yet). This file only reproduces the functions: the
// real unit is larger, so it is not linked.
#include <boost/function.hpp>
#include <typeinfo>

namespace std {
namespace tr1 {
namespace detail {

// 0x8001A34C
template <class T, class D>
void* shared_ptr_deleter<T, D>::get_deleter(const type_info& ti) const {
    return ti == typeid(D) ? (void*)&mPair : 0;
}

} // namespace detail
} // namespace tr1
} // namespace std

// inferred: any call of an empty boost::function0 instantiates the throw of bad_function_call
void fn_runtime_error_unk(boost::function0<void>& f) {
    f();
}
