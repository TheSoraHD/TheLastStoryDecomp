#ifndef EVENT_BEHAVIOR_H
#define EVENT_BEHAVIOR_H

// Event behaviours (Event::BehaviorMoveTo/Follow/Gesture) and the event objects they read. Only
// what Event/BehaviorFollow.cpp uses is declared; member names are invented.

#include "math/Vec.h"
#include "Event/Action.h"
#include "atn/SoundRef.h"
#include "Event/PathSpline.h"
#include "Event/CommonData.h"
#include "Event/Actor.h"
#include "Event/Sequence.h"

class CharaTask;
class GameMessageWindow;

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
class Sequence;
class Path;

class Cut;


// inferred: the event manager returned by Action::fn_805381C8 is the sequence of the cut


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
