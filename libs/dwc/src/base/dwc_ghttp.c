// Rewritten from the target assembly (DWC release build Aug 27 2010).
#include <dwc/dwc_ghttp.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>

// inferred: GameSpy GHTTP entry points, in order ghttpStartup, ghttpCleanup, ghttpThink,
// ghttpGetEx, ghttpCancelRequest and a per-request setter; no source yet
void fn_806F3EE0(void);
void fn_806F3F40(void);
void fn_806F41E0(void);
GHTTPRequest fn_806F3FB0(const char* url, const char* headers, char* buffer, int bufferSize,
                         GHTTPPost post, GHTTPBool throttle, GHTTPBool blocking,
                         void* progressCallback, void* completedCallback, void* param);
void fn_806F41F0(GHTTPRequest request);
void fn_806F4240(GHTTPRequest request, int value);

// inferred: request parameters given to the GHTTP callbacks
typedef struct DWCGHTTPParam {
    void* param;                                   // 0x00
    DWCGHTTPCompletionCallback completionCallback; // 0x04
    DWCGHTTPProgressCallback progressCallback;     // 0x08
    int unk0C;                                     // 0x0C inferred: never set here
    BOOL buffClear;                                // 0x10
} DWCGHTTPParam;

// inferred: one pending request, kept in a singly linked list
typedef struct DWCGHTTPParamEntry {
    DWCGHTTPParam param;              // 0x00
    char* buffer;                     // 0x14
    GHTTPRequest req;                 // 0x18
    struct DWCGHTTPParamEntry* next;  // 0x1C
} DWCGHTTPParamEntry;

// inferred: name; the explicit initializer puts it first in .bss, as in the target
static DWCGHTTPParamEntry* sParamList = NULL;
static int sGHTTPInitCount;                   // inferred: name

static void DWCi_RemoveParamEntry(DWCGHTTPParamEntry* entry);

BOOL DWC_InitGHTTP(const char* gameName) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_InitGHTTP\n");
    fn_806F3EE0();
    sGHTTPInitCount++;
    return TRUE;
}

BOOL DWC_ShutdownGHTTP(void) {
    DWCGHTTPParamEntry* entry;
    DWCGHTTPParamEntry* next;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_ShutdownGHTTP\n");

    if (sGHTTPInitCount <= 0) {
        return TRUE;
    }

    fn_806F3F40();

    if (--sGHTTPInitCount == 0) {
        next = sParamList;
        while (next != NULL) {
            entry = next;
            next = entry->next;
            if (entry->buffer != NULL) {
                DWC_Free(DWC_ALLOCTYPE_GHTTP, entry->buffer, 0);
            }
            DWC_Free(DWC_ALLOCTYPE_GHTTP, entry, 0);
        }
        sParamList = NULL;
    }

    return TRUE;
}

BOOL DWC_ProcessGHTTP(void) {
    if (DWCi_IsError()) {
        return FALSE;
    }
    fn_806F41E0();
    return TRUE;
}

static GHTTPBool DWCi_GHTTPCompletedCallback(GHTTPRequest request, GHTTPResult result,
                                             char* buffer, GHTTPByteCount bufferLen,
                                             void* param) {
    DWCGHTTPParamEntry* entry = (DWCGHTTPParamEntry*)param;
    DWCGHTTPCompletionCallback callback = entry->param.completionCallback;
    BOOL buffClear = entry->param.buffClear;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "GHTTPCompleteCallback result : %d\n", result);

    if (callback != NULL) {
        if (result == GHTTPSuccess) {
            callback(buffer, bufferLen, result, entry->param.param);
        } else if (result == GHTTPRequestCancelled) {
            callback(NULL, 0, result, entry->param.param);
        } else {
            DWCi_HandleGHTTPError(result);
            callback(NULL, 0, result, entry->param.param);
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Callback is NULL\n");
    }

    if (result != GHTTPSuccess || buffClear == TRUE) {
        if (entry != NULL && entry->buffer != NULL) {
            DWC_Free(DWC_ALLOCTYPE_GHTTP, entry->buffer, 0);
        } else {
            buffClear = TRUE;
        }
    }

    DWCi_RemoveParamEntry(entry);

    return buffClear ? TRUE : FALSE;
}

static void DWCi_GHTTPProgressCallback(GHTTPRequest request, GHTTPState state,
                                       const char* buffer, GHTTPByteCount bufferLen,
                                       GHTTPByteCount bytesReceived, GHTTPByteCount totalSize,
                                       void* param) {
    DWCGHTTPParamEntry* entry = (DWCGHTTPParamEntry*)param;

    if (entry->param.progressCallback != NULL) {
        entry->param.progressCallback(state, buffer, bufferLen, bytesReceived, totalSize,
                                      entry->param.param);
    }
}

static DWCGHTTPParamEntry* DWCi_AppendParamEntry(const DWCGHTTPParam* param) {
    DWCGHTTPParamEntry* entry = (DWCGHTTPParamEntry*)DWC_Alloc(DWC_ALLOCTYPE_GHTTP,
                                                               sizeof(DWCGHTTPParamEntry));
    if (entry == NULL) {
        return NULL;
    }

    entry->param = *param;
    entry->next = NULL;
    entry->buffer = NULL;

    if (sParamList == NULL) {
        sParamList = entry;
        return entry;
    }
    entry->next = sParamList;
    sParamList = entry;
    return entry;
}

// inferred: the 3-argument DWC_GetGHTTPData of the DWC API. The game never calls it, so the
// linker strips it; its "DWC_Alloc Error" literal still comes first in .data, which places it
// here. The body is a reconstruction (not verifiable against the target).
int DWC_GetGHTTPData(const char* url, DWCGHTTPCompletionCallback completionCallback, void* param) {
    DWCGHTTPParam ghttpParam;
    DWCGHTTPParamEntry* entry;
    GHTTPRequest req;

    if (DWCi_IsError()) {
        return -1;
    }

    ghttpParam.param = param;
    ghttpParam.completionCallback = completionCallback;
    ghttpParam.progressCallback = NULL;
    ghttpParam.buffClear = TRUE;

    entry = DWCi_AppendParamEntry(&ghttpParam);
    if (entry == NULL) {
        DWCi_HandleGHTTPError(-5);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
        completionCallback(NULL, 0, -5, param);
        return -5;
    }

    req = fn_806F3FB0(url, NULL, NULL, 0, NULL, FALSE, FALSE, NULL, DWCi_GHTTPCompletedCallback,
                      entry);
    if (req < 0) {
        DWCi_HandleGHTTPError(req);
        completionCallback(NULL, 0, req, param);
        DWCi_RemoveParamEntry(entry);
    }

    entry->req = req;
    return req;
}

int DWC_GetGHTTPDataEx(const char* url, int bufferLen, BOOL buffClear,
                       DWCGHTTPProgressCallback progressCallback,
                       DWCGHTTPCompletionCallback completionCallback, void* param) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_GetGHTTPDataEx\n");
    return fn_806A7D40(url, bufferLen, buffClear, NULL, progressCallback, completionCallback,
                       param);
}

int fn_806A7D40(const char* url, int bufferLen, BOOL buffClear, DWCGHTTPPost* post,
                DWCGHTTPProgressCallback progressCallback,
                DWCGHTTPCompletionCallback completionCallback, void* param) {
    DWCGHTTPParam ghttpParam;
    DWCGHTTPParamEntry* entry;
    char* buffer = NULL;
    GHTTPRequest req;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_GetGHTTPDataEx\n");

    if (DWCi_IsError()) {
        return -1;
    }

    ghttpParam.param = param;
    ghttpParam.completionCallback = completionCallback;
    ghttpParam.progressCallback = progressCallback;
    ghttpParam.buffClear = buffClear;

    entry = DWCi_AppendParamEntry(&ghttpParam);
    if (entry == NULL) {
        DWCi_HandleGHTTPError(-5);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
        completionCallback(NULL, 0, -5, param);
        return -5;
    }

    if (bufferLen > 0) {
        buffer = (char*)DWC_Alloc(DWC_ALLOCTYPE_GHTTP, bufferLen);
        if (buffer == NULL) {
            DWCi_HandleGHTTPError(-5);
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
            completionCallback(NULL, 0, -5, param);
            DWCi_RemoveParamEntry(entry);
            return -5;
        }
        entry->buffer = buffer;
    }

    req = fn_806F3FB0(url, NULL, buffer, bufferLen, post == NULL ? NULL : post->post, FALSE, FALSE,
                      progressCallback == NULL ? NULL : DWCi_GHTTPProgressCallback,
                      DWCi_GHTTPCompletedCallback, entry);
    if (req < 0) {
        DWCi_HandleGHTTPError(req);
        completionCallback(NULL, 0, req, param);
        if (entry->buffer != NULL) {
            DWC_Free(DWC_ALLOCTYPE_GHTTP, entry->buffer, 0);
        }
        DWCi_RemoveParamEntry(entry);
    }

    entry->req = req;
    fn_806F4240(req, 1);
    return req;
}

void DWC_CancelGHTTPRequest(int req) {
    DWCGHTTPParamEntry* entry;

    fn_806F41F0(req);

    entry = sParamList;
    while (entry != NULL && entry->req != req) {
        entry = entry->next;
    }

    if (entry != NULL) {
        if (entry->buffer != NULL) {
            DWC_Free(DWC_ALLOCTYPE_GHTTP, entry->buffer, 0);
        }
        DWCi_RemoveParamEntry(entry);
    }
}

int DWCi_HandleGHTTPError(int error) {
    int errorCode = -98000;          // inferred: base error code of the GHTTP sequence
    DWCError dwcError = (DWCError)7; // inferred: GHTTP error kind of DWCError

    if (error == 0) {
        return 0;
    }

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Main, DWCGHTTP error %d\n", error);

    switch (error) {
    case -7:
        errorCode -= 800;
        break;
    case -6:
        errorCode -= 810;
        break;
    case -5:
        errorCode -= 840;
        break;
    case -4:
    case -3:
    case -2:
        errorCode -= 820;
        break;
    case -1:
        errorCode -= 830;
        break;
    case 1:
    case 20:
        dwcError = DWC_ERROR_FATAL;
        errorCode -= 1;
        break;
    case 2:
        errorCode -= 840;
        break;
    case 3:
        errorCode -= 850;
        break;
    case 4:
        errorCode -= 30;
        break;
    case 5:
        errorCode -= 50;
        break;
    case 6:
    case 11:
    case 12:
        errorCode -= 20;
        break;
    case 7:
        errorCode -= 860;
        break;
    case 8:
    case 9:
    case 10:
        errorCode -= 870;
        break;
    case 13:
    case 14:
        errorCode -= 880;
        break;
    case 15:
        errorCode -= 890;
        break;
    case 16:
        errorCode -= 900;
        break;
    case 17:
        errorCode -= 910;
        break;
    }

    DWCi_SetError(dwcError, errorCode);
    return error;
}

static void DWCi_RemoveParamEntry(DWCGHTTPParamEntry* entry) {
    DWCGHTTPParamEntry* prev;
    DWCGHTTPParamEntry* cur;

    if (sParamList == NULL) {
        return;
    }

    if (sParamList == entry) {
        DWCGHTTPParamEntry* next = sParamList->next;
        DWC_Free(DWC_ALLOCTYPE_GHTTP, sParamList, 0);
        sParamList = next;
        return;
    }

    prev = sParamList;
    while ((cur = prev->next) != NULL) {
        if (cur != entry) {
            prev = cur;
        } else {
            prev->next = cur->next;
            DWC_Free(DWC_ALLOCTYPE_GHTTP, cur, 0);
            return;
        }
    }
}
