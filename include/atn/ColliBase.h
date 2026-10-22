#ifndef ATN_COLLIBASE_H
#define ATN_COLLIBASE_H

#include "atn/LH_ColliTree.h"
#include "atn/LH_ColliObject.h"
#include "math/Vec.h"

// Collision shapes of the engine. The constructors/destructors, the overrides and the vtables are
// in atn/atn_colli.cpp (0x80056DB8-0x80059660); the virtuals of atn::ColliBase itself, the
// collision manager (lbl_8087EE98, fn_8004CB70/fn_8004CBB0) and the ray/sphere queries are in
// atn/ColliBaseFunc_unk.cpp (0x8004C9B4); the shape tests (fn_800523A0, fn_80053254...) are in
// atn/ColliTest_unk.cpp and the triangle walkers of the loaded meshes (fn_80059B5C...) in
// atn/ColliCalc_unk.cpp. The class order below is the one of the file (the vtables of
// atn_colli.cpp are emitted in reverse order and the weak destructors in this order).

// inferred: nodes of the loaded collision data (defined in atn/atn_colli.cpp)
struct ColliObjNode_unk;
struct ColliTreeNode_unk;

namespace atn {

class ColliBase;

// inferred: sphere (center, radius) used as the query of the sphere tests (vf0C)
struct ColliSphereShape_unk {
    ColliSphereShape_unk() {}
    ColliSphereShape_unk(const Vector3& center, float radius) : mCenter(center), mRadius(radius) {}
    Vector3 mCenter;
    float mRadius;
};

// inferred: capsule (segment + radius) used as the query of the capsule tests (vf10) and by
// atn::ColliCapsule
struct ColliCapsuleShape_unk {
    ColliCapsuleShape_unk() {}
    ColliCapsuleShape_unk(const Vector3& start, const Vector3& end, float radius)
        : mStart(start), mEnd(end), mRadius(radius) {}
    // inferred: makes the segment vertical, keeping its length (ColliBase::mFlags 0x80000000;
    // name invented)
    void SetUpright_unk() {
        float len = (mEnd - mStart).Length_unk();
        mEnd = mStart;
        if (len < 0.0001f) {
            len = 0.0001f;
        }
        mEnd.y += len;
    }
    Vector3 mStart;
    Vector3 mEnd;
    float mRadius;
};

// inferred: oriented box (half size, then the world matrix) of atn::ColliBox (gmk::OnOff assigns it
// as one value)
struct ColliBoxShape_unk {
    Vector3 mSize;  // 0x00: half size
    Matrix3x4 mMtx; // 0x0C
};

// inferred: transforms a point by the rotation part of a matrix (name invented)
inline Vector3 Transform_unk(const Matrix3x4& m, const Vector3& v) {
    Vector3 r = v;
    PSMTXMultVec(m.m, &r, &r);
    return r;
}

// inferred: triangle of a box (atn::ColliBox) or of a loaded mesh: corners, unit normal and bounding
// sphere (center, radius) (0x40 bytes, built by ColliBox::fn_80057A78). Plain floats: the
// triangles of atn::ColliBox are not constructed one by one (no constructor loop in the inline
// constructors of the gimmicks), so the type has no constructor; the accessors give vectors.
struct ColliTriangle_unk {
    Vector3& Vertex_unk(int i) { return *(Vector3*)mVertex[i]; }
    Vector3& Normal_unk() { return *(Vector3*)mNormal; }
    Vector3& Center_unk() { return *(Vector3*)mCenter; }
    float mVertex[3][3]; // 0x00
    float mNormal[3];    // 0x24
    float mCenter[3];    // 0x30
    float mRadius;       // 0x3C
};

// inferred: one hit of a query (0x50 bytes, arrays of them are filled by the vf0C/vf10/vf14 tests;
// other files declare their own copy as CollResult_unk)
// (the constructor clears m34-m40: the results array of the collision manager and the local results
// of atn/ColliBaseFunc_unk.cpp; the copies are member by member, the vectors with psq)
struct ColliResult_unk {
    ColliResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    int m00;
    Vector3 m04;        // inferred: point of the shape that touches (distance to the query start)
    Vector3 m10;        // inferred: hit position (returned by fn_8004ECC0/fn_8004D314)
    Vector3 m1C;
    Vector3 m28;        // inferred: normal of the hit (its y classifies floor/wall/ceiling)
    void* m34;          // inferred: attribute of the hit (&ColliBase::m1C, or the mesh attribute)
    ColliBase* m38;     // inferred: hit object
    unsigned int m3C;   // inferred: flags of the hit, set by the queries of the collision manager
    void* m40;          // inferred: hit triangle of a mesh
    Vector3 m44;        // inferred: position of a special attribute (0x1000/0x2000/0x4000)
};

// inferred: grid of the collision manager (argument of vf1C, which updates the cell of the shape)
struct ColliGrid_unk {
    int m00;
    Vector3 mOrigin;  // 0x04
    unsigned char m10[0x1C - 0x10];
    float mCellSize;  // 0x1C
};

// inferred: bounds of the collision trees (argument of ColliTree::fn_80058EE4)
struct ColliTreeBounds_unk {
    void* m00;      // inferred: first tree added
    Vector3 mMin;   // 0x04
    Vector3 mMax;   // 0x10
    float mCellSize; // 0x1C
    int mCells;      // 0x20
};

// Layout from the constructor (0x80056DB8).
class ColliBase {
public:
    ColliBase(int type);  // 0x80056DB8 (inlined in this TU's own constructors)
    virtual ~ColliBase(); // 0x80056E40
    // The base versions (0x8004CF34...) do nothing / return 0. results: array of `max` hits;
    // returns the number of hits.
    virtual int vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                     int arg);
    // inferred: capsule/segment test (result: collision data whose +4 is the hit position;
    // EnemyFieldMagician::vf3C); nonzero on a hit
    virtual int vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                     int arg);
    virtual int vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg);
    virtual void vf18(int a, int b, int c, int d); // inferred: debug drawing
    virtual void vf1C(const ColliGrid_unk* grid);  // inferred: updates the grid cell (m10-m18)

    int fn_80056EB0(const int* cell) const; // inferred: the shape is in the cell range (x, z, size)
    // inferred: cell of a bounding sphere (inlined by the vf1C of the sphere, box and object; name
    // invented)
    void SetCell_unk(const ColliSphereShape_unk& s, const ColliGrid_unk* grid) {
        float size = grid->mCellSize;
        float inv = 1.0f / size;
        Vector3 c = s.mCenter - grid->mOrigin;
        c *= inv;
        m10 = (int)c.x;
        m14 = (int)c.z;
        m18 = (int)(s.mRadius / size) + 1;
    }

    int m04;             // inferred: type (0 sphere, 1 capsule, 2 box, 3 object, 4 tree)
    unsigned int mFlags; // 0x08: bit 2 set by ColliTree; 0x80000000 = upright capsule
    int m0C;             // inferred: owner
    int m10;             // inferred: grid cell x (-1 = none)
    int m14;             // inferred: grid cell z
    int m18;             // inferred: size in cells
    int m1C;             // inferred: attribute (its address is the attribute of the hits)
    int m20;
    int m24;
    unsigned char m28[0x14];
};

// The name is in the DOL (__vt__Q23atn11ColliSphere, 0x80777668). Layout inferred from
// gmk::LandMine.
class ColliSphere : public ColliBase {
public:
    // inferred: center and radius, assigned as one value
    typedef ColliSphereShape_unk Sphere_unk;
    ColliSphere() : ColliBase(0) {}
    virtual int vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                     int arg);
    virtual int vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                     int arg);
    virtual int vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg);
    virtual void vf18(int a, int b, int c, int d);
    virtual void vf1C(const ColliGrid_unk* grid);

    Sphere_unk mSphere; // 0x3C
};

// The name is in the DOL (__vt__Q23atn12ColliCapsule, 0x80777630); layout from CharaTask (+0x5B8).
class ColliCapsule : public ColliBase {
public:
    ColliCapsule() : ColliBase(1) {}
    virtual int vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                     int arg);
    virtual int vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                     int arg);
    virtual int vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg);
    virtual void vf18(int a, int b, int c, int d);
    virtual void vf1C(const ColliGrid_unk* grid);

    Vector3 m3C; // inferred: first point of the segment, other end of the capsule (CharaTask::vf20, vf64)
    Vector3 m48; // inferred: position (chr::ControlAppear); second point of the segment
    float m54;   // inferred: radius
};

// The name is in the DOL (__vt__Q23atn8ColliBox, 0x807775F8). 0x378 bytes: gmk::FloatingBox
// has its own members right after it.
class ColliBox : public ColliBase {
public:
    ColliBox() : ColliBase(2) {}
    virtual int vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                     int arg);
    virtual int vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                     int arg);
    virtual int vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg);
    virtual void vf18(int a, int b, int c, int d);
    virtual void vf1C(const ColliGrid_unk* grid);

    // inferred: sets the box (center, rotation, half size) and builds the triangles
    void fn_800575BC(const Vector3* pos, const Vector3* rot, const Vector3* size);
    void fn_800577D8(); // inferred: builds the triangles of m3C
    void fn_80057A78(ColliTriangle_unk* tri, const Vector3* a, const Vector3* b, const Vector3* c);
    // inferred: bounding sphere of the box (inlined in vf1C; name invented)
    ColliSphereShape_unk GetBound_unk() const {
        ColliSphereShape_unk bound;
        bound.mCenter = m3C.mMtx.GetTrans_unk();
        bound.mRadius = PSVECMag(&m3C.mSize);
        return bound;
    }

    ColliBoxShape_unk m3C;            // inferred: the box tested by fn_80051CD8
    ColliTriangle_unk mTriangles[12]; // 0x78: two per face, built by fn_800577D8
};

// The name is in the DOL (__vt__Q23atn11ColliObject, 0x807775C0); layout from the constructor.
class ColliObject : public ColliBase {
public:
    ColliObject(); // 0x80057F28
    virtual int vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                     int arg);
    virtual int vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                     int arg);
    virtual int vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg);
    virtual void vf18(int a, int b, int c, int d);
    virtual void vf1C(const ColliGrid_unk* grid);

    void fn_80058078(const char* path); // inferred: starts loading the object
    int fn_800580BC();                  // inferred: nonzero while loading
    void fn_800588FC();                 // inferred: updates with mMtx
    int fn_800584B4(int a, int b, int c); // inferred: debug drawing of the triangles
    int fn_800584D8(int a, int b, int n, ColliObjNode_unk* node, int c, int d, int e);
    // inferred: inlined in gmk::SkeltonsMirror::vf28 (name invented)
    void SetMtx_unk(const Matrix3x4& mtx) {
        mMtx = mtx;
        fn_800588FC();
    }

    LH_ColliObject mLoader; // 0x3C
    void* mObject;          // 0x44
    Matrix3x4 mMtx;         // 0x48: inferred: world matrix (identity in the constructor)
    ColliSphereShape_unk mBound; // 0x78: inferred: bounding sphere in world space (fn_800588FC)
};

// Layout inferred from the inlined constructor in ColliTree::ColliTree (0x80058A1C).
class ColliTree : public ColliBase {
public:
    ColliTree(); // 0x80058A1C
    virtual int vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                     int arg);
    virtual int vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                     int arg);
    virtual int vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg);
    virtual void vf18(int a, int b, int c, int d);

    void fn_80058B78(const char* path); // inferred: starts loading the tree
    int fn_80058BBC();                  // inferred: nonzero while loading
    void fn_80058EE4(ColliTreeBounds_unk* bounds); // inferred: adds the tree to the bounds
    int fn_80059240(int a, int b, int c); // inferred: debug drawing of the triangles
    int fn_80059264(int a, int b, int n, ColliTreeNode_unk* node, int c, int colored, int e);

    LH_ColliTree mLoader; // 0x3C
    void* mTree;          // 0x44
    Matrix3x4 mMtx;       // 0x48: inferred: identity in the constructor, unused here
};

} // namespace atn

#endif
