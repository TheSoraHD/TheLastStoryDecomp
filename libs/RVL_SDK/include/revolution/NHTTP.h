#ifndef REVOLUTION_NHTTP_H
#define REVOLUTION_NHTTP_H

// Public NHTTP (RevoEX HTTP library) API.
// Function names from koopthekoopa/wii-ipl (CC0 1.0), libs/RevoEX/include/revolution/nhttp.h
// and the Wii Menu 4.3 symbol map; signatures adapted to the NHTTP build linked by
// The Last Story (May 12 2009).

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* NHTTPRequestHandle;     // inferred
typedef void* NHTTPResponseHandle;    // inferred
typedef void* NHTTPConnectionHandle;  // inferred

typedef void* (*NHTTPAlloc)(u32 size, int align);
typedef void (*NHTTPFree)(void* ptr);

// inferred: completion callback (error, response, user parameter)
typedef void (*NHTTPRequestCallback)(int error, NHTTPResponseHandle response, void* param);
typedef void (*NHTTPCleanupCallback)(void);

int NHTTPStartup(NHTTPAlloc alloc, NHTTPFree free, u32 priority);
int NHTTPCleanupAsync(NHTTPCleanupCallback callback);
int NHTTPSSLGetError(void);

NHTTPRequestHandle NHTTPCreateRequest(const char* url, int method, void* buf, u32 bufSize,
                                      NHTTPRequestCallback callback, void* param);
NHTTPRequestHandle NHTTPCreateRequestEx(const char* url, int method, void* buf, u32 bufSize,
                                        NHTTPRequestCallback callback, void* param,
                                        void* bufferFullCallback, void* freeBufferCallback);
int NHTTPAddHeaderField(NHTTPRequestHandle request, const char* label, const char* value);
int NHTTPAddPostDataAscii(NHTTPRequestHandle request, const char* label, const char* value);
int NHTTPAddPostDataBinary(NHTTPRequestHandle request, const char* label, const void* value,
                           u32 length);
int NHTTPAddPostDataRaw(NHTTPRequestHandle request, const void* data, u32 length);
int NHTTPSetPostDataEncoding(NHTTPRequestHandle request, int encoding);
int NHTTPSendRequestAsync(NHTTPRequestHandle request);
int NHTTPCancelRequestAsync(int id);
void NHTTPDestroyResponse(NHTTPResponseHandle response);
int NHTTPGetBodyAll(NHTTPResponseHandle response, char** body);
int NHTTPGetResultCode(NHTTPResponseHandle response);
int NHTTPSetVerifyOption(NHTTPRequestHandle request, u32 option);
int NHTTPDisableVerifyOptionForDebug(NHTTPRequestHandle request, u32 option);
int NHTTPSetProxy(NHTTPRequestHandle request, const char* proxy, int port, const char* username,
                  const char* password);
int NHTTPSetProxyDefault(NHTTPRequestHandle request);
int NHTTPSetRootCADefault(NHTTPRequestHandle request);
int NHTTPSetClientCertDefault(NHTTPRequestHandle request);

#ifdef __cplusplus
}
#endif

#endif
