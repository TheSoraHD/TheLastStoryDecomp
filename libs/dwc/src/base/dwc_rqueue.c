// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Reliable send queue used by dwc_transport.c.
// inferred: file name; the range 0x806CD540-0x806CF02C holds three translation units
// (own .data/.bss each, calls between them never inlined).
#include <dwc/dwc_transport.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <revolution/OS.h>

#include <string.h>

// inferred: GameSpy types (gt2.h)
typedef void* GT2Connection;
typedef int GT2Result;
typedef int GT2Bool;
typedef GT2Result (*DWCGT2SendFunc)(GT2Connection connection, const void* message, int len,
                                    GT2Bool reliable);

// inferred: GameSpy entry points; names from the GameSpy SDK, no source yet
GT2Result gt2Send(GT2Connection connection, const void* message, int len,
                      GT2Bool reliable);
void gt2Ping(GT2Connection connection);
int gt2GetConnectionState(GT2Connection connection);
int gt2GetIncomingBufferFreeSpace(GT2Connection connection);
int gt2GetOutgoingBufferFreeSpace(GT2Connection connection);

// inferred: DWC matching functions (other units); names from older DWC builds where known
void fn_806A9CA0(void* dst, const void* src, u32 size); // DWCi_Np_CPUCopy
u8 fn_806B0E30(void);                                // DWC_GetMyAID
int fn_806B0EB0(u8** aidList);                       // DWC_GetAIDList
BOOL fn_806B1170(u8 aid);                            // DWC_IsValidAID
BOOL fn_806B12F0(void);                              // inferred: TRUE on the match server
GT2Connection fn_806B14D0(u8 aid);                   // DWCi_GetGT2Connection
u8 fn_806B1570(GT2Connection connection);            // DWCi_GetConnectionAID
BOOL fn_806B1730(u8 aid);                            // inferred: aid has a GT2 connection
void fn_806B1A00(u8 aid);                            // inferred: closes the connection to aid
u8 fn_806B30A0(void);                                // inferred: aid of the server, 0xFF if none
void fn_806B97B0(u8 aid, u16 type, const u8* message); // inferred: system message handler
void fn_806C9CE0(const u8* message, u32 size);       // inferred: unreliable match command
BOOL fn_806CB4C0(void);
void fn_806CB8F0(int reason);
BOOL fn_806CCB40(void);
int fn_806CCCB0(void);                               // inferred: number of hosts
// inferred: connection info of the server (only the aid at 0x16 is read here)
typedef struct DWCConnectionInfo {
    u8 pad[0x16];
    u8 aid; // 0x16
} DWCConnectionInfo;
DWCConnectionInfo* fn_806CCED0(int index);

#define DWC_ERROR_NETWORK ((DWCError)6) // inferred: NitroDWC value

// inferred: header in front of reliable data (little endian fields, as on the DS)
typedef struct DWCTransportHeader {
    int size;      // 0x00
    u16 type;      // 0x04
    char magic[2]; // 0x06 "DT"
} DWCTransportHeader;

// inferred: header in front of unreliable data relayed by the server
typedef struct DWCRelayHeader {
    char magic[3]; // 0x00
    u8 aid;        // 0x03 sender
    u32 aidBitmap; // 0x04 receivers
} DWCRelayHeader;

// inferred: GT2 reliable messages that could not be sent yet
typedef struct DWCReliableQueueEntry {
    GT2Connection connection;            // 0x00
    u8* data;                            // 0x04
    int size;                            // 0x08
    struct DWCReliableQueueEntry* next;  // 0x0C
} DWCReliableQueueEntry;

typedef struct DWCReliableQueue {
    DWCReliableQueueEntry* head; // 0x00
    int totalSize;               // 0x04
} DWCReliableQueue;

#define DWC_TRANSPORT_TYPE_DATA 1 // inferred

// inferred: send/recv states
enum {
    DWC_SEND_STATE_READY = 0,
    DWC_SEND_STATE_BUSY = 1
};

enum {
    DWC_RECV_STATE_NOBUF = 0,
    DWC_RECV_STATE_HEADER = 1,
    DWC_RECV_STATE_BODY = 2,
    DWC_RECV_STATE_SYSTEM = 3,
    DWC_RECV_STATE_ERROR = 4
};

// inferred: little endian conversion of the header fields (the DS byte order)
#define SWAP16(x) ((u16)(((u16)(x) >> 8) | ((u16)(x) << 8)))
static inline u32 SWAP32(u32 x) {
    x = ((x >> 8) & 0x00FF00FF) | ((x << 8) & 0xFF00FF00);
    return (x << 16) | (x >> 16);
}

// inferred: the reliable queue, the "DT" transport and DWCs_RecvDataBody each have their own
// .data and .bss in the target (see the notes at the parts below)
void fn_806CD540(void);
void fn_806CD560(void);
BOOL fn_806CD5E0(void);
BOOL fn_806CD610(void);
BOOL DWCi_ProcessReliableQueue(DWCGT2SendFunc sendFunc);
BOOL fn_806CD740(GT2Connection connection, const void* data, int size);
void DWCs_RecvDataBody(u8 aid, const u8* message, int len);

extern DWCTransportInfo* stpTransInfo;


static DWCReliableQueue sReliableQueue; // inferred: name

void fn_806CD540(void) {
    sReliableQueue.head = NULL;
    sReliableQueue.totalSize = 0;
}

void fn_806CD560(void) {
    DWCReliableQueueEntry* entry;

    entry = sReliableQueue.head;
    while (entry != NULL) {
        sReliableQueue.head = entry->next;
        DWC_Free(DWC_ALLOCTYPE_BASE, entry->data, entry->size);
        DWC_Free(DWC_ALLOCTYPE_BASE, entry, 0);
        entry = sReliableQueue.head;
    }

    fn_806CD540();
}

BOOL fn_806CD5E0(void) {
    return sReliableQueue.totalSize < 0x6000;
}

BOOL fn_806CD610(void) {
    return DWCi_ProcessReliableQueue(gt2Send);
}

BOOL DWCi_ProcessReliableQueue(DWCGT2SendFunc sendFunc) {
    DWCReliableQueueEntry* entry;
    int retry;

    for (entry = sReliableQueue.head; entry != NULL; entry = entry->next) {
        for (retry = 0; retry < 3; retry++) {
            if (sendFunc(entry->connection, entry->data, entry->size, TRUE) == 0) {
                sReliableQueue.totalSize -= entry->size;
                DWC_Free(DWC_ALLOCTYPE_BASE, entry->data, entry->size);
                entry->data = NULL;
                entry->size = 0;
                entry->connection = NULL;
                break;
            }
        }

        if (retry == 3) {
            DWC_Printf(DWC_REPORTFLAG_TRANSPORT,
                       "DWCi_ProcessReliableQueue() retry count exceeded!\n");
            break;
        }
    }

    entry = sReliableQueue.head;
    while (entry != NULL) {
        if (entry->data != NULL) {
            break;
        }
        sReliableQueue.head = entry->next;
        DWC_Free(DWC_ALLOCTYPE_BASE, entry, 0);
        entry = sReliableQueue.head;
    }

    return -(int)entry == 0; // inferred: the target computes it from -entry (neg; cntlzw)
}

BOOL fn_806CD740(GT2Connection connection, const void* data, int size) {
    DWCReliableQueueEntry** pEntry = &sReliableQueue.head;
    DWCReliableQueueEntry* entry;

    if (sReliableQueue.totalSize + size > 0x8000) {
        return FALSE;
    }

    for (entry = sReliableQueue.head; entry != NULL; entry = entry->next) {
        pEntry = &entry->next;
    }

    *pEntry = DWC_Alloc(DWC_ALLOCTYPE_BASE, sizeof(DWCReliableQueueEntry));
    if (*pEntry == NULL) {
        return FALSE;
    }

    (*pEntry)->data = DWC_Alloc(DWC_ALLOCTYPE_BASE, size);
    if ((*pEntry)->data == NULL) {
        DWC_Free(DWC_ALLOCTYPE_BASE, *pEntry, sizeof(DWCReliableQueueEntry));
        *pEntry = NULL;
        return FALSE;
    }

    (*pEntry)->connection = connection;
    (*pEntry)->size = size;
    (*pEntry)->next = NULL;
    fn_806A9CA0((*pEntry)->data, data, size);
    sReliableQueue.totalSize += size;
    return TRUE;
}
