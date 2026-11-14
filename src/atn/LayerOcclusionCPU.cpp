#include "atn/GXDecl.h"
#include "atn/MemoryManager.h"
#include "atn/Memory.h"
#include "math/Vec.h"
#include "macros.h"
#include "atn/DrawHelper.h"
#include "atn/RenderManager.h"
#include "atn/AABB_unk.h"
#include "atn/Color.h"
#include "atn/Layer3D.h"
#include "atn/RenderSetting_unk.h"

extern "C" int memcmp(const void* a, const void* b, unsigned long n);
extern "C" unsigned int GXGetTexBufferSize(unsigned short width, unsigned short height, unsigned int format,
                                           unsigned char mipmap, unsigned char maxLod);

// inferred: aligned array allocation (size, align, heap, name, file, line)
extern "C" void* fn_800848B4(unsigned long size, int align, int heap, const char* name, const char* file, int line);

// inferred: render state (viewport) and debug drawing
struct RenderState_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
extern "C" RenderState_unk* lbl_8087EEE0;
extern "C" void fn_80075F58(RenderState_unk* rs, int x, int y, int w, int h); // inferred: set viewport
extern "C" void fn_80075DEC(RenderState_unk* rs);
extern "C" void fn_800C0D4C(void* data, int arg1, int w, int h); // inferred: flushes a texture

extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (atn/RenderSetting_unk.h)


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

// inferred: software occlusion buffer (32x32 depth with 4 smaller levels)
class LayerOcclusionCPU : public IRenderLayer {
public:
    LayerOcclusionCPU();
    virtual ~LayerOcclusionCPU();
    virtual void vf10(void* arg);
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

// inferred: clips the polygon `in` (count vertices) by `plane` into `out` (see the definition)
extern "C" void fn_800B4390(const Plane_unk* plane, const atn::Vector3* in, int count, atn::Vector3* out,
                            int* outCount);
// inferred: draws the triangles of an occluder (see the definition)
extern "C" void fn_800B47AC(atn::LayerOcclusionCPU* layer, const unsigned short* indices, int count, int type,
                            int frac, const void* vertices, int clip, const Mtx* mtx);

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
// that loads 1.0f, 16.0f, 32.0f or 128.0f points at a different offset. 0.5f, 255.0f, 0.0f, 1.0f is
// the reverse of the order of the constants of atn::ColorToU8_unk (used by fn_800B47AC); here
// they stay in order of first use (1.0f, 0.0f, 255.0f, 16.0f, 32.0f, 128.0f, 0.5f).
// inferred: screen positions of the last 16 vertices (cache of fn_800B47AC, slot = index % 16; built
// by the __sinit at 0x800B6684) and the index of each slot (0xFFFF: empty)
atn::Vector3 lbl_807C74A0[16];
unsigned short lbl_807C7560[16];
// inferred: (0, 1) pair loaded by fn_800B4390: 0 is the threshold of the plane side, 1 the w of the
// vertices
extern "C" float lbl_807790A8[2] = {0.0f, 1.0f};
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

// 100% in objdiff; the .sdata2 offset of 1.0f differs (see the note on the constants)
void LayerOcclusionCPU::vf10(void* arg) {
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

// 100% in objdiff; the .sdata2 offset of 1.0f differs (see the note on the constants)
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

// NONMATCHING: written as C++ - the original is handwritten paired-single assembly (mfcr/mtcrf,
// ps_cmpo0, mcrf, ps_madds0, the intersection divides with fres), not approved
// inferred: Sutherland-Hodgman step: clips the convex polygon `in` (count vertices) by `plane` and
// writes the part on the negative side of the plane (n.p + d < 0) to `out` and its vertex count to
// `outCount`
extern "C" void fn_800B4390(const Plane_unk* plane, const atn::Vector3* in, int count, atn::Vector3* out,
                            int* outCount) {
    int n = 0;
    const atn::Vector3* prev = in;
    float dPrev = -(plane->mNormal.x * prev->x + plane->mNormal.y * prev->y + plane->mNormal.z * prev->z +
                    plane->mDist * lbl_807790A8[1]);
    for (int i = 1; i <= count; i++) {
        const atn::Vector3* cur = i == count ? in : &in[i];
        float dCur = -(plane->mNormal.x * cur->x + plane->mNormal.y * cur->y + plane->mNormal.z * cur->z +
                       plane->mDist * lbl_807790A8[1]);
        bool prevIn = lbl_807790A8[0] < dPrev;
        bool curIn = lbl_807790A8[0] < dCur;
        if (prevIn != curIn) {
            float t = dPrev / (dPrev - dCur);
            out->x = prev->x + (cur->x - prev->x) * t;
            out->y = prev->y + (cur->y - prev->y) * t;
            out->z = prev->z + (cur->z - prev->z) * t;
            out++;
            n++;
        }
        if (curIn) {
            *out++ = *cur;
            n++;
        }
        prev = cur;
        dPrev = dCur;
    }
    *outCount = n;
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

void fn_800B5528(LayerOcclusionCPU* layer, const float* verts, int count);

} // namespace atn

extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);


// inferred: vertex of an occluder mesh stored as three s16 with `frac` fractional bits. The original
// loads them with psq_l through GQR6 (set with mtspr from `frac`), not approved: written as the
// equivalent scale.
inline void LoadS16Vtx_unk(atn::Vector3& v, const short* p, int frac) {
    float scale = 1.0f / (float)(1 << frac);
    v.x = p[0] * scale;
    v.y = p[1] * scale;
    v.z = p[2] * scale;
}

// NONMATCHING: 77.6% - written as C++: the original sets GQR6 with mtspr and dequantizes the s16
// vertices with psq_l (asm not approved, see LoadS16Vtx_unk), so here the int to float conversions
// take a larger frame and other registers; the rest has the same structure
// inferred: draws `count` / 3 triangles of an occluder into the occlusion buffer. type 4: vertices of
// three floats; type 1: three s16 with `frac` fractional bits. Each triangle is transformed by `mtx`
// (if any); the ones facing away from the camera are skipped (green debug lines). The others are
// clipped by the six planes of the frustum (clip != 0) or go through a cache of 16 projected vertices,
// are projected to the screen and drawn by fn_800B5528. The polygons are built in the locked cache
// (0xE0000000 and 0xE00000C0), the spans of the rows at 0xE0000180.
extern "C" void fn_800B47AC(atn::LayerOcclusionCPU* layer, const unsigned short* indices, int count, int type,
                            int frac, const void* vertices, int clip, const Mtx* mtx) {
    atn::Vector3* polys[2];
    polys[0] = (atn::Vector3*)0xE0000000;
    polys[1] = (atn::Vector3*)0xE00000C0;
    layer->m28 = (atn::LayerOcclusionCPU::Span_unk*)0xE0000180;
    for (int i = 0; i < 16; i++) {
        lbl_807C7560[i] = 0xFFFF;
    }
    for (int t = 0; t < count / 3; t++) {
        unsigned short i0 = indices[0];
        unsigned short i1 = indices[1];
        unsigned short i2 = indices[2];
        indices += 3;
        atn::Vector3 v0;
        atn::Vector3 v1;
        atn::Vector3 v2;
        if (type == 4) {
            v0 = ((const atn::Vector3*)vertices)[i0];
            v1 = ((const atn::Vector3*)vertices)[i1];
            v2 = ((const atn::Vector3*)vertices)[i2];
        } else if (type == 1) {
            LoadS16Vtx_unk(v0, (const short*)vertices + i0 * 3, frac);
            LoadS16Vtx_unk(v1, (const short*)vertices + i1 * 3, frac);
            LoadS16Vtx_unk(v2, (const short*)vertices + i2 * 3, frac);
        }
        if (mtx != 0) {
            PSMTXMultVec(*mtx, &v0, &v0);
            PSMTXMultVec(*mtx, &v1, &v1);
            PSMTXMultVec(*mtx, &v2, &v2);
        }
        atn::RenderManager* rm = atn::RenderManager::instance_;
        atn::Vector3 eye = v0 - rm->mCamera.mPos;
        atn::Vector3 e1 = v1 - v0;
        atn::Vector3 e2 = v2 - v0;
        atn::Vector3 normal;
        PSVECCrossProduct(&e1, &e2, &normal);
        if (PSVECDotProduct(&normal, &eye) < 0.0f) {
            if (lbl_8087EFA8->m1E0 != 0) {
                atn::Color green(0.0f, 1.0f, 0.0f, 1.0f);
                lbl_8087EEB0->fn_80063764(v0, v1, 0.0f, green.ToU32());
                lbl_8087EEB0->fn_80063764(v1, v2, 0.0f, green.ToU32());
                lbl_8087EEB0->fn_80063764(v2, v0, 0.0f, green.ToU32());
            }
            continue;
        }
        atn::Vector3* poly = polys[0];
        poly[0] = v0;
        poly[1] = v1;
        poly[2] = v2;
        if (lbl_8087EFA8->m1E0 != 0) {
            atn::Color white(1.0f, 1.0f, 1.0f, 1.0f);
            lbl_8087EEB0->fn_80063764(v0, v1, 0.0f, white.ToU32());
            lbl_8087EEB0->fn_80063764(v1, v2, 0.0f, white.ToU32());
            lbl_8087EEB0->fn_80063764(v2, v0, 0.0f, white.ToU32());
        }
        int n = 3;
        if (clip != 0) {
            const Plane_unk* plane = rm->mCamera.mFrustum.mPlanes;
            for (int i = 0; i < 6 && n >= 3; i++, plane++) {
                int out = 0;
                fn_800B4390(plane, polys[i % 2], n, polys[(i + 1) % 2], &out);
                n = out;
            }
            if (n < 3) {
                continue;
            }
            atn::Vector3* p = polys[0];
            for (int i = 0; i < n; i++) {
                atn::Vector3 pos = p[i];
                p[i] = atn::RenderManager::instance_->GetScreenPosition(pos);
            }
        } else {
            unsigned short s0 = i0 % 16;
            unsigned short s1 = i1 % 16;
            unsigned short s2 = i2 % 16;
            if (i0 == lbl_807C7560[s0]) {
                polys[0][0] = lbl_807C74A0[s0];
            } else {
                polys[0][0] = atn::RenderManager::instance_->GetScreenPosition(polys[0][0]);
                lbl_807C7560[s0] = i0;
                lbl_807C74A0[s0] = polys[0][0];
            }
            if (i1 == lbl_807C7560[s1]) {
                polys[0][1] = lbl_807C74A0[s1];
            } else {
                polys[0][1] = atn::RenderManager::instance_->GetScreenPosition(polys[0][1]);
                lbl_807C7560[s1] = i1;
                lbl_807C74A0[s1] = polys[0][1];
            }
            if (i2 == lbl_807C7560[s2]) {
                polys[0][2] = lbl_807C74A0[s2];
            } else {
                polys[0][2] = atn::RenderManager::instance_->GetScreenPosition(polys[0][2]);
                lbl_807C7560[s2] = i2;
                lbl_807C74A0[s2] = polys[0][2];
            }
        }
        atn::fn_800B5528(layer, &polys[0][0].x, n);
    }
}

namespace atn {

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

// inferred: rows drawn by fn_800B5528 (clamp of the top and bottom rows of a polygon)
extern "C" {
int lbl_8087D888 = 0;
int lbl_8087D88C = 31;
int lbl_8087D890 = 0;
int lbl_8087D894 = 31;
}

// inferred: std::max
template <class T>
inline const T& Max_unk(const T& a, const T& b) {
    return a < b ? b : a;
}

// inferred: clamps v to [lo, hi]
inline int ClampRow_unk(int v, const int& lo, const int& hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// NONMATCHING: 89.2% - the farthest depth goes through __fsel and frsp here (fsel + fmr in the original),
// stack slots of the bounds, registers of the edge walks
// inferred: draws the convex polygon `verts` (x, y, depth; screen coordinates of the 32x32 buffer) into the
// occlusion buffer with its farthest depth: finds the top edge, walks the left and right edges down to the
// bottom vertex (fn_800B543C) and fills the spans
void fn_800B5528(LayerOcclusionCPU* layer, const float* verts, int count) {
    struct Point_unk {
        int x;
        int y;
    };
    int minX = (int)verts[0];
    int maxX = minX;
    int minY = (int)verts[1];
    int maxY = minY;
    Point_unk pts[16];
    pts[0].x = minX;
    pts[0].y = minY;
    float depth = verts[2];
    int top = 0;
    int bottom = 0;
    const float* v = verts + 3;
    for (int i = 1; i < count; i++, v += 3) {
        pts[i].x = (int)v[0];
        pts[i].y = (int)v[1];
        float z = v[2];
        depth = __fsel(depth - z, depth, z);
        minX = Min_unk(minX, pts[i].x);
        maxX = Max_unk(maxX, pts[i].x);
        if (pts[i].y < minY) {
            minY = pts[i].y;
            top = i;
        }
        if (pts[i].y > maxY) {
            maxY = pts[i].y;
            bottom = i;
        }
    }
    if (minY == maxY || minX == maxX) {
        return;
    }
    int first = top;
    while (pts[top].y == minY) {
        top = (top + count + 1) % count;
    }
    int right = (top + count - 1) % count;
    while (pts[first].y == minY) {
        first = (first + count - 1) % count;
    }
    int left = (first + count + 1) % count;
    int dir = -1;
    int flat = left != right;
    if (flat) {
        if (pts[left].x > pts[right].x) {
            int t = left;
            left = right;
            right = t;
            dir = 1;
        }
    } else {
        int prev = (left + count - 1) % count;
        int next = (right + 1) % count;
        if ((pts[next].x - pts[left].x) * (pts[prev].y - pts[left].y) -
                (pts[next].y - pts[left].y) * (pts[prev].x - pts[left].x) <
            0) {
            int t = left;
            left = right;
            right = t;
            dir = 1;
        }
    }
    int yEnd = ClampRow_unk(maxY, lbl_8087D888, lbl_8087D88C);
    int yStart = ClampRow_unk(minY, lbl_8087D890, lbl_8087D894);
    int rows = flat + (maxY - minY) - 1;
    if (flat + (yEnd - yStart) - 1 <= 0) {
        return;
    }
    int skip = !flat;
    do {
        int next = (count + left + dir) % count;
        fn_800B543C(layer, pts[left].x, pts[left].y, pts[next].x, pts[next].y, 1, skip);
        skip = 0;
        left = next;
    } while (left != bottom);
    skip = !flat;
    int back = -dir;
    do {
        int next = (count + right + back) % count;
        fn_800B543C(layer, pts[right].x - 1, pts[right].y, pts[next].x - 1, pts[next].y, 0, skip);
        skip = 0;
        right = next;
    } while (right != bottom);
    float* buffer = layer->mBuffers[0];
    int y0 = minY + 1 - flat;
    for (int y = y0; y < y0 + rows; y++) {
        if ((unsigned int)y <= 31) {
            LayerOcclusionCPU::Span_unk* span = &layer->m28[y];
            float* p = &buffer[y * 32 + span->mLeft];
            for (int x = span->mLeft; x <= span->mRight && x < 32; x++, p++) {
                if (x >= 0 && *p > depth) {
                    *p = depth;
                }
            }
        }
    }
}

// inferred: limits of the screen box and of the cells tested by fn_800B6218
// inferred: limits used by the code at 0x800B5994 (not written)
extern "C" {
float lbl_8087D898 = 0.0f;
float lbl_8087D89C = 0.0f;
float lbl_8087D8A0 = 0.0f;
float lbl_8087D8A4 = 0.0f;
int lbl_8087D8A8 = 0;
int lbl_8087D8AC = 31;
int lbl_8087D8B0 = 0;
int lbl_8087D8B4 = 31;
int lbl_8087D8B8 = 0;
int lbl_8087D8BC = 31;
int lbl_8087D8C0 = 0;
int lbl_8087D8C4 = 31;
}

extern "C" void fn_8006FEBC(AABB_unk* screen, const AABB_unk* box); // inferred: projects a box
extern "C" void fn_800704DC(AABB_unk* screen, const AABB_unk* box); // inferred: projects a view space box
extern "C" void fn_8006FE08(const AABB_unk* box, unsigned int color);  // inferred: debug box
extern "C" void PSMTXMultVecArray(const Mtx m, const atn::Vector3* src, atn::Vector3* dst, unsigned long count);
extern "C" double floor(double x);
extern "C" double ceil(double x);
int fn_800B64DC(LayerOcclusionCPU* layer, int x0, int y0, int x1, int y1, float depth, int level);

// inferred: clamps v to [lo, hi]
inline float ClampF_unk(float v, const float& lo, const float& hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

// inferred: bounds of the 8 corners of the box transformed by m (same as the inline ProjectBox_unk of
// atn/Geometry_unk.cpp with a 3x4 matrix); name invented
inline AABB_unk ViewBox_unk(const Mtx m, const AABB_unk* box) {
    float maxZ = box->mMax.z;
    float maxY = box->mMax.y;
    float maxX = box->mMax.x;
    float minX = box->mMin.x;
    float minY = box->mMin.y;
    float minZ = box->mMin.z;
    Vector3 corners[8];
    corners[0] = Vector3(maxX, maxY, maxZ);
    corners[1] = Vector3(minX, maxY, maxZ);
    corners[2] = Vector3(maxX, minY, maxZ);
    corners[3] = Vector3(minX, minY, maxZ);
    corners[4] = Vector3(maxX, maxY, minZ);
    corners[5] = Vector3(minX, maxY, minZ);
    corners[6] = Vector3(maxX, minY, minZ);
    corners[7] = Vector3(minX, minY, minZ);
    PSMTXMultVecArray(m, corners, corners, 8);
    Vector3 max(corners[0]);
    Vector3 min(corners[0]);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[1].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[2].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[3].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[4].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[5].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[6].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[7].x);
    return AABB_unk(min, max);
}

// NONMATCHING: 93.2% - scheduling of the box reads and of the corner temporaries, registers of the
// screen width/height
// inferred: the box (world coordinates) is hidden by the occlusion buffer: bounds of its corners in view
// space, outside the near/far range -> visible; screen rectangle, too small -> hidden, too large ->
// visible, else tested against the buffer pyramid from its smallest level (fn_800B64DC)
extern "C" int fn_800B5994(atn::LayerOcclusionCPU* layer, const atn::AABB_unk* box) {
    if (layer->m2C == 0) {
        return 0;
    }
    atn::AABB_unk view = ViewBox_unk(atn::RenderManager::instance_->mCamera.mView.m, box);
    atn::RenderManager* rm = atn::RenderManager::instance_;
    if (view.mMin.z < -rm->mCamera.mFar || view.mMax.z > -rm->mCamera.mNear) {
        if (lbl_8087EFA8->m1DC != 0) {
            fn_8006FE08(box, 0xFFFFFFFF);
        }
        return 0;
    }
    atn::AABB_unk screen;
    fn_800704DC(&screen, &view);
    float width = lbl_8087EEE0->mWidth;
    float height = lbl_8087EEE0->mHeight;
    screen.mMin.x = atn::ClampF_unk(screen.mMin.x, lbl_8087D898, width);
    screen.mMin.y = atn::ClampF_unk(screen.mMin.y, lbl_8087D89C, height);
    screen.mMax.x = atn::ClampF_unk(screen.mMax.x, lbl_8087D8A0, width);
    screen.mMax.y = atn::ClampF_unk(screen.mMax.y, lbl_8087D8A4, height);
    float h = screen.mMax.y - screen.mMin.y;
    float w = screen.mMax.x - screen.mMin.x;
    float minSize = lbl_8087EFA8->m1F4;
    if (w < minSize && h < minSize && w * h < minSize) {
        if (lbl_8087EFA8->m1DC != 0) {
            lbl_8087EEB0->fn_800610A4(0xFFFFFF00, screen.mMin.x, screen.mMin.y, 0.0f, w, h);
        }
        return 1;
    }
    float scaleX = 32.0f / lbl_8087EEE0->mWidth;
    float scaleY = 32.0f / lbl_8087EEE0->mHeight;
    int x0 = atn::ClampRow_unk((int)(float)floor(screen.mMin.x * scaleX), lbl_8087D8A8, lbl_8087D8AC);
    int y0 = atn::ClampRow_unk((int)(float)floor(screen.mMin.y * scaleY), lbl_8087D8B0, lbl_8087D8B4);
    int x1 = atn::ClampRow_unk((int)(float)ceil(screen.mMax.x * scaleX), lbl_8087D8B8, lbl_8087D8BC);
    int y1 = atn::ClampRow_unk((int)(float)ceil(screen.mMax.y * scaleY), lbl_8087D8C0, lbl_8087D8C4);
    if (x1 - x0 >= 31 || y1 - y0 >= 31) {
        if (lbl_8087EFA8->m1E4 != 0) {
            fn_8006FE08(box, 0xFF0000FF);
        }
        return 0;
    }
    if (atn::fn_800B64DC(layer, x0, y0, x1, y1, screen.mMin.z, 4) != 0) {
        if (lbl_8087EFA8->m1DC != 0) {
            lbl_8087EEB0->fn_800610A4(0xFFFF0000, screen.mMin.x, screen.mMin.y, 0.0f, w, h);
        }
        if (lbl_8087EFA8->m1E4 != 0) {
            fn_8006FE08(box, 0xFF00FFFF);
        }
        return 1;
    }
    if (lbl_8087EFA8->m1DC != 0) {
        lbl_8087EEB0->fn_800610A4(0xFF7F7F7F, screen.mMin.x, screen.mMin.y, 0.0f, w, h);
    }
    if (lbl_8087EFA8->m1E4 != 0) {
        fn_8006FE08(box, 0xFF00FF00);
    }
    return 0;
}

extern "C" {
float lbl_8087D8C8 = 0.0f;
float lbl_8087D8CC = 0.0f;
float lbl_8087D8D0 = 0.0f;
float lbl_8087D8D4 = 0.0f;
int lbl_8087D8D8 = 0;
int lbl_8087D8DC = 31;
int lbl_8087D8E0 = 0;
int lbl_8087D8E4 = 31;
int lbl_8087D8E8 = 0;
int lbl_8087D8EC = 31;
int lbl_8087D8F0 = 0;
int lbl_8087D8F4 = 31;
}

float fn_800B65B4(LayerOcclusionCPU* layer, int x0, int y0, int x1, int y1, float depth);

// NONMATCHING: 99.6% - only the data: the zero limits go to .sbss here (the original keeps them in .sdata, as
// with explicit_zero_data, which is only approved for atn/PostFxColorMatrix)
// inferred: part of the box (world coordinates) that is hidden: projects it, takes the cells of the 32x32
// buffer it covers and tests them at its nearest depth
float fn_800B6218(LayerOcclusionCPU* layer, const AABB_unk* box) {
    AABB_unk screen;
    fn_8006FEBC(&screen, box);
    float width = lbl_8087EEE0->mWidth;
    float height = lbl_8087EEE0->mHeight;
    screen.mMin.x = ClampF_unk(screen.mMin.x, lbl_8087D8C8, width);
    screen.mMin.y = ClampF_unk(screen.mMin.y, lbl_8087D8CC, height);
    screen.mMax.x = ClampF_unk(screen.mMax.x, lbl_8087D8D0, width);
    screen.mMax.y = ClampF_unk(screen.mMax.y, lbl_8087D8D4, height);
    float scaleX = 32.0f / lbl_8087EEE0->mWidth;
    float scaleY = 32.0f / lbl_8087EEE0->mHeight;
    int x0 = ClampRow_unk((int)(float)floor(screen.mMin.x * scaleX), lbl_8087D8D8, lbl_8087D8DC);
    int y0 = ClampRow_unk((int)(float)floor(screen.mMin.y * scaleY), lbl_8087D8E0, lbl_8087D8E4);
    int x1 = ClampRow_unk((int)(float)ceil(screen.mMax.x * scaleX), lbl_8087D8E8, lbl_8087D8EC);
    int y1 = ClampRow_unk((int)(float)ceil(screen.mMax.y * scaleY), lbl_8087D8F0, lbl_8087D8F4);
    return fn_800B65B4(layer, x0, y0, x1, y1, screen.mMin.z);
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
__declspec(noinline) float fn_800B65B4(LayerOcclusionCPU* layer, int x0, int y0, int x1, int y1, float depth) {
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
