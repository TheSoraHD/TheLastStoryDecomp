// atn/ColliTest_unk.cpp: the shape tests of the collision engine (file name invented): closest
// points between points, lines and segments, the bounding tests (sphere, box) and the hit tests
// that fill an atn::ColliResult_unk (sphere, capsule, swept sphere against a sphere, a capsule, a
// triangle or a box). The callers are atn/atn_colli.cpp, atn/ColliCalc_unk.cpp and the game.
#include "atn/AABB_unk.h"
#include "atn/ColliBase.h"
#include "math/Vec.h"

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);

// inferred: inline in the original (weak copy 0x8013C3B4, defined out of line in game/CharaTask.cpp)
inline atn::Vector3 atn::Vector3::operator-() const {
    return atn::Vector3(-x, -y, -z);
}

using atn::AABB_unk;
using atn::ColliBoxShape_unk;
using atn::ColliCapsuleShape_unk;
using atn::ColliResult_unk;
using atn::ColliSphereShape_unk;
using atn::ColliTriangle_unk;
using atn::Matrix3x4;
using atn::Vector3;

// inferred: infinite line (a point and a unit direction)
struct ColliLine_unk {
    Vector3 mPos;
    Vector3 mDir;
};

// inferred: segment (two points; the distance tests take the capsule shape and ignore its radius)
struct ColliSegment_unk {
    Vector3 mStart;
    Vector3 mEnd;
};

// inferred: squared distance from a point to a line; out: closest point of the line
extern "C" float fn_80050900(const Vector3* p, const ColliLine_unk* line, Vector3* out) {
    Vector3 d = *p - line->mPos;
    float t = PSVECDotProduct(&line->mDir, &d);
    d -= line->mDir * t;
    float sq = PSVECSquareMag(&d);
    if (out) {
        *out = *p - d;
    }
    return sq;
}

// inferred: squared distance from a point to a segment; out: closest point of the segment
extern "C" float fn_80050A1C(const Vector3* p, const ColliCapsuleShape_unk* seg, Vector3* out) {
    Vector3 dir = seg->mEnd - seg->mStart;
    Vector3 d = *p - seg->mStart;
    if (PSVECDotProduct(&dir, &d) < 0.0f) {
        float sq = PSVECSquareMag(&d);
        if (out) {
            *out = seg->mStart;
        }
        return sq;
    }
    Vector3 back = seg->mStart - seg->mEnd;
    Vector3 e = *p - seg->mEnd;
    PSVECNormalize(&back, &back);
    float t = PSVECDotProduct(&back, &e);
    if (t < 0.0f) {
        float sq = PSVECSquareMag(&e);
        if (out) {
            *out = seg->mEnd;
        }
        return sq;
    }
    Vector3 f = e - back * t;
    float sq = PSVECSquareMag(&f);
    if (out) {
        *out = *p - f;
    }
    return sq;
}

// inferred: squared distance between two lines; out: the closest point of each line
extern "C" float fn_80050C38(const ColliLine_unk* a, const ColliLine_unk* b, Vector3* out) {
    Vector3 d = b->mPos - a->mPos;
    float dd = PSVECDotProduct(&a->mDir, &b->mDir);
    float da = PSVECDotProduct(&d, &a->mDir);
    float db = PSVECDotProduct(&d, &b->mDir);
    float s, t;
    if (1.0f == dd || -1.0f == dd) {
        t = s = 0.0f;
    } else {
        float inv = 1.0f / (1.0f - dd * dd);
        s = inv * (da - dd * db);
        t = inv * (dd * da - db);
    }
    Vector3 pa = a->mPos + a->mDir * s;
    Vector3 pb = b->mPos + b->mDir * t;
    float sq = (pa - pb).LengthSq_unk();
    if (out) {
        out[0] = pa;
        out[1] = pb;
    }
    return sq;
}

// inferred: squared length by reference (takes temporaries; name invented)
inline float SquareMag_unk(const Vector3& v) {
    return PSVECSquareMag(&v);
}

// inferred: dot product of two vectors by reference (takes temporaries; name invented)
inline float Dot_unk(const Vector3& a, const Vector3& b) {
    return PSVECDotProduct(&a, &b);
}

// inferred: squared distance between two segments; out: the closest point of each segment,
// lineOut: the closest points of the two lines
// NONMATCHING: 98.6% - FPR allocation in the both-clipped case (q.z in f6) and r3/r4 of two copies
extern "C" float fn_80050E18(const ColliCapsuleShape_unk* a, const ColliCapsuleShape_unk* b, Vector3* out, Vector3* lineOut) {
    Vector3 da = a->mEnd - a->mStart;
    Vector3 db = b->mEnd - b->mStart;
    float lenA = da.Length_unk();
    float lenB = db.Length_unk();
    da.Normalize_unk();
    db.Normalize_unk();
    Vector3 d = b->mStart - a->mStart;
    float dd = PSVECDotProduct(&da, &db);
    float d1 = PSVECDotProduct(&d, &da);
    float d2 = PSVECDotProduct(&d, &db);
    int clip = 0;
    float s, t;
    if (1.0f == dd) {
        t = s = 0.0f;
        clip = 3;
    } else if (-1.0f == dd) {
        s = 0.0f;
        t = lenB;
        clip = 3;
    } else {
        float inv = 1.0f / (1.0f - dd * dd);
        s = inv * (d1 - dd * d2);
        t = inv * (dd * d1 - d2);
    }
    Vector3 pa = a->mStart + da * s;
    Vector3 pb = b->mStart + db * t;
    if (lineOut) {
        lineOut[0] = pa;
        lineOut[1] = pb;
    }
    if (s < 0.0f) {
        pa = a->mStart;
        clip += 1;
    } else if (s > lenA) {
        pa = a->mEnd;
        clip += 1;
    }
    if (t < 0.0f) {
        pb = b->mStart;
        clip += 2;
    } else if (t > lenB) {
        pb = b->mEnd;
        clip += 2;
    }
    float dist;
    Vector3 diff;
    if (clip == 0) {
        diff = pa - pb;
        dist = PSVECSquareMag(&diff);
    } else if (clip == 1) {
        diff = pa - b->mStart;
        float u = PSVECDotProduct(&db, &diff);
        diff -= db * u;
        pb = pa - diff;
        dist = PSVECSquareMag(&diff);
    } else if (clip == 2) {
        diff = pb - a->mStart;
        float u = PSVECDotProduct(&da, &diff);
        diff -= da * u;
        pa = pb - diff;
        dist = PSVECSquareMag(&diff);
    } else {
        Vector3 e = pa - pb;
        dist = PSVECSquareMag(&e);
        Vector3 f = pa - b->mStart;
        float u = PSVECDotProduct(&db, &f);
        f -= db * u;
        float distB = PSVECSquareMag(&f);
        Vector3 q = pa - f;
        if (Dot_unk(q - b->mStart, q - b->mEnd) > 0.0f) {
            distB = 1e16f;
        }
        Vector3 g = pb - a->mStart;
        float v = PSVECDotProduct(&da, &g);
        g -= da * v;
        float distA = PSVECSquareMag(&g);
        Vector3 r = pb - g;
        if (Dot_unk(r - a->mStart, r - a->mEnd) > 0.0f) {
            distA = 1e16f;
        }
        if (dist <= distB && dist <= distA) {
            dist = dist;
        } else if (distB <= distA) {
            dist = distB;
            pb = q;
        } else {
            dist = distA;
            pa = r;
        }
    }
    if (out) {
        out[0] = pa;
        out[1] = pb;
    }
    return dist;
}

// inferred: on which side of the segments the two points are (bits 1/2: first point past the
// start/end side of segA, 4/8: second point for segB); the points and ends are taken by value
// (name invented)
inline int Side_unk(Vector3 p0, Vector3 p1, Vector3 a0, Vector3 a1, Vector3 b0, Vector3 b1) {
    int flags = 0;
    Vector3 v0 = p0 - a0;
    Vector3 v1 = p0 - a1;
    Vector3 ea = a1 - a0;
    Vector3 v2 = p1 - a0;
    Vector3 v3 = p1 - a1;
    Vector3 eb = b1 - b0;
    float s0 = PSVECDotProduct(&ea, &v0);
    float t0 = PSVECDotProduct(&eb, &v2);
    float s1 = PSVECDotProduct(&ea, &v1);
    float t1 = PSVECDotProduct(&eb, &v3);
    if (s0 * s0 > 0.0f) {
        if (s0 > 0.0f) {
            flags |= 1;
        } else {
            flags |= 2;
        }
    }
    if (t0 * t0 > 0.0f) {
        if (t0 > 0.0f) {
            flags |= 4;
        } else {
            flags |= 8;
        }
    }
    return flags;
}

extern "C" double sqrt(double x);

// inferred: contact of two segments at the given distance (radius); pts: in, the closest points of
// the segments, moved back along lineA; -1 when they are parallel or too far, else the side flags
// NONMATCHING: 90.1% - stack order of the side test locals and of the negated direction
extern "C" int fn_80051684(Vector3* pts, const ColliCapsuleShape_unk* segA, const ColliLine_unk* lineA,
                           const ColliCapsuleShape_unk* segB, const ColliLine_unk* lineB, float radius) {
    Vector3 dirA = lineA->mDir;
    Vector3 dirB = lineB->mDir;
    float c = PSVECDotProduct(&dirA, &dirB);
    if (c < 0.0f) {
        c = -c;
        dirB = -dirB;
    }
    if ((c > 0.0f && atn::Abs_unk(c - 1.0f) < 1e-5f) || (c < 0.0f && atn::Abs_unk(1.0f + c) < 1e-5f)) {
        return -1;
    }
    Vector3 posA = lineA->mPos;
    Vector3 posB = lineB->mPos;
    float dist = fn_80050E18(segA, segB, pts, 0);
    float rr = radius * radius;
    if (dist > rr) {
        return -1;
    }
    float h = sqrt(rr - dist);
    pts[0] -= lineA->mDir * h;
    return Side_unk(pts[0], pts[1], segA->mStart, segA->mEnd, segB->mStart, segB->mEnd);
}

// inferred: the two spheres touch
extern "C" int fn_80051A88(const ColliSphereShape_unk* a, const ColliSphereShape_unk* b) {
    Vector3 d = a->mCenter - b->mCenter;
    return PSVECSquareMag(&d) <= (a->mRadius + b->mRadius) * (a->mRadius + b->mRadius);
}

// inferred: the sphere touches the capsule
extern "C" int fn_80051B14(const ColliSphereShape_unk* s, const ColliCapsuleShape_unk* c) {
    return fn_80050A1C(&s->mCenter, c, 0) <= (s->mRadius + c->mRadius) * (s->mRadius + c->mRadius);
}

// inferred: the segment of the capsule is in the sphere
extern "C" int fn_80051B70(const ColliCapsuleShape_unk* c, const ColliSphereShape_unk* s) {
    return fn_80050A1C(&s->mCenter, c, 0) <= s->mRadius * s->mRadius;
}

// inferred: the segment is in the capsule
extern "C" int fn_80051BC4(const ColliCapsuleShape_unk* seg, const ColliCapsuleShape_unk* c) {
    return fn_80050E18(seg, c, 0, 0) <= c->mRadius * c->mRadius;
}

// inferred: the swept sphere touches the sphere
extern "C" int fn_80051C10(const ColliCapsuleShape_unk* b, const ColliSphereShape_unk* s) {
    return !(fn_80050A1C(&s->mCenter, b, 0) > (b->mRadius + s->mRadius) * (b->mRadius + s->mRadius));
}

// inferred: the two capsules touch
extern "C" int fn_80051C78(const ColliCapsuleShape_unk* a, const ColliCapsuleShape_unk* b) {
    return fn_80050E18(a, b, 0, 0) <= (a->mRadius + b->mRadius) * (a->mRadius + b->mRadius);
}

// inferred: the sphere overlaps the box (name invented)
inline bool Overlap_unk(const AABB_unk& box, const ColliSphereShape_unk& s) {
    if (box.mMin.x > s.mCenter.x + s.mRadius) {
        return false;
    }
    if (box.mMin.y > s.mCenter.y + s.mRadius) {
        return false;
    }
    if (box.mMin.z > s.mCenter.z + s.mRadius) {
        return false;
    }
    if (box.mMax.x < s.mCenter.x - s.mRadius) {
        return false;
    }
    if (box.mMax.y < s.mCenter.y - s.mRadius) {
        return false;
    }
    if (box.mMax.z < s.mCenter.z - s.mRadius) {
        return false;
    }
    return true;
}

// inferred: the sphere touches the oriented box (test in the space of the box)
extern "C" int fn_80051CD8(const ColliSphereShape_unk* s, const ColliBoxShape_unk* box) {
    Matrix3x4 inv;
    AABB_unk aabb;
    ColliSphereShape_unk local;
    local.mRadius = s->mRadius;
    inv = box->mMtx;
    PSMTXInverse(inv.m, inv.m);
    local.mCenter = atn::Transform_unk(inv, s->mCenter);
    aabb.mMin = -box->mSize;
    aabb.mMax = box->mSize;
    return Overlap_unk(aabb, local);
}

// inferred: the segment may overlap the box (bounds of its two points; name invented)
inline bool Overlap_unk(const AABB_unk& box, const ColliSegment_unk& s) {
    if (box.mMin.x > s.mStart.x && box.mMin.x > s.mEnd.x) {
        return false;
    }
    if (box.mMin.y > s.mStart.y && box.mMin.y > s.mEnd.y) {
        return false;
    }
    if (box.mMin.z > s.mStart.z && box.mMin.z > s.mEnd.z) {
        return false;
    }
    if (box.mMax.x < s.mStart.x && box.mMax.x < s.mEnd.x) {
        return false;
    }
    if (box.mMax.y < s.mStart.y && box.mMax.y < s.mEnd.y) {
        return false;
    }
    if (box.mMax.z < s.mStart.z && box.mMax.z < s.mEnd.z) {
        return false;
    }
    return true;
}

// inferred: the capsule may overlap the box (bounds of its two spheres; name invented)
inline bool Overlap_unk(const AABB_unk& box, const ColliCapsuleShape_unk& c) {
    if (box.mMin.x > c.mStart.x + c.mRadius && box.mMin.x > c.mEnd.x + c.mRadius) {
        return false;
    }
    if (box.mMin.y > c.mStart.y + c.mRadius && box.mMin.y > c.mEnd.y + c.mRadius) {
        return false;
    }
    if (box.mMin.z > c.mStart.z + c.mRadius && box.mMin.z > c.mEnd.z + c.mRadius) {
        return false;
    }
    if (box.mMax.x < c.mStart.x - c.mRadius && box.mMax.x < c.mEnd.x - c.mRadius) {
        return false;
    }
    if (box.mMax.y < c.mStart.y - c.mRadius && box.mMax.y < c.mEnd.y - c.mRadius) {
        return false;
    }
    if (box.mMax.z < c.mStart.z - c.mRadius && box.mMax.z < c.mEnd.z - c.mRadius) {
        return false;
    }
    return true;
}

// inferred: the segment of the capsule may overlap the oriented box
// NONMATCHING: 99.9% - r4/r5 crossed in the copy of the negated half size (known __VecCopy pattern)
extern "C" int fn_80051E90(const ColliCapsuleShape_unk* c, const ColliBoxShape_unk* box) {
    Matrix3x4 inv0;
    Matrix3x4 inv1;
    ColliSegment_unk seg;
    AABB_unk aabb;
    inv0 = box->mMtx;
    PSMTXInverse(inv0.m, inv0.m);
    seg.mStart = atn::Transform_unk(inv0, c->mStart);
    inv1 = box->mMtx;
    PSMTXInverse(inv1.m, inv1.m);
    seg.mEnd = atn::Transform_unk(inv1, c->mEnd);
    aabb.mMin = -box->mSize;
    aabb.mMax = box->mSize;
    return Overlap_unk(aabb, seg);
}

// inferred: the swept sphere may overlap the oriented box
// NONMATCHING: 99.9% - r4/r5 crossed in the copy of the negated half size (known __VecCopy pattern)
extern "C" int fn_800520F0(const ColliCapsuleShape_unk* b, const ColliBoxShape_unk* box) {
    Matrix3x4 inv0;
    Matrix3x4 inv1;
    ColliCapsuleShape_unk local;
    AABB_unk aabb;
    local.mRadius = b->mRadius;
    inv0 = box->mMtx;
    PSMTXInverse(inv0.m, inv0.m);
    local.mStart = atn::Transform_unk(inv0, b->mStart);
    inv1 = box->mMtx;
    PSMTXInverse(inv1.m, inv1.m);
    local.mEnd = atn::Transform_unk(inv1, b->mEnd);
    aabb.mMin = -box->mSize;
    aabb.mMax = box->mSize;
    return Overlap_unk(aabb, local);
}

// inferred: sphere s against sphere; on a hit fills the result: m04 point of the sphere surface
// towards s, m10 the center of s pushed out, m1C the direction from the sphere to s
extern "C" int fn_800523A0(ColliResult_unk* r, const ColliSphereShape_unk* s, const ColliSphereShape_unk* sphere) {
    if (r) {
        r->m00 = 0;
    }
    Vector3 d = s->mCenter - sphere->mCenter;
    float sq = PSVECSquareMag(&d);
    float rs = s->mRadius;
    float rb = sphere->mRadius;
    if (sq > (rs + rb) * (rs + rb)) {
        return 0;
    }
    if (r) {
        r->m00 = 1;
        if (atn::Abs_unk(sq) < 1.1920929e-7f) {
            d.y = 0.1f;
        }
        Vector3 n = d;
        n.Normalize_unk();
        Vector3 a = n * rb + sphere->mCenter;
        Vector3 b = n * rs + a;
        r->m04 = a;
        r->m10 = b;
        r->m1C = n;
    }
    return 1;
}

// inferred: sphere s against capsule; on a hit: m04 point of the capsule surface towards s, m10
// the center of s pushed out, m1C/m28 the direction from the capsule to s
// NONMATCHING: 99.7% - r3/r4 crossed in the copy n = d (known __VecCopy pattern)
extern "C" int fn_8005256C(ColliResult_unk* r, const ColliSphereShape_unk* s, const ColliCapsuleShape_unk* c) {
    if (r) {
        r->m00 = 0;
    }
    Vector3 p;
    float sq = fn_80050A1C(&s->mCenter, c, &p);
    float rr = s->mRadius + c->mRadius;
    if (sq > rr * rr) {
        return 0;
    }
    if (r) {
        r->m00 = 1;
        Vector3 d = s->mCenter - p;
        Vector3 n;
        if (!(atn::Abs_unk(PSVECSquareMag(&d)) < 1.1920929e-7f)) {
            d.Normalize_unk();
        } else {
            d.Set_unk(0.0f, 0.0f, 1.0f);
        }
        n = d;
        Vector3 a = p + n * c->mRadius;
        r->m04 = a;
        r->m10 = p + n * rr;
        r->m1C = n;
        r->m28 = n;
    }
    return 1;
}

// inferred: sphere against the triangle (v0, v1, v2) of unit normal n, the center being at the
// signed distance dist of its plane. m00 of the result: 1 face, 2 edge, 3 corner
extern "C" int fn_80052760(ColliResult_unk* r, const ColliSphereShape_unk* s, const Vector3* v0, const Vector3* v1,
                           const Vector3* v2, const Vector3* n, float radius, float dist) {
    Vector3 q = s->mCenter - *n * dist;
    Vector3 e01 = *v1 - *v0;
    Vector3 e02 = *v2 - *v0;
    Vector3 e12 = *v2 - *v1;
    Vector3 e10 = *v0 - *v1;
    Vector3 e20 = *v0 - *v2;
    Vector3 e21 = *v1 - *v2;
    Vector3 d0 = q - *v0;
    Vector3 d1 = q - *v1;
    Vector3 d2 = q - *v2;
    Vector3 c0;
    Vector3 c1;
    Vector3 c2;
    int outside = -1;
    PSVECCrossProduct(&d0, &d1, &c0);
    PSVECCrossProduct(&d1, &d2, &c1);
    PSVECCrossProduct(&d2, &d0, &c2);
    if (PSVECDotProduct(n, &c0) < 0.0f) {
        outside = 0;
    }
    if (PSVECDotProduct(n, &c1) < 0.0f) {
        outside = 1;
    }
    if (PSVECDotProduct(n, &c2) < 0.0f) {
        outside = 2;
    }
    if (outside < 0) {
        if (r) {
            r->m00 = 1;
            r->m04 = q;
            r->m10 = q + *n * radius;
            r->m1C = *n;
        }
        return 1;
    }
    float rr = radius * radius;
    Vector3 w0 = s->mCenter - *v0;
    Vector3 w1 = s->mCenter - *v1;
    Vector3 w2 = s->mCenter - *v2;
    switch (outside) {
    case 0: {
        Vector3 dir = e01;
        dir.Normalize_unk();
        float t = PSVECDotProduct(&dir, &w0);
        Vector3 perp = w0 - dir * t;
        if (PSVECSquareMag(&perp) > rr) {
            return 0;
        }
        if (t >= 0.0f && PSVECDotProduct(&e10, &w1) >= 0.0f) {
            if (r) {
                r->m00 = 2;
                Vector3 pn = perp;
                pn.Normalize_unk();
                Vector3 hit = s->mCenter - perp;
                Vector3 out = hit + pn * radius;
                r->m04 = hit;
                r->m10 = out;
                r->m1C = pn;
            }
            return 1;
        }
        break;
    }
    case 1: {
        Vector3 dir = e12;
        dir.Normalize_unk();
        float t = PSVECDotProduct(&dir, &w1);
        Vector3 perp = w1 - dir * t;
        if (PSVECSquareMag(&perp) > rr) {
            return 0;
        }
        if (t >= 0.0f && PSVECDotProduct(&e21, &w2) >= 0.0f) {
            if (r) {
                r->m00 = 2;
                Vector3 pn = perp;
                pn.Normalize_unk();
                Vector3 hit = s->mCenter - perp;
                Vector3 out = hit + pn * radius;
                r->m04 = hit;
                r->m10 = out;
                r->m1C = pn;
            }
            return 1;
        }
        break;
    }
    case 2: {
        Vector3 dir = e20;
        dir.Normalize_unk();
        float t = PSVECDotProduct(&dir, &w2);
        Vector3 perp = w2 - dir * t;
        if (PSVECSquareMag(&perp) > rr) {
            return 0;
        }
        if (t >= 0.0f && PSVECDotProduct(&e02, &w0) >= 0.0f) {
            if (r) {
                r->m00 = 2;
                Vector3 pn = perp;
                pn.Normalize_unk();
                Vector3 hit = s->mCenter - perp;
                Vector3 out = hit + pn * radius;
                r->m04 = hit;
                r->m10 = out;
                r->m1C = pn;
            }
            return 1;
        }
        break;
    }
    }
    int corner = 0;
    Vector3 vp;
    Vector3 vd;
    if (outside != 1 && PSVECSquareMag(&w0) <= rr) {
        vp = *v0;
        vd = w0;
        corner = 1;
    }
    if (outside != 2 && PSVECSquareMag(&w1) <= rr) {
        vp = *v1;
        vd = w1;
        corner = 2;
    }
    if (outside != 0 && PSVECSquareMag(&w2) <= rr) {
        vp = *v2;
        vd = w2;
        corner = 3;
    }
    if (corner == 0) {
        return 0;
    }
    if (r) {
        r->m00 = 3;
        Vector3 dn = vd;
        dn.Normalize_unk();
        Vector3 out = vp + dn * radius;
        r->m04 = vp;
        r->m10 = out;
        r->m1C = dn;
    }
    return 1;
}

// inferred: sphere against a triangle
extern "C" int fn_80053254(ColliResult_unk* r, const ColliSphereShape_unk* s, ColliTriangle_unk* tri) {
    if (r) {
        r->m00 = 0;
    }
    float rs = s->mRadius;
    float rt = tri->mRadius;
    Vector3 d = tri->Center_unk() - s->mCenter;
    if (d.LengthSq_unk() > (rs + rt) * (rs + rt)) {
        return 0;
    }
    Vector3* n = &tri->Normal_unk();
    float dist = -PSVECDotProduct(n, &d);
    if (dist * dist > rs * rs) {
        return 0;
    }
    if (fn_80052760(r, s, &tri->Vertex_unk(0), &tri->Vertex_unk(1), &tri->Vertex_unk(2), n, rs, dist)) {
        if (r) {
            r->m28 = tri->Normal_unk();
        }
        return 1;
    }
    return 0;
}

// inferred: sphere against a triangle of a mesh placed by mtx (the bounding radius scaled by scale)
// NONMATCHING: 79.1% - the original realigns the stack to 64 bytes (some local is over-aligned)
extern "C" int fn_8005339C(ColliResult_unk* r, const ColliSphereShape_unk* s, ColliTriangle_unk* tri,
                           const Matrix3x4* mtx, float scale) {
    if (r) {
        r->m00 = 0;
    }
    float rs = s->mRadius;
    Vector3 c = tri->Center_unk();
    PSMTXMultVec(mtx->m, &c, &c);
    float rt = tri->mRadius * scale;
    Vector3 d = c - s->mCenter;
    if (d.LengthSq_unk() > (rs + rt) * (rs + rt)) {
        return 0;
    }
    Matrix3x4 rot = mtx->GetRotation_unk();
    Vector3 n = tri->Normal_unk();
    PSMTXMultVec(rot.m, &n, &n);
    n.Normalize_unk();
    float dist = Dot_unk(n, s->mCenter - c);
    if (dist * dist > rs * rs) {
        return 0;
    }
    Vector3 p0 = tri->Vertex_unk(0);
    Vector3 p1 = tri->Vertex_unk(1);
    Vector3 p2 = tri->Vertex_unk(2);
    PSMTXMultVec(mtx->m, &p0, &p0);
    PSMTXMultVec(mtx->m, &p1, &p1);
    PSMTXMultVec(mtx->m, &p2, &p2);
    if (fn_80052760(r, s, &p0, &p1, &p2, &n, rs, dist)) {
        if (r) {
            r->m28 = n;
        }
        return 1;
    }
    return 0;
}

// inferred: same point (name invented)
inline bool IsSame_unk(const Vector3& a, const Vector3& b) {
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

// inferred: p moved along dir by sqrt(k) when k is positive (name invented)
inline Vector3 Advance_unk(const Vector3& p, const Vector3& dir, const Vector3& c, float rad) {
    float k = rad * rad - SquareMag_unk(p - c);
    if (k <= 0.0f) {
        return p;
    }
    return p + dir * (float)sqrt(k);
}

// inferred: segment (moving point) c against the sphere: m04/m10 the first point of the segment on
// the sphere, m1C the normal there
// NONMATCHING: 99.9% - r3/r4 crossed in the copy of start + (start - end) (known __VecCopy pattern)
extern "C" int fn_8005361C(ColliResult_unk* r, const ColliCapsuleShape_unk* c, const ColliSphereShape_unk* sphere) {
    if (r) {
        r->m00 = 0;
    }
    float rad = sphere->mRadius;
    if ((sphere->mCenter - c->mStart).LengthSq_unk() <= rad * rad) {
        if (r) {
            r->m00 = 1;
            Vector3 p = c->mStart;
            if (IsSame_unk(c->mStart, sphere->mCenter)) {
                p = c->mStart + (c->mStart - c->mEnd);
            }
            r->m1C = (p - sphere->mCenter).GetNormalize();
            r->m10 = r->m04 = sphere->mCenter + r->m1C * rad;
        }
        return 1;
    }
    Vector3 dir = c->mEnd - c->mStart;
    Vector3 toC = sphere->mCenter - c->mStart;
    if (PSVECDotProduct(&dir, &toC) < 0.0f) {
        return 0;
    }
    Vector3 nd;
    nd = c->mStart - c->mEnd;
    nd.Normalize_unk();
    Vector3 w = sphere->mCenter - c->mEnd;
    float t = PSVECDotProduct(&nd, &w);
    Vector3 e;
    if (t < 0.0f) {
        if (PSVECSquareMag(&w) > rad * rad) {
            return 0;
        }
        e = w - nd * t;
        PSVECSquareMag(&e);
    } else {
        e = w - nd * t;
        if (PSVECSquareMag(&e) > rad * rad) {
            return 0;
        }
    }
    if (r) {
        r->m00 = 1;
        Vector3 h = sphere->mCenter - e;
        Vector3 g = Advance_unk(h, nd, sphere->mCenter, rad);
        r->m1C = (g - sphere->mCenter).GetNormalize();
        r->m04 = g;
        r->m10 = g;
    }
    return 1;
}

// inferred: segment (moving point) c against the capsule cap: m04/m10 the contact point of the
// segment, m1C the normal
extern "C" int fn_80053BD0(ColliResult_unk* r, const ColliCapsuleShape_unk* c, const ColliCapsuleShape_unk* cap) {
    Vector3 linePts[2];
    Vector3 pts[2];
    ColliLine_unk la;
    ColliLine_unk lb;
    ColliSphereShape_unk cp;
    ColliSphereShape_unk sp;
    Vector3 axis = cap->mStart - cap->mEnd;
    if (axis.IsZero_unk()) {
        return fn_8005361C(r, c, (const ColliSphereShape_unk*)&cap->mEnd);
    }
    if (r) {
        r->m00 = 0;
    }
    float rad = cap->mRadius;
    float d = fn_80050A1C(&c->mStart, cap, &cp.mCenter);
    if (d < rad * rad) {
        if (r) {
            r->m00 = 1;
            if (d < 1e-7f) {
                r->m1C.Set_unk(0.0f, 0.0f, 1.0f);
            } else {
                r->m1C = (c->mStart - cp.mCenter).GetNormalize();
            }
            r->m10 = r->m04 = c->mStart;
        }
        return 1;
    }
    if (fn_80050E18(c, cap, pts, linePts) > rad * rad) {
        return 0;
    }
    if (r) {
        r->m00 = 1;
        la.mDir = (c->mEnd - c->mStart).GetNormalize();
        lb.mDir = (cap->mEnd - cap->mStart).GetNormalize();
        la.mPos = c->mStart;
        lb.mPos = cap->mStart;
        if (fn_80051684(pts, c, &la, cap, &lb, rad) != -1) {
            r->m04 = pts[0];
            r->m10 = pts[0];
            r->m1C = (pts[0] - pts[1]).GetNormalize();
            return 1;
        }
        if (PSVECDotProduct(&la.mDir, &lb.mDir) > 0.0f) {
            sp.mRadius = cap->mRadius;
            sp.mCenter = cap->mStart;
            if (fn_8005361C(r, c, &sp)) {
                return 1;
            }
            sp.mCenter = cap->mEnd;
            if (fn_8005361C(r, c, &sp)) {
                return 1;
            }
        } else {
            sp.mRadius = cap->mRadius;
            sp.mCenter = cap->mEnd;
            if (fn_8005361C(r, c, &sp)) {
                return 1;
            }
            sp.mCenter = cap->mStart;
            if (fn_8005361C(r, c, &sp)) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

// inferred: the float is an infinity or a NaN (same helper as in game/CharaTask.cpp; name invented)
inline bool IsInfNan_unk(float f) {
    return (*(unsigned int*)&f & 0x7F800000) == 0x7F800000;
}

// inferred: a component of the vector is an infinity or a NaN (name invented)
inline bool IsInfNan_unk(const Vector3& v) {
    return IsInfNan_unk(v.x) || IsInfNan_unk(v.y) || IsInfNan_unk(v.z);
}

// inferred: plane a*x + b*y + c*z + d = 0 (names invented)
struct ColliPlane_unk {
    ColliPlane_unk(const Vector3& n, const Vector3& p)
        : mA(n.x), mB(n.y), mC(n.z), mD(-(p.x * n.x + p.y * n.y + p.z * n.z)) {}
    // inferred: point of the line through p0 and p1 on the plane (infinity when parallel)
    Vector3 Intersect_unk(const Vector3& p0, const Vector3& p1) const {
        Vector3 n(mA, mB, mC);
        float d0 = PSVECDotProduct(&n, &p0);
        float denom = d0 - PSVECDotProduct(&n, &p1);
        if (atn::Abs_unk(denom) < 1.1920929e-7f) {
            unsigned int bits = 0x7F800000;
            float inf = *(float*)&bits;
            return Vector3(inf, inf, inf);
        }
        float inv = 1.0f / denom;
        float dist = mA * p0.x + mB * p0.y + mC * p0.z + mD;
        float t = dist * inv;
        Vector3 d = p1 - p0;
        return d * t + p0;
    }
    float mA;
    float mB;
    float mC;
    float mD;
};

// inferred: segment (moving point) c against a triangle: m04/m10 the point of the plane crossed,
// m1C/m28 the normal
// NONMATCHING: 99.5% - GPR/FPR allocation of seg/dir and of the plane members
extern "C" int fn_80054038(ColliResult_unk* r, const ColliCapsuleShape_unk* c, ColliTriangle_unk* tri) {
    if (r) {
        r->m00 = 0;
    }
    Vector3* n = &tri->Normal_unk();
    const Vector3* end = &c->mEnd;
    float s0 = Dot_unk(*n, c->mStart - tri->Center_unk());
    if (s0 * Dot_unk(*n, *end - tri->Center_unk()) > 0.0f) {
        return 0;
    }
    Vector3 seg = *end - c->mStart;
    Vector3 dir = seg;
    dir.Normalize_unk();
    float rt = tri->mRadius;
    Vector3 w = tri->Center_unk() - c->mStart;
    float t = PSVECDotProduct(&dir, &w);
    Vector3 perp = w - dir * t;
    if (PSVECSquareMag(&perp) > rt * rt) {
        return 0;
    }
    if (PSVECDotProduct(&seg, n) > 0.0f) {
        return 0;
    }
    ColliPlane_unk plane(*n, tri->Center_unk());
    Vector3 hit = plane.Intersect_unk(c->mStart, *end);
    if (IsInfNan_unk(hit)) {
        return 0;
    }
    Vector3 a0 = hit - tri->Vertex_unk(0);
    Vector3 a1 = hit - tri->Vertex_unk(1);
    Vector3 a2 = hit - tri->Vertex_unk(2);
    Vector3 c0;
    Vector3 c1;
    Vector3 c2;
    int outside = -1;
    PSVECCrossProduct(&a0, &a1, &c0);
    PSVECCrossProduct(&a1, &a2, &c1);
    PSVECCrossProduct(&a2, &a0, &c2);
    if (PSVECDotProduct(n, &c0) < 0.0f) {
        outside = 0;
    }
    if (PSVECDotProduct(n, &c1) < 0.0f) {
        outside = 1;
    }
    if (PSVECDotProduct(n, &c2) < 0.0f) {
        outside = 2;
    }
    if (outside >= 0) {
        return 0;
    }
    if (r) {
        r->m00 = 1;
        r->m10 = r->m04 = hit;
        r->m1C = *n;
        r->m28 = tri->Normal_unk();
    }
    return 1;
}

// inferred: swept sphere b against the sphere: m04 the point of the sphere touched, m10 the center
// of b there, m1C the normal
extern "C" int fn_800551BC(ColliResult_unk* r, const ColliCapsuleShape_unk* b, const ColliSphereShape_unk* sphere) {
    if (r) {
        r->m00 = 0;
    }
    Vector3 p = b->mStart;
    float rs = sphere->mRadius;
    float rb = b->mRadius;
    float rad = rs + rb;
    if ((sphere->mCenter - p).LengthSq_unk() <= rad * rad) {
        if (r) {
            r->m00 = 1;
            if (IsSame_unk(p, sphere->mCenter)) {
                p += p - b->mEnd;
            }
            r->m1C = (p - sphere->mCenter).GetNormalize();
            r->m04 = sphere->mCenter + r->m1C * rs;
            r->m10 = sphere->mCenter + r->m1C * rad;
        }
        return 1;
    }
    Vector3 dir = b->mEnd - p;
    Vector3 toC = sphere->mCenter - p;
    if (PSVECDotProduct(&dir, &toC) < 0.0f) {
        return 0;
    }
    Vector3 nd;
    nd = p - b->mEnd;
    nd.Normalize_unk();
    Vector3 w = sphere->mCenter - b->mEnd;
    float t = PSVECDotProduct(&nd, &w);
    Vector3 e;
    if (t < 0.0f) {
        if (PSVECSquareMag(&w) > rad * rad) {
            return 0;
        }
        e = w - nd * t;
        PSVECSquareMag(&e);
    } else {
        e = w - nd * t;
        if (PSVECSquareMag(&e) > rad * rad) {
            return 0;
        }
    }
    if (r) {
        r->m00 = 1;
        Vector3 h = sphere->mCenter - e;
        Vector3 g = Advance_unk(h, nd, sphere->mCenter, rad);
        Vector3 gn = (g - sphere->mCenter).GetNormalize();
        Vector3 q = g - gn * -rb;
        r->m04 = q;
        r->m10 = g;
        r->m1C = gn;
    }
    return 1;
}

// inferred: swept sphere b against the capsule cap: m04 the point of the capsule touched, m10 the
// center of b there, m1C the normal
extern "C" int fn_80055810(ColliResult_unk* r, const ColliCapsuleShape_unk* b, const ColliCapsuleShape_unk* cap) {
    Vector3 linePts[2];
    ColliLine_unk la;
    ColliLine_unk lb;
    Vector3 pts[2];
    ColliSphereShape_unk cp;
    ColliSphereShape_unk sp;
    if (r) {
        r->m00 = 0;
    }
    float rad = b->mRadius + cap->mRadius;
    if (fn_80050E18(b, cap, 0, linePts) > rad * rad) {
        return 0;
    }
    if (r) {
        r->m00 = 1;
        if (fn_80050A1C(&b->mStart, cap, &cp.mCenter) <= rad * rad) {
            r->m00 = 1;
            r->m1C = (b->mStart - cp.mCenter).GetNormalize();
            r->m04 = cp.mCenter + r->m1C * cap->mRadius;
            r->m10 = cp.mCenter + r->m1C * rad;
            return 1;
        }
        la.mDir = (b->mStart - b->mEnd).GetNormalize();
        lb.mDir = (cap->mStart - cap->mEnd).GetNormalize();
        la.mPos = linePts[0];
        lb.mPos = linePts[1];
        if (fn_80051684(pts, b, &la, cap, &lb, rad) != -1) {
            r->m00 = 1;
            r->m10 = pts[0];
            r->m1C = (pts[0] - pts[1]).GetNormalize();
            r->m04 = pts[1] + r->m1C * cap->mRadius;
            return 1;
        }
        sp.mRadius = cap->mRadius;
        sp.mCenter = cap->mStart;
        if (fn_800551BC(r, b, &sp)) {
            return 1;
        }
        sp.mCenter = cap->mEnd;
        return fn_800551BC(r, b, &sp) != 0;
    }
    return 1;
}

// inferred: swept sphere b (from mStart to end along the unit dir, move = end - start) against the
// triangle (v0, v1, v2) of normal n and center c
extern "C" int fn_80055C40(ColliResult_unk* r, const ColliCapsuleShape_unk* b, const Vector3* end,
                           const Vector3* move, const Vector3* dir, const Vector3* v0, const Vector3* v1,
                           const Vector3* v2, const Vector3* n, const Vector3* c, float radius);

// inferred: swept sphere against a triangle
// NONMATCHING: 99.6% - GPR allocation of move/dir (r30/r31 swapped)
extern "C" int fn_8005691C(ColliResult_unk* r, const ColliCapsuleShape_unk* b, ColliTriangle_unk* tri) {
    if (r) {
        r->m00 = 0;
    }
    float rb = b->mRadius;
    float rt = tri->mRadius;
    const Vector3* end = &b->mEnd;
    Vector3* c = &tri->Center_unk();
    Vector3 move = *end - b->mStart;
    Vector3 dir = move;
    dir.Normalize_unk();
    Vector3 w = *c - b->mStart;
    float t = PSVECDotProduct(&dir, &w);
    Vector3 perp = w - dir * t;
    if (PSVECSquareMag(&perp) > (rb + rt) * (rb + rt)) {
        return 0;
    }
    if (fn_80055C40(r, b, end, &move, &dir, &tri->Vertex_unk(0), &tri->Vertex_unk(1), &tri->Vertex_unk(2),
                    &tri->Normal_unk(), c, rb)) {
        if (r) {
            r->m28 = tri->Normal_unk();
        }
        return 1;
    }
    return 0;
}

// inferred: swept sphere against a triangle of a mesh placed by mtx (the bounding radius scaled by
// scale)
// NONMATCHING: 87.2% - z-first order of the vertex copies and GPR allocation of move/dir
extern "C" int fn_80056AE8(ColliResult_unk* r, const ColliCapsuleShape_unk* b, ColliTriangle_unk* tri,
                           const Matrix3x4* mtx, float scale) {
    if (r) {
        r->m00 = 0;
    }
    float rb = b->mRadius;
    const Vector3* end = &b->mEnd;
    Vector3 c = tri->Center_unk();
    PSMTXMultVec(mtx->m, &c, &c);
    Vector3 move = *end - b->mStart;
    float rt = tri->mRadius * scale;
    Vector3 dir = move;
    dir.Normalize_unk();
    Vector3 w = c - b->mStart;
    float t = PSVECDotProduct(&dir, &w);
    Vector3 perp = w - dir * t;
    if (PSVECSquareMag(&perp) > (rb + rt) * (rb + rt)) {
        return 0;
    }
    Matrix3x4 rot = mtx->GetRotation_unk();
    Vector3 n = tri->Normal_unk();
    PSMTXMultVec(rot.m, &n, &n);
    n.Normalize_unk();
    Vector3 p0 = tri->Vertex_unk(0);
    PSMTXMultVec(mtx->m, &p0, &p0);
    Vector3 p1 = tri->Vertex_unk(1);
    PSMTXMultVec(mtx->m, &p1, &p1);
    Vector3 p2 = tri->Vertex_unk(2);
    PSMTXMultVec(mtx->m, &p2, &p2);
    if (fn_80055C40(r, b, end, &move, &dir, &p0, &p1, &p2, &n, &c, rb)) {
        if (r) {
            r->m28 = n;
        }
        return 1;
    }
    return 0;
}

// TODO: fn_8005454C (segment against an oriented box, 0xC70 bytes) and fn_80055C40 (swept sphere
// against a triangle, 0xCDC bytes, uses the int table lbl_80731048) are not written yet.
