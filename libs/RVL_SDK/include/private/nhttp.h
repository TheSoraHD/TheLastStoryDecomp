// NHTTP internal types for the units taken from mscharged-decomp (CC0): NHTTP_stdlib_RVL.c,
// NHTTP_bgnend.c... (libs/RVL_SDK/include/private/nhttp.h + revolution/nhttp.h of that project).
// d_nhttp.c and NHTTP_os_RVL.c (written earlier from the asm) still use revolution/nhttp/NHTTPi.h.
#ifndef PRIVATE_NHTTP_H
#define PRIVATE_NHTTP_H

#include <types.h>
#include <revolution/NCD.h>
#include <revolution/OS.h>
#include <revolution/SSL.h>

// ---- public part (mscharged-decomp revolution/nhttp.h and revolution/nhttp/nhttp_err.h) ----


typedef enum NHTTPErr
{
    NHTTP_ERROR_SYSTEM = -1,
    NHTTP_ERROR_NONE,
    NHTTP_ERROR_ALLOC,
    NHTTP_ERROR_TOOMANYREQ,
    NHTTP_ERROR_SOCKET,
    NHTTP_ERROR_DNS,
    NHTTP_ERROR_CONNECT,
    NHTTP_ERROR_BUFFULL,
    NHTTP_ERROR_HTTPPARSE,
    NHTTP_ERROR_CANCELED,
    NHTTP_ERROR_REVOLUTIONSDK,
    NHTTP_ERROR_REVOLUTIONWIFI,
    NHTTP_ERROR_UNKNOWN,
    NHTTP_ERROR_DNS_PROXY,
    NHTTP_ERROR_CONNECT_PROXY,
    NHTTP_ERROR_SSL,
    NHTTP_ERROR_BUSY,
    NHTTP_ERROR_SSL_ROOTCA,
    NHTTP_ERROR_SSL_CLIENTCERT,
    NHTTP_ERROR_MAX,
} NHTTPErr;





#define NHTTP_HDRRECVBUF_INILEN 1024
#define NHTTP_HDRRECVBUF_BLOCKSHIFT 9
#define NHTTP_HDRRECVBUF_BLOCKLEN (1 << NHTTP_HDRRECVBUF_BLOCKSHIFT)
#define NHTTP_HDRRECVBUF_BLOCKMASK (NHTTP_HDRRECVBUF_BLOCKLEN - 1)

#ifdef __cplusplus
extern "C"
{
#endif

    typedef void* (*NHTTPAlloc)(u32 size, int align);
    typedef void (*NHTTPFree)(void* buf);
    typedef void (*NHTTPCleanupCallback)();

    typedef struct NHTTPRequestInfo NHTTPRequest;
    typedef struct NHTTPResponseInfo NHTTPResponse;
    typedef s32 NHTTPConnection;

    typedef enum NHTTPReqMethod
    {
        NHTTP_REQMETHOD_GET,
        NHTTP_REQMETHOD_POST,
        NHTTP_REQMETHOD_HEAD,
    } NHTTPReqMethod;

    typedef enum NHTTPEncodingType
    {
        NHTTP_ENCODING_TYPE_AUTO,
        NHTTP_ENCODING_TYPE_URL,
        NHTTP_ENCODING_TYPE_MULTIPART,
        NHTTP_ENCODING_TYPE_MAX,
    } NHTTPEncodingType;

    typedef void (*NHTTPReqCallback)(NHTTPErr error, NHTTPResponse* response,
        void* userParam);
    typedef void* (*NHTTPResponseCallback)(void** buffer, void* data,
        u32 contentLength, NHTTPAlloc alloc, NHTTPFree free, void* userParam);
    typedef void (*NHTTPResponseCleanup)(void* buffer, NHTTPFree free,
        void* userParam);

    s32 NHTTPStartup(NHTTPAlloc alloc, NHTTPFree free, u32 priority);
    void NHTTPCleanupAsync(NHTTPCleanupCallback callback);
    s32 NHTTPSSLGetError(void);
    NHTTPRequest* NHTTPCreateRequest(const char* url, NHTTPReqMethod method,
        void* buffer, u32 bufferSize, NHTTPReqCallback callback,
        void* userParam);
    NHTTPRequest* NHTTPCreateRequestEx(const char* url, NHTTPReqMethod method,
        void* buffer, u32 bufferSize, NHTTPReqCallback callback,
        void* userParam, NHTTPResponseCallback responseCallback,
        NHTTPResponseCleanup cleanup);
    s32 NHTTPAddHeaderField(NHTTPRequest* request, const char* name,
        const char* value);
    s32 NHTTPAddPostDataAscii(NHTTPRequest* request, const char* name,
        const char* value);
    NHTTPConnection NHTTPSendRequestAsync(NHTTPRequest* request);
    s32 NHTTPCancelRequestAsync(NHTTPConnection connection);
    void NHTTPDestroyResponse(NHTTPResponse* response);
    s32 NHTTPGetBodyAll(NHTTPResponse* response, void** buffer);
    s32 NHTTPGetResultCode(NHTTPResponse* response);
    s32 NHTTPSetVerifyOption(NHTTPRequest* request, u32 option);
    s32 NHTTPSetProxy(NHTTPRequest* request, const char* server, u16 port,
        const char* username, const char* password);
    s32 NHTTPSetProxyDefault(NHTTPRequest* request);
    s32 NHTTPSetClientCertDefault(NHTTPRequest* request);
    s32 NHTTPSetRootCADefault(NHTTPRequest* request);

#ifdef __cplusplus
}
#endif


// ---- private part (mscharged-decomp private/nhttp.h) ----

#define STR_POSTBOUND "--t9Sf4yfjf1RtvDu3AA"
#define LEN_POSTBOUND 18
#define LEN_URLBUF 256
#undef MIN
#define MIN(AA, BB) (((AA) > (BB)) ? (BB) : (AA))

typedef struct NHTTPBgnEndInfo NHTTPBgnEndInfo;
typedef struct NHTTPRequestInfo NHTTPRequestInfo;
typedef struct NHTTPConnectionInfo NHTTPConnectionInfo;

struct NHTTPBgnEndInfo
{
    NCDIpConfig ipConfig;
    NHTTPAlloc alloc;
    NHTTPFree free;
    BOOL started;
    s32 socket;
    s32 sslError;
    NHTTPErr error;
    u32 _unk7DC;
    void* threadStack;
    void (*sslCallback)(SSLId sslId, void* param);
};

typedef enum NHTTPConnectionEvent
{
    NHTTP_EVENT_NONE,
    NHTTP_EVENT_POST_SEND,
    NHTTP_EVENT_BODY_RECV_FULL,
    NHTTP_EVENT_BODY_RECV_DONE,
    NHTTP_EVENT_COMPLETE,
    NHTTP_CALLBACK_EVENT_MAX,
} NHTTPConnectionEvent;

typedef s32 (*NHTTPConnectionCallback)(NHTTPConnectionInfo* connection,
    NHTTPConnectionEvent event, void* arg);

typedef struct NHTTPConnectionCallbackParam
{
    void* value;
    u32 _unk4;
    u32 _unk8;
    u32 _unkC;
} NHTTPConnectionCallbackParam;

typedef struct NHTTPPostSendArg
{
    const char* label;
    char* buf;
    u32 size;
    u32 offset;
} NHTTPPostSendArg;

typedef struct NHTTPBodyBufArg
{
    char* buf;
    u32 size;
    u32 offset;
} NHTTPBodyBufArg;

typedef struct NHTTPHeader
{
    struct NHTTPHeader* next;
    struct NHTTPHeader* prev;
    const char* name;
    const char* value;
    u32 length;
    u32 _unk14;
} NHTTPHeader;

typedef struct NHTTPReqQueue
{
    struct NHTTPReqQueue* next;
    struct NHTTPReqQueue* prev;
    s32 requestId;
    NHTTPRequestInfo* request;
    s32 _unk10;
} NHTTPReqQueue;

typedef struct NHTTPListInfo
{
    NHTTPReqQueue* reqQueue;
    s32 nextRequestId;
} NHTTPListInfo;

typedef struct NHTTPReqInfo
{
    NHTTPReqQueue* reqQueue;
} NHTTPReqInfo;

typedef struct NHTTPi_HDRBUFLIST
{
    struct NHTTPi_HDRBUFLIST* next_p;
    u8 block[NHTTP_HDRRECVBUF_BLOCKLEN];
} NHTTPi_HDRBUFLIST;

typedef struct NHTTPResponseInfo
{
    volatile s32 headerLen;
    volatile s32 bodyLen;
    volatile s32 totalBodyLen;
    volatile s32 contentLength;
    s32 isSuccess;
    s32 isHeaderParse;
    s32 httpStatus;
    u32 recvBufLen;
    char* allHeader_p;
    char* foundHeader_p;
    char* recvBuf_p;
    NHTTPResponseCallback bufFull;
    NHTTPResponseCleanup freeBuf;
    NHTTPi_HDRBUFLIST* hdrBufBlock_p;
    u8 hdrBufFirst[NHTTP_HDRRECVBUF_INILEN];
    void* param_p;
} NHTTPResponseInfo;

struct NHTTPRequestInfo
{
    BOOL cancel;
    s32 state;
    BOOL secure;
    BOOL proxyEnabled;
    BOOL isRawData;
    s32 hostEnd;
    s32 pathStart;
    s32 method;
    u32 port;
    char* url;
    char* host;
    NHTTPResponseInfo* response;
    NHTTPHeader* headers;
    NHTTPHeader* postData;
    char multipartBoundary[LEN_POSTBOUND + 2];
    char authorization[0x5C];
    s32 authorizationLength;
    SSLId sslId;
    const char* clientCertData;
    u32 clientCertSize;
    const char* privateKeyData;
    u32 privateKeySize;
    const char* rootCAData;
    u32 rootCASize;
    BOOL clientCertDefault;
    u32 verifyOption;
    NHTTPEncodingType encodingType;
    void* sslCallbackParam;
    u32 builtinRootCA;
    u32 builtinClientCert;
    char proxyServer[0x100];
    u32 proxyPort;
    char proxyAuthorization[0x5C];
    s32 proxyAuthorizationLength;
    u32 recvBufferSize;
    u32 sendBufferSize;
    const void* postBuffer;
    u32 postBufferSize;
    void* _unk244;
};

struct NHTTPConnectionInfo
{
    BOOL started;
    s32 state;
    s32 sslError;
    BOOL inProgress;
    NHTTPRequestInfo* request;
    NHTTPResponseInfo* response;
    s32 requestId;
    NHTTPConnectionCallback callback;
    NHTTPConnectionInfo* next;
    u32 postSendBuf;
    u32 postSendSize;
    u32 virtualContentLength;
    NHTTPReqCallback requestCallback;
    char recvBuffer[0x8000] ALIGN(32);
    u32 recvBufferOffset;
    u32 recvBufferRemaining;
};

typedef struct NHTTPSysInfo NHTTPSysInfo;
typedef struct NHTTPMutexInfo NHTTPMutexInfo;
typedef struct NHTTPThreadInfo NHTTPThreadInfo;

void* NHTTPi_alloc(u32 size, int align);
void NHTTPi_SetError(NHTTPBgnEndInfo* info, NHTTPErr error);
s32 NHTTPi_strlen(const char* string);
s32 NHTTPi_compareToken(const char* string, const char* token);
int NHTTPi_memfind(const char* data, int size,
    const char* pattern, int patternSize);
extern const char NHTTPi_strMultipartBound[];
NHTTPHeader* NHTTPi_getHdrFromList(NHTTPHeader** list);
BOOL NHTTP_AddHeaderField(NHTTPRequestInfo* request, NHTTPBgnEndInfo* info,
    const char* name, const char* value);
BOOL NHTTP_AddPostDataAscii(NHTTPRequestInfo* request, NHTTPBgnEndInfo* info,
    const char* label, const char* value);

void NHTTPi_InitBgnEndInfo(NHTTPBgnEndInfo* info);
void NHTTPi_InitListInfo(NHTTPListInfo* info);
void NHTTPi_InitRequestInfo(NHTTPReqInfo* info);
void NHTTPi_InitThreadInfo(NHTTPThreadInfo* info);

void NHTTPi_InitMutexInfo(NHTTPMutexInfo* info);
void NHTTPi_lockReqList(NHTTPMutexInfo* info);
void NHTTPi_unlockReqList(NHTTPMutexInfo* info);

NHTTPConnectionInfo* NHTTPi_ControlConnectionList(void* mutexInfo,
    void* handle, u32 mode);
s32 NHTTPi_CommitConnectionList(void* mutexInfo,
    NHTTPConnectionInfo* connection);
s32 NHTTPi_OmitConnectionList(void* mutexInfo,
    NHTTPConnectionInfo* connection);
NHTTPRequestInfo* NHTTPi_Connection2Request(void* mutexInfo,
    NHTTPConnectionInfo* connection);
NHTTPResponseInfo* NHTTPi_Connection2Response(void* mutexInfo,
    NHTTPConnectionInfo* connection);
NHTTPConnectionInfo* NHTTPi_Request2Connection(void* mutexInfo,
    NHTTPRequestInfo* request);
NHTTPConnectionInfo* NHTTPi_Response2Connection(void* mutexInfo,
    NHTTPResponseInfo* response);
NHTTPConnectionInfo* NHTTPi_GetConnection(void* mutexInfo,
    NHTTPConnectionInfo* connection);
NHTTPRequestInfo* NHTTPi_GetRequest(void* mutexInfo,
    NHTTPRequestInfo* request);
NHTTPResponseInfo* NHTTPi_GetResponse(void* mutexInfo,
    NHTTPResponseInfo* response);
s32 NHTTPi_GetConnectionListLength(void);
s32 NHTTPi_PostSendCallback(void* mutexInfo,
    NHTTPConnectionInfo* connection, const char* label, u32 arg);
void NHTTPi_BufferFullCallback(void* mutexInfo,
    NHTTPConnectionInfo* connection);
void NHTTPi_ReceivedCallback(void* mutexInfo,
    NHTTPConnectionInfo* connection);
void NHTTPi_CompleteCallback(void* mutexInfo,
    NHTTPConnectionInfo* connection);
NHTTPSysInfo* NHTTPi_GetSystemInfoP(void);
NHTTPBgnEndInfo* NHTTPi_GetBgnEndInfoP(NHTTPSysInfo* sysInfo_p);
NHTTPListInfo* NHTTPi_GetListInfoP(NHTTPSysInfo* sysInfo_p);
NHTTPReqInfo* NHTTPi_GetReqInfoP(NHTTPSysInfo* sysInfo_p);
void* NHTTPi_GetThreadInfoP(NHTTPSysInfo* sysInfo_p);
void* NHTTPi_GetMutexInfoP(NHTTPSysInfo* sysInfo_p);
void NHTTPi_SetVirtualContentLength(NHTTPConnectionInfo* connection,
    u32 length);
u32 NHTTPi_GetVirtualContentLength(NHTTPConnectionInfo* connection);

void NHTTPi_InitConnectionList(void);
s32 NHTTPi_SocCheckKeepAlive(s32 socket);
int NHTTPi_SocRecv(void* mutexInfo, NHTTPRequestInfo* request, int socket,
    char* buffer, int length, int flags);
s32 NHTTPi_RecvBuf(void* mutexInfo, NHTTPRequestInfo* request, s32 socket,
    s32 offset, s32 flags);
int NHTTPi_RecvBufN(void* mutexInfo, NHTTPRequestInfo* request, int socket,
    int offset, int length, int flags);
void NHTTPi_NotifyCompletion(NHTTPConnectionInfo* connection);

#endif // PRIVATE_NHTTP_H
