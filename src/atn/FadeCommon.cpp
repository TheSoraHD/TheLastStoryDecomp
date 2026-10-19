#include "atn/FadeCommon.h"
#include "atn/Memory.h"

// inferred: globals of unknown classes (automatic names)
struct Renderer2D_unk {
    unsigned char m00[0xA0];
    int mA0; // inferred: blend mode
};
struct RenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
extern "C" Renderer2D_unk* lbl_8087EEB0;
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" void* lbl_8087EFB4;
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
extern "C" void fn_80060D58(Renderer2D_unk* renderer, unsigned int color, float x, float y, float z, float w,
                            float h);
extern "C" void fn_8005ED6C(Renderer2D_unk* renderer, unsigned int color, atn::Texture_unk* tex, float z);
extern "C" void fn_800C1660(void* camera, atn::Texture_unk* tex, int format); // inferred: copies the frame buffer
extern "C" void fn_800D5A3C(atn::Texture_unk* tex);

namespace atn {

// inferred: interpolation of one 8-bit channel (name invented)
inline unsigned int LerpU8_unk(unsigned int a, unsigned int b, float t) {
    return (unsigned int)((1.0f - t) * a + t * b);
}

FadeCommon::~FadeCommon() {}

void FadeCommon::Update() {
    ((void)0); // inferred: an assertion compiled out (keeps Update out of line in vf10)
    if (mActive != 0) {
        mFrame += mStep;
    }
    if (IsEnd()) {
        if (mStep > 0) {
            mFrame = mWait + (mDuration + mDelay);
        } else {
            mFrame = 0;
        }
        if (mKillAtEnd != 0) {
            fn_800D246C(1);
        }
        if (mPauseAtEnd != 0) {
            mFlags |= 4;
        }
        if (mDeleteAtEnd != 0) {
            fn_800D2338(this);
        }
        mActive = 0;
    }
}

bool FadeCommon::IsEnd() const {
    if (mStep > 0) {
        return mFrame >= mWait + (mDuration + mDelay);
    }
    return mFrame <= 0;
}

inline unsigned int FadeCommon::GetColor_unk() const {
    if (mFrame < mDelay) {
        return mColor0;
    }
    if (mFrame >= mDuration + mDelay) {
        return mColor1;
    }
    float t = (float)(mFrame - mDelay) / (float)mDuration;
    unsigned int c;
    unsigned int c1 = mColor1;
    unsigned int c0 = mColor0;
    c = LerpU8_unk(c0 >> 24, c1 >> 24, t);
    c <<= 8;
    c |= LerpU8_unk((c0 >> 16) & 0xFF, (c1 >> 16) & 0xFF, t);
    c <<= 8;
    c |= LerpU8_unk((c0 >> 8) & 0xFF, (c1 >> 8) & 0xFF, t);
    c <<= 8;
    c |= LerpU8_unk(c0 & 0xFF, c1 & 0xFF, t);
    return c;

}

FadeColor* FadeColor::Create(Task* parent, int duration, unsigned int color0, unsigned int color1) {
    return new (12, "", "", 0) FadeColor(parent, duration, color0, color1);
}

FadeColor::~FadeColor() {}

void FadeColor::vf14() {
    if (mActive == 0) {
        return;
    }
    unsigned int color = GetColor_unk();
    if (m78 != 0) {
        lbl_8087EEB0->mA0 = 0x13;
    }
    fn_80060D58(lbl_8087EEB0, color, 0.0f, 0.0f, mZ, lbl_8087EEE0->mWidth, lbl_8087EEE0->mHeight);
    if (m78 != 0) {
        lbl_8087EEB0->mA0 = 0x12;
    }
}

FadeCapture::FadeCapture(Task* parent, int duration, unsigned int color0, unsigned int color1,
                         int ownBuffer)
    : FadeCommon(parent, duration, color0, color1), m78(0xE), mCaptured(0) {
    if (ownBuffer != 0) {
        mTex.fn_800D5B58(lbl_8087EEE0->mWidth, lbl_8087EEE0->mHeight, 6, 0, "ResolveTexture");
    } else {
        fn_800D5A3C(&mTex);
    }
}

FadeCapture* FadeCapture::Create(Task* parent, int duration, unsigned int color0, unsigned int color1,
                                 int ownBuffer) {
    return new (12, "", "", 0) FadeCapture(parent, duration, color0, color1, ownBuffer);
}

FadeCapture::~FadeCapture() {}

int FadeCapture::vf0C() {
    return 1;
}

void FadeCapture::vf14() {
    if (mActive == 0) {
        return;
    }
    fn_8005ED6C(lbl_8087EEB0, GetColor_unk(), &mTex, mZ);
}

void FadeCapture::Capture() {
    fn_800C1660(lbl_8087EFB4, &mTex, m78);
    mCaptured = 1;
}

void FadeCommon::vf10() {
    Update();
}

int FadeCommon::vf0C() {
    return 1;
}

void FadeCommon::vf14() {}

} // namespace atn
