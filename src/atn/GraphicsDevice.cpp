// Copyright (c) TheSoraHD. All rights reserved.
// Original file name: atn_graphics_device.cpp (string of OSPanic).

#include "atn/GraphicsDevice.h"
#include "atn/MemoryManager.h"
#include "atn/Texture_unk.h"
#include "atn/Memory.h"
#include "atn/RenderSetting_unk.h"
#include "atn/TextureCacheManager.h"
#include "atn/Light_unk.h"
#include "atn/Camera_unk.h"
#include "atn/Shader_unk.h"
#include "atn/DrawHelper.h"

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
void GXSetTexCoordCylWrap(int coord, unsigned char s, unsigned char t);
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
void GXSetVtxDescv(const GXVtxDescList* list);
void GXInitLightColor(GXLightObj* obj, GXColor color);
void GXInitLightPos(GXLightObj* obj, float x, float y, float z);
void GXInitLightAttn(GXLightObj* obj, float a0, float a1, float a2, float k0, float k1, float k2);
void GXInitLightDistAttn(GXLightObj* obj, float refDist, float refBright, int func);
void GXInitSpecularDir(GXLightObj* obj, float nx, float ny, float nz);
void GXLoadLightObjImm(GXLightObj* obj, int light);
void PSMTXInverse(const float src[3][4], float inv[3][4]);
void PSMTXTranspose(const float src[3][4], float xpose[3][4]);
void GXSetVtxAttrFmtv(int fmt, const GXVtxAttrFmtList* list);
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

extern "C" void* fn_800838B8(atn::MemoryManager* mgr, unsigned long size, int align, int heap, const char* name,
                             const char* file, int line); // inferred: allocation of a fixed block

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

// inferred: textures and matrices of the render system used by the effects of fn_80074EC8
struct RenderTextures_unk {
    unsigned char m000[0x460];
    GXTexObj m460;           // 0x460: light map (fn_800748D8)
    GXTexObj m480;           // 0x480: rim light (fn_80073A6C)
    atn::Matrix3x4 m4A0;     // 0x4A0: texture matrices of the rim light
    atn::Matrix3x4 m4D0;     // 0x4D0
};
// inferred: scene settings (NewCampScene2_unk of atn/CharaLightData_unk.cpp); only the shadow
// override is used here
struct RenderScene_unk {
    unsigned char m000[0x234];
    int m234;                // 0x234: overrides the shadow mode and color of RenderSetting_unk
    int m238;
    int m23C;                // 0x23C: shadow mode
    float m240;
    RsColor_unk m244;        // 0x244: shadow color
};
// inferred: render system (atn/RenderSystem_unk.cpp, lbl_8087EFB4); only the fields used here
struct RenderSystemView_unk {
    unsigned char m000[0xC];
    RenderTextures_unk* m00C;   // 0x00C
    unsigned char m010[0x104 - 0x10];
    Camera_unk mCamera;         // 0x104
    int m2F8;
    RenderScene_unk* mScene;    // 0x2FC
    unsigned char m300[0x318 - 0x300];
    Camera_unk mShadowCamera;   // 0x318: light camera of the shadow and light map
    GXTexObj m50C;              // 0x50C: shadow map (fn_80073A6C)
    unsigned char m52C[4];
    GXTexObj m530;              // 0x530: light of the normals (fn_800742C8)
};
extern "C" RenderSystemView_unk* lbl_8087EFB4;

// inferred: first object of .rodata; no reference left in the code
extern const int lbl_807316B8[4] = {0xFF, 0, 0, 0};

// inferred: texture caches selected by RenderSetting_unk::m3A8 (CacheMode); built by the static
// initializer of the file (0x80077058)
atn::TextureCacheManager8x32 lbl_807C6C08;
atn::TextureCacheManager4x128 lbl_807C6DB4;
OSThreadQueue lbl_807C6F60;  // inferred: threads waiting for a free frame
FrameQueue_unk lbl_807C6F68; // inferred: frames in flight
unsigned short lbl_8087D738 = 1; // inferred: next draw sync token
int lbl_8087D73C = 10;           // inferred: gamma set on the VI (fn_8007192C)
unsigned char lbl_8087EED8;      // inferred: break point enabled
unsigned char lbl_8087EED9;      // inferred: the finished frame waits for its XFB to be displayed
unsigned char lbl_8087EEDA;      // inferred: the XFB was swapped in the last retrace
unsigned short lbl_8087EEDC;     // inferred: draw sync token of the displayed frame
atn::GraphicsDevice_unk* lbl_8087EEE0; // inferred: the device (fn_80071220)

extern "C" void fn_80072BDC();
namespace atn {
struct ShaderDesc_unk;
}
extern "C" void fn_80074EC8(atn::GraphicsDevice_unk* device, const atn::ShaderDesc_unk* desc, int flags,
                            atn::TexProjector_unk* light, atn::TexProjector_unk* light2, atn::ShadowMap_unk* shadow);
extern "C" void fn_800761A8(atn::GraphicsDevice_unk* device);
extern "C" GXTexRegion* fn_80076FD4(GXTexObj* obj, unsigned int id);
extern "C" void fn_8009BF3C(void* mesh, void* material, void* lens, const float* color);

extern "C" void fn_80072D74(unsigned int retraceCount);
extern "C" void fn_80072E08(unsigned int retraceCount);

void operator delete(void* ptr);

using atn::GraphicsDevice_unk;
using atn::VtxFormat_unk;

namespace atn {

VtxFormat_unk::VtxFormat_unk(const VtxDescArray_unk& desc, const VtxAttrFmtArray_unk& fmt) : mAttrFmt(fmt), mDesc(desc) {}
} // namespace atn

// inferred: same vertex format (attribute formats, then descriptors)
extern "C" int fn_80071088(const VtxFormat_unk* a, const VtxFormat_unk* b) {
    for (int i = 0; i < 27; i++) {
        const GXVtxAttrFmtList* y = &b->mAttrFmt.mList[i];
        const GXVtxAttrFmtList* x = &a->mAttrFmt.mList[i];
        if (x->attr != y->attr || x->cnt != y->cnt || x->type != y->type || x->frac != y->frac) {
            return 0;
        }
    }
    for (int i = 0; i < 27; i++) {
        if (a->mDesc.mList[i].attr != b->mDesc.mList[i].attr || a->mDesc.mList[i].type != b->mDesc.mList[i].type) {
            return 0;
        }
    }
    return 1;
}

// inferred: creates the device (game/main)
extern "C" void fn_80071220() {
    if (lbl_8087EEE0 == 0) {
        lbl_8087EEE0 = new ((MemoryTag)6, "", "", 0) GraphicsDevice_unk();
    }
}

namespace atn {

GraphicsDevice_unk::GraphicsDevice_unk()
    : mWidth(0), mHeight(0), mWide(0), mClearColor(0xFF000000), mClearZ(0x00FFFFFF), mFifo(0), mFifoBuffer(0),
      mOffsetX(0.0f), mOffsetY(0.0f), mViewX(0), mViewY(0), mViewW(0), mViewH(0), mXfb0(0), mXfb1(0),
      mCurrentXfb(0), mNextXfb(0), mShaderDesc(0), mShaderFlags(0), mShadow(0), mVtxFormat(0), mTexCache(0), mPrevRegionCallback(0) {}
} // namespace atn

// inferred: stores the viewport/scissor rectangle
inline void SetViewRect_unk(GraphicsDevice_unk* device, int x, int y, int w, int h) {
    device->mViewX = x;
    device->mViewY = y;
    device->mViewW = w;
    device->mViewH = h;
}

extern "C" void fn_80075F58(GraphicsDevice_unk* device, int x, int y, int w, int h);

// inferred: video and GX initialization
// NONMATCHING: 98.41% - registers of the render mode copy and the overscan arithmetic
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
    device->mFifoBuffer = fn_800838B8(atn::MemoryManager::Instance(), 0x80000, 0x20, 1, "", "", 0);
    device->mXfb0 = atn::MemoryManager::Instance()->AllocFromMem2(xfbSize, 0x20, (MemoryTag)6, "", "", 0, 0);
    device->mXfb1 = atn::MemoryManager::Instance()->AllocFromMem2(xfbSize, 0x20, (MemoryTag)6, "", "", 0, 0);
    memset(device->mFifoBuffer, 0, 0x80000);
    memset(device->mXfb0, 0, xfbSize);
    memset(device->mXfb1, 0, xfbSize);
    device->mFifo = GXInit(device->mFifoBuffer, 0x80000);
    int h = device->mHeight;
    int w = device->mWidth;
    int y = 0;
    int x = 0;
    fn_80075F58(device, x, y, w, h);
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
// NONMATCHING: 95.56% - float registers of the viewport and scissor (fn_80075F58 inlined)
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
    fn_80075F58(device, x, y, w, h);
    GXInvalidateVtxCache();
    GXInvalidateTexAll();
}

// inferred: end of the frame: queues the copy to the XFB and waits while two frames are in flight
// NONMATCHING: 99.31% - one instruction of the vertical filter sum
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
inline void SetTexCache_unk(GraphicsDevice_unk* device, atn::ITextureCacheManager* cache) {
    if (cache != 0) {
        void* prev = GXSetTexRegionCallback(fn_80076FD4);
        if (device->mTexCache == 0) {
            device->mPrevRegionCallback = prev;
        }
    } else if (device->mPrevRegionCallback != 0) {
        GXSetTexRegionCallback(device->mPrevRegionCallback);
        device->mPrevRegionCallback = 0;
    }
    device->mTexCache = cache;
}
extern "C" void fn_80071E04(GraphicsDevice_unk* device);

// inferred: blend factors (indices of fn_800761A8); marks them dirty when they change
inline void SetBlendSrc_unk(GraphicsDevice_unk* device, int src) {
    device->mState.mBlendSrc = src;
    device->mDirty.mMask |= (device->mAppliedState.mBlendSrc != device->mState.mBlendSrc) << 4;
}
inline void SetBlendDst_unk(GraphicsDevice_unk* device, int dst) {
    device->mState.mBlendDst = dst;
    device->mDirty.mMask |= (device->mAppliedState.mBlendDst != device->mState.mBlendDst) << 5;
}
inline void SetBlendFactors_unk(GraphicsDevice_unk* device, int src, int dst) {
    SetBlendSrc_unk(device, src);
    SetBlendDst_unk(device, dst);
}
extern "C" void fn_80072914(GraphicsDevice_unk* device, int mode);
static void NextFrame_unk();

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
// NONMATCHING: 98.90% - registers of the initial GX state (viewport/scissor inlined)
extern "C" void fn_80071E04(GraphicsDevice_unk* device) {
    fn_80074EC8(device, 0, 0, 0, 0, 0);
    device->mVtxFormat = 0;
    int h = device->mHeight;
    int w = device->mWidth;
    int y = 0;
    int x = 0;
    fn_80075F58(device, x, y, w, h);
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
    device->mLights[0] = 0;
    device->mLights[1] = 0;
    device->mLights[2] = 0;
    device->mLights[3] = 0;
    device->mLights[4] = 0;
    device->mLights[5] = 0;
    device->mLights[6] = 0;
    device->mLights[7] = 0;
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
    GXSetTexCoordCylWrap(0, 0, 0);
    GXSetTexCoordScaleManually(1, 0, 0, 0);
    GXSetTexCoordCylWrap(1, 0, 0);
    GXSetTexCoordScaleManually(2, 0, 0, 0);
    GXSetTexCoordCylWrap(2, 0, 0);
    GXSetTexCoordScaleManually(3, 0, 0, 0);
    GXSetTexCoordCylWrap(3, 0, 0);
    GXSetTexCoordScaleManually(4, 0, 0, 0);
    GXSetTexCoordCylWrap(4, 0, 0);
    GXSetTexCoordScaleManually(5, 0, 0, 0);
    GXSetTexCoordCylWrap(5, 0, 0);
    GXSetTexCoordScaleManually(6, 0, 0, 0);
    GXSetTexCoordCylWrap(6, 0, 0);
    GXSetTexCoordScaleManually(7, 0, 0, 0);
    GXSetTexCoordCylWrap(7, 0, 0);
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
    fn_80075F58(device, x, y, device->mWidth, device->mHeight);
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
    device->mDirty.mMask = 0xFFF;
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
    device->mState.mUpdateMask = 3;
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
// NONMATCHING: 90.20% - the second TopFrame_unk keeps a redundant empty-queue check in the original
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
// NONMATCHING: 88.51% - same redundant empty-queue check as fn_80072BDC (NextFrame_unk inlined)
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

extern "C" {
void GXSetIndTexOrder(int stage, int coord, int map);
void GXSetIndTexCoordScale(int stage, int scaleS, int scaleT);
void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void GXSetTevKAlphaSel(int stage, int sel);
void GXSetTevIndirect(int stage, int indStage, int format, int bias, int mtx, int wrapS, int wrapT,
                      unsigned char addPrev, unsigned char utcLod, int alphaSel);
}

// inferred: GX resources used by the material set up by fn_80074EC8 (fn_80072F64); the effects added
// after the material (fn_80073194 ...) take their stages and texture coordinates from here
struct TevStats_unk {
    int mDual;       // 0x00: texture coordinates split by type (flag bit 0 of fn_80074EC8)
    int mTexMtxs;    // 0x04: texture matrices used (ShaderDesc_unk::m0E4)
    int mCoords0;    // 0x08: texture coordinates of type 0
    int mCoords13;   // 0x0C: texture coordinates of types 1-3
    int mCoords4;    // 0x10: texture coordinates of type 4
    int mTevStages;  // 0x14
    int mIndStages;  // 0x18
    int mSwapTables; // 0x1C: mask of the swap tables used
    int mKColors;    // 0x20: mask of the konstant colors used
};

// inferred: GX resources needed by an effect (lbl_807316D0)
struct TevCost_unk {
    int Coords() const { return mCoords0 + mCoords13 + mCoords4; }

    int mCoords0;
    int mCoords13;
    int mCoords4;
    int mTevStages;
    int mIndStages;
    int mSwapTables;
    int mKColors;
};

static const TevCost_unk sTevCosts[5] = {
    {0, 0, 2, 4, 0, 2, 1}, // fn_80073A6C
    {0, 1, 1, 3, 0, 0, 1}, // fn_800742C8
    {0, 0, 1, 1, 0, 0, 0}, // fn_800748D8
    {0, 0, 1, 1, 0, 0, 0}, // fn_80073194
    {0, 0, 1, 2, 0, 0, 0}, // fn_80074B24
};

inline int CountBits4_unk(int mask) {
    int n = 0;
    for (int i = 0; i < 4; i++) {
        if (mask & (1 << i)) {
            n++;
        }
    }
    return n;
}

// inferred: the effect still fits in the GX limits after the material
inline bool FitsTev_unk(const TevStats_unk* stats, int effect) {
    if (stats->mTevStages + sTevCosts[effect].mTevStages <= 16 && stats->mIndStages + sTevCosts[effect].mIndStages <= 4 &&
        CountBits4_unk(stats->mSwapTables) + sTevCosts[effect].mSwapTables <= 4 &&
        CountBits4_unk(stats->mKColors) + sTevCosts[effect].mKColors <= 4) {
        if (stats->mDual) {
            return stats->mCoords0 + sTevCosts[effect].mCoords0 <= 4 && stats->mCoords13 + sTevCosts[effect].mCoords13 <= 2 &&
                   stats->mCoords4 + sTevCosts[effect].mCoords4 <= 2;
        }
        return sTevCosts[effect].mCoords13 + sTevCosts[effect].mCoords4 + stats->mTexMtxs + sTevCosts[effect].mCoords0 <= 8;
    }
    return false;
}

// inferred: counts the GX resources used by a material
extern "C" void fn_80072F64(TevStats_unk* stats, const atn::ShaderDesc_unk* desc, int dual) {
    stats->mDual = dual;
    stats->mCoords0 = 0;
    stats->mCoords13 = 0;
    stats->mCoords4 = 0;
    stats->mTevStages = 0;
    stats->mIndStages = 0;
    stats->mSwapTables = 0;
    stats->mKColors = 0;
    stats->mTexMtxs = desc->m0E4;
    for (int i = 0; i < desc->mTexGenCount; i++) {
        switch (desc->mTexMtxs[i]->mType) {
        case 0:
            stats->mCoords0++;
            break;
        case 1:
        case 2:
        case 3:
            stats->mCoords13++;
            break;
        case 4:
            stats->mCoords4++;
            break;
        }
    }
    stats->mTevStages = desc->mTevStageCount;
    stats->mIndStages = desc->mIndStageCount;
    stats->mSwapTables = 1;
    stats->mKColors = 7;
    const atn::ShaderTevStage_unk* stage = desc->mTevStages;
    for (int i = 0; i < desc->mTevStageCount; stage++, i++) {
        stats->mSwapTables |= 1 << stage->mRasSwap;
        stats->mSwapTables |= 1 << stage->mTexSwap;
        if (stage->mColorIn[0] == 14 || stage->mColorIn[1] == 14 || stage->mColorIn[2] == 14 ||
            stage->mColorIn[3] == 14) {
            switch (stage->mKColorSel) {
            case 12:
            case 16:
            case 20:
            case 24:
            case 28:
                stats->mKColors |= 1;
                break;
            case 13:
            case 17:
            case 21:
            case 25:
            case 29:
                stats->mKColors |= 2;
                break;
            case 14:
            case 18:
            case 22:
            case 26:
            case 30:
                stats->mKColors |= 4;
                break;
            case 15:
            case 19:
            case 23:
            case 27:
            case 31:
                stats->mKColors |= 8;
                break;
            }
        }
        if (stage->mAlphaIn[0] == 6 || stage->mAlphaIn[1] == 6 || stage->mAlphaIn[2] == 6 ||
            stage->mAlphaIn[3] == 6) {
            switch (stage->mKAlphaSel) {
            case 16:
            case 20:
            case 24:
            case 28:
                stats->mKColors |= 1;
                break;
            case 17:
            case 21:
            case 25:
            case 29:
                stats->mKColors |= 2;
                break;
            case 18:
            case 22:
            case 26:
            case 30:
                stats->mKColors |= 4;
                break;
            case 19:
            case 23:
            case 27:
            case 31:
                stats->mKColors |= 8;
                break;
            }
        }
    }
}

// inferred: nonzero while a texture is loading. The symbol is named after its first known user
// (game/GimmickCommon.h); it is an atn::Texture_unk member.
struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void fn_800D5C84(float m[3][4], float sx, float sy, float c, float s, float tx, float ty);
extern "C" void fn_800D5E18(float m[3][4], float t, float b, float l, float r, float scaleS, float scaleT,
                            float transS, float transT);

inline atn::Matrix3x4 MtxMul_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

// inferred: GXLoadTexObj unless the object is already loaded in the map (same as fn_800763FC)
inline void LoadTexObj_unk(GraphicsDevice_unk* device, const void* obj, int map) {
    if (device->mTexObjs[map] == obj) {
        return;
    }
    device->mTexObjs[map] = obj;
    if (obj != 0) {
        GXLoadTexObj(obj, map);
    }
}

inline float Dot_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}

// inferred: the texture is loaded (same test as GimmickCommon::fn_803EDB0C)
inline bool IsTextureReady_unk(atn::Texture_unk* tex) {
    return tex->mImage != 0 || (((GimmickUvOwner_unk*)tex)->fn_800D59B8() == 0 && tex->m20.fn_80473EFC() == 3);
}

// inferred: projects a texture on the model from above (orthographic when mDir is zero) and
// modulates the color with it (TEV stage after the material, texture coordinate of type 4)
// NONMATCHING: 95.53% - register allocation (projector in r27, angle in f29) and load order of the arguments of fn_800D5E18
extern "C" void fn_80073194(TevStats_unk* stats, int flags, atn::TexProjector_unk* proj) {
    if (proj != 0 && 0.0f == proj->mSize.x && 0.0f == proj->mSize.y) {
        return;
    }
    int stage = stats->mTevStages++;
    int map = stats->mTexMtxs++;
    int coord;
    if (stats->mDual) {
        coord = stats->mCoords4++ + 6;
    } else {
        coord = stats->mCoords0 + (stats->mCoords13 + stats->mCoords4++);
    }
    int postMtx = map * 3 + 0x40;
    RenderSystemView_unk* view = lbl_8087EFB4;
    GXSetTexCoordGen2(coord, 0, 0, 0, 0, postMtx);
    atn::Vector2_unk size(1.0f, 1.0f);
    atn::Vector2_unk pos(0.0f, 0.0f);
    atn::Vector3 dir(0.0f, 0.0f, 0.0f);
    atn::Vector3 offset(0.0f, 0.0f, 0.0f);
    float angle = 0.0f;
    atn::Texture_unk* tex = &lbl_8087EEB0->m40;
    if (proj != 0 && IsTextureReady_unk(&proj->mTextures[proj->mTexture])) {
        pos = proj->mPos;
        size = proj->mSize;
        dir = proj->mDir;
        tex = &proj->mTextures[proj->mTexture];
        angle = 0.017453292f * proj->mAngle;
        offset = proj->mOffset;
    }
    fn_800763C0(lbl_8087EEE0, map, tex);
    atn::Matrix3x4 texMtx;
    atn::Matrix3x4 rot;
    fn_800D5C84(rot.m, 1.0f, 1.0f, sin(angle), cos(angle), 0.0f, 0.0f);
    if (PSVECSquareMag(&dir) < 0.001f) {
        atn::Matrix3x4 trans;
        trans.SetIdentity();
        trans.m[0][3] = -pos.x - offset.x;
        trans.m[2][3] = -pos.y - offset.z;
        atn::Matrix3x4 scale;
        memset(&scale, 0, sizeof(scale));
        scale.m[0][0] = 2.0f / size.x;
        scale.m[1][2] = 2.0f / size.y;
        atn::Matrix3x4 bias;
        memset(&bias, 0, sizeof(bias));
        bias.m[0][0] = 0.5f;
        bias.m[0][3] = 0.5f;
        bias.m[1][1] = 0.5f;
        bias.m[1][3] = 0.5f;
        bias.m[2][2] = 0.0f;
        bias.m[2][3] = 1.0f;
        texMtx = MtxMul_unk(rot, MtxMul_unk(bias, MtxMul_unk(scale, MtxMul_unk(trans, view->mCamera.mInvView))));
    } else {
        Camera_unk camera(0, 0);
        camera.mProjType = 1;
        PSVECNormalize(&dir, &dir);
        atn::Vector3 target = atn::Vector3(pos.x, 0.0f, pos.y) + offset;
        atn::Vector3 eye = target - dir * 100.0f;
        atn::Vector3 up(0.0f, 1.0f, 0.0f);
        if (atn::Abs_unk(Dot_unk(dir, atn::Vector3(0.0f, 1.0f, 0.0f))) > 0.99f) {
            up = atn::Vector3(0.0f, 0.0f, 1.0f);
            if (atn::Abs_unk(Dot_unk(dir, atn::Vector3(0.0f, 0.0f, 1.0f))) > 0.99f) {
                up = atn::Vector3(1.0f, 0.0f, 0.0f);
            }
        }
        camera.mUp = up;
        camera.mPos = eye;
        camera.mTarget = target;
        camera.mTop = pos.y + 0.5f * size.y;
        camera.mBottom = pos.y - 0.5f * size.y;
        camera.mLeft = pos.x - 0.5f * size.x;
        camera.mRight = pos.x + 0.5f * size.x;
        camera.fn_8004B378();
        atn::Matrix3x4 proj2;
        proj2.SetIdentity();
        fn_800D5E18(proj2.m, camera.mTop, camera.mBottom, camera.mLeft, camera.mRight, 0.5f, -0.5f, 0.5f, 0.5f);
        texMtx = MtxMul_unk(proj2, MtxMul_unk(rot, MtxMul_unk(camera.mView, view->mCamera.mInvView)));
    }
    GXLoadTexMtxImm(texMtx.m, postMtx, 0);
    GXSetTevSwapMode(stage, 0, 0);
    GXSetTevOrder(stage, coord, map, 0xFF);
    switch (proj->mBlend) {
    case 0:
        GXSetTevColorIn(stage, 15, 0, 8, 15);
        GXSetTevColorOp(stage, 0, 0, 0, 1, 0);
        break;
    case 1:
        GXSetTevColorIn(stage, 15, 0, 8, 15);
        GXSetTevColorOp(stage, 0, 0, 1, 1, 0);
        break;
    case 2:
        GXSetTevColorIn(stage, 15, 0, 8, 15);
        GXSetTevColorOp(stage, 0, 0, 2, 1, 0);
        break;
    case 3:
        GXSetTevColorIn(stage, 8, 15, 15, 0);
        GXSetTevColorOp(stage, 0, 0, 0, 1, 0);
        break;
    case 4:
        GXSetTevColorIn(stage, 8, 15, 15, 0);
        GXSetTevColorOp(stage, 0, 2, 0, 1, 0);
        break;
    case 5:
        GXSetTevColorIn(stage, 8, 15, 15, 0);
        GXSetTevColorOp(stage, 1, 0, 0, 1, 0);
        break;
    case 6:
        GXSetTevColorIn(stage, 8, 15, 15, 0);
        GXSetTevColorOp(stage, 1, 1, 0, 1, 0);
        break;
    }
    if (lbl_8087EFA8->m14C != 0) {
        GXSetTevColorIn(stage, 15, 15, 15, 8);
        GXSetTevColorOp(stage, 0, 0, 0, 1, 0);
    }
    GXSetTevDirect(stage);
    GXSetTevAlphaIn(stage, 7, 7, 7, 0);
    GXSetTevAlphaOp(stage, 0, 0, 0, 1, 0);
}


// inferred: first free entry of a 4 bit mask (swap tables, konstant colors); 0 when all are used
inline int AllocBit4_unk(int* mask) {
    for (int i = 0; i < 4; i++) {
        if (!(*mask & (1 << i))) {
            *mask |= 1 << i;
            return i;
        }
    }
    return 0;
}

// inferred: texture coordinate of type 4 for an effect (after the material ones)
inline int AllocCoord4_unk(TevStats_unk* stats) {
    if (stats->mDual) {
        return stats->mCoords4++ + 6;
    }
    return stats->mCoords0 + (stats->mCoords13 + stats->mCoords4++);
}

inline GXColor ToGXColor_unk(const RsColor_unk& c) {
    GXColor color;
    color.r = 255.0f * c.r;
    color.g = 255.0f * c.g;
    color.b = 255.0f * c.b;
    color.a = 255.0f * c.a;
    return color;
}

// inferred: rim light: two environment textures of the render system indexed by the normal, modulated
// by the shadow color (a third and fourth stage blend it with the material)
// NONMATCHING: 99.88% - argument registers of GXSetTevSwapModeTable and of the inlined texture loads
extern "C" void fn_80073A6C(TevStats_unk* stats, int flags) {
    int swap0 = AllocBit4_unk(&stats->mSwapTables);
    int swap1 = AllocBit4_unk(&stats->mSwapTables);
    int konst = AllocBit4_unk(&stats->mKColors);
    int stage0 = stats->mTevStages++;
    int stage1 = stats->mTevStages++;
    int stage2 = stats->mTevStages++;
    int kcsel = konst + 12;
    int stage3 = -1;
    if (lbl_8087EFA8->m12C == 0) {
        stage3 = stats->mTevStages++;
    }
    int map0 = stats->mTexMtxs++;
    int map1 = stats->mTexMtxs++;
    int coord0;
    if (stats->mDual) {
        coord0 = stats->mCoords4++ + 6;
    } else {
        coord0 = stats->mCoords0 + (stats->mCoords13 + stats->mCoords4++);
    }
    int coord1;
    if (stats->mDual) {
        coord1 = stats->mCoords4++ + 6;
    } else {
        coord1 = stats->mCoords0 + (stats->mCoords13 + stats->mCoords4++);
    }
    int postMtx0 = map0 * 3 + 0x40;
    int postMtx1 = postMtx0 + 3;
    GXSetTevSwapModeTable(swap0, 0, 3, 2, 3);
    GXSetTevSwapModeTable(swap1, 3, 1, 2, 3);
    RenderSetting_unk* rs = lbl_8087EFA8;
    RsColor_unk shadowColor = rs->m104.mColor;
    GXColor color = ToGXColor_unk(shadowColor);
    int mode = rs->m104.mMode;
    if (lbl_8087EFB4->mScene->m234 != 0) {
        mode = lbl_8087EFB4->mScene->m23C;
        color = ToGXColor_unk(lbl_8087EFB4->mScene->m244);
    }
    if (rs->m12C != 0) {
        GXColor red = {0xFF, 0x00, 0x00, 0xFF};
        color = red;
    }
    GXSetTevKColor(konst, color);
    GXSetTexCoordGen2(coord0, 0, 0, 0, 0, postMtx0);
    GXSetTexCoordGen2(coord1, 0, 0, 0, 0, postMtx1);
    LoadTexObj_unk(lbl_8087EEE0, &lbl_8087EFB4->m50C, map0);
    LoadTexObj_unk(lbl_8087EEE0, &lbl_8087EFB4->m00C->m480, map1);
    RenderTextures_unk* textures = lbl_8087EFB4->m00C;
    GXLoadTexMtxImm(textures->m4A0.m, postMtx0, 0);
    GXLoadTexMtxImm(textures->m4D0.m, postMtx1, 0);
    GXSetTevOrder(stage0, coord0, map0, 0xFF);
    GXSetTevColorIn(stage0, 15, 15, 15, 8);
    GXSetTevColorOp(stage0, 0, 0, 0, 1, 1);
    GXSetTevSwapMode(stage0, 0, swap0);
    GXSetTevDirect(stage0);
    GXSetTevAlphaIn(stage0, 7, 7, 7, 0);
    GXSetTevAlphaOp(stage0, 0, 0, 0, 1, 0);
    GXSetTevOrder(stage1, coord1, map1, 0xFF);
    GXSetTevColorIn(stage1, 2, 8, 12, 15);
    GXSetTevColorOp(stage1, 10, 0, 0, 0, 2);
    GXSetTevSwapMode(stage1, 0, swap1);
    GXSetTevDirect(stage1);
    GXSetTevAlphaIn(stage1, 7, 7, 7, 0);
    GXSetTevAlphaOp(stage1, 0, 0, 0, 1, 0);
    if (lbl_8087EFA8->m12C != 0) {
        GXSetTevKColorSel(stage2, kcsel);
        GXSetTevOrder(stage2, 0xFF, 0xFF, 0xFF);
        GXSetTevColorIn(stage2, 0, 14, 4, 15);
        GXSetTevColorOp(stage2, 0, 0, 0, 1, 0);
        GXSetTevSwapMode(stage2, 0, 0);
        GXSetTevDirect(stage2);
        GXSetTevAlphaIn(stage2, 7, 7, 7, 0);
        GXSetTevAlphaOp(stage2, 0, 0, 0, 1, 0);
    } else if (mode == 1) {
        GXSetTevKColorSel(stage2, kcsel);
        GXSetTevOrder(stage2, 0xFF, 0xFF, 0xFF);
        GXSetTevColorIn(stage2, 15, 14, 0, 0);
        GXSetTevColorOp(stage2, 0, 0, 0, 1, 1);
        GXSetTevSwapMode(stage2, 0, 0);
        GXSetTevDirect(stage2);
        GXSetTevAlphaIn(stage2, 7, 7, 7, 0);
        GXSetTevAlphaOp(stage2, 0, 0, 0, 1, 0);
        GXSetTevOrder(stage3, 0xFF, 0xFF, 0xFF);
        GXSetTevColorIn(stage3, 0, 2, 4, 15);
        GXSetTevColorOp(stage3, 0, 0, 0, 1, 0);
        GXSetTevSwapMode(stage3, 0, 0);
        GXSetTevDirect(stage3);
        GXSetTevAlphaIn(stage3, 7, 7, 7, 0);
        GXSetTevAlphaOp(stage3, 0, 0, 0, 1, 0);
    } else {
        GXSetTevKColorSel(stage2, kcsel);
        GXSetTevOrder(stage2, 0xFF, 0xFF, 0xFF);
        GXSetTevColorIn(stage2, 15, 14, 0, 15);
        GXSetTevColorOp(stage2, 0, 0, 0, 1, 1);
        GXSetTevSwapMode(stage2, 0, 0);
        GXSetTevDirect(stage2);
        GXSetTevAlphaIn(stage2, 7, 7, 7, 0);
        GXSetTevAlphaOp(stage2, 0, 0, 0, 1, 0);
        GXSetTevOrder(stage3, 0xFF, 0xFF, 0xFF);
        GXSetTevColorIn(stage3, 0, 2, 4, 15);
        GXSetTevColorOp(stage3, 0, 0, 0, 1, 0);
        GXSetTevSwapMode(stage3, 0, 0);
        GXSetTevDirect(stage3);
        GXSetTevAlphaIn(stage3, 7, 7, 7, 0);
        GXSetTevAlphaOp(stage3, 0, 0, 0, 1, 0);
    }
}


extern "C" void fn_805F8DA0(const Mtx src, Mtx dst); // inferred: PSMTXInvXpose

// inferred: 3x4 matrix cleared by its constructor
struct ZeroMatrix3x4_unk : public atn::Matrix3x4 {
    ZeroMatrix3x4_unk() {
        memset(m, 0, sizeof(m));
    }
};

// inferred: light map of the shadow camera (texture coordinate of type 4) lit by the normal in
// light space (texture coordinate of types 1-3); the third stage applies it to the material
// NONMATCHING: 99.33% - registers of the texture coordinate of types 1-3, reload of lbl_8087EFA8 and load order of the arguments of fn_800D5E18
extern "C" void fn_800742C8(TevStats_unk* stats, int flags) {
    int stage0 = stats->mTevStages++;
    int stage1 = stats->mTevStages++;
    int stage2 = stats->mTevStages++;
    int map0 = stats->mTexMtxs++;
    int map1 = stats->mTexMtxs++;
    int coord0;
    if (stats->mDual) {
        coord0 = stats->mCoords4++ + 6;
    } else {
        coord0 = stats->mCoords0 + (stats->mCoords13 + stats->mCoords4++);
    }
    int coord1;
    if (stats->mDual) {
        coord1 = stats->mCoords13++ + 4;
    } else {
        coord1 = stats->mCoords4 + (stats->mCoords0 + stats->mCoords13++);
    }
    int postMtx0 = map0 * 3 + 0x40;
    int postMtx1 = postMtx0 + 3;
    RenderSetting_unk* rs = lbl_8087EFA8;
    RsColor_unk shadowColor = rs->m104.mColor;
    GXColor color = ToGXColor_unk(shadowColor);
    int mode = rs->m104.mMode;
    if (lbl_8087EFB4->mScene->m234 != 0) {
        mode = lbl_8087EFB4->mScene->m23C;
        color = ToGXColor_unk(lbl_8087EFB4->mScene->m244);
    }
    if (lbl_8087EFA8->m12C != 0) {
        GXColor red = {0xFF, 0x00, 0x00, 0xFF};
        color = red;
    }
    GXSetTexCoordGen2(coord0, 0, 0, 0, 0, postMtx0);
    GXSetTexCoordGen2(coord1, 0, 1, 3, 0, postMtx1);
    LoadTexObj_unk(lbl_8087EEE0, &lbl_8087EFB4->m00C->m460, map0);
    LoadTexObj_unk(lbl_8087EEE0, &lbl_8087EFB4->m530, map1);
    RenderSystemView_unk* view = lbl_8087EFB4;
    atn::Matrix3x4 proj;
    proj.SetIdentity();
    fn_800D5E18(proj.m, view->mShadowCamera.mTop, view->mShadowCamera.mBottom, view->mShadowCamera.mLeft,
                view->mShadowCamera.mRight, 0.5f, -0.5f, 0.5f, 0.5f);
    static ZeroMatrix3x4_unk sNrmScale;
    sNrmScale.m[0][2] = 1.0f;
    sNrmScale.m[0][3] = 1.0f;
    atn::Matrix3x4 rot = view->mCamera.mInvView;
    rot.m[0][3] = 0.0f;
    rot.m[1][3] = 0.0f;
    rot.m[2][3] = 0.0f;
    atn::Matrix3x4 inv;
    fn_805F8DA0(view->mShadowCamera.mView.m, inv.m);
    inv.m[0][3] = 0.0f;
    inv.m[1][3] = 0.0f;
    inv.m[2][3] = 0.0f;
    atn::Matrix3x4 nrmMtx = MtxMul_unk(sNrmScale, MtxMul_unk(inv, rot));
    atn::Matrix3x4 posMtx = MtxMul_unk(proj, MtxMul_unk(view->mShadowCamera.mView, view->mCamera.mInvView));
    GXLoadTexMtxImm(posMtx.m, postMtx0, 0);
    GXLoadTexMtxImm(nrmMtx.m, postMtx1, 0);
    GXSetTevOrder(stage0, coord0, map0, 0xFF);
    GXSetTevColorIn(stage0, 15, 15, 15, 8);
    GXSetTevColorOp(stage0, 0, 0, 0, 1, 1);
    GXSetTevSwapMode(stage0, 0, 0);
    GXSetTevDirect(stage0);
    GXSetTevAlphaIn(stage0, 7, 7, 7, 0);
    GXSetTevAlphaOp(stage0, 0, 0, 0, 1, 0);
    GXSetTevOrder(stage1, coord1, map1, 0xFF);
    GXSetTevColorIn(stage1, 15, 8, 2, 15);
    GXSetTevColorOp(stage1, 0, 0, 0, 1, 1);
    GXSetTevSwapMode(stage1, 0, 0);
    GXSetTevDirect(stage1);
    GXSetTevAlphaIn(stage1, 7, 7, 7, 0);
    GXSetTevAlphaOp(stage1, 0, 0, 0, 1, 0);
    GXSetTevOrder(stage2, 0xFF, 0xFF, 0xFF);
    if (mode == 1) {
        GXSetTevColorIn(stage2, 0, 15, 15, 2);
    } else {
        GXSetTevColorIn(stage2, 0, 15, 2, 15);
    }
    GXSetTevColorOp(stage2, 0, 0, 0, 1, 0);
    GXSetTevSwapMode(stage2, 0, 0);
    GXSetTevDirect(stage2);
    GXSetTevAlphaIn(stage2, 7, 7, 7, 0);
    GXSetTevAlphaOp(stage2, 0, 0, 0, 1, 0);
}


// inferred: light map projected from the shadow camera on the model (simple light map, RenderSetting_unk
// m18C set); the material color is multiplied (or added, mode 1) with it
// NONMATCHING: 99.57% - argument registers of GXSetTexCoordGen2 and of the inlined texture load; load order of the arguments of fn_800D5E18
extern "C" void fn_800748D8(TevStats_unk* stats, int flags) {
    int stage = stats->mTevStages++;
    int map = stats->mTexMtxs++;
    int coord;
    if (stats->mDual) {
        coord = stats->mCoords4++ + 6;
    } else {
        coord = stats->mCoords0 + (stats->mCoords13 + stats->mCoords4++);
    }
    int postMtx = map * 3 + 0x40;
    GXSetTexCoordGen2(coord, 0, 0, 0, 0, postMtx);
    LoadTexObj_unk(lbl_8087EEE0, &lbl_8087EFB4->m00C->m460, map);
    atn::Matrix3x4 proj;
    proj.SetIdentity();
    RenderSystemView_unk* view = lbl_8087EFB4;
    fn_800D5E18(proj.m, view->mShadowCamera.mTop, view->mShadowCamera.mBottom, view->mShadowCamera.mLeft,
                view->mShadowCamera.mRight, 0.5f, -0.5f, 0.5f, 0.5f);
    atn::Matrix3x4 texMtx = MtxMul_unk(proj, MtxMul_unk(view->mShadowCamera.mView, view->mCamera.mInvView));
    GXLoadTexMtxImm(texMtx.m, postMtx, 0);
    int mode = lbl_8087EFA8->m104.mMode;
    if (lbl_8087EFB4->mScene->m234 != 0) {
        mode = lbl_8087EFB4->mScene->m23C;
    }
    GXSetTevOrder(stage, coord, map, 0xFF);
    if (mode == 1) {
        GXSetTevColorIn(stage, 0, 15, 15, 8);
    } else {
        GXSetTevColorIn(stage, 0, 15, 8, 15);
    }
    GXSetTevColorOp(stage, 0, 0, 0, 1, 0);
    GXSetTevSwapMode(stage, 0, 0);
    GXSetTevDirect(stage);
    GXSetTevAlphaIn(stage, 7, 7, 7, 0);
    GXSetTevAlphaOp(stage, 0, 0, 0, 1, 0);
}

extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path); // inferred: loads a texture

// inferred: shadow projected from the camera of the shadow map (two TEV stages)
// NONMATCHING: 99.53% - argument registers of GXSetTexCoordGen2 and load order of the arguments of fn_800D5E18
extern "C" void fn_80074B24(TevStats_unk* stats, atn::ShadowMap_unk* shadow, int flags) {
    int stage0 = stats->mTevStages++;
    int stage1 = stats->mTevStages++;
    int map = stats->mTexMtxs++;
    int coord;
    if (stats->mDual) {
        coord = stats->mCoords4++ + 6;
    } else {
        coord = stats->mCoords0 + (stats->mCoords13 + stats->mCoords4++);
    }
    int postMtx = map * 3 + 0x40;
    GXSetTexCoordGen2(coord, 0, 0, 0, 0, postMtx);
    atn::Texture_unk* tex = shadow->mTex;
    if (tex == 0) {
        static atn::Texture_unk sDefault;
        if (!IsTextureReady_unk(&sDefault)) {
            fn_800D594C(&sDefault, "sys_petrify.texture");
            sDefault.m2D = 1;
            sDefault.m2E = 0;
        }
        tex = &sDefault;
    }
    fn_800763C0(lbl_8087EEE0, map, tex);
    atn::Matrix3x4 proj;
    proj.SetIdentity();
    RenderSystemView_unk* view = lbl_8087EFB4;
    fn_800D5E18(proj.m, shadow->mCamera.mTop, shadow->mCamera.mBottom, shadow->mCamera.mLeft,
                shadow->mCamera.mRight, 0.5f, -0.5f, 0.5f, 0.5f);
    atn::Matrix3x4 texMtx = MtxMul_unk(proj, MtxMul_unk(shadow->mCamera.mView, view->mCamera.mInvView));
    GXLoadTexMtxImm(texMtx.m, postMtx, 0);
    GXSetTevOrder(stage0, coord, map, 4);
    GXSetTevColorIn(stage0, 15, 10, 8, 15);
    GXSetTevColorOp(stage0, 0, 0, 0, 1, 3);
    GXSetTevSwapMode(stage0, 0, 0);
    GXSetTevDirect(stage0);
    GXSetTevAlphaIn(stage0, 7, 7, 7, 4);
    GXSetTevAlphaOp(stage0, 0, 0, 0, 1, 3);
    GXSetTevOrder(stage1, 0xFF, 0xFF, 0xFF);
    GXSetTevColorIn(stage1, 0, 6, 7, 15);
    GXSetTevColorOp(stage1, 0, 0, 0, 1, 0);
    GXSetTevSwapMode(stage1, 0, 0);
    GXSetTevDirect(stage1);
    if (shadow->mFlags & 1) {
        GXSetTevColorIn(stage1, 15, 15, 15, 0);
        GXSetTevAlphaIn(stage1, 7, 0, 3, 7);
    } else {
        GXSetTevAlphaIn(stage1, 7, 7, 7, 0);
    }
    GXSetTevAlphaOp(stage1, 0, 0, 0, 1, 0);
}

atn::TexProjector_unk* lbl_8087EEE8; // inferred: projector of the effect of flag bit 2 (fn_80073194)
atn::TexProjector_unk* lbl_8087EEEC; // inferred: projector of the effect of flag bit 3


// inferred: sets up the TEV for a material and the effects of the flags (bit 0: texture coordinates
// split by type; bits 1-5: effects added after the material). The last setup is cached in the device.
// NONMATCHING: 98.55% - the original recomputes the row of sTevCosts (mulli) for the coordinate sum of mode 1/2; order of the cache stores
extern "C" void fn_80074EC8(GraphicsDevice_unk* device, const atn::ShaderDesc_unk* desc, int flags,
                            atn::TexProjector_unk* light, atn::TexProjector_unk* light2, atn::ShadowMap_unk* shadow) {
    if (lbl_8087EFA8->m204 != 0 && device->mShaderDesc != 0 && device->mShaderDesc == desc && device->mShaderFlags == flags &&
        (!(flags & 8) || lbl_8087EEEC == light2) && (!(flags & 4) || lbl_8087EEE8 == light) &&
        (shadow == 0 || !(flags & 0x20) || device->mShadow == shadow)) {
        return;
    }
    device->mShaderDesc = desc;
    device->mShaderFlags = flags;
    lbl_8087EEE8 = light;
    lbl_8087EEEC = light2;
    device->mShadow = shadow;
    if (desc == 0) {
        lbl_8087EEE8 = 0;
        lbl_8087EEEC = 0;
        return;
    }
    int dual = flags & 1;
    GXSetTevKColor(3, desc->mKColor3);
    unsigned char used[8];
    int coords[8];
    for (int i = 0; i < 8; i++) {
        used[i] = 0;
        coords[i] = 0xFF;
    }
    if (dual) {
        int next0 = 0;
        int next13 = 4;
        int next4 = 6;
        for (int i = 0; i < desc->mTexGenCount; i++) {
            switch (desc->mTexMtxs[i]->mType) {
            case 0:
                used[next0] = 1;
                coords[i] = next0++;
                break;
            case 1:
            case 2:
            case 3:
                used[next13] = 1;
                coords[i] = next13++;
                break;
            case 4:
                used[next4] = 1;
                coords[i] = next4++;
                break;
            }
        }
        for (int i = 7; i >= 0; i--) {
            if (used[i]) {
                break;
            }
        }
    } else {
        for (int i = 0; i < desc->mTexGenCount; i++) {
            coords[i] = i;
        }
    }
    int postMtx = 0x40;
    for (int i = 0; i < desc->mTexGenCount; i++) {
        atn::ShaderTexMtx_unk texMtx = *desc->mTexMtxs[i];
        atn::ShaderTexGen_unk texGen = *desc->mTexGens[i];
        switch (texMtx.mType) {
        case 0:
            if (!dual) {
                GXSetTexCoordGen2(coords[i], texGen.mFunc, texGen.mSrc, texGen.mMtx, 0, 0x7D);
            } else {
                GXSetTexCoordGen2(coords[i], texGen.mFunc, texGen.mSrc, 0x3C, 0, postMtx);
                postMtx += 3;
            }
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            GXSetTexCoordGen2(coords[i], texGen.mFunc, texGen.mSrc, texGen.mMtx, texMtx.mType != 4, postMtx);
            postMtx += 3;
            break;
        }
    }
    GXSetNumChans(desc->mNumChans / 2);
    GXSetNumIndStages(desc->mIndStageCount);
    if (desc->mIndStageCount > 0) {
        for (int i = 0; i < desc->mIndStageCount; i++) {
            atn::ShaderIndStage_unk ind = desc->mIndStages[i];
            GXSetIndTexOrder(i, coords[ind.mTexCoord], ind.mTexMap);
            GXSetIndTexCoordScale(i, ind.mScaleS, ind.mScaleT);
        }
    }
    for (int i = 0; i < 3; i++) {
        GXSetTevSwapModeTable(i, desc->mSwapTables[i][0], desc->mSwapTables[i][1], desc->mSwapTables[i][2],
                              desc->mSwapTables[i][3]);
    }
    for (int i = 0; i < desc->mTevStageCount; i++) {
        atn::ShaderTevStage_unk st = desc->mTevStages[i];
        int chan = 0xFF;
        switch (st.mChannel) {
        case 0:
            chan = 0xFF;
            break;
        case 1:
            chan = 4;
            break;
        case 2:
            chan = 5;
            break;
        case 3:
            chan = 6;
            break;
        }
        if (st.mTexMap == 0xFF || st.mTexCoord == 0xFF) {
            GXSetTevOrder(i, 0xFF, 0xFF, chan);
        } else {
            GXSetTevOrder(i, coords[st.mTexCoord], st.mTexMap, chan);
        }
        GXSetTevSwapMode(i, st.mRasSwap, st.mTexSwap);
        GXSetTevColorIn(i, st.mColorIn[0], st.mColorIn[1], st.mColorIn[2], st.mColorIn[3]);
        GXSetTevKColorSel(i, st.mKColorSel);
        GXSetTevColorOp(i, st.mColorOp, st.mColorBias, st.mColorScale, st.mColorClamp, st.mColorReg);
        GXSetTevAlphaIn(i, st.mAlphaIn[0], st.mAlphaIn[1], st.mAlphaIn[2], st.mAlphaIn[3]);
        GXSetTevKAlphaSel(i, st.mKAlphaSel);
        GXSetTevAlphaOp(i, st.mAlphaOp, st.mAlphaBias, st.mAlphaScale, st.mAlphaClamp, st.mAlphaReg);
        GXSetTevIndirect(i, st.mIndStage, st.mIndFormat, st.mIndBias, st.mIndMtx, st.mIndWrapS, st.mIndWrapT,
                         st.mIndAddPrev, st.mIndUtcLod, st.mIndAlphaSel);
    }
    TevStats_unk stats;
    fn_80072F64(&stats, desc, dual);
    if (flags & ~1) {
        if (shadow != 0 && (flags & 0x20)) {
            if (FitsTev_unk(&stats, 4)) {
                fn_80074B24(&stats, shadow, device->mShaderFlags);
            } else {
                device->mShaderFlags &= ~0x20;
            }
        }
        if (flags & 2) {
            if (FitsTev_unk(&stats, 0)) {
                fn_80073A6C(&stats, device->mShaderFlags);
            } else {
                device->mShaderFlags &= ~2;
            }
        } else if (flags & 0x10) {
            int mode;
            if (lbl_8087EFA8->m18C != 0) {
                mode = 2;
            } else {
                mode = 1;
            }
            if (FitsTev_unk(&stats, mode)) {
                if (mode == 2) {
                    fn_800748D8(&stats, device->mShaderFlags);
                } else {
                    fn_800742C8(&stats, device->mShaderFlags);
                }
            } else {
                device->mShaderFlags &= ~0x10;
            }
        }
        if (flags & 4) {
            if (lbl_8087EEE8 != 0 && FitsTev_unk(&stats, 3)) {
                fn_80073194(&stats, device->mShaderFlags, lbl_8087EEE8);
            } else {
                device->mShaderFlags &= ~4;
            }
        }
        if (flags & 8) {
            if (lbl_8087EEEC != 0 && FitsTev_unk(&stats, 3)) {
                fn_80073194(&stats, device->mShaderFlags, lbl_8087EEEC);
            } else {
                device->mShaderFlags &= ~8;
            }
        }
    }
    int numTexGens;
    if (stats.mDual) {
        if (stats.mCoords4 > 0) {
            numTexGens = stats.mCoords4 + 6;
        } else if (stats.mCoords13 > 0) {
            numTexGens = stats.mCoords13 + 4;
        } else {
            numTexGens = stats.mCoords0;
        }
    } else {
        numTexGens = stats.mCoords0 + (stats.mCoords13 + stats.mCoords4);
    }
    GXSetNumTexGens(numTexGens);
    GXSetNumTevStages(stats.mTevStages);
}
extern "C" void fn_800760BC(GraphicsDevice_unk* device, unsigned int left, unsigned int top, unsigned int width, unsigned int height);
extern "C" void fn_800760D0(GraphicsDevice_unk* device);
extern "C" void fn_80076174(GraphicsDevice_unk* device, int enabled);

static const int sCompare[8] = {0, 1, 3, 2, 4, 6, 5, 7}; // GX_NEVER .. GX_ALWAYS
static const int sBlendFactor[8] = {0, 1, 2, 3, 4, 5, 6, 7};
// inferred: tables of the same block whose users are not known (no reference left in the code)
extern const int lbl_807317A0[2] = {2, 3};
static const int sCull[2] = {0, 2}; // GX_CULL_NONE, GX_CULL_BACK
extern const int lbl_807317B0[2] = {1, 0};
extern "C" void fn_800761A8(GraphicsDevice_unk* device);
extern "C" void fn_800763C0(GraphicsDevice_unk* device, unsigned int map, atn::Texture_unk* tex);
extern "C" void fn_800763FC(GraphicsDevice_unk* device, const void* texObj, unsigned int map);
extern "C" void fn_80076F88(GraphicsDevice_unk* device, unsigned int index);
extern "C" int fn_800760E8(int numVerts, int prim);
extern "C" void fn_80076BE4(GraphicsDevice_unk* device, const VtxFormat_unk* format);
extern "C" float fn_80076FF8(GraphicsDevice_unk* device);

// inferred: full-screen viewport (atn/Layer2DBlur, atn/LayerOcclusionCPU)
// NONMATCHING: 98.79% - float registers of the scissor
extern "C" void fn_80075DEC(GraphicsDevice_unk* device) {
    int h = device->mHeight;
    int w = device->mWidth;
    int y = 0;
    int x = 0;
    fn_80075F58(device, x, y, w, h);
}

// inferred: sets the viewport and the scissor to a rectangle of the EFB (offset by mOffsetX/Y)
// NONMATCHING: 99.10% - float registers of the scissor
extern "C" void fn_80075F58(GraphicsDevice_unk* device, int x, int y, int w, int h) {
    device->mViewX = x;
    device->mViewY = y;
    device->mViewW = w;
    device->mViewH = h;
    GXSetViewport(device->mOffsetX + x, device->mOffsetY + y, w + device->mOffsetX, h + device->mOffsetY, 0.0f,
                  1.0f);
    GXSetScissor(device->mOffsetX + device->mViewX, device->mOffsetY + device->mViewY,
                 device->mViewW + device->mOffsetX, device->mViewH + device->mOffsetY);
}

extern "C" void fn_800760BC(GraphicsDevice_unk* device, unsigned int left, unsigned int top, unsigned int width,
                            unsigned int height) {
    GXSetScissor(left, top, width, height);
}

extern "C" void fn_800760D0(GraphicsDevice_unk* device) {
    GXSetScissor(device->mViewX, device->mViewY, device->mViewW, device->mViewH);
}
extern "C" void fn_8007642C(GraphicsDevice_unk* device, int index, Light_unk* light);

// inferred: number of primitives drawn by numVerts vertices (GXPrimitive)
extern "C" int fn_800760E8(int numVerts, int prim) {
    switch (prim) {
    case 0xB8: // GX_POINTS
        return numVerts;
    case 0xA8: // GX_LINES
        return numVerts / 2;
    case 0xB0: // GX_LINESTRIP
        return numVerts - 1;
    case 0x90: // GX_TRIANGLES
        return numVerts / 3;
    case 0x98: // GX_TRIANGLESTRIP
    case 0xA0: // GX_TRIANGLEFAN
        return numVerts - 2;
    case 0x80: // GX_QUADS
        return numVerts / 4;
    }
    return 0;
}

extern "C" void fn_80076174(GraphicsDevice_unk* device, int enabled) {
    if (enabled) {
        GXSetCopyFilter(device->mRMode.aa, device->mRMode.sample_pattern, 1, device->mRMode.vfilter);
    } else {
        GXSetCopyFilter(0, 0, 0, 0);
    }
}

// inferred: sends the dirty groups of mState to GX
// NONMATCHING: 99.70% - the tables are addressed from the start of .rodata (section symbol) in our object
extern "C" void fn_800761A8(GraphicsDevice_unk* device) {
    for (int i = 0; i < 12; i++) {
        if (!(device->mDirty.mMask & (1 << i))) {
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
            device->mDirty.mMask &= ~(1 << 0);
            device->mDirty.mMask &= ~(1 << 1);
            device->mDirty.mMask &= ~(1 << 2);
            break;
        case 3:
        case 4:
        case 5:
            if (device->mState.mBlend) {
                GXSetBlendMode(1, sBlendFactor[device->mState.mBlendSrc], sBlendFactor[device->mState.mBlendDst], 15);
            } else {
                GXSetBlendMode(0, 4, 5, 15);
            }
            device->mDirty.mMask &= ~(1 << 3);
            device->mDirty.mMask &= ~(1 << 4);
            device->mDirty.mMask &= ~(1 << 5);
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
            device->mDirty.mMask &= ~(1 << 8);
            device->mDirty.mMask &= ~(1 << 9);
            device->mDirty.mMask &= ~(1 << 10);
            break;
        case 11:
            GXSetAlphaUpdate(device->mState.mUpdateMask & 1);
            GXSetColorUpdate((device->mState.mUpdateMask >> 1) & 1);
            break;
        }
    }
    device->mAppliedState = device->mState;
    device->mDirty.mMask = 0;
}

// inferred: loads the GX texture of a Texture_unk (null: none) in a map
extern "C" void fn_800763C0(GraphicsDevice_unk* device, unsigned int map, atn::Texture_unk* tex) {
    const GXTexObj* obj = 0;
    if (tex != 0) {
        obj = &tex->mObj;
    }
    fn_800763FC(device, obj, map);
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

// inferred: loads a light; GX light 0 is the key light, as a far point light plus a specular light
// NONMATCHING: 205 vs 203 instructions (objdiff gives 0%) - register allocation of the vectors of the directional light
extern "C" void fn_8007642C(GraphicsDevice_unk* device, int index, Light_unk* light) {
    if (device->mLights[index] == light) {
        return;
    }
    device->mLights[index] = light;
    if (light == 0) {
        return;
    }
    atn::Matrix3x4& view = lbl_8087EFB4->mCamera.mView;
    if (index != 0) {
        GXLightObj obj;
        light->fn_80079994(&obj, view.m);
        GXLoadLightObjImm(&obj, 1 << index);
        return;
    }
    atn::Vector3 pos(0.0f, 0.0f, 0.0f);
    atn::Vector3 half(4.0f, 4.0f, 4.0f);
    PSVECNormalize(&half, &half);
    atn::Vector3 dir = light->mDir;
    atn::Vector3 far = atn::Vector3(-dir.x, -dir.y, -dir.z) * 1000000.0f;
    PSMTXMultVec(view.m, &far, &far);
    pos = far;
    half = atn::Vector3(-dir.x, -dir.y, -dir.z);
    PSVECNormalize(&half, &half);
    GXLightObj diffuse;
    GXColor color;
    color.r = (int)(255.0f * light->mColor.r);
    color.g = (int)(255.0f * light->mColor.g);
    color.b = (int)(255.0f * light->mColor.b);
    color.a = (int)(255.0f * light->mColor.a);
    GXInitLightColor(&diffuse, color);
    GXInitLightPos(&diffuse, pos.x, pos.y, pos.z);
    GXInitLightAttn(&diffuse, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    GXInitLightDistAttn(&diffuse, 1.0f, 0.07f, 0);
    GXLoadLightObjImm(&diffuse, 1);
    atn::Matrix3x4 normalMtx = view;
    normalMtx.m[0][3] = 0.0f;
    normalMtx.m[1][3] = 0.0f;
    normalMtx.m[2][3] = 0.0f;
    PSMTXInverse(normalMtx.m, normalMtx.m);
    PSMTXTranspose(normalMtx.m, normalMtx.m);
    PSMTXMultVec(normalMtx.m, &half, &half);
    PSVECNormalize(&half, &half);
    GXLightObj specular;
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXInitLightColor(&specular, white);
    GXInitSpecularDir(&specular, -half.x, -half.y, -half.z);
    GXInitLightAttn(&specular, 0.0f, 0.0f, 1.0f, 8.0f, 0.0f, -7.0f);
    GXLoadLightObjImm(&specular, 2);
}

// inferred: sets one field of mState (field index = its dirty bit) and marks it when it changed
extern "C" void fn_80076760(GraphicsDevice_unk* device, unsigned int field, unsigned int value) {
    switch (field) {
    case 0:
        device->mState.mAlphaTest = value;
        break;
    case 1:
        device->mState.mAlphaFunc = value;
        break;
    case 2:
        device->mState.mAlphaRef = value;
        break;
    case 3:
        device->mState.mBlend = value;
        break;
    case 4:
        device->mState.mBlendSrc = value;
        break;
    case 5:
        device->mState.mBlendDst = value;
        break;
    case 6:
        device->mState.mCull = value;
        break;
    case 7:
        device->mState.m20 = value;
        break;
    case 8:
        device->mState.mZTest = value;
        break;
    case 9:
        device->mState.mZFunc = value;
        break;
    case 10:
        device->mState.mZWrite = value;
        break;
    case 11:
        device->mState.mUpdateMask = value;
        break;
    }
    unsigned int dirty;
    switch (field) {
    case 0:
        dirty = (device->mAppliedState.mAlphaTest != device->mState.mAlphaTest) << 0;
        break;
    case 1:
        dirty = (device->mAppliedState.mAlphaFunc != device->mState.mAlphaFunc) << 1;
        break;
    case 2:
        dirty = (device->mAppliedState.mAlphaRef != device->mState.mAlphaRef) << 2;
        break;
    case 3:
        dirty = (device->mAppliedState.mBlend != device->mState.mBlend) << 3;
        break;
    case 4:
        dirty = (device->mAppliedState.mBlendSrc != device->mState.mBlendSrc) << 4;
        break;
    case 5:
        dirty = (device->mAppliedState.mBlendDst != device->mState.mBlendDst) << 5;
        break;
    case 6:
        dirty = (device->mAppliedState.mCull != device->mState.mCull) << 6;
        break;
    case 7:
        dirty = (device->mAppliedState.m20 != device->mState.m20) << 7;
        break;
    case 8:
        dirty = (device->mAppliedState.mZTest != device->mState.mZTest) << 8;
        break;
    case 9:
        dirty = (device->mAppliedState.mZFunc != device->mState.mZFunc) << 9;
        break;
    case 10:
        dirty = (device->mAppliedState.mZWrite != device->mState.mZWrite) << 10;
        break;
    case 11:
        dirty = (device->mAppliedState.mUpdateMask != device->mState.mUpdateMask) << 11;
        break;
    default:
        dirty = 0;
        break;
    }
    device->mDirty.mMask |= dirty;
}

// inferred: sets the whole state; the dirty mask becomes the fields that differ from the applied state
extern "C" void fn_80076A28(GraphicsDevice_unk* device, const atn::GXStateBits_unk* state) {
    device->mState = *state;
    atn::GXDirtyBits_unk dirty;
    dirty.mMask |= (device->mAppliedState.mAlphaTest != device->mState.mAlphaTest) << 0;
    dirty.mMask |= (device->mAppliedState.mAlphaFunc != device->mState.mAlphaFunc) << 1;
    dirty.mMask |= (device->mAppliedState.mAlphaRef != device->mState.mAlphaRef) << 2;
    dirty.mMask |= (device->mAppliedState.mBlend != device->mState.mBlend) << 3;
    dirty.mMask |= (device->mAppliedState.mBlendSrc != device->mState.mBlendSrc) << 4;
    dirty.mMask |= (device->mAppliedState.mBlendDst != device->mState.mBlendDst) << 5;
    dirty.mMask |= (device->mAppliedState.mCull != device->mState.mCull) << 6;
    dirty.mMask |= (device->mAppliedState.m20 != device->mState.m20) << 7;
    dirty.mMask |= (device->mAppliedState.mZTest != device->mState.mZTest) << 8;
    dirty.mMask |= (device->mAppliedState.mZFunc != device->mState.mZFunc) << 9;
    dirty.mMask |= (device->mAppliedState.mZWrite != device->mState.mZWrite) << 10;
    dirty.mMask |= (device->mAppliedState.mUpdateMask != device->mState.mUpdateMask) << 11;
    device->mDirty = dirty;
}

// inferred: sets the vertex format when it changes (0: forget the current one)
extern "C" void fn_80076BE4(GraphicsDevice_unk* device, const VtxFormat_unk* format) {
    if (format == 0) {
        device->mVtxFormat = format;
    } else if (device->mVtxFormat != format) {
        device->mVtxFormat = format;
        GXClearVtxDesc();
        GXSetVtxDescv(format->mDesc.mList);
        GXSetVtxAttrFmtv(0, format->mAttrFmt.mList);
    }
}

// inferred: finds or adds a vertex format in the cache; returns its index
extern "C" int fn_80076C44(GraphicsDevice_unk* device, const VtxFormat_unk* format) {
    int freeSlot = 256;
    for (int i = 0; i < 256; i++) {
        if (freeSlot == 256 && device->mVtxFormats[i].mFormat == 0) {
            freeSlot = i;
        }
        if (device->mVtxFormats[i].mFormat != 0 && fn_80071088(format, device->mVtxFormats[i].mFormat)) {
            device->mVtxFormats[i].mRefs++;
            return i;
        }
    }
    device->mVtxFormats[freeSlot].mFormat = new ((MemoryTag)6, "", "", 0) VtxFormat_unk(*format);
    device->mVtxFormats[freeSlot].mRefs = 1;
    return freeSlot;
}

// inferred: releases one reference to a cached vertex format
extern "C" void fn_80076F88(GraphicsDevice_unk* device, unsigned int index) {
    GraphicsDevice_unk::VtxFormatRef_unk* ref = &device->mVtxFormats[index];
    if (--ref->mRefs <= 0) {
        ::operator delete(ref->mFormat);
        ref->mFormat = 0;
    }
}

// inferred: texture region callback: asks the current cache
extern "C" GXTexRegion* fn_80076FD4(GXTexObj* obj, unsigned int id) {
    return lbl_8087EEE0->mTexCache->vf0C(obj, id);
}

// inferred: aspect ratio of the displayed image
// NONMATCHING: 91.67% - prologue order
extern "C" float fn_80076FF8(GraphicsDevice_unk* device) {
    float aspect;
    if (device->mWide) {
        aspect = 1.7777778f;
    } else {
        aspect = 1.3333334f;
    }
    return aspect * ((float)device->mRMode.efbHeight / (float)device->mRMode.xfbHeight);
}
