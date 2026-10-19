// Based on doldecomp/mkw (CC0 1.0), lib/dwc/common/dwc_memfunc.h and dwci_memfunc.h.
#ifndef DWC_MEMFUNC_H
#define DWC_MEMFUNC_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DWC_ALLOCTYPE_AUTH,
    DWC_ALLOCTYPE_AC,
    DWC_ALLOCTYPE_BM,
    DWC_ALLOCTYPE_UTIL,
    DWC_ALLOCTYPE_BASE,
    DWC_ALLOCTYPE_LANMATCH,
    DWC_ALLOCTYPE_GHTTP,
    DWC_ALLOCTYPE_RANKING,
    DWC_ALLOCTYPE_ENC,
    DWC_ALLOCTYPE_GS,
    DWC_ALLOCTYPE_ND,
    DWC_ALLOCTYPE_OPTION_CF,
    DWC_ALLOCTYPE_NHTTP,
    DWC_ALLOCTYPE_MAIL,
    DWC_ALLOCTYPE_NUM
} DWCAllocType;

typedef void* (*DWCAllocEx)(DWCAllocType name, u32 size, int align);
typedef void (*DWCFreeEx)(DWCAllocType name, void* ptr, u32 size);

#define DWCi_MEMFUNC_SIGNATURE 'DWCM'

typedef struct DWCi_AllocateHeader {
    u32 signature; // 0x00
    u32 size;      // 0x04
    u8 pad[24];    // 0x08
} DWCi_AllocateHeader;

void DWCi_SetMemFunc(DWCAllocEx alloc, DWCFreeEx free);
void* DWC_Alloc(DWCAllocType name, u32 size);
void* DWC_AllocEx(DWCAllocType name, u32 size, int align);
void DWC_Free(DWCAllocType name, void* ptr, u32 size);
void* DWC_Realloc(DWCAllocType name, void* ptr, u32 oldSize, u32 newSize);
void* DWC_ReallocEx(DWCAllocType name, void* ptr, u32 oldSize, u32 newSize, int align);

void* DWCi_GsMalloc(int size);
void* DWCi_GsRealloc(void* ptr, int size);
void DWCi_GsFree(void* ptr);
void* DWCi_GsMemalign(int align, int size);

#ifdef __cplusplus
}
#endif
#endif
