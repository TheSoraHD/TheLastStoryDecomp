// atn/ColliBaseFunc_unk.cpp: collision manager of the engine (file name invented; the singleton is
// lbl_8087EE98) and the base virtuals of atn::ColliBase. The manager keeps every collision shape
// (mAll) and the enabled ones (mActive), a grid built from the collision trees (m9008) and a buffer
// of 0x100 results used by its queries: sphere (fn_8004CCA0), segment (fn_8004ED34) and moving
// sphere (fn_8004D388).
#include "atn/ColliBase.h"
#include "atn/Memory.h"
#include "atn/ColliData_unk.h"
#include "math/Vec.h"
#include "std/new.h"

extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length (PSVECSquareMag)
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);


using atn::ColliBase;
using atn::ColliCapsuleShape_unk;
using atn::ColliGrid_unk;
using atn::ColliResult_unk;
using atn::ColliSphereShape_unk;
using atn::ColliTree;
using atn::ColliTreeBounds_unk;
using atn::Vector3;

// inferred: fixed array of shapes (count + 0x800 pointers)
struct ColliPtrArray_unk {
    ColliPtrArray_unk() : mCount(0) {}
    // inferred: removes the first occurrence, keeping the order
    void Erase_unk(ColliBase* colli) {
        ColliBase** end = &mData[mCount];
        ColliBase** p = mData;
        while (p != end && *p != colli) {
            p++;
        }
        if (p != end) {
            unsigned int i = p - mData;
            for (; i < mCount - 1; i++) {
                mData[i] = mData[i + 1];
            }
            mCount = mCount - 1;
        }
    }

    unsigned int Size_unk() const { return mCount; }
    // inferred: appends (the element is passed by reference)
    void PushBack_unk(ColliBase* const& colli) {
        new (&mData[mCount]) ColliBase*(colli);
        mCount++;
    }

    unsigned int mCount;
    ColliBase* mData[0x800];
};

// inferred: grid cell range of a query (same form as ColliBase::m10-m18)
struct ColliCell_unk {
    ColliCell_unk() : x(-1), z(-1), size(-1) {}
    int x;
    int z;
    int size;
};

// inferred: segment of the segment queries (the query shape of vf10 without a radius)
struct ColliSegment_unk {
    ColliSegment_unk(const Vector3& start, const Vector3& end) : mStart(start), mEnd(end) {}
    Vector3 mStart;
    Vector3 mEnd;
};

// inferred: the collision manager (0x9038 bytes, lbl_8087EE98)
struct ColliManager_unk {
    ColliManager_unk() {
        mBounds.m00 = 0;
        m902C = -1.0f;
        m9030 = 0;
        m9034 = 1;
    }

    ColliPtrArray_unk mAll;    // 0x0000: every shape
    ColliPtrArray_unk mActive; // 0x2004: shapes with flag 1 (rebuilt by fn_8004D124)
    ColliResult_unk mResults[0x100]; // 0x4008
    ColliTreeBounds_unk mBounds; // 0x9008: grid of the collision trees (also read as ColliGrid_unk)
    float m902C;   // inferred: height limit of the segment hits (used when m9030 is set)
    int m9030;
    int m9034;     // inferred: sets flag 1 of the floor hits
};

extern "C" ColliManager_unk* lbl_8087EE98;
ColliManager_unk* lbl_8087EE98;

extern "C" int fn_8004CD5C(ColliManager_unk* mgr, ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere,
                           unsigned int mask, ColliBase* ignore, int attr, const ColliCell_unk* cell);
extern "C" int fn_8004CF3C(ColliManager_unk* mgr, ColliResult_unk* results, int max,
                           const ColliSegment_unk* segment, unsigned int mask, ColliBase* ignore, int attr,
                           const ColliCell_unk* cell);
extern "C" int fn_8004D388(ColliManager_unk* mgr, ColliResult_unk* result, const Vector3* pos, const Vector3* move,
                           unsigned int mask, ColliBase* ignore, int attr, float radius);
extern "C" int fn_8004ED34(ColliManager_unk* mgr, ColliResult_unk* result, const Vector3* from, const Vector3* to,
                           unsigned int mask, ColliBase* ignore, int attr);

// inferred: creates the manager
extern "C" void fn_8004C9B4() {
    if (lbl_8087EE98 == 0) {
        lbl_8087EE98 = new ((MemoryTag)3, "", "", 0) ColliManager_unk;
    }
}

// inferred: adds a shape (0 when full)
extern "C" int fn_8004CB70(ColliManager_unk* mgr, ColliBase* colli) {
    if (mgr->mAll.Size_unk() >= 0x800) {
        return 0;
    }
    mgr->mAll.PushBack_unk(colli);
    return 1;
}

// inferred: removes a shape
extern "C" void fn_8004CBB0(ColliManager_unk* mgr, ColliBase* colli) {
    mgr->mAll.Erase_unk(colli);
    mgr->mActive.Erase_unk(colli);
}

// inferred: sphere query: hits of the enabled shapes in the cells of the sphere
extern "C" int fn_8004CCA0(ColliManager_unk* mgr, ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere,
                           unsigned int mask, ColliBase* ignore, int attr) {
    ColliCell_unk cell;
    const ColliGrid_unk* grid = (const ColliGrid_unk*)&mgr->mBounds;
    float size = grid->mCellSize;
    float inv = 1.0f / size;
    Vector3 c = sphere->mCenter - grid->mOrigin;
    c *= inv;
    cell.x = (int)c.x;
    cell.z = (int)c.z;
    cell.size = (int)(sphere->mRadius / size) + 1;
    return fn_8004CD5C(mgr, results, max, sphere, mask, ignore, attr, &cell);
}

extern "C" int fn_8004CD5C(ColliManager_unk* mgr, ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere,
                           unsigned int mask, ColliBase* ignore, int attr, const ColliCell_unk* cell) {
    unsigned int m = mask & 0xFFFFFF;
    if ((mask & 0x04000000) != 0x04000000) {
        attr = -1;
    }
    int n = 0;
    for (unsigned int i = 0; i < mgr->mActive.mCount; i++) {
        ColliBase* c = mgr->mActive.mData[i];
        if (c == ignore) {
            continue;
        }
        if ((mask & 0x80000000) == 0x80000000 && (c->mFlags & 2) == 2 && (c->mFlags & 0x40000000) != 0x40000000) {
            continue;
        }
        if ((mask & 0x80000000) == 0x80000000 && (c->mFlags & 0x10) == 0x10) {
            continue;
        }
        if ((mask & 0x40000000) == 0x40000000 && (c->mFlags & 2) != 2) {
            continue;
        }
        if ((mask & 0x20000000) == 0x20000000 && (c->mFlags & 4) != 4) {
            continue;
        }
        if ((mask & 0x10000000) == 0x10000000 && (c->mFlags & 0x10) != 0x10) {
            continue;
        }
        if ((mask & 0x08000000) == 0x08000000 && (c->mFlags & 0x20) != 0x20) {
            continue;
        }
        if ((mask & 0x08000000) != 0x08000000 && (c->mFlags & 0x20) == 0x20) {
            continue;
        }
        if ((m & c->m20) != 0) {
            continue;
        }
        if (!c->fn_80056EB0(&cell->x)) {
            continue;
        }
        int k = c->vf0C(results, max - n, sphere, m, attr);
        n += k;
        if (n >= max) {
            return n;
        }
        results += k;
    }
    return n;
}

namespace atn {

int ColliBase::vf0C(ColliResult_unk* results, int max, const ColliSphereShape_unk* sphere, unsigned int mask, int arg) {
    return 0;
}

} // namespace atn

// inferred: segment query (same filter as fn_8004CD5C)
extern "C" int fn_8004CF3C(ColliManager_unk* mgr, ColliResult_unk* results, int max,
                           const ColliSegment_unk* segment, unsigned int mask, ColliBase* ignore, int attr,
                           const ColliCell_unk* cell) {
    unsigned int m = mask & 0xFFFFFF;
    if ((mask & 0x04000000) != 0x04000000) {
        attr = -1;
    }
    int n = 0;
    for (unsigned int i = 0; i < mgr->mActive.mCount; i++) {
        ColliBase* c = mgr->mActive.mData[i];
        if (c == ignore) {
            continue;
        }
        if ((mask & 0x80000000) == 0x80000000 && (c->mFlags & 2) == 2 && (c->mFlags & 0x40000000) != 0x40000000) {
            continue;
        }
        if ((mask & 0x80000000) == 0x80000000 && (c->mFlags & 0x10) == 0x10) {
            continue;
        }
        if ((mask & 0x40000000) == 0x40000000 && (c->mFlags & 2) != 2) {
            continue;
        }
        if ((mask & 0x20000000) == 0x20000000 && (c->mFlags & 4) != 4) {
            continue;
        }
        if ((mask & 0x10000000) == 0x10000000 && (c->mFlags & 0x10) != 0x10) {
            continue;
        }
        if ((mask & 0x08000000) == 0x08000000 && (c->mFlags & 0x20) != 0x20) {
            continue;
        }
        if ((mask & 0x08000000) != 0x08000000 && (c->mFlags & 0x20) == 0x20) {
            continue;
        }
        if ((m & c->m20) != 0) {
            continue;
        }
        if (!c->fn_80056EB0(&cell->x)) {
            continue;
        }
        int k = c->vf10(results, max - n, (const ColliCapsuleShape_unk*)segment, m, attr);
        n += k;
        if (n >= max) {
            return n;
        }
        results += k;
    }
    return n;
}

namespace atn {

int ColliBase::vf10(ColliResult_unk* results, int max, const ColliCapsuleShape_unk* capsule, unsigned int mask,
                    int arg) {
    return 0;
}

int ColliBase::vf14(ColliResult_unk* results, int max, const void* box, unsigned int mask, int arg) {
    return 0;
}

} // namespace atn

// inferred: rebuilds the grid from the collision trees and the list of enabled shapes
extern "C" void fn_8004D124(ColliManager_unk* mgr) {
    mgr->mBounds.m00 = 0;
    for (unsigned int i = 0; i < mgr->mAll.mCount; i++) {
        ColliBase* c = mgr->mAll.mData[i];
        if ((c->mFlags & 1) == 1 && (c->mFlags & 4) == 4) {
            ((ColliTree*)c)->fn_80058EE4(&mgr->mBounds);
        }
    }
    mgr->mActive.mCount = 0;
    for (unsigned int i = 0; i < mgr->mAll.mCount; i++) {
        ColliBase* c = mgr->mAll.mData[i];
        if ((c->mFlags & 1) == 1) {
            mgr->mActive.PushBack_unk(c);
            if (mgr->mBounds.m00 != 0) {
                c->vf1C((const ColliGrid_unk*)&mgr->mBounds);
            } else {
                c->m10 = -1;
                c->m14 = -1;
                c->m18 = -1;
            }
        }
    }
}

namespace atn {

void ColliBase::vf1C(const ColliGrid_unk* grid) {
}

} // namespace atn

// inferred: debug drawing of the enabled shapes (flag 0x20 selects one of the two groups)
extern "C" void fn_8004D258(ColliManager_unk* mgr, int a, int b, int c, int d, int group) {
    for (unsigned int i = 0; i < mgr->mAll.mCount; i++) {
        ColliBase* colli = mgr->mAll.mData[i];
        if ((colli->mFlags & 1) == 1) {
            if (group != 0 && (colli->mFlags & 0x20) != 0x20) {
                continue;
            }
            if (group == 0 && (colli->mFlags & 0x20) == 0x20) {
                continue;
            }
            colli->vf18(a, b, c, d);
        }
    }
}

namespace atn {

void ColliBase::vf18(int a, int b, int c, int d) {
}

} // namespace atn

// inferred: segment query in the grid cells covered by the segment (inlined; same form as
// fn_8004CCA0; name invented)
inline int QuerySegment_unk(ColliManager_unk* mgr, ColliResult_unk* results, int max, const ColliSegment_unk& s,
                            unsigned int mask, ColliBase* ignore, int attr) {
    ColliCell_unk cell;
    const ColliGrid_unk* grid = (const ColliGrid_unk*)&mgr->mBounds;
    Vector3 a = s.mStart - grid->mOrigin;
    Vector3 b = s.mEnd - grid->mOrigin;
    Vector3 c = (b + a) * 0.5f;
    float r = (s.mStart - s.mEnd).Length_unk() * 0.5f;
    float size = grid->mCellSize;
    float inv = 1.0f / size;
    c *= inv;
    cell.x = (int)c.x;
    cell.z = (int)c.z;
    cell.size = (int)(r / size) + 1;
    return fn_8004CF3C(mgr, results, max, &s, mask, ignore, attr, &cell);
}

// inferred: helpers of the queries (names invented)
inline float Dot_unk(const Vector3& a, const Vector3& b) {
    return PSVECDotProduct(&a, &b);
}

inline float Distance_unk(const Vector3& a, const Vector3& b) {
    return (a - b).Length_unk();
}

// inferred: flag of the side of a hit on a shape that is not a tree
inline bool IsSide_unk(const ColliResult_unk* r) {
    return r->m38 != 0 && (r->m38->mFlags & 2) != 2;
}

// inferred: flags of the attribute of a hit
inline unsigned int AttrFlags_unk(const ColliResult_unk* r) {
    return ((const ColliAttr_unk*)r->m34)->mFlags;
}

// inferred: moving sphere query returning only the hit position
extern "C" int fn_8004D314(ColliManager_unk* mgr, Vector3* hit, const Vector3* pos, const Vector3* move,
                           unsigned int mask, ColliBase* ignore, int attr, float radius) {
    ColliResult_unk result;
    if (fn_8004D388(mgr, &result, pos, move, mask, ignore, attr, radius) != 0) {
        if (hit != 0) {
            *hit = result.m10;
        }
        return 1;
    }
    return 0;
}

// inferred: line in the xz plane (point and direction; arguments of fn_80050900/fn_80050C38)
struct ColliLine_unk {
    Vector3 mPoint;
    Vector3 mDir;
};
// inferred: closest points of two lines
struct ColliLinePoints_unk {
    Vector3 mA;
    Vector3 mB;
};
extern "C" float fn_80050900(const Vector3* p, const ColliLine_unk* line, float* t, Vector3* out); // inferred
extern "C" void fn_80050C38(const ColliLine_unk* a, const ColliLine_unk* b, ColliLinePoints_unk* out); // inferred
extern "C" double acos(double x);
extern "C" double cos(double x);
extern "C" double sqrt(double x);

#define COLLI_MAX_UNK(a, b) ((a) < (b) ? (b) : (a))
#define COLLI_MIN_UNK(a, b) ((a) > (b) ? (b) : (a))

// NONMATCHING: 85.80% - borrador: deslizamiento de una esfera por pasos (esquinas entre dos paredes, empuje
// de suelo); orden de bloques, registros y pila sin ajustar
// inferred: moving sphere query: moves the sphere by `move` in steps, sliding along the hit
// triangles; returns nonzero when something was hit
extern "C" int fn_8004D388(ColliManager_unk* mgr, ColliResult_unk* result, const Vector3* pos, const Vector3* move,
                           unsigned int mask, ColliBase* ignore, int attr, float radius) {
    if (radius <= 0.0f) {
        return 0;
    }
    Vector3 mv = *move;
    ColliResult_unk* results = mgr->mResults;
    ColliSphereShape_unk sphere(*pos, radius);
    int hit = 0;
    void* hitAttr = 0;
    Vector3 up(0.0f, 1.0f, 0.0f);
    float moveSq = PSVECSquareMag(&mv);
    unsigned int flags = 0;
    Vector3 dir = moveSq > 0.0001f ? mv.GetNormalize() : up;
    ColliBase* hitObj = 0;
    ColliResult_unk local[8];
    Vector3 special(0.0f, 0.0f, 0.0f);
    if (moveSq > 0.25f * (radius * radius)) {
        ColliResult_unk ray;
        Vector3 to = sphere.mCenter + mv;
        if (fn_8004ED34(mgr, &ray, pos, &to, mask | 0x800000, ignore, attr) != 0) {
            mv = ray.m04 - *pos;
            float len = mv.Length_unk();
            if (len > 0.01f) {
                Vector3 n = mv.GetNormalize();
                Vector3 back(n.x * COLLI_MIN_UNK(0.5f * radius, 0.95f * len), n.y * COLLI_MIN_UNK(0.5f * radius, 0.95f * len),
                             n.z * COLLI_MIN_UNK(0.5f * radius, 0.95f * len));
                mv -= back;
            }
            if (Dot_unk(ray.m28, up) > 0.70710677f) {
                if (ray.m34 != 0) {
                    hitAttr = ray.m34;
                }
                if (IsSide_unk(&ray)) {
                    flags |= 1;
                }
            } else if (Dot_unk(ray.m28, up) < -0.70710677f) {
                if (IsSide_unk(&ray)) {
                    flags |= 0x100;
                }
            } else if (IsSide_unk(&ray)) {
                flags |= 0x20;
            }
            if (ray.m38 != 0 && (ray.m38->mFlags & 2) == 2) {
                flags |= 2;
                hitObj = ray.m38;
            }
            unsigned int fl = 0;
            if (ray.m34 != 0) {
                if ((AttrFlags_unk(&ray) & 0x100) == 0x100) {
                    fl |= 4;
                    if (mgr->m9034 != 0) {
                        fl |= 1;
                    }
                    if (Dot_unk(ray.m28, dir) < -0.8660254f) {
                        fl |= 8;
                    }
                }
                if ((AttrFlags_unk(&ray) & 0x200) == 0x200) {
                    fl |= 0x10;
                }
                if ((AttrFlags_unk(&ray) & 0x1000) == 0x1000) {
                    fl |= 0x40;
                    special = ray.m04;
                }
                if ((AttrFlags_unk(&ray) & 0x2000) == 0x2000) {
                    fl |= 0x80;
                    special = ray.m04;
                }
                if ((AttrFlags_unk(&ray) & 0x4000) == 0x4000) {
                    fl |= 0x200;
                    special = ray.m04;
                }
                if ((AttrFlags_unk(&ray) & 0x8000) == 0x8000) {
                    fl |= 0x400;
                }
                if ((AttrFlags_unk(&ray) & 0x40000) == 0x40000) {
                    fl |= 0x800;
                }
            }
            flags |= fl;
        }
    }
    int maxHits = 4;
    if ((mask & 0x20) == 0x20) {
        maxHits = 8;
    }
    int steps;
    if ((mask & 0x01000000) != 0x01000000) {
        steps = COLLI_MIN_UNK(COLLI_MAX_UNK((int)(mv.Length_unk() / (0.2f * radius)), 1), 8);
    } else {
        steps = 1;
    }
    Vector3 stepMove = mv * (1.0f / (float)steps);
    double floorLimit = -0.8660254f;
    for (int s = 0; s < steps; s++) {
        mv = stepMove;
        int n = 0;
        while (n < maxHits) {
            Vector3 prev = sphere.mCenter;
            sphere.mCenter += mv;
            ColliCell_unk cell;
            const ColliGrid_unk* grid = (const ColliGrid_unk*)&mgr->mBounds;
            float size = grid->mCellSize;
            float inv = 1.0f / size;
            Vector3 c = sphere.mCenter - grid->mOrigin;
            c *= inv;
            cell.x = (int)c.x;
            cell.z = (int)c.z;
            cell.size = (int)(sphere.mRadius / size) + 1;
            int k = fn_8004CD5C(mgr, results, 0x100, &sphere, mask, ignore, attr, &cell);
            if (k == 0) {
                break;
            }
            float best = 1e16f;
            int bi = -1;
            unsigned int prio = 4;
            ColliResult_unk* r = results;
            for (int i = 0; i < k; i++, r++) {
                if ((AttrFlags_unk(r) & 0x400) == 0x400) {
                    flags |= 4;
                    continue;
                }
                if (prio < (unsigned int)r->m00) {
                    continue;
                }
                Vector3 d = prev - r->m04;
                float dist = PSVECSquareMag(&d);
                for (int j = 0; j < n; j++) {
                    if (local[j].m40 == r->m40) {
                        dist += 4.0f * (radius * radius);
                    }
                }
                if (prio > (unsigned int)r->m00 || best > dist) {
                    best = dist;
                    prio = r->m00;
                    bi = i;
                }
            }
            if (bi < 0) {
                break;
            }
            ColliResult_unk* h = &results[bi];
            float cosv = 0.0f;
            float best2 = 1e16f;
            int b2 = -1;
            int corner = 0;
            r = results;
            for (int i = 0; i < k; i++, r++) {
                if ((AttrFlags_unk(r) & 0x400) == 0x400 || i == bi || r->m00 != 1) {
                    continue;
                }
                Vector3 d = prev - r->m04;
                float dist = PSVECSquareMag(&d);
                if (best2 > dist && Dot_unk(r->m28, up) < 0.25881904f) {
                    float cs = Dot_unk(h->m28, r->m28);
                    if (cs > -0.9659258f && cs < 0.9659258f) {
                        best2 = dist;
                        b2 = i;
                        cosv = cs;
                    }
                }
            }
            float hitUp = Dot_unk(h->m28, up);
            local[n] = *h;
            int found = -1;
            for (int j = 0; j < n; j++) {
                if (local[j].m40 == local[n].m40) {
                    found = j;
                    break;
                }
            }
            if (found >= 0) {
                if (results[bi].m00 == 1) {
                    if (hitUp < 0.25881904f && n >= 2) {
                        int w = -1;
                        ColliResult_unk* l = local;
                        for (int j = 0; j < n; j++, l++) {
                            if (l->m00 == 1 && l->m40 != h->m40 && Dot_unk(l->m28, up) < 0.25881904f) {
                                float cs = Dot_unk(h->m28, l->m28);
                                if (cs > -0.9659258f && cs < 0.9659258f) {
                                    cosv = cs;
                                    w = j;
                                    break;
                                }
                            }
                        }
                        if (w >= 0) {
                            b2 = bi == 0;
                            corner = 1;
                            results[b2] = local[w];
                        }
                    }
                } else if ((mask & 0x20) != 0x20) {
                    break;
                }
            }
            hit = 1;
            int slide = 1;
            n++;
            if (b2 >= 0 && results[bi].m00 == 1 && hitUp < 0.25881904f) {
                float half = (float)cos(0.5f * (float)acos(cosv));
                Vector3 bis = results[bi].m28 + results[b2].m28;
                if (PSVECSquareMag(&bis) > 0.00001f) {
                    bis.Normalize_unk();
                    Vector3 hdir(dir.x, 0.0f, dir.z);
                    bool ok = true;
                    if (corner == 0) {
                        if (!(PSVECSquareMag(&hdir) > 0.00001f)) {
                            ok = false;
                        } else {
                            Vector3 hd = hdir.GetNormalize();
                            Vector3 nb(-bis.x, -bis.y, -bis.z);
                            if (!(Dot_unk(hd, nb) >= half)) {
                                ok = false;
                            }
                        }
                    }
                    if (ok) {
                        ColliLine_unk l1;
                        l1.mPoint.Set_unk(results[bi].m04.x, 0.0f, results[bi].m04.z);
                        PSVECCrossProduct(&results[bi].m28, &up, &l1.mDir);
                        ColliLine_unk l2;
                        l2.mPoint.Set_unk(results[b2].m04.x, 0.0f, results[b2].m04.z);
                        PSVECCrossProduct(&results[b2].m28, &up, &l2.mDir);
                        Vector3 q(results[bi].m10.x, 0.0f, results[bi].m10.z);
                        if (fn_80050900(&q, &l2, 0, &q) < sphere.mRadius * sphere.mRadius) {
                            ColliLinePoints_unk pts;
                            fn_80050C38(&l1, &l2, &pts);
                            Vector3 mid = (pts.mA + pts.mB) * 0.5f;
                            Vector3 e1 = l1.mPoint - mid;
                            Vector3 e2 = l2.mPoint - mid;
                            Vector3 sum = e1 + e2;
                            if (PSVECSquareMag(&sum) > 0.00001f && Dot_unk(sum, bis) > 0.0f) {
                                float t = (sphere.mRadius + 0.01f) / half;
                                Vector3 np = bis * t + mid;
                                np.y = sphere.mCenter.y;
                                Vector3 dd = sphere.mCenter - np;
                                if (PSVECSquareMag(&dd) < sphere.mRadius * sphere.mRadius) {
                                    slide = 0;
                                    sphere.mCenter = np;
                                    mv.x = mv.y = mv.z = 0.0f;
                                }
                            }
                        }
                    }
                }
            }
            if (slide != 0) {
                if (hitUp > 0.5f && results[bi].m38 != 0 && (results[bi].m38->mFlags & 2) != 2 &&
                    results[bi].m00 == 1) {
                    float dist = (float)sqrt(best);
                    float f = dist / hitUp;
                    sphere.mCenter.y += radius * f / dist - f + 1e-7f;
                    if (sphere.mCenter.y < results[bi].m10.y + 0.001f) {
                        sphere.mCenter.y = results[bi].m10.y + 0.001f;
                    }
                    mv.x = mv.y = mv.z = 0.0f;
                } else {
                    ColliResult_unk* b = &results[bi];
                    mv = sphere.mCenter - b->m10;
                    sphere.mCenter = b->m10 + b->m1C * 0.001f;
                    mv -= b->m1C * Dot_unk(mv, b->m1C);
                }
            }
            if (hitUp > 0.70710677f) {
                if (results[bi].m34 != 0) {
                    hitAttr = results[bi].m34;
                }
                if (IsSide_unk(&results[bi])) {
                    flags |= 1;
                }
            } else if (hitUp < -0.70710677f) {
                if (IsSide_unk(&results[bi])) {
                    flags |= 0x100;
                }
            } else if (IsSide_unk(&results[bi])) {
                flags |= 0x20;
            }
            if (results[bi].m38 != 0 && (results[bi].m38->mFlags & 2) == 2) {
                flags |= 2;
                hitObj = results[bi].m38;
            }
            unsigned int fl = 0;
            ColliResult_unk* hr = &results[bi];
            if (hr->m34 != 0) {
                if ((AttrFlags_unk(hr) & 0x100) == 0x100) {
                    fl |= 4;
                    if (mgr->m9034 != 0) {
                        fl |= 1;
                    }
                    if (Dot_unk(hr->m28, dir) < floorLimit) {
                        fl |= 8;
                    }
                }
                if ((AttrFlags_unk(hr) & 0x200) == 0x200) {
                    fl |= 0x10;
                }
                if ((AttrFlags_unk(hr) & 0x1000) == 0x1000) {
                    fl |= 0x40;
                    special = hr->m04;
                }
                if ((AttrFlags_unk(hr) & 0x2000) == 0x2000) {
                    fl |= 0x80;
                    special = hr->m04;
                }
                if ((AttrFlags_unk(hr) & 0x4000) == 0x4000) {
                    fl |= 0x200;
                    special = hr->m04;
                }
                if ((AttrFlags_unk(hr) & 0x8000) == 0x8000) {
                    fl |= 0x400;
                }
                if ((AttrFlags_unk(hr) & 0x40000) == 0x40000) {
                    fl |= 0x800;
                }
            }
            flags |= fl;
        }
    }
    int probed = 0;
    Vector3 total = sphere.mCenter - *pos;
    if (PSVECSquareMag(&total) > 0.25f * (radius * radius)) {
        total.Normalize_unk();
        ColliResult_unk ray;
        Vector3 to = sphere.mCenter + total * radius;
        if (fn_8004ED34(mgr, &ray, pos, &to, mask | 0x80000000, ignore, attr) != 0) {
            sphere.mCenter = ray.m04 + ray.m1C * radius;
            if (Dot_unk(ray.m28, up) > 0.70710677f) {
                if (ray.m34 != 0) {
                    hitAttr = ray.m34;
                }
                if (IsSide_unk(&ray)) {
                    flags |= 1;
                }
            } else if (Dot_unk(ray.m28, up) < -0.70710677f) {
                if (IsSide_unk(&ray)) {
                    flags |= 0x100;
                }
            } else if (IsSide_unk(&ray)) {
                flags |= 0x20;
            }
            if (ray.m38 != 0 && (ray.m38->mFlags & 2) == 2) {
                flags |= 2;
                hitObj = ray.m38;
            }
            unsigned int fl = 0;
            if (ray.m34 != 0) {
                if ((AttrFlags_unk(&ray) & 0x100) == 0x100) {
                    fl |= 4;
                    if (mgr->m9034 != 0) {
                        fl |= 1;
                    }
                    if (Dot_unk(ray.m28, dir) < -0.8660254f) {
                        fl |= 8;
                    }
                }
                if ((AttrFlags_unk(&ray) & 0x200) == 0x200) {
                    fl |= 0x10;
                }
                if ((AttrFlags_unk(&ray) & 0x1000) == 0x1000) {
                    fl |= 0x40;
                    special = ray.m04;
                }
                if ((AttrFlags_unk(&ray) & 0x2000) == 0x2000) {
                    fl |= 0x80;
                    special = ray.m04;
                }
                if ((AttrFlags_unk(&ray) & 0x4000) == 0x4000) {
                    fl |= 0x200;
                    special = ray.m04;
                }
                if ((AttrFlags_unk(&ray) & 0x8000) == 0x8000) {
                    fl |= 0x400;
                }
                if ((AttrFlags_unk(&ray) & 0x40000) == 0x40000) {
                    fl |= 0x800;
                }
            }
            flags |= fl;
        }
        probed = 1;
    }
    if (result != 0) {
        result->m10 = sphere.mCenter;
        result->m04 = sphere.mCenter;
        result->m34 = hitAttr;
        result->m3C = flags;
        result->m38 = hitObj;
        result->m44 = special;
        float lim = 10000.0f + (radius * radius + PSVECSquareMag(move));
        if (!((result->m10.x - pos->x) * (result->m10.x - pos->x) < lim &&
              (result->m10.y - pos->y) * (result->m10.y - pos->y) < lim &&
              (result->m10.z - pos->z) * (result->m10.z - pos->z) < lim)) {
            result->m10 = *pos;
            result->m04 = *pos;
            result->m34 = 0;
            result->m3C = 0;
            result->m38 = 0;
            return 0;
        }
    }
    if (result != 0 && hit != 0 && (probed != 0 || (flags & 2) == 2) && (mask & 0x02000000) == 0x02000000 &&
        (mask & 0x01000000) != 0x01000000) {
        mv = sphere.mCenter - *pos;
        if (PSVECSquareMag(&mv) > 0.04f * (radius * radius)) {
            fn_8004D388(mgr, result, pos, &mv, mask | 0x81000000, ignore, attr, radius);
            result->m34 = hitAttr;
            result->m3C = flags;
            result->m38 = hitObj;
            result->m44 = special;
        }
    }
    return hit;
}

// inferred: segment query returning only the hit position
extern "C" int fn_8004ECC0(ColliManager_unk* mgr, Vector3* hit, const Vector3* from, const Vector3* to,
                           unsigned int mask, ColliBase* ignore, int attr) {
    ColliResult_unk result;
    if (fn_8004ED34(mgr, &result, from, to, mask, ignore, attr) != 0) {
        if (hit != 0) {
            *hit = result.m10;
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 93.20% - numeracion de GPR salvados, forma de la referencia a hit->m38 en los flags de lado
// y planificacion del bloque de atributos
// inferred: segment query: nearest hit to the start (long segments are tested by steps of 200)
extern "C" int fn_8004ED34(ColliManager_unk* mgr, ColliResult_unk* result, const Vector3* from, const Vector3* to,
                           unsigned int mask, ColliBase* ignore, int attr) {
    ColliSegment_unk seg(*from, *to);
    Vector3 dir = *to - *from;
    ColliResult_unk* results = mgr->mResults;
    float len2 = PSVECSquareMag(&dir);
    if (len2 < 1e-7f) {
        return 0;
    }
    dir.Normalize_unk();
    while (true) {
        Vector3 d = seg.mEnd - seg.mStart;
        float len = d.Length_unk();
        if (atn::Abs_unk(len) < 1.1920929e-07f) {
            return 0;
        }
        bool split = len > 200.0f;
        if (split) {
            seg.mEnd = seg.mStart + dir * 200.0f;
        }
        int n = QuerySegment_unk(mgr, results, 0x100, seg, mask, ignore, attr);
        if (n != 0) {
            float best = 1e16f;
            int bi = -1;
            ColliResult_unk* r = results;
            for (int i = 0; i < n; i++, r++) {
                if (mgr->m9030 != 0 && r->m10.y > mgr->m902C) {
                    continue;
                }
                float dist = Distance_unk(seg.mStart, r->m04);
                if (best > dist) {
                    best = dist;
                    bi = i;
                }
            }
            if (bi != -1) {
                if (result != 0) {
                    ColliResult_unk* hit = &results[bi];
                    ColliResult_unk& h = *hit;
                    *result = *hit;
                    float dot = Dot_unk(hit->m28, Vector3(0.0f, 1.0f, 0.0f));
                    if (dot > 0.70710677f) {
                        if (hit->m38 != 0 && (h.m38->mFlags & 2) != 2) {
                            result->m3C |= 1;
                        }
                    } else if (dot < -0.70710677f) {
                        if (hit->m38 != 0 && (h.m38->mFlags & 2) != 2) {
                            result->m3C |= 0x100;
                        }
                    } else {
                        if (hit->m38 != 0 && (h.m38->mFlags & 2) != 2) {
                            result->m3C |= 0x20;
                        }
                    }
                    if (h.m38 != 0 && (results[bi].m38->mFlags & 2) == 2) {
                        result->m3C |= 2;
                        result->m38 = h.m38;
                    }
                    ColliAttr_unk* a = (ColliAttr_unk*)hit->m34;
                    int land = mgr->m9034;
                    unsigned int fl = 0;
                    if (a != 0) {
                        if ((a->mFlags & 0x100) == 0x100) {
                            fl |= 4;
                            if (land != 0) {
                                fl |= 1;
                            }
                            if (Dot_unk(hit->m28, dir) < -0.8660254f) {
                                fl |= 8;
                            }
                        }
                        if ((AttrFlags_unk(hit) & 0x200) == 0x200) {
                            fl |= 0x10;
                        }
                        if ((AttrFlags_unk(hit) & 0x1000) == 0x1000) {
                            fl |= 0x40;
                            result->m44 = hit->m04;
                        }
                        if ((AttrFlags_unk(hit) & 0x2000) == 0x2000) {
                            fl |= 0x80;
                            result->m44 = hit->m04;
                        }
                        if ((AttrFlags_unk(hit) & 0x4000) == 0x4000) {
                            fl |= 0x200;
                            result->m44 = hit->m04;
                        }
                        if ((AttrFlags_unk(hit) & 0x8000) == 0x8000) {
                            fl |= 0x400;
                        }
                        if ((AttrFlags_unk(hit) & 0x40000) == 0x40000) {
                            fl |= 0x800;
                        }
                    }
                    result->m3C |= fl;
                    float lim = 10000.0f + len2;
                    if (!((result->m04.x - from->x) * (result->m04.x - from->x) < lim &&
                          (result->m04.y - from->y) * (result->m04.y - from->y) < lim &&
                          (result->m04.z - from->z) * (result->m04.z - from->z) < lim)) {
                        return 0;
                    }
                }
                return 1;
            }
        }
        if (split) {
            seg.mStart = seg.mEnd;
            seg.mEnd = *to;
        } else {
            return 0;
        }
    }
}
