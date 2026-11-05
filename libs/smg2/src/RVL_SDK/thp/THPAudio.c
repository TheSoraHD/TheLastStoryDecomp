#include "revolution/thp.h"

// Version of August 2010 (written from the asm of The Last Story): the yn rounding is done
// before the shift, bit 1 of flag mixes both channels to mono at -3 dB (0x5A82 = 1/sqrt(2)),
// and the stereo case decodes both channels in the same loop.
u32 THPAudioDecode(s16 *audioBuffer, u8 *audioFrame, s32 flag) {
    THPAudioRecordHeader *header;
    THPAudioDecodeInfo decInfoL;
    THPAudioDecodeInfo decInfoR;
    u8 *left, *right;
    s16 *decLeftPtr, *decRightPtr;
    s16 yn1L, yn2L, yn1R, yn2R;
    u32 i;
    s32 step;
    s32 sample;
    s64 ynL, ynR;
    BOOL mono;

    if (audioBuffer == NULL || audioFrame == NULL) {
        return 0;
    }

    header = (THPAudioRecordHeader*)audioFrame;
    left = audioFrame + sizeof(THPAudioRecordHeader);
    right = left + header->offsetNextChannel;

    if (flag & 1) {
        decRightPtr = audioBuffer;
        decLeftPtr = audioBuffer + header->sampleSize;
        step = 1;
    }
    else {
        decRightPtr = audioBuffer;
        decLeftPtr = audioBuffer + 1;
        step = 2;
    }

    mono = FALSE;
    if (flag & 2) {
        mono = TRUE;
    }

    if (header->offsetNextChannel == 0) {
        __THPAudioInitialize(&decInfoL, left);

        yn1L = header->lYn1;
        yn2L = header->lYn2;

        for (i = 0; i < header->sampleSize; i++) {
            sample = __THPAudioGetNewSample(&decInfoL);
            ynL  = header->lCoef[decInfoL.predictor][1] * yn2L;
            ynL += header->lCoef[decInfoL.predictor][0] * yn1L;
            ynL += (sample << decInfoL.scale) << 11;
            ynL += 0x400;
            ynL >>= 11;

            if (ynL > 32767) {
                ynL = 32767;
            }
            else if (ynL < -32768) {
                ynL = -32768;
            }

            yn2L = yn1L;
            yn1L = (s16)ynL;

            if (mono) {
                ynL = ((ynL << 1) * 0x5A82 + 0x4000) >> 15;

                if (ynL > 32767) {
                    ynL = 32767;
                }
                else if (ynL < -32768) {
                    ynL = -32768;
                }
            }

            *decLeftPtr = (s16)ynL;
            decLeftPtr += step;
            *decRightPtr = (s16)ynL;
            decRightPtr += step;
        }
    }
    else {
        __THPAudioInitialize(&decInfoL, left);
        __THPAudioInitialize(&decInfoR, right);

        yn1L = header->lYn1;
        yn2L = header->lYn2;
        yn1R = header->rYn1;
        yn2R = header->rYn2;

        for (i = 0; i < header->sampleSize; i++) {
            sample = __THPAudioGetNewSample(&decInfoL);
            ynL  = header->lCoef[decInfoL.predictor][1] * yn2L;
            ynL += header->lCoef[decInfoL.predictor][0] * yn1L;
            ynL += (sample << decInfoL.scale) << 11;
            ynL += 0x400;
            ynL >>= 11;

            if (ynL > 32767) {
                ynL = 32767;
            }
            else if (ynL < -32768) {
                ynL = -32768;
            }

            yn2L = yn1L;
            yn1L = (s16)ynL;

            sample = __THPAudioGetNewSample(&decInfoR);
            ynR  = header->rCoef[decInfoR.predictor][1] * yn2R;
            ynR += header->rCoef[decInfoR.predictor][0] * yn1R;
            ynR += (sample << decInfoR.scale) << 11;
            ynR += 0x400;
            ynR >>= 11;

            if (ynR > 32767) {
                ynR = 32767;
            }
            else if (ynR < -32768) {
                ynR = -32768;
            }

            yn2R = yn1R;
            yn1R = (s16)ynR;

            if (mono) {
                ynL = ((ynL + ynR) * 0x5A82 + 0x4000) >> 15;

                if (ynL > 32767) {
                    ynL = 32767;
                }
                else if (ynL < -32768) {
                    ynL = -32768;
                }

                ynR = ynL;
            }

            *decLeftPtr = (s16)ynL;
            decLeftPtr += step;
            *decRightPtr = (s16)ynR;
            decRightPtr += step;
        }
    }

    return header->sampleSize;
}

static s32 __THPAudioGetNewSample(THPAudioDecodeInfo* info) {
    s32 sample;
  
    if (!(info->offsetNibbles & 0x0f)) {
        info->predictor = (u8)((*(info->encodeData) & 0x70) >> 4);
        info->scale = (u8)((*(info->encodeData) & 0xF));
        info->encodeData++;
        info->offsetNibbles += 2; 
    }

    if (info->offsetNibbles & 0x1) {
        sample = (s32)((*(info->encodeData) & 0xF) << 28) >> 28;
        info->encodeData++;
    }
    else {
        sample = (s32)((*(info->encodeData) & 0xF0) << 24) >> 28;
    }

    info->offsetNibbles++;
    return sample;
}

static void __THPAudioInitialize(THPAudioDecodeInfo* info, u8* ptr) {
    info->encodeData = ptr;
    info->offsetNibbles = 2;
    info->predictor = (u8)((*(info->encodeData) & 0x70) >> 4);
    info->scale = (u8)((*(info->encodeData) & 0xF));
    info->encodeData++;
    return;
}
