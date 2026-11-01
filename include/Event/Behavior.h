#ifndef EVENT_BEHAVIOR_H
#define EVENT_BEHAVIOR_H

// Event behaviours (Event::BehaviorMoveTo/Follow/Gesture) and the event objects they read. Only
// what Event/BehaviorFollow.cpp uses is declared; member names are invented.

#include "math/Vec.h"
#include "Event/Action.h"

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

class Cut;

// inferred: an actor of the event (0x1C0 bytes)
struct Actor_unk {
    CharaTask* fn_800128FC();      // inferred: character of the actor (0 if none)
    atn::Vector3 fn_80010374();    // inferred: position of the actor
    atn::Model* fn_80012A1C();     // inferred: model of the actor (0 if none)
    // inferred: plays motion `motion` on the actor (used by Event::BehaviorGesture)
    void fn_800119C0(int arg1, int motion, int arg3, int arg4, void* arg5, int arg6, int arg7, float arg8,
                     float arg9);
    void SetWait_unk(unsigned int v) { mWait_unk = v; } // inferred: inlined setter
    unsigned int GetWait_unk() const { return mWait_unk; }

    unsigned char m00[0x14];
    int m14; // inferred: id
    unsigned int m18_0 : 2;
    unsigned int mWait_unk : 1; // inferred: set while the actor has no character or is moving
    unsigned int m18_3 : 29;
    unsigned char m1C[0x20 - 0x1C];
    int m20; // inferred: state (6 is checked by BehaviorFollow::vf0C)
    unsigned char m24[0xB8 - 0x24];
    class Action* mB8[3]; // 0xB8: inferred: motion actions playing on the actor (ActionActorMotion)
    unsigned char mC4[0x1C0 - 0xC4];
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
        for (int i = 0; i < m168; i++) {
            Actor_unk* actor = &m164[(unsigned int)i];
            if (id == actor->m14) {
                return actor;
            }
        }
        return 0;
    }

    // inferred: rotates `v` by the yaw at 0xB4 (0x80541C94 in Event/Sequence)
    atn::Vector3 fn_80541C94(const atn::Vector3& v);

    // inferred: cut list entry (the cut and a second word)
    struct CutEntry_unk {
        Cut* mCut;
        int m04;
    };
    unsigned char m000[0xB8];
    CutEntry_unk* mB8; // 0xB8: inferred: cuts of the event
    int mBC;
    unsigned char mC0[0x114 - 0xC0];
    struct AudioEntry_unk* m114; // 0x114: inferred: multi audio streams of the event (0x18 bytes each)
    int m118;
    unsigned char m11C[0x124 - 0x11C];
    struct CameraAnim_unk* m124; // 0x124: inferred: camera animations of the event (0x1C bytes each)
    int m128;
    unsigned char m12C[0x164 - 0x12C];
    Actor_unk* m164;
    int m168;
    unsigned char m16C[0x190 - 0x16C];
    int m190; // inferred: id of the event (0x5E6 halves the music fades)
    unsigned char m194[0x198 - 0x194];
    float m198; // 0x198: inferred: current frame
    unsigned char m19C[0x1AC - 0x19C];
    int m1AC; // inferred: game variable changed by ActionNotify (-1 none)
    unsigned char m1B0[0x1D4 - 0x1B0];
    unsigned char m1D4[0x1EC - 0x1D4]; // inferred: message table of the event (fn_803B3B38)
    struct MsgWindow_unk* m1EC;        // inferred: message window
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
