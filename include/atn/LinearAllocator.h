#ifndef ATN_LINEARALLOCATOR_H
#define ATN_LINEARALLOCATOR_H

#include "IAllocator.h"

namespace atn {

// inferred: allocator that hands out consecutive blocks of a fixed buffer
class LinearAllocator : public IAllocator {
public:
    LinearAllocator(void* base, unsigned long size) : mBase(base), mSize(size), mUsed(0) {}
    virtual ~LinearAllocator() {}
    virtual void* vf0C(unsigned long size, unsigned long align, int heap, const char* name);
    virtual void vf10(void* p) {}
    virtual void* vf14() { return (unsigned char*)mBase + mSize; }
    virtual void* vf18() { return mBase; }
    virtual void vf1C() {}

    void* mBase;         // 0x04
    unsigned long mSize; // 0x08
    unsigned long mUsed; // 0x0C
};

} // namespace atn

#endif
