#ifndef DWC_ACCOUNT_H
#define DWC_ACCOUNT_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// inferred: layouts follow the accesses in dwc_account.c; names from the NitroDWC/RVL DWC API.
// The first word of every login id / friend data holds the flags in bits 11-31 and the
// top 11 bits of the 43-bit user id in bits 0-10.
#define DWC_ACC_USERID_HI32_SHIFT 0
#define DWC_ACC_USERID_HI32_MASK 0x7FF
#define DWC_ACC_FLAGS_SHIFT 11
#define DWC_ACC_FLAGS_MASK 0x1FFFFF

// inferred: flag bits
#define DWC_FRIENDDATA_TYPE_MASK 0x3
#define DWC_FRIENDDATA_BUDDY 0x4
#define DWC_FRIENDDATA_FLAG_8 0x8   // inferred: unknown meaning
#define DWC_FRIENDDATA_FLAG_10 0x10 // inferred: unknown meaning
#define DWC_FRIENDDATA_FLAG_20 0x20 // inferred: unknown meaning

#define DWC_FRIENDDATA_NODATA 0
#define DWC_FRIENDDATA_LOGIN_ID 1
#define DWC_FRIENDDATA_FRIEND_KEY 2
#define DWC_FRIENDDATA_GS_PROFILE_ID 3

#define DWC_ACC_USERNAME_STRING_BUFSIZE 21

typedef struct DWCAccLoginId {
    u32 id_data;     // 0x00
    u32 userid_lo32; // 0x04
    u32 playerid;    // 0x08
} DWCAccLoginId;

typedef struct DWCAccFriendKey {
    u32 id_data;        // 0x00
    u32 friendkey_lo32; // 0x04
    u32 friendkey_hi32; // 0x08
} DWCAccFriendKey;

typedef struct DWCAccGsProfileId {
    u32 id_data;  // 0x00
    int id;       // 0x04
    u32 reserved; // 0x08
} DWCAccGsProfileId;

typedef struct DWCAccFlag {
    u32 flags;     // 0x00
    u32 reserved;  // 0x04
    u32 reserved1; // 0x08
} DWCAccFlag;

typedef union DWCAccFriendData {
    DWCAccLoginId login_id;
    DWCAccFriendKey friend_key;
    DWCAccGsProfileId gs_profile_id;
    DWCAccFlag flags;
} DWCAccFriendData;

typedef struct DWCAccUserData {
    int size;                 // 0x00
    DWCAccLoginId pseudo;     // 0x04
    DWCAccLoginId authentic;  // 0x10
    int gs_profile_id;        // 0x1C
    int flag;                 // 0x20 bit 0: dirty
    u32 gamecode;             // 0x24
    int reserved[5];          // 0x28
    u32 crc32;                // 0x3C
} DWCAccUserData;

typedef DWCAccUserData DWCUserData;
typedef DWCAccFriendData DWCFriendData;

u64 DWCi_Acc_GetUserId(const DWCAccLoginId* loginid);
u32 DWCi_Acc_GetPlayerId(const DWCAccLoginId* loginid);
void DWCi_Acc_SetUserId(DWCAccLoginId* loginid, u64 userid);
void DWCi_Acc_SetPlayerId(DWCAccLoginId* loginid, u32 playerid);
BOOL DWC_IsBuddyFriendData(const DWCFriendData* frienddata);
int DWC_GetFriendDataType(const DWCFriendData* frienddata);
void DWCi_SetBuddyFriendData(DWCFriendData* frienddata);
BOOL DWC_CheckFriendKey(const DWCUserData* userdata, u64 friendKey);
void fn_806CF3A0(u64 num, int bits, char* str);
void DWCi_Acc_LoginIdToUserName(const DWCAccLoginId* loginid, u32 gamecode, char* username);
void DWCi_Acc_CreateUserData(DWCAccUserData* userdata, u32 gamecode);
void DWCi_Acc_CreateTempLoginId(DWCAccLoginId* loginid);
BOOL DWCi_Acc_CheckConsoleUserId(const DWCAccLoginId* loginid);
BOOL DWCi_Acc_IsValidLoginId(const DWCAccLoginId* loginid);
BOOL DWCi_Acc_IsAuthentic(const DWCAccUserData* userdata);
BOOL DWC_IsValidFriendData(const DWCFriendData* frienddata);
BOOL DWCi_Acc_IsValidFriendData(const DWCAccFriendData* frienddata);
void DWC_CreateUserData(DWCUserData* userdata);
BOOL DWC_CheckUserData(const DWCUserData* userdata);
BOOL DWC_CheckHasProfile(const DWCUserData* userdata);
void DWCi_Acc_SetLoginIdToUserData(DWCAccUserData* userdata, const DWCAccLoginId* loginid,
                                   int gs_profile_id);
BOOL DWC_CheckDirtyFlag(const DWCUserData* userdata);
void DWC_ClearDirtyFlag(DWCUserData* userdata);
int DWC_GetGsProfileId(const DWCUserData* userdata, const DWCFriendData* frienddata);
u64 DWC_CreateFriendKey(const DWCUserData* userdata);
void DWC_LoginIdToUserName(const DWCUserData* userdata, const DWCAccLoginId* loginid,
                           char* username);
BOOL DWC_IsEqualFriendData(const DWCFriendData* a, const DWCFriendData* b);
void DWC_ReportFriendData(const DWCUserData* userdata, const DWCFriendData* frienddata);
void DWC_ReportUserData(const DWCUserData* userdata);

#ifdef __cplusplus
}
#endif
#endif
