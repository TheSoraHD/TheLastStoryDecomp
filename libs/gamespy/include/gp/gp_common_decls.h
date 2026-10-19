/*
Common GameSpy declarations used by the GP (presence) sources.

Declarations taken from doldecomp/mkw (CC0 1.0) lib/gamespy/common/*.h,
lib/gamespy/md5.h and lib/gamespy/common/gsUdpEngine.h, commit
8b9b5ee653da28663e498039e52f5d19f929a011, and from the GameSpy OpenSDK
(github.com/nitrocaster/GameSpy, commit d1deb2d1a951cf77933dda040b8d311cc09815a7).
GameSpy SDK code, BSD license (IGN Entertainment), see
libs/gamespy/src/GP/LICENSE.txt.

This header is temporary: it collects the parts of the GameSpy common headers
(gsPlatform.h, gsDebug.h, gsAssert.h, gsAvailable.h, gsUdpEngine.h, md5.h)
that GP needs on top of gt2_common_decls.h, so the GP units can be built
before the common units are decompiled. It will be merged into the common
headers later.

Functions whose target symbol still has an automatic name are mapped with a
macro to their current symbols.txt name.
*/

#ifndef _GP_COMMON_DECLS_H_
#define _GP_COMMON_DECLS_H_

#include "gt2/gt2_common_decls.h"

#ifdef __cplusplus
extern "C" {
#endif

/* MSL stdio.h (the MSL headers used here do not define it) */
#ifndef FILENAME_MAX
#define FILENAME_MAX 256
#endif

/* MSL time.h (not present in the MSL headers used here); 32-bit: GPIPeer has
   inputBuffer at 0x1C */
typedef long time_t;

/* gsPlatformUtil.h: time() is gsiTimeInSec() on this platform */
time_t gsiTimeInSec(time_t* timer);
#define time(t) gsiTimeInSec(t)

/* MSL stdlib.h (not declared by the MSL headers used here). inferred: the
   target calls strtod with only the string argument set, so atof() resolves
   to strtod declared with a single parameter */
double strtod(const char* str);
#define atof(s) strtod(s)

/* gsPlatform.h */
/* assert expands to nothing: with ((void)0) the compiler no longer inlines
   gpiValueForKey into gpiCheckForError or gpiDestroyOperation into
   gpiRemoveOperation as the original does */
#undef assert
#define assert(x)

#define NOFILE 1
#define GSI_DOMAIN_NAME "gs.nintendowifi.net"
#define gsi_char char

char* _strlwr(char* string);
char* goastrdup(const char* src);

/* gsCommon.h */
#define GSI_LIMIT(x, minx, maxx)                                               \
  (((x) < (minx) ? (minx) : ((x) > (maxx) ? (maxx) : (x))))
#define GSI_WRAP(x, minx, maxx)                                                \
  (((x) < (minx) ? (maxx - 1) : ((x) >= (maxx) ? (minx) : (x))))
#define GSI_DIM(x) (sizeof(x) / sizeof((x)[0]))

/* gsPlatformSocket.h */
#define WSAETIMEDOUT SO_ETIMEDOUT
#define WSAEISCONN SO_EISCONN
#define WSAENOTCONN SO_ENOTCONN
#define WSAECONNREFUSED SO_ECONNREFUSED
#define WSAEINVAL SO_EINVAL
#define IPPROTO_TCP SO_IPPROTO_TCP
int connect(SOCKET sock, const SOCKADDR* addr, int len);
int send(SOCKET sock, const char* buf, int len, int flags);
int recv(SOCKET sock, char* buf, int len, int flags);
int listen(SOCKET sock, int backlog);
SOCKET accept(SOCKET sock, SOCKADDR* addr, int* len);
int shutdown(SOCKET sock, int how);
int GSISocketSelect(SOCKET theSocket, int* theReadFlag, int* theWriteFlag,
                    int* theExceptFlag);
#ifndef SOMAXCONN
#define SOMAXCONN 5
#endif

/* gsPlatformUtil.h */
/* inferred: the truncating strcpy at 0x806D9590 (still fn_806D9590 in symbols.txt) */
char* gsiSafeStrcpyA(char* dest, const char* src, unsigned int size);
void B64Encode(const char* input, char* output, int inlen, int encodingType);
void B64Decode(const char* input, char* output, int inlen, int* outlen,
               int encodingType);

/* gsAssert.h (release build) */
#define GS_ASSERT(x) {};
#define GS_ASSERT_STR(x, t) {};
#define GS_FAIL()
#define GS_FAIL_STR(x)

/* gsDebug.h (release build: debug output is compiled out) */
typedef int GSIDebugLevel;
#define GSIDebugLevel_None (GSIDebugLevel)(0x00)
#define GSIDebugLevel_HotError (GSIDebugLevel)(0x01)
#define GSIDebugLevel_WarmError (GSIDebugLevel)(0x02)
#define GSIDebugLevel_Warning (GSIDebugLevel)(0x04)
#define GSIDebugLevel_Notice (GSIDebugLevel)(0x08)
#define GSIDebugLevel_Comment (GSIDebugLevel)(0x10)
#define GSIDebugLevel_RawDump (GSIDebugLevel)(0x20)
#define GSIDebugLevel_StackTrace (GSIDebugLevel)(0x40)
#define GSIDebugLevel_Verbose (GSIDebugLevel)(0x1F)

typedef enum {
  GSIDebugType_Network,
  GSIDebugType_File,
  GSIDebugType_Memory,
  GSIDebugType_State,
  GSIDebugType_Misc,
  GSIDebugType_Count,
  GSIDebugType_All = GSIDebugType_Count
} GSIDebugType;

typedef enum {
  GSIDebugCat_App,
  GSIDebugCat_GP,
  GSIDebugCat_Peer,
  GSIDebugCat_QR2,
  GSIDebugCat_SB,
  GSIDebugCat_Voice,
  GSIDebugCat_AD,
  GSIDebugCat_NatNeg,
  GSIDebugCat_HTTP,
  GSIDebugCat_CDKey,
  GSIDebugCat_Common,
  GSIDebugCat_Count,
  GSIDebugCat_All = GSIDebugCat_Count
} GSIDebugCategory;

/* the arguments are still evaluated (calls to inet_ntoa remain in the code) */
#define gsDebugFormat(...) ((void)(__VA_ARGS__))
#define gsDebugVaList(...) ((void)(__VA_ARGS__))
#define gsDebugBinary(...) ((void)0)

/* gsAvailable.h */
typedef enum {
  GSIACWaiting,
  GSIACAvailable,
  GSIACUnavailable,
  GSIACTemporarilyUnavailable
} GSIACResult;

/* defined in gsAvailable.c; still named lbl_ in symbols.txt */
extern GSIACResult __GSIACResult;
extern char __GSIACGamename[64];

/* md5.h */
typedef unsigned char* POINTER;
typedef unsigned short int UINT2;
typedef unsigned int UINT4;

typedef struct {
  UINT4 state[4];
  UINT4 count[2];
  unsigned char buffer[64];
} MD5_CTX;

void MD5Init(MD5_CTX*);
void MD5Update(MD5_CTX*, unsigned char*, unsigned int);
void MD5Final(unsigned char[16], MD5_CTX*);
void MD5Print(unsigned char[16], char[33]);
void MD5Digest(unsigned char*, unsigned int, char[33]);

/* gsUdpEngine.h */
#define GS_UDP_DEFAULT_OUT_BUFFSIZE 1460
#define GS_UDP_DEFAULT_IN_BUFFSIZE 1500
#define GS_IP_ADDR_AND_PORT 22
#define GS_UDP_MSG_HEADER_LEN 16
#define GS_UDP_RELIABLE_MSG_HEADER 7

typedef enum _GSUdpErrorCode {
  GS_UDP_NO_ERROR,
  GS_UDP_NO_MEMORY,
  GS_UDP_REJECTED,
  GS_UDP_NETWORK_ERROR,
  GS_UDP_ADDRESS_ERROR,
  GS_UDP_ADDRESS_ALREADY_IN_USE,
  GS_UDP_TIMED_OUT,
  GS_UDP_REMOTE_ERROR,
  GS_UDP_SEND_FAILED,
  GS_UDP_INVALID_MESSAGE,
  GS_UDP_PARAMETER_ERROR,
  GS_UDP_NOT_INITIALIZED,
  GS_UDP_MSG_TOO_BIG,
  GS_UDP_UNKNOWN_ERROR,
  GS_UDP_NUM_ERROR_CODES
} GSUdpErrorCode;

typedef enum _GSUdpPeerState {
  GS_UDP_PEER_CONNECTING,
  GS_UDP_PEER_CONNECTED,
  GS_UDP_PEER_CLOSING,
  GS_UDP_PEER_CLOSED,
  GS_UDP_PEER_STATE_NUM
} GSUdpPeerState;

typedef enum _GSUdpCloseReason {
  GS_UDP_CLOSED_LOCALLY,
  GS_UDP_CLOSED_REMOTELY,
  GS_UDP_CLOSED_BY_COMM_ERROR,
  GS_UDP_CLOSED_BY_LOW_MEM,
  GS_UDP_CLOSED_NUM
} GSUdpCloseReason;

typedef void (*gsUdpErrorCallback)(GSUdpErrorCode theCode, void* theUserData);
typedef void (*gsUdpAppConnectAttemptCallback)(
    unsigned int theIp, unsigned short thePort, int theLatency,
    unsigned char* theInitMsg, unsigned int theInitMsgLen, void* theUserData);
typedef void (*gsUdpConnClosedCallback)(unsigned int ip, unsigned short port,
                                        GSUdpCloseReason reason,
                                        void* theUserData);
typedef void (*gsUdpConnReceivedDataCallback)(
    unsigned int ip, unsigned short port, unsigned char* message,
    unsigned int messageLength, gsi_bool reliable, void* theUserData);
typedef void (*gsUdpConnConnectedCallback)(unsigned int ip, unsigned short port,
                                           GSUdpErrorCode error,
                                           gsi_bool rejected,
                                           void* theUserData);
typedef void (*gsUdpConnPingCallback)(unsigned int ip, unsigned short port,
                                      unsigned int latency, void* theUserData);
typedef gsi_bool (*gsUdpUnknownMsgCallback)(unsigned int ip,
                                            unsigned short port,
                                            unsigned char* message,
                                            unsigned int messageLength,
                                            void* theUserData);

/* gsUdpEngine functions still named fn_ in symbols.txt */

gsi_bool gsUdpEngineIsInitialized(void);
GSUdpErrorCode gsUdpEngineInitialize(
    unsigned short thePort, int theIncomingBufSize, int theOutgoingBufSize,
    gsUdpErrorCallback theAppNetworkError,
    gsUdpConnConnectedCallback theAppConnected,
    gsUdpConnClosedCallback theAppClosed, gsUdpConnPingCallback theAppPing,
    gsUdpConnReceivedDataCallback theAppReceive,
    gsUdpUnknownMsgCallback theAppUnownMsg,
    gsUdpAppConnectAttemptCallback theAppConnectAttempt, void* theAppUserData);
GSUdpErrorCode gsUdpEngineThink(void);
GSUdpErrorCode gsUdpEngineShutdown(void);
GSUdpErrorCode gsUdpEngineGetPeerState(unsigned int theIp,
                                       unsigned short thePort,
                                       GSUdpPeerState* thePeerState);
GSUdpErrorCode
gsUdpEngineStartTalkingToPeer(unsigned int theIp, unsigned short thePort,
                              char theInitMsg[GS_UDP_MSG_HEADER_LEN],
                              int timeOut);
GSUdpErrorCode gsUdpEngineAcceptPeer(unsigned int theIp,
                                     unsigned short thePort);
GSUdpErrorCode gsUdpEngineRejectPeer(unsigned int theIp,
                                     unsigned short thePort);
GSUdpErrorCode gsUdpEngineSendMessage(unsigned int theIp,
                                      unsigned short thePort,
                                      char theHeader[GS_UDP_MSG_HEADER_LEN],
                                      unsigned char* theMsg,
                                      unsigned int theMsgLen,
                                      gsi_bool theReliable);
GSUdpErrorCode
gsUdpEngineAddMsgHandler(char theInitMsg[GS_UDP_MSG_HEADER_LEN],
                         char theHeader[GS_UDP_MSG_HEADER_LEN],
                         gsUdpErrorCallback theMsgHandlerError,
                         gsUdpConnConnectedCallback theMsgHandlerConnected,
                         gsUdpConnClosedCallback theMsgHandlerClosed,
                         gsUdpConnPingCallback theMsgHandlerPing,
                         gsUdpConnReceivedDataCallback theMsgHandlerRecv,
                         void* theUserData);
GSUdpErrorCode
gsUdpEngineRemoveMsgHandler(char theHeader[GS_UDP_MSG_HEADER_LEN]);
gsi_bool gsUdpEngineNoMoreMsgHandlers(void);
gsi_bool gsUdpEngineNoApp(void);
int gsUdpEngineGetPeerOutBufferFreeSpace(unsigned int theIp,
                                         unsigned short thePort);
unsigned short gsUdpEngineGetLocalPort(void);

#ifdef __cplusplus
}
#endif

#endif // _GP_COMMON_DECLS_H_
