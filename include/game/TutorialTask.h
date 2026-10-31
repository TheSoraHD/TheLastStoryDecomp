#ifndef GAME_TUTORIALTASK_H
#define GAME_TUTORIALTASK_H

#include "atn/Task.h"
#include "atn/LH_Binary.h"
#include "atn/LH_Motion.h"
#include "atn/Camera_unk.h"
#include "atn/VideoTHP.h"
#include "Event/MotionData.h"

class CharaTask;
namespace atn {
class D2A_Task;
} // namespace atn

// inferred: row of the tutorial table (fn_8021F09C, db/TutorialTable_unk.cpp); only the members used here
struct TutorialData_unk {
    int mId;
    int m04;
    char mName[0x20]; // 0x08: inferred: name of the tutorial files
    wchar_t mTitle[0x40]; // 0x28
    int mA8;
    char mText[0x40]; // 0xAC: inferred: name of the text pane
};

// inferred: page of a tutorial (0x4C bytes, "database/tutorial/<name>"); names invented
struct TutorialPage_unk {
    TutorialPage_unk() : mEnabled(0), mLayout(0), mVideo(0), mVideoLayout(0) {} // 0x805BA118
    ~TutorialPage_unk() {                                                         // 0x805B9EC8
        if (mVideo != 0) {
            delete mVideo;
        }
    }

    int mEnabled;           // 0x00
    atn::D2A_Task* mLayout; // 0x04
    Event::MotionHandle_unk mMotion; // 0x08: inferred: motion of the character during the page
    float mFrame;           // 0x14: inferred: frame of the motion
    atn::Vector3 mPos;      // 0x18: inferred: camera position (relative to the character)
    atn::Vector3 mTarget;   // 0x24: inferred: camera target
    float mFovy;            // 0x30
    int mType;              // 0x34: inferred: 1 shows the attack type
    float m38;
    atn::VideoTHP* mVideo;  // 0x3C
    atn::D2A_Task* mVideoLayout; // 0x40
    int mMsg0;              // 0x44
    int mMsg1;              // 0x48
};

// inferred: fixed array of the pages; operator[] is a weak copy (0x805BA348); name invented
template <class T, int N>
struct TutorialArray_unk {
    T& operator[](unsigned int i) { return mData[i]; }
    T mData[N];
};

// inferred from __vt__12TutorialTask (0x807979B0): tutorial shown in the camp or in the field
class TutorialTask : public atn::Task {
public:
    static TutorialTask* fn_805B9F50(atn::Task* parent, int id, CharaTask* chara); // inferred: creates the task
    TutorialTask(atn::Task* parent, int id, CharaTask* chara); // 0x805B9FC4
    virtual ~TutorialTask() {}                                 // 0x805BD3AC
    virtual int vf0C();                                        // 0x805BA168
    virtual void vf10();                                       // 0x805BAC14
    virtual void vf14();                                       // 0x805BB59C
    void fn_805BA354(const Camera_unk* camera); // inferred: starts the tutorial from the camera
    void fn_805BA934();                         // inferred: starts the current page
    void fn_805BB654();                         // inferred: reads the pages
    void fn_805BBD7C();                         // inferred: next page
    void fn_805BC1C0();                         // inferred: free position of the camera

    int mId;                   // 0x48
    TutorialData_unk* mData;   // 0x4C
    CharaTask* mChara;         // 0x50
    atn::Vector3 mPos;         // 0x54: inferred: position of the character
    atn::Vector3 mRot;         // 0x60
    atn::Vector3 mPos2;        // 0x6C
    atn::Vector3 mRot2;        // 0x78
    float m84;                 // inferred: frames of the transition
    int mState;                // 0x88
    Camera_unk mCamera;        // 0x8C
    Camera_unk mCamera2;       // 0x280
    TutorialArray_unk<TutorialPage_unk, 4> mPages; // 0x474
    atn::LH_Binary mFile;      // 0x5A4
    int mLoading;              // 0x5AC
    unsigned int mPage;        // 0x5B0
    int m5B4;
    struct TutorialFade_unk* m5B8; // inferred: fade object (virtual 0x8, 0xC)
    int m5BC;
    int m5C0;
};

// inferred: line of a popup (0x10 bytes); names invented
struct TutorialPopupLine_unk {
    TutorialPopupLine_unk() : mEnabled(0) {}
    int mEnabled;           // 0x00
    atn::D2A_Task* mLayout; // 0x04
    float mWait;            // 0x08
    int mType;              // 0x0C
};

// inferred from __vt__13TutorialPopup (0x80797974): popup of the field tutorials
class TutorialPopup : public atn::Task {
public:
    static TutorialPopup* fn_805BC6C8(atn::Task* parent, int id, CharaTask* chara); // inferred: creates the task
    TutorialPopup(atn::Task* parent, int id, CharaTask* chara); // 0x805BC73C
    virtual ~TutorialPopup() {}                                 // 0x805BD44C
    virtual int vf0C();                                         // 0x805BC9F0
    virtual void vf10();                                        // 0x805BCCFC
    void fn_805BCF04();                                         // inferred: reads the lines
    void fn_805BD298();                                         // inferred: next line

    int mId;                 // 0x48
    TutorialData_unk* mData; // 0x4C
    CharaTask* mChara;       // 0x50
    float mTime;             // 0x54
    int mState;              // 0x58
    atn::LH_Binary mFile;    // 0x5C
    int mLoading;            // 0x64
    TutorialPopupLine_unk mLines[4]; // 0x68
    unsigned int mLine;      // 0xA8
    float mSpeed;            // 0xAC
    atn::D2A_Task* mShadow;  // 0xB0: inferred: "tips_shd.d2a"
};

#endif
