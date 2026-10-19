// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Login sequence: remote authentication (NAS) followed by the GameSpy Presence (GP) connection,
// creating the authentic login id the first time. Structure and names are inferred from the code
// and the debug strings (the NitroDWC/RVL DWC dwc_login.c has the same flow).
#include <dwc/dwc_account.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <stdio.h>
#include <string.h>

// inferred: GameSpy Presence types (gp.h)
typedef void* GPConnection;
typedef int GPResult;
typedef void (*GPCallback)(GPConnection* connection, void* arg, void* param);

typedef struct GPConnectResponseArg {
    GPResult result; // 0x00
    int profile;     // 0x04
} GPConnectResponseArg;

typedef struct GPGetInfoResponseArg {
    GPResult result;  // 0x00
    int profile;      // 0x04
    char unk08[0x86]; // 0x08 nick, uniquenick, email, firstname
    char lastname[1]; // 0x8E inferred: only the offset is known
} GPGetInfoResponseArg;

// inferred: GameSpy Presence entry points (gpProcess, gpConnectPreAuthenticated, gpDisconnect,
// gpGetInfo, gpSetInfos)
GPResult fn_806DA8D0(GPConnection* connection);
GPResult fn_806DA980(GPConnection* connection, const char* authtoken,
                     const char* partnerchallenge, int firewall, int blocking,
                     GPCallback callback, void* param);
GPResult fn_806DAAB0(GPConnection* connection);
GPResult fn_806DAC00(GPConnection* connection, int profile, int checkCache, int blocking,
                     GPCallback callback, void* param);
GPResult fn_806DACE0(GPConnection* connection, int info, const char* value);

// inferred: remote authentication interface (dwc_auth_interface.c)
int fn_806D0980(const char* gamename, const char* ingamesn, u64 userid, void* alloc,
                 void* free);
void fn_806D0C30(void);
void fn_806D0EA0(void);
BOOL fn_806D1590(void);
BOOL fn_806D15E0(void);
int fn_806D1600(void);
void fn_806D1610(char* authToken, char* partnerChallenge);
void fn_806D1660(void);
u64 fn_806D1670(void);
BOOL fn_806D1690(void);

// inferred: console id / name helpers (dwc_init.c area)
u64 fn_806AA690(void);
const char* fn_806AA7C0(void);
const char* fn_806AA7D0(void);
// inferred: friend manager / match callbacks started after the login
GPResult fn_806BB440(int status);
BOOL fn_806B13C0(void);
void fn_806CD450(int profile);
void fn_806CD460(void);

typedef void (*DWCLoginCallback)(DWCError error, int profileID, void* param);

typedef enum DWCLoginState {
    DWC_LOGIN_STATE_INIT = 0,
    DWC_LOGIN_STATE_REMOTE_AUTH = 1,
    DWC_LOGIN_STATE_CONNECTING = 2,
    DWC_LOGIN_STATE_GPGETINFO = 3,
    DWC_LOGIN_STATE_GPSETINFO = 4,
    DWC_LOGIN_STATE_CONNECTED = 5
} DWCLoginState;

// inferred: login control (0x268 bytes, allocated by the caller)
typedef struct DWCLoginControl {
    GPConnection* pGpObj;       // 0x000
    DWCLoginState state;        // 0x004
    int productID;              // 0x008
    u32 gamecode;               // 0x00C
    const char* playerName;     // 0x010
    DWCLoginCallback callback;  // 0x014
    void* param;                // 0x018
    DWCUserData* userdata;      // 0x01C
    u8 unk20[0x10];             // 0x020
    BOOL connectFlag;           // 0x030
    u8 unk34[4];                // 0x034
    s64 connectTime;            // 0x038
    DWCAccLoginId tempLoginId;  // 0x040
    char authToken[256];        // 0x04C
    char partnerChallenge[256]; // 0x14C
    char username[28];          // 0x24C
} DWCLoginControl;

static DWCLoginControl* stpLoginCnt;

int fn_806AEF90(void);
void fn_806AF170(void);
void fn_806AF3F0(GPConnection* connection, GPGetInfoResponseArg* arg, void* param);
void fn_806AED20(GPConnection* connection, GPConnectResponseArg* arg, void* param);

void fn_806AE750(DWCLoginControl* logcnt, DWCUserData* userdata, GPConnection* pGpObj,
                 int productID, u32 gamecode, const char* playerName, DWCLoginCallback callback,
                 void* param) {
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "Login Init\n");
    stpLoginCnt = logcnt;
    memset(logcnt, 0, sizeof(DWCLoginControl));
    stpLoginCnt->pGpObj = pGpObj;
    stpLoginCnt->state = DWC_LOGIN_STATE_INIT;
    stpLoginCnt->productID = productID;
    stpLoginCnt->gamecode = gamecode;
    stpLoginCnt->playerName = playerName;
    stpLoginCnt->callback = callback;
    stpLoginCnt->param = param;
    stpLoginCnt->userdata = userdata;

    DWC_Printf(DWC_REPORTFLAG_LOGIN, "******************************************\n");
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "userdata::\n");
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "  pseudo    UserID   : %016llx\n",
               DWCi_Acc_GetUserId(&userdata->pseudo));
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "  pseudo    PlayerID : %08x\n",
               DWCi_Acc_GetPlayerId(&userdata->pseudo));
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "  authentic UserID   : %016llx\n",
               DWCi_Acc_GetUserId(&userdata->authentic));
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "  authentic PlayerID : %08x\n",
               DWCi_Acc_GetPlayerId(&userdata->authentic));
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "console::\n");
    if (fn_806D1690()) {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, "  authentic UserID   : %016llx\n", fn_806D1670());
    } else {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, "  pseudo    UserID   : %016llx\n", fn_806D1670());
    }
    DWC_Printf(DWC_REPORTFLAG_LOGIN, "******************************************\n");
}

BOOL fn_806AE910(void) {
    if (fn_806AEF90()) {
        stpLoginCnt->state = DWC_LOGIN_STATE_REMOTE_AUTH;
        stpLoginCnt->connectFlag = FALSE;
        return TRUE;
    }
    return FALSE;
}

void fn_806AEB10(DWCError error, int errorCode);

void fn_806AE960(void) {
    if (stpLoginCnt == NULL || DWCi_IsError()) {
        return;
    }

    switch (stpLoginCnt->state) {
    case DWC_LOGIN_STATE_REMOTE_AUTH:
        fn_806AF170();
        break;
    case DWC_LOGIN_STATE_CONNECTING:
    case DWC_LOGIN_STATE_GPGETINFO:
    case DWC_LOGIN_STATE_GPSETINFO:
        if (stpLoginCnt->pGpObj != NULL && *stpLoginCnt->pGpObj != NULL) {
            fn_806DA8D0(stpLoginCnt->pGpObj);
        }
        if (stpLoginCnt->connectFlag) {
            s64 now = OSGetTime();
            if (OS_TICKS_TO_MSEC(now - stpLoginCnt->connectTime) > 60000) {
                fn_806AEB10(DWC_ERROR_NETWORK, -61070);
                stpLoginCnt->connectFlag = FALSE;
            }
        }
        break;
    default:
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "Ignore invalid login state.\n");
        break;
    }
}

DWCUserData* fn_806AEAF0(void) {
    if (stpLoginCnt != NULL) {
        return stpLoginCnt->userdata;
    }
    return NULL;
}

static inline void DWCi_CloseLogin(void) {
    if (stpLoginCnt == NULL) {
        return;
    }
    fn_806D1660();
    stpLoginCnt->state = DWC_LOGIN_STATE_INIT;
    stpLoginCnt->connectFlag = FALSE;
}

void fn_806AEB10(DWCError error, int errorCode) {
    if (stpLoginCnt == NULL || error == DWC_ERROR_NONE) {
        return;
    }
    DWCi_SetError(error, errorCode);
    if (stpLoginCnt->callback != NULL) {
        stpLoginCnt->callback(error, 0, stpLoginCnt->param);
    }
    DWCi_CloseLogin();
}

void fn_806AEBB0(void) {
    if (!fn_806D1590()) {
        fn_806D0C30();
    }
    stpLoginCnt = NULL;
}

GPResult fn_806AEBF0(GPResult result) {
    DWCError error;
    int errorCode;

    if (result == 0) {
        return 0;
    }
    DWC_Printf(DWC_REPORTFLAG_ERROR, "Login, GP error %d\n", result);

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
    fn_806AEB10(error, errorCode - 61000);
    return result;
}

void fn_806AED20(GPConnection* connection, GPConnectResponseArg* arg, void* param) {
    char consoleId[32];
    char userid[64];

    DWC_Printf(DWC_REPORTFLAG_LOGIN, "Finished connecting to GP server, result = %d\n",
               arg->result);
    stpLoginCnt->connectFlag = FALSE;

    snprintf(consoleId, 31, "%s:%016llu", fn_806AA7C0(), fn_806AA690());
    if (fn_806AEBF0(fn_806DACE0(connection, 0x704, consoleId))) {
        return;
    }
    fn_806CF3A0(fn_806D1670(), 43, userid);
    if (fn_806AEBF0(fn_806DACE0(connection, 0x717, userid))) {
        return;
    }
    if (fn_806AEBF0(fn_806DACE0(connection, 0x708, fn_806AA7D0()))) {
        return;
    }

    if (arg->result == 0) {
        if (stpLoginCnt->state == DWC_LOGIN_STATE_CONNECTING) {
            if (stpLoginCnt->userdata->gs_profile_id == arg->profile) {
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "  gs profile id is valid.\n");
                stpLoginCnt->state = DWC_LOGIN_STATE_CONNECTED;
                if (fn_806AEBF0(fn_806BB440(1))) {
                    return;
                }
                stpLoginCnt->callback(DWC_ERROR_NONE, arg->profile, stpLoginCnt->param);
                if (fn_806B13C0() == FALSE) {
                    fn_806CD450(arg->profile);
                    fn_806CD460();
                    return;
                }
                return;
            } else {
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "  gs profile id is invalid.\n");
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "  You can't use copied userdata.\n");
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "  Please find DWC_AllowMovedUserData().\n");
                fn_806AEB10(DWC_ERROR_NETWORK, -60000);
            }
        } else if (stpLoginCnt->state == DWC_LOGIN_STATE_GPGETINFO) {
            if (fn_806AEBF0(fn_806DAC00(connection, arg->profile, 0, 0, (GPCallback)fn_806AF3F0,
                                        NULL))) {
                return;
            }
        }
    } else {
        fn_806AEBF0(arg->result);
    }
}

int fn_806AEF90(void) {
    u64 userid;

    DWC_Printf(DWC_REPORTFLAG_LOGIN, "Start Remote Auth\n");
    if (DWCi_Acc_IsAuthentic(stpLoginCnt->userdata)) {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, "  Hmm.. you already have authentic login id.\n");
        DWCi_Acc_LoginIdToUserName(&stpLoginCnt->userdata->authentic,
                                   stpLoginCnt->userdata->gamecode, stpLoginCnt->username);
        userid = DWCi_Acc_GetUserId(&stpLoginCnt->userdata->authentic);
    } else {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, "  Hmm.. you need to create authentic login id.\n");
        if (!DWCi_Acc_IsValidLoginId(&stpLoginCnt->tempLoginId)) {
            DWC_Printf(DWC_REPORTFLAG_LOGIN,
                       "    Hmm.. you are the first time to get authentic login id.");
            if (DWCi_Acc_CheckConsoleUserId(&stpLoginCnt->userdata->pseudo)) {
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "- copy temp loginid from pseudo login id\n");
                stpLoginCnt->tempLoginId = stpLoginCnt->userdata->pseudo;
            } else {
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "- create temp loginid from console user id\n");
                DWCi_Acc_CreateTempLoginId(&stpLoginCnt->tempLoginId);
            }
        } else {
            DWC_Printf(DWC_REPORTFLAG_LOGIN,
                       "    Hmm.. you are NOT the first times to get authentic login id.\n");
            {
                u64 seed = OSGetTime();
                DWCi_Acc_SetPlayerId(&stpLoginCnt->tempLoginId,
                                     (u32)((seed * 0x5D588B656C078965 + 0x269EC3) >> 32));
            }
        }
        DWCi_Acc_LoginIdToUserName(&stpLoginCnt->tempLoginId, stpLoginCnt->gamecode,
                                   stpLoginCnt->username);
        userid = 0;
    }
    // inferred: the second argument points into the user name (after the 9 user id characters)
    return fn_806D0980(stpLoginCnt->playerName, stpLoginCnt->username + 9, userid, DWC_Alloc,
                       DWC_Free);
}

static inline GPResult DWCi_ConnectGP(void) {
    s64 now = OSGetTime();
    stpLoginCnt->connectTime = now;
    stpLoginCnt->connectFlag = TRUE;
    return fn_806DA980(stpLoginCnt->pGpObj, stpLoginCnt->authToken,
                       stpLoginCnt->partnerChallenge, 1, 0, (GPCallback)fn_806AED20, NULL);
}

void fn_806AF170(void) {
    int errorCode;

    fn_806D0EA0();
    if (!fn_806D1590()) {
        return;
    }

    if (fn_806D15E0()) {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, " *** Auth Done\n");
        fn_806D1610(stpLoginCnt->authToken, stpLoginCnt->partnerChallenge);
        fn_806D1660();
        if (DWCi_Acc_IsAuthentic(stpLoginCnt->userdata)) {
            DWC_Printf(DWC_REPORTFLAG_LOGIN, "Succeeded to remote authentication.\n");
            if (fn_806AEBF0(DWCi_ConnectGP())) {
                return;
            }
            stpLoginCnt->state = DWC_LOGIN_STATE_CONNECTING;
        } else {
            DWCi_Acc_SetUserId(&stpLoginCnt->tempLoginId, fn_806D1670());
            DWC_Printf(DWC_REPORTFLAG_LOGIN, "Succeeded to remote authentication.\n");
            if (fn_806AEBF0(DWCi_ConnectGP())) {
                return;
            }
            stpLoginCnt->state = DWC_LOGIN_STATE_GPGETINFO;
        }
    } else {
        errorCode = fn_806D1600();
        DWC_Printf(DWC_REPORTFLAG_LOGIN, " *** Auth Error [%d]\n", errorCode);
        if (errorCode <= -29000) {
            fn_806AEB10(DWC_ERROR_FATAL, errorCode);
        } else {
            fn_806AEB10(DWC_ERROR_AUTH_ANY, errorCode);
        }
    }
}

void fn_806AF3F0(GPConnection* connection, GPGetInfoResponseArg* arg, void* param) {
    char lastname[DWC_ACC_USERNAME_STRING_BUFSIZE];
    char pseudoName[DWC_ACC_USERNAME_STRING_BUFSIZE];
    char tempName[DWC_ACC_USERNAME_STRING_BUFSIZE];

    if (arg->result == 0) {
        if (stpLoginCnt->state == DWC_LOGIN_STATE_GPGETINFO) {
            if (arg->lastname[0] == '\0') {
                DWC_Printf(DWC_REPORTFLAG_LOGIN,
                           "    login id is authenticated. set lastname field.\n");
                DWCi_Acc_LoginIdToUserName(&stpLoginCnt->userdata->pseudo, stpLoginCnt->gamecode,
                                           lastname);
                if (fn_806AEBF0(fn_806DACE0(connection, 0x705, lastname))) {
                    return;
                }
                stpLoginCnt->state = DWC_LOGIN_STATE_GPSETINFO;
                if (fn_806AEBF0(fn_806DAC00(connection, arg->profile, 0, 0,
                                            (GPCallback)fn_806AF3F0, NULL))) {
                    return;
                }
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "    call gpSetInfos\n");
            } else {
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "    this login id is used by anybody.... retry.\n");
                fn_806DAAB0(connection);
                fn_806AEF90();
                stpLoginCnt->state = DWC_LOGIN_STATE_REMOTE_AUTH;
            }
        } else if (stpLoginCnt->state == DWC_LOGIN_STATE_GPSETINFO) {
            DWCi_Acc_LoginIdToUserName(&stpLoginCnt->userdata->pseudo, stpLoginCnt->gamecode,
                                       pseudoName);
            if (strcmp(arg->lastname, pseudoName) == 0) {
                DWCi_Acc_LoginIdToUserName(&stpLoginCnt->tempLoginId, stpLoginCnt->gamecode,
                                           tempName);
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "    Account is created : %s(%s) - %d.\n", tempName,
                           pseudoName, arg->profile);
                DWCi_Acc_SetLoginIdToUserData(stpLoginCnt->userdata, &stpLoginCnt->tempLoginId,
                                              arg->profile);
                fn_806DAAB0(connection);
                DWC_Printf(DWC_REPORTFLAG_LOGIN, "Succeeded to remote authentication.\n");
                if (fn_806AEBF0(DWCi_ConnectGP())) {
                    return;
                }
                stpLoginCnt->state = DWC_LOGIN_STATE_CONNECTING;
            } else {
                DWC_Printf(DWC_REPORTFLAG_LOGIN,
                           "    Login but gpSetInfo failed... %s : %d retry gpGetInfo.\n",
                           arg->lastname, arg->profile);
                if (fn_806AEBF0(fn_806DAC00(connection, arg->profile, 0, 0,
                                            (GPCallback)fn_806AF3F0, NULL))) {
                    return;
                }
            }
        }
    } else {
        DWC_Printf(DWC_REPORTFLAG_LOGIN, " ERROR: gpGetInfo. why??? : %d\n", arg->result);
    }
}

BOOL fn_806AF670(void) {
    if (stpLoginCnt != NULL && stpLoginCnt->state == DWC_LOGIN_STATE_CONNECTED) {
        return TRUE;
    }
    return FALSE;
}
