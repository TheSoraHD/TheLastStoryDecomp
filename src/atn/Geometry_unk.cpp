// atn/Geometry_unk.cpp: utilidades de cajas alineadas (AABB): interseccion con rayos, volumen,
// dibujo de depuracion, proyeccion a pantalla y escalado (nombre de fichero inventado).
#include "math/Vec.h"
#include "atn/Sphere.h"
#include "atn/AABB_unk.h"
#include "atn/DrawHelper.h"

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void fn_805F9640(const float a[4][4], const float b[4][4], float ab[4][4]); // inferred: PSMTX44Concat
extern "C" void fn_805F97D0(const float m[4][4], const atn::Vector3* src, atn::Vector3* dst,
                            unsigned long n); // inferred: PSMTX44MultVecArray

// inferred: plane (normal, distance); names invented
struct Plane_unk {
    atn::Vector3 mNormal;
    float mDist;
};

// inferred: ray against box: true when the ray from `origin` along `dir` hits it (t is the
// distance along dir)
// NONMATCHING: 98.09% - FPR allocation of the hoisted constants
extern "C" int fn_8006FAB0(const atn::AABB_unk* box, float* t, const atn::Vector3* origin, const atn::Vector3* dir) {
    Plane_unk planes[6] = {};
    planes[0].mNormal = atn::Vector3(1.0f, 0.0f, 0.0f);
    planes[0].mDist = -box->mMin.x;
    planes[1].mNormal = atn::Vector3(-1.0f, 0.0f, 0.0f);
    planes[1].mDist = box->mMax.x;
    planes[2].mNormal = atn::Vector3(0.0f, 1.0f, 0.0f);
    planes[2].mDist = -box->mMin.y;
    planes[3].mNormal = atn::Vector3(0.0f, -1.0f, 0.0f);
    planes[3].mDist = box->mMax.y;
    planes[4].mNormal = atn::Vector3(0.0f, 0.0f, 1.0f);
    planes[4].mDist = -box->mMin.z;
    planes[5].mNormal = atn::Vector3(0.0f, 0.0f, -1.0f);
    planes[5].mDist = box->mMax.z;
    *t = 0.0f;
    atn::Vector3 point(*origin);
    int hit = 0;
    for (int i = 0; i < 6 && hit == 0; i++) {
        Plane_unk* plane = &planes[i];
        float denom = PSVECDotProduct(&plane->mNormal, dir);
        float dist = plane->mDist + PSVECDotProduct(&plane->mNormal, origin);
        if ((float)__fabs(dist) < 1.1920929e-7f) {
            return 1;
        }
        if ((float)__fabs(denom) >= 1.1920929e-7f) {
            *t = -dist / denom;
            if (*t >= 0.0f) {
                hit = 1;
                point = *dir * *t + *origin;
                Plane_unk* other = planes;
                for (int j = 0; j < 6 && hit != 0; j++) {
                    if (j != i) {
                        hit = 0.00015 + (other->mDist + PSVECDotProduct(&other->mNormal, &point)) >= 0.0;
                    }
                    other++;
                }
            }
        }
    }
    return hit;
}

// inferred: volume of the box
extern "C" float fn_8006FDD8(const atn::AABB_unk* box) {
    float w = box->mMax.x - box->mMin.x;
    float h = box->mMax.y - box->mMin.y;
    float d = box->mMax.z - box->mMin.z;
    return w * h * d;
}

// inferred: debug drawing of the box
extern "C" void fn_8006FE08(const atn::AABB_unk* box, unsigned int color) {
    atn::Vector3 center = (box->mMax + box->mMin) * 0.5f;
    atn::Vector3 half = (box->mMax - box->mMin) * 0.5f;
    lbl_8087EEB0->fn_800629F0(center, half, 0.0f, color, 0);
}

// inferred: camera (view matrix at 0x15C, projection at 0x18C); names invented
struct GeoCamera_unk {
    unsigned char m00[0x15C];
    atn::Matrix3x4 mView;  // 0x15C
    float mProj[4][4];     // 0x18C
};
// inferred: render state (viewport size at 0x3C/0x40, as in atn/LensFlare.cpp)
struct GeoRenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
extern "C" GeoCamera_unk* lbl_8087EFB4;
extern "C" GeoRenderState_unk* lbl_8087EEE0;

// inferred: 4x4 matrix from a 3x4 one (the column 3 is cleared first, unlike the constructor of
// atn::Matrix44_unk; the rows go through temporaries in ascending order: inlined); name invented
inline void ToMatrix44_unk(atn::Matrix44_unk* out, const atn::Matrix3x4& a) {
    out->m[0][3] = 0.0f;
    out->m[1][3] = 0.0f;
    out->m[2][3] = 0.0f;
    out->m[3][0] = 0.0f;
    out->m[3][1] = 0.0f;
    out->m[3][2] = 0.0f;
    out->m[3][3] = 1.0f;
    out->SetRow_unk(0, a.GetRow_unk(0));
    out->SetRow_unk(1, a.GetRow_unk(1));
    out->SetRow_unk(2, a.GetRow_unk(2));
    out->SetTrans_unk(a.GetTrans_unk());
}

// inferred: bounds of the 8 corners of the box transformed by m (inlined in fn_8006FEBC and
// fn_800704DC: its locals are in ascending order on the stack); name invented
inline atn::AABB_unk ProjectBox_unk(const float m[4][4], const atn::AABB_unk* box) {
    // inferred: read in this order (max z first): it decides which values get saved FPRs
    float maxZ = box->mMax.z;
    float maxY = box->mMax.y;
    float maxX = box->mMax.x;
    float minX = box->mMin.x;
    float minY = box->mMin.y;
    float minZ = box->mMin.z;
    atn::Vector3 corners[8];
    corners[0] = atn::Vector3(maxX, maxY, maxZ);
    corners[1] = atn::Vector3(minX, maxY, maxZ);
    corners[2] = atn::Vector3(maxX, minY, maxZ);
    corners[3] = atn::Vector3(minX, minY, maxZ);
    corners[4] = atn::Vector3(maxX, maxY, minZ);
    corners[5] = atn::Vector3(minX, maxY, minZ);
    corners[6] = atn::Vector3(maxX, minY, minZ);
    corners[7] = atn::Vector3(minX, minY, minZ);
    fn_805F97D0(m, corners, corners, 8);
    atn::Vector3 max(corners[0]);
    atn::Vector3 min(corners[0]);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[1].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[2].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[3].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[4].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[5].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[6].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[7].x);
    return atn::AABB_unk(min, max);
}

// inferred: screen rectangle of the box (projects the 8 corners with the camera)
extern "C" void fn_8006FEBC(atn::AABB_unk* screen, const atn::AABB_unk* box) {
    GeoCamera_unk* cam = lbl_8087EFB4;
    atn::Matrix44_unk view;
    ToMatrix44_unk(&view, cam->mView);
    float m[4][4];
    fn_805F9640(cam->mProj, view.m, m);
    atn::AABB_unk b = ProjectBox_unk(m, box);
    float hw = 0.5f * (float)lbl_8087EEE0->mWidth;
    float hh = 0.5f * (float)lbl_8087EEE0->mHeight;
    atn::AABB_unk r;
    r.mMax.x = b.mMax.x * hw + hw;
    r.mMin.x = b.mMin.x * hw + hw;
    r.mMin.y = -b.mMax.y * hh + hh;
    r.mMax.y = -b.mMin.y * hh + hh;
    r.mMax.z = 1.0f + b.mMax.z;
    r.mMin.z = 1.0f + b.mMin.z;
    *screen = r;
}

// inferred: as fn_8006FEBC with the combined matrix of the camera
extern "C" void fn_800704DC(atn::AABB_unk* screen, const atn::AABB_unk* box) {
    atn::AABB_unk b = ProjectBox_unk(lbl_8087EFB4->mProj, box);
    float hw = 0.5f * (float)lbl_8087EEE0->mWidth;
    float hh = 0.5f * (float)lbl_8087EEE0->mHeight;
    screen->mMax.x = b.mMax.x * hw + hw;
    screen->mMin.x = b.mMin.x * hw + hw;
    screen->mMin.y = -b.mMax.y * hh + hh;
    screen->mMax.y = -b.mMin.y * hh + hh;
    screen->mMax.z = 1.0f + b.mMax.z;
    screen->mMin.z = 1.0f + b.mMin.z;
}

// inferred: true when the point is inside the box
extern "C" int fn_800709F4(const atn::AABB_unk* box, const atn::Vector3* p) {
    int ret = 0;
    if (box->mMin.x <= p->x && box->mMax.x >= p->x && box->mMin.y <= p->y && box->mMax.y >= p->y &&
        box->mMin.z <= p->z && box->mMax.z >= p->z) {
        ret = 1;
    }
    return ret;
}

// inferred: true when a corner of `other` is inside `box`
extern "C" int fn_80070A70(const atn::AABB_unk* box, const atn::AABB_unk* other) {
    for (int i = 0; i < 8; i++) {
        atn::Vector3 corner((i & 1) ? other->mMin.x : other->mMax.x, (i & 2) ? other->mMin.y : other->mMax.y,
                            (i & 4) ? other->mMin.z : other->mMax.z);
        if (fn_800709F4(box, &corner)) {
            return 1;
        }
    }
    return 0;
}

// inferred: the box scaled around its center
// NONMATCHING: ~58% - the original does not fuse the scale and the add (separate fmuls/fadds)
extern "C" atn::AABB_unk fn_80070B60(const atn::AABB_unk* box, float scale) {
    atn::Vector3 center = (box->mMax - box->mMin) * 0.5f + box->mMin;
    atn::AABB_unk r;
    r.mMax = box->mMax - center;
    r.mMin = box->mMin - center;
    r.mMax.x = r.mMax.x * scale + center.x;
    r.mMax.y = r.mMax.y * scale + center.y;
    r.mMax.z = r.mMax.z * scale + center.z;
    r.mMin.x = r.mMin.x * scale + center.x;
    r.mMin.y = r.mMin.y * scale + center.y;
    r.mMin.z = r.mMin.z * scale + center.z;
    return r;
}

namespace atn {

AABB_unk AABB_unk::Merge_unk(const AABB_unk& o) const {
    AABB_unk r;
    __VecMin(&r.mMin.x, &mMin.x, &o.mMin.x);
    __VecMax(&r.mMax.x, &mMax.x, &o.mMax.x);
    return r;
}

} // namespace atn

// inferred: vertical cylinder (position, radius, height); names invented
struct Cylinder_unk {
    atn::Vector3 mPos;
    float mRadius;
    float mHeight;
};
// inferred: line segment passed to fn_80050A1C (see game/CharaTask.cpp)
struct Segment_unk {
    Segment_unk() {}
    Segment_unk(const atn::Vector3& start, const atn::Vector3& end) : mStart(start), mEnd(end) {}
    atn::Vector3 mStart;
    atn::Vector3 mEnd;
};
extern "C" float fn_80050A1C(const atn::Vector3* point, const Segment_unk* seg, atn::Vector3* out);

// NONMATCHING: 97.41% - the end point is stored through its own address register in the original
// inferred: cylinder against sphere
extern "C" int fn_80070D04(const Cylinder_unk* cyl, const atn::Sphere* sphere) {
    if (sphere->mCenter.y + sphere->mRadius >= cyl->mPos.y &&
        sphere->mCenter.y - sphere->mRadius <= cyl->mPos.y + cyl->mHeight) {
        Segment_unk seg(cyl->mPos, cyl->mPos + atn::Vector3(0.0f, 1.0f, 0.0f) * cyl->mHeight);
        float d = fn_80050A1C(&sphere->mCenter, &seg, 0);
        float r = cyl->mRadius + sphere->mRadius;
        return d <= r * r;
    }
    return 0;
}
