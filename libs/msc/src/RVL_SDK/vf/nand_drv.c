// Source: mscharged-decomp src/RVL_SDK/vf/nand_drv.c (CC0).
// TLS: only the two globals written by VFSysInit survive; every function and l_nandFunc are deadstripped.
#include <revolution/types.h>

s32 VF_nand_sleep_msec;
s32 VF_nand_retry_max;
