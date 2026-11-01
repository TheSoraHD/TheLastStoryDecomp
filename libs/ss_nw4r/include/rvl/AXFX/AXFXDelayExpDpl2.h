#ifndef RVL_SDK_AXFX_DELAY_EXP_DPL2_H
#define RVL_SDK_AXFX_DELAY_EXP_DPL2_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

// The Last Story: Dolby Pro Logic II version of AXFX_DELAY_EXP (four channels).
// inferred: layout of the parameters from nw4r::snd::FxDelay::SetParam (0x8070E3F0).
struct AXFX_BUS;

typedef struct AXFX_DELAY_EXP_DPL2 {
    u8 state[0x40];         // at 0x0
    f32 maxDelay;           // at 0x40
    f32 delay;              // at 0x44
    f32 feedback;           // at 0x48
    f32 lpf;                // at 0x4C
    struct AXFX_BUS *busIn;  // at 0x50
    struct AXFX_BUS *busOut; // at 0x54
    f32 outGain;            // at 0x58
    f32 sendGain;           // at 0x5C
} AXFX_DELAY_EXP_DPL2;

u32 AXFXDelayExpGetMemSizeDpl2(const AXFX_DELAY_EXP_DPL2 *fx);
BOOL AXFXDelayExpInitDpl2(AXFX_DELAY_EXP_DPL2 *fx);
BOOL AXFXDelayExpSettingsDpl2(AXFX_DELAY_EXP_DPL2 *fx);
BOOL AXFXDelayExpSettingsUpdateDpl2(AXFX_DELAY_EXP_DPL2 *fx);
void AXFXDelayExpShutdownDpl2(AXFX_DELAY_EXP_DPL2 *fx);
void AXFXDelayExpCallbackDpl2(void *chans, AXFX_DELAY_EXP_DPL2 *fx);

#ifdef __cplusplus
}
#endif
#endif
