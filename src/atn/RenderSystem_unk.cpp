#include "atn/GXDecl.h"
#include "atn/MemoryManager.h"
#include "atn/Camera_unk.h"
#include "atn/Light_unk.h"
#include "atn/Layer3D.h"
#include "atn/Layer2DBlur.h"
#include "atn/LayerPost3D.h"
#include "atn/LayerPostFx.h"
#include "atn/LayerReflect.h"
#include "atn/Memory.h"
#include "atn/Sphere.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"
#include "std/vector.h"
#include "algorithm"
#include <boost/function.hpp>
#include <new>
#include "atn/DrawHelper.h"

struct RenderCallback_unk {
    RenderCallback_unk(boost::function<void(void*)> func, int arg);

    boost::function<void(void*)> mFunc;
    int mArg;
};

struct MemoryMode_unk {
    MemoryMode_unk(int mode, unsigned long align);
    ~MemoryMode_unk();
};

extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                             unsigned char mipmap, unsigned char maxLod);
extern "C" void DCFlushRange(void* address, unsigned long size);
extern "C" int lbl_8087D8F8;
extern "C" int lbl_8087D8FC;
extern "C" int lbl_8087D900;
extern "C" int lbl_8087D904;
extern "C" float lbl_8087D908;
extern "C" float lbl_8087D90C;
extern "C" unsigned char lbl_8087D81C[];
extern "C" int lbl_8087EFB8;
extern "C" void* lbl_8087EFA8;
extern "C" void* lbl_8087EFB4;
extern "C" void* lbl_8087EF8C;

extern "C" void GXSetTexCopySrc(unsigned short left, unsigned short top, unsigned short width,
                                  unsigned short height);
extern "C" void GXSetTexCopyDst(unsigned short width, unsigned short height, int format, unsigned char mipmap);
extern "C" void GXCopyTex(void* dest, unsigned char clear);
extern "C" void GXPixModeSync();
extern "C" void GXSetFog(int type, void* color, float start, float end, float nearZ, float farZ);
extern "C" void GXGetViewportv(float viewport[6]);
extern "C" void GXProject(float x, float y, float z, const float matrix[3][4], const float projection[7],
                            const float viewport[6], float* screenX, float* screenY, float* screenZ);
extern "C" void fn_800D5F68(Mtx out, int mode, int a, int b, int c);
extern "C" void fn_8009DE60(void* batch, float bias);
extern "C" void fn_80071D80(void* device, void* texture);
extern "C" void fn_800BECF0(void* entries, unsigned int count);
extern "C" void fn_80071AFC(void* state);
extern "C" void fn_8007A4CC__13LightList_unkFv(void* lights);
extern "C" void fn_80075DEC(void* state);
extern "C" void fn_8007192C(void* state);
extern "C" void LCEnable();
extern "C" void LCDisable();
extern "C" void fn_800B95C4();
extern "C" int SCGetProgressiveMode();
extern "C" unsigned int VIGetDTVStatus();
extern "C" void* memset(void* dest, int value, unsigned long size);
void operator delete[](void* ptr);
extern "C" unsigned char lbl_8087D910[];
extern "C" unsigned char lbl_8087D914[];
// inferred: scene of the camp and the map (0x2B4 bytes). The full class is in atn/CharaLightData_unk.cpp;
// only the constructor and the fields set up here are declared (pending: one header for it).
struct NewCampScene2_unk {
    NewCampScene2_unk(); // 0x800C1D98
    unsigned char m000[0x00C];
    float mAmbient[4];               // 0x00C: inferred: R, G, B (debug menu "Ambient")
    float mLS[4];                    // 0x01C: inferred: "LS R/G/B"
    unsigned char m02C[0x03C - 0x02C];
    int mFogMode;                    // 0x03C
    float mFogStart;                 // 0x040
    float mFogEnd;                   // 0x044
    float mFogColor[3];              // 0x048
    unsigned char m054[0x058 - 0x054];
    std::vector<Light_unk> mLights;  // 0x058
    unsigned char m064[0x2B4 - 0x064];
};

namespace atn {
// inferred: declared here only for their constructors; the classes are in atn/LayerShadow.cpp and
// atn/LayerOcclusionCPU.cpp, which use local copies of IRenderLayer (vf10 takes an argument there).
// Pending: one header for the layer classes.
class LayerShadow : public Layer3D {
public:
    LayerShadow(); // 0x800B6604 ("Shadow")
    unsigned char m5C[0x500 - 0x5C];
};
class LayerOcclusionCPU : public IRenderLayer {
public:
    LayerOcclusionCPU();
    virtual void vf10();
    virtual void vf14(void* arg);
    virtual void vf1C();
    virtual void vf20();
    unsigned char m14[0x30 - 0x14];
};
} // namespace atn
extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step, int arg6,
                              int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_800C2E30(void* scene, void* menu);
extern "C" Light_unk* fn_800C2448(void* scene, int index);

extern "C" void* fn_800BC874(void* renderSystem);
void fn_800C5644();
extern "C" void fn_8007A100();
extern "C" void fn_800BC618(void* renderSystem, GXTexObj* texture, int offset);
extern "C" int lbl_8087D72C;
extern "C" int lbl_8087D728;
extern "C" void* lbl_8087EEE0;
extern "C" void* memcpy(void* dst, const void* src, unsigned long size);

namespace atn {

class Scene;
} // namespace atn

typedef boost::function<atn::Scene*(atn::Sphere, void*)> RenderSceneCallback_unk;

struct RenderSceneCallbackOwner_unk {
    unsigned char m000[0x10C];
    atn::Vector3 m10C;
    atn::Vector3 m118;
    unsigned char m124[0x2FC - 0x124];
    atn::Scene* m2FC;
    RenderSceneCallback_unk m300;
    void* m314;
    unsigned char m318[0x54964 - 0x318];
    int m54964; // inferred: selects the camera position used by fn_800C1424
};
extern "C" atn::Scene* fn_800C122C(RenderSceneCallbackOwner_unk* render, atn::Sphere sphere);
extern "C" atn::Scene* fn_800C1424(RenderSceneCallbackOwner_unk* render);
extern "C" void fn_800C0AA4(unsigned char* data, unsigned int width, unsigned int height, unsigned int xShift, unsigned int yShift, unsigned int elementSize, unsigned int rowStride, unsigned char* scratch);
extern "C" void fn_800C0BF8(unsigned char* data, unsigned int width, unsigned int height, unsigned int xShift, unsigned int yShift, unsigned int elementSize, unsigned int rowStride, unsigned char* scratch);
extern "C" int fn_800C0D4C(unsigned char* image, unsigned int format, unsigned int width, unsigned int height);
extern "C" int fn_800C0FBC(unsigned char* image, unsigned int format, unsigned int width, unsigned int height);
extern "C" void fn_800BC618(void* render, GXTexObj* texture, int offset);

static inline int fn_800BF_sortBefore(void* left, void* right) {
    float leftOrder = *(float*)((unsigned char*)left + 4);
    float rightOrder = *(float*)((unsigned char*)right + 4);
    return leftOrder > rightOrder ||
           (leftOrder == rightOrder && (unsigned int)left < (unsigned int)right);
}

extern "C" void fn_800BF558(void** first, void** last, void* context);
extern "C" void fn_800BF9C4(void** first, void** second, void** third);
extern "C" void fn_800BF0EC(void** first, void** last, void* context);
extern "C" void fn_800BF558(void** first, void** last, void* context);
static inline int fn_800BF9C4_ordered(void* const* left, void* const* right) {
    void* a = *left;
    void* b = *right;
    float keyA = *(float*)((unsigned char*)a + 4);
    float keyB = *(float*)((unsigned char*)b + 4);
    return !(keyB > keyA || (keyB == keyA && (unsigned int)b < (unsigned int)a));
}
extern "C" void fn_800BF9C4(void** first, void** second, void** third);
extern "C" void* memset(void* dest, int value, unsigned long size);
extern "C" void fn_800BEDA8(unsigned int shift, const unsigned int* inputPairs, unsigned int* outputPairs, unsigned int count);
extern "C" void fn_800BECF0(void* first, unsigned int count);

extern "C" void GXClearVtxDesc();
extern "C" void GXSetVtxDesc(int attr, int type);
extern "C" void GXSetVtxAttrFmt(int fmt, int attr, int count, int type, unsigned char frac);
extern "C" void C_MTXOrtho(float matrix[4][4], float top, float bottom, float left, float right, float nearZ,
                             float farZ);
extern "C" void GXSetProjection(const float matrix[4][4], int type);
extern "C" void PSMTXIdentity(float matrix[3][4]);
extern "C" void GXLoadPosMtxImm(const float matrix[3][4], unsigned int id);
extern "C" void GXSetCurrentMtx(unsigned int id);
extern "C" void GXBegin(int type, int format, unsigned short count);
extern "C" void fn_80076760(void* render, int stage, int mode);
extern "C" void fn_800761A8(void* render);
extern "C" void fn_80076A28(void* render, void* previous);
extern "C" unsigned int __cvt_fp2unsigned(float value);

struct RenderColor_unk {
    RenderColor_unk() {}
    RenderColor_unk(const RenderColor_unk& color) {
        float r_ = color.r;
        float g_ = color.g;
        float b_ = color.b;
        float a_ = color.a;
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    float r, g, b, a;
};
static unsigned char fn_800BFCB4_colorByte(float value);

static inline unsigned int fn_800BFCB4_colorWord(const float* color) {
    return ((unsigned int)fn_800BFCB4_colorByte(color[3]) << 24) |
           ((unsigned int)fn_800BFCB4_colorByte(color[0]) << 16) |
           ((unsigned int)fn_800BFCB4_colorByte(color[1]) << 8) |
           (unsigned int)fn_800BFCB4_colorByte(color[2]);
}
extern "C" void fn_800BFCB4(void* render, float* c0, float* c1, float* c2, float* c3, unsigned int width, unsigned int height, float a, float b);
extern "C" void fn_800BFC18(void* render, RenderColor_unk color, float a, float b);
extern "C" void fn_800C08B0(void* render);
extern "C" void fn_800C0508(void* render, int clear);
extern "C" void fn_800C0590(void* render, int clear, int format);

struct RenderQueueEntry_unk {
    void* render;
    float order;
    RenderQueueEntry_unk* next;
};

struct RenderSystemPassTables_unk {
    void* active[20];
    RenderQueueEntry_unk* heads[20];
    unsigned int counts[20];
};
extern "C" void fn_800BDB58(void* renderSystem, void* render, float order, int pass);
extern "C" void fn_800BDBC8(void* renderSystem);
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos);
extern "C" void fn_800BC798();

extern "C" int fn_800B5994(void* value, const void* bounds);
extern "C" float fn_800B6218(void* value, const void* bounds);
extern "C" void fn_800C06B0(void* render, const void* fog);
extern "C" int fn_800C0A50(void* renderSystem, const void* bounds);
extern "C" void* fn_800C0A8C(void* value);
extern "C" void* fn_800C0A98(void* value);
extern "C" void fn_800BDB40(void* render, int value);
extern "C" void fn_800BFAC4();
extern "C" int fn_800C16B4();
extern "C" void fn_800C16BC();
extern "C" float fn_800C169C(void* renderSystem, const void* bounds);
extern "C" void fn_800C1660(void* render, void* tex, int format);

extern "C" void fn_800BC468(void* render, GXTexObj* tex) {
    // NONMATCHING: 80.88% - the reference unrolls the tiled gradient loop two pixels at a time.
    unsigned int size = GXGetTexBufferSize(256, 1, 1, 0, 0);
    unsigned char* image;
    if (*(void**)((unsigned char*)render + 0x550) == 0) {
        image = (unsigned char*)atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "",
                                             "", 0, 0);
    } else {
        image = (unsigned char*)*(void**)((unsigned char*)render + 0x550);
    }

    for (int i = 0; i < 256; ++i) {
        int value = (i - 128) * 4;
        if (value < lbl_8087D8F8) {
            value = lbl_8087D8F8;
        } else if (value > lbl_8087D8FC) {
            value = lbl_8087D8FC;
        }
        int offset = (i & 7) + ((i & 0x7C) << 2);
        image[offset] = (unsigned char)value;
        image[offset + 8] = (unsigned char)value;
        image[offset + 16] = (unsigned char)value;
        image[offset + 24] = (unsigned char)value;
    }

    GXInitTexObj(tex, image, 256, 1, 1, 0, 1, 0);
    GXInitTexObjLOD(tex, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    DCFlushRange(image, size);
}

extern "C" void fn_800BC618(void* render, GXTexObj* texture, int offset) {
    // NONMATCHING: 97.92% - two clamp branches still differ.
    unsigned short* remap;
    unsigned int size;
    size = GXGetTexBufferSize(256, 256, 3, 0, 0);
    remap = (unsigned short*)*(void**)((unsigned char*)render + 0x52C);
    if (remap == 0) {
        remap = (unsigned short*)atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6,
                                               "",
                                               "", 0, 0);
    }
    // inferred: 256x256 halfword remap table, with each source value clamped to 16 bits.
    for (unsigned int i = 0; i < 0x10000; ++i) {
        int value = (int)i + offset;
        if (value < 0) {
            value = 0;
        } else if (value > 0xFFFF) {
            value = 0xFFFF;
        }
        unsigned int index0 = ((unsigned int)value & 3) << 2;
        unsigned int index1 = (((unsigned int)value >> 2) & 0x3F) << 10;
        unsigned int index2 = (((unsigned int)value >> 10) & 0x3F) << 4;
        unsigned int index3 = ((unsigned int)value >> 8) & 3;
        unsigned int index = (index0 + index1) + (index2 + index3);
        remap[index] = (unsigned short)i;
    }
    GXInitTexObj(texture, remap, 256, 256, 3, 0, 1, 0);
    GXInitTexObjLOD(texture, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    DCFlushRange(remap, size);
    *(void**)((unsigned char*)render + 0x52C) = remap;
}

extern "C" void fn_800BC798() {
    fn_800B95C4();
    unsigned char progressiveMode = (unsigned char)SCGetProgressiveMode();
    if (progressiveMode == 1 && VIGetDTVStatus() == 1) {
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3B0) = 6;
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3B4) = 8;
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3B8) = 10;
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3BC) = 16;
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3C0) = 10;
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3C4) = 8;
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3C8) = 6;
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3AC) = 0;
    } else {
        *(int*)((unsigned char*)lbl_8087EFA8 + 0x3AC) = 1;
    }

    if (lbl_8087EFB4 == 0) {
        void* renderSystem = ::operator new(0x54968, (MemoryTag)6, "",
                                                "", 0);
        if (renderSystem != 0) {
            renderSystem = fn_800BC874(renderSystem);
        }
        lbl_8087EFB4 = renderSystem;
    }
}

extern "C" void* fn_800BC874(void* renderSystem) {
    // inferred: initialize render cameras, callbacks, the default scene, and its render layers.
    // NONMATCHING: 77.52% - render-state validation and additional menu fields remain incomplete;
    // "3D Outline" and "3D Object" store the Layer3D vtable once more in the original (class unknown).
    unsigned char* self = (unsigned char*)renderSystem;
    *(int*)(self + 0xF0) = 0;
    *(int*)(self + 0xF4) = 0;
    *(int*)(self + 0xF8) = 0;
    *(float*)(self + 0xFC) = 0.0f;
    *(float*)(self + 0x100) = 0.0f;
    new (self + 0x104) Camera_unk(0, 0);
    *(int*)(self + 0x2F8) = -1;
    *(int*)(self + 0x300) = 0;
    *(int*)(self + 0x314) = 0;
    new (self + 0x318) Camera_unk(0, 0);
    *(int*)(self + 0x52C) = 0;
    *(int*)(self + 0x550) = 0;
    *(int*)(self + 0x554) = 0;
    new (self + 0x558) Camera_unk(0, 0);
    new (self + 0x74C) atn::Texture_unk();
    *(float*)(self + 0x898) = 150.0f;
    *(float*)(self + 0x89C) = 500.0f;
    *(float*)(self + 0x8A0) = 0.5f;
    *(float*)(self + 0x8A4) = 0.5f;
    *(float*)(self + 0x8A8) = 0.5f;
    *(float*)(self + 0x8AC) = 1.0f;
    *(int*)(self + 0x890) = 0;
    *(int*)(self + 0x894) = 0;
    new (self + 0x8B0) RenderCallback_unk(boost::function<void(void*)>(), 0);
    new (self + 0x8C8) RenderCallback_unk(boost::function<void(void*)>(), 0);
    new (self + 0x8E0) RenderCallback_unk(boost::function<void(void*)>(), 0);
    new (self + 0x8F8) RenderCallback_unk(boost::function<void(void*)>(), 0);
    *(int*)(self + 0x910) = 0;
    *(int*)(self + 0x958) = 0;
    *(int*)(self + 0x895C) = 0;
    *(int*)(self + 0x54960) = 0;
    *(int*)(self + 0x54964) = 0;

    // inferred: 4096 fixed render records, each 0x4C bytes, begin at 0x8960.
    struct RenderRecord_unk {
        int field_00;
        int field_04;
        int field_08;
        int field_0C;
        int field_10;
        int field_14;
        int field_18;
        int field_1C;
        int field_20;
        float field_24;
        float field_28;
        float field_2C;
        float field_30;
        int field_34;
        const char* name;
        int field_3C;
        int field_40;
        int field_44;
        int field_48;
    };
    RenderRecord_unk* recordsEnd = (RenderRecord_unk*)(self + 0x54960);
    RenderRecord_unk* records = (RenderRecord_unk*)(self + 0x8960);
    for (; records != recordsEnd; ++records) {
        records->field_04 = 0;
        records->field_08 = 0;
        records->field_0C = 0;
        records->field_10 = 0;
        records->field_14 = 0;
        records->field_18 = 0;
        records->field_1C = 0;
        records->field_20 = 0;
        records->field_24 = 1.0f;
        records->field_28 = 1.0f;
        records->field_2C = 1.0f;
        records->field_30 = 1.0f;
        records->field_34 = 0;
        records->name = (const char*)lbl_8087D81C;
        records->field_3C = 0;
        records->field_40 = 0;
        records->field_44 = 0;
        records->field_48 = 0;
        records->field_00 = 0;
    }
    memset(self + 0x50, 0, 0x50);
    memset(self + 0xA0, 0, 0x50);
    memset(self, 0, 0x50);
    NewCampScene2_unk* scene = new ((MemoryTag)6, "", "", 0) NewCampScene2_unk();
    lbl_8087EFB8 = (int)scene;
    *(void**)(self + 0x2FC) = scene;
    Light_unk light;
    atn::Vector3 direction((-2.0f), (-3.0f), 4.0f);
    atn::Vector3 normalizedDirection;
    PSVECNormalize(&direction, &normalizedDirection);
    LightColor_unk color(1.0f, 1.0f, 1.0f, 1.0f);
    light.fn_800790D0(normalizedDirection, color);
    scene->mLights.push_back(light);

    void* sceneMenu = *(void**)((unsigned char*)lbl_8087EFA8 + 0x4C);
    void* menuGroup = fn_8008937C(sceneMenu, "Reflection");
    fn_800C2E30(scene, menuGroup);

    menuGroup = fn_8008937C(sceneMenu, "Fog");
    fn_800874C8(menuGroup, "Fog Mode", &scene->mFogMode, 0, 6, 1,
                0, 0);
    fn_8008771C(menuGroup, "Fog Start", &scene->mFogStart, 0, 0,
                0.0f, 10000.0f, 1.0f);
    fn_8008771C(menuGroup, "Fog End", &scene->mFogEnd, 0, 0,
                0.0f, 10000.0f, 1.0f);
    fn_8008771C(menuGroup, "Fog R", &scene->mFogColor[0], 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Fog G", &scene->mFogColor[1], 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Fog B", &scene->mFogColor[2], 0, 0,
                0.0f, 1.0f, 0.01f);

    menuGroup = fn_8008937C(sceneMenu, "KeyLight");
    fn_800C2448(scene, 0)->fn_80079BC8(menuGroup);
    menuGroup = fn_8008937C(sceneMenu, "Ambient");
    fn_8008771C(menuGroup, "Ambient R", &scene->mAmbient[0], 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Ambient G", &scene->mAmbient[1], 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Ambient B", &scene->mAmbient[2], 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "LS R", &scene->mLS[0], 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "LS G", &scene->mLS[1], 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "LS B", &scene->mLS[2], 0, 0,
                0.0f, 1.0f, 0.01f);

    // The layer pointer table occupies the first 20 words of RenderSystem.
    atn::IRenderLayer** layers = (atn::IRenderLayer**)self;
    layers[0] = new ((MemoryTag)6, "", "", 0) atn::LayerOcclusionCPU();
    layers[1] = new ((MemoryTag)6, "", "", 0) atn::Layer2D("Composite");
    layers[2] = new ((MemoryTag)6, "", "", 0) atn::LayerPrerender();
    layers[3] = new ((MemoryTag)6, "", "", 0) atn::LayerShadow();
    layers[4] = new ((MemoryTag)6, "", "", 0) atn::LayerReflect();
    layers[5] = 0;
    layers[10] = new ((MemoryTag)6, "", "", 0) atn::Layer3D("3D Outline");
    layers[6] = new ((MemoryTag)6, "", "", 0) atn::Layer3D("3D Object", 9, -1);
    layers[7] = new ((MemoryTag)6, "", "", 0) atn::LayerZPrePass();
    layers[8] = new ((MemoryTag)6, "", "", 0) atn::Layer3D("3D Alpha");
    layers[9] = new ((MemoryTag)6, "", "", 0) atn::LayerPost3D();
    layers[11] = new ((MemoryTag)6, "", "", 0) atn::Layer3D("Effect");
    layers[12] = new ((MemoryTag)6, "", "", 0) atn::Layer2D("2D Effect");
    layers[13] = new ((MemoryTag)6, "", "", 0) atn::Layer3D("3D Debug");
    layers[14] = new ((MemoryTag)6, "", "", 0) atn::LayerPostFx();
    layers[15] = new ((MemoryTag)6, "", "", 0) atn::Layer2DBlur();
    layers[18] = new ((MemoryTag)6, "", "", 0) atn::Layer2D("2D Object");
    atn::Layer2D3D* layer2D3D = new ((MemoryTag)6, "", "", 0) atn::Layer2D3D();
    if (layer2D3D != 0) {
        layer2D3D->m14 = 0;
    }
    layers[16] = layer2D3D;
    layers[17] = new ((MemoryTag)6, "", "", 0) atn::LayerPostFx();
    layers[19] = new ((MemoryTag)6, "", "", 0) atn::Layer2D("Debug");

    void* layerMenu = fn_8008937C(*(void**)((unsigned char*)lbl_8087EFA8 + 0x4C), "Layers");
    for (int i = 0; i < 20; ++i) {
        if (layers[i] != 0) {
            layers[i]->vf0C(layerMenu);
        }
    }
    return renderSystem;
}

RenderCallback_unk::RenderCallback_unk(boost::function<void(void*)> func, int arg) : mFunc(func), mArg(arg) {}

extern "C" void fn_800BD9C0(void* render, int queueCapacity) {
    // NONMATCHING: 93.59% - temporary texture setup still differs in register allocation.
    const char* file = "";
    {
        MemoryMode_unk mode(0, 0x20);
        *(int*)((unsigned char*)render + 0xF4) = queueCapacity;
        *(void**)((unsigned char*)render + 0xF0) =
            ::operator new[](queueCapacity, (MemoryTag)6, (const char*)&lbl_8087D72C,
                               (const char*)&lbl_8087D728, 0);
    }
    fn_800C5644();
    fn_8007A100();

    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    unsigned int size = GXGetTexBufferSize((unsigned short)width, (unsigned short)height, 0x13, 0, 0);
    void* depth = atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "",
                              "", 0, 0);
    *(void**)((unsigned char*)render + 0x79C) = depth;
    ((atn::Texture_unk*)((unsigned char*)render + 0x74C))
        ->fn_800D5B58(width, height, 6, 1, "FrameBuffer");
    GXInitTexObjLOD((GXTexObj*)((unsigned char*)render + 0x74C), 1, 1, 0.0f, 0.0f,
                    0.0f, 0, 0, 0);
    fn_800BC468(render, (GXTexObj*)((unsigned char*)render + 0x530));
    fn_800BC618(render, (GXTexObj*)((unsigned char*)render + 0x50C),
                *(int*)((unsigned char*)lbl_8087EFA8 + 0x154));
    GXInitTexObj((GXTexObj*)((unsigned char*)render + 0x77C),
                 *(void**)((unsigned char*)render + 0x79C), (unsigned short)width,
                 (unsigned short)height, 3, 0, 0, 0);
    GXInitTexObjLOD((GXTexObj*)((unsigned char*)render + 0x77C), 0, 0, 0.0f, 0.0f,
                    0.0f, 0, 0, 0);
}

extern "C" void fn_800BDB40(void* render, int value) {
    int* field = (int*)((unsigned char*)render + 0x2FC);
    *field = value;
    if (value != 0) {
        return;
    }
    *field = lbl_8087EFB8;
}

extern "C" void fn_800BDB58(void* renderSystem, void* render, float order, int pass) {
    // NONMATCHING: 97.50% - the remaining differences are register allocation.
    unsigned char* system = (unsigned char*)renderSystem;
    RenderSystemPassTables_unk* passes = (RenderSystemPassTables_unk*)renderSystem;
    if (passes->active[pass] == 0) {
        return;
    }

    int tail = *(int*)(system + 0xF8);
    int next = tail + 0x10;
    RenderQueueEntry_unk* entry;
    if (next > *(int*)(system + 0xF4)) {
        entry = 0;
    } else {
        void* base = *(void**)(system + 0xF0);
        *(int*)(system + 0xF8) = next;
        entry = (RenderQueueEntry_unk*)((unsigned char*)base + tail);
    }
    if (entry == 0) {
        return;
    }
    entry->render = render;
    entry->order = order;
    entry->next = 0;
    entry->next = passes->heads[pass];
    passes->heads[pass] = entry;
    ++passes->counts[pass];
}

extern "C" void fn_800BDBC8(void* renderSystem) {
    // inferred: collect, sort, and relink the queued render entries for each of 20 passes.
    // poolcheck: MWCC folds the safe-area products (640.0f * 0.078f...); the original multiplies at run
    // time, so its operands were not literals there (source form unknown).
    // NONMATCHING: 72.91% - remaining render-state details and drawing callbacks still differ.
    unsigned char* system = (unsigned char*)renderSystem;
    Mtx matrixWork[8];
    void** entries = 0;
    unsigned int capacity = 0;
    unsigned char* passSystem = system;

    for (int pass = 0; pass < 20; ++pass) {
        unsigned int requested = *(unsigned int*)(passSystem + 0xA0);
        if (requested >= capacity) {
            unsigned int newCapacity = requested;
            void** replacement = (void**)::operator new[](
                newCapacity * 4, (MemoryTag)0, (const char*)lbl_8087D914, (const char*)lbl_8087D910, 0);
            for (unsigned int i = 0; i < capacity; ++i) {
                replacement[i] = entries[i];
            }
            if (entries != 0) {
                ::operator delete[](entries);
            }
            entries = replacement;
            capacity = newCapacity;
        }

        unsigned int count = 0;
        void* node = *(void**)(passSystem + 0x50);
        while (node != 0) {
            if (count == capacity) {
                unsigned int newCapacity = capacity == 0 ? 8 : capacity * 2;
                void** replacement = (void**)::operator new[](
                    newCapacity * 4, (MemoryTag)0, (const char*)lbl_8087D914, (const char*)lbl_8087D910, 0);
                for (unsigned int i = 0; i < count; ++i) {
                    replacement[i] = entries[i];
                }
                if (entries != 0) {
                    ::operator delete[](entries);
                }
                entries = replacement;
                capacity = newCapacity;
            }
            entries[count++] = node;
            node = *(void**)((unsigned char*)node + 8);
        }

        if (count != 0) {
            unsigned char ascending = 0;
            fn_800BF0EC(entries, entries + count, &ascending);
            for (unsigned int i = 0; i < count; ++i) {
                void* next = i + 1 < count ? entries[i + 1] : 0;
                *(void**)((unsigned char*)entries[i] + 8) = next;
            }
            *(void**)(passSystem + 0x50) = entries[0];
        } else {
            *(void**)(passSystem + 0x50) = 0;
        }
        passSystem += 4;
    }

    if (entries != 0) {
        ::operator delete[](entries);
    }

    atn::DrawHelper* debugDrawer = lbl_8087EEB0;
    void* renderState = lbl_8087EEE0;
    if (*(int*)((unsigned char*)lbl_8087EFA8 + 0x1C) != 0 &&
        *(int*)((unsigned char*)renderState + 0x44) != 0) {
        float half = 0.5f;
        float extent = 640.0f - 514.2857f;
        float left = 640.0f - half * extent;
        float right = half * extent;
        *(int*)((unsigned char*)debugDrawer + 0xA0) = 0x13;
        debugDrawer->fn_8006144C(left, 0.0f, left, 448.0f, 0xFFFF0000, 0.0f);
        debugDrawer->fn_8006144C(right, 448.0f, right, 0.0f, 0xFFFF0000, 0.0f);
        *(int*)((unsigned char*)debugDrawer + 0xA0) = 0x12;
    }

    if (*(int*)((unsigned char*)lbl_8087EFA8 + 0x18) != 0) {
        float left = 640.0f * 0.07800001f;
        float top = 448.0f * 0.095999986f;
        float right = 640.0f * (1.0f - 0.07800001f);
        float bottom = 448.0f * (1.0f - 0.095999986f);
        *(int*)((unsigned char*)debugDrawer + 0xA0) = 0x13;
        debugDrawer->fn_8006144C(left, top, right, top, 0xFFFFFF00, 0.0f);
        debugDrawer->fn_8006144C(right, top, right, bottom, 0xFFFFFF00, 0.0f);
        debugDrawer->fn_8006144C(right, bottom, left, bottom, 0xFFFFFF00, 0.0f);
        debugDrawer->fn_8006144C(left, bottom, left, top, 0xFFFFFF00, 0.0f);

        left = 640.0f * 0.065f;
        top = 448.0f * 0.065f;
        right = 640.0f * (1.0f - 0.065f);
        bottom = 448.0f * (1.0f - 0.065f);
        *(int*)((unsigned char*)debugDrawer + 0xA0) = 0x12;
        debugDrawer->fn_8006144C(left, top, right, top, 0xFFFFFFFF, 0.0f);
        debugDrawer->fn_8006144C(right, top, right, bottom, 0xFFFFFFFF, 0.0f);
        debugDrawer->fn_8006144C(right, bottom, left, bottom, 0xFFFFFFFF, 0.0f);
        debugDrawer->fn_8006144C(left, bottom, left, top, 0xFFFFFFFF, 0.0f);
    }

    *(int*)((unsigned char*)lbl_8087EF8C + 0x54) = 0;
    unsigned char* manager = (unsigned char*)lbl_8087EFA8;
    unsigned char* managerState = (unsigned char*)lbl_8087EEE0;
    float colorValue = *(float*)(manager + 0x3C);
    unsigned int red = 0;
    if (colorValue >= 1.0f) {
        red = 0xFF;
    } else if (!(colorValue <= 0.0f)) {
        red = (unsigned int)(255.0f * colorValue + 0.5f);
    }
    colorValue = *(float*)(manager + 0x40);
    unsigned int green = 0;
    if (colorValue >= 1.0f) {
        green = 0xFF;
    } else if (!(colorValue <= 0.0f)) {
        green = (unsigned int)(255.0f * colorValue + 0.5f);
    }
    colorValue = *(float*)(manager + 0x44);
    unsigned int blue = 0;
    if (colorValue >= 1.0f) {
        blue = 0xFF;
    } else if (!(colorValue <= 0.0f)) {
        blue = (unsigned int)(255.0f * colorValue + 0.5f);
    }
    colorValue = *(float*)(manager + 0x48);
    unsigned int alpha = 0;
    if (colorValue >= 1.0f) {
        alpha = 0xFF;
    } else if (!(colorValue <= 0.0f)) {
        alpha = (unsigned int)(255.0f * colorValue + 0.5f);
    }
    *(unsigned int*)(managerState + 0x48) =
        (alpha << 24) | (red << 16) | (green << 8) | blue;
    fn_8007192C(managerState);

    *(void**)(system + 0x2FC) = 0;
    *(void**)(system + 0x2FC) = (void*)lbl_8087EFB8;
    RenderCallback_unk* beforeLayers = (RenderCallback_unk*)(system + 0x8B0);
    if (*(void**)beforeLayers != 0) {
        beforeLayers->mFunc((void*)beforeLayers->mArg);
    }
    *(float*)(system + 0xFC) = 0.0f;
    *(float*)(system + 0x100) = 0.0f;
    fn_80075DEC(lbl_8087EEE0);
    if (*(int*)((unsigned char*)lbl_8087EFA8 + 4) != 0) {
        fn_8007A4CC__13LightList_unkFv(lbl_8087EEF8);
    }

    passSystem = system;
    for (int pass = 0; pass < 20; ++pass, passSystem += 4) {
        if (pass == 3 && *(int*)((unsigned char*)lbl_8087EFA8 + 0x104) == 0) {
            continue;
        }
        if (pass == 4 && *(int*)(system + 0x54960) == 0) {
            continue;
        }
        *(int*)(system + 0x2F8) = pass;
        void* layer = *(void**)passSystem;
        if (layer != 0 && *(int*)((unsigned char*)layer + 4) != 0) {
            typedef void (*LayerQueueCall)(void*, void*);
            ((LayerQueueCall)(*(void***)layer)[4])(layer, *(void**)(passSystem + 0x50));
        }
    }

    // Prepare the renderer's per-frame projection matrices.
    fn_800D5F68(matrixWork[2], 1, -1, -1, 0);
    __MtxCopy(*(Mtx*)(system + 0x800), matrixWork[2]);
    if (*(int*)(system + 0x554) != 0) {
        __MtxCopy(matrixWork[0], *(Mtx*)(system + 0x1D4));
        matrixWork[0][0][3] = 0.0f;
        matrixWork[0][1][3] = 0.0f;
        matrixWork[0][2][3] = 0.0f;
        __MtxCopy(matrixWork[1], *(Mtx*)(system + 0x5B0));
        matrixWork[1][0][3] = 0.0f;
        matrixWork[1][1][3] = 0.0f;
        matrixWork[1][2][3] = 0.0f;
        PSMTXConcat(matrixWork[1], matrixWork[0], matrixWork[4]);
        PSMTXConcat(*(Mtx*)(system + 0x800), matrixWork[4], matrixWork[3]);
        __MtxCopy(*(Mtx*)(system + 0x800), matrixWork[3]);
    }
    fn_800D5F68(matrixWork[5], 2, -1, -1, 0);
    __MtxCopy(*(Mtx*)(system + 0x7A0), matrixWork[5]);
    fn_800D5F68(matrixWork[6], 3, -1, -1, 0);
    __MtxCopy(*(Mtx*)(system + 0x7D0), matrixWork[6]);
    fn_800D5F68(matrixWork[7], 4, -1, -1, 0);
    __MtxCopy(*(Mtx*)(system + 0x830), matrixWork[7]);
    *(float*)(system + 0x860) = 1.0f;
    *(float*)(system + 0x864) = 0.0f;
    *(float*)(system + 0x868) = 0.0f;
    *(float*)(system + 0x86C) = 0.0f;
    *(float*)(system + 0x870) = 0.0f;
    *(float*)(system + 0x874) = 1.0f;
    *(float*)(system + 0x878) = 0.0f;
    *(float*)(system + 0x87C) = 0.0f;
    *(float*)(system + 0x880) = 0.0f;
    *(float*)(system + 0x884) = 0.0f;
    *(float*)(system + 0x888) = 1.0f;
    *(float*)(system + 0x88C) = 0.0f;

    LCEnable();
    unsigned char* layerSlot = system;
    for (int pass = 0; pass < 20; ++pass, layerSlot += 4) {
        *(int*)(system + 0x890) = 0;
        if (pass == 3 && *(int*)((unsigned char*)lbl_8087EFA8 + 0x104) == 0) {
            continue;
        }
        if (pass == 4 && *(int*)(system + 0x54960) == 0) {
            continue;
        }
        *(int*)(system + 0x2F8) = pass;
        void* layer = *(void**)layerSlot;
        void* queue = *(void**)(layerSlot + 0x50);
        if (layer != 0 && *(int*)((unsigned char*)layer + 4) != 0 && queue != 0) {
            typedef void (*LayerCall)(void*);
            typedef void (*LayerQueueCall)(void*, void*);
            *(int*)(system + 0x958) = 0;
            *(int*)(system + 0x895C) = 0;
            ((LayerCall)(*(void***)layer)[7])(layer);
            ((LayerQueueCall)(*(void***)layer)[5])(layer, queue);
            if (*(int*)((unsigned char*)lbl_8087EFA8 + 0x39C) != 0 &&
                *(int*)(system + 0x895C) != 0) {
                fn_800BECF0(system + 0x95C, *(unsigned int*)(system + 0x958));
            }
            unsigned int batchCount = *(unsigned int*)(system + 0x895C);
            for (unsigned int i = 0; i < batchCount; ++i) {
                unsigned int index = *(unsigned int*)(system + 0x960 + i * 8);
                fn_8009DE60(system + 0x8960 + index * 0x4C, 0.0f);
            }
            ((LayerQueueCall)(*(void***)layer)[6])(layer, queue);
            ((LayerCall)(*(void***)layer)[8])(layer);
        }

        unsigned int textureCount = *(unsigned int*)(system + 0x910);
        for (unsigned int i = 0; i < textureCount; ++i) {
            unsigned char* copy = system + 0x914 + i * 8;
            if (*(unsigned int*)(copy + 4) == pass) {
                fn_80071D80(lbl_8087EEE0, *(void**)copy);
            }
        }
    }
    LCDisable();

    RenderCallback_unk* afterLayers = (RenderCallback_unk*)(system + 0x8C8);
    if (*(void**)afterLayers != 0) {
        afterLayers->mFunc((void*)afterLayers->mArg);
    }
    memset(system + 0x50, 0, 0x50);
    memset(system + 0xA0, 0, 0x50);
    *(int*)(system + 0xF8) = 0;
    *(int*)((unsigned char*)lbl_8087EEB0 + 0xC) = 0;
    *(int*)(system + 0x910) = 0;
    *(int*)(system + 0x54960) = 0;
    RenderCallback_unk* afterCleanup = (RenderCallback_unk*)(system + 0x8E0);
    if (*(void**)afterCleanup != 0) {
        afterCleanup->mFunc((void*)afterCleanup->mArg);
    }
    fn_80071AFC(lbl_8087EEE0);
    RenderCallback_unk* finalCallback = (RenderCallback_unk*)(system + 0x8F8);
    if (*(void**)finalCallback != 0) {
        finalCallback->mFunc((void*)finalCallback->mArg);
    }
}

extern "C" void fn_800BECF0(void* first, unsigned int count) {
    // inferred: apply the four quadrant transforms using a temporary stack buffer.
    // NONMATCHING: 93.24% - scratch pointer arithmetic and call argument setup still differ.
    unsigned char scratch[0x8000];
    unsigned char* scratchData = scratch + 8;
    fn_800BEDA8(0, (const unsigned int*)first, (unsigned int*)scratchData, count);
    fn_800BEDA8(8, (const unsigned int*)scratchData, (unsigned int*)first, count);
    fn_800BEDA8(16, (const unsigned int*)first, (unsigned int*)scratchData, count);
    fn_800BEDA8(24, (const unsigned int*)scratchData, (unsigned int*)first, count);
}

static unsigned char fn_800BFCB4_colorByte(float value) {
    if (value >= 1.0f) {
        return 0xFF;
    }
    if (value <= 0.0f) {
        return 0;
    }
    return (unsigned char)__cvt_fp2unsigned(255.0f * value + 0.5f);
}

extern "C" void fn_800BEDA8(unsigned int shift, const unsigned int* inputPairs,
                              unsigned int* outputPairs, unsigned int count) {
    // inferred: stable counting-sort pass over 8-byte key/value records, grouped by one key byte.
    // NONMATCHING: 37.60% - the compiler's loop and register layout still differ from the unrolled reference.
    unsigned int histogram[256];
    memset(histogram, 0, sizeof(histogram));
    {
        const unsigned int* input = inputPairs;
        unsigned int groups = count >> 3;
        if (groups != 0) {
        do {
            ++histogram[(input[0] >> shift) & 0xFF];
            ++histogram[(input[2] >> shift) & 0xFF];
            ++histogram[(input[4] >> shift) & 0xFF];
            ++histogram[(input[6] >> shift) & 0xFF];
            ++histogram[(input[8] >> shift) & 0xFF];
            ++histogram[(input[10] >> shift) & 0xFF];
            ++histogram[(input[12] >> shift) & 0xFF];
            ++histogram[(input[14] >> shift) & 0xFF];
            input += 16;
        } while (--groups != 0);
        }
        for (unsigned int tail = count & 7; tail != 0; --tail, input += 2) {
            unsigned int bucket = (*input >> shift) & 0xFF;
            ++histogram[bucket];
        }
    }
    unsigned int offset = 0;
    unsigned int* bucketPointer = histogram;
    for (unsigned int i = 0; i < 256; ++i, ++bucketPointer) {
        unsigned int entries = *bucketPointer;
        *bucketPointer = offset;
        offset += entries;
    }

    {
    const unsigned int* input = inputPairs;
    unsigned int groups = count >> 2;
    if (groups != 0) {
    do {
        unsigned int key = input[0];
        unsigned int value = input[1];
        unsigned int bucket = (key >> shift) & 0xFF;
        unsigned int destination = histogram[bucket]++;
        outputPairs[destination * 2] = key;
        outputPairs[destination * 2 + 1] = value;
        key = input[2];
        value = input[3];
        bucket = (key >> shift) & 0xFF;
        destination = histogram[bucket]++;
        outputPairs[destination * 2] = key;
        outputPairs[destination * 2 + 1] = value;
        key = input[4];
        value = input[5];
        bucket = (key >> shift) & 0xFF;
        destination = histogram[bucket]++;
        outputPairs[destination * 2] = key;
        outputPairs[destination * 2 + 1] = value;
        key = input[6];
        value = input[7];
        bucket = (key >> shift) & 0xFF;
        destination = histogram[bucket]++;
        outputPairs[destination * 2] = key;
        outputPairs[destination * 2 + 1] = value;
        input += 8;
    } while (--groups != 0);
    }
    for (unsigned int tail = count & 3; tail != 0; --tail, input += 2) {
        unsigned int key = input[0];
        unsigned int value = input[1];
        unsigned int bucket = (key >> shift) & 0xFF;
        unsigned int destination = histogram[bucket]++;
        outputPairs[destination * 2] = key;
        outputPairs[destination * 2 + 1] = value;
    }
    }
}

extern "C" void fn_800BF0EC(void** first, void** last, void* context) {
    // inferred: first MSL __sort instantiation for render-entry pointers; small ranges use selection sort.
    // NONMATCHING: 90.08% - MWCC register allocation and partition loops still differ.
    for (;;) {
        int count = (int)(last - first);
        if (count <= 1) {
            return;
        }
        if (count <= 20) {
            if (first != last) {
                for (void** end = last - 1; first != end; ++first) {
                    void** minimum = first;
                    for (void** current = first + 1; current != last; ++current) {
                        if (fn_800BF_sortBefore(*current, *minimum)) {
                            minimum = current;
                        }
                    }
                    if (minimum != first) {
                        void* value = *minimum;
                        *minimum = *first;
                        *first = value;
                    }
                }
            }
            return;
        }

        int* state = &lbl_8087D900;
        void** a = first + (count / 4) + (*state % 5);
        if (++*state >= 5) {
            *state = -4;
        }
        void** b = first + ((count * 3) / 4) + (*state % 5);
        if (++*state >= 5) {
            *state = -4;
        }
        void** pivot = last - 1;
        fn_800BF9C4(a, b, pivot);

        void** i = first;
        void** j;
        while (fn_800BF_sortBefore(*i, *pivot)) {
            ++i;
        }
        j = pivot;
        while (i != --j && !fn_800BF_sortBefore(*j, *pivot)) {
        }
        if (i < j) {
            void* value = *i;
            *i = *j;
            *j = value;
            ++i;
            for (;;) {
                while (fn_800BF_sortBefore(*i, *pivot)) {
                    ++i;
                }
                while (!fn_800BF_sortBefore(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                value = *i;
                *i = *j;
                *j = value;
                ++i;
            }
        }

        if (i == first) {
            void** back = last - 1;
            void* value = *i;
            *i = *pivot;
            *pivot = value;
            ++i;
            if (!fn_800BF_sortBefore(*first, *back)) {
                while (i != last && !fn_800BF_sortBefore(*first, *i)) {
                    ++i;
                }
                if (i < back) {
                    value = *i;
                    *i = *back;
                    *back = value;
                }
            }
            j = back;
            if (i < j) {
                for (;;) {
                    while (!fn_800BF_sortBefore(*first, *i)) {
                        ++i;
                    }
                    while (fn_800BF_sortBefore(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    value = *i;
                    *i = *j;
                    *j = value;
                    ++i;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            fn_800BF558(first, i, context);
            first = i;
        } else {
            fn_800BF558(i, last, context);
            last = i;
        }
    }
}

extern "C" void fn_800BF558(void** first, void** last, void* context) {
    // inferred: recursive MSL __sort instantiation for render-entry pointers.
    // NONMATCHING: 90.08% - MWCC register allocation and partition loops still differ.
    for (;;) {
        int count = (int)(last - first);
        if (count <= 1) {
            return;
        }
        if (count <= 20) {
            if (first != last) {
                for (void** end = last - 1; first != end; ++first) {
                    void** minimum = first;
                    for (void** current = first + 1; current != last; ++current) {
                        if (fn_800BF_sortBefore(*current, *minimum)) {
                            minimum = current;
                        }
                    }
                    if (minimum != first) {
                        void* value = *minimum;
                        *minimum = *first;
                        *first = value;
                    }
                }
            }
            return;
        }

        int* state = &lbl_8087D904;
        void** a = first + (count / 4) + (*state % 5);
        if (++*state >= 5) {
            *state = -4;
        }
        void** b = first + ((count * 3) / 4) + (*state % 5);
        if (++*state >= 5) {
            *state = -4;
        }
        void** pivot = last - 1;
        fn_800BF9C4(a, b, pivot);

        void** i = first;
        void** j;
        while (fn_800BF_sortBefore(*i, *pivot)) {
            ++i;
        }
        j = pivot;
        while (i != --j && !fn_800BF_sortBefore(*j, *pivot)) {
        }
        if (i < j) {
            void* value = *i;
            *i = *j;
            *j = value;
            ++i;
            for (;;) {
                while (fn_800BF_sortBefore(*i, *pivot)) {
                    ++i;
                }
                while (!fn_800BF_sortBefore(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                value = *i;
                *i = *j;
                *j = value;
                ++i;
            }
        }

        if (i == first) {
            void** back = last - 1;
            void* value = *i;
            *i = *pivot;
            *pivot = value;
            ++i;
            if (!fn_800BF_sortBefore(*first, *back)) {
                while (i != last && !fn_800BF_sortBefore(*first, *i)) {
                    ++i;
                }
                if (i < back) {
                    value = *i;
                    *i = *back;
                    *back = value;
                }
            }
            j = back;
            if (i < j) {
                for (;;) {
                    while (!fn_800BF_sortBefore(*first, *i)) {
                        ++i;
                    }
                    while (fn_800BF_sortBefore(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    value = *i;
                    *i = *j;
                    *j = value;
                    ++i;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            fn_800BF558(first, i, context);
            first = i;
        } else {
            fn_800BF558(i, last, context);
            last = i;
        }
    }
}

extern "C" void fn_800BF9C4(void** first, void** second, void** third) {
    // inferred: three-entry ordering network for render keys; NONMATCHING: 91.88% - MWCC folds comparison flags differently.
    int firstOrdered = fn_800BF9C4_ordered(first, third);
    int secondOrdered = fn_800BF9C4_ordered(second, third);
    if (firstOrdered != 0 && secondOrdered != 0) {
        return;
    }
    if (firstOrdered == 0 && secondOrdered == 0) {
        void* value = *first;
        *first = *second;
        *second = value;
        return;
    }
    if (fn_800BF9C4_ordered(third, first) != 0) {
        void* value = *first;
        *first = *second;
        *second = value;
    }
    if (firstOrdered != 0) {
        void* value = *second;
        *second = *third;
        *third = value;
    } else {
        void* value = *first;
        *first = *third;
        *third = value;
    }
}

extern "C" void fn_800BFAC4() {}

extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos) {
    float projection[7];
    projection[0] = 0.0f; // inferred: GX_PERSPECTIVE
    projection[1] = *(float*)((unsigned char*)camera + 0x18C);
    projection[2] = *(float*)((unsigned char*)camera + 0x194);
    projection[3] = *(float*)((unsigned char*)camera + 0x1A0);
    projection[4] = *(float*)((unsigned char*)camera + 0x1A4);
    projection[5] = *(float*)((unsigned char*)camera + 0x1B4);
    projection[6] = *(float*)((unsigned char*)camera + 0x1B8);
    float viewport[6];
    GXGetViewportv(viewport);
    GXProject(pos->x, pos->y, pos->z, (const float (*)[4])((unsigned char*)camera + 0x15C), projection, viewport,
              &out->x, &out->y, &out->z);
}

extern "C" void fn_800BFB70(void* render) {
    // inferred: invoke the common projection helper with six all-ones work vectors and zero scalar bounds.
    // NONMATCHING: 81.90% - local vector placement and initialization order differ.
    float one = 1.0f;
    float vectors[6][4] = {
        {one, one, one, one},
        {one, one, one, one},
        {one, one, one, one},
        {one, one, one, one},
        {one, one, one, one},
        {one, one, one, one},
    };
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    fn_800BFCB4(render, vectors[4], vectors[3], vectors[2], vectors[1], width, height,
                0.0f, 0.0f);
}

extern "C" void fn_800BFC18(void* render, RenderColor_unk color, float a, float b) {
    // inferred: duplicate the supplied four-float vector into the work arrays before projection setup.
    // NONMATCHING: 94.87% - the width load is scheduled before the reference's array setup.
    float alpha, blue, g, r;
    r = color.r;
    g = color.g;
    blue = color.b;
    alpha = color.a;
    float vectors[5][4];
    float* first = vectors[0];
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    vectors[4][0] = r; vectors[4][1] = g; vectors[4][2] = blue; vectors[4][3] = alpha;
    vectors[3][0] = r; vectors[3][1] = g; vectors[3][2] = blue; vectors[3][3] = alpha;
    vectors[2][0] = r; vectors[2][1] = g; vectors[2][2] = blue; vectors[2][3] = alpha;
    vectors[1][0] = r; vectors[1][1] = g; vectors[1][2] = blue; vectors[1][3] = alpha;
    vectors[0][0] = r; vectors[0][1] = g; vectors[0][2] = blue; vectors[0][3] = alpha;
    fn_800BFCB4(render, first, vectors[1], vectors[2], vectors[3], width, height, a, b);
}

extern "C" void fn_800BFCB4(void* render, float* c0, float* c1, float* c2, float* c3,
                              unsigned int width, unsigned int height, float a, float b) {
    // inferred: set an orthographic projection and draw a four-corner colored screen quad.
    // NONMATCHING: 78.71% - color conversion and FIFO vertex writes still differ from the SDK call sequence.
    void* global = lbl_8087EEE0;
    unsigned int previous = *(unsigned int*)((unsigned char*)global + 0xD0);
    fn_80076760(global, 8, 1);
    fn_80076760(global, 9, 7);
    fn_80076760(global, 10, 0);
    fn_800761A8(global);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);

    unsigned int colors[4];
    colors[0] = fn_800BFCB4_colorWord(c0);
    colors[1] = fn_800BFCB4_colorWord(c1);
    colors[2] = fn_800BFCB4_colorWord(c2);
    colors[3] = fn_800BFCB4_colorWord(c3);
    unsigned char* colorBytes = (unsigned char*)colors;

    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);

    float projection[4][4];
    C_MTXOrtho(projection, 0.0f, (float)height, 0.0f, (float)width, 0.0f, lbl_8087D908);
    GXSetProjection(projection, 1);
    float position[3][4];
    PSMTXIdentity(position);
    GXLoadPosMtxImm(position, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);

    volatile unsigned char* fifo = (volatile unsigned char*)0xCC008000;
    *(volatile float*)fifo = a;
    *(volatile float*)fifo = b;
    *(volatile float*)fifo = lbl_8087D90C;
    fifo[0] = colorBytes[0]; fifo[0] = colorBytes[1]; fifo[0] = colorBytes[2]; fifo[0] = colorBytes[3];
    *(volatile float*)fifo = 0.0f;
    *(volatile float*)fifo = 0.0f;

    *(volatile float*)fifo = (float)width + a;
    *(volatile float*)fifo = b;
    *(volatile float*)fifo = lbl_8087D90C;
    fifo[0] = colorBytes[4]; fifo[0] = colorBytes[5]; fifo[0] = colorBytes[6]; fifo[0] = colorBytes[7];
    *(volatile float*)fifo = 1.0f;
    *(volatile float*)fifo = 0.0f;

    *(volatile float*)fifo = (float)width;
    *(volatile float*)fifo = (float)height + b;
    *(volatile float*)fifo = lbl_8087D90C;
    fifo[0] = colorBytes[8]; fifo[0] = colorBytes[9]; fifo[0] = colorBytes[10]; fifo[0] = colorBytes[11];
    *(volatile float*)fifo = 1.0f;
    *(volatile float*)fifo = 1.0f;

    *(volatile float*)fifo = 0.0f;
    *(volatile float*)fifo = (float)height;
    *(volatile float*)fifo = lbl_8087D90C;
    fifo[0] = colorBytes[12]; fifo[0] = colorBytes[13]; fifo[0] = colorBytes[14]; fifo[0] = colorBytes[15];
    *(volatile float*)fifo = 0.0f;
    *(volatile float*)fifo = 1.0f;
    fn_80076A28(global, &previous);
}

extern "C" void fn_800C0508(void* render, int clear) {
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    GXSetTexCopySrc(0, 0, (unsigned short)width, (unsigned short)height);
    GXSetTexCopyDst((unsigned short)width, (unsigned short)height, 6, 0);
    GXCopyTex(*(void**)((unsigned char*)render + 0x774), (unsigned char)clear);
    GXPixModeSync();
}

extern "C" void fn_800C0590(void* render, int clear, int format) {
    // NONMATCHING: texture object address is kept in a register instead of recalculated.
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    GXSetTexCopySrc(0, 0, (unsigned short)width, (unsigned short)height);
    GXSetTexCopyDst((unsigned short)width, (unsigned short)height, format, 0);
    GXCopyTex(*(void**)((unsigned char*)render + 0x79C), (unsigned char)clear);
    GXPixModeSync();

    GXTexObj* tex = (GXTexObj*)((unsigned char*)render + 0x77C);
    if (format != 0x16) {
        GXInitTexObj(tex, *(void**)((unsigned char*)render + 0x79C), (unsigned short)width,
                     (unsigned short)height, 3, 0, 0, 0);
        GXInitTexObjLOD(tex, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    } else {
        GXInitTexObj(tex, *(void**)((unsigned char*)render + 0x79C), (unsigned short)width,
                     (unsigned short)height, 0x16, 0, 0, 0);
        GXInitTexObjLOD(tex, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    }
}

extern "C" void fn_800C06B0(void* render, const void* fog) {
    // NONMATCHING: 94.60% - conversion spill placement and early GX argument setup still differ.
    if (*(int*)((unsigned char*)lbl_8087EFA8 + 0x2C) != 0) {
        if (*(int*)((unsigned char*)render + 0x890) != 0) {
            *(int*)((unsigned char*)render + 0x890) = 0;
            unsigned int color = 0;
            float farZ = *(float*)((unsigned char*)render + 0x1D0);
            float nearZ = *(float*)((unsigned char*)render + 0x1CC);
            GXSetFog(0, &color, 0.0f, 0.0f, nearZ, farZ);
        }
        return;
    }

    unsigned char* bytes = (unsigned char*)render;
    if (*(int*)(bytes + 0x890) != 0 && *(const int*)fog == *(int*)(bytes + 0x894) &&
        *(const float*)((const unsigned char*)fog + 4) == *(float*)(bytes + 0x898) &&
        *(const float*)((const unsigned char*)fog + 8) == *(float*)(bytes + 0x89C) &&
        *(const float*)((const unsigned char*)fog + 0xC) == *(float*)(bytes + 0x8A0) &&
        *(const float*)((const unsigned char*)fog + 0x10) == *(float*)(bytes + 0x8A4) &&
        *(const float*)((const unsigned char*)fog + 0x14) == *(float*)(bytes + 0x8A8)) {
        return;
    }
    int fogType = *(const int*)fog;
    *(int*)(bytes + 0x894) = fogType;
    *(float*)(bytes + 0x898) = *(const float*)((const unsigned char*)fog + 4);
    *(float*)(bytes + 0x89C) = *(const float*)((const unsigned char*)fog + 8);
    *(int*)(bytes + 0x8A0) = *(const int*)((const unsigned char*)fog + 0xC);
    *(int*)(bytes + 0x8A4) = *(const int*)((const unsigned char*)fog + 0x10);
    *(int*)(bytes + 0x8A8) = *(const int*)((const unsigned char*)fog + 0x14);
    *(int*)(bytes + 0x8AC) = *(const int*)((const unsigned char*)fog + 0x18);
    *(int*)(bytes + 0x890) = 1;

    int type = 0;
    switch (fogType) {
    case 0: type = 0; break;
    case 1: type = 2; break;
    case 2: type = 4; break;
    case 3: type = 5; break;
    case 4: type = 6; break;
    case 5: type = 7; break;
    }
    unsigned char colorBytes[4] = {0, 0, 0, 0};
    unsigned int* color = (unsigned int*)(colorBytes + 8);
    const float scale = 255.0f;
    float red = *(float*)(bytes + 0x8A0);
    float green = *(float*)(bytes + 0x8A4);
    float blue = *(float*)(bytes + 0x8A8);
    float alpha = *(float*)(bytes + 0x8AC);
    colorBytes[0] = (unsigned char)(int)(red * scale);
    colorBytes[1] = (unsigned char)(int)(green * scale);
    colorBytes[2] = (unsigned char)(int)(blue * scale);
    colorBytes[3] = (unsigned char)(int)(alpha * scale);
    *color = *(unsigned int*)colorBytes;
    GXSetFog(type, color, *(float*)(bytes + 0x898), *(float*)(bytes + 0x89C),
             *(float*)(bytes + 0x1CC), *(float*)(bytes + 0x1D0));
}

extern "C" void fn_800C08B0(void* render) {
    // NONMATCHING: 90.00% - only the stack color-pointer setup remains out of order.
    if (*(int*)((unsigned char*)render + 0x890) != 0) {
        *(int*)((unsigned char*)render + 0x890) = 0;
        unsigned int color = 0;
        float farZ = *(float*)((unsigned char*)render + 0x1D0);
        float nearZ = *(float*)((unsigned char*)render + 0x1CC);
        GXSetFog(0, &color, 0.0f, 0.0f, nearZ, farZ);
    }
}

extern "C" void fn_800C0900(Mtx out, void* render, int mode, int a, int b, int c) {
    switch (mode) {
    case 1:
        *(atn::Matrix3x4*)out = *(atn::Matrix3x4*)((unsigned char*)render + 0x800);
        break;
    case 2:
        *(atn::Matrix3x4*)out = *(atn::Matrix3x4*)((unsigned char*)render + 0x7A0);
        break;
    case 3:
        *(atn::Matrix3x4*)out = *(atn::Matrix3x4*)((unsigned char*)render + 0x7D0);
        break;
    case 4:
        if (a <= 0) {
            *(atn::Matrix3x4*)out = *(atn::Matrix3x4*)((unsigned char*)render + 0x830);
        } else {
            fn_800D5F68(out, 4, a, b, c);
        }
        break;
    default:
        *(atn::Matrix3x4*)out = *(atn::Matrix3x4*)((unsigned char*)render + 0x860);
        break;
    }
}

extern "C" int fn_800C0A50(void* renderSystem, const void* bounds) {
    void* pass = *(void**)renderSystem;
    if (pass != 0) {
        return !fn_800B5994(pass, bounds);
    }
    return 1;
}

extern "C" void* fn_800C0A8C(void* value) {
    return (unsigned char*)*(void**)((unsigned char*)value + 0x10) + 0x5C;
}

extern "C" void* fn_800C0A98(void* value) {
    return (unsigned char*)*(void**)((unsigned char*)value + 0x14) + 0x5C;
}

extern "C" void fn_800C0AA4(unsigned char* data, unsigned int width, unsigned int height,
                               unsigned int xShift, unsigned int yShift, unsigned int elementSize,
                               unsigned int rowStride, unsigned char* scratch) {
    // inferred: rearranges one tiled block through a temporary row buffer; NONMATCHING: 94.59% - loop register allocation differs.
    register unsigned char* output = data;
    register unsigned int pixelSize = elementSize;
    register unsigned int stride = rowStride;
    register unsigned char* temporary = scratch;
    unsigned int blocksX = width >> xShift;
    unsigned int blocksY = height >> yShift;
    unsigned int scratchSize = stride << yShift;
    unsigned int rows = 1 << yShift;
    for (unsigned int blockY = 0; blockY < blocksY; ++blockY) {
        memcpy(temporary, output, scratchSize);
        unsigned int sourceOffset = 0;
        for (unsigned int blockX = 0; blockX < blocksX; ++blockX) {
            unsigned char* sourceRow = temporary + sourceOffset;
            for (unsigned int row = 0; row < rows; ++row) {
                for (unsigned int byte = 0; byte < pixelSize; ++byte) {
                    output[byte] = sourceRow[byte];
                }
                output += pixelSize;
                sourceRow += stride;
            }
            sourceOffset += pixelSize;
        }
    }
}

extern "C" void fn_800C0BF8(unsigned char* data, unsigned int width, unsigned int height,
                               unsigned int xShift, unsigned int yShift, unsigned int elementSize,
                               unsigned int rowStride, unsigned char* scratch) {
    // inferred: inverse tiled-block rearrangement; NONMATCHING: 92.94% - loop register allocation differs.
    unsigned char* input = data;
    unsigned int pixelSize = elementSize;
    unsigned int stride = rowStride;
    unsigned char* temporary = scratch;
    unsigned int blocksX = width >> xShift;
    unsigned int blocksY = height >> yShift;
    unsigned int scratchSize = stride << yShift;
    unsigned int rows = 1 << yShift;
    for (unsigned int blockY = 0; blockY < blocksY; ++blockY) {
        unsigned int sourceOffset = 0;
        for (unsigned int blockX = 0; blockX < blocksX; ++blockX) {
            unsigned char* destRow = temporary + sourceOffset;
            for (unsigned int row = 0; row < rows; ++row) {
                for (unsigned int byte = 0; byte < pixelSize; ++byte) {
                    destRow[byte] = input[byte];
                }
                input += pixelSize;
                destRow += stride;
            }
            sourceOffset += pixelSize;
        }
        memcpy(input, temporary, scratchSize);
    }
}

extern "C" int fn_800C0D4C(unsigned char* image, unsigned int format, unsigned int width,
                             unsigned int height) {
    // inferred: converts supported GX texture layouts to tiled storage and flushes the image.
    // NONMATCHING: 87.99% - switch dispatch and loop scheduling still differ from MWCC.
    unsigned char scratch[0x2000];
    switch (format) {
    case 0:
    case 8:
        fn_800C0AA4(image, width, height, 3, 3, 4, width >> 1, scratch);
        DCFlushRange(image, (width * height) >> 1);
        break;
    case 1:
    case 2:
    case 9:
        fn_800C0AA4(image, width, height, 3, 2, 8, width, scratch);
        DCFlushRange(image, width * height);
        break;
    case 3:
    case 4:
    case 5:
        fn_800C0AA4(image, width, height, 2, 2, 8, width << 1, scratch);
        DCFlushRange(image, (width * height) << 1);
        break;
    case 6: {
        unsigned int blocksWide = width >> 2;
        unsigned int blocksHigh = height >> 2;
        unsigned int rowPairCount = 2;
        unsigned char* row = image;
        for (unsigned int blockY = 0; blockY < blocksHigh; ++blockY) {
            memcpy(scratch, row, width * 16);
            unsigned int sourceOffset = 0;
            unsigned char* tile = row;
            for (unsigned int blockX = 0; blockX < blocksWide; ++blockX) {
                const unsigned char* source = scratch + sourceOffset;
                for (unsigned int rowPair = 0; rowPair < rowPairCount; ++rowPair) {
                    tile[0] = source[0];
                    tile[1] = source[1];
                    tile[32] = source[2];
                    tile[33] = source[3];
                    tile[2] = source[4];
                    tile[3] = source[5];
                    tile[34] = source[6];
                    tile[35] = source[7];
                    tile[4] = source[8];
                    tile[5] = source[9];
                    tile[36] = source[10];
                    tile[37] = source[11];
                    tile[6] = source[12];
                    tile[7] = source[13];
                    tile[38] = source[14];
                    tile[39] = source[15];
                    source += width * 4;
                    tile += 16;
                }
                sourceOffset += 16;
                tile += 32;
            }
            row += width * 16;
        }
        DCFlushRange(image, (width * height) << 2);
        break;
    }
    }
    return 1;
}

extern "C" int fn_800C0FBC(unsigned char* image, unsigned int format, unsigned int width,
                             unsigned int height) {
    // inferred: converts supported GX texture layouts back to linear storage and flushes the image.
    // NONMATCHING: 87.72% - switch dispatch and tile loop scheduling still differ from MWCC.
    unsigned char scratch[0x2000];
    switch (format) {
    case 0:
    case 8:
        fn_800C0BF8(image, width, height, 3, 3, 4, width >> 1, scratch);
        DCFlushRange(image, (width * height) >> 1);
        break;
    case 1:
    case 2:
        fn_800C0BF8(image, width, height, 3, 2, 8, width, scratch);
        DCFlushRange(image, width * height);
        break;
    case 3:
    case 4:
    case 5:
        fn_800C0BF8(image, width, height, 2, 2, 8, width << 1, scratch);
        DCFlushRange(image, (width * height) << 1);
        break;
    case 6:
    case 22: {
        unsigned int blocksWide = width >> 2;
        unsigned int blocksHigh = height >> 2;
        unsigned int rowPairCount = 2;
        unsigned char* row = image;
        unsigned char* tile = image;
        for (unsigned int blockY = 0; blockY < blocksHigh; ++blockY) {
            unsigned int destOffset = 0;
            for (unsigned int blockX = 0; blockX < blocksWide; ++blockX) {
                unsigned char* dest = scratch + destOffset;
                for (unsigned int rowPair = 0; rowPair < rowPairCount; ++rowPair) {
                    dest[0] = tile[0];
                    dest[1] = tile[1];
                    dest[2] = tile[32];
                    dest[3] = tile[33];
                    dest[4] = tile[2];
                    dest[5] = tile[3];
                    dest[6] = tile[34];
                    dest[7] = tile[35];
                    dest[8] = tile[4];
                    dest[9] = tile[5];
                    dest[10] = tile[36];
                    dest[11] = tile[37];
                    dest[12] = tile[6];
                    dest[13] = tile[7];
                    dest[14] = tile[38];
                    dest[15] = tile[39];
                    dest += width * 4;
                    dest[0] = tile[8];
                    dest[1] = tile[9];
                    dest[2] = tile[40];
                    dest[3] = tile[41];
                    dest[4] = tile[10];
                    dest[5] = tile[11];
                    dest[6] = tile[42];
                    dest[7] = tile[43];
                    dest[8] = tile[12];
                    dest[9] = tile[13];
                    dest[10] = tile[44];
                    dest[11] = tile[45];
                    dest[12] = tile[14];
                    dest[13] = tile[15];
                    dest[14] = tile[46];
                    dest[15] = tile[47];
                    tile += 16;
                    dest += width * 4;
                }
                tile += 32;
                destOffset += 16;
            }
            memcpy(row, scratch, width * 16);
            row = tile;
        }
        DCFlushRange(image, (width * height) << 2);
        break;
    }
    case 9:
        fn_800C0BF8(image, width, height, 3, 2, 8, width, scratch);
        DCFlushRange(image, width * height);
        break;
    }
    return 1;
}

extern "C" atn::Scene* fn_800C122C(RenderSceneCallbackOwner_unk* render, atn::Sphere sphere) {
    // inferred: run the configured sphere-to-scene callback and fall back to the stored scene.
    // NONMATCHING: 96.75% - callback invoker data label and aggregate load order differ.
    if (render->m300) {
        atn::Scene* scene = render->m300(sphere, render->m314);
        if (scene != 0) {
            return scene;
        }
    }
    return render->m2FC;
}

extern "C" atn::Scene* fn_800C1424(RenderSceneCallbackOwner_unk* render) {
    // inferred: build a unit sphere at the selected camera position and query the scene callback.
    // NONMATCHING: 95.92% - callback invoker data and temporary stack layout differ.
    const atn::Vector3& position = render->m54964 != 0 ? render->m10C : render->m118;
    atn::Sphere sourceSphere;
    sourceSphere.mCenter.Assign_unk(position);
    sourceSphere.mRadius = 1.0f;
    atn::Sphere sphere(sourceSphere);
    if (render->m300) {
        atn::Scene* scene = render->m300(sphere, render->m314);
        if (scene != 0) {
            return scene;
        }
    }
    return render->m2FC;
}

extern "C" void fn_800C1660(void* render, void* tex, int format) {
    // inferred: preserve the by-value texture/format temporary before enqueueing the pair.
    struct TextureFormatPair {
        void* texture;
        int format;
    };
    volatile TextureFormatPair pair;
    pair.texture = tex;
    pair.format = format;
    int* count = (int*)((unsigned char*)render + 0x910);
    unsigned char* entry = (unsigned char*)render + 0x914 + *count * 8;
    if (entry != 0) {
        *(void**)entry = tex;
        *(int*)(entry + 4) = format;
    }
    ++*count;
}

extern "C" float fn_800C169C(void* renderSystem, const void* bounds) {
    void* pass = *(void**)renderSystem;
    if (pass != 0) {
        return fn_800B6218(pass, bounds);
    }
    return 0.0f;
}

extern "C" int fn_800C16B4() {
    return lbl_8087EFB8;
}

extern "C" void fn_800C16BC() {}
