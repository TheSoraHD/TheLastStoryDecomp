#ifndef IALLOCATOR_H
#define IALLOCATOR_H

// inferred: allocator interface (no vtable of its own in the DOL). Virtuals named by vtable offset.
// vf0C takes a heap number and a name besides size and alignment (atn::LinearAllocator and the
// calls of atn/SmallAllocator_8.cpp pass them).
class IAllocator {
public:
    virtual ~IAllocator() {}
    virtual void* vf0C(unsigned long size, unsigned long align, int heap, const char* name) = 0; // allocate
    virtual void vf10(void* p) = 0;                                  // free
    virtual void* vf14() = 0;                                        // end of the managed area
    virtual void* vf18() = 0;                                        // start of the managed area
    virtual void vf1C() = 0;
};

#endif
