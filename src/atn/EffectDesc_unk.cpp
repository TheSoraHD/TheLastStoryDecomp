#include "atn/Memory.h"
#include "atn/GXDecl.h"
#include "atn/Light_unk.h"
#include "atn/Model_unk.h"
#include "math/Vec.h"

// Sorted drawing of the effect primitives: polygon strips (items A, 0x70 bytes) and models (items B,
// 0x5C bytes) are queued by the effect system while it updates, bucket-sorted by view depth and
// drawn afterwards, with a near fade ("Effect" debug menu: FadeNear, FadeNearWidth). Names invented.

extern "C" {
void GXBegin(int type, int fmt, unsigned short count);
void GXSetChanMatColor(int chan, GXColor color);
void GXSetChanAmbColor(int chan, GXColor color);
void GXClearVtxDesc();
void GXSetVtxDesc(int attr, int type);
void GXSetVtxAttrFmt(int fmt, int attr, int cnt, int type, unsigned char frac);
void GXLoadPosMtxImm(const float mtx[][4], unsigned int id);
void GXLoadNrmMtxImm(const float mtx[][4], unsigned int id);
void GXSetCurrentMtx(unsigned int id);
void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void GXSetTevKAlphaSel(int stage, int sel);
void GXLoadLightObjImm(const GXLightObj* obj, int light);
void PSMTXInverse(const float src[][4], float inv[][4]);
void PSMTXTranspose(const float src[][4], float xpose[][4]);
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

// inferred: camera of the scene (fields named by use)
struct EffectCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mPos;     // 0x10C
    unsigned char m118[0x15C - 0x118];
    atn::Matrix3x4 mView;  // 0x15C
    unsigned char m18C[0x2F8 - 0x18C];
    int m2F8;              // 0x2F8: inferred: render pass (4 = each item has its own matrix)
    void* m2FC;            // 0x2FC: inferred: lights
    unsigned char m300[0x954 - 0x300];
    int m954;
    unsigned char m958[0x895C - 0x958];
    unsigned int m895C;
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

extern "C" void fn_80236938(int count, EffectVertex_unk* verts, const float* color, const atn::Vector3* center,
                            int lit);

// inferred: functions of the renderer and the camera (automatic names)
extern "C" void fn_80071E04(EffectRender_unk* render);
extern "C" void fn_80076760(EffectRender_unk* render, int state, int value);
extern "C" void fn_800763C0(EffectRender_unk* render, int stage, void* texture);
extern "C" void fn_800C06B0(EffectCamera_unk* camera, void* arg);
extern "C" void fn_800C08B0(EffectCamera_unk* camera);
extern "C" Light_unk* fn_800C2448(void* lights, int index);
// inferred: functions of the models (automatic names)
extern "C" void fn_80094F14(Model_unk* model, float a, float b);
extern "C" void fn_80094D88(Model_unk* model, int blend, int a, int b, int c, int d);

// inferred: blend state of the renderer (bit fields set by fn_80234CD8, names invented)
struct EffectBlend_unk {
    unsigned int m0 : 1;
    unsigned int m1 : 3;
    unsigned int m4 : 8;
    unsigned int m12 : 1;
    unsigned int m13 : 3;
    unsigned int m16 : 3;
    unsigned int m19 : 1;
    unsigned int m20 : 1;
    unsigned int m21 : 1;
    unsigned int m22 : 3;
    unsigned int m25 : 3;
    unsigned int m28 : 4;
};
extern "C" void fn_80076A28(EffectRender_unk* render, EffectBlend_unk* blend);

// inferred: the virtual functions of the models used here
struct EffectModelVt_unk {
    virtual ~EffectModelVt_unk();
    virtual void vf0C();
    virtual void vf10(EffectCamera_unk* camera);
    virtual void vf14();
    virtual void vf18(EffectCamera_unk* camera);
};

// inferred: name invented
inline bool IsFar_unk(EffectCamera_unk* camera) {
    return camera->m895C >= 0x1000;
}

#define EFFECT_MAX(a, b) ((a) > (b) ? (a) : (b))

// inferred: length of an axis of a matrix
inline float Mag_unk(const atn::Vector3& v) {
    return PSVECMag(&v);
}

// inferred: render settings of an item
inline void SetItemRender_unk(unsigned int flags, void* tex0, void* tex1) {
    EffectRender_unk* render = lbl_8087EEE0;
    fn_80076760(render, 8, !((flags >> 7) & 1));
    fn_80076760(render, 9, 2);
    fn_80076760(render, 10, !((flags >> 3) & 1));
    int blend;
    switch (flags & 0x1F00) {
    case 0x100:
        blend = 1;
        break;
    case 0x200:
        blend = 2;
        break;
    case 0x800:
        blend = 3;
        break;
    case 0x400:
        blend = 4;
        break;
    case 0x1000:
        blend = 5;
        break;
    default:
        blend = 0;
        break;
    }
    fn_80072914(lbl_8087EEE0, blend);
    fn_800763C0(lbl_8087EEE0, 0, tex0);
}

// NONMATCHING: 95.14% - one saved register more than the original; the flag of the camera is
// computed with a branch (the original materialises the unsigned compare with orc/subf), the
// temporaries of the scale are laid out the other way round on the stack, and registers differ
// inferred: draws the queued models, then the queued strips (deferred ones go to sBuf)
extern "C" void fn_80234CD8() {
    if (sStripCount == 0 && sModelCount == 0) {
        return;
    }
    if (sModelCount != 0) {
        SortByDepth_unk((EffectModelItem_unk**)sBuckets, sModels, sModelCount, 0x400, sModelFar, sModelNear);
        fn_80071E04(lbl_8087EEE0);
        int restore = 0;
        if (lbl_8087EFB4->m954 != 0 && !IsFar_unk(lbl_8087EFB4)) {
            restore = 1;
        }
        lbl_8087EFB4->m954 = 0;
        for (int i = 0x3FF; i >= 0; i--) {
            for (EffectModelItem_unk* item = (EffectModelItem_unk*)sBuckets[i]; item != 0; item = item->mNext) {
                Model_unk* model = (Model_unk*)item->mModel;
                if (model == 0) {
                    continue;
                }
                fn_80094F14(model, item->m50, item->m54);
                *(atn::Matrix3x4*)model->mMtx = item->mMtx;
                atn::Vector3 scale(Mag_unk(atn::Vector3(item->mMtx.m[0][0], item->mMtx.m[1][0], item->mMtx.m[2][0])),
                                   Mag_unk(atn::Vector3(item->mMtx.m[0][1], item->mMtx.m[1][1], item->mMtx.m[2][1])),
                                   Mag_unk(atn::Vector3(item->mMtx.m[0][2], item->mMtx.m[1][2], item->mMtx.m[2][2])));
                model->m54 = EFFECT_MAX(scale.x, EFFECT_MAX(scale.y, scale.z));
                fn_800902C0(model, 0, boost::function<void()>());
                model->m40.m00 = item->mColor.r;
                model->m40.m04 = item->mColor.g;
                model->m40.m08 = item->mColor.b;
                model->m40.m0C = item->mColor.a;
                unsigned int flags = item->mFlags;
                int blend;
                switch (flags & 0x1F00) {
                case 0x100:
                    blend = 0xD;
                    break;
                case 0x200:
                    blend = 9;
                    break;
                case 0x800:
                    blend = 0xA;
                    break;
                case 0x400:
                    blend = 0xB;
                    break;
                case 0x1000:
                    blend = 0xC;
                    break;
                default:
                    blend = 3;
                    break;
                }
                fn_80094D88(model, blend, (flags >> 2) & 1, !((flags >> 4) & 1), (flags >> 7) & 1, (flags >> 3) & 1);
                if (lbl_8087EFB4->m2F8 == 4) {
                    model->mFlags |= 0x1400;
                } else {
                    model->mFlags &= ~0x400;
                    model->mFlags &= ~0x1000;
                }
                *(int*)&model->m1DC = item->m58;
                ((EffectModelVt_unk*)model)->vf18(lbl_8087EFB4);
                ((EffectModelVt_unk*)model)->vf10(lbl_8087EFB4);
            }
        }
        lbl_8087EFB4->m954 = restore;
    }
    if (sStripCount == 0) {
        return;
    }
    EffectBlend_unk state;
    state.m0 = 0;
    state.m1 = 7;
    state.m4 = 0;
    state.m12 = 1;
    state.m13 = 4;
    state.m16 = 5;
    state.m19 = 1;
    state.m20 = 0;
    state.m21 = 1;
    state.m22 = 2;
    state.m25 = 7;
    fn_80076A28(lbl_8087EEE0, &state);
    fn_80076760(lbl_8087EEE0, 6, 0);
    EffectRender_unk* render = lbl_8087EEE0;
    fn_80076760(render, 4, 4);
    fn_80076760(render, 5, 5);
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevColorIn(0, 0xF, 8, 0xA, 0xF);
    GXSetTevAlphaIn(0, 7, 4, 5, 7);
    GXSetTevColorOp(0, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
    GXSetTevKColorSel(0, 0xC);
    GXSetTevKAlphaSel(0, 0x1C);
    GXSetTevDirect(0);
    GXSetTevOrder(1, 1, 1, 4);
    GXSetTevColorIn(1, 0xF, 0, 0xC, 0xF);
    GXSetTevAlphaIn(1, 7, 0, 4, 7);
    GXSetTevColorOp(1, 0, 0, 0, 1, 0);
    GXSetTevAlphaOp(1, 0, 0, 0, 1, 0);
    GXSetTevKColorSel(1, 0xC);
    GXSetTevKAlphaSel(1, 0x1C);
    GXSetTevDirect(1);
    GXSetTevSwapModeTable(1, 0, 0, 0, 0);
    GXSetTevSwapMode(1, 0, 1);
    GXColor black = {0, 0, 0, 0};
    GXSetChanAmbColor(4, black);
    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(1, 1, 4, 0x3C, 0, 0x7D);
    atn::Matrix3x4 view = lbl_8087EFB4->mView;
    GXLoadPosMtxImm(view.m, 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    GXLightObj light;
    fn_800C2448(lbl_8087EFB4->m2FC, 0)->fn_80079994(&light, view.m);
    GXLoadLightObjImm(&light, 1);
    atn::Matrix3x4 inv = view;
    PSMTXInverse(inv.m, inv.m);
    atn::Matrix3x4 nrm = inv;
    PSMTXTranspose(nrm.m, nrm.m);
    GXLoadNrmMtxImm(nrm.m, 0);
    SortByDepth_unk((EffectStrip_unk**)sBuckets, sStrips, sStripCount, 0x400, sStripFar, sStripNear);
    int lit = 0;
    int twoStages = 0;
    int useCenter = 1;
    int forceCenter = 0;
    for (int i = 0x3FF; i >= 0; i--) {
        for (EffectStrip_unk* item = (EffectStrip_unk*)sBuckets[i]; item != 0; item = item->mNext) {
            if ((item->mFlags & 0x40000) == 0x40000 || (item->mFlags & 0x80000) == 0x80000) {
                if (sBufCount < sMaxBuf) {
                    sBuf[sBufCount] = item;
                    sBufCount++;
                }
                continue;
            }
            if ((item->mFlags & 4) == 4) {
                if (lit != 0) {
                    GXSetChanCtrl(4, 0, 0, 0, 0, 0, 2);
                    GXClearVtxDesc();
                    GXSetVtxDesc(9, 1);
                    GXSetVtxDesc(0xD, 1);
                    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
                    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
                    lit = 0;
                }
                if ((item->mFlags & 0x200000) == 0x200000) {
                    if (twoStages == 0) {
                        GXSetNumTevStages(2);
                        GXSetNumTexGens(2);
                        twoStages = 1;
                    }
                } else if (twoStages != 0) {
                    GXSetNumTevStages(1);
                    GXSetNumTexGens(1);
                    twoStages = 0;
                }
                SetItemRender_unk(item->mFlags, item->m0C, item->m10);
                if ((item->mFlags & 0x200000) == 0x200000 && item->m10 != 0) {
                    fn_800763C0(lbl_8087EEE0, 1, item->m10);
                }
                if (lbl_8087EFB4->m2F8 == 4) {
                    GXLoadPosMtxImm(item->mMtx.m, 0);
                }
                fn_80236938(item->mCount, item->mVerts, item->mColor, 0, 0);
            } else {
                if (lit == 0) {
                    GXSetChanCtrl(0, 1, 0, 0, 1, 2, 2);
                    GXSetChanCtrl(2, 0, 0, 0, 0, 0, 2);
                    GXClearVtxDesc();
                    GXSetVtxDesc(9, 1);
                    GXSetVtxDesc(0xA, 1);
                    GXSetVtxDesc(0xD, 1);
                    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
                    GXSetVtxAttrFmt(0, 0xA, 0, 4, 0);
                    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
                    lit = 1;
                }
                SetItemRender_unk(item->mFlags, item->m0C, item->m10);
                if ((item->mFlags & 0x200000) == 0x200000 && item->m10 != 0) {
                    fn_800763C0(lbl_8087EEE0, 1, item->m10);
                }
                if (lbl_8087EFB4->m2F8 == 4) {
                    GXLoadPosMtxImm(item->mMtx.m, 0);
                    atn::Matrix3x4 itemInv = item->mMtx;
                    PSMTXInverse(itemInv.m, itemInv.m);
                    atn::Matrix3x4 itemNrm = itemInv;
                    PSMTXTranspose(itemNrm.m, itemNrm.m);
                    GXLoadNrmMtxImm(itemNrm.m, 0);
                }
                fn_80236938(item->mCount, item->mVerts, item->mColor,
                            (forceCenter != 0 || useCenter != 0) ? &item->mCenter : 0, 1);
            }
            if ((item->mFlags & 0x10) == 0x10) {
                fn_800C06B0(lbl_8087EFB4, (char*)item->m6C + 0x3C);
            } else {
                fn_800C08B0(lbl_8087EFB4);
            }
        }
    }
    GXSetTevSwapMode(1, 0, 0);
}

// inferred: draws the deferred strips of sBuf with an indirect texture (heat haze); not decompiled
// yet (0x80235BF8, 0xC74 bytes: indirect matrices, seven texture coordinates per vertex)
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

// NONMATCHING: 99.95% - code identical; the 6.0f constant is at another position in .sdata2 (the
// draw functions, not decompiled yet, use 0.2f and 1/7 before it)
// inferred: queues a model with the matrix `mtx`
extern "C" void fn_80236FFC(const atn::Matrix3x4* mtx, void* model, const float* color, unsigned int flags,
                            const float* uv, const atn::Matrix3x4* view, int arg) {
    if (sModelCount >= sMaxModels) {
        return;
    }
    atn::Vector3 p = mtx->GetTrans_unk();
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
