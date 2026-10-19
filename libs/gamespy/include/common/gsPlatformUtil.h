// GameSpy SDK utilities (see libs/gamespy/license.txt).
#ifndef __GSPLATFORMUTIL_H__
#define __GSPLATFORMUTIL_H__

#include "common/gsPlatform.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct GSIResolveHostnameInfo* GSIResolveHostnameHandle;
#define GSI_STILL_RESOLVING_HOSTNAME 0
#define GSI_ERROR_RESOLVING_HOSTNAME 0xFFFFFFFF

int gsiStartResolvingHostname(const char* hostname, GSIResolveHostnameHandle* handle);
void gsiCancelResolvingHostname(GSIResolveHostnameHandle handle);
unsigned int gsiGetResolvedIP(GSIResolveHostnameHandle handle);

long gsiTimeInSec(long* timer);
char* goastrdup(const char* src);
char* _strlwr(char* string);
void SocketStartUp(void);
void SocketShutDown(void);
gsi_time current_time(void);
void msleep(gsi_time msec);
void Util_RandSeed(unsigned long seed);
int Util_RandInt(int low, int high);
void B64Decode(const char* input, char* output, int inlen, int* outlen, int encodingType);
void B64Encode(const char* input, char* output, int inlen, int encodingType);
gsi_u32 gsiSafeStrcpyA(char* dst, const char* src, gsi_u32 size);

#ifdef __cplusplus
}
#endif

#endif
