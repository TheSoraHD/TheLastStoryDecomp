/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/gt2/gt2Buffer.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GT2 SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 2002 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef _GT2BUFFER_H_
#define _GT2BUFFER_H_

#include "gt2/gt2Main.h"

GT2Bool gti2AllocateBuffer(GTI2Buffer* buffer, int size);

int gti2GetBufferFreeSpace(const GTI2Buffer* buffer);

void gti2BufferWriteByte(GTI2Buffer* buffer, GT2Byte b);
void gti2BufferWriteUShort(GTI2Buffer* buffer, unsigned short s);
void gti2BufferWriteData(GTI2Buffer* buffer, const GT2Byte* data, int len);

// shortens the buffer by "shortenBy" (length, not size)
void gti2BufferShorten(GTI2Buffer* buffer, int start, int shortenBy);

#endif // _GT2BUFFER_H_
