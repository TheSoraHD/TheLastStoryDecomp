/*
Common GameSpy declarations used by the server browsing and NAT negotiation
sources that are not (yet) in the common headers.

Declarations taken from doldecomp/mkw (CC0 1.0) lib/gamespy/common/*.h and
lib/gamespy/qr2/qr2regkeys.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011,
and from the GameSpy OpenSDK (github.com/nitrocaster/GameSpy, commit
d1deb2d1a951cf77933dda040b8d311cc09815a7).
GameSpy SDK code, BSD license (IGN Entertainment), see
libs/gamespy/src/serverbrowsing/LICENSE.txt.
*/

#ifndef _SB_COMMON_DECLS_H_
#define _SB_COMMON_DECLS_H_

#include "common/gsCommon.h"
#include "common/gsAvailable.h"
#include "darray.h"
#include "hashtable.h"
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

/* gsPlatform.h */
typedef gsi_u32 goa_uint32;
#define gsi_char char

/* gsPlatform.h, Revolution */
#ifndef _REVOLUTION
#define _REVOLUTION
#endif
/* raw sockets are undefined on Revolution */
#define SB_NO_ICMP_SUPPORT
#define GS_STATIC_CALLBACK

#undef assert
#define assert(x)

#ifndef _GS_TIME_T_DEFINED
#define _GS_TIME_T_DEFINED
/* MSL time.h (not present in the MSL headers used here) */
typedef long time_t;
#endif

/* gsPlatformUtil.h: time() is gsiTimeInSec() on this platform */
#define time(t) gsiTimeInSec(t)

/* MSL stdlib.h (not declared by the MSL headers used here). inferred (see GP):
   atof() resolves to strtod declared with a single parameter */
double strtod(const char* str);
#define atof(s) strtod(s)

/* inferred (see gt2): the MSL used for GameSpy expands abs() to the __abs intrinsic */
#undef abs
#define abs(x) __abs(x)

/* MSL extras.h */
int stricmp(const char*, const char*);
int strnicmp(const char*, const char*, size_t);
#define _stricmp strcasecmp
#define _strnicmp strnicmp
int strcasecmp(const char*, const char*);
#define strncasecmp strnicmp

/* gsPlatformSocket.h, Revolution */
#define inet_ntoa(in) SOInetNtoA(in)

/* gsPlatform.h */
#define _tcscpy strcpy
#define _tcslen strlen

#define gsi_is_true(x) ((x) != gsi_false)
#define gsi_is_false(x) ((x) == gsi_false)

/* qr2/qr2regkeys.h (owned by the qr2 sources). The target symbols still have
   automatic names: mapped to their current symbols.txt names */
#define MAX_REGISTERED_KEYS 254
#define NUM_RESERVED_KEYS 50
extern const char* qr2_registered_key_list[];
gsi_bool qr2_internal_is_master_only_key(const char* keyname);

#ifdef __cplusplus
}
#endif

#endif
