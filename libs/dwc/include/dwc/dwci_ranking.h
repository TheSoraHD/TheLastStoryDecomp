// Internal ranking requests (dwci_ranking.c). Names inferred.
#ifndef DWCI_RANKING_H
#define DWCI_RANKING_H

#include <types.h>
#include <dwc/dwc_ranking.h>
#include <dwc/dwci_encsession.h>

#ifdef __cplusplus
extern "C" {
#endif

// inferred: server passed to DWCi_RankingInitialize (DWC_RnkInitialize passes 1 when the auth
// server is 1)
typedef enum { DWCi_RNK_SERVER_DEBUG = 0, DWCi_RNK_SERVER_RELEASE = 1 } DWCiRnkServer;

void DWCi_RankingInitialize(DWCiRnkServer server, const char* initdata);
DWCiEncState DWCi_RankingProcess(void);
void DWCi_RankingCancel(void);
void DWCi_RankingShutdown(void);
const char* DWCi_RankingGetResponse(int* size);
DWCiEncResult DWCi_RankingPutScoreAsync(u32 category, int pid, DWCRnkRegion region, s32 score,
                                        void* data, u32 size);
DWCiEncResult DWCi_RankingGetScoreAsync(DWCRnkGetMode mode, u32 category, int pid,
                                        DWCRnkRegion region, DWCRnkGetParam* param);

#ifdef __cplusplus
}
#endif

#endif
