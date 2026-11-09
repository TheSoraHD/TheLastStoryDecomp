#include "game/MiniMapTask.h"

#include "atn/Memory.h"
#include "atn/RenderManager.h"
#include "atn/GraphicsDevice.h"
#include "atn/Layer3D.h"
#include "atn/ColorF_unk.h"
#include "atn/Material_unk.h"
#include "atn/DrawCommand_unk.h"
#include "atn/GXDecl.h"

extern "C" MiniMapTask* fn_804A7EA4(atn::Task* parent, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) MiniMapTask(parent, path);
    }
    return 0;
}

// NONMATCHING: 21.93% - resource setup and record initialization are incomplete.
MiniMapTask::MiniMapTask(atn::Task* parent, const char* path)
    : atn::Task(parent), m4C(1), m50(), m58(), m208(0x00A00001, 0x20), m640(0, 0), m834(-1), m838(-1),
      m83C(), m860(), m15C4(), m15F4(), m160C(0) {
    m50.vf0C(path);
}

// NONMATCHING: 55.67% - target destructor releases resources and owned tasks.
MiniMapTask::~MiniMapTask() {}

// NONMATCHING: 1.75% - target handles the resource completion callback.
int MiniMapTask::vf0C() {
    return 0;
}

// NONMATCHING: 0.62% - target updates minimap animation/state.
void MiniMapTask::vf10() {}

// NONMATCHING: 70.76% - target preserves direct float-to-integer conversion details.
extern "C" unsigned int fn_804A88C8(const float* rgba) {
    const unsigned int r = (int)(rgba[0] * 255.0f);
    const unsigned int g = (int)(rgba[1] * 255.0f);
    const unsigned int b = (int)(rgba[2] * 255.0f);
    const unsigned int a = (int)(rgba[3] * 255.0f);
    return (r << 24) | (g << 16) | (b << 8) | a;
}

extern "C" void fn_804A893C(unsigned int* object, unsigned int flags) {
    object[1] |= flags;
}

// NONMATCHING: 0.07% - target performs the main minimap draw/update pass.
void MiniMapTask::vf14() {}

// NONMATCHING: 0.22% - target builds and submits minimap render data.
void MiniMapTask::vf18(atn::RenderManager* view) {
    (void)view;
}


// ---- fn_804ABF84: draw of the minimap (over the inlining budget: the accessors are weak copies) ----
extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" float fn_80076FF8(atn::GraphicsDevice_unk* device); // inferred: screen aspect ratio
extern "C" void fn_80076760(atn::GraphicsDevice_unk* device, int state, int value);
extern "C" void fn_80075DEC(atn::GraphicsDevice_unk* device); // inferred: restores the viewport
// inferred: accessors (inline: weak copies, some of other files)
inline atn::GraphicsDevice_unk* GetGraphics_unk() {
    return lbl_8087EEE0;
}
extern "C" inline void fn_804A9E4C(atn::Camera* camera, int projType) {
    camera->mProjType = projType;
}
extern "C" inline void fn_804A9E74(atn::Camera* camera, float x, float y, float z) {
    camera->mUp.x = x;
    camera->mUp.y = y;
    camera->mUp.z = z;
}
extern "C" inline int fn_804A9E84(atn::GraphicsDevice_unk* device) {
    return device->mWidth;
}
extern "C" inline int fn_804A9E8C(atn::GraphicsDevice_unk* device) {
    return device->mHeight;
}
extern "C" inline void fn_804A9E94(atn::Camera* camera, float top) {
    camera->mTop = top;
}
extern "C" inline void fn_804A9E9C(atn::Camera* camera, float bottom) {
    camera->mBottom = bottom;
}
extern "C" inline void fn_804A9EA4(atn::Camera* camera, float left) {
    camera->mLeft = left;
}
extern "C" inline void fn_804A9EAC(atn::Camera* camera, float right) {
    camera->mRight = right;
}
extern "C" inline atn::Matrix44_unk* fn_804ABEE4(atn::Camera* camera) {
    return &camera->mProj;
}
extern "C" inline atn::IRenderLayer* fn_804ABEEC(atn::RenderManager* view, int i) {
    return view->mLayers[i];
}
extern "C" inline int fn_804ABEF8(atn::IRenderLayer* layer) {
    return layer->m0C;
}
extern "C" inline void fn_804ABF00(atn::IRenderLayer* layer, int flags) {
    layer->m0C = flags;
}
extern "C" inline MaterialSet_unk* fn_804AC3C8(atn::Model* model) {
    return &model->m104;
}
extern "C" inline unsigned int fn_804AC3D0(Material_unk* material) {
    return material->m00;
}
// inferred: sets the color of a material (weak copy 0x80316F9C of game/Em316LaserSphere.cpp)
extern "C" inline void fn_80316F9C(Material_unk* material, const GXColor* c) {
    material->mColor.r = c->r;
    material->mColor.g = c->g;
    material->mColor.b = c->b;
    material->mColor.a = c->a;
}

// ---- vf10: draw of the minimap into a 256x256 texture and of that texture with the mask (over the
// inlining budget in the original: the accessors are weak copies) ----
extern "C" void fn_80071E04(atn::GraphicsDevice_unk* device);
// inferred: the minimap is drawn directly (no texture)
extern "C" __declspec(noinline) inline int fn_804A9E44(MiniMapTask* task) {
    return task->m160C;
}
// inferred: flag 4 of the model (the model has transparent parts)
extern "C" __declspec(noinline) inline unsigned int fn_804ABF08(atn::Model* model) {
    return (*(unsigned int*)((char*)model + 4) >> 4) & 1;
}
namespace atn {
// inferred: atn::PostFxManager (include/atn/PostFx.h redeclares lbl_8087EEE0 with another type)
class PostFxManager {
public:
    void* fn_800A96AC(int width, int height, int format); // inferred: temporary texture
};
} // namespace atn
extern "C" atn::PostFxManager* lbl_8087EF8C;
// inferred: view of atn::PostFxManager (lbl_8087EF8C), only the member used here
struct MMPostFx_unk {
    unsigned char m00[0x54];
    int m54;
};
extern "C" __declspec(noinline) inline void fn_804ABF14(atn::PostFxManager* fx) {
    ((MMPostFx_unk*)fx)->m54 = 0;
}
extern "C" __declspec(noinline) inline atn::PostFxManager* fn_804ABF20() {
    return lbl_8087EF8C;
}
extern "C" __declspec(noinline) inline void fn_804ABF28(atn::GraphicsDevice_unk* device, int value) {
    fn_80076760(device, 3, value);
}
// inferred: GXSetTexCoordGen (the SDK inline)
extern "C" __declspec(noinline) inline void fn_804ABF34(int dst, int func, int src, unsigned int mtx) {
    GXSetTexCoordGen2(dst, func, src, mtx, 0, 0x7D);
}
extern "C" __declspec(noinline) inline GXTexObj* fn_804ABF40(atn::Texture_unk* texture) {
    return (GXTexObj*)texture;
}
extern "C" __declspec(noinline) inline atn::Texture_unk* fn_804ABF44(atn::RenderManager* view) {
    return &view->m74C;
}

// NONMATCHING: draft - draws the minimap into a temporary 256x256 texture (the transparent parts in a
// second pass) and draws it on the screen through the mask texture m58[3]
void MiniMapTask::vf10(atn::RenderManager* view) {
    if (fn_804A9E44(this) != 0) {
        fn_804ABF84(view);
        return;
    }
    atn::Camera saved(*view->fn_800F7258());
    view->SetCamera_unk(m640);
    fn_80071D74(GetGraphics_unk(), fn_804ABEE4(&m640)->m);
    int width = fn_804A9E84(GetGraphics_unk());
    int height = fn_804A9E8C(GetGraphics_unk());
    atn::IRenderLayer* layer = fn_804ABEEC(fn_8008B964(), 0x10);
    int flags = fn_804ABEF8(layer);
    view->fn_800C0508(1);
    fn_800760BC(GetGraphics_unk(), (width - 0x100) / 2, (height - 0x100) / 2, 0x100, 0x100);
    fn_804ABF00(layer, flags & ~2);
    m208.vf10(view);
    if (fn_804ABF08(&m208) != 0) {
        fn_804ABF00(layer, flags & ~1);
        m208.vf10(view);
    }
    fn_80075DEC(GetGraphics_unk());
    fn_800760D0(GetGraphics_unk());
    fn_804ABF14(fn_804ABF20());
    void* image = fn_804ABF20()->fn_800A96AC(0x100, 0x100, 4);
    GXTexObj tex;
    GXInitTexObj(&tex, image, 0x100, 0x100, 4, 0, 0, 0);
    GXInitTexObjLOD(&tex, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    GXSetTexCopySrc((unsigned short)(width - 0x100) / 2, (unsigned short)(height - 0x100) / 2, 0x100, 0x100);
    GXSetTexCopyDst(0x100, 0x100, 4, 0);
    GXCopyTex(image, 0);
    GXPixModeSync();
    fn_80071E04(GetGraphics_unk());
    fn_804ABF28(GetGraphics_unk(), 0);
    GXSetNumTexGens(1);
    GXSetNumChans(0);
    GXSetNumTevStages(1);
    GXSetBlendMode(0, 1, 0, 3);
    fn_804ABF34(0, 1, 4, 0x3C);
    GXSetTevOp(0, 3);
    GXSetTevOrder(0, 0, 0, 0xFF);
    fn_800763FC(GetGraphics_unk(), fn_804ABF40(fn_804ABF44(view)), 0);
    view->fn_800BFB70();
    atn::DrawMaskVertex_unk verts[4];
    verts[0].Set_unk(atn::Vector3(m5DC, m5E0, 0.0f), 0xFFFFFFFF, atn::Vector4_unk(0.0f, 0.0f, 0.0f, 0.0f));
    verts[1].Set_unk(atn::Vector3(m5DC + m5E4, m5E0, 0.0f), 0xFFFFFFFF, atn::Vector4_unk(1.0f, 0.0f, 1.0f, 0.0f));
    verts[2].Set_unk(atn::Vector3(m5DC + m5E4, m5E0 + m5E8, 0.0f), 0xFFFFFFFF,
                     atn::Vector4_unk(1.0f, 1.0f, 1.0f, 1.0f));
    verts[3].Set_unk(atn::Vector3(m5DC, m5E0 + m5E8, 0.0f), 0xFFFFFFFF, atn::Vector4_unk(0.0f, 1.0f, 0.0f, 1.0f));
    fn_804ABF28(GetGraphics_unk(), 1);
    atn::DrawHelper::ComDrawTextureMask cmd;
    cmd.m04 = 1;
    cmd.mVertex = verts;
    cmd.mTex0 = (atn::Texture_unk*)&tex;
    cmd.mTex1 = (atn::Texture_unk*)fn_804ABF40(&m58[3]);
    cmd.m14 = 0;
    cmd.m18 = 0;
    cmd.vf10(view);
    fn_804ABF14(fn_804ABF20());
    fn_80071E04(GetGraphics_unk());
    fn_804ABF00(layer, flags);
    view->SetCamera_unk(saved);
    fn_80071D68(GetGraphics_unk(), fn_804ABEE4(&saved)->m);
}

// NONMATCHING: draft - draws the minimap model with its orthographic camera: the transparent parts first
// (alpha 0.6, house/line/road hidden), then the rest, and restores the camera of the view
void MiniMapTask::fn_804ABF84(atn::RenderManager* view) {
    atn::Camera saved(*view->fn_800F7258());
    atn::Camera camera(m640);
    float h = m5FC * (float)GetGraphics_unk()->GetHeight_unk() / 52.0f;
    float aspect = fn_80076FF8(GetGraphics_unk());
    fn_804A9E94(&camera, 0.5f * h);
    fn_804A9E9C(&camera, 0.5f * -h);
    fn_804A9EA4(&camera, 0.5f * -h * aspect);
    fn_804A9EAC(&camera, 0.5f * h * aspect);
    fn_804A9E4C(&camera, 1);
    // inferred: the original calls Camera::SetPosition(float, float, float) and SetTarget(float, float, float)
    // (weak copies 0x804A9E54/0x804A9E64); a new inline in atn/Camera_unk.h renumbers 187 units
    camera.mPos.Set_unk(50.0f, 100.0f, -70.0f);
    camera.mTarget.Set_unk(50.0f, 0.0f, -70.0f);
    fn_804A9E74(&camera, 0.0f, 0.0f, -1.0f);
    camera.fn_8004B378();
    view->SetCamera_unk(camera);
    fn_80071D74(GetGraphics_unk(), fn_804ABEE4(&camera)->m);
    fn_804A9E84(GetGraphics_unk());
    fn_804A9E8C(GetGraphics_unk());
    atn::IRenderLayer* layer = fn_804ABEEC(fn_8008B964(), 0x10);
    int flags = fn_804ABEF8(layer);
    fn_804ABF00(layer, flags & ~1);
    fn_804A893C((unsigned int*)&m208, 0x10);
    for (unsigned int i = 0; i < fn_804AC3C8(&m208)->mMaterials.size(); i++) {
        Material_unk* material = fn_804AC3C8(&m208)->mMaterials[i];
        ColorF_unk color(fn_8000DD98(material));
        color.a *= 0.6f;
        unsigned int c = fn_804A88C8(&color.r);
        fn_80316F9C(material, (const GXColor*)&c);
        material->fn_8007FAF0(5);
    }
    m208.fn_80093D98("m_house_alpha", 0);
    m208.fn_80093D98("m_line_alpha", 0);
    m208.fn_80093D98("m_road_alpha", 0);
    fn_80076760(GetGraphics_unk(), 0xB, 0);
    m208.vf10(view);
    fn_80076760(GetGraphics_unk(), 0xB, 3);
    m208.vf10(view);
    for (unsigned int i = 0; i < fn_804AC3C8(&m208)->mMaterials.size(); i++) {
        m208.fn_80093E30(fn_804AC3D0(fn_804AC3C8(&m208)->mMaterials[i]), 0);
    }
    m208.fn_80093D98("m_house_alpha", 1);
    m208.fn_80093D98("m_line_alpha", 1);
    m208.fn_80093D98("m_road_alpha", 1);
    m208.vf10(view);
    for (unsigned int i = 0; i < fn_804AC3C8(&m208)->mMaterials.size(); i++) {
        Material_unk* material = fn_804AC3C8(&m208)->mMaterials[i];
        ColorF_unk color(fn_8000DD98(material));
        color.a = 1.0f;
        unsigned int c = fn_804A88C8(&color.r);
        fn_80316F9C(material, (const GXColor*)&c);
        material->fn_8007FAF0(1);
        m208.fn_80093E30(fn_804AC3D0(material), 1);
    }
    fn_80075DEC(GetGraphics_unk());
    fn_800760D0(GetGraphics_unk());
    fn_804ABF00(layer, flags);
    view->SetCamera_unk(saved);
    fn_80071D68(GetGraphics_unk(), fn_804ABEE4(&saved)->m);
}

// inferred: forwarded to the minimap model
int MiniMapTask::GetRenderType_unk() {
    return m208.GetRenderType_unk();
}

void MiniMapTask::vf1C() {
    m208.vf1C();
}
