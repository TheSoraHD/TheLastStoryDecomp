// DWC ranking library (DWC_Rnk*). Names and enum order follow the public NitroDWC/RVL DWC
// ranking API; values checked against the target code.
#ifndef DWC_RANKING_H
#define DWC_RANKING_H

#include <types.h>
#include <dwc/dwc_account.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DWC_RNK_CATEGORY_MAX 1000
#define DWC_RNK_DATA_MAX 764
#define DWC_RNK_GET_MAX 30
#define DWC_RNK_FRIENDS_MAX 64

typedef enum {
    DWC_RNK_SUCCESS = 0,
    DWC_RNK_IN_ERROR,
    DWC_RNK_ERROR_INVALID_PARAMETER,
    DWC_RNK_ERROR_INIT_ALREADYINITIALIZED,
    DWC_RNK_ERROR_INIT_INVALID_INITDATASIZE,
    DWC_RNK_ERROR_INIT_INVALID_INITDATA,
    DWC_RNK_ERROR_INIT_INVALID_USERDATA,
    DWC_RNK_ERROR_PUT_NOTREADY,
    DWC_RNK_ERROR_PUT_INVALID_KEY,
    DWC_RNK_ERROR_PUT_NOMEMORY,
    DWC_RNK_ERROR_GET_NOTREADY,
    DWC_RNK_ERROR_GET_INVALID_KEY,
    DWC_RNK_ERROR_GET_NOMEMORY,
    DWC_RNK_ERROR_CANCEL_NOTASK,
    DWC_RNK_PROCESS_NOTASK,
    DWC_RNK_PROCESS_TIMEOUT,
    DWC_RNK_ERROR_INVALID_MODE,
    DWC_RNK_ERROR_NOTCOMPLETED,
    DWC_RNK_ERROR_EMPTY_RESPONSE
} DWCRnkError;

typedef enum {
    DWC_RNK_STATE_NOTREADY = 0,
    DWC_RNK_STATE_INITIALIZED,
    DWC_RNK_STATE_PUT_ASYNC,
    DWC_RNK_STATE_GET_ASYNC,
    DWC_RNK_STATE_COMPLETED,
    DWC_RNK_STATE_TIMEOUT,
    DWC_RNK_STATE_ERROR
} DWCRnkState;

typedef u32 DWCRnkRegion;

typedef enum {
    DWC_RNK_GET_MODE_ORDER = 0,
    DWC_RNK_GET_MODE_TOPLIST,
    DWC_RNK_GET_MODE_NEAR,
    DWC_RNK_GET_MODE_FRIENDS,
    DWC_RNK_GET_MODE_NEAR_HI,
    DWC_RNK_GET_MODE_NEAR_LOW
} DWCRnkGetMode;

typedef struct DWCRnkData {
    u32 order;        // 0x00
    s32 pid;          // 0x04
    s32 score;        // 0x08
    DWCRnkRegion region; // 0x0C
    u32 lastupdate;   // 0x10
    u32 size;         // 0x14
    void* userdata;   // 0x18
} DWCRnkData;

// inferred: "size" is the size of the used option member; the third word of order/toplist/near
// is not read by this library
typedef struct DWCRnkGetParam {
    u32 size;  // 0x00
    union {
        u32 data;
        struct {
            u32 sort;
            u32 since;
        } order;
        struct {
            u32 sort;
            u32 limit;
            u32 since;
        } toplist;
        struct {
            u32 sort;
            u32 limit;
            u32 since;
        } near;
        struct {
            u32 sort;
            u32 limit;
            s32 friends[DWC_RNK_FRIENDS_MAX + 1];
        } friends;
    } option;  // 0x04
} DWCRnkGetParam;

DWCRnkError DWC_RnkInitialize(const char* initdata, const DWCUserData* userdata);
DWCRnkError DWC_RnkShutdown(void);
DWCRnkError DWC_RnkPutScoreAsync(u32 category, DWCRnkRegion region, s32 score, void* data,
                                 u32 size);
DWCRnkError DWC_RnkGetScoreAsync(DWCRnkGetMode mode, u32 category, DWCRnkRegion region,
                                 DWCRnkGetParam* param);
DWCRnkError DWC_RnkCancelProcess(void);
DWCRnkError DWC_RnkProcess(void);
DWCRnkState DWC_RnkGetState(void);
DWCRnkError DWC_RnkResGetRow(DWCRnkData* out, u32 index);
DWCRnkError DWC_RnkResGetRowCount(u32* count);

#ifdef __cplusplus
}
#endif

#endif
