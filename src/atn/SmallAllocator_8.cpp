// inferred: engine memory manager (atn::SmallAllocator<N> pools, five atn::DLAllocator heaps and
// the global operators new/delete). The file starts at 0x80081E54 (the MEMAllocator callbacks,
// atn::MemoryManager::Instance and the constructor are outside the split); names invented except
// atn::MemoryManager::Instance/Free (RSO).
#include "IAllocator.h"
#include "atn/DLAllocator.h"
#include "atn/MemoryManager.h"

extern "C" void OSLockMutex(OSMutex_unk* mutex);
extern "C" void OSUnlockMutex(OSMutex_unk* mutex);
extern "C" void* OSGetMEM1ArenaHi();
extern "C" void* OSGetMEM1ArenaLo();
extern "C" void* OSGetMEM2ArenaHi();
extern "C" void* OSGetMEM2ArenaLo();

// inferred: allocation call with the name argument the game passes (the IAllocator declaration
// of atn/DLAllocator.cpp only lists size and alignment)
class NamedAllocator_unk {
public:
    virtual ~NamedAllocator_unk();
    virtual void* vf0C(unsigned long size, unsigned long align, int heap, const char* name);
    virtual void vf10(void* p);
    virtual void* vf14();
    virtual void* vf18();
};
// inferred: DLAllocator::fn_80084EE8 also receives the heap name
extern "C" void fn_80084EE8__Q23atn11DLAllocatorFPvUl(atn::DLAllocator* heap, void* base, unsigned long size,
                                                     const char* name);


// inferred: locks the manager mutex while in scope
struct MemoryLock_unk {
    MemoryLock_unk(atn::MemoryManager* mgr) {
        mMutex = mgr->mUseMutex != 0 ? &mgr->mMutex : 0;
        if (mMutex != 0) {
            OSLockMutex(mMutex);
        }
    }
    ~MemoryLock_unk() {
        if (mMutex != 0) {
            OSUnlockMutex(mMutex);
        }
    }
    OSMutex_unk* mMutex;
};

extern "C" void fn_80082CAC(atn::MemoryManager* mgr);
extern "C" void* fn_80081E54(void* allocator, unsigned long size);
extern "C" void fn_800820BC(void* allocator, void* p);
extern "C" void* fn_80082260(void* allocator, unsigned long size);
extern "C" void* fn_800832AC(atn::MemoryManager* mgr, unsigned long size, unsigned long align, int heap);

static const atn::SmallPoolConfig_unk sSmall8Config[8] = {
    {4, 0x100}, {8, 0x80}, {0x10, 0x200}, {0x20, 0x400}, {0x30, 0x100}, {0x40, 0x80}, {0x50, 0x40}, {0x60, 0x80},
};
static const atn::SmallPoolConfig_unk sSmall6Config[6] = {
    {4, 1}, {8, 1}, {0x10, 1}, {0x20, 1}, {0x30, 1}, {0x40, 1},
};

atn::MemoryManager::MemoryManager()
    : mMode(0), mAlign(0x20), mSmall8(sSmall8Config), mSmall6(sSmall6Config), mUseMutex(0) {
    fn_80082CAC(this);
}


extern "C" void* fn_800838B8(atn::MemoryManager* mgr, unsigned long size, unsigned long align, int heap,
                             const char* name);

// inferred: MEMAllocator callbacks (MEM1, free, MEM2)
extern "C" void* fn_80081E54(void* allocator, unsigned long size) {
    return fn_800838B8(atn::MemoryManager::Instance(), size, 0x20, 0, "");
}

extern "C" void fn_800820BC(void* allocator, void* p) {
    atn::MemoryManager::Instance()->Free(p);
}

extern "C" void* fn_80082260(void* allocator, unsigned long size) {
    return atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)0, "", "", 0, 0);
}

// inferred: allocation hooks of the C library
extern "C" void* __sys_alloc(unsigned long size) {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    return fn_800832AC(mgr, size, mgr->mAlign, 0);
}

extern "C" void __sys_free(void* p) {
    atn::MemoryManager::Instance()->Free(p);
}

atn::MemoryManager* atn::MemoryManager::Instance() {
    static atn::MemoryManager sInstance;
    return &sInstance;
}

static MEMAllocatorFunc_unk sMem1Func = {fn_80081E54, fn_800820BC};
static MEMAllocatorFunc_unk sMem2Func = {fn_80082260, fn_800820BC};

// NONMATCHING: 85.6% - register allocation (the original uses r25-r31 and shares r31 between the
// arena end and the buffer pointer)
// inferred: lays out the heaps in the arenas and the small allocators
extern "C" void fn_80082CAC(atn::MemoryManager* mgr) {
    unsigned char* hi = (unsigned char*)OSGetMEM1ArenaHi();
    unsigned char* lo = (unsigned char*)OSGetMEM1ArenaLo();
    unsigned long size = hi - lo;
    fn_80084EE8__Q23atn11DLAllocatorFPvUl(&mgr->mMatrix, lo, 0x100000, "MatrixBuffers");
    fn_80084EE8__Q23atn11DLAllocatorFPvUl(&mgr->mMem1, lo + 0x100000, size - 0x100000, "MEM1");
    hi = (unsigned char*)OSGetMEM2ArenaHi();
    lo = (unsigned char*)OSGetMEM2ArenaLo();
    size = hi - lo;
    if (size > 0x4000000) {
        unsigned long mem2 = size - 0x4100000;
        fn_80084EE8__Q23atn11DLAllocatorFPvUl(&mgr->mMem2, lo, mem2, "MEM2");
        lo += mem2;
        fn_80084EE8__Q23atn11DLAllocatorFPvUl(&mgr->mOverflow, lo + 0x200000, size - mem2 - 0x200000, "Overflow");
        fn_80084EE8__Q23atn11DLAllocatorFPvUl(&mgr->mMindows, lo, 0x200000, "Mindows");
    } else {
        fn_80084EE8__Q23atn11DLAllocatorFPvUl(&mgr->mMem2, lo, size, "MEM2");
    }
    mgr->mSdk[0].mFunc = &sMem1Func;
    mgr->mSdk[0].mHeap = 0;
    mgr->mSdk[0].mParam1 = 0;
    mgr->mSdk[0].mParam2 = 0;
    mgr->mSdk[1].mFunc = &sMem1Func;
    mgr->mSdk[1].mHeap = 0;
    mgr->mSdk[1].mParam1 = 0;
    mgr->mSdk[1].mParam2 = 0;
    mgr->mSdk[2].mFunc = &sMem2Func;
    mgr->mSdk[2].mHeap = 0;
    mgr->mSdk[2].mParam1 = 0;
    mgr->mSdk[2].mParam2 = 0;

    mgr->mSmall8.Setup_unk(((NamedAllocator_unk*)&mgr->mMem1)->vf0C(mgr->mSmall8.GetSize_unk(), 0x20, 0, "SMA1"));
    mgr->mSmall6.Setup_unk(((NamedAllocator_unk*)&mgr->mMem2)->vf0C(mgr->mSmall6.GetSize_unk(), 0x20, 0, "SMA2"));
}

// inferred: allocates size bytes; heap 0xD/0xE/9 select a heap, small blocks go to the pools
extern "C" void* fn_800832AC(atn::MemoryManager* mgr, unsigned long size, unsigned long align, int heap) {
    MemoryLock_unk lock(mgr);
    if (heap == 0xD) {
        return ((NamedAllocator_unk*)&mgr->mOverflow)->vf0C(size, align, heap, "");
    }
    if (heap == 0xE) {
        void* mindows = ((NamedAllocator_unk*)&mgr->mMindows)->vf0C(size, align, heap, "");
        return mindows;
    }
    void* p;
    if (mgr->mMode == 0 && size <= 0x60 && mgr->mSmall8.m04 != 0) {
        p = ((NamedAllocator_unk*)&mgr->mSmall8)->vf0C(size, align, heap, "");
        if (p != 0) {
            return p;
        }
        if (size <= 0x40 && mgr->mSmall6.m04 != 0) {
            p = ((NamedAllocator_unk*)&mgr->mSmall6)->vf0C(size, align, heap, "");
            if (p != 0) {
                return p;
            }
        }
    } else if (mgr->mMode == 1 && size <= 0x40 && mgr->mSmall6.m04 != 0) {
        p = ((NamedAllocator_unk*)&mgr->mSmall6)->vf0C(size, align, heap, "");
        if (p != 0) {
            return p;
        }
    }
    if (heap == 9) {
        p = ((NamedAllocator_unk*)&mgr->mMatrix)->vf0C(size, align, heap, "");
    } else if (mgr->mMode == 0) {
        p = ((NamedAllocator_unk*)&mgr->mMem1)->vf0C(size, align, heap, "");
        if (p == 0) {
            p = ((NamedAllocator_unk*)&mgr->mMem2)->vf0C(size, align, heap, "");
            if (p == 0) {
                p = ((NamedAllocator_unk*)&mgr->mOverflow)->vf0C(size, align, heap, "");
            }
        }
    } else {
        p = ((NamedAllocator_unk*)&mgr->mMem2)->vf0C(size, align, heap, "");
        if (p == 0) {
            p = ((NamedAllocator_unk*)&mgr->mOverflow)->vf0C(size, align, heap, "");
        }
    }
    return p;
}

namespace atn {

// NONMATCHING: 87.0% - same code, volatile registers numbered differently
template <int N>
void* SmallAllocator<N>::vf0C(unsigned long size, unsigned long align, int heap, const char* name) {
    const SmallPoolConfig_unk* config = mConfig;
    while (config->mSize < size) {
        config++;
    }
    int index = config - mConfig;
    unsigned long* end = mPools[mCount - 1].mBitmap + mConfig[mCount - 1].mCount;
    for (unsigned long* word = mPools[index].mBitmap; word != end; word++) {
        unsigned long bits = *word;
        if (bits != 0xFFFFFFFF) {
            for (int bit = 0; bit < 32; bit++) {
                unsigned long mask = 1 << bit;
                if ((mask & bits) != mask) {
                    *word |= mask;
                    int pool = 0;
                    unsigned long group = word - mPools[0].mBitmap;
                    for (const SmallPoolConfig_unk* c = mConfig; group >= c->mCount; c++) {
                        group -= mConfig[pool].mCount;
                        pool++;
                    }
                    SmallPool_unk& p = mPools[pool];
                    unsigned long blockSize = mConfig[pool].mSize;
                    p.mUsed++;
                    void* ret = p.mBase + bit * blockSize + group * (blockSize << 5);
                    p.mPeak = p.mPeak > p.mUsed ? p.mPeak : p.mUsed;
                    return ret;
                }
            }
        }
    }
    return 0;
}

} // namespace atn

// inferred: allocation with the default alignment
extern "C" void* fn_800838AC(atn::MemoryManager* mgr, unsigned long size, int heap) {
    return fn_800832AC(mgr, size, mgr->mAlign, heap);
}

// inferred: allocation from MEM1 (heap 9: matrix buffers), then Overflow or MEM2
extern "C" void* fn_800838B8(atn::MemoryManager* mgr, unsigned long size, unsigned long align, int heap,
                             const char* name) {
    MemoryLock_unk lock(mgr);
    void* p;
    if (heap == 9) {
        p = ((NamedAllocator_unk*)&mgr->mMatrix)->vf0C(size, align, heap, name);
    } else {
        p = ((NamedAllocator_unk*)&mgr->mMem1)->vf0C(size, align, heap, name);
    }
    if (p == 0) {
        if (mgr->mOverflow.fn_80084FA0() != 0) {
            p = ((NamedAllocator_unk*)&mgr->mOverflow)->vf0C(size, align, heap, name);
        } else {
            p = ((NamedAllocator_unk*)&mgr->mMem2)->vf0C(size, align, heap, name);
        }
    }
    return p;
}

// inferred: allocation from MEM2, then Overflow
void* atn::MemoryManager::AllocFromMem2(unsigned long size, unsigned long align, MemoryTag heap, const char* name,
                                        const char* file, int line, int mayFail) {
    void* p;
    {
        MemoryLock_unk lock(this);
        p = ((NamedAllocator_unk*)&mMem2)->vf0C(size, align, heap, name);
        if (p == 0) {
            p = ((NamedAllocator_unk*)&mOverflow)->vf0C(size, align, heap, name);
        }
    }
    if (mayFail != 0 && p == 0) {
        return 0;
    }
    return p;
}

// inferred: true when p is inside the area of the allocator
static inline bool MemoryContains_unk(IAllocator* allocator, void* p) {
    bool ret = false;
    if (p >= allocator->vf18() && p <= allocator->vf14()) {
        ret = true;
    }
    return ret;
}

// inferred: frees p in the allocator that owns it
void atn::MemoryManager::Free(void* p) {
    if (p == 0) {
        return;
    }
    MemoryLock_unk lock(this);
    if (MemoryContains_unk(&mSmall8, p)) {
        mSmall8.vf10(p);
    } else if (MemoryContains_unk(&mSmall6, p)) {
        mSmall6.vf10(p);
    } else if (MemoryContains_unk(&mMem1, p)) {
        mMem1.vf10(p);
    } else if (MemoryContains_unk(&mMatrix, p)) {
        mMatrix.vf10(p);
    } else if (MemoryContains_unk(&mMem2, p)) {
        mMem2.vf10(p);
    } else if (MemoryContains_unk(&mOverflow, p)) {
        mOverflow.vf10(p);
    } else if (MemoryContains_unk(&mMindows, p)) {
        mMindows.vf10(p);
    }
}

namespace atn {

template <int N>
void SmallAllocator<N>::vf10(void* p) {
    for (int i = mCount - 1; i >= 0; i--) {
        if (mPools[i].mBase <= p) {
            SmallPool_unk& pool = mPools[i];
            int block = ((unsigned char*)p - pool.mBase) / mConfig[i].mSize;
            pool.mBitmap[block / 32] &= ~(1 << (block % 32));
            pool.mUsed--;
            return;
        }
    }
}

} // namespace atn

// inferred: selects the allocation mode while in scope (name invented)
struct MemoryMode_unk {
    MemoryMode_unk(int mode, unsigned long align);
    ~MemoryMode_unk();
};

MemoryMode_unk::MemoryMode_unk(int mode, unsigned long align) {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    mgr->mMode = mode;
    mgr->mAlign = align;
}

// NONMATCHING: 42.5% - the operator delete part inlines the manager constructor (see __dl__FPv)
MemoryMode_unk::~MemoryMode_unk() {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    mgr->mMode = 0;
    mgr->mAlign = 0x20;
}

void* operator new(unsigned long size, int heap, const char* name, const char* file, int line) {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    return fn_800832AC(mgr, size, mgr->mAlign, heap);
}

void* operator new(unsigned long size) {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    return fn_800832AC(mgr, size, mgr->mAlign, 0);
}

// NONMATCHING: 0% (structure) - the original calls the manager constructor fn_80082970 out of
// line inside the inlined MemoryManager::Instance; ours inlines it like operator new does. The constructor,
// MemoryManager::Instance, the destructors and operator delete call each other (a cycle), and the order in
// which MWCC resolves it decides which copies get the constructor inlined.
void operator delete(void* p) {
    atn::MemoryManager::Instance()->Free(p);
}

void* operator new[](unsigned long size, int heap, const char* name, const char* file, int line) {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    return fn_800832AC(mgr, size, mgr->mAlign, heap);
}

// inferred: array allocation with an explicit alignment
extern "C" void* fn_800848B4(unsigned long size, unsigned long align, int heap, const char* name, const char* file,
                             int line) {
    return fn_800832AC(atn::MemoryManager::Instance(), size, align, heap);
}

void* operator new[](unsigned long size) {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    return fn_800832AC(mgr, size, mgr->mAlign, 0);
}

void operator delete[](void* p) {
    atn::MemoryManager::Instance()->Free(p);
}

