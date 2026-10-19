/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/gt2/gt2Auth.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GT2 SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 2002 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef _GT2AUTH_H_
#define _GT2AUTH_H_

#define GTI2_CHALLENGE_LEN 32
#define GTI2_RESPONSE_LEN 32

#ifdef __cplusplus
extern "C" {
#endif

GT2Byte* gti2GetChallenge(GT2Byte* buffer);

GT2Byte* gti2GetResponse(GT2Byte* buffer, const GT2Byte* challenge);

GT2Bool gti2CheckResponse(const GT2Byte* response1, const GT2Byte* response2);

#ifdef __cplusplus
}
#endif

#endif // _GT2AUTH_H_
