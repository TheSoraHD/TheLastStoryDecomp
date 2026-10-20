#include "atn/LensFlare.h"
#include "atn/GXDecl.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

// inferred: GX functions not in atn/GXDecl.h (enums as int)
extern "C" {
void GXBegin(int type, int fmt, unsigned short count);
}

// inferred: write gather pipe of the GX FIFO
union LensWGPipe_unk {
    float f;
    unsigned int u;
};
#define GXWGFifo (*(volatile LensWGPipe_unk*)0xCC008000)

// inferred: loads a texture by name (atn::Texture_unk member, automatic name)
extern "C" void fn_800D5908(atn::Texture_unk* tex, const char* name);
// inferred: nonzero while a texture is loading. The symbol is named after its first known user
// (game/GimmickCommon.h); it is an atn::Texture_unk member.
struct GimmickUvOwner_unk {
    int fn_800D59B8();
};

// inferred: debug menu entries
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);
extern "C" void fn_80088AF4(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: color

// inferred: global render settings; m4C is the root debug menu group
struct LensSettings_unk {
    unsigned char m00[0x4C];
    void* m4C;
};
extern "C" LensSettings_unk* lbl_8087EFA8;

// inferred: light of the scene (direction at 0x14)
struct LensLight_unk {
    unsigned char m00[0x14];
    float mDirX;
    float mDirY;
    float mDirZ;
};

// inferred: camera passed to the renderers (see also atn/Rain.cpp)
struct LensCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mPos;          // 0x10C
    unsigned char m118[0x204 - 0x118];
    unsigned char mFrustum[0xF8]; // 0x204
    void* m2FC;                 // inferred: light manager
};
extern "C" LensCamera_unk* lbl_8087EFB4;
extern "C" void fn_800BDB40(LensCamera_unk* camera, int arg);
extern "C" LensLight_unk* fn_800C2448(void* lights, int index);
extern "C" void fn_800BDB58(LensCamera_unk* camera, void* render, float order, int pass); // inferred: queues a render

// inferred: sphere and box used by the visibility tests
struct LensSphere_unk {
    atn::Vector3 mCenter;
    float mRadius;
};
struct LensBox_unk {
    LensBox_unk() {}
    LensBox_unk(const atn::Vector3& min, const atn::Vector3& max) : mMin(min), mMax(max) {}
    atn::Vector3 mMin;
    atn::Vector3 mMax;
};
extern "C" int fn_8004FF58(void* frustum, const LensSphere_unk* sphere);
extern "C" float fn_800C169C(LensCamera_unk* camera, const LensBox_unk* box); // inferred: visible fraction
extern "C" void fn_8006FEBC(LensBox_unk* screen, const LensBox_unk* box);     // inferred: projects a box

// inferred: render state (viewport size at 0x3C/0x40)
struct LensRenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;
    int mHeight;
};
extern "C" LensRenderState_unk* lbl_8087EEE0;

// inferred: screen limits and alpha range (globals of another file)
extern "C" float lbl_8087D740;
extern "C" float lbl_8087D744;
extern "C" float lbl_8087D748;
extern "C" float lbl_8087D74C;
extern "C" float lbl_8087D750;
extern "C" float lbl_8087D754;

// inferred: RGBA color of an element
struct LensColor_unk {
    LensColor_unk() {}
    LensColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: sets the color of an element (its m0C-m18 fields)
inline void LensSetColor_unk(atn::LensFlareElem* e, const LensColor_unk& c) {
    e->m0C = c.r;
    e->m10 = c.g;
    e->m14 = c.b;
    e->m18 = c.a;
}

// inferred: creates the debug menu group of `handle` under `parent`
inline void* LensCreateGroup_unk(ResHandle& handle, void* parent, const char* name) {
    if (handle.mRes == 0 && parent != 0 && lbl_8087EF10 != 0) {
        handle.mRes = fn_8008937C(parent, name);
        return handle.mRes;
    }
    return 0;
}

// inferred: one vertex of the flare quad fan (position, ARGB color, uv)
inline unsigned int LensToByte_unk(float f) {
    return f >= 1.0f ? 0xFF : (f <= 0.0f ? 0 : (unsigned int)(255.0f * f + 0.5f));
}

struct LensVertex_unk {
    LensVertex_unk() {}
    LensVertex_unk(const atn::Vector3& pos, const LensColor_unk& color, const atn::Vector2_unk& uv) {
        unsigned int r = LensToByte_unk(color.r);
        unsigned int g = LensToByte_unk(color.g);
        unsigned int b = LensToByte_unk(color.b);
        unsigned int a = LensToByte_unk(color.a);
        mPos = pos;
        mColor = (a << 24) | (r << 16) | (g << 8) | b;
        mUV = uv;
    }
    atn::Vector3 mPos;
    unsigned int mColor;
    atn::Vector2_unk mUV;
};

namespace atn {
extern "C" void fn_80078938(LensFlare* flare, float alpha);
}

// inferred: clamps v to [min, max]
inline float LensClamp_unk(float v, float min, float max) {
    return v < min ? min : (v > max ? max : v);
}

namespace atn {

LensFlare::LensFlare(int arg, int count, const Vector3& pos, float size) : m04(arg), mPos(pos), m14(size) {
    for (int i = 0; i < 6; i++) {
        char name[0x100];
        sprintf(name, "common/fx_lensflare%02d.texture", i + 1);
        fn_800D5908(&m20[i], name);
    }
    mElems.Resize_unk(count);
    void* group = LensCreateGroup_unk(m140, lbl_8087EFA8->m4C, "LensFlare");
    for (unsigned int i = 0; i < mElems.mCount; i++) {
        LensFlareElem* e = &mElems[i];
        e->m08 = 0.0f;
        e->m04 = 0.0f;
        e->m00 = 0;
        LensSetColor_unk(e, LensColor_unk(1.0f, 1.0f, 1.0f, 1.0f));
        char name[0x100];
        sprintf(name, "Flare %d", i);
        void* elemGroup = fn_8008937C(group, name);
        fn_800874C8(elemGroup, "Type", &e->m00, 0, 6, 1, 0, 0);
        fn_8008771C(elemGroup, "Size", &e->m04, 0, 0, 0.0f, 1000.0f, 0.1f);
        fn_8008771C(elemGroup, "Position", &e->m08, 0, 0, -100.0f, 100.0f, 0.01f);
        fn_80088AF4(elemGroup, "Color", &e->m0C, 0, 0, 0.0f, 1.0f, 0.01f);
    }
}

LensFlareElem::LensFlareElem() {}

LensFlare::~LensFlare() {}

int LensFlare::fn_800774D4() {
    int loading = 0;
    for (int i = 0; i < 6; i++) {
        loading |= ((GimmickUvOwner_unk*)&m20[i])->fn_800D59B8();
    }
    return loading;
}

void LensFlare::fn_80077534() {
    fn_800BDB58(lbl_8087EFB4, this, 0.0f, 0xC);
}

// NONMATCHING: 90.2% - register allocation of the negated light direction and of the clamps,
// and the order of the loads of the projected box
void LensFlare::vf10(void* arg) {
    LensCamera_unk* camera = (LensCamera_unk*)arg;
    fn_800BDB40(camera, 0);
    if (m04 == 1) {
        LensLight_unk* light = fn_800C2448(camera->m2FC, 0);
        Vector3 dir(-light->mDirX, -light->mDirY, -light->mDirZ);
        mPos = camera->mPos + dir * 800.0f;
    }
    LensSphere_unk sphere;
    sphere.mCenter = mPos;
    sphere.mRadius = m14;
    if (fn_8004FF58(camera->mFrustum, &sphere)) {
        float r = sphere.mRadius;
        LensBox_unk box;
        box.mMin = sphere.mCenter - Vector3(r, r, r);
        box.mMax = sphere.mCenter + Vector3(r, r, r);
        float visible = fn_800C169C(lbl_8087EFB4, &box);
        if (visible != 0.0f) {
            LensBox_unk screen;
            fn_8006FEBC(&screen, &box);
            float width = lbl_8087EEE0->mWidth;
            float height = lbl_8087EEE0->mHeight;
            float area = (screen.mMin.x - screen.mMax.x) * (screen.mMin.y - screen.mMax.y);
            screen.mMin.x = LensClamp_unk(screen.mMin.x, lbl_8087D740, width);
            screen.mMin.y = LensClamp_unk(screen.mMin.y, lbl_8087D744, height);
            screen.mMax.x = LensClamp_unk(screen.mMax.x, lbl_8087D748, width);
            screen.mMax.y = LensClamp_unk(screen.mMax.y, lbl_8087D74C, height);
            float alpha = (1.0f - visible) -
                          (1.0f - (screen.mMin.x - screen.mMax.x) * (screen.mMin.y - screen.mMax.y) / area);
            fn_80078938(this, LensClamp_unk(alpha, lbl_8087D750, lbl_8087D754));
        }
    }
}

} // namespace atn

// NONMATCHING: 75.3% - same size and instructions, but the original finishes each vertex before
// starting the next (ours hoists the position sums) and needs one saved register less
// inferred: draws one flare element as a fan of 8 triangles around center
extern "C" void fn_8007786C(const atn::Vector3& center, const atn::Vector3& right, const atn::Vector3& up,
                            const LensColor_unk& color) {
    LensVertex_unk v[10] = {
        LensVertex_unk(center, color, atn::Vector2_unk(0.0f, 1.0f)),
        LensVertex_unk(center + right, color, atn::Vector2_unk(0.0f, 0.0f)),
        LensVertex_unk(center + right + up, color, atn::Vector2_unk(1.0f, 0.0f)),
        LensVertex_unk(center + up, color, atn::Vector2_unk(1.0f, 1.0f)),
        LensVertex_unk(center - right + up, color, atn::Vector2_unk(1.0f, 0.0f)),
        LensVertex_unk(center - right, color, atn::Vector2_unk(0.0f, 0.0f)),
        LensVertex_unk(center - right - up, color, atn::Vector2_unk(1.0f, 0.0f)),
        LensVertex_unk(center - up, color, atn::Vector2_unk(1.0f, 1.0f)),
        LensVertex_unk(center + right - up, color, atn::Vector2_unk(1.0f, 0.0f)),
        LensVertex_unk(center + right, color, atn::Vector2_unk(0.0f, 0.0f)),
    };
    GXBegin(0xA0, 0, 10);
    for (int i = 0; i < 10; i++) {
        GXWGFifo.f = v[i].mPos.x;
        GXWGFifo.f = v[i].mPos.y;
        GXWGFifo.f = v[i].mPos.z;
        GXWGFifo.u = (v[i].mColor << 8) | (v[i].mColor >> 24);
        GXWGFifo.f = v[i].mUV.x;
        GXWGFifo.f = v[i].mUV.y;
    }
}

// NONMATCHING: not decompiled - fn_80078938 (0x80078938, sets up GX and draws every element with
// fn_8007786C along the axis between the sun and the screen center)
