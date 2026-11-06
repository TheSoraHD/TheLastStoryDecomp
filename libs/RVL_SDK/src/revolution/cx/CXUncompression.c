#include <revolution/cx/CXUncompression.h>

// TLS keeps CXUncompressAny and the decoders it reaches; CXUncompressRL only survives
// inlined into CXUncompressAny, the rest of the file is deadstripped.

static inline u32 CXiConvertEndian_(u32 x) {
    return ((x & 0xFF000000) >> 24) | ((x & 0xFF0000) >> 8) | ((x & 0xFF00) << 8) | ((x & 0xFF) << 24);
}

static inline u16 CXiConvertEndian16_(u16 x) {
    return ((x & 0xFF00) >> 8) | ((x & 0xFF) << 8);
}

static inline CXCompressionType CXGetCompressionType(const void* srcp) {
    return (CXCompressionType)(*(const u8*)srcp & CX_COMPRESSION_TYPE_MASK);
}

static inline void CXUncompressRL(const void* srcp, void* destp);

// NONMATCHING: 84% - the original places the inlined CXUncompressRL after the tail calls
// (beq to a "b" trampoline) and keeps a loop-alignment nop; tried switch/if forms,
// break/return cases, RL defined before/after, inline/static inline.
void CXUncompressAny(const void* srcp, void* destp) {
    switch (CXGetCompressionType(srcp)) {
    case CX_COMPRESSION_RL:
        CXUncompressRL(srcp, destp);
        break;
    case CX_COMPRESSION_LZ:
        CXUncompressLZ(srcp, destp);
        break;
    case CX_COMPRESSION_HUFFMAN:
        CXUncompressHuffman(srcp, destp);
        break;
    case CX_COMPRESSION_DIFF:
        CXUnfilterDiff(srcp, destp);
        break;
    }
}

static inline void CXUncompressRL(const void* srcp, void* destp) {
    const u8* pSrc = (const u8*)srcp;
    u8* pDst = (u8*)destp;
    u32 destCount = CXiConvertEndian_(*(const u32*)pSrc) >> 8;

    pSrc += 4;
    if (destCount == 0) {
        destCount = CXiConvertEndian_(*(const u32*)pSrc);
        pSrc += 4;
    }

    while (destCount > 0) {
        u8 flags = *pSrc++;
        u32 length = flags & 0x7F;

        if (!(flags & 0x80)) {
            length++;
            if (length > destCount) {
                length = destCount;
            }
            destCount -= length;
            do {
                *pDst++ = *pSrc++;
            } while (--length > 0);
        } else {
            u8 srcTmp;

            length += 3;
            if (length > destCount) {
                length = destCount;
            }
            destCount -= length;
            srcTmp = *pSrc++;
            do {
                *pDst++ = srcTmp;
            } while (--length > 0);
        }
    }
}

void CXUncompressLZ(const void* srcp, void* destp) {
    const u8* pSrc = (const u8*)srcp;
    u8* pDst = (u8*)destp;
    BOOL exFormat;
    u32 destCount = CXiConvertEndian_(*(const u32*)pSrc) >> 8;

    exFormat = (*pSrc & 0x0F) ? TRUE : FALSE;

    pSrc += 4;
    if (destCount == 0) {
        destCount = CXiConvertEndian_(*(const u32*)pSrc);
        pSrc += 4;
    }

    while (destCount > 0) {
        u32 i;
        u32 flags = *pSrc++;

        for (i = 0; i < 8; i++) {
            if (!(flags & 0x80)) {
                *pDst++ = *pSrc++;
                destCount--;
            } else {
                s32 length = *pSrc >> 4;
                s32 offset;

                if (!exFormat) {
                    length += 3;
                } else if (length == 1) {
                    length = (*pSrc++ & 0x0F) << 12;
                    length |= *pSrc++ << 4;
                    length |= *pSrc >> 4;
                    length += 0xFF + 0xF + 3;
                } else if (length == 0) {
                    length = (*pSrc++ & 0x0F) << 4;
                    length |= *pSrc >> 4;
                    length += 0xF + 2;
                } else {
                    length += 1;
                }

                offset = (*pSrc++ & 0x0F) << 8;
                offset = (offset | *pSrc++) + 1;

                if (length > destCount) {
                    length = destCount;
                }
                destCount -= length;
                do {
                    *pDst = pDst[-offset];
                    pDst++;
                } while (--length > 0);
            }

            if (destCount == 0) {
                break;
            }
            flags <<= 1;
        }
    }
}

void CXUncompressHuffman(const void* srcp, void* destp) {
    const u32* pSrc = (const u32*)srcp;
    u32* pDst = (u32*)destp;
    s32 destCount = CXiConvertEndian_(*pSrc) >> 8;
    u8* treep = (destCount != 0) ? ((u8*)pSrc + 4) : ((u8*)pSrc + 8);
    u8* treeStartp = treep + 1;
    u32 dataBit = *(const u8*)pSrc & 0x0F;
    u32 destTmp = 0;
    u32 destTmpCount = 0;
    u32 destTmpDataNum = 4 + (dataBit & 0x7);

    if (destCount == 0) {
        destCount = CXiConvertEndian_(*(pSrc + 1));
    }

    pSrc = (const u32*)(treep + ((*treep + 1) << 1));
    treep = treeStartp;

    while (destCount > 0) {
        s32 srcCount = 32;
        u32 srcTmp = CXiConvertEndian_(*pSrc++);

        while (--srcCount >= 0) {
            u32 treeShift = srcTmp >> 31;
            u32 treeCheck = *treep;

            treeCheck <<= treeShift;
            treep = (u8*)((((u32)treep >> 1) << 1) + (((*treep & 0x3F) + 1) << 1) + treeShift);
            if (treeCheck & 0x80) {
                destTmp >>= dataBit;
                destTmp |= *treep << (32 - dataBit);
                treep = treeStartp;
                destTmpCount++;
                if (destTmpCount == destTmpDataNum) {
                    *pDst++ = CXiConvertEndian_(destTmp);
                    destCount -= 4;
                    destTmpCount = 0;
                }
            }
            if (destCount <= 0) {
                break;
            }
            srcTmp <<= 1;
        }
    }
}

void CXUnfilterDiff(const void* srcp, void* destp) {
    const u8* pSrc = (const u8*)srcp;
    u8* pDst = (u8*)destp;
    u32 bitSize = *pSrc & 0x0F;
    s32 destCount = CXiConvertEndian_(*(const u32*)pSrc) >> 8;
    u32 sum = 0;

    pSrc += 4;

    if (bitSize != 1) {
        do {
            u8 tmp = *pSrc++;
            destCount--;
            sum += tmp;
            *pDst++ = (u8)sum;
        } while (destCount > 0);
    } else {
        do {
            u16 tmp = CXiConvertEndian16_(*(const u16*)pSrc);
            pSrc += 2;
            destCount -= 2;
            sum += tmp;
            *(u16*)pDst = CXiConvertEndian16_((u16)sum);
            pDst += 2;
        } while (destCount > 0);
    }
}
