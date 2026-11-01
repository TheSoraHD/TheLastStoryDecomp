#ifndef EVENT_BEHAVIOR_H
#define EVENT_BEHAVIOR_H

// Event behaviours (Event::BehaviorMoveTo/Follow/Gesture) and the event objects they read. Only
// what Event/BehaviorFollow.cpp uses is declared; member names are invented.

#include "math/Vec.h"
#include "Event/Action.h"
#include "atn/SoundRef.h"
#include "Event/PathSpline.h"

class CharaTask;

namespace atn {

class Model;
class LH_Xsca;

// inferred: base giving the class a single instance (a function-local static, see
// Event/Behavior_unk.cpp)
template <class T>
class Singleton {
public:
    static T& GetInstance_unk() {
        static T sInstance;
        return sInstance;
    }
};

} // namespace atn

namespace Event {

class Dialog;
class Path;

class Cut;

// inferred: an actor of the event (0x1C0 bytes)
struct Actor_unk {
    CharaTask* fn_800128FC();      // inferred: character of the actor (0 if none)
    atn::Vector3 fn_80010374();    // inferred: position of the actor
    atn::Vector3 fn_80010B68();    // inferred: rotation of the actor
    atn::Model* fn_80012A1C();     // inferred: model of the actor (0 if none)
    // inferred: plays motion `motion` on the actor (used by Event::BehaviorGesture)
    void fn_800119C0(int arg1, int motion, int arg3, int arg4, void* arg5, int arg6, int arg7, float arg8,
                     float arg9);
    void fn_8001011C(const atn::Vector3& pos); // inferred: moves the actor to `pos`
    void fn_80010490(const atn::Vector3& rot); // inferred: sets the rotation (radians)
    void fn_80011420(float yaw);               // inferred: sets the rotation around y
    // inferred: places the actor with a matrix, optionally relative to the node matrix `node`
    void fn_8001122C(MtxPtr node, int arg2, int arg3, const atn::Matrix3x4& mtx);
    void fn_800121F0(int arg, float distance); // inferred: called with the distance moved
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

    unsigned char m00[0x14];
    int m14; // inferred: id
    unsigned int m18_0 : 2;
    unsigned int mWait_unk : 1; // inferred: set while the actor has no character or is moving
    unsigned int m18_3 : 29;
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
    unsigned char m134[0x1C0 - 0x134];
};

// inferred: objects of the event found by their id at 0x14 (actors, paths): {data, count} (names
// invented); Find_unk is a template member emitted at the end of the file (0x8055A400, defined in
// Event/ActionActorPosition.cpp)
template <class T>
struct IdList_unk {
    T* Find_unk(int id);
    T* mData;
    int mSize;
};

// inferred: camera animation of an event (0x1C bytes, found by id)
struct CameraAnim_unk {
    unsigned char m00[0x14];
    int m14;              // inferred: id
    atn::LH_Xsca* m18;    // inferred: the loaded animation
};

// inferred: event manager returned by Action::fn_805381C8
struct EventManager_unk {
    // inferred: lookup of an actor by id (inlined)
    // The unsigned index gives the registers of the original in BehaviorFollow/BehaviorMoveTo::vf0C
    Actor_unk* FindActor_unk(int id) {
        for (int i = 0; i < m164.mSize; i++) {
            Actor_unk* actor = &m164.mData[(unsigned int)i];
            if (id == actor->m14) {
                return actor;
            }
        }
        return 0;
    }

    // inferred: lookup of a path by id (inlined in ActionActorPosition::vf0C; defined there)
    inline Path* FindPath_unk(int id);
    inline IdList_unk<Actor_unk>* GetActors_unk(); // inferred: weak copy 0x80551D20 (Event/ActionActorPosition.cpp)

    // inferred: rotates `v` by the yaw at 0xB4 (0x80541C94 in Event/Sequence)
    atn::Vector3 fn_80541C94(const atn::Vector3& v);
    void fn_8053E8C8(int state); // inferred: sets the state (0x194)

    // inferred: cut list entry (the cut and a second word)
    struct CutEntry_unk {
        Cut* mCut;
        int m04;
    };
    unsigned char m000[0x50];
    int m50; // inferred: kind of event (3: messages go to the HUD window)
    unsigned char m054[0x98 - 0x54];
    unsigned int m98; // inferred: flags (0x40000 cleared by the checks of Event::ActionCheck)
    unsigned char m09C[0xB8 - 0x9C];
    CutEntry_unk* mB8; // 0xB8: inferred: cuts of the event
    int mBC;
    unsigned char mC0[0xD4 - 0xC0];
    struct AnimEntry_unk* mD4; // 0xD4: inferred: 2D animations of the event (0x20 bytes each)
    int mD8;
    unsigned char mDC[0xE4 - 0xDC];
    struct EffectEntry_unk* mE4; // 0xE4: inferred: effects of the event (0x20 bytes each)
    int mE8;
    unsigned char mEC[0x114 - 0xEC];
    struct AudioEntry_unk* m114; // 0x114: inferred: multi audio streams of the event (0x18 bytes each)
    int m118;
    unsigned char m11C[0x124 - 0x11C];
    struct CameraAnim_unk* m124; // 0x124: inferred: camera animations of the event (0x1C bytes each)
    int m128;
    unsigned char m12C[0x144 - 0x12C];
    struct MovieEntry_unk* m144; // 0x144: inferred: movies of the event (0x1C bytes each)
    int m148;
    unsigned char m14C[0x154 - 0x14C];
    IdList_unk<Path> m154; // 0x154: inferred: paths of the event (0x84 bytes each)
    unsigned char m15C[0x164 - 0x15C];
    IdList_unk<Actor_unk> m164; // 0x164
    unsigned char m16C[0x174 - 0x16C];
    atn::SoundRef m174[4]; // 0x174: inferred: sounds of the event (ActionEnvSound)
    unsigned char m184[0x190 - 0x184];
    int m190; // inferred: id of the event (0x5E6 halves the music fades)
    unsigned char m194[0x198 - 0x194];
    float m198; // 0x198: inferred: current frame
    unsigned char m19C[0x1AC - 0x19C];
    int m1AC; // inferred: game variable changed by ActionNotify (-1 none)
    unsigned char m1B0[0x1D4 - 0x1B0];
    unsigned char m1D4[0x1EC - 0x1D4]; // inferred: message table of the event (fn_803B3B38)
    struct MsgWindow_unk* m1EC;        // inferred: message window
    unsigned char m1F0[0x280 - 0x1F0];
    Dialog* m280; // inferred: balloons of the conversations (ActionDialogText/Speaker, ActionVoicePlay)
};

struct ActionOwner_unk {
    unsigned char m00[0x10];
    int m10; // inferred: id of the actor
};


class ActionProcessor {
public:
    virtual int vf08(Action* arg) = 0;
    virtual int vf0C(Action* arg, int phase) = 0;
};

class BehaviorMoveTo : public ActionProcessor, public atn::Singleton<BehaviorMoveTo> {
public:
    ~BehaviorMoveTo();
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class BehaviorFollow : public ActionProcessor, public atn::Singleton<BehaviorFollow> {
public:
    ~BehaviorFollow();
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class BehaviorGesture : public ActionProcessor, public atn::Singleton<BehaviorGesture> {
public:
    ~BehaviorGesture();
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

// inferred: table of the behaviours (Event/Behavior_unk.cpp); names invented
struct BehaviorRef_unk {
    BehaviorRef_unk() : mBehavior(0) {}
    ActionProcessor* mBehavior;
};

void fn_8010D98C();                     // inferred: fills the table
ActionProcessor* fn_8010DB54(int kind); // inferred: behaviour of a kind

} // namespace Event

#endif
