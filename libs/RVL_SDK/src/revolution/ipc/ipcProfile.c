#include <revolution/IPC.h>
#include <revolution/OS.h>
#include <string.h>

// Aug 2010 SDK version (The Last Story): the requests are copied with their path and time stamp.
// Written from the target assembly (0x8061DD20-0x8061E12C); the Feb 2009 version (Xenoblade) only
// kept the request pointers and descriptors. Array names inferred.

#define IPC_PROF_REQ_MAX 96
#define IPC_PROF_FD_MAX 128
#define IPC_PROF_PATH_LEN 48

static s32 IpcNumUnIssuedReqs;
static s32 IpcNumPendingReqs;

static IPCRequest IpcReqArray[IPC_PROF_REQ_MAX];
static IPCRequestEx* IpcReqPtrArray[IPC_PROF_REQ_MAX];
static s64 IpcReqTimeArray[IPC_PROF_REQ_MAX];
static char IpcFdPathArray[IPC_PROF_FD_MAX][IPC_PROF_PATH_LEN];
static char IpcReqPathArray[IPC_PROF_REQ_MAX][IPC_PROF_PATH_LEN];

static void AddReqInfo(IPCRequestEx* req);
static void DelReqInfo(IPCRequestEx* req);

// unused (stripped by the linker); its references give the order of the arrays in .bss
void IPCGetQueueStatus(IPCRequest** reqs, IPCRequestEx*** ptrs, s64** times, char** fdPaths,
                       char** reqPaths) {
    *reqs = IpcReqArray;
    *ptrs = IpcReqPtrArray;
    *times = IpcReqTimeArray;
    *fdPaths = IpcFdPathArray[0];
    *reqPaths = IpcReqPathArray[0];
}

void IPCiProfInit(void) {
    int i;

    IpcNumPendingReqs = 0;
    IpcNumUnIssuedReqs = 0;

    for (i = 0; i < IPC_PROF_REQ_MAX; i++) {
        IpcReqPtrArray[i] = NULL;
        IpcReqTimeArray[i] = 0;
    }

    memset(IpcFdPathArray, 0, sizeof(IpcFdPathArray));
    memset(IpcReqPathArray, 0, sizeof(IpcReqPathArray));
    memset(IpcReqArray, 0, sizeof(IpcReqArray));
}

void IPCiProfQueueReq(IPCRequestEx* req, s32 fd) {
    IpcNumPendingReqs++;
    IpcNumUnIssuedReqs++;
    AddReqInfo(req);
}

void IPCiProfAck(void) {
    IpcNumUnIssuedReqs--;
}

void IPCiProfReply(IPCRequestEx* req, s32 fd) {
    IpcNumPendingReqs--;
    DelReqInfo(req);
}

// NONMATCHING: 99.65% - &IpcReqArray[i] and the base of IpcReqPathArray swap r25/r27
static void AddReqInfo(IPCRequestEx* req) {
    u32 i;
    BOOL enabled;

    for (i = 0; i < IPC_PROF_REQ_MAX; i++) {
        if (IpcReqPtrArray[i] == NULL) {
            enabled = OSDisableInterrupts();

            IpcReqPtrArray[i] = req;
            IpcReqArray[i] = req->base;
            IpcReqTimeArray[i] = OSGetTime();

            if ((u32)IpcReqArray[i].type == IPC_REQ_OPEN) {
                char* path = IpcReqPathArray[i];
                strncpy(path, OSPhysicalToCached((u32)IpcReqArray[i].open.path), 47);
                IpcReqPathArray[i][47] = '\0';
                IpcReqArray[i].open.path = path;
            }

            OSRestoreInterrupts(enabled);
            return;
        }
    }
}

static void DelReqInfo(IPCRequestEx* req) {
    u32 i;
    BOOL enabled;

    for (i = 0; i < IPC_PROF_REQ_MAX; i++) {
        if (req == IpcReqPtrArray[i] && (u32)req->base.fd == (u32)IpcReqArray[i].type) {
            enabled = OSDisableInterrupts();

            if ((u32)IpcReqArray[i].type == IPC_REQ_OPEN && req->base.ret >= 0) {
                strncpy(IpcFdPathArray[req->base.ret], IpcReqArray[i].open.path,
                        IPC_PROF_PATH_LEN - 1);
                IpcFdPathArray[req->base.ret][IPC_PROF_PATH_LEN - 1] = '\0';
                memset(IpcReqPathArray[i], 0, IPC_PROF_PATH_LEN);
            }

            if ((u32)IpcReqArray[i].type == IPC_REQ_CLOSE) {
                memset(IpcFdPathArray[IpcReqArray[i].fd], 0, IPC_PROF_PATH_LEN);
            }

            IpcReqPtrArray[i] = NULL;
            memset(&IpcReqArray[i], 0, sizeof(IPCRequest));
            IpcReqTimeArray[i] = 0;

            OSRestoreInterrupts(enabled);
            return;
        }
    }
}
