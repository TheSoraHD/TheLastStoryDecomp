#ifndef DWC_BASE64_H
#define DWC_BASE64_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// inferred: names from the NitroDWC/RVL DWC API (URL-safe Base64 with '.', '-' and '*' padding)
int DWC_Base64Encode(const char* src, u32 srcLen, char* dst, u32 dstLen);
int DWC_Base64Decode(const char* src, u32 srcLen, char* dst, u32 dstLen);

#ifdef __cplusplus
}
#endif
#endif
