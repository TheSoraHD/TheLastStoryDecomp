/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/gt2/gt2Utility.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GT2 SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 2002 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef _GT2UTILITY_H_
#define _GT2UTILITY_H_

#include "gt2/gt2Main.h"

void gti2MessageCheck(const GT2Byte** message, int* len);

#ifdef RECV_LOG
void gti2LogMessage(unsigned int fromIP, unsigned short fromPort,
                    unsigned int toIP, unsigned short toPort,
                    const GT2Byte* message, int len);
#endif

#endif // _GT2UTILITY_H_
