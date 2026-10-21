// atn/ColliBaseFunc_unk.cpp: collision manager of the engine (file name invented; the singleton is
// lbl_8087EE98) and the base virtuals of atn::ColliBase. The manager keeps every collision shape
// (mAll) and the enabled ones (mActive), a grid built from the collision trees (m9008) and a buffer
// of 0x100 results used by its queries: sphere (fn_8004CCA0), segment (fn_8004ED34) and moving
// sphere (fn_8004D388).
#include "atn/ColliBase.h"
#include "atn/ColliData_unk.h"
#include "math/Vec.h"
#include "std/new.h"

extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length (PSVECSquareMag)
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);

void* operator new(unsigned long size, int heap, const char* file, const char* name, int line);

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
extern "C" int fn_8004D388(ColliManager_unk* mgr, ColliResult_unk* result, const ColliSphereShape_unk* sphere,
                           const Vector3* move, unsigned int mask, ColliBase* ignore, int attr, float radius);
extern "C" int fn_8004ED34(ColliManager_unk* mgr, ColliResult_unk* result, const Vector3* from, const Vector3* to,
                           unsigned int mask, ColliBase* ignore, int attr);

// inferred: creates the manager
extern "C" void fn_8004C9B4() {
    if (lbl_8087EE98 == 0) {
        lbl_8087EE98 = new (3, "", "", 0) ColliManager_unk;
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

// inferred: moving sphere query returning only the hit position
extern "C" int fn_8004D314(ColliManager_unk* mgr, Vector3* hit, const ColliSphereShape_unk* sphere, const Vector3* move,
                           unsigned int mask, ColliBase* ignore, int attr, float radius) {
    ColliResult_unk result;
    if (fn_8004D388(mgr, &result, sphere, move, mask, ignore, attr, radius) != 0) {
        if (hit != 0) {
            *hit = result.m10;
        }
        return 1;
    }
    return 0;
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

// NONMATCHING: 93.20% - numeracion de GPR salvados, forma de la referencia a hit->m38 en los flags de lado
// y planificacion del bloque de atributos
// inferred: segment query: nearest hit to the start (long segments are tested by steps of 200)
extern "C" int fn_8004ED34(ColliManager_unk* mgr, ColliResult_unk* result, const Vector3* from, const Vector3* to,
                           unsigned int mask, ColliBase* ignore, int attr) {
    ColliSegment_unk seg(*from, *to);
    Vector3 dir = *to - *from;
    ColliResult_unk* results = mgr->mResults;
    float len2 = fn_805F9920(&dir);
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
