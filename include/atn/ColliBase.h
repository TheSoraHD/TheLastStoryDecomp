#ifndef ATN_COLLIBASE_H
#define ATN_COLLIBASE_H

#include "atn/LH_ColliTree.h"
#include "atn/LH_ColliObject.h"
#include "math/Vec.h"

namespace atn {

// Layout inferred from the inlined constructor in ColliTree::ColliTree (0x80058A1C).
class ColliBase {
public:
    ColliBase(int type);  // 0x80056DB8 (inlined in this TU's own constructors)
    virtual ~ColliBase(); // 0x80056E40
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();

    int m04;
    unsigned int mFlags; // 0x08
    int m0C;
    int m10;
    int m14;
    int m18;
    int m1C;
    int m20;
    int m24;
    unsigned char m28[0x14];
};

class ColliTree : public ColliBase {
public:
    ColliTree(); // 0x80058A1C
    virtual ~ColliTree() {}

    void fn_80058B78(const char* path); // inferred: starts loading the tree
    int fn_80058BBC();                  // inferred: nonzero while loading

    LH_ColliTree mLoader; // 0x3C
    void* mTree;          // 0x44
    float m48[12];
};

// The name is in the DOL (__vt__Q23atn8ColliBox, 0x807775F8). 0x378 bytes: gmk::FloatingBox
// has its own members right after it.
class ColliBox : public ColliBase {
public:
    ColliBox() : ColliBase(2) {}
    virtual ~ColliBox() {}

    unsigned char m3C[0x378 - 0x3C]; // inferred: m3C is the box tested by fn_80051CD8
};

// The name is in the DOL (__vt__Q23atn11ColliSphere, 0x80777668). Layout inferred from
// gmk::LandMine.
class ColliSphere : public ColliBase {
public:
    // inferred: center and radius, assigned as one value
    struct Sphere_unk {
        Sphere_unk() {}
        Sphere_unk(const Vector3& center, float radius) : mCenter(center), mRadius(radius) {}
        Vector3 mCenter;
        float mRadius;
    };
    ColliSphere() : ColliBase(0) {}
    virtual ~ColliSphere() {}

    Sphere_unk mSphere; // 0x3C
};

// The name is in the DOL (__vt__Q23atn12ColliCapsule, 0x80777630); layout from CharaTask (+0x5B8).
class ColliCapsule : public ColliBase {
public:
    ColliCapsule() : ColliBase(1) {}
    virtual ~ColliCapsule() {}

    Vector3 m3C; // inferred: first point of the segment, other end of the capsule (CharaTask::vf20, vf64)
    Vector3 m48; // inferred: position (chr::ControlAppear); second point of the segment
    float m54;   // inferred: radius
};

// The name is in the DOL (__vt__Q23atn11ColliObject, 0x807775C0); layout from the constructor.
class ColliObject : public ColliBase {
public:
    ColliObject(); // 0x80057F28
    virtual ~ColliObject() {}

    void fn_80058078(const char* path); // inferred: starts loading the object
    int fn_800580BC();                  // inferred: nonzero while loading
    void fn_800588FC();                 // inferred: updates with mMtx
    // inferred: inlined in gmk::SkeltonsMirror::vf28 (name invented)
    void SetMtx_unk(const Matrix3x4& mtx) {
        mMtx = mtx;
        fn_800588FC();
    }

    LH_ColliObject mLoader; // 0x3C
    void* mObject;          // 0x44
    Matrix3x4 mMtx;         // 0x48: inferred: world matrix (identity in the constructor)
    unsigned char m78[0x88 - 0x78];
};

} // namespace atn

#endif
