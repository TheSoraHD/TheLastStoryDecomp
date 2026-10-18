#include "atn/DLAllocator.h"

// inferred: dlmalloc mspace API (create_mspace_with_base, destroy_mspace, mspace_memalign,
// mspace_free, mspace_mallinfo); automatic names until the library is identified.
struct DLMallInfo {
    int m[10];
};
extern "C" void* fn_8022AB18(void* base, unsigned long size, int locked);
extern "C" void fn_8022ADB8(void* msp);
extern "C" void* fn_8022B848(void* msp, unsigned long align, unsigned long size);
extern "C" void fn_8022B114(void* msp, void* p);
extern "C" DLMallInfo fn_8022B84C(void* msp);

namespace atn {

DLAllocator::DLAllocator() : mBase(0), mSize(0), m0C(0), m10(0), mSpace(0) {}

DLAllocator::~DLAllocator() {
    if (mSpace != 0) {
        fn_8022ADB8(mSpace);
    }
}

void DLAllocator::fn_80084EE8(void* base, unsigned long size) {
    mBase = base;
    mSize = size;
    mSpace = fn_8022AB18(base, size, 1);
}

void* DLAllocator::vf0C(unsigned long size, unsigned long align) {
    if (mBase != 0) {
        return fn_8022B848(mSpace, align, size);
    }
    return 0;
}

void DLAllocator::vf10(void* p) {
    if (mSpace != 0) {
        fn_8022B114(mSpace, p);
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
        DLMallInfo info = fn_8022B84C(mSpace);
    }
}

} // namespace atn
