#ifndef EVENT_ACTOR_H
#define EVENT_ACTOR_H

// Event::Actor (vtable __vt__Q25Event5Actor, code in Event/Puppeteer_CharaTask.cpp): an actor of an event
// sequence. Still named Actor here (the mangled names of its methods use it); member names invented.

#include "math/Vec.h"
#include "Event/CommonData.h"
#include "Event/PathSpline.h"
#include "atn/Array_unk.h"
#include "std/vector.h"
#include "game/IntVector_unk.h"
#include "atn/ColorF_unk.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

class CharaTask;
namespace atn {
class Task;
}

namespace atn {
class Model;
}

namespace Event {

class Action;
class Path;
class Sequence;

// inferred: 0x2C-byte key of the animation lists of an actor (a constructor: built with
// __construct_new_array); content unknown
struct ActorKey2C_unk {
    ActorKey2C_unk() {}
    int m00;
    atn::Vector3 m04;
    atn::Vector3 m10;
    atn::Vector3 m1C;
    float m28;
};

// inferred: 0x10-byte key of the lists of an actor (built with __construct_new_array): offset saved for a
// node of the model (ActionActorNode::vf08)
struct ActorKey10_unk {
    ActorKey10_unk() {}
    int m00;          // inferred: node index
    atn::Vector3 m04; // inferred: offset (position in m118, rotation in m124)
};

// inferred: 8-byte entry of the list at 0xAC of an actor
struct ActorPair_unk {
    int m00;
    int m04;
};

// inferred: animation state of an actor (0x34 bytes at 0x24)
struct ActorAnim_unk {
    atn::SimpleVector<ActorKey2C_unk> m00;
    int m0C; // inferred: saved by fn_8000F1EC, given back with CharaTask::fn_8014DEE4 (-1: none)
    // inferred: saved position (0x10) and rotation (0x1C) of the character, two atn::Vector3: as Vector3
    // members the implicit constructor of the actor renumbers the @N labels of Event/Sequence and
    // Event/EventParser_unk (see docs/paralelo-20261113-qG2.md)
    unsigned char m10[0x28 - 0x10];
    IntVector_unk m28; // inferred: saved states of the physics bones
};

// inferred: eye movement of an actor (0x34 bytes at 0x18C, cleared by its constructor; names invented)
struct ActorEyes_unk {
    ActorEyes_unk() { Clear_unk(); }
    void Clear_unk() { memset(this, 0, sizeof(ActorEyes_unk)); }
    int m00;             // inferred: frames left of the current eye movement (ActionActorEyes)
    atn::Vector3 m04;    // inferred: goal of the eyes
    atn::Vector3 m10;    // inferred: current eye angles
    atn::Vector3 m1C;
    atn::Vector3 m28; // inferred: point looked at (ActionActorEyes::vf0C)
};

// inferred: an actor of the event (0x1C0 bytes)
struct Actor : public CommonData {
    Actor(class Sequence* owner, const char* name, int arg); // 0x8000D528
    virtual void vf08();
    virtual void vf0C();
    virtual int vf10() const;
    inline CharaTask* GetChara_unk(); // inferred: defined in Event/Puppeteer_CharaTask.cpp
    inline Sequence* GetSequence_unk() const; // inferred: weak copy 0x8000EB84 (defined in Event/Puppeteer_CharaTask.cpp)
    // inferred: binds the actor to what its name designates (Actor::vf08; names invented)
    void fn_8000DB24(Sequence* seq, int kind, int id); // inferred: set point character ("em", "fr", "np")
    void fn_8000E474(Sequence* seq, int kind, int id); // inferred: event character ("ev")
    void fn_8000E5D0(int id, int usbId);               // inferred: gimmick ("gm")
    void fn_8000E6B0(Sequence* seq, int id);           // inferred: model of the sequence ("md")
    void fn_8000E7B8(Sequence* seq, int id);           // inferred: data of the list 0x104 of the sequence ("dm")
    CharaTask* fn_800128FC();      // inferred: character of the actor (0 if none)
    CharaTask* fn_80012934();      // inferred: same code as fn_800128FC
    void fn_80012540(int arg);
    void fn_8001256C(int show);
    void fn_800125C8(int on);
    void* fn_8001260C();
    void fn_80012638(int on);
    void fn_8001271C(int on);
    void fn_80012774(int on);
    void fn_80012CBC(const atn::Vector3& pos, float speed);
    void fn_80012CE4(const atn::Vector3& pos, float speed);
    void fn_80012D0C(float speed);
    void fn_800134B8(const atn::Vector3& pos, float speed);
    void fn_800134E0(const atn::Vector3& pos, float speed);
    void fn_80013508(float* target, const atn::Vector3& pos, float speed);
    void fn_80013530(float speed);
    atn::Vector3 fn_80010374();    // inferred: position of the actor
    atn::Vector3 fn_80010B68() const; // inferred: rotation of the actor
    atn::Model* fn_8001296C();     // inferred: same code as fn_80012A1C
    atn::Model* fn_80012A1C();     // inferred: model of the actor (0 if none)
    inline atn::Model* GetModel_unk(); // inferred: inlined copy of fn_8001296C (Event/Puppeteer_CharaTask.cpp)
    MtxPtr fn_80012AC4(unsigned int node); // inferred: node matrix of the actor (ActionEffectPlay)
    void fn_8001268C(int msg, int state); // inferred: message to a gimmick actor
    void fn_8000EE8C();            // inferred: update (Event::Sequence::vf10)
    void fn_8000F1EC();            // inferred: start of the event
    void fn_8000FAE8();            // inferred: end of the event
    int fn_800127CC(int a, int b); // inferred: the character has the part (a: id)
    void fn_8001282C(int a, int b);
    // inferred: plays motion `motion` on the actor (used by Event::BehaviorGesture)
    void fn_800119C0(int arg1, int motion, int arg3, int arg4, void* arg5, int arg6, int arg7, float arg8,
                     float arg9);
    void fn_80011E98(int, Sequence*, int, int, int, float); // inferred: facial motion (ActionActorFacial)
    void fn_80012054(int, int, int, float, float);         // inferred: motion of a slot (Event::Dialog)
    void fn_80012294(int, int, int);                       // inferred: stops a track (ActionActorMotion)
    void fn_80013558(); // inferred: saves the material colours of the model (ActionActorMaterial::vf0C)
    void fn_8001011C(const atn::Vector3& pos); // inferred: moves the actor to `pos`
    void fn_80010490(const atn::Vector3& rot); // inferred: sets the rotation (radians)
    void fn_80011420(float yaw);               // inferred: sets the rotation around y
    inline void SetRotation_unk(const atn::Vector3&); // inferred: body of fn_80010490 (Event/Puppeteer_CharaTask.cpp)
    // inferred: places the actor with a matrix, optionally relative to the node matrix `node`
    void fn_8001122C(MtxPtr node, int arg2, int arg3, const atn::Matrix3x4& mtx);
    void fn_800121F0(int arg, float distance); // inferred: called with the distance moved
    float fn_80011964(); // inferred: rotation around y
    atn::Vector3 fn_800112E0(); // inferred: front of the actor
    // inferred: turns the eyes towards `target` (node, camera position, jitter, weight, angle)
    void fn_80012D34(Actor* target, int node, const atn::Vector3& cam, int arg4, const atn::Vector3& jitter,
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
    inline void SetTarget_unk(atn::Vector3& v);
    inline int GetState_unk() const;
    inline atn::Vector3 GetPathPos_unk(float t);
    inline void TurnTo_unk(const atn::Vector3& dir);
    inline void SetPath_unk(class Path* path); // inferred: builds mPath from an event path

    union {
        struct {
            unsigned int m18_0 : 1; // inferred: the state of the character is saved (fn_8000F1EC/fn_8000FAE8)
            unsigned int m18_1 : 1;
            unsigned int mWait_unk : 1; // inferred: set while the actor has no character or is moving
            unsigned int m18_3 : 3;
            unsigned int m18_6 : 1; // inferred: set for set point characters and gimmicks
            unsigned int m18_7 : 1; // inferred: the actor is not reset at the start of the event (Event::Sequence)
            // inferred: state saved by fn_8000F1EC and given back by fn_8000FAE8
            unsigned int m18_8 : 8;  // inferred: control mode (CharaTask::fn_8016E970)
            unsigned int m18_16 : 1; // inferred: task stopped (atn::Task::mFlags 4)
            unsigned int m18_17 : 1; // inferred: argument of atn::Task::fn_800D246C
            unsigned int m18_18 : 1; // inferred: shown (CharaTask::fn_8016E4C4; GimmickCommon::m9C)
            unsigned int m18_19 : 1; // inferred: bit 0 of the collision flags
            unsigned int m18_20 : 1; // inferred: bit 0 of CharaStatus::mParamB.m9C
            unsigned int m18_21 : 1; // inferred: model visible (CharaTask::vf98/vf9C)
            unsigned int m18_22 : 1; // inferred: motion commands enabled (CharaTask::m1220.m10)
            unsigned int m18_23 : 9;
        };
        unsigned int m18;
    };
    atn::Task* m1C; // inferred: what the actor drives (by m20: 1 and 7 Puppeteer<CharaTask>, 2 a gimmick, 3 Puppeteer<atn::Model>)
    int m20; // inferred: kind of actor (6 is checked by BehaviorFollow::vf0C)
    ActorAnim_unk m24;    // 0x24
    int m58;              // inferred: -1 after construction
    PathSpline_unk mPath; // 0x5C: inferred: path of the actor (ActionActorPosition, mode 1)
    std::vector<ActorPair_unk> mAC;
    class Action* mB8[3]; // 0xB8: inferred: motion actions playing on the actor (ActionActorMotion)
    atn::Vector3 mC4;     // 0xC4: inferred: rotation
    atn::Vector3 mD0;     // 0xD0: inferred: position the actor moves from
    atn::Vector3 mDC;
    atn::Vector3 mE8;
    atn::Vector3 mF4;
    atn::Vector3 m100;
    atn::Vector3 m10C;
    atn::SimpleVector<ActorKey10_unk> m118;
    atn::SimpleVector<ActorKey10_unk> m124;
    int m130; // inferred: curve of the moves (fn_8048BD04); -1 after construction
    int m134; // inferred: -1 after construction
    int m138;
    int m13C;
    int m140;
    unsigned char m144; // inferred: 1 after construction
    unsigned char m145;
    unsigned char m146;
    IntVector_unk m148;
    int m154;
    std::vector<ColorF_unk> m158; // inferred: material colors of the model (Actor::vf08)
    IntVector_unk m164; // inferred: materials shown (Actor::vf08)
    std::vector<ColorF_unk> m170;
    IntVector_unk m17C;
    unsigned char m188; // inferred: 1 after construction
    ActorEyes_unk m18C; // 0x18C
};

} // namespace Event

#endif
