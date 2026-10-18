#ifndef STD_LIST_H
#define STD_LIST_H

// Partial reconstruction of the MSL C++ list used by the game. Only what the game code inlines is
// here. Layout inferred from the loader manager (atn::LoadManager_unk, 0x80472DCC): size, then the
// sentinel node {prev, next}; nodes are {prev, next, value}. Class names are invented.

#include "std/msl_common.h"
#include "std/new.h"

namespace std {

struct __list_node_base_unk {
    __list_node_base_unk* mPrev;
    __list_node_base_unk* mNext;
};

template <class T>
struct __list_node_unk : public __list_node_base_unk {
    T mData;
};

// inferred: holds a new node until it is linked (frees it otherwise). Built on the stack by insert:
// {owner, node}, cleared after the link (0x80472F10).
template <class T>
struct __list_node_holder_unk {
    __list_node_holder_unk(__list_node_base_unk* owner, __list_node_unk<T>* node)
        : mOwner(owner), mNode(node) {}
    ~__list_node_holder_unk() {
        if (mNode != 0) {
            ::operator delete(mNode);
        }
    }
    __list_node_base_unk* mOwner;
    __list_node_unk<T>* mNode;
};

template <class T>
class list {
public:
    typedef unsigned int size_type;

    class iterator {
    public:
        iterator() {}
        iterator(__list_node_base_unk* node) : mNode(node) {}
        T& operator*() const { return ((__list_node_unk<T>*)mNode)->mData; }
        iterator& operator++() {
            mNode = mNode->mNext;
            return *this;
        }
        bool operator==(const iterator& other) const { return mNode == other.mNode; }
        bool operator!=(const iterator& other) const { return mNode != other.mNode; }
        __list_node_base_unk* mNode;
    };

    iterator begin() { return iterator(mTail.mNext); }
    iterator end() { return iterator(&mTail); }
    size_type size() const { return mSize; }

    iterator insert(iterator pos, const T& x) {
        __list_node_unk<T>* node = __allocate();
        __list_node_holder_unk<T> holder(&mTail, node);
        // inferred: through a pointer the holder stays in memory and its destructor is folded but
        // left as dead code after the link, as in the original (0x80472F10)
        __list_node_holder_unk<T>* h = &holder;
        new (&node->mData) T(x);
        __link_before(pos.mNode, h->mNode);
        h->mNode = 0;
        mSize++;
        return iterator(node);
    }
    void push_back(const T& x) { insert(end(), x); }

private:
    static void __link_before(__list_node_base_unk* pos, __list_node_base_unk* node) {
        pos->mPrev->mNext = node;
        node->mPrev = pos->mPrev;
        pos->mPrev = node;
        node->mNext = pos;
    }
    static __list_node_unk<T>* __allocate() {
        __list_node_unk<T>* p = (__list_node_unk<T>*)::operator new(sizeof(__list_node_unk<T>));
        if (p == 0) {
            __msl_error("Memory allocation failure");
        }
        return p;
    }

public:
    size_type mSize;
    __list_node_base_unk mTail;
};

} // namespace std

#endif
