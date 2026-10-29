// Copyright (c) TheSoraHD. All rights reserved.
// Original file name: atn_graphics_device.cpp (string of OSPanic).

#include "atn/GraphicsDevice.h"
#include "atn/Texture_unk.h"
#include "atn/Memory.h"
#include "atn/RenderSetting_unk.h"

extern "C" {

void GXSetProjection(const float projection[4][4], int type);
void GXSetScissor(unsigned int left, unsigned int top, unsigned int width, unsigned int height);
void GXSetViewport(float left, float top, float width, float height, float nearZ, float farZ);
void GXSetCopyFilter(unsigned char antialias, const unsigned char samples[12][2], unsigned char vertical,
                     const unsigned char filter[7]);
void GXLoadTexObj(const void* obj, unsigned int map);
int GXGetTexObjFmt(const GXTexObj* obj);
void GXSetTexCopySrc(unsigned short left, unsigned short top, unsigned short width, unsigned short height);
void GXSetTexCopyDst(unsigned short width, unsigned short height, int format, unsigned char mipmap);
void GXCopyTex(void* destination, unsigned char clear);
void GXPixModeSync();
void GXSetCopyClear(GXColor color, unsigned int z);
void GXInvalidateVtxCache();
void GXInvalidateTexAll();
void fn_80607120(int gamma); // inferred: VISetGamma
void GXFlush();
void GXGetCPUFifo(GXFifoObj* fifo);
void GXGetFifoPtrs(GXFifoObj* fifo, void** readPtr, void** writePtr);
void GXEnableBreakPt(void* breakPt);
void GXSetDrawSync(unsigned short token);
void GXResetOverflowCount();
void GXDisableBreakPt();
unsigned short fn_806140B0(); // inferred: GXReadDrawSync
void GXSetColorUpdate(unsigned char enable);
void GXSetAlphaUpdate(unsigned char enable);
void GXClearVtxDesc();
void GXSetLineWidth(unsigned char width, int texOffsets);
void GXSetPointSize(unsigned char size, int texOffsets);
void GXEnableTexOffsets(int coord, unsigned char line, unsigned char point);
void GXSetTexCoordScaleManually(int coord, unsigned char enable, unsigned short s, unsigned short t);
void fn_806168C0(int coord, unsigned char s, unsigned char t); // inferred: GXSetTexCoordCylWrap
void GXLoadPosMtxImm(const float mtx[3][4], unsigned int id);
void GXLoadNrmMtxImm(const float mtx[3][4], unsigned int id);
void GXSetCurrentMtx(unsigned int id);
void GXSetCoPlanar(unsigned char enable);
void GXSetCullMode(int mode);
void GXSetClipMode(int mode);
void GXSetScissorBoxOffset(int x, int y);
void GXSetChanAmbColor(int chan, GXColor color);
void GXSetChanMatColor(int chan, GXColor color);
void* GXSetTexRegionCallback(void* callback);
void GXSetAlphaCompare(int comp0, unsigned char ref0, int op, int comp1, unsigned char ref1);
void GXSetZTexture(int op, int fmt, unsigned int bias);
void GXSetTevKAlphaSel(int stage, int sel);
void GXSetIndTexCoordScale(int stage, int scaleS, int scaleT);
void GXSetFog(int type, float startZ, float endZ, float nearZ, float farZ, GXColor color);
void GXSetFogRangeAdj(unsigned char enable, unsigned short center, void* table);
void GXSetZCompLoc(unsigned char beforeTex);
void GXSetDither(unsigned char dither);
void GXSetDstAlpha(unsigned char enable, unsigned char alpha);
int OSDisableInterrupts();
int OSRestoreInterrupts(int level);
GXFifoObj* GXInit(void* base, unsigned int size);
void fn_80614B20(const GXRenderModeObj* rmin, GXRenderModeObj* rmout, unsigned short hor,
                 unsigned short ver); // inferred: GXAdjustForOverscan
float GXGetYScaleFactor(unsigned short efbHeight, unsigned short xfbHeight);
unsigned int GXSetDispCopyYScale(float yscale);
void GXSetDispCopySrc(unsigned short left, unsigned short top, unsigned short width, unsigned short height);
void GXSetDispCopyDst(unsigned short width, unsigned short height);
void GXSetDispCopyGamma(int gamma);
void GXSetPixelFmt(int pixFmt, int zFmt);
void GXCopyDisp(void* dest, unsigned char clear);
void* GXSetBreakPtCallback(void (*callback)());
unsigned char SCGetProgressiveMode();
unsigned char SCGetEuRgb60Mode();
unsigned char SCGetAspectRatio();
unsigned int VIGetTvFormat();
unsigned int fn_806052C0(); // inferred: VIGetDTVStatus
void VIConfigure(const GXRenderModeObj* rm);
void VISetNextFrameBuffer(void* fb);
void VIFlush();
void VIWaitForRetrace();
void VISetBlack(int black);
void* VISetPreRetraceCallback(void (*callback)(unsigned int));
void* VISetPostRetraceCallback(void (*callback)(unsigned int));
void OSPanic(const char* file, int line, const char* msg, ...);
void* memset(void* dst, int value, unsigned long size);
}

struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void* fn_800838B8(MemoryManager_unk* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line); // inferred: allocation of a fixed block
extern "C" void* fn_800839EC(MemoryManager_unk* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line, int arg7);

// SDK render modes
extern "C" GXRenderModeObj lbl_807B093C; // GXNtsc480IntDf
extern "C" GXRenderModeObj lbl_807B0978; // GXNtsc480Prog
extern "C" GXRenderModeObj lbl_807B09F0; // GXMpal480IntDf
extern "C" GXRenderModeObj lbl_807B0A2C; // GXMpal480Prog
extern "C" GXRenderModeObj lbl_807B0AA4; // GXPal528IntDf
extern "C" GXRenderModeObj lbl_807B0B1C; // GXEurgb60Hz480IntDf
extern "C" GXRenderModeObj lbl_807B0B58; // GXEurgb60Hz480Prog

struct OSThreadQueue {
    void* head;
    void* tail;
};
extern "C" void OSInitThreadQueue(OSThreadQueue* queue);
extern "C" void OSSleepThread(OSThreadQueue* queue);
extern "C" void OSWakeupThread(OSThreadQueue* queue);

// inferred: frame submitted to the GPU and waiting for its copy to the XFB
struct FrameEntry_unk {
    void* mFifoPtr;       // 0x00: write pointer of the FIFO at the end of the frame (break point)
    int m04;
    void* mXfb;           // 0x08
    unsigned short mToken; // 0x0C: draw sync token
};

// inferred: circular queue of the frames in flight (5 entries; mTail == 5: empty)
struct FrameQueue_unk {
    FrameEntry_unk mEntries[5]; // 0x00
    unsigned short mTail;       // 0x50
    unsigned short mHead;       // 0x52

};

// inferred: adds a frame at the tail of the queue
static void PushFrame_unk(FrameQueue_unk* queue, void* fifoPtr, void* xfb, unsigned short token) {
    if (queue->mTail == 5) {
        queue->mHead = 0;
        queue->mTail = 0;
    } else {
        queue->mTail = (queue->mTail + 1) % 5;
        if (queue->mTail == queue->mHead) {
            OSPanic("atn_graphics_device.cpp", 1019, "queue overflow");
        }
    }
    FrameEntry_unk& entry = queue->mEntries[queue->mTail];
    entry.mFifoPtr = fifoPtr;
    entry.m04 = 0;
    entry.mXfb = xfb;
    entry.mToken = token;
}

// inferred: removes the frame at the head of the queue
static void PopFrame_unk(FrameQueue_unk* queue) {
    if (queue->mTail == 5) {
        OSPanic("atn_graphics_device.cpp", 1032, "queue underflow");
    }
    if (queue->mHead == queue->mTail) {
        queue->mTail = 5;
    } else {
        queue->mHead = (queue->mHead + 1) % 5;
    }
}

// inferred: frame at the head of the queue
static FrameEntry_unk TopFrame_unk(FrameQueue_unk* queue) {
    if (queue->mTail == 5) {
        OSPanic("atn_graphics_device.cpp", 1051, "queue_top: queue empty");
    }
    return queue->mEntries[queue->mHead];
}

// inferred: number of frames in the queue
static unsigned int CountFrames_unk(FrameQueue_unk* queue) {
    if (queue->mTail == 5) {
        return 0;
    }
    unsigned int size;
    if (queue->mTail >= queue->mHead) {
        size = queue->mTail - queue->mHead;
    } else {
        size = queue->mTail + 5 - queue->mHead;
    }
    return size + 1;
}

extern "C" RenderSetting_unk* lbl_8087EFA8;

int lbl_8087D73C = 10; // inferred: gamma set on the VI (fn_8007192C)

OSThreadQueue lbl_807C6F60;  // inferred: threads waiting for a free frame
FrameQueue_unk lbl_807C6F68; // inferred: frames in flight
unsigned short lbl_8087D738 = 1; // inferred: next draw sync token
unsigned char lbl_8087EED8;      // inferred: break point enabled
unsigned char lbl_8087EED9;      // inferred: the finished frame waits for its XFB to be displayed
unsigned char lbl_8087EEDA;      // inferred: the XFB was swapped in the last retrace
unsigned short lbl_8087EEDC;     // inferred: draw sync token of the displayed frame

extern "C" void fn_80072BDC();
extern "C" void fn_80074EC8(atn::GraphicsDevice_unk* device, void* desc, unsigned int flags, void* light, void* shadow,
                            void* arg5);
extern "C" void fn_800761A8(atn::GraphicsDevice_unk* device);
extern "C" void* fn_80076FD4(void* texObj, int map); // inferred: texture region callback of the caches
extern "C" void fn_8009BF3C(void* mesh, void* material, void* lens, const float* color);

// inferred: texture caches (0x1AC bytes) selected by RenderSetting_unk::m3A8 (CacheMode)
struct TexCache_unk {
    unsigned char m000[0x1AC];
};
TexCache_unk lbl_807C6C08;
TexCache_unk lbl_807C6DB4;
extern "C" void fn_80072D74(unsigned int retraceCount);
extern "C" void fn_80072E08(unsigned int retraceCount);

void operator delete(void* ptr);

using atn::GraphicsDevice_unk;
using atn::VtxFormat_unk;

namespace atn {

VtxFormat_unk::VtxFormat_unk(const GXVtxDescList* desc, const GXVtxAttrFmtList* fmt) {
    for (int i = 0; i < 27; i++) {
        mAttrFmt[i] = fmt[i];
    }
    for (int i = 0; i < 27; i++) {
        mDesc[i] = desc[i];
    }
}
} // namespace atn

// inferred: same vertex format (attribute formats, then descriptors)
// NONMATCHING: 65.57% - loop/control-flow and register allocation differ from the original.
extern "C" int fn_80071088(const VtxFormat_unk* a, const VtxFormat_unk* b) {
    for (int i = 0; i < 27; i++) {
        const GXVtxAttrFmtList* x = &a->mAttrFmt[i];
        const GXVtxAttrFmtList* y = &b->mAttrFmt[i];
        if (x->attr != y->attr || x->cnt != y->cnt || x->type != y->type || x->frac != y->frac) {
            return 0;
        }
    }
    for (int i = 0; i < 27; i++) {
        if (a->mDesc[i].attr != b->mDesc[i].attr || a->mDesc[i].type != b->mDesc[i].type) {
            return 0;
        }
    }
    return 1;
}

extern "C" GraphicsDevice_unk* lbl_8087EEE0;

// inferred: creates the device (game/main)
extern "C" void fn_80071220() {
    if (lbl_8087EEE0 == 0) {
        lbl_8087EEE0 = new (6, "", "", 0) GraphicsDevice_unk();
    }
}

namespace atn {

GraphicsDevice_unk::GraphicsDevice_unk()
    : mWidth(0), mHeight(0), mWide(0), mClearColor(0xFF000000), mClearZ(0x00FFFFFF), mFifo(0), mFifoBuffer(0),
      mOffsetX(0.0f), mOffsetY(0.0f), mViewX(0), mViewY(0), mViewW(0), mViewH(0), mXfb0(0), mXfb1(0),
      mCurrentXfb(0), mNextXfb(0), mC0(0), mC4(0), mC8(0), mDirty(0), mDC(0), m8E0(0), m8E4(0) {}
} // namespace atn

// inferred: stores the viewport/scissor rectangle
inline void SetViewRect_unk(GraphicsDevice_unk* device, int x, int y, int w, int h) {
    device->mViewX = x;
    device->mViewY = y;
    device->mViewW = w;
    device->mViewH = h;
}

// inferred: sets the viewport and the scissor to a rectangle of the EFB (offset by mOffsetX/Y)
static void SetViewport_unk(GraphicsDevice_unk* device, int x, int y, int w, int h) {
    device->mViewX = x;
    device->mViewY = y;
    device->mViewW = w;
    device->mViewH = h;
    GXSetViewport(device->mOffsetX + x, device->mOffsetY + y, w + device->mOffsetX, h + device->mOffsetY, 0.0f,
                  1.0f);
    GXSetScissor(device->mOffsetX + device->mViewX, device->mOffsetY + device->mViewY,
                 device->mViewW + device->mOffsetX, device->mViewH + device->mOffsetY);
}

// inferred: video and GX initialization
extern "C" int fn_800713D4(GraphicsDevice_unk* device, int width, int height) {
    device->mWidth = width;
    device->mHeight = height;
    SetViewRect_unk(device, 0, 0, 0, 0);
    GXRenderModeObj* rm = 0;
    if (SCGetProgressiveMode() == 1 && fn_806052C0() == 1) {
        switch (VIGetTvFormat()) {
        case 0:
            rm = &lbl_807B0978;
            break;
        case 1:
        case 5:
            rm = &lbl_807B0B58;
            break;
        case 2:
            rm = &lbl_807B0A2C;
            break;
        default:
            OSPanic("atn_graphics_device.cpp", 243, "invalid TV format\n");
            break;
        }
    } else {
        switch (VIGetTvFormat()) {
        case 0:
            rm = &lbl_807B093C;
            break;
        case 1:
            if (SCGetEuRgb60Mode() == 1) {
                rm = &lbl_807B0B1C;
            } else {
                rm = &lbl_807B0AA4;
            }
            break;
        case 5:
            rm = &lbl_807B0B1C;
            break;
        case 2:
            rm = &lbl_807B09F0;
            break;
        default:
            OSPanic("atn_graphics_device.cpp", 264, "invalid TV format\n");
            break;
        }
    }
    device->mWide = SCGetAspectRatio();
    unsigned short xfbHeight = rm->xfbHeight;
    fn_80614B20(rm, &device->mRMode, 0, 16);
    device->mRMode.xfbHeight = xfbHeight;
    device->mRMode.viHeight = xfbHeight;
    device->mRMode.viYOrigin = 0;
    if (VIGetTvFormat() == 1) {
        device->mRMode.fbWidth = 640;
        device->mRMode.efbHeight = 448;
        device->mRMode.viYOrigin = rm->viYOrigin;
    }
    if (device->mWide) {
        device->mRMode.viWidth = 680;
        device->mRMode.viXOrigin = (720 - device->mRMode.viWidth) / 2;
    } else if (device->mRMode.viTVmode == 1) {
        device->mRMode.viWidth = 654;
        device->mRMode.viXOrigin = (720 - device->mRMode.viWidth) / 2;
    } else {
        device->mRMode.viWidth = 658;
        device->mRMode.viXOrigin = (720 - device->mRMode.viWidth) / 2;
    }
    device->mWidth = device->mRMode.fbWidth;
    device->mHeight = device->mRMode.efbHeight;
    unsigned long xfbSize = (unsigned short)((unsigned short)(rm->fbWidth + 15) & ~15) * rm->xfbHeight * 2;
    device->mFifoBuffer = fn_800838B8(fn_800827E0(), 0x80000, 0x20, 1, "", "", 0);
    device->mXfb0 = fn_800839EC(fn_800827E0(), xfbSize, 0x20, 6, "", "", 0, 0);
    device->mXfb1 = fn_800839EC(fn_800827E0(), xfbSize, 0x20, 6, "", "", 0, 0);
    memset(device->mFifoBuffer, 0, 0x80000);
    memset(device->mXfb0, 0, xfbSize);
    memset(device->mXfb1, 0, xfbSize);
    device->mFifo = GXInit(device->mFifoBuffer, 0x80000);
    int h = device->mHeight;
    int w = device->mWidth;
    int y = 0;
    int x = 0;
    SetViewport_unk(device, x, y, w, h);
    unsigned short lines = GXSetDispCopyYScale(GXGetYScaleFactor(device->mRMode.efbHeight, device->mRMode.xfbHeight));
    GXSetDispCopySrc(0, 0, device->mRMode.fbWidth, device->mRMode.efbHeight);
    GXSetDispCopyDst(device->mRMode.fbWidth, lines);
    GXSetCopyFilter(device->mRMode.aa, device->mRMode.sample_pattern, 0, device->mRMode.vfilter);
    GXSetDispCopyGamma(0);
    if (device->mRMode.aa) {
        GXSetPixelFmt(2, 0);
    } else {
        GXSetPixelFmt(0, 0);
    }
    GXCopyDisp(device->mXfb1, 1);
    VIConfigure(&device->mRMode);
    VISetNextFrameBuffer(device->mXfb0);
    device->mNextXfb = device->mXfb1;
    device->mCurrentXfb = device->mXfb0;
    VIFlush();
    VIWaitForRetrace();
    if (device->mRMode.viTVmode & 1) {
        VIWaitForRetrace();
    }
    VISetBlack(0);
    lbl_807C6F68.mTail = 5;
    OSInitThreadQueue(&lbl_807C6F60);
    VISetPreRetraceCallback(fn_80072D74);
    VISetPostRetraceCallback(fn_80072E08);
    GXSetBreakPtCallback(fn_80072BDC);
    return 1;
}

// inferred: ARGB word to GXColor
inline GXColor ToGXColor_unk(unsigned int argb) {
    GXColor color;
    color.b = argb;
    color.r = argb >> 16;
    color.g = argb >> 8;
    color.a = argb >> 24;
    return color;
}

// inferred: start of a frame: gamma, clear color, full viewport, cache invalidation
extern "C" void fn_8007192C(GraphicsDevice_unk* device) {
    if (lbl_8087D73C != lbl_8087EFA8->m034) {
        lbl_8087D73C = lbl_8087EFA8->m034;
        fn_80607120(lbl_8087D73C);
    }
    GXSetDispCopyGamma(lbl_8087EFA8->m030);
    GXSetCopyClear(ToGXColor_unk(device->mClearColor), device->mClearZ);
    int h = device->mHeight;
    int w = device->mWidth;
    int y = 0;
    int x = 0;
    SetViewport_unk(device, x, y, w, h);
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
}

// inferred: end of the frame: queues the copy to the XFB and waits while two frames are in flight
extern "C" void fn_80071AFC(GraphicsDevice_unk* device) {
    GXSetZMode(1, 3, 1);
    GXSetColorUpdate(1);
    GXFlush();
    GXGetCPUFifo(device->mFifo);
    void* readPtr;
    void* writePtr;
    GXGetFifoPtrs(device->mFifo, &readPtr, &writePtr);
    void* fifoPtr = writePtr;
    unsigned short token = lbl_8087D738;
    void* xfb = device->mCurrentXfb;
    int enabled = OSDisableInterrupts();
    PushFrame_unk(&lbl_807C6F68, fifoPtr, xfb, token);
    OSRestoreInterrupts(enabled);
    if (!lbl_8087EED8) {
        lbl_8087EED8 = 1;
        GXEnableBreakPt(writePtr);
    }
    GXSetDrawSync(lbl_8087D738);
    unsigned char vfilter[7];
    int sum = 0;
    for (int i = 0; i < 7; i++) {
        vfilter[i] = lbl_8087EFA8->m3B0[i];
        sum += vfilter[i];
    }
    lbl_8087EFA8->m3B0[7] = sum;
    GXSetCopyFilter(0, 0, lbl_8087EFA8->m3AC, vfilter);
    GXCopyDisp(device->mCurrentXfb, 1);
    GXFlush();
    lbl_8087D738++;
    device->mCurrentXfb = device->mCurrentXfb == device->mXfb0 ? device->mXfb1 : device->mXfb0;
    GXResetOverflowCount();
    enabled = OSDisableInterrupts();
    while (CountFrames_unk(&lbl_807C6F68) > 1) {
        OSRestoreInterrupts(enabled);
        OSSleepThread(&lbl_807C6F60);
        enabled = OSDisableInterrupts();
    }
    OSRestoreInterrupts(enabled);
}

// inferred: selects the texture cache (installs its region callback, or restores the previous one)
inline void SetTexCache_unk(GraphicsDevice_unk* device, TexCache_unk* cache) {
    if (cache != 0) {
        void* prev = GXSetTexRegionCallback(fn_80076FD4);
        if (device->m8E0 == 0) {
            device->m8E4 = (int)prev;
        }
    } else if (device->m8E4 != 0) {
        GXSetTexRegionCallback((void*)device->m8E4);
        device->m8E4 = 0;
    }
    device->m8E0 = (int)cache;
}
extern "C" void fn_80071E04(GraphicsDevice_unk* device);

// inferred: blend factors (indices of fn_800761A8); marks them dirty when they change
inline void SetBlendSrc_unk(GraphicsDevice_unk* device, int src) {
    device->mState.mBlendSrc = src;
    device->mDirty |= (device->mAppliedState.mBlendSrc != device->mState.mBlendSrc) << 4;
}
inline void SetBlendDst_unk(GraphicsDevice_unk* device, int dst) {
    device->mState.mBlendDst = dst;
    device->mDirty |= (device->mAppliedState.mBlendDst != device->mState.mBlendDst) << 5;
}
inline void SetBlendFactors_unk(GraphicsDevice_unk* device, int src, int dst) {
    SetBlendSrc_unk(device, src);
    SetBlendDst_unk(device, dst);
}
extern "C" void fn_80072914(GraphicsDevice_unk* device, int mode);
static void NextFrame_unk();
extern "C" void fn_80072BDC();
extern "C" void fn_80072D74(unsigned int retraceCount);
extern "C" void fn_80072E08(unsigned int retraceCount);

extern "C" void fn_80071D60(GraphicsDevice_unk* device, int value) {
    device->mCC = value;
}

extern "C" void fn_80071D68(GraphicsDevice_unk* device, const float projection[4][4]) {
    GXSetProjection(projection, 0);
}

extern "C" void fn_80071D74(GraphicsDevice_unk* device, const float projection[4][4]) {
    GXSetProjection(projection, 1);
}

// inferred: copies the EFB into the texture
extern "C" void fn_80071D80(GraphicsDevice_unk* device, atn::Texture_unk* texture) {
    GXSetTexCopySrc(0, 0, (unsigned short)device->mWidth, (unsigned short)device->mHeight);
    int format = GXGetTexObjFmt(&texture->mObj);
    GXSetTexCopyDst((unsigned short)device->mWidth, (unsigned short)device->mHeight, format, 0);
    GXCopyTex(texture->mImage, 0);
    GXPixModeSync();
}

// inferred: resets the GX state to the defaults of the game
extern "C" void fn_80071E04(GraphicsDevice_unk* device) {
    fn_80074EC8(device, 0, 0, 0, 0, 0);
    int h = device->mHeight;
    int w = device->mWidth;
    int y = 0;
    int x = 0;
    device->mDC = 0;
    SetViewport_unk(device, x, y, w, h);
    float white[4];
    white[0] = 1.0f;
    white[1] = 1.0f;
    white[2] = 1.0f;
    white[3] = 1.0f;
    fn_8009BF3C(0, 0, 0, white);
    device->mTexObjs[0] = 0;
    device->mTexObjs[1] = 0;
    device->mTexObjs[2] = 0;
    device->mTexObjs[3] = 0;
    device->mTexObjs[4] = 0;
    device->mTexObjs[5] = 0;
    device->mTexObjs[6] = 0;
    device->mTexObjs[7] = 0;
    device->mTexObjs[8] = 0;
    device->mTexObjs[9] = 0;
    device->mTexObjs[10] = 0;
    device->mTexObjs[11] = 0;
    device->mTexObjs[12] = 0;
    device->mTexObjs[13] = 0;
    device->mTexObjs[14] = 0;
    device->mTexObjs[15] = 0;
    GXSetTexCoordGen2(0, 1, 4, 60, 0, 125);
    GXSetTexCoordGen2(1, 1, 5, 60, 0, 125);
    GXSetTexCoordGen2(2, 1, 6, 60, 0, 125);
    GXSetTexCoordGen2(3, 1, 7, 60, 0, 125);
    GXSetTexCoordGen2(4, 1, 8, 60, 0, 125);
    GXSetTexCoordGen2(5, 1, 9, 60, 0, 125);
    GXSetTexCoordGen2(6, 1, 10, 60, 0, 125);
    GXSetTexCoordGen2(7, 1, 11, 60, 0, 125);
    GXSetNumTexGens(1);
    GXClearVtxDesc();
    GXInvalidateVtxCache();
    GXSetLineWidth(6, 0);
    GXSetPointSize(6, 0);
    GXEnableTexOffsets(0, 0, 0);
    GXEnableTexOffsets(1, 0, 0);
    GXEnableTexOffsets(2, 0, 0);
    GXEnableTexOffsets(3, 0, 0);
    GXEnableTexOffsets(4, 0, 0);
    GXEnableTexOffsets(5, 0, 0);
    GXEnableTexOffsets(6, 0, 0);
    GXEnableTexOffsets(7, 0, 0);
    GXSetTexCoordScaleManually(0, 0, 0, 0);
    fn_806168C0(0, 0, 0);
    GXSetTexCoordScaleManually(1, 0, 0, 0);
    fn_806168C0(1, 0, 0);
    GXSetTexCoordScaleManually(2, 0, 0, 0);
    fn_806168C0(2, 0, 0);
    GXSetTexCoordScaleManually(3, 0, 0, 0);
    fn_806168C0(3, 0, 0);
    GXSetTexCoordScaleManually(4, 0, 0, 0);
    fn_806168C0(4, 0, 0);
    GXSetTexCoordScaleManually(5, 0, 0, 0);
    fn_806168C0(5, 0, 0);
    GXSetTexCoordScaleManually(6, 0, 0, 0);
    fn_806168C0(6, 0, 0);
    GXSetTexCoordScaleManually(7, 0, 0, 0);
    fn_806168C0(7, 0, 0);
    float identity[3][4];
    identity[0][0] = 1.0f;
    identity[0][1] = 0.0f;
    identity[0][2] = 0.0f;
    identity[0][3] = 0.0f;
    identity[1][0] = 0.0f;
    identity[1][1] = 1.0f;
    identity[1][2] = 0.0f;
    identity[1][3] = 0.0f;
    identity[2][0] = 0.0f;
    identity[2][1] = 0.0f;
    identity[2][2] = 1.0f;
    identity[2][3] = 0.0f;
    GXLoadPosMtxImm(identity, 0);
    GXLoadNrmMtxImm(identity, 0);
    GXSetCurrentMtx(0);
    GXLoadTexMtxImm(identity, 60, 0);
    GXLoadTexMtxImm(identity, 125, 0);
    SetViewport_unk(device, x, y, device->mWidth, device->mHeight);
    GXSetCoPlanar(0);
    GXSetCullMode(2);
    GXSetClipMode(0);
    GXSetScissorBoxOffset(0, 0);
    GXSetNumChans(0);
    GXSetChanCtrl(4, 0, 0, 1, 0, 0, 2);
    GXColor amb0 = {0, 0, 0, 0};
    GXSetChanAmbColor(4, amb0);
    GXColor mat0 = {0xFF, 0xFF, 0xFF, 0xFF};
    GXSetChanMatColor(4, mat0);
    GXSetChanCtrl(5, 0, 0, 1, 0, 0, 2);
    GXColor amb1 = {0, 0, 0, 0};
    GXSetChanAmbColor(5, amb1);
    GXColor mat1 = {0xFF, 0xFF, 0xFF, 0xFF};
    GXSetChanMatColor(5, mat1);
    GXInvalidateTexAll();
    switch (lbl_8087EFA8->m3A8) {
    default:
        SetTexCache_unk(device, 0);
        break;
    case 1:
        SetTexCache_unk(device, &lbl_807C6C08);
        break;
    case 2:
        SetTexCache_unk(device, &lbl_807C6DB4);
        break;
    }
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOrder(1, 1, 1, 4);
    GXSetTevOrder(2, 2, 2, 4);
    GXSetTevOrder(3, 3, 3, 4);
    GXSetTevOrder(4, 4, 4, 4);
    GXSetTevOrder(5, 5, 5, 4);
    GXSetTevOrder(6, 6, 6, 4);
    GXSetTevOrder(7, 7, 7, 4);
    GXSetTevOrder(8, 0xFF, 0xFF, 0xFF);
    GXSetTevOrder(9, 0xFF, 0xFF, 0xFF);
    GXSetTevOrder(10, 0xFF, 0xFF, 0xFF);
    GXSetTevOrder(11, 0xFF, 0xFF, 0xFF);
    GXSetTevOrder(12, 0xFF, 0xFF, 0xFF);
    GXSetTevOrder(13, 0xFF, 0xFF, 0xFF);
    GXSetTevOrder(14, 0xFF, 0xFF, 0xFF);
    GXSetTevOrder(15, 0xFF, 0xFF, 0xFF);
    GXSetNumTevStages(1);
    GXSetTevOp(0, 3);
    GXSetAlphaCompare(7, 0, 0, 7, 0);
    GXSetZTexture(0, 0x11, 0);
    for (int stage = 0; stage < 16; stage++) {
        GXSetTevKColorSel(stage, 6);
        GXSetTevKAlphaSel(stage, 0);
        GXSetTevSwapMode(stage, 0, 0);
    }
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(1, 0, 0, 0, 3);
    GXSetTevSwapModeTable(2, 1, 1, 1, 3);
    GXSetTevSwapModeTable(3, 2, 2, 2, 3);
    for (int stage = 0; stage < 16; stage++) {
        GXSetTevDirect(stage);
    }
    GXSetNumIndStages(0);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetIndTexCoordScale(1, 0, 0);
    GXSetIndTexCoordScale(2, 0, 0);
    GXSetIndTexCoordScale(3, 0, 0);
    GXColor fogColor = {0, 0, 0, 0};
    GXSetFog(0, 0.0f, 1.0f, 0.1f, 1.0f, fogColor);
    GXSetFogRangeAdj(0, 0, 0);
    GXSetBlendMode(0, 4, 5, 0);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetZMode(1, 3, 1);
    GXSetZCompLoc(1);
    GXSetDither(0);
    GXSetDstAlpha(0, 0);
    device->mDirty = 0xFFF;
    device->mState.mAlphaTest = 0;
    device->mState.mAlphaFunc = 7;
    device->mState.mAlphaRef = 0;
    device->mState.mBlend = 1;
    device->mState.mBlendSrc = 4;
    device->mState.mBlendDst = 5;
    device->mState.mCull = 1;
    device->mState.m20 = 0;
    device->mState.mZTest = 1;
    device->mState.mZFunc = 2;
    device->mState.mZWrite = 1;
    device->mState.mColorWrite = 1;
    device->mState.mAlphaWrite = 1;
    fn_800761A8(device);
}

// inferred: blend presets (atn/ComDrawTexture)
extern "C" void fn_80072914(GraphicsDevice_unk* device, int mode) {
    switch (mode) {
    case 0:
        SetBlendFactors_unk(device, 4, 5);
        break;
    case 1:
        SetBlendFactors_unk(device, 4, 1);
        break;
    case 2:
        SetBlendFactors_unk(device, 0, 2);
        break;
    case 3:
        SetBlendFactors_unk(device, 9, 1);
        break;
    case 4:
        SetBlendFactors_unk(device, 0, 5);
        break;
    case 5:
        SetBlendFactors_unk(device, 0, 3);
        break;
    }
}

// inferred: the frame at the head is done: wakes the waiting thread and moves the break point
static void NextFrame_unk() {
    PopFrame_unk(&lbl_807C6F68);
    OSWakeupThread(&lbl_807C6F60);
    if (lbl_807C6F68.mTail == 5) {
        GXDisableBreakPt();
        lbl_8087EED8 = 0;
    } else {
        FrameEntry_unk next = TopFrame_unk(&lbl_807C6F68);
        GXEnableBreakPt(next.mFifoPtr);
    }
}

// inferred: GX break point callback (the GPU reached the end of a frame)
extern "C" void fn_80072BDC() {
    FrameEntry_unk frame = TopFrame_unk(&lbl_807C6F68);
    if (frame.mXfb == lbl_8087EEE0->mNextXfb) {
        lbl_8087EED9 = 1;
    } else {
        NextFrame_unk();
    }
}

// inferred: VI pre-retrace callback: shows the next XFB when its frame has been drawn
extern "C" void fn_80072D74(unsigned int retraceCount) {
    GraphicsDevice_unk* device = lbl_8087EEE0;
    if (device->mCC != 0 && (retraceCount & 1)) {
        VIFlush();
    } else {
        unsigned short token = fn_806140B0();
        if (token == (unsigned short)(lbl_8087EEDC + 1)) {
            lbl_8087EEDC = token;
            device->mNextXfb = device->mNextXfb == device->mXfb0 ? device->mXfb1 : device->mXfb0;
            VISetNextFrameBuffer(lbl_8087EEE0->mNextXfb);
            VIFlush();
            lbl_8087EEDA = 1;
        }
    }
}

// inferred: VI post-retrace callback: releases the frame that waited for the swap
extern "C" void fn_80072E08(unsigned int retraceCount) {
    if (lbl_8087EEE0->mCC != 0 && (retraceCount & 1)) {
        return;
    }
    if (lbl_8087EED9 && lbl_8087EEDA) {
        lbl_8087EED9 = 0;
        lbl_8087EEDA = 0;
        NextFrame_unk();
    }
}

extern "C" void fn_800760BC(GraphicsDevice_unk* device, unsigned int left, unsigned int top, unsigned int width,
                            unsigned int height) {
    GXSetScissor(left, top, width, height);
}

extern "C" void fn_800760D0(GraphicsDevice_unk* device) {
    GXSetScissor(device->mViewX, device->mViewY, device->mViewW, device->mViewH);
}

extern "C" void fn_80076174(GraphicsDevice_unk* device, int enabled) {
    if (enabled) {
        GXSetCopyFilter(device->mRMode.aa, device->mRMode.sample_pattern, 1, device->mRMode.vfilter);
    } else {
        GXSetCopyFilter(0, 0, 0, 0);
    }
}

static const int sCompare[8] = {0, 1, 3, 2, 4, 6, 5, 7}; // GX_NEVER .. GX_ALWAYS
static const int sBlendFactor[8] = {0, 1, 2, 3, 4, 5, 6, 7};
static const int sCull[2] = {0, 2}; // GX_CULL_NONE, GX_CULL_BACK

// inferred: sends the dirty groups of mState to GX
extern "C" void fn_800761A8(GraphicsDevice_unk* device) {
    for (int i = 0; i < 12; i++) {
        if (!(device->mDirty & (1 << i))) {
            continue;
        }
        switch (i) {
        case 0:
        case 1:
        case 2:
            if (device->mState.mAlphaTest) {
                GXSetAlphaCompare(sCompare[device->mState.mAlphaFunc], device->mState.mAlphaRef, 0, 7, 0);
                GXSetZCompLoc(0);
            } else {
                GXSetAlphaCompare(7, 0, 0, 7, 0);
                GXSetZCompLoc(1);
            }
            device->mDirty &= ~(1 << 0);
            device->mDirty &= ~(1 << 1);
            device->mDirty &= ~(1 << 2);
            break;
        case 3:
        case 4:
        case 5:
            if (device->mState.mBlend) {
                GXSetBlendMode(1, sBlendFactor[device->mState.mBlendSrc], sBlendFactor[device->mState.mBlendDst], 15);
            } else {
                GXSetBlendMode(0, 4, 5, 15);
            }
            device->mDirty &= ~(1 << 3);
            device->mDirty &= ~(1 << 4);
            device->mDirty &= ~(1 << 5);
            break;
        case 6:
            GXSetCullMode(sCull[device->mState.mCull]);
            break;
        case 7:
            break;
        case 8:
        case 9:
        case 10:
            GXSetZMode(device->mState.mZTest, sCompare[device->mState.mZFunc], device->mState.mZWrite);
            device->mDirty &= ~(1 << 8);
            device->mDirty &= ~(1 << 9);
            device->mDirty &= ~(1 << 10);
            break;
        case 11:
            GXSetAlphaUpdate(device->mState.mAlphaWrite);
            GXSetColorUpdate(device->mState.mColorWrite);
            break;
        }
    }
    device->mAppliedState = device->mState;
    device->mDirty = 0;
}

// NONMATCHING: 72.00% - MWCC keeps the nullable texture in r5 instead of normalizing it through r6.
extern "C" void fn_800763C0(GraphicsDevice_unk* device, unsigned int map, const void* texObj) {
    const void** slot = device->mTexObjs + map;
    if (*slot != texObj) {
        *slot = texObj;
        if (texObj != 0) {
            GXLoadTexObj(texObj, map);
        }
    }
}

extern "C" void fn_800763FC(GraphicsDevice_unk* device, const void* texObj, unsigned int map) {
    const void** slot = device->mTexObjs + map;
    if (*slot != texObj) {
        *slot = texObj;
        if (texObj != 0) {
            GXLoadTexObj(texObj, map);
        }
    }
}

// inferred: releases one reference to a display-list allocation
extern "C" void fn_80076F88(GraphicsDevice_unk* device, unsigned int index) {
    GraphicsDevice_unk::DisplayListRef_unk* ref = &device->mDisplayLists[index];
    if (--ref->mRefs <= 0) {
        ::operator delete(ref->mData);
        ref->mData = 0;
    }
}
