// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Profanity check through the NAS "prof" request. Function names from Mario Kart Wii
// (doldecomp/mkw config/RMCP01/symbols.txt: DWCi_CheckProfanityAsync, DWC_CheckProfanityProcess,
// DWC_CheckProfanityAsync); the file name and the rest are inferred.
#include <dwc/dwc_auth_interface.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <string.h>
#include <wchar.h>

// inferred: game code string (HTTP_X_GAMECD, gamecd); its 4th character is the region
const char* fn_805EBFD0(void);

// result of the prof request (same layout as in dwc_auth_interface.c)
struct DWCAuthPrWords {
    char words[0x34];  // 0x00 inferred: one '0'/'1' per non-empty word
    int error;         // 0x34
};

// inferred: DWC_CheckProfanityProcess results
typedef enum {
    DWC_PROF_STATE_NOT_INITIALIZED = 0,
    DWC_PROF_STATE_OPERATING = 1,
    DWC_PROF_STATE_SUCCESS = 2,
    DWC_PROF_STATE_ERROR = 3
} DWCProfState;

#define DWC_PROF_MAX_WORDS 50     // inferred: from the wordsnum check
#define DWC_PROF_MAX_LENGTH 500   // inferred: "too many characters %d(> 500)"

// inferred: values outside the DWCError enum of dwc_error.h
#define DWC_ERROR_PROF_ANY ((DWCError)17)
#define DWC_ERROR_PROF_NETWORK ((DWCError)18)

// inferred: context of the running check (0x30 bytes)
typedef struct DWCProfContext {
    int wordsnum;      // 0x00
    u16* buffer;       // 0x04 inferred: tab-separated words sent to the server
    char* result;      // 0x08
    int* badwordsnum;  // 0x0C
    s64 timeout;       // 0x10
    BOOL timedOut;     // 0x18
    u8 pad1C[4];       // 0x1C
    u64 usedMask;      // 0x20 inferred: bit i set when words[i] is not empty
    int emptyNum;      // 0x28
    u8 pad2C[4];       // 0x2C
} DWCProfContext;

static DWCProfContext sProfContext;

int DWC_GetIngamesnCheckResult(void) {
    return DWCi_Auth_GetIngamesnCheckResult();
}

BOOL DWCi_CheckProfanityAsync(const u16** words, int wordsnum, const char* reserved, int timeout,
                              char* result, int* badwordsnum, char region) {
    int total;
    int i;
    int pos;
    int j;
    u16* buf;
    BOOL ret;

    memset(&sProfContext, 0, sizeof(sProfContext));
    sProfContext.timedOut = FALSE;
    sProfContext.wordsnum = wordsnum;
    sProfContext.result = result;
    sProfContext.badwordsnum = badwordsnum;

    if (words == NULL || (u32)(wordsnum - 1) > DWC_PROF_MAX_WORDS - 1 || result == NULL ||
        badwordsnum == NULL) {
        return FALSE;
    }

    total = 0;
    for (i = 0; i < wordsnum; i++) {
        if (wcslen(words[i]) != 0) {
            total += wcslen(words[i]) + 1;
            sProfContext.usedMask |= (u64)(1LL << i);
        } else {
            sProfContext.emptyNum++;
        }
    }
    if (total > 0) {
        total--;
    }
    if (total > DWC_PROF_MAX_LENGTH) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, "too many characters %d(> 500)\n", total);
        return FALSE;
    }
    if (sProfContext.usedMask == 0) {
        return TRUE;
    }

    buf = DWC_Alloc(DWC_ALLOCTYPE_AUTH, (total + 1) * 2);
    sProfContext.buffer = buf;
    if (buf == NULL) {
        return FALSE;
    }
    pos = 0;
    for (j = 0; j < wordsnum; j++) {
        if (sProfContext.usedMask & (1LL << j)) {
            wcscpy(&buf[pos], words[j]);
            pos += wcslen(words[j]);
            if (pos >= total) {
                break;
            }
            buf[pos++] = L'\t';
        }
    }

    ret = fn_806D0B40((const char*)sProfContext.buffer, total * 2, region, DWC_Alloc, DWC_Free);
    if (ret == TRUE) {
        if (timeout <= 0) {
            sProfContext.timeout = OSGetTime() + OS_MSEC_TO_TICKS(30000);
        } else {
            sProfContext.timeout = OSGetTime() + timeout * (OS_TIME_SPEED / 1000);
        }
    } else {
        DWC_Free(DWC_ALLOCTYPE_AUTH, sProfContext.buffer, 0);
        sProfContext.buffer = NULL;
    }
    return ret;
}

int DWC_CheckProfanityProcess(void) {
    DWCProfState state;
    DWCAuthPrWords* pr;
    int error;
    int empty;
    int i;
    int j;

    if (sProfContext.buffer == NULL) {
        return DWC_PROF_STATE_NOT_INITIALIZED;
    }

    if (sProfContext.usedMask == 0) {
        DWC_Printf(DWC_REPORTFLAG_AUTH,
                   " all words are empty. profcheck successed unconditionally\n");
        memset(sProfContext.result, 0, sProfContext.wordsnum);
        *sProfContext.badwordsnum = 0;
        for (i = 0; i < sProfContext.wordsnum; i++) {
            sProfContext.result[i] = 0;
        }
        state = DWC_PROF_STATE_SUCCESS;
    } else if (DWCi_Auth_IsFinished()) {
        DWC_Printf(DWC_REPORTFLAG_AUTH, " Auth completed\n");
        pr = fn_806D16B0();
        if (DWCi_Auth_IsSucceeded()) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, " profcheck successed\n");
            memset(sProfContext.result, 0, sProfContext.wordsnum);
            *sProfContext.badwordsnum = 0;
            if (sProfContext.wordsnum - sProfContext.emptyNum != strlen(pr->words)) {
                DWCi_SetError(DWC_ERROR_PROF_ANY, -33020);
                state = DWC_PROF_STATE_ERROR;
            } else {
                empty = 0;
                for (j = 0; j < sProfContext.wordsnum; j++) {
                    if (!(sProfContext.usedMask & (1LL << j))) {
                        empty++;
                        sProfContext.result[j] = 0;
                    } else if (pr->words[j - empty] == '0') {
                        sProfContext.result[j] = 0;
                    } else {
                        sProfContext.result[j] = 1;
                        (*sProfContext.badwordsnum)++;
                    }
                }
                state = DWC_PROF_STATE_SUCCESS;
            }
        } else {
            error = DWCi_Auth_GetErrorCode();
            DWC_Printf(DWC_REPORTFLAG_AUTH, " profcheck failed (%d)\n", error);
            if (pr->error == 1) {
                DWCi_SetError(DWC_ERROR_PROF_NETWORK, -33010);
            } else if ((u32)(error + 23999) <= 999 || error == -20101) {
                DWCi_SetError(DWC_ERROR_PROF_NETWORK, -33020);
            } else if ((u32)(error + 29999) <= 999 || error == -20100) {
                DWCi_SetError(DWC_ERROR_PROF_NETWORK, -33030);
            } else {
                DWCi_SetError(DWC_ERROR_PROF_ANY, error % 1000 - 33000);
            }
            state = DWC_PROF_STATE_ERROR;
        }
    } else {
        if (sProfContext.timeout > 0 && OSGetTime() > sProfContext.timeout &&
            !sProfContext.timedOut) {
            DWC_Printf(DWC_REPORTFLAG_AUTH, "timeout hit\n");
            sProfContext.timedOut = TRUE;
            DWCi_Auth_AbortAuthentication();
            DWCi_SetError(DWC_ERROR_PROF_NETWORK, -33010);
            state = DWC_PROF_STATE_ERROR;
        } else {
            DWCi_Auth_ProcessAuthentication();
            return DWC_PROF_STATE_OPERATING;
        }
    }

    if (sProfContext.buffer != NULL) {
        DWC_Free(DWC_ALLOCTYPE_AUTH, sProfContext.buffer, 0);
    }
    sProfContext.buffer = NULL;
    return state;
}

BOOL DWC_CheckProfanityAsync(const u16** words, int wordsnum, const char* reserved, int timeout,
                             char* result, int* badwordsnum) {
    return DWCi_CheckProfanityAsync(words, wordsnum, reserved, timeout, result, badwordsnum,
                                    fn_805EBFD0()[3]);
}
