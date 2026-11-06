// inferred: Dolby Pro Logic II version of AXFXDelayExp.c (four channels in arrays), written from the asm
#include <revolution/AX.h>
#include <revolution/AXFX.h>
#include <revolution/OS.h>
#include <string.h>

static BOOL __InitParams(AXFX_DELAY_EXP_DPL2* fx);

static BOOL __AllocDelayLine(AXFX_DELAY_EXP_DPL2* fx) {
    u32 i;

    for (i = 0; i < AXFX_DPL2_CHANNEL_MAX; i++) {
        fx->line[i] = __AXFXAlloc(fx->maxLength * sizeof(s32));
        if (fx->line[i] == NULL) {
            return FALSE;
        }
    }

    return TRUE;
}

static void __FreeDelayLine(AXFX_DELAY_EXP_DPL2* fx) {
    u32 i;

    for (i = 0; i < AXFX_DPL2_CHANNEL_MAX; i++) {
        if (fx->line[i] != NULL) {
            __AXFXFree(fx->line[i]);
            fx->line[i] = NULL;
        }
    }
}

u32 AXFXDelayExpGetMemSizeDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    return (s32)(32.0f * fx->maxDelay) * (AXFX_DPL2_CHANNEL_MAX * sizeof(s32));
}

BOOL AXFXDelayExpInitDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    BOOL enabled = OSDisableInterrupts();

    if (AXGetMode() != AX_OUTPUT_DPL2) {
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    fx->active = 1;

    if (fx->maxDelay <= 0.0f) {
        AXFXDelayExpShutdownDpl2(fx);
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    fx->maxLength = (u32)(32.0f * fx->maxDelay);
    if (fx->maxLength == 0) {
        fx->maxLength = 1;
    }

    if (!__AllocDelayLine(fx)) {
        AXFXDelayExpShutdownDpl2(fx);
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    if (!__InitParams(fx)) {
        AXFXDelayExpShutdownDpl2(fx);
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    fx->active |= 2;
    fx->active &= ~1;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL AXFXDelayExpSettingsDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    BOOL enabled = OSDisableInterrupts();
    fx->active |= 1;
    AXFXDelayExpShutdownDpl2(fx);

    if (!AXFXDelayExpInitDpl2(fx)) {
        AXFXDelayExpShutdownDpl2(fx);
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    fx->active |= 2;
    fx->active &= ~1;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

BOOL AXFXDelayExpSettingsUpdateDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    BOOL enabled = OSDisableInterrupts();
    fx->active |= 1;

    if (!__InitParams(fx)) {
        AXFXDelayExpShutdownDpl2(fx);
        OSRestoreInterrupts(enabled);
        return FALSE;
    }

    fx->active |= 2;
    fx->active &= ~1;
    OSRestoreInterrupts(enabled);
    return TRUE;
}

void AXFXDelayExpShutdownDpl2(AXFX_DELAY_EXP_DPL2* fx) {
    BOOL enabled = OSDisableInterrupts();
    fx->active |= 1;
    __FreeDelayLine(fx);
    OSRestoreInterrupts(enabled);
}

void AXFXDelayExpCallbackDpl2(AXFX_BUS_DPL2* bus, AXFX_DELAY_EXP_DPL2* fx) {
    s32* out[AXFX_DPL2_CHANNEL_MAX];
    s32 delayed[AXFX_DPL2_CHANNEL_MAX];
    s32 filtered[AXFX_DPL2_CHANNEL_MAX];
    s32* input[AXFX_DPL2_CHANNEL_MAX];
    s32* sendOut[AXFX_DPL2_CHANNEL_MAX];
    u32 i;
    u32 ch;
    s32 lpfCoef;
    s32 inverseLpf;

    if (fx->active) {
        fx->active &= ~2;
        return;
    }

    lpfCoef = fx->lpfCoef;
    inverseLpf = 128 - lpfCoef;

    out[0] = bus->L;
    out[1] = bus->R;
    out[2] = bus->Ls;
    out[3] = bus->Rs;

    if (fx->busIn) {
        input[0] = fx->busIn->L;
        input[1] = fx->busIn->R;
        input[2] = fx->busIn->Ls;
        input[3] = fx->busIn->Rs;
    }

    if (fx->busOut) {
        sendOut[0] = fx->busOut->L;
        sendOut[1] = fx->busOut->R;
        sendOut[2] = fx->busOut->Ls;
        sendOut[3] = fx->busOut->Rs;
    }

    for (i = 0; i < 96; i++) {
        for (ch = 0; ch < AXFX_DPL2_CHANNEL_MAX; ch++) {
            delayed[ch] = fx->line[ch][fx->curPos];

            if (fx->busIn) {
                filtered[ch] = inverseLpf * (*out[ch] + *input[ch]++) +
                               lpfCoef * fx->lastLpfOut[ch];
            } else {
                filtered[ch] =
                    inverseLpf * *out[ch] + lpfCoef * fx->lastLpfOut[ch];
            }

            filtered[ch] >>= 7;
            fx->lastLpfOut[ch] = filtered[ch];
            fx->line[ch][fx->curPos] =
                filtered[ch] + ((delayed[ch] * fx->feedbackGain) >> 7);
            *out[ch]++ = (delayed[ch] * fx->outGainCalc) >> 7;

            if (fx->busOut) {
                *sendOut[ch]++ = (delayed[ch] * fx->sendGainCalc) >> 7;
            }
        }

        if (++fx->curPos >= fx->length) {
            fx->curPos = 0;
        }
    }
}

static BOOL __InitParams(AXFX_DELAY_EXP_DPL2* fx) {
    f32 lpf;
    u32 i;

    if (fx->delay > fx->maxDelay) {
        return FALSE;
    }
    if (fx->feedback < 0.0f || fx->feedback >= 1.0f) {
        return FALSE;
    }
    if (fx->lpf < 0.0f || fx->lpf > 1.0f) {
        return FALSE;
    }
    if (fx->outGain < 0.0f || fx->outGain > 1.0f) {
        return FALSE;
    }
    if (fx->sendGain < 0.0f || fx->sendGain > 1.0f) {
        return FALSE;
    }

    fx->length = (u32)(32.0f * fx->delay);
    if (fx->length == 0) {
        fx->length = 1;
    }

    fx->curPos = 0;
    fx->feedbackGain = (s32)(128.0f * fx->feedback);

    lpf = 1.0f - fx->lpf;
    if (lpf > 0.95f) {
        lpf = 0.95f;
    }
    fx->lpfCoef = (s32)(128.0f * lpf);

    for (i = 0; i < AXFX_DPL2_CHANNEL_MAX; i++) {
        memset(fx->line[i], 0, fx->maxLength * sizeof(s32));
        fx->lastLpfOut[i] = 0;
    }

    fx->outGainCalc = (s32)(128.0f * fx->outGain);
    fx->sendGainCalc = (s32)(128.0f * fx->sendGain);
    return TRUE;
}
