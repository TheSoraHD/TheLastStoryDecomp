// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Public ranking API (DWC_Rnk*): names from the NitroDWC/RVL DWC ranking API, matched to the
// target by its error codes; no symbols exist for this module.
#include <dwc/dwc_account.h>
#include <dwc/dwc_auth_interface.h>
#include <dwc/dwc_error.h>
#include <dwc/dwc_ranking.h>
#include <dwc/dwc_report.h>
#include <dwc/dwci_ranking.h>
#include <revolution/OS.h>

#define DWC_RNK_TIMEOUT_MSEC 30000  // inferred

// inferred: header of a get2.asp response (see DWCi_RankingGetCallback)
typedef struct DWCiRnkResHeader {
    int size;    // 0x00
    u32 unk04;   // 0x04 inferred: not used here
    u32 mode;    // 0x08
    u32* body;   // 0x0C inferred: count, total, rows
} DWCiRnkResHeader;

// inferred: ranking control block
typedef struct DWCiRnkControl {
    DWCRnkState state;  // 0x00
    int pid;            // 0x04
} DWCiRnkControl;

static DWCiRnkControl sRnkCnt;
static s64 sRnkStartTime;

DWCRnkError DWC_RnkInitialize(const char* initdata, const DWCUserData* userdata) {
    int pid;

    if (!DWC_CheckUserData(userdata) || !DWC_CheckHasProfile(userdata)) {
        return DWC_RNK_ERROR_INIT_INVALID_USERDATA;
    }
    pid = userdata->gs_profile_id;
    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    if (sRnkCnt.state != DWC_RNK_STATE_NOTREADY) {
        return DWC_RNK_ERROR_INIT_ALREADYINITIALIZED;
    }
    sRnkCnt.pid = pid;
    if (fn_806D0970() == 1) {
        DWCi_RankingInitialize(DWCi_RNK_SERVER_RELEASE, initdata);
    } else {
        DWCi_RankingInitialize(DWCi_RNK_SERVER_DEBUG, initdata);
    }
    sRnkCnt.state = DWC_RNK_STATE_INITIALIZED;
    return DWC_RNK_SUCCESS;
}

DWCRnkError DWC_RnkShutdown(void) {
    DWCi_RankingShutdown();
    sRnkCnt.state = DWC_RNK_STATE_NOTREADY;
    return DWC_RNK_SUCCESS;
}

DWCRnkError DWC_RnkPutScoreAsync(u32 category, DWCRnkRegion region, s32 score, void* data,
                                 u32 size) {
    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    if (sRnkCnt.state != DWC_RNK_STATE_INITIALIZED && sRnkCnt.state != DWC_RNK_STATE_COMPLETED) {
        return DWC_RNK_ERROR_PUT_NOTREADY;
    }
    if (category > DWC_RNK_CATEGORY_MAX || size > DWC_RNK_DATA_MAX) {
        return DWC_RNK_ERROR_INVALID_PARAMETER;
    }
    if (data == NULL && size != 0) {
        return DWC_RNK_ERROR_INVALID_PARAMETER;
    }
    switch (DWCi_RankingPutScoreAsync(category, sRnkCnt.pid, region, score, data, size)) {
    case DWCi_ENC_RESULT_NOT_INITIALIZED:
        return DWC_RNK_ERROR_PUT_INVALID_KEY;
    case DWCi_ENC_RESULT_OUT_OF_MEMORY:
        return DWC_RNK_ERROR_PUT_NOMEMORY;
    }
    sRnkCnt.state = DWC_RNK_STATE_PUT_ASYNC;
    sRnkStartTime = OSGetTime();
    DWC_Printf(DWC_REPORTFLAG_INFO, "Timeout counter has been reset.\n");
    return DWC_RNK_SUCCESS;
}

DWCRnkError DWC_RnkGetScoreAsync(DWCRnkGetMode mode, u32 category, DWCRnkRegion region,
                                 DWCRnkGetParam* param) {
    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    if (sRnkCnt.state != DWC_RNK_STATE_INITIALIZED && sRnkCnt.state != DWC_RNK_STATE_COMPLETED) {
        return DWC_RNK_ERROR_GET_NOTREADY;
    }
    if (category > DWC_RNK_CATEGORY_MAX || param == NULL) {
        return DWC_RNK_ERROR_INVALID_PARAMETER;
    }
    switch (mode) {
    case DWC_RNK_GET_MODE_ORDER:
        if (param->size != sizeof(param->option.order)) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        if (param->option.order.sort > 1) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        break;
    case DWC_RNK_GET_MODE_TOPLIST:
        if (param->size != sizeof(param->option.toplist)) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        if (param->option.toplist.sort > 1) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        if (param->option.toplist.limit > DWC_RNK_GET_MAX || param->option.toplist.limit == 0) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        break;
    case DWC_RNK_GET_MODE_NEAR:
    case DWC_RNK_GET_MODE_NEAR_HI:
    case DWC_RNK_GET_MODE_NEAR_LOW:
        if (param->size != sizeof(param->option.near)) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        if (param->option.near.sort > 1) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        if (param->option.near.limit < 2 || param->option.near.limit > DWC_RNK_GET_MAX) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        break;
    case DWC_RNK_GET_MODE_FRIENDS:
        if (param->size != sizeof(param->option.friends)) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        if (param->option.friends.sort > 1) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        if (param->option.friends.limit < 2 ||
            param->option.friends.limit > DWC_RNK_FRIENDS_MAX + 1) {
            return DWC_RNK_ERROR_INVALID_PARAMETER;
        }
        break;
    }
    switch (DWCi_RankingGetScoreAsync(mode, category, sRnkCnt.pid, region, param)) {
    case DWCi_ENC_RESULT_NOT_INITIALIZED:
        return DWC_RNK_ERROR_GET_INVALID_KEY;
    case DWCi_ENC_RESULT_OUT_OF_MEMORY:
        return DWC_RNK_ERROR_GET_NOMEMORY;
    }
    sRnkCnt.state = DWC_RNK_STATE_GET_ASYNC;
    sRnkStartTime = OSGetTime();
    DWC_Printf(DWC_REPORTFLAG_INFO, "Timeout counter has been reset.\n");
    return DWC_RNK_SUCCESS;
}

DWCRnkError DWC_RnkCancelProcess(void) {
    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    if (sRnkCnt.state != DWC_RNK_STATE_PUT_ASYNC && sRnkCnt.state != DWC_RNK_STATE_GET_ASYNC) {
        return DWC_RNK_ERROR_CANCEL_NOTASK;
    }
    DWCi_RankingCancel();
    sRnkCnt.state = DWC_RNK_STATE_ERROR;
    return DWC_RNK_SUCCESS;
}

DWCRnkError DWC_RnkProcess(void) {
    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    if (sRnkCnt.state == DWC_RNK_STATE_TIMEOUT) {
        return DWC_RNK_PROCESS_TIMEOUT;
    }
    if (sRnkCnt.state != DWC_RNK_STATE_PUT_ASYNC && sRnkCnt.state != DWC_RNK_STATE_GET_ASYNC) {
        return DWC_RNK_PROCESS_NOTASK;
    }
    switch (DWCi_RankingProcess()) {
    case DWCi_ENC_STATE_ERROR:
        sRnkCnt.state = DWC_RNK_STATE_ERROR;
        break;
    case DWCi_ENC_STATE_CANCELED:
        sRnkCnt.state = DWC_RNK_STATE_ERROR;
        break;
    case DWCi_ENC_STATE_REQUEST_TOKEN:
    case DWCi_ENC_STATE_GET_TOKEN:
    case DWCi_ENC_STATE_REQUEST_DATA:
    case DWCi_ENC_STATE_GET_DATA:
        if (OS_TICKS_TO_MSEC(OSGetTime() - sRnkStartTime) > DWC_RNK_TIMEOUT_MSEC) {
            DWC_Printf(DWC_REPORTFLAG_INFO, "Timeout!!");
            DWCi_RankingCancel();
            sRnkCnt.state = DWC_RNK_STATE_TIMEOUT;
            DWCi_SetError(DWC_ERROR_GHTTP_ANY, -98070);
            return DWC_RNK_PROCESS_TIMEOUT;
        }
        break;
    case DWCi_ENC_STATE_COMPLETED:
        sRnkCnt.state = DWC_RNK_STATE_COMPLETED;
        break;
    }
    return DWC_RNK_SUCCESS;
}

DWCRnkState DWC_RnkGetState(void) {
    return sRnkCnt.state;
}

static DWCRnkError DWCi_RnkResGetHeader(DWCiRnkResHeader* header) {
    const char* data;

    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    if (sRnkCnt.state != DWC_RNK_STATE_COMPLETED) {
        return DWC_RNK_ERROR_NOTCOMPLETED;
    }
    data = DWCi_RankingGetResponse(&header->size);
    if (header->size == 0) {
        return DWC_RNK_ERROR_EMPTY_RESPONSE;
    }
    header->mode = *(const u32*)data;
    header->body = (u32*)(data + 4);
    return DWC_RNK_SUCCESS;
}

// NONMATCHING: ~94% - the target keeps header.size in a register from the check in
// DWCi_RnkResGetHeader and loads it into r5 there; tried several bound expressions
DWCRnkError DWC_RnkResGetRow(DWCRnkData* out, u32 index) {
    DWCiRnkResHeader header;
    DWCRnkError err;
    u8* row;

    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    err = DWCi_RnkResGetHeader(&header);
    if (err != DWC_RNK_SUCCESS) {
        return err;
    }
    if (header.mode == DWC_RNK_GET_MODE_ORDER) {
        return DWC_RNK_ERROR_INVALID_MODE;
    }
    if (out == NULL) {
        return DWC_RNK_ERROR_INVALID_PARAMETER;
    }
    if (index >= header.body[0]) {
        return DWC_RNK_ERROR_INVALID_PARAMETER;
    }
    row = (u8*)&header.body[2];
    while (index-- != 0) {
        row = row + ((DWCRnkData*)row)->size + 0x18;
    }
    if (row + ((DWCRnkData*)row)->size + 0x18 > header.size + (u8*)header.body) {
        return DWC_RNK_ERROR_INVALID_PARAMETER;
    }
    *out = *(DWCRnkData*)row;
    out->userdata = row + 0x18;
    return DWC_RNK_SUCCESS;
}

DWCRnkError DWC_RnkResGetRowCount(u32* count) {
    DWCiRnkResHeader header;
    DWCRnkError err;

    if (DWCi_IsError()) {
        return DWC_RNK_IN_ERROR;
    }
    err = DWCi_RnkResGetHeader(&header);
    if (err != DWC_RNK_SUCCESS) {
        return err;
    }
    if (count == NULL) {
        return DWC_RNK_ERROR_INVALID_PARAMETER;
    }
    if (header.mode == DWC_RNK_GET_MODE_ORDER) {
        return DWC_RNK_ERROR_INVALID_MODE;
    }
    *count = header.body[0];
    return DWC_RNK_SUCCESS;
}
