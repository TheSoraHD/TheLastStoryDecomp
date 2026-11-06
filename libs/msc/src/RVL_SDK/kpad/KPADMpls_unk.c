// inferred: Motion Plus part of the TLS (2010) KPAD library, a separate object in the original (own
// .sdata2 pool after KPAD's, own .data and .sbss). Name invented; written from the asm.
#include <revolution/kpad/KPADPriv.h>

#include <internal/fdlibm.h>

// Only the small accessors are written; the work area (0xDB0 bytes per channel) and names are inferred.
typedef struct KPADMplsWork_unk {
    u8 pad_0x0[0x4];
    s32 m004;          // 0x004 inferred
    u8 pad_0x8[0xC2];
    s8 m0CA;           // 0x0CA inferred: 1 or -1
    u8 pad_0xCB[0xCD5];
    s32 mDA0;          // 0xDA0
    s32 mDA4;          // 0xDA4
    s32 mDA8;          // 0xDA8
    u8 pad_0xDAC[0x4];
} KPADMplsWork_unk;

static KPADMplsWork_unk* kp_mpls_work_unk;

void fn_80658DB0(KPADChannel chan, BOOL flag)
{
    KPADMplsWork_unk* w = &kp_mpls_work_unk[chan];
    if (kp_mpls_work_unk == NULL)
        return;
    if (!flag)
        w->m0CA = 1;
    else
        w->m0CA = -1;
}

BOOL fn_80658F30(void)
{
    return kp_mpls_work_unk != NULL;
}

void fn_8065AA50(KPADChannel chan, s32 a, s32 b, s32 c)
{
    KPADMplsWork_unk* w = &kp_mpls_work_unk[chan];
    if (kp_mpls_work_unk == NULL)
        return;
    w->mDA0 = a;
    w->mDA4 = b;
    w->mDA8 = c;
}

void fn_8065AEB0(s32 value)
{
    if (kp_mpls_work_unk == NULL)
        return;
    kp_mpls_work_unk->m004 = value;
}
