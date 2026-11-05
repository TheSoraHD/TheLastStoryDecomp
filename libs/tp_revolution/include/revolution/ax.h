#ifndef _REVOLUTION_AX_H_
#define _REVOLUTION_AX_H_

#include <revolution/types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define AX_SAMPLE_RATE 32000
#define AX_SAMPLES_PER_FRAME 96
#define AX_SAMPLE_DEPTH_BYTES sizeof(u32)
#define AX_SAMPLES_PER_FRAME_RMT 18
#define AX_FRAME_SIZE (AX_SAMPLES_PER_FRAME * AX_SAMPLE_DEPTH_BYTES)
#define AX_MAX_VOLUME 32768

#define AX_VOICE_STOP 0
#define AX_VOICE_RUN  1

// Wii layout of the parameter block (libs/RVL_SDK/include/revolution/ax/AXPB.h): the TP copy had the
// GameCube one (u16 mixerCtrl, no remote fields), which the homebuttonLib sound sources do not match.
typedef struct _AXPBUPDATE {
    /* 0x00 */ u16 updNum[5];
    /* 0x0A */ u16 dataHi;
    /* 0x0C */ u16 dataLo;
} AXPBUPDATE;

typedef struct _AXPBFIR {
    /* 0x00 */ u16 numCoefs;
    /* 0x02 */ u16 coefsHi;
    /* 0x04 */ u16 coefsLo;
} AXPBFIR;

typedef struct _AXPBMIX {
    u16 vL;          // at 0x0
    u16 vDeltaL;     // at 0x2
    u16 vR;          // at 0x4
    u16 vDeltaR;     // at 0x6
    u16 vAuxAL;      // at 0x8
    u16 vDeltaAuxAL; // at 0xA
    u16 vAuxAR;      // at 0xC
    u16 vDeltaAuxAR; // at 0xE
    u16 vAuxBL;      // at 0x10
    u16 vDeltaAuxBL; // at 0x12
    u16 vAuxBR;      // at 0x14
    u16 vDeltaAuxBR; // at 0x16
    u16 vAuxCL;      // at 0x18
    u16 vDeltaAuxCL; // at 0x1A
    u16 vAuxCR;      // at 0x1C
    u16 vDeltaAuxCR; // at 0x1E
    u16 vS;          // at 0x20
    u16 vDeltaS;     // at 0x22
    u16 vAuxAS;      // at 0x24
    u16 vDeltaAuxAS; // at 0x26
    u16 vAuxBS;      // at 0x28
    u16 vDeltaAuxBS; // at 0x2A
    u16 vAuxCS;      // at 0x2C
    u16 vDeltaAuxCS; // at 0x2E
} AXPBMIX;

typedef struct _AXPBITD {
    u16 flag;         // at 0x0
    u16 bufferHi;     // at 0x2
    u16 bufferLo;     // at 0x4
    u16 shiftL;       // at 0x6
    u16 shiftR;       // at 0x8
    u16 targetShiftL; // at 0xA
    u16 targetShiftR; // at 0xC
} AXPBITD;

typedef struct _AXPBDPOP {
    s16 aL;     // at 0x0
    s16 aAuxAL; // at 0x2
    s16 aAuxBL; // at 0x4
    s16 aAuxCL; // at 0x6
    s16 aR;     // at 0x8
    s16 aAuxAR; // at 0xA
    s16 aAuxBR; // at 0xC
    s16 aAuxCR; // at 0xE
    s16 aS;     // at 0x10
    s16 aAuxAS; // at 0x12
    s16 aAuxBS; // at 0x14
    s16 aAuxCS; // at 0x16
} AXPBDPOP;

typedef struct _AXPBVE {
    u16 currentVolume; // at 0x0
    s16 currentDelta;  // at 0x2
} AXPBVE;

typedef struct _AXPBADDR {
    u16 loopFlag;         // at 0x0
    u16 format;           // at 0x2
    u16 loopAddressHi;    // at 0x4
    u16 loopAddressLo;    // at 0x6
    u16 endAddressHi;     // at 0x8
    u16 endAddressLo;     // at 0xA
    u16 currentAddressHi; // at 0xC
    u16 currentAddressLo; // at 0xE
} AXPBADDR;

typedef struct _AXPBADPCM {
    u16 a[8][2];    // at 0x0
    u16 gain;       // at 0x20
    u16 pred_scale; // at 0x22
    u16 yn1;        // at 0x24
    u16 yn2;        // at 0x26
} AXPBADPCM;

typedef struct _AXPBSRC {
    u16 ratioHi;            // at 0x0
    u16 ratioLo;            // at 0x2
    u16 currentAddressFrac; // at 0x4
    u16 last_samples[4];    // at 0x6
} AXPBSRC;

typedef struct _AXPBADPCMLOOP {
    u16 loop_pred_scale; // at 0x0
    u16 loop_yn1;        // at 0x2
    u16 loop_yn2;        // at 0x4
} AXPBADPCMLOOP;

typedef struct _AXPBLPF {
    u16 on;  // at 0x0
    u16 yn1; // at 0x2
    u16 a0;  // at 0x4
    u16 b0;  // at 0x6
} AXPBLPF;

typedef struct _AXPBBIQUAD {
    u16 on;  // at 0x0
    u16 xn1; // at 0x2
    u16 xn2; // at 0x4
    u16 yn1; // at 0x6
    u16 yn2; // at 0x8
    u16 b0;  // at 0xA
    u16 b1;  // at 0xC
    u16 b2;  // at 0xE
    u16 a1;  // at 0x10
    u16 a2;  // at 0x12
} AXPBBIQUAD;

typedef struct _AXPBRMTMIX {
    u16 vMain0;      // at 0x0
    u16 vDeltaMain0; // at 0x2
    u16 vAux0;       // at 0x4
    u16 vDeltaAux0;  // at 0x6
    u16 vMain1;      // at 0x8
    u16 vDeltaMain1; // at 0xA
    u16 vAux1;       // at 0xC
    u16 vDeltaAux1;  // at 0xE
    u16 vMain2;      // at 0x10
    u16 vDeltaMain2; // at 0x12
    u16 vAux2;       // at 0x14
    u16 vDeltaAux2;  // at 0x16
    u16 vMain3;      // at 0x18
    u16 vDeltaMain3; // at 0x1A
    u16 vAux3;       // at 0x1C
    u16 vDeltaAux3;  // at 0x1E
} AXPBRMTMIX;

typedef struct _AXPBRMTDPOP {
    s16 aMain0; // at 0x0
    s16 aMain1; // at 0x2
    s16 aMain2; // at 0x4
    s16 aMain3; // at 0x6
    s16 aAux0;  // at 0x8
    s16 aAux1;  // at 0xA
    s16 aAux2;  // at 0xC
    s16 aAux3;  // at 0xE
} AXPBRMTDPOP;

typedef struct _AXPBRMTSRC {
    u16 currentAddressFrac; // at 0x0
    u16 last_samples[4];    // at 0x2
} AXPBRMTSRC;

typedef union __AXPBRMTIIR {
    AXPBLPF lpf;
    AXPBBIQUAD biquad;
} AXPBRMTIIR;

typedef struct _AXPB {
    u16 nextHi;                // at 0x0
    u16 nextLo;                // at 0x2
    u16 currHi;                // at 0x4
    u16 currLo;                // at 0x6
    u16 srcSelect;             // at 0x8
    u16 coefSelect;            // at 0xA
    u32 mixerCtrl;             // at 0xC
    u16 state;                 // at 0x10
    u16 type;                  // at 0x12
    AXPBMIX mix;               // at 0x14
    AXPBITD itd;               // at 0x44
    AXPBDPOP dpop;             // at 0x52
    AXPBVE ve;                 // at 0x6A
    AXPBADDR addr;             // at 0x6E
    AXPBADPCM adpcm;           // at 0x7E
    AXPBSRC src;               // at 0xA6
    AXPBADPCMLOOP adpcmLoop;   // at 0xB4
    AXPBLPF lpf;               // at 0xBA
    AXPBBIQUAD biquad;         // at 0xC2
    u16 remote;                // at 0xD6
    u16 rmtMixerCtrl;          // at 0xD8
    AXPBRMTMIX rmtMix;         // at 0xDA
    AXPBRMTDPOP rmtDpop;       // at 0xFA
    AXPBRMTSRC rmtSrc;         // at 0x10A
    AXPBRMTIIR rmtIIR;         // at 0x114
    u8 padding[0x140 - 0x128]; // at 0x128
} AXPB;

typedef struct _AXVPB {
    /* 0x000 */ void* next;
    /* 0x004 */ void* prev;
    /* 0x008 */ void* next1;
    /* 0x00C */ u32 priority;
    /* 0x010 */ void (*callback)(void*);
    /* 0x014 */ u32 userContext;
    /* 0x018 */ u32 index;
    /* 0x01C */ u32 sync;
    /* 0x020 */ u32 depop;
    /* 0x024 */ void* itdBuffer;
    /* 0x028 */ AXPB pb; // Wii layout (libs/RVL_SDK/include/revolution/ax/AXVPB.h)
} AXVPB;

typedef struct _AXPBITDBUFFER {
    /* 0x00 */ s16 data[32];
} AXPBITDBUFFER;

typedef struct _AXPBU {
    /* 0x00 */ u16 data[128];
} AXPBU;

typedef struct _AXSPB {
    /* 0x00 */ u16 dpopLHi;
    /* 0x02 */ u16 dpopLLo;
    /* 0x04 */ s16 dpopLDelta;
    /* 0x06 */ u16 dpopRHi;
    /* 0x08 */ u16 dpopRLo;
    /* 0x0A */ s16 dpopRDelta;
    /* 0x0C */ u16 dpopSHi;
    /* 0x0E */ u16 dpopSLo;
    /* 0x10 */ s16 dpopSDelta;
    /* 0x12 */ u16 dpopALHi;
    /* 0x14 */ u16 dpopALLo;
    /* 0x16 */ s16 dpopALDelta;
    /* 0x18 */ u16 dpopARHi;
    /* 0x1A */ u16 dpopARLo;
    /* 0x1C */ s16 dpopARDelta;
    /* 0x1E */ u16 dpopASHi;
    /* 0x20 */ u16 dpopASLo;
    /* 0x22 */ s16 dpopASDelta;
    /* 0x24 */ u16 dpopBLHi;
    /* 0x26 */ u16 dpopBLLo;
    /* 0x28 */ s16 dpopBLDelta;
    /* 0x2A */ u16 dpopBRHi;
    /* 0x2C */ u16 dpopBRLo;
    /* 0x2E */ s16 dpopBRDelta;
    /* 0x30 */ u16 dpopBSHi;
    /* 0x32 */ u16 dpopBSLo;
    /* 0x34 */ s16 dpopBSDelta;
} AXSPB;

typedef struct _AXPROFILE {
    /* 0x00 */ u64 axFrameStart;
    /* 0x08 */ u64 auxProcessingStart;
    /* 0x10 */ u64 auxProcessingEnd;
    /* 0x18 */ u64 userCallbackStart;
    /* 0x20 */ u64 userCallbackEnd;
    /* 0x28 */ u64 axFrameEnd;
    /* 0x30 */ u32 axNumVoices;
} AXPROFILE;

typedef struct AX_AUX_DATA {
    /* 0x00 */ s32* l;
    /* 0x00 */ s32* r;
    /* 0x00 */ s32* s;
} AX_AUX_DATA;

typedef struct AX_AUX_DATA_DPL2 {
    /* 0x00 */ s32* l;
    /* 0x00 */ s32* r;
    /* 0x00 */ s32* ls;
    /* 0x00 */ s32* rs;
} AX_AUX_DATA_DPL2;

typedef void (*AXCallback)();
typedef void (*AXAuxCallback)(void* chans, void* context);

#define AX_DSP_SLAVE_LENGTH 0xF80
#define AX_MAX_VOICES 64

#define AX_SRC_TYPE_NONE     0
#define AX_SRC_TYPE_LINEAR   1
#define AX_SRC_TYPE_4TAP_8K  2
#define AX_SRC_TYPE_4TAP_12K 3
#define AX_SRC_TYPE_4TAP_16K 4

// sync flags
#define AX_SYNC_FLAG_COPYALL       (1 << 31)
#define AX_SYNC_FLAG_UNK1          (1 << 30) // reserved, unused?
#define AX_SYNC_FLAG_UNK2          (1 << 29) // reserved, unused?
#define AX_SYNC_FLAG_UNK3          (1 << 28) // reserved, unused?
#define AX_SYNC_FLAG_UNK4          (1 << 27) // reserved, unused?
#define AX_SYNC_FLAG_UNK5          (1 << 26) // reserved, unused?
#define AX_SYNC_FLAG_UNK6          (1 << 25) // reserved, unused?
#define AX_SYNC_FLAG_UNK7          (1 << 24) // reserved, unused?
#define AX_SYNC_FLAG_UNK8          (1 << 23) // reserved, unused?
#define AX_SYNC_FLAG_UNK9          (1 << 22) // reserved, unused?
#define AX_SYNC_FLAG_UNK10         (1 << 21) // reserved, unused?
#define AX_SYNC_FLAG_COPYADPCMLOOP (1 << 20)
#define AX_SYNC_FLAG_COPYRATIO     (1 << 19)
#define AX_SYNC_FLAG_COPYSRC       (1 << 18)
#define AX_SYNC_FLAG_COPYADPCM     (1 << 17)
#define AX_SYNC_FLAG_COPYCURADDR   (1 << 16)
#define AX_SYNC_FLAG_COPYENDADDR   (1 << 15)
#define AX_SYNC_FLAG_COPYLOOPADDR  (1 << 14)
#define AX_SYNC_FLAG_COPYLOOP      (1 << 13)
#define AX_SYNC_FLAG_COPYADDR      (1 << 12)
#define AX_SYNC_FLAG_COPYFIR       (1 << 11)
#define AX_SYNC_FLAG_SWAPVOL       (1 << 10)
#define AX_SYNC_FLAG_COPYVOL       (1 << 9)
#define AX_SYNC_FLAG_COPYDPOP      (1 << 8)
#define AX_SYNC_FLAG_COPYUPDATE    (1 << 7)
#define AX_SYNC_FLAG_COPYTSHIFT    (1 << 6)
#define AX_SYNC_FLAG_COPYITD       (1 << 5)
#define AX_SYNC_FLAG_COPYAXPBMIX   (1 << 4)
#define AX_SYNC_FLAG_COPYTYPE      (1 << 3)
#define AX_SYNC_FLAG_COPYSTATE     (1 << 2)
#define AX_SYNC_FLAG_COPYMXRCTRL   (1 << 1)
#define AX_SYNC_FLAG_COPYSELECT    (1 << 0)

#define AX_PRIORITY_STACKS 32

// AX
void AXInit(void);
void AXInitEx(u32 outputBufferMode);
void AXQuit(void);

// AXAlloc
void AXFreeVoice(AXVPB* p);
AXVPB* AXAcquireVoice(u32 priority, void (*callback)(void *), u32 userContext);
void AXSetVoicePriority(AXVPB* p, u32 priority);

// AXAux
void AXRegisterAuxACallback(AXAuxCallback callback, void* context);
void AXRegisterAuxBCallback(void (*callback)(void*, void*), void* context);

// AXCL
void AXSetMode(u32 mode);
u32 AXGetMode(void);

// AXOut
extern AXPROFILE __AXLocalProfile;

void AXSetStepMode(u32 i);
AXCallback AXRegisterCallback(AXCallback callback);

// AXProf
void AXInitProfile(AXPROFILE* profile, u32 maxProfiles);
u32 AXGetProfile(void);

// AXVPB
void AXSetVoiceSrcType(AXVPB* p, u32 type);
void AXSetVoiceState(AXVPB* p, u16 state);
void AXSetVoiceType(AXVPB* p, u16 type);
void AXSetVoiceMix(AXVPB* p, AXPBMIX* mix);
void AXSetVoiceItdOn(AXVPB* p);
void AXSetVoiceItdTarget(AXVPB* p, u16 lShift, u16 rShift);
void AXSetVoiceUpdateIncrement(AXVPB* p);
void AXSetVoiceUpdateWrite(AXVPB* p, u16 param, u16 data);
void AXSetVoiceDpop(AXVPB* p, AXPBDPOP* dpop);
void AXSetVoiceVe(AXVPB* p, AXPBVE* ve);
void AXSetVoiceVeDelta(AXVPB* p, s16 delta);
void AXSetVoiceFir(AXVPB* p, AXPBFIR* fir);
void AXSetVoiceAddr(AXVPB* p, AXPBADDR* addr);
void AXSetVoiceLoop(AXVPB* p, u16 loop);
void AXSetVoiceLoopAddr(AXVPB* p, u32 addr);
void AXSetVoiceEndAddr(AXVPB* p, u32 addr);
void AXSetVoiceCurrentAddr(AXVPB* p, u32 addr);
void AXSetVoiceAdpcm(AXVPB* p, AXPBADPCM* adpcm);
void AXSetVoiceSrc(AXVPB* p, AXPBSRC* src_);
void AXSetVoiceSrcRatio(AXVPB* p, f32 ratio);
void AXSetVoiceAdpcmLoop(AXVPB* p, AXPBADPCMLOOP* adpcmloop);
void AXSetMaxDspCycles(u32 cycles);
u32 AXGetMaxDspCycles(void);
u32 AXGetDspCycles(void);
void AXSetVoiceLpf(AXVPB* p, AXPBLPF* lpf);
void AXSetVoiceLpfCoefs(AXVPB* p, u16 a0, u16 b0);
void AXGetLpfCoefs(u16 freq, u16* a0, u16* b0);

void AXGetAuxACallback(AXAuxCallback* cbOut, void** contextOut);
u16 AXGetAuxAReturnVolume(void);
u16 AXGetAuxBReturnVolume(void);
u16 AXGetAuxCReturnVolume(void);
void AXSetAuxAReturnVolume(u16 volume);
void AXSetAuxBReturnVolume(u16 volume);
void AXSetAuxCReturnVolume(u16 volume);
void AXSetMasterVolume(u16 volume);
u16 AXGetMasterVolume(void);

// DSPCode
extern u16 axDspSlaveLength;
extern u16 axDspSlave[AX_DSP_SLAVE_LENGTH];

#ifdef __cplusplus
}
#endif

#endif // _REVOLUTION_AX_H_
