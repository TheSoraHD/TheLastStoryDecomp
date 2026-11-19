// atn/PostFxMotionBlur.cpp: motion blur post effect (the last frames kept in a ring of four textures and
// blended with decreasing weights).
#include "atn/RenderSetting_unk.h"
#include "atn/PostFx.h"
#include "atn/MemoryManager.h"
#include "atn/DrawHelper.h"
#include "atn/RenderManager.h"

extern "C" double pow(double x, double y);
extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                           unsigned char mipmap, unsigned char maxLod);

// inferred: engine memory manager (class of atn/SmallAllocator_8.cpp; opaque here)

extern "C" RenderSetting_unk* lbl_8087EFA8;




// inferred: scalar lerp (a const float& helper; with plain floats the FPRs of the fade loop differ)
static inline float Lerp1_unk(const float& a, const float& b, float t) { return t * (b - a) + a; }
namespace atn {

class PostFxMotionBlur : public IPostFx {
public:
    PostFxMotionBlur();
    virtual ~PostFxMotionBlur() {}
    virtual void vf10(RenderManager* view);
    void fn_800AFEBC(int div);

    GXTexObj mTex[4];  // 0x04: inferred: ring of the last frames
    void* mImages[4];  // 0x84
    int mIndex;        // 0x94: inferred: last frame written (-1: none)
    int mCount;        // 0x98: inferred: number of older frames
    float m9C;         // 0x9C: inferred: weight curve exponents
    float mA0;
    int mA4;           // 0xA4: inferred: last enabled flag
    float mA8;         // 0xA8: inferred: fade factor
    int mDiv;          // 0xAC: inferred: size divisor of the textures
};

PostFxMotionBlur::PostFxMotionBlur()
    : mIndex(-1), mCount(0), m9C(0.0f), mA0(1.0f), mA4(0), mA8(0.0f), mDiv(0) {
    for (int i = 0; i < 4; i++) {
        mImages[i] = 0;
    }
    fn_800AFEBC(2);
}

// inferred: (re)allocates the ring for textures of 1/div of the screen
void PostFxMotionBlur::fn_800AFEBC(int div) {
    if (mDiv != div) {
        mDiv = div;
        int w = lbl_8087EEE0->mWidth / div;
        int h = lbl_8087EEE0->mHeight / div;
        for (int i = 0; i < 4; i++) {
            if (mImages[i] != 0) {
                atn::MemoryManager::Instance()->Free(mImages[i]);
            }
            unsigned int size = GXGetTexBufferSize(w, h, 4, 0, 0);
            mImages[i] = atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "", "", 0, 0);
            GXInitTexObj(&mTex[i], mImages[i], w, h, 4, 0, 0, 0);
            GXInitTexObjLOD(&mTex[i], 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        }
        mIndex = -1;
        mCount = 0;
    }
}

// NONMATCHING: 99.90% - registers only: the parameter block/flag at the start (r4/r6 against the hoisted
// 0x4330 of the conversions); the fade loop matches with Lerp1_unk (const float&) (~25 variants)
void PostFxMotionBlur::vf10(RenderManager* view) {
    if (mA4 != lbl_8087EFA8->m240.mEnabled) {
        mA4 = lbl_8087EFA8->m240.mEnabled;
        if (lbl_8087EFA8->m240.mEnabled != 0) {
            mA8 = lbl_8087EFA8->m240.mStartBlend != 0 ? 1.0f : 0.0f;
        } else {
            mA8 = lbl_8087EFA8->m240.mEndBlend != 0 ? 1.0f : 0.0f;
        }
    } else {
        if (mA8 > 0.0f) {
            mA8 -= lbl_8087EFA8->m240.mEnabled != 0 ? lbl_8087EFA8->m240.mStartBlendSpeed : lbl_8087EFA8->m240.mEndBlendSpeed;
        }
        if (lbl_8087EFA8->m240.mEnabled == 0 && mA8 <= 0.0f) {
            mIndex = -1;
            mCount = 0;
            return;
        }
    }
    mA0 = lbl_8087EFA8->m240.mScale;
    m9C = lbl_8087EFA8->m240.mShift;
    float a;
    float weights[6];
    float sum = 0.0f;
    for (int i = 0; i < mCount + 1; i++) {
        float x = i;
        a = pow(x, mA0);
        weights[i] = (1.0f + (float)pow(x, m9C)) / (1.0f + a);

        sum += weights[i];
    }
    float inv = 1.0f / sum;
    for (int i = 0; i < mCount + 1; i++) {
        weights[i] *= inv;
    }
    if (mA8 > 0.0f) {
        float sum2 = 0.0f;
        for (int i = 0; i < mCount + 1; i++) {
            if (mA4 != 0) {
                weights[i] = Lerp1_unk(i == 0 ? 1.0f : 0.0f, weights[i], 1.0f - mA8);
            } else {
                weights[i] = Lerp1_unk(weights[i], i == 0 ? 1.0f : 0.0f, 1.0f - mA8);
            }
            sum2 += weights[i];
        }
        float inv2 = 1.0f / sum2;
        for (int i = 0; i < mCount + 1; i++) {
            weights[i] *= inv2;
        }
    }
    atn::RenderManager::instance_->fn_800C0508(1);
    int fullW = lbl_8087EEE0->mWidth;
    int fullH = lbl_8087EEE0->mHeight;
    int index = (mIndex + 1) % 4;
    int w = fullW / mDiv;
    int h = fullH / mDiv;
    // inferred: copies the current frame into the ring
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevDirect(0);
    fn_800763FC(lbl_8087EEE0, &atn::RenderManager::instance_->m74C.mObj, 0);
    GXSetBlendMode(0, 1, 0, 3);
    fn_80075F58(lbl_8087EEE0, 0, 0, w, h);
    atn::RenderManager::instance_->fn_800BFB70();
    GXSetTexCopySrc(0, 0, w, h);
    GXSetTexCopyDst(w, h, 4, 0);
    GXCopyTex(mImages[index], 1);
    // inferred: draws the frame and the older ones with their weights
    GXSetNumTexGens(1);
    GXSetNumChans(1);
    GXSetNumTevStages(1);
    GXSetNumIndStages(0);
    GXSetTevOp(0, 0);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetChanCtrl(4, 0, 1, 1, 0, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTevDirect(0);
    fn_800763FC(lbl_8087EEE0, &atn::RenderManager::instance_->m74C.mObj, 0);
    fn_80075DEC(lbl_8087EEE0);
    GXSetBlendMode(1, 4, 0, 3);
    atn::Color color;
    color.r = weights[0];
    color.g = weights[0];
    color.b = weights[0];
    color.a = 1.0f;
    atn::RenderManager::instance_->fn_800BFC18(color, 0.0f, 0.0f);
    GXSetBlendMode(1, 4, 1, 3);
    for (int i = 0; i < mCount; i++) {
        color.r = weights[i + 1];
        color.g = weights[i + 1];
        color.b = weights[i + 1];
        fn_800763FC(lbl_8087EEE0, &mTex[(mIndex - i + 4) % 4], 0);
        atn::RenderManager::instance_->fn_800BFC18(color, 0.0f, 0.0f);
    }
    if (lbl_8087EFA8->m240.mDebug != 0) {
        float x = 16.0f;
        float y = fullH - fullH / 4.0f - 32.0f;
        float size = (fullW - 32.0f - 64.0f) / 4.0f;

        for (int i = 0; i < mCount; i++) {
            lbl_8087EEB0->fn_8005DF90(-1, &mTex[(mIndex - i + 4) % 4], 0, x, y, 0.0f, size, 0.7f * size);
            x += 16.0f + size;
        }
        lbl_8087EEB0->fn_8005DF90(-1, &mTex[index], 0, x, y, 0.0f, size, 0.7f * size);
    }
    if (lbl_8087EFA8->m240.mFrozen == 0) {
        mIndex = (mIndex + 1) % 4;
        mCount = mCount + 1 > 3 ? 3 : mCount + 1;
    }
}

} // namespace atn
