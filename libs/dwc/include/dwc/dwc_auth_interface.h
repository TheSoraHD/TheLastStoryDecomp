// Names from Pokemon Battle Revolution (doldecomp/pbr), dwcsec_auth/dwc_auth_interface.s.
#ifndef DWC_AUTH_INTERFACE_H
#define DWC_AUTH_INTERFACE_H
#include <types.h>
#include <stddef.h>
#include <dwc/dwc_memfunc.h>
#ifdef __cplusplus
extern "C" {
#endif

// inferred: allocator given to DWCi_Auth_StartAuthentication (DWC_Alloc is passed)
typedef void* (*DWCAuthAlloc)(DWCAllocType name, u32 size);

typedef struct DWCAuthPrWords DWCAuthPrWords;

BOOL DWCi_Auth_InitInterface(int authServer);
BOOL DWCi_Auth_StartAuthentication(const wchar_t* ingamesn, const char* gsbrcd, u64 userId,
                                   DWCAuthAlloc alloc, DWCFreeEx free);
// inferred: starts a "prof" (profanity check) request; no name in older builds
BOOL fn_806D0B40(const char* words, u32 wordsSize, char region, DWCAuthAlloc alloc,
                 DWCFreeEx free);
int fn_806D0970(void);  // inferred: returns the auth server given to DWCi_Auth_InitInterface
void DWCi_Auth_AbortAuthentication(void);
void DWCi_Auth_ProcessAuthentication(void);
BOOL DWCi_Auth_IsFinished(void);
BOOL DWCi_Auth_IsSucceeded(void);
int DWCi_Auth_GetErrorCode(void);
void DWCi_Auth_GetGameSpyToken(char* token, char* challenge);
u64 DWCi_Auth_GetConsoleUserId(void);
void fn_806D1660(void);
BOOL fn_806D1690(void);
int DWCi_Auth_GetIngamesnCheckResult(void);
DWCAuthPrWords* fn_806D16B0(void);  // inferred: result of the prof request
void DWCi_Auth_SendRequest(int action, const wchar_t* ingamesn, const char* gsbrcd, u64 userId);
void DWCi_Auth_HandleNandResult(int next, int onBusy, int onNoExists, int onAccess);

#ifdef __cplusplus
}
#endif
#endif
