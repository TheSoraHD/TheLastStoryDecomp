// Rewritten from the target assembly (DWC release build Aug 27 2010).
// DWC friend roster management on top of GameSpy Presence (GP) and the GameSpy persistent
// storage ("stats") server. Function and field names follow the NitroDWC API where known.
#include <dwc/dwc_friend.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// inferred: GameSpy GP constants
#define GP_NO_ERROR 0
#define GP_MEMORY_ERROR 1
#define GP_PARAMETER_ERROR 2
#define GP_NETWORK_ERROR 3
#define GP_SERVER_ERROR 4
#define GP_NON_BLOCKING 0
#define GP_DONT_CHECK_CACHE 0
#define GP_MORE 0x600
#define GP_DONE 0x601

// inferred: GameSpy persistent storage types (gstats/gpersist.h)
#define pd_private_ro 0
#define pd_private_rw 1
#define pd_public_ro 2
#define pd_public_rw 3

// inferred: DWC values used here
#define DWC_STATUS_OFFLINE 0
#define DWC_STATUS_ONLINE 1
#define DWC_STATUS_PLAYING 2
#define DWC_STATUS_MATCH_SC_SV 6
#define DWC_GP_STATUS_NO_CHANGE (-1)
#define DWC_ERROR_NETWORK ((DWCError)6)

#define DWC_ECODE_SEQ_FRIEND (-70000)
#define DWC_ECODE_GS_GP (-1000)
#define DWC_ECODE_TYPE_ALLOC (-1)
#define DWC_ECODE_TYPE_PARAM (-2)
#define DWC_ECODE_TYPE_UNEXPECTED (-9)
#define DWC_ECODE_TYPE_NETWORK (-10)
#define DWC_ECODE_TYPE_SERVER (-20)

// inferred: DWC friend data types (DWC_GetFriendDataType)
#define DWC_FRIENDDATA_NODATA 0
#define DWC_FRIENDDATA_LOGIN_ID 1
#define DWC_FRIENDDATA_FRIEND_KEY 2
#define DWC_FRIENDDATA_GS_PROFILE_ID 3

#define DWC_FRIEND_GP_PROCESS_INTERVAL 300 // inferred: milliseconds between gpProcess calls

// inferred: GameSpy internal connection object (gpi.h); only the cached own status is read
struct GPIConnection {
    u8 pad0[0x23C];
    GPEnum lastStatus;             // 0x23C
    u8 pad240[0x3B8 - 0x240];
    char lastStatusString[256];    // 0x3B8
    char lastLocationString[256];  // 0x4B8
};

// inferred: GameSpy GP structures (gp.h)
typedef struct GPBuddyStatus {
    GPProfile profile;         // 0x000
    GPEnum status;             // 0x004
    char statusString[256];    // 0x008
    char locationString[256];  // 0x108
    u32 ip;                    // 0x208
    int port;                  // 0x20C
    GPEnum quietModeFlags;     // 0x210
} GPBuddyStatus;

typedef struct GPProfileSearchMatch {
    GPProfile profile; // 0x00
    u8 pad[0xB0 - 4];
} GPProfileSearchMatch;

typedef struct GPProfileSearchResponseArg {
    GPResult result;                // 0x00
    int numMatches;                 // 0x04
    GPEnum more;                    // 0x08
    GPProfileSearchMatch* matches;  // 0x0C
} GPProfileSearchResponseArg;

typedef struct GPGetInfoResponseArg {
    GPResult result;      // 0x00
    GPProfile profile;    // 0x04
    char nick[31];        // 0x08
    char uniquenick[21];  // 0x27
    char email[51];       // 0x3C
    char firstname[31];   // 0x6F
    char lastname[31];    // 0x8E
} GPGetInfoResponseArg;

typedef struct GPReverseBuddy {
    GPProfile profile; // 0x00
    u8 pad[0x1C - 4];
} GPReverseBuddy;

typedef struct GPGetReverseBuddiesListResponseArg {
    GPResult result;          // 0x00
    int numOfProfiles;        // 0x04
    GPReverseBuddy* profiles; // 0x08
} GPGetReverseBuddiesListResponseArg;

typedef void (*GPCallback)(GPConnection* connection, void* arg, void* param);
typedef void (*PersDataSaveCallbackFn)(int localid, int profileid, int type, int index,
                                       int success, u32 modified, void* instance);

// inferred: GameSpy GP / persistent entry points; names from the GameSpy SDK, no source yet
GPResult fn_806DA8D0(GPConnection* connection);                                  // gpProcess
GPResult fn_806DAB30(GPConnection* connection, const char* nick, const char* uniquenick,
                     const char* email, const char* firstname, const char* lastname,
                     int icquin, int blocking, GPCallback callback, void* param); // gpProfileSearch
GPResult fn_806DAC00(GPConnection* connection, GPProfile profile, int checkCache,
                     int blocking, GPCallback callback, void* param);            // gpGetInfo
GPResult fn_806DAD50(GPConnection* connection, GPProfile profile, const char* reason); // gpSendBuddyRequest
GPResult fn_806DAEE0(GPConnection* connection, GPProfile profile);              // gpAuthBuddyRequest
GPResult fn_806DAF50(GPConnection* connection, GPProfile profile);              // gpDenyBuddyRequest
GPResult fn_806DB050(GPConnection* connection, int* numBuddies);                // gpGetNumBuddies
GPResult fn_806DB0A0(GPConnection* connection, int index, GPBuddyStatus* status); // gpGetBuddyStatus
GPResult fn_806DB220(GPConnection* connection, GPProfile profile, int* index);   // gpGetBuddyIndex
int fn_806DB310(GPConnection* connection, GPProfile profile);                   // gpIsBuddy
GPResult fn_806DB3D0(GPConnection* connection, GPProfile profile);              // gpDeleteBuddy
GPResult fn_806DB460(GPConnection* connection, GPEnum status, const char* statusString,
                     const char* locationString);                               // gpSetStatus
GPResult fn_806DB730(GPConnection* connection, int* targetProfiles, int numOfTargets,
                     int blocking, GPCallback callback, void* param); // gpGetReverseBuddiesList
void fn_806F87A0(void);                                                         // CloseStatsConnection
int fn_806F8A60(void);                                                          // IsStatsConnected
void fn_806F8A80(int localid, int profileid, int type, int index, char* keyvalues,
                 PersDataSaveCallbackFn callback, void* instance);              // SetPersistDataValues
int fn_806F8B00(void);                                                          // PersistThink

// inferred: dwc_login entry points
const DWCUserData* fn_806AEAF0(void); // DWCi_GetUserData
BOOL fn_806AF670(void);               // DWCi_CheckLogin

// inferred: dwc_account friend data accessors (by their use here)
BOOL fn_806CF0E0(const DWCFriendData* friendData);
BOOL fn_806CF130(const DWCFriendData* friendData);
int fn_806CF190(const DWCFriendData* friendData);
int fn_806CF1A0(const DWCFriendData* friendData);
int fn_806CF1B0(const DWCFriendData* friendData); // DWC_GetFriendDataType
void fn_806CF1C0(DWCFriendData* friendData);
void fn_806CF200(DWCFriendData* friendData, int value);
void fn_806CF250(DWCFriendData* friendData, int value);
void fn_806CF2A0(DWCFriendData* friendData, int value);
int fn_806D0060(const DWCUserData* userdata, const DWCFriendData* friendData);
void fn_806D02E0(DWCFriendData* friendData, int profileID);
void fn_806D0350(const DWCUserData* userdata, const DWCFriendData* friendData, char* loginIdStr);

// inferred: other DWC helpers
int fn_806ABA10(const char* key, char* value, const char* string, char separator); // DWC_GetCommonValueString
BOOL fn_806CD3E0(const GPBuddyStatus* status); // dwc_match

static DWCFriendControl* stpFriendCnt;
static int stPersState;     // inferred: 2 once logged in to the storage server
static int stPersThinking;  // inferred
static int stPersCloseReq;  // inferred
static int stPersClosing;   // inferred

static void DWCi_UpdateFriendReq(DWCFriendData friendList[], int friendListLen);
static void DWCi_SendBuddyRequest(int profileID);
static BOOL DWCi_CheckSameFriend(DWCFriendData friendList[], int index, int profileID);
static int DWCi_RefreshFriendListAll(DWCFriendData friendList[], int friendListLen,
                                     int profileID);
static BOOL DWCi_GetFriendBuddyStatus(const DWCFriendData* friendData, GPBuddyStatus* status);
static GPResult DWCi_HandleGPError(GPResult result);
static void DWCi_GPProfileSearchCallback(GPConnection* connection,
                                         GPProfileSearchResponseArg* arg, int index);
static void DWCi_GPGetInfoCallback_RecvBuddyRequest(GPConnection* connection,
                                                    GPGetInfoResponseArg* arg, void* param);
static void DWCi_GPGetInfoCallback_RecvAuthMessage(GPConnection* connection,
                                                   GPGetInfoResponseArg* arg, void* param);
static void DWCi_PersSaveCallback(int localid, int profileid, int type, int index, int success,
                                  u32 modified, void* instance);
static void DWCi_GPGetReverseBuddiesListCallback(GPConnection* connection,
                                                 GPGetReverseBuddiesListResponseArg* arg,
                                                 void* param);

u8 DWC_GetFriendStatusSC(const DWCFriendData* friendData, u8* maxEntry, u8* numEntry,
                         char* statusString) {
    char valueStr[4];
    GPBuddyStatus status;
    u8 ret;

    if (DWCi_GetFriendBuddyStatus(friendData, &status)) {
        if (status.status == DWC_STATUS_MATCH_SC_SV) {
            if (maxEntry != NULL) {
                if (fn_806ABA10("SCM", valueStr, status.statusString, '/') > 0) {
                    *maxEntry = (u8)strtoul(valueStr, NULL, 10);
                } else {
                    *maxEntry = 0;
                }
            }
            if (numEntry != NULL) {
                if (fn_806ABA10("SCN", valueStr, status.statusString, '/') > 0) {
                    *numEntry = (u8)strtoul(valueStr, NULL, 10);
                } else {
                    *numEntry = 0;
                }
            }
        } else {
            if (maxEntry != NULL) {
                *maxEntry = 0;
            }
            if (numEntry != NULL) {
                *numEntry = 0;
            }
        }

        if (statusString != NULL) {
            strcpy(statusString, status.locationString);
        }

        ret = (u8)status.status;
        if (fn_806CD3E0(&status)) {
            ret = DWC_STATUS_PLAYING;
        }
        return ret;
    }

    if (maxEntry != NULL) {
        *maxEntry = 0;
    }
    if (numEntry != NULL) {
        *numEntry = 0;
    }
    return DWC_STATUS_OFFLINE;
}

BOOL DWC_SetOwnStatusString(const char* statusString) {
    if (stpFriendCnt == NULL || !fn_806AF670()) {
        return FALSE;
    }
    return DWCi_SetGPStatus(DWC_GP_STATUS_NO_CHANGE, NULL, statusString) == GP_NO_ERROR;
}

BOOL DWC_GetOwnStatusString(char* statusString) {
    GPIConnection* connection;

    if (stpFriendCnt == NULL || (connection = *stpFriendCnt->pGpObj) == NULL ||
        statusString == NULL) {
        return FALSE;
    }
    strcpy(statusString, connection->lastLocationString);
    return TRUE;
}

BOOL DWC_CanChangeFriendList(void) {
    if (stpFriendCnt != NULL &&
        (stpFriendCnt->buddyUpdateState == 1 || stpFriendCnt->buddyUpdateState == 2)) {
        return FALSE;
    }
    return TRUE;
}

void DWC_DeleteBuddyFriendData(DWCFriendData* friendData) {
    int profileID;

    if (stpFriendCnt != NULL && fn_806AF670() && fn_806AEAF0() != NULL &&
        (profileID = fn_806D0060(fn_806AEAF0(), friendData)) != 0 && profileID != -1 &&
        fn_806DB310(stpFriendCnt->pGpObj, profileID)) {
        fn_806DB3D0(stpFriendCnt->pGpObj, profileID);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_DeleteBuddyFriendData : Deleted buddy.\n");
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_DeleteBuddyFriendData : Only clear data.\n");
    }

    memset(friendData, 0, sizeof(DWCFriendData));
}

BOOL DWC_SetBuddyFriendCallback(DWCBuddyFriendCallback callback, void* param) {
    if (stpFriendCnt == NULL) {
        return FALSE;
    }
    stpFriendCnt->buddyCallback = callback;
    stpFriendCnt->buddyParam = param;
    return TRUE;
}

static void DWCi_SetPersData(int profileID, int type, char* keyvalues, void* param) {
    if (stpFriendCnt->persCallbackLevel == 0xFFFFFFFF) {
        DWCi_StopFriendProcess(DWC_ERROR_FATAL, DWC_ECODE_SEQ_FRIEND + DWC_ECODE_TYPE_UNEXPECTED);
    } else {
        stpFriendCnt->persCallbackLevel++;
    }

    fn_806F8A80(0, profileID, type, 0, keyvalues, DWCi_PersSaveCallback, param);
}

BOOL fn_806AC0A0(char* keyvalues, void* param) {
    if (stPersState != 2 || DWCi_IsError() || stpFriendCnt == NULL) {
        return FALSE;
    }

    DWCi_SetPersData(stpFriendCnt->profileID, pd_public_rw, keyvalues, param);
    return TRUE;
}

void DWCi_FriendInit(DWCFriendControl* friendcnt, GPConnection* pGpObj, const DWCUserData* userdata,
                     DWCFriendData* friendList, int friendListLen) {
    int i;

    stpFriendCnt = friendcnt;
    stpFriendCnt->state = 0;
    stpFriendCnt->pGpObj = pGpObj;
    stpFriendCnt->gpProcessCount = 0;
    stpFriendCnt->lastGpProcess = 0;
    stpFriendCnt->friendListLen = friendListLen;
    stpFriendCnt->friendList = friendList;
    stpFriendCnt->buddyUpdateIdx = 0;
    stpFriendCnt->friendListChanged = 0;
    stpFriendCnt->buddyUpdateState = 0;
    stpFriendCnt->svUpdateComplete = 0;
    stpFriendCnt->persCallbackLevel = 0;
    stpFriendCnt->profileID = 0;
    stpFriendCnt->userdata = userdata;
    stpFriendCnt->updateCallback = NULL;
    stpFriendCnt->updateParam = NULL;
    stpFriendCnt->statusCallback = NULL;
    stpFriendCnt->statusParam = NULL;
    stpFriendCnt->deleteCallback = NULL;
    stpFriendCnt->deleteParam = NULL;
    stpFriendCnt->buddyCallback = NULL;
    stpFriendCnt->buddyParam = NULL;
    stpFriendCnt->persLoginCallback = NULL;
    stpFriendCnt->persLoginParam = NULL;
    stpFriendCnt->saveCallback = NULL;
    stpFriendCnt->saveParam = NULL;
    stpFriendCnt->reverseBuddyList = NULL;

    for (i = 0; i < friendListLen; i++) {
        fn_806CF2A0(&stpFriendCnt->friendList[i], 0);
    }
}

// inferred: calls gpProcess at most every DWC_FRIEND_GP_PROCESS_INTERVAL ms
static GPResult DWCi_ProcessGP(void) {
    GPResult result = GP_NO_ERROR;
    s64 now = OSGetTime();

    if (OS_TICKS_TO_MSEC(now - stpFriendCnt->lastGpProcess) >= DWC_FRIEND_GP_PROCESS_INTERVAL) {
        stpFriendCnt->gpProcessCount++;
        result = fn_806DA8D0(stpFriendCnt->pGpObj);
        now = OSGetTime();
        stpFriendCnt->lastGpProcess = now;
    }
    return result;
}

void DWCi_FriendProcess(void) {
    int i;
    int num;
    int j;
    GPResult result;

    if (stpFriendCnt == NULL || DWCi_IsError()) {
        return;
    }

    if (stpFriendCnt->friendList == NULL) {
        if (stpFriendCnt->pGpObj != NULL && *stpFriendCnt->pGpObj != NULL) {
            DWCi_ProcessGP();
        }
        return;
    }

    if (stpFriendCnt->persCallbackLevel != 0 || fn_806F8A60()) {
        stPersThinking = TRUE;
        stPersCloseReq = FALSE;
        if (!fn_806F8B00()) {
            stPersThinking = FALSE;
            DWC_Printf(DWC_REPORTFLAG_WARNING, "Connection to the stats server was lost\n");
        }
        stPersThinking = FALSE;
        if (stPersCloseReq == TRUE) {
            stPersCloseReq = FALSE;
            fn_806F87A0();
        }
    }

    if (stpFriendCnt->pGpObj != NULL && *stpFriendCnt->pGpObj != NULL) {
        result = DWCi_ProcessGP();
        if (result != GP_NO_ERROR) {
            return;
        }
        if (stpFriendCnt->state == 0) {
            return;
        }

        if (stpFriendCnt->friendList && stpFriendCnt->buddyUpdateState != 3 &&
            stpFriendCnt->gpProcessCount > 7) {
            if (stpFriendCnt->buddyUpdateState == 0 || stpFriendCnt->buddyUpdateState == 1) {
                DWCi_UpdateFriendReq(stpFriendCnt->friendList, stpFriendCnt->friendListLen);
            }
            if (stpFriendCnt->buddyUpdateIdx >= stpFriendCnt->friendListLen) {
                stpFriendCnt->buddyUpdateState = 3;
                stpFriendCnt->svUpdateComplete++;
            }
        }
    }

    if (stpFriendCnt->svUpdateComplete < 2) {
        return;
    }

    if (stpFriendCnt->pGpObj != NULL && *stpFriendCnt->pGpObj != NULL) {
        num = 0;
        for (i = 0; i < stpFriendCnt->friendListLen; i++) {
            if (fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_GS_PROFILE_ID) {
                num++;
            }
        }

        if (num == 0) {
            stpFriendCnt->svUpdateComplete = 0;
            stpFriendCnt->updateCallback(DWC_ERROR_NONE, stpFriendCnt->friendListChanged,
                                         stpFriendCnt->updateParam);
            stpFriendCnt->state = 2;
            return;
        }

        stpFriendCnt->reverseBuddyList = (int*)DWC_Alloc(DWC_ALLOCTYPE_BASE, num * sizeof(int));
        if (stpFriendCnt->reverseBuddyList == NULL) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "DWC_Alloc Error\n");
            DWCi_StopFriendProcess(DWC_ERROR_FATAL, DWC_ECODE_SEQ_FRIEND + DWC_ECODE_TYPE_ALLOC);
            return;
        }

        j = 0;
        for (i = 0; i < stpFriendCnt->friendListLen; i++) {
            if (fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_GS_PROFILE_ID) {
                stpFriendCnt->reverseBuddyList[j] =
                    fn_806D0060(fn_806AEAF0(), &stpFriendCnt->friendList[i]);
                j++;
            }
        }

        if (DWCi_HandleGPError(fn_806DB730(stpFriendCnt->pGpObj, stpFriendCnt->reverseBuddyList,
                                           num, GP_NON_BLOCKING,
                                           (GPCallback)DWCi_GPGetReverseBuddiesListCallback,
                                           NULL)) != GP_NO_ERROR) {
            return;
        }
    }

    stpFriendCnt->svUpdateComplete = 0;
}

BOOL DWCi_IsFriendUpdating(void) {
    BOOL ret = FALSE;

    if (stpFriendCnt != NULL && stpFriendCnt->state == 1) {
        ret = TRUE;
    }
    return ret;
}

void DWCi_UpdateServersAsync(const char* playerName, int unused,
                             DWCUpdateServersCallback updateCallback, void* updateParam,
                             DWCFriendStatusCallback statusCallback, void* statusParam,
                             DWCDeleteFriendListCallback deleteCallback, void* deleteParam) {
    stpFriendCnt->updateCallback = updateCallback;
    stpFriendCnt->updateParam = updateParam;
    stpFriendCnt->statusCallback = statusCallback;
    stpFriendCnt->statusParam = statusParam;
    stpFriendCnt->deleteCallback = deleteCallback;
    stpFriendCnt->deleteParam = deleteParam;
    stpFriendCnt->friendListChanged = 0;
    stpFriendCnt->buddyUpdateState = 0;
    stpFriendCnt->svUpdateComplete = 0;
    stpFriendCnt->buddyUpdateIdx = 0;
    stpFriendCnt->state = 1;

    if (stpFriendCnt->friendList == NULL) {
        stpFriendCnt->svUpdateComplete++;
    }
    stpFriendCnt->svUpdateComplete++;
}

static void DWCi_CloseFriendProcess(void) {
    if (stpFriendCnt == NULL) {
        return;
    }
    stpFriendCnt->state = 0;
    stpFriendCnt->buddyUpdateState = 0;
    stpFriendCnt->svUpdateComplete = 0;
}

void DWCi_StopFriendProcess(DWCError error, int errorCode) {
    if (stpFriendCnt == NULL || error == DWC_ERROR_NONE) {
        return;
    }

    DWCi_SetError(error, errorCode);

    if (DWCi_IsFriendUpdating()) {
        if (stpFriendCnt->reverseBuddyList != NULL) {
            DWC_Free(DWC_ALLOCTYPE_BASE, stpFriendCnt->reverseBuddyList, 0);
            stpFriendCnt->reverseBuddyList = NULL;
        }
        stpFriendCnt->updateCallback(error, stpFriendCnt->friendListChanged,
                                     stpFriendCnt->updateParam);
    }

    DWCi_CloseFriendProcess();
}

void DWCi_GPRecvBuddyRequestCallback(GPConnection* connection, GPRecvBuddyRequestArg* arg,
                                     void* param) {
    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Received buddy request from %u\n", arg->profile);

    if (stpFriendCnt->friendList != NULL) {
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Begin to search gpInfo.\n");
        fn_806DAC00(connection, arg->profile, GP_DONT_CHECK_CACHE, GP_NON_BLOCKING,
                    (GPCallback)DWCi_GPGetInfoCallback_RecvBuddyRequest, NULL);
    }
}

BOOL DWCi_GPRecvBuddyAuthCallback(GPConnection* connection, GPRecvBuddyRequestArg* arg,
                                  void* param) {
    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Received buddy authenticated message from %u.\n",
               arg->profile);
    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Begin to search gpInfo.\n");
    fn_806DAC00(connection, arg->profile, GP_DONT_CHECK_CACHE, GP_NON_BLOCKING,
                (GPCallback)DWCi_GPGetInfoCallback_RecvAuthMessage, NULL);
    return TRUE;
}

void DWCi_GPRecvBuddyStatusCallback(GPConnection* connection, GPRecvBuddyStatusArg* arg,
                                    void* param) {
    GPBuddyStatus status;
    int index;

    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "RECV update friend status. p:%d\n", arg->profile);

    index = DWCi_GetFriendListIndex(arg->profile);
    if (index == -1) {
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, " But from already deleted friend.\n");
        if (fn_806DB310(connection, arg->profile)) {
            fn_806DB3D0(connection, arg->profile);
            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, " delete friend again.\n");
        }
        return;
    }

    if (stpFriendCnt->statusCallback != NULL) {
        fn_806DB0A0(connection, arg->index, &status);
        stpFriendCnt->statusCallback(index, (u8)status.status, status.locationString,
                                     stpFriendCnt->statusParam);
    }
}

int DWCi_GetProfileIDFromList(int index) {
    int profileID;

    if (stpFriendCnt->friendList == NULL) {
        return 0;
    }
    profileID = fn_806D0060(fn_806AEAF0(), &stpFriendCnt->friendList[index]);
    if (!(profileID != 0 && profileID != -1)) {
        return 0;
    }
    return profileID;
}

int DWCi_GetFriendListIndex(int profileID) {
    int i;

    if (stpFriendCnt == NULL || profileID == 0) {
        return -1;
    }

    for (i = 0; i < stpFriendCnt->friendListLen; i++) {
        if (profileID == DWCi_GetProfileIDFromList(i)) {
            return i;
        }
    }
    return -1;
}

void DWCi_ResetGPProcessTime(void) {
    if (stpFriendCnt != NULL) {
        stpFriendCnt->gpProcessCount = 0;
        stpFriendCnt->lastGpProcess = OSGetTime();
    }
}

GPResult DWCi_SetGPStatus(int status, const char* statusString, const char* locationString) {
    if (stpFriendCnt == NULL || !fn_806AF670()) {
        return GP_NO_ERROR;
    }

    if (status == DWC_GP_STATUS_NO_CHANGE) {
        status = (*stpFriendCnt->pGpObj)->lastStatus;
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Change GP status->status %d\n", status);
    }

    if (statusString == NULL) {
        statusString = (*stpFriendCnt->pGpObj)->lastStatusString;
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Change GP status->statusString %s\n", statusString);
    }

    if (locationString == NULL) {
        locationString = (*stpFriendCnt->pGpObj)->lastLocationString;
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Change GP status->locationString %s\n", locationString);
    }

    return fn_806DB460(stpFriendCnt->pGpObj, status, statusString, locationString);
}

void DWCi_ShutdownFriend(void) {
    stpFriendCnt = NULL;
}

static void DWCi_UpdateFriendReq(DWCFriendData friendList[], int friendListLen) {
    int numBuddies;
    int buddyIdx;
    char loginIdStr[0x18];
    GPBuddyStatus status;
    int i;
    int profileID;

    if (stpFriendCnt->buddyUpdateState == 0) {
        DWCi_HandleGPError(fn_806DB050(stpFriendCnt->pGpObj, &numBuddies));
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "gpGetNumBuddies -> %d\n", numBuddies);

        for (buddyIdx = 0; buddyIdx < numBuddies; buddyIdx++) {
            DWCi_HandleGPError(fn_806DB0A0(stpFriendCnt->pGpObj, buddyIdx, &status));

            for (i = 0; i < friendListLen; i++) {
                if (status.profile == DWCi_GetProfileIDFromList(i)) {
                    if (!fn_806CF130(&friendList[i])) {
                        fn_806D02E0(&friendList[i], status.profile);
                        fn_806CF1C0(&friendList[i]);
                        if (fn_806CF0E0(&stpFriendCnt->friendList[i])) {
                            fn_806CF250(&stpFriendCnt->friendList[i], 1);
                        }
                        stpFriendCnt->friendListChanged = TRUE;
                    }
                    break;
                }
            }

            if (i == friendListLen) {
                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Deleted buddy %u\n", status.profile);
                DWCi_HandleGPError(fn_806DB3D0(stpFriendCnt->pGpObj, status.profile));
                numBuddies--;
                buddyIdx--;
            }
        }

        stpFriendCnt->buddyUpdateState = 1;
    }

    for (; stpFriendCnt->buddyUpdateIdx < friendListLen; stpFriendCnt->buddyUpdateIdx++) {
        if (fn_806CF1A0(&friendList[stpFriendCnt->buddyUpdateIdx])) {
            continue;
        }

        profileID = DWCi_GetProfileIDFromList(stpFriendCnt->buddyUpdateIdx);
        if (profileID != 0) {
            if (DWCi_CheckSameFriend(friendList, stpFriendCnt->buddyUpdateIdx, profileID)) {
                continue;
            }
            DWCi_HandleGPError(fn_806DB220(stpFriendCnt->pGpObj, profileID, &buddyIdx));
            if (buddyIdx == -1) {
                DWCi_SendBuddyRequest(profileID);
                fn_806CF2A0(&friendList[stpFriendCnt->buddyUpdateIdx], 1);
            }
        } else if (fn_806D0060(fn_806AEAF0(), &friendList[stpFriendCnt->buddyUpdateIdx]) == -1) {
            fn_806D0350(fn_806AEAF0(), &friendList[stpFriendCnt->buddyUpdateIdx], loginIdStr);
            fn_806DAB30(stpFriendCnt->pGpObj, NULL, NULL, NULL, NULL, loginIdStr, 0,
                        GP_NON_BLOCKING, (GPCallback)DWCi_GPProfileSearchCallback,
                        (void*)stpFriendCnt->buddyUpdateIdx);
            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Called gpProfileSearch().\n");
            stpFriendCnt->buddyUpdateState = 2;
            return;
        }
    }
}

// inferred: sends a GP buddy request to the given profile
static void DWCi_SendBuddyRequest(int profileID) {
    DWCi_HandleGPError(fn_806DAD50(stpFriendCnt->pGpObj, profileID, ""));
    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Send buddy request to %u\n", profileID);
}

static void DWCi_DeleteFriendFromList(DWCFriendData* friendData, int deletedIndex, int srcIndex) {
    if (stpFriendCnt == NULL) {
        return;
    }
    memset(friendData, 0, sizeof(DWCFriendData));
    if (stpFriendCnt->deleteCallback != NULL) {
        stpFriendCnt->deleteCallback(deletedIndex, srcIndex, stpFriendCnt->deleteParam);
    }
}

static BOOL DWCi_CheckSameFriend(DWCFriendData friendList[], int index, int profileID) {
    int i;
    int pid;

    if (fn_806CF1B0(&friendList[index]) == DWC_FRIENDDATA_LOGIN_ID) {
        return FALSE;
    }

    for (i = 0; i < index; i++) {
        pid = DWCi_GetProfileIDFromList(i);
        if (pid != 0 && pid == profileID) {
            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Found same friend in the list [%d] & [%d], %d\n",
                       i, index, fn_806CF1B0(&friendList[i]));
            if (fn_806CF130(&friendList[index]) && !fn_806CF130(&friendList[i])) {
                DWCi_DeleteFriendFromList(&friendList[i], i, index);
            } else {
                DWCi_DeleteFriendFromList(&friendList[index], index, i);
            }
            stpFriendCnt->friendListChanged = TRUE;
            return TRUE;
        }
    }
    return FALSE;
}

static int DWCi_RefreshFriendListAll(DWCFriendData friendList[], int friendListLen,
                                     int profileID) {
    int index = -1;
    int i;
    int j;
    int pid;

    for (i = 0; i < friendListLen; i++) {
        pid = DWCi_GetProfileIDFromList(i);
        if (pid == 0) {
            continue;
        }
        if (pid == profileID) {
            index = i;
        }

        for (j = i + 1; j < friendListLen; j++) {
            if (pid != DWCi_GetProfileIDFromList(j)) {
                continue;
            }

            if (fn_806CF1B0(&friendList[i]) == DWC_FRIENDDATA_FRIEND_KEY &&
                fn_806CF1B0(&friendList[j]) == DWC_FRIENDDATA_GS_PROFILE_ID) {
                fn_806D02E0(&friendList[i], pid);
            }
            if (fn_806CF130(&friendList[j])) {
                fn_806CF1C0(&friendList[i]);
                if (fn_806CF0E0(&stpFriendCnt->friendList[i])) {
                    fn_806CF250(&stpFriendCnt->friendList[i], 1);
                }
            }

            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Found same friend in the list [%d] & [%d], %d\n",
                       i, j, fn_806CF1B0(&friendList[i]));
            DWCi_DeleteFriendFromList(&friendList[j], j, i);
            stpFriendCnt->friendListChanged = TRUE;
        }
    }

    return index;
}

static BOOL DWCi_GetFriendBuddyStatus(const DWCFriendData* friendData, GPBuddyStatus* status) {
    int buddyIdx = 0;
    int profileID;
    GPResult gpResult;

    if (stpFriendCnt == NULL || !fn_806AF670()) {
        return FALSE;
    }

    profileID = fn_806D0060(fn_806AEAF0(), friendData);
    if (profileID > 0) {
        gpResult = fn_806DB220(stpFriendCnt->pGpObj, profileID, &buddyIdx);
        if (gpResult != GP_NO_ERROR) {
            return FALSE;
        }
    }
    if (profileID <= 0 || buddyIdx == -1) {
        return FALSE;
    }

    {
        u8 ret;
        gpResult = fn_806DB0A0(stpFriendCnt->pGpObj, buddyIdx, status);
        ret = (gpResult == GP_NO_ERROR);
        return ret;
    }
}

static GPResult DWCi_HandleGPError(GPResult result) {
    int errorCode;
    DWCError dwcError;

    if (result == GP_NO_ERROR) {
        return GP_NO_ERROR;
    }

    DWC_Printf(DWC_REPORTFLAG_ERROR, "Friend, GP error %d\n", result);

    switch (result) {
    case GP_MEMORY_ERROR:
        dwcError = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_ALLOC;
        break;
    case GP_PARAMETER_ERROR:
        dwcError = DWC_ERROR_FATAL;
        errorCode = DWC_ECODE_TYPE_PARAM;
        break;
    case GP_NETWORK_ERROR:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_NETWORK;
        break;
    case GP_SERVER_ERROR:
        dwcError = DWC_ERROR_NETWORK;
        errorCode = DWC_ECODE_TYPE_SERVER;
        break;
    }

    DWCi_StopFriendProcess(dwcError, errorCode + DWC_ECODE_SEQ_FRIEND + DWC_ECODE_GS_GP);
    return result;
}

// inferred: the persistent storage error handler of the DWC API. Nothing in the game uses it,
// so the linker strips it; its string still sits here in .data. The body is a reconstruction.
int DWCi_HandlePersError(int error) {
    if (error == 0) {
        return 0;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Friend, persistent error %d\n", error);
    DWCi_StopFriendProcess(DWC_ERROR_NETWORK, DWC_ECODE_SEQ_FRIEND + DWC_ECODE_TYPE_SERVER);
    return error;
}

static void DWCi_CallBuddyFriendCallback(int index) {
    if (stpFriendCnt->buddyCallback != NULL && stpFriendCnt->state != 1) {
        stpFriendCnt->buddyCallback(index, stpFriendCnt->buddyParam);
    }
}

static void DWCi_CallFriendStatusCallback(int index) {
    GPBuddyStatus status;
    u8 friendStatus;

    if (stpFriendCnt->statusCallback != NULL) {
        friendStatus = DWC_GetFriendStatusSC(&stpFriendCnt->friendList[index], NULL, NULL,
                                             status.locationString);
        stpFriendCnt->statusCallback(index, friendStatus, status.locationString,
                                     stpFriendCnt->statusParam);
    }
}

static void DWCi_GPProfileSearchCallback(GPConnection* connection,
                                         GPProfileSearchResponseArg* arg, int index) {
    int buddyIdx;
    int i;

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "ProfileSearch: num:%d m:%x\n", arg->numMatches, arg->more);

    if (arg->result == GP_NO_ERROR && arg->numMatches != 0 &&
        fn_806CF1B0(&stpFriendCnt->friendList[index]) != DWC_FRIENDDATA_NODATA) {
        if (arg->numMatches > 1) {
            DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Happned to find %d profiles.\n", arg->numMatches);
        }

        if (stpFriendCnt->state != 1) {
            return;
        }

        for (i = 0; i < arg->numMatches; i++) {
            if (DWCi_CheckSameFriend(stpFriendCnt->friendList, index, arg->matches[i].profile)) {
                stpFriendCnt->buddyUpdateIdx++;
                stpFriendCnt->buddyUpdateState = 1;
                arg->more = GP_DONE;
                return;
            }
        }

        for (i = 0; i < arg->numMatches; i++) {
            DWCi_HandleGPError(fn_806DB220(connection, arg->matches[i].profile, &buddyIdx));
            if (buddyIdx == -1) {
                DWCi_SendBuddyRequest(arg->matches[i].profile);
                fn_806CF2A0(&stpFriendCnt->friendList[index], 1);
            } else {
                fn_806D02E0(&stpFriendCnt->friendList[index], arg->matches[0].profile);
                fn_806CF1C0(&stpFriendCnt->friendList[index]);
                fn_806CF200(&stpFriendCnt->friendList[index], 1);
                if (fn_806CF0E0(&stpFriendCnt->friendList[index])) {
                    fn_806CF250(&stpFriendCnt->friendList[index], 1);
                }
                DWCi_CallBuddyFriendCallback(index);
                DWCi_CallFriendStatusCallback(index);
                stpFriendCnt->buddyUpdateIdx++;
                stpFriendCnt->buddyUpdateState = 1;
                arg->more = GP_DONE;
                stpFriendCnt->friendListChanged = TRUE;
                return;
            }
        }

        if (arg->more != GP_MORE) {
            stpFriendCnt->buddyUpdateIdx++;
            stpFriendCnt->buddyUpdateState = 1;
        } else {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, "more profiles will come...\n");
        }
    } else if (arg->result != GP_NO_ERROR) {
        if (DWCi_HandleGPError(arg->result) != GP_NO_ERROR) {
            return;
        }
    } else if (stpFriendCnt->state == 1 ||
               fn_806CF1B0(&stpFriendCnt->friendList[index]) == DWC_FRIENDDATA_NODATA) {
        stpFriendCnt->buddyUpdateIdx++;
        stpFriendCnt->buddyUpdateState = 1;
    }
}

static void DWCi_GPGetInfoCallback_RecvBuddyRequest(GPConnection* connection,
                                                    GPGetInfoResponseArg* arg, void* param) {
    int index;
    char gamecode[5];
    char loginIdStr[0x18];
    int i;
    BOOL found = FALSE;
    BOOL alreadyBuddy = FALSE;
    u32 code;

    if (arg->result != GP_NO_ERROR) {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, " ERROR: GetInfo Req. why??? : %d\n", arg->result);
        return;
    }

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "GetInfo Req: profileID %u, lastname '%s'.\n", arg->profile,
               arg->lastname);

    for (i = 0; i < stpFriendCnt->friendListLen; i++) {
        if (fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_LOGIN_ID) {
            fn_806D0350(fn_806AEAF0(), &stpFriendCnt->friendList[i], loginIdStr);
            if (strcmp(loginIdStr, arg->lastname) == 0) {
                if (fn_806CF0E0(&stpFriendCnt->friendList[i])) {
                    alreadyBuddy = TRUE;
                }
                fn_806DAEE0(connection, arg->profile);
                fn_806D02E0(&stpFriendCnt->friendList[i], arg->profile);
                found = TRUE;
                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Auth buddy request from %u, friend[%d]mp.\n",
                           arg->profile, i);
            }
        } else if (fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_GS_PROFILE_ID ||
                   fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_FRIEND_KEY) {
            code = *(u32*)((u8*)fn_806AEAF0() + 0x24); // inferred: gamecode in DWCUserData
            snprintf(gamecode, sizeof(gamecode), "%c%c%c%c", (u8)(code >> 24), (u8)(code >> 16),
                     (u8)(code >> 8), (u8)code);
            if (arg->profile == fn_806D0060(fn_806AEAF0(), &stpFriendCnt->friendList[i]) &&
                memcmp(gamecode, &arg->lastname[9], 4) == 0) {
                if (fn_806CF0E0(&stpFriendCnt->friendList[i])) {
                    alreadyBuddy = TRUE;
                }
                fn_806DAEE0(connection, arg->profile);
                found = TRUE;
                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Auth buddy request from %u, friend[%d]gs.\n",
                           arg->profile, i);
            }
        }
    }

    if (found) {
        DWCi_SendBuddyRequest(arg->profile);

        index = DWCi_RefreshFriendListAll(stpFriendCnt->friendList, stpFriendCnt->friendListLen,
                                          arg->profile);
        fn_806CF200(&stpFriendCnt->friendList[index], 1);
        if (fn_806CF0E0(&stpFriendCnt->friendList[index])) {
            fn_806CF250(&stpFriendCnt->friendList[index], 1);
            if (!alreadyBuddy) {
                DWCi_CallBuddyFriendCallback(index);
                DWCi_CallFriendStatusCallback(index);
                stpFriendCnt->friendListChanged = TRUE;
            }
        }
    } else {
        fn_806DAF50(connection, arg->profile);
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Denied buddy request from %u.\n", arg->profile);
    }
}

static void DWCi_GPGetInfoCallback_RecvAuthMessage(GPConnection* connection,
                                                   GPGetInfoResponseArg* arg, void* param) {
    char loginIdStr[0x18];
    int index;
    int i;
    BOOL found = FALSE;
    BOOL alreadyFriend = FALSE;

    if (arg->result != GP_NO_ERROR) {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, " ERROR: GetInfo Auth. why??? : %d\n", arg->result);
        return;
    }

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "GetInfo Auth: profileID %u, lastname '%s'.\n", arg->profile,
               arg->lastname);

    for (i = 0; i < stpFriendCnt->friendListLen; i++) {
        if (fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_LOGIN_ID) {
            fn_806D0350(fn_806AEAF0(), &stpFriendCnt->friendList[i], loginIdStr);
            if (strcmp(loginIdStr, arg->lastname) == 0) {
                fn_806D02E0(&stpFriendCnt->friendList[i], arg->profile);
                fn_806CF1C0(&stpFriendCnt->friendList[i]);
                found = TRUE;
                DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Established buddy with %u, friend[%d]mp.\n",
                           arg->profile, i);
            }
        } else if (fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_GS_PROFILE_ID ||
                   fn_806CF1B0(&stpFriendCnt->friendList[i]) == DWC_FRIENDDATA_FRIEND_KEY) {
            if (arg->profile == fn_806D0060(fn_806AEAF0(), &stpFriendCnt->friendList[i])) {
                if (fn_806CF130(&stpFriendCnt->friendList[i]) == TRUE) {
                    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "This profile is already my friend[%d].\n",
                               i);
                    alreadyFriend = TRUE;
                } else {
                    fn_806D02E0(&stpFriendCnt->friendList[i], arg->profile);
                    fn_806CF1C0(&stpFriendCnt->friendList[i]);
                    found = TRUE;
                    DWC_Printf(DWC_REPORTFLAG_UPDATE_SV,
                               "Established buddy with %u, friend[%d]gs.\n", arg->profile, i);
                }
            }
        }
    }

    if (found) {
        index = DWCi_RefreshFriendListAll(stpFriendCnt->friendList, stpFriendCnt->friendListLen,
                                          arg->profile);
        if (!alreadyFriend) {
            fn_806CF200(&stpFriendCnt->friendList[index], 1);
            if (fn_806CF0E0(&stpFriendCnt->friendList[index])) {
                fn_806CF250(&stpFriendCnt->friendList[index], 1);
            }
            DWCi_CallBuddyFriendCallback(index);
            DWCi_CallFriendStatusCallback(index);
        }
        stpFriendCnt->friendListChanged = TRUE;
    } else if (!alreadyFriend) {
        DWC_Printf(DWC_REPORTFLAG_UPDATE_SV, "Not Established buddy with %u.\n", arg->profile);
    }
}

// inferred: authentication callback of the storage server login (DWC_LoginToStorageServerAsync).
// The game never logs in to the storage server, so the linker strips both; the strings still
// sit here in .data. The bodies are reconstructions.
static void DWCi_PersAuthCallback(int localid, int profileid, int authenticated, char* errmsg,
                                  void* instance) {
    if (!authenticated) {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Stats server authentication failed.\n");
        DWC_Printf(DWC_REPORTFLAG_ERROR, "%s\n", errmsg);
        stPersState = 0;
        if (stpFriendCnt->persLoginCallback != NULL) {
            stpFriendCnt->persLoginCallback(DWC_ERROR_NETWORK, stpFriendCnt->persLoginParam);
        }
        return;
    }

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "Stats server authentication succeeded.\n");
    stPersState = 2;
    stpFriendCnt->profileID = profileid;
    if (stpFriendCnt->persLoginCallback != NULL) {
        stpFriendCnt->persLoginCallback(DWC_ERROR_NONE, stpFriendCnt->persLoginParam);
    }
}

// inferred: stand-in for the storage server login entry point, which hands the callback to
// GameSpy; only kept so the callback (and its strings) is compiled.
void* DWCi_GetPersAuthCallback(void) {
    return (void*)DWCi_PersAuthCallback;
}

static void DWCi_PersSaveCallback(int localid, int profileid, int type, int index, int success,
                                  u32 modified, void* instance) {
    BOOL isPublic;

    if (stpFriendCnt->persCallbackLevel == 0) {
        DWCi_StopFriendProcess(DWC_ERROR_FATAL, DWC_ECODE_SEQ_FRIEND + DWC_ECODE_TYPE_UNEXPECTED);
    } else {
        stpFriendCnt->persCallbackLevel--;
    }

    DWC_Printf(DWC_REPORTFLAG_DEBUG, "Saved data to persistent server.\n");

    isPublic = (u32)(type - pd_public_ro) <= 1;

    if (success) {
        if (stpFriendCnt->saveCallback != NULL) {
            stpFriendCnt->saveCallback(TRUE, isPublic, instance);
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Failed to save persistent data.\n");
        if (stpFriendCnt->saveCallback != NULL) {
            stpFriendCnt->saveCallback(FALSE, isPublic, instance);
        }
    }
}

void fn_806AE4A0(void) {
    stPersClosing = TRUE;
}

void fn_806AE4B0(void) {
    stPersClosing = FALSE;
}

static void DWCi_GPGetReverseBuddiesListCallback(GPConnection* connection,
                                                 GPGetReverseBuddiesListResponseArg* arg,
                                                 void* param) {
    int i;
    int j;

    if (!DWCi_IsFriendUpdating()) {
        return;
    }

    if (stpFriendCnt->reverseBuddyList != NULL) {
        DWC_Free(DWC_ALLOCTYPE_BASE, stpFriendCnt->reverseBuddyList, 0);
        stpFriendCnt->reverseBuddyList = NULL;
    }

    if (arg->result != GP_NO_ERROR) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, " ERROR: GetReverseBuddies Req. why??? : %d\n",
                   arg->result);
        stpFriendCnt->updateCallback(DWC_ERROR_NONE, stpFriendCnt->friendListChanged,
                                     stpFriendCnt->updateParam);
        stpFriendCnt->state = 2;
        return;
    }

    for (i = 0; i < arg->numOfProfiles; i++) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "GetReverseBuddies   profileID = %u \n",
                   arg->profiles[i].profile);
    }

    for (i = 0; i < stpFriendCnt->friendListLen; i++) {
        if (fn_806CF1B0(&stpFriendCnt->friendList[i]) != DWC_FRIENDDATA_GS_PROFILE_ID) {
            continue;
        }

        for (j = 0; j < arg->numOfProfiles; j++) {
            if (arg->profiles[j].profile ==
                fn_806D0060(fn_806AEAF0(), &stpFriendCnt->friendList[i])) {
                if (fn_806CF190(&stpFriendCnt->friendList[i]) == 0) {
                    stpFriendCnt->friendListChanged = TRUE;
                    fn_806CF200(&stpFriendCnt->friendList[i], 1);
                    if (fn_806CF0E0(&stpFriendCnt->friendList[i])) {
                        fn_806CF250(&stpFriendCnt->friendList[i], 1);
                    }
                }
                break;
            }
        }

        if (j == arg->numOfProfiles && fn_806CF190(&stpFriendCnt->friendList[i]) == 1) {
            stpFriendCnt->friendListChanged = TRUE;
            fn_806CF200(&stpFriendCnt->friendList[i], 0);
        }

        if (fn_806CF0E0(&stpFriendCnt->friendList[i])) {
            fn_806CF250(&stpFriendCnt->friendList[i], 1);
        }
    }

    stpFriendCnt->updateCallback(DWC_ERROR_NONE, stpFriendCnt->friendListChanged,
                                 stpFriendCnt->updateParam);
    stpFriendCnt->state = 2;
}
