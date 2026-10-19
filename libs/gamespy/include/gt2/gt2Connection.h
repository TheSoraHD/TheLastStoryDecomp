/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/gt2/gt2Connection.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GT2 SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 2002 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef _GT2CONNECTION_H_
#define _GT2CONNECTION_H_

#include "gt2/gt2Main.h"

GT2Result gti2NewOutgoingConnection(GT2Socket socket, GT2Connection* connection,
                                    unsigned int ip, unsigned short port);
GT2Result gti2NewIncomingConnection(GT2Socket socket, GT2Connection* connection,
                                    unsigned int ip, unsigned short port);

GT2Result gti2StartConnectionAttempt(GT2Connection connection,
                                     const GT2Byte* message, int len,
                                     GT2ConnectionCallbacks* callbacks);

GT2Bool gti2AcceptConnection(GT2Connection connection,
                             GT2ConnectionCallbacks* callbacks);

void gti2RejectConnection(GT2Connection connection, const GT2Byte* message,
                          int len);

GT2Bool gti2ConnectionSendData(GT2Connection connection, const GT2Byte* message,
                               int len);

GT2Bool gti2ConnectionThink(GT2Connection connection, gsi_time now);

void gti2CloseConnection(GT2Connection connection, GT2Bool hard);

void gti2ConnectionClosed(GT2Connection connection);

void gti2ConnectionCleanup(GT2Connection connection);

#endif // _GT2CONNECTION_H_
