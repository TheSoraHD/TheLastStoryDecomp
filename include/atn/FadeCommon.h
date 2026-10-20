#ifndef ATN_FADECOMMON_H
#define ATN_FADECOMMON_H

#include "atn/Task.h"
#include "atn/Texture_unk.h"

namespace atn {

// inferred: screen fade task (names of the members invented)
class FadeCommon : public Task {
public:
    FadeCommon(Task* parent, int duration, unsigned int color0, unsigned int color1)
        : Task(parent), mActive(0), mFrame(0), mStep(1), mDuration(duration), mDelay(0), mWait(0), mKillAtEnd(0),
          mPauseAtEnd(0), mDeleteAtEnd(0), mColor0(color0), mColor1(color1), mZ(-100.0f) {}
    virtual ~FadeCommon();
    virtual int vf0C();  // 0x8006A944
    virtual void vf10(); // 0x8006A940
    virtual void vf14(); // 0x8006A94C

    void Update();       // 0x8006A0F4
    bool IsEnd() const;  // 0x8006A204
    unsigned int GetColor_unk() const;

    int mActive;               // 0x48
    int mFrame;                // 0x4C
    int mStep;                 // 0x50
    int mDuration;             // 0x54
    int mDelay;                // 0x58
    int mWait;                 // 0x5C
    int mKillAtEnd;            // 0x60
    int mPauseAtEnd;           // 0x64
    int mDeleteAtEnd;          // 0x68
    unsigned int mColor0;      // 0x6C
    unsigned int mColor1;      // 0x70
    float mZ;                  // 0x74
};

class FadeColor : public FadeCommon {
public:
    FadeColor(Task* parent, int duration, unsigned int color0, unsigned int color1)
        : FadeCommon(parent, duration, color0, color1), m78(0) {}
    static FadeColor* Create(Task* parent, int duration, unsigned int color0, unsigned int color1); // 0x8006A250
    virtual ~FadeColor();
    virtual void vf14(); // inferred: draw

    int m78; // inferred: subtractive blending
    int m7C;
};

class FadeCapture : public FadeCommon {
public:
    FadeCapture(Task* parent, int duration, unsigned int color0, unsigned int color1, int ownBuffer);
    static FadeCapture* Create(Task* parent, int duration, unsigned int color0, unsigned int color1,
                               int ownBuffer); // 0x8006A5A8
    virtual ~FadeCapture();
    virtual int vf0C();
    virtual void vf14(); // inferred: draw
    void Capture();      // 0x8006A900

    int m78;            // inferred: copy format
    Texture_unk mTex;   // 0x7C
    int mCaptured;      // 0xAC
};

} // namespace atn

#endif
