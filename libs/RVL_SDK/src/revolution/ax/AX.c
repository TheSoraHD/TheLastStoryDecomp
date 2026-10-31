#include <revolution/AX.h>
#include <revolution/OS.h>

const char* __AXVersion =
    "<< RVL_SDK - AX \trelease build: Aug 23 2010 17:29:51 (0x4302_145) >>";

static BOOL __init = FALSE;

void AXInit(void) {
    AXInitEx(0);
}

void AXInitEx(u32 mode) {
    if (!__init) {
        OSRegisterVersion(__AXVersion);

        __AXAllocInit();
        __AXVPBInit();
        __AXSPBInit();
        __AXAuxInit();
        __AXClInit();
        __AXOutInit(mode);

        __init = TRUE;
    }
}

//unused
void AXInitSpecifyMem(){
}

//unused
void AXInitExSpecifyMem(){
}

void AXQuit(){
    if(__init){
        __AXOutQuit();
        __AXAllocQuit();
        __AXVPBQuit();
        __AXSPBQuit();
        __AXAuxQuit();
        __AXClQuit();
        
        __init = FALSE;
    }
}

BOOL AXIsInit(){
    return __init;
}
