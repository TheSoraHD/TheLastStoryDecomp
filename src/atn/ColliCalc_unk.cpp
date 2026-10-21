// atn/ColliCalc_unk.cpp: queries against the loaded collision data (file name invented). The
// sphere, capsule and swept sphere queries of atn::ColliObject walk the node tree of the object
// (fn_80059BAC and its copies) and test the triangles of each node (fn_80059660...); the queries
// of atn::ColliTree walk its octree (fn_8005AADC...). Each query has its own copy of the code.
#include "atn/ColliBase.h"
#include "atn/ColliData_unk.h"
#include "math/Vec.h"

extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);

using atn::ColliCapsuleShape_unk;
using atn::ColliResult_unk;
using atn::ColliSphereShape_unk;
using atn::ColliTriangle_unk;
using atn::Matrix3x4;
using atn::Vector3;

// inferred: the third query shape (vf14) has the layout of a capsule (two points and a radius): a
// swept sphere
typedef ColliCapsuleShape_unk ColliSweepShape_unk;

// inferred: shape tests (atn/ColliTest_unk.cpp)
extern "C" int fn_800523A0(ColliResult_unk* r, const ColliSphereShape_unk* s, const ColliSphereShape_unk* sphere);
extern "C" int fn_8005361C(ColliResult_unk* r, const ColliCapsuleShape_unk* c, const ColliSphereShape_unk* sphere);
extern "C" int fn_800551BC(ColliResult_unk* r, const ColliSweepShape_unk* b, const ColliSphereShape_unk* sphere);
extern "C" int fn_80051A88(const ColliSphereShape_unk* s, const ColliSphereShape_unk* bound);
extern "C" int fn_80051B70(const ColliCapsuleShape_unk* c, const ColliSphereShape_unk* bound);
extern "C" int fn_80051C10(const ColliSweepShape_unk* b, const ColliSphereShape_unk* bound);
extern "C" int fn_80053254(ColliResult_unk* r, const ColliSphereShape_unk* s, const ColliTriangle_unk* tri);
extern "C" int fn_80054038(ColliResult_unk* r, const ColliCapsuleShape_unk* c, const ColliTriangle_unk* tri);
extern "C" int fn_8005691C(ColliResult_unk* r, const ColliSweepShape_unk* b, const ColliTriangle_unk* tri);
// inferred: tests against a triangle in local space (transformed by mtx, radius scaled by scale)
extern "C" int fn_8005339C(ColliResult_unk* r, const ColliSphereShape_unk* s, const ColliObjTriangle_unk* tri,
                           const Matrix3x4* mtx, float scale);
extern "C" int fn_80056AE8(ColliResult_unk* r, const ColliSweepShape_unk* b, const ColliObjTriangle_unk* tri,
                           const Matrix3x4* mtx, float scale);

extern "C" int fn_80059BAC(ColliResult_unk* results, int max, const ColliSphereShape_unk* s, ColliObjNode_unk* node,
                           const Matrix3x4* mtx, const Vector3* scale, unsigned int mask, int attr);
extern "C" int fn_8005A0D4(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* c, ColliObjNode_unk* node,
                           const Matrix3x4* mtx, const Vector3* scale, unsigned int mask, int attr);
extern "C" int fn_8005A5FC(ColliResult_unk* results, int max, const ColliSweepShape_unk* b, ColliObjNode_unk* node,
                           const Matrix3x4* mtx, const Vector3* scale, unsigned int mask, int attr);
extern "C" int fn_8005AADC(ColliResult_unk* results, int max, const ColliSphereShape_unk* s, ColliTreeNode_unk* node,
                           unsigned int mask, int attr);
extern "C" int fn_8005AD7C(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* c, ColliTreeNode_unk* node,
                           unsigned int mask, int attr);
extern "C" int fn_8005B064(ColliResult_unk* results, int max, const ColliSweepShape_unk* b, ColliTreeNode_unk* node,
                           unsigned int mask, int attr);

// inferred: the attribute passes the query filter (no flag of the mask, any or the same id)
inline bool __AttrMatch_unk(const ColliAttr_unk* a, unsigned int mask, int attr) {
    return (a->mFlags & mask) == 0 && (attr < 0 || attr == a->mId);
}

// inferred: triangle in world space (static work copy of the capsule query)
struct ColliCalcTriangle_unk {
    Vector3 mVertex[3];
    Vector3 mNormal;
    Vector3 mCenter;
    float mRadius;
    // inferred: triangle transformed to world space (name invented)
    void Set_unk(const ColliTriangle_unk& src, const Matrix3x4* mtx, float scale);
};

// inferred: point transformed by a matrix (name invented)
inline Vector3 __Transform_unk(const Matrix3x4* m, const Vector3& v) {
    Vector3 r = v;
    PSMTXMultVec(m->m, &r, &r);
    return r;
}

// inferred: direction transformed by the rotation part of a matrix and normalized (name invented)
inline Vector3 __TransformNormal_unk(const Matrix3x4* m, const Vector3& v) {
    Matrix3x4 rot = m->GetRotation_unk();
    Vector3 r = v;
    PSMTXMultVec(rot.m, &r, &r);
    PSVECNormalize(&r, &r);
    return r;
}

inline void ColliCalcTriangle_unk::Set_unk(const ColliTriangle_unk& src, const Matrix3x4* mtx, float scale) {
    Vector3 n;
    mVertex[0] = __Transform_unk(mtx, ((ColliTriangle_unk&)src).Vertex_unk(0));
    mVertex[1] = __Transform_unk(mtx, ((ColliTriangle_unk&)src).Vertex_unk(1));
    mVertex[2] = __Transform_unk(mtx, ((ColliTriangle_unk&)src).Vertex_unk(2));
    mCenter = __Transform_unk(mtx, ((ColliTriangle_unk&)src).Center_unk());
    Matrix3x4 rot = mtx->GetRotation_unk();
    n = ((ColliTriangle_unk&)src).Normal_unk();
    PSMTXMultVec(rot.m, &n, &n);
    PSVECNormalize(&n, &n);
    mNormal = n;
    mRadius = src.mRadius * scale;
}

extern "C" int fn_80059660(ColliResult_unk* results, int max, const ColliSphereShape_unk* s, ColliObjTriList_unk* list,
                           const Matrix3x4* mtx, const ColliSphereShape_unk* bound, unsigned int mask, int attr,
                           float scale) {
    if (list->mTriangles == 0) {
        return fn_800523A0(results, s, bound) != 0;
    }
    if (fn_80051A88(s, bound) == 0) {
        return 0;
    }
    int n = 0;
    for (unsigned int i = 0; i < list->mCount; i++) {
        ColliObjTriangle_unk* tri = &list->mTriangles[i];
        ColliAttr_unk* a = tri->mAttr;
        if ((mask & a->mFlags) == 0 && (attr < 0 || attr == a->mId) && fn_8005339C(results, s, tri, mtx, scale) != 0) {
            results->m34 = a;
            n++;
            results->m40 = tri;
            if (n >= max) {
                return n;
            }
            results++;
        }
    }
    return n;
}

extern "C" int fn_80059784(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* c,
                           ColliObjTriList_unk* list, const Matrix3x4* mtx, const ColliSphereShape_unk* bound,
                           unsigned int mask, int attr, float scale) {
    if (list->mTriangles == 0) {
        return fn_8005361C(results, c, bound) != 0;
    }
    if (fn_80051B70(c, bound) == 0) {
        return 0;
    }
    int n = 0;
    for (unsigned int i = 0; i < list->mCount; i++) {
        ColliTriangle_unk* src = &list->mTriangles[i].mTri;
        static ColliCalcTriangle_unk tri;
        tri.Set_unk(*src, mtx, scale);
        ColliAttr_unk* a = list->mTriangles[i].mAttr;
        if ((mask & a->mFlags) == 0 && (attr < 0 || attr == a->mId) && fn_80054038(results, c, (ColliTriangle_unk*)&tri) != 0) {
            n++;
            results->m34 = a;
            if (n >= max) {
                return n;
            }
            results++;
        }
    }
    return n;
}

extern "C" int fn_80059A40(ColliResult_unk* results, int max, const ColliSweepShape_unk* b, ColliObjTriList_unk* list,
                           const Matrix3x4* mtx, const ColliSphereShape_unk* bound, unsigned int mask, int attr,
                           float scale) {
    if (list->mTriangles == 0) {
        return fn_800551BC(results, b, bound) != 0;
    }
    if (fn_80051C10(b, bound) == 0) {
        return 0;
    }
    int n = 0;
    for (unsigned int i = 0; i < list->mCount; i++) {
        ColliObjTriangle_unk* tri = &list->mTriangles[i];
        ColliAttr_unk* a = tri->mAttr;
        if ((mask & a->mFlags) == 0 && (attr < 0 || attr == a->mId) && fn_80056AE8(results, b, tri, mtx, scale) != 0) {
            n++;
            results->m34 = a;
            if (n >= max) {
                return n;
            }
            results++;
        }
    }
    return n;
}

extern "C" int fn_80059B5C(ColliResult_unk* results, int max, const ColliSphereShape_unk* s, ColliObjData_unk* object,
                           const Matrix3x4* mtx, unsigned int mask, int attr) {
    if (object == 0) {
        return 0;
    }
    Vector3 scale(1.0f, 1.0f, 1.0f);
    return fn_80059BAC(results, max, s, object->mRoot, mtx, &scale, mask, attr);
}

// Helpers of the node walkers (as in atn/LH_ColliObject.cpp; names invented)
inline Vector3 __Vec3_unk(const ColliVec_unk& p) {
    return Vector3(p.x, p.y, p.z);
}

inline void __MtxTranslate_unk(Matrix3x4& m, const ColliVec_unk& p) {
    Mtx a;
    Mtx b;
    const Vector3& v = __Vec3_unk(p);
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
inline void __MtxRotateXYZ_unk(Matrix3x4& m, const ColliVec_unk& p) {
    const Vector3& v = __Vec3_unk(p);
    Mtx b;
    Matrix3x4 r;
    MtxPtr rp = r.m;
    r.SetIdentity();
    __MtxRotAxis_unk(rp, 'z', v.z);
    __MtxRotAxis_unk(rp, 'y', v.y);
    __MtxRotAxis_unk(rp, 'x', v.x);
    PSMTXConcat(m.m, rp, b);
    __MtxCopy(m.m, b);
}

inline void __MtxScale_unk(Matrix3x4& m, const ColliVec_unk& p) {
    Mtx a;
    Matrix3x4 b;
    const Vector3& v = __Vec3_unk(p);
    PSMTXScale(a, v.x, v.y, v.z);
    PSMTXConcat(m.m, a, b.m);
    m = b;
}

// inferred: bounding sphere of a triangle list in world space, then the test of its triangles
// (inlined in the node walkers; names invented)
inline int __TestSphereList_unk(ColliResult_unk* results, int max, const ColliSphereShape_unk* s,
                                ColliObjTriList_unk* list, const Matrix3x4& m, float k, unsigned int mask, int attr) {
    ColliSphereShape_unk bound;
    Vector3 c;
    c.x = list->mCenter.x;
    c.y = list->mCenter.y;
    c.z = list->mCenter.z;
    PSMTXMultVec(m.m, &c, &c);
    bound.mCenter = c;
    bound.mRadius = list->mRadius * k;
    return fn_80059660(results, max, s, list, &m, &bound, mask, attr, k);
}

inline int __TestCapsuleList_unk(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* cap,
                                 ColliObjTriList_unk* list, const Matrix3x4& m, float k, unsigned int mask, int attr) {
    ColliSphereShape_unk bound;
    Vector3 c;
    c.x = list->mCenter.x;
    c.y = list->mCenter.y;
    c.z = list->mCenter.z;
    PSMTXMultVec(m.m, &c, &c);
    bound.mCenter = c;
    bound.mRadius = list->mRadius * k;
    return fn_80059784(results, max, cap, list, &m, &bound, mask, attr, k);
}

inline int __TestSweepList_unk(ColliResult_unk* results, int max, const ColliSweepShape_unk* b,
                               ColliObjTriList_unk* list, const Matrix3x4& m, float k, unsigned int mask, int attr) {
    ColliSphereShape_unk bound;
    Vector3 c;
    c.x = list->mCenter.x;
    c.y = list->mCenter.y;
    c.z = list->mCenter.z;
    PSMTXMultVec(m.m, &c, &c);
    bound.mCenter = c;
    bound.mRadius = list->mRadius * k;
    return fn_80059A40(results, max, b, list, &m, &bound, mask, attr, k);
}

extern "C" int fn_80059BAC(ColliResult_unk* results, int max, const ColliSphereShape_unk* s, ColliObjNode_unk* node,
                           const Matrix3x4* mtx, const Vector3* scale, unsigned int mask, int attr) {
    int n = 0;
    while (node != 0) {
        if (n >= max) {
            return n;
        }
        Vector3 scl(*scale);
        Matrix3x4 m(*mtx);
        if (node->mFlags & 1) {
            __MtxTranslate_unk(m, node->mTrans);
        }
        if (node->mFlags & 4) {
            __MtxRotateXYZ_unk(m, node->mRot);
        }
        if (node->mFlags & 8) {
            __MtxScale_unk(m, node->mScale);
            scl.x *= node->mScale.x;
            scl.y *= node->mScale.y;
            scl.z *= node->mScale.z;
        }
        if (node->mList != 0) {
            float k = 1.01f * (0.57735026f * PSVECMag(&scl));
            int r = __TestSphereList_unk(results, max - n, s, node->mList, m, k, mask, attr);
            n += r;
            if (n >= max) {
                return n;
            }
            results += r;
        }
        if (node->m38 != 0) {
            int r = fn_80059BAC(results, max - n, s, node->m38, &m, &scl, mask, attr);
            n += r;
            if (n >= max) {
                return n;
            }
            results += r;
        }
        node = node->m3C;
    }
    return n;
}

extern "C" int fn_8005A084(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* c, ColliObjData_unk* object,
                           const Matrix3x4* mtx, unsigned int mask, int attr) {
    if (object == 0) {
        return 0;
    }
    Vector3 scale(1.0f, 1.0f, 1.0f);
    return fn_8005A0D4(results, max, c, object->mRoot, mtx, &scale, mask, attr);
}

extern "C" int fn_8005A0D4(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* c, ColliObjNode_unk* node,
                           const Matrix3x4* mtx, const Vector3* scale, unsigned int mask, int attr) {
    int n = 0;
    while (node != 0) {
        if (n >= max) {
            return n;
        }
        Vector3 scl(*scale);
        Matrix3x4 m(*mtx);
        if (node->mFlags & 1) {
            __MtxTranslate_unk(m, node->mTrans);
        }
        if (node->mFlags & 4) {
            __MtxRotateXYZ_unk(m, node->mRot);
        }
        if (node->mFlags & 8) {
            __MtxScale_unk(m, node->mScale);
            scl.x *= node->mScale.x;
            scl.y *= node->mScale.y;
            scl.z *= node->mScale.z;
        }
        if (node->mList != 0) {
            float k = 1.01f * (0.57735026f * PSVECMag(&scl));
            int r = __TestCapsuleList_unk(results, max - n, c, node->mList, m, k, mask, attr);
            n += r;
            if (n >= max) {
                return n;
            }
            results += r;
        }
        if (node->m38 != 0) {
            int r = fn_8005A0D4(results, max - n, c, node->m38, &m, &scl, mask, attr);
            n += r;
            if (n >= max) {
                return n;
            }
            results += r;
        }
        node = node->m3C;
    }
    return n;
}

extern "C" int fn_8005A5AC(ColliResult_unk* results, int max, const ColliSweepShape_unk* b, ColliObjData_unk* object,
                           const Matrix3x4* mtx, unsigned int mask, int attr) {
    if (object == 0) {
        return 0;
    }
    Vector3 scale(1.0f, 1.0f, 1.0f);
    return fn_8005A5FC(results, max, b, object->mRoot, mtx, &scale, mask, attr);
}

extern "C" int fn_8005A5FC(ColliResult_unk* results, int max, const ColliSweepShape_unk* b, ColliObjNode_unk* node,
                           const Matrix3x4* mtx, const Vector3* scale, unsigned int mask, int attr) {
    int n = 0;
    while (node != 0) {
        if (n >= max) {
            return n;
        }
        Vector3 scl(*scale);
        Matrix3x4 m(*mtx);
        if (node->mFlags & 1) {
            __MtxTranslate_unk(m, node->mTrans);
        }
        if (node->mFlags & 4) {
            __MtxRotateXYZ_unk(m, node->mRot);
        }
        if (node->mFlags & 8) {
            __MtxScale_unk(m, node->mScale);
            scl.x *= node->mScale.x;
            scl.y *= node->mScale.y;
            scl.z *= node->mScale.z;
        }
        if (node->mList != 0) {
            float k = 1.01f * (0.57735026f * PSVECMag(&scl));
            int r = __TestSweepList_unk(results, max - n, b, node->mList, m, k, mask, attr);
            n += r;
            if (n >= max) {
                return n;
            }
            results += r;
        }
        if (node->m38 != 0) {
            int r = fn_8005A5FC(results, max - n, b, node->m38, &m, &scl, mask, attr);
            n += r;
            if (n >= max) {
                return n;
            }
            results += r;
        }
        node = node->m3C;
    }
    return n;
}

// inferred: the sphere touches the box of the node
inline bool __SphereInNode_unk(const ColliTreeNode_unk* node, const ColliSphereShape_unk* s) {
    if (node->mBox[0].x > s->mCenter.x + s->mRadius) {
        return false;
    }
    if (node->mBox[0].y > s->mCenter.y + s->mRadius) {
        return false;
    }
    if (node->mBox[0].z > s->mCenter.z + s->mRadius) {
        return false;
    }
    if (node->mBox[1].x < s->mCenter.x - s->mRadius) {
        return false;
    }
    if (node->mBox[1].y < s->mCenter.y - s->mRadius) {
        return false;
    }
    return !(node->mBox[1].z < s->mCenter.z - s->mRadius);
}

// inferred: the sphere touches the z range [min, max] (center and radius in z)
inline bool __SphereInRangeZ_unk(float min, float max, float r, float z) {
    if (min > z + r) {
        return false;
    }
    return !(max < z - r);
}

extern "C" int fn_8005AAD4(ColliResult_unk* results, int max, const ColliSphereShape_unk* s, ColliTreeData_unk* tree,
                           unsigned int mask, int attr) {
    return fn_8005AADC(results, max, s, tree->mRoot, mask, attr);
}

extern "C" int fn_8005AADC(ColliResult_unk* results, int max, const ColliSphereShape_unk* s, ColliTreeNode_unk* node,
                           unsigned int mask, int attr) {
    int n = 0;
    if (__SphereInNode_unk(node, s)) {
        ColliTreeTriangle_unk** tris = node->mTriangles;
        for (int i = 0; i < node->mCount; i++) {
            ColliTreeTriangle_unk* tri = tris[i];
            if (n >= max) {
                return n;
            }
            ColliAttr_unk* a = tri->mAttr;
            if ((mask & a->mFlags) == 0 && (attr < 0 || attr == a->mId) && fn_80053254(results, s, &tri->mTri) != 0) {
                n++;
                results->m34 = a;
                results->m40 = &tri->mTri;
                if (n >= max) {
                    return n;
                }
                results++;
            }
        }
        int i = 0;
        float mid = 0.5f * (node->mBox[0].z + node->mBox[1].z);
        if (!__SphereInRangeZ_unk(node->mBox[0].z, mid, s->mRadius, s->mCenter.z)) {
            i = 4;
        }
        for (; i < 8; i++) {
            if (node->mChildren[i] != 0) {
                int r = fn_8005AADC(results, max - n, s, node->mChildren[i], mask, attr);
                n += r;
                if (n >= max) {
                    return n;
                }
                results += r;
            }
            if (i == 3 && !__SphereInRangeZ_unk(mid, node->mBox[1].z, s->mRadius, s->mCenter.z)) {
                break;
            }
        }
    }
    return n;
}

// inferred: the segment of the capsule touches the box of the node (the radius is not used)
inline bool __SegmentInNode_unk(const ColliTreeNode_unk* node, const ColliCapsuleShape_unk* c) {
    if (node->mBox[0].x > c->mStart.x && node->mBox[0].x > c->mEnd.x) {
        return false;
    }
    if (node->mBox[0].y > c->mStart.y && node->mBox[0].y > c->mEnd.y) {
        return false;
    }
    if (node->mBox[0].z > c->mStart.z && node->mBox[0].z > c->mEnd.z) {
        return false;
    }
    if (node->mBox[1].x < c->mStart.x && node->mBox[1].x < c->mEnd.x) {
        return false;
    }
    if (node->mBox[1].y < c->mStart.y && node->mBox[1].y < c->mEnd.y) {
        return false;
    }
    if (node->mBox[1].z < c->mStart.z && node->mBox[1].z < c->mEnd.z) {
        return false;
    }
    return true;
}

inline bool __SegmentInRangeZ_unk(float min, float max, float z0, float z1) {
    if (min > z0 && min > z1) {
        return false;
    }
    if (max < z0 && max < z1) {
        return false;
    }
    return true;
}

extern "C" int fn_8005AD74(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* c, ColliTreeData_unk* tree,
                           unsigned int mask, int attr) {
    return fn_8005AD7C(results, max, c, tree->mRoot, mask, attr);
}

extern "C" int fn_8005AD7C(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* c, ColliTreeNode_unk* node,
                           unsigned int mask, int attr) {
    int n = 0;
    if (__SegmentInNode_unk(node, c)) {
        ColliTreeTriangle_unk** tris = node->mTriangles;
        for (int i = 0; i < node->mCount; i++) {
            ColliTreeTriangle_unk* tri = tris[i];
            if (n >= max) {
                return n;
            }
            ColliAttr_unk* a = tri->mAttr;
            if ((mask & a->mFlags) == 0 && (attr < 0 || attr == a->mId) && fn_80054038(results, c, &tri->mTri) != 0) {
                n++;
                results->m34 = a;
                results->m40 = &tri->mTri;
                if (n >= max) {
                    return n;
                }
                results++;
            }
        }
        int i = 0;
        float mid = 0.5f * (node->mBox[0].z + node->mBox[1].z);
        if (!__SegmentInRangeZ_unk(node->mBox[0].z, mid, c->mStart.z, c->mEnd.z)) {
            i = 4;
        }
        for (; i < 8; i++) {
            if (node->mChildren[i] != 0) {
                int r = fn_8005AD7C(results, max - n, c, node->mChildren[i], mask, attr);
                n += r;
                if (n >= max) {
                    return n;
                }
                results += r;
            }
            if (i == 3 && !__SegmentInRangeZ_unk(mid, node->mBox[1].z, c->mStart.z, c->mEnd.z)) {
                break;
            }
        }
    }
    return n;
}

// inferred: the swept sphere touches the box of the node
inline bool __SweepInNode_unk(const ColliTreeNode_unk* node, const ColliSweepShape_unk* b) {
    if (node->mBox[0].x > b->mStart.x + b->mRadius && node->mBox[0].x > b->mEnd.x + b->mRadius) {
        return false;
    }
    if (node->mBox[0].y > b->mStart.y + b->mRadius && node->mBox[0].y > b->mEnd.y + b->mRadius) {
        return false;
    }
    if (node->mBox[0].z > b->mStart.z + b->mRadius && node->mBox[0].z > b->mEnd.z + b->mRadius) {
        return false;
    }
    if (node->mBox[1].x < b->mStart.x - b->mRadius && node->mBox[1].x < b->mEnd.x - b->mRadius) {
        return false;
    }
    if (node->mBox[1].y < b->mStart.y - b->mRadius && node->mBox[1].y < b->mEnd.y - b->mRadius) {
        return false;
    }
    if (node->mBox[1].z < b->mStart.z - b->mRadius && node->mBox[1].z < b->mEnd.z - b->mRadius) {
        return false;
    }
    return true;
}

inline bool __SweepInRangeZ_unk(float min, float max, const ColliSweepShape_unk* b) {
    if (min > b->mStart.z + b->mRadius && min > b->mEnd.z + b->mRadius) {
        return false;
    }
    if (max < b->mStart.z - b->mRadius && max < b->mEnd.z - b->mRadius) {
        return false;
    }
    return true;
}

extern "C" int fn_8005B05C(ColliResult_unk* results, int max, const ColliSweepShape_unk* b, ColliTreeData_unk* tree,
                           unsigned int mask, int attr) {
    return fn_8005B064(results, max, b, tree->mRoot, mask, attr);
}

extern "C" int fn_8005B064(ColliResult_unk* results, int max, const ColliSweepShape_unk* b, ColliTreeNode_unk* node,
                           unsigned int mask, int attr) {
    int n = 0;
    if (__SweepInNode_unk(node, b)) {
        ColliTreeTriangle_unk** tris = node->mTriangles;
        for (int i = 0; i < node->mCount; i++) {
            ColliTreeTriangle_unk* tri = tris[i];
            if (n >= max) {
                return n;
            }
            ColliAttr_unk* a = tri->mAttr;
            if ((mask & a->mFlags) == 0 && (attr < 0 || attr == a->mId) && fn_8005691C(results, b, &tri->mTri) != 0) {
                n++;
                results->m34 = a;
                results->m40 = &tri->mTri;
                if (n >= max) {
                    return n;
                }
                results++;
            }
        }
        int i = 0;
        float mid = 0.5f * (node->mBox[0].z + node->mBox[1].z);
        if (!__SweepInRangeZ_unk(node->mBox[0].z, mid, b)) {
            i = 4;
        }
        for (; i < 8; i++) {
            if (node->mChildren[i] != 0) {
                int r = fn_8005B064(results, max - n, b, node->mChildren[i], mask, attr);
                n += r;
                if (n >= max) {
                    return n;
                }
                results += r;
            }
            if (i == 3 && !__SweepInRangeZ_unk(mid, node->mBox[1].z, b)) {
                break;
            }
        }
    }
    return n;
}
