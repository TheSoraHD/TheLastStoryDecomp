// Based on doldecomp/mkw (CC0 1.0), lib/dwc/common/dwc_init.c (commit
// 8b9b5ee653da28663e498039e52f5d19f929a011). The Last Story links a later DWC build
// (Aug 27 2010): the auth interface result is checked and the init flag lives in this file.
#include <dwc/dwc_init.h>
#include <dwc/dwc_auth_interface.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <string.h>

// inferred: gsiMemoryCallbacksSet (GameSpy, MKW name)
void gsiMemoryCallbacksSet(void* mallocFn, void* freeFn, void* reallocFn, void* memalignFn);
// inferred: gethostbyname (GameSpy socket layer, MKW name)
void* gethostbyname(const char* name);
// inferred: DWCi_CfReset (MKW name)
void fn_806CF030(void);
// inferred: reads the console friend code (DWCi_Np_GetConsoleId in MKW)
void fn_806AA690(void);

// inferred: gcd_gamename (GameSpy) and the stats server host name buffer
extern char gcd_gamename[];
extern char StatsServerHostname[];

static u32 dwci_gamecode;
// inferred: library initialized flag (DWCi_Np_SetInitFlag in MKW)
static int dwci_initialized;

int DWC_Init(DWCAuthServer authServer, const char* gameName, u32 gameCode, DWCAllocEx alloc,
             DWCFreeEx free) {
    OSRegisterVersion("<< RVL_SDK - DWC \trelease build: Aug 27 2010 19:01:14 (0x4302_172) >>");

    DWCi_SetMemFunc(alloc, free);

    if (DWCi_Auth_InitInterface(authServer) == 0) {
        DWC_Printf(DWC_REPORTFLAG_WARNING, "Failed to initialize auth interface.\n");
        return -1;
    }

    dwci_gamecode = gameCode;

    gsiMemoryCallbacksSet(DWCi_GsMalloc, DWCi_GsFree, DWCi_GsRealloc, DWCi_GsMemalign);

    strcpy(gcd_gamename, gameName);

    if (authServer == DWC_AUTHSERVER_DEBUG) {
        strcpy(StatsServerHostname, "sdkdev.gamespy.com");
    } else {
        strcpy(StatsServerHostname, "gamestats.gs.nintendowifi.net");
    }

    dwci_initialized = TRUE;
    fn_806AA690();

    return 0;
}

void DWC_Shutdown(void) {
    gsiMemoryCallbacksSet(DWCi_GsMalloc, DWCi_GsFree, DWCi_GsRealloc, DWCi_GsMemalign);
    gethostbyname("clear");
    fn_806CF030();
    dwci_initialized = FALSE;
}

u32 DWCi_GetGamecode(void) {
    return dwci_gamecode;
}
