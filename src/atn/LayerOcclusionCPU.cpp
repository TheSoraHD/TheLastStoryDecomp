#include "atn/GXDecl.h"
#include "atn/MemoryManager.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "macros.h"
#include "atn/DrawHelper.h"
#include "atn/RenderManager.h"

extern "C" int memcmp(const void* a, const void* b, unsigned long n);
extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                           unsigned char mipmap, unsigned char maxLod);

// inferred: registers a debug toggle `name` bound to `flag` (see atn/Layer3D.h)
extern "C" void fn_80087994(void* menu, const char* name, int* flag, int arg3, int arg4);
// inferred: aligned array allocation (size, align, heap, name, file, line)
extern "C" void* fn_800848B4(unsigned long size, int align, int heap, const char* name, const char* file, int line);

// inferred: render state (viewport) and debug drawing
struct RenderState_unk;
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" void fn_80075F58(RenderState_unk* rs, int x, int y, int w, int h); // inferred: set viewport
extern "C" void fn_80075DEC(RenderState_unk* rs);
extern "C" void fn_800C0D4C(void* data, int arg1, int w, int h); // inferred: flushes a texture

// inferred: global render settings; m1DC enables the occlusion buffer view
struct RenderSettings_unk {
    unsigned char m000[0x1D8];
    float m1D8; // inferred: depth bias of the occlusion tests ("ZBias")
    int m1DC;
};
extern "C" RenderSettings_unk* lbl_8087EFA8;


// inferred: identity matrix (.bss)
extern "C" Mtx lbl_807C7060;

namespace atn {

// inferred: node of the occluder tree (0x28 bytes)
struct OccNode_unk {
    float mBox[6];       // 0x00: inferred: bounding box
    short mChild[4];     // 0x18
    unsigned short m20;  // 0x20: inferred: first triangle when fully visible
    unsigned short m22;  // 0x22: inferred: triangle count when fully visible
    unsigned short m24;  // 0x24: inferred: first triangle when partially visible
    unsigned short m26;  // 0x26: inferred: triangle count when partially visible
};

// inferred: occluder tree of a model
struct OccTree_unk {
    unsigned char m00[0x14];
    OccNode_unk* mNodes;      // 0x14
    unsigned char m18[4];
    unsigned short* mIndices; // 0x1C
    unsigned char m20[4];
    int m24;                  // 0x24: inferred: vertex format (-1: 4 floats per vertex)
    void* m28;                // 0x28: inferred: vertices
};

// inferred: occluder mesh of a model and its material chain (names invented)
struct OccFormat_unk {
    unsigned char m00[0xC];
    int m0C;
    unsigned char m10[4];
    int m14;
};
struct OccVtx_unk {
    unsigned char m00[0x1C];
    OccFormat_unk* m1C;
};
struct OccShape_unk {
    unsigned char m00[0x14];
    OccVtx_unk* m14;
    unsigned char m18[4];
    void* m1C;
};
struct OccShapes_unk {
    unsigned char m00[0x2C];
    OccShape_unk** m2C;
};
struct OccMesh_unk {
    unsigned char m00[0x14];
    int mMtx;                  // 0x14
    int mShape;                // 0x18
    int mCount;                // 0x1C
    unsigned short* mIndices;  // 0x20
};
struct OccMeshList_unk {
    unsigned char m00[0x10];
    int mCount;            // 0x10
    OccMesh_unk** mMeshes; // 0x14
};
struct OccModel_unk {
    unsigned char m000[0x3C];
    Mtx* mMtx;              // 0x3C
    unsigned char m040[0x164 - 0x40];
    unsigned char m164[8];  // 0x164: inferred: occluder data
    OccShapes_unk* m16C;
};
extern "C" OccMeshList_unk* fn_80476194(void* data);
extern "C" OccTree_unk* fn_804761B0(void* data);

// inferred: object of the render list (see atn/Layer3D.h); vf0C returns nonzero when hidden
struct OccObj_unk {
    virtual void vf08();
    virtual int vf0C();
};
struct OccRenderNode_unk {
    OccModel_unk* mObj;
    float m04;
    OccRenderNode_unk* mNext;
};

// inferred: render layer interface. Local copy of atn::IRenderLayer (atn/Layer3D.h): this layer
// shows that vf10 takes an argument.
class IRenderLayer {
public:
    IRenderLayer(const char* name, int arg0C = -1, int arg10 = -1) : m04(1), mName(name), m0C(arg0C), m10(arg10) {}
    virtual ~IRenderLayer() {}
    virtual void vf0C(void* menu) { fn_80087994(menu, mName, &m04, 0, 0); }
    virtual void vf10(int arg) = 0;
    virtual void vf14(void* arg) = 0;
    virtual void vf18() {}
    virtual void vf1C() = 0;
    virtual void vf20() = 0;

    int m04;
    const char* mName; // 0x08
    int m0C;
    int m10;
};

// inferred: software occlusion buffer (32x32 depth with 4 smaller levels)
class LayerOcclusionCPU : public IRenderLayer {
public:
    LayerOcclusionCPU();
    virtual ~LayerOcclusionCPU();
    virtual void vf10(int arg);
    virtual void vf14(void* arg);
    virtual void vf1C();
    virtual void vf20();

    void fn_800B448C(const OccTree_unk* tree, void* frustum, int index);
    void fn_800B4694(OccModel_unk* model, OccMeshList_unk* list);

    float* mBuffers[5]; // 0x14: inferred: (32 >> i) squared values each
    struct Span_unk {
        int mLeft;
        int mRight;
    }* m28;             // inferred: x range of each row of the triangle being drawn (fn_800B543C)
    int m2C;            // inferred: something was drawn this frame
};

} // namespace atn

// NONMATCHING: not decompiled - handwritten paired-single assembly (polygon clipping with
// ps_madds0/fres and mcrf)
extern "C" void fn_800B4390(const float* plane, const float* in, int count, float* out, int* outCount);
// NONMATCHING: not decompiled - rasterizer written with GQR setup (mtspr GQR6) and the locked
// cache, calls fn_800B4390
extern "C" void fn_800B47AC(atn::LayerOcclusionCPU* layer, const unsigned short* indices, int count, int stride,
                            int format, void* vertices, int clip, Mtx* mtx);

// inferred: std::min
template <class T>
inline const T& Min_unk(const T& a, const T& b) {
    return b < a ? b : a;
}

// NONMATCHING: 96.9% - the four values are stored to the stack in the right order but the
// original gives the first one f3 and the second f2 (ours the other way round)
// inferred: halves a buffer, keeping the nearest of each 2x2 block
static void fn_800B3978(float* dst, const float* src, int w, int h) {
    int halfW = w / 2;
    int halfH = h / 2;
    for (int y = 0; y < halfH; y++) {
        float* d = &dst[y * halfW];
        for (int x = 0; x < halfW; x++) {
            float b;
            float c;
            float e;
            float m;
            m = src[w * (y * 2) + x * 2];
            b = src[w * (y * 2) + x * 2 + 1];
            c = src[w * (y * 2 + 1) + x * 2];
            e = src[w * (y * 2 + 1) + x * 2 + 1];
            m = Min_unk(m, b);
            m = Min_unk(m, c);
            m = Min_unk(m, e);
            *d++ = m;
        }
    }
}

// Note on the constants: the original .sdata2 starts with 0.5f, 255.0f and 0.0f before the 1.0f
// used by the constructor (the first function that loads a constant). They must come from a function placed
// before the constructor and removed by the linker; it is not reconstructed, so every function
// that loads 1.0f, 16.0f, 32.0f or 128.0f points at a different offset.
static bool sTexReady;
static unsigned char* sTexData;
static GXTexObj sTexObj;

namespace atn {

// NONMATCHING: 98.8% - .sdata2 offset of 1.0f (see the note on the constants)
LayerOcclusionCPU::LayerOcclusionCPU() : IRenderLayer("OcclusionCPU") {
    m2C = 0;
    for (int i = 0; i < 5; i++) {
        int n = 32 >> i;
        n *= n;
        mBuffers[i] = (float*)fn_800848B4(n * 4, 0x20, 6, "", "", 0);
        for (int j = 0; j < n; j++) {
            mBuffers[i][j] = 1.0f;
        }
    }
}

LayerOcclusionCPU::~LayerOcclusionCPU() {
    for (int i = 0; i < 5; i++) {
        delete[] mBuffers[i];
    }
}

// NONMATCHING: 99.4% - the model and mesh list pointers get r23/r28 swapped, and the frustum
// and child loop counter r30/r31 swapped
void LayerOcclusionCPU::vf14(void* arg) {
    for (OccRenderNode_unk* node = (OccRenderNode_unk*)arg; node != 0; node = node->mNext) {
        if (((OccObj_unk*)node->mObj)->vf0C() == 0) {
            OccModel_unk* model = node->mObj;
            OccMeshList_unk* meshes = fn_80476194(model->m164);
            const OccTree_unk* tree = fn_804761B0(model->m164);
            if (meshes != 0) {
                fn_800B4694(model, meshes);
            } else if (tree != 0) {
                fn_800B448C(tree, &atn::RenderManager::instance_->mCamera.mFrustum, 0);
            }
            m2C = 1;
        }
    }
}

// NONMATCHING: 99.9% - .sdata2 offset of 1.0f (see the note on the constants)
void LayerOcclusionCPU::vf10(int arg) {
    if (arg != 0) {
        return;
    }
    for (int i = 0; i < 5; i++) {
        int n = 32 >> i;
        n *= n;
        for (int j = 0; j < n; j++) {
            mBuffers[i][j] = 1.0f;
        }
    }
}

// NONMATCHING: 99.9% - .sdata2 offset of 1.0f (see the note on the constants)
void LayerOcclusionCPU::vf1C() {
    float* buf = mBuffers[0];
    for (int i = 0; i < 32 * 32; i++) {
        buf[i] = 1.0f;
    }
    fn_80075F58(lbl_8087EEE0, 0, 0, 32, 32);
    m2C = 0;
}

// NONMATCHING: 98.3% - .sdata2 offsets, and the row pointers of the conversion loop swap
// registers
void LayerOcclusionCPU::vf20() {
    for (int i = 0; i < 4; i++) {
        int size = 32 >> i;
        fn_800B3978(mBuffers[i + 1], mBuffers[i], size, size);
    }
    fn_80075DEC(lbl_8087EEE0);
    if (lbl_8087EFA8->m1DC != 0) {
        if (!sTexReady) {
            unsigned int size = GXGetTexBufferSize(32, 32, 1, 0, 0);
            sTexData = (unsigned char*)atn::MemoryManager::Instance()->AllocFromMem2(size, 0x20, (MemoryTag)6, "", "", 0, 0);
            GXInitTexObj(&sTexObj, sTexData, 32, 32, 1, 0, 0, 0);
            GXInitTexObjLOD(&sTexObj, 0, 0, 0.0f, 0.0f, 0.0f, 0, 0, 0);
            sTexReady = true;
        }
        unsigned char* tex = sTexData;
        float* buf = mBuffers[0];
        for (int y = 0; y < 32; y++) {
            for (int x = 0; x < 32; x++) {
                float v = buf[y * 32 + x];
                int c = (int)(255.0f * v);
                if (v == 1.0f) {
                    c = 0;
                }
                tex[y * 32 + x] = c;
            }
        }
        fn_800C0D4C(sTexData, 1, 32, 32);
        lbl_8087EEB0->fn_8005DF90(-1, &sTexObj, 0, 16.0f, 32.0f, 0.0f, 128.0f, 128.0f);
    }
}

void LayerOcclusionCPU::fn_800B448C(const OccTree_unk* tree, void* frustum, int index) {
    OccNode_unk* node = &tree->mNodes[index];
    int result = fn_800500FC((const Frustum_unk*)frustum, (const atn::Vector3*)node);
    if (result == 2) {
        fn_800B47AC(this, &tree->mIndices[node->m20 * 3], node->m22 * 3, tree->m24 == -1 ? 4 : 1, tree->m24,
                    tree->m28, 0, 0);
    } else if (result == 1) {
        fn_800B47AC(this, &tree->mIndices[node->m24 * 3], node->m26 * 3, tree->m24 == -1 ? 4 : 1, tree->m24,
                    tree->m28, 1, 0);
        for (int i = 0; i < 4; i++) {
            if (node->mChild[i] != -1) {
                fn_800B448C(tree, frustum, node->mChild[i]);
            }
        }
    }
}

// NONMATCHING: 90.6% - the original evaluates the matrix argument (identity ? 0 : &mtx) after
// all the other arguments; must stay large enough not to be inlined into vf14
void LayerOcclusionCPU::fn_800B4694(OccModel_unk* model, OccMeshList_unk* list) {
    Mtx mtx;
    for (int i = 0; i < list->mCount; i++) {
        OccMesh_unk* mesh = list->mMeshes[i];
        Mtx* src;
        if (mesh->mMtx < 0) {
            src = 0;
        } else {
            src = &model->mMtx[mesh->mMtx];
        }
        __MtxCopy(mtx, *src);
        int identity = memcmp(mtx, lbl_807C7060, sizeof(Mtx)) == 0;
        OccShape_unk* shape = model->m16C->m2C[mesh->mShape];
        Mtx* m = &mtx;
        if (identity) {
            m = 0;
        }
        fn_800B47AC(this, mesh->mIndices, mesh->mCount, shape->m14->m1C->m0C, shape->m14->m1C->m14, shape->m1C, 1, m);
    }
}

// inferred: walks the edge (x0, y0)-(x1, y1) and stores its x on each row (left or right end of the span),
// from row y0 + skip
void fn_800B543C(LayerOcclusionCPU* layer, int x0, int y0, int x1, int y1, int left, int skip) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    if (dy == 0) {
        return;
    }
    float slope = (float)dx / (float)dy;
    int start = y0 + skip < 0 ? 0 : y0 + skip;
    int end = y1 < 32 ? y1 : 32;
    for (int y = start; y < end; y++) {
        if ((unsigned int)y <= 31) {
            int x = x0 + (int)(slope * (float)(y - y0));
            if (left) {
                layer->m28[y].mLeft = x;
            } else {
                layer->m28[y].mRight = x;
            }
        }
    }
}

// NONMATCHING: 99.6% - f0/f2 of the bias and the buffer value swapped
// inferred: the rectangle (x0, y0)-(x1, y1) of the buffer, at `depth`, is hidden at the level `level` of
// the buffer pyramid (each visible cell is tested again one level lower)
int fn_800B64DC(LayerOcclusionCPU* layer, int x0, int y0, int x1, int y1, float depth, int level) {
    int sx0 = x0 >> level;
    int sy0 = y0 >> level;
    int sx1 = x1 >> level;
    int sy1 = y1 >> level;
    float* data = layer->mBuffers[level];
    int width = 32 >> level;
    float bias = lbl_8087EFA8->m1D8;
    for (int y = sy0; y <= sy1; y++) {
        for (int x = sx0; x <= sx1; x++) {
            if (depth <= data[y * width + x] + bias) {
                return 0;
            }
            if (level > 0) {
                return fn_800B64DC(layer, x0, y0, x1, y1, depth, level - 1);
            }
        }
    }
    return 1;
}

// NONMATCHING: 99.6% - f0/f2 of the bias and the buffer value swapped (as fn_800B64DC)
// inferred: part of the rectangle (x0, y0)-(x1, y1) of the full buffer that is behind `depth`
float fn_800B65B4(LayerOcclusionCPU* layer, int x0, int y0, int x1, int y1, float depth) {
    float* data = layer->mBuffers[0];
    int count = 0;
    float bias = lbl_8087EFA8->m1D8;
    for (int y = y0; y <= y1; y++) {
        for (int x = x0; x <= x1; x++) {
            if (depth > data[y * 32 + x] + bias) {
                count++;
            }
        }
    }
    return (float)count / (float)((x1 - x0 + 1) * (y1 - y0 + 1));
}

} // namespace atn

// inferred: scratch vertices (16 Vector3; built by the __sinit at 0x800B6684, used by the clipping
// code at 0x800B47AC-0x800B6218 that is not reconstructed)
atn::Vector3 lbl_807C74A0[16];
