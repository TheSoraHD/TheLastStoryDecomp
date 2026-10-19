// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Matchmaking: GameSpy QR2 (reporting), SB (server browsing), NN (NAT negotiation) and GT2
// connections used to build the mesh of matched consoles.
// Function names come from the DWC API and from older DWC builds where known; the rest keep
// their address names.
#include <dwc/dwc_base64.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_account.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// inferred: GameSpy Presence types (gp.h) and dwc_friend.c entry points
typedef struct GPIConnection GPIConnection;
typedef GPIConnection* GPConnection;
typedef int GPResult;
int DWCi_GetFriendListIndex(int profileID);
GPResult DWCi_SetGPStatus(int status, const char* statusString, const char* locationString);

// inferred: GP buddy status (gp.h); only the status string is read here
typedef struct GPBuddyStatus {
    int profile;              // 0x000
    int status;               // 0x004
    char statusString[256];   // 0x008
    char locationString[256]; // 0x108
} GPBuddyStatus;

// inferred: GameSpy types (gt2.h, qr2.h, sb_serverbrowsing.h)
typedef void* GT2Socket;
typedef void* GT2Connection;
typedef int GT2Result;
typedef struct qr2_implementation_s* qr2_t;
typedef struct _ServerBrowser* ServerBrowser;
typedef struct _SBServer* SBServer;

// inferred: GameSpy entry points; names from the GameSpy SDK, no source yet
void gt2CloseAllConnectionsHard(GT2Socket socket);
SBServer ServerBrowserGetServer(ServerBrowser sb, int index);                         // ServerBrowserGetServer
int SBServerGetIntValueA(SBServer server, const char* key, int idefault);           // SBServerGetIntValue
const char* SBServerGetStringValueA(SBServer server, const char* key, const char* sdefault); // SBServerGetStringValue
void qr2_register_keyA(int keyid, const char* key);                              // qr2_register_key

// inferred: dwc_common.c (DWC_SetCommonKeyValueString, DWC_AddCommonKeyValueString,
// DWC_GetCommonValueString)
void fn_806AB920(const char* key, const char* value, char* string, char separator);
void fn_806AB980(const char* key, const char* value, char* string, char separator);
int fn_806ABA10(const char* key, char* value, const char* string, char separator);

// inferred: dwc_main.c
int fn_806B1230(void);  // DWC_GetState
BOOL fn_806B12F0(void); // inferred: TRUE on the match server

// inferred: matched callback (DWCMatchedSCCallback of the DWC API)
typedef void (*DWCMatchedCallback)(DWCError error, BOOL cancel, BOOL self, BOOL isServer,
                                   int index, void* param);

// inferred: names from the state name table of DWCi_SetMatchStatus
typedef enum DWCMatchState {
    DWC_MATCH_STATE_INIT = 0,
    DWC_MATCH_STATE_CL_WAITING,
    DWC_MATCH_STATE_CL_SEARCH_HOST,
    DWC_MATCH_STATE_CL_WAIT_RESV,
    DWC_MATCH_STATE_CL_SEARCH_EVAL_HOST,
    DWC_MATCH_STATE_CL_NN,
    DWC_MATCH_STATE_CL_GT2,
    DWC_MATCH_STATE_CL_CANCEL_SYN,
    DWC_MATCH_STATE_CL_SYN,
    DWC_MATCH_STATE_CL_SVDOWN_1,
    DWC_MATCH_STATE_CL_SVDOWN_2,
    DWC_MATCH_STATE_CL_SVDOWN_3,
    DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST,
    DWC_MATCH_STATE_SV_WAITING,
    DWC_MATCH_STATE_SV_OWN_NN,
    DWC_MATCH_STATE_SV_OWN_GT2,
    DWC_MATCH_STATE_SV_WAIT_CL_LINK,
    DWC_MATCH_STATE_SV_CANCEL_SYN,
    DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT,
    DWC_MATCH_STATE_SV_SYN,
    DWC_MATCH_STATE_SV_SYN_WAIT,
    DWC_MATCH_STATE_WAIT_CLOSE,
    DWC_MATCH_STATE_SEARCH_OWN,
    DWC_MATCH_STATE_SV_SYN_CLOSE_WAIT,
    DWC_MATCH_STATE_NUM
} DWCMatchState;

// inferred: server evaluation callback (DWCEvalPlayerCallback of the DWC API)
typedef int (*DWCEvalCallback)(int index, void* param);

// inferred: reservation filter callback (DWC_ConnectToAnybodyAsync and friends)
typedef BOOL (*DWCResvCallback)(void* userData, void* param);

// inferred: suspend callback (DWCSuspendCallback of the DWC API)
typedef void (*DWCSuspendCallback)(DWCError error, BOOL suspend, void* param);

// inferred: IPv4 address (SOInAddr of SO.h)
typedef struct SOInAddr {
    u32 addr;
} SOInAddr;

// inferred: socket address (SOSockAddrIn)
typedef struct SOSockAddrIn {
    u8 len;     // 0x00
    u8 family;  // 0x01
    u16 port;   // 0x02
    u32 addr;   // 0x04
} SOSockAddrIn;

// inferred: header of the matching commands sent through SB/GT2 (little endian, as on the DS)
typedef struct DWCMatchCommandHeader {
    char magic[4];  // 0x00
    u32 ver;        // 0x04
    u8 command;     // 0x08
    u8 size;        // 0x09
    u16 port;       // 0x0A
    u32 ip;         // 0x0C
    int profileID;  // 0x10
} DWCMatchCommandHeader;

// inferred: nonport tick getter (inline in this build)
typedef struct DWCMatchCommand {
    DWCMatchCommandHeader header; // 0x00
    u32 data[32];                 // 0x14
} DWCMatchCommand;

static inline s64 DWCi_Np_GetTick(void) {
    return OSGetTime();
}

static inline u32 DWCi_SwapU32(u32 x) {
    x = ((x >> 8) & 0x00ff00ff) | ((x << 8) & 0xff00ff00);
    return (x >> 16) | (x << 16);
}

static inline u16 DWCi_SwapU16(u16 x) {
    return (u16)((x >> 8) | (x << 8));
}

// BEGIN STRUCT DWCNodeInfo
typedef struct DWCNodeInfo {
    int profileID;                           // 0x000
    u32 ip;                                  // 0x004
    u32 localIP;                             // 0x008
    u16 port;                                // 0x00C
    u16 localPort;                           // 0x00E
    u32 nnIP;                                // 0x010
    u16 nnPort;                              // 0x014
    u8 aid;                                  // 0x016
    u8 unk17;                                // 0x017
    int unk18;                               // 0x018 inferred: mesh NN count
    u8 pad1C[0x4]; // 0x01C
    s64 unk20;                               // 0x020 inferred: only for the 8-byte alignment of the struct
    u32 unk28;                               // 0x028
    u8 pad2C[0x4]; // 0x02C
} DWCNodeInfo;
// END STRUCT DWCNodeInfo

// BEGIN STRUCT DWCNodeInfoList
typedef struct DWCNodeInfoList {
    int nodeNum;                             // 0x000
    u8 pad4[0x4]; // 0x004
    DWCNodeInfo nodeInfo[32];                // 0x008
} DWCNodeInfoList;
// END STRUCT DWCNodeInfoList

// BEGIN STRUCT DWCResendCommand
typedef struct DWCResendCommand {
    u8 command;                              // 0x000
    u8 unk1;                                 // 0x001
    u16 port;                                // 0x002
    u32 ip;                                  // 0x004
    u32 data[32];                            // 0x008
    int profileID;                           // 0x088
    int len;                                 // 0x08C
    s64 time;                                // 0x090
} DWCResendCommand;
// END STRUCT DWCResendCommand

// BEGIN STRUCT DWCNNData
typedef struct DWCNNData {
    u8 isChild;                              // 0x000
    u8 unk1;                                 // 0x001
    u16 port;                                // 0x002
    u32 ip;                                  // 0x004
    int cookie;                              // 0x008
} DWCNNData;
// END STRUCT DWCNNData

// BEGIN STRUCT DWCMatchControl
typedef struct DWCMatchControl {
    GPConnection* pGpObj;                    // 0x000
    GT2Socket* pGt2Socket;                   // 0x004
    void* gt2Callbacks;                      // 0x008
    u8 unkC;                                 // 0x00C
    u8 unkD;                                 // 0x00D
    u8 padE[0x2]; // 0x00E
    qr2_t qr2Obj;                            // 0x010
    volatile u8 qr2MatchType;                // 0x014 inferred: re-read on every access in the target (volatile)
    volatile u8 qr2MatchSubType;             // 0x015 inferred: re-read on every access in the target (volatile)
    volatile u8 qr2IsReserved;               // 0x016 inferred: re-read on every access in the target (volatile)
    u8 qr2NumEntry;                          // 0x017
    u8 unk18;                                // 0x018
    u8 unk19;                                // 0x019
    u8 pad1A[0x2]; // 0x01A
    u32 suspendBitmap;                       // 0x01C
    s64 unk20;                               // 0x020
    BOOL suspended;                          // 0x028
    int suspendReq;                          // 0x02C
    u8 unk30;                                // 0x030
    u8 pad31[0x3]; // 0x031
    DWCSuspendCallback suspendCallback;      // 0x034
    void* suspendParam;                      // 0x038
    u8 pad3C[0x4]; // 0x03C
    s64 suspendTime;                         // 0x040
    int unk48;                               // 0x048
    int suspending;                          // 0x04C
    u8 unk50;                                // 0x050
    u8 pad51[0x7]; // 0x051
    DWCNodeInfoList nodeInfoList;            // 0x058
    DWCNodeInfo tempNewNodeInfo;             // 0x660
    DWCNodeInfo unk690;                      // 0x690
    int unk6C0;                              // 0x6C0
    int unk6C4;                              // 0x6C4
    DWCNodeInfo unk6C8;                      // 0x6C8
    u16 qr2Port;                             // 0x6F8
    u8 pad6FA[0x2]; // 0x6FA
    u32 qr2IP;                               // 0x6FC
    int unk700;                              // 0x700
    ServerBrowser sbObj;                     // 0x704
    int unk708;                              // 0x708
    u8 pad70C[0x4]; // 0x70C
    s64 unk710;                              // 0x710
    s64 unk718;                              // 0x718
    u8 unk720;                               // 0x720
    u8 unk721;                               // 0x721
    u16 unk722;                              // 0x722
    int unk724;                              // 0x724
    s64 unk728;                              // 0x728
    s64 unk730;                              // 0x730
    DWCNNData nnData;                        // 0x738
    int state;                               // 0x744
    u8 unk748;                               // 0x748
    u8 friendIdxListIdx;                     // 0x749
    u8 pad74A[0x6]; // 0x74A
    u8 unk750;                               // 0x750
    u8 closing;                              // 0x751
    u8 unk752;                               // 0x752
    u8 unk753;                               // 0x753
    u8 unk754;                               // 0x754
    u8 unk755;                               // 0x755
    u8 unk756;                               // 0x756
    u8 unk757;                               // 0x757
    u16 unk758;                              // 0x758
    u16 unk75A;                              // 0x75A
    u32 resvCheckValue;                      // 0x75C
    int unk760;                              // 0x760
    u8 pad764[0x4]; // 0x764
    s64 unk768;                              // 0x768
    int unk770;                              // 0x770
    u8 pad774[0x4]; // 0x774
    s64 unk778;                              // 0x778
    int unk780;                              // 0x780
    int unk784;                              // 0x784
    s64 unk788;                              // 0x788
    s64 unk790;                              // 0x790
    s64 unk798;                              // 0x798
    s64 unk7A0;                              // 0x7A0
    int profileID;                           // 0x7A8
    int unk7AC;                              // 0x7AC
    int friendIdx;                           // 0x7B0
    const char* gameName;                    // 0x7B4
    const char* secretKey;                   // 0x7B8
    DWCFriendData* friendList;               // 0x7BC
    int friendListLen;                       // 0x7C0
    u8 friendIdxList[64];                    // 0x7C4
    int friendIdxListLen;                    // 0x804
    DWCResendCommand resend;                 // 0x808
    DWCMatchedCallback matchedCallback;      // 0x8A0
    void* matchedParam;                      // 0x8A4
    void* unk8A8;                            // 0x8A8
    void* unk8AC;                            // 0x8AC
    DWCEvalCallback evalCallback;            // 0x8B0
    void* evalParam;                         // 0x8B4
    int unk8B8;                              // 0x8B8
    int unk8BC;                              // 0x8BC
    u32 groupID;                             // 0x8C0
    u8 unk8C4;                               // 0x8C4
    u8 pad8C5[0x3]; // 0x8C5
    int unk8C8;                              // 0x8C8
    u8 unk8CC;                               // 0x8CC
    u8 unk8CD;                               // 0x8CD
    u8 pad8CE[0x2]; // 0x8CE
    s64 unk8D0;                              // 0x8D0
    u8 unk8D8;                               // 0x8D8
    u8 pad8D9[0x3]; // 0x8D9
    int unk8DC;                              // 0x8DC
    int unk8E0;                              // 0x8E0
    u8 unk8E4;                               // 0x8E4
    u8 cancelSilent;                         // 0x8E5
    u8 unk8E6;                               // 0x8E6
    u8 pad8E7[0x1]; // 0x8E7
    DWCResvCallback resvCallback;            // 0x8E8
    u8 unk8EC[4];                            // 0x8EC
    void* resvParam;                         // 0x8F0
    int unk8F4;                              // 0x8F4
    int unk8F8;                              // 0x8F8
    u8 pad8FC[0x4]; // 0x8FC
    s64 unk900;                              // 0x900
    int unk908;                              // 0x908
    u8 pad90C[0x4]; // 0x90C
} DWCMatchControl;
// END STRUCT DWCMatchControl

// inferred: one QR2 key added by the game (DWC_AddMatchKeyInt/String)
typedef struct DWCGameMatchKeyData {
    u8 keyID;       // 0x00
    u8 isStr;       // 0x01
    u16 pad;        // 0x02
    char* keyStr;   // 0x04
    void* value;    // 0x08
} DWCGameMatchKeyData;

#define DWC_QR2_GAME_KEY_START 100
#define DWC_QR2_GAME_KEY_NUM 154




// inferred: initialized data at the start of .data; names unknown
static int stUnkData5 = 5;
static int stServerSearchRatio = -2;

static char* stpAddFilter = NULL;
// inferred: callback nesting/free requests of the SB and NN objects; separate statics (not a
// struct): the target only pools their addresses where three or more statics are used
static int stSBBusy = 0;
static int stSBFreeReq = 0;
static int stNNBusy = 0;
static int stNNFreeReq = 0;
static DWCGameMatchKeyData stGameMatchKeys[DWC_QR2_GAME_KEY_NUM] = {0};
static s64 stLastStateTime = 0;
// inferred: defined (as a global) near the end of the file: every function before fn_806CD4E0
// reaches it through its own symbol, never through the pooled .bss base like the statics
extern DWCMatchControl* stpMatchCnt;

// inferred: GameSpy QR2/SB/NN entry points
typedef void (*qr2_serverkeycallback_t)(int keyid, void* outbuf, void* userdata);
typedef void (*qr2_playerteamkeycallback_t)(int keyid, int index, void* outbuf, void* userdata);
typedef void (*qr2_keylistcallback_t)(int keytype, void* keybuffer, void* userdata);
typedef int (*qr2_countcallback_t)(int keytype, void* userdata);
typedef void (*qr2_adderrorcallback_t)(int error, char* errmsg, void* userdata);
int qr2_init_socketA(qr2_t* qrec, int s, int boundport, const char* gamename, const char* secret_key,
                int ispublic, int natnegotiate, qr2_serverkeycallback_t server_key_callback,
                qr2_playerteamkeycallback_t player_key_callback,
                qr2_playerteamkeycallback_t team_key_callback,
                qr2_keylistcallback_t key_list_callback, qr2_countcallback_t playerteam_count_callback,
                qr2_adderrorcallback_t adderror_callback, void* userdata); // qr2_init_socket
void qr2_shutdown(qr2_t qrec);
void qr2_send_statechanged(qr2_t qrec);
void qr2_register_publicaddress_callback(qr2_t qrec, void* callback);
void qr2_register_natneg_callback(qr2_t qrec, void* callback);
void qr2_register_clientmessage_callback(qr2_t qrec, void* callback);
int gt2GetSocketSOCKET(GT2Socket socket);
int gt2GetLocalPort(GT2Socket socket);
void ServerBrowserFree(ServerBrowser sb);                                 // ServerBrowserFree
void NNCancel(int cookie);
void NNFreeNegotiateList(void);                                             // NNFreeNegotiateList

// inferred: dwc_main.c / dwc_login.c / dwc_common.c
u8 fn_806B0E30(void);  // DWC_GetMyAID
u32 fn_806B0F80(void); // DWC_GetAIDBitmap
void fn_806AEB10(DWCError error, int errorCode); // inferred: stops the login
u32 fn_806ABB70(u32 max);                         // inferred: random number below max

ServerBrowser ServerBrowserNewA(const char* queryForGamename, const char* queryFromGamename,
                          const char* queryFromKey, int queryFromVersion, int maxConcurrentUpdates,
                          int queryVersion, int lanBrowse, void* callback, void* instance); // ServerBrowserNew
void fn_806A9CA0(void* dst, const void* src, u32 size); // DWCi_Np_CPUCopy
u16 fn_806A4270(u16 x);                                 // SOHtoNs
char* fn_806A420C(SOInAddr in);                         // SOInetNtoA
void qr2_parse_queryA(qr2_t qrec, u8* query, int len, SOSockAddrIn* sender); // qr2_parse_query
extern u8 NNMagicData[];                               // NNMagicData
void NNProcessData(u8* data, int len, SOSockAddrIn* sender); // NNProcessData
void qr2_keybuffer_add(void* keybuffer, int keyid);
void* fn_806B14D0(u8 aid);                              // DWCi_GetGT2Connection

// EXTERNS
// inferred: per connection info kept by dwc_main (GT2 connection data)
typedef struct DWCConnectionInfo {
    u8 index;      // 0x0
    u8 aid;        // 0x1
    int profileID; // 0x4
} DWCConnectionInfo;
GT2Connection* fn_806B1660(int index);               // inferred: GT2 connection slot
DWCConnectionInfo* fn_806B1710(int index);           // inferred: connection info slot
void fn_806CE700(u8 aid);
BOOL gt2Accept(GT2Connection connection, void* callbacks);
void gt2Reject(GT2Connection connection, const char* message, int len);
void gt2SetConnectionData(GT2Connection connection, void* data);
u16 fn_806A4264(u16 x);                              // SONtoHs
GT2Result gt2Connect(GT2Socket socket, GT2Connection* connection, const char* remoteAddress,
                      const char* message, int len, int timeout, void* callbacks, int blocking); // gt2Connect
u16 gt2GetRemotePort(GT2Connection connection);
void fn_806B0980(void);                              // inferred: server down handling (dwc_main)
void qr2_think(qr2_t qrec);
void gt2Think(GT2Socket socket);
void ServerBrowserThink(ServerBrowser sb);                  // ServerBrowserThink
int ServerBrowserState(ServerBrowser sb);
int NNThink(void);                               // NNThink
// inferred: notification of a new client joining the group
typedef void (*DWCNewClientCallback)(int index, void* param);
GT2Connection* fn_806B1680(int profileID, BOOL unk); // inferred: GT2 connection of profileID
void gt2CloseConnectionHard(GT2Connection connection);
BOOL DWCi_SendReliable(u32 type, u8 aid, const void* buffer, int size);
void fn_806B1A00(u8 aid);                 // inferred: closes the connection to aid
void fn_806CE750(u8 srcAid, u8 dstAid, const void* buffer, int size, int relay);
BOOL gpIsBuddy(GPConnection* connection, int profileID);
int gpSendBuddyMessageA(GPConnection* connection, int profileID, const char* message); // gpSendBuddyMessage
const char* gt2AddressToString(u32 ip, int unk1, int unk2);
int ServerBrowserSendMessageToServerA(ServerBrowser sb, const char* ip, u16 port, const void* data, int len); // ServerBrowserSendMessageToServer
u32 ServerBrowserGetMyPublicIPAddr(ServerBrowser sb);                   // ServerBrowserGetMyPublicIPAddr
u32 fn_806A4260(u32 x);                              // SONtoHl
int ServerBrowserSendNatNegotiateCookieToServerA(ServerBrowser sb, const char* ip, u16 port, int cookie); // ServerBrowserSendNatNegotiateCookieToServer
int NNBeginNegotiationWithSocket(int socket, int cookie, int clientindex, void* progresscallback,
                void* completedcallback, void* userdata); // NNBeginNegotiationWithSocket
const DWCUserData* fn_806AEAF0(void);                // DWCi_GetUserData
int gpGetBuddyIndex(GPConnection* connection, int profileID, int* index);
int gpGetBuddyStatus(GPConnection* connection, int index, GPBuddyStatus* status);
const char* SBServerGetPrivateAddress(SBServer server); // SBServerGetPrivateAddress
u32 SBServerGetPrivateInetAddress(SBServer server);         // SBServerGetPrivateInetAddress
u16 SBServerGetPrivateQueryPort(SBServer server);         // SBServerGetPrivateQueryPort
const char* SBServerGetPublicAddress(SBServer server); // SBServerGetPublicAddress
u32 SBServerGetPublicInetAddress(SBServer server);         // SBServerGetPublicInetAddress
u16 SBServerGetPublicQueryPort(SBServer server);         // SBServerGetPublicQueryPort
int SBServerHasPrivateAddress(SBServer server);         // SBServerHasPrivateAddress
int ServerBrowserCount(ServerBrowser sb);        // ServerBrowserCount
void ServerBrowserRemoveServer(ServerBrowser sb, SBServer server);
void SBServerAddIntKeyValue(SBServer server, const char* key, int value); // SBServerSetIntValue
void ServerBrowserSortA(ServerBrowser sb, int ascending, const char* sortkey, int sorttype); // ServerBrowserSort

void ServerBrowserClear(ServerBrowser sb); // ServerBrowserClear
void qr2_buffer_add_int(void* outbuf, int value);
void qr2_buffer_addA(void* outbuf, const char* value);  // qr2_buffer_add
int ServerBrowserLimitUpdateA(ServerBrowser sb, BOOL async, BOOL disconnectOnComplete, const u8* basicFields,
                int numBasicFields, const char* serverFilter, int maxServers); // ServerBrowserLimitUpdate
int fn_806B0DE0(void);
int fn_806B15B0(void);
int DWCi_GetProfileIDFromList(int index);

static const int stServerSelectWeight[6] = {3, 3, 2, 2, 1, 1};

// BEGIN PROTOS
BOOL DWC_RegisterMatchStatus(void);
BOOL DWC_IsValidCancelMatch(void);
BOOL fn_806B3160(void);
static u8 DWCi_GetNewMatchKey(void);
u8 fn_806B3220(u8 keyID, const char* keyString, const int* valueSrc);
u8 fn_806B3680(u8 keyID, const char* keyString, const char* valueSrc);
int fn_806B3AE0(int index, const char* key, int idefault);
const char* fn_806B3B80(int index, const char* key, const char* sdefault);
u32 fn_806B3C20(void);
BOOL fn_806B3C40(BOOL suspend, void* callback, void* param);
static BOOL DWCi_IsSuspendTarget(void);
BOOL fn_806B3CC0(void);
static void DWCi_ClearGameMatchKeys(void);
void DWCi_MatchInit(DWCMatchControl* matchcnt, GPConnection* pGpObj, GT2Socket* pGt2Socket, void* gt2Callbacks, const char* gameName, const char* secretKey, DWCFriendData* friendList, int friendListLen);
int fn_806B40C0(int unused);
static void DWCi_SBStartup(void);
void fn_806B42B0(int unk8E0, u8 numEntry, const char* addFilter, DWCMatchedCallback callback, void* param, void* unk8A8, void* unk8AC, DWCEvalCallback evalCallback, void* evalParam, DWCResvCallback resvCallback, const u8* unk8EC, void* resvParam);
void fn_806B4940(int unk8E0, u8 numEntry, DWCMatchedCallback callback, void* param, void* unk8A8, void* unk8AC, DWCResvCallback resvCallback, const u8* unk8EC, void* resvParam);
void fn_806B4D20(int unk8E0, int profileID, DWCMatchedCallback callback, void* param, void* unk8A8, void* unk8AC, DWCResvCallback resvCallback, const u8* unk8EC, void* resvParam);
void fn_806B5860(BOOL process);
int fn_806B8430(GT2Socket socket, u32 ip, u16 port, u8* message, int len);
void fn_806B85B0(GT2Socket socket, GT2Connection connection, u32 ip, u16 port, int latency, const char* message, int msgLen);
void fn_806B8D60(GT2Connection connection, GT2Result result, const char* message, int len);
void fn_806B9410(GPConnection* connection, int profileID, char* message);
static void DWCi_CallMatchedCallback(DWCError error, BOOL cancel, BOOL self);
void fn_806B9500(DWCError error, int errorCode);
void fn_806B9740(void);
void fn_806B97B0(u8 aid, u16 type, const u8* message);
inline static void DWCi_ReadSynNodeInfo(DWCNodeInfo* node, const u32* p);
void fn_806B9820(u8 aid, u16 type, const u8* message);
void fn_806BAC80(const u32* data);
BOOL fn_806BAE10(DWCError error, int errorCode);
void fn_806BB3F0(int arg);
static BOOL DWCi_IsMatchLocked(void);
GPResult fn_806BB440(int status);
static void DWCi_FreeAddFilter(void);
void fn_806BB5E0(void);
BOOL fn_806BB6A0(void);
void fn_806BB6C0(int level);
void fn_806BBBA0(u8 matchType, u8 numEntry, DWCMatchedCallback callback, void* param);
void fn_806BBCA0(void);
int DWCi_SBUpdateAsync(int profileID);
static BOOL DWCi_IsPrivateAddress(u32 ip);
inline static int DWCi_DoNatNegotiation(DWCNNData* nnData);
int fn_806BC4C0(int isChild, int cookie, DWCNodeInfo* node);
static BOOL DWCi_IsGT2Target(int profileID);
int fn_806BC860(u8 command, int profileID, u32 ip, u16 port, const u32* data, int len);
inline static const char* DWCi_GetResvDenyReasonString(u32 reason);
inline static void DWCi_TempNewNodeInfo_Clear(void);
BOOL fn_806BCD40(u8 command, int profileID, u32 ip, u16 port, const u32* data, int len);
static BOOL DWCi_CanReserveServer(int profileID);
int fn_806BFBA0(int profileID, u32 ip, u16 port, u8 matchType, BOOL meshReserve, u32 topology, void* userData, BOOL illegal);
int fn_806BFEA0(int profileID, u32 ip, u16 port, u8 matchType, BOOL meshReserve, u32 topology, void* userData, BOOL illegal);
static int DWCi_NodeInfoList_FindAID(u8 aid);
static u8 DWCi_GetNewAID(void);
int fn_806C03B0(int profileID, u32 ip, u16 port);
int fn_806C05F0(int profileID, BOOL delay);
static void DWCi_InitTempNewNodeInfo(int profileID);
int fn_806C0980(BOOL delay, BOOL keepIndex, int prevProfileID);
BOOL fn_806C0C20(int prevProfileID);
static BOOL DWCi_SendCancelMatchSynCommand(int profileID, u8 type);
BOOL fn_806C10C0(int profileID, int level);
static int DWCi_SendResvCancelCommand(void);
BOOL fn_806C1430(int profileID);
void fn_806C1AC0(int level);
static void DWCi_NodeInfoList_Add(const DWCNodeInfo* node);
static void DWCi_NodeInfoList_InsertTop(const DWCNodeInfo* node);
void fn_806C2880(void);
BOOL fn_806C2F70(void);
BOOL fn_806C3270(void);
void fn_806C3670(void);
static void DWCi_FinishCancelMatching(void);
void fn_806C3ED0(int level);
static u8 DWCi_GetAIDFromProfileID(int profileID);
BOOL fn_806C4430(u32 bitmap);
void fn_806C4740(u8 aid, u16 type);
static void DWCi_SendTypeMatchAck(void);
BOOL fn_806C4B30(void);
BOOL fn_806C50A0(int profileID, u8 command, int friendIdx);
BOOL fn_806C57F0(void);
int fn_806C5C10(GPResult result);
int fn_806C5EB0(int result);
int fn_806C6180(int result);
int fn_806C6470(int result);
inline static int DWCi_GetValidServerPID(SBServer server);
inline static void DWCi_SetTempNewNodeInfoFromServer(SBServer server);
void DWCi_SBCallback(ServerBrowser sb, int reason, SBServer server, void* instance);
void fn_806C8700(SBServer server);
BOOL fn_806C89D0(BOOL sort);
void fn_806C8BE0(void);
static const char* DWCi_GetQR2KeyName(int keyid);
void fn_806C8DB0(int keyid, void* outbuf, void* userdata);
static void fn_806C9670(int keyid, int index, void* outbuf, void* userdata);
static void fn_806C9680(int keyid, int index, void* outbuf, void* userdata);
void fn_806C9690(int keytype, void* keybuffer, void* userdata);
static int fn_806C9790(int keytype, void* userdata);
void fn_806C97A0(int error, char* errmsg, void* userdata);
void fn_806C9810(unsigned int ip, unsigned short port, void* userdata);
static int DWCi_HandleNNError(int result);
void fn_806C9880(int cookie, void* userdata);
void fn_806C9CE0(const u8* message, u32 size);
void fn_806C9E60(const u8* message, int len, void* userdata);
void fn_806CA020(int state, void* userdata);
inline static BOOL DWCi_CountNNFailure(void* remoteaddr);
void fn_806CA040(int result, int socket, void* remoteaddr, void* userdata);
static int DWCi_HandleMatchCommandError(int result);
BOOL fn_806CB3A0(void);
BOOL fn_806CB4C0(void);
void fn_806CB550(BOOL newGroup);
int fn_806CB8F0(BOOL force);
BOOL fn_806CBB20(u8 aid, int suspend, int fromCommand);
BOOL fn_806CC020(void);
static void DWCi_SetSuspendRequest(int suspend, DWCSuspendCallback callback, void* param);
BOOL fn_806CC550(int suspend, DWCSuspendCallback callback, void* param);
BOOL fn_806CC6D0(void);
BOOL fn_806CC850(int suspend);
BOOL fn_806CCB40(void);
void fn_806CCB70(DWCMatchState state);
BOOL fn_806CCC70(void);
void fn_806CCC90(void);
int fn_806CCCB0(void);
BOOL fn_806CCCC0(u8 aid);
BOOL fn_806CCDB0(int profileID);
DWCNodeInfo* fn_806CCEA0(void);
DWCNodeInfo* fn_806CCED0(int index);
DWCNodeInfo* fn_806CCF00(int profileID);
DWCNodeInfo* fn_806CCF50(u8 aid);
s64 fn_806CCFA0(void);
BOOL fn_806CD150(void);
BOOL fn_806CD3C0(void);
u8 fn_806CD3E0(const GPBuddyStatus* status);
void fn_806CD450(int profileID);
void fn_806CD460(void);
void fn_806CD4E0(void);
// END PROTOS

// inferred: defined at the end of the file (its strings follow all the others in .data), so it is
// declared here and cannot be static
extern char* stStateNames[DWC_MATCH_STATE_NUM];

// ---- functions ----
// @806B3100
// stripped by the linker; kept for its strings
BOOL DWC_RegisterMatchStatus(void) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_RegisterMatchStatus() was called!!\n");
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "But ignored.\n");
    return FALSE;
}

// @806B3150
BOOL DWC_IsValidCancelMatch(void) {
    if (DWCi_IsError()) {
        return FALSE;
    }
    if (stpMatchCnt == NULL || fn_806B1230() != 4 || stpMatchCnt->unk752) {
        return FALSE;
    }
    return TRUE;
}

// @806B3160
BOOL fn_806B3160(void) {
    BOOL result;
    if (DWC_IsValidCancelMatch()) {
        fn_806C3670();
        result = TRUE;
    } else {
        if (!stpMatchCnt->cancelSilent) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Now unable to cancel.\n");
        }
        result = FALSE;
    }
    stpMatchCnt->cancelSilent = FALSE;
    return result;
}

// @806B3170
static u8 DWCi_GetNewMatchKey(void) {
    u8 i;
    for (i = 0; i < DWC_QR2_GAME_KEY_NUM; i++) {
        if (!stGameMatchKeys[i].keyID) {
            return (u8)(i + DWC_QR2_GAME_KEY_START);
        }
    }
    return 0;
}

// @806B3220
u8 fn_806B3220(u8 keyID, const char* keyString, const int* valueSrc) {
    if (keyString == NULL || valueSrc == NULL) {
        return 0;
    }
    if (keyID >= DWC_QR2_GAME_KEY_START && stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyID) {
        if (stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr != NULL &&
            strcmp(stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr, keyString) != 0) {
            return 0;
        }
    } else {
        keyID = DWCi_GetNewMatchKey();
        if (keyID == 0) {
            return 0;
        }
    }
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyID = keyID;
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].isStr = FALSE;
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].pad = 0;
    if (stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr != NULL) {
        DWC_Free(DWC_ALLOCTYPE_BASE, stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr, 0);
    }
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr =
        DWC_Alloc(DWC_ALLOCTYPE_BASE, strlen(keyString) + 1);
    if (stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr == NULL) {
        fn_806B9500(DWC_ERROR_FATAL, -80001);
        return 0;
    }
    strcpy(stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr, keyString);
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_AddMatchKeyInt: key='%s', value=%d\n", keyString, *valueSrc);
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].value = (void*)valueSrc;
    qr2_register_keyA(keyID, stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr);
    return keyID;
}

// @806B3680
u8 fn_806B3680(u8 keyID, const char* keyString, const char* valueSrc) {
    if (keyString == NULL || valueSrc == NULL) {
        return 0;
    }
    if (keyID >= DWC_QR2_GAME_KEY_START && stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyID) {
        if (stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr != NULL &&
            strcmp(stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr, keyString) != 0) {
            return 0;
        }
    } else {
        keyID = DWCi_GetNewMatchKey();
        if (keyID == 0) {
            return 0;
        }
    }
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyID = keyID;
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].isStr = TRUE;
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].pad = 0;
    if (stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr != NULL) {
        DWC_Free(DWC_ALLOCTYPE_BASE, stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr, 0);
    }
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr =
        DWC_Alloc(DWC_ALLOCTYPE_BASE, strlen(keyString) + 1);
    if (stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr == NULL) {
        fn_806B9500(DWC_ERROR_FATAL, -80001);
        return 0;
    }
    strcpy(stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr, keyString);
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_AddMatchKeyString: key='%s' value='%s'\n", keyString, valueSrc);
    stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].value = (void*)valueSrc;
    qr2_register_keyA(keyID, stGameMatchKeys[keyID - DWC_QR2_GAME_KEY_START].keyStr);
    return keyID;
}

// @806B3AE0
int fn_806B3AE0(int index, const char* key, int idefault) {
    SBServer server;
    if (stpMatchCnt == NULL || DWCi_IsError()) {
        return idefault;
    }
    server = ServerBrowserGetServer(stpMatchCnt->sbObj, index);
    if (server != NULL) {
        return SBServerGetIntValueA(server, key, idefault);
    }
    return idefault;
}

// @806B3B80
const char* fn_806B3B80(int index, const char* key, const char* sdefault) {
    SBServer server;
    if (stpMatchCnt == NULL || DWCi_IsError()) {
        return sdefault;
    }
    server = ServerBrowserGetServer(stpMatchCnt->sbObj, index);
    if (server != NULL) {
        return SBServerGetStringValueA(server, key, sdefault);
    }
    return sdefault;
}

// @806B3C20
u32 fn_806B3C20(void) {
    if (stpMatchCnt == NULL) {
        return 0;
    }
    return stpMatchCnt->groupID;
}

// @806B3C40
BOOL fn_806B3C40(BOOL suspend, void* callback, void* param) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "!!DWC_RequestSuspendMatchAsync(%s) was called!!\n",
               suspend ? "TRUE" : "FALSE");
    return fn_806CC550(suspend, callback, param);
}

// @806B3CC0
static BOOL DWCi_IsSuspendTarget(void) {
    if (fn_806B12F0()) {
        if (stpMatchCnt->state == DWC_MATCH_STATE_SV_WAITING ||
            stpMatchCnt->state == DWC_MATCH_STATE_SEARCH_OWN) {
            return TRUE;
        }
        return FALSE;
    }
    return TRUE;
}

BOOL fn_806B3CC0(void) {
    int result = -1;
    u32 bitmap;
    if (stpMatchCnt == NULL) {
        return FALSE;
    }
    if (!stpMatchCnt->qr2IsReserved) {
        return FALSE;
    }
    if (!fn_806B0DE0()) {
        return FALSE;
    }
    bitmap = stpMatchCnt->suspendBitmap;
    bitmap &= fn_806B0F80();
    if (!DWCi_IsSuspendTarget()) {
        bitmap &= ~(1 << fn_806B0E30());
    }
    if ((bitmap ^ fn_806B0F80()) == 0) {
        result = 1;
    } else if ((bitmap & fn_806B0F80()) == 0) {
        result = 0;
    }
    if (result != -1) {
        stpMatchCnt->suspended = (result == 1);
    }
    return stpMatchCnt->suspended;
}

// @806B3DF0
static void DWCi_ClearGameMatchKeys(void) {
    int i;
    for (i = 0; i < DWC_QR2_GAME_KEY_NUM; i++) {
        if (stGameMatchKeys[i].keyStr != NULL) {
            DWC_Free(DWC_ALLOCTYPE_BASE, stGameMatchKeys[i].keyStr, 0);
        }
    }
    memset(stGameMatchKeys, 0, sizeof(stGameMatchKeys));
}

// NONMATCHING: 99.44% - register allocation of the inlined DWCi_SetMatchStatus (oldState/stLastStateTime address swapped)
void DWCi_MatchInit(DWCMatchControl* matchcnt, GPConnection* pGpObj, GT2Socket* pGt2Socket,
                 void* gt2Callbacks, const char* gameName, const char* secretKey, DWCFriendData* friendList,
                 int friendListLen) {
    stpMatchCnt = matchcnt;
    stLastStateTime = DWCi_Np_GetTick();
    stpMatchCnt->pGpObj = pGpObj;
    stpMatchCnt->pGt2Socket = pGt2Socket;
    stpMatchCnt->gt2Callbacks = gt2Callbacks;
    stpMatchCnt->qr2Obj = NULL;
    stpMatchCnt->qr2IP = 0;
    stpMatchCnt->qr2Port = 0;
    stpMatchCnt->sbObj = NULL;
    fn_806CCB70(DWC_MATCH_STATE_INIT);
    stpMatchCnt->unk19 = 0;
    stpMatchCnt->profileID = 0;
    stpMatchCnt->gameName = gameName;
    stpMatchCnt->secretKey = secretKey;
    stpMatchCnt->friendList = friendList;
    stpMatchCnt->friendListLen = friendListLen;
    memset(stpMatchCnt->friendIdxList, 0, sizeof(stpMatchCnt->friendIdxList));
    stpMatchCnt->friendIdxListLen = 0;
    stpMatchCnt->matchedCallback = NULL;
    stpMatchCnt->matchedParam = NULL;
    stpMatchCnt->evalCallback = NULL;
    stpMatchCnt->evalParam = NULL;
    stpMatchCnt->unk8B8 = 0;
    stpMatchCnt->unk8BC = 0;
    stpMatchCnt->groupID = 0;
    stpMatchCnt->unk8C4 = 0;
    stpMatchCnt->unk8C8 = 0;
    stpMatchCnt->qr2MatchSubType = 0;
    stpMatchCnt->qr2IsReserved = 0;
    stpMatchCnt->unk718 = 0;
    stpMatchCnt->unk8F8 = 0;
    stpMatchCnt->unk908 = 1;
    stpMatchCnt->cancelSilent = 0;
    stpMatchCnt->unk8E6 = 0;
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout reset.(%s)\n", __FUNCTION__);
    stpMatchCnt->unk8E0 = 0;
    fn_806CCC90();
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    DWCi_ClearGameMatchKeys();
    fn_806BB6C0(0);
}

// @806B40C0
// NONMATCHING: 99.34% - register allocation / small scheduling differences
int fn_806B40C0(int unused) {
    DWCMatchControl* cnt = stpMatchCnt;
    int result;
    int retry = 0;
    if (cnt->qr2Obj != NULL) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "QR2 is already set up.\n");
        return 0;
    }
    for (retry = 0; retry < 1; retry++) {
        result = qr2_init_socketA(&cnt->qr2Obj, gt2GetSocketSOCKET(*cnt->pGt2Socket),
                             (u16)gt2GetLocalPort(*cnt->pGt2Socket), cnt->gameName, cnt->secretKey, 1, 1,
                             fn_806C8DB0, fn_806C9670, fn_806C9680, fn_806C9690, fn_806C9790,
                             fn_806C97A0, NULL);
        if (result == 0) {
            break;
        }
        qr2_shutdown(stpMatchCnt->qr2Obj);
        stpMatchCnt->qr2Obj = NULL;
        if (result != 3 || retry == 0) {
            fn_806C6180(result);
            return result;
        }
    }
    stpMatchCnt->qr2IP = 0;
    stpMatchCnt->qr2Port = 0;
    qr2_register_publicaddress_callback(stpMatchCnt->qr2Obj, fn_806C9810);
    qr2_register_natneg_callback(stpMatchCnt->qr2Obj, fn_806C9880);
    qr2_register_clientmessage_callback(stpMatchCnt->qr2Obj, fn_806C9E60);
    fn_806CD460();
    qr2_send_statechanged(stpMatchCnt->qr2Obj);
    return result;
}

// @806B42A0
static void DWCi_SBStartup(void) {
    DWCMatchControl* cnt = stpMatchCnt;
    if (cnt->sbObj == NULL) {
        cnt->sbObj = ServerBrowserNewA(cnt->gameName, cnt->gameName, cnt->secretKey, 0, 20, 1, 0,
                                 DWCi_SBCallback, NULL);
    }
}

// @806B42B0
// NONMATCHING: 98.69% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806B42B0(int unk8E0, u8 numEntry, const char* addFilter, DWCMatchedCallback callback,
                 void* param, void* unk8A8, void* unk8AC, DWCEvalCallback evalCallback,
                 void* evalParam, DWCResvCallback resvCallback, const u8* unk8EC, void* resvParam) {
    char filter[512];
    char hostState[512];
    int size;
    DWCi_FreeAddFilter();
    if (addFilter != NULL) {
        snprintf(hostState, 0x1ff, "(%s = %u or %s = %u)", "dwc_hoststate", 0, "dwc_hoststate", 2);
        snprintf(filter, 0x1ff,
                 "%s = %d and %s != %u and maxplayers = %d and numplayers < %d and %s = %d and %s = %u and %s",
                 "dwc_mver", 90, "dwc_pid", -1, 32, 32, "dwc_mtype", 3, "dwc_suspend", 0, hostState);
        size = 0x1ff - strlen(filter) - strlen(" and ()");
        stpAddFilter = DWC_Alloc(DWC_ALLOCTYPE_BASE, size);
        if (stpAddFilter == NULL) {
            fn_806B9500(DWC_ERROR_FATAL, -80001);
            return;
        }
        fn_806A9CA0(stpAddFilter, addFilter, size);
        stpAddFilter[size - 1] = '\0';
    }
    fn_806BBBA0(0, numEntry, callback, param);
    stpMatchCnt->unk8A8 = unk8A8;
    stpMatchCnt->unk8AC = unk8AC;
    stpMatchCnt->evalCallback = evalCallback;
    stpMatchCnt->evalParam = evalParam;
    stpMatchCnt->unk8E0 = unk8E0;
    stpMatchCnt->resvCallback = resvCallback;
    if (unk8EC != NULL) {
        memcpy(stpMatchCnt->unk8EC, unk8EC, sizeof(stpMatchCnt->unk8EC));
    } else {
        memset(stpMatchCnt->unk8EC, 0, sizeof(stpMatchCnt->unk8EC));
    }
    stpMatchCnt->resvParam = resvParam;
    fn_806CCB70(DWC_MATCH_STATE_SEARCH_OWN);
    DWCi_SBStartup();
    if (stpMatchCnt->sbObj == NULL && fn_806C5EB0(5)) {
        return;
    }
    fn_806BB440(3);
    if (stpMatchCnt->qr2Obj == NULL) {
        if (fn_806B40C0(stpMatchCnt->profileID)) {
            return;
        }
    } else {
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
    }
    if (fn_806C5EB0(DWCi_SBUpdateAsync(stpMatchCnt->profileID))) {
        return;
    }
}

// @806B4940
// NONMATCHING: 97.28% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806B4940(int unk8E0, u8 numEntry, DWCMatchedCallback callback, void* param,
                 void* unk8A8, void* unk8AC, DWCResvCallback resvCallback, const u8* unk8EC,
                 void* resvParam) {
    u32 groupID;
    fn_806BBBA0(2, numEntry, callback, param);
    stpMatchCnt->unk8A8 = unk8A8;
    stpMatchCnt->unk8AC = unk8AC;
    stpMatchCnt->unk8E0 = unk8E0;
    stpMatchCnt->resvCallback = resvCallback;
    if (unk8EC != NULL) {
        memcpy(stpMatchCnt->unk8EC, unk8EC, sizeof(stpMatchCnt->unk8EC));
    } else {
        memset(stpMatchCnt->unk8EC, 0, sizeof(stpMatchCnt->unk8EC));
    }
    stpMatchCnt->resvParam = resvParam;
    fn_806CCB70(DWC_MATCH_STATE_SEARCH_OWN);
    if (fn_806C5C10(fn_806BB440(6))) {
        return;
    }
    stpMatchCnt->qr2IsReserved = 2;
    if (stpMatchCnt->qr2Obj != NULL) {
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
    }
    stpMatchCnt->unk8F4 = 1;
    if (stpMatchCnt->qr2Obj == NULL && fn_806B40C0(stpMatchCnt->profileID)) {
        return;
    }
    groupID = stpMatchCnt->profileID + (fn_806ABB70(10000000) + 100000000);
    DWC_Printf(DWC_REPORTFLAG_INFO, "Create New Group ID:%u\n", groupID);
    stpMatchCnt->groupID = groupID;
    DWCi_SBStartup();
    qr2_send_statechanged(stpMatchCnt->qr2Obj);
    if (fn_806C5EB0(DWCi_SBUpdateAsync(stpMatchCnt->profileID))) {
        return;
    }
}

// @806B4D20
// NONMATCHING: 99.95% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806B4D20(int unk8E0, int profileID, DWCMatchedCallback callback, void* param, void* unk8A8,
                 void* unk8AC, DWCResvCallback resvCallback, const u8* unk8EC, void* resvParam) {
    DWCNodeInfo tempNode;
    DWCNodeInfo myNode;
    DWCNodeInfo zeroNode;
    u8 save753;
    int result;
    if (stpMatchCnt->unk8C8) {
        tempNode = stpMatchCnt->tempNewNodeInfo;
        myNode = *fn_806CCF00(stpMatchCnt->profileID);
        save753 = stpMatchCnt->unk753;
    }
    fn_806BBBA0(3, 0, callback, param);
    stpMatchCnt->unk8E0 = unk8E0;
    if (!stpMatchCnt->unk8C8) {
        memset(&zeroNode, 0, sizeof(DWCNodeInfo));
        DWCi_InitTempNewNodeInfo(profileID);
    } else {
        stpMatchCnt->tempNewNodeInfo = tempNode;
        DWCi_NodeInfoList_InsertTop(&myNode);
        fn_806A9CA0(stpMatchCnt->unk8EC, &myNode.unk28, sizeof(stpMatchCnt->unk8EC));
        stpMatchCnt->unk753 = save753;
    }
    stpMatchCnt->unk8A8 = unk8A8;
    stpMatchCnt->unk8AC = unk8AC;
    stpMatchCnt->resvCallback = resvCallback;
    if (unk8EC != NULL) {
        memcpy(stpMatchCnt->unk8EC, unk8EC, sizeof(stpMatchCnt->unk8EC));
    } else {
        memset(stpMatchCnt->unk8EC, 0, sizeof(stpMatchCnt->unk8EC));
    }
    stpMatchCnt->resvParam = resvParam;
    stpMatchCnt->unk18 = 1;
    stpMatchCnt->unk700 = stpMatchCnt->profileID;
    stpMatchCnt->qr2IsReserved = 1;
    if (stpMatchCnt->qr2Obj != NULL) {
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
    }
    if (!stpMatchCnt->unk8C8) {
        fn_806CCB70(DWC_MATCH_STATE_SEARCH_OWN);
    }
    DWCi_SBStartup();
    if (stpMatchCnt->sbObj == NULL && fn_806C5EB0(5)) {
        return;
    }
    if (fn_806C5C10(fn_806BB440(5))) {
        return;
    }
    if (stpMatchCnt->qr2Obj == NULL) {
        if (fn_806B40C0(stpMatchCnt->profileID)) {
            return;
        }
    } else {
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
    }
    if (!stpMatchCnt->unk8C8) {
        if (!fn_806C5EB0(DWCi_SBUpdateAsync(stpMatchCnt->profileID))) {
            return;
        }
        return;
    }
    qr2_send_statechanged(stpMatchCnt->qr2Obj);
    fn_806CCB70(DWC_MATCH_STATE_CL_WAIT_RESV);
    result = fn_806C05F0(profileID, FALSE);
    if (stpMatchCnt->qr2MatchSubType == 0) {
        fn_806C5EB0(result);
    } else if (fn_806C5C10(result)) {
        return;
    }
}

// @806B5860
// NONMATCHING: 93.77% - large state machine; register allocation, scheduling and the inlined status-string stack layout differ
void fn_806B5860(BOOL process) {
    DWCMatchControl* cnt;
    int level;
    int i;
    if (stpMatchCnt == NULL || DWCi_IsError()) {
        return;
    }
    if (!process) {
        if (stpMatchCnt->qr2Obj != NULL) {
            qr2_think(stpMatchCnt->qr2Obj);
        }
        if (stpMatchCnt->pGt2Socket != NULL) {
            gt2Think(*stpMatchCnt->pGt2Socket);
        }
        return;
    }
    cnt = stpMatchCnt;
    if (cnt->state == DWC_MATCH_STATE_INIT) {
        return;
    }
    switch (cnt->state) {
    case DWC_MATCH_STATE_CL_WAIT_RESV:
        if (cnt->unk770 &&
            OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk778) > (u32)stpMatchCnt->unk770) {
            stpMatchCnt->unk770 = 0;
            if (stpMatchCnt->unk8C8) {
                if (stpMatchCnt->unk8E6) {
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "GroupID Timeout: wait server response %d/%d.\n",
                               stpMatchCnt->unk753, stpMatchCnt->unk8E6);
                }
                if (++stpMatchCnt->unk753 > stpMatchCnt->unk8E6) {
                    fn_806B9500((DWCError)25, -80432);
                    return;
                }
                fn_806C0C20(0);
                return;
            }
            if (fn_806CD150() == TRUE) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh NN resv timed out.\n");
                fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
                break;
            }
            if (stpMatchCnt->qr2MatchSubType == 3) {
                if (stpMatchCnt->unk8E6) {
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout: wait server response %d/%d.\n",
                               stpMatchCnt->unk753, stpMatchCnt->unk8E6);
                }
                if (++stpMatchCnt->unk753 > stpMatchCnt->unk8E6) {
                    fn_806B9500((DWCError)25, -80430);
                    return;
                }
                if (DWCi_HandleMatchCommandError(fn_806C05F0(stpMatchCnt->tempNewNodeInfo.profileID, FALSE))) {
                    return;
                }
            } else if (stpMatchCnt->qr2MatchSubType == 1) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "<FRIEND> NN resv(with %u) timed out. Try next server.\n",
                           stpMatchCnt->tempNewNodeInfo.profileID);
                memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
                if (!fn_806C0C20(0)) {
                    return;
                }
            } else {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN resv(with %u) timed out. Try next server.\n",
                           stpMatchCnt->tempNewNodeInfo.profileID);
                memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
                if (!fn_806C0C20(0)) {
                    return;
                }
            }
        }
        cnt = stpMatchCnt;
        if (cnt->unk760 && OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk768) >= 3000) {
            if (DWCi_HandleMatchCommandError(fn_806C05F0(stpMatchCnt->tempNewNodeInfo.profileID, FALSE))) {
                return;
            }
        }
        break;
    case DWC_MATCH_STATE_CL_SEARCH_HOST:
    case DWC_MATCH_STATE_CL_SEARCH_EVAL_HOST:
    case DWC_MATCH_STATE_SEARCH_OWN:
        if (cnt->unk708 > 0) {
            u32 timeout;
            if (cnt->state == DWC_MATCH_STATE_CL_SEARCH_HOST) {
                if (cnt->unk708 == 1 || (u32)cnt->unk8DC <= 1) {
                    timeout = 4000;
                } else {
                    timeout = 12000;
                }
            } else if (cnt->state == DWC_MATCH_STATE_SEARCH_OWN) {
                timeout = 4000;
            } else {
                timeout = 12000;
                if (cnt->unk708 == 1) {
                    timeout = 4000;
                }
            }
            if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk710) > timeout) {
                if (fn_806C5EB0(DWCi_SBUpdateAsync(stpMatchCnt->unk7AC))) {
                    return;
                }
                stpMatchCnt->unk708 = 0;
            }
        }
        break;
    case DWC_MATCH_STATE_CL_GT2:
        if (cnt->unk730) {
            if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk730) > 25000) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout: wait gt2Connect().\n");
                stpMatchCnt->unk730 = 0;
                if (fn_806CD150() == TRUE) {
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh NN resv timed out.\n");
                    fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
                    break;
                }
                if (fn_806C1430(stpMatchCnt->tempNewNodeInfo.profileID)) {
                    break;
                }
                memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
                return;
            }
        } else if (cnt->resend.command == 6 && OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->resend.time) > 6000) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "RTT Timeout with DWC_MATCH_STATE_CL_GT2.\n");
            if (fn_806CD150() == TRUE) {
                stpMatchCnt->resend.command = 0xFF;
                stpMatchCnt->resend.unk1 = 0;
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Stop resending command %d.\n", 6);
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh NN resv timed out.\n");
                fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
                break;
            }
            if (++stpMatchCnt->resend.unk1 > 5) {
                stpMatchCnt->resend.command = 0xFF;
                stpMatchCnt->resend.unk1 = 0;
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Stop resending command %d.\n", 6);
                if (fn_806C1430(stpMatchCnt->tempNewNodeInfo.profileID)) {
                    break;
                }
                memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
                return;
            }
            cnt = stpMatchCnt;
            if (DWCi_HandleMatchCommandError(fn_806BC860(6, cnt->resend.profileID, cnt->resend.ip, cnt->resend.port,
                                                         cnt->resend.data, cnt->resend.len))) {
                return;
            }
        }
        break;
    case DWC_MATCH_STATE_SV_OWN_NN:
        if (cnt->resend.command != 2) {
            break;
        }
        if ((cnt->qr2MatchSubType == 0 && OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->resend.time) > 6000) ||
            (stpMatchCnt->qr2MatchSubType != 0 &&
             OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - stpMatchCnt->resend.time) > 19000)) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Reservation timeout. Cancel reservation.\n");
            stpMatchCnt->resend.command = 0xFF;
            stpMatchCnt->resend.unk1 = 0;
            if (fn_806CD150() == TRUE) {
                fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
                break;
            }
            level = 2;
            if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) && !stpMatchCnt->unkD) {
                level = 1;
            }
            if (!fn_806C10C0(stpMatchCnt->tempNewNodeInfo.profileID, level)) {
                return;
            }
        }
        break;
    case DWC_MATCH_STATE_SV_WAIT_CL_LINK:
        if (cnt->resend.command != 8) {
            break;
        }
        if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->resend.time) > 30000) {
            if (++stpMatchCnt->resend.unk1 > 0) {
                stpMatchCnt->resend.command = 0xFF;
                stpMatchCnt->resend.unk1 = 0;
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Wait clients connecting timeout.\n");
                level = 2;
                if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) &&
                    !stpMatchCnt->unkD) {
                    level = 1;
                }
                if (!fn_806C10C0(stpMatchCnt->tempNewNodeInfo.profileID, level)) {
                    return;
                }
            }
        }
        break;
    case DWC_MATCH_STATE_CL_WAITING:
        if (fn_806B1230() != 4) {
            break;
        }
        cnt = stpMatchCnt;
        if (!cnt->unk757) {
            break;
        }
        if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk7A0) > 30000) {
            cnt = stpMatchCnt;
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "No data from server %d/%d.\n", cnt->unk756, 5);
            if (stpMatchCnt->unk756 >= 5) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout: Connection to server was shut down.\n");
                if (fn_806C1430(cnt->tempNewNodeInfo.profileID)) {
                    break;
                }
                return;
            }
            if (DWCi_HandleMatchCommandError(fn_806BC860(0x40, cnt->tempNewNodeInfo.profileID,
                                                         cnt->tempNewNodeInfo.ip, cnt->tempNewNodeInfo.port, NULL,
                                                         0))) {
                return;
            }
            stpMatchCnt->unk756++;
            stpMatchCnt->unk7A0 = DWCi_Np_GetTick() - OS_MSEC_TO_TICKS(24000);
        }
        break;
    case DWC_MATCH_STATE_CL_SVDOWN_1: {
        DWCNodeInfo* myNode;
        fn_806CD150();
        cnt = stpMatchCnt;
        cnt->unk8B8 = 0;
        cnt->unk8CC = 0;
        cnt->unk8CD = 0;
        if (!fn_806CB4C0() && !fn_806B14D0(fn_806CCED0(0)->aid)) {
            DWC_Printf(DWC_REPORTFLAG_INFO,
                       "[shutdown myself] It doesn't have the GT2 connection with next server.\n");
            fn_806B9500((DWCError)25, -80431);
            return;
        }
        myNode = fn_806CCF00(stpMatchCnt->profileID);
        cnt->unk8B8 |= 1 << (myNode == NULL ? 0xFF : myNode->aid);
        if (fn_806CB4C0()) {
            if (stpMatchCnt->unk8E0 == 1) {
                fn_806B9500((DWCError)25, -80431);
                break;
            }
            DWC_Printf(DWC_REPORTFLAG_INFO, "Reserve: I am Next Server!!\n");
            fn_806CB3A0();
            fn_806CCB70(DWC_MATCH_STATE_CL_SVDOWN_2);
            qr2_send_statechanged(stpMatchCnt->qr2Obj);
            stpMatchCnt->unk756 = 0;
            stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
        } else {
            qr2_send_statechanged(stpMatchCnt->qr2Obj);
            fn_806CCB70(DWC_MATCH_STATE_CL_SVDOWN_2);
            stpMatchCnt->unk756 = 0;
            stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
        }
        break;
    }
    case DWC_MATCH_STATE_CL_SVDOWN_2:
        if (fn_806CB4C0()) {
            cnt = stpMatchCnt;
            if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk7A0) > 5000) {
                fn_806CB3A0();
                if (++stpMatchCnt->unk8CC > 5) {
                    u32 noResponse = ~(stpMatchCnt->unk8B8 & stpMatchCnt->unk8BC);
                    for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
                        DWCNodeInfo* node = fn_806CCED0(i);
                        if (noResponse & (1 << node->aid)) {
                            DWC_Printf(DWC_REPORTFLAG_INFO,
                                       "**** [SV Changing] shutdown no response host(PID:%u aid:%d)\n",
                                       node->profileID, node->aid);
                            fn_806B1A00(node->aid);
                        }
                    }
                    stpMatchCnt->unk8B8 = stpMatchCnt->unk8BC;
                }
            }
            if (stpMatchCnt->unk8B8 == stpMatchCnt->unk8BC) {
                fn_806CCB70(DWC_MATCH_STATE_CL_SVDOWN_3);
            }
        } else {
            cnt = stpMatchCnt;
            if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk7A0) > 60000) {
                DWC_Printf(DWC_REPORTFLAG_INFO, "Backup message timed out.\n");
                fn_806B9500((DWCError)25, -80431);
            }
        }
        break;
    case DWC_MATCH_STATE_CL_SVDOWN_3:
        if (fn_806CB4C0()) {
            if (stpMatchCnt->unk8E0 == 1) {
                fn_806B9500((DWCError)25, -80431);
                break;
            }
            DWC_Printf(DWC_REPORTFLAG_INFO, "Server Down Detected!!\n");
            fn_806CB550(FALSE);
            DWC_Printf(DWC_REPORTFLAG_INFO, "I am New Server!!\n");
            stpMatchCnt->unk8D0 = 0;
            for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
                fn_806CE700(fn_806CCED0(i)->aid);
            }
            cnt = stpMatchCnt;
            if (cnt->suspendReq != 0xFF) {
                if (cnt->suspendReq == 1) {
                    cnt->suspendBitmap = fn_806B0F80();
                } else {
                    cnt->suspendBitmap = 0;
                }
                DWC_Printf(DWC_REPORTFLAG_INFO, "DWCi_AutoSuspendComplete - suspend is %s.\n",
                           stpMatchCnt->suspendReq ? "TRUE" : "FALSE");
                if (stpMatchCnt->suspendCallback != NULL) {
                    stpMatchCnt->suspendCallback(DWC_ERROR_NONE, stpMatchCnt->suspendReq,
                                                 stpMatchCnt->suspendParam);
                }
                qr2_send_statechanged(stpMatchCnt->qr2Obj);
                stpMatchCnt->suspendReq = 0xFF;
                stpMatchCnt->suspending = 0;
            }
            qr2_send_statechanged(stpMatchCnt->qr2Obj);
        } else {
            cnt = stpMatchCnt;
            if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk7A0) > 20000) {
                fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
            }
        }
        break;
    case DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST:
        if (cnt->unk708 > 0 && OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk710) > 8000) {
            if (++stpMatchCnt->unk8C4 > 15) {
                fn_806B9500((DWCError)25, -80432);
                break;
            }
            DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "Retry search host by groupID[%d/%d].\n", stpMatchCnt->unk8C4, 15);
            if (fn_806C5EB0(DWCi_SBUpdateAsync(0))) {
                return;
            }
            stpMatchCnt->unk708 = 0;
        }
        break;
    }
    cnt = stpMatchCnt;
    if ((cnt->state == DWC_MATCH_STATE_SV_OWN_NN || cnt->state == DWC_MATCH_STATE_CL_NN) && cnt->unk728 &&
        OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk728) > 10000) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout : wait NN retry.\n");
        fn_806CA040(1, 0, NULL, &stpMatchCnt->nnData);
    }
    if (stpMatchCnt->unk8E0 == 0 && !stpMatchCnt->unk6C0 && stpMatchCnt->state == DWC_MATCH_STATE_CL_WAITING &&
        !stpMatchCnt->unk690.profileID && stpMatchCnt->nodeInfoList.nodeNum > 2) {
        for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
            DWCNodeInfo* node = fn_806CCED0(i);
            if (fn_806CCF00(stpMatchCnt->profileID) == node || fn_806B14D0(node->aid)) {
                continue;
            }
            if (node->unk20 == 0) {
                node->unk20 = fn_806CCFA0();
            }
            if (node->unk20 < OSGetTime() && node->unk18 <= stUnkData5) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** TRY mesh create: aid(%d)\n", node->aid);
                node->unk20 = fn_806CCFA0();
                stpMatchCnt->tempNewNodeInfo = *node;
                stpMatchCnt->unk6C0 = 1;
                if (DWCi_HandleMatchCommandError(fn_806C05F0(node->profileID, FALSE))) {
                    stpMatchCnt->unk6C0 = 0;
                    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
                } else {
                    stpMatchCnt->unk6C4 = 0;
                    memset(&stpMatchCnt->unk6C8, 0, sizeof(DWCNodeInfo));
                    fn_806CCB70(DWC_MATCH_STATE_CL_WAIT_RESV);
                }
                break;
            }
        }
    }
    if (stpMatchCnt->sbObj != NULL) {
        stSBBusy = 0;
        stSBFreeReq = 0;
        stNNBusy = 1;
        ServerBrowserThink(stpMatchCnt->sbObj);
        if (stSBFreeReq) {
            ServerBrowserFree(stpMatchCnt->sbObj);
            stpMatchCnt->sbObj = NULL;
        }
    }
    if (stpMatchCnt->sbObj != NULL && ServerBrowserState(stpMatchCnt->sbObj)) {
        cnt = stpMatchCnt;
        if (cnt->unk718 && cnt->unk718 < OSGetTime()) {
            fn_806B9500(DWC_ERROR_NETWORK, -85010);
            DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "ServerBrowserLimitUpdate timeout.\n");
        }
    }
    stNNFreeReq = 0;
    stNNBusy = 1;
    {
        qr2_t qr2 = stpMatchCnt->qr2Obj;
        if (qr2 != NULL) {
            qr2_think(qr2);
            // inferred: field of the GameSpy qr2 object (an empty debug check remained)
            if (*(int*)((u8*)qr2 + 0xBC) == 0 && stpMatchCnt->qr2MatchType) {
            }
        }
    }
    NNThink();
    stNNBusy = 0;
    if (stNNFreeReq == 1) {
        stNNFreeReq = 0;
        NNFreeNegotiateList();
    }
    if (stpMatchCnt->pGt2Socket != NULL) {
        gt2Think(*stpMatchCnt->pGt2Socket);
    }
    cnt = stpMatchCnt;
    if (cnt->state == DWC_MATCH_STATE_WAIT_CLOSE && OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk798) > 3000) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "RTT Timeout with DWCi_MatchProcess.\n");
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout : Wait prior profileID.\n");
        stpMatchCnt->unk18 = 0;
        stpMatchCnt->unk700 = 0;
        stpMatchCnt->closing = 0;
        fn_806C3ED0(1);
    }
    fn_806CC6D0();
    fn_806CB8F0(FALSE);
    if (fn_806C4B30()) {
        if (fn_806C57F0()) {
        }
    }
}

// @806B8430
int fn_806B8430(GT2Socket socket, u32 ip, u16 port, u8* message, int len) {
    SOSockAddrIn addr;
    u8 c;
    if (len == 0 || message == NULL) {
        return 0;
    }
    memset(&addr, 0, sizeof(addr));
    addr.family = 2;
    addr.addr = ip;
    addr.port = fn_806A4270(port);
    c = message[0];
    if ((c == 0xFE && message[1] == 0xFD) || c == '\\') {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "GT2 Unrecognized : Received QR2 data.\n");
        if (stpMatchCnt->qr2Obj != NULL) {
            qr2_parse_queryA(stpMatchCnt->qr2Obj, message, len, &addr);
        } else {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "  ignore qr2 message.\n");
        }
    } else if (memcmp(message, NNMagicData, 6) == 0) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "GT2 Unrecognized : Received NN data.\n");
        NNProcessData(message, len, &addr);
    } else if (c == 0xFE && message[1] == 0xFE) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "GT2 Unrecognized : Not Connected gt2 data.\n");
        return 0;
    } else {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "GT2 Unrecognized : Received unrecognized data.\n");
        return 0;
    }
    return 1;
}

// @806B85B0
// NONMATCHING: 99.48% - register allocation of the inlined DWCi_SetMatchStatus
void fn_806B85B0(GT2Socket socket, GT2Connection connection, u32 ip, u16 port, int latency,
                 const char* message, int msgLen) {
    int profileID = 0;
    int index;
    GT2Connection* pConn;
    DWCConnectionInfo* info;
    DWCMatchControl* cnt;
    if (stpMatchCnt != NULL && stpMatchCnt->state == DWC_MATCH_STATE_CL_NN) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "DWCi_GT2ConnectAttemptCallback in DWC_MATCH_STATE_CL_NN\n");
        return;
    }
    if (stpMatchCnt == NULL || stpMatchCnt->state != DWC_MATCH_STATE_CL_GT2 || stpMatchCnt->unk752) {
        gt2Reject(connection, "Init state", -1);
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2Reject was called : Init state\n");
        return;
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Got DWCi_GT2ConnectAttemptCallback latency(%d) msg(%s)\n", latency,
               message);
    if (message && msgLen) {
        profileID = strtoul(message, NULL, 10);
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "remote PID = %d\n", profileID);
    index = fn_806B15B0();
    if (index == -1) {
        gt2Reject(connection, "Server full", -1);
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2Reject was called : Server full\n");
        if (fn_806CD150() == TRUE) {
            stpMatchCnt->resend.command = 0xFF;
            stpMatchCnt->resend.unk1 = 0;
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Mesh stop.\n");
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        } else {
            fn_806B9500((DWCError)6, -87100);
        }
        return;
    }
    cnt = stpMatchCnt;
    if (ip != cnt->tempNewNodeInfo.nnIP || fn_806A4264(port) != cnt->tempNewNodeInfo.nnPort) {
        if (profileID == stpMatchCnt->tempNewNodeInfo.profileID) {
            DWC_Printf(DWC_REPORTFLAG_WARNING, "gt2Connect() came before NN complete.\n");
            stpMatchCnt->tempNewNodeInfo.nnIP = ip;
            stpMatchCnt->tempNewNodeInfo.nnPort = fn_806A4264(port);
        } else {
            gt2Reject(connection, "Unknown connect attempt", -1);
            DWC_Printf(DWC_REPORTFLAG_WARNING, "gt2Reject was called : Unknown connect attempt from %s\n",
                       gt2AddressToString(ip, port, 0));
            return;
        }
    }
    stpMatchCnt->unk730 = 0;
    if (!gt2Accept(connection, stpMatchCnt->gt2Callbacks)) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "Unexpected failure to gt2Accept.\n");
        fn_806B9500((DWCError)6, -80410);
        return;
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Accepted connection from %s (latency %d)\n", gt2AddressToString(ip, port, 0),
               latency);
    stpMatchCnt->resend.command = 0xFF;
    stpMatchCnt->resend.unk1 = 0;
    if (stpMatchCnt->unk6C0 == 0 && stpMatchCnt->nodeInfoList.nodeNum == 1) {
        stpMatchCnt->unk758 = (latency >> 1) < 0xFFFF ? (latency >> 1) : 0xFFFF;
    }
    pConn = fn_806B1660(index);
    info = fn_806B1710(index);
    *pConn = connection;
    info->index = index;
    info->aid = stpMatchCnt->tempNewNodeInfo.aid;
    info->profileID = stpMatchCnt->tempNewNodeInfo.profileID;
    fn_806CE700(info->aid);
    gt2SetConnectionData(connection, info);
    fn_806C1AC0(2);
}

// @806B8D60
// NONMATCHING: 98.40% - register allocation of the inlined DWCi_SetMatchStatus
void fn_806B8D60(GT2Connection connection, GT2Result result, const char* message, int len) {
    char pidStr[24];
    int index;
    GT2Connection* pConn;
    DWCConnectionInfo* info;
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "DWCi_GT2ConnectedCallback(port=%d,result=%d,msg=[%s])\n",
               gt2GetRemotePort(connection), result, message ? message : "");
    if (stpMatchCnt == NULL || (stpMatchCnt->state != DWC_MATCH_STATE_CL_GT2 &&
                                stpMatchCnt->state != DWC_MATCH_STATE_SV_OWN_GT2)) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2ConnectedCallback: Already cancel(port=%d,result=%d,msg=[%s]).\n",
                   gt2GetRemotePort(connection), result, message ? message : "");
        if (stpMatchCnt->unk6C4 == 15) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2ConnectedCallback: mesh creating.. go next process.\n");
        } else {
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "@@@@@ gt2ConnectedCallback: but ignored.\n");
            return;
        }
    }
    if (result != 0) {
        if (message == NULL) {
            message = "";
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "GT2 connect failed %d: %s\n", result, message);
        if (fn_806CD150() == TRUE) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh NN resv timed out.\n");
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
            return;
        }
        if (stpMatchCnt->unk6C4 == 15) {
            stpMatchCnt->unk6C4 = 0;
            memset(&stpMatchCnt->unk6C8, 0, sizeof(DWCNodeInfo));
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh gt2 failed.\n");
            return;
        }
        if (result == 5) {
            return;
        }
        if (result == 6) {
            stpMatchCnt->unkC++;
            if (stpMatchCnt->unkC > 5) {
                int level;
                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2Connect() retry over.\n");
                stpMatchCnt->unkC = 0;
                level = 2;
                if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) &&
                    !stpMatchCnt->unkD) {
                    level = 1;
                }
                fn_806C10C0(stpMatchCnt->tempNewNodeInfo.profileID, level);
            } else {
                DWCMatchControl* cnt;
                GT2Result gtResult;
                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Retry to gt2Connect.\n");
                snprintf(pidStr, sizeof(pidStr), "%u", stpMatchCnt->profileID);
                cnt = stpMatchCnt;
                gtResult = gt2Connect(*cnt->pGt2Socket, NULL,
                                       gt2AddressToString(cnt->tempNewNodeInfo.nnIP, cnt->tempNewNodeInfo.nnPort, 0),
                                       pidStr, -1, 5000, cnt->gt2Callbacks, 0);
                if (gtResult == 1) {
                    fn_806C6470(gtResult);
                } else if (gtResult != 0) {
                    int level = 2;
                    if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) &&
                        !stpMatchCnt->unkD) {
                        level = 1;
                    }
                    if (!fn_806C10C0(stpMatchCnt->tempNewNodeInfo.profileID, level)) {
                        return;
                    }
                }
            }
        } else {
            int level = 2;
            if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) &&
                !stpMatchCnt->unkD) {
                level = 1;
            }
            if (!fn_806C10C0(stpMatchCnt->tempNewNodeInfo.profileID, level)) {
                return;
            }
        }
        return;
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "GT2 connected.\n");
    index = fn_806B15B0();
    if (index == -1) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "Don't continue matching without closing connections!!\n");
        fn_806B9500((DWCError)6, -87100);
        return;
    }
    pConn = fn_806B1660(index);
    info = fn_806B1710(index);
    *pConn = connection;
    info->index = index;
    if (stpMatchCnt->unk6C4 != 15) {
        info->aid = stpMatchCnt->tempNewNodeInfo.aid;
        info->profileID = stpMatchCnt->tempNewNodeInfo.profileID;
    } else {
        info->aid = stpMatchCnt->unk6C8.aid;
        info->profileID = stpMatchCnt->unk6C8.profileID;
    }
    fn_806CE700(info->aid);
    gt2SetConnectionData(connection, info);
    if (stpMatchCnt->state == DWC_MATCH_STATE_SV_OWN_GT2 || stpMatchCnt->unk6C4 == 15) {
        fn_806C1AC0(0);
    } else {
        fn_806C1AC0(1);
    }
}

// @806B9410
void fn_806B9410(GPConnection* connection, int profileID, char* message) {
    u32 data[128];
    int len;
    int size;
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "<GP> RECV-0x%02x <- [--------:-----] [pid=%u]\n", (u8)message[0],
               profileID);
    len = strlen(message) - 1;
    if (len == 0) {
        size = 0;
    } else {
        size = DWC_Base64Decode(message + 1, len, (char*)data, sizeof(data));
        if (size == -1 || size % 4 != 0 || size > (int)sizeof(data)) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore illegal data(%d,%d).\n", len, size);
            return;
        }
        size /= 4;
    }
    fn_806BCD40((u8)message[0], profileID, 0, 0, data, size);
}

// @806B94F0
// inferred: calls the matched callback of the game
static void DWCi_CallMatchedCallback(DWCError error, BOOL cancel, BOOL self) {
    stpMatchCnt->matchedCallback(error, cancel, self, stpMatchCnt->qr2MatchType == 2,
                                 DWCi_GetFriendListIndex(stpMatchCnt->friendIdx),
                                 stpMatchCnt->matchedParam);
}

// @806B9500
void fn_806B9500(DWCError error, int errorCode) {
    DWCMatchControl* cnt;
    if (stpMatchCnt == NULL || error == DWC_ERROR_NONE) {
        return;
    }
    stpMatchCnt->closing = TRUE;
    gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
    stpMatchCnt->closing = FALSE;
    DWCi_SetError(error, errorCode);
    fn_806BB440(1);
    DWCi_CallMatchedCallback(error, FALSE, stpMatchCnt->friendIdx ? FALSE : TRUE);
    fn_806BBCA0();
}

// @806B9740
void fn_806B9740(void) {
    if (fn_806B1230() == 3) {
        stpMatchCnt->qr2NumEntry = 0;
        stpMatchCnt->groupID = 0;
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_ClearQR2Key ignored.\n");
    }
}

// @806B97B0
void fn_806B97B0(u8 aid, u16 type, const u8* message) {
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Received SYN %d packet from aid %d.\n", type - 2, aid);
    fn_806B9820(aid, type, message);
}

// @806B9820
// inferred: reads one node entry of a SYN packet (always inlined)
inline static void DWCi_ReadSynNodeInfo(DWCNodeInfo* node, const u32* p) {
    memset(node, 0, sizeof(DWCNodeInfo));
    node->aid = (u8)DWCi_SwapU32(p[0]);
    node->unk17 = (u8)(DWCi_SwapU32(p[0]) >> 16);
    node->profileID = DWCi_SwapU32(p[1]);
    node->ip = p[2];
    node->localIP = p[3];
    node->port = (u16)(DWCi_SwapU32(p[4]) >> 16);
    node->localPort = (u16)DWCi_SwapU32(p[4]);
    memcpy(&node->unk28, &p[5], sizeof(node->unk28));
}

// NONMATCHING: 90.23% - loads of the SYN node entries are scheduled before the stores in our build
void fn_806B9820(u8 aid, u16 type, const u8* message) {
    const u32* data = (const u32*)message;
    DWCMatchControl* cnt;
    switch (type) {
    case 2:
        if (stpMatchCnt->state != DWC_MATCH_STATE_CL_WAITING) {
            break;
        }
        if (DWCi_SwapU32(data[0]) == 1) {
            stpMatchCnt->friendIdx = 0;
            stpMatchCnt->unk690.profileID = 0;
            stpMatchCnt->unk8F8 = DWCi_SwapU32(data[1]);
            if (stpMatchCnt->unk8E0 == 2) {
                DWCi_NodeInfoList_InsertTop(&stpMatchCnt->tempNewNodeInfo);
            }
        } else {
            DWCNodeInfo node;
            DWCi_ReadSynNodeInfo(&node, &data[1]);
            stpMatchCnt->unk690 = node;
            stpMatchCnt->unk8F8 = DWCi_SwapU32(data[7]);
            stpMatchCnt->friendIdx = node.profileID;
        }
        cnt = stpMatchCnt;
        if (cnt->nodeInfoList.nodeNum == 1) {
            cnt->unk7A0 = 90000;
        } else {
            cnt->unk7A0 = cnt->nodeInfoList.nodeNum * 12000;
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "SYN wait time = %d\n", cnt->unk7A0);
        stpMatchCnt->unk756 = 0;
        fn_806CCB70(DWC_MATCH_STATE_CL_SYN);
        fn_806C4740(aid, 3);
        break;
    case 3:
        if (stpMatchCnt->state == DWC_MATCH_STATE_SV_SYN) {
            u16 latency;
            u32 bitmap;
            int i;
            stpMatchCnt->unk780 |= 1 << aid;
            latency = message[0] | (message[1] << 8);
            if (latency > stpMatchCnt->unk758) {
                stpMatchCnt->unk758 = latency;
            }
            cnt = stpMatchCnt;
            bitmap = 0;
            for (i = 1; i < cnt->nodeInfoList.nodeNum; i++) {
                bitmap |= 1 << fn_806CCED0(i)->aid;
            }
            if ((bitmap | (1 << stpMatchCnt->tempNewNodeInfo.aid)) != stpMatchCnt->unk780) {
                break;
            }
            cnt->unk8F8 = fn_806ABB70(-2) + 1;
            DWCi_SendTypeMatchAck();
            stpMatchCnt->unk754 = 0;
            stpMatchCnt->unk788 = DWCi_Np_GetTick();
        } else {
            fn_806C4740(aid, 4);
        }
        break;
    case 4:
        if (stpMatchCnt->state == DWC_MATCH_STATE_CL_SYN) {
            int num = DWCi_SwapU32(data[0]);
            if (num != 0) {
                int i;
                for (i = 0; i < num; i++) {
                    DWCNodeInfo node;
                    DWCi_ReadSynNodeInfo(&node, &data[(num - 1 - i) * 6 + 1]);
                    DWCi_NodeInfoList_InsertTop(&node);
                }
                stpMatchCnt->unk8F8 = DWCi_SwapU32(data[num * 6 + 1]);
                DWCi_NodeInfoList_InsertTop(&stpMatchCnt->tempNewNodeInfo);
                memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
            } else {
                stpMatchCnt->unk8F8 = DWCi_SwapU32(data[1]);
            }
            fn_806C1AC0(3);
        }
        break;
    }
}

// @806BAC80
// NONMATCHING: 99.44% - register allocation / small scheduling differences
void fn_806BAC80(const u32* data) {
    u8 i;
    u32 deadBitmap;
    DWCNodeInfo* server;
    u32 uid;
    u32 bitmap;
    u32 profileID;
    profileID = DWCi_SwapU32(data[0]);
    bitmap = DWCi_SwapU32(data[1]);
    uid = DWCi_SwapU32(data[2]);
    DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Received Polling packet from pid %d(%08x).\n", profileID, bitmap);
    if (fn_806B12F0()) {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "I'm a server. ignore.\n");
        return;
    }
    if (uid != stpMatchCnt->unk8F8) {
        DWC_Printf(DWC_REPORTFLAG_INFO, "ignore serverPollingUID(%d).\n", uid);
        return;
    }
    server = fn_806CCEA0();
    if (server == NULL || server->profileID != profileID) {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "command from wrong server. ignore.\n");
        return;
    }
    deadBitmap = fn_806B0F80();
    deadBitmap &= deadBitmap ^ bitmap;
    if (deadBitmap) {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "check dead aid: dead aidBitmap is %08x.\n", deadBitmap);
        for (i = 0; i < 32; i++) {
            if (deadBitmap & (1 << i)) {
                fn_806B1A00(i);
            }
        }
    }
}

// @806BAE10
// NONMATCHING: 99.63% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806BAE10(DWCError error, int errorCode) {
    if (fn_806B1230() != 4) {
        return FALSE;
    }
    if (stpMatchCnt->qr2MatchType == 2) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_ProcessMatchClosing: SC_SV.\n");
        return TRUE;
    }
    if (error) {
        fn_806B9500(error, errorCode - 80000);
        return TRUE;
    }
    if (stpMatchCnt->unk752 == 1 || stpMatchCnt->closing == 1) {
        return TRUE;
    }
    if (stpMatchCnt->nnData.cookie) {
        NNCancel(stpMatchCnt->nnData.cookie);
        stpMatchCnt->nnData.cookie = 0;
    }
    if (stpMatchCnt->nodeInfoList.nodeNum > 1) {
        if (!stpMatchCnt->closing) {
            stpMatchCnt->closing = 2;
            gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
        }
    } else if (stpMatchCnt->qr2MatchSubType == 3) {
        fn_806B9500((DWCError)25, -80430);
    } else if (stpMatchCnt->state == DWC_MATCH_STATE_CL_WAITING) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Wait prior profileID.\n");
        fn_806CCB70(DWC_MATCH_STATE_WAIT_CLOSE);
        stpMatchCnt->unk798 = DWCi_Np_GetTick();
    } else {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Restart matching immediately.\n");
        fn_806C3ED0(1);
    }
    return TRUE;
}

// @806BB3F0
void fn_806BB3F0(int arg) {
    int level;
    if (stpMatchCnt->closing == 1) {
        return;
    }
    level = 2;
    if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) &&
        !stpMatchCnt->unkD) {
        level = 1;
    }
    fn_806C10C0(arg, level);
}

// @806BB430
static BOOL DWCi_IsMatchLocked(void) {
    if (!stpMatchCnt->unk908) {
        return FALSE;
    }
    if (fn_806B12F0() &&
        (stpMatchCnt->qr2MatchSubType == 1 || stpMatchCnt->qr2MatchSubType == 3 ||
         stpMatchCnt->qr2MatchSubType == 2) &&
        stpMatchCnt->unk8F4 == 0) {
        return TRUE;
    }
    return FALSE;
}

// @806BB440
GPResult fn_806BB440(int status) {
    char statusStr[37]; // inferred: 37-39 bytes (rounded to 44 alone and to 40 when inlined)
    char valueStr[12];
    u32 numHost;
    snprintf(valueStr, sizeof(valueStr), "%u", stpMatchCnt->qr2NumEntry + 1);
    fn_806AB920("SCM", valueStr, statusStr, '/');
    numHost = stpMatchCnt->nodeInfoList.nodeNum;
    if (stpMatchCnt->qr2MatchSubType == 2 && numHost == 0) {
        numHost = 1;
    }
    snprintf(valueStr, sizeof(valueStr), "%u", numHost);
    fn_806AB980("SCN", valueStr, statusStr, '/');
    snprintf(valueStr, sizeof(valueStr), "%u", 90);
    fn_806AB980("VER", valueStr, statusStr, '/');
    snprintf(valueStr, sizeof(valueStr), "%u", DWCi_IsMatchLocked() != FALSE);
    fn_806AB980("LCK", valueStr, statusStr, '/');
    return DWCi_SetGPStatus(status, statusStr, NULL);
}

// @806BB5E0
static void DWCi_FreeAddFilter(void) {
    if (stpAddFilter != NULL) {
        DWC_Free(DWC_ALLOCTYPE_BASE, stpAddFilter, 0);
        stpAddFilter = NULL;
    }
}

void fn_806BB5E0(void) {
    stpMatchCnt = NULL;
    stLastStateTime = DWCi_Np_GetTick();
    DWCi_FreeAddFilter();
    DWCi_ClearGameMatchKeys();
}

// @806BB6A0
BOOL fn_806BB6A0(void) {
    return stpMatchCnt == NULL;
}

// @806BB6C0
// NONMATCHING: 99.89% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806BB6C0(int level) {
    stpMatchCnt->unkC = 0;
    stpMatchCnt->unk720 = 0;
    {
        DWCMatchControl* cnt = stpMatchCnt;
        cnt->unk722 = fn_806ABB70(0x10000);
    }
    stpMatchCnt->unk724 = 0;
    stpMatchCnt->unk728 = 0;
    stpMatchCnt->unk730 = 0;
    stpMatchCnt->unk748 = 0;
    stpMatchCnt->unk752 = 0;
    stpMatchCnt->unk753 = 0;
    stpMatchCnt->unk754 = 0;
    stpMatchCnt->unk755 = 0;
    stpMatchCnt->unk750 = 0;
    stpMatchCnt->closing = 0;
    stpMatchCnt->unk75A = 0;
    stpMatchCnt->unk788 = 0;
    stpMatchCnt->unk798 = 0;
    memset(&stpMatchCnt->resend, 0, sizeof(DWCResendCommand));
    stpMatchCnt->unk6C0 = 0;
    stpMatchCnt->unk6C4 = 0;
    memset(&stpMatchCnt->unk6C8, 0, sizeof(DWCNodeInfo));
    if (level == 2) {
        if ((int)stpMatchCnt->qr2IsReserved == 2) {
            fn_806CCB70(DWC_MATCH_STATE_SV_WAITING);
        } else if ((int)stpMatchCnt->qr2IsReserved == 1) {
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        } else {
            DWC_Printf(DWC_REPORTFLAG_WARNING, "DWCi_ResetMatchParam: Host is Free!\n");
        }
        return;
    }
    stpMatchCnt->unk18 = 0;
    stpMatchCnt->unk700 = 0;
    stpMatchCnt->unk708 = 0;
    stpMatchCnt->unk758 = 0;
    stpMatchCnt->friendIdxListIdx = 0;
    stpMatchCnt->unk760 = 0;
    stpMatchCnt->unk768 = 0;
    stpMatchCnt->unk770 = 0;
    stpMatchCnt->unk778 = 0;
    stpMatchCnt->unk780 = 0;
    stpMatchCnt->unk7AC = 0;
    memset(&stpMatchCnt->nnData, 0, sizeof(DWCNNData));
    stpMatchCnt->unk8B8 = 0;
    stpMatchCnt->unk8BC = 0;
    stpMatchCnt->unk8C4 = 0;
    stpMatchCnt->unk8D8 = 1;
    stpMatchCnt->unk8DC = 0;
    stpMatchCnt->qr2IsReserved = 0;
    stpMatchCnt->unkD = 0;
    stpMatchCnt->resvCallback = NULL;
    stpMatchCnt->resvParam = NULL;
    fn_806CCC90();
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    stpMatchCnt->unk690.profileID = 0;
    if (level == 1) {
        if (stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) {
            fn_806CCB70(DWC_MATCH_STATE_SEARCH_OWN);
        }
        return;
    }
    stpMatchCnt->qr2MatchType = 0;
    stpMatchCnt->qr2NumEntry = 0;
    stpMatchCnt->unk50 = 0;
    stpMatchCnt->friendIdx = 0;
    stpMatchCnt->unk756 = 0;
    stpMatchCnt->unk7A0 = 0;
    stpMatchCnt->unk8A8 = 0;
    stpMatchCnt->unk8AC = 0;
    memset(stpMatchCnt->unk8EC, 0, sizeof(stpMatchCnt->unk8EC));
}

// @806BBBA0
void fn_806BBBA0(u8 matchType, u8 numEntry, DWCMatchedCallback callback, void* param) {
    fn_806BB6C0(0);
    stpMatchCnt->qr2MatchType = matchType;
    stpMatchCnt->qr2NumEntry = numEntry;
    stpMatchCnt->matchedCallback = callback;
    stpMatchCnt->matchedParam = param;
    stpMatchCnt->unk721 = 0;
    stpMatchCnt->suspendReq = 0xFF;
    stpMatchCnt->unk30 = 0;
    stpMatchCnt->qr2IsReserved = 0;
    stpMatchCnt->unk8D8 = 1;
    stpMatchCnt->unk8DC = 0;
    stpMatchCnt->qr2NumEntry = numEntry;
    stpMatchCnt->suspendBitmap = 0;
    stpMatchCnt->suspended = 0;
    stpMatchCnt->unk20 = 0;
    stpMatchCnt->suspending = 0;
    stpMatchCnt->unk8F4 = 1;
    stpMatchCnt->unk900 = 0;
    stpMatchCnt->resvCheckValue = 0;
}

// @806BBCA0
// NONMATCHING: 99.77% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806BBCA0(void) {
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, " Close Match....\n");
    if (stpMatchCnt == NULL) {
        return;
    }
    if (stpMatchCnt->sbObj != NULL) {
        if (!stSBBusy) {
            ServerBrowserFree(stpMatchCnt->sbObj);
            stpMatchCnt->sbObj = NULL;
        } else {
            stSBFreeReq = TRUE;
        }
    }
    NNCancel(stpMatchCnt->nnData.cookie);
    if (!stNNBusy) {
        NNFreeNegotiateList();
    } else {
        stNNFreeReq = TRUE;
    }
    fn_806CCB70(DWC_MATCH_STATE_INIT);
    DWCi_FreeAddFilter();
    DWCi_ClearGameMatchKeys();
    stpMatchCnt->unk50 = 1;
    fn_806CCC90();
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    stpMatchCnt->unk8F4 = 1;
    fn_806BB440(1);
}

// @806BC010
// NONMATCHING: 93.69% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
int DWCi_SBUpdateAsync(int profileID) {
    u8 keyList[DWC_QR2_GAME_KEY_NUM + 9];
    char hostState[512];
    char filter[512];
    int numKeys;
    BOOL async;
    int result;
    int i;
    keyList[0] = 8;
    keyList[1] = 10;
    keyList[2] = 50;
    keyList[3] = 51;
    keyList[4] = 52;
    keyList[5] = 53;
    keyList[6] = 54;
    keyList[7] = 55;
    keyList[8] = 56;
    async = TRUE;
    numKeys = 9;
    if (stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) {
        for (i = 0; i < DWC_QR2_GAME_KEY_NUM; i++) {
            if (stGameMatchKeys[i].keyID) {
                keyList[numKeys++] = stGameMatchKeys[i].keyID;
            }
        }
    }
    switch (stpMatchCnt->state) {
    case DWC_MATCH_STATE_CL_SEARCH_HOST: {
        int ratio = stServerSearchRatio;
        int sub;
        int numEntry;
        int myProfileID;
        int len;
        u8 searchState;
        if (ratio == -2) {
            searchState = 3;
            if (stpMatchCnt->unk8D8 == 1) {
                searchState = 2;
            }
            stpMatchCnt->unk8D8 = 0;
        } else if (ratio == -1) {
            searchState = 0;
            if (stpMatchCnt->unk8D8) {
                searchState = 2;
            }
            stpMatchCnt->unk8D8 ^= 1;
        } else {
            searchState = 0;
            if (fn_806ABB70(100) < ratio) {
                searchState = 2;
            }
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "ServerSearchRatio=%d %% (next search->%d)\n", ratio,
                       searchState);
        }
        sub = stpMatchCnt->qr2MatchSubType;
        numEntry = stpMatchCnt->qr2NumEntry;
        myProfileID = stpMatchCnt->profileID;
        if (searchState == 3) {
            snprintf(hostState, 0x1ff, "(%s = %u or %s = %u)", "dwc_hoststate", 0, "dwc_hoststate", 2);
        } else {
            snprintf(hostState, 0x1ff, "%s = %u", "dwc_hoststate", searchState);
        }
        snprintf(filter, 0x1ff,
                 "%s = %d and %s != %u and maxplayers = %d and numplayers < %d and %s = %d and %s = %u and %s",
                 "dwc_mver", 90, "dwc_pid", myProfileID, numEntry, numEntry, "dwc_mtype", sub,
                 "dwc_suspend", 0, hostState);
        len = strlen(filter);
        if (stpAddFilter != NULL) {
            snprintf(filter + len, 0x1ff - len, " and (%s)", stpAddFilter);
            filter[0x1fe] = '\0';
        }
        stpMatchCnt->unk8DC++;
        break;
    }
    case DWC_MATCH_STATE_CL_WAIT_RESV:
    case DWC_MATCH_STATE_CL_SEARCH_EVAL_HOST:
    case DWC_MATCH_STATE_SEARCH_OWN:
        snprintf(filter, 0x1ff, "%s = %u", "dwc_pid", profileID);
        stpMatchCnt->unk7AC = profileID;
        break;
    case DWC_MATCH_STATE_CL_SVDOWN_1:
    case DWC_MATCH_STATE_CL_SVDOWN_2:
    case DWC_MATCH_STATE_CL_SVDOWN_3:
        snprintf(filter, 0x1ff, "%s = %u", "dwc_pid", profileID);
        async = FALSE;
        break;
    case DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST:
        snprintf(filter, 0x1ff, "(%s = %u) and (%s = %u) and (%s != %u)", "dwc_groupid",
                 stpMatchCnt->groupID, "dwc_hoststate", 2, "dwc_pid", stpMatchCnt->profileID);
        break;
    default:
        DWC_Printf(DWC_REPORTFLAG_ERROR, "---DWCi_SBUpdateAsync() illegal state %d.\n", stpMatchCnt->state);
        break;
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "ServerBrowserFilter : %s\n", filter);
    ServerBrowserClear(stpMatchCnt->sbObj);
    result = ServerBrowserLimitUpdateA(stpMatchCnt->sbObj, async, 0, keyList, numKeys, filter,
                         stpMatchCnt->state == DWC_MATCH_STATE_SEARCH_OWN ? 1 : 6);
    if (result == 2) {
        DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "Retry\n");
    }
    if (result == 0) {
        stpMatchCnt->unk718 = DWCi_Np_GetTick() + OS_MSEC_TO_TICKS(30000);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout set.(%s)\n", __FUNCTION__);
    }
    return result;
}

// @806BC4B0
static BOOL DWCi_IsPrivateAddress(u32 ip) {
    u32 a = ip >> 24;
    u32 b = (ip >> 16) & 0xff;
    if (a == 10) {
        return TRUE;
    }
    if (a == 172 && 16 <= b && b <= 31) {
        return TRUE;
    }
    if (a == 192 && b == 168) {
        return TRUE;
    }
    return FALSE;
}

// @806BC4C0
// inferred: shared by DWCi_NNStart and the NN completed callback (always inlined)
inline static int DWCi_DoNatNegotiation(DWCNNData* nnData) {
    int result;
    if (!nnData->isChild) {
        if (fn_806C5EB0(ServerBrowserSendNatNegotiateCookieToServerA(stpMatchCnt->sbObj, gt2AddressToString(nnData->ip, 0, 0), nnData->port,
                                    nnData->cookie))) {
            return 2;
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Send NN cookie = %x.\n", nnData->cookie);
    }
    result = NNBeginNegotiationWithSocket(gt2GetSocketSOCKET(*stpMatchCnt->pGt2Socket), nnData->cookie, nnData->isChild, fn_806CA020,
                         fn_806CA040, nnData);
    if (result == 3) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, " dns error occurs when NatNegotiation begin... retry\n");
    }
    return result;
}

// NONMATCHING: 94.60% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
int fn_806BC4C0(int isChild, int cookie, DWCNodeInfo* node) {
    int result = 0;
    BOOL doNN;
    NNCancel(stpMatchCnt->nnData.cookie);
    if (!isChild) {
        cookie = (u16)stpMatchCnt->profileID | (stpMatchCnt->unk722 << 16);
        if (node->unk17) {
            if (node->ip == ServerBrowserGetMyPublicIPAddr(stpMatchCnt->sbObj)) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Server[%u] is behind same NAT as me.\n", node->profileID);
                doNN = FALSE;
                node->nnIP = node->localIP;
                node->nnPort = node->localPort;
            } else {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Server[%u] is behind NAT.\n", node->profileID);
                doNN = TRUE;
            }
        } else if (DWCi_IsPrivateAddress(fn_806A4260(SOGetHostID()))) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Server[%u] is not behind NAT. But I'm behind NAT.\n",
                       node->profileID);
            doNN = TRUE;
        } else {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Both I and Server[%u] are not behind NAT.\n", node->profileID);
            doNN = FALSE;
            node->nnIP = node->ip;
            node->nnPort = node->port;
        }
        if (doNN) {
            {
                DWCMatchControl* cnt = stpMatchCnt;
                cnt->unk722 = fn_806ABB70(0x10000);
            }
            stpMatchCnt->nnData.cookie = cookie;

        } else {
            u32 data[2];
            int ret;
            data[0] = SOGetHostID();
            data[1] = DWCi_SwapU32((u16)gt2GetLocalPort(*stpMatchCnt->pGt2Socket));
            ret = fn_806BC860(6, node->profileID, node->ip, node->port, data, 2);
            stpMatchCnt->resend.unk1 = 0;
            if (ret) {
                return 2;
            }
            stpMatchCnt->nnData.cookie = 0;
        }
        stpMatchCnt->nnData.isChild = 0;
        stpMatchCnt->nnData.unk1 = 0;
        stpMatchCnt->nnData.port = node->port;
        stpMatchCnt->nnData.ip = node->ip;
    } else {
        doNN = TRUE;
        stpMatchCnt->nnData.isChild = 1;
        stpMatchCnt->nnData.unk1 = 0;
        stpMatchCnt->nnData.port = 0;
        stpMatchCnt->nnData.ip = 0;
        stpMatchCnt->nnData.cookie = cookie;
    }
    if (doNN) {
        result = DWCi_DoNatNegotiation(&stpMatchCnt->nnData);
    } else {
        DWCMatchControl* cnt = stpMatchCnt;
        fn_806CA040(0, gt2GetSocketSOCKET(*cnt->pGt2Socket), NULL, &cnt->nnData);
        stpMatchCnt->unk730 = 0;
    }
    return result;
}

// @806BC850
static BOOL DWCi_IsGT2Target(int profileID) {
    if (!fn_806B1680(profileID, TRUE)) {
        return FALSE;
    }
    return fn_806CCF00(profileID) != NULL;
}

// @806BC860
// NONMATCHING: 97.14% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
int fn_806BC860(u8 command, int profileID, u32 ip, u16 port, const u32* data, int len) {
    DWCMatchCommand sbCommand;
    DWCMatchCommand gt2Command;
    char gpMessage[0x200];
    char base64[0x200];
    int result;
    int size;
    if (DWCi_IsGT2Target(profileID)) {
        DWCNodeInfo* node;
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "SEND GT2 MSG TO PID:%u\n", profileID);
        node = fn_806CCF00(profileID);
        size = len;
        if (data != NULL && len != 0) {
            fn_806A9CA0(gt2Command.data, data, len * 4);
        } else {
            size = 0;
        }
        strcpy(gt2Command.header.magic, "\xBB\x49\xCC\x4D");
        gt2Command.header.ver = DWCi_SwapU32(90);
        gt2Command.header.command = command;
        gt2Command.header.size = (u8)(size * 4);
        gt2Command.header.port = DWCi_SwapU16(stpMatchCnt->qr2Port);
        gt2Command.header.ip = stpMatchCnt->qr2IP;
        gt2Command.header.profileID = DWCi_SwapU32(stpMatchCnt->profileID);
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "<GT2> SEND-0x%02x -> [--------:-----] [pid=%u]\n", command,
                   profileID);
        fn_806CE750(fn_806B0E30(), node->aid, &gt2Command, gt2Command.header.size + sizeof(DWCMatchCommandHeader), 0);
        result = 0;
    } else if (gpIsBuddy(stpMatchCnt->pGpObj, profileID)) {
        GPConnection* connection;
        int length;
        char* p;
        int b64len = 0;
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "SEND BUDDY MSG TO PID:%u\n", profileID);
        if (data != NULL && len != 0) {
            b64len = DWC_Base64Encode((const char*)data, len * 4, base64, sizeof(base64));
        }
        base64[b64len] = '\0';
        connection = stpMatchCnt->pGpObj;
        length = snprintf(gpMessage, sizeof(gpMessage), "%s%dv%s", "GPCM", 90, "MAT");
        gpMessage[length] = command;
        p = &gpMessage[length + 1];
        *p = '\0';
        length = strlen(base64);
        fn_806A9CA0(p, base64, length);
        p[length] = '\0';
        result = gpSendBuddyMessageA(connection, profileID, gpMessage);
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "<GP> SEND-0x%02x -> [--------:-----] [pid=%u]\n", command,
                   profileID);
    } else {
        if (ip == 0) {
            DWCNodeInfo* node = fn_806CCF00(profileID);
            ip = node->ip;
            port = node->port;
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "FROM BM but peer is not buddy.\n");
        }
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "SEND SB MSG TO IP/PORT:(%08x:%d)\n", ip, port);
        size = len;
        if (data != NULL && len != 0) {
            fn_806A9CA0(sbCommand.data, data, len * 4);
        } else {
            size = 0;
        }
        strcpy(sbCommand.header.magic, "\xBB\x49\xCC\x4D");
        sbCommand.header.ver = DWCi_SwapU32(90);
        sbCommand.header.command = command;
        sbCommand.header.size = (u8)(size * 4);
        sbCommand.header.port = DWCi_SwapU16(stpMatchCnt->qr2Port);
        sbCommand.header.ip = stpMatchCnt->qr2IP;
        sbCommand.header.profileID = DWCi_SwapU32(stpMatchCnt->profileID);
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "<SB> SEND-0x%02x -> [%08x:%d] [pid=--------]\n", command, ip,
                   port);
        result = ServerBrowserSendMessageToServerA(stpMatchCnt->sbObj, gt2AddressToString(ip, 0, 0), port, &sbCommand,
                             sbCommand.header.size + sizeof(DWCMatchCommandHeader));
        if (result == 2) {
        }
    }
    if (command == 2 || command == 6 || command == 8 || command == 9) {
        stpMatchCnt->resend.command = command;
        stpMatchCnt->resend.port = port;
        stpMatchCnt->resend.ip = ip;
        stpMatchCnt->resend.profileID = profileID;
        stpMatchCnt->resend.len = len;
        stpMatchCnt->resend.time = DWCi_Np_GetTick();
        if (data != NULL && len != 0) {
            fn_806A9CA0(stpMatchCnt->resend.data, data, len * 4);
        }
    }
    return result;
}

// @806BCD40
// inferred: text of a reservation deny reason (always inlined)
inline static const char* DWCi_GetResvDenyReasonString(u32 reason) {
    switch (reason) {
    case 16:
        return "Game server is fully occupied.";
    case 17:
        return "This Domain is already closed.";
    case 18:
        return "The condition was not satisfied.";
    case 19:
        return "This Domain is already locked.";
    case 20:
        return "It tried to go to the client for the reservation.";
    case 21:
        return "It is a reservation to the friend who doesn't exist in the list.";
    case 22:
        return "It was rejected by the attempt callback.";
    case 23:
        return "The reservation came from a different other host.";
    case 24:
        return "Illegal mesh reservation.";
    default:
        return "";
    }
}

// inferred: (always inlined)
inline static void DWCi_TempNewNodeInfo_Clear(void) {
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_TempNewNodeInfo_Clear()\n");
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
}

// NONMATCHING: 95.94% - large command switch; register allocation and scheduling differ
BOOL fn_806BCD40(u8 command, int profileID, u32 ip, u16 port, const u32* data, int len) {
    int sendLen = 0;
    DWCNodeInfo* node;
    if (stpMatchCnt == NULL || stpMatchCnt->state == DWC_MATCH_STATE_INIT) {
        return TRUE;
    }
    if (fn_806B1230() == 4 && stpMatchCnt->state == DWC_MATCH_STATE_CL_WAITING &&
        (node = fn_806CCED0(0)) != NULL && profileID == node->profileID) {
        stpMatchCnt->unk756 = 0;
        stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
    }
    switch (command) {
    case 1: {
        DWCNodeInfo newNode;
        u32 sendData[14];
        u8 matchType = DWCi_SwapU32(data[0]);
        u32 meshReserve;
        u32 topology;
        u32 resvCheckValue;
        BOOL illegal;
        u8 result;
        DWCNodeInfo* myNode;
        memset(&newNode, 0, sizeof(DWCNodeInfo));
        newNode.profileID = profileID;
        newNode.ip = data[1];
        newNode.port = (u16)DWCi_SwapU32(data[2]);
        newNode.localIP = data[3];
        newNode.localPort = (u16)DWCi_SwapU32(data[4]);
        newNode.aid = 0xFF;
        newNode.unk17 = (u8)DWCi_SwapU32(data[5]);
        meshReserve = DWCi_SwapU32(data[6]);
        topology = DWCi_SwapU32(data[7]);
        memcpy(&newNode.unk28, &data[8], sizeof(newNode.unk28));
        resvCheckValue = DWCi_SwapU32(data[9]);
        illegal = FALSE;
        if (len >= 11) {
            illegal = DWCi_SwapU32(data[10]) == 1;
        }
        result = fn_806BFBA0(newNode.profileID, newNode.ip, newNode.port, matchType, meshReserve != 0, topology,
                             &newNode.unk28, illegal);
        if (result == 2) {
            memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
            stpMatchCnt->tempNewNodeInfo = newNode;
            if (DWCi_HandleMatchCommandError(fn_806C03B0(profileID, ip, port))) {
                return FALSE;
            }
            if (stpMatchCnt->unk6C0) {
                stpMatchCnt->tempNewNodeInfo.aid = fn_806CCF00(stpMatchCnt->tempNewNodeInfo.profileID)->aid;
            }
            if (!stpMatchCnt->unk6C0 && stpMatchCnt->qr2MatchType == 2 &&
                stpMatchCnt->state != DWC_MATCH_STATE_SV_OWN_NN &&
                (!(stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) ||
                 stpMatchCnt->unkD) &&
                stpMatchCnt->unk8A8 != NULL) {
                DWCMatchControl* cnt = stpMatchCnt;
                ((DWCNewClientCallback)cnt->unk8A8)(DWCi_GetFriendListIndex(profileID), cnt->unk8AC);
            }
            myNode = fn_806CCF00(stpMatchCnt->profileID);
            sendData[0] = DWCi_SwapU32(stpMatchCnt->qr2NumEntry);
            sendData[1] = DWCi_SwapU32(myNode->aid);
            sendData[2] = DWCi_SwapU32(myNode->profileID);
            sendData[3] = myNode->ip;
            sendData[4] = DWCi_SwapU32(myNode->port);
            sendData[5] = myNode->localIP;
            sendData[6] = DWCi_SwapU32(myNode->localPort);
            sendData[7] = DWCi_SwapU32(myNode->unk17);
            sendData[8] = myNode->unk28;
            sendData[9] = DWCi_SwapU32(stpMatchCnt->groupID);
            sendData[10] = DWCi_SwapU32(stpMatchCnt->tempNewNodeInfo.aid);
            sendData[11] = DWCi_SwapU32(resvCheckValue);
            sendData[12] = DWCi_SwapU32(stpMatchCnt->nodeInfoList.nodeNum);
            sendData[13] = DWCi_SwapU32(stpMatchCnt->unk6C0 != 0);
            sendLen = 14;
            DWC_Printf(DWC_REPORTFLAG_INFO, "Send <DWC_MATCH_COMMAND_RESV_OK> to: [%u]\n", newNode.profileID);
            DWC_Printf(DWC_REPORTFLAG_INFO, "Send Server AID(%d)\n", myNode->aid);
            DWC_Printf(DWC_REPORTFLAG_INFO, "Send Group ID(%d)\n", stpMatchCnt->groupID);
            DWC_Printf(DWC_REPORTFLAG_INFO, "Send Max Entry(%d)\n", stpMatchCnt->qr2NumEntry);
            DWC_Printf(DWC_REPORTFLAG_INFO, "new client aid: [%u]\n", stpMatchCnt->tempNewNodeInfo.aid);
            fn_806CCB70(DWC_MATCH_STATE_SV_OWN_NN);
        } else if (result == 3) {
            DWC_Printf(DWC_REPORTFLAG_INFO, "send RESV_DENY. %s\n",
                       DWCi_GetResvDenyReasonString(stpMatchCnt->unk8E4));
            sendLen = 1;
            sendData[0] = DWCi_SwapU32(stpMatchCnt->unk8E4);
        }
        if (result != 0xFF) {
            if (DWCi_HandleMatchCommandError(
                    fn_806BC860(result, newNode.profileID, newNode.ip, newNode.port, sendData, sendLen))) {
                return FALSE;
            }
        }
        break;
    }
    case 2: {
        DWCNodeInfo serverNode;
        u8 numEntry = DWCi_SwapU32(data[0]);
        DWCNodeInfo* myNode = NULL;
        u32 groupID;
        if (stpMatchCnt->state != DWC_MATCH_STATE_CL_WAIT_RESV) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "****** Ignore NN reservation ****** \n");
            break;
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Succeeded NN reservation.\n");
        if (profileID != stpMatchCnt->unk7AC) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "****** Ignore RESV_OK. from wrong server(%d). ****** \n");
            break;
        }
        if (profileID != DWCi_SwapU32(data[2])) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "****** Ignore RESV_OK. wrong server info. ****** \n");
            break;
        }
        if (stpMatchCnt->resvCheckValue != DWCi_SwapU32(data[11])) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "****** Ignore RESV_OK. wrong resvCheckValue. ****** \n");
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "****** myvalue:%u  wrong value:%u. ****** \n",
                       stpMatchCnt->resvCheckValue, DWCi_SwapU32(data[11]));
            break;
        }
        if (len >= 14) {
            BOOL meshReserve = DWCi_SwapU32(data[13]) == 1;
            if (meshReserve != stpMatchCnt->unk6C0) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "****** Ignore RESV_OK. wrong meshReserve. ****** \n");
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "****** my(%u) reserved:%s target(%u) reserved:%s. ****** \n",
                           stpMatchCnt->profileID, stpMatchCnt->unk6C0 ? "TRUE" : "FALSE",
                           DWCi_SwapU32(data[2]), meshReserve ? "TRUE" : "FALSE");
                break;
            }
        }
        stpMatchCnt->unk750 = 0;
        stpMatchCnt->unk770 = 0;
        stpMatchCnt->unk760 = 0;
        memset(&serverNode, 0, sizeof(DWCNodeInfo));
        serverNode.aid = (u8)DWCi_SwapU32(data[1]);
        serverNode.profileID = DWCi_SwapU32(data[2]);
        serverNode.ip = data[3];
        serverNode.port = (u16)DWCi_SwapU32(data[4]);
        serverNode.localIP = data[5];
        serverNode.localPort = (u16)DWCi_SwapU32(data[6]);
        serverNode.unk17 = (u8)DWCi_SwapU32(data[7]);
        memcpy(&serverNode.unk28, &data[8], sizeof(serverNode.unk28));
        groupID = DWCi_SwapU32(data[9]);
        if (!stpMatchCnt->unk6C0) {
            myNode = fn_806CCF00(stpMatchCnt->profileID);
            myNode->aid = (u8)DWCi_SwapU32(data[10]);
        }
        stpMatchCnt->tempNewNodeInfo = serverNode;
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Server IP:%x, port:%d\n", serverNode.ip, serverNode.localPort);
        if (stpMatchCnt->qr2MatchType == 3) {
            stpMatchCnt->groupID = groupID;
            stpMatchCnt->qr2NumEntry = numEntry;
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Recv Server AID:%d\n", serverNode.aid);
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Recv GroupID:%u.\n", stpMatchCnt->groupID);
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Recv Max Entry:%d.\n", stpMatchCnt->qr2NumEntry);
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Recv User Data: 0[%d] 1[%d] 2[%d] 3[%d].\n",
                       ((u8*)&stpMatchCnt->tempNewNodeInfo.unk28)[0], ((u8*)&stpMatchCnt->tempNewNodeInfo.unk28)[1],
                       ((u8*)&stpMatchCnt->tempNewNodeInfo.unk28)[2],
                       ((u8*)&stpMatchCnt->tempNewNodeInfo.unk28)[3]);
            if (myNode != NULL) {
                DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "My AID:%d.\n", myNode->aid);
            }
        }
        if (stpMatchCnt->qr2MatchSubType == 1 && stpMatchCnt->evalCallback && !stpMatchCnt->unk6C0 &&
            !stpMatchCnt->unk8C8) {
            fn_806CCB70(DWC_MATCH_STATE_CL_SEARCH_EVAL_HOST);
            if (fn_806C5EB0(DWCi_SBUpdateAsync(profileID))) {
                return FALSE;
            }
            break;
        }
        fn_806CCB70(DWC_MATCH_STATE_CL_NN);
        if (DWCi_HandleNNError(fn_806BC4C0(0, 0, &stpMatchCnt->tempNewNodeInfo))) {
            return FALSE;
        }
        break;
    }
    case 3: {
        u32 reason;
        if (stpMatchCnt->state != DWC_MATCH_STATE_CL_WAIT_RESV || profileID != stpMatchCnt->unk7AC) {
            break;
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Reservation was denied by %u.\n", profileID);
        reason = DWCi_SwapU32(data[0]);
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "%s\n", DWCi_GetResvDenyReasonString(reason));
        if (fn_806CD150() == TRUE) {
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
            break;
        }
        if (stpMatchCnt->unk8C8 || stpMatchCnt->qr2MatchSubType == 3) {
            fn_806B9500(reason == 16 ? (DWCError)12 : (DWCError)24, 0);
            return FALSE;
        }
        return fn_806C0C20(stpMatchCnt->unk7AC);
    }
    case 4:
        if (stpMatchCnt->state != DWC_MATCH_STATE_CL_WAIT_RESV) {
            break;
        }
        if (fn_806CD150() == TRUE) {
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
            break;
        }
        if (stpMatchCnt->unk8C8) {
            if (profileID != stpMatchCnt->unk7AC) {
                break;
            }
            if (!gpIsBuddy(stpMatchCnt->pGpObj, profileID)) {
                break;
            }
            stpMatchCnt->unk8E6 = 1;
            stpMatchCnt->unk753 = 0;
            stpMatchCnt->unk770 = 12000;
            stpMatchCnt->unk778 = DWCi_Np_GetTick();
            break;
        }
        if (profileID != stpMatchCnt->unk7AC) {
            break;
        }
        stpMatchCnt->unk778 = DWCi_Np_GetTick();
        stpMatchCnt->unk750 = 0;
        if (stpMatchCnt->qr2MatchSubType == 0) {
            DWCi_TempNewNodeInfo_Clear();
            fn_806CCB70(DWC_MATCH_STATE_CL_SEARCH_HOST);
            stpMatchCnt->unk708 = 1;
            stpMatchCnt->unk710 = DWCi_Np_GetTick();
        } else if (stpMatchCnt->qr2MatchSubType == 1) {
            DWCi_TempNewNodeInfo_Clear();
            fn_806C0980(TRUE, FALSE, 0);
        } else if (stpMatchCnt->qr2MatchSubType == 3) {
            stpMatchCnt->unk8E6 = 1;
            stpMatchCnt->unk753 = 0;
            stpMatchCnt->unk770 = 12000;
            stpMatchCnt->unk778 = DWCi_Np_GetTick();
        }
        break;
    case 5: {
        int level;
        if (!stpMatchCnt->unk18 || profileID != stpMatchCnt->unk700) {
            break;
        }
        if (fn_806CD150() == TRUE) {
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
            break;
        }
        if (stpMatchCnt->qr2MatchType == 2 && stpMatchCnt->tempNewNodeInfo.profileID &&
            fn_806B1680(stpMatchCnt->tempNewNodeInfo.profileID, FALSE) &&
            profileID == stpMatchCnt->tempNewNodeInfo.profileID) {
            gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
        }
        level = 2;
        if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) && !stpMatchCnt->unkD) {
            level = 1;
        }
        if (!fn_806C10C0(profileID, level)) {
            return FALSE;
        }
        break;
    }
    case 6: {
        SOSockAddrIn addr;
        u32 nnIP = data[0];
        u16 nnPort = (u16)DWCi_SwapU32(data[1]);
        DWCMatchControl* cnt;
        int state;
        if (stpMatchCnt->tempNewNodeInfo.profileID == 0) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "new node info is invaild. ignore.\n");
            break;
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN parent is behind same NAT as me. Received IP %x & port %d\n", nnIP,
                   nnPort);
        if (stpMatchCnt->unk8E0 != 2 && stpMatchCnt->qr2MatchType == 3 && !stpMatchCnt->unk6C0) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "I'm client. not in mesh making..ignore.\n");
            break;
        }
        state = stpMatchCnt->state;
        if (state == DWC_MATCH_STATE_CL_WAITING) {
            fn_806CCB70(DWC_MATCH_STATE_CL_NN);
        } else if (!((state == DWC_MATCH_STATE_CL_NN || state == DWC_MATCH_STATE_SV_OWN_NN) &&
                     profileID == stpMatchCnt->unk700)) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "But already canceled reservation.\n");
            break;
        }
        stpMatchCnt->resend.command = 0xFF;
        addr.addr = nnIP;
        addr.port = fn_806A4270(nnPort);
        stpMatchCnt->nnData.isChild = 1;
        cnt = stpMatchCnt;
        fn_806CA040(0, gt2GetSocketSOCKET(*cnt->pGt2Socket), &addr, &cnt->nnData);
        stpMatchCnt->unk730 = 0;
        break;
    }
    case 7: {
        DWCMatchControl* cnt;
        int newPID;
        u8 newAID;
        if (fn_806CD150() == TRUE) {
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        }
        cnt = stpMatchCnt;
        if (cnt->state != DWC_MATCH_STATE_CL_WAITING || profileID != fn_806CCED0(0)->profileID) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed NEW_PID_AID command.\n");
            break;
        }
        newPID = DWCi_SwapU32(data[0]);
        newAID = (u8)DWCi_SwapU32(data[1]);
        if (cnt->unk690.profileID) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore dup NEW_PID_AID command. I'm waiting pid(%u)..\n",
                       cnt->unk690.profileID);
            break;
        }
        cnt->unk8B8 = 0;
        if (stpMatchCnt->unk8E0 == 2) {
            stpMatchCnt->tempNewNodeInfo.profileID = newPID;
            stpMatchCnt->tempNewNodeInfo.aid = newAID;
        } else {
            stpMatchCnt->unk690.profileID = newPID;
        }
        cnt = stpMatchCnt;
        if (cnt->unk8A8 != NULL) {
            ((DWCNewClientCallback)cnt->unk8A8)(DWCi_GetFriendListIndex(newPID), cnt->unk8AC);
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Received new client's profileID = %u & aid = %d.\n", newPID, newAID);
        break;
    }
    case 9: {
        DWCMatchControl* cnt = stpMatchCnt;
        DWCNodeInfo* linkNode = fn_806CCED0(cnt->nodeInfoList.nodeNum - 1 - cnt->unk748);
        if (cnt->state == DWC_MATCH_STATE_SV_WAIT_CL_LINK && linkNode != NULL &&
            linkNode->profileID == DWCi_SwapU32(data[0])) {
            cnt->unk748++;
            fn_806C1AC0(0);
        } else {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Ignore delayed LINK_CLS_SUC command %d.\n", 9);
        }
        break;
    }
    case 8: {
        DWCNodeInfo* newNode;
        int newPID;
        u8 newAID;
        SOInAddr addr;
        if (stpMatchCnt->state != DWC_MATCH_STATE_CL_WAITING ||
            profileID != stpMatchCnt->tempNewNodeInfo.profileID) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed LINK_CLS_REQ command.\n");
            break;
        }
        newNode = &stpMatchCnt->tempNewNodeInfo;
        newPID = DWCi_SwapU32(data[0]);
        newAID = (u8)DWCi_SwapU32(data[2]);
        if (fn_806B14D0(newAID)) {
            u32 resendData[1];
            resendData[0] = DWCi_SwapU32(newPID);
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Resend command %d for delayed command %d.\n", 9, 8);
            if (DWCi_HandleMatchCommandError(fn_806BC860(9, profileID, newNode->ip, newNode->port, resendData, 1))) {
                return FALSE;
            }
            break;
        }
        stpMatchCnt->unk690 = stpMatchCnt->tempNewNodeInfo;
        newNode->profileID = newPID;
        newNode->aid = newAID;
        newNode->ip = data[3];
        newNode->port = (u16)DWCi_SwapU32(data[4]);
        newNode->localIP = data[5];
        newNode->localPort = (u16)DWCi_SwapU32(data[6]);
        newNode->unk17 = (u8)DWCi_SwapU32(data[7]);
        memcpy(&newNode->unk28, &data[8], sizeof(newNode->unk28));
        addr.addr = newNode->ip;
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Client IP:%s, port:%d\n", fn_806A420C(addr), newNode->port);
        fn_806CCB70(DWC_MATCH_STATE_CL_NN);
        stpMatchCnt->unk770 = 0;
        stpMatchCnt->unk760 = 0;
        fn_806BC4C0(0, 0, newNode);
        break;
    }
    case 10: {
        DWCMatchControl* cnt = stpMatchCnt;
        int newPID;
        u8 newAID;
        if (cnt->state != DWC_MATCH_STATE_CL_WAITING || profileID != fn_806CCED0(0)->profileID) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed LINK_CLS_REP command.\n");
            break;
        }
        newPID = DWCi_SwapU32(data[0]);
        newAID = (u8)DWCi_SwapU32(data[1]);
        if (newPID != cnt->tempNewNodeInfo.profileID || newAID != cnt->tempNewNodeInfo.aid) {
            DWC_Printf(DWC_REPORTFLAG_INFO, "LINK_CLS_REP: recovery aid(%d->%d) and profileid(%d->%d).\n",
                       cnt->tempNewNodeInfo.aid, newAID, cnt->tempNewNodeInfo.profileID, newPID);
            stpMatchCnt->tempNewNodeInfo.aid = newAID;
            stpMatchCnt->tempNewNodeInfo.profileID = newPID;
        } else {
            DWC_Printf(DWC_REPORTFLAG_INFO, "LINK_CLS_REP: client info is valid.\n");
        }
        break;
    }
    case 12:
        if (profileID != stpMatchCnt->tempNewNodeInfo.profileID) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed CANCEL command.\n");
            break;
        }
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Received cancel command from %u data[0] = %d.\n", profileID,
                   DWCi_SwapU32(data[0]));
        if (stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) {
            if (!fn_806C1430(profileID)) {
                return FALSE;
            }
            break;
        }
        if (stpMatchCnt->qr2MatchType == 3) {
            stpMatchCnt->friendIdx = profileID;
            stpMatchCnt->closing = TRUE;
            gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
            stpMatchCnt->closing = FALSE;
            fn_806C3ED0(0);
        }
        break;
    case 13:
    case 14:
    case 15:
        if (!fn_806C50A0(profileID, command, DWCi_SwapU32(data[0]))) {
            return FALSE;
        }
        break;
    case 16: {
        DWCNodeInfo* server = fn_806CCED0(0);
        u8 closing;
        int i;
        if (server == NULL) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "server node is NULL!!\n");
            return TRUE;
        }
        if (profileID != server->profileID) {
            return TRUE;
        }
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Close shutdown client.\n");
        closing = stpMatchCnt->closing;
        stpMatchCnt->closing = TRUE;
        for (i = 0; i < len; i++, data++) {
            DWCNodeInfo* closeNode = fn_806CCF00(DWCi_SwapU32(data[0]));
            if (closeNode == NULL && stpMatchCnt->unk690.profileID == DWCi_SwapU32(data[0])) {
                fn_806B1A00(stpMatchCnt->unk690.aid);
                stpMatchCnt->unk690.profileID = 0;
            } else if (closeNode != NULL) {
                fn_806B1A00(closeNode->aid);
            }
        }
        stpMatchCnt->closing = closing;
        break;
    }
    case 0x40:
        if (stpMatchCnt->tempNewNodeInfo.profileID && profileID == stpMatchCnt->tempNewNodeInfo.profileID) {
            if (DWCi_HandleMatchCommandError(fn_806BC860(0x41, profileID, ip, port, NULL, 0))) {
                return FALSE;
            }
        }
        break;
    case 0x41:
        break;
    case 0x52: {
        DWCNodeInfo* myNode;
        DWCNodeInfo* srcNode;
        u8 myAID;
        BOOL down;
        if (!stpMatchCnt->unkD) {
            break;
        }
        if (fn_806CB4C0()) {
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "WARN!! SVDOWNQUERY to Next server!!\n");
            return FALSE;
        }
        myNode = fn_806CCF00(stpMatchCnt->profileID);
        myAID = myNode != NULL ? myNode->aid : 0xFF;
        if (myAID == 0xFF) {
            return FALSE;
        }
        srcNode = fn_806CCF00(profileID);
        if ((srcNode != NULL ? srcNode->aid : 0xFF) == 0xFF) {
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "already next server downed[%u].\n", profileID);
            return FALSE;
        }
        if (profileID != fn_806CCED0(0)->profileID) {
            down = FALSE;
        } else {
            down = ((1 << myAID) & stpMatchCnt->unk8B8) != 0;
        }
        if (DWCi_HandleMatchCommandError(fn_806BC860(down ? 0x53 : 0x54, profileID, ip, port, NULL, 0))) {
            return FALSE;
        }
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Send ServerDown is %s:%u.\n", down ? "down" : "alive", profileID);
        stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
        if (down && stpMatchCnt->state == DWC_MATCH_STATE_CL_SVDOWN_2) {
            int i;
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
            for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
                fn_806CE700(fn_806CCED0(i)->aid);
            }
        }
        break;
    }
    case 0x53:
        if (!stpMatchCnt->unkD) {
            break;
        }
        if (fn_806CB4C0()) {
            DWCNodeInfo* srcNode = fn_806CCF00(profileID);
            u8 aid = srcNode != NULL ? srcNode->aid : 0xFF;
            int result;
            if (aid == 0xFF) {
                DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "WARN!! idx of PID %u is 0xff.\n", profileID);
                return FALSE;
            }
            stpMatchCnt->unk8B8 |= 1 << aid;
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Recv ServerDown State: ACK:%u.\n", profileID);
            result = fn_806BC860(0x53, profileID, ip, port, NULL, 0);
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Send ACK to [%u]: IP:%d Port:%d\n", profileID, ip, port);
            if (DWCi_HandleMatchCommandError(result)) {
                DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Send Failed!\n");
                return FALSE;
            }
        } else {
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Recv ACK From Backup.\n");
        }
        break;
    case 0x54:
        if (!stpMatchCnt->unkD) {
            break;
        }
        if (++stpMatchCnt->unk8CD > stpMatchCnt->nodeInfoList.nodeNum * 5) {
            fn_806B0980();
            break;
        }
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Recv ServerDown State: NAK:%u.\n", profileID);
        stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
        break;
    case 0x55:
        if (!stpMatchCnt->unkD) {
            break;
        }
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "recv SVDOWN_KEEP.\n");
        break;
    case 0x70:
        DWCi_SBUpdateAsync(0);
        stpMatchCnt->unk756 = 0;
        stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
        fn_806CCB70(DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST);
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Retry SB Search.\n");
        break;
    case 0x82: {
        int srcPID = DWCi_SwapU32(data[0]);
        u8 fromServer = (u8)DWCi_SwapU32(data[1]);
        u8 suspend = (u8)DWCi_SwapU32(data[2]);
        if (fn_806CCF00(srcPID) == NULL) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore suspend cmd(from [%u] who isn't in list).\n", srcPID);
            break;
        }
        if ((int)stpMatchCnt->qr2IsReserved == 2) {
            u8 aid = (u8)DWCi_SwapU32(data[3]);
            if (fromServer == 1) {
                DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore suspend cmd(from SV[%u],I'm SV).\n", srcPID);
                break;
            }
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Server: AID(%u) suspend(%u).\n", aid, suspend);
            fn_806CBB20(aid, suspend, 1);
        } else if ((int)stpMatchCnt->qr2IsReserved == 1) {
            if (fromServer == 0) {
                DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore suspend cmd(from CL[%u],I'm CL).\n", srcPID);
                break;
            }
            if (len == 2) {
                if (stpMatchCnt->unk30 != 1) {
                    stpMatchCnt->unk30 = 1;
                    stpMatchCnt->unk900 = 8000;
                    stpMatchCnt->unk20 = DWCi_Np_GetTick();
                    DWC_Printf(DWC_REPORTFLAG_DEBUG, "*** DWC_MATCH_COMMAND_SUSPEND_MATCH: get server responce.\n");
                } else {
                    DWC_Printf(DWC_REPORTFLAG_DEBUG,
                               "*** DWC_MATCH_COMMAND_SUSPEND_MATCH: It has not completed it yet.\n");
                }
            } else {
                u32 bitmap;
                stpMatchCnt->unk30 = 1;
                bitmap = DWCi_SwapU32(data[3]);
                DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "Client: suspend(%u). Bitmap(%u)\n", suspend, bitmap);
                stpMatchCnt->suspendBitmap = bitmap;
            }
        }
        break;
    }
    case 0x83:
        fn_806BAC80(data);
        break;
    default:
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Received unexpected matching command 0x%02x.\n", command);
        break;
    }
    return TRUE;
}

// @806BFB90
static BOOL DWCi_CanReserveServer(int profileID) {
    DWCNodeInfo* node = fn_806CCF00(profileID);
    if (node == NULL) {
        return FALSE;
    }
    if (node == fn_806CCF00(stpMatchCnt->profileID)) {
        return FALSE;
    }
    return fn_806B14D0(node->aid) == NULL;
}

// @806BFBA0
// NONMATCHING: 98.93% - register allocation / small scheduling differences
int fn_806BFBA0(int profileID, u32 ip, u16 port, u8 matchType, BOOL meshReserve, u32 topology,
                void* userData, BOOL illegal) {
    int result;
    stpMatchCnt->unk8E4 = 1;
    switch (stpMatchCnt->qr2IsReserved) {
    case 1:
        if (DWCi_CanReserveServer(profileID)) {
            if (!stpMatchCnt->unk6C0) {
                if (stpMatchCnt->state == DWC_MATCH_STATE_CL_WAITING) {
                    if (!stpMatchCnt->unk690.profileID) {
                        stpMatchCnt->unk6C0 = 1;
                        stpMatchCnt->unk6C4 = 0;
                        memset(&stpMatchCnt->unk6C8, 0, sizeof(DWCNodeInfo));
                        result = 2;
                        stpMatchCnt->resend.time = DWCi_Np_GetTick();
                        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** 1-A\n");
                    } else {
                        result = 4;
                        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** 1-B\n");
                    }
                } else {
                    result = 4;
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** 2\n");
                }
            } else if (stpMatchCnt->state == DWC_MATCH_STATE_CL_WAIT_RESV) {
                DWCNodeInfo* myNode = fn_806CCF00(stpMatchCnt->profileID);
                if (profileID == stpMatchCnt->tempNewNodeInfo.profileID) {
                    if (myNode->profileID < profileID) {
                        result = 2;
                        stpMatchCnt->resend.time = DWCi_Np_GetTick();
                        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** 3\n");
                    } else {
                        result = 0xFF;
                        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** 4\n");
                    }
                } else {
                    result = 4;
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** 5\n");
                }
            } else {
                result = 4;
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** 6\n");
            }
        } else {
            stpMatchCnt->unk8E4 = 20;
            result = 3;
        }
        break;
    case 0:
    case 2:
        result = fn_806BFEA0(profileID, ip, port, matchType, meshReserve, topology, userData, illegal);
        break;
    }
    return result;
}

// @806BFEA0
// NONMATCHING: 88.06% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
int fn_806BFEA0(int profileID, u32 ip, u16 port, u8 matchType, BOOL meshReserve, u32 topology,
               void* userData, BOOL illegal) {
    u8 result;
    if (DWCi_IsMatchLocked()) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "*** this server is locked!!!\n");
        stpMatchCnt->unk8E4 = 19;
        return 3;
    }
    if (illegal) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "*** illegal mesh reserve command(%u).\n", profileID);
        stpMatchCnt->unk8E4 = 24;
        return 3;
    }
    if (!meshReserve) {
        if ((matchType == 0 && stpMatchCnt->qr2MatchSubType != 0) ||
            (matchType == 1 && stpMatchCnt->qr2MatchSubType != 1) ||
            (matchType == 3 && stpMatchCnt->qr2MatchSubType != 2)) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** my match type(%d) resv match type(%d).\n",
                       stpMatchCnt->qr2MatchSubType, matchType);
            stpMatchCnt->unk8E4 = 18;
            return 3;
        }
        if (stpMatchCnt->qr2MatchSubType == 1) {
            BOOL found;
            if (!gpIsBuddy(stpMatchCnt->pGpObj, profileID) || DWCi_GetFriendListIndex(profileID) == -1) {
                return 0xFF;
            }
            if (stpMatchCnt->friendList == NULL) {
                found = FALSE;
            } else {
                int i;
                found = FALSE;
                for (i = 0; i < stpMatchCnt->friendIdxListLen; i++) {
                    int pid = DWCi_GetProfileIDFromList(stpMatchCnt->friendIdxList[i]);
                    if (pid > 0 && pid == profileID) {
                        found = TRUE;
                        break;
                    }
                }
            }
            if (!found) {
                stpMatchCnt->unk8E4 = 21;
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "This friend doesn't exist in friendIdxList.\n");
                return 3;
            }
        } else if (stpMatchCnt->qr2MatchSubType == 2) {
            if (!gpIsBuddy(stpMatchCnt->pGpObj, profileID) || DWCi_GetFriendListIndex(profileID) == -1) {
                return 0xFF;
            }
        }
    }
    if (stpMatchCnt->qr2NumEntry + 1 == fn_806B0DE0()) {
        stpMatchCnt->unk8E4 = 16;
        result = 3;
    } else if (stpMatchCnt->state == DWC_MATCH_STATE_SEARCH_OWN) {
        result = 4;
    } else if (fn_806B3CC0() || stpMatchCnt->suspending) {
        stpMatchCnt->unk8E4 = 17;
        result = 3;
    } else if (topology != stpMatchCnt->unk8E0) {
        result = 3;
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** my topology(%d) resv topology(%d).\n", stpMatchCnt->unk8E0,
                   topology);
        stpMatchCnt->unk8E4 = 18;
    } else if (fn_806CCF00(profileID) != NULL) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "*** pid(%u) has already been in list.\n", profileID);
        result = 0xFF;
    } else if ((stpMatchCnt->state == DWC_MATCH_STATE_SV_WAITING ||
                stpMatchCnt->state == DWC_MATCH_STATE_CL_SEARCH_HOST ||
                stpMatchCnt->state == DWC_MATCH_STATE_CL_WAIT_RESV) &&
               (stpMatchCnt->qr2IP != 0 || stpMatchCnt->qr2Port != 0) && (ip != 0 || port != 0)) {
        if (stpMatchCnt->qr2MatchType != 2 && stpMatchCnt->unk7AC) {
            if (profileID == stpMatchCnt->unk7AC) {
                if (stpMatchCnt->profileID < profileID) {
                    result = 2;
                } else {
                    result = 0xFF;
                }
            } else {
                stpMatchCnt->unk8E4 = 23;
                result = 3;
            }
        } else {
            result = 2;
        }
    } else if (stpMatchCnt->state == DWC_MATCH_STATE_SV_OWN_NN && profileID == stpMatchCnt->unk700) {
        result = 0xFF;
    } else {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "please wait: qr2IP:%u qr2Port:%u qr2IPM:%u qr2PortM: %u\n", ip,
                   port, stpMatchCnt->qr2IP, stpMatchCnt->qr2Port);
        result = 4;
    }
    if (result == 2) {
        if (stpMatchCnt->resvCallback != NULL &&
            !stpMatchCnt->resvCallback(userData, stpMatchCnt->resvParam)) {
            stpMatchCnt->unk8E4 = 22;
            return 3;
        }
        if (stpMatchCnt->qr2IsReserved == 0) {
            fn_806CB550(TRUE);
        }
    }
    return result;
}

// @806C03A0
static int DWCi_NodeInfoList_FindAID(u8 aid) {
    int i;
    for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
        if (aid == stpMatchCnt->nodeInfoList.nodeInfo[i].aid) {
            break;
        }
    }
    return i;
}

// @806C03A8
static u8 DWCi_GetNewAID(void) {
    u32 aid;
    if (stpMatchCnt->nodeInfoList.nodeNum == 32) {
        return 0xFF;
    }
    for (aid = 0; aid < 32; aid++) {
        if (DWCi_NodeInfoList_FindAID((u8)aid) == stpMatchCnt->nodeInfoList.nodeNum) {
            return (u8)aid;
        }
    }
    return 0xFF;
}

// @806C03B0
// NONMATCHING: 98.34% - register allocation / small scheduling differences
int fn_806C03B0(int profileID, u32 ip, u16 port) {
    u32 data[2];
    DWCNodeInfo* myNode;
    int i;
    if (stpMatchCnt->unk18 && profileID == stpMatchCnt->unk700) {
        return 0;
    }
    stpMatchCnt->unk18 = 1;
    stpMatchCnt->unk700 = profileID;
    stpMatchCnt->unk760 = 0;
    stpMatchCnt->unk770 = 0;
    qr2_send_statechanged(stpMatchCnt->qr2Obj);
    stpMatchCnt->unk7AC = 0;
    stpMatchCnt->tempNewNodeInfo.aid = DWCi_GetNewAID();
    if (!stpMatchCnt->unk6C0) {
        data[0] = DWCi_SwapU32(stpMatchCnt->tempNewNodeInfo.profileID);
        data[1] = DWCi_SwapU32(stpMatchCnt->tempNewNodeInfo.aid);
        myNode = fn_806CCF00(stpMatchCnt->profileID);
        for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
            DWCNodeInfo* node = fn_806CCED0(i);
            if (myNode->profileID != node->profileID) {
                int result = fn_806BC860(7, node->profileID, node->ip, node->port, data, 2);
                if (result) {
                    return result;
                }
            }
        }
    }
    return 0;
}

// @806C05F0
// NONMATCHING: 87.34% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
int fn_806C05F0(int profileID, BOOL delay) {
    u32 data[11];
    u32 ip = 0;
    u16 port = 0;
    DWCNodeInfo* myNode;
    if (delay || (stpMatchCnt->qr2IP == 0 && stpMatchCnt->qr2Port == 0)) {
        stpMatchCnt->unk760 = 1;
        stpMatchCnt->unk768 = DWCi_Np_GetTick();
        stpMatchCnt->unk7AC = 0;
        if (!delay) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "Delay ResvCommand - qr2IP & qr2Port = 0.\n");
        }
        return 0;
    }
    if (stpMatchCnt->qr2MatchSubType == 0) {
        ip = stpMatchCnt->tempNewNodeInfo.ip;
        port = stpMatchCnt->tempNewNodeInfo.port;
        stpMatchCnt->unk7AC = profileID;
    } else {
        stpMatchCnt->unk7AC = profileID;
        if (!gpIsBuddy(stpMatchCnt->pGpObj, profileID)) {
            ip = stpMatchCnt->tempNewNodeInfo.ip;
            port = stpMatchCnt->tempNewNodeInfo.port;
        }
    }
    if (stpMatchCnt->qr2MatchSubType == 3 ||
        (stpMatchCnt->unk8C8 && gpIsBuddy(stpMatchCnt->pGpObj, profileID))) {
        stpMatchCnt->unk8E6 = 0;
        stpMatchCnt->unk770 = 30000;
    } else {
        stpMatchCnt->unk8E6 = 5;
        stpMatchCnt->unk770 = 12000;
    }
    stpMatchCnt->unk778 = DWCi_Np_GetTick();
    stpMatchCnt->unk760 = 0;
    myNode = fn_806CCF00(stpMatchCnt->profileID);
    data[0] = DWCi_SwapU32(stpMatchCnt->qr2MatchSubType);
    data[1] = myNode->ip;
    data[2] = DWCi_SwapU32(myNode->port);
    data[3] = myNode->localIP;
    data[4] = DWCi_SwapU32(myNode->localPort);
    data[5] = DWCi_SwapU32(myNode->unk17);
    data[6] = DWCi_SwapU32(stpMatchCnt->unk8C8);
    data[7] = DWCi_SwapU32(stpMatchCnt->unk8E0);
    data[8] = *(u32*)stpMatchCnt->unk8EC;
    stpMatchCnt->resvCheckValue = fn_806ABB70(0xFFFFFFFE) + 1;
    data[9] = DWCi_SwapU32(stpMatchCnt->resvCheckValue);
    data[10] = DWCi_SwapU32(stpMatchCnt->unk6C0 != 0);
    return fn_806BC860(1, profileID, ip, port, data, 11);
}

// @806C0970
static void DWCi_InitTempNewNodeInfo(int profileID) {
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    stpMatchCnt->tempNewNodeInfo.aid = 0xFF;
    stpMatchCnt->tempNewNodeInfo.profileID = profileID;
}

// @806C0980
// NONMATCHING: 93.60% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
int fn_806C0980(BOOL delay, BOOL keepIndex, int prevProfileID) {
    BOOL found = FALSE;
    int i;
    int profileID;
    if (!keepIndex) {
        stpMatchCnt->friendIdxListIdx = (stpMatchCnt->friendIdxListIdx + 1) % stpMatchCnt->friendIdxListLen;
    }
    stpMatchCnt->qr2MatchType = 3;
    for (i = 0; i < stpMatchCnt->friendIdxListLen;
         stpMatchCnt->friendIdxListIdx = (++i + stpMatchCnt->friendIdxListIdx) % stpMatchCnt->friendIdxListLen) {
        DWCMatchControl* cnt = stpMatchCnt;
        profileID = DWC_GetGsProfileId(fn_806AEAF0(),
                                       &cnt->friendList[cnt->friendIdxList[cnt->friendIdxListIdx]]);
        if (profileID != 0 && profileID != -1 &&
            DWCi_Acc_IsValidFriendData(
                &stpMatchCnt->friendList[stpMatchCnt->friendIdxList[stpMatchCnt->friendIdxListIdx]]) &&
            fn_806CCF00(profileID) == NULL) {
            int index;
            GPBuddyStatus status;
            int error = gpGetBuddyIndex(stpMatchCnt->pGpObj, profileID, &index);
            error |= gpGetBuddyStatus(stpMatchCnt->pGpObj, index, &status);
            if (fn_806CD3E0(&status)) {
                DWC_Printf(DWC_REPORTFLAG_DEBUG, "this buddy is locked. search next buddy...\n");
            } else if (error == 0 && status.status == 4) {
                found = TRUE;
                break;
            }
        }
    }
    if (found) {
        if (profileID == prevProfileID) {
            delay = TRUE;
        }
        DWCi_InitTempNewNodeInfo(profileID);
        return fn_806C05F0(profileID, delay);
    }
    stpMatchCnt->unk770 = 3000;
    stpMatchCnt->unk778 = DWCi_Np_GetTick();
    stpMatchCnt->unk760 = 0;
    return 0;
}

// @806C0C20
// NONMATCHING: 99.49% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C0C20(int prevProfileID) {
    stpMatchCnt->unk7AC = 0;
    stpMatchCnt->unk750 = 0;
    stpMatchCnt->unk778 = DWCi_Np_GetTick();
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    if (stpMatchCnt->unk8C8) {
        fn_806CCB70(DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST);
        if (fn_806C5EB0(DWCi_SBUpdateAsync(0))) {
            return FALSE;
        }
    } else if (stpMatchCnt->qr2MatchSubType == 0) {
        fn_806CCB70(DWC_MATCH_STATE_CL_SEARCH_HOST);
        if (prevProfileID) {
            stpMatchCnt->unk708 = 1;
            stpMatchCnt->unk710 = DWCi_Np_GetTick();
        } else if (fn_806C5EB0(DWCi_SBUpdateAsync(0))) {
            return FALSE;
        }
    } else if (stpMatchCnt->qr2MatchSubType == 1) {
        if (DWCi_HandleMatchCommandError(fn_806C0980(TRUE, FALSE, prevProfileID))) {
            return FALSE;
        }
    } else if (stpMatchCnt->qr2MatchSubType == 3) {
        fn_806B9500(DWC_ERROR_NETWORK, -80410);
        return FALSE;
    }
    return TRUE;
}

// @806C10B0
static BOOL DWCi_SendCancelMatchSynCommand(int profileID, u8 type) {
    u32 data[1];
    int len = 0;
    DWCNodeInfo* node;
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Sent CANCEL SYN %d command to %u.\n", type, profileID);
    if (type == 0) {
        data[0] = DWCi_SwapU32(stpMatchCnt->friendIdx);
        len = 1;
    }
    node = fn_806CCF00(profileID);
    if (node == NULL) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "DWCi_NodeInfoList_GetNodeInfoForProfileID is NULL\n");
        return FALSE;
    }
    if (DWCi_HandleMatchCommandError(
            fn_806BC860(0xD + type, profileID, node->ip, node->port, data, len))) {
        return FALSE;
    }
    stpMatchCnt->unk790 = DWCi_Np_GetTick();
    return TRUE;
}

// @806C10C0
// NONMATCHING: 76.05% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C10C0(int profileID, int level) {
    int i;
    if (stpMatchCnt->tempNewNodeInfo.profileID) {
        memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    }
    stpMatchCnt->resend.command = 0xFF;
    if (stpMatchCnt->nnData.cookie) {
        NNCancel(stpMatchCnt->nnData.cookie);
        stpMatchCnt->nnData.cookie = 0;
    }
    stpMatchCnt->unk7AC = 0;
    if (stpMatchCnt->qr2IsReserved == 2) {
        GT2Connection* connection;
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_CancelPreConnectedServerProcess : server\n");
        stpMatchCnt->unk18 = 0;
        stpMatchCnt->unk700 = 0;
        fn_806CCB70(DWC_MATCH_STATE_SV_CANCEL_SYN);
        stpMatchCnt->unk784 = 0;
        stpMatchCnt->unk75A = 0;
        stpMatchCnt->friendIdx = profileID;
        connection = fn_806B1680(profileID, FALSE);
        if (connection != NULL) {
            stpMatchCnt->closing = TRUE;
            gt2CloseConnectionHard(*connection);
            stpMatchCnt->closing = FALSE;
        }
        fn_806CCDB0(profileID);
        for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
            if (!DWCi_SendCancelMatchSynCommand(fn_806CCED0(i)->profileID, 0)) {
                return FALSE;
            }
        }
        if (stpMatchCnt->nodeInfoList.nodeNum == 1) {
            fn_806C3ED0(level);
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_CancelPreConnectedServerProcess : client\n");
        fn_806CD150();
    }
    return TRUE;
}

// @806C1420
static int DWCi_SendResvCancelCommand(void) {
    int result;
    if (stpMatchCnt->tempNewNodeInfo.profileID == 0) {
        return 0;
    }
    result = fn_806BC860(5, stpMatchCnt->tempNewNodeInfo.profileID, stpMatchCnt->tempNewNodeInfo.ip,
                         stpMatchCnt->tempNewNodeInfo.port, NULL, 0);
    stpMatchCnt->unk7AC = 0;
    return result;
}

// @806C1430
// NONMATCHING: 99.64% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C1430(int profileID) {
    BOOL result = TRUE;
    if (stpMatchCnt->unk8C8 || stpMatchCnt->qr2MatchSubType == 3) {
        if (stpMatchCnt->nodeInfoList.nodeNum > 1) {
            stpMatchCnt->closing = TRUE;
            gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
            stpMatchCnt->closing = FALSE;
        }
        if (stpMatchCnt->unk8C8) {
            fn_806B9500((DWCError)25, -80432);
        } else {
            fn_806B9500((DWCError)25, -80430);
        }
        return FALSE;
    }
    if (fn_806CD150() == TRUE) {
        return TRUE;
    }
    if (stpMatchCnt->unk690.profileID) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Clear temp new client info.\n");
        stpMatchCnt->unk690.profileID = 0;
    }
    if (stpMatchCnt->nnData.cookie) {
        NNCancel(stpMatchCnt->nnData.cookie);
        stpMatchCnt->nnData.cookie = 0;
    }
    if ((stpMatchCnt->unk8E0 == 2 && stpMatchCnt->unk757) ||
        (stpMatchCnt->tempNewNodeInfo.profileID && fn_806B1680(stpMatchCnt->tempNewNodeInfo.profileID, TRUE)) ||
        stpMatchCnt->nodeInfoList.nodeNum > 1) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Close all connection and restart matching.\n");
        if (stpMatchCnt->qr2MatchSubType == 2 || stpMatchCnt->qr2MatchSubType == 3) {
            DWC_Printf(DWC_REPORTFLAG_WARNING, "DWCi_RestartFromTimeout() shouldn't be called.\n");
        } else {
            stpMatchCnt->closing = TRUE;
            gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
            stpMatchCnt->closing = FALSE;
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Closed all connections and restart matching.\n");
            fn_806C3ED0(1);
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Cancel and restart client process.\n");
        fn_806CCB70(DWC_MATCH_STATE_CL_WAIT_RESV);
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Cancel and retry to reserve.\n");
        result = fn_806C0C20(0);
    }
    return result;
}

// @806C1AC0
// NONMATCHING: 98.25% - register allocation
void fn_806C1AC0(int level) {
    DWCMatchControl* cnt;
    int i;
    switch (level) {
    case 0:
        if (stpMatchCnt->unk8E0 == 2) {
            fn_806C3270();
        } else {
            fn_806C2F70();
        }
        break;
    case 1:
        fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        if (stpMatchCnt->qr2MatchType == 3) {
            stpMatchCnt->friendIdx = stpMatchCnt->tempNewNodeInfo.profileID;
        }
        break;
    case 2:
        fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        if (fn_806CD150() == TRUE) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh create completed.\n");
            stpMatchCnt->unk6C4 = 0;
            memset(&stpMatchCnt->unk6C8, 0, sizeof(DWCNodeInfo));
            break;
        }
        stpMatchCnt->qr2IsReserved = 1;
        if (stpMatchCnt->qr2Obj != NULL) {
            qr2_send_statechanged(stpMatchCnt->qr2Obj);
        }
        stpMatchCnt->qr2MatchType = 3;
        if (stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) {
            stpMatchCnt->unk18 = 1;
            stpMatchCnt->unk700 = stpMatchCnt->profileID;
        }
        stpMatchCnt->unk756 = 0;
        stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
        if (stpMatchCnt->unk8E0 != 2) {
            break;
        }
        if (stpMatchCnt->unk690.profileID) {
            u32 data[1];
            int result;
            data[0] = DWCi_SwapU32(stpMatchCnt->tempNewNodeInfo.profileID);
            DWCi_NodeInfoList_InsertTop(&stpMatchCnt->tempNewNodeInfo);
            stpMatchCnt->tempNewNodeInfo = stpMatchCnt->unk690;
            result = fn_806BC860(9, stpMatchCnt->tempNewNodeInfo.profileID, stpMatchCnt->tempNewNodeInfo.ip,
                                 stpMatchCnt->tempNewNodeInfo.port, data, 1);
            memset(&stpMatchCnt->unk690, 0, sizeof(DWCNodeInfo));
            if (stpMatchCnt->qr2MatchSubType == 0 ? fn_806C5EB0(result) : fn_806C5C10(result)) {
                return;
            }
        }
        stpMatchCnt->unk757 = 1;
        break;
    case 3:
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Accepted connectionCompleted matching!\n");
        stpMatchCnt->unkD = 1;
        stpMatchCnt->unk721 = 0;
        if (stpMatchCnt->unk8E0 == 2) {
            fn_806C2880();
            stpMatchCnt->unk757 = 0;
        } else {
            fn_806C2880();
        }
        stpMatchCnt->unk8C8 = 0;
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
        cnt = stpMatchCnt;
        cnt->matchedCallback(DWC_ERROR_NONE, FALSE, cnt->friendIdx == 0, FALSE,
                             DWCi_GetFriendListIndex(cnt->friendIdx), cnt->matchedParam);
        if (stpMatchCnt->qr2MatchType == 2) {
            if (fn_806C5C10(fn_806BB440(-1))) {
                return;
            }
            fn_806CCB70(DWC_MATCH_STATE_SV_WAITING);
        } else {
            fn_806BB440(2);
            fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        }
        stpMatchCnt->friendIdx = 0;
        stpMatchCnt->unk752 = 0;
        for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
            fn_806CE700(fn_806CCED0(i)->aid);
        }
        break;
    }
    if (stpMatchCnt->sbObj != NULL) {
        ServerBrowserClear(stpMatchCnt->sbObj);
        stpMatchCnt->unk718 = 0;
    }
}

// @806C2870
static void DWCi_NodeInfoList_Add(const DWCNodeInfo* node) {
    if (stpMatchCnt->nodeInfoList.nodeNum == 32) {
        return;
    }
    stpMatchCnt->nodeInfoList.nodeInfo[stpMatchCnt->nodeInfoList.nodeNum] = *node;
    stpMatchCnt->nodeInfoList.nodeNum++;
}

// @806C2874
static void DWCi_NodeInfoList_InsertTop(const DWCNodeInfo* node) {
    int i;
    if (stpMatchCnt->nodeInfoList.nodeNum == 32) {
        return;
    }
    for (i = stpMatchCnt->nodeInfoList.nodeNum; i > 0; i--) {
        stpMatchCnt->nodeInfoList.nodeInfo[i] = stpMatchCnt->nodeInfoList.nodeInfo[i - 1];
    }
    stpMatchCnt->nodeInfoList.nodeInfo[0] = *node;
    stpMatchCnt->nodeInfoList.nodeNum++;
}

// @806C2880
void fn_806C2880(void) {
    DWCMatchControl* cnt = stpMatchCnt;
    if (cnt->qr2MatchType == 2) {
        if (cnt->tempNewNodeInfo.profileID) {
            DWCi_NodeInfoList_Add(&cnt->tempNewNodeInfo);
            memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
        }
        return;
    }
    if (cnt->nodeInfoList.nodeNum == 1) {
        DWCi_NodeInfoList_InsertTop(&cnt->tempNewNodeInfo);
        memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
        return;
    }
    if (cnt->unk690.profileID) {
        DWCNodeInfo* node = fn_806CCF00(cnt->unk690.profileID);
        if (node != NULL) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "*** WARNING *** pid(%u) aid(%d) is in list..close\n",
                       node->profileID, node->aid);
            fn_806B1A00(node->aid);
        }
        node = fn_806CCF50(cnt->unk690.aid);
        if (node != NULL) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "*** WARNING *** pid(%u) aid(%d) is in list..close\n",
                       node->profileID, node->aid);
            fn_806B1A00(node->aid);
        }
        DWCi_NodeInfoList_Add(&cnt->unk690);
        cnt->unk690.profileID = 0;
    }
}

// @806C2F70
// NONMATCHING: 99.26% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C2F70(void) {
    BOOL result = FALSE;
    int i;
    if (stpMatchCnt->unk6C0 || stpMatchCnt->unk6C4 == DWC_MATCH_STATE_SV_OWN_GT2) {
        stpMatchCnt->unk18 = 0;
        stpMatchCnt->unk700 = 0;
        fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        fn_806CD150();
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh create completed.\n");
        return FALSE;
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Tell new client completion of matching.\n");
    stpMatchCnt->unk18 = 0;
    stpMatchCnt->unk700 = 0;
    if (!stpMatchCnt->unkD && stpMatchCnt->nodeInfoList.nodeNum == 1) {
        stpMatchCnt->unk8D0 = DWCi_Np_GetTick();
    }
    qr2_send_statechanged(stpMatchCnt->qr2Obj);
    if (stpMatchCnt->qr2MatchType == 2) {
        stpMatchCnt->friendIdx = stpMatchCnt->tempNewNodeInfo.profileID;
    }
    fn_806CCB70(DWC_MATCH_STATE_SV_SYN);
    stpMatchCnt->unk780 = 0;
    for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
        fn_806C4740(fn_806CCED0(i)->aid, 2);
    }
    fn_806C4740(stpMatchCnt->tempNewNodeInfo.aid, 2);
    if (stpMatchCnt->qr2MatchType != 2) {
        result = TRUE;
    }
    return result;
}

// @806C3270
// NONMATCHING: 84.86% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C3270(void) {
    DWCMatchControl* cnt = stpMatchCnt;
    DWCNodeInfo newNode = cnt->tempNewNodeInfo;
    u32 data[9];
    DWCNodeInfo* node;
    u8 index;
    if (cnt->unk748 >= cnt->nodeInfoList.nodeNum - 1) {
        cnt->unk748 = 0;
        return fn_806C2F70();
    }
    index = (u8)(cnt->nodeInfoList.nodeNum - 1 - cnt->unk748);
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Send client-client link request(%d).\n", index);
    fn_806CCB70(DWC_MATCH_STATE_SV_WAIT_CL_LINK);
    node = fn_806CCED0(index);
    data[0] = DWCi_SwapU32(node->profileID);
    data[1] = DWCi_SwapU32(index);
    data[2] = DWCi_SwapU32(node->aid);
    data[3] = node->ip;
    data[4] = DWCi_SwapU32(node->port);
    data[5] = node->localIP;
    data[6] = DWCi_SwapU32(node->localPort);
    data[7] = DWCi_SwapU32(node->unk17);
    data[8] = node->unk28;
    if (stpMatchCnt->state != DWC_MATCH_STATE_SV_SYN) {
        if (DWCi_HandleMatchCommandError(
                fn_806BC860(8, newNode.profileID, newNode.ip, newNode.port, data, 9))) {
            return FALSE;
        }
        node = fn_806CCED0(index);
        data[0] = DWCi_SwapU32(newNode.profileID);
        data[1] = DWCi_SwapU32(newNode.aid);
        if (DWCi_HandleMatchCommandError(
                fn_806BC860(10, node->profileID, node->ip, node->port, data, 2))) {
            return FALSE;
        }
        stpMatchCnt->resend.unk1 = 0;
    }
    return FALSE;
}

// @806C3670
// NONMATCHING: 94.49% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806C3670(void) {
    BOOL hasConnection;
    DWC_Printf(DWC_REPORTFLAG_DEBUG, "CANCEL! state %d, numHost=%d.\n", stpMatchCnt->state,
               stpMatchCnt->nodeInfoList.nodeNum);
    stpMatchCnt->friendIdx = 0;
    if (stpMatchCnt->state == DWC_MATCH_STATE_SEARCH_OWN) {
        DWCi_FinishCancelMatching();
        return;
    }
    hasConnection = fn_806B15B0() != 0;
    stpMatchCnt->unk752 = 1;
    if (stpMatchCnt->qr2MatchType == 3) {
        if (stpMatchCnt->nnData.cookie) {
            NNCancel(stpMatchCnt->nnData.cookie);
            stpMatchCnt->nnData.cookie = 0;
        }
        stpMatchCnt->unk752 = 1;
        if (hasConnection) {
            gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
        } else if (DWCi_HandleMatchCommandError(DWCi_SendResvCancelCommand())) {
            return;
        }
        DWCi_FinishCancelMatching();
        return;
    }
    if ((u32)(stpMatchCnt->state - DWC_MATCH_STATE_CL_WAIT_RESV) <= 3 ||
        (u32)(stpMatchCnt->state - DWC_MATCH_STATE_SV_OWN_NN) <= 1) {
        u32 data[1];
        int result;
        if (stpMatchCnt->unk7AC && DWCi_HandleMatchCommandError(DWCi_SendResvCancelCommand())) {
            return;
        }
        data[0] = 0;
        result = 0;
        if (stpMatchCnt->unk18 && stpMatchCnt->unk700 && stpMatchCnt->unk700 != stpMatchCnt->profileID) {
            result = fn_806BC860(0xC, stpMatchCnt->unk700, stpMatchCnt->tempNewNodeInfo.ip,
                                 stpMatchCnt->tempNewNodeInfo.port, data, 1);
            stpMatchCnt->unk18 = 0;
            stpMatchCnt->unk700 = 0;
            memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
        }
        if (DWCi_HandleMatchCommandError(result)) {
            return;
        }
    }
    if (stpMatchCnt->nnData.cookie) {
        NNCancel(stpMatchCnt->nnData.cookie);
        stpMatchCnt->nnData.cookie = 0;
    }
    if (hasConnection) {
        stpMatchCnt->unk752 = 1;
        gt2CloseAllConnectionsHard(*stpMatchCnt->pGt2Socket);
    }
    DWCi_FinishCancelMatching();
}

// @806C3EC0
static void DWCi_FinishCancelMatching(void) {
    DWCMatchControl* cnt;
    BOOL isServer;
    if (fn_806C5C10(fn_806BB440(1))) {
        return;
    }
    fn_806BBCA0();
    cnt = stpMatchCnt;
    if (cnt->friendIdx) {
        isServer = TRUE;
    } else {
        isServer = cnt->qr2MatchType == 2;
    }
    cnt->matchedCallback(DWC_ERROR_NONE, TRUE, cnt->friendIdx == 0, isServer,
                         DWCi_GetFriendListIndex(cnt->friendIdx), cnt->matchedParam);
    stpMatchCnt->unk752 = 0;
}

// @806C3ED0
// NONMATCHING: 93.63% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806C3ED0(int level) {
    DWCMatchControl* cnt;
    if (level == 0) {
        DWCi_FinishCancelMatching();
        return;
    }
    fn_806BB6C0(level);
    cnt = stpMatchCnt;
    if (cnt->qr2MatchSubType == 2 || cnt->qr2MatchSubType == 3) {
        DWCMatchControl* cnt2 = stpMatchCnt;
        BOOL self = cnt2->friendIdx == 0;
        cnt->matchedCallback(DWC_ERROR_NONE, TRUE, self, FALSE, DWCi_GetFriendListIndex(cnt2->friendIdx),
                             cnt2->matchedParam);
        return;
    }
    if (cnt->qr2MatchSubType == 0) {
        if (cnt->unkD) {
            BOOL self = cnt->friendIdx == 0;
            cnt->matchedCallback(DWC_ERROR_NONE, TRUE, self, FALSE,
                                 DWCi_GetFriendListIndex(cnt->friendIdx), cnt->matchedParam);
        }
        if (level == 1) {
            stpMatchCnt->qr2IsReserved = 0;
            if (stpMatchCnt->qr2Obj != NULL) {
                qr2_send_statechanged(stpMatchCnt->qr2Obj);
            }
            fn_806CCB70(DWC_MATCH_STATE_SEARCH_OWN);
            if (fn_806C5EB0(DWCi_SBUpdateAsync(stpMatchCnt->profileID))) {
            }
        }
        return;
    }
    if (cnt->qr2MatchSubType == 1) {
        if (cnt->unkD) {
            BOOL self = cnt->friendIdx == 0;
            cnt->matchedCallback(DWC_ERROR_NONE, TRUE, self, FALSE,
                                 DWCi_GetFriendListIndex(cnt->friendIdx), cnt->matchedParam);
        }
        if (level == 1) {
            stpMatchCnt->qr2IsReserved = 0;
            if (stpMatchCnt->qr2Obj != NULL) {
                qr2_send_statechanged(stpMatchCnt->qr2Obj);
            }
            stpMatchCnt->unk8C8 = 0;
            fn_806CCB70(DWC_MATCH_STATE_SEARCH_OWN);
            if (fn_806C5EB0(DWCi_SBUpdateAsync(stpMatchCnt->profileID))) {
            }
        }
        return;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "ERROR - DWCi_RestartFromCancel : matchType %d, level %d\n",
               cnt->qr2MatchType, level);
}

// @806C4420
static u8 DWCi_GetAIDFromProfileID(int profileID) {
    DWCNodeInfo* node = fn_806CCF00(profileID);
    if (node == NULL) {
        return 0xFF;
    }
    return node->aid;
}

// @806C4430
// NONMATCHING: 96.06% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C4430(u32 bitmap) {
    int closeList[32];
    u32 sendList[32];
    int numSend = 0;
    int numClose = 0;
    BOOL closeTemp = FALSE;
    BOOL result = TRUE;
    int i;
    for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
        DWCNodeInfo* node = fn_806CCED0(i);
        if (node != NULL) {
            if ((1 << node->aid) & bitmap) {
                closeList[numClose++] = node->profileID;
            } else {
                sendList[numSend++] = DWCi_SwapU32(node->profileID);
            }
        }
    }
    if (stpMatchCnt->tempNewNodeInfo.profileID) {
        if ((1 << stpMatchCnt->tempNewNodeInfo.aid) & bitmap) {
            closeList[numClose] = stpMatchCnt->tempNewNodeInfo.profileID;
            numClose++;
        } else {
            closeTemp = TRUE;
            sendList[numSend] = DWCi_SwapU32(stpMatchCnt->tempNewNodeInfo.profileID);
            numSend++;
        }
    }
    for (i = 0; i < numClose; i++) {
        DWCNodeInfo* node = fn_806CCF00(closeList[i]);
        if (stpMatchCnt->tempNewNodeInfo.profileID && node == NULL &&
            closeList[i] == stpMatchCnt->tempNewNodeInfo.profileID) {
            node = &stpMatchCnt->tempNewNodeInfo;
        }
        if (node != NULL) {
            if (DWCi_HandleMatchCommandError(
                    fn_806BC860(0x10, closeList[i], node->ip, node->port, sendList, numSend))) {
                result = FALSE;
            }
        }
    }
    stpMatchCnt->closing = TRUE;
    if (closeTemp) {
        fn_806B1A00(stpMatchCnt->tempNewNodeInfo.aid);
        memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    }
    for (i = 0; i < numSend; i++) {
        u8 aid = DWCi_GetAIDFromProfileID(DWCi_SwapU32(sendList[i]));
        if (aid != 0xFF) {
            fn_806B1A00(aid);
        }
    }
    stpMatchCnt->closing = FALSE;
    return result;
}

// @806C4740
// NONMATCHING: 94.16% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806C4740(u8 aid, u16 type) {
    u8 buf[8];
    u8* data = buf;
    int size = 4;
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Sent SYN %d packet to aid %d.\n", type - 2, aid);
    switch (type) {
    case 2:
        if (stpMatchCnt->tempNewNodeInfo.profileID && aid == stpMatchCnt->tempNewNodeInfo.aid) {
            *(u32*)buf = DWCi_SwapU32(1);
        } else {
            u32* p;
            size = 0x1C;
            data = DWC_Alloc(DWC_ALLOCTYPE_BASE, 0x1C);
            if (data == NULL) {
                return;
            }
            p = (u32*)data;
            p[0] = 0;
            p[1] = DWCi_SwapU32((stpMatchCnt->tempNewNodeInfo.unk17 << 16) |
                                stpMatchCnt->tempNewNodeInfo.aid);
            p[2] = DWCi_SwapU32(stpMatchCnt->tempNewNodeInfo.profileID);
            p[3] = stpMatchCnt->tempNewNodeInfo.ip;
            p[4] = stpMatchCnt->tempNewNodeInfo.localIP;
            p[5] = DWCi_SwapU32((stpMatchCnt->tempNewNodeInfo.port << 16) |
                                stpMatchCnt->tempNewNodeInfo.localPort);
            p[6] = stpMatchCnt->tempNewNodeInfo.unk28;
        }
        break;
    case 3:
        buf[0] = (u8)stpMatchCnt->unk758;
        buf[1] = (u8)(stpMatchCnt->unk758 >> 8);
        break;
    case 4:
        if (stpMatchCnt->tempNewNodeInfo.profileID && aid == stpMatchCnt->tempNewNodeInfo.aid) {
            int num;
            if (stpMatchCnt->unk8E0 == 2) {
                num = 0;
            } else {
                num = stpMatchCnt->nodeInfoList.nodeNum - 1;
            }
            if (num == 0) {
                buf[0] = 0;
                buf[1] = 0;
                buf[2] = 0;
                buf[3] = 0;
                size = 8;
                ((u32*)buf)[1] = DWCi_SwapU32(stpMatchCnt->unk8F8);
            } else {
                u32* p;
                int idx;
                u8 i;
                size = num * 0x18 + 8;
                p = DWC_Alloc(DWC_ALLOCTYPE_BASE, size);
                if (p == NULL) {
                    return;
                }
                p[0] = DWCi_SwapU32(num);
                idx = 1;
                for (i = 0; i < num; i++) {
                    DWCNodeInfo* node = fn_806CCED0(i + 1);
                    p[idx++] = DWCi_SwapU32((node->unk17 << 16) | node->aid);
                    p[idx++] = DWCi_SwapU32(node->profileID);
                    p[idx++] = node->ip;
                    p[idx++] = node->localIP;
                    p[idx++] = DWCi_SwapU32((node->port << 16) | node->localPort);
                    p[idx++] = node->unk28;
                }
                p[idx] = DWCi_SwapU32(stpMatchCnt->unk8F8);
                data = (u8*)p;
            }
        } else {
            buf[0] = 0;
            buf[1] = 0;
            buf[2] = 0;
            buf[3] = 0;
            size = 8;
            ((u32*)buf)[1] = DWCi_SwapU32(stpMatchCnt->unk8F8);
        }
        break;
    }
    DWCi_SendReliable(type, aid, data, size);
    if (data != buf) {
        DWC_Free(DWC_ALLOCTYPE_BASE, data, 0);
    }
    stpMatchCnt->unk788 = DWCi_Np_GetTick();
}

// @806C4B20
static void DWCi_SendTypeMatchAck(void) {
    int i;
    if ((int)stpMatchCnt->qr2IsReserved != 2) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "[WARN] DWCi_SendTypeMatchAck: I'm not server!!\n");
        return;
    }
    for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
        fn_806C4740(fn_806CCED0(i)->aid, 4);
    }
    if (stpMatchCnt->tempNewNodeInfo.profileID) {
        fn_806C4740(stpMatchCnt->tempNewNodeInfo.aid, 4);
    }
    fn_806CCB70(DWC_MATCH_STATE_SV_SYN_WAIT);
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Wait max latency %d msec.\n", stpMatchCnt->unk758);
}

// @806C4B30
// NONMATCHING: 95.04% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C4B30(void) {
    DWCMatchControl* cnt = stpMatchCnt;
    u64 msec;
    int i;
    if (cnt->state != DWC_MATCH_STATE_CL_SYN && cnt->state != DWC_MATCH_STATE_SV_SYN &&
        cnt->state != DWC_MATCH_STATE_SV_SYN_CLOSE_WAIT && cnt->state != DWC_MATCH_STATE_SV_SYN_WAIT) {
        return TRUE;
    }
    msec = OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk788);
    switch (stpMatchCnt->state) {
    case DWC_MATCH_STATE_CL_SYN:
        if (msec > stpMatchCnt->unk7A0) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Timeout: [SYN] Connection to server was shut down.\n");
            stpMatchCnt->unk690.profileID = 0;
            if (!fn_806C1430(fn_806CCEA0()->profileID)) {
                return FALSE;
            }
        }
        break;
    case DWC_MATCH_STATE_SV_SYN:
        if (msec > 6000) {
            if (++stpMatchCnt->unk754 > 5) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                           "Timeout: wait SYN-ACK (aidbitmap 0x%x). Restart matching.\n",
                           stpMatchCnt->unk780);
                stpMatchCnt->unk754 = 0;
                stpMatchCnt->unk788 = DWCi_Np_GetTick();
                if (fn_806C4430(stpMatchCnt->unk780)) {
                    if (stpMatchCnt->nodeInfoList.nodeNum > 1 || stpMatchCnt->tempNewNodeInfo.profileID) {
                        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_ServerSynTimeoutExec: Continue Syn exec.\n");
                        stpMatchCnt->unk788 = DWCi_Np_GetTick();
                        fn_806CCB70(DWC_MATCH_STATE_SV_SYN_CLOSE_WAIT);
                    } else {
                        int level;
                        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWCi_ServerSynTimeoutExec: Restart match.\n");
                        level = 2;
                        if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) &&
                            !stpMatchCnt->unkD) {
                            level = 1;
                        }
                        fn_806C10C0(stpMatchCnt->friendIdx, level);
                    }
                }
            } else {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "** Timeout: resent SYN-ACK...(%d/%d)\n",
                           stpMatchCnt->unk754, 5);
                for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
                    DWCNodeInfo* node = fn_806CCED0(i);
                    if (!((1 << node->aid) & stpMatchCnt->unk780)) {
                        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "resent SYN-ACK to aid(%d).\n", node->aid);
                        fn_806C4740(node->aid, 2);
                    }
                }
                if (!((1 << stpMatchCnt->tempNewNodeInfo.aid) & stpMatchCnt->unk780)) {
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "resent SYN-ACK to aid(%d)(new client).\n",
                               stpMatchCnt->tempNewNodeInfo.aid);
                    fn_806C4740(stpMatchCnt->tempNewNodeInfo.aid, 2);
                }
            }
        }
        break;
    case DWC_MATCH_STATE_SV_SYN_CLOSE_WAIT:
        if (msec > 3000) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "DWC_MATCH_STATE_SV_SYN_CLOSE_WAIT: send ack command.\n");
            DWCi_SendTypeMatchAck();
        }
        break;
    case DWC_MATCH_STATE_SV_SYN_WAIT:
        if (msec > stpMatchCnt->unk758) {
            fn_806C1AC0(3);
        }
        break;
    }
    return TRUE;
}

// @806C50A0
// NONMATCHING: 97.33% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C50A0(int profileID, u8 command, int friendIdx) {
    int i;
    DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Received CANCEL SYN %d command from %u.\n", command - 0xD,
               profileID);
    if (fn_806B1230() != 5) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Ignore delayed CANCEL SYN.\n");
        return TRUE;
    }
    switch (command) {
    case 0xD:
        if (stpMatchCnt->state != DWC_MATCH_STATE_CL_CANCEL_SYN) {
            fn_806CCB70(DWC_MATCH_STATE_CL_CANCEL_SYN);
            stpMatchCnt->friendIdx = friendIdx;
            if (stpMatchCnt->unk690.profileID) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Clear temp new client info.\n");
                stpMatchCnt->unk690.profileID = 0;
            }
            memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
        }
        if (!DWCi_SendCancelMatchSynCommand(profileID, 1)) {
            return FALSE;
        }
        break;
    case 0xE:
        if (stpMatchCnt->state == DWC_MATCH_STATE_SV_CANCEL_SYN) {
            DWCMatchControl* cnt = stpMatchCnt;
            u64 latency = (u64)OS_TICKS_TO_MSEC((u64)(DWCi_Np_GetTick() - cnt->unk790)) / 2;
            u8 aid;
            u32 bitmap;
            if (latency > 300 && latency - 300 > cnt->unk75A) {
                cnt->unk75A = (u16)(latency - 300);
            }
            aid = DWCi_GetAIDFromProfileID(profileID);
            if (aid != 0xFF) {
                stpMatchCnt->unk784 |= 1 << aid;
            }
            bitmap = 0;
            for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
                bitmap |= 1 << fn_806CCED0(i)->aid;
            }
            if (bitmap == stpMatchCnt->unk784) {
                for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
                    if (!DWCi_SendCancelMatchSynCommand(fn_806CCED0(i)->profileID, 2)) {
                        return FALSE;
                    }
                }
                fn_806CCB70(DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT);
                DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "Wait max latency %d msec.\n", stpMatchCnt->unk75A);
            }
        } else {
            if (!DWCi_SendCancelMatchSynCommand(profileID, 2)) {
                return FALSE;
            }
        }
        break;
    case 0xF:
        if (stpMatchCnt->state == DWC_MATCH_STATE_CL_CANCEL_SYN) {
            fn_806C3ED0(2);
        }
        break;
    }
    return TRUE;
}

// @806C57F0
// NONMATCHING: 93.05% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806C57F0(void) {
    DWCMatchControl* cnt = stpMatchCnt;
    s64 msec;
    int i;
    if (cnt->state != DWC_MATCH_STATE_CL_CANCEL_SYN &&
        (u32)(cnt->state - DWC_MATCH_STATE_SV_CANCEL_SYN) > 1) {
        return TRUE;
    }
    msec = OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk790);
    switch (stpMatchCnt->state) {
    case DWC_MATCH_STATE_CL_CANCEL_SYN:
        if (msec > 6000) {
            if (!DWCi_SendCancelMatchSynCommand(fn_806CCED0(0)->profileID, 1)) {
                return FALSE;
            }
        }
        break;
    case DWC_MATCH_STATE_SV_CANCEL_SYN:
        if (msec > 6000) {
            if (++stpMatchCnt->unk755 > 5) {
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Timeout: wait cancel SYN-ACK (aidbitmap 0x%x).\n",
                           stpMatchCnt->unk784);
                if (!fn_806C4430(stpMatchCnt->unk784)) {
                    return FALSE;
                }
                if (stpMatchCnt->nodeInfoList.nodeNum > 1) {
                    stpMatchCnt->unk755 = 0;
                    stpMatchCnt->unk790 = DWCi_Np_GetTick();
                } else {
                    fn_806C3ED0(2);
                }
            } else {
                for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
                    DWCNodeInfo* node = fn_806CCED0(i);
                    if (!((1 << node->aid) & stpMatchCnt->unk784)) {
                        if (!DWCi_SendCancelMatchSynCommand(node->profileID, 0)) {
                            return FALSE;
                        }
                    }
                }
            }
        }
        break;
    case DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT:
        if (msec > stpMatchCnt->unk75A) {
            fn_806C3ED0(2);
        }
        break;
    }
    return TRUE;
}

// @806C5C10
int fn_806C5C10(GPResult result) {
    DWCError dwcError;
    int errorCode;
    if (result == 0) {
        return 0;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, GP error %d\n", result);
    switch (result) {
    case 1:
        dwcError = DWC_ERROR_FATAL;
        errorCode = -1;
        break;
    case 2:
        dwcError = DWC_ERROR_FATAL;
        errorCode = -2;
        break;
    case 3:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -10;
        break;
    case 4:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -20;
        break;
    }
    fn_806B9500(dwcError, errorCode - 81000);
    return result;
}

// @806C5EB0
int fn_806C5EB0(int result) {
    DWCError dwcError;
    int errorCode;
    if (result == 0) {
        return 0;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, SB error %d\n", result);
    switch (result) {
    case 1:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -50;
        break;
    case 2:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -30;
        break;
    case 3:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -20;
        break;
    case 4:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -40;
        break;
    case 5:
        dwcError = DWC_ERROR_FATAL;
        errorCode = -1;
        break;
    case 6:
        dwcError = DWC_ERROR_FATAL;
        errorCode = -2;
        break;
    }
    fn_806B9500(dwcError, errorCode - 85000);
    return result;
}

// @806C6180
int fn_806C6180(int result) {
    DWCError dwcError;
    int errorCode;
    if (result == 0) {
        return 0;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, QR2 error %d\n", result);
    switch (result) {
    case 1:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -50;
        break;
    case 2:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -60;
        break;
    case 3:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -30;
        break;
    case 4:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -80;
        break;
    case 5:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -20;
        break;
    }
    switch (fn_806B1230()) {
    case 2:
        fn_806AEB10(dwcError, errorCode - 64000);
        break;
    case 4:
        fn_806B9500(dwcError, errorCode - 84000);
        break;
    default:
        DWCi_SetError(dwcError, errorCode - 94000);
        break;
    }
    return result;
}

// @806C6470
int fn_806C6470(int result) {
    DWCError dwcError;
    int errorCode;
    if (result == 0) {
        return 0;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, GT2 error %d\n", result);
    switch (result) {
    case 1:
        dwcError = DWC_ERROR_FATAL;
        errorCode = -1;
        break;
    case 2:
    case 5:
        dwcError = DWC_ERROR_NONE;
        errorCode = 0;
        result = 0;
        break;
    case 3:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -10;
        break;
    case 4:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -30;
        break;
    case 6:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -70;
        break;
    case 7:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -80;
        break;
    }
    if (dwcError != DWC_ERROR_NONE) {
        fn_806B9500(dwcError, errorCode - 87000);
    }
    return result;
}

// @806C6750
// inferred: checks the keys every DWC server reports (always inlined)
inline static int DWCi_GetValidServerPID(SBServer server) {
    if (SBServerGetIntValueA(server, "numplayers", -1) == -1) {
        return 0;
    }
    if (SBServerGetIntValueA(server, "maxplayers", -1) == -1) {
        return 0;
    }
    if (SBServerGetIntValueA(server, "dwc_mtype", -1) == -1) {
        return 0;
    }
    if (SBServerGetIntValueA(server, "dwc_mver", -1) == -1) {
        return 0;
    }
    return SBServerGetIntValueA(server, "dwc_pid", 0);
}

// inferred: sets the temporary new node from a server browser entry (always inlined)
inline static void DWCi_SetTempNewNodeInfoFromServer(SBServer server) {
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    stpMatchCnt->tempNewNodeInfo.profileID = SBServerGetIntValueA(server, "dwc_pid", 0);
    stpMatchCnt->tempNewNodeInfo.aid = 0xFF;
    stpMatchCnt->tempNewNodeInfo.ip = SBServerGetPublicInetAddress(server);
    stpMatchCnt->tempNewNodeInfo.port = SBServerGetPublicQueryPort(server);
    stpMatchCnt->tempNewNodeInfo.localIP = SBServerGetPrivateInetAddress(server);
    stpMatchCnt->tempNewNodeInfo.localPort = SBServerGetPrivateQueryPort(server);
    stpMatchCnt->tempNewNodeInfo.unk17 = SBServerHasPrivateAddress(server);
}

// NONMATCHING: 96.14% - register allocation and scheduling differ
void DWCi_SBCallback(ServerBrowser sb, int reason, SBServer server, void* instance) {
    int i;
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "SBCallback : reason %d (state = %d)\n", reason, stpMatchCnt->state);
    stSBBusy++;
    if (stpMatchCnt->unk718) {
        if ((u32)(reason - 4) <= 1) {
            stpMatchCnt->unk718 = 0;
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout reset.(%s)\n", __FUNCTION__);
        } else {
            stpMatchCnt->unk718 = DWCi_Np_GetTick() + OS_MSEC_TO_TICKS(30000);
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "ServerBrowserLimitUpdate timeout update.(%s)\n", __FUNCTION__);
        }
    }
    switch (reason) {
    case 0:
        fn_806C8700(server);
        break;
    case 4:
        stpMatchCnt->unk718 = 0;
        for (i = 0; i < ServerBrowserCount(sb); i++) {
            server = ServerBrowserGetServer(sb, i);
            if (!DWCi_GetValidServerPID(server)) {
                ServerBrowserRemoveServer(sb, server);
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "Deleted server [%d].\n", i);
                i--;
            }
        }
        switch (stpMatchCnt->state) {
        case DWC_MATCH_STATE_SEARCH_OWN:
            for (i = 0; i < ServerBrowserCount(sb); i++) {
                server = ServerBrowserGetServer(sb, i);
                if (stpMatchCnt->qr2IP && stpMatchCnt->qr2IP == SBServerGetPublicInetAddress(server) && stpMatchCnt->qr2Port &&
                    stpMatchCnt->qr2Port == SBServerGetPublicQueryPort(server)) {
                    break;
                }
            }
            if (i < ServerBrowserCount(sb)) {
                DWCNodeInfo node;
                memset(&node, 0, sizeof(DWCNodeInfo));
                node.profileID = stpMatchCnt->profileID;
                node.aid = 0xFF;
                node.ip = stpMatchCnt->qr2IP;
                node.port = stpMatchCnt->qr2Port;
                node.localIP = SBServerGetPrivateInetAddress(server);
                node.localPort = SBServerGetPrivateQueryPort(server);
                node.unk17 = SBServerHasPrivateAddress(server);
                fn_806A9CA0(&node.unk28, stpMatchCnt->unk8EC, sizeof(node.unk28));
                if (stpMatchCnt->qr2MatchSubType == 0) {
                    DWCi_NodeInfoList_InsertTop(&node);
                    fn_806CC020();
                } else if (stpMatchCnt->qr2MatchSubType == 1) {
                    DWCi_NodeInfoList_InsertTop(&node);
                    fn_806CC020();
                } else if (stpMatchCnt->qr2MatchSubType == 2) {
                    node.aid = DWCi_GetNewAID();
                    DWCi_NodeInfoList_InsertTop(&node);
                    fn_806CC020();
                } else if (stpMatchCnt->qr2MatchSubType == 3) {
                    DWCi_NodeInfoList_InsertTop(&node);
                    fn_806CC020();
                }
            } else {
                stpMatchCnt->unk708 = 2;
                stpMatchCnt->unk710 = DWCi_Np_GetTick();
            }
            break;
        case DWC_MATCH_STATE_CL_SEARCH_HOST:
            fn_806C89D0(TRUE);
            fn_806C8BE0();
            if (ServerBrowserCount(sb)) {
                server = ServerBrowserGetServer(sb, 0);
                DWCi_SetTempNewNodeInfoFromServer(server);
                stpMatchCnt->qr2MatchType = 3;
                DWCi_SBStartup();
                if (stpMatchCnt->sbObj == NULL && fn_806C5EB0(5)) {
                    break;
                }
                fn_806CCB70(DWC_MATCH_STATE_CL_WAIT_RESV);
                DWCi_HandleMatchCommandError(fn_806C05F0(stpMatchCnt->tempNewNodeInfo.profileID, FALSE));
            } else {
                stpMatchCnt->unk708 = 2;
                stpMatchCnt->unk710 = DWCi_Np_GetTick();
                memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
            }
            break;
        case DWC_MATCH_STATE_CL_SEARCH_EVAL_HOST: {
            DWCMatchControl* cnt = stpMatchCnt;
            SOInAddr addr;
            int pid;
            addr.addr = cnt->tempNewNodeInfo.ip;
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "searchIP: %s, searchPort: %d\n", fn_806A420C(addr),
                       cnt->tempNewNodeInfo.port);
            while (ServerBrowserCount(sb)) {
                server = ServerBrowserGetServer(sb, 0);
                if (SBServerGetPublicInetAddress(server) == stpMatchCnt->tempNewNodeInfo.ip &&
                    SBServerGetPublicQueryPort(server) == stpMatchCnt->tempNewNodeInfo.port) {
                    break;
                }
                ServerBrowserRemoveServer(sb, server);
            }
            if (ServerBrowserCount(sb) == 0) {
                stpMatchCnt->unk708 = 2;
                stpMatchCnt->unk710 = DWCi_Np_GetTick();
                break;
            }
            pid = SBServerGetIntValueA(ServerBrowserGetServer(sb, 0), "dwc_pid", 0);
            if (stpMatchCnt->qr2MatchSubType == 1) {
                if (pid != stpMatchCnt->unk7AC) {
                    DWC_Printf(DWC_REPORTFLAG_MATCH_NN,
                               "@@@@ @@@ BAD **** profileID != DWCi_GetMatchCnt()->reqProfileID ****.\n");
                }
                if (pid != stpMatchCnt->unk7AC || !fn_806C89D0(FALSE)) {
                    if (DWCi_HandleMatchCommandError(DWCi_SendResvCancelCommand())) {
                        break;
                    }
                    fn_806CCB70(DWC_MATCH_STATE_CL_WAIT_RESV);
                    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
                    DWCi_HandleMatchCommandError(fn_806C0980(FALSE, FALSE, stpMatchCnt->unk7AC));
                    break;
                }
            }
            fn_806CCB70(DWC_MATCH_STATE_CL_NN);
            server = ServerBrowserGetServer(sb, 0);
            DWCi_HandleNNError(fn_806BC4C0(0, 0, &stpMatchCnt->tempNewNodeInfo));
            break;
        }
        case DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST: {
            int num = ServerBrowserCount(sb);
            SBServer best;
            DWCMatchControl* cnt;
            if (num) {
                if (num > 1) {
                    int maxNum = -1;
                    for (i = 0; i < num; i++) {
                        int n;
                        server = ServerBrowserGetServer(sb, i);
                        n = SBServerGetIntValueA(server, "numplayers", -1);
                        DWC_Printf(DWC_REPORTFLAG_SB_UPDATE,
                                   "*** groupID search *** groupID: find server numplayers: %d.\n", n);
                        if (maxNum < n) {
                            best = server;
                            maxNum = n;
                            DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "*** groupID search *** select this server(%d).\n",
                                       n);
                        }
                    }
                } else {
                    best = ServerBrowserGetServer(sb, 0);
                }
                DWCi_SetTempNewNodeInfoFromServer(best);
                stpMatchCnt->qr2MatchSubType = SBServerGetIntValueA(best, "dwc_mtype", -1);
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "Find server num by groupID: %d.\n", num);
                cnt = stpMatchCnt;
                fn_806B4D20(cnt->unk8E0, SBServerGetIntValueA(best, "dwc_pid", 0), cnt->matchedCallback, cnt->matchedParam,
                            cnt->unk8A8, cnt->unk8AC, cnt->resvCallback, cnt->unk8EC, cnt->resvParam);
            } else {
                stpMatchCnt->unk708 = 2;
                stpMatchCnt->unk710 = DWCi_Np_GetTick();
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "Find server: host not found.\n");
            }
            break;
        }
        }
        break;
    }
    stSBBusy--;
}

// @806C8700
void fn_806C8700(SBServer server) {
    int i;
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "SBServerGetPrivateAddress     = %s\n", SBServerGetPrivateAddress(server));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "SBServerGetPrivateInetAddress = %x\n", SBServerGetPrivateInetAddress(server));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "SBServerGetPrivateQueryPort   = %d\n", SBServerGetPrivateQueryPort(server));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "SBServerGetPublicAddress      = %s\n", SBServerGetPublicAddress(server));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "SBServerGetPublicInetAddres   = %x\n", SBServerGetPublicInetAddress(server));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "SBServerGetPublicQueryPort    = %d\n", SBServerGetPublicQueryPort(server));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "SBServerHasPrivateAddress     = %d\n", SBServerHasPrivateAddress(server));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "numplayers  = %d\n", SBServerGetIntValueA(server, "numplayers", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "maxplayers  = %d\n", SBServerGetIntValueA(server, "maxplayers", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s     = %u\n", "dwc_pid", SBServerGetIntValueA(server, "dwc_pid", 0));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %d\n", "dwc_mtype", SBServerGetIntValueA(server, "dwc_mtype", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %d\n", "dwc_mver", SBServerGetIntValueA(server, "dwc_mver", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %d\n", "dwc_groupid",
               SBServerGetIntValueA(server, "dwc_groupid", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %d\n", "dwc_hoststate",
               SBServerGetIntValueA(server, "dwc_hoststate", -1));
    DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s   = %d\n", "dwc_suspend",
               SBServerGetIntValueA(server, "dwc_suspend", -1));
    for (i = 0; i < DWC_QR2_GAME_KEY_NUM; i++) {
        if (stGameMatchKeys[i].keyID) {
            if (stGameMatchKeys[i].isStr) {
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s  = %s\n", stGameMatchKeys[i].keyStr,
                           SBServerGetStringValueA(server, stGameMatchKeys[i].keyStr, "NONE"));
            } else {
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "%s  = %d\n", stGameMatchKeys[i].keyStr,
                           SBServerGetIntValueA(server, stGameMatchKeys[i].keyStr, -1));
            }
        }
    }
}

// @806C89D0
BOOL fn_806C89D0(BOOL sort) {
    BOOL deleted = FALSE;
    int i;
    for (i = 0; i < ServerBrowserCount(stpMatchCnt->sbObj); i++) {
        SBServer server = ServerBrowserGetServer(stpMatchCnt->sbObj, i);
        if (stpMatchCnt->qr2MatchSubType == 0) {
            int pid = SBServerGetIntValueA(server, "dwc_pid", 0);
            BOOL removed = FALSE;
            if (fn_806CCF00(pid) != NULL) {
                ServerBrowserRemoveServer(stpMatchCnt->sbObj, server);
                removed = TRUE;
                i--;
            }
            if (removed) {
                continue;
            }
        }
        if (stpMatchCnt->evalCallback != NULL) {
            int eval = stpMatchCnt->evalCallback(i, stpMatchCnt->evalParam);
            if (eval > 0) {
                if (eval > 0x7FFFFF) {
                    eval = 0x7FFFFF;
                }
                SBServerAddIntKeyValue(server, "dwc_eval", (eval << 8) | fn_806ABB70(0x100));
            } else {
                ServerBrowserRemoveServer(stpMatchCnt->sbObj, server);
                DWC_Printf(DWC_REPORTFLAG_SB_UPDATE, "Deleted server [%d] (eval point is %d).\n", i, eval);
                deleted = TRUE;
                i--;
            }
        } else {
            SBServerAddIntKeyValue(server, "dwc_eval", fn_806ABB70(0x80));
        }
    }
    if (sort && ServerBrowserCount(stpMatchCnt->sbObj)) {
        ServerBrowserSortA(stpMatchCnt->sbObj, 0, "dwc_eval", 0);
    }
    if (deleted && ServerBrowserCount(stpMatchCnt->sbObj) == 0) {
        return FALSE;
    }
    return TRUE;
}

// @806C8BE0
// NONMATCHING: 99.07% - register allocation / small scheduling differences
void fn_806C8BE0(void) {
    int ratio[6];
    int maxEval = 0;
    int total = 0;
    int i;
    u32 r;
    if (ServerBrowserCount(stpMatchCnt->sbObj) <= 1) {
        return;
    }
    for (i = 0; i < ServerBrowserCount(stpMatchCnt->sbObj); i++) {
        int eval = SBServerGetIntValueA(ServerBrowserGetServer(stpMatchCnt->sbObj, i), "dwc_eval", -1);
        if (eval > maxEval) {
            maxEval = eval;
        }
        total += stServerSelectWeight[i];
    }
    r = fn_806ABB70(100);
    for (i = 0; i < ServerBrowserCount(stpMatchCnt->sbObj); i++) {
        if (i == ServerBrowserCount(stpMatchCnt->sbObj) - 1) {
            ratio[i] = 100;
            break;
        }
        ratio[i] = stServerSelectWeight[i] * 100 / total + (i > 0 ? ratio[i - 1] : 0);
        if (r < ratio[i]) {
            break;
        }
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Server[%d] is selected (%d/100: rand %d)\n", i, ratio[i], r);
    if (maxEval < 0x7FFFFFFF) {
        maxEval++;
    }
    SBServerAddIntKeyValue(ServerBrowserGetServer(stpMatchCnt->sbObj, i), "dwc_eval", maxEval);
    ServerBrowserSortA(stpMatchCnt->sbObj, 0, "dwc_eval", 0);
}

// @806C8DA0
static const char* DWCi_GetQR2KeyName(int keyid) {
    switch (keyid) {
    case 8:
        return "NUMPLAYERS";
    case 10:
        return "MAXPLAYERS";
    case 50:
        return "PID";
    case 51:
        return "TYPE";
    case 52:
        return "VER";
    case 53:
        return "EVAL";
    case 54:
        return "GROUPID";
    case 55:
        return "HOST";
    case 56:
        return "SUSPEND";
    default:
        return "Unknown";
    }
}

// @806C8DB0
// NONMATCHING: 98.18% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806C8DB0(int keyid, void* outbuf, void* userdata) {
    switch (keyid) {
    case 8: {
        int num = stpMatchCnt->nodeInfoList.nodeNum - 1;
        if (num < 0) {
            num = 0;
        }
        qr2_buffer_add_int(outbuf, num);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), num);
        break;
    }
    case 10:
        qr2_buffer_add_int(outbuf, stpMatchCnt->qr2NumEntry);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), stpMatchCnt->qr2NumEntry);
        break;
    case 50:
        qr2_buffer_add_int(outbuf, stpMatchCnt->profileID);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), stpMatchCnt->profileID);
        break;
    case 51:
        qr2_buffer_add_int(outbuf, stpMatchCnt->qr2MatchSubType);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), stpMatchCnt->qr2MatchSubType);
        break;
    case 52:
        qr2_buffer_add_int(outbuf, 90);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), 90);
        break;
    case 53:
        qr2_buffer_add_int(outbuf, 1);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), 1);
        break;
    case 54:
        qr2_buffer_add_int(outbuf, (int)stpMatchCnt->qr2IsReserved == 2 ? stpMatchCnt->groupID : 0);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid),
                   (int)stpMatchCnt->qr2IsReserved == 2 ? stpMatchCnt->groupID : 0);
        break;
    case 55:
        qr2_buffer_add_int(outbuf, stpMatchCnt->qr2IsReserved);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), stpMatchCnt->qr2IsReserved);
        break;
    case 56: {
        int suspended = stpMatchCnt->suspended;
        qr2_buffer_add_int(outbuf, suspended);
        DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received ServerKeyReq : keyID %d(%s) - %d\n", keyid,
                   DWCi_GetQR2KeyName(keyid), suspended);
        break;
    }
    default:
        if (keyid >= DWC_QR2_GAME_KEY_START &&
            keyid < DWC_QR2_GAME_KEY_START + DWC_QR2_GAME_KEY_NUM) {
            DWCGameMatchKeyData* key = &stGameMatchKeys[keyid - DWC_QR2_GAME_KEY_START];
            if (key->keyID) {
                if (key->isStr) {
                    qr2_buffer_addA(outbuf, (const char*)key->value);
                } else {
                    qr2_buffer_add_int(outbuf, *(int*)key->value);
                }
            }
        }
        break;
    }
}

// @806C9670
static void fn_806C9670(int keyid, int index, void* outbuf, void* userdata) {
}

// @806C9680
static void fn_806C9680(int keyid, int index, void* outbuf, void* userdata) {
}

// @806C9690
void fn_806C9690(int keytype, void* keybuffer, void* userdata) {
    int i;
    if (keytype == 0) {
        qr2_keybuffer_add(keybuffer, 8);
        qr2_keybuffer_add(keybuffer, 10);
        qr2_keybuffer_add(keybuffer, 50);
        qr2_keybuffer_add(keybuffer, 51);
        qr2_keybuffer_add(keybuffer, 52);
        qr2_keybuffer_add(keybuffer, 53);
        qr2_keybuffer_add(keybuffer, 54);
        qr2_keybuffer_add(keybuffer, 55);
        qr2_keybuffer_add(keybuffer, 56);
        for (i = 0; i < DWC_QR2_GAME_KEY_NUM; i++) {
            if (stGameMatchKeys[i].keyID) {
                qr2_keybuffer_add(keybuffer, stGameMatchKeys[i].keyID);
            }
        }
    }
    DWC_Printf(DWC_REPORTFLAG_QR2_REQ, "QR2, Received KeyListReq : keytype %d\n", keytype);
}

// @806C9790
static int fn_806C9790(int keytype, void* userdata) {
    return 0;
}

// @806C97A0
void fn_806C97A0(int error, char* errmsg, void* userdata) {
    DWC_Printf(DWC_REPORTFLAG_ERROR, "QR2 Failed query addition to master server %d\n", error);
    DWC_Printf(DWC_REPORTFLAG_ERROR, "%s\n", errmsg);
    fn_806C6180(error);
}

// @806C9810
void fn_806C9810(unsigned int ip, unsigned short port, void* userdata) {
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Got my query IP %s & port %d.\n", fn_806A420C(*(SOInAddr*)&ip), port);
    stpMatchCnt->qr2IP = ip;
    stpMatchCnt->qr2Port = port;
}

// @806C9870
static int DWCi_HandleNNError(int result) {
    DWCError dwcError;
    int errorCode;
    if (result == 0) {
        return 0;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Match, NN error %d\n", result);
    switch (result) {
    case 1:
        dwcError = DWC_ERROR_FATAL;
        errorCode = -1;
        break;
    case 2:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -50;
        break;
    case 3:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = -30;
        break;
    }
    fn_806B9500(dwcError, errorCode - 86000);
    return result;
}

// @806C9880
// NONMATCHING: 95.92% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806C9880(int cookie, void* userdata) {
    int state;
    int result;
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Got NN request, cookie = %x.\n", cookie);
    if (!fn_806B12F0() && stpMatchCnt->unk690.profileID) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "But I'm waiting new client...\n", cookie);
        return;
    }
    state = stpMatchCnt->state;
    if (state == DWC_MATCH_STATE_CL_WAITING) {
        if (stpMatchCnt->unk8E0 != 2) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "I'm in CL_WAITING stat..ignore.\n");
            return;
        }
        fn_806CCB70(DWC_MATCH_STATE_CL_NN);
    } else if (state != DWC_MATCH_STATE_CL_NN && state != DWC_MATCH_STATE_SV_OWN_NN) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "But already canceled reservation.\n");
        return;
    }
    if (cookie == stpMatchCnt->unk724) {
        stpMatchCnt->unk720++;
    } else {
        stpMatchCnt->unk720 = 0;
        stpMatchCnt->unk724 = cookie;
    }
    stpMatchCnt->unk728 = 0;
    result = DWCi_HandleNNError(fn_806BC4C0(1, cookie, &stpMatchCnt->tempNewNodeInfo));
    if (result == 0) {
        stpMatchCnt->resend.command = 0xFF;
    }
}

// @806C9CE0
void fn_806C9CE0(const u8* message, u32 size) {
    DWCMatchCommand cmd;
    if (fn_806B1230() != 4 &&
        !(fn_806B1230() == 5 &&
          (stpMatchCnt->qr2MatchType == 2 || stpMatchCnt->qr2MatchType == 3))) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed GT2 matching command.\n");
        return;
    }
    fn_806A9CA0(&cmd.header, message, sizeof(cmd.header));
    cmd.header.ver = DWCi_SwapU32(cmd.header.ver);
    cmd.header.port = DWCi_SwapU16(cmd.header.port);
    cmd.header.profileID = DWCi_SwapU32(cmd.header.profileID);
    if (cmd.header.ver != 90) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "Got different version GT2 command.\n");
        return;
    }
    if (size != cmd.header.size + sizeof(cmd.header)) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "Got wrong data size GT2 command.\n");
        return;
    }
    fn_806A9CA0(cmd.data, message + sizeof(cmd.header), cmd.header.size);
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "<GT2> RECV-0x%02x <- [--------:-----] [pid=%u]\n", cmd.header.command,
               cmd.header.profileID);
    fn_806BCD40(cmd.header.command, cmd.header.profileID, cmd.header.ip, cmd.header.port, cmd.data, cmd.header.size >> 2);
}

// @806C9E60
void fn_806C9E60(const u8* message, int len, void* userdata) {
    DWCMatchCommand cmd;
    int pos = 0;
    if (fn_806B1230() != 4 &&
        !(fn_806B1230() == 5 &&
          (stpMatchCnt->qr2MatchType == 2 || stpMatchCnt->qr2MatchType == 3))) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed SB matching command.\n");
        return;
    }
    while (pos + (int)sizeof(cmd.header) <= len) {
        fn_806A9CA0(&cmd.header, message, sizeof(cmd.header));
        cmd.header.ver = DWCi_SwapU32(cmd.header.ver);
        cmd.header.port = DWCi_SwapU16(cmd.header.port);
        cmd.header.profileID = DWCi_SwapU32(cmd.header.profileID);
        if (strncmp(cmd.header.magic, "\xBB\x49\xCC\x4D", 4) != 0) {
            DWC_Printf(DWC_REPORTFLAG_WARNING, "Got undefined SBcommand.\n");
            return;
        }
        if (cmd.header.ver != 90) {
            DWC_Printf(DWC_REPORTFLAG_WARNING, "Got different version SBcommand.\n");
            return;
        }
        fn_806A9CA0(cmd.data, message + sizeof(cmd.header), cmd.header.size);
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "<SB> RECV-0x%02x <- [%08x:%d] [pid=%u]\n", cmd.header.command,
                   cmd.header.ip, cmd.header.port, cmd.header.profileID);
        if (!fn_806BCD40(cmd.header.command, cmd.header.profileID, cmd.header.ip, cmd.header.port, cmd.data,
                         cmd.header.size >> 2)) {
            return;
        }
        pos += cmd.header.size + sizeof(cmd.header);
    }
}

// @806CA020
void fn_806CA020(int state, void* userdata) {
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN, Got state update: %d\n", state);
}

// @806CA040
// inferred: common NN failure handling of the NN completed callback (always inlined)
inline static BOOL DWCi_CountNNFailure(void* remoteaddr) {
    if (stpMatchCnt->qr2MatchSubType != 3) {
        stpMatchCnt->unk721++;
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN failure %d/%d.\n", stpMatchCnt->unk721, 5);
    }
    if (stpMatchCnt->qr2MatchSubType == 3 || stpMatchCnt->unk8C8 || stpMatchCnt->unk721 >= 5) {
        if (remoteaddr) {
            stSBBusy++;
        }
        fn_806B9500(DWC_ERROR_NETWORK, -86420);
        if (remoteaddr) {
            stSBBusy--;
        }
        return FALSE;
    }
    return TRUE;
}

// NONMATCHING: 96.65% - register allocation and scheduling differ
void fn_806CA040(int result, int socket, void* remoteaddr, void* userdata) {
    DWCNNData* nnData = (DWCNNData*)userdata;
    SOSockAddrIn* addr = (SOSockAddrIn*)remoteaddr;
    DWCMatchControl* cnt;
    char pidStr[24];
    int nnResult;
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN, Complete NAT Negotiation. result : %d\n", result);
    if (nnData) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN cookie = %x.\n", nnData->cookie);
    }
    cnt = stpMatchCnt;
    if ((cnt->state != DWC_MATCH_STATE_CL_NN && cnt->state != DWC_MATCH_STATE_SV_OWN_NN) || !nnData) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed NN after cancel.\n");
        return;
    }
    if (result == 0) {
        if (addr) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN, remote address : %s\n",
                       gt2AddressToString(addr->addr, fn_806A4264(addr->port), 0));
        }
        if (nnData->isChild) {
            GT2Result gtResult;
            stpMatchCnt->tempNewNodeInfo.nnIP = addr->addr;
            stpMatchCnt->tempNewNodeInfo.nnPort = fn_806A4264(addr->port);
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN child finished Nat Negotiation.\n");
            stpMatchCnt->unk720 = 0;
            stpMatchCnt->unk724 = 0;
            stpMatchCnt->unk728 = 0;
            if (stpMatchCnt->state == DWC_MATCH_STATE_SV_OWN_NN) {
                fn_806CCB70(DWC_MATCH_STATE_SV_OWN_GT2);
            } else {
                fn_806CCB70(DWC_MATCH_STATE_CL_GT2);
            }

            stpMatchCnt->unkC = 0;
            DWC_Printf(DWC_REPORTFLAG_MATCH_GT2, "gt2Connect() to (%s)\n",
                       gt2AddressToString(stpMatchCnt->tempNewNodeInfo.nnIP, stpMatchCnt->tempNewNodeInfo.nnPort, 0));
            snprintf(pidStr, sizeof(pidStr), "%u", stpMatchCnt->profileID);
            cnt = stpMatchCnt;
            gtResult = gt2Connect(*cnt->pGt2Socket, NULL,
                                   gt2AddressToString(cnt->tempNewNodeInfo.nnIP, cnt->tempNewNodeInfo.nnPort, 0), pidStr,
                                   -1, 5000, cnt->gt2Callbacks, 0);
            cnt = stpMatchCnt;
            if (cnt->unk6C0 && gtResult) {
                fn_806CCF00(cnt->tempNewNodeInfo.profileID)->unk18++;
                DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh NN Count++.\n");
                fn_806CD150();
                fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
                return;
            }
            if (gtResult == 1) {
                fn_806C6470(gtResult);
            } else if (gtResult != 0) {
                int level = 2;
                if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) && !cnt->unkD) {
                    level = 1;
                }
                if (!fn_806C10C0(stpMatchCnt->tempNewNodeInfo.profileID, level)) {
                    return;
                }
            }
        } else {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "NN parent finished Nat Negotiation.\n");
            if (addr) {
                stpMatchCnt->tempNewNodeInfo.nnIP = addr->addr;
                stpMatchCnt->tempNewNodeInfo.nnPort = fn_806A4264(addr->port);
            }
            stpMatchCnt->unk730 = DWCi_Np_GetTick();
            fn_806CCB70(DWC_MATCH_STATE_CL_GT2);
        }
        return;
    }
    if (nnData->cookie == 0) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore delayed NN error after cancel.\n");
        return;
    }
    if (cnt->unk6C0) {
        fn_806CCF00(stpMatchCnt->tempNewNodeInfo.profileID)->unk18++;
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Mesh NN Count++.\n");
        fn_806CD150();
        fn_806CCB70(DWC_MATCH_STATE_CL_WAITING);
        return;
    }
    {
        DWCError error = DWC_ERROR_NONE;
        int errorCode = 0;
        if (result == 0) {
            nnResult = 0;
        } else {
            DWC_Printf(DWC_REPORTFLAG_WARNING, "Match, NN result %d\n", result);
            switch (result) {
            case 1:
                nnResult = 1;
                break;
            case 2:
                nnResult = 2;
                break;
            case 3:
                nnResult = 3;
                break;
            default:
                error = DWC_ERROR_NETWORK;
                errorCode = -86009;
                nnResult = result;
                break;
            }
            fn_806B9500(error, errorCode);
        }
    }
    if (nnResult < 1 || nnResult > 3) {
        return;
    }
    if (!nnData->isChild) {
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Failed %d/%d NN send.\n", nnData->unk1, 1);
        if (nnResult == 1 || nnResult == 3 || (nnResult == 2 && nnData->unk1 >= 1)) {
            DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Abort NN.\n");
            NNCancel(stpMatchCnt->nnData.cookie);
            nnData->cookie = 0;
            if (DWCi_CountNNFailure(addr)) {
                if (!fn_806C1430(stpMatchCnt->tempNewNodeInfo.profileID)) {
                    return;
                }
            }
        } else {
            nnData->unk1++;
            if (DWCi_HandleNNError(DWCi_DoNatNegotiation(nnData))) {
                return;
            }
        }
        return;
    }
    DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Failed %d/%d NN recv.\n", stpMatchCnt->unk720, 1);
    stpMatchCnt->unk728 = DWCi_Np_GetTick();
    if (nnResult == 1 || nnResult == 3 || (nnResult == 2 && stpMatchCnt->unk720 >= 1)) {
        int level;
        DWC_Printf(DWC_REPORTFLAG_MATCH_NN, "Abort NN.\n");
        NNCancel(stpMatchCnt->nnData.cookie);
        nnData->cookie = 0;
        if (stpMatchCnt->qr2MatchSubType != 3 && stpMatchCnt->qr2MatchSubType != 2 &&
            !DWCi_CountNNFailure(addr)) {
            return;
        }
        stpMatchCnt->unk720 = 0;
        stpMatchCnt->unk724 = 0;
        stpMatchCnt->unk728 = 0;
        level = 2;
        if ((stpMatchCnt->qr2MatchSubType == 0 || stpMatchCnt->qr2MatchSubType == 1) && !stpMatchCnt->unkD) {
            level = 1;
        }
        if (!fn_806C10C0(stpMatchCnt->tempNewNodeInfo.profileID, level)) {
        }
    }
}

// @806CB390
static int DWCi_HandleMatchCommandError(int result) {
    if (stpMatchCnt->qr2MatchSubType == 0) {
        return fn_806C5EB0(result);
    }
    return fn_806C5C10(result);
}

// @806CB3A0
BOOL fn_806CB3A0(void) {
    int i;
    int num = stpMatchCnt->nodeInfoList.nodeNum;
    for (i = 1; i < num; i++) {
        DWCNodeInfo* node = fn_806CCED0(i);
        u32 data[1];
        u8 command;
        data[0] = DWCi_SwapU32(fn_806B0F80());
        command = 0x52;
        if ((1 << node->aid) & stpMatchCnt->unk8B8) {
            command = 0x55;
        }
        if (DWCi_HandleMatchCommandError(
                fn_806BC860(command, node->profileID, node->ip, node->port, data, 1))) {
            return FALSE;
        }
    }
    stpMatchCnt->unk756 = 0;
    stpMatchCnt->unk7A0 = DWCi_Np_GetTick();
    return TRUE;
}

// @806CB4C0
BOOL fn_806CB4C0(void) {
    DWCNodeInfo* node = fn_806CCED0(0);
    if (node == NULL) {
        DWC_Printf(DWC_REPORTFLAG_INFO, "*** DWCi_IsBackupMine. ignored(state=%d)\n",
                   stpMatchCnt != NULL ? stpMatchCnt->state : 0);
        return FALSE;
    }
    return node->profileID == stpMatchCnt->profileID;
}

// @806CB550
// NONMATCHING: 97.02% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
void fn_806CB550(BOOL newGroup) {
    int status;
    if (newGroup) {
        u32 groupID = stpMatchCnt->profileID + (fn_806ABB70(10000000) + 100000000);
        DWC_Printf(DWC_REPORTFLAG_INFO, "Create New Group ID:%u\n", groupID);
        stpMatchCnt->groupID = groupID;
        fn_806CCF00(stpMatchCnt->profileID)->aid = 0;
        stpMatchCnt->unk8F4 = 1;
    }
    stpMatchCnt->qr2MatchType = 2;
    fn_806CCB70(DWC_MATCH_STATE_SV_WAITING);
    stpMatchCnt->qr2IsReserved = 2;
    if (stpMatchCnt->qr2Obj != NULL) {
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
    }
    stpMatchCnt->unk700 = 0;
    stpMatchCnt->unk18 = 0;
    if (stpMatchCnt->qr2MatchSubType == 0) {
        status = 3;
    } else {
        status = 6;
        if (stpMatchCnt->qr2MatchSubType == 1) {
            status = 4;
        }
    }
    if (stpMatchCnt->qr2MatchSubType == 3) {
        stpMatchCnt->qr2MatchSubType = 2;
    }
    fn_806C5C10(fn_806BB440(status));
    stpMatchCnt->unk7AC = 0;
}

// @806CB8F0
int fn_806CB8F0(BOOL force) {
    DWCMatchControl* cnt;
    u32 data[3];
    int i;
    if (!fn_806B12F0()) {
        return TRUE;
    }
    cnt = stpMatchCnt;
    switch (cnt->state) {
    case DWC_MATCH_STATE_SV_SYN:
    case DWC_MATCH_STATE_SV_SYN_WAIT:
        return TRUE;
    }
    if (force || OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - cnt->unk8D0) > 15000) {
        data[0] = DWCi_SwapU32(stpMatchCnt->profileID);
        data[1] = DWCi_SwapU32(fn_806B0F80());
        data[2] = DWCi_SwapU32(stpMatchCnt->unk8F8);
        for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
            DWCNodeInfo* node = fn_806CCED0(i);
            if (node != fn_806CCF00(stpMatchCnt->profileID)) {
                if (DWCi_HandleMatchCommandError(
                        fn_806BC860(0x83, node->profileID, node->ip, node->port, data, 3))) {
                    return -1;
                }
            }
        }
        stpMatchCnt->unk8D0 = DWCi_Np_GetTick();
    }
    return TRUE;
}

// @806CBB20
BOOL fn_806CBB20(u8 aid, int suspend, int fromCommand) {
    DWCNodeInfo* server = fn_806CCED0(0);
    u8 serverAid = server->aid;
    u32 oldBitmap = stpMatchCnt->suspendBitmap;
    u32 bitmap;
    BOOL sendAll;
    BOOL result;
    u32 data[4];
    int i;
    if (suspend) {
        stpMatchCnt->suspendBitmap = oldBitmap | (1 << aid);
    } else {
        stpMatchCnt->suspendBitmap = oldBitmap & ~(1 << aid);
    }
    if (oldBitmap != stpMatchCnt->suspendBitmap) {
        if (aid == serverAid) {
            DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "**Server** AID(%u) suspend(%u) bitmap(%u->%u).\n", aid,
                       suspend, oldBitmap, stpMatchCnt->suspendBitmap);
        }
        fn_806BB440(-1);
    }
    bitmap = stpMatchCnt->suspendBitmap;
    if (!DWCi_IsSuspendTarget()) {
        bitmap &= ~(1 << fn_806B0E30());
    }
    result = fn_806B3CC0();
    sendAll = TRUE;
    if ((suspend && result) == FALSE) {
        if ((suspend || result) != FALSE) {
            sendAll = FALSE;
        }
    }

    if (fromCommand == 1) {
        DWCNodeInfo* node = fn_806CCF50(aid);
        int len = 2;
        data[0] = DWCi_SwapU32(stpMatchCnt->profileID);
        data[1] = DWCi_SwapU32(1);
        if (sendAll && stpMatchCnt->unk30) {
            data[2] = DWCi_SwapU32(suspend);
            data[3] = DWCi_SwapU32(bitmap);
            len = 4;
        }
        if (DWCi_HandleMatchCommandError(
                fn_806BC860(0x82, node->profileID, node->ip, node->port, data, len))) {
            return FALSE;
        }
    }
    if (sendAll && stpMatchCnt->unk30 != 1) {
        stpMatchCnt->unk30 = 1;
        data[0] = DWCi_SwapU32(stpMatchCnt->profileID);
        data[1] = DWCi_SwapU32(1);
        data[2] = DWCi_SwapU32(suspend);
        data[3] = DWCi_SwapU32(bitmap);
        for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
            DWCNodeInfo* node = fn_806CCED0(i);
            if (node != server) {
                if (DWCi_HandleMatchCommandError(
                        fn_806BC860(0x82, node->profileID, node->ip, node->port, data, 4))) {
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

// @806CC020
// NONMATCHING: 98.73% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806CC020(void) {
    if (stpMatchCnt->sbObj != NULL) {
        ServerBrowserClear(stpMatchCnt->sbObj);
    }
    if (stpMatchCnt->unk8C8) {
        fn_806CCB70(DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST);
    } else if (stpMatchCnt->qr2MatchSubType == 0) {
        fn_806CCB70(DWC_MATCH_STATE_CL_SEARCH_HOST);
    } else if (stpMatchCnt->qr2MatchSubType == 1) {
        fn_806CCB70(DWC_MATCH_STATE_CL_WAIT_RESV);
        if (DWCi_HandleMatchCommandError(((int (*)(BOOL, BOOL))fn_806C0980)(FALSE, TRUE))) {
            return FALSE;
        }
        return TRUE;
    } else if (stpMatchCnt->qr2MatchSubType == 2) {
        fn_806CCB70(DWC_MATCH_STATE_SV_WAITING);
        stpMatchCnt->unkD = 1;
        return TRUE;
    } else if (stpMatchCnt->qr2MatchSubType == 3) {
        if (gpIsBuddy(stpMatchCnt->pGpObj, stpMatchCnt->tempNewNodeInfo.profileID)) {
            fn_806CCB70(DWC_MATCH_STATE_CL_WAIT_RESV);
            if (DWCi_HandleMatchCommandError(fn_806C05F0(stpMatchCnt->tempNewNodeInfo.profileID, FALSE))) {
                return FALSE;
            }
        }
        return TRUE;
    }
    stpMatchCnt->unk7AC = 0;
    return !fn_806C5EB0(DWCi_SBUpdateAsync(stpMatchCnt->unk7AC));
}

// @806CC540
static void DWCi_SetSuspendRequest(int suspend, DWCSuspendCallback callback, void* param) {
    stpMatchCnt->unk30 = 0;
    stpMatchCnt->suspendReq = suspend;
    stpMatchCnt->suspendCallback = callback;
    stpMatchCnt->suspendParam = param;
    stpMatchCnt->unk20 = 0;
    stpMatchCnt->suspendTime = DWCi_Np_GetTick();
    stpMatchCnt->suspending = TRUE;
}

// @806CC550
BOOL fn_806CC550(int suspend, DWCSuspendCallback callback, void* param) {
    if (DWCi_IsError() || stpMatchCnt == NULL || !stpMatchCnt->qr2IsReserved ||
        (fn_806B1230() != 4 && fn_806B1230() != 5) || !stpMatchCnt->qr2IsReserved ||
        (stpMatchCnt->state != DWC_MATCH_STATE_SV_WAITING &&
         stpMatchCnt->state != DWC_MATCH_STATE_CL_WAITING) ||
        callback == NULL || !stpMatchCnt->unkD) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "but ignored.\n");
        return FALSE;
    }
    if (stpMatchCnt->suspending) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "***<SUSPEND> suspend process(%s) has not ended yet!\n",
                   stpMatchCnt->suspendReq ? "TRUE" : "FALSE");
        return FALSE;
    }
    if (suspend != stpMatchCnt->suspendReq) {
        DWCi_SetSuspendRequest(suspend, callback, param);
    }
    return TRUE;
}

// @806CC6D0
BOOL fn_806CC6D0(void) {
    int suspend;
    BOOL result;
    if (!stpMatchCnt->suspending) {
        return FALSE;
    }
    if (stpMatchCnt->unk48) {
        DWCi_SetSuspendRequest(stpMatchCnt->suspendReq, stpMatchCnt->suspendCallback,
                               stpMatchCnt->suspendParam);
        stpMatchCnt->unk48 = 0;
    }
    suspend = stpMatchCnt->suspendReq;
    if (suspend == 0xFF) {
        return FALSE;
    }
    result = fn_806B3CC0();
    if ((suspend && result) || (!suspend && !result)) {
        DWC_Printf(DWC_REPORTFLAG_RECV_INFO, "suspend Match(%s) completed!\n", suspend ? "TRUE" : "FALSE");
        if (stpMatchCnt->suspendCallback != NULL) {
            stpMatchCnt->suspendCallback(DWC_ERROR_NONE, stpMatchCnt->suspendReq,
                                         stpMatchCnt->suspendParam);
        }
        qr2_send_statechanged(stpMatchCnt->qr2Obj);
        stpMatchCnt->suspendReq = 0xFF;
        stpMatchCnt->suspending = FALSE;
        return TRUE;
    }
    fn_806CC850(suspend);
    return TRUE;
}

// @806CC850
// NONMATCHING: 86.74% - register allocation and scheduling differ (inlined DWCi_SetMatchStatus/DWCi_SetGPStatus)
BOOL fn_806CC850(int suspend) {
    DWCNodeInfo* myNode;
    DWCNodeInfo* server;
    u32 data[4];
    if (stpMatchCnt->qr2IsReserved == 2) {
        return fn_806CBB20(fn_806CCF00(stpMatchCnt->profileID)->aid, suspend, 0);
    }
    if (stpMatchCnt->state != DWC_MATCH_STATE_CL_WAITING) {
        return TRUE;
    }
    if (stpMatchCnt->unk30 &&
        OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - stpMatchCnt->unk20) < stpMatchCnt->unk900) {
        return TRUE;
    }
    if (OS_TICKS_TO_MSEC(DWCi_Np_GetTick() - stpMatchCnt->unk20) > 3000) {
        myNode = fn_806CCF00(stpMatchCnt->profileID);
        server = fn_806CCED0(0);
        data[0] = DWCi_SwapU32(stpMatchCnt->profileID);
        data[1] = 0;
        data[2] = DWCi_SwapU32(suspend);
        data[3] = DWCi_SwapU32(myNode->aid);
        if (DWCi_HandleMatchCommandError(
                fn_806BC860(0x82, server->profileID, server->ip, server->port, data, 4))) {
            return FALSE;
        }
        stpMatchCnt->unk20 = DWCi_Np_GetTick();
        stpMatchCnt->unk900 = 3000;
    }
    return TRUE;
}

// @806CCB40
BOOL fn_806CCB40(void) {
    return (u32)(stpMatchCnt->state - DWC_MATCH_STATE_CL_SVDOWN_1) <= 2;
}

// @806CCB70
void fn_806CCB70(DWCMatchState state) {
    int oldState;
    s64 prev;
    int msec;
    int sec;
    oldState = stpMatchCnt->state;
    if (oldState != state) {
        prev = stLastStateTime;
        stLastStateTime = DWCi_Np_GetTick();
        msec = (int)OS_TICKS_TO_MSEC(stLastStateTime - prev);
        sec = msec / 1000;
        msec = (msec - sec + 50) / 100;
        DWC_Printf(DWC_REPORTFLAG_INFO, "status changed %s(%d.%d sec) -> %s\n", stStateNames[oldState],
                   sec, msec, stStateNames[state]);
    }
    stpMatchCnt->state = state;
}

// @806CCC70
BOOL fn_806CCC70(void) {
    return stpMatchCnt->unk8E0 != 1;
}

// @806CCC90
void fn_806CCC90(void) {
    memset(&stpMatchCnt->nodeInfoList, 0, sizeof(DWCNodeInfoList));
}

// @806CCCB0
int fn_806CCCB0(void) {
    return stpMatchCnt->nodeInfoList.nodeNum;
}

// @806CCCC0
// NONMATCHING: 95.92% - register allocation / small scheduling differences
BOOL fn_806CCCC0(u8 aid) {
    DWCMatchControl* cnt = stpMatchCnt;
    int i;
    if (cnt->nodeInfoList.nodeNum == 0) {
        return FALSE;
    }
    for (i = 0; i < cnt->nodeInfoList.nodeNum; i++) {
        if (aid == cnt->nodeInfoList.nodeInfo[i].aid) {
            break;
        }
    }
    if (i == cnt->nodeInfoList.nodeNum) {
        return FALSE;
    }
    for (; i < cnt->nodeInfoList.nodeNum - 1; i++) {
        stpMatchCnt->nodeInfoList.nodeInfo[i] = stpMatchCnt->nodeInfoList.nodeInfo[i + 1];
    }
    cnt->nodeInfoList.nodeNum--;
    return TRUE;
}

// @806CCDB0
// NONMATCHING: 92.28% - register allocation / small scheduling differences
BOOL fn_806CCDB0(int profileID) {
    DWCNodeInfoList* list = &stpMatchCnt->nodeInfoList;
    int i;
    if (list->nodeNum == 0) {
        return FALSE;
    }
    for (i = 0; i < list->nodeNum; i++) {
        if (profileID == list->nodeInfo[i].profileID) {
            break;
        }
    }
    if (i == list->nodeNum) {
        return FALSE;
    }
    for (; i < list->nodeNum - 1; i++) {
        stpMatchCnt->nodeInfoList.nodeInfo[i] = stpMatchCnt->nodeInfoList.nodeInfo[i + 1];
    }
    list->nodeNum = list->nodeNum - 1;
    return TRUE;
}

// @806CCEA0
DWCNodeInfo* fn_806CCEA0(void) {
    if (stpMatchCnt->nodeInfoList.nodeNum >= 1) {
        return &stpMatchCnt->nodeInfoList.nodeInfo[0];
    }
    return NULL;
}

// @806CCED0
DWCNodeInfo* fn_806CCED0(int index) {
    if (index < stpMatchCnt->nodeInfoList.nodeNum) {
        return &stpMatchCnt->nodeInfoList.nodeInfo[index];
    }
    return NULL;
}

// @806CCF00
DWCNodeInfo* fn_806CCF00(int profileID) {
    int pid = profileID; // inferred: local copy (gives the register order of the inlined copies)
    int i;
    for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
        if (pid == stpMatchCnt->nodeInfoList.nodeInfo[i].profileID) {

            return &stpMatchCnt->nodeInfoList.nodeInfo[i];
        }
    }
    return NULL;
}

// @806CCF50
DWCNodeInfo* fn_806CCF50(u8 aid) {
    int i;
    for (i = 0; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {
        if (aid == stpMatchCnt->nodeInfoList.nodeInfo[i].aid) {
            return &stpMatchCnt->nodeInfoList.nodeInfo[i];
        }
    }
    return NULL;
}

// @806CCFA0
// NONMATCHING: 71.53% - the loop is rotated differently and the 64-bit tick products are
// computed in another order (target multiplies count by the ticks per ms first)
s64 fn_806CCFA0(void) {
    DWCNodeInfo* myNode = fn_806CCF00(stpMatchCnt->profileID);
    int count = 0;
    int myIndex = 0;
    int i;
    s64 time = DWCi_Np_GetTick() + OS_MSEC_TO_TICKS((u64)3000);
    for (i = 1; i < stpMatchCnt->nodeInfoList.nodeNum; i++) {

        DWCNodeInfo* node = fn_806CCED0(i);
        if (myNode == node) {
            myIndex = i;
        } else {
            fn_806B14D0(node->aid);
            if (fn_806B14D0(node->aid)) {
                count++;
            }
        }
    }
    time += OS_MSEC_TO_TICKS((s64)count) * 3000;
    return time + OS_MSEC_TO_TICKS((s64)((stpMatchCnt->nodeInfoList.nodeNum - 1 - myIndex) * 6000));
}

// @806CD150
BOOL fn_806CD150(void) {
    DWC_Printf(DWC_REPORTFLAG_INFO, "mesh making stat(%d)\n", stpMatchCnt->unk6C0);
    if (!stpMatchCnt->unk6C0) {
        return FALSE;
    }
    stpMatchCnt->unk6C0 = 0;
    stpMatchCnt->unk6C4 = stpMatchCnt->state;
    DWC_Printf(DWC_REPORTFLAG_INFO, "temp new node info validity(%d)\n",
               stpMatchCnt->tempNewNodeInfo.profileID != 0);
    if (stpMatchCnt->tempNewNodeInfo.profileID) {
        DWC_Printf(DWC_REPORTFLAG_INFO, "stop mesh create.(aid:%2d pid:%u)\n",
                   stpMatchCnt->tempNewNodeInfo.aid, stpMatchCnt->tempNewNodeInfo.profileID);
        stpMatchCnt->unk6C8 = stpMatchCnt->tempNewNodeInfo;
    } else {
        memset(&stpMatchCnt->unk6C8, 0, sizeof(DWCNodeInfo));
    }
    if (stpMatchCnt->nnData.cookie) {
        NNCancel(stpMatchCnt->nnData.cookie);
        stpMatchCnt->nnData.cookie = 0;
        stpMatchCnt->unk720 = 0;
        stpMatchCnt->unk724 = 0;
        stpMatchCnt->unk728 = 0;
    }
    if (stpMatchCnt->state == DWC_MATCH_STATE_CL_WAIT_RESV ||
        stpMatchCnt->state == DWC_MATCH_STATE_CL_NN || stpMatchCnt->state == DWC_MATCH_STATE_CL_GT2 ||
        stpMatchCnt->state == DWC_MATCH_STATE_SV_OWN_NN ||
        stpMatchCnt->state == DWC_MATCH_STATE_SV_OWN_GT2) {
        if (stpMatchCnt->unk7AC) {
            if (DWCi_HandleMatchCommandError(DWCi_SendResvCancelCommand())) {
                memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
                return TRUE;
            }
        }
        stpMatchCnt->unk18 = 0;
        stpMatchCnt->unk700 = 0;
    }
    memset(&stpMatchCnt->tempNewNodeInfo, 0, sizeof(DWCNodeInfo));
    return TRUE;
}

// @806CD3C0
BOOL fn_806CD3C0(void) {
    return stpMatchCnt->unk8E0 == 2;
}

// @806CD3E0
u8 fn_806CD3E0(const GPBuddyStatus* status) {
    u8 locked = 0;
    char valueStr[4] = {0};
    if (fn_806ABA10("LCK", valueStr, status->statusString, '/') > 0) {
        locked = (u8)strtoul(valueStr, NULL, 10);
    }
    return locked;
}

// @806CD450
void fn_806CD450(int profileID) {
    stpMatchCnt->profileID = profileID;
}

// @806CD460
void fn_806CD460(void) {
    qr2_register_keyA(50, "dwc_pid");
    qr2_register_keyA(51, "dwc_mtype");
    qr2_register_keyA(52, "dwc_mver");
    qr2_register_keyA(53, "dwc_eval");
    qr2_register_keyA(54, "dwc_groupid");
    qr2_register_keyA(55, "dwc_hoststate");
    qr2_register_keyA(56, "dwc_suspend");
}

DWCMatchControl* stpMatchCnt = NULL;

// @806CD4E0
void fn_806CD4E0(void) {
    NNCancel(stpMatchCnt->nnData.cookie);
    if (!stNNBusy) {
        NNFreeNegotiateList();
    } else {
        stNNFreeReq = TRUE;
    }
}

char* stStateNames[DWC_MATCH_STATE_NUM] = {
    "DWC_MATCH_STATE_INIT",
    "DWC_MATCH_STATE_CL_WAITING",
    "DWC_MATCH_STATE_CL_SEARCH_HOST",
    "DWC_MATCH_STATE_CL_WAIT_RESV",
    "DWC_MATCH_STATE_CL_SEARCH_EVAL_HOST",
    "DWC_MATCH_STATE_CL_NN",
    "DWC_MATCH_STATE_CL_GT2",
    "DWC_MATCH_STATE_CL_CANCEL_SYN",
    "DWC_MATCH_STATE_CL_SYN",
    "DWC_MATCH_STATE_CL_SVDOWN_1",
    "DWC_MATCH_STATE_CL_SVDOWN_2",
    "DWC_MATCH_STATE_CL_SVDOWN_3",
    "DWC_MATCH_STATE_CL_SEARCH_GROUPID_HOST",
    "DWC_MATCH_STATE_SV_WAITING",
    "DWC_MATCH_STATE_SV_OWN_NN",
    "DWC_MATCH_STATE_SV_OWN_GT2",
    "DWC_MATCH_STATE_SV_WAIT_CL_LINK",
    "DWC_MATCH_STATE_SV_CANCEL_SYN",
    "DWC_MATCH_STATE_SV_CANCEL_SYN_WAIT",
    "DWC_MATCH_STATE_SV_SYN",
    "DWC_MATCH_STATE_SV_SYN_WAIT",
    "DWC_MATCH_STATE_WAIT_CLOSE",
    "DWC_MATCH_STATE_SEARCH_OWN",
    "DWC_MATCH_STATE_SV_SYN_CLOSE_WAIT",
};
