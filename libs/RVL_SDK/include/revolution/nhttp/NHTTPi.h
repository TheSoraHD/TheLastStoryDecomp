#ifndef REVOLUTION_NHTTP_NHTTPI_H
#define REVOLUTION_NHTTP_NHTTPI_H

// Internal NHTTP (RevoEX HTTP library) declarations.
// Function names come from the Wii Menu 4.3 symbol map (koopthekoopa/wii-ipl
// config/43E/symbols.txt) and Pokemon Battle Revolution (doldecomp/pbr).
// Structure layouts are inferred from field offsets in The Last Story.

#include <revolution/OS.h>
#include <revolution/NHTTP.h>

#ifdef __cplusplus
extern "C" {
#endif

// NCD proxy server profile (layout from koopthekoopa/wii-ipl NCDTypes.h)
typedef struct NCDProxyServerProfile {
    u8 mode;            // 0x000
    u8 authType;        // 0x001
    u8 reserved[2];     // 0x002
    char server[256];   // 0x004
    u16 port;           // 0x104
    char username[33];  // 0x106
    char password[33];  // 0x127
} NCDProxyServerProfile;

// inferred: mutex wrapper used by the request list lock
typedef struct NHTTPiMutexInfo {
    OSMutex mutex;    // 0x00
    int initialized;  // 0x18
} NHTTPiMutexInfo;

// inferred: communication thread state
typedef struct NHTTPiThreadInfo {
    OSMessageQueue queue;       // 0x000
    OSMessage messages[3];      // 0x020
    OSThread thread;            // 0x030
    int isMessageQueueCreated;  // 0x348
} NHTTPiThreadInfo;

// inferred: field at 0x7DC of the object passed to NHTTPi_destroyCommThread
typedef struct NHTTPiReqInfo {
    u8 unk000[0x7DC];
    int isThreadEnd;  // 0x7DC
} NHTTPiReqInfo;

// inferred: library-wide state
typedef struct NHTTPiSysInfo NHTTPiSysInfo;

// inferred: startup state; begins with the NCD IP configuration
typedef struct NHTTPiBgnEndInfo {
    u8 unk000[0x28];
    NCDProxyServerProfile httpProxy;  // 0x028
    NCDProxyServerProfile sslProxy;   // 0x170
} NHTTPiBgnEndInfo;

// inferred: receive buffer callbacks given to NHTTPCreateRequestEx
typedef char* (*NHTTPiBufferFullCallback)(char** buf, u32* size, u32 contentLength,
                                         NHTTPAlloc alloc, NHTTPFree free, void* param);
// inferred: post data callback
typedef int (*NHTTPiPostSendCallback)(char* buf, u32* size, u32* offset, u32 length,
                                      void* param);

// inferred: argument block passed to the connection callback
typedef struct NHTTPiCallbackArg {
    char* buf;   // 0x0
    u32 size;    // 0x4
    u32 offset;  // 0x8
    u32 length;  // 0xC
} NHTTPiCallbackArg;

// inferred: per-response object
typedef struct NHTTPiResponse {
    u8 unk00[0x14];
    int isHeaderParsed;                          // 0x14
    int resultCode;                              // 0x18
    u8 unk1C[0x2C - 0x1C];
    NHTTPiBufferFullCallback bufferFullCallback;  // 0x2C
    void* freeBufferCallback;                    // 0x30
} NHTTPiResponse;

// inferred: per-request object
typedef struct NHTTPiRequest {
    u8 unk000[4];
    int isStarted;               // 0x004
    int isSSL;                   // 0x008
    int useProxy;                // 0x00C
    u8 unk010[0x2C - 0x10];
    NHTTPiResponse* response;    // 0x02C
    u8 unk030[0xB0 - 0x30];
    const void* clientCert;      // 0x0B0
    u32 clientCertLen;           // 0x0B4
    const void* privateKey;      // 0x0B8
    u32 privateKeyLen;           // 0x0BC
    const void* rootCA;          // 0x0C0
    u32 rootCALen;               // 0x0C4
    int useBuiltinClientCert;    // 0x0C8
    u32 verifyOption;            // 0x0CC
    u8 unk0D0[0xD8 - 0xD0];
    int builtinRootCA;           // 0x0D8
    int builtinClientCert;       // 0x0DC
    char proxyAddr[256];         // 0x0E0
    int proxyPort;               // 0x1E0
    char proxyAuth[0x5C];        // 0x1E4
    int proxyAuthLen;            // 0x240
    u8 unk244[0x254 - 0x244];
    NHTTPiPostSendCallback postSendCallback;  // 0x254
} NHTTPiRequest;

// inferred: per-connection object
typedef struct NHTTPiConnection {
    u8 unk00[0x14];
    NHTTPResponseHandle response;  // 0x14
    int id;                        // 0x18
    u8 unk1C[0x30 - 0x1C];
    NHTTPRequestCallback callback;  // 0x30
} NHTTPiConnection;

// NHTTP_bgnend.c
void* NHTTPi_alloc(u32 size, int align);
void NHTTPi_free(void* ptr);
int NHTTPi_GetSSLError(NHTTPiSysInfo* sys);
BOOL NHTTPi_Startup(NHTTPiSysInfo* sys, NHTTPAlloc alloc, NHTTPFree free, u32 priority);
int NHTTPi_CleanupAsync(NHTTPiSysInfo* sys, NHTTPCleanupCallback callback);

// NHTTP_control.c
BOOL NHTTP_AddHeaderField(NHTTPiRequest* req, NHTTPiBgnEndInfo* bgnEnd, const char* label,
                          const char* value);
BOOL NHTTP_AddPostDataAscii(NHTTPiRequest* req, NHTTPiBgnEndInfo* bgnEnd, const char* label,
                            const char* value);
// inferred: stripped from The Last Story; signatures follow NHTTP_AddPostDataAscii
BOOL NHTTP_AddPostDataBinary(NHTTPiRequest* req, NHTTPiBgnEndInfo* bgnEnd, const char* label,
                             const void* value, u32 length);
BOOL NHTTP_AddPostDataRaw(NHTTPiRequest* req, const void* data, u32 length);
BOOL NHTTP_SetPostDataEncoding(NHTTPiRequest* req, int encoding);

// NHTTP_os_RVL.c
void NHTTPi_InitMutexInfo(NHTTPiMutexInfo* info);
void NHTTPi_initLockReqList(NHTTPiMutexInfo* info);
void NHTTPi_exitLockReqList(NHTTPiMutexInfo* info);
void NHTTPi_lockReqList(NHTTPiMutexInfo* info);
void NHTTPi_unlockReqList(NHTTPiMutexInfo* info);
BOOL NHTTPi_createCommThread(NHTTPiThreadInfo* info, u32 priority, void* stack);
void NHTTPi_destroyCommThread(NHTTPiThreadInfo* info, NHTTPiReqInfo* reqInfo);
void NHTTPi_idleCommThread(NHTTPiThreadInfo* info);
void NHTTPi_kickCommThread(NHTTPiThreadInfo* info);
void NHTTPi_CheckCurrentThread(NHTTPiThreadInfo* info, BOOL isCommThread);
void* NHTTPi_CommThreadProc(void* param);

// NHTTP_request.c
BOOL NHTTP_DestroyRequest(NHTTPiSysInfo* sys, NHTTPiRequest* req);
BOOL NHTTP_CancelRequestAsync(NHTTPiSysInfo* sys, int id);

// NHTTP_response.c
void NHTTP_DestroyResponse(NHTTPiMutexInfo* mutex, NHTTPResponseHandle response);

// NHTTP_stdlib_RVL.c
void NHTTPi_memcpy(void* dst, const void* src, u32 len);
int NHTTPi_strlen(const char* str);
void NHTTPi_memclr(void* dst, u32 len);
int NHTTPi_Base64Encode(char* dst, const char* src);

// NHTTP_thread.c
void NHTTPi_IsCreateCommThreadMessageQueueOn(NHTTPiThreadInfo* info);
int NHTTPi_IsCreateCommThreadMessageQueue(NHTTPiThreadInfo* info);
void NHTTPi_CommThreadProcMain(void);

// connection functions (d_nhttp_private.c in Pokemon Battle Revolution)
NHTTPConnectionHandle NHTTPCreateConnection(const char* url, int method, void* buf, u32 bufSize,
                                            void* callback, void* param);
int NHTTPStartConnection(NHTTPConnectionHandle connection);
int NHTTPGetBodyBuffer(NHTTPConnectionHandle connection, char** body, u32* size);
void* NHTTPGetUserParam(NHTTPConnectionHandle connection);
int NHTTPGetConnectionError(NHTTPConnectionHandle connection);

// connection list (d_nhttp_common.c in Pokemon Battle Revolution)
void NHTTPi_OmitConnectionList(NHTTPiMutexInfo* mutex, NHTTPiConnection* conn);
NHTTPiRequest* NHTTPi_Connection2Request(NHTTPiMutexInfo* mutex, NHTTPiConnection* conn);
NHTTPiResponse* NHTTPi_Connection2Response(NHTTPiMutexInfo* mutex, NHTTPiConnection* conn);
NHTTPiConnection* NHTTPi_Request2Connection(NHTTPiMutexInfo* mutex, NHTTPRequestHandle request);
NHTTPiConnection* NHTTPi_Response2Connection(NHTTPiMutexInfo* mutex, NHTTPResponseHandle response);
NHTTPiConnection* NHTTPi_GetConnection(NHTTPiMutexInfo* mutex, NHTTPConnectionHandle connection);
NHTTPiRequest* NHTTPi_GetRequest(NHTTPiMutexInfo* mutex, NHTTPRequestHandle request);
NHTTPiResponse* NHTTPi_GetResponse(NHTTPiMutexInfo* mutex, NHTTPResponseHandle response);
NHTTPiSysInfo* NHTTPi_GetSystemInfoP(void);
NHTTPiBgnEndInfo* NHTTPi_GetBgnEndInfoP(NHTTPiSysInfo* sys);
NHTTPiMutexInfo* NHTTPi_GetMutexInfoP(NHTTPiSysInfo* sys);
u32 NHTTPi_GetVirtualContentLength(NHTTPiConnection* conn);

#ifdef __cplusplus
}
#endif

#endif
