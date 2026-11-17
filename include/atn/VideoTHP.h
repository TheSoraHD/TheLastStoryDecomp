#ifndef ATN_VIDEOTHP_H
#define ATN_VIDEOTHP_H

#include "atn/IRender.h"

// inferred: owning handle to a debug menu group (like game/ResHandle.h, without constructor); its members are
// defined in atn/VideoTHP.cpp (inline bodies here would renumber the @N labels of the files that include this)
struct DebugGroup_unk {
    ~DebugGroup_unk();
    void* Create(const char*);
    void* mGroup;
};

namespace atn {

struct Vector2f_unk {
    float x, y;
};

// inferred: video player interface (RTTI emitted by atn/VideoTHP.cpp, which defines its inline members)
class Video : public IRender {
public:
    Video();
    virtual ~Video();
    virtual void vf20() = 0;
    virtual void vf24() = 0;
    virtual void vf28() = 0; // inferred: draws the movie
    virtual float vf2C() = 0;
    virtual unsigned int vf30() = 0; // inferred: current frame (TutorialTask::vf10)
    virtual unsigned int vf34() = 0;
    virtual void vf38(float, int) = 0;
    virtual void vf3C(float, int) = 0; // inferred: starts the playback (volume, fade frames)

    int m04;   // inferred: layout mode (0, or 2/3 for RTMV movies)
    int m08;   // inferred: state (2 finished, 3..5 set by TutorialTask)
    int m0C;   // inferred: loop
    float m10; // inferred: alpha / depth of the movie (TutorialTask::fn_805BA934)
    Vector2_unk mPos; // 0x14: inferred: position (copied with paired singles by TutorialTask::fn_805BA934)
    int m1C;   // inferred: finished
};

// inferred: THP movie player (atn/VideoTHP.cpp, 0x98 bytes)
class VideoTHP : public Video {
public:
    VideoTHP(const char*); // 0x800DCA70
    virtual ~VideoTHP();
    virtual void vf10(RenderManager*);
    virtual int vf14(const Matrix3x4*, const Vector3*, const Frustum_unk*, AABB_unk*);
    virtual void vf18(RenderManager*);
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual float vf2C();
    virtual unsigned int vf30();
    virtual unsigned int vf34();
    virtual void vf38(float, int);
    virtual void vf3C(float, int);

    void SetName_unk(const char*);

    unsigned int m20[3];     // THPVideoInfo
    unsigned int m2C[4];     // THPAudioInfo
    void* m3C;               // inferred: THP work buffer
    char mName[0x40];        // 0x40
    unsigned int m80;        // inferred: current frame
    Vector2f_unk m84;        // inferred: scale
    Vector2f_unk m8C;        // inferred: offset
    DebugGroup_unk m94;      // inferred: debug menu group
};

} // namespace atn

#endif
