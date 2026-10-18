#include "atn/LH_ColliObject.h"
#include "atn/LoadManager.h"
#include "math/Vec.h"

extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" float PSVECMag(const atn::Vector3* v);

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))

// inferred: bounding sphere of a collision node
struct ColliSphere_unk {
    unsigned char m00[0xC];
    float mX; // 0x0C: center
    float mY;
    float mZ;
    float mRadius; // 0x18
};

// Helpers of fn_80472424 (names invented). The stack layout of the original (the transform vectors
// below the radius temporaries, the rotation steps in ascending order) and its register use (the
// rotation matrix address kept in a register, the scale product not) depend on this split.
inline atn::Vector3 __ToVec_unk(const float* p) {
    return atn::Vector3(p[0], p[1], p[2]);
}

inline void __MtxTranslate_unk(atn::Matrix3x4& m, const float* p) {
    Mtx a;
    Mtx b;
    const atn::Vector3& v = __ToVec_unk(p);
    PSMTXTrans(a, v.x, v.y, v.z);
    PSMTXConcat(m.m, a, b);
    __MtxCopy(m.m, b);
}

inline void __MtxRotAxis_unk(Mtx r, char axis, float rad) {
    if (rad != 0.0f) {
        Mtx a;
        Mtx t;
        PSMTXRotRad(a, axis, rad);
        PSMTXConcat(r, a, t);
        __MtxCopy(r, t);
    }
}

// inferred: rotation z * y * x
inline void __MtxRotateXYZ_unk(atn::Matrix3x4& m, const float* p) {
    const atn::Vector3& v = __ToVec_unk(p);
    Mtx b;
    atn::Matrix3x4 r;
    MtxPtr rp = r.m;
    r.SetIdentity();
    __MtxRotAxis_unk(rp, 'z', v.z);
    __MtxRotAxis_unk(rp, 'y', v.y);
    __MtxRotAxis_unk(rp, 'x', v.x);
    PSMTXConcat(m.m, rp, b);
    __MtxCopy(m.m, b);
}

inline void __MtxScale_unk(atn::Matrix3x4& m, const float* p) {
    Mtx a;
    atn::Matrix3x4 b;
    const atn::Vector3& v = __ToVec_unk(p);
    PSMTXScale(a, v.x, v.y, v.z);
    PSMTXConcat(m.m, a, b.m);
    m = b;
}

inline atn::Vector3 __Lerp_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (a - b) * t + b;
}

// inferred: fabs through a float function (fabs + frsp)
inline float __Abs_unk(float x) {
    return __fabs(x);
}

inline float __Length_unk(const atn::Vector3& v) {
    return PSVECMag(&v);
}

// inferred: sphere (name invented)
struct Sphere_unk {
    Sphere_unk() {}
    Sphere_unk(float f) : mCenter(f, f, f), mRadius(f) {}
    void SetCenter_unk(const atn::Vector3& c) { mCenter = c; }
    atn::Vector3 mCenter;
    float mRadius;
};

// inferred: node of the collision tree
struct ColliNode_unk {
    unsigned char m00[0x8];
    unsigned int mFlags;     // 0x08: 1 translate, 4 rotate, 8 scale
    ColliSphere_unk* mSphere; // 0x0C
    float mTrans[3];         // 0x10
    float mRot[3];           // 0x1C
    unsigned char m28[0x4];
    float mScale[3];         // 0x2C
    ColliNode_unk* mChild;   // 0x38
    ColliNode_unk* mNext;    // 0x3C
};

// inferred: object built from the loaded data (constructor 0x8043F5C4, destructor 0x8043F63C).
// At most 0x34 bytes: the original places it at sp+0x14, after the 12-byte scale vector.
struct ColliObjectData_unk {
    ColliObjectData_unk(void* data);
    ~ColliObjectData_unk();
    unsigned char m00[0x20];
    void* m20; // the collision object
    unsigned char m24[0x34 - 0x24];
};

// inferred: collision object of the file
struct ColliObject_unk {
    unsigned char m00[0xC];
    ColliNode_unk* mRoot; // 0x0C: inferred: root node
};

namespace atn {

// inferred from __vt__Q23atn15LoadColliObject (0x8078FC10)
class LoadColliObject : public LoadCore {
public:
    LoadColliObject(const char* path)
        : LoadCore(path), m5C(0), mBound(0.0f) {}
    virtual ~LoadColliObject();
    virtual void vf0C();
    // inferred: grows the bounding sphere with the spheres of the node tree
    void fn_80472424(void* node, const Matrix3x4& mtx, const Vector3& scale);

    ColliObject_unk* m5C;
    Sphere_unk mBound; // 0x60: inferred: bounding sphere of the whole object
};

LoadColliObject::~LoadColliObject() {}

void LoadColliObject::vf0C() {
    if (mState == 2) {
        ColliObjectData_unk data(m54);
        m5C = (ColliObject_unk*)data.m20;
        fn_80472424(m5C->mRoot, lbl_807C7060, Vector3(1.0f, 1.0f, 1.0f));
        fn_80473104(3);
    }
}

void LoadColliObject::fn_80472424(void* p, const Matrix3x4& mtx, const Vector3& scale) {
    while (p != 0) {
        ColliNode_unk* node = (ColliNode_unk*)p;
        ColliSphere_unk* sphere = node->mSphere;
        Vector3 scl(scale);
        Matrix3x4 m(mtx);
        if (node->mFlags & 1) {
            __MtxTranslate_unk(m, node->mTrans);
        }
        if (node->mFlags & 4) {
            __MtxRotateXYZ_unk(m, node->mRot);
        }
        if (node->mFlags & 8) {
            __MtxScale_unk(m, node->mScale);
            scl.x *= node->mScale[0];
            scl.y *= node->mScale[1];
            scl.z *= node->mScale[2];
        }
        if (sphere != 0) {
            float k = 1.01f * (0.57735026f * PSVECMag(&scl));
            Sphere_unk bound;
            Vector3 c;
            c.x = sphere->mX;
            c.y = sphere->mY;
            c.z = sphere->mZ;
            PSMTXMultVec(m.m, &c, &c);
            bound.SetCenter_unk(c);
            bound.mRadius = sphere->mRadius * k;
            if (bound.mRadius > 0.0f) {
                if (__Abs_unk(mBound.mRadius) < 1.1920929e-07f) {
                    mBound = bound;
                } else {
                    float t = mBound.mRadius / (mBound.mRadius + bound.mRadius);
                    Vector3 center = __Lerp_unk(mBound.mCenter, bound.mCenter, t);
                    float radius = __MAX_unk(bound.mRadius + __Length_unk(bound.mCenter - center),
                                             mBound.mRadius + __Length_unk(mBound.mCenter - center));
                    mBound.mCenter = center;
                    mBound.mRadius = radius;
                }
            }
        }
        if (node->mChild != 0) {
            fn_80472424(node->mChild, m, scl);
        }
        p = node->mNext;
    }
}

void* LH_ColliObject::fn_80472A34() {
    if (mCore != 0) {
        return ((LoadColliObject*)mCore)->m5C;
    }
    return 0;
}

void LH_ColliObject::vf0C(const char* path) {
    fn_80473F88();
    mCore = lbl_8087F518->Get_unk<LoadColliObject>(path, 1);
}

} // namespace atn
