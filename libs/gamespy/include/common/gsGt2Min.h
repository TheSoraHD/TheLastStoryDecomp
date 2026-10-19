// GameSpy Transport SDK 2 public header, reduced to what the common units need
// (gt2/gt2.h is the full header used by the GT2 units; to be unified later)
// (see libs/gamespy/license.txt).
#ifndef _GS_GT2_MIN_H_
#define _GS_GT2_MIN_H_

#include "common/gsCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef int GT2Bool;
#define GT2False 0
#define GT2True 1
typedef unsigned char GT2Byte;
typedef struct GTI2Socket* GT2Socket;
typedef struct GTI2Connection* GT2Connection;
typedef unsigned short GT2MessageID;

typedef enum {
    GT2Success,
    GT2OutOfMemory,
    GT2Rejected,
    GT2NetworkError,
    GT2AddressError,
    GT2DuplicateAddress,
    GT2TimedOut,
    GT2NegotiationError,
    GT2InvalidConnection,
    GT2InvalidMessage,
    GT2SendFailed
} GT2Result;

typedef enum {
    GT2Connecting,
    GT2Connected,
    GT2Closing,
    GT2Closed
} GT2ConnectionState;

typedef enum {
    GT2LocalClose,
    GT2RemoteClose,
    GT2CommunicationError,
    GT2SocketError,
    GT2NotEnoughMemory
} GT2CloseReason;

typedef void (*gt2SocketErrorCallback)(GT2Socket socket);
typedef void (*gt2ConnectedCallback)(GT2Connection connection, GT2Result result, GT2Byte* message,
                                     int len);
typedef void (*gt2ReceivedCallback)(GT2Connection connection, GT2Byte* message, int len,
                                    GT2Bool reliable);
typedef void (*gt2ClosedCallback)(GT2Connection connection, GT2CloseReason reason);
typedef void (*gt2PingCallback)(GT2Connection connection, int latency);

typedef struct {
    gt2ConnectedCallback connected;
    gt2ReceivedCallback received;
    gt2ClosedCallback closed;
    gt2PingCallback ping;
} GT2ConnectionCallbacks;

typedef void (*gt2ConnectAttemptCallback)(GT2Socket socket, GT2Connection connection,
                                          unsigned int ip, unsigned short port, int latency,
                                          GT2Byte* message, int len);
typedef GT2Bool (*gt2UnrecognizedMessageCallback)(GT2Socket socket, unsigned int ip,
                                                  unsigned short port, GT2Byte* message, int len);

// The gt2Main.c and gt2Utility.c functions are not named in symbols.txt yet; these macros map
// the SDK names to their addresses (same order and sizes as the SDK).

GT2Result gt2CreateSocket(GT2Socket* socket, const char* localAddress, int outgoingBufferSize,
                          int incomingBufferSize, gt2SocketErrorCallback callback);
void gt2CloseSocket(GT2Socket socket);
void gt2Think(GT2Socket socket);
GT2Result gt2Connect(GT2Socket socket, GT2Connection* connection, const char* remoteAddress,
                     const GT2Byte* message, int len, int timeout,
                     GT2ConnectionCallbacks* callbacks, GT2Bool blocking);
GT2Result gt2Send(GT2Connection connection, const GT2Byte* message, int len, GT2Bool reliable);
void gt2Listen(GT2Socket socket, gt2ConnectAttemptCallback callback);
GT2Bool gt2Accept(GT2Connection connection, GT2ConnectionCallbacks* callbacks);
void gt2Reject(GT2Connection connection, const GT2Byte* message, int len);
SOCKET gt2GetSocketSOCKET(GT2Socket socket);
void gt2SetUnrecognizedMessageCallback(GT2Socket socket, gt2UnrecognizedMessageCallback callback);
GT2ConnectionState gt2GetConnectionState(GT2Connection connection);
unsigned int gt2GetRemoteIP(GT2Connection connection);
unsigned short gt2GetRemotePort(GT2Connection connection);
unsigned int gt2GetLocalIP(GT2Socket socket);
unsigned short gt2GetLocalPort(GT2Socket socket);
int gt2GetOutgoingBufferSize(GT2Connection connection);
int gt2GetOutgoingBufferFreeSpace(GT2Connection connection);
const char* gt2AddressToString(unsigned int ip, unsigned short port, char string[22]);

#ifdef __cplusplus
}
#endif

#endif
