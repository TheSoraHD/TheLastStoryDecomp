#include "nw4r/snd/FxDelay.h"

#include "common.h"
#include "nw4r/snd/AxManager.h"
#include "nw4r/snd/global.h"
#include "nw4r/ut/algorithm.h"

// The Last Story: 2010 version (AXFX_DELAY_EXP + DPL2), written by analogy with
// snd_FxReverbStdDpl2.cpp.

namespace nw4r {
namespace snd {

FxDelay::FxDelay() : mIsActive(0) {
    DelayParam p;
    SetParam(p);
}

u32 FxDelay::GetRequiredMemSize() {
    return ut::Max(
        ut::RoundUp(AXFXDelayExpGetMemSizeDpl2(&mAxfxParamDpl2) + detail::AxfxImpl::HEAP_SIZE_MIN, 32),
        ut::RoundUp(AXFXDelayExpGetMemSize(&mAxfxParam) + detail::AxfxImpl::HEAP_SIZE_MIN, 32)
    );
}

bool FxDelay::AssignWorkBuffer(void *pBuffer, u32 size) {
    return mImpl.CreateHeap(pBuffer, size);
}

void FxDelay::ReleaseWorkBuffer() {
    mImpl.DestroyHeap();
}

bool FxDelay::StartUp() {
    u32 memSize = GetRequiredMemSize();
    if (memSize > mImpl.GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    BOOL success;
    switch (detail::AxManager::GetInstance().GetOutputMode()) {
        case OUTPUT_MODE_DPL2: {
            mOutputMode = 1;

            mImpl.HookAlloc(&allocHook, &freeHook);
            success = AXFXDelayExpInitDpl2(&mAxfxParamDpl2);
            mImpl.RestoreAlloc(allocHook, freeHook);

            (void)AXFXDelayExpGetMemSizeDpl2(&mAxfxParamDpl2); // debug leftover
            break;
        }
        default: {
            mOutputMode = 0;

            mImpl.HookAlloc(&allocHook, &freeHook);
            success = AXFXDelayExpInit(&mAxfxParam);
            mImpl.RestoreAlloc(allocHook, freeHook);

            (void)AXFXDelayExpGetMemSize(&mAxfxParam); // debug leftover
            break;
        }
    }

    mIsActive = 1;

    return success;
}

void FxDelay::Shutdown() {
    if (!mIsActive) {
        return;
    }
    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    mIsActive = 0;
    mImpl.HookAlloc(&allocHook, &freeHook);

    switch (mOutputMode) {
        case 1: {
            AXFXDelayExpShutdownDpl2(&mAxfxParamDpl2);
            break;
        }
        default: {
            AXFXDelayExpShutdown(&mAxfxParam);
            break;
        }
    }
    mImpl.RestoreAlloc(allocHook, freeHook);
}

bool FxDelay::SetParam(const DelayParam &rParam) {
    mParam = rParam;

    f32 maxDelay = ut::Max(rParam.maxDelay, 0.04f);
    bool b = maxDelay != mAxfxParam.maxDelay;

    mAxfxParam.maxDelay = mAxfxParamDpl2.maxDelay = maxDelay;
    mAxfxParam.delay = mAxfxParamDpl2.delay = ut::Clamp(rParam.delay, 0.04f, rParam.maxDelay);
    mAxfxParam.feedback = mAxfxParamDpl2.feedback = ut::Clamp(rParam.feedback, 0.0f, 0.99f);
    mAxfxParam.lpf = mAxfxParamDpl2.lpf = ut::Clamp(rParam.lpf, 0.0f, 1.0f);
    mAxfxParam.outGain = mAxfxParamDpl2.outGain = ut::Clamp(rParam.outGain, 0.0f, 1.0f);

    mAxfxParam.busIn = NULL;
    mAxfxParam.busOut = NULL;
    mAxfxParam.sendGain = 0.0f;

    mAxfxParamDpl2.busIn = NULL;
    mAxfxParamDpl2.busOut = NULL;
    mAxfxParamDpl2.sendGain = 0.0f;

    if (!mIsActive) {
        return true;
    }

    u32 memSize = GetRequiredMemSize();
    if (memSize > mImpl.GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    BOOL success;
    if (b) {
        mImpl.HookAlloc(&allocHook, &freeHook);
        switch (mOutputMode) {
            case 1:  success = AXFXDelayExpSettingsDpl2(&mAxfxParamDpl2); break;
            default: success = AXFXDelayExpSettings(&mAxfxParam); break;
        }

        mImpl.RestoreAlloc(allocHook, freeHook);
    } else {
        switch (mOutputMode) {
            case 1:  success = AXFXDelayExpSettingsUpdateDpl2(&mAxfxParamDpl2); break;
            default: success = AXFXDelayExpSettingsUpdate(&mAxfxParam); break;
        }
    }

    return success;
}

void FxDelay::UpdateBuffer(
    int channels, void **ppBuffer, u32 size, SampleFormat sampleFormat, f32 sampleRate, OutputMode outputMode
) {
    if (!mIsActive) {
        return;
    }

    if ((outputMode == OUTPUT_MODE_DPL2) != mOutputMode) {
        return;
    }

    switch (mOutputMode) {
        case 1: {
            void *chans[AX_DPL2_MAX];
            chans[AX_DPL2_L] = ppBuffer[AX_DPL2_L];
            chans[AX_DPL2_R] = ppBuffer[AX_DPL2_R];
            chans[AX_DPL2_LS] = ppBuffer[AX_DPL2_LS];
            chans[AX_DPL2_RS] = ppBuffer[AX_DPL2_RS];
            AXFXDelayExpCallbackDpl2(chans, &mAxfxParamDpl2);
            break;
        }
        default: {
            void *chans[3];
            chans[0] = ppBuffer[0];
            chans[1] = ppBuffer[1];
            chans[2] = ppBuffer[2];
            AXFXDelayExpCallback(chans, &mAxfxParam);
            break;
        }
    }
}

void FxDelay::OnChangeOutputMode() {
    if (mOutputMode != (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2)) {
        Shutdown();
        StartUp();
    }
}

} // namespace snd
} // namespace nw4r
