#ifndef DWC_FRIEND_H
#define DWC_FRIEND_H
#include <types.h>
#include <dwc/dwc_error.h>
#ifdef __cplusplus
extern "C" {
#endif

// inferred: GameSpy Presence (GP) types used by the DWC friend code (gp.h)
typedef struct GPIConnection GPIConnection;
typedef GPIConnection* GPConnection;
typedef int GPResult;
typedef int GPProfile;
typedef int GPEnum;

// inferred: GP callback arguments (gp.h)
typedef struct GPRecvBuddyRequestArg {
    GPProfile profile; // 0x00
} GPRecvBuddyRequestArg;

typedef struct GPRecvBuddyStatusArg {
    GPProfile profile; // 0x00
    u32 date;          // 0x04
    int index;         // 0x08
} GPRecvBuddyStatusArg;

// inferred: one entry of the DWC friend roster (12 bytes, cleared with memset)
typedef struct DWCFriendData {
    u8 data[12]; // 0x00
} DWCFriendData;

// inferred: opaque here; owned by dwc_account
typedef struct DWCUserData DWCUserData;

// inferred: callback types from the NitroDWC/RVL DWC API (argument order from the call sites)
typedef void (*DWCUpdateServersCallback)(DWCError error, BOOL isChanged, void* param);
typedef void (*DWCFriendStatusCallback)(int index, u8 status, const char* statusString,
                                        void* param);
typedef void (*DWCDeleteFriendListCallback)(int deletedIndex, int srcIndex, void* param);
typedef void (*DWCBuddyFriendCallback)(int index, void* param);
typedef void (*DWCStorageLoginCallback)(DWCError error, void* param);
typedef void (*DWCSaveToServerCallback)(BOOL success, BOOL isPublic, void* param);

// inferred: DWC friend manager state (DWCstFriendControl); field names from the NitroDWC API
typedef struct DWCFriendControl {
    int state;                                  // 0x00 0: idle, 1: updating servers, 2: done
    GPConnection* pGpObj;                       // 0x04
    u32 gpProcessCount;                         // 0x08
    s64 lastGpProcess;                          // 0x10
    int friendListLen;                          // 0x18
    DWCFriendData* friendList;                  // 0x1C
    u8 buddyUpdateIdx;                          // 0x20
    u8 friendListChanged;                       // 0x21
    u8 buddyUpdateState;                        // 0x22
    u8 svUpdateComplete;                        // 0x23
    u32 persCallbackLevel;                      // 0x24
    int profileID;                              // 0x28
    const DWCUserData* userdata;                // 0x2C
    DWCUpdateServersCallback updateCallback;    // 0x30
    void* updateParam;                          // 0x34
    DWCFriendStatusCallback statusCallback;     // 0x38
    void* statusParam;                          // 0x3C
    DWCDeleteFriendListCallback deleteCallback; // 0x40
    void* deleteParam;                          // 0x44
    DWCBuddyFriendCallback buddyCallback;       // 0x48
    void* buddyParam;                           // 0x4C
    DWCStorageLoginCallback persLoginCallback;  // 0x50
    void* persLoginParam;                       // 0x54
    DWCSaveToServerCallback saveCallback;       // 0x58
    void* saveParam;                            // 0x5C
    int* reverseBuddyList;                      // 0x60 inferred: profile IDs sent to gpGetReverseBuddiesList
} DWCFriendControl;

u8 DWC_GetFriendStatusSC(const DWCFriendData* friendData, u8* maxEntry, u8* numEntry,
                         char* statusString);
BOOL DWC_SetOwnStatusString(const char* statusString);
BOOL DWC_GetOwnStatusString(char* statusString);
BOOL DWC_CanChangeFriendList(void);
void DWC_DeleteBuddyFriendData(DWCFriendData* friendData);
BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void* param);
// inferred: saves GameSpy persistent data of type pd_public_rw; real name unknown
BOOL fn_806AC0A0(char* keyvalues, void* param);

void DWCi_FriendInit(DWCFriendControl* friendcnt, GPConnection* pGpObj, const DWCUserData* userdata,
                     DWCFriendData* friendList, int friendListLen);
void DWCi_FriendProcess(void);
BOOL DWCi_IsFriendUpdating(void);
void DWCi_UpdateServersAsync(const char* playerName, int unused,
                             DWCUpdateServersCallback updateCallback, void* updateParam,
                             DWCFriendStatusCallback statusCallback, void* statusParam,
                             DWCDeleteFriendListCallback deleteCallback, void* deleteParam);
void DWCi_StopFriendProcess(DWCError error, int errorCode);
// inferred: GP callbacks registered by dwc_main (gpSetCallback)
void DWCi_GPRecvBuddyRequestCallback(GPConnection* connection, GPRecvBuddyRequestArg* arg,
                                     void* param);
BOOL DWCi_GPRecvBuddyAuthCallback(GPConnection* connection, GPRecvBuddyRequestArg* arg,
                                  void* param);
void DWCi_GPRecvBuddyStatusCallback(GPConnection* connection, GPRecvBuddyStatusArg* arg,
                                    void* param);
int DWCi_GetProfileIDFromList(int index);
int DWCi_GetFriendListIndex(int profileID);
void DWCi_ResetGPProcessTime(void);
GPResult DWCi_SetGPStatus(int status, const char* statusString, const char* locationString);
void DWCi_ShutdownFriend(void);
// inferred: set/cleared by dwc_main around CloseStatsConnection; names unknown
void fn_806AE4A0(void);
void fn_806AE4B0(void);

#ifdef __cplusplus
}
#endif
#endif
