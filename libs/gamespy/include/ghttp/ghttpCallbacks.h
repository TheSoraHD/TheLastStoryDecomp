/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/ghttp/ghttpCallbacks.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GHTTP SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 1999-2007 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef __GHTTPCALLBACKS_H__
#define __GHTTPCALLBACKS_H__

// clang-format off
#include "ghttp/ghttpMain.h"
#include "ghttp/ghttpConnection.h"
// clang-format on

#ifdef __cplusplus
extern "C" {
#endif

// Call the completed callback for this connection.
///////////////////////////////////////////////////
void ghiCallCompletedCallback(GHIConnection* connection);

// Call the progress callback for this connection.
//////////////////////////////////////////////////
void ghiCallProgressCallback(GHIConnection* connection, const char* buffer,
                             GHTTPByteCount bufferLen);

// Call the post callback for this connection.
//////////////////////////////////////////////
void ghiCallPostCallback(GHIConnection* connection);

#ifdef __cplusplus
}
#endif

#endif
