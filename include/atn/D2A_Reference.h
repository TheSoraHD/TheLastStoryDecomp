#ifndef ATN_D2A_REFERENCE_H
#define ATN_D2A_REFERENCE_H

#include "atn/D2A_Common.h"

namespace atn {

// inferred: owned array {count, data}, released with delete[]
template <typename T>
struct D2A_Array {
    D2A_Array() : mCount(0), mData(0) {}
    ~D2A_Array() {
        if (mData != 0) {
            delete[] mData;
        }
        mData = 0;
        mCount = 0;
    }
    int mCount;
    T* mData;
};

class D2A_Reference : public D2A_Common {
public:
    D2A_Reference(D2A_Data* owner, unsigned short id);
    virtual ~D2A_Reference();
    virtual void vf18();

    D2A_Array<int> m12C; // inferred element type (trivially destructible)
};

} // namespace atn

#endif
