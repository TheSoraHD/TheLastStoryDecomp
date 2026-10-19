// Small compatibility layer for the GHTTP and QR2 sources: SDK macros that the
// reduced common headers do not provide (gsi_char helpers, Revolution socket
// error names, assert/min/max). All definitions are guarded.
// GameSpy SDK, BSD license (IGN Entertainment), see libs/gamespy/license.txt.
#ifndef __GSCOMPAT_H__
#define __GSCOMPAT_H__

#include "common/gsCommon.h"
#include <revolution/SO.h>

// Narrow-char build: the SDK's gsi_char/_T string helpers (normally in gsPlatform.h).
#ifndef gsi_char
#define gsi_char char
#endif
#ifndef _T
#define _T(a) a
#endif
#ifndef _tsnprintf
#define _tsnprintf snprintf
#define _tcscpy strcpy
#define _tcsncpy strncpy
#define _tcscat strcat
#define _tcslen strlen
#define _tfopen fopen
#endif
// No file system on this target (MKW builds ghttp the same way).
#ifndef NOFILE
#define NOFILE
#endif
// gsDebug.h categories/levels (the debug macros discard them in this build).
#ifndef GSIDebugLevel_HotError
typedef gsi_u8 GSIDebugLevel;
#define GSIDebugLevel_HotError (GSIDebugLevel)(1 << 0)
#define GSIDebugLevel_WarmError (GSIDebugLevel)(1 << 1)
#define GSIDebugLevel_Warning (GSIDebugLevel)(1 << 2)
#define GSIDebugLevel_Notice (GSIDebugLevel)(1 << 3)
#define GSIDebugLevel_Comment (GSIDebugLevel)(1 << 4)
#define GSIDebugLevel_RawDump (GSIDebugLevel)(1 << 5)
#define GSIDebugLevel_StackTrace (GSIDebugLevel)(1 << 6)
typedef enum {
  GSIDebugType_Network,
  GSIDebugType_File,
  GSIDebugType_Memory,
  GSIDebugType_State,
  GSIDebugType_Misc,
  GSIDebugType_Count
} GSIDebugType;
typedef enum {
  GSIDebugCat_App,
  GSIDebugCat_GP,
  GSIDebugCat_Peer,
  GSIDebugCat_QR2,
  GSIDebugCat_SB,
  GSIDebugCat_Voice,
  GSIDebugCat_AD,
  GSIDebugCat_NatNeg,
  GSIDebugCat_HTTP,
  GSIDebugCat_CDKey,
  GSIDebugCat_Common,
  GSIDebugCat_Count
} GSIDebugCategory;
#endif
#ifndef gsDebugBinary
#define gsDebugBinary(cat, type, level, ...) ((void)(__VA_ARGS__))
#endif
#ifndef GS_FAIL
#define GS_FAIL()
#endif
#ifndef EOF
#define EOF (-1)
#endif
// Socket error names (gsPlatformSocket.h, Revolution flavour).
#ifndef WSAEWOULDBLOCK
#define WSAEWOULDBLOCK SO_EWOULDBLOCK
#define WSAEINPROGRESS SO_EINPROGRESS
#define WSAEALREADY SO_EALREADY
#define WSAENOTCONN SO_ENOTCONN
#define WSAEISCONN SO_EISCONN
#define WSAETIMEDOUT SO_ETIMEDOUT
#endif
#ifndef inet_ntoa
#define inet_ntoa(n) SOInetNtoA(n)
#endif
#ifndef gsi_is_false
#define gsi_is_false(x) ((x) == gsi_false)
#define gsi_is_true(x) ((x) != gsi_false)
#endif
#ifndef assert
#define assert(x) ((void)0)
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif

#endif
