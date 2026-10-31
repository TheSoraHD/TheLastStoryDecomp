// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Function names from Pokemon Battle Revolution (doldecomp/pbr, dwcsec_auth/dwc_auth_interface.s)
// where that older build has the same function.
#include <dwc/dwc_auth_interface.h>
#include <dwc/dwc_base64.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/NAND.h>
#include <revolution/NHTTP.h>
#include <revolution/OS.h>
#include <revolution/SC.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// inferred: NCD interface configuration; only the fields read here
typedef struct NCDIfConfig_unk {
    u8 selectedMedia;  // 0x000 inferred: 1 = wireless
    u8 unk001[3];      // 0x001
    u8 confMethod;     // 0x004 inferred: wireless configuration method
    u8 unk005[0x159];  // 0x005
} NCDIfConfig_unk;

s32 NCDGetCurrentIfConfig(NCDIfConfig_unk* config);

// inferred: other DWC / system helpers, no source yet
const char* OSGetAppGamename(void);  // inferred: game code string (HTTP_X_GAMECD, gamecd)
void fn_80698A28(u8* macAddress);
const char* SCGetProductCode_unk(void);  // inferred: console serial number prefix
BOOL SCGetProductSN_unk(u32* serial);  // inferred: console serial number
s64 fn_806AA690(void);          // inferred: console friend code
char* strtok(char* str, const char* delim);  // strtok

// inferred: results of the nas request (DWC "auth" results)
typedef struct DWCAuthResult {
    volatile int returnCode;        // 0x000 inferred: 1 = success, negative = error code
    char token[0x12D];              // 0x004
    char challenge[0x4A];           // 0x131
    char locator[0x35];             // 0x17B
    u64 userId;                     // 0x1B0
    int ingamesnCheckResult;        // 0x1B8
    s64 timeDiff;                   // 0x1C0 inferred: server time - console time
    NHTTPResponseHandle response;   // 0x1C8
} DWCAuthResult;

// inferred: results of the svcloc request
typedef struct DWCAuthSvcResult {
    int statusData;          // 0x000
    char svcHost[0x41];      // 0x004
    char serviceToken[0x12F]; // 0x045
} DWCAuthSvcResult;

// inferred: contents of /shared2/DWC_AUTHDATA (read and written with NAND, 32-byte aligned)
typedef struct DWCAuthData {
    u64 userId;    // 0x00
    u8 pad[0x18];  // 0x08
} DWCAuthData;

// inferred: results of the prof (profanity check) request
struct DWCAuthPrWords {
    char words[0x34];  // 0x00
    int error;         // 0x34
};

// inferred: work area allocated by DWCi_Auth_StartAuthentication
typedef struct DWCAuthWork {
    u8 unk0000[0x4000];
    NCDIfConfig_unk ifConfig;        // 0x4000
    wchar_t ingamesn[0x1A];          // 0x415E
    char gsbrcd[0xC];                // 0x4192
    char svc[6];                     // 0x419E
    const char* words;               // 0x41A4
    u32 wordsSize;                   // 0x41A8
    int hasUserId;                   // 0x41AC
    char wregion[2];                 // 0x41B0
    char recvBuf[0x1000];            // 0x41B2
    char postBuf[0x802];             // 0x51B2
    int retryCount;                  // 0x59B4
    int nandDone;                    // 0x59B8
    s32 nandResult;                  // 0x59BC
    int aborted;                     // 0x59C0
    int requestId;                   // 0x59C4
    int mode;                        // 0x59C8 inferred: 1 = nas, 3 = prof
    NANDCommandBlock cmdBlock;       // 0x59CC
    NANDFileInfo fileInfo;           // 0x5A88
    DWCAuthAlloc alloc;              // 0x5B14
    DWCFreeEx free;                  // 0x5B18
    s64 startTime;                   // 0x5B20
    u64 userId;                      // 0x5B28
} DWCAuthWork;

// inferred: states of DWCi_Auth_ProcessAuthentication
enum {
    DWC_AUTH_STATE_IDLE,
    DWC_AUTH_STATE_OPEN_READ,
    DWC_AUTH_STATE_WAIT_OPEN_READ,
    DWC_AUTH_STATE_READ,
    DWC_AUTH_STATE_WAIT_READ,
    DWC_AUTH_STATE_CLOSE_INVALID,
    DWC_AUTH_STATE_WAIT_CLOSE_INVALID,
    DWC_AUTH_STATE_DELETE,
    DWC_AUTH_STATE_WAIT_DELETE,
    DWC_AUTH_STATE_ACCTCREATE,
    DWC_AUTH_STATE_WAIT_ACCTCREATE,
    DWC_AUTH_STATE_CREATE,
    DWC_AUTH_STATE_WAIT_CREATE,
    DWC_AUTH_STATE_OPEN_WRITE,
    DWC_AUTH_STATE_WAIT_OPEN_WRITE,
    DWC_AUTH_STATE_WRITE,
    DWC_AUTH_STATE_WAIT_WRITE,
    DWC_AUTH_STATE_CLOSE,
    DWC_AUTH_STATE_WAIT_CLOSE,
    DWC_AUTH_STATE_LOGIN,
    DWC_AUTH_STATE_WAIT_LOGIN,
    DWC_AUTH_STATE_CANCEL,
    DWC_AUTH_STATE_WAIT_CANCEL,
    DWC_AUTH_STATE_CLEANUP,
    DWC_AUTH_STATE_WAIT_CLEANUP,
    DWC_AUTH_STATE_END,
    DWC_AUTH_STATE_FINISHED,
    DWC_AUTH_STATE_NONE  // inferred: "no transition" argument of DWCi_Auth_HandleNandResult
};

#define DWC_AUTH_ALLOCTYPE_NHTTP ((DWCAllocType)13)  // inferred

static const char* sAuthDataPath = "/shared2/DWC_AUTHDATA";
static const char* sNasUrl[] = {
    "https://naswii.test.nintendowifi.net/ac",
    "https://naswii.nintendowifi.net/ac",
    "https://naswii.dev.nintendowifi.net/ac",
};
static const char* sProfUrl[] = {
    "https://naswii.test.nintendowifi.net/pr",
    "https://naswii.nintendowifi.net/pr",
    "https://naswii.dev.nintendowifi.net/pr",
};

static DWCAuthResult sResult;
static DWCAuthSvcResult sSvcResult;
static DWCAuthData sAuthData ALIGN(32);
static DWCAuthWork* sWork;
static volatile int sState;
static int sAuthServer;
static int sUseWorkUserId;  // inferred: login with the user id given to StartAuthentication
static DWCAuthPrWords sPrWords;

#define DWC_AUTH_URL() \
    (sWork->mode != 3 ? sNasUrl[sAuthServer] : sProfUrl[sAuthServer])

static void* nhttp_alloc(u32 size, int align);
static void nhttp_free(void* ptr);
static void nand_callback(s32 result, NANDCommandBlock* block);
static void DWCi_Auth_HandleResponse(int error, NHTTPResponseHandle response, void* param);
static void DWCi_Auth_EndProcess(void);

BOOL DWCi_Auth_InitInterface(int authServer) {
    memset(&sResult, 0, sizeof(sResult));
    memset(&sSvcResult, 0, sizeof(sSvcResult));
    memset(&sAuthData, 0, sizeof(sAuthData));
    sWork = NULL;
    sState = DWC_AUTH_STATE_IDLE;
    sAuthServer = authServer;
    sResult.ingamesnCheckResult = 0;
    sUseWorkUserId = 0;
    return TRUE;
}

int fn_806D0970(void) {
    return sAuthServer;
}

BOOL DWCi_Auth_StartAuthentication(const wchar_t* ingamesn, const char* gsbrcd, u64 userId,
                                   DWCAuthAlloc alloc, DWCFreeEx free) {
    s32 ret;

    if (sState != DWC_AUTH_STATE_IDLE && sState != DWC_AUTH_STATE_FINISHED) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " auth is processing\n");
        return FALSE;
    }

    sWork = (DWCAuthWork*)alloc(DWC_ALLOCTYPE_AUTH, sizeof(DWCAuthWork));
    if (sWork == NULL) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " memory shortage\n");
        return FALSE;
    }
    memset(sWork, 0, sizeof(DWCAuthWork));
    sWork->alloc = alloc;
    sWork->free = free;

    ret = NCDGetCurrentIfConfig(&sWork->ifConfig);
    if (ret != 0) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " NCDGetCurrentIfConfig failed.[%d]\n", ret);
        goto error;
    }
    if (NHTTPStartup(nhttp_alloc, nhttp_free, 17) < 0) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " failed to start NHTTP\n");
        goto error;
    }

    wcsncpy(sWork->ingamesn, ingamesn, 0x1A);
    strncpy(sWork->gsbrcd, gsbrcd, 0xC);
    sWork->mode = 1;
    memset(&sResult, 0, sizeof(sResult));
    sResult.ingamesnCheckResult = 0;
    sWork->userId = userId;
    sState = DWC_AUTH_STATE_OPEN_READ;
    return TRUE;

error:
    sWork->free(DWC_ALLOCTYPE_AUTH, sWork, 0);
    return FALSE;
}

BOOL fn_806D0B40(const char* words, u32 wordsSize, char region, DWCAuthAlloc alloc,
                 DWCFreeEx free) {
    memset(&sPrWords, 0, sizeof(sPrWords));
    sPrWords.error = 0;
    DWCi_Auth_StartAuthentication(L"", "", 0, alloc, free);
    sWork->words = words;
    sWork->wordsSize = wordsSize;
    sWork->wregion[0] = region;
    sWork->wregion[1] = '\0';
    sWork->mode = 3;
    if (sAuthData.userId == 0) {
        sWork->hasUserId = FALSE;
    } else {
        sWork->hasUserId = TRUE;
    }
    return TRUE;
}

void DWCi_Auth_AbortAuthentication(void) {
    BOOL loop;

    while (sState == DWC_AUTH_STATE_WAIT_CLEANUP) {
        OSSleepTicks(OS_MSEC_TO_TICKS(1));
        DWC_Printf(DWC_REPORTFLAG_AUTH, "wait()\n");
    }

    if (DWCi_Auth_IsFinished()) {
        sResult.returnCode = -20100;
        return;
    }

    if (sWork != NULL) {
        sWork->aborted = TRUE;

        loop = TRUE;
        while (loop) {
            switch (sState) {
            case DWC_AUTH_STATE_WAIT_OPEN_READ:
            case DWC_AUTH_STATE_READ:
            case DWC_AUTH_STATE_WAIT_READ:
            case DWC_AUTH_STATE_CLOSE_INVALID:
            case DWC_AUTH_STATE_WAIT_CLOSE_INVALID:
            case DWC_AUTH_STATE_DELETE:
            case DWC_AUTH_STATE_WAIT_DELETE:
            case DWC_AUTH_STATE_CREATE:
            case DWC_AUTH_STATE_OPEN_WRITE:
            case DWC_AUTH_STATE_WRITE:
            case DWC_AUTH_STATE_WAIT_WRITE:
            case DWC_AUTH_STATE_CLOSE:
            case DWC_AUTH_STATE_WAIT_CLOSE:
                DWCi_Auth_ProcessAuthentication();
                OSSleepTicks(OS_MSEC_TO_TICKS(1));
                break;
            default:
                loop = FALSE;
                break;
            }
        }

        if (sState == DWC_AUTH_STATE_WAIT_LOGIN) {
            NHTTPCancelRequestAsync(sWork->requestId);
            while (TRUE) {
                if (sResult.returnCode != 0) {
                    break;
                }
                OSSleepTicks(OS_MSEC_TO_TICKS(1));
            }
        }

        if (sResult.response != NULL) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, "NHTTPDestroyResponse()\n");
            NHTTPDestroyResponse(sResult.response);
            sResult.response = NULL;
        }

        if (sState != DWC_AUTH_STATE_IDLE && sState != DWC_AUTH_STATE_WAIT_CLEANUP &&
            sState != DWC_AUTH_STATE_END && sState != DWC_AUTH_STATE_FINISHED)
        {
            sState = DWC_AUTH_STATE_WAIT_CLEANUP;
            NHTTPCleanupAsync(DWCi_Auth_EndProcess);
            while (sState == DWC_AUTH_STATE_WAIT_CLEANUP) {
                OSSleepTicks(OS_MSEC_TO_TICKS(1));
            }
        }

        sState = DWC_AUTH_STATE_FINISHED;
        sResult.returnCode = -20100;
    }
}

void DWCi_Auth_ProcessAuthentication(void) {
    switch (sState) {
    case DWC_AUTH_STATE_OPEN_READ:
        if (sWork->mode == 3) {
            sState = DWC_AUTH_STATE_LOGIN;
            break;
        }
        if (sAuthData.userId != 0) {
            sState = DWC_AUTH_STATE_LOGIN;
            break;
        }
        NANDPrivateOpenAsync(sAuthDataPath, &sWork->fileInfo, NAND_ACCESS_READ, nand_callback,
                             &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_OPEN_READ;
        break;
    case DWC_AUTH_STATE_WAIT_OPEN_READ:
        if (sWork->nandDone) {
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_READ, DWC_AUTH_STATE_OPEN_READ,
                                       DWC_AUTH_STATE_ACCTCREATE, DWC_AUTH_STATE_DELETE);
        }
        break;
    case DWC_AUTH_STATE_READ:
        NANDReadAsync(&sWork->fileInfo, &sAuthData, sizeof(sAuthData), nand_callback,
                      &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_READ;
        break;
    case DWC_AUTH_STATE_WAIT_READ:
        if (sWork->nandDone) {
            if (sWork->nandResult == sizeof(sAuthData)) {
                DWC_Printf(DWC_REPORTFLAG_AUTH, " read userid = %llu\n", sAuthData.userId);
                sWork->nandResult = NAND_RESULT_OK;
            } else if (sWork->nandResult >= 0) {
                DWC_Printf(DWC_REPORTFLAG_AUTH, " illegal size userid read = %d\n",
                           sWork->nandResult);
                sWork->nandResult = NAND_RESULT_ACCESS;
            }
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_CLOSE, DWC_AUTH_STATE_READ,
                                       DWC_AUTH_STATE_NONE, DWC_AUTH_STATE_CLOSE_INVALID);
        }
        break;
    case DWC_AUTH_STATE_CLOSE_INVALID:
        NANDCloseAsync(&sWork->fileInfo, nand_callback, &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_CLOSE_INVALID;
        break;
    case DWC_AUTH_STATE_WAIT_CLOSE_INVALID:
        if (sWork->nandDone) {
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_DELETE, DWC_AUTH_STATE_CLOSE_INVALID,
                                       DWC_AUTH_STATE_NONE, DWC_AUTH_STATE_NONE);
        }
        break;
    case DWC_AUTH_STATE_DELETE:
        NANDPrivateDeleteAsync(sAuthDataPath, nand_callback, &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_DELETE;
        break;
    case DWC_AUTH_STATE_WAIT_DELETE:
        if (sWork->nandDone) {
            if (sWork->nandResult == NAND_RESULT_OK) {
                DWC_Printf(DWC_REPORTFLAG_AUTH, " delete illegal userid.\n");
            }
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_ACCTCREATE, DWC_AUTH_STATE_DELETE,
                                       DWC_AUTH_STATE_ACCTCREATE, DWC_AUTH_STATE_NONE);
        }
        break;
    case DWC_AUTH_STATE_ACCTCREATE:
        DWCi_Auth_SendRequest(0, NULL, NULL, 0);
        sWork->startTime = OSGetTime();
        sState = DWC_AUTH_STATE_WAIT_ACCTCREATE;
        break;
    case DWC_AUTH_STATE_WAIT_ACCTCREATE:
        if (sResult.returnCode == 1) {
            sState = DWC_AUTH_STATE_CREATE;
        } else if (sResult.returnCode >= -20999 && sResult.returnCode <= -20102) {
            sState = DWC_AUTH_STATE_CLEANUP;
        } else if (sResult.returnCode < 0) {
            sWork->retryCount++;
            if (sWork->retryCount < 3) {
                sState = DWC_AUTH_STATE_ACCTCREATE;
            } else {
                sState = DWC_AUTH_STATE_CLEANUP;
            }
        } else if (OS_TICKS_TO_MSEC(OSGetTime() - sWork->startTime) > 30000) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, " acctcreate timeout.\n");
            sState = DWC_AUTH_STATE_CANCEL;
        }
        break;
    case DWC_AUTH_STATE_CREATE:
        NANDPrivateCreateAsync(sAuthDataPath, NAND_PERM_RWALL, 0, nand_callback,
                               &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_CREATE;
        break;
    case DWC_AUTH_STATE_WAIT_CREATE:
        if (sWork->nandDone) {
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_OPEN_WRITE, DWC_AUTH_STATE_CREATE,
                                       DWC_AUTH_STATE_NONE, DWC_AUTH_STATE_NONE);
        }
        break;
    case DWC_AUTH_STATE_OPEN_WRITE:
        NANDPrivateOpenAsync(sAuthDataPath, &sWork->fileInfo, NAND_ACCESS_WRITE, nand_callback,
                             &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_OPEN_WRITE;
        break;
    case DWC_AUTH_STATE_WAIT_OPEN_WRITE:
        if (sWork->nandDone) {
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_WRITE, DWC_AUTH_STATE_OPEN_WRITE,
                                       DWC_AUTH_STATE_NONE, DWC_AUTH_STATE_NONE);
        }
        break;
    case DWC_AUTH_STATE_WRITE:
        NANDWriteAsync(&sWork->fileInfo, &sAuthData, sizeof(sAuthData), nand_callback,
                       &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_WRITE;
        break;
    case DWC_AUTH_STATE_WAIT_WRITE:
        if (sWork->nandDone) {
            if (sWork->nandResult == sizeof(sAuthData)) {
                sWork->nandResult = NAND_RESULT_OK;
            } else {
                DWC_Printf(DWC_REPORTFLAG_AUTH, " illegal size userid write = %d\n",
                           sWork->nandResult);
                sWork->nandResult = NAND_RESULT_ACCESS;
            }
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_CLOSE, DWC_AUTH_STATE_WRITE,
                                       DWC_AUTH_STATE_NONE, DWC_AUTH_STATE_NONE);
        }
        break;
    case DWC_AUTH_STATE_CLOSE:
        NANDCloseAsync(&sWork->fileInfo, nand_callback, &sWork->cmdBlock);
        sState = DWC_AUTH_STATE_WAIT_CLOSE;
        break;
    case DWC_AUTH_STATE_WAIT_CLOSE:
        if (sWork->nandDone) {
            DWCi_Auth_HandleNandResult(DWC_AUTH_STATE_LOGIN, DWC_AUTH_STATE_CLOSE,
                                       DWC_AUTH_STATE_NONE, DWC_AUTH_STATE_NONE);
        }
        break;
    case DWC_AUTH_STATE_LOGIN:
        if (sUseWorkUserId) {
            DWCi_Auth_SendRequest(sWork->mode, sWork->ingamesn, sWork->gsbrcd, sWork->userId);
        } else {
            DWCi_Auth_SendRequest(sWork->mode, sWork->ingamesn, sWork->gsbrcd,
                                  sAuthData.userId);
        }
        sWork->startTime = OSGetTime();
        sState = DWC_AUTH_STATE_WAIT_LOGIN;
        break;
    case DWC_AUTH_STATE_WAIT_LOGIN:
        if (sResult.returnCode == 1) {
            sState = DWC_AUTH_STATE_CLEANUP;
        } else if (sResult.returnCode >= -20999 && sResult.returnCode <= -20102) {
            sState = DWC_AUTH_STATE_CLEANUP;
        } else if (sResult.returnCode < 0) {
            sWork->retryCount++;
            if (sWork->retryCount < 3) {
                sState = DWC_AUTH_STATE_LOGIN;
            } else {
                sState = DWC_AUTH_STATE_CLEANUP;
            }
        } else if (OS_TICKS_TO_MSEC(OSGetTime() - sWork->startTime) > 30000) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, " login timeout.\n");
            sState = DWC_AUTH_STATE_CANCEL;
        }
        break;
    case DWC_AUTH_STATE_CANCEL:
        sState = DWC_AUTH_STATE_WAIT_CANCEL;
        NHTTPCancelRequestAsync(sWork->requestId);
        break;
    case DWC_AUTH_STATE_WAIT_CANCEL:
        if (sResult.returnCode != 0) {
            sResult.returnCode = -20100;
            sState = DWC_AUTH_STATE_CLEANUP;
        }
        break;
    case DWC_AUTH_STATE_CLEANUP:
        sState = DWC_AUTH_STATE_WAIT_CLEANUP;
        if (sResult.response != (NHTTPResponseHandle)NULL) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, "NHTTPDestroyResponse()\n");
            NHTTPDestroyResponse(sResult.response);
            sResult.response = NULL;
        }
        NHTTPCleanupAsync(DWCi_Auth_EndProcess);
        break;
    case DWC_AUTH_STATE_IDLE:
    case DWC_AUTH_STATE_WAIT_CLEANUP:
    case DWC_AUTH_STATE_END:
    case DWC_AUTH_STATE_FINISHED:
        break;
    }
}

BOOL DWCi_Auth_IsFinished(void) {
    if (sState == DWC_AUTH_STATE_END) {
        sState = DWC_AUTH_STATE_FINISHED;
        return TRUE;
    }
    if (sState == DWC_AUTH_STATE_IDLE || sState == DWC_AUTH_STATE_FINISHED) {
        return TRUE;
    }
    return FALSE;
}

BOOL DWCi_Auth_IsSucceeded(void) {
    return sResult.returnCode == 1;
}

int DWCi_Auth_GetErrorCode(void) {
    return sResult.returnCode;
}

void DWCi_Auth_GetGameSpyToken(char* token, char* challenge) {
    strcpy(token, sResult.token);
    strcpy(challenge, sResult.challenge);
}

void fn_806D1660(void) {}

u64 DWCi_Auth_GetConsoleUserId(void) {
    return sAuthData.userId;
}

BOOL fn_806D1690(void) {
    return TRUE;
}

int DWCi_Auth_GetIngamesnCheckResult(void) {
    return sResult.ingamesnCheckResult;
}

DWCAuthPrWords* fn_806D16B0(void) {
    return &sPrWords;
}

static void* nhttp_alloc(u32 size, int align) {
    return sWork->alloc(DWC_AUTH_ALLOCTYPE_NHTTP, size);
}

static void nhttp_free(void* ptr) {
    sWork->free(DWC_AUTH_ALLOCTYPE_NHTTP, ptr, 0);
}

static void nand_callback(s32 result, NANDCommandBlock* block) {
    sWork->nandDone = TRUE;
    sWork->nandResult = result;
}

#define DWC_AUTH_ADD_POST(label, src, srcLen)                    \
    do {                                                         \
        len = DWC_Base64Encode((src), (srcLen), post, 0x800);    \
        post[len] = '\0';                                        \
        value = post;                                            \
        post += len + 1;                                         \
        NHTTPAddPostDataAscii(request, (label), value);          \
    } while (0)

void DWCi_Auth_SendRequest(int action, const wchar_t* ingamesn, const char* gsbrcd, u64 userId) {
    static char host[0x100];
    // note: the declaration order below gives the stack layout of the target
    char buf[0x100];
    char homeDir[0x40];
    OSCalendarTime cal;
    u8 macAddress[6];
    NANDStatus status;
    u32 serial;
    const char* url;
    NHTTPRequestHandle request;
    char* post;
    char* value;
    const char* str;
    int len;
    s32 ret;

    post = sWork->postBuf;
    url = DWC_AUTH_URL();
    DWC_Printf(DWC_REPORTFLAG_AUTH, " url = %s\n", url);

    request = NHTTPCreateRequest(url, 1, sWork->recvBuf, sizeof(sWork->recvBuf),
                                 DWCi_Auth_HandleResponse, NULL);
    if (NHTTPSetRootCADefault(request) != 0) {
        OSPanic("dwc_auth_interface.c", 1047, "\tDWC Auth: failed to set RootCA\n");
    }
    if (NHTTPSetClientCertDefault(request) != 0) {
        OSPanic("dwc_auth_interface.c", 1051, "\tDWC Auth: failed to set ClientCert\n");
    }
    NHTTPSetProxyDefault(request);
    NHTTPSetVerifyOption(request, 2);
    NHTTPAddHeaderField(request, "User-Agent", "RVL SDK/1.0");

    if (sWork->mode != 3) {
        str = strstr(sNasUrl[sAuthServer], "//");
    } else {
        str = strstr(sProfUrl[sAuthServer], "//");
    }
    strcpy(host, str + 2);
    value = strstr(host, "/");
    *value = '\0';
    NHTTPAddHeaderField(request, "Host", host);
    NHTTPAddHeaderField(request, "HTTP_X_GAMECD", OSGetAppGamename());
    DWC_Printf(DWC_REPORTFLAG_AUTH, " HTTP_X_GAMECD = %s\n", OSGetAppGamename());

    switch (action) {
    case 0:
        str = "acctcreate";
        DWC_AUTH_ADD_POST("action", str, strlen(str));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " action = acctcreate\n");
        break;
    case 1:
        str = "login";
        DWC_AUTH_ADD_POST("action", str, strlen(str));
        DWC_AUTH_ADD_POST("gsbrcd", gsbrcd, strlen(gsbrcd));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " action = login\n");
        DWC_Printf(DWC_REPORTFLAG_AUTH, " gsbrcd = %s\n", gsbrcd);
        sprintf(buf, "%013llu", userId);
        DWC_AUTH_ADD_POST("userid", buf, strlen(buf));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " userid = 0x%016llx\n", userId);
        DWC_AUTH_ADD_POST("ingamesn", (const char*)ingamesn, wcslen(ingamesn) * 2);
        break;
    case 2:
        str = "svcloc";
        DWC_AUTH_ADD_POST("action", str, strlen(str));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " action = svcloc\n");
        str = sWork->svc;
        DWC_AUTH_ADD_POST("svc", str, strlen(str));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " svc = %s\n", sWork->svc);
        sprintf(buf, "%013llu", userId);
        DWC_AUTH_ADD_POST("userid", buf, strlen(buf));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " userid = 0x%016llx\n", userId);
        break;
    case 3:
        if (sWork->hasUserId == TRUE) {
            sprintf(buf, "%013llu", userId);
            DWC_AUTH_ADD_POST("userid", buf, strlen(buf));
            DWC_Printf(DWC_REPORTFLAG_AUTH, " userid = 0x%016llx\n", userId);
        }
        str = sWork->wregion;
        DWC_AUTH_ADD_POST("wregion", str, strlen(str));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " wregion = %s\n", sWork->wregion);
        NHTTPAddPostDataAscii(request, "wtype", "");
        DWC_Printf(DWC_REPORTFLAG_AUTH, " wtype = \n");
        str = "UTF-16BE";
        DWC_AUTH_ADD_POST("wenc", str, strlen(str));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " wenc = UTF-16BE\n");
        DWC_AUTH_ADD_POST("words", sWork->words, sWork->wordsSize);
        break;
    }

    str = "001000";
    DWC_AUTH_ADD_POST("sdkver", str, strlen(str));
    str = OSGetAppGamename();
    DWC_AUTH_ADD_POST("gamecd", str, strlen(str));

    ret = NANDGetHomeDir(homeDir);
    if (ret == NAND_RESULT_OK) {
        ret = NANDGetStatus(homeDir, &status);
        if (ret == NAND_RESULT_OK) {
            if (status.groupId == 2) {
                strncpy(buf, "02", 3);
            } else {
                sprintf(buf, "%c%c", (status.groupId >> 8) & 0xFF, status.groupId & 0xFF);
            }
        } else {
            DWC_Printf(DWC_REPORTFLAG_AUTH, " NANDGetStatus failed.[%d]\n", ret);
            strncpy(buf, "00", 3);
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " NANDGetHomeDir failed.[%d]\n", ret);
        strncpy(buf, "00", 3);
    }
    DWC_AUTH_ADD_POST("makercd", buf, strlen(buf));
    DWC_Printf(DWC_REPORTFLAG_AUTH, " makercd = %s\n", buf);

    str = "1";
    DWC_AUTH_ADD_POST("unitcd", str, strlen(str));

    fn_80698A28(macAddress);
    sprintf(buf, "%02x%02x%02x%02x%02x%02x", macAddress[0], macAddress[1], macAddress[2],
            macAddress[3], macAddress[4], macAddress[5]);
    DWC_AUTH_ADD_POST("macadr", buf, strlen(buf));
    DWC_Printf(DWC_REPORTFLAG_AUTH, " macadr = %s\n", buf);

    sprintf(buf, "%02d", SCGetLanguage());
    DWC_Printf(DWC_REPORTFLAG_AUTH, " lang = %s\n", buf);
    DWC_AUTH_ADD_POST("lang", buf, strlen(buf));

    if (sWork->mode != 3) {
        OSTicksToCalendarTime(OSGetTime(), &cal);
        sprintf(buf, "%02d%02d%02d%02d%02d%02d", cal.year % 100, cal.month + 1, cal.mday,
                cal.hour, cal.min, cal.sec);
        DWC_AUTH_ADD_POST("devtime", buf, strlen(buf));
        DWC_Printf(DWC_REPORTFLAG_AUTH, " CalendarTime = %s\n", buf);
    }

    if (sWork->ifConfig.selectedMedia == 1 && sWork->mode != 3) {
        sprintf(buf, "%02d", sWork->ifConfig.confMethod);
        DWC_Printf(DWC_REPORTFLAG_AUTH, " confmethod = %s\n", buf);
        DWC_AUTH_ADD_POST("confmethod", buf, strlen(buf));
    }

    str = SCGetProductCode_unk();
    if (str != NULL && SCGetProductSN_unk(&serial)) {
        sprintf(buf, "%s%09d", str, serial);
        DWC_Printf(DWC_REPORTFLAG_AUTH, " csnum = %s\n", buf);
        DWC_AUTH_ADD_POST("csnum", buf, strlen(buf));
    }

    sprintf(buf, "%016lld", fn_806AA690());
    DWC_Printf(DWC_REPORTFLAG_AUTH, " cfc = %s\n", buf);
    DWC_AUTH_ADD_POST("cfc", buf, strlen(buf));

    if (sWork->mode != 3) {
        sprintf(buf, "%02d", SCGetProductArea());
        DWC_Printf(DWC_REPORTFLAG_AUTH, " region = %s\n", buf);
        DWC_AUTH_ADD_POST("region", buf, strlen(buf));
    }

    sWork->requestId = NHTTPSendRequestAsync(request);
    sResult.returnCode = 0;
}

#define DWC_AUTH_DECODE(key) \
    (len = DWC_Base64Decode(token + strlen(key), strlen(token) - strlen(key), buf, sizeof(buf)), \
     buf[len] = '\0')

static void DWCi_Auth_HandleResponse(int error, NHTTPResponseHandle response, void* param) {
    // note: the declaration order below gives the stack and register layout of the target
    char buf[0x100];
    OSCalendarTime cal;
    char* errBody;
    char* body;
    char returnCd[4];
    s64 serverTime;
    int code;
    char* token;
    int len;
    const char* delim;
    BOOL hasReturnCd;

    if (sResult.response != (NHTTPResponseHandle)NULL) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, "NHTTPDestroyResponse()\n");
        NHTTPDestroyResponse(sResult.response);
    }
    sResult.response = response;

    DWC_Printf(DWC_REPORTFLAG_AUTH, " request_callback = %d\n", error);

    if (error == 8) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " nhttp canceled(%d)\n", error);
        sResult.returnCode = 2;
        return;
    }
    if (error != 0) {
        if (error == 14) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, " ssl error(%d)\n", NHTTPSSLGetError());
        }
        DWC_Printf(DWC_REPORTFLAG_AUTH, " nhttp error(%d)\n", error);
        sResult.returnCode = -20100;
        sPrWords.error = TRUE;
        return;
    }

    code = NHTTPGetResultCode(response);
    if (code != 200) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " status code is not 200, but %d\n", code);
        sResult.returnCode = -23000 - code;
        if (NHTTPGetBodyAll(response, &errBody) > 0) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, "%s\n", errBody);
        } else {
            DWC_Printf(DWC_REPORTFLAG_AUTH, "no body\n", errBody);
        }
        return;
    }

    delim = "&\r\n";
    hasReturnCd = FALSE;
    if (NHTTPGetBodyAll(response, &body) > 0) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " %s\n", body);
        for (token = strtok(body, delim); token != (char*)NULL; token = strtok(NULL, delim))
        {
            if (strncmp(token, "retry=", strlen("retry=")) == 0) {
                DWC_AUTH_DECODE("retry=");
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) retry=%s\n", len, buf);
            } else if (strncmp(token, "returncd=", strlen("returncd=")) == 0) {
                DWC_AUTH_DECODE("returncd=");
                strcpy(returnCd, buf);
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) returncd=%s\n", len, buf);
                hasReturnCd = TRUE;
            } else if (strncmp(token, "datetime=", strlen("datetime=")) == 0) {
                DWC_AUTH_DECODE("datetime=");
                if (sscanf(buf, "%04d%02d%02d%02d%02d%02d", &cal.year, &cal.month, &cal.mday,
                           &cal.hour, &cal.min, &cal.sec) != 6)
                {
                    DWC_Printf(DWC_REPORTFLAG_AUTH, " cannot parse datetime: %s\n", buf);
                    serverTime = 0;
                } else {
                    cal.month -= 1;
                    cal.wday = 0;
                    cal.yday = 0;
                    cal.msec = 0;
                    cal.usec = 0;
                    serverTime = OSCalendarTimeToTicks(&cal);
                }
                sResult.timeDiff = serverTime - OSGetTime();
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) datetime=%s\n", len, buf);
            } else if (strncmp(token, "locator=", strlen("locator=")) == 0) {
                DWC_AUTH_DECODE("locator=");
                strcpy(sResult.locator, buf);
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) locator=%s\n", len, buf);
            } else if (strncmp(token, "token=", strlen("token=")) == 0) {
                DWC_AUTH_DECODE("token=");
                strcpy(sResult.token, buf);
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) token=%s\n", len, buf);
            } else if (strncmp(token, "challenge=", strlen("challenge=")) == 0) {
                DWC_AUTH_DECODE("challenge=");
                strcpy(sResult.challenge, buf);
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) challenge=%s\n", len, buf);
            } else if (strncmp(token, "userid=", strlen("userid=")) == 0) {
                DWC_AUTH_DECODE("userid=");
                sscanf(buf, "%llu", &sResult.userId);
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) userid=%llu\n", len, sResult.userId);
                sAuthData.userId = sResult.userId;
            } else if (strncmp(token, "svchost=", strlen("svchost=")) == 0) {
                DWC_AUTH_DECODE("svchost=");
                strcpy(sSvcResult.svcHost, buf);
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) svlhost=%s\n", len, buf);
            } else if (strncmp(token, "servicetoken=", strlen("servicetoken=")) == 0) {
                DWC_AUTH_DECODE("servicetoken=");
                strcpy(sSvcResult.serviceToken, buf);
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) servicetoken=%s\n", len, buf);
            } else if (strncmp(token, "statusdata=", strlen("statusdata=")) == 0) {
                DWC_AUTH_DECODE("statusdata=");
                if (buf[0] == 'Y') {
                    sSvcResult.statusData = TRUE;
                } else {
                    sSvcResult.statusData = FALSE;
                }
                DWC_Printf(DWC_REPORTFLAG_AUTH, " (%d) statusdata=%s\n", len, buf);
            } else if (strncmp(token, "prwords=", strlen("prwords=")) == 0) {
                DWC_AUTH_DECODE("prwords=");
                strcpy(sPrWords.words, buf);
            } else {
                DWC_Printf(DWC_REPORTFLAG_AUTH, " unknown token : %s\n", token);
            }
        }
    }

    if (hasReturnCd) {
        code = strtol(returnCd, NULL, 10);
        if (code == 0 && sWork->mode != 3) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, " cannot parse returncd(%s)\n", returnCd);
            sResult.returnCode = -20101;
            return;
        }
        if (code == 1 && sWork->mode == 3) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, " prof server maintenance(%s)\n", returnCd);
            sResult.returnCode = -33001;
            return;
        }
        if (code >= 100) {
            if (sWork->mode == 3 && code == 100) {
                DWC_Printf(DWC_REPORTFLAG_AUTH,
                           " prof server retruns error (%d) but ignored by DWC library\n", code);
            } else {
                DWC_Printf(DWC_REPORTFLAG_AUTH, " server retruns error (%d)\n", code);
                sResult.returnCode = -20000 - code;
                return;
            }
        }
        if (code == 40) {
            sResult.ingamesnCheckResult = 2;
        } else {
            sResult.ingamesnCheckResult = 1;
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " no return code.\n");
        sResult.returnCode = -20101;
        return;
    }

    sResult.returnCode = 1;
}

static void DWCi_Auth_EndProcess(void) {
    DWC_Printf(DWC_REPORTFLAG_AUTH, "DWCi_Auth_EndProcess()\n");
    sWork->free(DWC_ALLOCTYPE_AUTH, sWork, 0);
    sWork = NULL;
    sState = DWC_AUTH_STATE_END;
}

void DWCi_Auth_HandleNandResult(int next, int onBusy, int onNoExists, int onAccess) {
    s32 result;

    sWork->nandDone = FALSE;
    if (onBusy != DWC_AUTH_STATE_NONE && sWork->nandResult == NAND_RESULT_BUSY &&
        sWork->retryCount < 5)
    {
        sWork->retryCount++;
        sState = onBusy;
        return;
    }

    sWork->retryCount = 0;
    result = sWork->nandResult;
    if (result == NAND_RESULT_OK) {
        sState = next;
    } else if (onNoExists != DWC_AUTH_STATE_NONE && result == NAND_RESULT_NOEXISTS) {
        sState = onNoExists;
    } else if (onAccess != DWC_AUTH_STATE_NONE && result == NAND_RESULT_ACCESS) {
        sState = onAccess;
    } else {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " NAND access failed.[%d]\n", result);
        sResult.returnCode = sWork->nandResult == NAND_RESULT_CORRUPT ? -29001 : -29000;
        DWCi_Auth_EndProcess();
    }
}
