// NHTTP (RevoEX) public request API. Written from the target assembly; names from the
// Wii Menu 4.3 symbol map (koopthekoopa/wii-ipl) and Pokemon Battle Revolution
// (doldecomp/pbr), which links an older version of this file.
#include <revolution/OS.h>
#include <revolution/nhttp/NHTTPi.h>
#include <revolution/version.h>

#include <stdio.h>

// inferred: connection callback events
enum {
    NHTTP_EVENT_POSTSEND = 1,
    NHTTP_EVENT_BODY_RECV_FULL = 2,
    NHTTP_EVENT_BODY_RECV_DONE = 3,
    NHTTP_EVENT_COMPLETE = 4
};

RVL_LIB_VERSION(NHTTP, "May 12 2009", "11:01:00", "0x4199_60831");
// inferred: never registered in this build (only its string survives in the DOL)
const char* const __NHTTPUnofficialVersion =
    "<< RVL_SDK - NHTTP \trelease build: May 12 2009 11:01:00 (0x4199_60831) UNOFFICIAL >>";
RVL_LIB_VERSION(NHTTPCREATE, "May 12 2009", "11:01:00", "0x4199_60831");

static BOOL nhttpRegistered = FALSE;
static BOOL nhttpCreateRegistered = FALSE;  // inferred

int NHTTPStartup(NHTTPAlloc alloc, NHTTPFree free, u32 priority) {
    NHTTPiSysInfo* sys = NHTTPi_GetSystemInfoP();

    if (!nhttpRegistered) {
        OSRegisterVersion(__NHTTPVersion);
        nhttpRegistered = TRUE;
    }

    return NHTTPi_Startup(sys, alloc, free, priority) ? 0 : -1;
}

int NHTTPCleanupAsync(NHTTPCleanupCallback callback) {
    return NHTTPi_CleanupAsync(NHTTPi_GetSystemInfoP(), callback);
}

int NHTTPSSLGetError(void) {
    return NHTTPi_GetSSLError(NHTTPi_GetSystemInfoP());
}

// inferred: helper for the post data event
static int NHTTPi_CallPostSendCallback(NHTTPiMutexInfo* mutex, NHTTPiConnection* conn,
                                       NHTTPiCallbackArg* arg) {
    NHTTPiRequest* req;
    NHTTPiPostSendCallback callback;

    if (conn != NULL && (req = NHTTPi_Connection2Request(mutex, conn)) != NULL &&
        (callback = req->postSendCallback) != NULL) {
        return callback(arg->buf, &arg->size, &arg->offset, arg->length, NHTTPGetUserParam(conn));
    }

    return -1;
}

// inferred: helper for the receive buffer full event
static void NHTTPi_CallBufferFullCallback(NHTTPiMutexInfo* mutex, NHTTPiConnection* conn,
                                          NHTTPiCallbackArg* arg) {
    NHTTPiResponse* res;
    NHTTPiBufferFullCallback callback;

    if (conn != NULL && (res = NHTTPi_Connection2Response(mutex, conn)) != NULL &&
        (callback = res->bufferFullCallback) != NULL) {
        char* buf = arg->buf;
        u32 contentLength = NHTTPi_GetVirtualContentLength(conn);
        arg->buf = callback(&buf, &arg->size, contentLength, NHTTPi_alloc, NHTTPi_free,
                            NHTTPGetUserParam(conn));
        if (arg->buf != NULL && buf != NULL) {
            arg->offset = 0;
        }
    }
}

// inferred: helper for the completion event
static void NHTTPi_CallCompleteCallback(NHTTPiMutexInfo* mutex, NHTTPiConnection* conn) {
    NHTTPRequestCallback callback;
    NHTTPiResponse* res;

    if (conn != NULL && (callback = conn->callback) != NULL &&
        (res = NHTTPi_Connection2Response(mutex, conn)) != NULL) {
        int error = NHTTPGetConnectionError(conn);
        callback(error, res, NHTTPGetUserParam(conn));
    }
}

static int NHTTPi_TemplateConnectionCallback(NHTTPConnectionHandle connection, int event,
                                             NHTTPiCallbackArg* arg) {
    NHTTPiMutexInfo* mutex = NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP());
    NHTTPiConnection* conn = NHTTPi_GetConnection(mutex, connection);
    int result = 0;

    switch (event) {
    case NHTTP_EVENT_POSTSEND:
        result = NHTTPi_CallPostSendCallback(mutex, conn, arg);
        break;
    case NHTTP_EVENT_BODY_RECV_FULL:
        NHTTPi_CallBufferFullCallback(mutex, conn, arg);
        result = 0;
        break;
    case NHTTP_EVENT_BODY_RECV_DONE:
        break;
    case NHTTP_EVENT_COMPLETE:
        NHTTPi_CallCompleteCallback(mutex, conn);
        result = 0;
        break;
    }

    return result;
}

NHTTPRequestHandle NHTTPCreateRequestEx(const char* url, int method, void* buf, u32 bufSize,
                                        NHTTPRequestCallback callback, void* param,
                                        void* bufferFullCallback, void* freeBufferCallback) {
    NHTTPConnectionHandle connection = NHTTPCreateConnection(
        url, method, buf, bufSize, NHTTPi_TemplateConnectionCallback, param);
    NHTTPiRequest* req = NULL;

    if (connection != NULL) {
        NHTTPiSysInfo* sys = NHTTPi_GetSystemInfoP();
        NHTTPiMutexInfo* mutex = NHTTPi_GetMutexInfoP(sys);
        NHTTPiConnection* conn = NHTTPi_GetConnection(mutex, connection);

        req = NHTTPi_Connection2Request(mutex, conn);
        if (req != NULL) {
            if (req->response != NULL) {
                conn->callback = callback;
                req->response->bufferFullCallback = bufferFullCallback;
                req->response->freeBufferCallback = freeBufferCallback;
                return req;
            }

            NHTTP_DestroyRequest(sys, req);
            NHTTPi_OmitConnectionList(mutex, conn);
            NHTTPi_free(conn);
            req = NULL;
        }
    }

    return req;
}

NHTTPRequestHandle NHTTPCreateRequest(const char* url, int method, void* buf, u32 bufSize,
                                      NHTTPRequestCallback callback, void* param) {
    if (!nhttpCreateRegistered) {
        OSRegisterVersion(__NHTTPCREATEVersion);
        nhttpCreateRegistered = TRUE;
    }

    return NHTTPCreateRequestEx(url, method, buf, bufSize, callback, param, NULL, NULL);
}

int NHTTPAddHeaderField(NHTTPRequestHandle request, const char* label, const char* value) {
    NHTTPiSysInfo* sys = NHTTPi_GetSystemInfoP();
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(sys), request);
    NHTTPiBgnEndInfo* bgnEnd = NHTTPi_GetBgnEndInfoP(sys);

    if (req == NULL) {
        return -1;
    }
    if (req->isStarted) {
        printf("%s can be called before NHTTPStartConnection()\n", __FUNCTION__);
        return -1;
    }

    return NHTTP_AddHeaderField(req, bgnEnd, label, value) ? 0 : -1;
}

int NHTTPAddPostDataAscii(NHTTPRequestHandle request, const char* label, const char* value) {
    NHTTPiSysInfo* sys = NHTTPi_GetSystemInfoP();
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(sys), request);
    NHTTPiBgnEndInfo* bgnEnd = NHTTPi_GetBgnEndInfoP(sys);

    if (req == NULL) {
        return -1;
    }
    if (req->isStarted) {
        printf("%s can be called before NHTTPStartConnection()\n", __FUNCTION__);
        return -1;
    }

    return NHTTP_AddPostDataAscii(req, bgnEnd, label, value) ? 0 : -1;
}

// inferred: NHTTPAddPostDataBinary, NHTTPAddPostDataRaw and NHTTPSetPostDataEncoding are
// stripped from The Last Story; only their strings remain in the DOL. The bodies follow
// NHTTPAddPostDataAscii.
int NHTTPAddPostDataBinary(NHTTPRequestHandle request, const char* label, const void* value,
                           u32 length) {
    NHTTPiSysInfo* sys = NHTTPi_GetSystemInfoP();
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(sys), request);
    NHTTPiBgnEndInfo* bgnEnd = NHTTPi_GetBgnEndInfoP(sys);

    if (req == NULL) {
        return -1;
    }
    if (req->isStarted) {
        printf("%s can be called before NHTTPStartConnection()\n", __FUNCTION__);
        return -1;
    }

    return NHTTP_AddPostDataBinary(req, bgnEnd, label, value, length) ? 0 : -1;
}

int NHTTPAddPostDataRaw(NHTTPRequestHandle request, const void* data, u32 length) {
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);

    if (req == NULL) {
        return -1;
    }
    if (req->isStarted) {
        printf("%s can be called before NHTTPStartConnection()\n", __FUNCTION__);
        return -1;
    }
    if (!NHTTP_AddPostDataRaw(req, data, length)) {
        printf("already called NHTTPAddPostDataAscii or NHTTPAddPostDataBinary "
               "(exclusive fucntion)\n");
        return -1;
    }

    return 0;
}

int NHTTPSetPostDataEncoding(NHTTPRequestHandle request, int encoding) {
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);

    if (req == NULL) {
        return -1;
    }
    if (req->isStarted) {
        printf("%s can be called before NHTTPStartConnection()\n", __FUNCTION__);
        return -1;
    }

    return NHTTP_SetPostDataEncoding(req, encoding) ? 0 : -1;
}

int NHTTPSendRequestAsync(NHTTPRequestHandle request) {
    NHTTPiConnection* conn =
        NHTTPi_Request2Connection(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);

    if (conn != NULL && NHTTPStartConnection(conn) == 0) {
        return conn->id;
    }

    return -1;
}

int NHTTPCancelRequestAsync(int id) {
    return NHTTP_CancelRequestAsync(NHTTPi_GetSystemInfoP(), id) ? 0 : -1;
}

void NHTTPDestroyResponse(NHTTPResponseHandle response) {
    NHTTPiMutexInfo* mutex = NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP());
    NHTTPiConnection* conn = NHTTPi_Response2Connection(mutex, response);

    if (conn != NULL) {
        if (NHTTPi_Connection2Response(mutex, conn) != NULL) {
            NHTTP_DestroyResponse(mutex, conn->response);
        }
        NHTTPi_OmitConnectionList(mutex, conn);
        NHTTPi_free(conn);
    }
}

int NHTTPGetBodyAll(NHTTPResponseHandle response, char** body) {
    NHTTPiConnection* conn =
        NHTTPi_Response2Connection(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), response);
    if (conn != NULL) {
        u32 size = 0;
        return NHTTPGetBodyBuffer(conn, body, &size);
    }

    return -1;
}

int NHTTPGetResultCode(NHTTPResponseHandle response) {
    NHTTPiResponse* res =
        NHTTPi_GetResponse(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), response);

    if (res == NULL) {
        return -1;
    }
    if (res->isHeaderParsed) {
        return res->resultCode;
    }

    return -1;
}

int NHTTPSetVerifyOption(NHTTPRequestHandle request, u32 option) {
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);

    if (req == NULL) {
        return -1;
    }

    req->verifyOption = option;
    return 0;
}

int NHTTPSetProxy(NHTTPRequestHandle request, const char* proxy, int port, const char* username,
                  const char* password) {
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);
    int len;
    int userLen;
    int passLen;
    char auth[65];

    if (req == NULL || proxy == NULL) {
        return -1;
    }

    len = NHTTPi_strlen(proxy);
    if (len > 256) {
        printf("proxy-address exceeded 256 characters\n");
        return -1;
    }
    NHTTPi_memcpy(req->proxyAddr, proxy, len);
    req->proxyPort = port;

    if (username != NULL && password != NULL) {
        userLen = NHTTPi_strlen(username);
        passLen = NHTTPi_strlen(password);
        if (userLen > 32) {
            printf("username exceeded 32 characters\n");
            return -1;
        }
        if (passLen > 32) {
            printf("password exceeded 32 characters\n");
            return -1;
        }
        NHTTPi_memclr(auth, sizeof(auth));
        NHTTPi_memcpy(auth, username, userLen);
        NHTTPi_memcpy(&auth[userLen], ":", 1);
        NHTTPi_memcpy(1 + userLen + auth, password, passLen);
        req->proxyAuthLen = NHTTPi_Base64Encode(req->proxyAuth, auth);
    }

    req->useProxy = TRUE;
    return 0;
}

int NHTTPSetProxyDefault(NHTTPRequestHandle request) {
    NHTTPiSysInfo* sys = NHTTPi_GetSystemInfoP();
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(sys), request);
    NHTTPiBgnEndInfo* bgnEnd = NHTTPi_GetBgnEndInfoP(sys);
    NCDProxyServerProfile* proxy;

    if (req->isSSL) {
        proxy = &bgnEnd->sslProxy;
    } else {
        proxy = &bgnEnd->httpProxy;
    }

    if (proxy->mode == 1) {
        const char* username = NULL;
        const char* password = NULL;
        int result;

        if (proxy->authType == 1) {
            username = proxy->username;
            password = proxy->password;
        }

        OSReport("Using proxy server %s:%d (%s/%s).\n", proxy->server, proxy->port,
                 username != NULL ? username : "[no-auth]",
                 password != NULL ? password : "[no-auth]");

        result = NHTTPSetProxy(req, proxy->server, proxy->port, username, password);
        if (result < 0) {
            OSReport("NHTTPSetProxy failed.(%d)\n", result);
            OSPanic("d_nhttp.c", 988, "NHTTPSetProxy");
        } else {
            return 0;
        }
    }

    return -1;
}

int NHTTPSetRootCADefault(NHTTPRequestHandle request) {
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);
    int result;

    if (req == NULL) {
        result = -1;
    } else {
        req->builtinRootCA = 0;
        req->rootCA = NULL;
        req->rootCALen = 0;
        result = 0;
    }

    return result;
}

int NHTTPSetClientCertDefault(NHTTPRequestHandle request) {
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);
    int result;

    if (req == NULL) {
        result = -1;
    } else {
        req->builtinClientCert = 0;
        req->useBuiltinClientCert = TRUE;
        req->clientCert = NULL;
        req->clientCertLen = 0;
        req->privateKey = NULL;
        req->privateKeyLen = 0;
        result = 0;
    }

    return result;
}

// inferred: stripped from The Last Story; only its name string remains in the DOL.
int NHTTPDisableVerifyOptionForDebug(NHTTPRequestHandle request, u32 option) {
    NHTTPiRequest* req = NHTTPi_GetRequest(NHTTPi_GetMutexInfoP(NHTTPi_GetSystemInfoP()), request);

    if (req == NULL) {
        return -1;
    }
    if (req->isStarted) {
        printf("%s can be called before NHTTPStartConnection()\n", __FUNCTION__);
        return -1;
    }

    req->verifyOption = option;
    return 0;
}
