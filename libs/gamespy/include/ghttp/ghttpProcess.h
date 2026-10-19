/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/ghttp/ghttpProcess.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GHTTP SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 1999-2007 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef __GHTTPPROCESS_H__
#define __GHTTPPROCESS_H__

#include "ghttp/ghttpConnection.h"
#include "ghttp/ghttpMain.h"

#ifdef __cplusplus
extern "C" {
#endif

void ghiDoSocketInit(GHIConnection* connection);
void ghiDoHostLookup(GHIConnection* connection);
void ghiDoLookupPending(GHIConnection* connection);
void ghiDoConnecting(GHIConnection* connection);
void ghiDoSecuringSession(GHIConnection* connection);
void ghiDoSendingRequest(GHIConnection* connection);
void ghiDoPosting(GHIConnection* connection);
void ghiDoWaiting(GHIConnection* connection);
void ghiDoReceivingStatus(GHIConnection* connection);
void ghiDoReceivingHeaders(GHIConnection* connection);
void ghiDoReceivingFile(GHIConnection* connection);

#ifdef __cplusplus
}
#endif

#endif
