// GameSpy SDK gsPlatformUtil.c, Revolution only, adapted to the version linked in this game
// (see libs/gamespy/license.txt).
//
// The thread functions (gsiStartThread ... gsiLeaveCriticalSection) live in their own TU,
// gsPlatformThread.c, as in the SDK: the target never inlines them into the callers below.
#define GSI_THREADS_IN_OWN_TU
#include "common/gsCommon.h"
#include "common/gsPlatformUtil.h"
#include <ctype.h>

#include "common/revolution/gsUtilRevolution.c"

typedef struct GSIResolveHostnameInfo {
    char* hostname;        // 0x00
    unsigned int ip;       // 0x04
    int finishedResolving; // 0x08
    GSIThreadID threadID;  // 0x10
} GSIResolveHostnameInfo;

static void gsiResolveHostnameThread(void* arg) {
    static GSICriticalSection aHostnameCrit;
    static int aInitialized = 0;
    HOSTENT* aHostAddr;
    GSIResolveHostnameHandle handle = (GSIResolveHostnameHandle)arg;

    if (!aInitialized) {
        gsiInitializeCriticalSection(&aHostnameCrit);
        aInitialized = 1;
    }
    gsiEnterCriticalSection(&aHostnameCrit);

    aHostAddr = gethostbyname(handle->hostname);
    if (aHostAddr != 0) {
        char* ip;
        ip = SOInetNtoA(*(SOInAddr*)aHostAddr->addrList[0]);
        handle->ip = inet_addr(ip);
    } else {
        // couldnt reach host
        handle->ip = GSI_ERROR_RESOLVING_HOSTNAME;
    }

    handle->finishedResolving = 1;

    gsiLeaveCriticalSection(&aHostnameCrit);
}

int gsiStartResolvingHostname(const char* hostname, GSIResolveHostnameHandle* handle) {
    GSIResolveHostnameInfo* info;

    info = (GSIResolveHostnameInfo*)gsimalloc(sizeof(GSIResolveHostnameInfo));
    if (!info)
        return -1;

    // make a copy of the hostname so the thread has access to it
    info->hostname = goastrdup(hostname);
    if (!info->hostname) {
        gsifree(info);
        return -1;
    }

    info->finishedResolving = 0;

    if (gsiStartThread(gsiResolveHostnameThread, (0x1000), info, &info->threadID) == -1) {
        gsifree(info->hostname);
        info->hostname = NULL;
        gsifree(info);
        info = NULL;
        return -1;
    }

    *handle = info;

    return 0;
}

void gsiCancelResolvingHostname(GSIResolveHostnameHandle handle) {
    gsiCancelThread(handle->threadID);

    if (handle->hostname) {
        gsifree(handle->hostname);
        handle->hostname = NULL;
    }
    gsifree(handle);
    handle = NULL;
}

unsigned int gsiGetResolvedIP(GSIResolveHostnameHandle handle) {
    unsigned int ip;

    if (!handle->finishedResolving)
        return GSI_STILL_RESOLVING_HOSTNAME;

    ip = handle->ip;

    gsiCleanupThread(handle->threadID);
    gsifree(handle->hostname);
    gsifree(handle);
    handle = NULL;

    return ip;
}

char* goastrdup(const char* src) {
    char* res;
    size_t len;
    if (src == NULL)
        return NULL;
    len = strlen(src) + 1;
    res = (char*)gsimalloc(len);
    if (res != NULL)
        memcpy(res, src, len);
    return res;
}

char* _strlwr(char* string) {
    char* hold = string;
    while (*string) {
        *string = (char)tolower(*string);
        string++;
    }

    return hold;
}

void SocketStartUp(void) {
}

void SocketShutDown(void) {
}

// returns current time in milliseconds
gsi_time current_time(void) {
    return (gsi_time)OS_TICKS_TO_MSEC(OSGetTime());
}

void msleep(gsi_time msec) {
    OSSleepTicks(OS_MSEC_TO_TICKS((s64)msec));
}

const char _dnames[] = {"SunMonTueWedThuFriSat"};
const char _mnames[] = {"JanFebMarAprMayJunJulAugSepOctNovDec"};

#define RANa 16807               // multiplier
#define LONGRAND_MAX 2147483647L // 2**31 - 1

static long randomnum = 1;

static long nextlongrand(long seed) {
    unsigned long lo, hi;
    lo = RANa * (unsigned long)(seed & 0xFFFF);
    hi = RANa * ((unsigned long)seed >> 16);
    lo += (hi & 0x7FFF) << 16;

    if (lo > LONGRAND_MAX) {
        lo &= LONGRAND_MAX;
        ++lo;
    }
    lo += hi >> 15;

    if (lo > LONGRAND_MAX) {
        lo &= LONGRAND_MAX;
        ++lo;
    }

    return (long)lo;
}

static long longrand(void) {
    randomnum = nextlongrand(randomnum);
    return randomnum;
}

void Util_RandSeed(unsigned long seed) {
    // nonzero seed
    randomnum = seed ? (long)(seed & LONGRAND_MAX) : 1;
}

int Util_RandInt(int low, int high) {
    unsigned int range = (unsigned int)high - low;
    int num;

    if (range == 0)
        return (low); // Prevent divide by zero

    num = (int)(longrand() % range);

    return (num + low);
}

static void QuartToTrip(char* quart, char* trip, int inlen) {
    if (inlen >= 2)
        trip[0] = (char)(quart[0] << 2 | quart[1] >> 4);
    if (inlen >= 3)
        trip[1] = (char)((quart[1] & 0x0F) << 4 | quart[2] >> 2);
    if (inlen >= 4)
        trip[2] = (char)((quart[2] & 0x3) << 6 | quart[3]);
}

static void TripToQuart(const char* trip, char* quart, int inlen) {
    unsigned char triptemp[3];
    int i;
    for (i = 0; i < inlen; i++) {
        triptemp[i] = (unsigned char)trip[i];
    }
    while (i < 3) // fill the rest with 0
    {
        triptemp[i] = 0;
        i++;
    }
    quart[0] = (char)(triptemp[0] >> 2);
    quart[1] = (char)(((triptemp[0] & 3) << 4) | (triptemp[1] >> 4));
    quart[2] = (char)((triptemp[1] & 0x0F) << 2 | (triptemp[2] >> 6));
    quart[3] = (char)(triptemp[2] & 0x3F);
}

const char defaultEncoding[] = {'+', '/', '='};
const char alternateEncoding[] = {'[', ']', '_'};
const char urlSafeEncodeing[] = {'-', '_', '='};

void B64Decode(const char* input, char* output, int inlen, int* outlen, int encodingType) {
    const char* encoding = NULL;
    const char* holdin = input;
    int readpos = 0;
    int writepos = 0;
    char block[4];

    switch (encodingType) {
    case 1:
        encoding = alternateEncoding;
        break;
    case 2:
        encoding = urlSafeEncodeing;
        break;
    default:
        encoding = defaultEncoding;
    }

    if (inlen <= 0) {
        if (outlen)
            *outlen = 0;
        output[0] = '\0';
        return;
    }

    // Break at end of string or padding character
    while (readpos < inlen && input[readpos] != encoding[2]) {
        if (input[readpos] >= '0' && input[readpos] <= '9')
            block[readpos % 4] = (char)(input[readpos] - 48 + 52);
        else if (input[readpos] >= 'a' && input[readpos] <= 'z')
            block[readpos % 4] = (char)(input[readpos] - 71);
        else if (input[readpos] >= 'A' && input[readpos] <= 'Z')
            block[readpos % 4] = (char)(input[readpos] - 65);
        else if (input[readpos] == encoding[0])
            block[readpos % 4] = 62;
        else if (input[readpos] == encoding[1])
            block[readpos % 4] = 63;

        // padding or '\0' characters also mark end of input
        else if (input[readpos] == encoding[2])
            break;
        else if (input[readpos] == '\0')
            break;
        else {
            if (outlen)
                *outlen = 0;
            output[0] = '\0';
            return; // invaid data
        }

        // every 4 bytes, convert QuartToTrip into destination
        if (readpos % 4 == 3) {
            QuartToTrip(block, &output[writepos], 4);
            writepos += 3;
        }
        readpos++;
    }

    // Convert any leftover characters in block
    if ((readpos != 0) && (readpos % 4 != 0)) {
        // fill block with pad (required for QuartToTrip)
        memset(&block[readpos % 4], encoding[2], (unsigned int)4 - (readpos % 4));
        QuartToTrip(block, &output[writepos], readpos % 4);

        // output bytes depend on the number of non-pad input bytes
        if (readpos % 4 == 3)
            writepos += 2;
        else
            writepos += 1;
    }

    if (outlen)
        *outlen = writepos;

    GSI_UNUSED(holdin);
}

#define min(a, b) ((a) < (b) ? (a) : (b))

void B64Encode(const char* input, char* output, int inlen, int encodingType) {
    const char* encoding;
    char* holdout = output;
    char* lastchar;
    int todo = inlen;

    switch (encodingType) {
    case 1:
        encoding = alternateEncoding;
        break;
    case 2:
        encoding = urlSafeEncodeing;
        break;
    default:
        encoding = defaultEncoding;
    }

    // assume interval of 3
    while (todo > 0) {
        TripToQuart(input, output, min(todo, 3));
        output += 4;
        input += 3;
        todo -= 3;
    }
    lastchar = output;
    if (inlen % 3 == 1)
        lastchar -= 2;
    else if (inlen % 3 == 2)
        lastchar -= 1;
    *output = 0; // null terminate!
    while (output > holdout) {
        output--;
        if (output >= lastchar) // pad the end
            *output = encoding[2];
        else if (*output <= 25)
            *output = (char)(*output + 65);
        else if (*output <= 51)
            *output = (char)(*output + 71);
        else if (*output <= 61)
            *output = (char)(*output + 48 - 52);
        else if (*output == 62)
            *output = encoding[0];
        else if (*output == 63)
            *output = encoding[1];
    }
}

// inferred: name and placement; bounded copy that always terminates and returns the length copied
gsi_u32 gsiSafeStrcpyA(char* dst, const char* src, gsi_u32 size) {
    gsi_u32 len = strlen(src);
    if (len > size - 1)
        len = size - 1;
    memcpy(dst, src, len);
    dst[len] = '\0';
    return len;
}
