#ifndef RVL_SDK_AXFX_DELAY_EXP_H
#define RVL_SDK_AXFX_DELAY_EXP_H
#include <types.h>
#include <revolution/axfx/AXFXCommon.h>
#ifdef __cplusplus
extern "C" {
#endif

// Source: wii-ipl decomp (CC0), revolution/axfx headers.

typedef struct AXFX_DELAY_EXP {
    s32* lineL;                               // 0x00
    s32* lineR;                               // 0x04
    s32* lineS;                               // 0x08
    u32 curPos;                               // 0x0C
    u32 length;                               // 0x10
    u32 maxLength;                            // 0x14
    s32 feedbackGain;                         // 0x18
    s32 lastLpfOut[AXFX_STEREO_CHANNEL_MAX];  // 0x1C
    s32 lpfCoef;                              // 0x28
    s32 outGainCalc;                          // 0x2C
    s32 sendGainCalc;                         // 0x30
    u32 active;                               // 0x34
    f32 maxDelay;                             // 0x38
    f32 delay;                                // 0x3C
    f32 feedback;                             // 0x40
    f32 lpf;                                  // 0x44
    AXFX_BUS* busIn;                          // 0x48
    AXFX_BUS* busOut;                         // 0x4C
    f32 outGain;                              // 0x50
    f32 sendGain;                             // 0x54
} AXFX_DELAY_EXP;

u32 AXFXDelayExpGetMemSize(AXFX_DELAY_EXP* delay);
BOOL AXFXDelayExpInit(AXFX_DELAY_EXP* delay);
BOOL AXFXDelayExpSettings(AXFX_DELAY_EXP* delay);
BOOL AXFXDelayExpSettingsUpdate(AXFX_DELAY_EXP* delay);
void AXFXDelayExpShutdown(AXFX_DELAY_EXP* delay);
void AXFXDelayExpCallback(AXFX_BUS* bus, AXFX_DELAY_EXP* delay);

#ifdef __cplusplus
}
#endif
#endif
