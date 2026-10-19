/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/gt2/gt2Filter.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GT2 SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 2002 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef _GT2FILTER_H_
#define _GT2FILTER_H_

#include "gt2/gt2Main.h"

GT2Bool gti2AddSendFilter(GT2Connection connection,
                          gt2SendFilterCallback callback);
void gti2RemoveSendFilter(GT2Connection connection,
                          gt2SendFilterCallback callback);
GT2Bool gti2FilteredSend(GT2Connection connection, int filterID,
                         const GT2Byte* message, int len, GT2Bool reliable);

GT2Bool gti2AddReceiveFilter(GT2Connection connection,
                             gt2ReceiveFilterCallback callback);
void gti2RemoveReceiveFilter(GT2Connection connection,
                             gt2ReceiveFilterCallback callback);
GT2Bool gti2FilteredReceive(GT2Connection connection, int filterID,
                            GT2Byte* message, int len, GT2Bool reliable);

#endif // _GT2FILTER_H_
