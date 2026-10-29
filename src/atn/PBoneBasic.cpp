#include "atn/Csv.h"
#include "atn/Model_unk.h"
#include "math/Vec.h"
#include "std/string.h"
#include <boost/function.hpp>

// inferred: physics bones (a bone that swings between two nodes of a model, read from a CSV).
// Member names are invented.

extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" atn::Vector3 lbl_807C7030;                // inferred: zero vector
extern "C" int fn_800DC12C(const char* s);           // inferred: string to int
extern "C" float fn_800DC288(const char* s);         // inferred: string to float

// inferred: debug menu entries and debug drawing
extern "C" void* fn_8008937C(void* parent, const char* name);
extern "C" void fn_80087994(void* group, const char* name, int* flag, int arg3, int arg4);
extern "C" void fn_80087858(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: angle
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063D3C(void* debug, const atn::Vector3& pos, int color, float size, float arg);
extern "C" void fn_800638B0(void* debug, const atn::Vector3& from, const atn::Vector3& to, unsigned int color,
                            float arg4, float arg5);

// inferred: model data of the loaded file (same as the local copy in atn/LH_Model.cpp)
struct ModelJoint_unk {
    unsigned char m00[0x1C];
    int m1C; // inferred: parent node
};
struct ModelData_unk {
    unsigned char m00[0x48];
    ModelJoint_unk** mJoints; // 0x48
};

// inferred: pose of a model, built on the stack to compute the joint matrices (same as the local
// copy in atn/LH_Model.cpp)
struct ModelPose_unk {
    ModelPose_unk(ModelData_unk* model, atn::Matrix3x4* matrices, int a, boost::function<void()> callback, int b,
                  int c);

    unsigned char m00[0x18];
    boost::function<void()> m18;
    boost::function<void()> m2C;
    unsigned char m40[0x58 - 0x40];
};
// inferred: updates the matrices of the joints under `joint`
extern "C" void fn_8009ADA4(ModelPose_unk* pose, int joint, void* arg2, atn::Matrix3x4* parent, int arg4);

// inferred: model of the bone (node matrices at m3C)
struct PBoneModel_unk {
    unsigned char m000[0x3C];
    atn::Matrix3x4* mNodeMtx; // 0x3C
    unsigned char m040[0x16C - 0x40];
    ModelData_unk* m16C;
    unsigned char m170[0x220 - 0x170];
    void* m220;

    atn::Matrix3x4* GetNodeMtx_unk(int index) { return index < 0 ? 0 : &mNodeMtx[index]; }
};

// inferred: rotation (x, y, z) that turns the z axis to dir
void fn_800A73A4(atn::Vector3* rot, const atn::Vector3* dir);

namespace atn {

// inferred: interface of the physics bones
class PBoneInterface {
public:
    virtual ~PBoneInterface() {}
    virtual void vf0C() = 0;
    virtual void vf10() = 0;
    virtual void vf14() = 0;
    virtual void vf18() = 0;
    virtual int vf1C(PBoneModel_unk* model, CsvBase<char, 9, 13, 34>* csv) = 0;
    virtual void vf20(int enable) = 0;
    virtual int vf24() = 0;
    virtual void vf28(const Vector3& force) = 0;
    virtual void vf2C(void* menu) = 0;
    virtual const char* vf30() = 0;
    virtual void vf34() = 0;
};

class PBoneBasic : public PBoneInterface {
public:
    PBoneBasic();
    virtual ~PBoneBasic();
    virtual void vf0C(); // inferred: reset
    virtual void vf10(); // inferred: update
    virtual void vf14(); // inferred: rest pose
    virtual void vf18(); // inferred: debug draw
    virtual int vf1C(PBoneModel_unk* model, CsvBase<char, 9, 13, 34>* csv); // inferred: load
    virtual void vf20(int enable) { m04 = enable; }
    virtual int vf24() { return m04; }
    virtual void vf28(const Vector3& force);
    virtual void vf2C(void* menu); // inferred: debug menu
    virtual const char* vf30() { return mName.data(); }
    virtual void vf34(); // inferred: physics step

    int m04;          // inferred: enabled
    Vector3 m08;      // inferred: tip position
    Vector3 m14;      // inferred: direction
    Vector3 m20;      // inferred: velocity
    Vector3 m2C;      // inferred: previous root position / external force
    int m38;          // inferred: root node
    int m3C;          // inferred: tip node
    int m40;
    int m44;
    float m48;        // inferred: Y limit min
    float m4C;        // inferred: Z limit min
    float m50;
    float m54;        // inferred: Y limit max
    float m58;        // inferred: Z limit max
    float m5C;        // inferred: length
    float m60;        // inferred: wait
    PBoneModel_unk* m64;
    int m68;          // inferred: debug display
    std::string mName; // 0x6C
};

// inferred: bone pushed by an external force
class PBoneOutForce : public PBoneBasic {
public:
    PBoneOutForce();
    virtual ~PBoneOutForce();
    // NONMATCHING: 75.0% - the original stores z before x and y (psq_l, lfs, stfs, psq_st); the
    // Vector3 copy of math/Vec.h stores x and y first
    virtual void vf28(const Vector3& force) {
        if (m04 != 0) {
            m2C = force;
        }
    }
    virtual void vf34();
};

} // namespace atn

// inferred: atan2 on floats (name invented)
inline float Atan2_unk(float y, float x) {
    return atan2(y, x);
}

// inferred: angles (x = 0, y, z) of dir; also computes its ToRotation_unk, which is not used
// NONMATCHING: 93.4% - the epsilon is loaded before the prologue stores in the original, and
// the Vector3 copy of math/Vec.h gets the source and destination address registers swapped (copy of dir)
void fn_800A73A4(atn::Vector3* rot, const atn::Vector3* dir) {
    if (atn::Abs_unk(dir->x) < 1.1920929e-7f) {
        rot->y = dir->z > 0.0f ? 1.5707964f : -1.5707964f;
        rot->z = 0.0f;
    } else if (dir->x < 0.0f) {
        rot->y = -(float)atan2(dir->z, -dir->x);
        atn::Matrix3x4 m;
        m.fn_8013A13C(rot->y);
        atn::Vector3 v(*dir);
        PSMTXMultVec(m.m, &v, &v);
        atn::Vector3 r = dir->ToRotation_unk();
        rot->z = Atan2_unk(v.y, v.x);
    } else {
        rot->y = -(float)atan2(dir->z, dir->x);
        atn::Matrix3x4 m;
        m.fn_8013A13C(-rot->y);
        atn::Vector3 v(*dir);
        PSMTXMultVec(m.m, &v, &v);
        atn::Vector3 r = dir->ToRotation_unk();
        rot->z = Atan2_unk(v.y, v.x);
    }
    rot->x = 0.0f;
}

// inferred: cross product by value (name invented)
inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: sets the translation of m (name invented)
inline void SetTrans_unk(atn::Matrix3x4* m, const atn::Vector3& v) {
    m->m[0][3] = v.x;
    m->m[1][3] = v.y;
    m->m[2][3] = v.z;
}

// inferred: *m = *m * rot_axis(rad), without the zero test of __MtxRotate (name invented)
inline void RotateAlways_unk(atn::Matrix3x4& m, char axis, float rad) {
    Mtx r;
    PSMTXRotRad(r, axis, rad);
    Mtx t;
    PSMTXConcat(m.m, r, t);
    __MtxCopy(m.m, t);
}

namespace atn {

PBoneBasic::PBoneBasic()
    : m04(1), m08(0.0f, 0.0f, 0.0f), m14(0.0f, 0.0f, 0.0f), m20(0.0f, 0.0f, 0.0f), m2C(0.0f, 0.0f, 0.0f), m64(0),
      m68(0) {}

PBoneBasic::~PBoneBasic() {}

// NONMATCHING: 99.6% - the Vector3 copy of math/Vec.h gets the source and destination address registers swapped
// (copies of the node positions)
void PBoneBasic::vf0C() {
    Vector3 root;
    root = m64->GetNodeMtx_unk(m38)->GetTrans_unk();
    Vector3 tip;
    tip = m64->GetNodeMtx_unk(m3C)->GetTrans_unk();
    m5C = (tip - root).Length_unk();
    m14 = (tip - root).GetNormalize();
    vf14();
}

// NONMATCHING: 96.7% - the original has three more Vector3 temporaries on the stack (0x8, 0x14,
// 0x38; only one component of each is stored), so the frame is 0x20 bytes bigger
void PBoneBasic::vf10() {
    if (m04 == 0) {
        return;
    }
    Matrix3x4* root = m64->GetNodeMtx_unk(m38);
    Matrix3x4* tip = m64->GetNodeMtx_unk(m3C);
    if (root == 0 || tip == 0) {
        return;
    }
    if (m14.IsZero_unk()) {
        vf0C();
    }
    Vector3 rootPos = root->GetTrans_unk();
    vf34();
    m14 += m20;
    Matrix3x4 rot(*root);
    rot.m[0][3] = 0.0f;
    rot.m[1][3] = 0.0f;
    rot.m[2][3] = 0.0f;
    Matrix3x4 inv(rot);
    PSMTXInverse(inv.m, inv.m);
    Vector3 angles(0.0f, 0.0f, 0.0f);
    Vector3 dir(m14);
    PSVECNormalize(&dir, &dir);
    PSMTXMultVec(inv.m, &dir, &dir);
    fn_800A73A4(&angles, &dir);
    if (angles.y < m48) {
        angles.y = m48;
    }
    if (angles.y > m54) {
        angles.y = m54;
    }
    if (angles.z < m4C) {
        angles.z = m4C;
    }
    if (angles.z > m58) {
        angles.z = m58;
    }
    m14 = Vector3(1.0f, 0.0f, 0.0f);
    Matrix3x4 m;
    m.SetIdentity();
    __MtxRotate(m.m, 'z', angles.z);
    __MtxRotate(m.m, 'y', angles.y);
    __MtxRotate(m.m, 'x', angles.x);
    PSMTXMultVec(m.m, &m14, &m14);
    PSMTXMultVec(rot.m, &m14, &m14);
    m14 *= m5C;
    Matrix3x4 local;
    local.SetIdentity();
    RotateAlways_unk(local, 'y', angles.y);
    RotateAlways_unk(local, 'z', angles.z);
    Matrix3x4 world;
    PSMTXConcat(root->m, local.m, world.m);
    *root = world;
    SetTrans_unk(root, rootPos);
    SetTrans_unk(m64->GetNodeMtx_unk(m3C), rootPos + m14);
    ModelJoint_unk* joint = m64->m16C->mJoints[m3C];
    ModelPose_unk pose(m64->m16C, m64->mNodeMtx, 0, boost::function<void()>(), 0, 0);
    fn_8009ADA4(&pose, m3C, m64->m220, m64->GetNodeMtx_unk(joint->m1C), 0);
    m08 = rootPos;
    m2C = lbl_807C7030;
}

void PBoneBasic::vf34() {
    Matrix3x4* root = m64->GetNodeMtx_unk(m38);
    Matrix3x4* tip = m64->GetNodeMtx_unk(m3C);
    Vector3 rootPos = root->GetTrans_unk();
    m2C = m08 - rootPos;
    m20 += m2C * 0.5f;
    float gravity = 0.0333f * (9.8f * (0.1f * m60));
    Vector3 v = m14 + m20;
    PSVECNormalize(&v, &v);
    Vector3 c;
    PSVECCrossProduct(&v, &Vector3(0.0f, -1.0f, 0.0f), &c);
    if (fn_805F9920(&c) > 0.0001f) {
        Vector3 t;
        PSVECCrossProduct(&c, &v, &t);
        Vector3 n;
        n = t;
        PSVECNormalize(&n, &n);
        float dot = PSVECDotProduct(&Vector3(0.0f, -1.0f, 0.0f), &n);
        Vector3 force = n * (gravity * dot);
        m20 += force;
    }
    m20 *= 0.9f;
}

// NONMATCHING: 99.6% - the Vector3 copy of math/Vec.h gets the source and destination address registers swapped
// (copies of the node positions)
void PBoneBasic::vf14() {
    Vector3 root;
    root = m64->GetNodeMtx_unk(m38)->GetTrans_unk();
    Vector3 tip;
    tip = m64->GetNodeMtx_unk(m3C)->GetTrans_unk();
    m08 = root;
    m14 = (tip - root).GetNormalize();
    m20 = lbl_807C7030;
}

int PBoneBasic::vf1C(PBoneModel_unk* model, CsvBase<char, 9, 13, 34>* csv) {
    m64 = model;
    mName = csv->fn_8005B9CC();
    m38 = ((atn::Model*)m64)->GetNodeNoByName(mName.data(), 0);
    m3C = ((atn::Model*)m64)->GetNodeNoByName(csv->fn_8005B9CC(), 0);
    m40 = fn_800DC12C(csv->fn_8005B9CC());
    m48 = 0.017453292f * fn_800DC288(csv->fn_8005B9CC());
    m54 = 0.017453292f * fn_800DC288(csv->fn_8005B9CC());
    m4C = 0.017453292f * fn_800DC288(csv->fn_8005B9CC());
    m58 = 0.017453292f * fn_800DC288(csv->fn_8005B9CC());
    m60 = fn_800DC288(csv->fn_8005B9CC());
    m5C = 1.0f;
    return 1;
}

void PBoneBasic::vf28(const Vector3& force) {}

void PBoneBasic::vf18() {
    if (m68 == 0) {
        return;
    }
    Matrix3x4* root = m64->GetNodeMtx_unk(m38);
    Matrix3x4* tip = m64->GetNodeMtx_unk(m3C);
    Matrix3x4 rot(*root);
    rot.m[0][3] = 0.0f;
    rot.m[1][3] = 0.0f;
    rot.m[2][3] = 0.0f;
    fn_80063D3C(lbl_8087EEB0, root->GetTrans_unk(), -1, 0.1f, 0.0f);
    fn_80063D3C(lbl_8087EEB0, tip->GetTrans_unk(), -1, 0.1f, 0.0f);
    Vector3 axis(1.0f, 0.0f, 0.0f);
    PSMTXMultVec(rot.m, &axis, &axis);
    fn_800638B0(lbl_8087EEB0, root->GetTrans_unk(), root->GetTrans_unk() + axis * 3.0f, 0xFFFF0000, 0.0f, 1.0f);
    axis.Set_unk(0.0f, 1.0f, 0.0f);
    PSMTXMultVec(rot.m, &axis, &axis);
    fn_800638B0(lbl_8087EEB0, root->GetTrans_unk(), root->GetTrans_unk() + axis * 3.0f, 0xFF00FF00, 0.0f, 1.0f);
    axis.Set_unk(0.0f, 0.0f, 1.0f);
    PSMTXMultVec(rot.m, &axis, &axis);
    fn_800638B0(lbl_8087EEB0, root->GetTrans_unk(), root->GetTrans_unk() + axis * 3.0f, 0xFF0000FF, 0.0f, 1.0f);
}

void PBoneBasic::vf2C(void* menu) {
    void* group = fn_8008937C(menu, mName.data());
    fn_80087994(group, "Enable", &m04, 0, 0);
    fn_80087994(group, "DebugDisp", &m68, 0, 0);
    void* y = fn_8008937C(group, "Y LIMIT");
    fn_80087858(y, "MIN", &m48, 0, 0, -180.0f, 180.0f, 2.0f);
    fn_80087858(y, "MAX", &m54, 0, 0, -180.0f, 180.0f, 2.0f);
    void* z = fn_8008937C(group, "Z LIMIT");
    fn_80087858(z, "MIN", &m4C, 0, 0, -180.0f, 180.0f, 2.0f);
    fn_80087858(z, "MAX", &m58, 0, 0, -180.0f, 180.0f, 2.0f);
    fn_8008771C(group, "WAIT", &m60, 0, 0, -10.0f, 10.0f, 0.01f);
}

PBoneOutForce::PBoneOutForce() {}

PBoneOutForce::~PBoneOutForce() {}

// NONMATCHING: 99.9% - the Vector3 copy of math/Vec.h gets the source and destination address registers swapped
// (c = Cross_unk(c, v))
void PBoneOutForce::vf34() {
    Matrix3x4* root = m64->GetNodeMtx_unk(m38);
    Matrix3x4* tip = m64->GetNodeMtx_unk(m3C);
    Vector3 rootPos = root->GetTrans_unk();
    m20 += m2C * 0.033f;
    float gravity = 0.0333f * (9.8f * (0.1f * m60));
    Vector3 v = m14 + m20;
    PSVECNormalize(&v, &v);
    Vector3 dir;
    dir = tip->GetTrans_unk() - root->GetTrans_unk();
    PSVECNormalize(&dir, &dir);
    PSVECNormalize(&dir, &dir);
    Vector3 c = Cross_unk(v, dir);
    c = Cross_unk(c, v);
    if (!c.IsZero_unk()) {
        PSVECNormalize(&c, &c);
    }
    float dot = PSVECDotProduct(&dir, &c);
    c *= gravity * dot;
    m20 += c;
    m20 *= 0.9f;
}

} // namespace atn
