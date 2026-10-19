// NHTTP (RevoEX) OS abstraction layer for RVL: request list mutex and the
// communication thread. Written from the target assembly; names from the Wii Menu
// 4.3 symbol map (koopthekoopa/wii-ipl) and Pokemon Battle Revolution (doldecomp/pbr).
#include <revolution/OS.h>
#include <revolution/nhttp/NHTTPi.h>

#define NHTTP_COMM_THREAD_STACK_SIZE 0x2000  // inferred

void NHTTPi_InitMutexInfo(NHTTPiMutexInfo* info) {
    info->initialized = FALSE;
}

void NHTTPi_initLockReqList(NHTTPiMutexInfo* info) {
    if (!info->initialized) {
        OSInitMutex(&info->mutex);
        info->initialized = TRUE;
    }
}

void NHTTPi_exitLockReqList(NHTTPiMutexInfo* info) {
}

void NHTTPi_lockReqList(NHTTPiMutexInfo* info) {
    OSLockMutex(&info->mutex);
}

void NHTTPi_unlockReqList(NHTTPiMutexInfo* info) {
    OSUnlockMutex(&info->mutex);
}

BOOL NHTTPi_createCommThread(NHTTPiThreadInfo* info, u32 priority, void* stack) {
    if (!NHTTPi_IsCreateCommThreadMessageQueue(info)) {
        OSInitMessageQueue(&info->queue, info->messages, 3);
        NHTTPi_IsCreateCommThreadMessageQueueOn(info);
    }
    OSCreateThread(&info->thread, NHTTPi_CommThreadProc, NULL,
                   (u8*)stack + NHTTP_COMM_THREAD_STACK_SIZE, NHTTP_COMM_THREAD_STACK_SIZE,
                   priority, 0);
    OSResumeThread(&info->thread);
    return TRUE;
}

void NHTTPi_destroyCommThread(NHTTPiThreadInfo* info, NHTTPiReqInfo* reqInfo) {
    reqInfo->isThreadEnd = TRUE;
    OSSendMessage(&info->queue, NULL, 0);
    OSJoinThread(&info->thread, NULL);
}

void NHTTPi_idleCommThread(NHTTPiThreadInfo* info) {
    OSMessage msg;
    OSReceiveMessage(&info->queue, &msg, OS_MSG_BLOCKING);
}

void NHTTPi_kickCommThread(NHTTPiThreadInfo* info) {
    OSSendMessage(&info->queue, NULL, 0);
}

void NHTTPi_CheckCurrentThread(NHTTPiThreadInfo* info, BOOL isCommThread) {
    OSThread* current = OSGetCurrentThread();
    OSThread* commThread = &info->thread;

    if (current != NULL) {
        if ((!isCommThread && current != commThread) || (isCommThread && current == commThread)) {
            OSReport("%s:illegal thread\n", __FUNCTION__);
            OSPanic("NHTTP_os_RVL.c", 223, "halt\n");
        }
    }
}

void* NHTTPi_CommThreadProc(void* param) {
    NHTTPi_CommThreadProcMain();
    return NULL;
}
