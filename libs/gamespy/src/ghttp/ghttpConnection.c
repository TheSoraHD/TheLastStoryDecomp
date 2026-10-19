/* Based on doldecomp/mkw (CC0 1.0) lib/gamespy/ghttp/ghttpConnection.c, commit 8b9b5ee653da28663e498039e52f5d19f929a011.
   Adapted to the newer GHTTP version in this game (changes marked "inferred" come from the
   target code; ghiSendBufferedData/ghiDoSend/ghiTrySendThenBuffer follow the GameSpy OpenSDK,
   nitrocaster/GameSpy commit d1deb2d1).
   GameSpy SDK code, BSD license (IGN Entertainment), see libs/gamespy/src/gt2/LICENSE.txt. */

/*
GameSpy GHTTP SDK
Dan "Mr. Pants" Schoenblum
dan@gamespy.com

Copyright 1999-2007 GameSpy Industries, Inc

devsupport@gamespy.com
*/

#include "ghttp/ghttpConnection.h"
#include "ghttp/ghttpCommon.h"

// Initial size and increment amount for the connections array.
///////////////////////////////////////////////////////////////
#define CONNECTIONS_CHUNK_LEN 4

// An array of pointers to GHIConnection objects.
// A GHTTPRequest is an index into this array.
/////////////////////////////////////////////////
static GHIConnection** ghiConnections;
static int ghiConnectionsLen;
static int ghiNumConnections;
static int ghiNextUniqueID;

// Finds a gsifree slot in the ghiConnections array.
// If there are no gsifree slots, the array size will be increased.
////////////////////////////////////////////////////////////////
static int ghiFindFreeSlot(void) {
  int i;
  GHIConnection** tempPtr;
  int oldLen;
  int newLen;

  // Look for an open slot.
  /////////////////////////
  for (i = 0; i < ghiConnectionsLen; i++) {
    if (!ghiConnections[i]->inUse)
      return i;
  }

  assert(ghiNumConnections == ghiConnectionsLen);

  // Nothing found, resize the array.
  ///////////////////////////////////
  oldLen = ghiConnectionsLen;
  newLen = (ghiConnectionsLen + CONNECTIONS_CHUNK_LEN);
  tempPtr = (GHIConnection**)gsirealloc(ghiConnections,
                                        sizeof(GHIConnection*) * newLen);
  if (!tempPtr)
    return -1;
  ghiConnections = tempPtr;

  // Create the new connection objects.
  /////////////////////////////////////
  for (i = oldLen; i < newLen; i++) {
    ghiConnections[i] = (GHIConnection*)gsimalloc(sizeof(GHIConnection));
    if (!ghiConnections[i]) {
      for (i--; i >= oldLen; i--)
        gsifree(ghiConnections[i]);
      return -1;
    }
    ghiConnections[i]->inUse = GHTTPFalse;
  }

  // Update the length.
  /////////////////////
  ghiConnectionsLen = newLen;

  return oldLen;
}

GHIConnection* ghiNewConnection(void) {
  int slot;
  GHIConnection* connection;
  GHTTPBool bResult;

  ghiLock();

  // Find a gsifree slot.
  ////////////////////
  slot = ghiFindFreeSlot();
  if (slot == -1) {
    ghiUnlock();
    return NULL;
  }

  // Get a pointer to the object.
  ///////////////////////////////
  connection = ghiConnections[slot];

  // Init the object.
  ///////////////////
  memset(connection, 0, sizeof(GHIConnection));
  connection->inUse = GHTTPTrue;
  connection->request = (GHTTPRequest)slot;
  connection->uniqueID = ghiNextUniqueID++;
  connection->type = GHIGET;
  connection->state = GHTTPSocketInit;
  connection->URL = NULL;
  connection->serverAddress = NULL;
  connection->serverIP = INADDR_ANY;
  connection->serverPort = (unsigned short)0;
  connection->requestPath = NULL;
  connection->sendHeaders = NULL;
  connection->saveFile = NULL;
  connection->blocking = GHTTPFalse;
  connection->persistConnection = GHTTPFalse;
  connection->result = GHTTPSuccess;
  connection->progressCallback = NULL;
  connection->completedCallback = NULL;
  connection->callbackParam = NULL;
  connection->socket = INVALID_SOCKET;
  connection->socketError = 0;
  connection->userBufferSupplied = GHTTPFalse;
  connection->statusMajorVersion = 0;
  connection->statusMinorVersion = 0;
  connection->statusCode = 0;
  connection->statusStringIndex = 0;
  connection->headerStringIndex = 0;
  connection->completed = GHTTPFalse;
  connection->fileBytesReceived = 0;
  connection->totalSize = -1;
  connection->redirectURL = NULL;
  connection->redirectCount = 0;
  connection->chunkedTransfer = GHTTPFalse;
  connection->processing = GHTTPFalse;
  connection->throttle = GHTTPFalse;
  connection->lastThrottleRecv = 0;
  connection->post = NULL;
  connection->maxRecvTime = 500; // Prevent blocking in async mode with systems
                                 // that never generate WSAEWOULDBLOCK
  connection->proxyOverridePort = GHI_DEFAULT_PORT;
  connection->proxyOverrideServer = NULL;
  connection->encryptor.mInterface = NULL;
  connection->encryptor.mInitialized = GHTTPFalse;
  connection->unk1CC = 0;

// handle used for asynch DNS lookups
#if !defined(GSI_NO_THREADS)
  connection->handle = NULL;
#endif

  bResult = ghiInitBuffer(connection, &connection->sendBuffer,
                          SEND_BUFFER_INITIAL_SIZE, SEND_BUFFER_INCREMENT_SIZE);
  if (bResult)
    bResult =
        ghiInitBuffer(connection, &connection->encodeBuffer,
                      ENCODE_BUFFER_INITIAL_SIZE, ENCODE_BUFFER_INCREMENT_SIZE);
  if (bResult)
    bResult =
        ghiInitBuffer(connection, &connection->recvBuffer,
                      RECV_BUFFER_INITIAL_SIZE, RECV_BUFFER_INCREMENT_SIZE);
  if (bResult)
    bResult =
        ghiInitBuffer(connection, &connection->decodeBuffer,
                      DECODE_BUFFER_INITIAL_SIZE, DECODE_BUFFER_INCREMENT_SIZE);

  if (!bResult) {
    ghiFreeConnection(connection);
    ghiUnlock();
    return NULL;
  }

  // One more connection.
  ///////////////////////
  ghiNumConnections++;

  ghiUnlock();

  return connection;
}

// inferred: the newer SDK splits ghiFreeConnection into a close step and a
// free step (both called from the stripped request-cancel path as well).
GHTTPBool ghiCloseConnection(GHIConnection* connection) {
  if (!connection)
    return GHTTPFalse;
  if (!connection->inUse)
    return GHTTPFalse;
  if (connection->request < 0)
    return GHTTPFalse;
  if (connection->request >= ghiConnectionsLen)
    return GHTTPFalse;

  ghiLock();

  // Cancel the asychronous lookup.
  /////////////////////////////////
  if (connection->handle) {
    gsiCancelResolvingHostname(connection->handle);
    connection->handle = NULL;
  }

  // Close the socket.
  ////////////////////
  if (connection->socket != INVALID_SOCKET) {
    shutdown(connection->socket, 2);
    closesocket(connection->socket);
    connection->socket = INVALID_SOCKET;
  }

  ghiUnlock();

  return GHTTPTrue;
}

GHTTPBool ghiFreeConnectionData(GHIConnection* connection) {
  if (!connection)
    return GHTTPFalse;
  if (!connection->inUse)
    return GHTTPFalse;
  if (connection->request < 0)
    return GHTTPFalse;
  if (connection->request >= ghiConnectionsLen)
    return GHTTPFalse;

  ghiLock();

  // Free the various strings.
  ////////////////////////////
  gsifree(connection->URL);
  connection->URL = NULL;
  gsifree(connection->unk34);
  connection->unk34 = NULL;
  gsifree(connection->serverAddress);
  connection->serverAddress = NULL;
  gsifree(connection->requestPath);
  connection->requestPath = NULL;
  gsifree(connection->sendHeaders);
  connection->sendHeaders = NULL;
  gsifree(connection->redirectURL);
  connection->sendHeaders = NULL; // sic (the original clears sendHeaders again)
  gsifree(connection->proxyOverrideServer);
  connection->proxyOverrideServer = NULL;

  // Free the buffers.
  ////////////////////
  ghiFreeBuffer(&connection->sendBuffer);
  ghiFreeBuffer(&connection->encodeBuffer);
  ghiFreeBuffer(&connection->recvBuffer);
  ghiFreeBuffer(&connection->decodeBuffer);
  ghiFreeBuffer(&connection->getFileBuffer);

  // Free the posting state.
  //////////////////////////
  if (connection->postingState.states)
    ghiPostCleanupState(connection);

  // Check for an encryptor.
  //////////////////////////
  if (connection->encryptor.mInitialized != GHTTPFalse) {
    if (connection->encryptor.mCleanupFunc)
      (connection->encryptor.mCleanupFunc)(connection, &connection->encryptor);
    connection->encryptor.mInitialized = GHTTPFalse;
  }

  connection->unk1CC = 0;

  // Free the slot.
  /////////////////
  connection->inUse = GHTTPFalse;

  // One less connection.
  ///////////////////////
  ghiNumConnections--;

  ghiUnlock();

  return GHTTPTrue;
}

GHTTPBool ghiFreeConnection(GHIConnection* connection) {
  if (!connection)
    return GHTTPFalse;
  if (!connection->inUse)
    return GHTTPFalse;
  if (connection->request < 0)
    return GHTTPFalse;
  if (connection->request >= ghiConnectionsLen)
    return GHTTPFalse;

  if (ghiCloseConnection(connection) != GHTTPTrue)
    return GHTTPFalse;

  return ghiFreeConnectionData(connection);
}

GHIConnection* ghiRequestToConnection(GHTTPRequest request) {
  GHIConnection* connection;

  assert(request >= 0);
  assert(request < ghiConnectionsLen);

  ghiLock();

  // Check args.
  //////////////
  if ((request < 0) || (request >= ghiConnectionsLen)) {
    ghiUnlock();
    return NULL;
  }

  connection = ghiConnections[request];

  // Check for not in use.
  ////////////////////////
  if (!connection->inUse)
    connection = NULL;

  ghiUnlock();

  return connection;
}

void ghiEnumConnections(GHTTPBool (*callback)(GHIConnection*)) {
  int i;

  // Check for no connections.
  ////////////////////////////
  if (ghiNumConnections <= 0)
    return;

  ghiLock();
  for (i = 0; i < ghiConnectionsLen; i++)
    if (ghiConnections[i]->inUse)
      callback(ghiConnections[i]);
  ghiUnlock();
}

void ghiRedirectConnection(GHIConnection* connection) {
  assert(connection);
  assert(connection->redirectURL);

  gsDebugFormat(GSIDebugCat_HTTP, GSIDebugType_State, GSIDebugLevel_Comment,
                "Redirecting Connection\n");

  // Cancel the asychronous lookup if it has not already been done
  /////////////////////////////////////////////////////////////////
  if (connection->handle) {
    gsiCancelResolvingHostname(connection->handle);
    connection->handle = NULL;
  }

  // Close the socket.
  ////////////////////
  shutdown(connection->socket, 2);
  closesocket(connection->socket);

  // New URL.
  ///////////
  gsifree(connection->URL);
  connection->URL = connection->redirectURL;
  connection->redirectURL = NULL;

  // Reset stuff parsed from the URL.
  ///////////////////////////////////
  gsifree(connection->serverAddress);
  connection->serverAddress = NULL;
  connection->serverIP = 0;
  connection->serverPort = 0;
  gsifree(connection->requestPath);
  connection->requestPath = NULL;

  // Reset state.
  ///////////////
  connection->state = GHTTPSocketInit;
  connection->socket = INVALID_SOCKET;

  // Reset buffers.
  /////////////////
  ghiResetBuffer(&connection->sendBuffer);
  ghiResetBuffer(&connection->encodeBuffer);
  ghiResetBuffer(&connection->recvBuffer);
  ghiResetBuffer(&connection->decodeBuffer);

  // Reset status.
  ////////////////
  connection->statusMajorVersion = 0;
  connection->statusMinorVersion = 0;
  connection->statusCode = 0;
  connection->statusStringIndex = 0;

  connection->headerStringIndex = 0;

  // The connection isn't closed.
  ///////////////////////////////
  connection->connectionClosed = GHTTPFalse;

  // Check for an encryptor
  if (connection->encryptor.mInitialized != GHTTPFalse) {
    // cleanup the encryptor
    if (connection->encryptor.mCleanupFunc)
      (connection->encryptor.mCleanupFunc)(connection, &connection->encryptor);
    connection->encryptor.mInitialized = GHTTPFalse;

    // if the redirect isn't secure, clear it
    if (strncmp("https://", connection->URL, 8) != 0) {
      connection->encryptor.mEngine = GHTTPEncryptionEngine_None;
      connection->encryptor.mInterface = NULL;
    }
  }

  // One more redirect.
  /////////////////////
  connection->redirectCount++;
}

void ghiCleanupConnections(void) {
  int i;

  if (!ghiConnections)
    return;

  // Cleanup all running connections.
  ///////////////////////////////////
  ghiEnumConnections(ghiFreeConnection);

  // Cleanup the connection states.
  /////////////////////////////////
  for (i = 0; i < ghiConnectionsLen; i++)
    gsifree(ghiConnections[i]);
  gsifree(ghiConnections);
  ghiConnections = NULL;
  ghiConnectionsLen = 0;
  ghiNumConnections = 0;
}
