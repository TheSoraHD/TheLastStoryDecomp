#include "atn/Memory.h"
#include "atn/GXDecl.h"
#include "math/Vec.h"

// Sorted drawing of the effect primitives: polygon strips (items A, 0x70 bytes) and models (items B,
// 0x5C bytes) are queued by the effect system while it updates, bucket-sorted by view depth and
// drawn afterwards, with a near fade ("Effect" debug menu: FadeNear, FadeNearWidth). Names invented.

extern "C" {
void GXBegin(int type, int fmt, unsigned short count);
void GXSetChanMatColor(int chan, GXColor color);
}

extern "C" void* memset(void* dst, int c, unsigned long n);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);

// inferred: write gather pipe of the GX FIFO (as in atn/LensFlare.cpp)
union EffectWGPipe_unk {
    float f;
    unsigned int u;
};
#define GXWGFifo (*(volatile EffectWGPipe_unk*)0xCC008000)

// inferred: the GX vertex writers of the SDK (static inline there)
static inline void GXPosition3f32(float x, float y, float z) {
    GXWGFifo.f = x;
    GXWGFifo.f = y;
    GXWGFifo.f = z;
}
static inline void GXNormal3f32(float x, float y, float z) {
    GXWGFifo.f = x;
    GXWGFifo.f = y;
    GXWGFifo.f = z;
}
static inline void GXTexCoord2f32(float s, float t) {
    GXWGFifo.f = s;
    GXWGFifo.f = t;
}

// inferred: debug menu entries (see atn/LensFlare.cpp)
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
struct EffectSettings_unk {
    unsigned char m00[0x4C];
    void* m4C; // root debug menu group
};
extern "C" EffectSettings_unk* lbl_8087EFA8;

// inferred: camera of the scene (position at 0x10C)
struct EffectCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" EffectCamera_unk* lbl_8087EFB4;

// inferred: renderer (automatic names)
struct EffectRender_unk;
extern "C" EffectRender_unk* lbl_8087EEE0;
extern "C" void fn_80072914(EffectRender_unk* render, int arg);
extern "C" void fn_800761A8(EffectRender_unk* render);

// inferred: vertex of a strip (position and uv)
struct EffectVertex_unk {
    atn::Vector3 mPos;
    float u, v;
};

// inferred: queued strip (0x70 bytes)
struct EffectStrip_unk {
    EffectStrip_unk();
    float mDepth;               // 0x00
    EffectStrip_unk* mNext;     // 0x04
    unsigned int mFlags;        // 0x08
    void* m0C;
    void* m10;
    EffectVertex_unk* mVerts;   // 0x14
    int mCount;                 // 0x18
    float mColor[4];            // 0x1C
    atn::Vector3 mCenter;       // 0x2C
    float m38;
    atn::Matrix3x4 mMtx;        // 0x3C
    int m6C;
};

// inferred: queued model (0x5C bytes)
// inferred: color of a queued model, cleared by its constructor
struct EffectItemColor_unk {
    EffectItemColor_unk() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
    float r, g, b, a;
};

struct EffectModelItem_unk {
    EffectModelItem_unk();
    float mDepth;                 // 0x00
    EffectModelItem_unk* mNext;   // 0x04
    unsigned int mFlags;          // 0x08
    void* mModel;                 // 0x0C
    atn::Matrix3x4 mMtx;          // 0x10
    EffectItemColor_unk mColor;   // 0x40
    float m50;
    float m54;
    int m58;
};

static int sMaxStrips = 0x800;
static int sMaxModels = 0x400;
static int sMaxBuf = 0x100;
static float sFadeNear = -15.0f;
static float sFadeNearWidth = 30.0f;

static int sMaxVerts = sMaxStrips * 8;
static void** sBuckets;
static float sStripNear;
static float sStripFar;
static EffectStrip_unk* sStrips;
static int sStripCount;
static float sModelNear;
static float sModelFar;
static EffectModelItem_unk* sModels;
static int sModelCount;
static void** sBuf;
static int sBufCount;
static EffectVertex_unk* sVerts;
static int sVertPos;
static int sVertUsed;

// inferred: allocates the queues
extern "C" void fn_80234A8C(int strips, int models, int buf, int verts) {
    sMaxStrips = strips;
    sMaxModels = models;
    sMaxBuf = buf;
    if (verts < 0) {
        sMaxVerts = strips * 8;
    }
    sBuckets = new (7, "", "", 0) void*[0x400];
    sStrips = new (7, "", "", 0) EffectStrip_unk[sMaxStrips];
    sModels = new (7, "", "", 0) EffectModelItem_unk[sMaxModels];
    // inferred: raw storage (EffectVertex_unk has a Vector3, whose empty constructor is not called)
    sVerts = (EffectVertex_unk*)new (7, "", "", 0) unsigned char[sMaxVerts * sizeof(EffectVertex_unk)];
    sBuf = new (7, "", "", 0) void*[sMaxBuf];
    sStripCount = 0;
    sModelCount = 0;
    sVertPos = 0;
    sBufCount = 0;
    if (lbl_8087EFA8 != 0) {
        void* group = fn_8008937C(lbl_8087EFA8->m4C, "Effect");
        fn_8008771C(group, "FadeNear", &sFadeNear, 0, 0, -1000.0f, 0.0f, 1.0f);
        fn_8008771C(group, "FadeNearWidth", &sFadeNearWidth, 0, 0, 1.0f, 10000.0f, 1.0f);
    }
}

EffectStrip_unk::EffectStrip_unk()
    : mDepth(0.0f), mNext(0), mFlags(0), m0C(0), m10(0), mVerts(0), mCount(0), m38(0.0f), m6C(0) {}

EffectModelItem_unk::EffectModelItem_unk() : mDepth(0.0f), mNext(0), mFlags(0), mModel(0), m58(0) {}

// inferred: allocates `count` vertices of the ring buffer (0 if the frame is full)
extern "C" EffectVertex_unk* fn_80234C84(int count) {
    if (sVertPos + count >= sMaxVerts) {
        sVertPos = 0;
    }
    int used = sVertUsed + count;
    if (used >= sMaxVerts) {
        return 0;
    }
    EffectVertex_unk* v = &sVerts[sVertPos];
    sVertPos += count;
    sVertUsed = used;
    return v;
}

// inferred: draws the queued strips (not decompiled yet)
extern "C" void fn_80234CD8();

// NONMATCHING: 99.63% (both copies) - the range and 1 - t use f5/f4 the other way round; tried
// clamp helpers, a local for 1 - t and other forms of the range
// inferred: sorts the items in `buckets` by depth between `near` and `far` (back to front); flag
// 0x10000 forces the last bucket, 0x20000 the first one
template <class T>
void SortByDepth_unk(T** buckets, T* items, unsigned int count, int bucketCount, float far, float near) {
    memset(buckets, 0, bucketCount * sizeof(T*));
    float range = far - near;
    if (0.0f == range) {
        range = 1.0f;
    }
    for (unsigned int i = 0; i < count; i++, items++) {
        int index;
        if (items->mFlags & 0x10000) {
            index = bucketCount - 1;
        } else if (items->mFlags & 0x20000) {
            index = 0;
        } else {
            float t = (items->mDepth - near) / range;
            t = (t < 0.0f) ? 0.0f : ((t > 1.0f) ? 1.0f : t);
            float s = 1.0f - t;
            index = (int)(s * (bucketCount - 3)) + 1;
        }
        items->mNext = buckets[index];
        buckets[index] = items;
    }
}

template void SortByDepth_unk<EffectStrip_unk>(EffectStrip_unk**, EffectStrip_unk*, unsigned int, int, float, float);
template void SortByDepth_unk<EffectModelItem_unk>(EffectModelItem_unk**, EffectModelItem_unk*, unsigned int, int,
                                                   float, float);

// inferred: draws the queued models (not decompiled yet)
extern "C" void fn_80235BF8();

// inferred: functions of the effect system (atn/EffectSystem_unk.cpp, automatic names)
class EffectSystem_unk;
extern "C" void fn_8022D078(EffectSystem_unk* system);
extern "C" void fn_8022D478(EffectSystem_unk* system, void* arg);
extern "C" void fn_802380D4(EffectSystem_unk* system, float dt);

// inferred: the effect system (defined in atn/EffectManager.cpp)
class EffectSystem_unk {
public:
    void fn_8023686C(float rate);
    void fn_802368B0(void* arg);
};

// inferred: update
void EffectSystem_unk::fn_8023686C(float rate) {
    fn_8022D078(this);
    fn_802380D4(this, rate);
}

// inferred: draw: queues the primitives of the effects, then draws them sorted
void EffectSystem_unk::fn_802368B0(void* arg) {
    fn_80072914(lbl_8087EEE0, 0);
    sStripCount = 0;
    sModelCount = 0;
    sVertUsed = 0;
    sStripNear = 999999.0f;
    sStripFar = -999999.0f;
    sModelNear = 999999.0f;
    sModelFar = -999999.0f;
    fn_8022D478(this, arg);
    fn_80234CD8();
    fn_80235BF8();
    fn_80072914(lbl_8087EEE0, 0);
}

// inferred: color from four floats in [0, 1]
inline GXColor ToGXColor_unk(const float* color) {
    GXColor c;
    c.r = (int)(255.0f * color[0]);
    c.g = (int)(255.0f * color[1]);
    c.b = (int)(255.0f * color[2]);
    c.a = (int)(255.0f * color[3]);
    return c;
}

// NONMATCHING: 94.73% - scheduling of the position loads/FIFO stores, the registers that hold the
// addresses of the vector temporaries and the camera copy (r3/r4)
// inferred: draws a strip; with `lit` the normals point halfway between the center and the camera
extern "C" void fn_80236938(int count, EffectVertex_unk* verts, const float* color, const atn::Vector3* center,
                            int lit) {
    atn::Vector3 eye;
    if (lit != 0) {
        eye = lbl_8087EFB4->mPos;
    }
    GXSetChanMatColor(4, ToGXColor_unk(color));
    fn_800761A8(lbl_8087EEE0);
    GXBegin(0x98, 0, count);
    for (int i = 0; i < count; i++) {
        float x = verts[i].mPos.x;
        float y = verts[i].mPos.y;
        float z = verts[i].mPos.z;
        GXPosition3f32(x, y, z);
        if (lit != 0) {
            atn::Vector3 pos(x, y, z);
            atn::Vector3 toCenter;
            toCenter = pos - *center;
            PSVECNormalize(&toCenter, &toCenter);
            atn::Vector3 toEye;
            toEye = eye - pos;
            PSVECNormalize(&toEye, &toEye);
            atn::Vector3 normal;
            normal = toCenter + toEye;
            PSVECNormalize(&normal, &normal);
            GXNormal3f32(normal.x, normal.y, normal.z);
        }
        GXTexCoord2f32(verts[i].u, verts[i].v);
    }
}

// inferred: near fade of an item at depth `depth` (flag 0x800000 makes the fade distance 6 times longer,
// 0x400000 enables it)
inline float GetFadeNear_unk(unsigned int flags) {
    if ((flags & 0x800000) == 0x800000) {
        return 6.0f * sFadeNear;
    }
    return sFadeNear;
}

// NONMATCHING: 99.77% - the original rounds the minimum depth of the vertex loop with frsp (as if
// it went through a double); a double local, a cast and a double min helper did not give it
// inferred: queues a strip of `count` vertices
extern "C" void fn_80236B84(int count, EffectVertex_unk* verts, void* a, void* b, const float* color,
                            unsigned int flags, int useCenter, const atn::Matrix3x4* view, float f, int c) {
    if (sStripCount >= sMaxStrips) {
        return;
    }
    atn::Vector3 viewPos(0.0f, 0.0f, 0.0f);
    float depth = 999999.0f;
    atn::Vector3 center(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < count; i++) {
        center.x += verts[i].mPos.x;
        center.y += verts[i].mPos.y;
        center.z += verts[i].mPos.z;
    }
    center.x /= count;
    center.y /= count;
    center.z /= count;
    if (useCenter != 0) {
        atn::Vector3 p = center;
        PSMTXMultVec(view->m, &p, &p);
        viewPos.Assign_unk(p);
        depth = p.z;
    } else {
        for (int i = 0; i < count; i++) {
            atn::Vector3 p = verts[i].mPos;
            PSMTXMultVec(view->m, &p, &p);
            viewPos.Assign_unk(p);
            if (p.z < depth) {
                depth = p.z;
            }
        }
    }
    float fadeNear = GetFadeNear_unk(flags);
    if ((flags & 0x400000) == 0x400000 && depth >= fadeNear) {
        return;
    }
    EffectStrip_unk* s = &sStrips[sStripCount];
    s->mDepth = depth;
    s->mNext = 0;
    s->m0C = a;
    s->m10 = b;
    s->mVerts = verts;
    s->mFlags = flags;
    s->mCount = count;
    s->mColor[0] = color[0];
    s->mColor[1] = color[1];
    s->mColor[2] = color[2];
    s->mColor[3] = color[3];
    if ((flags & 0x400000) == 0x400000) {
        float alpha = -(depth - fadeNear) / sFadeNearWidth;
        float k = (1.0f < alpha) ? 1.0f : alpha;
        s->mColor[3] = color[3] * k;
    }
    s->mCenter = center;
    s->m38 = f;
    s->mMtx = *view;
    s->m6C = c;
    if (sStripNear > depth) {
        sStripNear = depth;
    }
    if (sStripFar < depth) {
        sStripFar = depth;
    }
    sStripCount++;
}

// inferred: translation of a matrix
inline atn::Vector3 GetTranslation_unk(const atn::Matrix3x4* m) {
    return atn::Vector3(m->m[0][3], m->m[1][3], m->m[2][3]);
}

// NONMATCHING: 99.95% - code identical; the 6.0f constant is at another position in .sdata2 (the
// draw functions, not decompiled yet, use 0.2f and 1/7 before it)
// inferred: queues a model with the matrix `mtx`
extern "C" void fn_80236FFC(const atn::Matrix3x4* mtx, void* model, const float* color, unsigned int flags,
                            const float* uv, const atn::Matrix3x4* view, int arg) {
    if (sModelCount >= sMaxModels) {
        return;
    }
    atn::Vector3 p = GetTranslation_unk(mtx);
    PSMTXMultVec(view->m, &p, &p);
    float depth = p.z;
    float fadeNear = GetFadeNear_unk(flags);
    if ((flags & 0x400000) == 0x400000 && depth >= fadeNear) {
        return;
    }
    EffectModelItem_unk* m = &sModels[sModelCount];
    m->mDepth = depth;
    m->mNext = 0;
    m->mModel = model;
    m->mMtx = *mtx;
    m->mColor.r = color[0];
    m->mColor.g = color[1];
    m->mColor.b = color[2];
    m->mColor.a = color[3];
    if ((flags & 0x400000) == 0x400000) {
        float alpha = -(depth - fadeNear) / sFadeNearWidth;
        float k = (1.0f < alpha) ? 1.0f : alpha;
        m->mColor.a = color[3] * k;
    }
    m->mFlags = flags;
    m->m50 = uv[0];
    m->m54 = uv[1];
    m->m58 = arg;
    if (sModelNear > depth) {
        sModelNear = depth;
    }
    if (sModelFar < depth) {
        sModelFar = depth;
    }
    sModelCount++;
}
