// Rewritten from the target assembly (DWC release build Aug 27 2010).
// User data / login id / friend data handling. Function names follow the NitroDWC/RVL DWC API
// (an older build of this file is in Pokemon Battle Revolution with symbols); the helpers and
// the layouts are inferred from the code.
#include <dwc/dwc_account.h>
#include <dwc/dwc_init.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <stdio.h>
#include <string.h>

// inferred: MATH CRC functions of the SDK (MATH_CRC8InitTable, MATH_CRC32InitTableRev,
// MATH_CalcCRC8, MATH_CalcCRC32 in NitroSDK naming)
typedef struct MATHCRC8Table {
    u8 table[256];
} MATHCRC8Table;
typedef struct MATHCRC32Table {
    u32 table[256];
} MATHCRC32Table;
void fn_806A243C(MATHCRC8Table* table, u8 poly);
void fn_806A24DC(MATHCRC32Table* table, u32 poly);
u8 fn_806A257C(const MATHCRC8Table* table, const void* data, u32 dataLength);
u32 fn_806A264C(const MATHCRC32Table* table, const void* data, u32 dataLength);
// inferred: message digest (SHA-1/MD5 style) of a buffer
void fn_806AB8B0(u8* digest, const void* data, u32 dataLength);
// inferred: DWCi_Auth_GetConsoleUserId (dwc_auth_interface.c)
u64 DWCi_Auth_GetConsoleUserId(void);

// inferred: list of allocations freed by fn_806CF030 (DWCi_CfReset in MKW)
typedef struct DWCiCfEntry {
    u8 unk00[0x18];
    struct DWCiCfEntry* next; // 0x18
} DWCiCfEntry;

static DWCiCfEntry* dwci_cfList;

// inferred: not referenced in this game (dead-stripped); its format string heads the pool
void DWCi_Acc_FriendKeyToString(char* str, u64 friendKey) {
    snprintf(str, 13, "%012llu", friendKey);
}

int fn_806CF030(void) {
    while (dwci_cfList != NULL) {
        DWCiCfEntry* next = dwci_cfList->next;
        DWC_Free(DWC_ALLOCTYPE_NHTTP, dwci_cfList, 0);
        dwci_cfList = next;
    }
    return 0;
}

static inline u32 DWCi_Acc_GetMaskBits(u32 data, u32 shift, u32 mask) {
    return (data >> shift) & mask;
}

static inline void DWCi_Acc_SetMaskBits(u32* ptr, u32 data, u32 shift, u32 mask) {
    if (data & ~mask) {
        return;
    }
    *ptr = (*ptr & ~(mask << shift)) | (data << shift);
}

static inline u32 DWCi_Acc_GetFlags(const DWCAccFlag* data) {
    return DWCi_Acc_GetMaskBits(data->flags, DWC_ACC_FLAGS_SHIFT, DWC_ACC_FLAGS_MASK);
}

static inline void DWCi_Acc_SetFlags(DWCAccFlag* data, u32 flags) {
    DWCi_Acc_SetMaskBits(&data->flags, flags, DWC_ACC_FLAGS_SHIFT, DWC_ACC_FLAGS_MASK);
}

static inline u32 DWCi_Acc_GetFlag_DataType(const DWCAccFlag* data) {
    return DWCi_Acc_GetFlags(data) & DWC_FRIENDDATA_TYPE_MASK;
}

static inline void DWCi_Acc_SetFlag_DataType(DWCAccFlag* data, u32 type) {
    DWCi_Acc_SetFlags(data, (DWCi_Acc_GetFlags(data) & ~DWC_FRIENDDATA_TYPE_MASK) | type);
}

u64 DWCi_Acc_GetUserId(const DWCAccLoginId* loginid) {
    u64 userid = DWCi_Acc_GetMaskBits(loginid->id_data, DWC_ACC_USERID_HI32_SHIFT,
                                      DWC_ACC_USERID_HI32_MASK);
    return (userid << 32) | loginid->userid_lo32;
}

u32 DWCi_Acc_GetPlayerId(const DWCAccLoginId* loginid) {
    return loginid->playerid;
}

void DWCi_Acc_SetUserId(DWCAccLoginId* loginid, u64 userid) {
    DWCi_Acc_SetMaskBits(&loginid->id_data, (u32)(userid >> 32), DWC_ACC_USERID_HI32_SHIFT,
                         DWC_ACC_USERID_HI32_MASK);
    loginid->userid_lo32 = (u32)userid;
}

void DWCi_Acc_SetPlayerId(DWCAccLoginId* loginid, u32 playerid) {
    loginid->playerid = playerid;
}

BOOL fn_806CF0E0(const DWCFriendData* frienddata) {
    if (DWC_IsBuddyFriendData(frienddata) &&
        (DWCi_Acc_GetFlags(&frienddata->flags) & DWC_FRIENDDATA_FLAG_8) == DWC_FRIENDDATA_FLAG_8) {
        return TRUE;
    }
    return FALSE;
}

BOOL DWC_IsBuddyFriendData(const DWCFriendData* frienddata) {
    u32 flags = DWCi_Acc_GetFlags(&frienddata->flags);
    if ((flags & DWC_FRIENDDATA_TYPE_MASK) == DWC_FRIENDDATA_GS_PROFILE_ID) {
        return (flags >> 2) & 1 ? TRUE : FALSE;
    }
    return FALSE;
}

BOOL fn_806CF160(const DWCFriendData* frienddata) {
    u32 flags = DWCi_Acc_GetFlags(&frienddata->flags);
    if ((flags & DWC_FRIENDDATA_FLAG_10) == DWC_FRIENDDATA_FLAG_10 &&
        (flags & DWC_FRIENDDATA_FLAG_8) != DWC_FRIENDDATA_FLAG_8) {
        return TRUE;
    }
    return FALSE;
}

BOOL fn_806CF190(const DWCFriendData* frienddata) {
    return (DWCi_Acc_GetFlags(&frienddata->flags) >> 3) & 1;
}

BOOL fn_806CF1A0(const DWCFriendData* frienddata) {
    return (DWCi_Acc_GetFlags(&frienddata->flags) >> 5) & 1;
}

int DWC_GetFriendDataType(const DWCFriendData* frienddata) {
    return DWCi_Acc_GetFlag_DataType(&frienddata->flags);
}

void DWCi_SetBuddyFriendData(DWCFriendData* frienddata) {
    u32 flags = DWCi_Acc_GetFlags(&frienddata->flags);
    if ((flags & DWC_FRIENDDATA_TYPE_MASK) != DWC_FRIENDDATA_GS_PROFILE_ID) {
        return;
    }
    DWCi_Acc_SetFlags(&frienddata->flags, (flags & ~DWC_FRIENDDATA_BUDDY) | DWC_FRIENDDATA_BUDDY);
}

void fn_806CF200(DWCFriendData* frienddata, BOOL set) {
    u32 flags;
    if (set) {
        flags = (DWCi_Acc_GetFlags(&frienddata->flags) & ~DWC_FRIENDDATA_FLAG_8) |
                DWC_FRIENDDATA_FLAG_8;
    } else {
        flags = frienddata->flags.flags >> DWC_ACC_FLAGS_SHIFT;
        flags &= ~DWC_FRIENDDATA_FLAG_8;
    }
    DWCi_Acc_SetFlags(&frienddata->flags, flags);
}

void fn_806CF250(DWCFriendData* frienddata, BOOL set) {
    u32 flags;
    if (set) {
        flags = (DWCi_Acc_GetFlags(&frienddata->flags) & ~DWC_FRIENDDATA_FLAG_10) |
                DWC_FRIENDDATA_FLAG_10;
    } else {
        flags = frienddata->flags.flags >> DWC_ACC_FLAGS_SHIFT;
        flags &= ~DWC_FRIENDDATA_FLAG_10;
    }
    DWCi_Acc_SetFlags(&frienddata->flags, flags);
}

void fn_806CF2A0(DWCFriendData* frienddata, BOOL set) {
    u32 flags;
    if (set) {
        flags = (DWCi_Acc_GetFlags(&frienddata->flags) & ~DWC_FRIENDDATA_FLAG_20) |
                DWC_FRIENDDATA_FLAG_20;
    } else {
        flags = frienddata->flags.flags >> DWC_ACC_FLAGS_SHIFT;
        flags &= ~DWC_FRIENDDATA_FLAG_20;
    }
    DWCi_Acc_SetFlags(&frienddata->flags, flags);
}

static inline u32 DWCi_Acc_SwapU32(u32 x) {
    x = ((x >> 8) & 0x00FF00FF) | ((x << 8) & 0xFF00FF00);
    return (x >> 16) | (x << 16);
}

// inferred: friend key = (hash of the profile id and the game code) << 32 | profile id
static inline u64 DWCi_Acc_CreateFriendKey(int gs_profile_id, u32 gamecode, int method) {
    u32 buf[2];
    int hash;
    buf[0] = DWCi_Acc_SwapU32((u32)gs_profile_id);
    buf[1] = DWCi_Acc_SwapU32(gamecode);
    switch (method) {
    case 0: {
        MATHCRC8Table table;
        fn_806A243C(&table, 7);
        hash = fn_806A257C(&table, buf, sizeof(buf)) & 0x7F;
        break;
    }
    case 1: {
        u8 digest[16];
        fn_806AB8B0(digest, buf, sizeof(buf));
        hash = digest[0] >> 1;
        break;
    }
    }
    return ((u64)hash << 32) | (u32)gs_profile_id;
}

static inline BOOL DWCi_Acc_CheckFriendKey(u64 friendKey, u32 gamecode, int method) {
    int gs_profile_id = (int)friendKey;
    if (gs_profile_id <= 0) {
        return FALSE;
    }
    return friendKey == DWCi_Acc_CreateFriendKey(gs_profile_id, gamecode, method);
}

// NONMATCHING: ~75% - register allocation of the two byte swaps differs (same instructions);
// tried: separate MD5 helper, u64/struct buffers, swap forms and statement orders
BOOL DWC_CheckFriendKey(const DWCUserData* userdata, u64 friendKey) {
    return DWCi_Acc_CheckFriendKey(friendKey, userdata->gamecode, 1);
}

void fn_806CF3A0(u64 num, int bits, char* str) {
    int i;
    int len = (bits + 4) / 5;
    const char* table = "0123456789abcdefghijklmnopqrstuv";
    for (i = 0; i < len; i++) {
        *(str + len - 1 - i) = table[num & 0x1F];
        num >>= 5;
    }
    str[len] = '\0';
}

void DWCi_Acc_LoginIdToUserName(const DWCAccLoginId* loginid, u32 gamecode, char* username) {
    char userid[DWC_ACC_USERNAME_STRING_BUFSIZE];
    char playerid[DWC_ACC_USERNAME_STRING_BUFSIZE];
    fn_806CF3A0(DWCi_Acc_GetUserId(loginid), 43, userid);
    fn_806CF3A0(DWCi_Acc_GetPlayerId(loginid), 32, playerid);
    snprintf(username, DWC_ACC_USERNAME_STRING_BUFSIZE, "%s%c%c%c%c%s", userid,
             (u8)(gamecode >> 24), (u8)(gamecode >> 16), (u8)(gamecode >> 8), (u8)gamecode,
             playerid);
}

static inline void DWCi_Acc_CreateTempLoginIdInline(DWCAccLoginId* loginid) {
    u64 seed = (u32)OSGetTime();
    DWCi_Acc_SetUserId(loginid, DWCi_Auth_GetConsoleUserId());
    DWCi_Acc_SetPlayerId(loginid, (u32)((seed * 0x5D588B656C078965 + 0x269EC3) >> 32));
    DWCi_Acc_SetFlag_DataType((DWCAccFlag*)loginid, DWC_FRIENDDATA_LOGIN_ID);
}

static inline u32 DWCi_Acc_GetCrc32(const DWCAccUserData* userdata) {
    MATHCRC32Table table;
    u32 buf[16];
    int i;
    fn_806A24DC(&table, 0xEDB88320);
    for (i = 0; i < 16; i++) {
        buf[i] = DWCi_Acc_SwapU32(((const u32*)userdata)[i]);
    }
    return fn_806A264C(&table, buf, 0x3C);
}

static inline void DWCi_Acc_UpdateCrc32(DWCAccUserData* userdata) {
    userdata->crc32 = DWCi_Acc_GetCrc32(userdata);
    userdata->flag |= 1;
}

void DWCi_Acc_CreateUserData(DWCAccUserData* userdata, u32 gamecode) {
    MATHCRC32Table table;
    u32 buf[16];
    int i;
    memset(userdata, 0, sizeof(DWCAccUserData));
    userdata->size = sizeof(DWCAccUserData);
    userdata->gs_profile_id = 0;
    userdata->gamecode = gamecode;
    DWCi_Acc_CreateTempLoginIdInline(&userdata->pseudo);
    DWCi_Acc_SetFlag_DataType((DWCAccFlag*)&userdata->authentic, DWC_FRIENDDATA_NODATA);
    fn_806A24DC(&table, 0xEDB88320);
    for (i = 0; i < 16; i++) {
        buf[i] = DWCi_Acc_SwapU32(((const u32*)userdata)[i]);
    }
    userdata->crc32 = fn_806A264C(&table, buf, 0x3C);
    userdata->flag |= 1;
}

void DWCi_Acc_CreateTempLoginId(DWCAccLoginId* loginid) {
    DWCi_Acc_CreateTempLoginIdInline(loginid);
}

BOOL DWCi_Acc_CheckConsoleUserId(const DWCAccLoginId* loginid) {
    return DWCi_Acc_GetUserId(loginid) == DWCi_Auth_GetConsoleUserId();
}

BOOL DWCi_Acc_IsValidLoginId(const DWCAccLoginId* loginid) {
    return DWCi_Acc_GetFlag_DataType((const DWCAccFlag*)loginid) == DWC_FRIENDDATA_LOGIN_ID;
}

BOOL DWCi_Acc_IsAuthentic(const DWCAccUserData* userdata) {
    return DWCi_Acc_IsValidLoginId(&userdata->authentic);
}

BOOL DWC_IsValidFriendData(const DWCFriendData* frienddata) {
    return DWCi_Acc_GetFlag_DataType(&frienddata->flags) != DWC_FRIENDDATA_NODATA;
}

BOOL DWCi_Acc_IsValidFriendData(const DWCAccFriendData* frienddata) {
    return DWCi_Acc_GetFlag_DataType(&frienddata->flags) != DWC_FRIENDDATA_NODATA;
}

void DWC_CreateUserData(DWCUserData* userdata) {
    DWCi_Acc_CreateUserData(userdata, DWCi_GetGamecode());
}

BOOL DWC_CheckUserData(const DWCUserData* userdata) {
    u32 crc = DWCi_Acc_GetCrc32(userdata);
    return crc == userdata->crc32;
}

// inferred: debug dump of the user data; nothing calls it in this game (only its strings remain)
void DWCi_Acc_DumpUserData(const DWCUserData* userdata) {
    int i;
    for (i = 0; i < 16; i++) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "%08x: %08x\n", i * 4, ((const u32*)userdata)[i]);
    }
    if (DWC_CheckUserData(userdata)) {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "valid\n");
    } else {
        DWC_Printf(DWC_REPORTFLAG_DEBUG, "invalid\n");
    }
}

BOOL DWC_CheckHasProfile(const DWCUserData* userdata) {
    BOOL ret = FALSE;
    if (DWCi_Acc_IsAuthentic(userdata) && userdata->gs_profile_id > 0) {
        ret = TRUE;
    }
    return ret ? TRUE : FALSE;
}

void DWCi_Acc_SetLoginIdToUserData(DWCAccUserData* userdata, const DWCAccLoginId* loginid,
                                   int gs_profile_id) {
    userdata->authentic = *loginid;
    userdata->gs_profile_id = gs_profile_id;
    DWCi_Acc_UpdateCrc32(userdata);
}

BOOL DWC_CheckDirtyFlag(const DWCUserData* userdata) {
    return userdata->flag & 1;
}

void fn_806CFEB0(DWCUserData* userdata);

void DWC_ClearDirtyFlag(DWCUserData* userdata) {
    fn_806CFEB0(userdata);
}

void fn_806CFEB0(DWCUserData* userdata) {
    MATHCRC32Table table;
    u32 buf[16];
    int i;
    userdata->flag &= ~1;
    fn_806A24DC(&table, 0xEDB88320);
    for (i = 0; i < 16; i++) {
        buf[i] = DWCi_Acc_SwapU32(((const u32*)userdata)[i]);
    }
    userdata->crc32 = fn_806A264C(&table, buf, 0x3C);
}


u64 fn_806D0030(const DWCFriendData* frienddata) {
    if (DWCi_Acc_GetFlag_DataType(&frienddata->flags) == DWC_FRIENDDATA_FRIEND_KEY) {
        return ((u64)frienddata->friend_key.friendkey_hi32 << 32) |
               frienddata->friend_key.friendkey_lo32;
    }
    return 0;
}

// NONMATCHING: ~75% - the target keeps the game code in a register across the loop but swaps it
// inside; here it is either reloaded each iteration or the swap is hoisted
int DWC_GetGsProfileId(const DWCUserData* userdata, const DWCFriendData* frienddata) {
    switch (DWCi_Acc_GetFlag_DataType(&frienddata->flags)) {
    case DWC_FRIENDDATA_FRIEND_KEY: {
        u64 friendKey = ((u64)frienddata->friend_key.friendkey_hi32 << 32) |
                        frienddata->friend_key.friendkey_lo32;
        int i;
        for (i = 0; i < 2; i++) {
            if (DWCi_Acc_CheckFriendKey(friendKey, userdata->gamecode, i)) {
                return (int)(friendKey & 0xFFFFFFFF);
            }
        }
        return 0;
    }
    case DWC_FRIENDDATA_GS_PROFILE_ID:
        return frienddata->gs_profile_id.id;
    case DWC_FRIENDDATA_LOGIN_ID:
        return -1;
    default:
        return 0;
    }
}

// NONMATCHING: ~70% - same byte swap register allocation difference as DWC_CheckFriendKey
u64 DWC_CreateFriendKey(const DWCUserData* userdata) {
    u64 friendKey = 0;
    if (userdata->gs_profile_id != 0) {
        friendKey = DWCi_Acc_CreateFriendKey(userdata->gs_profile_id, userdata->gamecode, 1);
    }
    return friendKey;
}

void DWC_CreateFriendKeyToken(DWCFriendData* token, u64 friendKey) {
    memset(token, 0, sizeof(DWCFriendData));
    token->friend_key.friendkey_lo32 = (u32)friendKey;
    token->friend_key.friendkey_hi32 = (u32)(friendKey >> 32);
    DWCi_Acc_SetFlag_DataType(&token->flags, DWC_FRIENDDATA_FRIEND_KEY);
}

void fn_806D02E0(DWCFriendData* frienddata, int gs_profile_id) {
    memset(frienddata, 0, sizeof(DWCFriendData));
    frienddata->gs_profile_id.id = gs_profile_id;
    DWCi_Acc_SetFlag_DataType(&frienddata->flags, DWC_FRIENDDATA_GS_PROFILE_ID);
}

void DWC_LoginIdToUserName(const DWCUserData* userdata, const DWCAccLoginId* loginid,
                           char* username) {
    DWCi_Acc_LoginIdToUserName(loginid, userdata->gamecode, username);
}

BOOL DWC_IsEqualFriendData(const DWCFriendData* a, const DWCFriendData* b) {
    u32 type = DWCi_Acc_GetFlag_DataType(&a->flags);
    if (type != DWCi_Acc_GetFlag_DataType(&b->flags)) {
        return FALSE;
    }
    if (type == DWC_FRIENDDATA_GS_PROFILE_ID) {
        return a->gs_profile_id.id == b->gs_profile_id.id;
    } else if (type == DWC_FRIENDDATA_LOGIN_ID) {
        BOOL ret = FALSE;
        if (DWCi_Acc_GetUserId(&a->login_id) == DWCi_Acc_GetUserId(&b->login_id) &&
            DWCi_Acc_GetPlayerId(&a->login_id) == DWCi_Acc_GetPlayerId(&b->login_id)) {
            ret = TRUE;
        }
        return ret;
    } else if (type == DWC_FRIENDDATA_FRIEND_KEY) {
        return (((u64)a->friend_key.friendkey_hi32 << 32) | a->friend_key.friendkey_lo32) ==
               (((u64)b->friend_key.friendkey_hi32 << 32) | b->friend_key.friendkey_lo32);
    }
    return FALSE;
}

// NONMATCHING: ~85% - the target compares the cases in order 2,3,1 and lays out the bodies 3,2,1,
// and its inlined user name buffers are swapped; making the user name call one inline level
// shallower fixes the buffers but then this function gets inlined into DWC_ReportUserData
void DWC_ReportFriendData(const DWCUserData* userdata, const DWCFriendData* frienddata) {
    char username[DWC_ACC_USERNAME_STRING_BUFSIZE];
    char friendKey[13];
    u32 flags = DWCi_Acc_GetFlags(&frienddata->flags);
    switch (flags & DWC_FRIENDDATA_TYPE_MASK) {
    case DWC_FRIENDDATA_GS_PROFILE_ID:
        if ((flags & DWC_FRIENDDATA_BUDDY) == DWC_FRIENDDATA_BUDDY &&
            (flags & DWC_FRIENDDATA_FLAG_8) == DWC_FRIENDDATA_FLAG_8) {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, " GS_ID : %d (ok)\n", frienddata->gs_profile_id.id);
        } else {
            DWC_Printf(DWC_REPORTFLAG_DEBUG, " GS_ID : %d\n", frienddata->gs_profile_id.id);
        }
        break;
    case DWC_FRIENDDATA_FRIEND_KEY:
        DWCi_Acc_FriendKeyToString(friendKey, ((u64)frienddata->friend_key.friendkey_hi32 << 32) |
                                                  frienddata->friend_key.friendkey_lo32);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, " F_KEY : %s\n", friendKey);
        break;
    case DWC_FRIENDDATA_LOGIN_ID:
        DWC_LoginIdToUserName(userdata, &frienddata->login_id, username);
        DWC_Printf(DWC_REPORTFLAG_DEBUG, " LN_ID : %s\n", username);
        break;
    default:
        DWC_Printf(DWC_REPORTFLAG_DEBUG, " NO_DATA \n");
        break;
    }
}

void DWC_ReportUserData(const DWCUserData* userdata) {
    DWC_Printf(DWC_REPORTFLAG_ALL, "*******************************\n");
    DWC_Printf(DWC_REPORTFLAG_ALL, " [pseudo login id]\n");
    DWC_ReportFriendData(userdata, (const DWCFriendData*)&userdata->pseudo);
    DWC_Printf(DWC_REPORTFLAG_ALL, "+++++++++++++++++++++++++++++++\n");
    DWC_Printf(DWC_REPORTFLAG_ALL, " [authentic login id]\n");
    DWC_ReportFriendData(userdata, (const DWCFriendData*)&userdata->authentic);
    DWC_Printf(DWC_REPORTFLAG_ALL, "+++++++++++++++++++++++++++++++\n");
    DWC_Printf(DWC_REPORTFLAG_ALL, " GS_ID : %d\n", userdata->gs_profile_id);
    DWC_Printf(DWC_REPORTFLAG_ALL, "*******************************\n");
}
