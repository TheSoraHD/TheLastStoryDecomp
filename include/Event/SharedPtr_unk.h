#ifndef EVENT_SHAREDPTR_UNK_H
#define EVENT_SHAREDPTR_UNK_H

// Partial reconstruction of boost::shared_ptr as used by the Event tree (Cut, Label, Section,
// Channel). Layout from the inlined constructors: sp_counted_base is {use count, weak count,
// vptr} (MWCC puts the vptr after the data members declared before the first virtual) and
// sp_counted_impl_p adds the pointer at 0xC.
// The original counts with lwarx/stwcx. loops (boost's CodeWarrior PPC atomics are asm
// functions); without asm they are plain increments here, so every function that copies or
// releases a shared_ptr cannot match.

#include <new>
#include "boost/checked_delete.hpp"
#include "boost/throw_exception.hpp"

namespace std {
class type_info;
}

namespace boost {

namespace detail {

class sp_counted_base {
    long use_count_;
    long weak_count_;

public:
    sp_counted_base() : use_count_(1), weak_count_(1) {}
    virtual ~sp_counted_base() {}
    virtual void dispose() = 0;
    // inferred: weak copy 0x8053AC68 (Event/Channel)
    virtual void destroy() { delete this; }
    virtual void* get_deleter(const std::type_info& ti) = 0;

    void add_ref_copy() { ++use_count_; } // inferred: atomic in the original
    void release() {
        if (--use_count_ == 0) { // inferred: atomic in the original
            dispose();
            weak_release();
        }
    }
    void weak_release() {
        if (--weak_count_ == 0) { // inferred: atomic in the original
            destroy();
        }
    }
};

template <class X>
class sp_counted_impl_p : public sp_counted_base {
    X* px_;

public:
    explicit sp_counted_impl_p(X* px) : px_(px) {}
    virtual void dispose() { boost::checked_delete(px_); }
    virtual void* get_deleter(const std::type_info&) { return 0; }
};

class shared_count {
    sp_counted_base* pi_;

public:
    template <class Y>
    explicit shared_count(Y* p) : pi_(0) {
        pi_ = new sp_counted_impl_p<Y>(p);
        if (pi_ == 0) {
            boost::checked_delete(p);
            boost::throw_exception(std::bad_alloc());
        }
    }
    shared_count(const shared_count& r) : pi_(r.pi_) {
        if (pi_ != 0) {
            pi_->add_ref_copy();
        }
    }
    ~shared_count() {
        if (pi_ != 0) {
            pi_->release();
        }
    }
};

// inferred: weak copy 0x8053AC88 (Event/Channel), variadic and empty
inline void sp_enable_shared_from_this(const shared_count&, ...) {}

} // namespace detail

template <class T>
class shared_ptr {
public:
    template <class Y>
    explicit shared_ptr(Y* p) : px(p), pn(p) {
        detail::sp_enable_shared_from_this(pn, p, p);
    }
    T* get() const { return px; }
    T* operator->() const { return px; }

    T* px;
    detail::shared_count pn;
};

} // namespace boost

#endif
