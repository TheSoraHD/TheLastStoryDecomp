#include "Event/Behavior.h"
#include "Event/ActionProcessors_unk.h"
#include "Event/Path.h"
#include "Event/Manager.h"
#include "atn/Model_unk.h"
#include "math/VecOps_unk.h"
#include "math/DegToRad_unk.h"
#include "std/ios_init.h"
#include "atn/RenderManager.h"
#include "atn/PBone.h"
#include "game/CharaTask.h"
#include "atn/Color.h"

// Actor actions of the events: processors with vf08 (per frame) and vf0C (phase), as in
// Event/ActionCameraMode. Class names from the RTTI; vtables in the order Material, Wind, Setting,
// Node, Heading, Pose, Eyes, Facial, Motion, Position.

namespace atn {


} // namespace atn

// inferred: model of an actor (0 if none); not the same as Actor::fn_80012A1C
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

inline DataList_unk<Actor>* EventManager_unk::GetActors_unk() {
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

inline bool Actor::IsState6_unk() const {
    return m20 == 6;
}
inline int Actor::GetCurve_unk() const {
    return m130;
}
inline atn::Vector3 Actor::GetTarget_unk() const {
    return mD0;
}
inline atn::Vector3 Actor::GetRot_unk() const {
    return mC4;
}
inline void Actor::SetRot_unk(const atn::Vector3& v) {
    mC4 = v;
}
// NONMATCHING: weak copy 0x80551E3C 60% - the original stores z before the paired single (tried
// Assign_unk, a temporary and a by-value parameter)
inline void Actor::SetTarget_unk(atn::Vector3& v) {
    mD0 = v;
}
inline int Actor::GetState_unk() const {
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
// weak copy 0x8055202C, Actor::GetPathPos_unk)
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

inline atn::Vector3 Actor::GetPathPos_unk(float t) {
    return mPath.GetPos_unk(t);
}

inline void Actor::TurnTo_unk(const atn::Vector3& dir) {
    fn_80010490(dir.ToRotation_unk());
}

// inferred: matrix of the node of `model` with name hash `hash` (weak copy 0x80551D90; a free function
// here: a member in atn/Model_unk.h would renumber the @N labels of its 245 files)
inline MtxPtr GetNodeMtxByHash_unk(atn::Model* model, unsigned int hash) {
    return model->GetNodeMtx_unk(model->fn_800928B0(hash, 0));
}

} // namespace Event


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

// inferred: getters compiled in another file (weak copies in Event/DebugInfo_unk)
extern "C" int fn_80224610(ActionParam_unk* param);   // inferred: flag value of a parameter
extern "C" int fn_802245B8(ActionParam_unk* param);   // inferred: integer value of a parameter
extern "C" int fn_80224618(ActionOwner_unk* section); // inferred: section->m10 (actor id)
extern "C" int fn_80223EB8(Action* arg);              // inferred: arg->m18 (length in frames)
extern "C" Manager* fn_8000DB1C();              // inferred: returns lbl_8087F540
extern "C" void PSMTXTranspose(const Mtx src, Mtx dst);
extern "C" int strcmp(const char* a, const char* b);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path

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
    Actor* actor = manager->GetActors_unk()->Find_unk(fn_80224618(section));
    atn::Vector3 start = actor->fn_80010374();
    int moved = 0;
    int moveArg = 0;
    int mode = arg->GetParamAt_unk(0)->GetMode_unk();
    if (mode == 0) {
        // to an actor (or one of its nodes) plus an offset
        Actor* target = manager->GetActors_unk()->Find_unk(arg->GetParamAt_unk(1)->GetActorId_unk());
        atn::Vector3 offset = arg->fn_805381F0(3);
        int keepLocal = fn_80224610(arg->GetParamAt_unk(6));
        int addRot = fn_80224610(arg->GetParamAt_unk(10));
        if (target->IsState6_unk()) {
            offset = manager->fn_80541BD8(offset);
        }
        atn::Vector3 pos = target->fn_80010374();
        atn::Vector3 cur = actor->fn_80010374();
        MtxPtr node = 0;
        atn::Model* model = target->fn_8001296C();
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
            float s = fn_8000DB1C()->fn_8048BD04(actor->GetCurve_unk(), 1.0f - t / (float)fn_80223EB8(arg));
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
        Actor* target = manager->GetActors_unk()->Find_unk(id);
        atn::Vector3 pos = target->fn_80010374();
        atn::Vector3 offset = arg->fn_805381F0(3);
        if (target->IsState6_unk()) {
            offset = manager->fn_80541BD8(offset);
        }
        atn::Model* model = target->fn_8001296C();
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
            s = fn_8000DB1C()->fn_8048BD04(actor->GetCurve_unk(), s);
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
        Actor* target = manager->GetActors_unk()->Find_unk(id);
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
inline void Actor::SetPath_unk(Path* path) {
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
            Actor* actor = manager->FindActor_unk(section->m10);
            arg->fn_80538CE0(14, actor->fn_80010B68());
            if (arg->GetParam_unk(12)->m04.i != 0) {
                actor->fn_800119C0(0, arg->GetParam_unk(13)->m04.i, 1, 10, manager, 0, 1, 1.0f, 0.0f);
            }
            if (arg->m18 > 0) {
                actor->m130 = lbl_8087F540->fn_8048B3B8(arg->GetParam_unk(17)->m04.i);
            }
            ActionActorPosition::vf08(arg);
        } else if (mode == 1) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor* actor = manager->FindActor_unk(section->m10);
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
            Actor* actor = manager->FindActor_unk(section->m10);
            if (arg->GetParam_unk(8)->m04.i != 0) {
                actor->fn_800119C0(0, arg->GetParam_unk(9)->m04.i, 1, 10, manager, 0, 1, 1.0f, 0.0f);
            }
            if (arg->m18 > 0) {
                actor->m130 = lbl_8087F540->fn_8048B3B8(arg->GetParam_unk(10)->m04.i);
            }
        } else if (mode == 3) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor* actor = manager->FindActor_unk(section->m10);
            arg->fn_80538CE0(7, actor->fn_80010B68());
        }
        break;
    }
    case 3: {
        int mode = arg->GetParam_unk(0)->m04.i;
        if ((mode == 0 || mode == 2) && arg->m18 > 0) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            Actor* actor = manager->FindActor_unk(section->m10);
            lbl_8087F540->fn_8048C884(actor->m130);
        }
        break;
    }
    case 5: {
        if (arg->GetParam_unk(0)->m04.i == 1) {
            int id = arg->GetParam_unk(1)->m04.i;
            if (id != -1) {
                ActionOwner_unk* section = arg->fn_805381A0();
                EventManager_unk* manager = arg->fn_805381C8();
                Actor* actor = manager->FindActor_unk(section->m10);
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
            Actor* target = manager->FindActor_unk(arg->GetParam_unk(1)->m04.i);
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


// inferred: free motion slot of the actor for `arg` (-1 if none)
inline int AddMotionAction_unk(Actor* actor, Action* arg) {
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
        Actor* actor = manager->FindActor_unk(section->m10);
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
        actor->fn_80012540(stop);
        int slot = AddMotionAction_unk(actor, arg);
        if (slot == -1) {
            break;
        }
        if (upper != 0) {
            actor->fn_800119C0(1, a1, b1, motion, manager, slot, c, blend1, speed1);
            actor->fn_800119C0(2, a2, b2, motion, manager, slot, c, blend2, speed2);
        } else {
            actor->fn_800119C0(0, a1, b1, motion, manager, slot, c, blend1, speed1);
            actor->fn_80012294(2, 0, 0);
        }
        break;
    }
    case 3:
    case 4: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
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


int ActionActorFacial::vf0C(Action* arg, int phase) {
    if (phase == 0) {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        Actor* actor = manager->FindActor_unk(id);
        int face = arg->GetParam_unk(0)->m04.i;
        // inferred: parameters 1 and 2 are read but not used (only their loads are left)
        float unused1 = arg->GetParam_unk(1)->m04.f;
        float unused2 = arg->GetParam_unk(2)->m04.i;
        int a = arg->GetParam_unk(4)->m04.i;
        int b = arg->GetParam_unk(5)->m04.i;
        int c = arg->GetParam_unk(6)->m04.i;
        float d = arg->GetParam_unk(7)->m04.f;
        actor->fn_80011E98(face, manager, a, b, c, d);
    }
    return 0;
}


extern "C" int rand();

// NONMATCHING: 95.7% - volatile registers of the two inlined actor lookups and the parameter checks; the
// weight 1.0 in f29 (ours f30)
void ActionActorEyes::fn_80553A74(Action* arg, float now) {
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    Actor* actor = manager->FindActor_unk(section->m10);
    float t = now - (float)arg->m10;
    float weight = 1.0f;
    Actor* target = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
    int node = arg->GetParam_unk(1)->m04.i;
    atn::Vector3 cam = atn::RenderManager::instance_->mCamera.mPos;
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
    Actor* actor = manager->FindActor_unk(section->m10);
    float weight = 1.0f;
    Actor* target = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
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

// inferred: eyes while the wandering window is closed: with `mode` towards the angles of the parameters 6-8,
// otherwise towards the node of the target actor moved by the parameters 2-4; for `left` > 0 the eyes ease
// from the last goal (actor->mF4 / actor->m100) along the curve of the actor (`length`, `v` and `now` unused)
// NONMATCHING: 93.7% - stack frame 0x10 smaller here (slots of the vectors), volatile registers of the two
// actor lookups and the order of the loads of arg->m18 and of the event manager
void ActionActorEyes::fn_80554088(Action* arg, int length, atn::Vector3* v, int mode, float left, float now) {
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    Actor* actor = manager->FindActor_unk(section->m10);
    float weight = 1.0f;
    Actor* target = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
    int node = arg->GetParam_unk(1)->m04.i;
    float t = 1.0f;
    if (left > 0.0f) {
        t -= left / (float)arg->m18;
        t = lbl_8087F540->fn_8048BD04(actor->m138, t);
    }
    if (mode != 0) {
        atn::Vector3 angles;
        angles.x = atn::DegToRad_unk(arg->GetParam_unk(6)->m04.f);
        angles.y = atn::DegToRad_unk(arg->GetParam_unk(7)->m04.f);
        angles.z = atn::DegToRad_unk(arg->GetParam_unk(8)->m04.f);
        atn::Vector3 goal(lbl_807C7030);
        goal += angles;
        if (t == 1.0f) {
            goal = angles;
            actor->mF4 = goal;
        } else {
            atn::Vector3 prev = actor->mF4;
            goal = prev + (goal - prev) * t;
        }
        actor->fn_80012D34(target, node, goal, mode, lbl_807C7030, 0, weight, 0.0f);
        return;
    }
    atn::Vector3 offset(arg->GetParam_unk(2)->m04.f, arg->GetParam_unk(3)->m04.f, arg->GetParam_unk(4)->m04.f);
    atn::Vector3 prev = actor->m100;
    atn::Vector3 pos = target->fn_80010374();
    if (target->fn_8001296C() != 0) {
        MtxPtr mtx = GetNodeMtxByHash_unk(target->fn_8001296C(), node);
        if (mtx != 0) {
            pos = atn::Vector3(mtx[0][3], mtx[1][3], mtx[2][3]);
        }
    }
    pos += offset;
    if (t == 1.0f) {
        atn::Vector3 dir = pos - actor->fn_80010374();
        if (PSVECMag(&dir) > 0.0f) {
            PSVECNormalize(&dir, &dir);
            // inferred: turn between the eyes and the front of the actor (computed and not used)
            atn::Vector3 rot = dir.ToRotationZ_unk();
            rot -= actor->fn_800112E0().ToRotationZ_unk();
        }
        pos = offset;
        actor->m100 = pos;
    } else {
        atn::Vector3 d = pos - prev;
        float dist = PSVECMag(&d);
        if (dist > 0.0f) {
            dist *= t;
            PSVECNormalize(&d, &d);
        }
        prev = prev + d * dist;
        pos = prev;
    }
    actor->fn_80012D34(target, node, pos, mode, lbl_807C7030, 0, weight, 0.0f);
}

// NONMATCHING: 92.4% - saved registers (the original gives r31/r30 to camera/mode and r28 to arg) and the
// volatile registers of the inlined lookups
int ActionActorEyes::vf08(Action* arg) {
    float now = arg->fn_805381C8()->m198;
    float frames = arg->GetParam_unk(12)->m04.f;
    int camera = arg->GetParam_unk(13)->m04.i;
    int mode = arg->GetParam_unk(9)->m04.i;
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    Actor* actor = manager->FindActor_unk(section->m10);
    Actor* target = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
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
        if (actor->m18C.m00 <= 0) {
            Actor* target2 = manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
            float rx = atn::Abs_unk(0.017453292f * arg->GetParam_unk(10)->m04.f);
            float ry = atn::Abs_unk(0.017453292f * arg->GetParam_unk(11)->m04.f);
            actor->m18C.m04.Set_unk(2.0f * rx * ((float)rand() / 32767.0f) - rx, 2.0f * ry * ((float)rand() / 32767.0f) - ry,
                                0.0f);
            actor->m18C.m00 = (int)frames;
        } else {
            fn_80553DC8(arg, &actor->m18C.m10, &actor->m18C.m04, mode, frames);
            actor->m18C.m00--;
        }
    } else {
        float cur = arg->fn_805381C8()->m198;
        float fend = (float)end;
        float min = lbl_8087F540->mB4;
        float left = fend - cur;
        if (left < min) {
            left = 0.0f;
        }
        fn_80554088(arg, length, &actor->m18C.m1C, mode, left, now);
    }
    return 0;
}

// NONMATCHING: 96.5% - registers of the start of phase 0 (the original loads m18 and the size of the
// parameters before the lookup), stack slots of the node positions (as ActionActorHeading::vf0C) and
// operand order of the degree conversions of phase 3
// inferred: eyes of an actor; phase 0 sets the point they look at (at the height of the right eye above the
// reference node, in front of the actor) unless parameter 9 is set, phases 2 and 3 end the eye movement
int ActionActorEyes::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        int length = arg->m18;
        manager->FindActor_unk(arg->GetParam_unk(0)->m04.i);
        int fixed = arg->GetParam_unk(9)->m04.i;
        if (length > 0) {
            int curve = arg->GetParam_unk(15)->m04.i;
            actor->m138 = lbl_8087F540->fn_8048B3B8(curve);
        }
        if (fixed != 0) {
            if (actor->m145 != 0) {
                atn::Vector3 zero(0.0f, 0.0f, 0.0f);
                actor->mF4 = zero;
                actor->m145 = 0;
            }
        } else if (actor->m145 != 0) {
            atn::Vector3 pos = actor->fn_80010374();
            if (actor->fn_8001296C() != 0) {
                MtxPtr eye = actor->fn_8001296C()->GetNodeMtx_unk("eye_R");
                MtxPtr ref = actor->fn_8001296C()->GetNodeMtx_unk("reference");
                if (eye != 0 && ref != 0) {
                    const atn::Vector3& refPos = atn::Vector3(ref[0][3], ref[1][3], ref[2][3]);
                    const atn::Vector3& eyePos = atn::Vector3(eye[0][3], eye[1][3], eye[2][3]);
                    float height = eyePos.y - refPos.y;
                    atn::Vector3 head(refPos.x, refPos.y + height, refPos.z);
                    (head - pos).Length_unk();
                    pos = head;
                }
            }
            pos += actor->fn_800112E0() * 10.0f;
            actor->m100 = pos;
            actor->m145 = 0;
            actor->m18C.m28 = pos;
        }
        actor->m18C.m00 = 0;
        break;
    }
    case 2: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        if (arg->m18 > 0) {
            lbl_8087F540->fn_8048C884(actor->m138);
        }
        if (arg->GetParam_unk(5)->m04.i == 0) {
            float speed = arg->m1C > 0 ? 2.0f / arg->m1C : 1.0f;
            actor->fn_80013530(speed);
        }
        break;
    }
    case 3: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        if (arg->GetParam_unk(5)->m04.i == 0) {
            actor->fn_80013530(1.0f);
            actor->m18C.m1C = lbl_807C7030;
        } else {
            atn::Vector3 rot;
            rot.x = arg->GetParam_unk(6)->m04.f * 0.017453292f;
            rot.y = arg->GetParam_unk(7)->m04.f * 0.017453292f;
            rot.z = arg->GetParam_unk(8)->m04.f * 0.017453292f;
            actor->m18C.m1C = rot;
        }
    }
    case 6: {
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(0)->m04.i;
        Actor* actor = manager->FindActor_unk(id);
        if (actor->m20 == 6) {
            arg->fn_80538DC4(2);
        }
        break;
    }
    }
    return 0;
}

// NONMATCHING: stack slots of the node positions of phase 0 (the original puts the reference position lowest
// and the head point highest; tried the positions as temporaries and the head point first) and the
// registers of the parameter of phase 6
// inferred: heading of an actor; phase 0 sets the point it looks at (in front of its head, or of its
// position when the head is far from the reference node)
int ActionActorHeading::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        if (arg->m18 > 0) {
            int curve = arg->GetParam_unk(9)->m04.i;
            actor->m134 = lbl_8087F540->fn_8048B3B8(curve);
        }
        if (actor->m144 == 0) {
            break;
        }
        atn::Vector3 pos = actor->fn_80010374();
        if (actor->fn_8001296C() != 0) {
            MtxPtr neck = actor->fn_8001296C()->GetNodeMtx_unk("neck");
            MtxPtr ref = actor->fn_8001296C()->GetNodeMtx_unk("reference");
            if (neck != 0 && ref != 0) {
                atn::Vector3 refPos(ref[0][3], ref[1][3], ref[2][3]);
                atn::Vector3 neckPos(neck[0][3], neck[1][3], neck[2][3]);
                float height = neckPos.y - refPos.y;
                atn::Vector3 head(refPos.x, refPos.y + height, refPos.z);
                if ((head - pos).Length_unk() <= height) {
                    pos = head;
                }
            }
        }
        pos += actor->fn_800112E0() * 10.0f;
        actor->mE8 = pos;
        actor->m144 = 0;
        break;
    }
    case 3:
        if (arg->m18 > 0) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            int id = section->m10;
            Actor* actor = manager->FindActor_unk(id);
            lbl_8087F540->fn_8048C884(actor->m134);
        }
        break;
    case 2:
        if (arg->GetParam_unk(8)->m04.i == 0) {
            ActionOwner_unk* section = arg->fn_805381A0();
            EventManager_unk* manager = arg->fn_805381C8();
            int id = section->m10;
            Actor* actor = manager->FindActor_unk(id);
            float speed = arg->m1C > 0 ? 1.0f / arg->m1C : 1.0f;
            actor->fn_80012D0C(speed);
        }
        break;
    case 6: {
        EventManager_unk* manager = arg->fn_805381C8();
        int id = arg->GetParam_unk(0)->m04.i;
        Actor* actor = manager->FindActor_unk(id);
        if (actor->m20 == 6) {
            arg->fn_80538DC4(2);
        }
        break;
    }
    }
    return 0;
}

// NONMATCHING: 98.6% - the original reloads the size of the actor list for each lookup of the phase 3 (ours
// keeps it in a register) and swaps r29/r31
// inferred: node edit of an actor; phase 0 resets the key lists of the edit, phases 2 and 3 (end) reset the
// edit of the node (parameter 0: name hash) unless parameter 8 keeps it
int ActionActorNode::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        if (arg->m18 > 0) {
            actor->m13C = lbl_8087F540->fn_8048B3B8(arg->GetParam_unk(7)->m04.i);
        }
        if (actor->m146 == 0 && arg->m14 - arg->m10 != 0) {
            break;
        }
        actor->m118.clear();
        actor->m124.clear();
        actor->m146 = 0;
        break;
    }
    case 2: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        if (arg->m1C >= 1) {
            actor->m146 = 1;
        }
    }
    case 3: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        manager->FindActor_unk(id);
        if (arg->GetParam_unk(8)->m04.i != 0) {
            break;
        }
        Actor* actor = manager->FindActor_unk(id);
        Actor* target = manager->FindActor_unk(id);
        atn::Model* model = actor->fn_8001296C();
        if (model == 0) {
            return 0;
        }
        atn::Model::Res_unk* res = model->GetRes_unk();
        int node = model->fn_800928B0(arg->GetParam_unk(0)->m04.i, 0);
        model->fn_800948F0(res->m48[node]->m10);
        atn::Vector3 one(1.0f, 1.0f, 1.0f);
        model->fn_80093F1C(res->m48[node]->m10, lbl_807C7030, lbl_807C7030, one, 0);
        target->m146 = 1;
        break;
    }
    }
    return 0;
}

// NONMATCHING: offsets of the strings in @stringBase0 ("eye_R", "reference" and "neck" come first in the
// original: used by the functions of ActionActorEyes/Pose/Heading/Node not written yet)
// inferred: the node `name` is a hair node of the hair kind `kind` (the cases fall through: each kind also
// moves the nodes of the following ones)
int ActionActorSetting::fn_80557E00(int kind, const char* name) {
    switch (kind) {
    case 0:
    case 2:
    case 3:
        if (strcmp(name, "hair_b") == 0 || strcmp(name, "hair_f") == 0) {
            return 1;
        }
    case 1:
        if (strcmp(name, "hair_b") == 0) {
            return 1;
        }
    case 4:
        if (strcmp(name, "hair_f") == 0 || strcmp(name, "hair_b_long1") == 0) {
            return 1;
        }
    case 5:
        if (strcmp(name, "hair_b1") == 0 || strcmp(name, "hair_l") == 0 || strcmp(name, "hair_r") == 0) {
            return 1;
        }
    case 6:
        if (strcmp(name, "hair_b1") == 0 || strcmp(name, "hair_f") == 0) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: offsets of the strings (as fn_80557E00); the original walks the bones through the address
// of the count (chara + 0x434, as a FixedArray_unk of count and array: atn::Model::m384/m388 would be one
// member) and keeps the node name in one more saved register
// inferred: enables the physics bones of the character of `actor` that the settings move (hair, earrings,
// waist cloth and bust), by the kind of the model ("pc101" to "pc107")
void ActionActorSetting::fn_80557F84(Actor* actor, int hair, int earring, int waist, int bust) {
    CharaTask* chara = actor->fn_800128FC();
    if (chara == 0) {
        return;
    }
    std::string name = fn_8006B174(std::string(chara->GetModel_unk()->fn_8008B130()));
    name = std::string(name, 0, 5);
    int kind = -1;
    if (name.compare("pc101") == 0) {
        kind = 0;
    } else if (name.compare("pc102") == 0) {
        kind = 1;
    } else if (name.compare("pc103") == 0) {
        kind = 2;
    } else if (name.compare("pc104") == 0) {
        kind = 3;
    } else if (name.compare("pc105") == 0) {
        kind = 4;
    } else if (name.compare("pc106") == 0) {
        kind = 5;
    } else if (name.compare("pc107") == 0) {
        kind = 6;
    }
    atn::Model* model = chara->GetModel_unk();
    int onHair = 0;
    int onEarring = 0;
    int onWaist = 0;
    int onBust = 0;
    for (int i = 0; i < (int)model->m384; i++) {
        const char* node = model->m388[i]->vf30();
        if (hair != 0) {
            onHair = fn_80557E00(kind, node);
        }
        if (earring != 0) {
            switch (kind) {
            case 0:
                if (strcmp(node, "earring_L") == 0) {
                    onEarring = 1;
                    break;
                }
            case 2:
            case 6:
                if (strcmp(node, "earring_L") == 0 || strcmp(node, "earring_R") == 0) {
                    onEarring = 1;
                    break;
                }
            default:
                onEarring = 0;
                break;
            }
        }
        if (waist != 0) {
            switch (kind) {
            case 0:
            case 1:
            case 3:
                if (strcmp(node, "waistcloth_L2") == 0 || strcmp(node, "waistcloth_R2") == 0) {
                    onWaist = 1;
                    break;
                }
            case 5:
                if (strcmp(node, "waistcloth_L1") == 0 || strcmp(node, "waistcloth_R1") == 0) {
                    onWaist = 1;
                    break;
                }
            default:
                onWaist = 0;
                break;
            }
        }
        if (bust != 0) {
            switch (kind) {
            case 2:
            case 5:
            case 6:
                if (strcmp(node, "bust_L") == 0 || strcmp(node, "bust_R") == 0) {
                    onBust = 1;
                    break;
                }
            default:
                onBust = 0;
                break;
            }
        }
        if (onHair != 0 || onEarring != 0 || onWaist != 0 || onBust != 0) {
            model->m388[i]->vf20(1);
        } else {
            model->m388[i]->vf20(0);
        }
    }
}

// inferred: starts a blink of the character over `length` frames with the fades of the action (inlined in
// ActionActorSetting::vf0C; vf08 has the same code with other arguments of the second blink; name invented)
inline void StartBlink_unk(CharaTask* chara, Action* arg, int length) {
    ActionParam_unk* param = arg->GetParam_unk(4);
    float fadeOut = (float)arg->m18;
    float fadeIn = (float)arg->m1C;
    float total = (float)length;
    float weight = param->m04.f;
    if ((fadeOut >= 1.0f || fadeIn >= 1.0f) && total >= 1.0f) {
        chara->m1188.StartBlink(weight, 1.0f, total, fadeOut, fadeIn);
        chara->m1188.m78 = 1;
    } else if ((atn::Abs_unk(fadeOut) < 1.1920929e-07f && atn::Abs_unk(fadeIn) < 1.1920929e-07f) || total == 0.0f) {
        chara->m1188.StartBlink(weight, 1.0f, total, 3.0f, 4.0f);
        if (total >= 1.0f) {
            chara->m1188.m78 = 1;
        }
    } else {
        chara->m1188.StartBlink(weight, 1.0f, 0.0f, 0.0f, 0.0f);
    }
}

// inferred: the hash and the state of the node of kind 6 are the same locals in phases 0 and 3 (registers)
// inferred: settings of an actor (parameter 0: kind); phase 0 applies them, phases 3 and 5 undo some
int ActionActorSetting::vf0C(Action* arg, int phase) {
    unsigned int hash;
    int show;
    switch (phase) {
    case 0: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        Actor* actor = manager->FindActor_unk(id);
        switch (arg->GetParam_unk(0)->m04.i) {
        case 0:
            actor->fn_8001256C(arg->GetParam_unk(1)->m04.i);
            break;
        case 1:
            actor->fn_800125C8(arg->GetParam_unk(1)->m04.i);
            break;
        case 2:
        {
            int id2 = arg->GetParam_unk(1)->m04.h[1];
            int b = arg->GetParam_unk(1)->m04.h[0];
            actor->fn_8001282C(id2, b);
        }
            break;
        case 3: {
            CharaTask* chara = actor != 0 ? actor->fn_800128FC() : 0;
            if (chara == 0) {
                break;
            }
            int length = arg->m14 - arg->m10;
            if (length <= 0) {
                break;
            }
            StartBlink_unk(chara, arg, length);
            break;
        }
        case 4:
        {
            int msg = arg->GetParam_unk(1)->m04.i;
            int state = arg->GetParam_unk(2)->m04.i;
            actor->fn_8001268C(msg, state);
        }
            break;
        case 5:
            actor->fn_80012638(arg->GetParam_unk(1)->m04.i);
            break;
        case 6: {
            hash = arg->GetParam_unk(1)->m04.i;
            show = arg->GetParam_unk(2)->m04.i;
            atn::Model* model = actor->fn_8001296C();
            if (model != 0) {
                arg->m38 = model->fn_80093D28(hash);
                model->fn_800937E8(hash, show);
            }
            break;
        }
        case 7:
            actor->fn_8001271C(arg->GetParam_unk(1)->m04.i);
            break;
        case 8: {
            CharaTask* chara = actor->fn_800128FC();
            if (chara != 0) {
                if (arg->GetParam_unk(1)->m04.i != 0) {
                    chara->m6A0 |= 0x10;
                } else {
                    chara->m6A0 &= ~0x10;
                }
            }
            break;
        }
        case 9: {
            CharaTask* chara = actor->fn_800128FC();
            if (chara != 0) {
                if (arg->GetParam_unk(1)->m04.i != 0) {
                    chara->mF94 = chara->mF98 - 1;
                } else {
                    chara->mF94 = 0;
                }
                chara->mFlag23 = 1;
            }
            break;
        }
        case 10:
        {
            int hair = arg->GetParam_unk(1)->m04.i;
            int earring = arg->GetParam_unk(2)->m04.i;
            int waist = arg->GetParam_unk(3)->m04.i;
            fn_80557F84(actor, hair, earring, waist, 0);
        }
            break;
        }
        break;
    }
    case 5: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        Actor* actor = manager->FindActor_unk(id);
        if (arg->GetParam_unk(0)->m04.i == 4) {
            actor->fn_8001268C(arg->GetParam_unk(1)->m04.i, 1);
        }
        break;
    }
    case 3: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        Actor* actor = manager->FindActor_unk(id);
        ActionParam_unk* param = arg->GetParam_unk(0);
        if (param->m04.i == 3) {
            CharaTask* chara = actor != 0 ? actor->fn_800128FC() : 0;
            if (chara == 0) {
                break;
            }
            if (arg->m14 - arg->m10 > 0) {
                chara->m1188.m78 = 0;
            }
        } else if (param->m04.i == 6) {
            hash = arg->GetParam_unk(1)->m04.i;
            show = arg->m38;
            atn::Model* model = actor->fn_8001296C();
            if (model == 0) {
                break;
            }
            if (show == -1) {
                model->fn_80093C20(hash);
            } else {
                model->fn_800937E8(hash, show != 0);
            }
        } else if (param->m04.i == 9) {
            CharaTask* chara = actor->fn_800128FC();
            if (chara == 0) {
                break;
            }
            if (arg->GetParam_unk(1)->m04.i != 0) {
                chara->mF94 = 0;
            } else {
                chara->mF94 = chara->mF98 - 1;
                chara->mFlag23 = 0;
            }
        }
        break;
    }
    }
    return 0;
}

// NONMATCHING: float registers (the constant of the int conversions in f6 in the original, the clamps of
// the kind 9); tried the weight read after the conversions, Clamp_unk and the clamps written out
// inferred: per frame part of the settings: kind 3 starts a blink of the character, kind 9 moves its
// counter mF94 towards zero or towards mF98 over the frames left
int ActionActorSetting::vf08(Action* arg) {
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    int id = section->m10;
    Actor* actor = manager->FindActor_unk(id);
    switch (arg->GetParam_unk(0)->m04.i) {
    case 3: {
        CharaTask* chara = actor != 0 ? actor->fn_800128FC() : 0;
        if (chara == 0) {
            break;
        }
        int length = arg->m14 - arg->m10;
        if (length != 0) {
            break;
        }
        ActionParam_unk* param = arg->GetParam_unk(4);
        float fadeOut = (float)arg->m18;
        float fadeIn = (float)arg->m1C;
        float total = (float)length;
        float weight = param->m04.f;
        if ((fadeOut >= 1.0f || fadeIn >= 1.0f) && total >= 1.0f) {
            chara->m1188.StartBlink(weight, 1.0f, total, fadeOut, fadeIn);
            chara->m1188.m78 = 1;
        } else if ((atn::Abs_unk(fadeOut) < 1.1920929e-07f && atn::Abs_unk(fadeIn) < 1.1920929e-07f) ||
                   total == 0.0f) {
            chara->m1188.StartBlink(weight, total, 0.0f, 3.0f, 4.0f);
            if (total >= 1.0f) {
                chara->m1188.m78 = 1;
            }
        } else {
            chara->m1188.StartBlink(weight, 1.0f, 0.0f, 0.0f, 0.0f);
        }
        break;
    }
    case 9: {
        int down = arg->GetParam_unk(1)->m04.i;
        CharaTask* chara = actor != 0 ? actor->fn_800128FC() : 0;
        if (chara == 0) {
            break;
        }
        float max = (float)chara->mF98;
        float cur = (float)chara->mF94;
        cur = max <= cur ? max : cur;
        float lo = 0.0f;
        cur = lo >= cur ? lo : cur;
        float left = (float)arg->m14 - manager->m198;
        float next;
        if (down != 0) {
            next = cur - cur / left;
        } else {
            next = cur + (max - cur) / left;
        }
        float top = max - 1.0f;
        next = top <= next ? top : next;
        float lo2 = 0.0f;
        next = lo2 >= next ? lo2 : next;
        chara->mF94 = (int)next;
        break;
    }
    }
    return 0;
}

int ActionActorWind::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        int id = section->m10;
        atn::Model* model = manager->FindActor_unk(id)->fn_8001296C();
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
// NONMATCHING: 99.4% - saved registers (actor/fade lengths/resource permuted) and the event manager kept in
// a register for the curve (the original reloads it); tried the declaration orders
// inferred: per frame part of a material change: shows the material (parameter 0: name hash, 1: shown)
// and fades its colour from the saved one to the colour of the action (parameters 2-5: alpha, red, green,
// blue) over m18 frames, and back over the last m1C frames (to the colour saved at the start or at the
// load of the actor: parameters 6 and 7)
int ActionActorMaterial::vf08(Action* arg) {
    arg->fn_805381C8();
    ActionOwner_unk* section = arg->fn_805381A0();
    EventManager_unk* manager = arg->fn_805381C8();
    int length;
    int start;
    int fadeIn;
    int fadeOut;
    Actor* actor;
    actor = manager->FindActor_unk(section->m10);
    start = arg->m10;
    length = arg->m14 - start;
    fadeIn = arg->m18;
    fadeOut = arg->m1C;
    atn::Model* model = actor->fn_8001296C();
    atn::Model::Res_unk* res = model->GetRes_unk();
    int index = model->fn_8009290C(arg->GetParam_unk(0)->m04.i);
    if (index >= 0) {
    int shown = arg->GetParam_unk(1)->m04.i;
    const char* const* name = &res->m50[index];
    model->fn_80093D98(*name, shown);
    if (shown != 0) {
    atn::Color target;
    target.a = arg->GetParam_unk(2)->m04.f;
    target.r = arg->GetParam_unk(3)->m04.f;
    target.g = arg->GetParam_unk(4)->m04.f;
    target.b = arg->GetParam_unk(5)->m04.f;
    ColorF_unk base = actor->m170[index];
    float now = arg->fn_805381C8()->m198;
    GXColor diffuseGX = fn_8000DD98(model->fn_80094B6C(*name));
    ColorF_unk diffuse(diffuseGX);
    GXColor ambientGX = model->fn_80094B6C(*name)->m1C;
    ColorF_unk ambient(ambientGX);
    GXColor thirdGX = model->fn_80094B6C(*name)->m20;
    ColorF_unk third(thirdGX);
    float t = 1.0f;
    if (now <= (float)(start + fadeIn) && fadeIn >= 1) {
        int fadeEnd = arg->m18;
        int fadeStart = arg->m10;
        float cur = arg->fn_805381C8()->m198;
        float end = (float)(fadeStart + fadeEnd);
        float min = lbl_8087F540->mB4;
        float left = end - cur;
        if (left < min) {
            left = 0.0f;
        }
        if (left > 0.0f) {
            t -= left / fadeIn;
        }
        float k = lbl_8087F540->fn_8048BD04(actor->m140, t);
        if (k == 1.0f) {
            diffuse.a = target.a;
            diffuse.r = target.r;
            diffuse.g = target.g;
            diffuse.b = target.b;
        } else {
            diffuse.a = base.a + (target.a - base.a) * k;
            diffuse.r = base.r + (target.r - base.r) * k;
            diffuse.g = base.g + (target.g - base.g) * k;
            diffuse.b = base.b + (target.b - base.b) * k;
        }
    }
    if (now >= (float)(length - fadeOut) && fadeOut >= 1) {
        atn::Color dst;
        if (arg->GetParam_unk(6)->m04.i != 0) {
            ColorF_unk saved = actor->m170[index];
            dst.r = saved.r;
            dst.g = saved.g;
            dst.b = saved.b;
            dst.a = saved.a;
        }
        if (arg->GetParam_unk(7)->m04.i != 0) {
            ColorF_unk loaded = actor->m158[index];
            dst.r = loaded.r;
            dst.g = loaded.g;
            dst.b = loaded.b;
            dst.a = loaded.a;
        }
        if (now == (float)length) {
            diffuse.a = dst.a;
            diffuse.r = dst.r;
            diffuse.g = dst.g;
            diffuse.b = dst.b;
        } else {
            diffuse.a += (dst.a - diffuse.a) / fadeOut;
            diffuse.r += (dst.r - diffuse.r) / fadeOut;
            diffuse.g += (dst.g - diffuse.g) / fadeOut;
            diffuse.b += (dst.b - diffuse.b) / fadeOut;
        }
    }
    model->fn_80094958(*name, &diffuse, &third, &ambient);
    }
    }
    return 0;
}
// NONMATCHING: 95.4% - stack slots of the colours (the frame of the original is 0x10 bytes larger) and saved
// registers permuted
// inferred: material change of an actor; phase 0 sets the colour of the action at once (without fades),
// phases 3 and 4 (end) restore the colour saved at the start (parameter 6) or at the load of the actor
// (parameter 7), or save the current one
int ActionActorMaterial::vf0C(Action* arg, int phase) {
    switch (phase) {
    case 0: {
        int length = arg->m14 - arg->m10;
        int fadeIn = arg->m18;
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        atn::Model* model = actor->fn_8001296C();
        atn::Model::Res_unk* res = model->GetRes_unk();
        int index = model->fn_8009290C(arg->GetParam_unk(0)->m04.i);
        if (arg->m18 > 0) {
            int curve = arg->GetParam_unk(8)->m04.i;
            actor->m140 = lbl_8087F540->fn_8048B3B8(curve);
        }
        if (actor->m188 != 0) {
            actor->fn_80013558();
            actor->m188 = 0;
        }
        if (index >= 0) {
            const char* const* name = &res->m50[index];
            Material_unk* material = model->fn_80094B6C(*name);
            if (length == 0 || fadeIn == 0) {
                atn::Color target;
                target.a = arg->GetParam_unk(2)->m04.f;
                target.r = arg->GetParam_unk(3)->m04.f;
                target.g = arg->GetParam_unk(4)->m04.f;
                target.b = arg->GetParam_unk(5)->m04.f;
                GXColor ambientGX = material->m1C;
                ColorF_unk ambient(ambientGX);
                GXColor thirdGX = material->m20;
                ColorF_unk third(thirdGX);
                model->fn_80094958(*name, &target, &third, &ambient);
            }
        }
        break;
    }
    case 3:
    case 4: {
        ActionOwner_unk* section = arg->fn_805381A0();
        EventManager_unk* manager = arg->fn_805381C8();
        Actor* actor = manager->FindActor_unk(section->m10);
        atn::Model* model = actor->fn_8001296C();
        atn::Model::Res_unk* res = model->GetRes_unk();
        int index = model->fn_8009290C(arg->GetParam_unk(0)->m04.i);
        const char* const* name = &res->m50[index];
        Material_unk* material = model->fn_80094B6C(*name);
        if (index < 0) {
            break;
        }
        int done = 0;
        if (arg->GetParam_unk(6)->m04.i != 0) {
            ColorF_unk saved = actor->m170[index];
            GXColor ambientGX = material->m1C;
            ColorF_unk ambient(ambientGX);
            GXColor thirdGX = material->m20;
            ColorF_unk third(thirdGX);
            model->fn_80094958(*name, &saved, &third, &ambient);
            model->fn_80093D98(*name, actor->m17C[index]);
            done = 1;
        }
        if (arg->GetParam_unk(7)->m04.i != 0) {
            ColorF_unk loaded = actor->m158[index];
            GXColor ambientGX = material->m1C;
            ColorF_unk ambient(ambientGX);
            GXColor thirdGX = material->m20;
            ColorF_unk third(thirdGX);
            model->fn_80094958(*name, &loaded, &third, &ambient);
            model->fn_80093D98(*name, actor->m164[index]);
            actor->m170[index] = loaded;
            atn::Color color(loaded.r, loaded.g, loaded.b, loaded.a);
            actor->m17C[index] = color.ToU32();
            done = 1;
        }
        if (done == 0) {
            ColorF_unk diffuse(fn_8000DD98(model->fn_80094B6C(*name)));
            int shown = model->fn_80093E90(fn_800DC6B4(*name));
            actor->m170[index] = diffuse;
            actor->m17C[index] = shown;
        }
        break;
    }
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
