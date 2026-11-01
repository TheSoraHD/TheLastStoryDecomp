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

// NONMATCHING: 99.7% - the quotient is in f1 here, f0 in the original (tried several groupings)
// inferred: scales a displacement to the part covered in one step of the event (`frames` left)
void ScaleStep_unk(atn::Vector3& v, float frames) {
    float len = PSVECMag(&v);
    if (len > 0.0f) {
        float scale = (len / frames) * lbl_8087F540->mB4;
        PSVECNormalize(&v, &v);
        v.x *= scale;
        v.y *= scale;
        v.z *= scale;
    }
}

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

extern "C" void fn_80012540(Actor_unk* actor, int arg);                 // inferred: stops the motions
extern "C" void fn_80012294(Actor_unk* actor, int track, int a, int b); // inferred: stops a track

// inferred: free motion slot of the actor for `arg` (-1 if none)
inline int AddMotionAction_unk(Actor_unk* actor, Action* arg) {
    for (int i = 0; i < 3; i++) {
        if (actor->mB8[i] == 0) {
            actor->mB8[i] = arg;
            return i;
        }
    }
    return -1;
}

// NONMATCHING: 97.2% - saved registers: the original reuses the register of the section for the actor
int ActionActorMotion::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor_unk* actor = manager->FindActor_unk(section->m10);
        int motion = arg->GetParam_unk(0)->m04.i;
        int a1 = arg->GetParam_unk(1)->m04.i;
        float blend1 = arg->GetParam_unk(2)->m04.f;
        float speed1 = arg->GetParam_unk(3)->m04.i;
        int b1 = arg->GetParam_unk(4)->m04.i;
        int upper = arg->GetParam_unk(5)->m04.i;
        int a2 = arg->GetParam_unk(6)->m04.i;
        float blend2 = arg->GetParam_unk(7)->m04.f;
        float speed2 = arg->GetParam_unk(8)->m04.i;
        int b2 = arg->GetParam_unk(9)->m04.i;
        int stop = arg->GetParam_unk(10)->m04.i;
        int c = arg->GetParam_unk(11)->m04.i;
        fn_80012540(actor, stop);
        int slot = AddMotionAction_unk(actor, arg);
        if (slot == -1) {
            break;
        }
        if (upper != 0) {
            actor->fn_800119C0(1, a1, b1, motion, manager, slot, c, blend1, speed1);
            actor->fn_800119C0(2, a2, b2, motion, manager, slot, c, blend2, speed2);
        } else {
            actor->fn_800119C0(0, a1, b1, motion, manager, slot, c, blend1, speed1);
            fn_80012294(actor, 2, 0, 0);
        }
        break;
    }
    case 3:
    case 4: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor_unk* actor = manager->FindActor_unk(section->m10);
        if (actor->mB8[0] == arg) {
            actor->mB8[0] = 0;
        }
        if (actor->mB8[1] == arg) {
            actor->mB8[1] = 0;
        }
        if (actor->mB8[2] == arg) {
            actor->mB8[2] = 0;
        }
        break;
    }
    }
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
