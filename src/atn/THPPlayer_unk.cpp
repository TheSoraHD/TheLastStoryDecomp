// inferred: THP movie player without threads, adapted from THPSimple.c of the THP SDK sample
// "thpsimple" (THPSimpleInit 0x805B55A8 ... THPSimpleGetVolume 0x805B6C0C, plus a stereo/mono
// switch at 0x805B6C44). The decoding, drawing and audio mixing run from atn::VideoTHP. The public
// functions keep their automatic names (atn/VideoTHP.cpp calls them); the SDK names are in the
// comments. Name of the file invented.

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef int BOOL;

extern "C" {

// inferred: DVDFileInfo of the RVL SDK (0x3C bytes)
struct DVDFileInfo {
    u8 m00[0x3C];
};
typedef void (*DVDCallback)(s32 result, DVDFileInfo* fileInfo);
typedef void (*AIDCallback)(void);

void* memset(void* dst, int c, unsigned long n);
void* memcpy(void* dst, const void* src, unsigned long n);
int strcmp(const char* a, const char* b);
BOOL fn_805FA270(const char* fileName, DVDFileInfo* fileInfo); // DVDOpen
s32 DVDReadPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, s32 prio);
BOOL DVDReadAsyncPrio(DVDFileInfo* fileInfo, void* addr, s32 length, s32 offset, DVDCallback callback,
                      s32 prio);
BOOL DVDClose(DVDFileInfo* fileInfo);
BOOL DVDCancel(DVDFileInfo* block);
BOOL OSDisableInterrupts(void);
BOOL OSEnableInterrupts(void);
BOOL OSRestoreInterrupts(BOOL level);
void DCFlushRange(void* addr, u32 nBytes);
void DCInvalidateRange(void* addr, u32 nBytes);
AIDCallback AIRegisterDMACallback(AIDCallback callback);
void AIInitDMA(u32 start, u32 length);
void AIStartDMA(void);
u32 AIGetDMAStartAddr(void);
u32 AIGetDSPSampleRate(void);
BOOL fn_805E7480(void); // THPInit
s32 fn_805E4090(void* file, void* tileY, void* tileU, void* tileV, void* work); // THPVideoDecode
u32 fn_805E7520(s16* audioBuffer, u8* audioFrame, s32 flag);                  // THPAudioDecode
void fn_805B4DF0(void);                          // THPGXRestore (atn/THPDraw_unk.cpp)
void fn_805B4F08(void* rmode);                   // THPGXYuv2RgbSetup
void fn_805B53CC(void* yData, void* uData, void* vData, short x, short y, short textureWidth,
                 short textureHeight, short polygonWidth, short polygonHeight); // THPGXYuv2RgbDraw

// inferred: THPHeader of the SDK (0x30 bytes)
struct THPHeader {
    char magic[4];
    u32 version;
    u32 bufSize;
    u32 audioMaxSamples;
    f32 frameRate;
    u32 numFrames;
    u32 firstFrameSize;
    u32 movieDataSize;
    u32 compInfoDataOffsets;
    u32 offsetDataOffsets;
    u32 movieDataOffsets;
    u32 finalFrameDataOffsets;
};

struct THPFrameCompInfo {
    u32 numComponents;
    u8 frameComp[16];
};

struct THPVideoInfo {
    u32 xSize;
    u32 ySize;
    u32 videoType;
};

struct THPAudioInfo {
    u32 sndChannels;
    u32 sndFrequency;
    u32 sndNumSamples;
    u32 sndNumTracks;
};

struct THPReadBuffer {
    u8* ptr;
    s32 frameNumber;
    BOOL isValid;
};

struct THPTextureSet {
    u8* ytexture;
    u8* utexture;
    u8* vtexture;
    s32 frameNumber;
};

struct THPAudioBuffer {
    s16* buffer;
    s16* curPtr;
    u32 validSample;
};

// inferred: THPSimple control block of the SDK sample (0x18C bytes), plus mSoundMode
struct THPSimple {
    DVDFileInfo fileInfo;            // 0x00
    THPHeader header;                // 0x3C
    THPFrameCompInfo compInfo;       // 0x6C
    THPVideoInfo videoInfo;          // 0x80
    THPAudioInfo audioInfo;          // 0x8C
    void* thpWork;                   // 0x9C
    BOOL open;                       // 0xA0
    u8 preFetchState;                // 0xA4
    u8 audioState;                   // 0xA5
    u8 loop;                         // 0xA6
    u8 audioExist;                   // 0xA7
    s32 curOffset;                   // 0xA8
    BOOL readError;                  // 0xAC
    BOOL readProgress;               // 0xB0
    s32 nextDecodeIndex;             // 0xB4
    s32 readIndex;                   // 0xB8
    s32 readSize;                    // 0xBC
    u32 totalReadFrame;              // 0xC0
    f32 curVolume;                   // 0xC4
    f32 targetVolume;                // 0xC8
    f32 deltaVolume;                 // 0xCC
    s32 rampCount;                   // 0xD0
    THPReadBuffer readBuffer[10];    // 0xD4
    THPTextureSet textureSet;        // 0x14C
    THPAudioBuffer audioBuffer[3];   // 0x15C
    s32 audioDecodeIndex;            // 0x180
    s32 audioOutputIndex;            // 0x184
    u32 soundMode;                   // 0x188: inferred: 0 mixes the two channels (mono)
};

static u16 VolumeTable[] = {
    0x0000, 0x0002, 0x0008, 0x0012, 0x0020, 0x0032, 0x0049, 0x0063, 0x0082, 0x00A4, 0x00CB, 0x00F5,
    0x0124, 0x0157, 0x018E, 0x01C9, 0x0208, 0x024B, 0x0292, 0x02DD, 0x032C, 0x037F, 0x03D7, 0x0432,
    0x0492, 0x04F5, 0x055D, 0x05C9, 0x0638, 0x06AC, 0x0724, 0x07A0, 0x0820, 0x08A4, 0x092C, 0x09B8,
    0x0A48, 0x0ADD, 0x0B75, 0x0C12, 0x0CB2, 0x0D57, 0x0DFF, 0x0EAC, 0x0F5D, 0x1012, 0x10CA, 0x1187,
    0x1248, 0x130D, 0x13D7, 0x14A4, 0x1575, 0x164A, 0x1724, 0x1801, 0x18E3, 0x19C8, 0x1AB2, 0x1BA0,
    0x1C91, 0x1D87, 0x1E81, 0x1F7F, 0x2081, 0x2187, 0x2291, 0x239F, 0x24B2, 0x25C8, 0x26E2, 0x2801,
    0x2923, 0x2A4A, 0x2B75, 0x2CA3, 0x2DD6, 0x2F0D, 0x3048, 0x3187, 0x32CA, 0x3411, 0x355C, 0x36AB,
    0x37FF, 0x3956, 0x3AB1, 0x3C11, 0x3D74, 0x3EDC, 0x4048, 0x41B7, 0x432B, 0x44A3, 0x461F, 0x479F,
    0x4923, 0x4AAB, 0x4C37, 0x4DC7, 0x4F5C, 0x50F4, 0x5290, 0x5431, 0x55D6, 0x577E, 0x592B, 0x5ADC,
    0x5C90, 0x5E49, 0x6006, 0x61C7, 0x638C, 0x6555, 0x6722, 0x68F4, 0x6AC9, 0x6CA2, 0x6E80, 0x7061,
    0x7247, 0x7430, 0x761E, 0x7810, 0x7A06, 0x7C00, 0x7DFE, 0x8000,
};

static THPSimple SimpleControl;
static BOOL Initialized;
static s32 WorkBuffer[16] __attribute__((aligned(32)));
static s32 SoundBufferIndex;
static AIDCallback OldAIDCallback;
static s16* LastAudioBuffer;
static s16* CurAudioBuffer;
static s32 AudioSystem;
static s16 SoundBuffer[2][0xC0] __attribute__((aligned(32)));

static void fn_805B6978(void);
static void fn_805B5CF0(s32 result, DVDFileInfo* fileInfo);

// THPSimpleInit
BOOL fn_805B55A8(s32 audioSystem) {
    BOOL old;

    memset(&SimpleControl, 0, sizeof(THPSimple));
    SimpleControl.soundMode = 1;
    if (!fn_805E7480()) {
        return 0;
    }
    old = OSDisableInterrupts();
    AudioSystem = audioSystem;
    SoundBufferIndex = 0;
    LastAudioBuffer = 0;
    CurAudioBuffer = 0;
    OldAIDCallback = AIRegisterDMACallback(fn_805B6978);
    if (OldAIDCallback == 0 && AudioSystem != 0) {
        AIRegisterDMACallback(0);
        OSRestoreInterrupts(old);
        return 0;
    }
    OSRestoreInterrupts(old);
    if (AudioSystem == 0) {
        memset(SoundBuffer, 0, sizeof(SoundBuffer));
        DCFlushRange(SoundBuffer, sizeof(SoundBuffer));
        AIInitDMA((u32)SoundBuffer[SoundBufferIndex], sizeof(SoundBuffer[0]));
        AIStartDMA();
    }
    Initialized = 1;
    return 1;
}

// THPSimpleQuit
void fn_805B56C0(void) {
    BOOL old = OSDisableInterrupts();
    if (OldAIDCallback) {
        AIRegisterDMACallback(OldAIDCallback);
    }
    OSRestoreInterrupts(old);
    Initialized = 0;
}

// THPSimpleOpen
BOOL fn_805B5710(const char* fileName) {
    s32 offset;
    u32 i;

    if (!Initialized) {
        return 0;
    }
    if (SimpleControl.open) {
        return 0;
    }
    memset(&SimpleControl.videoInfo, 0, sizeof(THPVideoInfo));
    memset(&SimpleControl.audioInfo, 0, sizeof(THPAudioInfo));
    if (!fn_805FA270(fileName, &SimpleControl.fileInfo)) {
        return 0;
    }
    if (DVDReadPrio(&SimpleControl.fileInfo, WorkBuffer, 64, 0, 2) < 0) {
        DVDClose(&SimpleControl.fileInfo);
        return 0;
    }
    memcpy(&SimpleControl.header, WorkBuffer, sizeof(THPHeader));
    if (strcmp(SimpleControl.header.magic, "THP") != 0) {
        DVDClose(&SimpleControl.fileInfo);
        return 0;
    }
    if (SimpleControl.header.version != 0x11000) {
        DVDClose(&SimpleControl.fileInfo);
        return 0;
    }
    offset = SimpleControl.header.compInfoDataOffsets;
    if (DVDReadPrio(&SimpleControl.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
        DVDClose(&SimpleControl.fileInfo);
        return 0;
    }
    memcpy(&SimpleControl.compInfo, WorkBuffer, sizeof(THPFrameCompInfo));
    offset += sizeof(THPFrameCompInfo);
    SimpleControl.audioExist = 0;
    for (i = 0; i < SimpleControl.compInfo.numComponents; i++) {
        switch (SimpleControl.compInfo.frameComp[i]) {
        case 0:
            if (DVDReadPrio(&SimpleControl.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
                DVDClose(&SimpleControl.fileInfo);
                return 0;
            }
            memcpy(&SimpleControl.videoInfo, WorkBuffer, sizeof(THPVideoInfo));
            offset += sizeof(THPVideoInfo);
            break;
        case 1:
            if (DVDReadPrio(&SimpleControl.fileInfo, WorkBuffer, 32, offset, 2) < 0) {
                DVDClose(&SimpleControl.fileInfo);
                return 0;
            }
            memcpy(&SimpleControl.audioInfo, WorkBuffer, sizeof(THPAudioInfo));
            SimpleControl.audioExist = 1;
            offset += sizeof(THPAudioInfo);
            break;
        default:
            return 0;
        }
    }
    SimpleControl.curOffset = SimpleControl.header.movieDataOffsets;
    SimpleControl.readSize = SimpleControl.header.firstFrameSize;
    SimpleControl.readIndex = 0;
    SimpleControl.totalReadFrame = 0;
    SimpleControl.readError = 0;
    SimpleControl.textureSet.frameNumber = -1;
    SimpleControl.nextDecodeIndex = 0;
    SimpleControl.audioDecodeIndex = 0;
    SimpleControl.audioOutputIndex = 0;
    SimpleControl.preFetchState = 0;
    SimpleControl.audioState = 0;
    SimpleControl.loop = 0;
    SimpleControl.open = 1;
    SimpleControl.curVolume = 127.0f;
    SimpleControl.targetVolume = 127.0f;
    SimpleControl.rampCount = 0;
    return 1;
}

// THPSimpleClose
BOOL fn_805B59BC(void) {
    if (SimpleControl.open && SimpleControl.preFetchState == 0) {
        if (SimpleControl.audioExist) {
            if (SimpleControl.audioState == 1) {
                return 0;
            }
        } else {
            SimpleControl.audioState = 0;
        }
        if (!SimpleControl.readProgress) {
            SimpleControl.open = 0;
            DVDClose(&SimpleControl.fileInfo);
            return 1;
        }
    }
    return 0;
}

#define OSRoundUp32B(x) (((u32)(x) + 31) & ~31)

// THPSimpleCalcNeedMemory
u32 fn_805B5A4C(void) {
    u32 size;

    if (SimpleControl.open) {
        size = OSRoundUp32B(SimpleControl.header.bufSize) * 10;
        size += OSRoundUp32B(SimpleControl.videoInfo.xSize * SimpleControl.videoInfo.ySize);
        size += OSRoundUp32B(SimpleControl.videoInfo.xSize * SimpleControl.videoInfo.ySize / 4);
        size += OSRoundUp32B(SimpleControl.videoInfo.xSize * SimpleControl.videoInfo.ySize / 4);
        if (SimpleControl.audioExist) {
            size += OSRoundUp32B(SimpleControl.header.audioMaxSamples * 4) * 3;
        }
        size += 0x1000;
        return size;
    }
    return 0;
}

// NONMATCHING: 95.74% - the original stores the same zero register for isValid and validSample;
// here the read buffer loop gets a second one (tried both stores' types, loop counter types, the
// body order, two loops).
// THPSimpleSetBuffer
BOOL fn_805B5AD4(u8* buffer) {
    s32 i;
    u8* ptr;
    u32 ysize;
    u32 uvsize;

    if (SimpleControl.open && SimpleControl.preFetchState == 0) {
        if (SimpleControl.audioState == 1) {
            return 0;
        }
        ysize = OSRoundUp32B(SimpleControl.videoInfo.xSize * SimpleControl.videoInfo.ySize);
        uvsize = OSRoundUp32B(SimpleControl.videoInfo.xSize * SimpleControl.videoInfo.ySize / 4);
        ptr = buffer;
        SimpleControl.textureSet.ytexture = ptr;
        DCInvalidateRange(ptr, ysize);
        ptr += ysize;
        SimpleControl.textureSet.utexture = ptr;
        DCInvalidateRange(ptr, uvsize);
        ptr += uvsize;
        SimpleControl.textureSet.vtexture = ptr;
        DCInvalidateRange(ptr, uvsize);
        ptr += uvsize;
        for (i = 0; i < 10; i++) {
            SimpleControl.readBuffer[i].ptr = ptr;
            ptr += OSRoundUp32B(SimpleControl.header.bufSize);
            SimpleControl.readBuffer[i].isValid = 0;
        }
        if (SimpleControl.audioExist) {
            for (i = 0; i < 3; i++) {
                SimpleControl.audioBuffer[i].buffer = (s16*)ptr;
                SimpleControl.audioBuffer[i].curPtr = (s16*)ptr;
                SimpleControl.audioBuffer[i].validSample = 0;
                ptr += OSRoundUp32B(SimpleControl.header.audioMaxSamples * 4);
            }
        }
        SimpleControl.thpWork = ptr;
    }
    return 1;
}

// inferred: ReadFrameAsync of the SDK sample: starts reading the next frame (the callers test that
// the next buffer is free)
static inline void ReadFrameAsync(void) {
    if (SimpleControl.totalReadFrame > SimpleControl.header.numFrames - 1) {
        if (SimpleControl.loop != 1) {
            return;
        }
        SimpleControl.totalReadFrame = 0;
        SimpleControl.curOffset = SimpleControl.header.movieDataOffsets;
        SimpleControl.readSize = SimpleControl.header.firstFrameSize;
    }
    SimpleControl.readProgress = 1;
    if (DVDReadAsyncPrio(&SimpleControl.fileInfo, SimpleControl.readBuffer[SimpleControl.readIndex].ptr,
                         SimpleControl.readSize, SimpleControl.curOffset, fn_805B5CF0, 2) != 1) {
        SimpleControl.readProgress = 0;
        SimpleControl.readError = 1;
    }
}

// inferred: ReadFrameAsyncCallback of the SDK sample
static void fn_805B5CF0(s32 result, DVDFileInfo* fileInfo) {
    if (result == -1) {
        SimpleControl.readError = 1;
        return;
    }
    if (result == -3) {
        return;
    }
    SimpleControl.readProgress = 0;
    SimpleControl.readBuffer[SimpleControl.readIndex].frameNumber = SimpleControl.totalReadFrame;
    SimpleControl.totalReadFrame++;
    SimpleControl.readBuffer[SimpleControl.readIndex].isValid = 1;
    SimpleControl.curOffset += SimpleControl.readSize;
    SimpleControl.readSize = *(s32*)SimpleControl.readBuffer[SimpleControl.readIndex].ptr;
    SimpleControl.readIndex = SimpleControl.readIndex + 1 >= 10 ? 0 : SimpleControl.readIndex + 1;
    if (!SimpleControl.readBuffer[SimpleControl.readIndex].isValid && !SimpleControl.readError &&
        SimpleControl.preFetchState == 1) {
        ReadFrameAsync();
    }
}

// THPSimplePreLoad
BOOL fn_805B5E7C(s32 loop) {
    u32 i;
    u32 readNum;

    if (SimpleControl.open && SimpleControl.preFetchState == 0) {
        readNum = 10;
        if (!loop && SimpleControl.header.numFrames < 10) {
            readNum = SimpleControl.header.numFrames;
        }
        for (i = 0; i < readNum; i++) {
            if (DVDReadPrio(&SimpleControl.fileInfo, SimpleControl.readBuffer[SimpleControl.readIndex].ptr,
                            SimpleControl.readSize, SimpleControl.curOffset, 2) < 0) {
                SimpleControl.readError = 1;
                return 0;
            }
            SimpleControl.curOffset += SimpleControl.readSize;
            SimpleControl.readSize = *(s32*)SimpleControl.readBuffer[SimpleControl.readIndex].ptr;
            SimpleControl.readBuffer[SimpleControl.readIndex].isValid = 1;
            SimpleControl.readBuffer[SimpleControl.readIndex].frameNumber = SimpleControl.totalReadFrame;
            SimpleControl.readIndex = SimpleControl.readIndex + 1 >= 10 ? 0 : SimpleControl.readIndex + 1;
            SimpleControl.totalReadFrame++;
            if (SimpleControl.totalReadFrame > SimpleControl.header.numFrames - 1 && SimpleControl.loop == 1) {
                SimpleControl.totalReadFrame = 0;
                SimpleControl.curOffset = SimpleControl.header.movieDataOffsets;
                SimpleControl.readSize = SimpleControl.header.firstFrameSize;
            }
        }
        SimpleControl.loop = loop;
        SimpleControl.preFetchState = 1;
        return 1;
    }
    return 0;
}

// THPSimpleAudioStart
void fn_805B6004(void) {
    SimpleControl.audioState = 1;
}

// THPSimpleAudioStop
void fn_805B6018(void) {
    SimpleControl.audioState = 0;
}

// NONMATCHING: 89.26% - same stores, but the original loads movieDataOffsets/firstFrameSize/
// targetVolume between the isValid and the validSample stores and keeps zero in r6 and the base in
// r7 (here the other way round). Tried counter types and statement orders.
// THPSimpleLoadStop
BOOL fn_805B602C(void) {
    s32 i;

    if (SimpleControl.open && SimpleControl.audioState == 0) {
        SimpleControl.preFetchState = 0;
        if (SimpleControl.readProgress) {
            DVDCancel(&SimpleControl.fileInfo);
            SimpleControl.readProgress = 0;
        }
        for (i = 0; i < 10; i++) {
            SimpleControl.readBuffer[i].isValid = 0;
        }
        for (i = 0; i < 3; i++) {
            SimpleControl.audioBuffer[i].validSample = 0;
        }
        SimpleControl.textureSet.frameNumber = -1;
        SimpleControl.curOffset = SimpleControl.header.movieDataOffsets;
        SimpleControl.readSize = SimpleControl.header.firstFrameSize;
        SimpleControl.readIndex = 0;
        SimpleControl.totalReadFrame = 0;
        SimpleControl.readError = 0;
        SimpleControl.nextDecodeIndex = 0;
        SimpleControl.audioDecodeIndex = 0;
        SimpleControl.audioOutputIndex = 0;
        SimpleControl.curVolume = SimpleControl.targetVolume;
        SimpleControl.rampCount = 0;
        return 1;
    }
    return 0;
}

// inferred: VideoDecode of the SDK sample
static inline BOOL VideoDecode(u8* videoFrame) {
    if (fn_805E4090(videoFrame, SimpleControl.textureSet.ytexture, SimpleControl.textureSet.utexture,
                    SimpleControl.textureSet.vtexture, SimpleControl.thpWork) == 0) {
        SimpleControl.textureSet.frameNumber =
            SimpleControl.readBuffer[SimpleControl.nextDecodeIndex].frameNumber;
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.81% - only the audio loop: the original gives the component counter r31 and the
// frameComp pointer r29 (here swapped). Tried the order of the local declarations and an s32 counter.
// THPSimpleDecode
s32 fn_805B6120(s32 audioTrack) {
    u32 i;
    BOOL old;
    u8* ptr;
    u32* compSizePtr;
    u32 sample;

    if (SimpleControl.readBuffer[SimpleControl.nextDecodeIndex].isValid) {
        compSizePtr = (u32*)(SimpleControl.readBuffer[SimpleControl.nextDecodeIndex].ptr + 8);
        ptr = SimpleControl.readBuffer[SimpleControl.nextDecodeIndex].ptr +
              SimpleControl.compInfo.numComponents * 4 + 8;
        if (SimpleControl.audioExist) {
            if (audioTrack < 0 || audioTrack >= SimpleControl.audioInfo.sndNumTracks) {
                return 4;
            }
            if (SimpleControl.audioBuffer[SimpleControl.audioDecodeIndex].validSample == 0) {
                for (i = 0; i < SimpleControl.compInfo.numComponents; i++) {
                    switch (SimpleControl.compInfo.frameComp[i]) {
                    case 0:
                        if (!VideoDecode(ptr)) {
                            return 1;
                        }
                        break;
                    case 1:
                        sample = fn_805E7520(SimpleControl.audioBuffer[SimpleControl.audioDecodeIndex].buffer,
                                             ptr + *compSizePtr * audioTrack, 0);
                        old = OSDisableInterrupts();
                        SimpleControl.audioBuffer[SimpleControl.audioDecodeIndex].validSample = sample;
                        SimpleControl.audioBuffer[SimpleControl.audioDecodeIndex].curPtr =
                            SimpleControl.audioBuffer[SimpleControl.audioDecodeIndex].buffer;
                        OSRestoreInterrupts(old);
                        SimpleControl.audioDecodeIndex++;
                        if (SimpleControl.audioDecodeIndex >= 3) {
                            SimpleControl.audioDecodeIndex = 0;
                        }
                        break;
                    }
                    ptr += *compSizePtr;
                    compSizePtr++;
                }
            } else {
                return 3;
            }
        } else {
            for (i = 0; i < SimpleControl.compInfo.numComponents; i++) {
                switch (SimpleControl.compInfo.frameComp[i]) {
                case 0:
                    if (!VideoDecode(ptr)) {
                        return 1;
                    }
                    break;
                }
                ptr += *compSizePtr;
                compSizePtr++;
            }
        }
        SimpleControl.readBuffer[SimpleControl.nextDecodeIndex].isValid = 0;
        SimpleControl.nextDecodeIndex =
            SimpleControl.nextDecodeIndex + 1 >= 10 ? 0 : SimpleControl.nextDecodeIndex + 1;
        old = OSDisableInterrupts();
        if (!SimpleControl.readBuffer[SimpleControl.readIndex].isValid && !SimpleControl.readProgress &&
            !SimpleControl.readError && SimpleControl.preFetchState == 1) {
            ReadFrameAsync();
        }
        OSRestoreInterrupts(old);
        return 0;
    }
    return 2;
}

// THPSimpleDrawCurrentFrame
s32 fn_805B6474(void* rmode, u32 x, u32 y, u32 polygonW, u32 polygonH) {
    if (SimpleControl.textureSet.frameNumber >= 0) {
        fn_805B4F08(rmode);
        fn_805B53CC(SimpleControl.textureSet.ytexture, SimpleControl.textureSet.utexture,
                    SimpleControl.textureSet.vtexture, (s16)x, (s16)y, (s16)SimpleControl.videoInfo.xSize,
                    (s16)SimpleControl.videoInfo.ySize, (s16)polygonW, (s16)polygonH);
        fn_805B4DF0();
        return SimpleControl.textureSet.frameNumber;
    }
    return -1;
}

// NONMATCHING: 97.62% - registers only: the original keeps SimpleControl in r6 and VolumeTable in
// r11 (here r7 and r6) and every temporary shifts by one. Tried the order of the locals and the
// table access form.
// inferred: MixAudio of the SDK sample, with the mono switch of the game
static void fn_805B6504(s16* destination, s16* source, u32 sample) {
    u32 sampleNum;
    u32 i;
    s16* src;
    s32 mixR;
    s32 mixL;
    u16 attenuation;

    if (source) {
        if (SimpleControl.open && SimpleControl.audioState == 1 && SimpleControl.audioExist) {
            while (1) {
                if (SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample) {
                    if (SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample >= sample) {
                        sampleNum = sample;
                    } else {
                        sampleNum = SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample;
                    }
                    src = SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].curPtr;
                    for (i = 0; i < sampleNum; i++) {
                        if (SimpleControl.rampCount) {
                            SimpleControl.rampCount--;
                            SimpleControl.curVolume += SimpleControl.deltaVolume;
                        } else {
                            SimpleControl.curVolume = SimpleControl.targetVolume;
                        }
                        attenuation = VolumeTable[(s32)SimpleControl.curVolume];
                        mixL = (attenuation * src[0]) >> 15;
                        mixR = (attenuation * src[1]) >> 15;
                        src += 2;
                        if (SimpleControl.soundMode == 0) {
                            mixL = (mixL + mixR + 1) >> 1;
                            mixR = mixL;
                        }
                        mixL += source[0];
                        if (mixL < -32768) {
                            mixL = -32768;
                        } else if (mixL > 32767) {
                            mixL = 32767;
                        }
                        destination[0] = mixL;
                        mixR += source[1];
                        source += 2;
                        if (mixR < -32768) {
                            mixR = -32768;
                        } else if (mixR > 32767) {
                            mixR = 32767;
                        }
                        destination[1] = mixR;
                        destination += 2;
                    }
                    sample -= sampleNum;
                    SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample -= sampleNum;
                    SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].curPtr = src;
                    if (SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample == 0) {
                        SimpleControl.audioOutputIndex++;
                        if (SimpleControl.audioOutputIndex >= 3) {
                            SimpleControl.audioOutputIndex = 0;
                        }
                    }
                    if (sample == 0) {
                        break;
                    }
                } else {
                    memcpy(destination, source, sample << 2);
                    break;
                }
            }
        } else {
            memcpy(destination, source, sample << 2);
        }
    } else {
        if (SimpleControl.open && SimpleControl.audioState == 1 && SimpleControl.audioExist) {
            while (1) {
                if (SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample) {
                    if (SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample >= sample) {
                        sampleNum = sample;
                    } else {
                        sampleNum = SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample;
                    }
                    src = SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].curPtr;
                    for (i = 0; i < sampleNum; i++) {
                        if (SimpleControl.rampCount) {
                            SimpleControl.rampCount--;
                            SimpleControl.curVolume += SimpleControl.deltaVolume;
                        } else {
                            SimpleControl.curVolume = SimpleControl.targetVolume;
                        }
                        attenuation = VolumeTable[(s32)SimpleControl.curVolume];
                        mixL = (attenuation * src[0]) >> 15;
                        mixR = (attenuation * src[1]) >> 15;
                        src += 2;
                        if (SimpleControl.soundMode == 0) {
                            mixL = (mixL + mixR + 1) >> 1;
                            mixR = mixL;
                        }
                        if (mixL < -32768) {
                            mixL = -32768;
                        } else if (mixL > 32767) {
                            mixL = 32767;
                        }
                        destination[0] = mixL;
                        if (mixR < -32768) {
                            mixR = -32768;
                        } else if (mixR > 32767) {
                            mixR = 32767;
                        }
                        destination[1] = mixR;
                        destination += 2;
                    }
                    sample -= sampleNum;
                    SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample -= sampleNum;
                    SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].curPtr = src;
                    if (SimpleControl.audioBuffer[SimpleControl.audioOutputIndex].validSample == 0) {
                        SimpleControl.audioOutputIndex++;
                        if (SimpleControl.audioOutputIndex >= 3) {
                            SimpleControl.audioOutputIndex = 0;
                        }
                    }
                    if (sample == 0) {
                        break;
                    }
                } else {
                    memset(destination, 0, sample << 2);
                    break;
                }
            }
        } else {
            memset(destination, 0, sample << 2);
        }
    }
}

// THPSimpleGetVideoInfo
BOOL fn_805B68A0(THPVideoInfo* videoInfo) {
    if (SimpleControl.open) {
        memcpy(videoInfo, &SimpleControl.videoInfo, sizeof(THPVideoInfo));
        return 1;
    }
    return 0;
}

// THPSimpleGetAudioInfo
BOOL fn_805B68E8(THPAudioInfo* audioInfo) {
    if (SimpleControl.open) {
        memcpy(audioInfo, &SimpleControl.audioInfo, sizeof(THPAudioInfo));
        return 1;
    }
    return 0;
}

// THPSimpleGetFrameRate
f32 fn_805B6930(void) {
    if (SimpleControl.open) {
        return SimpleControl.header.frameRate;
    }
    return 0.0f;
}

// THPSimpleGetTotalFrame
u32 fn_805B6954(void) {
    if (SimpleControl.open) {
        return SimpleControl.header.numFrames;
    }
    return 0;
}

// inferred: THPAudioMixCallback of the SDK sample (AI DMA callback)
static void fn_805B6978(void) {
    BOOL old;

    if (AudioSystem == 0) {
        SoundBufferIndex ^= 1;
        AIInitDMA((u32)SoundBuffer[SoundBufferIndex], sizeof(SoundBuffer[0]));
        old = OSEnableInterrupts();
        fn_805B6504(SoundBuffer[SoundBufferIndex], 0, 0x60);
        DCFlushRange(SoundBuffer[SoundBufferIndex], sizeof(SoundBuffer[0]));
        OSRestoreInterrupts(old);
    } else {
        if (AudioSystem == 1) {
            if (LastAudioBuffer) {
                CurAudioBuffer = LastAudioBuffer;
            }
            OldAIDCallback();
            LastAudioBuffer = (s16*)(AIGetDMAStartAddr() + 0x80000000);
        } else {
            OldAIDCallback();
            CurAudioBuffer = (s16*)(AIGetDMAStartAddr() + 0x80000000);
        }
        SoundBufferIndex ^= 1;
        AIInitDMA((u32)SoundBuffer[SoundBufferIndex], sizeof(SoundBuffer[0]));
        old = OSEnableInterrupts();
        if (CurAudioBuffer) {
            DCInvalidateRange(CurAudioBuffer, sizeof(SoundBuffer[0]));
        }
        fn_805B6504(SoundBuffer[SoundBufferIndex], CurAudioBuffer, 0x60);
        DCFlushRange(SoundBuffer[SoundBufferIndex], sizeof(SoundBuffer[0]));
        OSRestoreInterrupts(old);
    }
}

// THPSimpleSetVolume
BOOL fn_805B6AE0(s32 vol, s32 time) {
    BOOL old;
    s32 samplePerMs;

    if (SimpleControl.open && SimpleControl.audioExist) {
        u32 rate = AIGetDSPSampleRate();
        samplePerMs = 48;
        if (rate == 0) {
            samplePerMs = 32;
        }
        if (vol > 127) {
            vol = 127;
        }
        if (vol < 0) {
            vol = 0;
        }
        if (time > 60000) {
            time = 60000;
        }
        if (time < 0) {
            time = 0;
        }
        old = OSDisableInterrupts();
        SimpleControl.targetVolume = vol;
        if (time) {
            SimpleControl.rampCount = samplePerMs * time;
            SimpleControl.deltaVolume =
                (SimpleControl.targetVolume - SimpleControl.curVolume) / SimpleControl.rampCount;
        } else {
            SimpleControl.rampCount = 0;
            SimpleControl.curVolume = SimpleControl.targetVolume;
        }
        OSRestoreInterrupts(old);
        return 1;
    }
    return 0;
}

// THPSimpleGetVolume
s32 fn_805B6C0C(void) {
    if (SimpleControl.open) {
        return SimpleControl.curVolume;
    }
    return -1;
}

// inferred: selects stereo (1) or mono (0) output, called by atn::VideoTHP with the AudioDevice
// output mode
void fn_805B6C44(u32 mode) {
    if (mode > 1) {
        return;
    }
    SimpleControl.soundMode = mode;
}

} // extern "C"
