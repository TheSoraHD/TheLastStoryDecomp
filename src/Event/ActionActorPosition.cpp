#include "Event/Behavior.h"
#include "atn/Model_unk.h"
#include "math/VecOps_unk.h"

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
    // inferred: moves the actor of a position action; t is the number of frames left (static: vf08
    // only passes the action)
    static void fn_80551194(Action* arg, float t);
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

// inferred: getters compiled in another file (weak copies in Event/DebugInfo_unk)
extern "C" int fn_80224610(ActionParam_unk* param);   // inferred: flag value of a parameter
extern "C" int fn_802245B8(ActionParam_unk* param);   // inferred: integer value of a parameter
extern "C" int fn_80224618(ActionOwner_unk* section); // inferred: section->m10 (actor id)
extern "C" int fn_80223EB8(Action* arg);              // inferred: arg->m18 (length in frames)
extern "C" EventStep_unk* fn_8000DB1C();              // inferred: returns lbl_8087F540
extern "C" float fn_8048BD04(EventStep_unk* step, int curve, float t); // inferred: value of a move curve
extern "C" atn::Vector3 fn_80541BD8(EventManager_unk* manager, const atn::Vector3& pos); // inferred: event to world
extern "C" float fn_80541CF0(EventManager_unk* manager, float yaw); // inferred: event to world yaw
extern "C" void fn_8008CD1C(atn::Matrix3x4* dst, const atn::Matrix3x4* src); // inferred: matrix copy
extern "C" void PSMTXTranspose(const Mtx src, Mtx dst);

// inferred: helpers of the moves (names invented; weak copies after fn_80551194)
inline atn::Vector3 GetAxisZ_unk(const atn::Matrix3x4& m) {
    return atn::Vector3(m.m[2][0], m.m[2][1], m.m[2][2]);
}

// inferred: identity by value (constructed in place: no call to the empty constructor of the local)
inline atn::Matrix3x4 MakeIdentity_unk() {
    atn::Matrix3x4 m;
    m.SetIdentity();
    return m;
}

inline void Transpose_unk(atn::Matrix3x4& m) {
    PSMTXTranspose(m.m, m.m);
}

// inferred: rotation matrix from angles, applied in the order y, x, z
inline atn::Matrix3x4 MakeRotYXZ_unk(const atn::Vector3& rot) {
    atn::Matrix3x4 m;
    m.SetIdentity();
    if (0.0f != rot.y) {
        atn::Matrix3x4::RotateAxis_unk(m, 'y', rot.y);
    }
    if (0.0f != rot.x) {
        atn::Matrix3x4::RotateAxis_unk(m, 'x', rot.x);
    }
    if (0.0f != rot.z) {
        atn::Matrix3x4::RotateAxis_unk(m, 'z', rot.z);
    }
    return m;
}

inline float Distance_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return (b - a).Length_unk();
}

// inferred: a template (its weak copy 0x80552430 comes after the other weak copies)
template <class T>
inline T Clamp_unk(T x, T lo, T hi) {
    hi = (hi <= x) ? hi : x;
    lo = (lo >= hi) ? lo : hi;
    return lo;
}

// NONMATCHING: new
void ActionActorPosition::fn_80551194(Action* arg, float t) {
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    Actor_unk* actor = manager->GetActors_unk()->Find_unk(fn_80224618(section));
    atn::Vector3 start = actor->fn_80010374();
    int moved = 0;
    int moveArg = 0;
    int mode = arg->GetParamAt_unk(0)->GetMode_unk();
    if (mode == 0) {
        // to an actor (or one of its nodes) plus an offset
        Actor_unk* target = manager->GetActors_unk()->Find_unk(arg->GetParamAt_unk(1)->GetActorId_unk());
        atn::Vector3 offset = arg->fn_805381F0(3);
        int keepLocal = fn_80224610(arg->GetParamAt_unk(6));
        int addRot = fn_80224610(arg->GetParamAt_unk(10));
        if (target->IsState6_unk()) {
            offset = fn_80541BD8(manager, offset);
        }
        atn::Vector3 pos = target->fn_80010374();
        atn::Vector3 cur = actor->fn_80010374();
        MtxPtr node = 0;
        atn::Model* model = fn_8001296C(target);
        if (model != 0) {
            node = model->GetNodeMtxByHash_unk(arg->GetParamAt_unk(2)->GetHash_unk());
            if (node != 0) {
                pos.Set_unk(0.0f, 0.0f, 0.0f);
            }
        }
        pos += offset;
        atn::Vector3 rot(lbl_807C7030);
        atn::Vector3 diff = pos - cur;
        int face = fn_80224610(arg->GetParamAt_unk(11));
        if (face != 0) {
            if (node == 0) {
                rot = diff.ToRotation_unk();
            }
        } else {
            rot = arg->fn_80538C14(7);
            fn_DegToRad_unk(&rot.x);
            if (target->IsState6_unk()) {
                rot.y = fn_80541CF0(manager, rot.y);
            }
            if (addRot != 0 && node == 0) {
                rot += arg->fn_80538C14(14);
            }
        }
        if (t > 0.0f) {
            float s = 1.0f - t / (float)fn_80223EB8(arg);
            s = fn_8048BD04(fn_8000DB1C(), actor->GetCurve_unk(), s);
            atn::Vector3 from = actor->GetTarget_unk();
            atn::Vector3 d = pos - from;
            float len = d.Length_unk();
            if (len > 0.0f) {
                d.Normalize_unk();
                len *= s;
            }
            if (node == 0) {
                actor->fn_8001011C(from + len * d);
            } else {
                atn::Matrix3x4 m(node);
                atn::Vector3 d2 = pos + m.GetTrans_unk() - from;
                len = d2.Length_unk();
                if (len > 0.0f) {
                    d2.Normalize_unk();
                    len *= s;
                }
                atn::Vector3 to = from + len * d2;
                offset = to - m.GetTrans_unk();
            }
            if (face == 0 || diff.Length_unk() > 0.0f) {
                atn::Vector3 r = actor->GetRot_unk();
                atn::Vector3 dr = rot - r;
                ScaleStep_unk(dr, t);
                rot = r + dr;
                actor->fn_80010490(rot);
                actor->SetRot_unk(rot);
            }
        } else {
            if (node == 0) {
                actor->SetTarget_unk(pos);
            } else {
                actor->SetTarget_unk(pos + ((atn::Matrix3x4*)node)->GetTrans_unk());
            }
            actor->fn_8001011C(pos);
            if (face == 0 || diff.Length_unk() > 0.0f) {
                actor->fn_80010490(rot);
                actor->SetRot_unk(rot);
            }
        }
        atn::Matrix3x4 mtx = MakeIdentity_unk();
        if (node != 0) {
            if (actor->GetState_unk() == 7 || actor->GetState_unk() == 1) {
                atn::Matrix3x4 nm(node);
                nm.fn_8010F668();
                if (face == 0 && addRot == 0) {
                    rot += GetAxisZ_unk(nm).ToRotation_unk();
                }
                if (keepLocal == 0) {
                    Transpose_unk(nm);
                    offset.fn_80011410(nm);
                    pos = offset;
                }
                atn::Matrix3x4 r = MakeRotYXZ_unk(rot);
                fn_8008CD1C(&mtx, &r);
                mtx.fn_801479E4(pos);
            }
        }
        actor->fn_8001122C(node, 1, 1, mtx);
        moved = fn_80224610(arg->GetParamAt_unk(12));
        moveArg = fn_802245B8(arg->GetParamAt_unk(13));
    } else if (mode == 1) {
        // along the path of the actor
        if (arg->GetParamAt_unk(1)->GetIndex_unk() != -1) {
            float u = Clamp_unk(1.0f - t / (float)fn_80223EB8(arg), 0.0f, 1.0f);
            actor->fn_8001011C(fn_80541BD8(manager, actor->GetPathPos_unk(u)));
            int turn = arg->GetParamAt_unk(2)->GetMode_unk();
            if (turn != 0) {
                float u0 = Clamp_unk(u - 0.001f, 0.0f, 1.0f);
                float u1 = Clamp_unk(0.001f + u, 0.0f, 1.0f);
                atn::Vector3 a = fn_80541BD8(manager, actor->GetPathPos_unk(u0));
                atn::Vector3 b = fn_80541BD8(manager, actor->GetPathPos_unk(u1));
                if (turn == 1) {
                    actor->fn_80011420((b - a).ToRotation_unk().y);
                } else if (turn == 2) {
                    actor->TurnTo_unk(b - a);
                }
            }
        }
        moved = fn_80224610(arg->GetParamAt_unk(3));
        moveArg = fn_802245B8(arg->GetParamAt_unk(4));
    } else if (mode == 2) {
        // to an actor (or one of its nodes) plus an offset, without rotation
        Actor_unk* target = manager->GetActors_unk()->Find_unk(arg->GetParamAt_unk(1)->GetActorId_unk());
        atn::Vector3 pos = target->fn_80010374();
        atn::Vector3 offset = arg->fn_805381F0(3);
        if (target->IsState6_unk()) {
            offset = fn_80541BD8(manager, offset);
        }
        atn::Model* model = fn_8001296C(target);
        if (model != 0) {
            MtxPtr node = model->GetNodeMtxByHash_unk(arg->GetParamAt_unk(2)->GetHash_unk());
            if (node != 0) {
                pos = ((atn::Matrix3x4*)node)->GetTrans_unk();
                atn::Matrix3x4 m(node);
                m.fn_8010F668();
                if (fn_80224610(arg->GetParamAt_unk(6)) != 0) {
                    offset.fn_80011410(m);
                }
            }
        }
        pos += offset;
        if (t > 0.0f) {
            float s = 1.0f - t / (float)fn_80223EB8(arg);
            s = fn_8048BD04(fn_8000DB1C(), actor->GetCurve_unk(), s);
            atn::Vector3 from = actor->GetTarget_unk();
            atn::Vector3 d = pos - from;
            float len = d.Length_unk();
            if (len > 0.0f) {
                d.Normalize_unk();
                len *= s;
            }
            actor->fn_8001011C(from + len * d);
        } else {
            actor->SetTarget_unk(pos);
            actor->fn_8001011C(pos);
        }
        moved = fn_80224610(arg->GetParamAt_unk(8));
        moveArg = fn_802245B8(arg->GetParamAt_unk(9));
    } else if (mode == 3) {
        // rotation only (optionally relative to a node)
        Actor_unk* target = manager->GetActors_unk()->Find_unk(arg->GetParamAt_unk(1)->GetActorId_unk());
        MtxPtr node = arg->fn_805389B8(1);
        int addRot = fn_80224610(arg->GetParamAt_unk(6));
        atn::Vector3 rot = arg->fn_80538C14(3);
        fn_DegToRad_unk(&rot.x);
        if (target->IsState6_unk()) {
            rot.y = fn_80541CF0(manager, rot.y);
        }
        atn::Vector3 cur = actor->GetRot_unk();
        if (addRot != 0 && node == 0) {
            rot += arg->fn_80538C14(7);
        }
        if (t > 0.0f) {
            if (node == 0) {
                atn::Vector3 d = rot - cur;
                ScaleStep_unk(d, t);
                rot = cur + d;
                actor->fn_80010490(rot);
                actor->SetRot_unk(rot);
            }
        } else {
            if (node == 0) {
                actor->fn_80010490(rot);
            }
            actor->SetRot_unk(rot);
        }
        atn::Matrix3x4 mtx = MakeIdentity_unk();
        if (node != 0) {
            atn::Matrix3x4 r = MakeRotYXZ_unk(rot);
            fn_8008CD1C(&mtx, &r);
        }
        actor->fn_8001122C(node, 0, 1, mtx);
    }
    if (moved != 0) {
        actor->fn_800121F0(moveArg, Distance_unk(start, actor->fn_80010374()));
    }
}

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
