// atn/PostFxDof.cpp: depth of field post effect (blurred copy of the frame mixed by a depth lookup table).
#include "atn/RenderSetting_unk.h"
#include "atn/PostFx.h"
#include "atn/MemoryManager.h"
#include "atn/DrawHelper.h"
#include "atn/RenderManager.h"

extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                           unsigned char mipmap, unsigned char maxLod);
extern "C" void GXSetIndTexOrder(int stage, int coord, int map);
extern "C" void GXSetIndTexCoordScale(int stage, int scaleS, int scaleT);
extern "C" void GXSetTevIndWarp(int stage, int indStage, unsigned char signedOffset, unsigned char replaceMode,
                                int matrix);
extern "C" void GXSetIndTexMtx(int id, const float offset[2][3], signed char scaleExp);
extern "C" void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
extern "C" void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
extern "C" void DCFlushRange(void* addr, unsigned int size);

// inferred: engine memory manager (class of atn/SmallAllocator_8.cpp; opaque here)

extern "C" RenderSetting_unk* lbl_8087EFA8;


extern "C" void fn_800C0D4C(void* image, int format, int width, int height); // inferred: converts to texture tiles
extern "C" void fn_800A981C(GXTexObj* src, void* dst, int width, int height, int format, const float* weights,
                            int forward, float dx, float dy);

namespace atn {

class PostFxDof : public IPostFx {
public:
    PostFxDof();
    virtual ~PostFxDof() {}
    virtual void vf10(RenderManager* view);
    void fn_800ACCB8();

    int m04;        // 0x04: inferred: last mE0
    GXTexObj m08;   // 0x08
    GXTexObj m28;   // 0x28
    GXTexObj m48;   // 0x48: inferred: depth lookup table (256x256 IA8)
    void* mTable;   // 0x68
    int m6C;        // 0x6C: inferred: table built for these depths
    int m70;
    int m74;
    int m78;
};

PostFxDof::PostFxDof() : m6C(-1), m70(-1), m74(-1), m78(-1) {
    unsigned int size = GXGetTexBufferSize(0x100, 0x100, 1, 0, 0);
    mTable = atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "", "", 0, 0);
    GXInitTexObj(&m48, mTable, 0x100, 0x100, 1, 0, 0, 0);
    GXInitTexObjLOD(&m48, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
}

void PostFxDof::vf10(RenderManager* view) {
    int fullW = lbl_8087EEE0->mWidth;
    int fullH = lbl_8087EEE0->mHeight;
    int w = lbl_8087EFA8->mDof.mWidth;
    int h = lbl_8087EFA8->mDof.mHeight;
    fn_800ACCB8();
    void* image0 = lbl_8087EF8C->fn_800A96AC(w, h, 6);
    GXInitTexObj(&m08, image0, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&m08, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    void* image1 = lbl_8087EF8C->fn_800A96AC(w, h, 6);
    GXInitTexObj(&m28, image1, w, h, 6, 0, 0, 0);
    GXInitTexObjLOD(&m28, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXPixModeSync();
    GXInvalidateTexAll();
    atn::RenderManager::instance_->fn_800C0508(0);
    GXSetTexCopySrc(0, 0, fullW, fullH);
    GXSetTexCopyDst(w, h, 6, 1);
    GXCopyTex(image0, 1);
    GXPixModeSync();
    fn_800A981C(&m08, image1, w, h, 6, 0, 0, lbl_8087EFA8->mDof.mBlurWidth, 0.0f);
    fn_800A981C(&m28, image0, w, h, 6, 0, 0, 0.0f, lbl_8087EFA8->mDof.mBlurHeight);
    GXSetNumTevStages(3);
    GXSetNumChans(0);
    GXSetNumTexGens(4);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(2, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(3, 1, 4, 0x3C, 0, 0x7D);
    fn_800763FC(lbl_8087EEE0, &atn::RenderManager::instance_->m74C.mObj, 0);
    fn_800763FC(lbl_8087EEE0, &atn::RenderManager::instance_->m77C, 1);
    fn_800763FC(lbl_8087EEE0, &m08, 2);
    fn_800763FC(lbl_8087EEE0, &m48, 3);
    // inferred: stage 0 reads the depth, the indirect stage turns it into lookup table coordinates
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevSwapModeTable(0, 0, 1, 2, 3);
    GXSetTevSwapModeTable(3, 0, 0, 0, 0);
    GXSetNumIndStages(1);
    GXSetIndTexOrder(0, 1, 1);
    GXSetIndTexCoordScale(0, 0, 0);
    GXSetTevIndWarp(1, 0, 0, 1, 1);
    float mtx[2][3];
    float zero = 0.0f;
    float half = 0.5f;
    mtx[0][0] = zero;
    mtx[0][1] = half;
    mtx[0][2] = zero;
    mtx[1][0] = half;
    mtx[1][1] = zero;
    mtx[1][2] = zero;
    GXSetIndTexMtx(1, mtx, 1);
    GXSetTevOp(1, 4);
    GXSetTevColorIn(1, 0xF, 0xF, 0xF, 8);
    GXSetTevColorOp(1, 0, 0, 0, 1, 1);
    GXSetTevOrder(1, 3, 3, 0xFF);
    GXSetTevSwapMode(1, 0, 0);
    // inferred: stage 2 mixes the frame and its blurred copy by the table value
    GXSetTevColorIn(2, 0, 8, 2, 0xF);
    GXSetTevColorOp(2, 0, 0, 0, 1, 0);
    GXSetTevOrder(2, 2, 2, 0xFF);
    GXSetTevSwapMode(2, 0, 0);
    GXSetTevAlphaIn(2, 7, 7, 7, 5);
    GXSetTevAlphaOp(2, 0, 0, 0, 1, 0);
    if (lbl_8087EFA8->mDof.mView != 0) {
        GXColor kColor = {0, 0, 0xFF, 0xFF};
        GXSetTevKColor(1, kColor);
        GXSetTevKColorSel(2, 0xD);
        GXSetTevColorIn(2, 0, 0xE, 2, 0xF);
    }
    fn_80075DEC(lbl_8087EEE0);
    atn::RenderManager::instance_->fn_800BFB70();
    if (lbl_8087EFA8->mDof.mDebug != 0) {
        lbl_8087EEB0->fn_8005DF90(-1, &atn::RenderManager::instance_->m77C, 0, 16.0f, 16.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
        lbl_8087EEB0->fn_8005DF90(-1, &m08, 0, 16.0f, 32.0f + fullH / 3.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
        lbl_8087EEB0->fn_8005DF90(-1, &m48, 0, 32.0f + fullW / 3.0f, 32.0f + fullH / 3.0f, 0.0f, fullW / 3.0f, fullH / 3.0f);
    }
}

// inferred: depth buffer value of a view distance (name invented)
static inline float DofDepth_unk(float n, float f, float z) {
    return 1.0f - (f * n / z - n) / (f - n);
}

// inferred: rebuilds the depth lookup table when the parameters change
void PostFxDof::fn_800ACCB8() {
    atn::RenderManager* fb = atn::RenderManager::instance_;
    float n = fb->mCamera.mNear;
    float f = fb->mCamera.mFar;
    float z0 = DofDepth_unk(n, f, lbl_8087EFA8->mDof.mStart);
    float z1 = DofDepth_unk(n, f, lbl_8087EFA8->mDof.mEnd);
    float z2 = DofDepth_unk(n, f, lbl_8087EFA8->mDof.mNearStart);
    float z3 = DofDepth_unk(n, f, lbl_8087EFA8->mDof.mNearEnd);
    int d0 = 65535.0f * z0;
    int d1 = 65535.0f * z1;
    int d2 = 65535.0f * z2;
    int d3 = 65535.0f * z3;
    if (d1 <= d0) {
        d1 = d0 + 1;
    }
    if (d2 <= d3) {
        d2 = d3 + 1;
    }
    if (m6C != d0 || m70 != d1 || m74 != d2 || m78 != d3 || m04 != lbl_8087EFA8->mDof.mClearFar) {
        unsigned char* table = (unsigned char*)mTable;
        for (int i = 0; i < 0x10000; i++) {
            int y = i / 256;
            int x = i % 256;
            float d = i / 65535.0f;
            float a = (d - z0) / (z1 - z0);
            if (a < 0.0f) {
                a = 0.0f;
            } else if (a > 1.0f) {
                a = 1.0f;
            }
            float b = -((d - z2) / (z2 - z3));
            if (b < 0.0f) {
                b = 0.0f;
            } else if (b > 1.0f) {
                b = 1.0f;
            }
            float c = a + b;
            if (c < 0.0f) {
                c = 0.0f;
            } else if (c > 1.0f) {
                c = 1.0f;
            }
            table[x * 256 + y] = (int)(65535.0f * c) >> 8;
        }
        m04 = lbl_8087EFA8->mDof.mClearFar;
        if (m04 != 0) {
            for (int y = 1; y <= 2; y++) {
                for (int x = 1; x <= 2; x++) {
                    table[(0x100 - y) * 256 + (0x100 - x)] = 0;
                }
            }
        }
        unsigned int size = GXGetTexBufferSize(0x100, 0x100, 1, 0, 0);
        fn_800C0D4C(mTable, 1, 0x100, 0x100);
        DCFlushRange(mTable, size);
        m6C = d0;
        m70 = d1;
        m74 = d2;
        m78 = d3;
    }
    GXInitTexObj(&m48, mTable, 0x100, 0x100, 1, 0, 0, 0);
    GXInitTexObjLOD(&m48, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
}

} // namespace atn
