#include "Event/Behavior.h"

// Actor actions of the events: processors with vf08 (per frame) and vf0C (phase), as in
// Event/ActionCameraMode. Class names from the RTTI; vtables in the order Material, Wind, Setting,
// Node, Heading, Pose, Eyes, Facial, Motion, Position.

namespace atn {

// inferred: base named in the RTTI of the processors (no member is used here)
template <class T>
class SingletonNoStatic {};

} // namespace atn

// inferred: model of an actor (0 if none); not the same as Actor_unk::fn_80012A1C
extern "C" atn::Model* fn_8001296C(Event::Actor_unk* actor);
// inferred: starts a wind on the model (direction, frames, two strengths)
extern "C" void fn_80097750(atn::Model* model, const atn::Vector3& dir, int frames, float a, float b);

namespace Event {

// inferred: the event manager (lbl_8087F540) seen through its frame step
struct EventStep_unk {
    unsigned char m00[0xB4];
    float mB4; // inferred: frames of the current step
};

} // namespace Event

extern "C" Event::EventStep_unk* lbl_8087F540;

namespace Event {

class ActionActorPosition : public ActionProcessor, public atn::SingletonNoStatic<ActionActorPosition> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorMotion : public ActionProcessor, public atn::SingletonNoStatic<ActionActorMotion> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorFacial : public ActionProcessor, public atn::SingletonNoStatic<ActionActorFacial> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorEyes : public ActionProcessor, public atn::SingletonNoStatic<ActionActorEyes> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorPose : public ActionProcessor, public atn::SingletonNoStatic<ActionActorPose> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorHeading : public ActionProcessor, public atn::SingletonNoStatic<ActionActorHeading> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorNode : public ActionProcessor, public atn::SingletonNoStatic<ActionActorNode> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorSetting : public ActionProcessor, public atn::SingletonNoStatic<ActionActorSetting> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorWind : public ActionProcessor, public atn::SingletonNoStatic<ActionActorWind> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

class ActionActorMaterial : public ActionProcessor, public atn::SingletonNoStatic<ActionActorMaterial> {
public:
    virtual int vf08(Action* arg);
    virtual int vf0C(Action* arg, int phase);
};

// inferred: moves the actor of a position action (0x80551194); t is the number of frames left
extern "C" void fn_80551194(Action* arg, float t);

// NONMATCHING: 96.2% - the original loads lbl_8087F540 before the current frame (tried locals, the
// comparison both ways and a ternary)
int ActionActorPosition::vf08(Action* arg) {
    int length = arg->m18;
    int start = arg->m10;
    EventManager_unk* manager = arg->fn_805381C8();
    float now = manager->m198;
    float left = (float)(start + length) - now;
    if (left < lbl_8087F540->mB4) {
        left = 0.0f;
    }
    fn_80551194(arg, left);
    return 0;
}

// inferred: sets the facial expression of an actor (0x80011E98)
extern "C" void fn_80011E98(Actor_unk* actor, int face, EventManager_unk* manager, int a, int b, int c, float d);

int ActionActorFacial::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        Actor_unk* actor = manager->FindActor_unk(id);
        int face = arg->GetParam_unk(0)->m04.i;
        // inferred: parameters 1 and 2 are read but not used (only their loads are left)
        float unused1 = arg->GetParam_unk(1)->m04.f;
        float unused2 = arg->GetParam_unk(2)->m04.i;
        int a = arg->GetParam_unk(4)->m04.i;
        int b = arg->GetParam_unk(5)->m04.i;
        int c = arg->GetParam_unk(6)->m04.i;
        float d = arg->GetParam_unk(7)->m04.f;
        fn_80011E98(actor, face, manager, a, b, c, d);
    }
    return 0;
}

int ActionActorWind::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        atn::Model* model = fn_8001296C(manager->FindActor_unk(id));
        if (model != 0) {
            atn::Vector3 dir;
            dir = arg->fn_80538B18(0);
            float b = arg->GetParam_unk(3)->m04.f;
            float a = arg->GetParam_unk(4)->m04.f;
            fn_80097750(model, dir, arg->m14 - arg->m10, a, b);
        }
        break;
    }
    case 6:
        arg->fn_80538FD4(0);
        break;
    }
    return 0;
}

int ActionActorWind::vf08(Action* arg) {
    return 0;
}

int ActionActorPose::vf08(Action* arg) {
    return 0;
}

int ActionActorFacial::vf08(Action* arg) {
    return 0;
}

int ActionActorMotion::vf08(Action* arg) {
    return 0;
}

} // namespace Event
