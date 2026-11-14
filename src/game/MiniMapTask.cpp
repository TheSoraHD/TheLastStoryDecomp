#include "game/MiniMapTask.h"
#include "game/CharaTask.h"

#include "atn/Memory.h"
#include "atn/RenderManager.h"
#include "atn/GraphicsDevice.h"
#include "atn/Layer3D.h"
#include "atn/ColorF_unk.h"
#include "atn/Material_unk.h"
#include "atn/DrawCommand_unk.h"
#include "atn/GXDecl.h"
#include "atn/Csv.h"
#include "string.h"
#include "atn/D2A_Task.h"
#include "atn/DrawHelper.h"
#include "game/GameTask.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/GimmickCommon.h"

extern "C" void fn_8008771C(void* menu, const char* name, float* value, float min, float max, float step,
                            void (*func)(void*, void*), void* arg); // inferred: AddFloat of the debug menu

extern "C" MiniMapTask* fn_804A7EA4(atn::Task* parent, const char* path) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) MiniMapTask(parent, path);
    }
    return 0;
}

// inferred: loads the resource, the eight textures (m58 is not loaded), the radar model and the frame d2anime
// (started stopped), and registers the debug menu "Minimap"
MiniMapTask::MiniMapTask(atn::Task* parent, const char* path)
    : atn::Task(parent), m4C(1), m208(0x00A00001, 0x20), m5DC(0.0f), m5E0(0.0f), m5E4(0.0f), m5E8(0.0f),
      m5EC(0.0f), m5F0(0.0f), m5F4(0.0f), m5F8(0.0f), m5FC(500.0f), m600(1.0f, 1.0f, 1.0f, 0.0f),
      m610(1.0f, 1.0f, 1.0f, 1.0f), m620(1.0f, 1.0f, 1.0f, 1.0f), m630(16.0f), m634(20.0f), m638(8.0f), m63C(12.0f),
      m640(0, 0), m15C0(3000.0f), m160C(0) {
    m50.vf0C(path);
    mE8.fn_800D5908("common/mmap_base_mask.texture");
    mB8.fn_800D5908("common/mmap_circle01.texture");
    m148.fn_800D5908("common/mmap_circle01_mask.texture");
    m88.fn_800D5908("common/mmap_triangle01.texture");
    m118.fn_800D5908("common/mmap_triangle01_mask.texture");
    m1A8.fn_800D5908("common/mmap_shine_circle.texture");
    m178.fn_800D5908("common/mmap_shine_triangle.texture");
    m1D8.fn_800D5908("data/texture/mmap_arrow.texture");
    m208.fn_8008AD4C("data/model/radar.model", 0);
    (m5D8 = fn_801F3FF8(this, "mmap_arrow.d2a", 0))->fn_800D246C(1);
    void* group = m15BC.Load("Minimap");
    fn_80087994(group, "Enable", &m4C, 0, 0);
    fn_8008771C(group, "CamLength", &m15C0, 0.0f, 1000000.0f, 1.0f, 0, 0);
    fn_80087994(group, "DrawAll", &m160C, 0, 0);
}

MiniMapTask::~MiniMapTask() {}

// inferred: the textures are still loading (atn::Texture method at 0x800D59B8, named in symbols.txt as a method of
// GimmickUvOwner_unk, the view of atn::Texture of game/GimmickCommon.h)
static inline int IsTextureLoading_unk(atn::Texture_unk& texture) {
    return ((GimmickUvOwner_unk*)&texture)->fn_800D59B8();
}

// inferred: waits for the resource, the nine textures, the task and the model; then starts the frame d2anime
// (repeat, stopped) and reads the csv
int MiniMapTask::vf0C() {
    int loading = 0;
    if (m50.fn_80473F50() || IsTextureLoading_unk(m58) || IsTextureLoading_unk(m88) ||
        IsTextureLoading_unk(mB8) || IsTextureLoading_unk(mE8) || IsTextureLoading_unk(m118) ||
        IsTextureLoading_unk(m148) || IsTextureLoading_unk(m178) || IsTextureLoading_unk(m1A8) ||
        IsTextureLoading_unk(m1D8) || !fn_800D3FA4() || m208.fn_8008B140()) {
        loading = 1;
    }
    if (!loading) {
        m5D8->fn_800D246C(0);
        m5D8->mRepeat = 1;
        m5D8->mFlags |= 4;
        fn_804AA490();
        return 1;
    }
    return 0;
}

#define MINIMAP_MAX_UNK(a, b) ((a) > (b) ? (a) : (b))
// inferred: column of a matrix (as in gmk/PigeonManager.cpp)
inline atn::Vector3 MiniMapCol_unk(const atn::Matrix3x4& m, int i) {
    return atn::Vector3(m.m[0][i], m.m[1][i], m.m[2][i]);
}
// inferred: largest scale axis of a matrix (as in gmk/PigeonManager.cpp)
inline float MiniMapMaxScale_unk(const atn::Matrix3x4& mtx) {
    atn::Vector3 s;
    atn::Vector3 az = MiniMapCol_unk(mtx, 2);
    float sz = PSVECMag(&az);
    atn::Vector3 ay = MiniMapCol_unk(mtx, 1);
    float sy = PSVECMag(&ay);
    atn::Vector3 ax = MiniMapCol_unk(mtx, 0);
    float sx = PSVECMag(&ax);
    s.x = sx;
    s.y = sy;
    s.z = sz;
    return MINIMAP_MAX_UNK(s.x, MINIMAP_MAX_UNK(s.y, s.z));
}
extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix

// inferred: moves the markers of the marked characters to them (shown while the character can act) and updates
// the matrices of the minimap model at the identity
void MiniMapTask::vf10() {
    for (unsigned int i = 0; i < mCharaMarks.mCount; i++) {
        int marker = mCharaMarks.mData[i].mMarker;
        mMarkers[marker].mPos = mCharaMarks.mData[i].mChara->mPos;
        if (mCharaMarks.mData[i].mChara->CanAct_unk()) {
            mMarkers[marker].m00 = 1;
        } else {
            mMarkers[marker].m00 = 0;
        }
    }
    const atn::Matrix3x4& mtx = lbl_807C7060;
    static_cast<atn::Matrix3x4&>(m208.mMtx) = mtx;
    m208.m54 = MiniMapMaxScale_unk(mtx);
    m208.CalcMatrixStack(atn::ModelNodeCallback_unk());
}

// inferred: GXColor of a float colour (the bytes are stored one by one and read back as a word)
extern "C" unsigned int fn_804A88C8(const float* rgba) {
    GXColor c;
    c.r = (int)(255.0f * rgba[0]);
    c.g = (int)(255.0f * rgba[1]);
    c.b = (int)(255.0f * rgba[2]);
    c.a = (int)(255.0f * rgba[3]);
    return *(unsigned int*)&c;
}

extern "C" void fn_804A893C(unsigned int* object, unsigned int flags) {
    object[1] |= flags;
}

// NONMATCHING: 0.07% - target performs the main minimap draw/update pass.
void MiniMapTask::vf14() {}

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0;
extern "C" float fn_80076FF8(atn::GraphicsDevice_unk* device); // inferred: screen aspect ratio
inline atn::GraphicsDevice_unk* GetGraphics_unk() {
    return lbl_8087EEE0;
}
extern "C" GameTask* lbl_8087F430;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value); // inferred: sets a value of a pane
inline void atn::D2A_Task::SetLayoutValue_unk(const char* name, float value) {
    fn_801FECE0(&mData.m10, fn_800DC6B4(name), value);
}

// inferred: position of a point of the map on the screen (x and z of the map, 52 / m5FC pixels per unit)
inline atn::Vector3 MiniMapToScreen_unk(const atn::Vector3& p, float sx, float scale) {
    atn::Vector3 d((p.x + -50.0f) * sx * scale, 0.0f * scale, (p.z + 70.0f) * scale);
    return atn::Vector3(0.5f * GetGraphics_unk()->GetWidth_unk() + d.x, d.y,
                        0.5f * GetGraphics_unk()->GetHeight_unk() + d.z);
}

// NONMATCHING: 82% - stack layout (the original keeps the leader position at 0x1c and has fewer
// temporaries) and order of the float operations; tried the frame shown before/after the conversion
// inferred: places the frame of the minimap (position of the leader on the map, turned with it) and draws
// the active markers (a point of the size m63C in the colour m620 under each, then the marker in its colour
// through the mask textures)
void MiniMapTask::fn_804A9EB4() {
    float scale = 52.0f / m5FC;
    float aspect = fn_80076FF8(GetGraphics_unk());
    float sx = 1.0f / (aspect / ((float)GetGraphics_unk()->GetWidth_unk() / (float)GetGraphics_unk()->GetHeight_unk()));
    CharaTask* leader = lbl_8087F8A0->mHead;
    atn::Vector3 pos = leader->mPos;
    pos = MiniMapToScreen_unk(pos, sx, scale);
    m5D8->MeltTask();
    float base[5];
    base[0] = pos.x;
    base[1] = pos.z;
    base[2] = 1.0f;
    base[3] = 1.0f;
    base[4] = 10.0f;
    fn_801F4728(m5D8, "basePos", base);
    float rot = leader->mRot.y - 3.1415927f;
    m5D8->SetLayoutValue_unk("rot01", 57.295776f * -rot);
    m5D8->SetLayoutValue_unk("rot02", 57.295776f * -rot);
    m5D8->SetLayoutValue_unk("rot03", 57.295776f * -rot);
    for (unsigned int i = 0; i < 64; i++) {
        MiniMapMarker_unk* m = &mMarkers[i];
        if (m->m00 != 1) {
            continue;
        }
        if (m->m04 != -1 && !lbl_8087F430->fn_80370174(m->m04)) {
            continue;
        }
        atn::Vector3 screen = MiniMapToScreen_unk(m->mPos, sx, scale);
        lbl_8087EEB0->fn_8005E3E4(m620.ToU32(), &m1A8, screen.x - 0.5f * m63C * m->mScale,
                                  screen.z - 0.5f * m63C * m->mScale, 10.0f, m63C * m->mScale, m63C * m->mScale,
                                  0.0f, 0.0f, 1.0f, 1.0f, 1, 1);
        lbl_8087EEB0->fn_8005E7F4(m->mColor.ToU32(), &mB8, &m148, screen.x - 0.5f * m638 * m->mScale,
                                  screen.z - 0.5f * m638 * m->mScale, 10.0f, m638 * m->mScale, m638 * m->mScale,
                                  0.0f, 0.0f, 1.0f, 1.0f);
    }
    atn::RenderManager::instance_->AddRender(this, 2, 0.0f);
    atn::RenderManager::instance_->AddRender(this, 16, 0.0f);
}

extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int

// NONMATCHING: 99.67% - register order of the constants kept over the loop (1 in r26, 0 in r29 and the
// stack addresses in the original); tried the size in a local, the marker as an index or a pointer
// inferred: reads the csv of the minimap: markers ("LOCATION"), radar, point, arrow, off areas, lower layer
void MiniMapTask::fn_804AA490() {
    atn::Csv csv((const char*)m50.fn_80470580(), m50.fn_8047059C());
    do {
        const char* name = csv.fn_8005B9CC();
        if (strcmp(name, "LOCATION") == 0) {
            csv.fn_8005B9CC();
            float x = fn_800DC288(csv.fn_8005B9CC());
            float y = fn_800DC288(csv.fn_8005B9CC());
            float z = fn_800DC288(csv.fn_8005B9CC());
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            float a = fn_800DC288(csv.fn_8005B9CC());
            int marker = fn_804AABA8(atn::Vector3(x, y, z), atn::Color(r, g, b, a), 1.0f);
            mMarkers[marker].m24 = 0;
            const char* id = csv.fn_8005B9CC();
            if (strcmp(id, "") != 0) {
                mMarkers[marker].m04 = fn_800DC12C(id);
            }
        } else if (strcmp(name, "RADAR") == 0) {
            m5FC = fn_800DC288(csv.fn_8005B9CC());
            m5DC = fn_800DC288(csv.fn_8005B9CC());
            m5E0 = fn_800DC288(csv.fn_8005B9CC());
            m5E4 = fn_800DC288(csv.fn_8005B9CC());
            m5E8 = fn_800DC288(csv.fn_8005B9CC());
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            float a = fn_800DC288(csv.fn_8005B9CC());
            m600 = atn::Color(r, g, b, a);
            m5EC = fn_800DC288(csv.fn_8005B9CC());
            m5F0 = fn_800DC288(csv.fn_8005B9CC());
            m5F4 = fn_800DC288(csv.fn_8005B9CC());
            m5F8 = fn_800DC288(csv.fn_8005B9CC());
        } else if (strcmp(name, "POINT") == 0) {
            m638 = fn_800DC288(csv.fn_8005B9CC());
            m63C = fn_800DC288(csv.fn_8005B9CC());
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            float a = fn_800DC288(csv.fn_8005B9CC());
            m620 = atn::Color(r, g, b, a);
        } else if (strcmp(name, "ARROW") == 0) {
            m630 = fn_800DC288(csv.fn_8005B9CC());
            m634 = fn_800DC288(csv.fn_8005B9CC());
            float r = fn_800DC288(csv.fn_8005B9CC());
            float g = fn_800DC288(csv.fn_8005B9CC());
            float b = fn_800DC288(csv.fn_8005B9CC());
            float a = fn_800DC288(csv.fn_8005B9CC());
            m610 = atn::Color(r, g, b, a);
        } else if (strcmp(name, "OFF_AREA") == 0) {
            MiniMapArea_unk area;
            area.mMin.x = fn_800DC288(csv.fn_8005B9CC());
            area.mMin.y = fn_800DC288(csv.fn_8005B9CC());
            area.mMax.x = fn_800DC288(csv.fn_8005B9CC());
            area.mMax.y = fn_800DC288(csv.fn_8005B9CC());
            m1538.PushBack_unk(area);
        } else if (strcmp(name, "LOWER_LAYER") == 0) {
            m15F4 = fn_800DC288(csv.fn_8005B9CC());
            m15F8 = fn_800DC288(csv.fn_8005B9CC());
            m15FC = fn_800DC288(csv.fn_8005B9CC());
            m1600 = fn_800DC288(csv.fn_8005B9CC());
            m1604 = fn_800DC288(csv.fn_8005B9CC());
            m1608 = 1.0f;
        }
    } while (csv.fn_8005BBF8() != 0);
}

int MiniMapTask::fn_804AABA8(atn::Vector3 pos, atn::Color color, float scale) {
    int found = -1;
    for (int i = 0; i < 64; i++) {
        if (mMarkers[i].m00 == -1) {
            found = i;
            break;
        }
    }
    if (found != -1) {
        mMarkers[found] = MiniMapMarker_unk(pos, color, scale);
    }
    return found;
}

void MiniMapTask::fn_804AAD54(int marker) {
    mMarkers[marker].m00 = -1;
}

// NONMATCHING: 99.58% - registers of the inlined fn_804AABA8 (r8/r9/r11 for the argument copy and the
// constants); tried push_back with the capacity check, operator[] in the search loop
void MiniMapTask::fn_804AAD68(CharaTask* chara, const atn::Color* color) {
    if (chara == 0) {
        return;
    }
    for (unsigned int i = 0; i < mCharaMarks.mCount; i++) {
        if (mCharaMarks.mData[i].mChara == chara) {
            return;
        }
    }
    int marker = fn_804AABA8(chara->mPos, *color, 1.0f);
    if (marker != -1 && !mCharaMarks.full()) {
        mCharaMarks.PushBack_unk(MiniMapCharaMark_unk(chara, marker));
        mMarkers[marker].m24 = 0;
    }
}

void MiniMapTask::fn_804AAFCC(CharaTask* chara) {
    for (MiniMapCharaMark_unk* it = mCharaMarks.begin(); it != mCharaMarks.end(); it++) {
        if (it->mChara == chara) {
            fn_804AAD54(it->mMarker);
            mCharaMarks.erase(it);
            return;
        }
    }
}

// inferred: second draw pass of the minimap model with the minimap camera
// NONMATCHING: 98.5% - scheduling of the inlined implicit copies of atn::Camera (as its weak copy constructor)
void MiniMapTask::vf18(atn::RenderManager* view) {
    atn::Camera saved(view->mCamera);
    view->SetCamera_unk(m640);
    m208.vf18(view);
    view->SetCamera_unk(saved);
}


// ---- fn_804ABF84: draw of the minimap (over the inlining budget: the accessors are weak copies) ----
extern "C" void fn_80076760(atn::GraphicsDevice_unk* device, int state, int value);
extern "C" void fn_80075DEC(atn::GraphicsDevice_unk* device); // inferred: restores the viewport
// inferred: accessors (inline: weak copies, some of other files)
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

// inferred: draws the minimap into a temporary 256x256 texture (the transparent parts in a second pass) and
// draws it on the screen through the mask texture mE8
// NONMATCHING: 0% - equivalent (checked call by call against the original), but here it is under the inlining
// budget and inlines every accessor, while the original calls them all (about 36 statements of candidates short:
// an artificial first candidate of 36-40 statements puts it over)
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
    cmd.mTex1 = (atn::Texture_unk*)fn_804ABF40(&mE8);
    cmd.m14 = 0;
    cmd.m18 = 0;
    cmd.vf10(view);
    fn_804ABF14(fn_804ABF20());
    fn_80071E04(GetGraphics_unk());
    fn_804ABF00(layer, flags);
    view->SetCamera_unk(saved);
    fn_80071D68(GetGraphics_unk(), fn_804ABEE4(&saved)->m);
}

// inferred: camera setters (declared in atn/Camera_unk.h; weak copies 0x804A9E54/0x804A9E64)
inline void atn::Camera::SetPosition(float x, float y, float z) {
    mPos.x = x;
    mPos.y = y;
    mPos.z = z;
}
inline void atn::Camera::SetTarget(float x, float y, float z) {
    mTarget.x = x;
    mTarget.y = y;
    mTarget.z = z;
}

// NONMATCHING: draws the minimap model with its orthographic camera: the transparent parts first
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
    camera.SetPosition(50.0f, 100.0f, -70.0f);
    camera.SetTarget(50.0f, 0.0f, -70.0f);
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
