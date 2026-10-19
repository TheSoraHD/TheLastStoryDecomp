/*
Common GameSpy declarations used by the GT2 sources.

This header is temporary: it collects the parts of the GameSpy common headers
(gsPlatform.h, gsPlatformSocket.h, gsPlatformUtil.h, gsMemory.h, darray.h,
hashtable.h) that GT2 needs, so the GT2 units can be built before the common
units are decompiled. It will be merged into the common headers later.

Functions whose target symbol still has an automatic name are mapped with a
macro to their current symbols.txt name.
*/

#ifndef _GT2_COMMON_DECLS_H_
#define _GT2_COMMON_DECLS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <revolution/SO.h>

#ifdef __cplusplus
extern "C" {
#endif

/* inferred: the MSL used for GameSpy expands abs() to the __abs intrinsic */
#undef abs
#define abs(x) __abs(x)

/* gsPlatform.h */
#define GSI_BIG_ENDIAN
#define GS_STATIC_CALLBACK
#define GSI_UNUSED(x) (void)(x)

#ifndef assert
#define assert(x) ((void)0)
#endif

typedef char gsi_i8;
typedef unsigned char gsi_u8;
typedef short gsi_i16;
typedef unsigned short gsi_u16;
typedef int gsi_i32;
typedef unsigned int gsi_u32;
typedef unsigned int gsi_time;
typedef int gsi_bool;
#define gsi_false ((gsi_bool)0)
#define gsi_true ((gsi_bool)1)

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#define GSI_MIN(a, b) (((a) < (b) ? (a) : (b)))
#define GSI_MAX(a, b) (((a) > (b) ? (a) : (b)))

/* gsPlatformSocket.h */
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif
#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif
#define gsiSocketIsError(theReturnValue) ((theReturnValue) == -1)
#define gsiSocketIsNotError(theReturnValue) ((theReturnValue) != -1)

#define WSAEWOULDBLOCK SO_EWOULDBLOCK
#define WSAEINPROGRESS SO_EINPROGRESS
#define WSAEALREADY SO_EALREADY
#define WSAEMSGSIZE SO_EMSGSIZE
#define WSAENETRESET SO_ENETRESET
#define WSAECONNABORTED SO_ECONNABORTED
#define WSAECONNRESET SO_ECONNRESET
#define WSAEHOSTUNREACH SO_EHOSTUNREACH
#define WSAENETUNREACH SO_ENETUNREACH
#define WSAENOBUFS SO_ENOBUFS

#define AF_INET SO_PF_INET
#define SOCK_DGRAM SO_SOCK_DGRAM
#define SOCK_STREAM SO_SOCK_STREAM
#define IPPROTO_UDP SO_IPPROTO_UDP
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

int socket(int pf, int type, int protocol);
int closesocket(SOCKET sock);
int bind(SOCKET sock, const SOCKADDR* addr, int len);
int recvfrom(SOCKET sock, char* buf, int len, int flags, SOCKADDR* addr,
             int* fromlen);
int sendto(SOCKET sock, const char* buf, int len, int flags,
           const SOCKADDR* addr, int tolen);
int setsockopt(SOCKET sock, int level, int optname, const char* optval,
               int optlen);
int getsockname(SOCKET sock, SOCKADDR* addr, int* len);

#define gethostbyaddr(a, l, t) SOGetHostByAddr(a, l, t)
SOHostEnt* fn_806D8060(const char* name);
#define gethostbyname(name) fn_806D8060(name)

/* SO byte order functions (SONtoHl, SONtoHs, SOHtoNl, SOHtoNs) */
u32 fn_806A4260(u32 netlong);
u16 fn_806A4264(u16 netshort);
u32 fn_806A426C(u32 hostlong);
u16 fn_806A4270(u16 hostshort);
#define htonl(l) fn_806A426C((u32)l)
#define ntohl(l) fn_806A4260((u32)l)
#define htons(s) fn_806A4270((u16)s)
#define ntohs(s) fn_806A4264((u16)s)
char* fn_806A420C(SOInAddr in); /* SOInetNtoA */
#define inet_ntoa(n) fn_806A420C(n)
unsigned long inet_addr(const char* name);

int fn_806D7F20(SOCKET sock);
#define GOAGetLastError(sock) fn_806D7F20(sock)

int SetSockBlocking(SOCKET sock, int isblocking);
int SetReceiveBufferSize(SOCKET sock, int size);
int CanReceiveOnSocket(SOCKET sock);
int CanSendOnSocket(SOCKET sock);
HOSTENT* getlocalhost(void);
int IsPrivateIP(IN_ADDR* addr);

/* gsPlatformUtil.h */
gsi_time current_time(void);
void Util_RandSeed(unsigned long seed);
int Util_RandInt(int low, int high);

/* gsMemory.h */
void* gsimalloc(size_t size);
void* gsirealloc(void* ptr, size_t size);
void gsifree(void* ptr);

/* darray.h */
typedef struct DArrayImplementation* DArray;
typedef int (*ArrayCompareFn)(const void* elem1, const void* elem2);
typedef void (*ArrayMapFn)(void* elem, void* clientData);
typedef int (*ArrayMapFn2)(void* elem, void* clientData);
typedef void (*ArrayElementFreeFn)(void* elem);
#define NOT_FOUND -1

DArray ArrayNew(int elemSize, int numElemsToAllocate,
                ArrayElementFreeFn elemFreeFn);
void ArrayFree(DArray array);
int ArrayLength(const DArray array);
void* ArrayNth(DArray array, int n);
void ArrayAppend(DArray array, const void* newElem);
void ArrayInsertAt(DArray array, const void* newElem, int n);
void ArrayInsertSorted(DArray array, const void* newElem,
                       ArrayCompareFn comparator);
void ArrayDeleteAt(DArray array, int n);
void ArrayRemoveAt(DArray array, int n);
void ArrayReplaceAt(DArray array, const void* newElem, int n);
void ArraySort(DArray array, ArrayCompareFn comparator);
int ArraySearch(DArray array, const void* key, ArrayCompareFn comparator,
                int fromIndex, int isSorted);
void ArrayMap(DArray array, ArrayMapFn fn, void* clientData);
void ArrayMapBackwards(DArray array, ArrayMapFn fn, void* clientData);
void* ArrayMap2(DArray array, ArrayMapFn2 fn, void* clientData);
void* ArrayMapBackwards2(DArray array, ArrayMapFn2 fn, void* clientData);
void ArrayClear(DArray array);

/* hashtable.h */
typedef struct HashImplementation* HashTable;
typedef int (*TableHashFn)(const void* elem, int numBuckets);
typedef int (*TableCompareFn)(const void* elem1, const void* elem2);
typedef void (*TableMapFn)(void* elem, void* clientData);
typedef int (*TableMapFn2)(void* elem, void* clientData);
typedef void (*TableElementFreeFn)(void* elem);

HashTable TableNew(int elemSize, int nBuckets, TableHashFn hashFn,
                   TableCompareFn compFn, TableElementFreeFn freeFn);
HashTable TableNew2(int elemSize, int nBuckets, int nChains, TableHashFn hashFn,
                    TableCompareFn compFn, TableElementFreeFn freeFn);
void TableFree(HashTable table);
int TableCount(HashTable table);
void TableEnter(HashTable table, const void* newElem);
int TableRemove(HashTable table, const void* delElem);
void* TableLookup(HashTable table, const void* elemKey);
void TableMap(HashTable table, TableMapFn fn, void* clientData);
void TableMapSafe(HashTable table, TableMapFn fn, void* clientData);
void* TableMap2(HashTable table, TableMapFn2 fn, void* clientData);
void* TableMapSafe2(HashTable table, TableMapFn2 fn, void* clientData);

#ifdef __cplusplus
}
#endif

#endif // _GT2_COMMON_DECLS_H_
