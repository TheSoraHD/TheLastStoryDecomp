// GameSpy SDK availability check (see libs/gamespy/license.txt).
#ifndef __GSAVAILABLE_H__
#define __GSAVAILABLE_H__

#include "common/gsCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GSIACWaiting,
    GSIACAvailable,
    GSIACUnavailable,
    GSIACTemporarilyUnavailable
} GSIACResult;

extern GSIACResult __GSIACResult;
extern char __GSIACGamename[64];
extern char GSIACHostname[64];

void GSIStartAvailableCheckA(const char* gamename);
GSIACResult GSIAvailableCheckThink(void);

#ifdef __cplusplus
}
#endif

#endif
