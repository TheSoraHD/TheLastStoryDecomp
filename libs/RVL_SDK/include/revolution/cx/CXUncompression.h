#ifndef RVL_SDK_CX_CX_UNCOMPRESSION_H
#define RVL_SDK_CX_CX_UNCOMPRESSION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <types.h>

// inferred: compression types tested by CXUncompressAny (0x806254D0)
typedef enum {
    CX_COMPRESSION_LZ = 0x10,
    CX_COMPRESSION_HUFFMAN = 0x20,
    CX_COMPRESSION_RL = 0x30,
    CX_COMPRESSION_DIFF = 0x80,
    CX_COMPRESSION_TYPE_MASK = 0xF0
} CXCompressionType;

void CXUncompressAny(const void* srcp, void* destp);
void CXUncompressLZ(const void* srcp, void* destp);
void CXUncompressHuffman(const void* srcp, void* destp);
void CXUnfilterDiff(const void* srcp, void* destp);

#ifdef __cplusplus
}
#endif
#endif
