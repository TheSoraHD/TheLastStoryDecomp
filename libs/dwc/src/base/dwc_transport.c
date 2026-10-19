// Rewritten from the target assembly (DWC release build Aug 27 2010).
// GT2 based data transport between the matched consoles: reliable data is split by
// sendSplitMax and sent behind a "DT" header, unreliable data can be relayed by the server.
// Function names come from the DWC API and from older DWC builds.
// inferred: file name; the range 0x806CD540-0x806CF02C holds three translation units
// (own .data/.bss each, calls between them never inlined).
#include <dwc/dwc_transport.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <string.h>

// inferred: GameSpy types (gt2.h)
typedef void* GT2Connection;
typedef int GT2Result;
typedef int GT2Bool;
typedef GT2Result (*DWCGT2SendFunc)(GT2Connection connection, const void* message, int len,
                                    GT2Bool reliable);

// inferred: GameSpy entry points; names from the GameSpy SDK, no source yet
GT2Result gt2Send(GT2Connection connection, const void* message, int len,
                      GT2Bool reliable);
void gt2Ping(GT2Connection connection);
int gt2GetConnectionState(GT2Connection connection);
int gt2GetIncomingBufferFreeSpace(GT2Connection connection);
int gt2GetOutgoingBufferFreeSpace(GT2Connection connection);

// inferred: DWC matching functions (other units); names from older DWC builds where known
void fn_806A9CA0(void* dst, const void* src, u32 size); // DWCi_Np_CPUCopy
u8 fn_806B0E30(void);                                // DWC_GetMyAID
int fn_806B0EB0(u8** aidList);                       // DWC_GetAIDList
BOOL fn_806B1170(u8 aid);                            // DWC_IsValidAID
BOOL fn_806B12F0(void);                              // inferred: TRUE on the match server
GT2Connection fn_806B14D0(u8 aid);                   // DWCi_GetGT2Connection
u8 fn_806B1570(GT2Connection connection);            // DWCi_GetConnectionAID
BOOL fn_806B1730(u8 aid);                            // inferred: aid has a GT2 connection
void fn_806B1A00(u8 aid);                            // inferred: closes the connection to aid
u8 fn_806B30A0(void);                                // inferred: aid of the server, 0xFF if none
void fn_806B97B0(u8 aid, u16 type, const u8* message); // inferred: system message handler
void fn_806C9CE0(const u8* message, u32 size);       // inferred: unreliable match command
BOOL fn_806CB4C0(void);
void fn_806CB8F0(int reason);
BOOL fn_806CCB40(void);
int fn_806CCCB0(void);                               // inferred: number of hosts
// inferred: connection info of the server (only the aid at 0x16 is read here)
typedef struct DWCConnectionInfo {
    u8 pad[0x16];
    u8 aid; // 0x16
} DWCConnectionInfo;
DWCConnectionInfo* fn_806CCED0(int index);

#define DWC_ERROR_NETWORK ((DWCError)6) // inferred: NitroDWC value

// inferred: header in front of reliable data (little endian fields, as on the DS)
typedef struct DWCTransportHeader {
    int size;      // 0x00
    u16 type;      // 0x04
    char magic[2]; // 0x06 "DT"
} DWCTransportHeader;

// inferred: header in front of unreliable data relayed by the server
typedef struct DWCRelayHeader {
    char magic[3]; // 0x00
    u8 aid;        // 0x03 sender
    u32 aidBitmap; // 0x04 receivers
} DWCRelayHeader;

// inferred: GT2 reliable messages that could not be sent yet
typedef struct DWCReliableQueueEntry {
    GT2Connection connection;            // 0x00
    u8* data;                            // 0x04
    int size;                            // 0x08
    struct DWCReliableQueueEntry* next;  // 0x0C
} DWCReliableQueueEntry;

typedef struct DWCReliableQueue {
    DWCReliableQueueEntry* head; // 0x00
    int totalSize;               // 0x04
} DWCReliableQueue;

#define DWC_TRANSPORT_TYPE_DATA 1 // inferred

// inferred: send/recv states
enum {
    DWC_SEND_STATE_READY = 0,
    DWC_SEND_STATE_BUSY = 1
};

enum {
    DWC_RECV_STATE_NOBUF = 0,
    DWC_RECV_STATE_HEADER = 1,
    DWC_RECV_STATE_BODY = 2,
    DWC_RECV_STATE_SYSTEM = 3,
    DWC_RECV_STATE_ERROR = 4
};

// inferred: little endian conversion of the header fields (the DS byte order)
#define SWAP16(x) ((u16)(((u16)(x) >> 8) | ((u16)(x) << 8)))
static inline u32 SWAP32(u32 x) {
    x = ((x >> 8) & 0x00FF00FF) | ((x << 8) & 0xFF00FF00);
    return (x << 16) | (x >> 16);
}

// inferred: the reliable queue, the "DT" transport and DWCs_RecvDataBody each have their own
// .data and .bss in the target (see the notes at the parts below)
void fn_806CD540(void);
void fn_806CD560(void);
BOOL fn_806CD5E0(void);
BOOL fn_806CD610(void);
BOOL DWCi_ProcessReliableQueue(DWCGT2SendFunc sendFunc);
BOOL fn_806CD740(GT2Connection connection, const void* data, int size);
void DWCs_RecvDataBody(u8 aid, const u8* message, int len);

extern DWCTransportInfo* stpTransInfo;


DWCTransportInfo* stpTransInfo; // inferred: name

// inferred: sends now if nothing is queued, otherwise queues the message
static inline void DWCi_SendReliableData(GT2Connection connection, const void* data, int size) {
    if (fn_806CD610()) {
        GT2Result result = gt2Send(connection, data, size, TRUE);
        if (result == 0) {
            return;
        }
        DWC_Printf(DWC_REPORTFLAG_SEND_INFO, "gt2Send() Failed code:%d\n", result);
    }

    if (!fn_806CD740(connection, data, size)) {
        DWCi_SetError(DWC_ERROR_NETWORK, -97630);
    }
}

// inferred: free space of the GT2 outgoing buffer, minus a reserve
static inline int DWCi_GetOutgoingFreeSpace(u8 aid) {
    int freeSpace = gt2GetOutgoingBufferFreeSpace(fn_806B14D0(aid)) - 519;
    return freeSpace > 0 ? freeSpace : 0;
}

// inferred: space needed for the first packet of each transport type
static inline int DWCi_GetHeaderSize(int type) {
    switch (type) {
    case 2:
        return 0x24;
    case 3:
        return 0xC;
    case 4: {
        int n = fn_806CCCB0() - 1;
        if (n == 0) {
            return 0x10;
        }
        return n * 0x18 + 0x10;
    }
    default:
        return sizeof(DWCTransportHeader);
    }
}

BOOL DWC_IsSendableReliable(u8 aid);
BOOL fn_806CD880(u8 aid, u32 type);

BOOL DWC_IsSendableReliable(u8 aid) {
    return fn_806CD880(aid, DWC_TRANSPORT_TYPE_DATA);
}

BOOL fn_806CD880(u8 aid, u32 type) {
    int freeSpace;

    if (DWCi_IsError() || (type == DWC_TRANSPORT_TYPE_DATA && !fn_806B1170(aid))) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "aid %d is unavailable.\n", aid);
        return FALSE;
    }

    if (!fn_806B1730(aid)) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "aid %d doesn't have GT2 connection.\n", aid);
        return FALSE;
    }

    if (stpTransInfo->connections[aid].sendState == DWC_SEND_STATE_BUSY) {
        DWC_Printf(DWC_REPORTFLAG_SEND_INFO, "+++ Cannot send to %d from %d (busy)\n", aid,
                   fn_806B0E30());
        return FALSE;
    }

    freeSpace = DWCi_GetOutgoingFreeSpace(aid);
    if (freeSpace < DWCi_GetHeaderSize(type)) {
        DWC_Printf(DWC_REPORTFLAG_SEND_INFO,
                   "+++ Cannot send to %d from %d (outgoing buffer is not enough) %d < %d\n",
                   aid, fn_806B0E30(), freeSpace, DWCi_GetHeaderSize(type));
        return FALSE;
    }

    return fn_806CD5E0() != 0;
}

BOOL DWCi_SendReliable(u32 type, u8 aid, const void* buffer, int size);

BOOL DWC_SendReliable(u8 aid, const void* buffer, int size) {
    return DWCi_SendReliable(DWC_TRANSPORT_TYPE_DATA, aid, buffer, size);
}

BOOL DWCi_SendReliable(u32 type, u8 aid, const void* buffer, int size) {
    DWCTransportConnection* connection = &stpTransInfo->connections[aid];
    DWCTransportHeader header;

    if (!fn_806CD880(aid, type)) {
        return FALSE;
    }

    connection->sendState = 1;
    connection->sendBuffer = buffer;
    connection->sendingSize = 0;
    connection->requestSendSize = size;

    strncpy(header.magic, "DT", 2);
    header.type = SWAP16(type);
    header.size = SWAP32(size);
    DWCi_SendReliableData(fn_806B14D0(aid), &header, sizeof(header));

    if (size > stpTransInfo->sendSplitMax) {
        size = stpTransInfo->sendSplitMax;
    }

    if (size > DWCi_GetOutgoingFreeSpace(aid)) {
        return TRUE;
    }

    DWCi_SendReliableData(fn_806B14D0(aid), buffer, size);

    connection->sendingSize += size;
    if (connection->sendingSize == connection->requestSendSize) {
        int sentSize = connection->requestSendSize;
        connection->sendState = 0;
        connection->sendBuffer = NULL;
        connection->sendingSize = 0;
        connection->requestSendSize = 0;

        if (stpTransInfo->sendCallback != NULL && type == DWC_TRANSPORT_TYPE_DATA) {
            stpTransInfo->sendCallback(sentSize, aid, stpTransInfo->sendParam);
        }
    }

    return TRUE;
}

void fn_806CE750(u8 srcAid, u8 dstAid, const void* buffer, int size, int relay);

BOOL DWC_SendUnreliable(u8 aid, const void* buffer, int size) {
    if (DWCi_IsError()) {
        return FALSE;
    }

    if (!fn_806B1170(aid)) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "aid %d is now unavailable.\n", aid);
        return FALSE;
    }

    if (size > 1462) {
        DWC_Printf(DWC_REPORTFLAG_SEND_INFO, "+++ SendUnreliable size is too large ( %d > %d )\n",
                   size, 1462);
        return FALSE;
    }

    fn_806CE750(fn_806B0E30(), aid, buffer, size, TRUE);

    if (stpTransInfo->sendCallback != NULL) {
        stpTransInfo->sendCallback(size, aid, stpTransInfo->sendParam);
    }

    return TRUE;
}

BOOL DWC_SetRecvBuffer(u8 aid, void* recvBuffer, int size) {
    DWCTransportConnection* connection = &stpTransInfo->connections[aid];

    if (connection->recvState == DWC_RECV_STATE_BODY) {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "+++ Cannot set recv buffer\n");
        return FALSE;
    }

    connection->recvBuffer = recvBuffer;
    connection->recvBufferSize = size;
    connection->recvState = 1;
    connection->recvingSize = 0;
    connection->requestRecvSize = 0;
    return TRUE;
}

BOOL DWC_Ping(u8 aid) {
    GT2Connection connection;

    if (DWCi_IsError()) {
        return FALSE;
    }

    connection = fn_806B14D0(aid);
    if (aid == fn_806B0E30() || connection == NULL || gt2GetConnectionState(connection) != 1) {
        DWC_Printf(DWC_REPORTFLAG_SEND_INFO, "DWC_Ping:not connected yet:%d\n", aid);
        return FALSE;
    }

    gt2Ping(connection);
    return TRUE;
}

BOOL DWC_SetUserSendCallback(DWCUserSendCallback callback, void* param) {
    if (stpTransInfo == NULL) {
        return FALSE;
    }

    stpTransInfo->sendCallback = callback;
    stpTransInfo->sendParam = param;
    return TRUE;
}

BOOL DWC_SetUserRecvCallback(DWCUserRecvCallback callback, void* param) {
    if (stpTransInfo == NULL) {
        return FALSE;
    }

    stpTransInfo->recvCallback = callback;
    stpTransInfo->recvParam = param;
    return TRUE;
}

BOOL DWC_SetUserRecvTimeoutCallback(DWCUserRecvTimeoutCallback callback, void* param) {
    if (stpTransInfo == NULL) {
        return FALSE;
    }

    stpTransInfo->recvTimeoutCallback = callback;
    stpTransInfo->recvTimeoutParam = param;
    return TRUE;
}

BOOL DWC_SetUserPingCallback(DWCUserPingCallback callback, void* param) {
    if (stpTransInfo == NULL) {
        return FALSE;
    }

    stpTransInfo->pingCallback = callback;
    stpTransInfo->pingParam = param;
    return TRUE;
}

BOOL DWC_SetRecvTimeoutTime(u8 aid, u32 time) {
    if (stpTransInfo == NULL) {
        return FALSE;
    }

    stpTransInfo->connections[aid].recvTimeoutTime = time;
    stpTransInfo->connections[aid].previousRecvTick = OSGetTime();
    return TRUE;
}

// inferred: sets the connection timeout [ms]; real name unknown
BOOL fn_806CE060(u32 time) {
    if (stpTransInfo == NULL || time == 0) {
        return FALSE;
    }

    stpTransInfo->connectionTimeout = time;
    return TRUE;
}

void DWCi_InitTransport(DWCTransportInfo* info) {
    stpTransInfo = info;
    memset(info, 0, sizeof(DWCTransportInfo));
    stpTransInfo->sendSplitMax = 1465;
    if (stpTransInfo != NULL) {
        stpTransInfo->connectionTimeout = 20000;
    }
    fn_806CD540();
}

void fn_806CE700(u8 aid);
void DWCs_HandleReliableMessage(GT2Connection connection, const u8* message, int len);
void DWCs_HandleUnreliableMessage(GT2Connection connection, const u8* message, int len);

void DWCi_RecvCallback(GT2Connection connection, const u8* message, int len, GT2Bool reliable) {
    u8 aid;

    if (stpTransInfo == NULL) {
        return;
    }

    if (message == NULL || len == 0) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "Recv NULL message %x, size = %d\n", message, len);
        return;
    }

    aid = fn_806B1570(connection);
    if (aid < DWC_MAX_CONNECTIONS && aid != fn_806B0E30()) {
        fn_806CE700(aid);
    }

    if (reliable) {
        DWCs_HandleReliableMessage(connection, message, len);
    } else {
        DWCs_HandleUnreliableMessage(connection, message, len);
    }
}

void DWCi_PingCallback(GT2Connection connection, int latency) {
    if (stpTransInfo->pingCallback != NULL) {
        u8 aid = fn_806B1570(connection);
        stpTransInfo->pingCallback(latency, aid, stpTransInfo->pingParam);
    }
}

static inline u32 DWCi_GetConnectionTimeout(void) {
    if (stpTransInfo != NULL) {
        return stpTransInfo->connectionTimeout;
    }
    return 0;
}

// inferred: calls the user timeout callback when nothing was received from aid for too long
static inline void DWCi_CheckRecvTimeout(u8 aid, s64 tick, DWCTransportConnection* connection) {

    if (stpTransInfo->recvTimeoutCallback != NULL) {
        u32 timeoutTime = connection->recvTimeoutTime;
        if (timeoutTime != 0) {
            u32 time = OS_TICKS_TO_MSEC(tick - connection->previousRecvTick);
            if (time > timeoutTime) {
                DWC_Printf(DWC_REPORTFLAG_RECV_INFO,
                           "DWCi_TransportProcess:timeout aid=%d,time=%d[ms],timeout "
                           "time=%d[ms]\n",
                           aid, time, timeoutTime);
                stpTransInfo->recvTimeoutCallback(aid, stpTransInfo->recvTimeoutParam);
                connection->previousRecvTick = tick;
            }
        }
    }
}

// inferred: sends the next part of the reliable data being sent to aid
static inline void DWCi_SendSplitData(u8 aid, DWCTransportConnection* connection) {
    int sendSize;
    int freeSpace;

    if (connection->sendState != DWC_SEND_STATE_BUSY) {
        return;
    }

    sendSize = connection->requestSendSize - connection->sendingSize;
    if (sendSize > stpTransInfo->sendSplitMax) {
        sendSize = stpTransInfo->sendSplitMax;
    }

    freeSpace = DWCi_GetOutgoingFreeSpace(aid);
    if (freeSpace < sendSize) {
        DWC_Printf(DWC_REPORTFLAG_SEND_INFO,
                   "DWCi_TransportProcess:freeSpace < sendSize:aid:%d, %d < %d\n", aid, freeSpace,
                   sendSize);
        return;
    }

    DWCi_SendReliableData(fn_806B14D0(aid), connection->sendBuffer + connection->sendingSize,
                          sendSize);

    connection->sendingSize += sendSize;
    if (connection->sendingSize == connection->requestSendSize) {
        int sentSize = connection->requestSendSize;
        connection->sendState = DWC_SEND_STATE_READY;
        connection->sendBuffer = NULL;
        connection->sendingSize = 0;
        connection->requestSendSize = 0;

        if (stpTransInfo->sendCallback != NULL) {
            stpTransInfo->sendCallback(sentSize, aid, stpTransInfo->sendParam);
        }
    }
}

// NONMATCHING: 92% (as a separate translation unit) - now/timeout get swapped saved registers
// (r26/r27 vs r27/r28/r26); tried declaration order, int/OSTime/u64 types, a ternary for the
// timeout, inline helpers for the keep-alive part
void DWCi_TransportProcess(void) {
    u8* aidList;
    int numAid;
    int i;
    s64 now;
    u32 timeout;

    if (stpTransInfo == NULL) {
        return;
    }

    numAid = fn_806B0EB0(&aidList);
    now = OSGetTime();
    timeout = DWCi_GetConnectionTimeout();

    if (fn_806B12F0() || (fn_806CCB40() && fn_806CB4C0())) {
        for (i = 0; i < numAid; i++) {
            u8 aid = aidList[i];
            if (aid != fn_806B0E30()) {
                s64 lastTick = stpTransInfo->connections[aid].lastRecvTick;
                u32 time = OS_TICKS_TO_MSEC(now - lastTick);
                if (time > timeout) {
                    DWC_Printf(DWC_REPORTFLAG_RECV_INFO,
                               "shutdown client: DWCi_TransportProcess:timeout aid=%d,time=%d[ms],"
                               "timeout time=%d[ms]\n",
                               aid, time, lastTick);
                    fn_806B1A00(aid);
                    fn_806CB8F0(1);
                }
            }
        }
    } else {
        u8 serverAid = fn_806B30A0();
        if (serverAid != 0xFF && serverAid != fn_806B0E30()) {
            s64 lastTick = stpTransInfo->connections[serverAid].lastRecvTick;
            u32 time = OS_TICKS_TO_MSEC(now - lastTick);
            if (time > timeout) {
                DWC_Printf(DWC_REPORTFLAG_RECV_INFO,
                           "shutdown server: DWCi_TransportProcess:timeout aid=%d,time=%d[ms],"
                           "timeout time=%d[ms]\n",
                           serverAid, time, lastTick);
                fn_806B1A00(serverAid);
            }
        }
    }

    for (i = 0; i < numAid; i++) {
        u8 aid = aidList[i];

        fn_806CD610();

        if (fn_806B1170(aid)) {
            s64 tick = OSGetTime();
            DWCi_CheckRecvTimeout(aid, tick, &stpTransInfo->connections[aid]);
        }

        if (aid != fn_806B0E30()) {
            DWCi_SendSplitData(aid, &stpTransInfo->connections[aid]);
        }
    }
}

void DWCi_ClearTransConnection(u8 aid) {
    if (stpTransInfo == NULL) {
        return;
    }

    stpTransInfo->connections[aid].sendingSize = 0;
    stpTransInfo->connections[aid].recvingSize = 0;
    stpTransInfo->connections[aid].requestSendSize = 0;
    stpTransInfo->connections[aid].requestRecvSize = 0;
    stpTransInfo->connections[aid].sendState = 0;
    stpTransInfo->connections[aid].recvBuffer = NULL;
    stpTransInfo->connections[aid].recvBufferSize = 0;
    stpTransInfo->connections[aid].recvState = 0;
    stpTransInfo->connections[aid].lastRecvType = 0;
}

void DWCi_ShutdownTransport(void) {
    stpTransInfo = NULL;
    fn_806CD560();
}

// inferred: refreshes the keep-alive time of aid; real name unknown
void fn_806CE700(u8 aid) {
    stpTransInfo->connections[aid].lastRecvTick = OSGetTime();
}

void fn_806CE750(u8 srcAid, u8 dstAid, const void* buffer, int size, int relay) {
    GT2Connection connection;
    DWCConnectionInfo* server;
    BOOL forward = FALSE;

    connection = fn_806B14D0(dstAid);
    if (connection == NULL) {
        if (!fn_806B12F0()) {
            server = fn_806CCED0(0);
            connection = fn_806B14D0(server->aid);
            if (connection == NULL) {
                DWC_Printf(DWC_REPORTFLAG_SEND_INFO,
                           "+++ drop relay unreliable data. (aid %d->%d)\n", srcAid, dstAid);
                DWC_Printf(DWC_REPORTFLAG_SEND_INFO,
                           "+++ sv gt2Connection is NULL.(sv aid:%d myaid:%d)\n", server->aid,
                           fn_806B0E30());
                return;
            }

            DWC_Printf(DWC_REPORTFLAG_SEND_INFO, "+++ forward unreliable. sent(aid %d->%d)\n",
                       srcAid, dstAid);
            forward = TRUE;
        } else {
            DWC_Printf(DWC_REPORTFLAG_SEND_INFO, "+++ drop relay unreliable data. (aid %d->%d)\n",
                       srcAid, dstAid);
            return;
        }
    }

    if (relay == TRUE && forward == TRUE) {
        DWCRelayHeader* header = DWC_Alloc(DWC_ALLOCTYPE_BASE, size + 8);
        if (header == NULL) {
            return;
        }

        strncpy(header->magic, "F\xFCW", 3);
        header->aid = srcAid;
        header->aidBitmap = SWAP32(1 << dstAid);
        fn_806A9CA0(header + 1, buffer, size);
        gt2Send(connection, header, size + sizeof(DWCRelayHeader), FALSE);
        if (header != NULL) {
            DWC_Free(DWC_ALLOCTYPE_BASE, header, 0);
        }
    } else {
        gt2Send(connection, buffer, size, FALSE);
    }
}

// inferred: never called in this build; the linker removed it but its string stayed
void DWCi_PrintServerUnavailable_unused(void) {
    DWC_Printf(DWC_REPORTFLAG_SEND_INFO, "server is now unavailable.\n");
}

void DWCs_RecvDataHeader(u8 aid, const u8* message, u32 len);

static inline u16 DWCi_GetHeaderType(const u8* message) {
    DWCTransportHeader header;

    fn_806A9CA0(&header, message, sizeof(header));
    if (memcmp(header.magic, "DT", 2) == 0) {
        return SWAP16(header.type);
    }
    return 0;
}

void DWCs_HandleReliableMessage(GT2Connection connection, const u8* message, int len) {
    u8 aid = fn_806B1570(connection);
    DWCTransportConnection* conn;

    if (aid == 0xFF) {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "+++ Invalid connection. has no DWCConnectionInfo\n");
        DWCi_SetError(DWC_ERROR_NETWORK, -97640);
        return;
    }

    conn = &stpTransInfo->connections[aid];
    switch ((u8)conn->recvState) { // inferred: compared unsigned here
    case 0: {
        u16 type = DWCi_GetHeaderType(message);
        if (type >= 2 && type <= 4) {
            DWCs_RecvDataHeader(aid, message, len);
        } else {
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "+++ Recv buffer is not set\n");
        }
        break;
    }
    case 1:
        DWCs_RecvDataHeader(aid, message, len);
        break;
    case 2:
        DWCs_RecvDataBody(aid, message, len);
        break;
    case 3:
        conn->recvState = conn->lastRecvState;
        switch (conn->lastRecvType) {
        case 2:
        case 3:
        case 4:
            fn_806B97B0(aid, conn->lastRecvType, message);
            break;
        }
        break;
    case 4:
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO,
                   "+++ Recv size is too large ( buffer size = %d < %d )\n", conn->recvBufferSize,
                   len);
        stpTransInfo->connections[aid].recvState = 1;
        stpTransInfo->connections[aid].recvingSize = 0;
        stpTransInfo->connections[aid].requestRecvSize = 0;
        break;
    default:
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Recv error (state is %d).\n", (u8)conn->recvState);
        DWCi_SetError(DWC_ERROR_NETWORK, -97610);
        break;
    }
}

// inferred: size of the match command header (GameSpy QR2 magic + data)
#define DWC_MATCH_COMMAND_HEADER_SIZE 0x14

// NONMATCHING: 97% (as a separate translation unit) - the copy of the aid into its saved
// register is scheduled before the 0xFF test instead of after it; tried declaration orders,
// separate assignments and a second variable
void DWCs_HandleUnreliableMessage(GT2Connection connection, const u8* message, int len) {
    DWCTransportConnection* conn;
    u8 aid = fn_806B1570(connection);
    int relayed = 0;
    BOOL forMe = FALSE;

    if (aid == 0xFF) {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "+++ Invalid connection. has no DWCConnectionInfo\n");
        DWCi_SetError(DWC_ERROR_NETWORK, -97640);
        return;
    }

    if ((u32)len >= DWC_MATCH_COMMAND_HEADER_SIZE &&
        strncmp((const char*)message, "\xBBI\xCCM", 4) == 0) {
        fn_806C9CE0(message, len);
        return;
    }

    if (len >= (int)sizeof(DWCRelayHeader) && memcmp(message, "F\xFCW", 3) == 0) {
        relayed = 1;
    }

    if (relayed == 1) {
        u32 aidBitmap = SWAP32(((DWCRelayHeader*)message)->aidBitmap);
        aid = ((DWCRelayHeader*)message)->aid;

        if (!fn_806B12F0()) {
            if (aidBitmap & (1 << fn_806B0E30())) {
                forMe = TRUE;
            } else {
                return;
            }
        } else {
            u32 i;
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO,
                       "+++ fwd received unreliable. sent(aid %d->%08x)\n", aid, aidBitmap);
            for (i = 0; i < DWC_MAX_CONNECTIONS; i++) {
                if (aidBitmap & (1 << (u8)i)) {
                    if ((u8)i == fn_806B0E30()) {
                        forMe = TRUE;
                    } else {
                        fn_806CE750(aid, i, message, len, FALSE);
                    }
                }
            }
        }

        if (!forMe) {
            return;
        }
    }

    conn = &stpTransInfo->connections[aid];
    if (relayed == 1) {
        message += sizeof(DWCRelayHeader);
        len -= sizeof(DWCRelayHeader);
    }

    if (conn->recvBuffer != NULL && conn->recvBufferSize >= len) {
        if (stpTransInfo->recvCallback != NULL) {
            stpTransInfo->recvCallback(aid, (u8*)message, len, stpTransInfo->recvParam);
        }

        if (stpTransInfo->recvTimeoutCallback != NULL && conn->recvTimeoutTime != 0) {
            conn->previousRecvTick = OSGetTime();
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Recv data size is too large (%d > %d)\n", len,
                   conn->recvBufferSize);
    }
}

void DWCs_RecvDataHeader(u8 aid, const u8* message, u32 len) {
    DWCTransportConnection* conn = &stpTransInfo->connections[aid];
    u16 type;

    conn->lastRecvState = conn->recvState;
    type = DWCi_GetHeaderType(message);

    switch (type) {
    case DWC_TRANSPORT_TYPE_DATA: {
        DWCTransportHeader header;

        if (len != sizeof(DWCTransportHeader)) {
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "+++ Invalid header from aid %d\n", aid);
            return;
        }

        fn_806A9CA0(&header, message, sizeof(header));
        header.size = SWAP32(header.size);
        header.type = SWAP16(header.type);

        conn->requestRecvSize = header.size;
        conn->recvingSize = 0;
        if (conn->recvBuffer != NULL && conn->recvBufferSize >= conn->requestRecvSize) {
            conn->recvState = DWC_RECV_STATE_BODY;
        } else {
            conn->recvState = DWC_RECV_STATE_ERROR;
        }
        break;
    }
    case 2:
    case 3:
    case 4:
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Received system header(%d).\n", type);
        conn->recvState = DWC_RECV_STATE_SYSTEM;
        break;
    default:
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "+++ Invalid header from aid %d (type:%d)\n", aid,
                   type);
        break;
    }

    conn->lastRecvType = type;
}
