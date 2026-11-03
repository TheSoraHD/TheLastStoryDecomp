#ifndef EVENT_ACTOR_H
#define EVENT_ACTOR_H

// Event::Actor (vtable __vt__Q25Event5Actor, code in Event/Puppeteer_CharaTask.cpp): an actor of an event
// sequence. Still named Actor_unk here (the mangled names of its methods use it); member names invented.

#include "math/Vec.h"
#include "Event/CommonData.h"
#include "Event/PathSpline.h"

class CharaTask;

namespace atn {
class Model;
}

namespace Event {

class Action;
class Path;
class Sequence;

// inferred: an actor of the event (0x1C0 bytes)
struct Actor_unk : public CommonData {
    Actor_unk(class Sequence* owner, const char* name, int arg); // 0x8000D528
    CharaTask* fn_800128FC();      // inferred: character of the actor (0 if none)
    atn::Vector3 fn_80010374();    // inferred: position of the actor
    atn::Vector3 fn_80010B68();    // inferred: rotation of the actor
    atn::Model* fn_80012A1C();     // inferred: model of the actor (0 if none)
    void fn_8000EE8C();            // inferred: update (Event::Sequence::vf10)
    void fn_8000F1EC();            // inferred: start of the event
    void fn_8000FAE8();            // inferred: end of the event
    void fn_800127CC(int a, int b); // inferred: pose of the actor (from an action of a character channel)
    void fn_8001282C(int a, int b);
    // inferred: plays motion `motion` on the actor (used by Event::BehaviorGesture)
    void fn_800119C0(int arg1, int motion, int arg3, int arg4, void* arg5, int arg6, int arg7, float arg8,
                     float arg9);
    void fn_8001011C(const atn::Vector3& pos); // inferred: moves the actor to `pos`
    void fn_80010490(const atn::Vector3& rot); // inferred: sets the rotation (radians)
    void fn_80011420(float yaw);               // inferred: sets the rotation around y
    // inferred: places the actor with a matrix, optionally relative to the node matrix `node`
    void fn_8001122C(MtxPtr node, int arg2, int arg3, const atn::Matrix3x4& mtx);
    void fn_800121F0(int arg, float distance); // inferred: called with the distance moved
    // inferred: turns the eyes towards `target` (node, camera position, jitter, weight, angle)
    void fn_80012D34(Actor_unk* target, int node, const atn::Vector3& cam, int arg4, const atn::Vector3& jitter,
                     int arg6, float weight, float angle);
    void SetWait_unk(unsigned int v) { mWait_unk = v; } // inferred: inlined setter
    unsigned int GetWait_unk() const { return mWait_unk; }
    // inferred: accessors called out of line by Event/ActionActorPosition (weak copies 0x80551D74-0x805521D8;
    // names invented). Defined in that file: a new inline body in this header renumbers the @N labels of
    // every file that includes it.
    inline bool IsState6_unk() const;
    inline int GetCurve_unk() const;
    inline atn::Vector3 GetTarget_unk() const;
    inline atn::Vector3 GetRot_unk() const;
    inline void SetRot_unk(const atn::Vector3& v);
    inline void SetTarget_unk(const atn::Vector3& v);
    inline int GetState_unk() const;
    inline atn::Vector3 GetPathPos_unk(float t);
    inline void TurnTo_unk(const atn::Vector3& dir);
    inline void SetPath_unk(class Path* path); // inferred: builds mPath from an event path

    unsigned int m18_0 : 2;
    unsigned int mWait_unk : 1; // inferred: set while the actor has no character or is moving
    unsigned int m18_3 : 4;
    unsigned int m18_7 : 1; // inferred: the actor is not reset at the start of the event (Event::Sequence)
    unsigned int m18_8 : 24;
    unsigned char m1C[0x20 - 0x1C];
    int m20; // inferred: state (6 is checked by BehaviorFollow::vf0C)
    unsigned char m24[0x5C - 0x24];
    PathSpline_unk mPath; // 0x5C: inferred: path of the actor (ActionActorPosition, mode 1)
    unsigned char mAC[0xB8 - 0xAC];
    class Action* mB8[3]; // 0xB8: inferred: motion actions playing on the actor (ActionActorMotion)
    atn::Vector3 mC4;     // 0xC4: inferred: rotation
    atn::Vector3 mD0;     // 0xD0: inferred: position the actor moves from
    unsigned char mDC[0x130 - 0xDC];
    int m130; // inferred: curve of the moves (fn_8048BD04)
    unsigned char m134[0x18C - 0x134];
    int m18C;             // 0x18C: inferred: frames left of the current eye movement (ActionActorEyes)
    atn::Vector3 m190;    // 0x190: inferred: goal of the eyes
    atn::Vector3 m19C;    // 0x19C: inferred: current eye angles
    atn::Vector3 m1A8;    // 0x1A8
    unsigned char m1B4[0x1C0 - 0x1B4];
};

} // namespace Event

#endif
