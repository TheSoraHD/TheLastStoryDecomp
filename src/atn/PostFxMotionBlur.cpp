// atn/PostFxMotionBlur.cpp: motion blur post effect (the last frames kept in a ring of four textures and
// blended with decreasing weights).
#include "atn/PostFx.h"

extern "C" double pow(double x, double y);
extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                           unsigned char mipmap, unsigned char maxLod);

// inferred: engine memory manager (class of atn/SmallAllocator_8.cpp; opaque here)
struct MemoryManager_unk;
extern "C" MemoryManager_unk* fn_800827E0();
extern "C" void* fn_800839EC(MemoryManager_unk* mgr, unsigned long size, unsigned long align, int heap,
                             const char* file, const char* func, int line, int arg);
extern "C" void fn_80083AD4(MemoryManager_unk* mgr, void* p);

// inferred: render parameters (lbl_8087EFA8, fields named by offset; see atn/PostFxBloom.cpp)
struct MotionBlurParams_unk {
    unsigned char m00[0x240];
    int m240;   // 0x240: inferred: enabled
    int m244;   // 0x244: inferred: debug display
    int m248;   // 0x248: inferred: pause (the ring is not advanced)
    float m24C; // 0x24C: inferred: weight curve exponents
    float m250;
    int m254;   // 0x254: inferred: fade in when enabled
    int m258;   // 0x258: inferred: fade out when disabled
    float m25C; // 0x25C: inferred: fade speeds
    float m260;
};
extern "C" MotionBlurParams_unk* lbl_8087EFA8;

// inferred: frame buffer object (lbl_8087EFB4); only the member used here
struct MotionBlurFrameBuffer_unk {
    unsigned char m00[0x74C];
    GXTexObj m74C; // 0x74C: inferred: color texture
};

// inferred: RGBA color passed by value (see atn/PostFxBloom.cpp)
struct MotionBlurColor_unk {
    MotionBlurColor_unk() {}
    MotionBlurColor_unk(const MotionBlurColor_unk& c) {
        float r_ = c.r;
        float g_ = c.g;
        float b_ = c.b;
        float a_ = c.a;
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    float r, g, b, a;
};

extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void fn_8005DF90(void* dbg, int color, GXTexObj* obj, int arg, float x, float y, float z, float w,
                            float h);
extern "C" void fn_800C0508(PostFxFrameBuffer_unk* fb, int arg);
extern "C" void fn_800BFC18(PostFxFrameBuffer_unk* fb, MotionBlurColor_unk color, float a, float b);

namespace atn {

class PostFxMotionBlur : public IPostFx {
public:
    PostFxMotionBlur();
    virtual ~PostFxMotionBlur() {}
    virtual void vf10(void* camera);
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
                fn_80083AD4(fn_800827E0(), mImages[i]);
            }
            unsigned int size = GXGetTexBufferSize(w, h, 4, 0, 0);
            mImages[i] = fn_800839EC(fn_800827E0(), size, 0x20, 6, "", "", 0, 0);
            GXInitTexObj(&mTex[i], mImages[i], w, h, 4, 0, 0, 0);
            GXInitTexObjLOD(&mTex[i], 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
        }
        mIndex = -1;
        mCount = 0;
    }
}

// NONMATCHING: 99.78% - registers only: the parameter block/flag at the start (r4/r6) and the FPRs of the
// fade loop (~14 variants)
void PostFxMotionBlur::vf10(void* camera) {
    if (mA4 != lbl_8087EFA8->m240) {
        mA4 = lbl_8087EFA8->m240;
        if (lbl_8087EFA8->m240 != 0) {
            mA8 = lbl_8087EFA8->m254 != 0 ? 1.0f : 0.0f;
        } else {
            mA8 = lbl_8087EFA8->m258 != 0 ? 1.0f : 0.0f;
        }
    } else {
        if (mA8 > 0.0f) {
            mA8 -= lbl_8087EFA8->m240 != 0 ? lbl_8087EFA8->m25C : lbl_8087EFA8->m260;
        }
        if (lbl_8087EFA8->m240 == 0 && mA8 <= 0.0f) {
            mIndex = -1;
            mCount = 0;
            return;
        }
    }
    mA0 = lbl_8087EFA8->m24C;
    m9C = lbl_8087EFA8->m250;
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
                float k = 1.0f - mA8;
                float t = i == 0 ? 1.0f : 0.0f;
                weights[i] = k * (weights[i] - t) + t;
            } else {
                float k = 1.0f - mA8;
                float t = i == 0 ? 1.0f : 0.0f;
                weights[i] = k * (t - weights[i]) + weights[i];
            }
            sum2 += weights[i];
        }
        float inv2 = 1.0f / sum2;
        for (int i = 0; i < mCount + 1; i++) {
            weights[i] *= inv2;
        }
    }
    fn_800C0508(lbl_8087EFB4, 1);
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
    fn_800763FC(lbl_8087EEE0, &((MotionBlurFrameBuffer_unk*)lbl_8087EFB4)->m74C, 0);
    GXSetBlendMode(0, 1, 0, 3);
    fn_80075F58(lbl_8087EEE0, 0, 0, w, h);
    fn_800BFB70(lbl_8087EFB4);
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
    fn_800763FC(lbl_8087EEE0, &((MotionBlurFrameBuffer_unk*)lbl_8087EFB4)->m74C, 0);
    fn_80075DEC(lbl_8087EEE0);
    GXSetBlendMode(1, 4, 0, 3);
    MotionBlurColor_unk color;
    color.r = weights[0];
    color.g = weights[0];
    color.b = weights[0];
    color.a = 1.0f;
    fn_800BFC18(lbl_8087EFB4, color, 0.0f, 0.0f);
    GXSetBlendMode(1, 4, 1, 3);
    for (int i = 0; i < mCount; i++) {
        color.r = weights[i + 1];
        color.g = weights[i + 1];
        color.b = weights[i + 1];
        fn_800763FC(lbl_8087EEE0, &mTex[(mIndex - i + 4) % 4], 0);
        fn_800BFC18(lbl_8087EFB4, color, 0.0f, 0.0f);
    }
    if (lbl_8087EFA8->m244 != 0) {
        float x = 16.0f;
        float y = fullH - fullH / 4.0f - 32.0f;
        float size = (fullW - 32.0f - 64.0f) / 4.0f;

        for (int i = 0; i < mCount; i++) {
            fn_8005DF90(lbl_8087EEB0, -1, &mTex[(mIndex - i + 4) % 4], 0, x, y, 0.0f, size, 0.7f * size);
            x += 16.0f + size;
        }
        fn_8005DF90(lbl_8087EEB0, -1, &mTex[index], 0, x, y, 0.0f, size, 0.7f * size);
    }
    if (lbl_8087EFA8->m248 == 0) {
        mIndex = (mIndex + 1) % 4;
        mCount = mCount + 1 > 3 ? 3 : mCount + 1;
    }
}

} // namespace atn
