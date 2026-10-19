#ifndef DWC_GHTTP_H
#define DWC_GHTTP_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// inferred: GameSpy GHTTP types (ghttp.h) used by the DWC wrapper
typedef int GHTTPRequest;
typedef int GHTTPResult;
typedef int GHTTPState;
typedef int GHTTPBool;
typedef int GHTTPByteCount;
typedef void* GHTTPPost;

// inferred: GHTTPResult values used by this file
#define GHTTPSuccess 0
#define GHTTPRequestCancelled 18

// inferred: callback types from the NitroDWC/RVL DWC API (argument order from the call sites)
typedef void (*DWCGHTTPCompletionCallback)(const char* buf, int buflen, GHTTPResult result,
                                           void* param);
typedef void (*DWCGHTTPProgressCallback)(GHTTPState state, const char* buf, int buflen,
                                         int bytesReceived, int totalSize, void* param);

// inferred: wrapper of a GHTTPPost (only the first field is read here)
typedef struct DWCGHTTPPost {
    GHTTPPost post; // 0x00
} DWCGHTTPPost;

BOOL DWC_InitGHTTP(const char* gameName);
BOOL DWC_ShutdownGHTTP(void);
BOOL DWC_ProcessGHTTP(void);
int DWC_GetGHTTPData(const char* url, DWCGHTTPCompletionCallback completionCallback, void* param);
int DWC_GetGHTTPDataEx(const char* url, int bufferLen, BOOL buffClear,
                       DWCGHTTPProgressCallback progressCallback,
                       DWCGHTTPCompletionCallback completionCallback, void* param);
// inferred: internal variant that also takes a POST object; real name unknown
int fn_806A7D40(const char* url, int bufferLen, BOOL buffClear, DWCGHTTPPost* post,
                DWCGHTTPProgressCallback progressCallback,
                DWCGHTTPCompletionCallback completionCallback, void* param);
void DWC_CancelGHTTPRequest(int req);
int DWCi_HandleGHTTPError(int error);

#ifdef __cplusplus
}
#endif
#endif
