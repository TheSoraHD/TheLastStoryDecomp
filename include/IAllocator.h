#ifndef IALLOCATOR_H
#define IALLOCATOR_H

// inferred: allocator interface (no vtable of its own in the DOL). Virtuals named by vtable offset.
class IAllocator {
public:
    virtual ~IAllocator() {}
    virtual void* vf0C(unsigned long size, unsigned long align) = 0; // allocate
    virtual void vf10(void* p) = 0;                                  // free
    virtual void* vf14() = 0;                                        // end of the managed area
    virtual void* vf18() = 0;                                        // start of the managed area
    virtual void vf1C() = 0;
};

#endif
