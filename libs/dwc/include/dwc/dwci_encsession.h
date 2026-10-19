// Encrypted gamestats2 session used by the DWC ranking library.
// Rewritten from the target assembly; names inferred (no symbols for this module).
#ifndef DWCI_ENCSESSION_H
#define DWCI_ENCSESSION_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

// inferred: server selection (index into the three base URLs)
typedef enum {
    DWC_ENC_SERVER_RELEASE = 0,  // http://gamestats2.gs.nintendowifi.net/
    DWC_ENC_SERVER_DEBUG = 1,    // http://sdkdev.gamespy.com/games/
    DWC_ENC_SERVER_TEST = 2      // http://ishikawa.servebeer.com/games/
} DWCEncServer;

// inferred: session states (DWCi_EncSessionProcess returns the current one)
typedef enum {
    DWCi_ENC_STATE_CANCELED = 0,
    DWCi_ENC_STATE_ERROR = 1,
    DWCi_ENC_STATE_NOT_INITIALIZED = 2,
    DWCi_ENC_STATE_INITIALIZED = 3,
    DWCi_ENC_STATE_REQUEST_TOKEN = 4,
    DWCi_ENC_STATE_GET_TOKEN = 5,
    DWCi_ENC_STATE_REQUEST_DATA = 6,
    DWCi_ENC_STATE_GET_DATA = 7,
    DWCi_ENC_STATE_COMPLETED = 8
} DWCiEncState;

// inferred: DWCi_EncSessionGetAsync results
typedef enum {
    DWCi_ENC_RESULT_SUCCESS = 0,
    DWCi_ENC_RESULT_ERROR = 1,
    DWCi_ENC_RESULT_OUT_OF_MEMORY = 2,
    DWCi_ENC_RESULT_NOT_INITIALIZED = 3
} DWCiEncResult;

// inferred: called with the verified response; returning FALSE fails the session
typedef BOOL (*DWCiEncSessionCallback)(const char* data, int size);

void DWCi_EncSessionInitialize(DWCEncServer server, const char* initdata);
DWCiEncState DWCi_EncSessionProcess(void);
void DWCi_EncSessionCancel(void);
void DWCi_EncSessionShutdown(void);
const char* DWCi_EncSessionGetResponse(int* size);
DWCiEncResult DWCi_EncSessionGetAsyncEx(const char* url, int pid, const void* data, int size,
                                        DWCiEncSessionCallback callback, BOOL useLastHash);
DWCiEncResult DWCi_EncSessionGetAsync(const char* url, int pid, const void* data, int size,
                                      DWCiEncSessionCallback callback);

#ifdef __cplusplus
}
#endif

#endif
