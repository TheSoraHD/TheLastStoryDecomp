// Based on doldecomp/mkw (CC0 1.0), lib/dwc/common/dwc_error.c (commit
// 8b9b5ee653da28663e498039e52f5d19f929a011). The Last Story links a later DWC build
// (Aug 27 2010); the error-type table was rewritten from the target assembly.
#include <dwc/dwc_error.h>

static DWCError stDwcLastError;
static int stDwcErrorCode;

int DWC_GetLastError(int* errorCode) {
    if (errorCode != NULL) {
        *errorCode = stDwcErrorCode;
    }
    return stDwcLastError;
}

int DWC_GetLastErrorEx(int* errorCode, DWCErrorType* errorType) {
    if (errorCode != NULL) {
        *errorCode = stDwcErrorCode;
    }

    if (errorType != NULL) {
        switch (stDwcLastError) {
        case 2:
        case 3:
        case 4:
        case 5:
        case 8:
        case 14:
        case 16:
        case 18:
        case 21:
            *errorType = DWC_ETYPE_DISCONNECT;
            break;
        case 6:
            *errorType = DWC_ETYPE_SHUTDOWN_FM;
            break;
        case 7:
            *errorType = DWC_ETYPE_SHUTDOWN_GHTTP;
            break;
        case 13:
            *errorType = DWC_ETYPE_SHUTDOWN_ND;
            break;
        case 22:
            *errorType = DWC_ETYPE_UNKNOWN_8;
            break;
        case 10:
        case 11:
        case 12:
        case 24:
            *errorType = DWC_ETYPE_LIGHT;
            break;
        case 15:
        case 17:
        case 19:
        case 20:
        case 23:
        case 25:
            *errorType = DWC_ETYPE_SHOW_ERROR;
            break;
        case 1:
        case DWC_ERROR_FATAL:
            *errorType = DWC_ETYPE_FATAL;
            break;
        default:
            *errorType = DWC_ETYPE_NO_ERROR;
            break;
        }
    }

    return stDwcLastError;
}

void DWC_ClearError(void) {
    if (stDwcLastError != DWC_ERROR_FATAL) {
        stDwcLastError = DWC_ERROR_NONE;
        stDwcErrorCode = 0;
    }
}

BOOL DWCi_IsError(void) {
    return stDwcLastError != DWC_ERROR_NONE;
}

void DWCi_SetError(DWCError error, int errorCode) {
    if (stDwcLastError != DWC_ERROR_FATAL) {
        stDwcLastError = error;
        stDwcErrorCode = errorCode;
    }
}
