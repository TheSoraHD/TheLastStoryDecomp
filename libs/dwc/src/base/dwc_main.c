// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Friends match main control: DWC_InitFriendsMatch / DWC_ProcessFriendsMatch, login, the
// matchmaking entry points and the GT2 / GP callbacks shared by all of them.
// Names follow the RVL DWC symbols of older builds (Pokemon Battle Revolution, Mario Kart Wii);
// the functions keep their fn_ADDR symbols, the real names are given in the comments.
#include <dwc/dwc_friend.h>
#include <dwc/dwc_transport.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// inferred: GameSpy types (gt2.h, gp.h)
typedef void* GT2Socket;
typedef void* GT2Connection;
typedef int GT2Result;
typedef int GT2Bool;
typedef int GT2CloseReason;

typedef struct GT2ConnectionCallbacks {
    void* connected; // 0x00
    void* received;  // 0x04
    void* closed;    // 0x08
    void* ping;      // 0x0C
} GT2ConnectionCallbacks;

typedef void (*GPCallback)(GPConnection* connection, void* arg, void* param);

typedef struct GPErrorArg {
    GPResult result;         // 0x00
    int errorCode;           // 0x04
    const char* errorString; // 0x08
    int fatal;               // 0x0C
} GPErrorArg;

typedef struct GPRecvBuddyMessageArg {
    GPProfile profile;   // 0x00
    u32 date;            // 0x04
    const char* message; // 0x08
} GPRecvBuddyMessageArg;

typedef struct GPBuddyStatus {
    GPProfile profile;        // 0x000
    GPEnum status;            // 0x004
    char statusString[256];   // 0x008
    char locationString[256]; // 0x108
    u32 ip;                   // 0x208
    int port;                 // 0x20C
    GPEnum quietModeFlags;    // 0x210
} GPBuddyStatus;

// inferred: GameSpy entry points; names from the GameSpy SDK, no source yet
GT2Result gt2CreateSocket(GT2Socket* socket, const char* localAddress, int outgoingBufferSize,
                      int incomingBufferSize, void* errorCallback);
void gt2CloseSocket(GT2Socket socket);
void gt2Listen(GT2Socket socket, void* callback);
void gt2CloseConnectionHard(GT2Connection connection);
void gt2CloseAllConnectionsHard(GT2Socket socket);
int gt2GetSocketSOCKET(GT2Socket socket);
void gt2SetUnrecognizedMessageCallback(GT2Socket socket, void* callback);
void* gt2GetConnectionData(GT2Connection connection);
const char* gt2AddressToString(u32 ip, u16 port, char* string);
int fn_806D7F20(int socket);                             // GOAGetLastError
void fn_806EF8B0(void* qrec);                            // qr2_send_statechanged
void fn_806EF930(void* qrec);                            // qr2_shutdown
void fn_806FFA10(void* sb);                              // ServerBrowserFree
void fn_806D76E0(const char* gamename);                  // GSIStartAvailableCheck
int fn_806D7860(void);                                   // GSIAvailableCheckThink
int fn_806D8060(const char* hostname); // inferred: caches the DNS query of hostname
GPResult fn_806DA880(GPConnection* connection, int productID, int namespaceID,
                     int partnerID);                                     // gpInitialize
void fn_806DA8B0(GPConnection* connection);                              // gpDestroy
GPResult fn_806DA8D0(GPConnection* connection);                          // gpDisconnect
GPResult fn_806DA910(GPConnection* connection, int func, void* callback,
                     void* param);                                       // gpSetCallback
GPResult fn_806DB0A0(GPConnection* connection, int index, GPBuddyStatus* status); // gpGetBuddyStatus
GPResult fn_806DB220(GPConnection* connection, GPProfile profile, int* index);   // gpGetBuddyIndex
int fn_806DB310(GPConnection* connection, GPProfile profile);                   // gpIsBuddy
void fn_806F87A0(void);                                                         // CloseStatsConnection

// MSL strtoul (not declared by our stdlib.h)
unsigned long strtoul(const char* str, char** end, int base);

// inferred: DWC utility functions (other units)
void fn_806A9CA0(void* dst, const void* src, u32 size); // DWCi_Np_CPUCopy
int fn_806ABA10(const char* key, char* value, const char* string, char separator); // DWC_GetCommonValueString
u32 fn_806ABB70(u32 max);                              // DWCi_GetMathRand32
u32 fn_806ABCC0(const u16* string);                    // wcslen

typedef void (*DWCLoginCallback)(DWCError error, int profileID, void* param);
typedef void (*DWCMatchedCallback)(DWCError error, BOOL cancel, BOOL self, BOOL isServer,
                                   int index, void* param);
typedef void (*DWCConnectionClosedCallback)(DWCError error, BOOL isLocal, BOOL isServer, u8 aid,
                                            int index, void* param);

// inferred: dwc_login entry points
void fn_806AE750(void* logcnt, const DWCUserData* userdata, GPConnection* pGpObj, int productID,
                 u32 gamecode, const u16* playerName, DWCLoginCallback callback, void* param);
BOOL fn_806AE910(void);                             // DWCi_LoginAsync
void fn_806AE960(void);                             // DWCi_LoginProcess
void fn_806AEB10(DWCError error, int errorCode);    // DWCi_StopLogin
void fn_806AEBB0(void);                             // DWCi_ShutdownLogin

// inferred: node info of a connected host (dwc_match.c)
typedef struct DWCNodeInfo {
    int profileID;  // 0x00
    u32 publicip;   // 0x04
    u32 privateip;  // 0x08
    u16 publicport; // 0x0C
    u16 privateport; // 0x0E
    u8 unk10[6];    // 0x10
    u8 aid;         // 0x16
    u8 unk17[0x11]; // 0x17
    u8 buf[4];      // 0x28
    u8 unk2C[4];    // 0x2C
} DWCNodeInfo;

// inferred: dwc_match entry points
void fn_806B3DF0(void* matchcnt, GPConnection* pGpObj, GT2Socket* pGt2Socket,
                 GT2ConnectionCallbacks* gt2Callbacks, const char* gameName,
                 const char* secretKey, DWCFriendData* friendList, int friendListLen);
BOOL fn_806B3160(void);
void fn_806B42B0(int a0, u8 numEntry, int a2, void* matchedCallback, void* matchedParam, int a5,
                 int a6, int a7, int a8, int a9, int a10, int a11);
void fn_806B4940(int a0, u8 maxEntry, void* matchedCallback, void* matchedParam, int a4, int a5,
                 int a6, int a7, int a8);
void fn_806B4D20(int a0, int profileID, void* matchedCallback, void* matchedParam, int a4, int a5,
                 int a6, int a7, int a8);
void fn_806B5860(int a0);
void fn_806B8430(void); // inferred: GT2 unrecognized message callback
void fn_806B85B0(void); // inferred: GT2 connect attempt callback
void fn_806B8D60(void); // inferred: GT2 connected callback
void fn_806B9410(GPConnection* connection, GPProfile profile, const char* command);
void fn_806B9500(DWCError error, int errorCode); // DWCi_StopMatching
void fn_806B9740(void);
BOOL fn_806BAE10(DWCError error, int errorCode, int profileID);
void fn_806BB3F0(int profileID);
GPResult fn_806BB440(int status);
void fn_806BB5E0(void);
BOOL fn_806BB6A0(void);
void fn_806BBCA0(void);
BOOL fn_806CB4C0(void);
void fn_806CCB70(int state);
BOOL fn_806CCC70(void);
void fn_806CCC90(void);
int fn_806CCCB0(void); // inferred: number of hosts
void fn_806CCCC0(u8 aid);
DWCNodeInfo* fn_806CCEA0(void);
DWCNodeInfo* fn_806CCED0(int index);
DWCNodeInfo* fn_806CCF00(int profileID);
DWCNodeInfo* fn_806CCF50(u8 aid);
int fn_806CD150(void);
BOOL fn_806CD3C0(void);
BOOL fn_806CD3E0(GPBuddyStatus* status);
void fn_806CD4E0(void);

// inferred: transport callbacks (dwc_transport.c)
void DWCi_RecvCallback(GT2Connection connection, const u8* message, int len, GT2Bool reliable);
void DWCi_PingCallback(GT2Connection connection, int latency);

// inferred: game name and secret key buffers (dwc_init.c area)
extern char lbl_80862150[];
extern char lbl_80862250[];

// inferred: only gamecode is read here (DWCAccUserData of dwc_account.h)
typedef struct DWCUserDataView {
    u8 unk00[0x24]; // 0x00
    u32 gamecode;   // 0x24
} DWCUserDataView;

typedef enum DWCState {
    DWC_STATE_INIT = 0,
    DWC_STATE_AVAILABLE_CHECK = 1,
    DWC_STATE_LOGIN = 2,
    DWC_STATE_ONLINE = 3,
    DWC_STATE_MATCHING = 4,
    DWC_STATE_CONNECTED = 5
} DWCState;

// inferred: login control of dwc_login.c (0x268 bytes)
typedef struct DWCLoginControl {
    GPConnection* pGpObj;       // 0x000
    int state;                  // 0x004
    int productID;              // 0x008
    u8 unk0C[0x40];             // 0x00C
    char authToken[256];        // 0x04C
    char partnerChallenge[256]; // 0x14C
    char username[28];          // 0x24C
} DWCLoginControl;

// inferred: match control of dwc_match.c (0x910 bytes); only the fields used here are named
typedef struct DWCMatchControl {
    u8 unk000[0xD];          // 0x000
    u8 unk00D;               // 0x00D
    u8 unk00E[2];            // 0x00E
    void* qr2Obj;            // 0x010
    volatile u8 qr2MatchType;       // 0x014 inferred: volatile, reread by every comparison
    volatile u8 qr2NNFailed;        // 0x015 inferred: 2 server, 3 client of a game server (volatile as above)
    u8 qr2IsReserved;        // 0x016 inferred: 2 when this host is the server
    u8 unk017[5];            // 0x017
    u32 aidBitmap;           // 0x01C
    u8 unk020[0x28];         // 0x020
    int unk048;              // 0x048
    int unk04C;              // 0x04C
    u8 unk050;               // 0x050
    u8 unk051[7];            // 0x051
    int nodeNum;                    // 0x058
    u8 unk05C[4];            // 0x05C
    DWCNodeInfo nodeInfo[32]; // 0x060
    u8 unk660[0xA4];         // 0x660
    void* sbObj;             // 0x704
    u8 unk708[0x3C];         // 0x708
    int state;               // 0x744
    u8 unk748[0x60];         // 0x748
    int unk7A8;              // 0x7A8
    u8 unk7AC[0x5C];         // 0x7AC
    u8 unk808;               // 0x808
    u8 unk809[0x8F];         // 0x809
    s64 unk898;              // 0x898
    u8 unk8A0[0x1C];         // 0x8A0
    u32 serverDownBase;      // 0x8BC
    u8 unk8C0[8];            // 0x8C0
    int unk8C8;              // 0x8C8
    u8 unk8CC[4];            // 0x8CC
    s64 unk8D0;              // 0x8D0
    u8 unk8D8[0xD];          // 0x8D8
    u8 unk8E5;               // 0x8E5
    u8 unk8E6[0xE];          // 0x8E6
    int unk8F4;              // 0x8F4
    u8 unk8F8[0x18];         // 0x8F8
} DWCMatchControl;

// inferred: DWC friends match control (DWCstFriendsMatchControl), 0x1498 bytes
typedef struct DWCFriendsMatchControl {
    GT2Socket gt2Socket;                         // 0x000
    GT2ConnectionCallbacks gt2Callbacks;         // 0x004
    int gt2SendBufSize;                          // 0x014
    int gt2RecvBufSize;                          // 0x018
    GPConnection gpObj;                          // 0x01C
    const DWCUserData* userdata;                 // 0x020
    DWCState state;                              // 0x024
    DWCState lastState;                          // 0x028
    u8 unk2C;                                    // 0x02C
    u8 closing;                                  // 0x02D inferred: set by CloseAllConnectionsHard
    u8 unk2E[2];                                 // 0x02E
    u16 playerName[26];                          // 0x030
    int profileID;                               // 0x064
    const char* gameName;                        // 0x068
    const char* secretKey;                       // 0x06C
    DWCLoginCallback loginCallback;              // 0x070
    void* loginParam;                            // 0x074
    DWCUpdateServersCallback updateCallback;     // 0x078
    void* updateParam;                           // 0x07C
    DWCMatchedCallback matchedCallback;          // 0x080
    void* matchedParam;                          // 0x084
    DWCConnectionClosedCallback closedCallback;  // 0x088
    void* closedParam;                           // 0x08C
    DWCLoginControl logcnt;                      // 0x090
    DWCFriendControl friendcnt;                  // 0x2F8
    DWCMatchControl matchcnt;                    // 0x360
    DWCTransportInfo transinfo;                  // 0xC70
} DWCFriendsMatchControl;

// inferred: connection data attached to every GT2 connection (8 bytes)
typedef struct DWCConnectionInfo {
    u8 index;      // 0x00
    u8 aid;        // 0x01
    u16 pad;       // 0x02
    int profileID; // 0x04
} DWCConnectionInfo;

#define DWC_MAX_CONNECTION 32

static DWCFriendsMatchControl* stpDwcCnt = NULL;
static int stGt2SocketError = 0; // inferred: name
static GT2Connection stGt2ConnectionList[DWC_MAX_CONNECTION];
static DWCConnectionInfo stConnectionInfoList[DWC_MAX_CONNECTION];
static u8 stAidList[DWC_MAX_CONNECTION];

static void DWCi_SetState(DWCState state);
static inline void DWCi_CacheDNS(const char* gamename);
GPResult fn_806B17B0(GPResult result);
GT2Result fn_806B1910(GT2Result result);
static void fn_806B1B60(DWCError error, int profileID, void* param);
static void fn_806B1CD0(DWCError error, BOOL isChanged, void* param);
static void fn_806B1CF0(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index,
                        void* param);
static void fn_806B2000(GPConnection* connection, GPErrorArg* arg, void* param);
static void fn_806B2470(GPConnection* connection, GPRecvBuddyMessageArg* arg,
                        void* param);
static void fn_806B25F0(GT2Connection connection, const u8* message, int len,
                        GT2Bool reliable);
void fn_806B2600(GT2Connection connection, GT2CloseReason reason, BOOL pseudo,
                 int aid);
static void fn_806B2FC0(GT2Connection connection, GT2CloseReason reason);
static void fn_806B2FD0(GT2Connection connection, int latency);
static void fn_806B3030(GT2Socket socket);
int fn_806B1A00(u8 aid);
u8 fn_806B1250(void);
u8 fn_806B0E30(void);
int fn_806B0DE0(void);
int fn_806B0EB0(u8** aidList);
BOOL fn_806B12F0(void);
GT2Connection fn_806B14D0(u8 aid);
DWCState fn_806B1230(void);

static inline void DWCi_ClearGT2ConnectionList(void) {
    memset(stGt2ConnectionList, 0, sizeof(stGt2ConnectionList));
    memset(stConnectionInfoList, 0, sizeof(stConnectionInfoList));
}

// DWC_InitFriendsMatch
void fn_806AF6A0(DWCUserData* userdata, int productID, const char* secretKey, int sendBufSize,
                 int recvBufSize, DWCFriendData* friendList, int friendListLen) {
    u32 len;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_InitFriendsMatch() was called!!\n");
    stpDwcCnt = DWC_Alloc(DWC_ALLOCTYPE_BASE, sizeof(DWCFriendsMatchControl));
    memset(stpDwcCnt, 0, sizeof(DWCFriendsMatchControl));
    DWC_ClearError();

    stpDwcCnt->gt2Socket = NULL;
    stpDwcCnt->gt2Callbacks.connected = fn_806B8D60;
    stpDwcCnt->gt2Callbacks.received = fn_806B25F0;
    stpDwcCnt->gt2Callbacks.closed = fn_806B2FC0;
    stpDwcCnt->gt2Callbacks.ping = fn_806B2FD0;
    stpDwcCnt->gt2SendBufSize = sendBufSize ? sendBufSize : 0x2000;
    stpDwcCnt->gt2RecvBufSize = recvBufSize ? recvBufSize : 0x2000;
    stpDwcCnt->gpObj = NULL;
    stpDwcCnt->userdata = userdata;
    stpDwcCnt->state = DWC_STATE_INIT;
    stpDwcCnt->lastState = DWC_STATE_INIT;
    stpDwcCnt->closing = FALSE;
    stpDwcCnt->profileID = 0;
    stpDwcCnt->gameName = lbl_80862150;
    stpDwcCnt->secretKey = lbl_80862250;
    stpDwcCnt->loginCallback = NULL;
    stpDwcCnt->loginParam = NULL;
    stpDwcCnt->updateCallback = NULL;
    stpDwcCnt->updateParam = NULL;
    stpDwcCnt->matchedCallback = NULL;
    stpDwcCnt->matchedParam = NULL;
    stpDwcCnt->closedCallback = NULL;
    stpDwcCnt->closedParam = NULL;
    DWCi_ClearGT2ConnectionList();

    fn_806AE750(&stpDwcCnt->logcnt, userdata, &stpDwcCnt->gpObj, productID,
                ((DWCUserDataView*)userdata)->gamecode, stpDwcCnt->playerName, fn_806B1B60,
                NULL);
    // inferred: the friend control keeps the player name in the field named userdata
    DWCi_FriendInit(&stpDwcCnt->friendcnt, &stpDwcCnt->gpObj,
                    (const DWCUserData*)stpDwcCnt->playerName, friendList, friendListLen);
    fn_806B3DF0(&stpDwcCnt->matchcnt, &stpDwcCnt->gpObj, &stpDwcCnt->gt2Socket,
                &stpDwcCnt->gt2Callbacks, lbl_80862150, lbl_80862250, friendList, friendListLen);
    DWCi_InitTransport(&stpDwcCnt->transinfo);

    len = strlen(secretKey) < 256 ? strlen(secretKey) : 255;
    fn_806A9CA0(lbl_80862250, secretKey, len);
    lbl_80862250[len] = '\0';
}

// DWC_ShutdownFriendsMatch
void fn_806AF910(void) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "!!DWC_ShutdownFriendsMatch() was called!! stpDwcCnt = 0x%x\n", stpDwcCnt);
    if (stpDwcCnt == NULL) {
        return;
    }

    if (stpDwcCnt->matchcnt.qr2Obj != NULL) {
        fn_806EF930(stpDwcCnt->matchcnt.qr2Obj);
        stpDwcCnt->matchcnt.qr2Obj = NULL;
    }
    stpDwcCnt->matchcnt.unk050 = 0;
    if (stpDwcCnt->matchcnt.sbObj != NULL) {
        fn_806FFA10(stpDwcCnt->matchcnt.sbObj);
        stpDwcCnt->matchcnt.sbObj = NULL;
    }

    fn_806CD4E0();
    fn_806AE4A0();
    fn_806F87A0();
    fn_806AE4B0();

    if (stpDwcCnt->gpObj != NULL) {
        fn_806DA910(&stpDwcCnt->gpObj, 0, NULL, NULL);
        fn_806DA910(&stpDwcCnt->gpObj, 3, NULL, NULL);
        fn_806DA910(&stpDwcCnt->gpObj, 1, NULL, NULL);
        fn_806DA910(&stpDwcCnt->gpObj, 2, NULL, NULL);
        fn_806DA8D0(&stpDwcCnt->gpObj);
        fn_806DA8B0(&stpDwcCnt->gpObj);
        stpDwcCnt->gpObj = NULL;
    }

    fn_806AEBB0();
    DWCi_ShutdownFriend();
    fn_806BB5E0();
    DWCi_ShutdownTransport();

    if (stpDwcCnt->gt2Socket != NULL) {
        gt2CloseSocket(stpDwcCnt->gt2Socket);
        stpDwcCnt->gt2Socket = NULL;
    }

    DWC_Free(DWC_ALLOCTYPE_BASE, stpDwcCnt, 0);
    stpDwcCnt = NULL;
}

// DWC_ProcessFriendsMatch
void fn_806AFAA0(void) {
    if (stpDwcCnt == NULL || stpDwcCnt->state == DWC_STATE_INIT || DWCi_IsError()) {
        return;
    }

    switch (stpDwcCnt->state) {
    case DWC_STATE_AVAILABLE_CHECK:
        switch (fn_806D7860()) {
        case 1:
            DWC_Printf(DWC_REPORTFLAG_ACHECK, "Confirmed the backend of GameSpy server.\n");
            if (fn_806B17B0(fn_806DA880(&stpDwcCnt->gpObj, stpDwcCnt->logcnt.productID,
                                               16, 11))) {
                return;
            }
            if (fn_806B17B0(
                    fn_806DA910(&stpDwcCnt->gpObj, 0, fn_806B2000, NULL))) {
                return;
            }
            if (fn_806B17B0(
                    fn_806DA910(&stpDwcCnt->gpObj, 3, fn_806B2470, NULL))) {
                return;
            }
            if (fn_806B17B0(
                    fn_806DA910(&stpDwcCnt->gpObj, 7, DWCi_GPRecvBuddyAuthCallback, NULL))) {
                return;
            }
            if (fn_806B17B0(
                    fn_806DA910(&stpDwcCnt->gpObj, 1, DWCi_GPRecvBuddyRequestCallback, NULL))) {
                return;
            }
            if (fn_806B17B0(
                    fn_806DA910(&stpDwcCnt->gpObj, 2, DWCi_GPRecvBuddyStatusCallback, NULL))) {
                return;
            }
            DWCi_SetState(DWC_STATE_LOGIN);
            if (!fn_806AE910()) {
                fn_806AEB10(DWC_ERROR_AUTH_ANY, -20100);
            }
            break;
        case 2:
            fn_806AEB10(DWC_ERROR_AUTH_OUT_OF_SERVICE, -20110);
            return;
        case 3:
            fn_806AEB10(DWC_ERROR_AUTH_STOP_SERVICE, -20101);
            return;
        }
        break;
    case DWC_STATE_LOGIN:
        fn_806AE960();
        break;
    case DWC_STATE_ONLINE:
        DWCi_FriendProcess();
        fn_806B5860(0);
        break;
    case DWC_STATE_MATCHING:
        fn_806B5860(1);
        DWCi_FriendProcess();
        break;
    case DWC_STATE_CONNECTED:
        DWCi_TransportProcess();
        DWCi_FriendProcess();
        if (stpDwcCnt->matchcnt.qr2MatchType == 2 || stpDwcCnt->matchcnt.qr2MatchType == 3) {
            fn_806B5860(1);
        } else if (stpDwcCnt->gt2Socket != NULL) {
            fn_806B5860(0);
        }
        break;
    }

    if (stpDwcCnt->matchcnt.unk050 == 1) {
        if (stpDwcCnt->matchcnt.qr2Obj != NULL) {
            fn_806EF930(stpDwcCnt->matchcnt.qr2Obj);
            stpDwcCnt->matchcnt.qr2Obj = NULL;
        }
        stpDwcCnt->matchcnt.unk050 = 0;
    }
}

// DWC_LoginAsync
BOOL fn_806AFDB0(const u16* ingamesn, const char* reserved, DWCLoginCallback callback,
                 void* param) {
    u32 len;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_LoginAsync() was called!!\n");
    if (ingamesn == NULL) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "ingamesn is NULL!!\n");
        return FALSE;
    }
    if (DWCi_IsError() || stpDwcCnt->state != DWC_STATE_INIT) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }

    stpDwcCnt->loginCallback = callback;
    stpDwcCnt->loginParam = param;
    if (ingamesn == NULL || ingamesn[0] == 0) {
        len = 0;
    } else {
        memset(stpDwcCnt->playerName, 0, sizeof(stpDwcCnt->playerName));
        len = fn_806ABCC0(ingamesn) <= 25 ? fn_806ABCC0(ingamesn) : 25;
        fn_806A9CA0(stpDwcCnt->playerName, ingamesn, len * 2);
    }
    stpDwcCnt->playerName[len] = 0;

    DWCi_CacheDNS(lbl_80862150);

    DWCi_SetState(DWC_STATE_AVAILABLE_CHECK);
    fn_806D76E0(stpDwcCnt->gameName);
    return TRUE;
}

// inferred: caches the DNS queries of the GameSpy servers (the ms%d hash is the one of
// the GameSpy server browsing SDK)
static inline void DWCi_CacheDNS(const char* gamename) {
    char hostname[256];
    unsigned int hashcode;

    if (!fn_806D8060("gpcm.gs.nintendowifi.net")) {
        goto fail;
    }
    if (!fn_806D8060("gpsp.gs.nintendowifi.net")) {
        goto fail;
    }
    if (!fn_806D8060("gamestats.gs.nintendowifi.net")) {
        goto fail;
    }
    if (!fn_806D8060("gamestats2.gs.nintendowifi.net")) {
        goto fail;
    }
    if (gamename == NULL) {
        return;
    }
    sprintf(hostname, "%s.available.gs.nintendowifi.net", gamename);
    if (!fn_806D8060(hostname)) {
        goto fail;
    }
    sprintf(hostname, "%s.natneg1.gs.nintendowifi.net", gamename);
    if (!fn_806D8060(hostname)) {
        goto fail;
    }
    sprintf(hostname, "%s.natneg2.gs.nintendowifi.net", gamename);
    if (!fn_806D8060(hostname)) {
        goto fail;
    }
    sprintf(hostname, "%s.natneg3.gs.nintendowifi.net", gamename);
    if (!fn_806D8060(hostname)) {
        goto fail;
    }
    sprintf(hostname, "%s.master.gs.nintendowifi.net", gamename);
    if (!fn_806D8060(hostname)) {
        goto fail;
    }
    sprintf(hostname, "%s.gamestats.gs.nintendowifi.net", gamename);
    if (!fn_806D8060(hostname)) {
        goto fail;
    }
    sprintf(hostname, "%s.gamestats2.gs.nintendowifi.net", gamename);
    if (!fn_806D8060(hostname)) {
        goto fail;
    }
    {
        const char* p = gamename;
        hashcode = 0;
        while (*p != '\0') {
            hashcode = hashcode * -1664117991 + tolower(*p);
            p++;
        }
    }
    sprintf(hostname, "%s.ms%d.gs.nintendowifi.net", lbl_80862150, hashcode % 20);
    if (fn_806D8060(hostname)) {
        return;
    }
fail:
    DWC_Printf(DWC_REPORTFLAG_WARNING, "Failed to cache DNS query.\n");
}

// DWC_UpdateServersAsync
BOOL fn_806B0180(const char* playerName, DWCUpdateServersCallback updateCallback,
                 void* updateParam, DWCFriendStatusCallback statusCallback, void* statusParam,
                 DWCDeleteFriendListCallback deleteCallback, void* deleteParam) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_UpdateServersAsync() was called!!\n");
    if (DWCi_IsError() || stpDwcCnt->state < DWC_STATE_ONLINE || DWCi_IsFriendUpdating()) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }

    stpDwcCnt->updateCallback = updateCallback;
    stpDwcCnt->updateParam = updateParam;
    // inferred: dwc_friend.h declares the second argument as an unused int
    DWCi_UpdateServersAsync(stpDwcCnt->logcnt.authToken, (int)stpDwcCnt->logcnt.partnerChallenge,
                            fn_806B1CD0, NULL, statusCallback, statusParam,
                            deleteCallback, deleteParam);
    return TRUE;
}

// DWC_ConnectToAnybodyAsync
BOOL fn_806B0260(int a0, u8 numEntry, int a2, DWCMatchedCallback matchedCallback,
                 void* matchedParam, int a5, int a6, int a7, int a8, int a9, int a10, int a11) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_ConnectToAnybodyAsync() was called!!\n");
    if (DWCi_IsError() || stpDwcCnt->state != DWC_STATE_ONLINE) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }

    DWCi_ClearGT2ConnectionList();
    stpDwcCnt->matchedCallback = matchedCallback;
    stpDwcCnt->matchedParam = matchedParam;
    DWCi_SetState(DWC_STATE_MATCHING);
    stpDwcCnt->matchcnt.qr2NNFailed = 0;
    stpDwcCnt->matchcnt.unk8C8 = 0;
    fn_806B42B0(a0, (u8)(numEntry - 1), a2, fn_806B1CF0, NULL, a5, a6, a7, a8, a9, a10,
                a11);
    return TRUE;
}

// DWC_ConnectToFriendsAsync (removed by the linker)
BOOL DWC_ConnectToFriendsAsync(void) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_ConnectToFriendsAsync() was called!!\n");
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
    return FALSE;
}

// DWC_SetupGameServer
BOOL fn_806B0410(int a0, u8 maxEntry, DWCMatchedCallback matchedCallback, void* matchedParam,
                 int a4, int a5, int a6, int a7, int a8) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_SetupGameServer() was called!!\n");
    if (DWCi_IsError() || stpDwcCnt->state != DWC_STATE_ONLINE) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }

    DWCi_ClearGT2ConnectionList();
    stpDwcCnt->matchedCallback = matchedCallback;
    stpDwcCnt->matchedParam = matchedParam;
    DWCi_SetState(DWC_STATE_MATCHING);
    stpDwcCnt->matchcnt.qr2NNFailed = 2;
    stpDwcCnt->matchcnt.unk8C8 = 0;
    fn_806B4940(a0, (u8)(maxEntry - 1), fn_806B1CF0, NULL, a4, a5, a6, a7, a8);
    return TRUE;
}

// DWC_ConnectToGameServerAsync
BOOL fn_806B05B0(int a0, int serverIndex, DWCMatchedCallback matchedCallback, void* matchedParam,
                 int a4, int a5, int a6, int a7, int a8) {
    GPBuddyStatus status;
    int buddyIdx = -1;
    char value[4];
    DWCError error;
    u8 maxEntry;
    u8 numEntry;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_ConnectToGameServerAsync() was called!!\n");
    if (DWCi_IsError() || stpDwcCnt->state != DWC_STATE_ONLINE) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return FALSE;
    }

    DWCi_ClearGT2ConnectionList();
    stpDwcCnt->matchedCallback = matchedCallback;
    stpDwcCnt->matchedParam = matchedParam;
    DWCi_SetState(DWC_STATE_MATCHING);

    // inferred: the friend list index argument is reused for the profile ID
    serverIndex = DWCi_GetProfileIDFromList(serverIndex);
    if (serverIndex == 0 || !fn_806DB310(&stpDwcCnt->gpObj, serverIndex)) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "pid %d is not buddy.\n", serverIndex);
        error = 11;
        goto fail;
    }

    fn_806DB220(&stpDwcCnt->gpObj, serverIndex, &buddyIdx);
    fn_806DB0A0(&stpDwcCnt->gpObj, buddyIdx, &status);
    if (status.status != 6) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "pid %d is not game server.\n", serverIndex);
        error = 11;
        goto fail;
    }

    value[0] = '0';
    if (fn_806CD3E0(&status)) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "pid %d is locked.\n", serverIndex);
        error = 24;
        goto fail;
    }

    fn_806ABA10("SCM", value, status.statusString, '/');
    maxEntry = (u8)strtoul(value, NULL, 10);
    fn_806ABA10("SCN", value, status.statusString, '/');
    numEntry = (u8)strtoul(value, NULL, 10);
    if (numEntry == maxEntry) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "pid %d is fully occupied.\n", serverIndex);
        error = 12;
        goto fail;
    }

    stpDwcCnt->matchcnt.qr2NNFailed = 3;
    stpDwcCnt->matchcnt.unk8C8 = 0;
    fn_806B4D20(a0, serverIndex, fn_806B1CF0, NULL, a4, a5, a6, a7, a8);
    return TRUE;

fail:
    DWCi_SetError(error, 0);
    stpDwcCnt->matchedCallback(error, FALSE, TRUE, FALSE, 0, stpDwcCnt->matchedParam);
    if (stpDwcCnt != NULL && stpDwcCnt->state == DWC_STATE_MATCHING) {
        DWCi_SetState(DWC_STATE_ONLINE);
        fn_806BB440(1);
    }
    return TRUE;
}

// DWC_ConnectToGameServerByGroupID (removed by the linker)
BOOL DWC_ConnectToGameServerByGroupID(void) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_ConnectToGameServerByGroupID() was called!!\n");
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
    return FALSE;
}

// DWC_SetConnectionClosedCallback
BOOL fn_806B0950(DWCConnectionClosedCallback callback, void* param) {
    if (stpDwcCnt == NULL) {
        return FALSE;
    }
    stpDwcCnt->closedCallback = callback;
    stpDwcCnt->closedParam = param;
    return TRUE;
}

// DWC_CloseAllConnectionsHard
int fn_806B0980(void) {
    BOOL found = FALSE;
    int i;
    DWCNodeInfo* node;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_CloseAllConnectionsHard() was called!!\n");
    stpDwcCnt->matchcnt.unk8E5 = 1;
    if (fn_806B3160()) {
        return 1;
    }
    if (stpDwcCnt == NULL || DWCi_IsError() ||
        (stpDwcCnt->state != DWC_STATE_MATCHING && stpDwcCnt->state != DWC_STATE_CONNECTED)) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return -1;
    }

    for (i = 0; i < DWC_MAX_CONNECTION; i++) {
        if (stGt2ConnectionList[i] != NULL) {
            found = TRUE;
            break;
        }
    }

    if (!found) {
        fn_806CCCB0();
        DWC_Printf(DWC_REPORTFLAG_TRANSPORT, "Closed 0 connection.\n");
        fn_806BB440(1);
        fn_806CD4E0();
        DWCi_SetState(DWC_STATE_ONLINE);
        fn_806B9740();
        fn_806CCC90();
        fn_806CCB70(0);
        fn_806BBCA0();
        return 1;
    }

    stpDwcCnt->closing = TRUE;
    gt2CloseAllConnectionsHard(stpDwcCnt->gt2Socket);
    while (fn_806CCCB0() > 1) {
        for (i = 0; i < fn_806CCCB0(); i++) {
            node = fn_806CCED0(i);
            if (node->profileID != stpDwcCnt->profileID) {
                fn_806B2600(NULL, 0, TRUE, node->aid);
            }
        }
    }
    stpDwcCnt->closing = FALSE;
    fn_806CCC90();
    fn_806CCB70(0);
    fn_806BBCA0();

    if (fn_806B1230() != DWC_STATE_ONLINE) {
        fn_806CD4E0();
        DWCi_SetState(DWC_STATE_ONLINE);
        fn_806B9740();
    }
    return 0;
}

// DWC_CloseConnectionHard
int fn_806B0CF0(u8 aid) {
    if (fn_806B12F0()) {
        return fn_806B1A00(aid);
    }
    return -3;
}

static inline BOOL DWCi_IsMatchAvailable(void) {
    return stpDwcCnt->matchcnt.qr2NNFailed == 2 || stpDwcCnt->matchcnt.unk00D;
}

// DWC_GetNumConnectionHost
int fn_806B0DE0(void) {
    if (stpDwcCnt == NULL || !DWCi_IsMatchAvailable()) {
        return 0;
    }
    return fn_806CCCB0();
}

// DWC_GetMyAID
u8 fn_806B0E30(void) {
    DWCNodeInfo* node;

    if (stpDwcCnt == NULL || !DWCi_IsMatchAvailable()) {
        return 0xFF;
    }
    node = fn_806CCF00(stpDwcCnt->profileID);
    if (node == NULL) {
        return 0xFF;
    }
    return node->aid;
}

// DWC_GetAIDList
int fn_806B0EB0(u8** aidList) {
    int i;

    if (stpDwcCnt == NULL || !DWCi_IsMatchAvailable()) {
        return 0;
    }
    memset(stAidList, 0, sizeof(stAidList));
    for (i = 0; i < fn_806CCCB0(); i++) {
        stAidList[i] = fn_806CCED0(i)->aid;
    }
    *aidList = stAidList;
    return fn_806CCCB0();
}

static inline u32 DWCi_AIDListToBitmap(const u8* aidList, int num) {
    u32 bitmap = 0;
    int i;

    for (i = 0; i < num; i++) {
        bitmap |= 1 << aidList[i];
    }
    return bitmap;
}

// DWC_GetAIDBitmap
u32 fn_806B0F80(void) {
    u8* aidList;
    int num;

    if (stpDwcCnt == NULL || !DWCi_IsMatchAvailable()) {
        return 0;
    }
    num = fn_806B0EB0(&aidList);
    return DWCi_AIDListToBitmap(aidList, num);
}

// DWC_IsValidAID
BOOL fn_806B1170(u8 aid) {
    if (stpDwcCnt == NULL) {
        return FALSE;
    }
    if (aid == fn_806B0E30()) {
        return FALSE;
    }
    return fn_806CCF50(aid) != NULL;
}

// DWC_GetState
DWCState fn_806B1230(void) {
    if (stpDwcCnt != NULL) {
        return stpDwcCnt->state;
    }
    return DWC_STATE_INIT;
}

// DWC_GetServerAID
u8 fn_806B1250(void) {
    DWCNodeInfo* node;

    if (stpDwcCnt == NULL || stpDwcCnt->matchcnt.state == 9 || stpDwcCnt->matchcnt.state == 10 ||
        stpDwcCnt->matchcnt.state == 11 || stpDwcCnt->matchcnt.state == 0 ||
        !stpDwcCnt->matchcnt.unk00D || !stpDwcCnt->matchcnt.qr2IsReserved) {
        return 0xFF;
    }
    node = fn_806CCED0(0);
    if (node == NULL) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "*** DWC_GetServerAID first node is NULL!!! ***\n");
        return 0xFF;
    }
    return node->aid;
}

// DWC_IsServerMyself
BOOL fn_806B12F0(void) {
    if (stpDwcCnt == NULL) {
        return FALSE;
    }
    if (fn_806B1250() == 0xFF) {
        return FALSE;
    }
    return stpDwcCnt->matchcnt.qr2IsReserved == 2;
}

// DWCi_GT2Startup
GT2Result fn_806B13C0(void) {
    u16 port;
    GT2Result result;

    if (stpDwcCnt->gt2Socket != NULL) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "gt2Socket is already made.\n");
        return 0;
    }

    port = (u16)(fn_806ABB70(0x4000) + 0xC000);
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "--- Private port = %d ---\n", port);
    result = gt2CreateSocket(&stpDwcCnt->gt2Socket, gt2AddressToString(0, port, NULL),
                         stpDwcCnt->gt2SendBufSize, stpDwcCnt->gt2RecvBufSize,
                         fn_806B3030);
    if (fn_806B1910(result)) {
        return result;
    }
    gt2Listen(stpDwcCnt->gt2Socket, fn_806B85B0);
    gt2SetUnrecognizedMessageCallback(stpDwcCnt->gt2Socket, fn_806B8430);
    return result;
}

// DWCi_GetGT2Connection
GT2Connection fn_806B14D0(u8 aid) {
    int i;

    if (stpDwcCnt == NULL) {
        return NULL;
    }
    for (i = 0; i < DWC_MAX_CONNECTION; i++) {
        if (stGt2ConnectionList[i] != NULL &&
            ((DWCConnectionInfo*)gt2GetConnectionData(stGt2ConnectionList[i]))->aid == aid) {
            return stGt2ConnectionList[i];
        }
    }
    return NULL;
}

// DWCi_GetConnectionAID
u8 fn_806B1570(GT2Connection connection) {
    DWCConnectionInfo* info = gt2GetConnectionData(connection);
    if (info != NULL) {
        return info->aid;
    }
    return 0xFF;
}

// DWCi_GT2GetConnectionListIdx
int fn_806B15B0(void) {
    int i;

    for (i = 0; i < DWC_MAX_CONNECTION; i++) {
        if (stGt2ConnectionList[i] == NULL) {
            return i;
        }
    }
    return -1;
}

// DWCi_GetGT2ConnectionByIdx
GT2Connection* fn_806B1660(int index) {
    return &stGt2ConnectionList[index];
}

// DWCi_GetGT2ConnectionByProfileID
GT2Connection* fn_806B1680(int profileID) {
    int i;

    for (i = 0; i < DWC_MAX_CONNECTION; i++) {
        if (stGt2ConnectionList[i] != NULL &&
            ((DWCConnectionInfo*)gt2GetConnectionData(stGt2ConnectionList[i]))->profileID == profileID) {
            return &stGt2ConnectionList[i];
        }
    }
    return NULL;
}

// DWCi_GetConnectionInfoByIdx
DWCConnectionInfo* fn_806B1710(int index) {
    return &stConnectionInfoList[index];
}

// DWCi_IsValidAID
BOOL fn_806B1730(u8 aid) {
    int i;

    for (i = 0; i < DWC_MAX_CONNECTION; i++) {
        if (stGt2ConnectionList[i] != NULL &&
            ((DWCConnectionInfo*)gt2GetConnectionData(stGt2ConnectionList[i]))->aid == aid) {
            return TRUE;
        }
    }
    return FALSE;
}

static void DWCi_SetState(DWCState state) {
    const char* stateNames[] = {
        "DWC_STATE_INIT",     "DWC_STATE_AVAILABLE_CHECK", "DWC_STATE_LOGIN",
        "DWC_STATE_ONLINE",   "DWC_STATE_MATCHING",        "DWC_STATE_CONNECTED",
    };

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "dwc state changed %s -> %s\n", stateNames[stpDwcCnt->state],
               stateNames[state]);
    stpDwcCnt->lastState = stpDwcCnt->state;
    stpDwcCnt->state = state;
}

// DWCi_HandleGPError
GPResult fn_806B17B0(GPResult result) {
    int errorCode;
    DWCError error;

    if (result == 0) {
        return 0;
    }

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Main, GP error %d\n", result);
    switch (result) {
    case 1:
        error = DWC_ERROR_FATAL;
        errorCode = -1;
        break;
    case 2:
        error = DWC_ERROR_FATAL;
        errorCode = -2;
        break;
    case 3:
        error = DWC_ERROR_NETWORK;
        errorCode = -10;
        break;
    case 4:
        error = DWC_ERROR_NETWORK;
        errorCode = -20;
        break;
    }

    switch (stpDwcCnt->state) {
    case DWC_STATE_AVAILABLE_CHECK:
        errorCode += -61000;
        fn_806AEB10(error, errorCode);
        break;
    case DWC_STATE_LOGIN:
        errorCode += -61000;
        if (stpDwcCnt->logcnt.state < 1) {
            fn_806AEB10(error, errorCode);
        } else {
            DWC_Printf(DWC_REPORTFLAG_ERROR, "Not handle an error here.\n");
        }
        break;
    case DWC_STATE_MATCHING:
        errorCode += -81000;
        fn_806B9500(error, errorCode);
        break;
    default:
        errorCode += -91000;
        break;
    }
    DWCi_StopFriendProcess(error, errorCode);
    return result;
}

// DWCi_HandleGT2Error
GT2Result fn_806B1910(GT2Result result) {
    DWCError error;
    int errorCode;

    if (result == 0) {
        return 0;
    }

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Main, GT2 error %d\n", result);
    switch (result) {
    case 1:
        error = DWC_ERROR_FATAL;
        errorCode = -1;
        break;
    case 2:
    case 5:
        error = DWC_ERROR_NONE;
        errorCode = 0;
        result = 0;
        break;
    case 3:
        error = DWC_ERROR_NETWORK;
        errorCode = -10;
        break;
    case 4:
        error = DWC_ERROR_NETWORK;
        errorCode = -30;
        break;
    case 6:
        error = DWC_ERROR_NETWORK;
        errorCode = -70;
        break;
    case 7:
        error = DWC_ERROR_NETWORK;
        errorCode = -80;
        break;
    }
    if (error != DWC_ERROR_NONE) {
        fn_806AEB10(error, errorCode + -67000);
    }
    return result;
}

// DWCi_CloseConnectionHard
int fn_806B1A00(u8 aid) {
    GT2Connection connection;
    DWCNodeInfo* node;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWCi_CloseConnectionHard() was called!! aid = %d.\n", aid);
    if (stpDwcCnt == NULL || DWCi_IsError() ||
        (stpDwcCnt->state != DWC_STATE_MATCHING && stpDwcCnt->state != DWC_STATE_CONNECTED)) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
        return -1;
    }

    connection = fn_806B14D0(aid);
    if (connection == NULL) {
        node = fn_806CCF50(aid);
        if (node == NULL) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "No connection!\n");
            return -2;
        }
        fn_806B2600(NULL, 0, TRUE, node->aid);
        return 0;
    }
    gt2CloseConnectionHard(connection);
    return 0;
}

// DWCi_CloseConnectionHardBitmap (removed by the linker)
u32 DWCi_CloseConnectionHardBitmap(u32* bitmap) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG,
               "!!DWCi_CloseConnectionHardBitmap() was called!! AID bitmap = 0x%x\n", *bitmap);
    return 0;
}

// DWCi_LoginCallback
static void fn_806B1B60(DWCError error, int profileID, void* param) {
    if (error == DWC_ERROR_NONE) {
        stpDwcCnt->profileID = profileID;
        DWCi_SetState(DWC_STATE_ONLINE);
        DWCi_ResetGPProcessTime();
    } else {
        DWCi_SetState(DWC_STATE_INIT);
    }
    if (stpDwcCnt->loginCallback != NULL) {
        stpDwcCnt->loginCallback(error, profileID, stpDwcCnt->loginParam);
    }
}

// DWCi_UpdateServersCallback
static void fn_806B1CD0(DWCError error, BOOL isChanged, void* param) {
    stpDwcCnt->updateCallback(error, isChanged, stpDwcCnt->updateParam);
}

// DWCi_MatchedCallback
// NONMATCHING: 96.4% - the [**user data] loop reuses stpDwcCnt loaded by the loop test, the
// original reloads it at the top of the body (same in fn_806B2600); tried node pointer locals,
// inline accessors and helpers, while loop
static void fn_806B1CF0(DWCError error, BOOL cancel, BOOL self, BOOL isServer, int index,
                        void* param) {
    int i;

    if (error == DWC_ERROR_NONE && cancel) {
        if (stpDwcCnt->matchcnt.state == 0) {
            DWCi_SetState(DWC_STATE_ONLINE);
            fn_806B9740();
        }
    } else if (error == DWC_ERROR_NONE) {
        DWCi_SetState(DWC_STATE_CONNECTED);
    }

    if (error == DWC_ERROR_NONE) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "[**node info]\n");
        for (i = 0; i < stpDwcCnt->matchcnt.nodeNum; i++) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "[%02d] aid = %d, pid = %u, qr2 pub adr=%s\n", i,
                       stpDwcCnt->matchcnt.nodeInfo[i].aid, stpDwcCnt->matchcnt.nodeInfo[i].profileID,
                       gt2AddressToString(stpDwcCnt->matchcnt.nodeInfo[i].publicip,
                                   stpDwcCnt->matchcnt.nodeInfo[i].publicport, NULL));
        }
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "[**user data]\n");
        for (i = 0; i < stpDwcCnt->matchcnt.nodeNum; i++) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "[%02d] aid = %d  buf[ 0=%d 1=%d 2=%d 3=%d ]\n", i,
                       stpDwcCnt->matchcnt.nodeInfo[i].aid, stpDwcCnt->matchcnt.nodeInfo[i].buf[0],
                       stpDwcCnt->matchcnt.nodeInfo[i].buf[1], stpDwcCnt->matchcnt.nodeInfo[i].buf[2],
                       stpDwcCnt->matchcnt.nodeInfo[i].buf[3]);
        }
    }

    if (!stpDwcCnt->matchcnt.unk8E5) {
        stpDwcCnt->matchedCallback(error, cancel, self, isServer, index, stpDwcCnt->matchedParam);
    }

    if (error != DWC_ERROR_NONE && stpDwcCnt != NULL &&
        (stpDwcCnt->state == DWC_STATE_MATCHING || stpDwcCnt->state == DWC_STATE_CONNECTED)) {
        DWCi_SetState(DWC_STATE_ONLINE);
    }
}

// DWCi_GPErrorCallback
static void fn_806B2000(GPConnection* connection, GPErrorArg* arg, void* param) {
    const char* errorString;
    const char* resultString;
    int errorCode = arg->errorCode;

    if (errorCode == 0x603 || errorCode == 0x901 || errorCode == 0xB01) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore GP ALLREADY_BUDDY or NOT_BUDDY %d.\n", errorCode);
        return;
    }

    switch (arg->result) {
    case 0:
        resultString = "GP_NO_ERROR";
        break;
    case 1:
        resultString = "GP_MEMORY_ERROR";
        break;
    case 2:
        resultString = "GP_PARAMETER_ERROR";
        break;
    case 3:
        resultString = "GP_NETWORK_ERROR";
        break;
    case 4:
        resultString = "GP_SERVER_ERROR";
        break;
    default:
        resultString = "Unknown result!\n";
        break;
    }

    switch (errorCode) {
    case 0x000: errorString = "GP_GENERAL"; break;
    case 0x001: errorString = "GP_PARSE"; break;
    case 0x002: errorString = "GP_NOT_LOGGED_IN"; break;
    case 0x003: errorString = "GP_BAD_SESSKEY"; break;
    case 0x004: errorString = "GP_DATABASE"; break;
    case 0x005: errorString = "GP_NETWORK"; break;
    case 0x006: errorString = "GP_FORCED_DISCONNECT"; break;
    case 0x007: errorString = "GP_CONNECTION_CLOSED"; break;
    case 0x100: errorString = "GP_LOGIN"; break;
    case 0x101: errorString = "GP_LOGIN_TIMEOUT"; break;
    case 0x102: errorString = "GP_LOGIN_BAD_NICK"; break;
    case 0x103: errorString = "GP_LOGIN_BAD_EMAIL"; break;
    case 0x104: errorString = "GP_LOGIN_BAD_PASSWORD"; break;
    case 0x105: errorString = "GP_LOGIN_BAD_PROFILE"; break;
    case 0x106: errorString = "GP_LOGIN_PROFILE_DELETED"; break;
    case 0x107: errorString = "GP_LOGIN_CONNECTION_FAILED"; break;
    case 0x108: errorString = "GP_LOGIN_SERVER_AUTH_FAILED"; break;
    case 0x200: errorString = "GP_NEWUSER"; break;
    case 0x201: errorString = "GP_NEWUSER_BAD_NICK"; break;
    case 0x202: errorString = "GP_NEWUSER_BAD_PASSWORD"; break;
    case 0x300: errorString = "GP_UPDATEUI"; break;
    case 0x301: errorString = "GP_UPDATEUI_BAD_EMAIL"; break;
    case 0x400: errorString = "GP_NEWPROFILE"; break;
    case 0x401: errorString = "GP_NEWPROFILE_BAD_NICK"; break;
    case 0x402: errorString = "GP_NEWPROFILE_BAD_OLD_NICK"; break;
    case 0x500: errorString = "GP_UPDATEPRO"; break;
    case 0x501: errorString = "GP_UPDATEPRO_BAD_NICK"; break;
    case 0x600: errorString = "GP_ADDBUDDY"; break;
    case 0x601: errorString = "GP_ADDBUDDY_BAD_FROM"; break;
    case 0x602: errorString = "GP_ADDBUDDY_BAD_NEW"; break;
    case 0x603: errorString = "GP_ADDBUDDY_ALREADY_BUDDY"; break;
    case 0x700: errorString = "GP_AUTHADD"; break;
    case 0x701: errorString = "GP_AUTHADD_BAD_FROM"; break;
    case 0x702: errorString = "GP_AUTHADD_BAD_SIG"; break;
    case 0x800: errorString = "GP_STATUS"; break;
    case 0x900: errorString = "GP_BM"; break;
    case 0x901: errorString = "GP_BM_NOT_BUDDY"; break;
    case 0xA00: errorString = "GP_GETPROFILE"; break;
    case 0xA01: errorString = "GP_GETPROFILE_BAD_PROFILE"; break;
    case 0xB00: errorString = "GP_DELBUDDY"; break;
    case 0xB01: errorString = "GP_DELBUDDY_NOT_BUDDY"; break;
    case 0xC00: errorString = "GP_DELPROFILE"; break;
    case 0xC01: errorString = "GP_DELPROFILE_LAST_PROFILE"; break;
    case 0xD00: errorString = "GP_SEARCH"; break;
    case 0xD01: errorString = "GP_SEARCH_CONNECTION_FAILED"; break;
    default: errorString = "Unknown error code!\n"; break;
    }

    if (arg->fatal) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "FATAL ERROR\n");
    } else {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "ERROR\n");
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "RESULT: %s (%d)\n", resultString, arg->result);
    DWC_Printf(DWC_REPORTFLAG_ERROR, "ERROR CODE: %s (0x%X)\n", errorString, arg->errorCode);
    DWC_Printf(DWC_REPORTFLAG_ERROR, "ERROR STRING: %s\n", arg->errorString);
    fn_806B17B0(3);
}

// DWCi_GPRecvBuddyMessageCallback
static void fn_806B2470(GPConnection* connection, GPRecvBuddyMessageArg* arg,
                        void* param) {
    char version[12] = {0};
    const char* message = arg->message;
    u32 len;

    if (memcmp(message, "GPCM", strlen("GPCM")) != 0) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "Received undefined buddy message. '%s'\n", message);
        return;
    }
    message += strlen("GPCM");

    len = strchr(message, 'v') - message;
    strncpy(version, message, len);
    if (len > 10 || strtoul(version, NULL, 10) != 90) {
        DWC_Printf(DWC_REPORTFLAG_WARNING,
                   "Received different version buddy message command. '%s'\n", message);
        return;
    }
    message += len + 1;

    if (memcmp(message, "MAT", strlen("MAT")) == 0) {
        if (stpDwcCnt->state == DWC_STATE_MATCHING ||
            (stpDwcCnt->state == DWC_STATE_CONNECTED &&
             (stpDwcCnt->matchcnt.qr2MatchType == 2 || stpDwcCnt->matchcnt.qr2MatchType == 3))) {
            message += strlen("MAT");
            fn_806B9410(connection, arg->profile, message);
        } else {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed GP matching command.\n");
        }
    }
}

// DWCi_GT2ReceivedCallback
static void fn_806B25F0(GT2Connection connection, const u8* message, int len,
                        GT2Bool reliable) {
    DWCi_RecvCallback(connection, message, len, reliable);
}

static inline void DWCi_DeleteNodeInfo(u8 aid) {
    if (fn_806CCF50(aid) != NULL) {
        fn_806CCCC0(aid);
    }
}

// DWCi_GT2ClosedProcess
// NONMATCHING: 98.9% - same [**user data] loop difference as fn_806B1CF0
void fn_806B2600(GT2Connection connection, GT2CloseReason reason, BOOL pseudo,
                 int aid) {
    int profileID = 0;
    int errorCode;
    int i;
    BOOL isServer = FALSE;
    BOOL valid;
    DWCError error;
    DWCConnectionInfo* info;
    DWCNodeInfo* node;

    if (fn_806BB6A0()) {
        DWC_Printf(DWC_REPORTFLAG_WARNING,
                   "Called DWC_ShutdownFriendsMatch() with unexpected status.\n");
        return;
    }

    switch (reason) {
    case 0:
    case 1:
        error = DWC_ERROR_NONE;
        errorCode = 0;
        break;
    case 2:
    case 3:
        error = DWC_ERROR_NETWORK;
        errorCode = -7600;
        break;
    case 4:
        error = DWC_ERROR_FATAL;
        errorCode = -7601;
        break;
    }

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "Connection was closed (reason %d) pseud(%d).\n", reason,
               pseudo);

    if (error == DWC_ERROR_NONE) {
        if (!pseudo) {
            info = gt2GetConnectionData(connection);
            if (info == NULL) {
                return;
            }
            aid = info->aid;
        }

        valid = fn_806CCF50((u8)aid) != NULL;
        if (!pseudo) {
            DWCi_ClearTransConnection((u8)aid);
        }
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "aid = %d (validity %d).\n", (u8)aid, valid);

        if (fn_806CCEA0() != NULL && (u8)aid == fn_806CCEA0()->aid) {
            isServer = TRUE;
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "**** SC server was dead....****\n");
        }

        node = fn_806CCF50((u8)aid);
        if (node == NULL) {
            profileID = 0;
        } else {
            profileID = node->profileID;
            fn_806CCCC0((u8)aid);
        }

        stpDwcCnt->matchcnt.aidBitmap &= ~(1 << (u8)aid);
        if (stpDwcCnt->matchcnt.unk04C && isServer) {
            if (!fn_806CB4C0()) {
                stpDwcCnt->matchcnt.unk048 = 1;
            }
        }

        if (profileID == 0) {
            profileID = ((DWCConnectionInfo*)gt2GetConnectionData(fn_806B14D0((u8)aid)))->profileID;
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "**** pid(%u) is not in node info.****\n", profileID);
        }

        if (!pseudo) {
            stGt2ConnectionList[info->index] = NULL;
        }
    }

    if (stpDwcCnt->matchcnt.unk8F4 == 1 && fn_806B12F0()) {
        stpDwcCnt->matchcnt.unk8F4 = 1;
    } else if (stpDwcCnt->matchcnt.qr2NNFailed == 3 || stpDwcCnt->matchcnt.qr2NNFailed == 2) {
        stpDwcCnt->matchcnt.unk8F4 = 0;
    } else if (fn_806B0DE0() == 1) {
        stpDwcCnt->matchcnt.unk8F4 = 1;
    } else {
        stpDwcCnt->matchcnt.unk8F4 = 0;
    }

    if (fn_806CB4C0()) {
        stpDwcCnt->matchcnt.serverDownBase = 0;
        for (i = 0; i < fn_806CCCB0();) {
            node = fn_806CCED0(i);
            if (node->profileID != stpDwcCnt->matchcnt.unk7A8 && fn_806B14D0(node->aid) == NULL) {
                if (stpDwcCnt->closedCallback != NULL) {
                    stpDwcCnt->closedCallback(error, reason == 0 ? TRUE : FALSE, isServer, node->aid,
                                              DWCi_GetFriendListIndex(node->profileID),
                                              stpDwcCnt->closedParam);
                }
                DWCi_DeleteNodeInfo(node->aid);
            } else {
                stpDwcCnt->matchcnt.serverDownBase |= 1 << fn_806CCED0(i)->aid;
                i++;
            }
        }
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "[next server] serverDownBase bitmap = 0x%x\n",
                   stpDwcCnt->matchcnt.serverDownBase);
    }

    if (reason != 0 && fn_806B12F0() == TRUE) {
        stpDwcCnt->matchcnt.unk8D0 = 0;
        if (fn_806CD3C0() && stpDwcCnt->matchcnt.state == 16 &&
            stpDwcCnt->matchcnt.unk808 == 8) {
            stpDwcCnt->matchcnt.unk898 = 0;
        }
    }

    if (!stpDwcCnt->closing && stpDwcCnt->state == DWC_STATE_CONNECTED && !valid) {
        if (stpDwcCnt->matchcnt.qr2MatchType == 2 && error == DWC_ERROR_NONE) {
            fn_806BB440(-1);
            fn_806BB3F0(profileID);
        }
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Closing process by matching SC.\n");
        return;
    }

    if (fn_806BAE10(error, errorCode, profileID)) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Closing process by matching.\n");
        return;
    }

    if (error != DWC_ERROR_NONE) {
        DWCi_SetError(error, errorCode + -80000);
        return;
    }

    if (stpDwcCnt->matchcnt.qr2MatchType == 2) {
        if (!stpDwcCnt->closing) {
            fn_806BB440(-1);
        } else if (fn_806CCCB0() == 1) {
            fn_806BB440(1);
        }
    } else if (fn_806CCCB0() == 1) {
        fn_806BB440(1);
    }

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "[**node info]\n");
    for (i = 0; i < stpDwcCnt->matchcnt.nodeNum; i++) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "[%02d] aid = %d, pid = %u, qr2 pub adr=%s\n", i,
                   stpDwcCnt->matchcnt.nodeInfo[i].aid, stpDwcCnt->matchcnt.nodeInfo[i].profileID,
                   gt2AddressToString(stpDwcCnt->matchcnt.nodeInfo[i].publicip,
                               stpDwcCnt->matchcnt.nodeInfo[i].publicport, NULL));
    }
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "[**user data]\n");
    for (i = 0; i < stpDwcCnt->matchcnt.nodeNum; i++) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "[%02d] aid = %d  buf[ 0=%d 1=%d 2=%d 3=%d ]\n", i,
                   stpDwcCnt->matchcnt.nodeInfo[i].aid, stpDwcCnt->matchcnt.nodeInfo[i].buf[0],
                   stpDwcCnt->matchcnt.nodeInfo[i].buf[1], stpDwcCnt->matchcnt.nodeInfo[i].buf[2],
                   stpDwcCnt->matchcnt.nodeInfo[i].buf[3]);
    }

    if (stpDwcCnt->matchcnt.qr2IsReserved == 2) {
        fn_806EF8B0(stpDwcCnt->matchcnt.qr2Obj);
    }

    if (stpDwcCnt->closedCallback != NULL && valid) {
        stpDwcCnt->closedCallback(error, reason == 0 ? TRUE : FALSE, isServer, (u8)aid,
                                  DWCi_GetFriendListIndex(profileID), stpDwcCnt->closedParam);
    }

    if (!stpDwcCnt->closing && stpDwcCnt->matchcnt.qr2MatchType == 2) {
        return;
    }

    if (fn_806CD150() == 1) {
        fn_806CCB70(1);
    }

    if (fn_806CCCB0() == 1) {
        if (stpDwcCnt->closing) {
            fn_806CD4E0();
            DWCi_SetState(DWC_STATE_ONLINE);
            fn_806B9740();
            stpDwcCnt->matchcnt.qr2IsReserved = 0;
        } else {
            fn_806CCB70(11);
        }
    } else if (stpDwcCnt->matchcnt.qr2MatchType == 3 && isServer && fn_806CCCB0() >= 1) {
        if (fn_806CCCB0() == 1 && fn_806CCC70()) {
            fn_806CCB70(11);
        } else {
            fn_806CCB70(9);
        }
    }
}

// DWCi_GT2ClosedCallback
static void fn_806B2FC0(GT2Connection connection, GT2CloseReason reason) {
    fn_806B2600(connection, reason, FALSE, 0xFF);
}

// DWCi_GT2PingCallback
static void fn_806B2FD0(GT2Connection connection, int latency) {
    DWC_Printf(DWC_REPORTFLAG_TRANSPORT, "Ping: %dms\n", latency);
    DWCi_PingCallback(connection, latency);
}

// DWCi_GT2SocketErrorCallback
static void fn_806B3030(GT2Socket socket) {
    stGt2SocketError = fn_806D7F20(gt2GetSocketSOCKET(socket));
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Socket fatal error! (%d)\n", stGt2SocketError);
    DWCi_SetError(DWC_ERROR_FATAL, -97003);
    stpDwcCnt->gt2Socket = NULL;
}

// inferred: aid of the server, or of the first node when there is no server
u8 fn_806B30A0(void) {
    u8 aid = fn_806B1250();
    if (aid == 0xFF) {
        DWCNodeInfo* node = fn_806CCED0(0);
        if (node != NULL) {
            aid = node->aid;
        }
    }
    return aid;
}
