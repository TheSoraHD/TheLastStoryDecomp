// Based on doldecomp/mkw (CC0 1.0), lib/dwc/common/dwc_error.h and dwci_error.h.
#ifndef DWC_ERROR_H
#define DWC_ERROR_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DWC_ERROR_NONE = 0,
    DWC_ERROR_DS_MEMORY_ANY = 1, // inferred: names 1-8 from the NitroDWC API
    DWC_ERROR_AUTH_ANY = 2,
    DWC_ERROR_AUTH_OUT_OF_SERVICE = 3,
    DWC_ERROR_AUTH_STOP_SERVICE = 4,
    DWC_ERROR_AC_ANY = 5,
    DWC_ERROR_NETWORK = 6,
    DWC_ERROR_GHTTP_ANY = 7,
    DWC_ERROR_DISCONNECTED = 8,
    DWC_ERROR_FATAL = 9
} DWCError;

// inferred: values follow the DWC_GetLastErrorEx switch of this DWC build
typedef enum {
    DWC_ETYPE_NO_ERROR = 0,
    DWC_ETYPE_LIGHT = 1,
    DWC_ETYPE_SHOW_ERROR = 2,
    DWC_ETYPE_SHUTDOWN_FM = 3,
    DWC_ETYPE_SHUTDOWN_GHTTP = 4,
    DWC_ETYPE_SHUTDOWN_ND = 5,
    DWC_ETYPE_DISCONNECT = 6,
    DWC_ETYPE_FATAL = 7,
    DWC_ETYPE_UNKNOWN_8 = 8
} DWCErrorType;

int DWC_GetLastError(int* errorCode);
int DWC_GetLastErrorEx(int* errorCode, DWCErrorType* errorType);
void DWC_ClearError(void);
BOOL DWCi_IsError(void);
void DWCi_SetError(DWCError error, int errorCode);

#ifdef __cplusplus
}
#endif
#endif
