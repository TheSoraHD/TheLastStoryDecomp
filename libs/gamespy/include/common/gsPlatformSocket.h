// GameSpy SDK socket layer, Revolution flavour (see libs/gamespy/license.txt).
#ifndef __GSPLATFORMSOCKET_H__
#define __GSPLATFORMSOCKET_H__

#include "common/gsPlatform.h"
#include "common/gsSORevolution.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AF_INET SO_PF_INET
#define SOCK_DGRAM SO_SOCK_DGRAM
#define SOCK_STREAM SO_SOCK_STREAM
#define IPPROTO_UDP SO_IPPROTO_UDP
#define IPPROTO_TCP SO_IPPROTO_TCP
#define INADDR_ANY SO_INADDR_ANY
#define SOL_SOCKET SO_SOL_SOCKET
#define SO_SNDBUF SO_SO_SNDBUF
#define SO_RCVBUF SO_SO_RCVBUF
#define SO_REUSEADDR SO_SO_REUSEADDR

typedef int SOCKET;
typedef struct SOSockAddr SOCKADDR;
#define sockaddr SOSockAddr
typedef struct SOSockAddrIn SOCKADDR_IN;
#define sockaddr_in SOSockAddrIn
#define sin_family family
#define sin_port port
#define sin_addr addr
typedef struct SOInAddr IN_ADDR;
#define in_addr SOInAddr
#define s_addr addr
typedef struct SOHostEnt HOSTENT;
#define hostent SOHostEnt
#define h_name name
#define h_aliases aliases
#define h_addrtype addrType
#define h_length length
#define h_addr_list addrList
#define h_addr addrList[0]

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define INADDR_NONE 0xffffffff

#define htonl SOHtoNl
#define ntohl SONtoHl
#define htons SOHtoNs
#define ntohs SONtoHs

int socket(int pf, int type, int protocol);
int closesocket(SOCKET sock);
int shutdown(SOCKET sock, int how);
int bind(SOCKET sock, const SOCKADDR* addr, int len);
int connect(SOCKET sock, const SOCKADDR* addr, int len);
int recv(SOCKET sock, char* buf, int len, int flags);
int recvfrom(SOCKET sock, char* buf, int len, int flags, SOCKADDR* addr, int* fromlen);
SOCKET send(SOCKET sock, const char* buf, int len, int flags);
SOCKET sendto(SOCKET sock, const char* buf, int len, int flags, const SOCKADDR* addr, int tolen);
SOCKET setsockopt(SOCKET sock, int level, int optname, const char* optval, int optlen);
int getsockname(SOCKET sock, SOCKADDR* addr, int* len);
unsigned long inet_addr(const char* name);
int GOAGetLastError(SOCKET sock);
int GSISocketSelect(SOCKET theSocket, int* theReadFlag, int* theWriteFlag, int* theExceptFlag);
HOSTENT* gethostbyname(const char* name);

#define gsiSocketIsError(theReturnValue) ((theReturnValue) == -1)
#define gsiSocketIsNotError(theReturnValue) ((theReturnValue) != -1)

int SetSockBlocking(SOCKET sock, int isblocking);
int SetReceiveBufferSize(SOCKET sock, int size);
int CanReceiveOnSocket(SOCKET sock);
int CanSendOnSocket(SOCKET sock);
HOSTENT* getlocalhost(void);
int IsPrivateIP(IN_ADDR* addr);
gsi_u32 gsiGetBroadcastIP(void);

#ifdef __cplusplus
}
#endif

#endif
