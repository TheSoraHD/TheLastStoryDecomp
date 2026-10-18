#ifndef STD_STDEXCEPT_H
#define STD_STDEXCEPT_H

// Partial reconstruction of MSL C++'s <stdexcept> as used by the game (no MSL C++ source is
// available). Inferred from the weak copies of the std/runtime_error.cpp unit (0x8001A268-
// 0x8001A34C), the shared_ptr deleter at 0x8001A34C-0x8001A440 (vtable 0x80775B98) and the
// bad_function_call throw inlined in boost::function0::operator() (CharaTask::fn_8013A258,
// 0x8013A380). The message is kept in a std::tr1::shared_ptr<char> to an array allocated with
// new[] (RTTI names in the DOL: std::tr1::detail::shared_ptr_deleter_common,
// std::tr1::detail::shared_ptr_deleter<char, std::detail::default_delete<char[]>>,
// std::detail::default_delete<char[]>). Member names are invented.

#include <exception>

extern "C" unsigned long strlen(const char* s);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strcmp(const char* a, const char* b);

namespace std {

class type_info;

namespace detail {

template <class T>
struct default_delete {
    void operator()(T* p) const { delete p; }
};

template <class T>
struct default_delete<T[]> {
    void operator()(T* p) const { delete[] p; }
};

} // namespace detail

namespace tr1 {
namespace detail {

// inferred: reference counts at 0x04/0x08; release() is out of line (0x806952C4)
class shared_ptr_deleter_common {
public:
    shared_ptr_deleter_common() : mUseCount(1), mWeakCount(1) {}
    virtual ~shared_ptr_deleter_common() {}
    virtual void* get_deleter(const type_info& ti) = 0;
    virtual void dispose() = 0;
    void release();

    long mUseCount;
    long mWeakCount;
};

// inferred: the (empty) deleter shares the address of the pointer (0x8001A34C returns this + 0xC)
template <class T, class D>
struct __ptr_deleter_pair_unk : public D {
    __ptr_deleter_pair_unk(T* p, const D& d) : D(d), mPtr(p) {}
    T* mPtr;
};

template <class T, class D>
class shared_ptr_deleter : public shared_ptr_deleter_common {
public:
    shared_ptr_deleter(T* p, const D& d) : mPair(p, d) {}
    virtual void* get_deleter(const type_info& ti);
    virtual void dispose() { ((D&)mPair)(mPair.mPtr); }

    __ptr_deleter_pair_unk<T, D> mPair;
};

} // namespace detail

// inferred: holds the pointer while the control block is allocated (released afterwards; its
// destructor survives as dead code in 0x8013A40C)
template <class T, class D>
struct __hold_ptr_unk {
    __hold_ptr_unk(T* p, D& d) : mPtr(p), mDeleter(d) {}
    ~__hold_ptr_unk() {
        if (mPtr != 0) {
            mDeleter(mPtr);
        }
    }
    T* release() {
        T* p = mPtr;
        mPtr = 0;
        return p;
    }
    T* mPtr;
    D& mDeleter;
};

// inferred: the destructor of runtime_error tests the address of its shared_ptr twice before the
// release (0x8013A43C), so the pointer and the count live in a base class with the destructor
template <class T>
struct __shared_ptr_base_unk {
    __shared_ptr_base_unk(T* p) : mPtr(p) {}
    ~__shared_ptr_base_unk() {
        if (mCntrl != 0) {
            mCntrl->release();
        }
    }

    T* mPtr;
    detail::shared_ptr_deleter_common* mCntrl;
};

template <class T>
class shared_ptr : public __shared_ptr_base_unk<T> {
public:
    template <class Y, class D>
    shared_ptr(Y* p, D d) : __shared_ptr_base_unk<T>(p) {
        __hold_ptr_unk<Y, D> hold(p, d);
        this->mCntrl = new detail::shared_ptr_deleter<Y, D>(p, d);
        hold.release();
    }
    T* get() const { return this->mPtr; }
};

} // namespace tr1

class logic_error : public exception {
public:
    explicit logic_error(const char* what_arg)
        : mWhat(new char[strlen(what_arg) + 1], std::detail::default_delete<char[]>()) {
        strcpy(mWhat.get(), what_arg);
    }
    virtual const char* what() const { return mWhat.get(); }

private:
    tr1::shared_ptr<char> mWhat;
};

class runtime_error : public exception {
public:
    explicit runtime_error(const char* what_arg)
        : mWhat(new char[strlen(what_arg) + 1], std::detail::default_delete<char[]>()) {
        strcpy(mWhat.get(), what_arg);
    }
    // declared only (boost::escaped_list_error, never used by the game)
    template <class S>
    explicit runtime_error(const S& what_arg);
    virtual const char* what() const { return mWhat.get(); } // 0x8001A268

private:
    tr1::shared_ptr<char> mWhat;
};

} // namespace std

#endif
