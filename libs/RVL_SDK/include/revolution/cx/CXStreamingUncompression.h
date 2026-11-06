#ifndef RVL_SDK_CX_CX_STREAMING_UNCOMPRESSION_H
#define RVL_SDK_CX_CX_STREAMING_UNCOMPRESSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

// inferred: layout from CXInitUncompContextLZ (0x806250D0) / CXReadUncompLZ (0x80625110)
typedef struct CXUncompContextLZ {
    u8* destp;          // 0x00
    s32 destCount;      // 0x04
    s32 forceDestCount; // 0x08
    s32 length;         // 0x0C
    u8 lengthFlg;       // 0x10
    u8 flags;           // 0x11
    u8 flagIndex;       // 0x12
    u8 headerSize;      // 0x13
    u8 exFormat;        // 0x14
} CXUncompContextLZ;

typedef struct CXUncompContextLH {
    u8* destp;
    s32 destCount;
    s32 forceDestCount;
    u16 huffTable9[1 << (9 + 1)];
    u16 huffTable12[1 << (5 + 1)];
    u16* nodep;
    s32 tableSize9;
    s32 tableSize12;
    u32 tableIdx;
    u32 stream;
    u32 stream_len;
    u16 length;
    s8 offset_bits;
    u8 headerSize;
} CXUncompContextLH;

void CXInitUncompContextLZ(CXUncompContextLZ* context, void* dest);
s32 CXReadUncompLZ(CXUncompContextLZ* context, const void* data, u32 len);

void CXInitUncompContextLH(CXUncompContextLH* context, void* dest);
void CXReadUncompLH(CXUncompContextLH* context, const void* data, u32 len);

#ifdef __cplusplus
}
#endif
#endif
