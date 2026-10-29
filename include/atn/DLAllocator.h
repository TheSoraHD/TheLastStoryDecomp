#ifndef ATN_DLALLOCATOR_H
#define ATN_DLALLOCATOR_H

#include "IAllocator.h"

namespace atn {

// inferred: allocator on top of a dlmalloc mspace created over a caller-provided buffer.
class DLAllocator : public IAllocator {
public:
    DLAllocator();
    virtual ~DLAllocator();
    void fn_80084EE8(void* base, unsigned long size); // inferred: init
    virtual void* vf0C(unsigned long size, unsigned long align, int heap, const char* name);
    virtual void vf10(void* p);
    virtual void* vf14();
    virtual void* vf18();
    void* fn_80084F84();
    unsigned long fn_80084FA0();
    virtual void vf1C();

    void* mBase;         // 0x04
    unsigned long mSize; // 0x08
    int m0C;
    void* m10;
    void* mSpace;        // 0x14: dlmalloc mspace
};

} // namespace atn

#endif
