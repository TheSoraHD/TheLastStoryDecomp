#include <revolution/vf.h>

void* VFipf_memset(void* dst, s32 c, u32 length) {
    s8* d;
    u32* ld;
    u32 lc;
    for (d = dst; ((((u32)d) & 3) != 0) && length; ++d) {
        --length;
        *d = c;
    }

    ld = (u32*)d;
    while (length > 3) {
        *(ld++) = (c | (c << 8)) | ((c << 24) | (c << 16));
        length -= 4;
    }

    for (lc = 0; (length--) != 0; ++lc) {
        *((s8*)ld + lc) = c;
    }

    return dst;
}
