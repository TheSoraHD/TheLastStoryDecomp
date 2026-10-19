// Based on doldecomp/mkw (CC0 1.0), lib/dwc/common/dwc_memfunc.c (commit
// 8b9b5ee653da28663e498039e52f5d19f929a011). The Last Story links a later DWC build
// (Aug 27 2010): allocation failures are reported instead of asserted.
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>

// inferred: memory copy helper of the DWC utilities (dst, src, size); no source yet
void fn_806A9CA0(void* dst, const void* src, u32 size);

static DWCAllocEx s_alloc;
static DWCFreeEx s_free;

static DWCi_AllocateHeader* DWCi_GetAllocateHeader(void* ptr) {
    return (DWCi_AllocateHeader*)((u8*)ptr - sizeof(DWCi_AllocateHeader));
}

static void* DWCi_SetAllocateHeader(DWCi_AllocateHeader* header, u32 size) {
    header->signature = DWCi_MEMFUNC_SIGNATURE;
    header->size = size;
    return header + 1;
}

void DWCi_SetMemFunc(DWCAllocEx alloc, DWCFreeEx free) {
    s_alloc = alloc;
    s_free = free;
}

void* DWC_Alloc(DWCAllocType name, u32 size) {
    return DWC_AllocEx(name, size, 32);
}

void* DWC_AllocEx(DWCAllocType name, u32 size, int align) {
    DWCi_AllocateHeader* header =
        (DWCi_AllocateHeader*)s_alloc(name, size + sizeof(DWCi_AllocateHeader), align);

    if (header == NULL) {
        DWC_Printf(DWC_REPORTFLAG_ERROR,
                   "Failed to alloc memory. Original requested size: %d, Required size: %d, align: %d.\n",
                   size, size + sizeof(DWCi_AllocateHeader), align);
        return NULL;
    }

    return DWCi_SetAllocateHeader(header, size);
}

void DWC_Free(DWCAllocType name, void* ptr, u32 size) {
    if (ptr != NULL) {
        s_free(name, DWCi_GetAllocateHeader(ptr), size);
    }
}

void* DWC_Realloc(DWCAllocType name, void* ptr, u32 oldSize, u32 newSize) {
    return DWC_ReallocEx(name, ptr, oldSize, newSize, 32);
}

void* DWC_ReallocEx(DWCAllocType name, void* ptr, u32 oldSize, u32 newSize, int align) {
    u32 allocSize;
    void* newPtr = DWC_AllocEx(name, newSize, align);

    if (newPtr == NULL) {
        return NULL;
    }

    if (ptr != NULL) {
        allocSize = DWCi_GetAllocateHeader(ptr)->size;
        fn_806A9CA0(newPtr, ptr, allocSize > newSize ? newSize : allocSize);
        DWC_Free(name, ptr, allocSize);
    }

    return newPtr;
}

// inferred: the GameSpy callbacks take a signed size (an unsigned one changes register use)
void* DWCi_GsMalloc(int size) {
    return DWC_Alloc(DWC_ALLOCTYPE_GS, size);
}

void* DWCi_GsRealloc(void* ptr, int size) {
    return DWC_ReallocEx(DWC_ALLOCTYPE_GS, ptr, size, size, 32);
}

void DWCi_GsFree(void* ptr) {
    DWC_Free(DWC_ALLOCTYPE_GS, ptr, 0);
}

void* DWCi_GsMemalign(int align, int size) {
    return DWC_AllocEx(DWC_ALLOCTYPE_GS, size, align);
}
