#include "atn/DLAllocator.h"

// dlmalloc 2.8.3 mspace API (atn/DLMalloc_unk.cpp)
struct DLMallInfo {
    int m[10];
};
extern "C" void* create_mspace_with_base(void* base, unsigned long size, int locked);
extern "C" void destroy_mspace(void* msp);
extern "C" void* mspace_memalign(void* msp, unsigned long align, unsigned long size);
extern "C" void mspace_free(void* msp, void* p);
extern "C" DLMallInfo mspace_mallinfo(void* msp);

namespace atn {

DLAllocator::DLAllocator() : mBase(0), mSize(0), m0C(0), m10(0), mSpace(0) {}

DLAllocator::~DLAllocator() {
    if (mSpace != 0) {
        destroy_mspace(mSpace);
    }
}

void DLAllocator::fn_80084EE8(void* base, unsigned long size) {
    mBase = base;
    mSize = size;
    mSpace = create_mspace_with_base(base, size, 1);
}

void* DLAllocator::vf0C(unsigned long size, unsigned long align) {
    if (mBase != 0) {
        return mspace_memalign(mSpace, align, size);
    }
    return 0;
}

void DLAllocator::vf10(void* p) {
    if (mSpace != 0) {
        mspace_free(mSpace, p);
    }
}

void* DLAllocator::vf14() { return (char*)mBase + mSize; }

void* DLAllocator::vf18() { return mBase; }

void* DLAllocator::fn_80084F84() {
    if (mBase != 0) {
        return m10;
    }
    return 0;
}

unsigned long DLAllocator::fn_80084FA0() { return mSize; }

void DLAllocator::vf1C() {
    if (mSpace != 0) {
        DLMallInfo info = mspace_mallinfo(mSpace);
    }
}

} // namespace atn
