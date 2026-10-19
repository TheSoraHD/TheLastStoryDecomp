// GameSpy SDK threads, Revolution flavour (see libs/gamespy/license.txt).
#ifndef __GSPLATFORMTHREAD_H__
#define __GSPLATFORMTHREAD_H__

#include "common/gsPlatform.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    OSThread mThread; // 0x000
    void* mStack;     // 0x318
} GSIThreadID;
typedef OSMutex GSICriticalSection;
typedef void (*GSThreadFunc)(void* arg);

int gsiStartThread(GSThreadFunc aThreadFunc, gsi_u32 theStackSize, void* arg,
                   GSIThreadID* theThreadIdOut);
void gsiCancelThread(GSIThreadID theThreadID);
void gsiCleanupThread(GSIThreadID theThreadID);
void gsiInitializeCriticalSection(GSICriticalSection* theCrit);
void gsiEnterCriticalSection(GSICriticalSection* theCrit);
void gsiLeaveCriticalSection(GSICriticalSection* theCrit);

#ifdef __cplusplus
}
#endif

#endif
