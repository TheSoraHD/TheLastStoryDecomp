#include "atn/LensFlare.h"
#include "atn/GXDecl.h"
#include "atn/RenderManager.h"
#include "atn/Scene.h"
#include "atn/Light_unk.h"
#include "atn/AABB_unk.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);

// inferred: GX functions not in atn/GXDecl.h (enums as int)
extern "C" {
void GXBegin(int type, int fmt, unsigned short count);
void GXClearVtxDesc();
void GXSetVtxDesc(int attr, int type);
void GXSetVtxAttrFmt(int fmt, int attr, int count, int type, unsigned char frac);
void GXLoadPosMtxImm(const Mtx m, unsigned int id);
void GXSetCurrentMtx(unsigned int id);
}

// inferred: write gather pipe of the GX FIFO
union LensWGPipe_unk {
    float f;
    unsigned int u;
};
#define GXWGFifo (*(volatile LensWGPipe_unk*)0xCC008000)

// inferred: loads a texture by name (atn::Texture_unk member, automatic name)
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



extern "C" void fn_8006FEBC(atn::AABB_unk* screen, const atn::AABB_unk* box);     // inferred: projects a box

// inferred: render state (viewport size at 0x3C/0x40)
struct LensRenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;
    int mHeight;
};
extern "C" LensRenderState_unk* lbl_8087EEE0;
extern "C" void fn_80076760(LensRenderState_unk* rs, int index, int value); // inferred: sets a render state
extern "C" void fn_80072914(LensRenderState_unk* rs, int mode);
extern "C" void fn_800761A8(LensRenderState_unk* rs);                     // inferred: sends the render states
extern "C" void fn_800763C0(LensRenderState_unk* rs, int map, atn::Texture_unk* tex); // inferred: loads a texture

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
    LensVertex_unk(const atn::Vector3& pos, unsigned int color, const atn::Vector2_unk& uv)
        : mPos(pos), mColor(color), mUV(uv) {}
    atn::Vector3 mPos;
    unsigned int mColor;
    atn::Vector2_unk mUV;
};

// inferred: clamps v to [min, max]
inline float LensClamp_unk(float v, float min, float max) {
    return v < min ? min : (v > max ? max : v);
}

namespace atn {

LensFlare::LensFlare(int arg, int count, const Vector3& pos, float size) : m04(arg), mPos(pos), m14(size) {
    for (int i = 0; i < 6; i++) {
        char name[0x100];
        sprintf(name, "common/fx_lensflare%02d.texture", i + 1);
        m20[i].fn_800D5908(name);
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
    atn::RenderManager::instance_->AddRender(this, 0xC, 0.0f);
}

// NONMATCHING: 90.2% - register allocation of the negated light direction and of the clamps,
// and the order of the loads of the projected box
void LensFlare::vf10(RenderManager* arg) {
    RenderManager* camera = arg;
    camera->fn_800BDB40(0);
    if (m04 == 1) {
        atn::Light* light = camera->mScene->GetLight(0);
        Vector3 dir(-light->mDir.x, -light->mDir.y, -light->mDir.z);
        mPos = camera->mCamera.mPos + dir * 800.0f;
    }
    atn::Sphere sphere;
    sphere.mCenter = mPos;
    sphere.mRadius = m14;
    if (fn_8004FF58(&camera->mCamera.mFrustum, &sphere)) {
        float r = sphere.mRadius;
        atn::AABB_unk box;
        box.mMin = sphere.mCenter - Vector3(r, r, r);
        box.mMax = sphere.mCenter + Vector3(r, r, r);
        float visible = atn::RenderManager::instance_->fn_800C169C(&box);
        if (visible != 0.0f) {
            atn::AABB_unk screen;
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
            fn_80078938(LensClamp_unk(alpha, lbl_8087D750, lbl_8087D754));
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

namespace atn {

// NONMATCHING: 77.7% - scheduling of the sun quad (dead code: built and not drawn, as in the original) and
// of the camera axes
// inferred: draws the elements along the line from the screen center to the sun (`alpha`: their opacity); the
// quad of the sun itself is built but not drawn (dead code of the original)
void LensFlare::fn_80078938(float alpha) {
    RenderManager* rm = RenderManager::instance_;
    Light* light = rm->mScene->GetLight(0);
    Vector3 dir(-light->mDir.x, -light->mDir.y, -light->mDir.z);
    Vector3 sun = rm->mCamera.mPos + dir * 800.0f;
    {
        Vector3 right =
            Vector3(rm->mCamera.mView.m[0][0], rm->mCamera.mView.m[0][1], rm->mCamera.mView.m[0][2]) * 50.0f;
        Vector3 up = Vector3(rm->mCamera.mView.m[1][0], rm->mCamera.mView.m[1][1], rm->mCamera.mView.m[1][2]) * 50.0f;
        unsigned int color = 0xFFFFFFFF;
        LensVertex_unk v[4] = {
            LensVertex_unk(sun + up - right, color, atn::Vector2_unk(1.0f, 0.0f)),
            LensVertex_unk(sun + up + right, color, atn::Vector2_unk(0.0f, 1.0f)),
            LensVertex_unk(sun - up - right, color, atn::Vector2_unk(1.0f, 1.0f)),
            LensVertex_unk(sun - up + right, color, atn::Vector2_unk(0.0f, 0.0f)),
        };
    }
    GXSetNumChans(1);
    GXSetNumTexGens(1);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 4);
    GXSetTevOp(0, 0);
    GXSetChanCtrl(4, 0, 1, 1, 1, 0, 2);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(0xB, 1);
    GXSetVtxDesc(0xD, 1);
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 0xB, 1, 5, 0);
    GXSetVtxAttrFmt(0, 0xD, 1, 4, 0);
    LensRenderState_unk* rs = lbl_8087EEE0;
    fn_80076760(rs, 8, 0);
    fn_80076760(rs, 9, 7);
    fn_80076760(rs, 10, 0);
    fn_80072914(lbl_8087EEE0, 1);
    Matrix3x4 identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    GXLoadPosMtxImm(identity.m, 0);
    GXSetCurrentMtx(0);
    Vector3 screen = rm->GetScreenPosition(sun);
    screen.z = 0.0f;
    float width = lbl_8087EEE0->mWidth;
    float height = lbl_8087EEE0->mHeight;
    Vector3 axisY(0.0f, 1.0f, 0.0f);
    Vector3 axisX(1.0f, 0.0f, 0.0f);
    Vector3 center(0.5f * width, 0.5f * height, 1.0f);
    Vector3 toSun(screen.x - center.x, screen.y - center.y, 0.0f);
    fn_800761A8(lbl_8087EEE0);
    for (unsigned int i = 0; i < mElems.mCount; i++) {
        LensFlareElem* e = &mElems[i];
        if (e->m00 != 0) {
            fn_800763C0(lbl_8087EEE0, 0, &m20[e->m00 - 1]);
            float aspect = height / width;
            Vector3 right(axisY.x * e->m04, axisY.y * e->m04 * aspect, axisY.z * e->m04);
            Vector3 up = axisX * e->m04;
            LensColor_unk color(e->m0C, e->m10, e->m14, alpha);
            Vector3 pos = Vector3(0.5f * width, 0.5f * height, 0.0f) + toSun * e->m08;
            fn_8007786C(pos, right, up, color);
        }
    }
}

} // namespace atn
