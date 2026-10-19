// GameSpy Stats/Tracking SDK gstats.h. Source: doldecomp/mkw lib/gamespy/gstats/gstats.h
// (GameSpy code, BSD-3, see libs/gamespy/license.txt), reduced to the declarations.
#ifndef _GSTATS_H_
#define _GSTATS_H_

#include "common/gsCommon.h"
#include "gstats/gbucket.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct statsgame_s* statsgame_t;

typedef enum { bo_set, bo_add, bo_sub, bo_mult, bo_div, bo_concat, bo_avg } bucketop_t;
#define NUMOPS 7

typedef enum { bl_server, bl_team, bl_player } bucketlevel_t;

typedef enum {
    init_none,
    init_failed,
    init_connecting,
    init_awaitchallenge,
    init_awaitsessionkey,
    init_complete
} initstate_t;

typedef void* (*BucketFunc)(bucketset_t set, char* name, void* value);
typedef int (*SetIntFunc)(statsgame_t game, char* name, BucketFunc func, int value, int index);
typedef double (*SetFloatFunc)(statsgame_t game, char* name, BucketFunc func, double value,
                               int index);
typedef char* (*SetStringFunc)(statsgame_t game, char* name, BucketFunc func, char* value,
                               int index);

extern BucketFunc bucketfuncs[NUMOPS];
extern void* bopfuncs[][3];

#define GE_NOERROR 0
#define GE_NOSOCKET 1
#define GE_NODNS 2
#define GE_NOCONNECT 3
#define GE_BUSY 4
#define GE_DATAERROR 5
#define GE_CONNECTING 6
#define GE_TIMEDOUT 7

#define SNAP_UPDATE 0
#define SNAP_FINAL 1

extern char gcd_secret_key[256];
extern char gcd_gamename[256];
extern char StatsServerHostname[64];

#define GenerateAuth GenerateAuthA
#define SendGameSnapShot SendGameSnapShotA
#define NewPlayer NewPlayerA
#define NewTeam NewTeamA

int InitStatsConnection(int gameport);
int InitStatsAsync(int gameport, gsi_time theInitTimeout);
int InitStatsThink(void);
int StatsThink(void);
int IsStatsConnected(void);
void CloseStatsConnection(void);
char* GetChallenge(statsgame_t game);
char* GenerateAuth(const char* challenge, const char* password, char response[33]);
statsgame_t NewGame(int usebuckets);
void FreeGame(statsgame_t game);
int SendGameSnapShot(statsgame_t game, const char* snapshot, int final);
void NewPlayer(statsgame_t game, int pnum, char* name);
void RemovePlayer(statsgame_t game, int pnum);
void NewTeam(statsgame_t game, int tnum, char* name);
void RemoveTeam(statsgame_t game, int tnum);
int GetPlayerIndex(statsgame_t game, int pnum);
int GetTeamIndex(statsgame_t game, int tnum);

#ifdef __cplusplus
}
#endif

#endif
