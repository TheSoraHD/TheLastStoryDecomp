#include "nw4r/snd/FxChorus.h"

#include "common.h"
#include "nw4r/snd/AxManager.h"
#include "nw4r/snd/global.h"
#include "nw4r/ut/algorithm.h"

// The Last Story: 2010 version (AXFX_CHORUS_EXP + DPL2), written by analogy with
// snd_FxReverbStdDpl2.cpp.

namespace nw4r {
namespace snd {

FxChorus::FxChorus() : mIsActive(0) {
    ChorusParam p;
    SetParam(p);
}

u32 FxChorus::GetRequiredMemSize() {
    return ut::Max(
        ut::RoundUp(AXFXChorusExpGetMemSizeDpl2(&mAxfxParamDpl2) + detail::AxfxImpl::HEAP_SIZE_MIN, 32),
        ut::RoundUp(AXFXChorusExpGetMemSize(&mAxfxParam) + detail::AxfxImpl::HEAP_SIZE_MIN, 32)
    );
}

bool FxChorus::AssignWorkBuffer(void *pBuffer, u32 size) {
    return mImpl.CreateHeap(pBuffer, size);
}

void FxChorus::ReleaseWorkBuffer() {
    mImpl.DestroyHeap();
}

bool FxChorus::StartUp() {
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
            success = AXFXChorusExpInitDpl2(&mAxfxParamDpl2);
            mImpl.RestoreAlloc(allocHook, freeHook);

            (void)AXFXChorusExpGetMemSizeDpl2(&mAxfxParamDpl2); // debug leftover
            break;
        }
        default: {
            mOutputMode = 0;

            mImpl.HookAlloc(&allocHook, &freeHook);
            success = AXFXChorusExpInit(&mAxfxParam);
            mImpl.RestoreAlloc(allocHook, freeHook);

            (void)AXFXChorusExpGetMemSize(&mAxfxParam); // debug leftover
            break;
        }
    }

    mIsActive = 1;

    return success;
}

void FxChorus::Shutdown() {
    if (!mIsActive) {
        return;
    }
    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    mIsActive = 0;
    mImpl.HookAlloc(&allocHook, &freeHook);

    switch (mOutputMode) {
        case 1: {
            AXFXChorusExpShutdownDpl2(&mAxfxParamDpl2);
            break;
        }
        default: {
            AXFXChorusExpShutdown(&mAxfxParam);
            break;
        }
    }
    mImpl.RestoreAlloc(allocHook, freeHook);
}

bool FxChorus::SetParam(const ChorusParam &rParam) {
    mParam = rParam;

    mAxfxParam.delayTime = mAxfxParamDpl2.delayTime = ut::Clamp(rParam.delayTime, 0.1f, 50.0f);
    mAxfxParam.depth = mAxfxParamDpl2.depth = ut::Clamp(rParam.depth, 0.0f, 1.0f);
    mAxfxParam.rate = mAxfxParamDpl2.rate = ut::Clamp(rParam.rate, 0.1f, 2.0f);
    mAxfxParam.feedback = mAxfxParamDpl2.feedback = ut::Clamp(rParam.feedback, 0.0f, 0.99f);
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

    BOOL success;
    switch (mOutputMode) {
        case 1:  success = AXFXChorusExpSettingsDpl2(&mAxfxParamDpl2); break;
        default: success = AXFXChorusExpSettings(&mAxfxParam); break;
    }

    return success;
}

void FxChorus::UpdateBuffer(
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
            AXFXChorusExpCallbackDpl2(chans, &mAxfxParamDpl2);
            break;
        }
        default: {
            void *chans[3];
            chans[0] = ppBuffer[0];
            chans[1] = ppBuffer[1];
            chans[2] = ppBuffer[2];
            AXFXChorusExpCallback(chans, &mAxfxParam);
            break;
        }
    }
}

void FxChorus::OnChangeOutputMode() {
    if (mOutputMode != (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2)) {
        Shutdown();
        StartUp();
    }
}

} // namespace snd
} // namespace nw4r
