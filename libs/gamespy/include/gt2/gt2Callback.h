/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/gt2/gt2Callback.h, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GT2 SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 2002 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#ifndef _GT2CALLBACK_H_
#define _GT2CALLBACK_H_

#include "gt2/gt2Main.h"

/*********************
** SOCKET CALLBACKS **
*********************/

GT2Bool gti2SocketErrorCallback(GT2Socket socket);

GT2Bool gti2ConnectAttemptCallback(GT2Socket socket, GT2Connection connection,
                                   unsigned int ip, unsigned short port,
                                   int latency, GT2Byte* message, int len);

/*************************
** CONNECTION CALLBACKS **
*************************/

GT2Bool gti2ConnectedCallback(GT2Connection connection, GT2Result result,
                              GT2Byte* message, int len);

GT2Bool gti2ReceivedCallback(GT2Connection connection, GT2Byte* message,
                             int len, GT2Bool reliable);

GT2Bool gti2ClosedCallback(GT2Connection connection, GT2CloseReason reason);

GT2Bool gti2PingCallback(GT2Connection connection, int latency);

/*********************
** FILTER CALLBACKS **
*********************/

GT2Bool gti2SendFilterCallback(GT2Connection connection, int filterID,
                               const GT2Byte* message, int len,
                               GT2Bool reliable);

GT2Bool gti2ReceiveFilterCallback(GT2Connection connection, int filterID,
                                  GT2Byte* message, int len, GT2Bool reliable);

/*******************
** DUMP CALLBACKS **
*******************/

GT2Bool gti2DumpCallback(GT2Socket socket, GT2Connection connection,
                         unsigned int ip, unsigned short port, GT2Bool reset,
                         const GT2Byte* message, int len, GT2Bool send);

/*****************************
** SOCKET SHARING CALLBACKS **
*****************************/

GT2Bool gti2UnrecognizedMessageCallback(GT2Socket socket, unsigned int ip,
                                        unsigned short port, GT2Byte* message,
                                        int len, GT2Bool* handled);

#endif // _GT2CALLBACK_H_
