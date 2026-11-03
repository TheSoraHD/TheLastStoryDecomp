#include "Event/Behavior.h"
#include "Event/Path.h"
#include "atn/Model_unk.h"
#include "math/VecOps_unk.h"
#include "math/DegToRad_unk.h"
#include "std/ios_init.h"

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

namespace Event {

// Inline members declared in the headers and defined here (a new inline body in a shared header renumbers
// the @N labels of every file that includes it). All of them are called out of line by fn_80551194.
inline int ActionParam_unk::GetMode_unk() const {
    return m04.i;
}
inline int ActionParam_unk::GetActorId_unk() const {
    return m04.i;
}
inline unsigned int ActionParam_unk::GetHash_unk() const {
    return m04.i;
}
inline int ActionParam_unk::GetIndex_unk() const {
    return m04.i;
}

inline DataList_unk<Actor_unk>* EventManager_unk::GetActors_unk() {
    return &m164;
}

template <class T>
T* DataList_unk<T>::Find_unk(int id) {
    for (int i = 0; i < (int)this->size(); i++) {
        T* p = &this->mData[i];
        if (id == p->m14) {
            return p;
        }
    }
    return 0;
}

inline bool Actor_unk::IsState6_unk() const {
    return m20 == 6;
}
inline int Actor_unk::GetCurve_unk() const {
    return m130;
}
inline atn::Vector3 Actor_unk::GetTarget_unk() const {
    return mD0;
}
inline atn::Vector3 Actor_unk::GetRot_unk() const {
    return mC4;
}
inline void Actor_unk::SetRot_unk(const atn::Vector3& v) {
    mC4 = v;
}
// NONMATCHING: weak copy 0x80551E3C 60% - the original stores z before the paired single (tried
// Assign_unk, a temporary and a by-value parameter)
inline void Actor_unk::SetTarget_unk(const atn::Vector3& v) {
    mD0 = v;
}
inline int Actor_unk::GetState_unk() const {
    return m20;
}

// inferred: point of segment `i` at `u` in [0, 1] (as WarLionPath_unk::Segment_unk)
inline atn::Vector3 PathSpline_unk::Segment_unk(int i, float u) {
    if (i < 0) {
        return mStart;
    }
    if (i >= mNumSegments - 1) {
        return mEnd;
    }
    if (mNumSegments < 2) {
        return lbl_807C7030;
    }
    atn::Vector3 r;
    r.x = mX[i].Eval_unk(u);
    r.y = mY[i].Eval_unk(u);
    r.z = mZ[i].Eval_unk(u);
    return r;
}

// inferred: point at `t` in [0, 1] of the whole path (as WarLionPath_unk::GetPos_unk; inlined in the
// weak copy 0x8055202C, Actor_unk::GetPathPos_unk)
inline atn::Vector3 PathSpline_unk::GetPos_unk(float t) {
    if (t <= 0.0f) {
        return mStart;
    }
    if (t >= 1.0f) {
        return mEnd;
    }
    t = mLength * t;
    for (int i = 0; i < mNumSegments - 1; i++) {
        float len = mLengths[i];
        if (t <= len) {
            return Segment_unk(i, t == 0.0f ? 0.0f : t / len);
        }
        t -= len;
    }
    return mStart;
}

inline atn::Vector3 Actor_unk::GetPathPos_unk(float t) {
    return mPath.GetPos_unk(t);
}

inline void Actor_unk::TurnTo_unk(const atn::Vector3& dir) {
    fn_80010490(dir.ToRotation_unk());
}

// inferred: matrix of the node of `model` with name hash `hash` (weak copy 0x80551D90; a free function
// here: a member in atn/Model_unk.h would renumber the @N labels of its 245 files)
inline MtxPtr GetNodeMtxByHash_unk(atn::Model* model, unsigned int hash) {
    return model->GetNodeMtx_unk(model->fn_800928B0(hash, 0));
}

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
    static void fn_80553A74(Action* arg, float now); // inferred: eyes towards the camera, with jitter
    // inferred: eyes towards a point (`cur` moves towards `goal` in `frames`; angles from the parameters
    // when `mode` is not 0)
    static void fn_80553DC8(Action* arg, atn::Vector3* cur, const atn::Vector3* goal, int mode, float frames);
    // inferred: eyes outside the wandering window (`left` frames left, at frame `now`)
    static void fn_80554088(Action* arg, int length, atn::Vector3* v, int mode, float left, float now);
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
extern "C" void PSMTXTranspose(const Mtx src, Mtx dst);

// inferred: step of `cur` towards `goal` in `frames` frames (name invented)
inline atn::Vector3 Approach_unk(const atn::Vector3& cur, const atn::Vector3& goal, float frames) {
    return (goal - cur) * (1.0f / frames) + cur;
}

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
            offset = manager->fn_80541BD8(offset);
        }
        atn::Vector3 pos = target->fn_80010374();
        atn::Vector3 cur = actor->fn_80010374();
        MtxPtr node = 0;
        atn::Model* model = fn_8001296C(target);
        if (model != 0) {
            node = GetNodeMtxByHash_unk(model, arg->GetParamAt_unk(2)->GetHash_unk());
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
                rot.y = manager->fn_80541CF0(rot.y);
            }
            if (addRot != 0 && node == 0) {
                rot += arg->fn_80538C14(14);
            }
        }
        if (t > 0.0f) {
            float s = fn_8048BD04(fn_8000DB1C(), actor->GetCurve_unk(), 1.0f - t / (float)fn_80223EB8(arg));
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
                atn::Vector3 world = pos + ((atn::Matrix3x4*)node)->GetTrans_unk();
                actor->SetTarget_unk(world);
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
                // the address of the temporary (warning "not an lvalue", as in game/Em403WarLion.cpp) gives
                // the stack slots of the original; a named local or a reference do not
                mtx = MakeRotYXZ_unk(rot);
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
            atn::Vector3 world = manager->fn_80541BD8(actor->GetPathPos_unk(u));
            actor->fn_8001011C(world);
            int turn = arg->GetParamAt_unk(2)->GetMode_unk();
            if (turn != 0) {
                float u0 = Clamp_unk(u - 0.001f, 0.0f, 1.0f);
                float u1 = Clamp_unk(0.001f + u, 0.0f, 1.0f);
                atn::Vector3 a = manager->fn_80541BD8(actor->GetPathPos_unk(u0));
                atn::Vector3 b = manager->fn_80541BD8(actor->GetPathPos_unk(u1));
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
        int id = arg->GetParamAt_unk(1)->GetActorId_unk();
        Actor_unk* target = manager->GetActors_unk()->Find_unk(id);
        atn::Vector3 pos = target->fn_80010374();
        atn::Vector3 offset = arg->fn_805381F0(3);
        if (target->IsState6_unk()) {
            offset = manager->fn_80541BD8(offset);
        }
        atn::Model* model = fn_8001296C(target);
        if (model != 0) {
            MtxPtr node = GetNodeMtxByHash_unk(model, arg->GetParamAt_unk(2)->GetHash_unk());
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
        int id = arg->GetParamAt_unk(1)->GetActorId_unk();
        Actor_unk* target = manager->GetActors_unk()->Find_unk(id);
        MtxPtr node = arg->fn_805389B8(1);
        int addRot = fn_80224610(arg->GetParamAt_unk(6));
        atn::Vector3 rot = arg->fn_80538C14(3);
        fn_DegToRad_unk(&rot.x);
        if (target->IsState6_unk()) {
            rot.y = manager->fn_80541CF0(rot.y);
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
            mtx = MakeRotYXZ_unk(rot);
        }
        actor->fn_8001122C(node, 0, 1, mtx);
    }
    if (moved != 0) {
        actor->fn_800121F0(moveArg, Distance_unk(start, actor->fn_80010374()));
    }
}

// inferred: helpers of the spline compiled in another file (weak copies, C names as in game/Em403WarLion.cpp)
// inferred: raw storage of a std::vector<float> (constructed and destroyed through the weak copies)
struct PathFloats_unk {
    float* mData;
    unsigned int mSize;
    unsigned int mCap;
};
extern "C" void fn_8032B314(std::vector<PathCubic_unk>* v, unsigned int n);          // inferred: resize
extern "C" PathFloats_unk* fn_8032B6F8(PathFloats_unk* v, unsigned int n);           // inferred: vector(n)
extern "C" void fn_8000D844(PathFloats_unk* v, int flag);                            // inferred: ~vector<float>
extern "C" void fn_8032B7F0(PathSpline_unk* spline, const float* y, PathCubic_unk* out); // inferred: solve
extern "C" void fn_8032BE88(std::vector<float>* v, unsigned int n);                  // inferred: resize
extern "C" float fn_8032C1EC(PathSpline_unk* spline, int i);                         // inferred: segment length
extern "C" int fn_8048B3B8(EventStep_unk* step, int id);    // inferred: move curve by id
extern "C" void fn_8048C884(EventStep_unk* step, int curve); // inferred: releases a move curve

inline Path* EventManager_unk::FindPath_unk(int id) {
    for (int i = 0; i < (int)m154.size(); i++) {
        Path* path = &m154[(unsigned int)i];
        if (id == path->m14) {
            return path;
        }
    }
    return 0;
}

// inferred: builds the spline through the points (same code as WarLionBuild_unk of game/Em403WarLion.cpp)
inline void PathSpline_unk::Build_unk(const atn::Vector3* pts, int n) {
    mNumSegments = n;
    fn_8032B314(&mX, n - 1);
    fn_8032B314(&mY, n - 1);
    fn_8032B314(&mZ, n - 1);
    PathFloats_unk zs;
    PathFloats_unk ys;
    PathFloats_unk xs;
    fn_8032B6F8(&xs, n);
    fn_8032B6F8(&ys, n);
    fn_8032B6F8(&zs, n);
    for (int i = 0; i < n; i++) {
        xs.mData[i] = pts[i].x;
        ys.mData[i] = pts[i].y;
        zs.mData[i] = pts[i].z;
    }
    fn_8032B7F0(this, xs.mData, mX.begin());
    fn_8032B7F0(this, ys.mData, mY.begin());
    fn_8032B7F0(this, zs.mData, mZ.begin());
    mStart = pts[0];
    mEnd = pts[n - 1];
    mLength = 0.0f;
    fn_8032BE88(&mLengths, n - 1);
    for (int i = 0; i < n - 1; i++) {
        mLengths[i] = fn_8032C1EC(this, i);
        mLength += mLengths[i];
    }
    fn_8000D844(&zs, -1);
    fn_8000D844(&ys, -1);
    fn_8000D844(&xs, -1);
}

// inferred: the path of the actor follows the points of an event path
inline void Actor_unk::SetPath_unk(Path* path) {
    std::vector<atn::Vector3> pts(path->mPoints.size());
    for (int i = 0; i < (int)path->mPoints.size(); i++) {
        pts[i] = path->mPoints[i].mPos;
    }
    mPath.Build_unk(pts.begin(), pts.size());
}

// NONMATCHING: 98.67% - saved registers: the original keeps arg in r26 and the count of the points in r31
// (ours r31/r30); the register of the frame step in the inlined vf08 (tried ~14 forms: shared locals, switch
// on the mode, members/free inlines for the path, locals for the points)
int ActionActorPosition::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        int mode = arg->GetParam_unk(0)->m04.i;
        if (mode == 0) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor_unk* actor = manager->FindActor_unk(section->m10);
            arg->fn_80538CE0(14, actor->fn_80010B68());
            if (arg->GetParam_unk(12)->m04.i != 0) {
                actor->fn_800119C0(0, arg->GetParam_unk(13)->m04.i, 1, 10, manager, 0, 1, 1.0f, 0.0f);
            }
            if (arg->m18 > 0) {
                actor->m130 = fn_8048B3B8(lbl_8087F540, arg->GetParam_unk(17)->m04.i);
            }
            ActionActorPosition::vf08(arg);
        } else if (mode == 1) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor_unk* actor = manager->FindActor_unk(section->m10);
            int id = arg->GetParam_unk(1)->m04.i;
            if (id != -1) {
                Path* path = manager->FindPath_unk(id);
                if (path != 0) {
                    actor->SetPath_unk(path);
                }
            }
            if (arg->GetParam_unk(3)->m04.i != 0) {
                actor->fn_800119C0(0, arg->GetParam_unk(4)->m04.i, 1, 10, manager, 0, 1, 1.0f, 0.0f);
            }
        } else if (mode == 2) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor_unk* actor = manager->FindActor_unk(section->m10);
            if (arg->GetParam_unk(8)->m04.i != 0) {
                actor->fn_800119C0(0, arg->GetParam_unk(9)->m04.i, 1, 10, manager, 0, 1, 1.0f, 0.0f);
            }
            if (arg->m18 > 0) {
                actor->m130 = fn_8048B3B8(lbl_8087F540, arg->GetParam_unk(10)->m04.i);
            }
        } else if (mode == 3) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor_unk* actor = manager->FindActor_unk(section->m10);
            arg->fn_80538CE0(7, actor->fn_80010B68());
        }
        break;
    }
    case 3: {
        int mode = arg->GetParam_unk(0)->m04.i;
        if ((mode == 0 || mode == 2) && arg->m18 > 0) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor_unk* actor = manager->FindActor_unk(section->m10);
            fn_8048C884(lbl_8087F540, actor->m130);
        }
        break;
    }
    case 5: {
        if (arg->GetParam_unk(0)->m04.i == 1) {
            int id = arg->GetParam_unk(1)->m04.i;
            if (id != -1) {
                ActionOwner_unk* section = arg->fn_805381A0();
                EventManager_unk* manager = arg->fn_805381C8();
                Actor_unk* actor = manager->FindActor_unk(section->m10);
                Path* path = manager->FindPath_unk(id);
                if (path != 0) {
                    actor->SetPath_unk(path);
                }
            }
        }
        fn_80551194(arg, 0.0f);
        break;
    }
    case 6: {
        if (arg->GetParam_unk(0)->m04.i == 0) {
            EventManager_unk* manager = arg->fn_805381C8();
            Actor_unk* target = manager->FindActor_unk(arg->GetParam_unk(1)->m04.i);
            if (target->m20 == 6) {
                arg->fn_80538DC4(3);
                float yaw = manager->fn_80541CF0(atn::DegToRad_unk(arg->GetParam_unk(8)->m04.f));
                float deg = 57.29578f * yaw;
                ActionParam_unk* p = arg->GetParam_unk(8);
                p->m00 = 3;
                p->m04.f = deg;
            }
        }
        break;
    }
    }
    return 0;
}

int ActionActorPosition::vf08(Action* arg) {
    int length = arg->m18;
    int start = arg->m10;
    EventManager_unk* manager = arg->fn_805381C8();
    float now = manager->m198;
    float end = (float)(start + length);
    float min = lbl_8087F540->mB4;
    float left = end - now;
    if (left < min) {
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

// inferred: the global camera (lbl_8087EFB4) seen through the point the eyes look at
struct EyesCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 m10C; // inferred: eye position
};

extern "C" EyesCamera_unk* lbl_8087EFB4;
extern "C" int rand();

// NONMATCHING: 95.7% - volatile registers of the two inlined actor lookups and the parameter checks; the
// weight 1.0 in f29 (ours f30)
void ActionActorEyes::fn_80553A74(Action* arg, float now) {
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    Actor_unk* actor = manager->FindActor_unk(section->m10);
    float t = now - (float)arg->m10;
    float weight = 1.0f;
    Actor_unk* target = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
    int node = arg->GetParam_unk(1)->m04.i;
    atn::Vector3 cam = lbl_8087EFB4->m10C;
    float angle = arg->GetParam_unk(14)->m04.f;
    int fadeIn = arg->m18;
    int length = arg->m14 - arg->m10;
    int fadeOut = arg->m1C;
    if ((float)fadeIn > t && fadeIn >= 1) {
        angle *= t / (float)fadeIn;
    } else if ((float)(length - fadeOut) < t && fadeOut >= 1) {
        angle *= ((float)length - t) / (float)fadeOut;
    }
    float rad = 0.017453292f * angle;
    float rx = atn::Abs_unk(0.017453292f * arg->GetParam_unk(10)->m04.f);
    float ry = atn::Abs_unk(0.017453292f * arg->GetParam_unk(11)->m04.f);
    atn::Vector3 jitter(lbl_807C7030);
    jitter.x = 2.0f * rx * ((float)rand() / 32767.0f) - rx;
    jitter.y = 2.0f * ry * ((float)rand() / 32767.0f) - ry;
    actor->fn_80012D34(target, node, cam, 0, jitter, 1, weight, rad);
}

// NONMATCHING: 99.1% - volatile registers of the second inlined actor lookup: the original loads the count
// before the id (same in fn_80553A74; a const int& parameter in FindActor_unk is worse)
void ActionActorEyes::fn_80553DC8(Action* arg, atn::Vector3* cur, const atn::Vector3* goal, int mode, float frames) {
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    Actor_unk* actor = manager->FindActor_unk(section->m10);
    float weight = 1.0f;
    Actor_unk* target = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
    int node = arg->GetParam_unk(1)->m04.i;
    *cur = Approach_unk(*cur, *goal, frames);
    if (mode != 0) {
        atn::Vector3 angles;
        angles.x = atn::DegToRad_unk(arg->GetParam_unk(6)->m04.f);
        angles.y = atn::DegToRad_unk(arg->GetParam_unk(7)->m04.f);
        angles.z = atn::DegToRad_unk(arg->GetParam_unk(8)->m04.f);
        actor->fn_80012D34(target, node, angles, mode, *cur, 0, weight, 0.0f);
    } else {
        atn::Vector3 pos = arg->fn_805381F0(2);
        if (target->m20 == 6) {
            pos = manager->fn_80541BD8(pos);
        }
        actor->fn_80012D34(target, node, pos, mode, *cur, 0, weight, 0.0f);
    }
}

// NONMATCHING: 92.4% - saved registers (the original gives r31/r30 to camera/mode and r28 to arg) and the
// volatile registers of the inlined lookups; fn_80554088 (between fn_80553DC8 and this one) is not written yet
int ActionActorEyes::vf08(Action* arg) {
    float now = arg->fn_805381C8()->m198;
    float frames = arg->GetParam_unk(12)->m04.f;
    int camera = arg->GetParam_unk(13)->m04.i;
    int mode = arg->GetParam_unk(9)->m04.i;
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    Actor_unk* actor = manager->FindActor_unk(section->m10);
    Actor_unk* target = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
    int length = arg->m14 - arg->m10;
    if (arg->m1C > 0 && (float)arg->m14 - manager->m198 < (float)arg->m1C) {
        return 0;
    }
    if (camera != 0) {
        fn_80553A74(arg, now);
        return 0;
    }
    bool wander = true;
    if (frames <= 0.0f) {
        wander = false;
    }
    int end = arg->m10 + arg->m18;
    if ((float)end > now || (float)(arg->m14 - arg->m1C) < now) {
        wander = false;
    }
    if (wander) {
        if (actor->m18C <= 0) {
            Actor_unk* target2 = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
            float rx = atn::Abs_unk(0.017453292f * arg->GetParam_unk(10)->m04.f);
            float ry = atn::Abs_unk(0.017453292f * arg->GetParam_unk(11)->m04.f);
            actor->m190.Set_unk(2.0f * rx * ((float)rand() / 32767.0f) - rx, 2.0f * ry * ((float)rand() / 32767.0f) - ry,
                                0.0f);
            actor->m18C = (int)frames;
        } else {
            fn_80553DC8(arg, &actor->m19C, &actor->m190, mode, frames);
            actor->m18C--;
        }
    } else {
        float cur = arg->fn_805381C8()->m198;
        float fend = (float)end;
        float min = lbl_8087F540->mB4;
        float left = fend - cur;
        if (left < min) {
            left = 0.0f;
        }
        fn_80554088(arg, length, &actor->m1A8, mode, left, now);
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
            model->fn_80097750(dir, a, b, arg->m14 - arg->m10);
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
