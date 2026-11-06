// Revolution SO entry points used by the GameSpy SDK.
// inferred: SO API order in the SO library (sizes and call signatures match); named in symbols.txt.
#ifndef __GSSOREVOLUTION_H__
#define __GSSOREVOLUTION_H__

#include <revolution/SO.h>


int SOSocket(int pf, int type, int protocol);
int SOClose(int s);
int SOBind(int s, const void* sockAddr);
int SOConnect(int s, const void* sockAddr);
int SOGetSockName(int s, void* sockAddr);
int SORecvFrom(int s, void* buf, int len, int flags, void* sockFrom);
int SORecv(int s, void* buf, int len, int flags);
int SOSendTo(int s, const void* buf, int len, int flags, const void* sockTo);
int SOSend(int s, const void* buf, int len, int flags);
int SOFcntl(int s, int cmd, ...);
int SOShutdown(int s, int how);
int SOPoll(SOPollFD fds[], unsigned nfds, s64 timeout);
int SOInetAtoN(const char* cp, SOInAddr* inp);
char* SOInetNtoA(SOInAddr in);
u32 SONtoHl(u32 netlong);
u16 SONtoHs(u16 netshort);
u32 SOHtoNl(u32 hostlong);
u16 SOHtoNs(u16 hostshort);
SOHostEnt* SOGetHostByName(const char* name);
int SOSetSockOpt(int s, int level, int optname, const void* optval, int optlen);

#define SO_F_GETFL 3
#define SO_F_SETFL 4
#define SO_O_NONBLOCK 0x04

#define SO_SO_REUSEADDR 0x0004
#define SO_SO_SNDBUF 0x1001
#define SO_SO_RCVBUF 0x1002


#define SO_CONFIG_IP_ADDR_NUMBER 0x4002
#define SO_CONFIG_IP_ADDR_TABLE 0x4003

#define SO_IP4_ALEN 4

typedef struct IPAddrEntry {
    u8 addr[4];
    u8 netmask[4];
    u8 bcastAddr[4];
} IPAddrEntry;

#endif
