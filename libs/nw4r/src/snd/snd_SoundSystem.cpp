#include <nw4r/snd.h>

#include <revolution/AX.h>
#include <revolution/OS.h>
#include <revolution/SC.h>

namespace {

NW4R_LIB_VERSION(SND, "Jun  9 2009", "03:24:56", "0x4302_145");

static bool sInitialized = false;
static int sMaxVoices; // inferred: AXGetMaxVoices() cached by InitSoundSystem

} // namespace

namespace nw4r {
namespace snd {

detail::TaskThread SoundSystem::sTaskThread;

void SoundSystem::InitSoundSystem(s32 soundThreadPriority,
                                  s32 dvdThreadPriority) {
    // inferred: 2010 default work size
    static u8 defaultSoundSystemWork[0x155E0] ALIGN(32);

    SoundSystemParam param;
    param.soundThreadPriority = soundThreadPriority;
    param.dvdThreadPriority = dvdThreadPriority;

    InitSoundSystem(param, defaultSoundSystemWork,
                    sizeof(defaultSoundSystemWork));
}

void SoundSystem::InitSoundSystem(const SoundSystemParam& rParam, void* pWork,
                                  u32 workSize) {
#pragma unused(workSize)

    if (sInitialized) {
        return;
    }

    sInitialized = true;

    OSRegisterVersion(NW4R_SND_Version_);

    detail::AxManager::GetInstance().Init();

    SCInit();
    while (SCCheckStatus() == SC_STATUS_BUSY) {
        ;
    }

    switch (SCGetSoundMode()) {
    case SC_SND_MONO: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_MONO);
        break;
    }

    case SC_SND_STEREO: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
        break;
    }

    case SC_SND_SURROUND: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_DPL2);
        break;
    }

    default: {
        detail::AxManager::GetInstance().SetOutputMode(OUTPUT_MODE_STEREO);
        break;
    }
    }

    detail::RemoteSpeakerManager::GetInstance().Setup();

    u8* pPtr = static_cast<u8*>(pWork);

    void* pDvdThreadStack = pPtr;
    pPtr += rParam.dvdThreadStackSize;

    void* pSoundThreadStack = pPtr;
    pPtr += rParam.soundThreadStackSize;

    sMaxVoices = AXGetMaxVoices();

    void* pAxVoiceWork = pPtr;
    pPtr += detail::AxVoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices);

    detail::AxVoiceManager::GetInstance().Setup(
        pAxVoiceWork,
        detail::AxVoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices));

    void* pVoiceWork = pPtr;
    pPtr += detail::VoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices);

    detail::VoiceManager::GetInstance().Setup(
        pVoiceWork,
        detail::VoiceManager::GetInstance().GetRequiredMemSize(sMaxVoices));

    void* pChannelWork = pPtr;
    pPtr += detail::ChannelManager::GetInstance().GetRequiredMemSize(sMaxVoices);

    detail::ChannelManager::GetInstance().Setup(
        pChannelWork,
        detail::ChannelManager::GetInstance().GetRequiredMemSize(sMaxVoices));

    detail::SeqPlayer::InitSeqPlayer();

    sTaskThread.Create(rParam.dvdThreadPriority, pDvdThreadStack,
                       rParam.dvdThreadStackSize);

    detail::SoundThread::GetInstance().Create(rParam.soundThreadPriority,
                                              pSoundThreadStack,
                                              rParam.soundThreadStackSize);
}

void SoundSystem::ShutdownSoundSystem() {
    if (!sInitialized) {
        return;
    }

    detail::SoundThread::GetInstance().Shutdown();
    detail::TaskManager::GetInstance().CancelAllTask();
    sTaskThread.Destroy();

    detail::RemoteSpeakerManager::GetInstance().Shutdown();
    detail::ChannelManager::GetInstance().Shutdown();
    detail::VoiceManager::GetInstance().Shutdown();
    detail::AxVoiceManager::GetInstance().Shutdown();
    detail::AxManager::GetInstance().Shutdown();

    sInitialized = false;
}

bool SoundSystem::IsInitializedSoundSystem() {
    return sInitialized;
}

#if 0 // not in this build
void SoundSystem::WaitForResetReady() {
    if (!sInitialized) {
        return;
    }

    u32 start = OSGetTick();

    while (!detail::AxManager::GetInstance().IsResetReady()) {
        if (OS_TICKS_TO_SEC(OSGetTick() - start) > 0) {
            break;
        }
    }
}

#endif

} // namespace snd
} // namespace nw4r
