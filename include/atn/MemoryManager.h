#ifndef ATN_MEMORYMANAGER_H
#define ATN_MEMORYMANAGER_H

// atn::MemoryManager: engine memory manager (atn::SmallAllocator<N> pools and five atn::DLAllocator
// heaps; code in atn/SmallAllocator_8.cpp). Names from the RSO: Instance, Free, AllocFromMem2; the rest invented.
#include "atn/DLAllocator.h"
#include "atn/SmallAllocator.h"

// inferred: OSMutex of the SDK (0x18 bytes)
struct OSMutex_unk {
    unsigned int m00[6];
};

// inferred: allocator callbacks of the SDK (MEMAllocator)
struct MEMAllocatorFunc_unk {
    void* (*mAlloc)(void* allocator, unsigned long size);
    void (*mFree)(void* allocator, void* p);
};
struct MEMAllocator_unk {
    const MEMAllocatorFunc_unk* mFunc;
    void* mHeap;
    unsigned long mParam1;
    unsigned long mParam2;
};

// RSO: global enum MemoryTag (heap of an allocation: operator new(unsigned long, MemoryTag, ...)
// and MemoryManager::AllocFromMem2); enumerator names unknown, values seen: 0, 1, 6
enum MemoryTag {};

namespace atn {

class MemoryManager {
public:
    MemoryManager();
    static MemoryManager* Instance(); // 0x800827E0
    void Free(void* p);               // 0x80083AD4
    // RSO: AllocFromMem2__Q23atn13MemoryManagerFUlUl9MemoryTagPCcPCcii (0x800839EC)
    void* AllocFromMem2(unsigned long size, unsigned long align, MemoryTag tag, const char* name, const char* file,
                        int line, int mayFail);

    int mMode;                              // 0x000: inferred: 1 = small blocks only from SMA2
    unsigned long mAlign;                   // 0x004
    MEMAllocator_unk mSdk[3];               // 0x008
    atn::SmallAllocator<8> mSmall8;         // 0x038
    atn::SmallAllocator<6> mSmall6;         // 0x0C8
    atn::DLAllocator mMem1;                 // 0x138
    atn::DLAllocator mMem2;                 // 0x150
    atn::DLAllocator mOverflow;             // 0x168
    atn::DLAllocator mMindows;              // 0x180
    atn::DLAllocator mMatrix;               // 0x198
    int mUseMutex;                          // 0x1B0
    OSMutex_unk mMutex;                     // 0x1B4
};

} // namespace atn

#endif
