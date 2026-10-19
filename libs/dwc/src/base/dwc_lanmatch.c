// Rewritten from the target assembly (DWC release build Aug 27 2010).
// LAN matchmaking for DWC: GameSpy QR2/ServerBrowsing find the other consoles on the LAN and
// GT2 connects them in a full mesh. Structure and field names are inferred from the code.
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>
#include <revolution/SO.h>

#include <string.h>

// inferred: GameSpy types (gt2.h, sb_serverbrowsing.h, qr2.h)
typedef void* GT2Socket;
typedef void* GT2Connection;
typedef void* ServerBrowser;
typedef void* SBServer;
typedef int GT2Result;

typedef struct GT2ConnectionCallbacks {
    void* connected; // 0x00
    void* received;  // 0x04
    void* closed;    // 0x08
    void* ping;      // 0x0C
} GT2ConnectionCallbacks;

// inferred: GameSpy entry points; names from the GameSpy SDK, no source yet
const char* gt2AddressToString(u32 ip, u16 port, char* string);
GT2Result gt2CreateSocket(GT2Socket* socket, const char* localAddress, int outgoingBufferSize,
                      int incomingBufferSize, void* errorCallback);
void gt2CloseSocket(GT2Socket socket);
void gt2Think(GT2Socket socket);
void gt2Listen(GT2Socket socket, void* callback);
void gt2Accept(GT2Connection connection, GT2ConnectionCallbacks* callbacks);
void gt2Reject(GT2Connection connection, const void* message, int len);
GT2Result gt2Connect(GT2Socket socket, GT2Connection* connection, const char* remoteAddress,
                      const void* message, int len, int timeout,
                      GT2ConnectionCallbacks* callbacks, int blocking);
GT2Result gt2Send(GT2Connection connection, const void* message, int len,
                      int reliable);
void gt2CloseConnectionHard(GT2Connection connection);
void gt2CloseAllConnections(GT2Socket socket);
u32 gt2GetRemoteIP(GT2Connection connection);
int qr2_initA(void* qrec, const char* ip, int baseport, const char* gamename,
                const char* secretKey, int isPublic, int natNegotiate, void* serverKeyCallback,
                void* playerKeyCallback, void* teamKeyCallback, void* keyListCallback,
                void* countCallback, void* addErrorCallback, void* userdata); // qr2_init
void qr2_think(void* qrec);                                           // qr2_think
void qr2_shutdown(void* qrec);                                           // qr2_shutdown
ServerBrowser ServerBrowserNewA(const char* queryForGamename, const char* queryFromGamename,
                          const char* queryFromKey, int queryFromVersion,
                          int maxConcurrentUpdates, int queryVersion, int lanBrowse,
                          void* callback, void* instance);              // ServerBrowserNew
void ServerBrowserFree(ServerBrowser sb);                                     // ServerBrowserFree
void ServerBrowserLANUpdate(ServerBrowser sb, int async, u16 startPort, u16 endPort); // ServerBrowserLANUpdate
void ServerBrowserThink(ServerBrowser sb);                                     // ServerBrowserThink
SBServer ServerBrowserGetServer(ServerBrowser sb, int index);                      // ServerBrowserGetServer
int ServerBrowserCount(ServerBrowser sb);                                      // ServerBrowserCount
u32 SBServerGetPublicInetAddress(SBServer server);                                       // SBServerGetPublicInetAddress

#define DWC_LAN_MAX_PLAYERS 16 // inferred
#define DWC_LAN_GT2_PORT 12345 // inferred
#define DWC_LAN_QR2_PORT 11111 // inferred

// inferred: one GT2 connection slot
typedef struct DWCLanConnection {
    GT2Connection conn; // 0x00
    int index;          // 0x04 player index, -1 when free
} DWCLanConnection;

// inferred: GT2 state
typedef struct DWCLanGT2 {
    GT2Socket socket;                                 // 0x00
    GT2ConnectionCallbacks callbacks;                 // 0x04
    DWCLanConnection conns[DWC_LAN_MAX_PLAYERS];      // 0x14
    int connIndex[DWC_LAN_MAX_PLAYERS];               // 0x94 slot of each player, -1 if none
    int numConnected;                                 // 0xD4
} DWCLanGT2;

// inferred: server browsing state
typedef struct DWCLanSB {
    ServerBrowser handle;            // 0x00
    int updated;                     // 0x04
    int active;                      // 0x08
    u32 addrs[DWC_LAN_MAX_PLAYERS];  // 0x0C
} DWCLanSB;

// inferred: message sent by the host to the other consoles
typedef struct DWCLanMessage {
    int type;                        // 0x00
    u32 addrs[DWC_LAN_MAX_PLAYERS];  // 0x04
} DWCLanMessage;

// inferred: one entry of the player table
typedef struct DWCLanPlayer {
    u32 addr;     // 0x00
    int complete; // 0x04
} DWCLanPlayer;

typedef void (*DWCLanStateCallback)(int state);                          // inferred
typedef void (*DWCLanRecvCallback)(int index, void* data, int size);     // inferred
typedef void (*DWCLanSendCallback)(int size, int index);                 // inferred

// inferred: control block (0x210 bytes, allocated by fn_806A8200)
typedef struct DWCLanControl {
    int state;                                 // 0x000
    int unk04;                                 // 0x004
    DWCLanSB sb;                               // 0x008
    DWCLanGT2 gt2;                             // 0x054
    DWCLanMessage msg;                         // 0x12C
    u32 mAddr;                                 // 0x170
    int myIndex;                               // 0x174
    int numPlayers;                            // 0x178
    DWCLanPlayer players[DWC_LAN_MAX_PLAYERS]; // 0x17C
    DWCLanStateCallback stateCallback;         // 0x1FC
    DWCLanRecvCallback recvCallback;           // 0x200
    DWCLanSendCallback sendCallback;           // 0x204
    s64 startTime;                             // 0x208
} DWCLanControl;

// inferred: GameSpy test game name and secret key
static const char* const sGameName = "dwctest";
static const char* const sSecretKey = "d4q9GZ";

static DWCLanControl* spCntl; // name from the "spCntl->mAddr" debug string
static DWCLanGT2* spGT2;      // inferred
static DWCLanSB* spSB;        // inferred

void fn_806A8DB0(void);
BOOL fn_806A8E80(int index, const void* data, int size, BOOL reliable);
void fn_806A9000(u32 addr);
static void DWCi_LanSBCallback(ServerBrowser sb, int reason, SBServer server, void* instance);
static void DWCi_LanServerKeyCallback(int keyid, void* outbuf, void* userdata);
static void DWCi_LanPlayerKeyCallback(int keyid, int index, void* outbuf, void* userdata);
static void DWCi_LanTeamKeyCallback(int keyid, int index, void* outbuf, void* userdata);
static void DWCi_LanKeyListCallback(int keytype, void* keybuffer, void* userdata);
static int DWCi_LanCountCallback(int keytype, void* userdata);
static void DWCi_LanAddErrorCallback(int error, char* errmsg, void* userdata);
static void DWCi_LanQuickSort(u32* v, int left, int right);

static GT2Connection* DWCi_LanGetConnection(int index) {
    int slot = spGT2->connIndex[index];
    if (slot == -1) {
        return NULL;
    }
    return &spGT2->conns[slot].conn;
}

static int DWCi_LanGetIndex(GT2Connection connection) {
    int i;
    for (i = 0; i < DWC_LAN_MAX_PLAYERS; i++) {
        if (connection == spGT2->conns[i].conn) {
            return spGT2->conns[i].index;
        }
    }
    return -1;
}

static GT2Connection* DWCi_LanAddConnection(int index) {
    int i;
    for (i = 0; spGT2->conns[i].conn != NULL && i < DWC_LAN_MAX_PLAYERS; i++) {
    }
    if (i < DWC_LAN_MAX_PLAYERS) {
        spGT2->connIndex[index] = i;
        spGT2->conns[i].index = index;
        return &spGT2->conns[i].conn;
    }
    return NULL;
}

void fn_806A8200(void) {
    spCntl = (DWCLanControl*)DWC_Alloc(DWC_ALLOCTYPE_LANMATCH, sizeof(DWCLanControl));
    memset(spCntl, 0, sizeof(DWCLanControl));
    spCntl->state = 0;
    spCntl->myIndex = -1;
    spCntl->startTime = OSGetTime();
}

void fn_806A8270(DWCLanRecvCallback callback) {
    spCntl->recvCallback = callback;
}

void fn_806A8280(DWCLanSendCallback callback) {
    spCntl->sendCallback = callback;
}

// NONMATCHING: 90.32% - the inlined DWCi_LanQuickSort keeps the same register/CSE difference as
// the standalone function (see below), which shifts the saved registers of the whole function
// (target saves r24-r31, we save r26-r31). The rest of the control flow matches.
void fn_806A8290(void) {
    int i;
    int num;
    int count;
    int myIndex;
    u32 addr;

    if (spCntl == NULL) {
        return;
    }

    if (spCntl->state > 0) {
        gt2Think(spGT2->socket);
        if (spCntl->state < 8 && spSB->handle != NULL) {
            if (spSB->active) {
                ServerBrowserThink(spSB->handle);
                if (spSB->updated == 1) {
                    spSB->updated = 0;
                    ServerBrowserLANUpdate(spSB->handle, 1, DWC_LAN_QR2_PORT, DWC_LAN_QR2_PORT);
                }
            }
            qr2_think(NULL);
        }
    }

    switch (spCntl->state) {
    case 0:
        spCntl->mAddr = SOGetHostID();
        if (spCntl->mAddr != 0) {
            DWC_Printf(DWC_REPORTFLAG_INFO, "spCntl->mAddr = %s\n",
                       gt2AddressToString(spCntl->mAddr, 0, NULL));
            spCntl->state = 1;
            fn_806A9000(spCntl->mAddr);
            spSB = &spCntl->sb;
            spSB->updated = 1;
            qr2_initA(NULL, NULL, DWC_LAN_QR2_PORT, sGameName, sSecretKey, 0, 0,
                        DWCi_LanServerKeyCallback, DWCi_LanPlayerKeyCallback,
                        DWCi_LanTeamKeyCallback, DWCi_LanKeyListCallback, DWCi_LanCountCallback,
                        DWCi_LanAddErrorCallback, NULL);
            spSB->handle = ServerBrowserNewA(sGameName, sGameName, sSecretKey, 0, 10, 1, 1,
                                       DWCi_LanSBCallback, NULL);
            spSB->active = 1;
        } else {
            s64 now = OSGetTime();
            if (now - spCntl->startTime > OS_MSEC_TO_TICKS(10000ULL)) {
                spCntl->stateCallback(6);
                fn_806A8DB0();
            }
        }
        break;

    case 1:
        count = ServerBrowserCount(spSB->handle);
        num = spCntl->numPlayers;
        if (count < num) {
            break;
        }
        spSB->active = 0;
        for (i = 0; i < num; i++) {
            spSB->addrs[i] = SBServerGetPublicInetAddress(ServerBrowserGetServer(spSB->handle, i));
        }
        DWCi_LanQuickSort(spSB->addrs, 0, num - 1);
        addr = spSB->addrs[0];
        if (addr != spCntl->mAddr) {
            spCntl->state = 5;
            gt2Connect(spGT2->socket, DWCi_LanAddConnection(0),
                        gt2AddressToString(addr, DWC_LAN_GT2_PORT, NULL), NULL, 0, 10000,
                        &spGT2->callbacks, 0);
        }
        break;

    case 3:
        myIndex = spCntl->myIndex;
        if (myIndex == -1) {
            break;
        }
        for (i = spCntl->numPlayers - 1; i > 0; i--) {
            if (i == spCntl->myIndex) {
                continue;
            }
            if (DWCi_LanGetConnection(i) != NULL) {
                continue;
            }
            if (i > myIndex) {
                spCntl->state = 6;
                addr = spCntl->players[i].addr;
                gt2Connect(spGT2->socket, DWCi_LanAddConnection(i),
                            gt2AddressToString(addr, DWC_LAN_GT2_PORT, NULL), NULL, 0, 10000,
                            &spGT2->callbacks, 0);
            }
            break;
        }
        if (i == 0) {
            spCntl->state = 7;
            spCntl->msg.type = 1;
            fn_806A8E80(0, &spCntl->msg, sizeof(DWCLanMessage), TRUE);
        }
        break;

    case 4:
        for (i = 1; i < spCntl->numPlayers; i++) {
            if (spCntl->players[i].complete == 0) {
                break;
            }
        }
        if (i == spCntl->numPlayers) {
            spCntl->msg.type = 2;
            for (i = 1; i < spCntl->numPlayers; i++) {
                fn_806A8E80(i, &spCntl->msg, sizeof(DWCLanMessage), TRUE);
            }
            spCntl->state = 8;
            spCntl->stateCallback(0);
        }
        break;
    }
}

BOOL fn_806A8D90(int numPlayers, DWCLanStateCallback callback) {
    spCntl->numPlayers = numPlayers;
    spCntl->stateCallback = callback;
    return TRUE;
}

void fn_806A8DB0(void) {
    qr2_shutdown(NULL);
    ServerBrowserFree(spSB->handle);
    spSB = NULL;
    gt2CloseAllConnections(spGT2->socket);
    gt2CloseSocket(spGT2->socket);
    spGT2 = NULL;
    if (spCntl != NULL) {
        DWC_Free(DWC_ALLOCTYPE_LANMATCH, spCntl, 0);
        spCntl = NULL;
    }
}

int fn_806A8E40(void) {
    if (spCntl->state < 8 || spCntl->myIndex < 0) {
        return -1;
    }
    return spCntl->myIndex;
}

int fn_806A8E70(void) {
    return spGT2->numConnected;
}

BOOL fn_806A8E80(int index, const void* data, int size, BOOL reliable) {
    gt2Send(*DWCi_LanGetConnection(index), data, size, reliable);
    if (spCntl->state == 8) {
        spCntl->sendCallback(size, index);
    }
    return TRUE;
}

static void DWCi_LanSocketErrorCallback(GT2Socket socket) {
    OSPanic("dwc_lanmatch.c", 471, "cn_sockerror\n"); // inferred: __FILE__/__LINE__ of the original
}

static void DWCi_LanSBCallback(ServerBrowser sb, int reason, SBServer server, void* instance) {
    DWC_Printf(DWC_REPORTFLAG_INFO, "CBServerBrowsing:(%d)\n", reason);
    if (reason == 4) {
        spCntl->sb.updated = 1;
    }
}

static void DWCi_LanServerKeyCallback(int keyid, void* outbuf, void* userdata) {
}

static void DWCi_LanPlayerKeyCallback(int keyid, int index, void* outbuf, void* userdata) {
}

static void DWCi_LanTeamKeyCallback(int keyid, int index, void* outbuf, void* userdata) {
}

static void DWCi_LanKeyListCallback(int keytype, void* keybuffer, void* userdata) {
}

static int DWCi_LanCountCallback(int keytype, void* userdata) {
    return 0;
}

static void DWCi_LanAddErrorCallback(int error, char* errmsg, void* userdata) {
}

static void DWCi_LanConnectedCallback(GT2Connection connection, GT2Result result, void* message,
                                      int len);
static void DWCi_LanReceivedCallback(GT2Connection connection, void* message, int len,
                                     int reliable);
static void DWCi_LanClosedCallback(GT2Connection connection, int reason);
static void DWCi_LanConnectAttemptCallback(GT2Socket socket, GT2Connection connection, u32 ip,
                                           u16 port, int latency, void* message, int len);

void fn_806A9000(u32 addr) {
    int i;

    spGT2 = &spCntl->gt2;
    spGT2->numConnected = 1;
    memset(spGT2->connIndex, -1, sizeof(spGT2->connIndex));
    // inferred: written out; with a loop this function is small enough to be inlined into
    // fn_806A8290, which the target does not do
    spGT2->conns[0].index = -1;
    spGT2->conns[1].index = -1;
    spGT2->conns[2].index = -1;
    spGT2->conns[3].index = -1;
    spGT2->conns[4].index = -1;
    spGT2->conns[5].index = -1;
    spGT2->conns[6].index = -1;
    spGT2->conns[7].index = -1;
    spGT2->conns[8].index = -1;
    spGT2->conns[9].index = -1;
    spGT2->conns[10].index = -1;
    spGT2->conns[11].index = -1;
    spGT2->conns[12].index = -1;
    spGT2->conns[13].index = -1;
    spGT2->conns[14].index = -1;
    spGT2->conns[15].index = -1;
    memset(&spGT2->callbacks, 0, sizeof(GT2ConnectionCallbacks));
    spGT2->callbacks.connected = DWCi_LanConnectedCallback;
    spGT2->callbacks.received = DWCi_LanReceivedCallback;
    spGT2->callbacks.closed = DWCi_LanClosedCallback;
    gt2CreateSocket(&spGT2->socket, gt2AddressToString(addr, DWC_LAN_GT2_PORT, NULL), 0, 0,
                DWCi_LanSocketErrorCallback);
    gt2Listen(spGT2->socket, DWCi_LanConnectAttemptCallback);
}

static void DWCi_LanConnectedCallback(GT2Connection connection, GT2Result result, void* message,
                                      int len) {
    if (result == 0) {
        spCntl->state = 3;
    } else {
        int index;
        int slot;

        spCntl->state = 1;
        index = DWCi_LanGetIndex(connection);
        slot = spGT2->connIndex[index];
        if (slot != -1) {
            spGT2->connIndex[index] = -1;
            spGT2->conns[slot].index = -1;
            gt2CloseConnectionHard(spGT2->conns[slot].conn);
        }
    }

    DWC_Printf(DWC_REPORTFLAG_INFO, "connect to %s (%d)\n",
               gt2AddressToString(gt2GetRemoteIP(connection), 0, NULL), result);
}

static void DWCi_LanReceivedCallback(GT2Connection connection, void* message, int len,
                                     int reliable) {
    int i = 0;
    DWCLanMessage* msg;
    int index;
    u32 addr;

    switch (spCntl->state) {
    case 8:
        spCntl->recvCallback(DWCi_LanGetIndex(connection), message, len);
        break;
    case 3:
        msg = (DWCLanMessage*)message;
        for (; (addr = msg->addrs[i]) != 0; i++) {
            spCntl->players[i].addr = addr;
            if (addr == spCntl->mAddr) {
                spCntl->myIndex = i;
            } else {
                spGT2->numConnected++;
            }
        }
        break;
    case 4:
        index = DWCi_LanGetIndex(connection);
        spCntl->players[index].complete = 1;
        DWC_Printf(DWC_REPORTFLAG_INFO, "%d complete\n", index);
        break;
    case 7:
        spCntl->state = 8;
        spCntl->stateCallback(0);
        break;
    }
}

static void DWCi_LanClosedCallback(GT2Connection connection, int reason) {
    DWC_Printf(DWC_REPORTFLAG_INFO, "cn_closed (%d)\n", reason);
}

// NONMATCHING: 99.13% - volatile register choice at the start (target loads spCntl into r3 and
// the state into r5, we use r7/r3) and in the inlined send loop. Tried: local for the state or
// spCntl, other declaration orders, a block-local index for the 3/6/7 case, other parameter
// types/counts.
static void DWCi_LanConnectAttemptCallback(GT2Socket socket, GT2Connection connection, u32 ip,
                                           u16 port, int latency, void* message, int len) {
    BOOL accepted = FALSE;
    int i;
    int index;
    u32 addr;

    switch (spCntl->state) {
    case 1:
        spCntl->state = 2;
        // fallthrough
    case 2:
        if (spGT2->numConnected >= spCntl->numPlayers) {
            break;
        }
        index = spGT2->numConnected++;
        *DWCi_LanAddConnection(index) = connection;
        gt2Accept(connection, &spGT2->callbacks);
        accepted = TRUE;
        if (spGT2->numConnected == spCntl->numPlayers) {
            spCntl->state = 4;
            spCntl->myIndex = 0;
            spCntl->msg.type = 0;
            memset(spCntl->msg.addrs, 0, sizeof(spCntl->msg.addrs));
            spCntl->msg.addrs[0] = spCntl->mAddr;
            for (i = 1; i < spGT2->numConnected; i++) {
                spCntl->msg.addrs[i] = gt2GetRemoteIP(*DWCi_LanGetConnection(i));
            }
            for (i = 1; i < spGT2->numConnected; i++) {
                fn_806A8E80(i, &spCntl->msg, sizeof(DWCLanMessage), TRUE);
            }
        }
        break;
    case 3:
    case 6:
    case 7:
        {
            int j = 0;
            u32 addr2 = gt2GetRemoteIP(connection);
            for (; spCntl->players[j].addr != 0; j++) {
                if (addr2 == spCntl->players[j].addr) {
                    break;
                }
            }
            *DWCi_LanAddConnection(j) = connection;
        }
        gt2Accept(connection, &spGT2->callbacks);
        accepted = TRUE;
        break;
    default:
        gt2Reject(connection, NULL, 0);
        break;
    }

    DWC_Printf(DWC_REPORTFLAG_INFO, "%s %s (%d)\n", accepted ? "accept" : "reject",
               gt2AddressToString(ip, port, NULL), spCntl->state);
}

static void DWCi_LanSwap(u32* v, int i, int j) {
    u32 tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

// NONMATCHING: 83.77% - same instructions except that the target keeps last * 4 of each inlined
// level in a saved register and addresses v[last + 1] of the next level as (v + last * 4)[1],
// while we recompute (last + 1) * 4 (two fewer saved registers). Tried: K&R swap(v, ++last, i),
// swap through pointers, swap written out, if (left < right) form, other declaration orders,
// static inline / inline helpers.
static void DWCi_LanQuickSort(u32* v, int left, int right) {
    int i;
    int last;

    if (left >= right) {
        return;
    }

    DWCi_LanSwap(v, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++) {
        if (v[i] < v[left]) {
            DWCi_LanSwap(v, i, ++last);
        }
    }
    DWCi_LanSwap(v, left, last);
    DWCi_LanQuickSort(v, left, last - 1);
    DWCi_LanQuickSort(v, last + 1, right);
}
