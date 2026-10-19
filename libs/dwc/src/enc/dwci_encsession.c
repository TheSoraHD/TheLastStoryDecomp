// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Encrypted gamestats2 session ("enc") used by the ranking library. No reference source or
// symbols exist for this module: file, function and field names are inferred.
#include <dwc/dwc_error.h>
#include <dwc/dwc_ghttp.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <dwc/dwci_encsession.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// inferred: GameSpy B64Encode (gsPlatformUtil.c)
void B64Encode(const char* input, char* output, int inlen, int encodingType);
// inferred: SHA-1 digest of len bytes (20-byte result)
void fn_806AA7E0(u8* digest, const void* data, int len);

#define DWCi_ENC_HASH_LEN 40  // inferred: hex SHA-1 length
#define DWCi_ENC_B64_LEN(n) ((((u32)(n) / 3) + (((u32)(n) % 3) ? 1 : 0)) * 4)

// inferred: session context (0xA0 bytes)
typedef struct DWCiEncSession {
    DWCiEncState state;                // 0x00
    BOOL ghttpInitialized;             // 0x04
    int ghttpRequest;                  // 0x08
    BOOL paramSet;                     // 0x0C
    char gameName[32];                 // 0x10
    u8 secretKey[20];                  // 0x30
    u32 randMul;                       // 0x44 inferred: LCG multiplier (a % 8 == 5)
    u32 randAdd;                       // 0x48 inferred: LCG increment (odd)
    u32 randMod;                       // 0x4C inferred: LCG modulus (power of two)
    u32 checkXor;                      // 0x50
    char* url;                         // 0x54
    char* hash;                        // 0x58 inferred: hash position inside url
    char* urlEnd;                      // 0x5C
    char* recvBuf;                     // 0x60
    int recvLen;                       // 0x64
    const char* response;              // 0x68
    int responseLen;                   // 0x6C
    DWCiEncSessionCallback callback;   // 0x70
    char lastHash[DWCi_ENC_HASH_LEN + 1]; // 0x74
} DWCiEncSession;

static DWCiEncSession sEncSession = {DWCi_ENC_STATE_NOT_INITIALIZED};
static u32 sEncSeed;
static const char* sEncServerUrl;

static BOOL DWCi_EncSessionSetParam(const char* gameName, const char* secretKey, u32 randMul,
                                    u32 randAdd, u32 randMod, u32 checkXor) {
    int i;
    int bits;

    sEncSession.paramSet = FALSE;
    if (strlen(gameName) >= 32) {
        return FALSE;
    }
    if (strlen(secretKey) != 20) {
        return FALSE;
    }
    if (((randMul - 5) & 7) != 0) {
        return FALSE;
    }
    if ((randAdd & 1) == 0) {
        return FALSE;
    }
    bits = 0;
    for (i = 0; i < 32; i++) {
        if (((randMod >> i) & 1) == 1) {
            bits++;
        }
    }
    if (bits != 1) {
        return FALSE;
    }
    strncpy(sEncSession.gameName, gameName, 32);
    memcpy(sEncSession.secretKey, secretKey, 20);
    sEncSession.randMul = randMul;
    sEncSession.randAdd = randAdd;
    sEncSession.randMod = randMod;
    sEncSession.checkXor = checkXor;
    sEncSession.paramSet = TRUE;
    return TRUE;
}

static BOOL DWCi_EncSessionCheckResponse(const char* buf, int len) {
    u8 digest[20];
    const char hex[] = "0123456789abcdef";
    char hash[DWCi_ENC_HASH_LEN + 1];
    u32 size;
    u8* tmp;
    int i;

    if (len <= DWCi_ENC_HASH_LEN) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "response too short\n");
        return FALSE;
    }
    size = len - DWCi_ENC_HASH_LEN;
    tmp = DWC_Alloc(DWC_ALLOCTYPE_ENC, DWCi_ENC_B64_LEN(size) + DWCi_ENC_HASH_LEN + 1);
    if (tmp == NULL) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "no enough memory\n");
        return FALSE;
    }
    memcpy(tmp, sEncSession.secretKey, 20);
    B64Encode(buf, (char*)tmp + 20, size, 2);
    memcpy(tmp + DWCi_ENC_B64_LEN(size) + 20, sEncSession.secretKey, 20);
    fn_806AA7E0(digest, tmp, DWCi_ENC_B64_LEN(size) + 40);
    DWC_Free(DWC_ALLOCTYPE_ENC, tmp, 0);
    for (i = 0; i < 20; i++) {
        hash[i * 2] = hex[digest[i] >> 4];
        hash[i * 2 + 1] = hex[digest[i] & 0xF];
    }
    hash[DWCi_ENC_HASH_LEN] = '\0';
    if (strncmp(&buf[len - DWCi_ENC_HASH_LEN], hash, DWCi_ENC_HASH_LEN) != 0) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "invalid HMAC\n");
        return FALSE;
    }
    sEncSession.response = buf;
    sEncSession.responseLen = size;
    return TRUE;
}

static void DWCi_EncSessionProgressCallback(GHTTPState state, const char* buf, int buflen,
                                            int bytesReceived, int totalSize, void* param) {}

static void DWCi_EncSessionCompletedCallback(const char* buf, int buflen, GHTTPResult result,
                                             void* param) {
    sEncSession.ghttpRequest = -1;
    if (sEncSession.state == DWCi_ENC_STATE_ERROR) {
        return;
    }
    if (result == GHTTPSuccess) {
        switch (sEncSession.state) {
        case DWCi_ENC_STATE_GET_TOKEN:
            if (buflen == 32) {
                const char hex[] = "0123456789abcdef";
                u8 data[20 + 32];
                u8* digest;
                int i;

                digest = (u8*)sEncSession.hash + 20;
                memcpy(data, sEncSession.secretKey, 20);
                memcpy(data + 20, buf, 32);
                fn_806AA7E0(digest, data, sizeof(data));
                for (i = 0; i < 20; i++) {
                    sEncSession.hash[i * 2] = hex[digest[i] >> 4];
                    sEncSession.hash[i * 2 + 1] = hex[digest[i] & 0xF];
                }
                *(sEncSession.hash - strlen("&hash=")) = '&';
                strncpy(sEncSession.lastHash, sEncSession.hash, DWCi_ENC_HASH_LEN + 1);
                sEncSession.state = DWCi_ENC_STATE_REQUEST_DATA;
            } else {
                DWC_Printf(DWC_REPORTFLAG_WARNING, "invalid response length : %d\n", buflen);
                if (!DWCi_IsError()) {
                    DWCi_SetError(DWC_ERROR_NETWORK, -90040);
                }
                sEncSession.state = DWCi_ENC_STATE_ERROR;
            }
            DWCi_GsFree((void*)buf);
            break;

        case DWCi_ENC_STATE_GET_DATA:
            if (strncmp(buf, "error:", strlen("error:")) == 0) {
                DWC_Printf(DWC_REPORTFLAG_WARNING, "invalid response : (%s)\n", buf);
                DWCi_GsFree((void*)buf);
                if (!DWCi_IsError()) {
                    DWCi_SetError(DWC_ERROR_NETWORK, -90040);
                }
                sEncSession.state = DWCi_ENC_STATE_ERROR;
                break;
            }
            if (sEncSession.recvBuf != NULL) {
                DWCi_GsFree(sEncSession.recvBuf);
                sEncSession.recvBuf = NULL;
            }
            if (!DWCi_EncSessionCheckResponse(buf, buflen)) {
                DWCi_GsFree((void*)buf);
                if (!DWCi_IsError()) {
                    DWCi_SetError(DWC_ERROR_NETWORK, -90040);
                }
                sEncSession.state = DWCi_ENC_STATE_ERROR;
                break;
            }
            if (sEncSession.callback != NULL && !sEncSession.callback(buf, buflen)) {
                DWCi_GsFree((void*)buf);
                if (!DWCi_IsError()) {
                    DWCi_SetError(DWC_ERROR_NETWORK, -90040);
                }
                sEncSession.state = DWCi_ENC_STATE_ERROR;
                break;
            }
            sEncSession.recvBuf = (char*)buf;
            sEncSession.recvLen = buflen;
            sEncSession.state = DWCi_ENC_STATE_COMPLETED;
            break;
        }
    } else {
        if (!DWCi_IsError()) {
            DWCi_SetError(DWC_ERROR_NETWORK, -90040);
        }
        sEncSession.state = DWCi_ENC_STATE_ERROR;
    }
}

// NONMATCHING: ~78% - unrolled copy loop of data uses a strength-reduced pointer and the xor
// operands are swapped; tried several index forms and xor expressions
static DWCiEncResult DWCi_EncSessionEncode(char* out, const u8* header, int headerLen,
                                           const u8* data, int dataLen) {
    int bufLen;
    int len;
    u8* buf;
    u32 sum;
    int i;

    len = dataLen + headerLen;
    bufLen = len + 4;
    sum = 0;
    buf = DWC_Alloc(DWC_ALLOCTYPE_ENC, bufLen);
    if (buf == NULL) {
        return DWCi_ENC_RESULT_OUT_OF_MEMORY;
    }
    for (i = 0; i < headerLen; i++) {
        buf[4 + i] = header[i];
    }
    for (i = 0; i < dataLen; i++) {
        buf[4 + headerLen + i] = data[i];
    }
    for (i = 0; i < len; i++) {
        sum += buf[4 + i];
    }
    sEncSeed = sum | (sum << 16);
    for (i = 0; i < len; i++) {
        sEncSeed = (sEncSession.randAdd + sEncSession.randMul * sEncSeed) % sEncSession.randMod;
        buf[4 + i] ^= (sEncSeed & 0xFF0000) >> 16;
    }
    sum ^= sEncSession.checkXor;
    buf[0] = (u8)(sum >> 24);
    buf[1] = (u8)(sum >> 16);
    buf[2] = (u8)(sum >> 8);
    buf[3] = (u8)sum;
    B64Encode((const char*)buf, out, len + 4, 2);
    DWC_Free(DWC_ALLOCTYPE_ENC, buf, 0);
    // inferred: the quotient is taken from bufLen and the remainder from len + 4
    out[((u32)bufLen / 3 + (((u32)(len + 4) % 3) ? 1 : 0)) * 4] = '\0';
    return DWCi_ENC_RESULT_SUCCESS;
}

void DWCi_EncSessionInitialize(DWCEncServer server, const char* initdata) {
    char num[9] = {0};
    char key[21];
    u32 randMul;
    u32 randAdd;
    u32 randMod;
    u32 checkXor;

    sEncSession.ghttpRequest = -1;
    sEncSession.url = NULL;
    sEncSession.hash = NULL;
    sEncSession.urlEnd = NULL;
    sEncSession.recvBuf = NULL;
    sEncSession.recvLen = 0;

    switch (server) {
    case DWC_ENC_SERVER_RELEASE:
        sEncServerUrl = "http://gamestats2.gs.nintendowifi.net/";
        break;
    case DWC_ENC_SERVER_DEBUG:
        sEncServerUrl = "http://sdkdev.gamespy.com/games/";
        break;
    case DWC_ENC_SERVER_TEST:
        sEncServerUrl = "http://ishikawa.servebeer.com/games/";
        break;
    }

    strncpy(key, initdata, 20);
    key[20] = '\0';
    randMul = strtoul(strncpy(num, initdata + 20, 8), NULL, 16);
    randAdd = strtoul(strncpy(num, initdata + 28, 8), NULL, 16);
    randMod = strtoul(strncpy(num, initdata + 36, 8), NULL, 16);
    checkXor = strtoul(strncpy(num, initdata + 44, 8), NULL, 16);
    DWCi_EncSessionSetParam(initdata + 52, key, randMul, randAdd, randMod, checkXor);
    DWC_InitGHTTP(NULL);
    sEncSession.state = DWCi_ENC_STATE_INITIALIZED;
    sEncSession.ghttpInitialized = TRUE;
}

DWCiEncState DWCi_EncSessionProcess(void) {
    switch (sEncSession.state) {
    case DWCi_ENC_STATE_ERROR:
        if (!DWCi_IsError()) {
            DWCi_SetError(DWC_ERROR_NETWORK, -90040);
        }
        sEncSession.state = DWCi_ENC_STATE_ERROR;
        break;
    case DWCi_ENC_STATE_REQUEST_TOKEN:
        sEncSession.ghttpRequest =
            DWC_GetGHTTPDataEx(sEncSession.url, 0, FALSE, DWCi_EncSessionProgressCallback,
                               DWCi_EncSessionCompletedCallback, NULL);
        if (sEncSession.ghttpRequest >= 0) {
            sEncSession.state = DWCi_ENC_STATE_GET_TOKEN;
        } else {
            if (!DWCi_IsError()) {
                DWCi_SetError(DWC_ERROR_NETWORK, -90040);
            }
            sEncSession.state = DWCi_ENC_STATE_ERROR;
        }
        break;
    case DWCi_ENC_STATE_GET_TOKEN:
        if (!DWC_ProcessGHTTP()) {
            if (!DWCi_IsError()) {
                DWCi_SetError(DWC_ERROR_NETWORK, -90040);
            }
            sEncSession.state = DWCi_ENC_STATE_ERROR;
        }
        break;
    case DWCi_ENC_STATE_REQUEST_DATA:
        sEncSession.ghttpRequest =
            DWC_GetGHTTPDataEx(sEncSession.url, 0, FALSE, DWCi_EncSessionProgressCallback,
                               DWCi_EncSessionCompletedCallback, NULL);
        if (sEncSession.ghttpRequest >= 0) {
            sEncSession.state = DWCi_ENC_STATE_GET_DATA;
        } else {
            if (!DWCi_IsError()) {
                DWCi_SetError(DWC_ERROR_NETWORK, -90040);
            }
            sEncSession.state = DWCi_ENC_STATE_ERROR;
        }
        break;
    case DWCi_ENC_STATE_GET_DATA:
        if (!DWC_ProcessGHTTP()) {
            if (!DWCi_IsError()) {
                DWCi_SetError(DWC_ERROR_NETWORK, -90040);
            }
            sEncSession.state = DWCi_ENC_STATE_ERROR;
        }
        break;
    }
    return sEncSession.state;
}

void DWCi_EncSessionCancel(void) {
    if (sEncSession.ghttpRequest >= 0) {
        DWC_CancelGHTTPRequest(sEncSession.ghttpRequest);
    }
    sEncSession.state = DWCi_ENC_STATE_CANCELED;
}

void DWCi_EncSessionShutdown(void) {
    if (!sEncSession.ghttpInitialized) {
        return;
    }
    if (sEncSession.url != NULL) {
        DWC_Free(DWC_ALLOCTYPE_ENC, sEncSession.url, 0);
        sEncSession.url = NULL;
    }
    if (sEncSession.recvBuf != NULL) {
        DWCi_GsFree(sEncSession.recvBuf);
        sEncSession.recvLen = 0;
        sEncSession.recvBuf = NULL;
    }
    DWC_ShutdownGHTTP();
    sEncSession.state = DWCi_ENC_STATE_NOT_INITIALIZED;
    sEncSession.ghttpInitialized = FALSE;
}

const char* DWCi_EncSessionGetResponse(int* size) {
    *size = sEncSession.responseLen;
    return sEncSession.response;
}

#define DWCi_ENC_SWAP32(x) \
    ((((x) & 0xFF) << 24) | (((x) & 0xFF00) << 8) | (((x) & 0xFF0000) >> 8) | \
     (((x) & 0xFF000000) >> 24))

// NONMATCHING: ~57% - order of the additions in the url length and r27/r28 swapped; tried
// several groupings of the length expression
DWCiEncResult DWCi_EncSessionGetAsyncEx(const char* url, int pid, const void* data, int size,
                                        DWCiEncSessionCallback callback, BOOL useLastHash) {
    char pidstr[16];
    u32 header[2];
    int len;

    if (sEncSession.paramSet != TRUE) {
        return DWCi_ENC_RESULT_NOT_INITIALIZED;
    }
    if (sEncSession.url != NULL) {
        DWC_Free(DWC_ALLOCTYPE_ENC, sEncSession.url, 0);
        sEncSession.url = NULL;
    }

    len = strlen(sEncServerUrl) + strlen(sEncSession.gameName) + strlen(url) + strlen("?pid=") +
          snprintf(pidstr, sizeof(pidstr), "%d", pid) + strlen("&hash=") + strlen("&data=") +
          DWCi_ENC_B64_LEN(size + 12) + DWCi_ENC_HASH_LEN + 1;
    sEncSession.url = DWC_Alloc(DWC_ALLOCTYPE_ENC, len);
    if (sEncSession.url == NULL) {
        return DWCi_ENC_RESULT_OUT_OF_MEMORY;
    }
    sprintf(sEncSession.url, "%s%s%s?pid=%d&hash=%s&data=", sEncServerUrl, sEncSession.gameName,
            url, pid, "0000000000000000000000000000000000000000", "");
    sEncSession.urlEnd = sEncSession.url + strlen(sEncSession.url);
    sEncSession.hash = sEncSession.urlEnd - strlen("&data=") - DWCi_ENC_HASH_LEN;

    header[0] = DWCi_ENC_SWAP32((u32)pid);
    header[1] = DWCi_ENC_SWAP32((u32)size);
    if (DWCi_EncSessionEncode(sEncSession.urlEnd, (const u8*)header, sizeof(header), data, size) ==
        DWCi_ENC_RESULT_OUT_OF_MEMORY) {
        DWC_Free(DWC_ALLOCTYPE_ENC, sEncSession.url, 0);
        sEncSession.url = NULL;
        return DWCi_ENC_RESULT_OUT_OF_MEMORY;
    }

    if (useLastHash) {
        memcpy(sEncSession.hash, sEncSession.lastHash, DWCi_ENC_HASH_LEN);
        sEncSession.state = DWCi_ENC_STATE_REQUEST_DATA;
    } else {
        *(sEncSession.hash - strlen("&hash=")) = '\0';
        sEncSession.state = DWCi_ENC_STATE_REQUEST_TOKEN;
    }
    sEncSession.callback = callback;
    return DWCi_ENC_RESULT_SUCCESS;
}

DWCiEncResult DWCi_EncSessionGetAsync(const char* url, int pid, const void* data, int size,
                                      DWCiEncSessionCallback callback) {
    return DWCi_EncSessionGetAsyncEx(url, pid, data, size, callback, FALSE);
}
