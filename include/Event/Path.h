#ifndef EVENT_PATH_H
#define EVENT_PATH_H

#include "Event/CommonData.h"
#include "atn/Camera_unk.h"
#include "std/string.h"
#include "std/vector.h"
#include "macros.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
struct Xsca_unk; // inferred: camera animation (xsca)

namespace Event {

struct Actor_unk;

// inferred: point of a path (position and rotation around y)
struct PathPoint_unk {
    atn::Vector3 mPos;
    float mRotY;
};

// inferred: cubic of one segment and axis: a0 + a1 u + a2 u^2 + a3 u^3
struct PathCubic_unk {
    // inferred: empty constructor (as WarLionCubic_unk): copied float by float
    PathCubic_unk() {}
    float m0;
    float m1;
    float m2;
    float m3;
};

// inferred: lengths of the segments: a class derived from std::vector<float> (its destructor has one more
// level than those of the other vectors, as WarLionPathList_unk of game/Em403WarLion.h)
struct PathLengths_unk : public std::vector<float> {};

// inferred: natural cubic spline through the points (names invented; see game/PathData_unk.cpp; same
// class as WarLionPath_unk of game/Em403WarLion.h)
struct PathSpline_unk {
    PathSpline_unk() : mNumSegments(0), mLength(0.0f) {}
    int mNumSegments;                    // 0x00
    float mLength;                       // 0x04
    atn::Vector3 mStart;                 // 0x08
    atn::Vector3 mEnd;                   // 0x14
    std::vector<PathCubic_unk> mX;       // 0x20
    std::vector<PathCubic_unk> mY;       // 0x2C
    std::vector<PathCubic_unk> mZ;       // 0x38
    PathLengths_unk mLengths;            // 0x44
};

// inferred: path of the event cameras (class name from the vtable, 0x84 bytes). The point
// editor of game/PathData_unk.cpp works on the same object.
// inferred: path (the owner m04 is the scene the points are relative to)
// NONMATCHING: implicit copy constructor 95.7% (Event/Path.cpp) - r6/r7 swapped between the vtable and m08
class Path : public CommonData {
public:
    Path() : m18(1) {}
    virtual void vf08() {}
    virtual void vf0C() {}
    virtual int vf10() const { return 1; }

    int m18;
    std::string m1C;
    std::vector<PathPoint_unk> mPoints;  // 0x28
    PathSpline_unk mSpline;              // 0x34
};

// inferred: what an event camera looks at or is placed on: an actor (optionally one of its
// nodes), an offset, or a path
struct CameraTarget_unk {
    CameraTarget_unk(Actor_unk* actor = 0, atn::Vector3 offset = lbl_807C7030)
        : mActor(actor), mNode(0), mOffset(offset), mPath(0) {}
    Actor_unk* mActor;    // 0x00
    int mNode;            // 0x04: inferred: node name hash
    atn::Vector3 mOffset; // 0x08
    Path* mPath;          // 0x14
};

// inferred: camera of the events (0x65C bytes; four of them in Event::Manager at +0xC8, created
// with __construct_array). Name invented.
// NONMATCHING: implicit Camera_unk::operator= emitted here 97.3% - the original copies the members
// from 0x10C first (layout of Camera_unk not fully known)
class EventCamera_unk : public Camera_unk {
public:
    EventCamera_unk();
    void fn_80112784(); // inferred: reset (Event/Sequence.cpp)
    void fn_80112840(); // inferred: aspect, clip planes and fovy from the game
    void fn_801128C4(int resetAngles);
    void fn_80112918(short a, short b, int id);
    void fn_80112974(int on, float a, float b);
    void fn_801129B4();
    void fn_801129C0(float t);
    static atn::Vector3 fn_80112A00(const CameraTarget_unk& target, float t); // inferred: world position
    void fn_80112B64(const atn::Vector3& pos, Path path, int keep);
    void fn_80112F5C(const CameraTarget_unk& eye, const CameraTarget_unk& at, int resetAngles, int keep, int id,
                     float roll);
    void fn_8011375C(const CameraTarget_unk& eye, int reset, float a, float b, float c);
    void fn_801139E4(const CameraTarget_unk& at, int reset, float dist, float roll);
    void fn_80113CD4(const CameraTarget_unk& eye, const CameraTarget_unk& at, int id, int reset, float roll);
    void fn_801140A0(Path* path, const CameraTarget_unk& at, int id, int reset, float speed, float t, float roll);
    void fn_80114468();
    void fn_8011447C(float a, float b, float c, float d, float e, float f, int frames, int arg);
    void fn_801144A8();
    void fn_8011462C(int on, const atn::Vector3& amp, const atn::Vector3& freq, float a, float b);
    void fn_801146A4(); // inferred: shake
    void fn_80114AB4(); // inferred: camera animation (Xsca)
    void fn_801151A4(); // inferred: rotation with the pad
    void fn_80115704(); // inferred: zoom with the pad
    void fn_801157DC(); // inferred: moves towards the goal
    void fn_801162A4(int arg); // inferred: update (Event/Manager.cpp)
    int fn_80116E7C();
    inline void MoveTarget_unk(const atn::Vector3& goal); // inferred: name invented

    // inferred: weak copies 0x80116BB8 and 0x80116BC8
    // NONMATCHING: 98.8% - the original stores m1FC straight from f1 (stfs without frsp)
    void SetFovy_unk(double fovy) {
        m1FC = fovy;
        mFovy = fovy;
    }
    int IsFlag1_unk() { return mBits.mFlag1; }

    int m1F4;                 // 0x1F4: inferred: mode (0 off, 1-6)
    union {
        unsigned int m1F8;    // 0x1F8
        struct {
            unsigned int mFlag0 : 1;
            unsigned int mFlag1 : 1;
            unsigned int mFlag2 : 1;
            unsigned int mFlagRest : 29;
        } mBits;
    };
    float m1FC;               // 0x1FC: inferred: default fovy
    float m200;               // 0x200
    atn::Vector3 m204;        // 0x204: inferred: position goal
    CameraTarget_unk m210;    // 0x210: inferred: eye
    atn::Vector3 m228;        // 0x228: inferred: target goal
    atn::Vector3 m234;
    CameraTarget_unk m240;    // 0x240: inferred: target
    float m258;               // 0x258: inferred: roll goal
    atn::Vector3 m25C;        // 0x25C: inferred: angles moved with the pad
    atn::Vector3 m268;        // 0x268: inferred: limits of m25C
    atn::Vector3 m274;
    short m280;
    short m282;
    short m284;
    short m286;
    float m288;
    int m28C;
    int m290;
    float m294;
    float m298;
    Path m29C;                // 0x29C
    Path m320;                // 0x320
    atn::Vector3 m3A4;
    float m3B0;
    float m3B4;
    atn::Vector3 m3B8;        // 0x3B8: inferred: shake amplitude
    atn::Vector3 m3C4;        // 0x3C4: inferred: shake speed
    float m3D0;
    float m3D4;
    atn::Vector3 m3D8;        // 0x3D8: inferred: current amplitude
    atn::Vector3 m3E4;        // 0x3E4: inferred: current speed
    atn::Vector3 m3F0;
    float m3FC;               // 0x3FC: inferred: shake time (-1 off)
    ::Xsca_unk* m400;         // 0x400: inferred: camera animation
    float m404;
    float m408;
    float m40C;
    float m410;
    float m414;
    float m418;
    int m41C;
    int m420;
    float m424;
    float m428;
    float m42C;
    float m430;
    float m434;
    float m438;
    int m43C;
    int m440;
    Camera_unk m444;          // 0x444
    float m638;
    float m63C;
    int m640;
    float m644;
    float m648;
    float m64C;
    float m650;
    float m654;
    float m658;
};

} // namespace Event

#endif
