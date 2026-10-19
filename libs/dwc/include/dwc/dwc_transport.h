#ifndef DWC_TRANSPORT_H
#define DWC_TRANSPORT_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// inferred: callback types from the NitroDWC/RVL DWC API (argument order from the call sites)
typedef void (*DWCUserSendCallback)(int size, u8 aid, void* param);
typedef void (*DWCUserRecvCallback)(u8 aid, u8* buffer, int size, void* param);
typedef void (*DWCUserRecvTimeoutCallback)(u8 aid, void* param);
typedef void (*DWCUserPingCallback)(int latency, u8 aid, void* param);

#define DWC_MAX_CONNECTIONS 32 // inferred: the per-aid table is 0x800 bytes of 0x40 entries

// inferred: per-aid transport state (0x40 bytes); names follow the NitroDWC transport
typedef struct DWCTransportConnection {
    const u8* sendBuffer;     // 0x00
    u8* recvBuffer;           // 0x04
    int recvBufferSize;       // 0x08
    int sendingSize;          // 0x0C
    int recvingSize;          // 0x10
    int requestSendSize;      // 0x14
    int requestRecvSize;      // 0x18
    s8 sendState;             // 0x1C
    s8 recvState;             // 0x1D
    s8 lastRecvState;         // 0x1E
    u8 pad1F;                 // 0x1F
    u16 pad20;                // 0x20
    u16 lastRecvType;         // 0x22
    u32 pad24;                // 0x24
    s64 previousRecvTick;     // 0x28 recv timeout reference
    u32 recvTimeoutTime;      // 0x30 [ms]
    u32 pad34;                // 0x34
    s64 lastRecvTick;         // 0x38 keep-alive reference
} DWCTransportConnection;

// inferred: transport control block, given by the caller of DWCi_InitTransport (0x828 bytes)
typedef struct DWCTransportInfo {
    DWCTransportConnection connections[DWC_MAX_CONNECTIONS]; // 0x000
    DWCUserSendCallback sendCallback;                        // 0x800
    DWCUserRecvCallback recvCallback;                        // 0x804
    DWCUserRecvTimeoutCallback recvTimeoutCallback;          // 0x808
    DWCUserPingCallback pingCallback;                        // 0x80C
    void* sendParam;                                         // 0x810
    void* recvParam;                                         // 0x814
    void* recvTimeoutParam;                                  // 0x818
    void* pingParam;                                         // 0x81C
    u16 sendSplitMax;                                        // 0x820
    u32 connectionTimeout;                                   // 0x824 [ms] inferred: name
} DWCTransportInfo;

BOOL DWC_IsSendableReliable(u8 aid);
BOOL DWC_SendReliable(u8 aid, const void* buffer, int size);
BOOL DWC_SendUnreliable(u8 aid, const void* buffer, int size);
BOOL DWC_SetRecvBuffer(u8 aid, void* recvBuffer, int size);
BOOL DWC_Ping(u8 aid);
BOOL DWC_SetUserSendCallback(DWCUserSendCallback callback, void* param);
BOOL DWC_SetUserRecvCallback(DWCUserRecvCallback callback, void* param);
BOOL DWC_SetUserRecvTimeoutCallback(DWCUserRecvTimeoutCallback callback, void* param);
BOOL DWC_SetUserPingCallback(DWCUserPingCallback callback, void* param);
BOOL DWC_SetRecvTimeoutTime(u8 aid, u32 time);

void DWCi_InitTransport(DWCTransportInfo* info);
void DWCi_ShutdownTransport(void);
void DWCi_TransportProcess(void);
void DWCi_ClearTransConnection(u8 aid);

#ifdef __cplusplus
}
#endif
#endif
