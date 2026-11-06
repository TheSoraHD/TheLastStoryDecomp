#include <homebuttonLib.h>

#include <revolution/ARC.h>
#include <revolution/OS.h>
#include <revolution/WENC.h>
#include <revolution/WPAD.h>

#include <cstring>

namespace homebutton {

// values of nw4hbm::snd::RemoteSpeaker (tp_revolution/homebuttonLib)
static const int SAMPLES_PER_AUDIO_PACKET = 40;
static const int SAMPLES_PER_ENCODED_PACKET = 20;

RemoteSpk* RemoteSpk::spInstance = NULL;

void RemoteSpk::SetInstance(RemoteSpk* pThis) {
    spInstance = pThis;
}

RemoteSpk* RemoteSpk::GetInstance() {
    return spInstance;
}

void RemoteSpk::GetPCMFromSeID(int id, s16*& rpPcm, int& rLength) {
    ARCFileInfo af;
    ARCFastOpen(&handle, id, &af);

    rpPcm = static_cast<s16*>(ARCGetStartAddrInMem(&af));
    rLength = ARCGetLength(&af);

    ARCClose(&af);
}

static bool MakeVolumeData(const s16* pSrc, s16* pDst, int vol, u32 size) {
    u32 enc_size = size <= SAMPLES_PER_AUDIO_PACKET
                       ? size
                       : SAMPLES_PER_AUDIO_PACKET;

    for (int i = 0; i < enc_size; i++) {
        *pDst++ = static_cast<s16>(*pSrc++ * vol / 10);
    }

    if (size > SAMPLES_PER_AUDIO_PACKET) {
        return false;
    }

    u32 zero_size = SAMPLES_PER_AUDIO_PACKET - size;

    for (int i = 0; i < zero_size; i++) {
        *pDst++ = 0;
    }

    return true;
}

void RemoteSpk::UpdateSpeaker(OSAlarm* /* pAlarm */,
                              OSContext* /* pContext */) {
    s16 pcmBuffer[SAMPLES_PER_AUDIO_PACKET];
    u8 adpcmBuffer[SAMPLES_PER_ENCODED_PACKET];

    if (GetInstance() == NULL) {
        return;
    }

    ChanInfo* pInfo = GetInstance()->info;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++, pInfo++) {
        if (pInfo->in_pcm != NULL && WPADIsSpeakerEnabled(i)) {
            BOOL intrStatus = OSDisableInterrupts();

            if (WPADCanSendStreamData(i)) {
                MakeVolumeData(pInfo->in_pcm, pcmBuffer, pInfo->vol,
                               pInfo->length / sizeof(s16));

                WENCGetEncodeData(
                    &pInfo->wencinfo, pInfo->first ? 0 : WENC_FLAG_USER_INFO,
                    pcmBuffer, ARRAY_SIZE(pcmBuffer), adpcmBuffer);

                WPADSendStreamData(i, adpcmBuffer, ARRAY_SIZE(adpcmBuffer));

                pInfo->first = false;
                pInfo->cannotSendCnt = 0;
                pInfo->in_pcm += ARRAY_SIZE(pcmBuffer);
                pInfo->length -= ARRAY_SIZE(pcmBuffer) * sizeof(s16);

                if (pInfo->length <= 0) {
                    pInfo->seId = -1;
                    pInfo->in_pcm = NULL;
                }
            } else {
                pInfo->cannotSendCnt++;

                // (60 in the version of 2010, as HBM_REVISION != 1 in tp_revolution)
                if (pInfo->cannotSendCnt > 60) {
                    pInfo->in_pcm = NULL;
                }
            }

            OSRestoreInterrupts(intrStatus);
        }
    }
}

// Version of July 2010 (HBM of The Last Story) from here on, written from the asm.
void RemoteSpk::ClearPcm() {
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        info[i].in_pcm = NULL;
        info[i].seId = -1;
    }
}

RemoteSpk::RemoteSpk(void* pSpkSeBuf) {
    SetInstance(this);

    if (pSpkSeBuf != NULL) {
        available = ARCInitHandle(pSpkSeBuf, &handle);
    } else {
        available = false;
    }

    OSCreateAlarm(&speakerAlarm);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCreateAlarm(&info[i].alarm);
        info[i].in_pcm = NULL;
        info[i].seId = -1;
        info[i].first = true;
        info[i].playReady = true;
    }
}

RemoteSpk::~RemoteSpk() {
    SetInstance(NULL);
    available = false;

    OSCancelAlarm(&speakerAlarm);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCancelAlarm(&info[i].alarm);
    }
}

void RemoteSpk::Start() {
    if (!available) {
        return;
    }

    OSCreateAlarm(&speakerAlarm);

    OSSetPeriodicAlarm(&speakerAlarm, OSGetTime(), OS_NSEC_TO_TICKS(6666667), &UpdateSpeaker);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCreateAlarm(&info[i].alarm);
        info[i].in_pcm = NULL;
        info[i].seId = -1;
        info[i].first = true;
        info[i].playReady = true;
    }
}

void RemoteSpk::Stop() {
    OSCancelAlarm(&speakerAlarm);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        OSCancelAlarm(&info[i].alarm);
    }
}

void RemoteSpk::DelaySpeakerOnCallback(OSAlarm* pAlarm,
                                       OSContext* /* pContext */) {
    s32 chan = reinterpret_cast<s32>(OSGetAlarmUserData(pAlarm));
    s32 result = WPADControlSpeaker(chan, WPAD_SPEAKER_ON, &SpeakerOnCallback);
}

void RemoteSpk::SpeakerOnCallback(s32 chan, s32 result) {
    RemoteSpk* pRmtSpk = GetInstance();
    if (pRmtSpk == NULL) {
        return;
    }

    switch (result) {
    case WPAD_ERR_OK: {
        pRmtSpk->info[chan].first = true;
        result =
            WPADControlSpeaker(chan, WPAD_SPEAKER_PLAY, &SpeakerPlayCallback);
        break;
    }

    case WPAD_ERR_TRANSFER: {
        pRmtSpk->info[chan].playReady = false;
        break;
    }

    case WPAD_ERR_COMMUNICATION_ERROR: {
        OSSetAlarmUserData(&pRmtSpk->info[chan].alarm,
                           reinterpret_cast<void*>(chan));

        OSCancelAlarm(&pRmtSpk->info[chan].alarm);
        OSSetAlarm(&pRmtSpk->info[chan].alarm, OS_MSEC_TO_TICKS(50),
                   &DelaySpeakerOnCallback);
        break;
    }
    }
}

void RemoteSpk::DelaySpeakerPlayCallback(OSAlarm* pAlarm,
                                         OSContext* /* pContext */) {
    s32 chan = reinterpret_cast<s32>(OSGetAlarmUserData(pAlarm));
    s32 result =
        WPADControlSpeaker(chan, WPAD_SPEAKER_PLAY, &SpeakerPlayCallback);
}

void RemoteSpk::SpeakerPlayCallback(s32 chan, s32 result) {
    RemoteSpk* pRmtSpk = GetInstance();
    if (pRmtSpk == NULL) {
        return;
    }

    switch (result) {
    case WPAD_ERR_OK: {
        pRmtSpk->info[chan].playReady = true;
        break;
    }

    case WPAD_ERR_TRANSFER: {
        pRmtSpk->info[chan].playReady = false;
        break;
    }

    case WPAD_ERR_NO_CONTROLLER: {
        pRmtSpk->info[chan].playReady = false;
        break;
    }

    case WPAD_ERR_COMMUNICATION_ERROR: {
        OSSetAlarmUserData(&pRmtSpk->info[chan].alarm,
                           reinterpret_cast<void*>(chan));

        OSCancelAlarm(&pRmtSpk->info[chan].alarm);
        OSSetAlarm(&pRmtSpk->info[chan].alarm, OS_MSEC_TO_TICKS(50),
                   &DelaySpeakerPlayCallback);
        break;
    }
    }
}

void RemoteSpk::Connect(s32 chan) {
    if (!available) {
        return;
    }

    int result = WPADControlSpeaker(chan, WPAD_SPEAKER_ON, &SpeakerOnCallback);

    // inferred: the original clears the encoder state with 8 word stores
    for (int i = 0; i < 8; i++) {
        ((u32*)&info[chan].wencinfo)[i] = 0;
    }
    info[chan].first = true;
    info[chan].playReady = false;
}

void RemoteSpk::Play(s32 chan, int seID, s8 vol) {
    if (!available) {
        return;
    }

    s16* pPcm;
    int length;
    GetPCMFromSeID(seID, pPcm, length);

    info[chan].cannotSendCnt = 0;
    info[chan].seId = seID;
    info[chan].length = length;
    info[chan].vol = vol;
    info[chan].in_pcm = pPcm;
}

bool RemoteSpk::isPlaying(s32 chan) const {
    return info[chan].in_pcm != NULL;
}

bool RemoteSpk::isPlayingId(s32 chan, int seId) const {
    if (isPlaying(chan) && info[chan].seId == seId) {
        return true;
    }

    return false;
}

bool RemoteSpk::isPlayReady(s32 chan) const {
    return info[chan].playReady != false;
}

} // namespace homebutton
