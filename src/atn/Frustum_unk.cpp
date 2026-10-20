// atn/Frustum_unk.cpp: view frustum of the engine camera (planes, bounds) and the visibility tests of
// spheres, boxes and segments (file name invented).
#include "atn/Camera_unk.h"

extern "C" unsigned int PSMTXInverse(const Mtx src, Mtx inv);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" double tan(double x);

using namespace atn;

// inferred: same helper as in game/CharaTask.cpp
inline float LengthSq_unk(const Vector3& v) {
    return fn_805F9920(&v);
}

// inferred: cross product by value (name invented)
inline Vector3 Cross_unk(const Vector3& a, const Vector3& b) {
    Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}

// inferred: float wrapper of the math library (name invented)
inline float TanF_unk(float x) {
    return tan(x);
}

// inferred: plane through three points (name invented)
inline void SetPlane_unk(Plane_unk& p, const Vector3& a, const Vector3& b, const Vector3& c) {
    Vector3 e1 = b - a;
    Vector3 e2 = c - a;
    Vector3 n = Cross_unk(e1, e2);
    p.mNormal = n.GetNormalized_unk();
    p.mDist = -PSVECDotProduct(&p.mNormal, &a);
}

// inferred: bounds (min, max) of n points
extern "C" void fn_8004F45C(Vector3* bounds, const Vector3* pts, int n) {
    bounds[0] = pts[0];
    bounds[1] = pts[0];
    for (int i = 1; i < n; i++) {
        const Vector3& p = pts[i];
        if (p.x < bounds[0].x) {
            bounds[0].x = p.x;
        } else if (p.x > bounds[1].x) {
            bounds[1].x = p.x;
        }
        if (p.y < bounds[0].y) {
            bounds[0].y = p.y;
        } else if (p.y > bounds[1].y) {
            bounds[1].y = p.y;
        }
        if (p.z < bounds[0].z) {
            bounds[0].z = p.z;
        } else if (p.z > bounds[1].z) {
            bounds[1].z = p.z;
        }
    }
}

extern "C" void fn_8004F50C(Frustum_unk* fr, const Mtx view, float fovy, float aspect, float n, float f) {
    float t = TanF_unk(0.5f * fovy);
    t *= n;
    float r = t * aspect;
    fn_8004F5B8(fr, view, 0, t, -t, -r, r, n, f, f / n);
}

extern "C" void fn_8004F5B8(Frustum_unk* fr, const Mtx view, int ortho, float t, float b, float l, float r, float n,
                            float f, float scale) {
    Matrix3x4 inv(view);
    PSMTXInverse(inv.m, inv.m);
    __MtxCopy(fr->mView.m, view);
    Vector3 eye(0.0f, 0.0f, 0.0f);
    Vector3 pts[8] = {
        Vector3(l, t, -n),
        Vector3(r, t, -n),
        Vector3(r, b, -n),
        Vector3(l, b, -n),
        Vector3(scale * l, scale * t, -f),
        Vector3(scale * r, scale * t, -f),
        Vector3(scale * r, scale * b, -f),
        Vector3(scale * l, scale * b, -f),
    };
    fr->mNear = -n;
    fr->mFar = -f;
    fr->mOrtho = ortho;
    if (ortho) {
        fr->mSides[0].mNormal = Vector3(1.0f, 0.0f, 0.0f);
        fr->mSides[0].mDist = -l;
        fr->mSides[1].mNormal = Vector3(-1.0f, 0.0f, 0.0f);
        fr->mSides[1].mDist = r;
        fr->mSides[2].mNormal = Vector3(0.0f, -1.0f, 0.0f);
        fr->mSides[2].mDist = t;
        fr->mSides[3].mNormal = Vector3(0.0f, 1.0f, 0.0f);
        fr->mSides[3].mDist = -b;
    } else {
        SetPlane_unk(fr->mSides[0], eye, pts[0], pts[3]);
        SetPlane_unk(fr->mSides[1], eye, pts[2], pts[1]);
        SetPlane_unk(fr->mSides[2], eye, pts[1], pts[0]);
        SetPlane_unk(fr->mSides[3], eye, pts[3], pts[2]);
    }
    Vector3* p = pts;
    for (int i = 0; i < 8; i++, p++) {
        PSMTXMultVec(inv.m, p, p);
    }
    PSMTXMultVec(inv.m, &eye, &eye);
    fn_8004F45C(&fr->mMin, pts, 8);
    SetPlane_unk(fr->mPlanes[0], eye, pts[0], pts[3]);
    SetPlane_unk(fr->mPlanes[1], eye, pts[2], pts[1]);
    SetPlane_unk(fr->mPlanes[2], pts[0], pts[2], pts[1]);
    SetPlane_unk(fr->mPlanes[3], pts[4], pts[6], pts[7]);
    SetPlane_unk(fr->mPlanes[4], eye, pts[1], pts[0]);
    SetPlane_unk(fr->mPlanes[5], eye, pts[3], pts[2]);
}

extern "C" int fn_8004FF58(const Frustum_unk* fr, const Sphere* sphere) {
    Vector3 c(sphere->mCenter);
    PSMTXMultVec(fr->mView.m, &c, &c);
    if (c.z - sphere->mRadius > fr->mNear) {
        return 0;
    }
    if (c.z + sphere->mRadius < fr->mFar) {
        return 0;
    }
    if (fr->mOrtho) {
        if (sphere->mRadius + (c.x + fr->mSides[0].mDist) < 0.0f) {
            return 0;
        }
        if (sphere->mRadius + (-c.x + fr->mSides[1].mDist) < 0.0f) {
            return 0;
        }
        if (sphere->mRadius + (c.y + fr->mSides[2].mDist) < 0.0f) {
            return 0;
        }
        if (sphere->mRadius + (-c.y + fr->mSides[3].mDist) < 0.0f) {
            return 0;
        }
    } else {
        if (c.x * fr->mSides[0].mNormal.x + c.z * fr->mSides[0].mNormal.z > sphere->mRadius) {
            return 0;
        }
        if (c.x * fr->mSides[1].mNormal.x + c.z * fr->mSides[1].mNormal.z > sphere->mRadius) {
            return 0;
        }
        if (c.y * fr->mSides[2].mNormal.y + c.z * fr->mSides[2].mNormal.z > sphere->mRadius) {
            return 0;
        }
        if (c.y * fr->mSides[3].mNormal.y + c.z * fr->mSides[3].mNormal.z > sphere->mRadius) {
            return 0;
        }
    }
    return 1;
}

// inferred: the box overlaps the bounds of the frustum (name invented)
inline bool Frustum_unk_Overlaps(const Frustum_unk* f, const Vector3* box) {
    if (box[0].x > f->mMax.x || f->mMin.x > box[1].x || box[0].y > f->mMax.y || f->mMin.y > box[1].y ||
        box[0].z > f->mMax.z || f->mMin.z > box[1].z) {
        return false;
    }
    return true;
}

extern "C" int fn_800500FC(const Frustum_unk* fr, const Vector3* box) {
    int ret = 2;
    if (!Frustum_unk_Overlaps(fr, box)) {
        return 0;
    }
    for (int i = 0; i < 6; i++) {
        int in = 0;
        int out = 0;
        for (int j = 0; j < 8 && (in == 0 || out == 0); j++) {
            Vector3 v((j & 1) ? box[0].x : box[1].x, (j & 2) ? box[0].y : box[1].y, (j & 4) ? box[0].z : box[1].z);
            bool outside = fr->mPlanes[i].mDist + PSVECDotProduct(&fr->mPlanes[i].mNormal, &v) > 0.0f;
            if (outside) {
                out++;
            }
            if (!outside) {
                in++;
            }
        }
        if (in == 0) {
            return 0;
        }
        if (out != 0) {
            ret = 1;
        }
    }
    return ret;
}

extern "C" int fn_800502A8(const Frustum_unk* fr, const Vector3* box) {
    if (!Frustum_unk_Overlaps(fr, box)) {
        return 0;
    }
    for (int i = 0; i < 6; i++) {
        Vector3 v;
        v.x = fr->mPlanes[i].mNormal.x >= 0.0f ? box[0].x : box[1].x;
        v.y = fr->mPlanes[i].mNormal.y >= 0.0f ? box[0].y : box[1].y;
        v.z = fr->mPlanes[i].mNormal.z >= 0.0f ? box[0].z : box[1].z;
        if (fr->mPlanes[i].mDist + PSVECDotProduct(&fr->mPlanes[i].mNormal, &v) > 0.0f) {
            return 0;
        }
    }
    return 1;
}

// inferred: the segment can be inside the bounds of the frustum (name invented)
inline bool Frustum_unk_SegOverlaps(const Frustum_unk* f, const Vector3* seg) {
    if (f->mMin.x > seg[0].x && f->mMin.x > seg[1].x) {
        return false;
    }
    if (f->mMin.y > seg[0].y && f->mMin.y > seg[1].y) {
        return false;
    }
    if (f->mMin.z > seg[0].z && f->mMin.z > seg[1].z) {
        return false;
    }
    if (f->mMax.x < seg[0].x && f->mMax.x < seg[1].x) {
        return false;
    }
    if (f->mMax.y < seg[0].y && f->mMax.y < seg[1].y) {
        return false;
    }
    if (f->mMax.z < seg[0].z && f->mMax.z < seg[1].z) {
        return false;
    }
    return true;
}

// inferred: segment (name invented)
struct Segment_unk {
    Vector3 a;
    Vector3 b;
};

// inferred: part of the segment a-b inside the plane (the normals point outwards) (name invented)
inline bool ClipPlane_unk(const Plane_unk& p, const Vector3& a, const Vector3& b, Vector3* out) {
    const Vector3* v[2] = {&a, &b};
    float d[2];
    for (int i = 0; i < 2; i++) {
        d[i] = p.mDist + PSVECDotProduct(v[i], &p.mNormal);
    }
    bool out0 = d[0] > 0.0f;
    bool out1 = d[1] > 0.0f;
    bool res;
    if (out0 && out1) {
        res = false;
        out[0] = *v[0];
        out[1] = *v[1];
    } else if (out0) {
        res = true;
        float t = d[0] / (d[0] - d[1]);
        Vector3 h = *v[0] + (*v[1] - *v[0]) * t;
        out[0] = h;
        out[1] = *v[1];
    } else if (out1) {
        res = true;
        float t = d[0] / (d[0] - d[1]);
        Vector3 h = *v[0] + (*v[1] - *v[0]) * t;
        out[0] = *v[0];
        out[1] = h;
    } else {
        res = true;
        out[0] = *v[0];
        out[1] = *v[1];
    }
    return res;
}

extern "C" int fn_80050420(const Frustum_unk* fr, const Vector3* seg, Vector3* out) {
    out[0] = seg[0];
    out[1] = seg[1];
    if (!Frustum_unk_SegOverlaps(fr, seg)) {
        return 0;
    }
    int ret = 0;
    for (int i = 0; i < 6; i++) {
        Segment_unk s;
        if (ClipPlane_unk(fr->mPlanes[i], seg[0], seg[1], &s.a)) {
            ret = 1;
            if (LengthSq_unk(out[0] - seg[0]) < LengthSq_unk(s.a - seg[0])) {
                out[0] = s.a;
            }
            if (LengthSq_unk(out[1] - seg[1]) < LengthSq_unk(s.b - seg[1])) {
                out[1] = s.b;
            }
        }
    }
    return ret;
}
