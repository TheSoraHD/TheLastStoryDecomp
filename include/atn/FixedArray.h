#ifndef ATN_FIXEDARRAY_H
#define ATN_FIXEDARRAY_H

#ifndef MSL_CPP_NEW_H
// placement new, as in MSL <new>
inline void* operator new(unsigned long, void* ptr) { return ptr; }
#endif

namespace atn {

// inferred: fixed-capacity array {count, elements[N]} used by game tasks (name unknown).
// push_back constructs the element in place (null check on the slot address, e.g. 0x805B9C64).
template <class T, int N>
class FixedArray_unk {
public:
    FixedArray_unk() : mSize(0) {}
    unsigned int size() const { return mSize; }
    T& operator[](int i) { return mData[i]; }
    void push_back(const T& value) {
        new (&mData[mSize]) T(value);
        mSize++;
    }

    unsigned int mSize;
    T mData[N];
};

} // namespace atn

#endif
