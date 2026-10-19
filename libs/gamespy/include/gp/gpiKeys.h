/* Based on the GameSpy OpenSDK (github.com/nitrocaster/GameSpy, commit d1deb2d1a951cf77933dda040b8d311cc09815a7)
   src/GameSpy/GP/gpiKeys.h, adapted to the version used in this game.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/GP/LICENSE.txt. */

#ifndef _GPIKEYS_H_
#define _GPIKEYS_H_

#include "gp/gpi.h"
#define GPI_INITIAL_NUM_KEYS 1

typedef struct {
  char* keyName;
  char* keyValue;
} GPIKey;

void gpiStatusInfoKeyFree(void* element);
GPResult gpiStatusInfoKeysInit(GPConnection* connection);
void gpiStatusInfoKeysDestroy(GPConnection* connection);
int gpiStatusInfoKeyCompFunc(const void* elem1, const void* elem2);
GPResult gpiStatusInfoAddKey(GPConnection* connection, DArray keys,
                             const char* theKeyName, const char* theKeyValue);
GPResult gpiStatusInfoDelKey(GPConnection* connection, DArray keys,
                             const char* keyName);
GPResult gpiStatusInfoSetKey(GPConnection* connection, DArray keys,
                             const char* keyName, const char* newKeyValue);
GPResult gpiStatusInfoGetKey(GPConnection* connection, DArray keys,
                             const char* keyName, char** keyValue);
GPResult gpiSaveKeysToBuffer(GPConnection* connection, char** buffer);
GPResult gpiStatusInfoCheckKey(GPConnection* connection, DArray keys,
                               const char* keyName, char** keyValue);
#endif
