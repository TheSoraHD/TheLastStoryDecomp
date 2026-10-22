// Rewritten from the target assembly (DWC release build Aug 27 2010).
// Ranking requests sent through the encrypted gamestats2 session. No symbols exist for this
// module: function names are inferred from the public DWC_Rnk* API that calls them.
#include <dwc/dwc_memfunc.h>
#include <dwc/dwc_report.h>
#include <dwc/dwci_encsession.h>
#include <dwc/dwci_ranking.h>

#include <string.h>

// the server expects little-endian words
#define DWCi_RNK_SWAP32(x) \
    ((((x) & 0xFF) << 24) | (((x) & 0xFF00) << 8) | (((x) & 0xFF0000) >> 8) | \
     (((x) & 0xFF000000) >> 24))

static BOOL DWCi_RankingGetCallback(const char* data, int size);

void DWCi_RankingInitialize(DWCiRnkServer server, const char* initdata) {
    DWCi_EncSessionInitialize(server == DWCi_RNK_SERVER_DEBUG ? DWC_ENC_SERVER_DEBUG
                                                              : DWC_ENC_SERVER_RELEASE,
                              initdata);
}

DWCiEncState DWCi_RankingProcess(void) {
    return DWCi_EncSessionProcess();
}

void DWCi_RankingCancel(void) {
    DWCi_EncSessionCancel();
}

void DWCi_RankingShutdown(void) {
    DWCi_EncSessionShutdown();
}

const char* DWCi_RankingGetResponse(int* size) {
    return DWCi_EncSessionGetResponse(size);
}

DWCiEncResult DWCi_RankingPutScoreAsync(u32 category, int pid, DWCRnkRegion region, s32 score,
                                        void* data, u32 size) {
    u32 header[4];
    DWCiEncResult ret;
    u8* buf;

    header[0] = DWCi_RNK_SWAP32(region);
    header[1] = DWCi_RNK_SWAP32(category);
    header[2] = DWCi_RNK_SWAP32((u32)score);
    header[3] = DWCi_RNK_SWAP32(size);

    buf = DWC_Alloc(DWC_ALLOCTYPE_RANKING, size + sizeof(header));
    if (buf == NULL) {
        return DWCi_ENC_RESULT_OUT_OF_MEMORY;
    }
    memcpy(buf, header, sizeof(header));
    memcpy(buf + sizeof(header), data, size);
    ret = DWCi_EncSessionGetAsync("/web/client/put2.asp", pid, buf, size + sizeof(header), NULL);
    DWC_Free(DWC_ALLOCTYPE_RANKING, buf, 0);
    return ret;
}

DWCiEncResult DWCi_RankingGetScoreAsync(DWCRnkGetMode mode, u32 category, int pid,
                                        DWCRnkRegion region, DWCRnkGetParam* param) {
    u32 header[4];
    DWCRnkGetParam p;
    u32* words;
    u32 i;
    DWCiEncResult ret;
    u8* buf;

    p = *param;
    header[0] = DWCi_RNK_SWAP32(region);
    header[1] = DWCi_RNK_SWAP32(category);
    header[2] = DWCi_RNK_SWAP32((u32)mode);
    header[3] = DWCi_RNK_SWAP32(p.size);

    words = &p.option.data;
    for (i = 0; i < p.size / 4; i++) {
        words[i] = DWCi_RNK_SWAP32(words[i]);
    }

    buf = DWC_Alloc(DWC_ALLOCTYPE_RANKING, p.size + sizeof(header));
    if (buf == NULL) {
        return DWCi_ENC_RESULT_OUT_OF_MEMORY;
    }
    memcpy(buf, header, sizeof(header));
    memcpy(buf + sizeof(header), &p.option, p.size);
    ret = DWCi_EncSessionGetAsync("/web/client/get2.asp", pid, buf, p.size + sizeof(header),
                                  DWCi_RankingGetCallback);
    DWC_Free(DWC_ALLOCTYPE_RANKING, buf, 0);
    return ret;
}

// inferred: converts the response to big-endian in place:
// u32 mode, u32 count, u32 total, then count rows of 6 words (order, pid, score, region,
// lastupdate, size) each followed by size bytes of user data
static BOOL DWCi_RankingGetCallback(const char* data, int size) {
    u32* p = (u32*)data;
    const char* end;
    u32 idx;
    u32 i;

    if (size > 0) {
    p[0] = DWCi_RNK_SWAP32(p[0]);
    switch (p[0]) {
    case DWC_RNK_GET_MODE_ORDER:
        p[1] = DWCi_RNK_SWAP32(p[1]);
        p[2] = DWCi_RNK_SWAP32(p[2]);
        break;
    case DWC_RNK_GET_MODE_TOPLIST:
    case DWC_RNK_GET_MODE_NEAR:
    case DWC_RNK_GET_MODE_FRIENDS:
    case DWC_RNK_GET_MODE_NEAR_HI:
    case DWC_RNK_GET_MODE_NEAR_LOW:
        end = data + size;
        p[1] = DWCi_RNK_SWAP32(p[1]);
        p[2] = DWCi_RNK_SWAP32(p[2]);
        idx = 3;
        for (i = 0; i < p[1]; i++) {
            if ((const char*)&p[idx + 5] > end) {
                DWC_Printf(DWC_REPORTFLAG_ERROR, "memory access over\n");
                return FALSE;
            }
            p[idx + 0] = DWCi_RNK_SWAP32(p[idx + 0]);
            p[idx + 1] = DWCi_RNK_SWAP32(p[idx + 1]);
            p[idx + 2] = DWCi_RNK_SWAP32(p[idx + 2]);
            p[idx + 3] = DWCi_RNK_SWAP32(p[idx + 3]);
            p[idx + 4] = DWCi_RNK_SWAP32(p[idx + 4]);
            idx += 6 + (p[idx + 5] = DWCi_RNK_SWAP32(p[idx + 5])) / 4;
        }
        break;
    default:
        DWC_Printf(DWC_REPORTFLAG_ERROR, "Invalid RNK_GET mode : %d\n", p[0]);
        return FALSE;
    }
    }
    return TRUE;
}
