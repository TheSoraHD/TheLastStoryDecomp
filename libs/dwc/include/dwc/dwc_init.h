// Based on doldecomp/mkw (CC0 1.0), lib/dwc/common/dwc_init.h.
#ifndef DWC_INIT_H
#define DWC_INIT_H
#include <dwc/dwc_memfunc.h>
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    DWC_AUTHSERVER_DEBUG,
    DWC_AUTHSERVER_RELEASE
} DWCAuthServer;

int DWC_Init(DWCAuthServer authServer, const char* gameName, u32 gameCode, DWCAllocEx alloc,
             DWCFreeEx free);
void DWC_Shutdown(void);
u32 DWCi_GetGamecode(void);

#ifdef __cplusplus
}
#endif
#endif
