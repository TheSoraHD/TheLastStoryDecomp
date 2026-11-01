#ifndef RVL_SDK_AXFX_DELAY_EXP_H
#define RVL_SDK_AXFX_DELAY_EXP_H
#include "common.h"
#ifdef __cplusplus
extern "C" {
#endif

// The Last Story: AXFX delay of the 2010 SDK (not in the Skyward Sword sources).
// inferred: layout of the parameters from nw4r::snd::FxDelay::SetParam (0x8070E3F0);
// the first 0x38 bytes (delay lines and state) are not reconstructed.
struct AXFX_BUS;

typedef struct AXFX_DELAY_EXP {
    u8 state[0x38];         // at 0x0
    f32 maxDelay;           // at 0x38
    f32 delay;              // at 0x3C
    f32 feedback;           // at 0x40
    f32 lpf;                // at 0x44
    struct AXFX_BUS *busIn;  // at 0x48
    struct AXFX_BUS *busOut; // at 0x4C
    f32 outGain;            // at 0x50
    f32 sendGain;           // at 0x54
} AXFX_DELAY_EXP;

u32 AXFXDelayExpGetMemSize(const AXFX_DELAY_EXP *fx);
BOOL AXFXDelayExpInit(AXFX_DELAY_EXP *fx);
BOOL AXFXDelayExpSettings(AXFX_DELAY_EXP *fx);
BOOL AXFXDelayExpSettingsUpdate(AXFX_DELAY_EXP *fx);
void AXFXDelayExpShutdown(AXFX_DELAY_EXP *fx);
void AXFXDelayExpCallback(void *chans, AXFX_DELAY_EXP *fx);

#ifdef __cplusplus
}
#endif
#endif
