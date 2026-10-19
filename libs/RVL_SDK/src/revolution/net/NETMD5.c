// MD5 message digest (RFC 1321), written from the target assembly (RevoEX NET library,
// Jun 9 2009 build). The transform is table driven instead of the unrolled reference code.
#include <types.h>
#include <string.h>

// inferred: context layout (0x58 bytes)
typedef struct NETMD5Context {
    u32 state[4];   // 0x00
    u64 count;      // 0x10 byte count
    u8 buffer[64];  // 0x18
} NETMD5Context;

static void fn_80699148(NETMD5Context* context);

// inferred: names of the tables
static u32 NETMD5T[64] = {
    0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE, 0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
    0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE, 0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,
    0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA, 0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
    0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED, 0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,
    0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C, 0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
    0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05, 0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,
    0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039, 0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
    0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1, 0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391,
};

static u32 NETMD5Index[48] = {
    1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12,
    5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2,
    0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9,
};

#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

// inferred: little-endian word load
static inline u32 NETiLoadLE32(const void* p) {
    return __lwbrx((void*)p, 0);
}

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

#define STEP(f, a, b, c, d, x, t, s) \
    (a) = (b) + ROTATE_LEFT((a) + f((b), (c), (d)) + ((x) + (t)), (s))

void NETMD5Init(NETMD5Context* context) {
    context->state[0] = 0x67452301;
    context->state[1] = 0xEFCDAB89;
    context->state[2] = 0x98BADCFE;
    context->state[3] = 0x10325476;
    context->count = 0;
}

void NETMD5Update(NETMD5Context* context, const void* input, u32 inputLen) {
    u32 index = (u32)context->count & 0x3F;
    u32 partLen;
    int n;
    const u8* data = input;

    context->count += inputLen;
    partLen = 64 - index;

    if (partLen > inputLen) {
        if (inputLen != 0) {
            memcpy(&context->buffer[index], data, inputLen);
        }
    } else {
        memcpy(&context->buffer[index], data, partLen);
        fn_80699148(context);
        inputLen -= partLen;
        data += partLen;

        for (n = inputLen >> 6; n > 0; n--) {
            memcpy(context->buffer, data, 64);
            data += 64;
            fn_80699148(context);
        }

        inputLen &= 0x3F;
        if (inputLen != 0) {
            memcpy(context->buffer, data, inputLen);
        }
    }
}

#define SWAP32(x) \
    ((((x) >> 24) & 0xFF) | (((x) >> 8) & 0xFF00) | (((x) << 8) & 0xFF0000) | (((x) << 24) & 0xFF000000))

void NETMD5GetDigest(NETMD5Context* context, void* digest) {
    u64 bits = context->count << 3;
    u32 lo = (u32)bits;
    u32 hi = (u32)(bits >> 32);
    u32 index;
    u32 padLen;

    NETMD5Update(context, "\x80", 1);

    index = (u32)context->count & 0x3F;
    padLen = 64 - index;
    if (padLen < 8) {
        memset(&context->buffer[index], 0, padLen);
        fn_80699148(context);
        index = 0;
        padLen = 64;
    }
    if (padLen > 8) {
        memset(&context->buffer[index], 0, padLen - 8);
    }

    *(u32*)&context->buffer[56] = SWAP32(lo);
    *(u32*)&context->buffer[60] = SWAP32(hi);
    fn_80699148(context);

    __stwbrx(context->state[0], digest, 0);
    __stwbrx(context->state[1], (u8*)digest + 4, 0);
    __stwbrx(context->state[2], (u8*)digest + 8, 0);
    __stwbrx(context->state[3], (u8*)digest + 12, 0);

    memset(context, 0, sizeof(NETMD5Context));
}

// NONMATCHING: 72.7% - same operations; register allocation differs (the target keeps the
// round 1 word pointer in r4 and a..d in r0/r5/r6/r7). Tried declaration orders, pointer types,
// association of the step sum and loop forms.
static void fn_80699148(NETMD5Context* context) {
    const u8* x = context->buffer;
    u32 a = context->state[0];
    u32 b = context->state[1];
    u32 c = context->state[2];
    u32 d = context->state[3];
    const void* p = x;
    const u32* t = NETMD5T;
    const u32* k;
    int i;

    for (i = 0; i < 4; i++) {
        STEP(F, a, b, c, d, NETiLoadLE32(p), t[0], 7);
        p = (const u8*)p + 4;
        STEP(F, d, a, b, c, NETiLoadLE32(p), t[1], 12);
        p = (const u8*)p + 4;
        STEP(F, c, d, a, b, NETiLoadLE32(p), t[2], 17);
        p = (const u8*)p + 4;
        STEP(F, b, c, d, a, NETiLoadLE32(p), t[3], 22);
        p = (const u8*)p + 4;
        t += 4;
    }

    k = NETMD5Index;
    for (i = 0; i < 4; i++) {
        {
            const void* q = x + k[0] * 4;
            STEP(G, a, b, c, d, NETiLoadLE32(q), t[0], 5);
        }
        {
            const void* q = x + k[1] * 4;
            STEP(G, d, a, b, c, NETiLoadLE32(q), t[1], 9);
        }
        {
            const void* q = x + k[2] * 4;
            STEP(G, c, d, a, b, NETiLoadLE32(q), t[2], 14);
        }
        {
            const void* q = x + k[3] * 4;
            STEP(G, b, c, d, a, NETiLoadLE32(q), t[3], 20);
        }
        t += 4;
        k += 4;
    }

    for (i = 0; i < 4; i++) {
        {
            const void* q = x + k[0] * 4;
            STEP(H, a, b, c, d, NETiLoadLE32(q), t[0], 4);
        }
        {
            const void* q = x + k[1] * 4;
            STEP(H, d, a, b, c, NETiLoadLE32(q), t[1], 11);
        }
        {
            const void* q = x + k[2] * 4;
            STEP(H, c, d, a, b, NETiLoadLE32(q), t[2], 16);
        }
        {
            const void* q = x + k[3] * 4;
            STEP(H, b, c, d, a, NETiLoadLE32(q), t[3], 23);
        }
        t += 4;
        k += 4;
    }

    for (i = 0; i < 4; i++) {
        {
            const void* q = x + k[0] * 4;
            STEP(I, a, b, c, d, NETiLoadLE32(q), t[0], 6);
        }
        {
            const void* q = x + k[1] * 4;
            STEP(I, d, a, b, c, NETiLoadLE32(q), t[1], 10);
        }
        {
            const void* q = x + k[2] * 4;
            STEP(I, c, d, a, b, NETiLoadLE32(q), t[2], 15);
        }
        {
            const void* q = x + k[3] * 4;
            STEP(I, b, c, d, a, NETiLoadLE32(q), t[3], 21);
        }
        t += 4;
        k += 4;
    }

    context->state[0] += a;
    context->state[1] += b;
    context->state[2] += c;
    context->state[3] += d;
}
