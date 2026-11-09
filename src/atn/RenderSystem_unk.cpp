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
#include "atn/Array_unk.h"
#include "atn/GraphicsDevice.h"

extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0; // inferred: graphics device
#include "atn/Sphere.h"
#include "atn/Texture_unk.h"
#include "math/Vec.h"
#include "std/vector.h"
#include "algorithm"
#include <boost/function.hpp>
#include <new>
#include "atn/DrawHelper.h"
#include "atn/RenderManager.h"
#include "atn/Scene.h"


struct MemoryMode_unk {
    MemoryMode_unk(int mode, unsigned long align);
    ~MemoryMode_unk();
};

extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                             unsigned char mipmap, unsigned char maxLod);
extern "C" void DCFlushRange(void* address, unsigned long size);
extern "C" int lbl_8087D8F8;
extern "C" int lbl_8087D8FC;
extern "C" float lbl_8087D908;
extern "C" float lbl_8087D90C;
extern "C" unsigned char lbl_8087D81C[];
extern "C" atn::Scene* lbl_8087EFB8; // inferred: default scene
#include "atn/RenderSetting_unk.h"

extern "C" RenderSetting_unk* lbl_8087EFA8; // render settings
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
extern "C" void fn_800BECF0(DrawSortKey_unk* keys, unsigned int count);
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
// inferred: draw order of the pass lists (std::sort comparator of fn_800BDBC8): higher mOrder first, then
// by address
struct RenderEntryLess_unk {
    bool operator()(const atn::RenderEntry_unk* x, const atn::RenderEntry_unk* y) const {
        return x->mOrder > y->mOrder || (x->mOrder == y->mOrder && x < y);
    }
};
template <class T, class Compare>
void Sort_unk(T* first, T* last, Compare comp);

extern "C" void* fn_8008937C(void* menu, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step, int arg6,
                              int arg7);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min,
                            float max, float step);

void fn_800C5644();
extern "C" void fn_8007A100();
extern "C" int lbl_8087D72C;
extern "C" int lbl_8087D728;
extern "C" void* memcpy(void* dst, const void* src, unsigned long size);

namespace atn {

class Scene;
} // namespace atn

extern "C" void fn_800C0AA4(unsigned char* data, unsigned int width, unsigned int height, unsigned int xShift, unsigned int yShift, unsigned int elementSize, unsigned int rowStride, unsigned char* scratch);
extern "C" void fn_800C0BF8(unsigned char* data, unsigned int width, unsigned int height, unsigned int xShift, unsigned int yShift, unsigned int elementSize, unsigned int rowStride, unsigned char* scratch);
extern "C" int fn_800C0D4C(unsigned char* image, unsigned int format, unsigned int width, unsigned int height);
extern "C" int fn_800C0FBC(unsigned char* image, unsigned int format, unsigned int width, unsigned int height);


extern "C" void* memset(void* dest, int value, unsigned long size);
extern "C" void fn_800BEDA8(int shift, const DrawSortKey_unk* in, DrawSortKey_unk* out, unsigned int count);

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
extern "C" void fn_80076760(atn::GraphicsDevice_unk* device, unsigned int field, unsigned int value);
extern "C" void fn_800761A8(atn::GraphicsDevice_unk* device);
extern "C" void fn_80076A28(atn::GraphicsDevice_unk* device, const atn::GXStateBits_unk* state);
extern "C" unsigned int __cvt_fp2unsigned(float value);



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
extern "C" void fn_800BC798();

extern "C" int fn_800B5994(void* value, const void* bounds);
extern "C" float fn_800B6218(void* value, const void* bounds);

// inferred: clamp with the bounds passed by reference (each bound is read twice; name invented)
static inline int ClampRef_unk(int value, const int& lo, const int& hi) {
    return value < lo ? lo : (value > hi ? hi : value);
}

void atn::RenderManager::fn_800BC468(GXTexObj* tex) {
    // inferred: 256x1 I8 ramp texture (4 rows of each 8x4 tile written)
    unsigned char* image;
    unsigned int size = GXGetTexBufferSize(256, 1, 1, 0, 0);
    if (m550 == 0) {
        image = (unsigned char*)atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "", "", 0,
                                                                            0);
    } else {
        image = (unsigned char*)m550;
    }

    for (int i = 0; i < 256; ++i) {
        int row = (i >> 3) * 32;
        int col = i & 7;
        int value = (i - 128) * 4;
        unsigned char c = ClampRef_unk(value, lbl_8087D8F8, lbl_8087D8FC);
        image[row + col] = c;
        image[row + col + 8] = c;
        image[row + col + 16] = c;
        image[row + col + 24] = c;
    }

    GXInitTexObj(tex, image, 256, 1, 1, 0, 1, 0);
    GXInitTexObjLOD(tex, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    DCFlushRange(image, size);
}

void atn::RenderManager::fn_800BC618(GXTexObj* texture, int offset) {
    // NONMATCHING: 97.92% - two clamp branches still differ.
    unsigned short* remap;
    unsigned int size;
    size = GXGetTexBufferSize(256, 256, 3, 0, 0);
    remap = (unsigned short*)*(void**)((unsigned char*)this + 0x52C);
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
    *(void**)((unsigned char*)this + 0x52C) = remap;
}

extern "C" void fn_800BC798() {
    fn_800B95C4();
    unsigned char progressiveMode = (unsigned char)SCGetProgressiveMode();
    if (progressiveMode == 1 && VIGetDTVStatus() == 1) {
        lbl_8087EFA8->m3B0[0] = 6;
        lbl_8087EFA8->m3B0[1] = 8;
        lbl_8087EFA8->m3B0[2] = 10;
        lbl_8087EFA8->m3B0[3] = 16;
        lbl_8087EFA8->m3B0[4] = 10;
        lbl_8087EFA8->m3B0[5] = 8;
        lbl_8087EFA8->m3B0[6] = 6;
        lbl_8087EFA8->m3AC = 0;
    } else {
        lbl_8087EFA8->m3AC = 1;
    }

    if (atn::RenderManager::instance_ == 0) {
        atn::RenderManager::instance_ = new ((MemoryTag)6, "", "", 0) atn::RenderManager();
    }
}

// inferred: inlined in the constructor of the render system (0x1000 commands)
inline DrawCmd_unk::DrawCmd_unk() {
    m04 = 0;
    m08 = 0;
    m0C = 0;
    m10 = 0;
    m14 = 0;
    m18 = 0;
    m1C = 0;
    m20 = 0;
    m24.m[0] = 1.0f;
    m24.m[1] = 1.0f;
    m24.m[2] = 1.0f;
    m24.m[3] = 1.0f;
    m34 = 0;
    m38 = (void*)lbl_8087D81C;
    m3C.mFunc1 = 0;
    m3C.mFunc2 = 0;
    m3C.mArg1 = 0;
    m3C.mArg2 = 0;
    m00 = 0;
}

atn::RenderManager::RenderManager()
    : mQueueBuffer(0), mQueueCapacity(0), mQueueUsed(0), m0FC(0.0f, 0.0f), mCamera(0, 0), m2F8(-1),
      mMapHitArg(0), mShadowCamera(0, 0), m52C(0), m550(0), m554(0), m558(0, 0), m890(0), m894(0), m898(150.0f),
      m89C(500.0f), m8A0(0.5f), m8A4(0.5f), m8A8(0.5f), m8AC(1.0f),
      m8B0(boost::function<void(void*)>(), 0), m8C8(boost::function<void(void*)>(), 0),
      m8E0(boost::function<void(void*)>(), 0), m8F8(boost::function<void(void*)>(), 0), mResolveCount(0), m54960(0),
      m54964(0) {
    // inferred: initialize render cameras, callbacks, the default scene, and its render layers.
    // NONMATCHING: 89.6%, equivalent - the default DrawCmd_unk of mCmds (the original builds its color and
    // callback as temporaries and tests m08 for the sort key, as fn_800BC438), the light is copied before
    // the push_back (Scene::AddLight_unk; inlining it here exceeds the inline budget) and registers.
    unsigned char* self = (unsigned char*)this;
    mKeys.mSize = 0;
    mCmds.mSize = 0;

    memset(self + 0x50, 0, 0x50);
    memset(self + 0xA0, 0, 0x50);
    memset(self, 0, 0x50);
    atn::Scene* scene = new ((MemoryTag)6, "", "", 0) atn::Scene();
    lbl_8087EFB8 = scene;
    *(void**)(self + 0x2FC) = scene;
    Light_unk light;
    atn::Vector3 direction = atn::Vector3(-2.0f, -3.0f, 4.0f);
    direction.Normalize_unk();
    light.fn_800790D0(direction, LightColor_unk(1.0f, 1.0f, 1.0f, 1.0f));
    mScene->mLights.push_back(light);

    void* menuGroup = fn_8008937C(lbl_8087EFA8->m04C, "Reflection");
    mScene->fn_800C2E30(menuGroup);

    menuGroup = fn_8008937C(lbl_8087EFA8->m04C, "Fog");
    fn_800874C8(menuGroup, "Fog Mode", &mScene->mFog.mMode, 0, 6, 1,
                0, 0);
    fn_8008771C(menuGroup, "Fog Start", &mScene->mFog.mStart, 0, 0,
                0.0f, 10000.0f, 1.0f);
    fn_8008771C(menuGroup, "Fog End", &mScene->mFog.mEnd, 0, 0,
                0.0f, 10000.0f, 1.0f);
    fn_8008771C(menuGroup, "Fog R", &mScene->mFog.mColor.r, 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Fog G", &mScene->mFog.mColor.g, 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Fog B", &mScene->mFog.mColor.b, 0, 0,
                0.0f, 1.0f, 0.01f);

    menuGroup = fn_8008937C(lbl_8087EFA8->m04C, "KeyLight");
    mScene->GetLight(0)->fn_80079BC8(menuGroup);
    menuGroup = fn_8008937C(lbl_8087EFA8->m04C, "Ambient");
    fn_8008771C(menuGroup, "Ambient R", &mScene->mAmbient.r, 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Ambient G", &mScene->mAmbient.g, 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "Ambient B", &mScene->mAmbient.b, 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "LS R", &mScene->mLS.r, 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "LS G", &mScene->mLS.g, 0, 0,
                0.0f, 1.0f, 0.01f);
    fn_8008771C(menuGroup, "LS B", &mScene->mLS.b, 0, 0,
                0.0f, 1.0f, 0.01f);

    // The layer pointer table occupies the first 20 words of RenderSystem.
    atn::IRenderLayer** layers = (atn::IRenderLayer**)self;
    layers[2] = new ((MemoryTag)6, "", "", 0) atn::LayerPrerender();
    layers[3] = new ((MemoryTag)6, "", "", 0) atn::LayerShadow();
    layers[0] = new ((MemoryTag)6, "", "", 0) atn::LayerOcclusionCPU();
    layers[4] = new ((MemoryTag)6, "", "", 0) atn::LayerReflect();
    layers[5] = 0;
    layers[1] = new ((MemoryTag)6, "", "", 0) atn::Layer2D("Composite");
    // inferred: m14 is 0 in the original for "3D Outline" and Layer2D3D (stored by their inlined
    // constructors; a fourth constructor argument in atn/Layer3D.h renumbers the @N labels of its users)
    atn::Layer3D* outline = new ((MemoryTag)6, "", "", 0) atn::Layer3D("3D Outline", -1, 1);
    if (outline != 0) {
        outline->m14 = 0;
    }
    layers[10] = outline;
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

    void* layerMenu = fn_8008937C(lbl_8087EFA8->m04C, "Layers");
    for (int i = 0; i < 20; ++i) {
        if (layers[i] != 0) {
            layers[i]->vf0C(layerMenu);
        }
    }
}

atn::RenderManager::FrameCallbackEntry::FrameCallbackEntry(boost::function<void(void*)> func, int arg)
    : mFunc(func), mArg(arg) {}

void atn::RenderManager::fn_800BD9C0(int queueCapacity, const std::string& name) {
    // inferred: allocates the draw queue buffer and the frame buffer, depth and light textures
    {
        MemoryMode_unk mode(0, 0x20);
        mQueueCapacity = queueCapacity;
        mQueueBuffer = (unsigned char*)::operator new[](queueCapacity, (MemoryTag)6,
                                                         (const char*)&lbl_8087D72C,
                                                         (const char*)&lbl_8087D728, 0);
    }
    fn_800C5644();
    fn_8007A100();

    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    unsigned int size = GXGetTexBufferSize((unsigned short)width, (unsigned short)height, 0x13, 0, 0);
    m79C = atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "", "", 0, 0);
    m74C.fn_800D5B58(width, height, 6, 1, "FrameBuffer");
    GXInitTexObjLOD(&m74C.mObj, 1, 1, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    fn_800BC468(&m530);
    fn_800BC618(&m50C, lbl_8087EFA8->m154);
    GXInitTexObj(&m77C, m79C, (unsigned short)width, (unsigned short)height, 3, 0, 0, 0);
    GXInitTexObjLOD(&m77C, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
}

void atn::RenderManager::fn_800BDB40(atn::Scene* scene) {
    mScene = scene;
    if (scene != 0) {
        return;
    }
    mScene = lbl_8087EFB8;
}


// inferred: bump allocation from the queue buffer (null when full); name invented
inline void* atn::RenderManager::AllocQueue_unk(int size) {
    int used = mQueueUsed;
    int next = used + size;
    if (next > mQueueCapacity) {
        return 0;
    }
    mQueueUsed = next;
    return mQueueBuffer + used;
}

void atn::RenderManager::AddRender(atn::IRender* render, int pass, float order) {
    if (mLayers[pass] == 0) {
        return;
    }
    atn::RenderEntry_unk* entry = (atn::RenderEntry_unk*)AllocQueue_unk(sizeof(atn::RenderEntry_unk));
    if (entry == 0) {
        return;
    }
    entry->mRender = render;
    entry->mOrder = order;
    entry->mNext = 0;
    entry->mNext = mPassHeads[pass];
    mPassHeads[pass] = entry;
    mPassCounts[pass]++;
}

// inferred: converts a color channel to a byte (inlined four times by fn_800BDBC8)
static inline unsigned int ColorChannel_unk(float value) {
    if (value >= 1.0f) {
        return 0xFF;
    }
    if (value <= 0.0f) {
        return 0;
    }
    return (unsigned int)(255.0f * value + 0.5f);
}

// inferred: draws the frame: sorts the draw lists of the passes, then lets each layer queue and draw them.
// The layers are called through their vtables (slots 0x10/0x14/0x18 take the draw list): the local copies
// of IRenderLayer in atn/LayerShadow.cpp and atn/LayerOcclusionCPU.cpp disagree on those signatures.
// poolcheck: MWCC folds the safe-area products (640.0f * 0.078f...); the original multiplies at run
// time, so its operands were not literals there (source form unknown).
// NONMATCHING: equivalent - register allocation and the folded safe-area constants
void atn::RenderManager::fn_800BDBC8() {
    typedef void (*LayerCall)(IRenderLayer*);
    typedef void (*LayerListCall)(IRenderLayer*, RenderEntry_unk*);
    atn::GraphicsDevice_unk* device = lbl_8087EEE0;

    {
        atn::SimpleVector<atn::RenderEntry_unk*> entries;
        for (int pass = 0; pass < 20; ++pass) {
            entries.reserve(mPassCounts[pass]);
            atn::RenderEntry_unk* entry = mPassHeads[pass];
            if (entry != 0) {
                entries.mSize = 0;
                while (entry != 0) {
                    entries.push_back(entry);
                    entry = entry->mNext;
                }
                Sort_unk(&entries[0], &entries[0] + entries.size(), RenderEntryLess_unk());
                for (unsigned int i = 0; i < entries.size(); ++i) {
                    if (i == entries.size() - 1) {
                        entries[i]->mNext = 0;
                    } else {
                        entries[i]->mNext = entries[i + 1];
                    }
                }
                mPassHeads[pass] = entries[0];
            }
        }
    }

    if (lbl_8087EFA8->m01C != 0 && device->mWide != 0) {
        float half = 0.5f;
        float extent = 640.0f - 514.2857f;
        float left = 640.0f - half * extent;
        float right = half * extent;
        lbl_8087EEB0->mA0 = 0x13;
        lbl_8087EEB0->fn_8006144C(left, 0.0f, left, 448.0f, 0xFFFF0000, 0.0f);
        lbl_8087EEB0->fn_8006144C(right, 448.0f, right, 0.0f, 0xFFFF0000, 0.0f);
        lbl_8087EEB0->mA0 = 0x12;
    }

    if (lbl_8087EFA8->m018 != 0) {
        float left = 640.0f * 0.07800001f;
        float top = 448.0f * 0.095999986f;
        float right = 640.0f * (1.0f - 0.07800001f);
        float bottom = 448.0f * (1.0f - 0.095999986f);
        lbl_8087EEB0->mA0 = 0x13;
        lbl_8087EEB0->fn_8006144C(left, top, right, top, 0xFFFFFF00, 0.0f);
        lbl_8087EEB0->fn_8006144C(right, top, right, bottom, 0xFFFFFF00, 0.0f);
        lbl_8087EEB0->fn_8006144C(right, bottom, left, bottom, 0xFFFFFF00, 0.0f);
        lbl_8087EEB0->fn_8006144C(left, bottom, left, top, 0xFFFFFF00, 0.0f);

        left = 640.0f * 0.065f;
        top = 448.0f * 0.065f;
        right = 640.0f * (1.0f - 0.065f);
        bottom = 448.0f * (1.0f - 0.065f);
        lbl_8087EEB0->mA0 = 0x12;
        lbl_8087EEB0->fn_8006144C(left, top, right, top, 0xFFFFFFFF, 0.0f);
        lbl_8087EEB0->fn_8006144C(right, top, right, bottom, 0xFFFFFFFF, 0.0f);
        lbl_8087EEB0->fn_8006144C(right, bottom, left, bottom, 0xFFFFFFFF, 0.0f);
        lbl_8087EEB0->fn_8006144C(left, bottom, left, top, 0xFFFFFFFF, 0.0f);
    }

    *(int*)((unsigned char*)lbl_8087EF8C + 0x54) = 0;
    unsigned int red = ColorChannel_unk(lbl_8087EFA8->m03C.r);
    unsigned int green = ColorChannel_unk(lbl_8087EFA8->m03C.g);
    unsigned int blue = ColorChannel_unk(lbl_8087EFA8->m03C.b);
    unsigned int alpha = ColorChannel_unk(lbl_8087EFA8->m03C.a);
    device->mClearColor = (alpha << 24) | (red << 16) | (green << 8) | blue;
    fn_8007192C(device);

    fn_800BDB40(0);
    if (m8B0.mFunc != 0) {
        m8B0.mFunc((void*)m8B0.mArg);
    }
    m0FC = Vector2_unk(0.0f, 0.0f);
    fn_80075DEC(lbl_8087EEE0);
    if (lbl_8087EFA8->m004 != 0) {
        fn_8007A4CC__13LightList_unkFv(lbl_8087EEF8);
    }

    for (int pass = 0; pass < 20; ++pass) {
        if (pass == 3 && lbl_8087EFA8->m104.mEnabled == 0) {
            continue;
        }
        if (pass == 4 && m54960 == 0) {
            continue;
        }
        m2F8 = pass;
        IRenderLayer* layer = mLayers[pass];
        if (layer != 0 && layer->m04 != 0) {
            ((LayerListCall)(*(void***)layer)[4])(layer, mPassHeads[pass]);
        }
    }

    Matrix3x4 work;
    fn_800D5F68(work.m, 1, -1, -1, 0);
    m7A0[2] = work;
    if (m554 != 0) {
        Matrix3x4 view = mCamera.mInvView;
        view.m[0][3] = 0.0f;
        view.m[1][3] = 0.0f;
        view.m[2][3] = 0.0f;
        Matrix3x4 view2 = m558.mView;
        view2.m[0][3] = 0.0f;
        view2.m[1][3] = 0.0f;
        view2.m[2][3] = 0.0f;
        Matrix3x4 inv;
        Matrix3x4 result;
        PSMTXConcat(view2.m, view.m, inv.m);
        PSMTXConcat(m7A0[2].m, inv.m, result.m);
        m7A0[2] = result;
    }
    Matrix3x4 work2;
    fn_800D5F68(work2.m, 2, -1, -1, 0);
    m7A0[0] = work2;
    Matrix3x4 work3;
    fn_800D5F68(work3.m, 3, -1, -1, 0);
    m7A0[1] = work3;
    Matrix3x4 work4;
    fn_800D5F68(work4.m, 4, -1, -1, 0);
    m7A0[3] = work4;
    m7A0[4].SetIdentity();

    LCEnable();
    for (int pass = 0; pass < 20; ++pass) {
        m890 = 0;
        if (pass == 3 && lbl_8087EFA8->m104.mEnabled == 0) {
            continue;
        }
        if (pass == 4 && m54960 == 0) {
            continue;
        }
        m2F8 = pass;
        if (mLayers[pass] != 0 && mLayers[pass]->m04 != 0 && mPassHeads[pass] != 0) {
            mKeys.mSize = 0;
            mCmds.mSize = 0;
            mLayers[pass]->vf1C();
            ((LayerListCall)(*(void***)mLayers[pass])[5])(mLayers[pass], mPassHeads[pass]);
            if (lbl_8087EFA8->m39C != 0 && mCmds.mSize != 0) {
                fn_800BECF0(mKeys.mData, mKeys.mSize);
            }
            for (unsigned int i = 0; i < mCmds.mSize; ++i) {
                fn_8009DE60(&mCmds.mData[mKeys.mData[i].mIndex], 0.0f);
            }
            ((LayerListCall)(*(void***)mLayers[pass])[6])(mLayers[pass], mPassHeads[pass]);
            mLayers[pass]->vf20();
        }
        for (unsigned int i = 0; i < mResolveCount; ++i) {
            if (pass == mResolve[i].mFormat) {
                fn_80071D80(device, mResolve[i].mTex);
            }
        }
    }
    LCDisable();

    if (m8C8.mFunc != 0) {
        m8C8.mFunc((void*)m8C8.mArg);
    }
    memset(mPassHeads, 0, sizeof(mPassHeads));
    memset(mPassCounts, 0, sizeof(mPassCounts));
    mQueueUsed = 0;
    *(int*)((unsigned char*)lbl_8087EEB0 + 0xC) = 0;
    mResolveCount = 0;
    m54960 = 0;
    if (m8E0.mFunc != 0) {
        m8E0.mFunc((void*)m8E0.mArg);
    }
    fn_80071AFC(device);
    if (m8F8.mFunc != 0) {
        m8F8.mFunc((void*)m8F8.mArg);
    }
}

// inferred: radix sort of the draw keys, one byte per pass, through a buffer on the stack
extern "C" void fn_800BECF0(DrawSortKey_unk* keys, unsigned int count) {
    unsigned char buffer[0x8000];
    DrawSortKey_unk* tmp = (DrawSortKey_unk*)buffer;
    fn_800BEDA8(0, keys, tmp, count);
    fn_800BEDA8(8, tmp, keys, count);
    fn_800BEDA8(16, keys, tmp, count);
    fn_800BEDA8(24, tmp, keys, count);
}


// inferred: one pass of the radix sort of the draw keys (stable counting sort on the byte at `shift`)
extern "C" void fn_800BEDA8(int shift, const DrawSortKey_unk* in, DrawSortKey_unk* out, unsigned int count) {
    unsigned int histogram[256];
    memset(histogram, 0, sizeof(histogram));
    const DrawSortKey_unk* p = in;
    for (unsigned int n = count; n != 0; n--) {
        histogram[((unsigned int)p->mKey >> shift) & 0xFF]++;
        p++;
    }
    unsigned int offset = 0;
    unsigned int* h = histogram;
    for (int i = 0; i < 256; i++) {
        unsigned int c = *h;
        *h = offset;
        offset += c;
        h++;
    }
    p = in;
    for (unsigned int n = count; n != 0; n--) {
        out[histogram[((unsigned int)p->mKey >> shift) & 0xFF]++] = *p;
        p++;
    }
}

template <class T>
inline void Swap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template <class T>
struct Ref_unk {
    typedef T& type;
};
template <class T>
struct Ref_unk<T&> {
    typedef T& type;
};

template <class T, class Compare>
inline T* MinElement_unk(T* first, T* last, Compare& comp) {
    T* it;
    T* result = first;
    if (first != last) {
        for (it = first + 1; it != last; it++) {
            if (comp(*it, *result)) {
                result = it;
            }
        }
    }
    return result;
}

template <class T, class Compare>
void Median3_unk(T* a, T* b, T* c, Compare& comp);

// inferred: MSL __sort (std::sort) of the pass lists, as atn/Loader.cpp Sort_unk: 0x800BF0EC (comparison by
// value) and 0x800BF558 (by reference, recursive part)
// NONMATCHING: 99.79% (both instantiations) - register of the minimum in the selection sort
template <class T, class Compare>
void Sort_unk(T* first, T* last, Compare comp) {
    typedef typename Ref_unk<Compare>::type CompareRef;
    static int sOffset = -4; // inferred: lbl_8087D900 and lbl_8087D904 (one per instantiation)
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            if (first != last) {
                for (; first != last - 1; first++) {
                    T* min = MinElement_unk(first, last, comp);
                    if (min != first) {
                        Swap_unk(*min, *first);
                    }
                }
            }
            return;
        }
        T* a = first + (n / 4 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        int n3 = n * 3 / 4;
        T* b = first + (n3 + sOffset % 5);
        if (++sOffset >= 5) {
            sOffset = -4;
        }
        T* i;
        T* j;
        T* pivot = last - 1;
        Median3_unk(a, b, pivot, comp);
        i = first;
        j = pivot;
        while (comp(*i, *pivot)) {
            i++;
        }
        while (i != --j && !comp(*j, *pivot)) {
        }
        if (i < j) {
            Swap_unk(*i, *j);
            i++;
            while (true) {
                while (comp(*i, *pivot)) {
                    i++;
                }
                while (!comp(*--j, *pivot)) {
                }
                if (i >= j) {
                    break;
                }
                Swap_unk(*i, *j);
                i++;
            }
        }
        if (i == first) {
            T* back = last - 1;
            Swap_unk(*i, *pivot);
            i++;
            if (!comp(*first, *back)) {
                while (i != last && !comp(*first, *i)) {
                    i++;
                }
                if (i < back) {
                    Swap_unk(*i, *back);
                }
            }
            j = back;
            if (i < j) {
                while (true) {
                    while (!comp(*first, *i)) {
                        i++;
                    }
                    while (comp(*first, *--j)) {
                    }
                    if (i >= j) {
                        break;
                    }
                    Swap_unk(*i, *j);
                    i++;
                }
            }
            first = i;
        } else if (i - first < last - i) {
            Sort_unk<T, CompareRef>(first, i, comp);
            first = i;
        } else {
            Sort_unk<T, CompareRef>(i, last, comp);
            last = i;
        }
    }
}

// inferred: MSL __sort3 (median of three of std::sort)
template <class T, class Compare>
void Median3_unk(T* a, T* b, T* c, Compare& comp) {
    bool ac = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ac && bc) {
        return;
    }
    if (!ac && !bc) {
        Swap_unk(*a, *b);
        return;
    }
    if (comp(*b, *a)) {
        Swap_unk(*a, *b);
    }
    if (ac) {
        Swap_unk(*b, *c);
    } else {
        Swap_unk(*a, *c);
    }
}
// emitted here in the original (after fn_800BEDA8)
template void Sort_unk<atn::RenderEntry_unk*, RenderEntryLess_unk>(atn::RenderEntry_unk**, atn::RenderEntry_unk**,
                                                                  RenderEntryLess_unk);

void atn::RenderManager::fn_800BFAC4() {}

atn::Vector3 atn::RenderManager::GetScreenPosition(const atn::Vector3& pos) const {
    float projection[7];
    projection[0] = 0.0f; // inferred: GX_PERSPECTIVE
    projection[1] = mCamera.mProj.m[0][0];
    projection[2] = mCamera.mProj.m[0][2];
    projection[3] = mCamera.mProj.m[1][1];
    projection[4] = mCamera.mProj.m[1][2];
    projection[5] = mCamera.mProj.m[2][2];
    projection[6] = mCamera.mProj.m[2][3];
    float viewport[6];
    GXGetViewportv(viewport);
    atn::Vector3 out;
    GXProject(pos.x, pos.y, pos.z, mCamera.mView.m, projection, viewport, &out.x, &out.y, &out.z);
    return out;
}

void atn::RenderManager::fn_800BFB70() {
    // inferred: full screen quad in white
    fn_800BFC18(atn::Color(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f);
}

void atn::RenderManager::fn_800BFC18(atn::Color color, float a, float b) {
    // inferred: full screen quad of one color
    atn::Color c(color);
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    fn_800BFCB4(c, c, c, c, width, height, a, b);
}

// inferred: ARGB word to GXColor (as in atn/GraphicsDevice.cpp)
inline GXColor ToGXColor_unk(unsigned int argb) {
    GXColor color;
    color.b = argb;
    color.r = argb >> 16;
    color.g = argb >> 8;
    color.a = argb >> 24;
    return color;
}

// inferred: color as an ARGB word (as fn_800BDBC8 builds the clear color)
inline unsigned int ColorToARGB_unk(const atn::Color& c) {
    unsigned int r = ColorChannel_unk(c.r);
    unsigned int g = ColorChannel_unk(c.g);
    unsigned int b = ColorChannel_unk(c.b);
    unsigned int a = ColorChannel_unk(c.a);
    unsigned int argb = a << 24;
    argb |= r << 16;
    argb |= g << 8;
    argb |= b;
    return argb;
}

// inferred: write gather pipe of the GX FIFO (as in atn/DrawHelper.cpp)
union RenderWGPipe_unk {
    float f;
    unsigned char u8;
};
#define GXWGFifo (*(volatile RenderWGPipe_unk*)0xCC008000)

static inline void GXPosition3f32(float x, float y, float z) {
    GXWGFifo.f = x;
    GXWGFifo.f = y;
    GXWGFifo.f = z;
}
static inline void GXColor4u8(unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    GXWGFifo.u8 = r;
    GXWGFifo.u8 = g;
    GXWGFifo.u8 = b;
    GXWGFifo.u8 = a;
}
static inline void GXTexCoord2f32(float s, float t) {
    GXWGFifo.f = s;
    GXWGFifo.f = t;
}

// inferred: screen quad of width x height at (x, y) with a color in each corner (top left, top right,
// bottom left, bottom right)
// NONMATCHING: 93.6% - order of the byte stores of the GXColor and scheduling of the vertex writes
void atn::RenderManager::fn_800BFCB4(atn::Color color0, atn::Color color1, atn::Color color2,
                                      atn::Color color3, unsigned int width, unsigned int height, float x,
                                      float y) {
    atn::GraphicsDevice_unk* device = lbl_8087EEE0;
    atn::GXStateBits_unk state = device->mState;
    fn_80076760(device, 8, 1);
    fn_80076760(device, 9, 7);
    fn_80076760(device, 10, 0);
    fn_800761A8(lbl_8087EEE0);
    GXClearVtxDesc();
    GXSetVtxDesc(9, 1);
    GXSetVtxDesc(11, 1);
    GXSetVtxDesc(13, 1);
    GXColor c0 = ToGXColor_unk(ColorToARGB_unk(color0));
    GXColor c1 = ToGXColor_unk(ColorToARGB_unk(color1));
    GXColor c2 = ToGXColor_unk(ColorToARGB_unk(color2));
    GXColor c3 = ToGXColor_unk(ColorToARGB_unk(color3));
    GXSetVtxAttrFmt(0, 9, 1, 4, 0);
    GXSetVtxAttrFmt(0, 11, 1, 5, 0);
    GXSetVtxAttrFmt(0, 13, 1, 4, 0);
    float projection[4][4];
    C_MTXOrtho(projection, 0.0f, (int)height, 0.0f, (int)width, 0.0f, lbl_8087D908);
    GXSetProjection(projection, 1);
    float position[3][4];
    PSMTXIdentity(position);
    GXLoadPosMtxImm(position, 0);
    GXSetCurrentMtx(0);
    GXBegin(0x80, 0, 4);
    float z = lbl_8087D90C;
    GXPosition3f32(x, y, z);
    GXColor4u8(c0.r, c0.g, c0.b, c0.a);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32((int)width + x, y, z);
    GXColor4u8(c1.r, c1.g, c1.b, c1.a);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32((int)width + x, (int)height + y, z);
    GXColor4u8(c3.r, c3.g, c3.b, c3.a);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(x, (int)height + y, z);
    GXColor4u8(c2.r, c2.g, c2.b, c2.a);
    GXTexCoord2f32(0.0f, 1.0f);
    fn_80076A28(lbl_8087EEE0, &state);
}

void atn::RenderManager::fn_800C0508(int clear) {
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    GXSetTexCopySrc(0, 0, (unsigned short)width, (unsigned short)height);
    GXSetTexCopyDst((unsigned short)width, (unsigned short)height, 6, 0);
    GXCopyTex(*(void**)((unsigned char*)this + 0x774), (unsigned char)clear);
    GXPixModeSync();
}

void atn::RenderManager::fn_800C0590(int clear, int format) {
    // inferred: copies the frame buffer into m79C and rebuilds its texture object m77C
    unsigned int width = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x3C);
    unsigned int height = *(unsigned int*)((unsigned char*)lbl_8087EEE0 + 0x40);
    GXSetTexCopySrc(0, 0, (unsigned short)width, (unsigned short)height);
    GXSetTexCopyDst((unsigned short)width, (unsigned short)height, format, 0);
    GXCopyTex(m79C, (unsigned char)clear);
    GXPixModeSync();
    if (format != 0x16) {
        GXInitTexObj(&m77C, m79C, (unsigned short)width, (unsigned short)height, 3, 0, 0, 0);
        GXInitTexObjLOD(&m77C, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    } else {
        GXInitTexObj(&m77C, m79C, (unsigned short)width, (unsigned short)height, 0x16, 0, 0, 0);
        GXInitTexObjLOD(&m77C, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
    }
}

void atn::RenderManager::fn_800C06B0(const atn::FogSettings* fog) {
    // NONMATCHING: 94.60% - conversion spill placement and early GX argument setup still differ.
    if (lbl_8087EFA8->m02C != 0) {
        if (*(int*)((unsigned char*)this + 0x890) != 0) {
            *(int*)((unsigned char*)this + 0x890) = 0;
            unsigned int color = 0;
            float farZ = *(float*)((unsigned char*)this + 0x1D0);
            float nearZ = *(float*)((unsigned char*)this + 0x1CC);
            GXSetFog(0, &color, 0.0f, 0.0f, nearZ, farZ);
        }
        return;
    }

    unsigned char* bytes = (unsigned char*)this;
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

void atn::RenderManager::fn_800C08B0() {
    // NONMATCHING: 90.00% - only the stack color-pointer setup remains out of order.
    if (*(int*)((unsigned char*)this + 0x890) != 0) {
        *(int*)((unsigned char*)this + 0x890) = 0;
        unsigned int color = 0;
        float farZ = *(float*)((unsigned char*)this + 0x1D0);
        float nearZ = *(float*)((unsigned char*)this + 0x1CC);
        GXSetFog(0, &color, 0.0f, 0.0f, nearZ, farZ);
    }
}

atn::Matrix3x4 atn::RenderManager::fn_800C0900(int mode, int a, int b, int c) {
    atn::Matrix3x4 out;
    switch (mode) {
    case 1:
        out = m7A0[2];
        break;
    case 2:
        out = m7A0[0];
        break;
    case 3:
        out = m7A0[1];
        break;
    case 4:
        if (a <= 0) {
            out = m7A0[3];
        } else {
            fn_800D5F68(out.m, 4, a, b, c);
        }
        break;
    default:
        out = m7A0[4];
        break;
    }
    return out;
}

int atn::RenderManager::fn_800C0A50(const atn::AABB_unk* bounds) {
    void* pass = *(void**)this;
    if (pass != 0) {
        return !fn_800B5994(pass, bounds);
    }
    return 1;
}

atn::Texture_unk* atn::RenderManager::fn_800C0A8C() {
    return &static_cast<atn::LayerReflect*>(mLayers[4])->mTexture;
}

atn::Texture_unk* atn::RenderManager::fn_800C0A98() {
    return &static_cast<atn::LayerReflect*>(mLayers[5])->mTexture;
}

extern "C" void fn_800C0AA4(unsigned char* data, unsigned int width, unsigned int height,
                               unsigned int xShift, unsigned int yShift, unsigned int elementSize,
                               unsigned int rowStride, unsigned char* scratch) {
    // inferred: rearranges the blocks of each band through the scratch rows
    unsigned int blocksX = width >> xShift;
    unsigned int blocksY = height >> yShift;
    unsigned int bandSize = rowStride << yShift;
    unsigned int blockY = 0;
    unsigned char* p = data;
    unsigned char* tmp = scratch;
    unsigned int stride = rowStride;
    unsigned int size = elementSize;
    for (; blockY < blocksY; ++blockY) {
        memcpy(tmp, p, bandSize);
        unsigned int blockX;
        unsigned char* src;
        unsigned int row;
        unsigned int i;
        for (blockX = 0; blockX < blocksX; ++blockX) {
            src = tmp + blockX * size;
            for (row = 0; row < (1u << yShift); ++row) {
                for (i = 0; i < size; ++i) {
                    p[i] = src[i];
                }
                p += size;
                src += stride;
            }
        }
    }
}

extern "C" void fn_800C0BF8(unsigned char* data, unsigned int width, unsigned int height,
                               unsigned int xShift, unsigned int yShift, unsigned int elementSize,
                               unsigned int rowStride, unsigned char* scratch) {
    // inferred: inverse of fn_800C0AA4
    unsigned int blocksX = width >> xShift;
    unsigned int blocksY = height >> yShift;
    unsigned int bandSize = rowStride << yShift;
    unsigned int blockY = 0;
    unsigned char* p = data;
    unsigned char* tmp = scratch;
    unsigned int stride = rowStride;
    unsigned int size = elementSize;
    for (; blockY < blocksY; ++blockY) {
        unsigned char* band = p;
        unsigned int blockX = 0;
        unsigned int offset = 0;
        for (; blockX < blocksX; ++blockX) {
            unsigned int row = 0;
            unsigned int i;
            unsigned char* dst = tmp + offset;
            for (; row < (1u << yShift); ++row) {
                for (i = 0; i < size; ++i) {
                    dst[i] = p[i];
                }
                dst += stride;
                p += size;
            }
            offset += size;
        }
        memcpy(band, tmp, bandSize);
    }
}

extern "C" int fn_800C0D4C(unsigned char* image, unsigned int format, unsigned int width,
                             unsigned int height) {
    // inferred: converts supported GX texture layouts to tiled storage and flushes the image.
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
        unsigned int blocksWide;
        unsigned int blocksHigh;
        unsigned int y;
        unsigned int rowBytes;
        unsigned int stride;
        unsigned char* tile;
        tile = image;
        stride = width << 2;
        blocksWide = width >> 2;
        blocksHigh = height >> 2;
        rowBytes = width << 4;
        y = 0;
        unsigned int pairs = 2;
        for (; y < blocksHigh; y++) {
            memcpy(scratch, tile, rowBytes);
            unsigned int x;
            x = 0;
            for (; x < blocksWide; x++) {
                const unsigned char* src = scratch + x * 16;
                for (unsigned int k = 0; k < pairs; k++) {
                    tile[0] = src[0];
                    tile[1] = src[1];
                    tile[32] = src[2];
                    tile[33] = src[3];
                    tile[2] = src[4];
                    tile[3] = src[5];
                    tile[34] = src[6];
                    tile[35] = src[7];
                    tile[4] = src[8];
                    tile[5] = src[9];
                    tile[36] = src[10];
                    tile[37] = src[11];
                    tile[6] = src[12];
                    tile[7] = src[13];
                    tile[38] = src[14];
                    tile[39] = src[15];
                    src += stride;
                    tile[8] = src[0];
                    tile[9] = src[1];
                    tile[40] = src[2];
                    tile[41] = src[3];
                    tile[10] = src[4];
                    tile[11] = src[5];
                    tile[42] = src[6];
                    tile[43] = src[7];
                    tile[12] = src[8];
                    tile[13] = src[9];
                    tile[44] = src[10];
                    tile[45] = src[11];
                    tile[14] = src[12];
                    tile[15] = src[13];
                    tile[46] = src[14];
                    tile[47] = src[15];
                    src += stride;
                    tile += 16;
                }
                tile += 32;
            }
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
    unsigned char scratch[0x2000];
    switch (format) {
    case 0:
    case 8:
        fn_800C0BF8(image, width, height, 3, 3, 4, width >> 1, scratch);
        DCFlushRange(image, (width * height) >> 1);
        break;
    case 1:
    case 2:
    case 9:
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
        unsigned int blocksWide;
        unsigned int blocksHigh;
        unsigned int y;
        unsigned int rowBytes;
        unsigned int stride;
        unsigned char* tile;
        tile = image;
        stride = width << 2;
        blocksWide = width >> 2;
        blocksHigh = height >> 2;
        rowBytes = width << 4;
        y = 0;
        unsigned int pairs = 2;
        for (; y < blocksHigh; y++) {
            unsigned char* row = tile;
            for (unsigned int x = 0; x < blocksWide; x++) {
                unsigned char* dst = scratch + x * 16;
                for (unsigned int k = 0; k < pairs; k++) {
                    dst[0] = tile[0];
                    dst[1] = tile[1];
                    dst[2] = tile[32];
                    dst[3] = tile[33];
                    dst[4] = tile[2];
                    dst[5] = tile[3];
                    dst[6] = tile[34];
                    dst[7] = tile[35];
                    dst[8] = tile[4];
                    dst[9] = tile[5];
                    dst[10] = tile[36];
                    dst[11] = tile[37];
                    dst[12] = tile[6];
                    dst[13] = tile[7];
                    dst[14] = tile[38];
                    dst[15] = tile[39];
                    dst += stride;
                    dst[0] = tile[8];
                    dst[1] = tile[9];
                    dst[2] = tile[40];
                    dst[3] = tile[41];
                    dst[4] = tile[10];
                    dst[5] = tile[11];
                    dst[6] = tile[42];
                    dst[7] = tile[43];
                    dst[8] = tile[12];
                    dst[9] = tile[13];
                    dst[10] = tile[44];
                    dst[11] = tile[45];
                    dst[12] = tile[14];
                    dst[13] = tile[15];
                    dst[14] = tile[46];
                    dst[15] = tile[47];
                    tile += 16;
                    dst += stride;
                }
                tile += 32;
            }
            memcpy(row, scratch, rowBytes);
        }
        DCFlushRange(image, (width * height) << 2);
        break;
    }
    }
    return 1;
}

atn::Scene* atn::RenderManager::fn_800C122C(atn::Sphere sphere) {
    // inferred: run the configured sphere-to-scene callback and fall back to the stored scene.
    // NONMATCHING: 96.75% - callback invoker data label and aggregate load order differ.
    if (mMapHit) {
        atn::Scene* scene = mMapHit(sphere, mMapHitArg);
        if (scene != 0) {
            return scene;
        }
    }
    return mScene;
}

atn::Scene* atn::RenderManager::fn_800C1424() {
    // inferred: scene of a unit sphere at the camera position (m54964) or at its target
    atn::Sphere sphere;
    sphere.mCenter = m54964 != 0 ? mCamera.mPos : mCamera.mTarget;
    sphere.mRadius = 1.0f;
    return fn_800C122C(sphere);
}

void atn::RenderManager::AddResolveTexture(atn::Texture_unk* tex, int format) {
    // inferred: preserve the by-value texture/format temporary before enqueueing the pair.
    struct TextureFormatPair {
        void* texture;
        int format;
    };
    volatile TextureFormatPair pair;
    pair.texture = tex;
    pair.format = format;
    int* count = (int*)((unsigned char*)this + 0x910);
    unsigned char* entry = (unsigned char*)this + 0x914 + *count * 8;
    if (entry != 0) {
        *(void**)entry = tex;
        *(int*)(entry + 4) = format;
    }
    ++*count;
}

float atn::RenderManager::fn_800C169C(const atn::AABB_unk* bounds) {
    void* pass = *(void**)this;
    if (pass != 0) {
        return fn_800B6218(pass, bounds);
    }
    return 0.0f;
}

atn::Scene* atn::RenderManager::fn_800C16B4() {
    return lbl_8087EFB8;
}

void atn::RenderManager::fn_800C16BC() {}
