#ifndef STD_VECTOR_WSTRING_H
#define STD_VECTOR_WSTRING_H

// inferred: std::vector<std::wstring>, same reallocation as std::vector<std::string> (std/vector_string.h).
// A header of its own: the specialization renumbers the @N labels of every unit that sees it.

#include "std/string.h"
#include "std/vector.h"

namespace std {

// inferred: std::vector<std::wstring> moves its strings the same way (growth of push_back in
// TacView::fn_805AC758, 0x805AD3C4: memcpy + memset of the old elements)
template <>
struct __vec_split_base_unk<basic_string<wchar_t> > {
    __vec_split_base_unk(void* alloc) {
        unsigned int* w = (unsigned int*)this;
        w[0] = 0;
        w[1] = 0;
        w[2] = 0;
        mAlloc = alloc;
    }
    ~__vec_split_base_unk() {
        if (mData != 0) {
            unsigned int n = mSize;
            basic_string<wchar_t>* p = mData + n;
            unsigned int& size = mSize;
            size -= n;
            for (; n != 0; n--) {
                --p;
                p->~basic_string();
            }
            ::operator delete(mData);
        }
    }
    basic_string<wchar_t>* mData;
    unsigned int mSize;
    unsigned int mCap;
    void* mAlloc;
};

template <>
struct __vec_split_unk<basic_string<wchar_t> > : public __vec_split_base_unk<basic_string<wchar_t> > {
    enum { __clear_src_unk = 1 }; // inferred: see std/vector.h
    __vec_split_unk(void* alloc) : __vec_split_base_unk<basic_string<wchar_t> >(alloc), mStart(0) {}
    basic_string<wchar_t>* begin() { return this->mData + mStart; }
    void push_back(const basic_string<wchar_t>& x) {
        new (begin() + this->mSize) basic_string<wchar_t>(x);
        this->mSize++;
    }
    // inferred: moves [first, last) in front of the stored elements as raw bytes
    void __push_front_unk(basic_string<wchar_t>* first, basic_string<wchar_t>* last) {
        // inferred: a byte difference, so that (data + size) - data is not folded (0x805AD480)
        int n = ((char*)last - (char*)first) / (int)sizeof(basic_string<wchar_t>);
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(basic_string<wchar_t>));
        memset(first, 0, n * sizeof(basic_string<wchar_t>));
        this->mSize += n;
    }
    void clear() {
        basic_string<wchar_t>* b = begin();
        basic_string<wchar_t>* p = b + this->mSize;
        while (p > b) {
            --p;
            p->~basic_string();
        }
        this->mSize = 0;
    }
    unsigned int mStart;
};

template <>
inline void vector<basic_string<wchar_t> >::__swap_out_unk(__vec_split_unk<basic_string<wchar_t> >& buf) {
    buf.__push_front_unk(this->mData, this->mData + this->mSize);
    __vec_swap_unk(this->mCap, buf.mCap);
    __vec_swap_unk(this->mData, buf.mData);
    __vec_swap_unk(this->mSize, buf.mSize);
    buf.clear();
}

} // namespace std

#endif
