#ifndef DWC_REPORT_H
#define DWC_REPORT_H
#include <types.h>
#ifdef __cplusplus
extern "C" {
#endif

// inferred: report level flags (8 is used for error messages)
#define DWC_REPORTFLAG_ERROR 0x00000008

// inferred: name from the MKW decomp; lives in dwc_report.c (no source yet)
void DWC_Printf(u32 level, const char* fmt, ...);

#ifdef __cplusplus
}
#endif
#endif
