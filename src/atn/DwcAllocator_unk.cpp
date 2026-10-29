// inferred: heap of the DWC (Nintendo Wi-Fi Connection) library: a 1 MB atn::DLAllocator taken from
// the engine memory manager, with the alloc/free callbacks given to the SDK. Name invented.
#include "atn/DLAllocator.h"
#include "atn/MemoryManager.h"
#include "atn/Memory.h"


// inferred: DLAllocator::fn_80084EE8 also receives the heap name (as in atn/SmallAllocator_8.cpp)
extern "C" void fn_80084EE8__Q23atn11DLAllocatorFPvUl(atn::DLAllocator* heap, void* base, unsigned long size,
                                                     const char* name);

// inferred: allocation call with the name argument the game passes (as in atn/SmallAllocator_8.cpp)
class NamedAllocator_unk {
public:
    virtual ~NamedAllocator_unk();
    virtual void* vf0C(unsigned long size, unsigned long align, int heap, const char* name);
    virtual void vf10(void* p);
    virtual void* vf14();
    virtual void* vf18();

    // inferred: the pointer is inside the managed area
    bool Contains_unk(void* p) { return p >= vf18() && p <= vf14(); }
};

// inferred: mutex of the network code (lock 0x80509A88, unlock 0x80509A90)
extern "C" void* lbl_8087F628;
extern "C" void fn_80509A88(void* mutex);
extern "C" void fn_80509A90(void* mutex);

NamedAllocator_unk* lbl_8087F878;

extern "C" void fn_8050D3E8() {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    mgr->mUseMutex = 1;
    lbl_8087F878 = (NamedAllocator_unk*)new (5, "", "", 0) atn::DLAllocator;
    void* base = atn::MemoryManager::Instance()->AllocFromMem2(0x100000, 0x20, (MemoryTag)5, "", "", 0, 0);
    fn_80084EE8__Q23atn11DLAllocatorFPvUl((atn::DLAllocator*)lbl_8087F878, base, 0x100000, "DwcAllocator");
}

extern "C" void fn_8050D484() {
    atn::MemoryManager* mgr = atn::MemoryManager::Instance();
    mgr->mUseMutex = 0;
    void* base = lbl_8087F878->vf18();
    atn::MemoryManager::Instance()->Free(base);
    delete lbl_8087F878;
    lbl_8087F878 = 0;
}

// inferred: DWCAllocEx(name, size, align)
extern "C" void* fn_8050D500(int name, unsigned long size, int align) {
    fn_80509A88(lbl_8087F628);
    void* p = lbl_8087F878->vf0C(size, align, 5, "DWCAlloc");
    fn_80509A90(lbl_8087F628);
    if (p == 0) {
        p = atn::MemoryManager::Instance()->AllocFromMem2(size, align, (MemoryTag)5, "", "", 0, 0);
    }
    return p;
}

// inferred: DWCFreeEx(name, ptr, size)
extern "C" void fn_8050D5B8(int name, void* p, unsigned long size) {
    if (p != 0) {
        if (lbl_8087F878->Contains_unk(p)) {
            fn_80509A88(lbl_8087F628);
            lbl_8087F878->vf10(p);
            fn_80509A90(lbl_8087F628);
        } else {
            atn::MemoryManager::Instance()->Free(p);
        }
    }
}

// inferred: alloc callback of another library (32-byte aligned)
extern "C" void* fn_8050D67C(int arg, unsigned long size) {
    fn_80509A88(lbl_8087F628);
    void* p = lbl_8087F878->vf0C(size, 0x20, 5, "DWCAlloc");
    fn_80509A90(lbl_8087F628);
    if (p == 0) {
        p = atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)5, "", "", 0, 0);
    }
    return p;
}

// inferred: free callback of the same library
extern "C" void fn_8050D728(int arg, void* p) {
    if (p != 0) {
        if (lbl_8087F878->Contains_unk(p)) {
            fn_80509A88(lbl_8087F628);
            lbl_8087F878->vf10(p);
            fn_80509A90(lbl_8087F628);
        } else {
            atn::MemoryManager::Instance()->Free(p);
        }
    }
}
