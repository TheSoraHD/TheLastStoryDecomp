// atn/atn_colli.cpp: collision shapes of the engine (atn::ColliBase and its subclasses
// ColliSphere, ColliCapsule, ColliBox, ColliObject and ColliTree). The file name is inferred from
// the engine files atn_<name>.cpp, which are linked in alphabetical order.
#include "atn/ColliBase.h"
#include "atn/Camera_unk.h"
#include "math/Vec.h"

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void GXBegin(int type, int fmt, unsigned short n);

// inferred: collision manager (atn/ColliBaseFunc_unk.cpp): lists of the shapes
struct ColliManager_unk;
extern "C" ColliManager_unk* lbl_8087EE98;
extern "C" int fn_8004CB70(ColliManager_unk* mgr, atn::ColliBase* colli);  // inferred: adds a shape
extern "C" void fn_8004CBB0(ColliManager_unk* mgr, atn::ColliBase* colli); // inferred: removes a shape

// inferred: debug drawing (atn::DrawHelper, lbl_8087EEB0)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063D3C(void* draw, const atn::Vector3* pos, unsigned int color, float radius, float arg);
extern "C" void fn_8006471C(void* draw, const atn::Vector3* a, const atn::Vector3* b, unsigned int color, float radius,
                            float arg);
extern "C" void fn_800629F0(void* draw, const atn::Vector3* center, const void* box, unsigned int color,
                            const atn::Matrix3x4* mtx, float arg); // inferred: draws a box
// inferred: queue the drawing of a collision object/tree (DrawHelper::ComDrawColliObject/Tree)
extern "C" void fn_80062908(void* draw, atn::ColliObject* colli, int a, int b, int c, int d, float arg);
extern "C" void fn_8006297C(void* draw, atn::ColliTree* colli, int a, int b, int c, int d, float arg);

// inferred: shape tests (atn/ColliTest_unk.cpp); nonzero on a hit, filling the result
extern "C" int fn_800523A0(atn::ColliResult_unk* r, const atn::ColliSphereShape_unk* s,
                           const atn::ColliSphereShape_unk* sphere);
extern "C" int fn_8005361C(atn::ColliResult_unk* r, const atn::ColliCapsuleShape_unk* c,
                           const atn::ColliSphereShape_unk* sphere);
extern "C" int fn_800551BC(atn::ColliResult_unk* r, const void* box, const atn::ColliSphereShape_unk* sphere);
extern "C" int fn_8005256C(atn::ColliResult_unk* r, const atn::ColliSphereShape_unk* s,
                           const atn::ColliCapsuleShape_unk* capsule);
extern "C" int fn_80053BD0(atn::ColliResult_unk* r, const atn::ColliCapsuleShape_unk* c,
                           const atn::ColliCapsuleShape_unk* capsule);
extern "C" int fn_80055810(atn::ColliResult_unk* r, const void* box, const atn::ColliCapsuleShape_unk* capsule);
// inferred: bounds tests against a box (no result)
extern "C" int fn_80051CD8(const atn::ColliSphereShape_unk* s, const atn::ColliBoxShape_unk* box);
extern "C" int fn_80051E90(const atn::ColliCapsuleShape_unk* c, const atn::ColliBoxShape_unk* box);
extern "C" int fn_800520F0(const void* b, const atn::ColliBoxShape_unk* box);
// inferred: tests against one triangle
extern "C" int fn_80053254(atn::ColliResult_unk* r, const atn::ColliSphereShape_unk* s,
                           const atn::ColliTriangle_unk* tri);
extern "C" int fn_80054038(atn::ColliResult_unk* r, const atn::ColliCapsuleShape_unk* c,
                           const atn::ColliTriangle_unk* tri);
extern "C" int fn_8005691C(atn::ColliResult_unk* r, const void* b, const atn::ColliTriangle_unk* tri);
// inferred: bounding sphere tests (no result)
extern "C" int fn_80051A88(const atn::ColliSphereShape_unk* s, const atn::ColliSphereShape_unk* bound);
extern "C" int fn_80051B70(const atn::ColliCapsuleShape_unk* c, const atn::ColliSphereShape_unk* bound);
extern "C" int fn_80051C10(const void* b, const atn::ColliSphereShape_unk* bound);
// inferred: triangle walkers of the loaded meshes (atn/ColliCalc_unk.cpp); return the number of hits
extern "C" int fn_80059B5C(atn::ColliResult_unk* r, int max, const atn::ColliSphereShape_unk* s, void* object,
                           const atn::Matrix3x4* mtx, unsigned int mask, int arg);
extern "C" int fn_8005A084(atn::ColliResult_unk* r, int max, const atn::ColliCapsuleShape_unk* c, void* object,
                           const atn::Matrix3x4* mtx, unsigned int mask, int arg);
extern "C" int fn_8005A5AC(atn::ColliResult_unk* r, int max, const void* b, void* object, const atn::Matrix3x4* mtx,
                           unsigned int mask, int arg);
extern "C" int fn_8005AAD4(atn::ColliResult_unk* r, int max, const atn::ColliSphereShape_unk* s, void* tree);
extern "C" int fn_8005AD74(atn::ColliResult_unk* r, int max, const atn::ColliCapsuleShape_unk* c, void* tree);
extern "C" int fn_8005B05C(atn::ColliResult_unk* r, int max, const void* b, void* tree);

// inferred: bounding sphere of a loaded collision object (LH_ColliObject)
extern "C" atn::ColliSphereShape_unk* fn_80472C90(atn::LH_ColliObject* loader);

// inferred: engine view (lbl_8087EFB4) with its frustum at 0x204
struct ColliView_unk {
    unsigned char m000[0x204];
    Frustum_unk mFrustum; // 0x204
};
extern "C" ColliView_unk* lbl_8087EFB4;

// inferred: write gather pipe of the GX FIFO (as in atn/LensFlare.cpp)
union ColliWGPipe_unk {
    float f;
    unsigned int u;
};
#define GXWGFifo (*(volatile ColliWGPipe_unk*)0xCC008000)

static inline void GXPosition3f32(float x, float y, float z) {
    GXWGFifo.f = x;
    GXWGFifo.f = y;
    GXWGFifo.f = z;
}
static inline void GXColor1u32(unsigned int c) {
    GXWGFifo.u = c;
}

// inferred: plain vector of the loaded data (copied member by member)
struct ColliVec_unk {
    float x, y, z;
};
// inferred: data vector as an engine vector (name invented)
inline atn::Vector3 __ToVec_unk(const ColliVec_unk& v) {
    atn::Vector3 r;
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    return r;
}
// inferred: triangle list of a loaded collision object node (triangles of 0x44 bytes)
struct ColliObjTriangle_unk {
    float mVertex[9];
    unsigned char m24[0x44 - 0x24];
};
struct ColliObjTriList_unk {
    int m00;
    unsigned int mCount;            // 0x04
    ColliObjTriangle_unk* mTriangles; // 0x08
};
struct ColliObjNode_unk {
    unsigned char m00[0x0C];
    ColliObjTriList_unk* mList; // 0x0C
    unsigned char m10[0x38 - 0x10];
    ColliObjNode_unk* m38;      // inferred: children
    ColliObjNode_unk* m3C;
};
struct ColliObjData_unk {
    unsigned char m00[0x0C];
    ColliObjNode_unk* mRoot; // 0x0C
};

// inferred: loaded collision tree (octree)
struct ColliTreeAttr_unk {
    unsigned char m00[0x08];
    unsigned int mColor; // 0x08
};
struct ColliTreeTriangle_unk {
    ColliTreeAttr_unk* mAttr; // 0x00
    int m04;
    atn::Vector3 mVertex[3];  // 0x08
};
// inferred: sum (name invented)
inline atn::Vector3 __Add_unk(atn::Vector3 a, atn::Vector3 b) {
    atn::Vector3 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}
struct ColliTreeNode_unk {
    int mCount;                        // 0x00
    ColliVec_unk mBox[2];              // 0x04: min, max
    ColliTreeNode_unk* mChildren[8];   // 0x1C
    ColliTreeTriangle_unk** mTriangles; // 0x3C
};
struct ColliTreeData_unk {
    unsigned char m00[0x08];
    ColliVec_unk mOffset;     // 0x08
    ColliTreeNode_unk* mRoot; // 0x14
};

namespace atn {

ColliBase::ColliBase(int type)
    : m04(type), mFlags(0), m0C(0), m10(-1), m14(-1), m18(-1), m1C(0), m20(0), m24(0) {
    memset(m28, 0, sizeof(m28));
    if (lbl_8087EE98 != 0) {
        fn_8004CB70(lbl_8087EE98, this);
    }
}

ColliBase::~ColliBase() {
    if (lbl_8087EE98 != 0) {
        fn_8004CBB0(lbl_8087EE98, this);
    }
}

int ColliBase::fn_80056EB0(const int* cell) const {
    if (m10 == -1 || cell[0] == -1) {
        return 1;
    }
    int size = cell[2] + (m18 - 1 < 0 ? 0 : m18 - 1);
    if (cell[0] - size <= m10 && m10 <= cell[0] + size && cell[1] - size <= m14 && m14 <= cell[1] + size) {
        return 1;
    }
    return 0;
}

int ColliSphere::vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                      int arg) {
    if (max > 0 && fn_800523A0(results, sphere, &mSphere) != 0) {
        results->m34 = &m1C;
        results->m38 = this;
        return 1;
    }
    return 0;
}

int ColliSphere::vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                      int arg) {
    if (max > 0 && fn_8005361C(results, capsule, &mSphere) != 0) {
        results->m34 = &m1C;
        results->m38 = this;
        return 1;
    }
    return 0;
}

int ColliSphere::vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg) {
    if (max > 0 && fn_800551BC(results, box, &mSphere) != 0) {
        results->m34 = &m1C;
        results->m38 = this;
        return 1;
    }
    return 0;
}

void ColliSphere::vf18(int a, int b, int c, int d) {
    fn_80063D3C(lbl_8087EEB0, &mSphere.mCenter, 0xFF00FF00, mSphere.mRadius, 0.0f);
}

void ColliSphere::vf1C(const ColliGrid_unk* grid) {
    SetCell_unk(mSphere, grid);
}

int ColliCapsule::vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                       int arg) {
    if (max > 0) {
        if ((mFlags & 2) == 2 && 3.0f * m54 < sphere->mRadius) {
            return 0;
        }
        ColliCapsuleShape_unk capsule(*(const ColliCapsuleShape_unk*)&m3C);
        if ((mFlags & 0x80000000) == 0x80000000) {
            capsule.SetUpright_unk();
        }
        if (fn_8005256C(results, sphere, &capsule) != 0) {
            results->m34 = &m1C;
            results->m38 = this;
            return 1;
        }
    }
    return 0;
}

int ColliCapsule::vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                       int arg) {
    if (max > 0) {
        if ((mask & 0x800000) == 0x800000 && (mFlags & 0x80000000) == 0x80000000) {
            ColliCapsuleShape_unk upright(*(const ColliCapsuleShape_unk*)&m3C);
            upright.SetUpright_unk();
            if (fn_80053BD0(results, capsule, &upright) != 0) {
                results->m34 = &m1C;
                results->m38 = this;
                return 1;
            }
        } else if (fn_80053BD0(results, capsule, (const ColliCapsuleShape_unk*)&m3C) != 0) {
            results->m34 = &m1C;
            results->m38 = this;
            return 1;
        }
    }
    return 0;
}

int ColliCapsule::vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg) {
    if (max > 0 && fn_80055810(results, box, (const ColliCapsuleShape_unk*)&m3C) != 0) {
        results->m34 = &m1C;
        results->m38 = this;
        return 1;
    }
    return 0;
}

void ColliCapsule::vf18(int a, int b, int c, int d) {
    fn_8006471C(lbl_8087EEB0, &m3C, &m48, 0xFF00FF00, m54, 0.0f);
}

void ColliCapsule::vf1C(const ColliGrid_unk* grid) {
    float size = grid->mCellSize;
    float inv = 1.0f / size;
    Vector3 a = m3C - grid->mOrigin;
    Vector3 b = m48 - grid->mOrigin;
    Vector3 c = (a + b) * 0.5f;
    c *= inv;
    m10 = (int)c.x;
    m14 = (int)c.z;
    m18 = (int)((m54 + (b - a).Length_unk()) / grid->mCellSize) + 1;
}

inline void __MtxRotAxis_unk(Mtx r, char axis, float rad) {
    if (rad != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, axis, rad);
        PSMTXConcat(r, a, t);
        __MtxCopy(r, t);
    }
}

// inferred: m = m * rotation z * y * x (same as the helpers of atn/LH_ColliObject.cpp)
inline void __MtxRotateXYZ_unk(Matrix3x4& m, const Vector3& v) {
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

void ColliBox::fn_800575BC(const Vector3* pos, const Vector3* rot, const Vector3* size) {
    Matrix3x4 m;
    PSMTXTrans(m.m, pos->x, pos->y, pos->z);
    __MtxRotateXYZ_unk(m, *rot);
    m3C.mMtx = m;
    m3C.mSize = *size;
    fn_800577D8();
}

void ColliBox::fn_800577D8() {
    Vector3 v[8];
    v[0].Set_unk(-m3C.mSize.x, -m3C.mSize.y, -m3C.mSize.z);
    v[1].Set_unk(-m3C.mSize.x, m3C.mSize.y, -m3C.mSize.z);
    v[2].Set_unk(m3C.mSize.x, -m3C.mSize.y, -m3C.mSize.z);
    v[3].Set_unk(m3C.mSize.x, m3C.mSize.y, -m3C.mSize.z);
    v[4].Set_unk(-m3C.mSize.x, -m3C.mSize.y, m3C.mSize.z);
    v[5].Set_unk(-m3C.mSize.x, m3C.mSize.y, m3C.mSize.z);
    v[6].Set_unk(m3C.mSize.x, -m3C.mSize.y, m3C.mSize.z);
    v[7].Set_unk(m3C.mSize.x, m3C.mSize.y, m3C.mSize.z);
    for (int i = 0; i < 8; i++) {
        v[i].fn_80011410(m3C.mMtx);
    }
    fn_80057A78(&mTriangles[0], &v[0], &v[1], &v[2]);
    fn_80057A78(&mTriangles[1], &v[3], &v[2], &v[1]);
    fn_80057A78(&mTriangles[2], &v[2], &v[3], &v[6]);
    fn_80057A78(&mTriangles[3], &v[7], &v[6], &v[3]);
    fn_80057A78(&mTriangles[4], &v[1], &v[0], &v[5]);
    fn_80057A78(&mTriangles[5], &v[4], &v[5], &v[0]);
    fn_80057A78(&mTriangles[6], &v[5], &v[4], &v[7]);
    fn_80057A78(&mTriangles[7], &v[6], &v[7], &v[4]);
    fn_80057A78(&mTriangles[8], &v[4], &v[0], &v[6]);
    fn_80057A78(&mTriangles[9], &v[2], &v[6], &v[0]);
    fn_80057A78(&mTriangles[10], &v[7], &v[3], &v[5]);
    fn_80057A78(&mTriangles[11], &v[1], &v[5], &v[3]);
}

void ColliBox::fn_80057A78(ColliTriangle_unk* tri, const Vector3* a, const Vector3* b, const Vector3* c) {
    tri->Vertex_unk(0) = *a;
    tri->Vertex_unk(1) = *b;
    tri->Vertex_unk(2) = *c;
    Vector3 e1 = *b - *a;
    Vector3 e2 = *c - *a;
    Vector3 n;
    PSVECCrossProduct(&e1, &e2, &n);
    tri->Normal_unk() = n;
    PSVECNormalize(&tri->Normal_unk(), &tri->Normal_unk());
    tri->Center_unk().Set_unk(0.5f * (b->x + c->x), 0.5f * (b->y + c->y), 0.5f * (b->z + c->z));
    Vector3 d = *a - tri->Center_unk();
    tri->mRadius = PSVECMag(&d);
}

int ColliBox::vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                   int arg) {
    if (max > 0 && fn_80051CD8(sphere, &m3C) != 0) {
        int n = 0;
        for (int i = 0; i < 12; i++) {
            if (fn_80053254(results, sphere, &mTriangles[i]) != 0) {
                results->m34 = &m1C;
                n++;
                results->m38 = this;
                results++;
                if (n >= max) {
                    break;
                }
            }
        }
        return n;
    }
    return 0;
}

int ColliBox::vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                   int arg) {
    if (max > 0 && fn_80051E90(capsule, &m3C) != 0) {
        int n = 0;
        for (int i = 0; i < 12; i++) {
            if (fn_80054038(results, capsule, &mTriangles[i]) != 0) {
                results->m34 = &m1C;
                n++;
                results->m38 = this;
                results++;
                if (n >= max) {
                    break;
                }
            }
        }
        return n;
    }
    return 0;
}

int ColliBox::vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg) {
    if (max > 0 && fn_800520F0(box, &m3C) != 0) {
        int n = 0;
        for (int i = 0; i < 12; i++) {
            if (fn_8005691C(results, box, &mTriangles[i]) != 0) {
                results->m34 = &m1C;
                n++;
                results->m38 = this;
                results++;
                if (n >= max) {
                    break;
                }
            }
        }
        return n;
    }
    return 0;
}

void ColliBox::vf18(int a, int b, int c, int d) {
    Vector3 zero(0.0f, 0.0f, 0.0f);
    fn_800629F0(lbl_8087EEB0, &zero, &m3C, 0xFF00FF00, &m3C.mMtx, 0.0f);
}

void ColliBox::vf1C(const ColliGrid_unk* grid) {
    SetCell_unk(GetBound_unk(), grid);
}

ColliObject::ColliObject() : ColliBase(3), mObject(0) {
    mMtx.SetIdentity();
}

void ColliObject::fn_80058078(const char* path) {
    mLoader.vf0C(path);
    mObject = 0;
}

int ColliObject::fn_800580BC() {
    if (mObject != 0) {
        return 0;
    }
    if (mLoader.fn_80473F50() != 0) {
        return 1;
    }
    mObject = mLoader.fn_80472A34();
    return 0;
}

int ColliObject::vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                      int arg) {
    if (fn_80051A88(sphere, &mBound) == 0) {
        return 0;
    }
    int n = fn_80059B5C(results, max, sphere, mObject, &mMtx, mask, arg);
    for (int i = 0; i < n; i++) {
        results[i].m38 = this;
    }
    return n;
}

int ColliObject::vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                      int arg) {
    if (fn_80051B70(capsule, &mBound) == 0) {
        return 0;
    }
    int n = fn_8005A084(results, max, capsule, mObject, &mMtx, mask, arg);
    for (int i = 0; i < n; i++) {
        results[i].m38 = this;
    }
    return n;
}

int ColliObject::vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg) {
    if (fn_80051C10(box, &mBound) == 0) {
        return 0;
    }
    int n = fn_8005A5AC(results, max, box, mObject, &mMtx, mask, arg);
    for (int i = 0; i < n; i++) {
        results[i].m38 = this;
    }
    return n;
}

void ColliObject::vf18(int a, int b, int c, int d) {
    if (mObject != 0) {
        fn_80062908(lbl_8087EEB0, this, a, b, c, d, 0.0f);
    }
}

int ColliObject::fn_800584B4(int a, int b, int c) {
    return fn_800584D8(0, 0, 0, ((ColliObjData_unk*)mObject)->mRoot, a, b, c);
}

int ColliObject::fn_800584D8(int a, int b, int n, ColliObjNode_unk* node, int c, int d, int e) {
    if (node->mList != 0 && node->mList->mTriangles != 0) {
        for (unsigned int i = 0; i < node->mList->mCount; i++) {
            const float* t = node->mList->mTriangles[i].mVertex;
            Vector3 v0 = __ToVec_unk(*(const ColliVec_unk*)&t[0]);
            Vector3 v1 = __ToVec_unk(*(const ColliVec_unk*)&t[3]);
            Vector3 v2 = __ToVec_unk(*(const ColliVec_unk*)&t[6]);
            Matrix3x4 m = mMtx;
            Vector3 trans = m.GetTrans_unk();
            m.m[0][3] = 0.0f;
            m.m[1][3] = 0.0f;
            m.m[2][3] = 0.0f;
            v0 = trans + Transform_unk(m, v0);
            v1 = trans + Transform_unk(m, v1);
            v2 = trans + Transform_unk(m, v2);
            GXBegin(0xB0, 0, 4);
            GXPosition3f32(v0.x, v0.y, v0.z);
            GXPosition3f32(v1.x, v1.y, v1.z);
            GXPosition3f32(v2.x, v2.y, v2.z);
            GXPosition3f32(v0.x, v0.y, v0.z);
            n += 3;
        }
    }
    if (node->m3C != 0) {
        n = fn_800584D8(a, b, n, node->m3C, c, d, e);
    }
    if (node->m38 != 0) {
        n = fn_800584D8(a, b, n, node->m38, c, d, e);
    }
    return n;
}

void ColliObject::vf1C(const ColliGrid_unk* grid) {
    SetCell_unk(mBound, grid);
}

// inferred: column of the rotation part (name invented)
inline Vector3 __GetColumn_unk(const Matrix3x4& m, int i) {
    return Vector3(m.m[0][i], m.m[1][i], m.m[2][i]);
}

// inferred: scale of the rotation part (name invented)
inline Vector3 __GetScale_unk(const Matrix3x4& m) {
    return Vector3(__GetColumn_unk(m, 0).Length_unk(), __GetColumn_unk(m, 1).Length_unk(),
                   __GetColumn_unk(m, 2).Length_unk());
}

void ColliObject::fn_800588FC() {
    ColliSphereShape_unk* bound = fn_80472C90(&mLoader);
    if (bound != 0) {
        mBound.mCenter = bound->mCenter;
        mBound.mRadius = bound->mRadius;
        PSMTXMultVec(mMtx.m, &mBound.mCenter, &mBound.mCenter);
        float sz = __GetColumn_unk(mMtx, 2).Length_unk();
        float sy = __GetColumn_unk(mMtx, 1).Length_unk();
        Vector3 scale(__GetColumn_unk(mMtx, 0).Length_unk(), sy, sz);
        mBound.mRadius *= scale.Length_unk();
    } else {
        mBound.mCenter.x = 0.0f;
        mBound.mCenter.y = 0.0f;
        mBound.mCenter.z = 0.0f;
        mBound.mRadius = 0.0f;
    }
}

ColliTree::ColliTree() : ColliBase(4), mTree(0) {
    mFlags |= 4;
    mMtx.SetIdentity();
}

void ColliTree::fn_80058B78(const char* path) {
    mLoader.vf0C(path);
    mTree = 0;
}

int ColliTree::fn_80058BBC() {
    if (mTree != 0) {
        return 0;
    }
    if (mLoader.fn_80473F50() != 0) {
        return 1;
    }
    mTree = mLoader.fn_80472D64();
    return 0;
}

int ColliTree::vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask,
                    int arg) {
    int n = fn_8005AAD4(results, max, sphere, mTree);
    for (int i = 0; i < n; i++) {
        results[i].m38 = this;
    }
    return n;
}

int ColliTree::vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                    int arg) {
    int n = fn_8005AD74(results, max, capsule, mTree);
    for (int i = 0; i < n; i++) {
        results[i].m38 = this;
    }
    return n;
}

int ColliTree::vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg) {
    int n = fn_8005B05C(results, max, box, mTree);
    for (int i = 0; i < n; i++) {
        results[i].m38 = this;
    }
    return n;
}

void ColliTree::fn_80058EE4(ColliTreeBounds_unk* bounds) {
    if (bounds->m00 == 0) {
        bounds->m00 = this;
        ColliTreeData_unk* tree = (ColliTreeData_unk*)mTree;
        bounds->mMin = __ToVec_unk(tree->mRoot->mBox[0]) + __ToVec_unk(tree->mOffset);
        bounds->mMax = __ToVec_unk(((ColliTreeData_unk*)mTree)->mRoot->mBox[1]) +
                       __ToVec_unk(((ColliTreeData_unk*)mTree)->mOffset);
        bounds->mCellSize = 20.0f;
        bounds->mCells = (int)((bounds->mMax.x - bounds->mMin.x) / bounds->mCellSize) + 1;
    } else {
        ColliTreeData_unk* tree = (ColliTreeData_unk*)mTree;
        Vector3 oldMin = bounds->mMin;
        Vector3 oldMax = bounds->mMax;
        Vector3 min = __ToVec_unk(tree->mRoot->mBox[0]) + __ToVec_unk(tree->mOffset);
        Vector3 max = __ToVec_unk(tree->mRoot->mBox[1]) + __ToVec_unk(tree->mOffset);
        __VecMin(&bounds->mMin.x, &oldMin.x, &min.x);
        __VecMax(&bounds->mMax.x, &oldMax.x, &max.x);
        bounds->mCellSize = 20.0f;
        bounds->mCells = (int)((bounds->mMax.x - bounds->mMin.x) / bounds->mCellSize) + 1;
    }
}

void ColliTree::vf18(int a, int b, int c, int d) {
    if (mTree != 0 && ((ColliTreeData_unk*)mTree)->mRoot != 0) {
        fn_8006297C(lbl_8087EEB0, this, a, b, c, d, 1500.0f);
    }
}

int ColliTree::fn_80059240(int a, int b, int c) {
    return fn_80059264(0, 0x30, 0, ((ColliTreeData_unk*)mTree)->mRoot, a, b, c);
}

int ColliTree::fn_80059264(int a, int b, int n, ColliTreeNode_unk* node, int c, int colored, int e) {
    if (fn_800502A8(&lbl_8087EFB4->mFrustum, (const Vector3*)node->mBox) == 0) {
        return n;
    }
    ColliTreeTriangle_unk** tris = node->mTriangles;
    if (tris != 0) {
        unsigned int color = 0xFFFFFFFF;
        for (int i = 0; i < node->mCount; i++) {
            ColliTreeTriangle_unk* t = tris[i];
            if (colored != 0) {
                if (t->mAttr != 0) {
                    color = t->mAttr->mColor;
                } else {
                    color = 0xFFFFFFFF;
                }
            }
            GXBegin(0xB0, 0, 4);
            GXPosition3f32(t->mVertex[0].x, t->mVertex[0].y, t->mVertex[0].z);
            GXColor1u32((color << 8) | (color >> 24));
            GXPosition3f32(t->mVertex[1].x, t->mVertex[1].y, t->mVertex[1].z);
            GXColor1u32((color << 8) | (color >> 24));
            GXPosition3f32(t->mVertex[2].x, t->mVertex[2].y, t->mVertex[2].z);
            GXColor1u32((color << 8) | (color >> 24));
            GXPosition3f32(t->mVertex[0].x, t->mVertex[0].y, t->mVertex[0].z);
            GXColor1u32((color << 8) | (color >> 24));
            n += 3;
        }
    }
    for (int i = 0; i < 8; i++) {
        if (node->mChildren[i] != 0) {
            n = fn_80059264(a, b, n, node->mChildren[i], c, colored, e);
        }
    }
    return n;
}

} // namespace atn
