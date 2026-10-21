// atn/ModelDraw_unk.cpp: GX drawing of the meshes of a model (file name invented). atn::Model
// (fn_8008E818) builds a DrawEntry_unk on the stack and calls fn_8009B93C; every visible node
// draws its parts with fn_8009D58C, which either queues a draw command in the camera (sorted
// later) or draws it at once with fn_8009DE60. All names are invented.
#include "atn/GXDecl.h"
#include "atn/LH_Model.h"
#include "atn/Shader_unk.h"
#include "atn/Sphere.h"
#include "atn/Texture_unk.h"
#include "game/GimmickCommon.h"
#include "math/Vec.h"
#include "std/new.h"

// inferred: up to four lights sorted by distance (atn/LightUtil_unk.cpp). The two selectors are
// called here on uninitialized temporaries: they return the set by value (declared here as
// static members; include/atn/Light_unk.h declares them as members of an existing set, which
// gives the same symbols)
struct Light_unk;
struct LightList_unk;
struct LightSet_unk {
    LightSet_unk();
    static LightSet_unk fn_8007A36C(LightList_unk* list, const atn::Vector3& pos, float radius, int arg);
    static LightSet_unk fn_8007A41C(LightList_unk* list);

    int mCount;
    Light_unk* mLight[4];
    float mDist[4];
};
extern "C" LightList_unk* lbl_8087EEF8;

extern "C" double cos(double x);
extern "C" double sin(double x);

extern "C" {
void GXLoadPosMtxImm(const float mtx[][4], unsigned int id);
void GXLoadNrmMtxImm(const float mtx[][4], unsigned int id);
void GXSetCurrentMtx(unsigned int id);
void GXSetChanMatColor(int chan, GXColor color);
void GXSetChanAmbColor(int chan, GXColor color);
void GXSetTevColor(int id, GXColor color);
void GXSetIndTexMtx(int id, const float offset[2][3], signed char scaleExp);
void GXSetTevAlphaIn(int stage, int a, int b, int c, int d);
void GXSetTevAlphaOp(int stage, int op, int bias, int scale, unsigned char clamp, int outReg);
void GXSetArray(int attr, const void* base, unsigned char stride);
void GXCallDisplayList(const void* list, unsigned int size);
}

// inferred: PSMTXInverse + transpose (normal matrix of a position matrix)
extern "C" void fn_805F8DA0(const Mtx src, Mtx dst);
// inferred: texture matrix from scale, rotation (sin, cos) and translation
extern "C" void fn_800D5C84(Mtx m, float sx, float sy, float s, float c, float tx, float ty);

// inferred: default UV transform of a texture stage (const object of atn/MaterialParser_unk)
extern "C" const float lbl_80880B88; // scale u
extern "C" const float lbl_80880B8C; // scale v
extern "C" const float lbl_80880B90; // rotation
extern "C" const float lbl_80880B94; // translation u
extern "C" const float lbl_80880B98; // translation v

// inferred: UV transform of a texture stage {scale, rotation, translation}
struct DrawUv_unk {
    float mScaleU;
    float mScaleV;
    float mRot;
    float mTransU;
    float mTransV;
};

// inferred: texture stage of a material (0x18 bytes, see atn/Shader_unk.cpp)
struct TexStage_unk {
    unsigned char m00[4];
    atn::Texture_unk* m04; // inferred: texture
    DrawUv_unk* m08;       // inferred: UV transform
    DrawUv_unk* m0C;       // inferred: indirect UV transform
    signed char m10;       // inferred: texture id
    unsigned char m11[0x18 - 0x11];
};

// inferred: texture of a shader (12 bytes, ShaderTexEntry_unk of atn/Shader_unk.h)
struct DrawTexEntry_unk {
    int mId;
    int mIndex;
    TexStage_unk* mBind;
};

// inferred: colour channel of a material description (0x18 bytes)
struct DrawChan_unk {
    int m00; // inferred: channel id
    int m04; // inferred: lighting enabled
    unsigned char m08[0x10 - 0x08];
    int m10; // inferred: diffuse function
    int m14; // inferred: attenuation
};

struct DrawTexGen_unk {
    unsigned char m00[0x18];
    int m18; // inferred: texture matrix id
};

struct DrawTexMtxSrc_unk {
    unsigned char m00[0x10];
    int m10; // inferred: projection mode
    int m14;
    int m18;
};

// inferred: indirect texture matrix of a description (0x10 bytes)
struct DrawIndMtx_unk {
    int m00; // inferred: texture
    int m04;
    int m08;
    int m0C;
};

// inferred: TEV stage of a description (0x88 bytes)
struct DrawTevStage_unk {
    unsigned char m00[0x2C];
    int m2C; // inferred: colour op
    int m30; // inferred: colour bias
    unsigned char m34[0x38 - 0x34];
    int m38; // inferred: colour clamp
    int m3C; // inferred: colour register
    unsigned char m40[0x88 - 0x40];
};

// inferred: material description (atn::ShaderDesc_unk of atn/Shader_unk.h)
struct DrawDesc_unk {
    unsigned char m000[0x20];
    GXColor m020;
    unsigned char m024[0x54 - 0x24];
    GXColor m054;
    unsigned char m058[0x80 - 0x58];
    int m080;              // inferred: number of colour channels
    DrawChan_unk m084[4];  // inferred: colour channels
    unsigned char m0E4_pad[0xE4 - 0xE4];
    int m0E4;              // inferred: number of texture stages
    unsigned char m0E8[0xEC - 0xE8];
    int m0EC;              // inferred: number of texture coordinates
    DrawTexGen_unk** m0F0;
    unsigned char m0F4[0xF8 - 0xF4];
    DrawTexMtxSrc_unk** m0F8;
    int m0FC;              // inferred: number of indirect matrices
    unsigned char m100[0x140 - 0x100];
    DrawIndMtx_unk m140[4];
    int m180_pad[4];
    int m190; // inferred: number of TEV stages
    unsigned char m194[0x1BC - 0x194];
    DrawTevStage_unk m1BC_pad[1];
};

// inferred: shader of a material (ShaderTex_unk of atn/Shader_unk.h)
struct DrawShader_unk {
    unsigned int mTexCount;
    DrawTexEntry_unk* mTex;
    unsigned int mOrderCount;
    int* mOrder;
    DrawDesc_unk* mDesc;
};

// inferred: GPU material (0x54 bytes, ShaderMaterial_unk of atn/Shader_unk.cpp)
struct ShaderMaterial_unk {
    unsigned int m00;
    unsigned int m04;
    DrawShader_unk* m08;
    TexStage_unk* m0C;
    unsigned char m10[4];
    unsigned int m14_0 : 1;
    unsigned int m14_1 : 3;
    unsigned int m14_4 : 8;
    unsigned int m14_12 : 1;
    unsigned int m14_13 : 6;
    unsigned int m14_19 : 1;
    unsigned int m14_20 : 1;
    unsigned int m14_21 : 1;
    unsigned int m14_22 : 3;
    unsigned int m14_25 : 1;
    unsigned int m14_26 : 6;
    GXColor m18;
    GXColor m1C;
    GXColor m20;
    GXColor m24;
    GXColor m28;
    GXColor m2C;
    unsigned char m30[4];
    int m34; // inferred: TEV scale of the last stage
    GXColor m38; // inferred: swap table
    int m3C;
    int m40;
    int m44; // inferred: sort mode (> 0: drawn later)
    unsigned char m48; // inferred: flags
    unsigned char m49; // inferred: pass mask
    unsigned char m4A; // inferred: number of texture stages
    unsigned char m4B;
    signed char m4C;
    unsigned char m4D;
    signed char m4E;
    signed char m4F;
    signed char m50;
    signed char m51;
    unsigned char m52[2];

    // inferred: texture stage of id `id`, searched from `i` (name invented)
    TexStage_unk* FindStage_unk(DrawTexEntry_unk* entry) {
        int i;
        int id = entry->mId;
        for (i = 0; i < m4A && m0C[i].m10 < id; i++) {
        }
        if (i < m4A && id == m0C[i].m10) {
            return &m0C[i];
        }
        return entry->mBind;
    }
};

extern "C" TexStage_unk* fn_80080528(ShaderMaterial_unk* mat, int id); // inferred: stage by id

// inferred: lens of the camera (Lens_unk of include/atn/Model_unk.h)
struct DrawLens_unk {
    unsigned char m00[0x0C];
    float m0C[4]; // inferred: ambient colour
    float m1C[4]; // inferred: constant colour
    unsigned char m2C[0x3C - 0x2C];
    unsigned char m3C[0x70 - 0x3C];
    int m70; // inferred: shadow enabled
    void* m74;
    unsigned char m78[0xC8 - 0x78];
    unsigned int mC8;
    unsigned char mCC[0x234 - 0xCC];
    int m234; // inferred: fog override
    unsigned char m238[0x23C - 0x238];
    int m23C;
    unsigned char m240[0x244 - 0x240];
    float m244[4];

    // inferred: shadow texture (name invented)
    void* GetShadow_unk() { return m70 != 0 ? (m74 != 0 ? m74 : (void*)m78) : 0; }
};
extern "C" int fn_800C24B4(DrawLens_unk* lens, int arg);
extern "C" void* fn_800C258C(DrawLens_unk* lens, const atn::Vector3* pos);

// inferred: mesh of a model
struct DrawArray_unk {
    int m00; // inferred: attribute (0xFF ends the list)
    void* m04;
    int m08;
};
struct DrawList_unk {
    unsigned char m00[0x10];
    unsigned int m10;
    void* m14;
};
struct DrawSubMesh_unk {
    unsigned char m00[0x10];
    int m10[10]; // inferred: matrix of each slot (-1 ends)
    unsigned char m38_pad[0x38 - 0x38];
    DrawList_unk* m38;
};
struct DrawMesh_unk {
    unsigned char m00[4];
    unsigned int m04;
    unsigned char m08[0x14 - 0x08];
    int m14; // inferred: matrix (-1: skinned)
    unsigned char m18[0x20 - 0x18];
    int m20;
    unsigned char m24[0x4C - 0x24];
    int m4C;
    DrawSubMesh_unk** m50;
    DrawArray_unk* m54;
};

// inferred: part of a node: material and mesh (12 bytes)
struct DrawPart_unk {
    int m00; // inferred: material
    int m04; // inferred: mesh
    int m08;
};
struct DrawNode_unk {
    unsigned char m00[0x10];
    const char* m10; // inferred: name
    unsigned char m14[0x18 - 0x14];
    int m18; // inferred: index
    unsigned char m1C[0x70 - 0x1C];
    int m70;
    DrawPart_unk* m74;
};
struct DrawModel_unk {
    unsigned char m00[0x40];
    DrawMesh_unk** m40;
    unsigned char m44[0x48 - 0x44];
    DrawNode_unk** m48;
    unsigned char m4C[0x50 - 0x4C];
    void* m50[1];
};

struct DrawRemap_unk {
    int mCount;
    int* mData;
};
struct DrawMaterials_unk {
    int mCount;
    ShaderMaterial_unk** mData;
};

// inferred: callbacks around the drawing of a mesh (InstCallback_unk of atn/ModelInstance.cpp)
struct DrawCallback_unk {
    void (*mFunc1)(void*, void*);
    void (*mFunc2)(void*, void*);
    void* mArg1;
    void* mArg2;
};

struct DrawColor_unk {
    float m[4];
};

// inferred: draw command of a mesh (0x4C bytes)
struct DrawCmd_unk {
    DrawCmd_unk(unsigned int flags, ShaderMaterial_unk* mat, DrawMesh_unk* mesh, DrawLens_unk* lens, Mtx* mtx,
                void* vtx, DrawArray_unk* arrays, DrawColor_unk color, void* m34, void* m38, DrawCallback_unk cb)
        : m04(flags), m08(mat), m0C(mesh), m10(lens), m14(mtx), m18(vtx), m1C(arrays), m20(0), m24(color),
          m34(m34), m38(m38), m3C(cb) {}

    int m00; // inferred: sort key
    unsigned int m04;
    ShaderMaterial_unk* m08;
    DrawMesh_unk* m0C;
    DrawLens_unk* m10;
    Mtx* m14;
    void* m18;
    DrawArray_unk* m1C;
    void* m20;
    DrawColor_unk m24;
    void* m34;
    void* m38;
    DrawCallback_unk m3C;
};
extern "C" void fn_800BC438(DrawCmd_unk* cmd); // inferred: computes the sort key

struct DrawSortKey_unk {
    DrawSortKey_unk(int key, int index) : mKey(key), mIndex(index) {}
    int mKey;
    int mIndex;
};

template <class T, int N>
struct DrawQueue_unk {
    void push_back(const T& v) {
        new (&mData[mSize]) T(v);
        mSize++;
    }
    unsigned int mSize;
    T mData[N];
};

// inferred: view of a pass of the camera
struct DrawPass_unk {
    unsigned char m00[0x0C];
    unsigned int m0C; // inferred: material mask
    unsigned int m10; // inferred: flag mask
    unsigned char m14[0x7C - 0x14];
    atn::Vector3 m7C; // inferred: eye
};

// inferred: global camera (lbl_8087EFB4)
struct DrawScene_unk {
    DrawPass_unk* mPass[11]; // inferred: view of each pass (3: the shadow)
    unsigned char m2C[0x15C - 0x2C];
    atn::Matrix3x4 m15C;
    unsigned char m18C[0x2F8 - 0x18C];
    int m2F8; // inferred: current pass
    DrawLens_unk* m2FC;
    unsigned char m300[0x418 - 0x300];
    unsigned char m418[0x74C - 0x418]; // inferred: frustum
    atn::Texture_unk m74C;
    unsigned char m77C[0x954 - 0x77C];
    int m954; // inferred: queueing enabled
    DrawQueue_unk<DrawSortKey_unk, 0x1000> mKeys;
    DrawQueue_unk<DrawCmd_unk, 0x1000> mCmds;

    void Push_unk(const DrawCmd_unk& cmd) {
        mKeys.push_back(DrawSortKey_unk(cmd.m00, mCmds.mSize));
        mCmds.push_back(cmd);
    }
};
extern "C" DrawScene_unk* lbl_8087EFB4;
extern "C" void fn_800C06B0(DrawScene_unk* scene, void* arg);
extern "C" void fn_800C08B0(DrawScene_unk* scene);
extern "C" void fn_800C0900(Mtx out, DrawScene_unk* scene, int mode, int a, int b, int c);
extern "C" atn::Texture_unk* fn_800C0A8C(DrawScene_unk* scene);
extern "C" atn::Texture_unk* fn_800C0A98(DrawScene_unk* scene);
extern "C" void fn_800BDB40(DrawScene_unk* scene, DrawLens_unk* lens);
extern "C" int fn_8004FF58(const void* frustum, const atn::Sphere* sphere);

// inferred: render options (lbl_8087EFA8)
struct DrawParams_unk {
    unsigned char m000[0x28];
    int m028;
    unsigned char m02C[0x104 - 0x2C];
    int m104;
    int m108;
    float m10C;
    float m110[4];
    unsigned char m120[0x134 - 0x120];
    int m134;
    unsigned char m138[0x13C - 0x138];
    int m13C;
    unsigned char m140[0x144 - 0x140];
    int m144;
    int m148;
    unsigned char m14C[0x150 - 0x14C];
    int m150;
    unsigned char m154[0x184 - 0x154];
    int m184;
    int m188;
    int m18C;
    unsigned char m190[0x208 - 0x190];
    int m208;
    unsigned char m20C[0x224 - 0x20C];
    int m224;
    int m228;
    float m22C;
    unsigned char m230[0x234 - 0x230];
    int m234;
    unsigned char m238[0x2F8 - 0x238];
    int m2F8;
    unsigned char m2FC[0x398 - 0x2FC];
    int m398;
    unsigned char m39C[0x3A0 - 0x39C];
    int m3A0;
};
extern "C" DrawParams_unk* lbl_8087EFA8;

// inferred: render state cache (lbl_8087EEE0)
struct DrawRender_unk;
extern "C" DrawRender_unk* lbl_8087EEE0;
extern "C" void fn_800761A8(DrawRender_unk* rs);
extern "C" void fn_800763C0(DrawRender_unk* rs, int stage, void* texture);
extern "C" void fn_8007642C(DrawRender_unk* rs, int index, void* light);
extern "C" void fn_80076760(DrawRender_unk* rs, int state, int value);
extern "C" void fn_80076A28(DrawRender_unk* rs, void* blend);
extern "C" void fn_80076BE4(DrawRender_unk* rs, void* vtx);
extern "C" void fn_80074EC8(DrawRender_unk* rs, DrawDesc_unk* desc, unsigned int flags, void* light, void* shadow,
                            void* arg);

// inferred: default textures (lbl_8087EEB0)
struct DrawTextures_unk {
    unsigned char m00[0x10];
    atn::Texture_unk m10; // inferred: white
    atn::Texture_unk m40;
};
extern "C" DrawTextures_unk* lbl_8087EEB0;

extern "C" unsigned char lbl_8087EFB0; // inferred: shadow pass
extern "C" ShaderManager_unk* lbl_8087EFE0;
extern "C" float lbl_8087D81C;
extern "C" void fn_800D594C(atn::Texture_unk* tex, const char* path); // inferred: loads a texture

extern "C" void* fn_8047614C(atn::LH_Model* model, int mesh); // inferred: vertex arrays
extern "C" void* fn_80476170(atn::LH_Model* model, int mesh); // inferred: vertex description

// inferred: draw state of a model (built on the stack by atn::Model, fn_8008E818)
struct DrawEntry_unk {
    DrawEntry_unk(atn::LH_Model* model, Mtx* mtx, Mtx* nodes, int m18, DrawColor_unk* color, DrawMaterials_unk* mats,
                  DrawRemap_unk* remap, const atn::Sphere* sphere, int* m40, unsigned int flags, DrawLens_unk* lens,
                  int m98, const DrawCallback_unk* cb);

    int m00; // inferred: pass
    int m04;
    atn::LH_Model* m08;
    DrawModel_unk* m0C;
    Mtx* m10;
    Mtx* m14; // inferred: node matrices
    int m18;
    DrawColor_unk* m1C;
    DrawMaterials_unk* m20;
    DrawRemap_unk* m24;
    int m28;
    float m2C; // inferred: largest scale
    atn::Sphere m30;
    int* m40; // inferred: node remap
    unsigned char* m44; // inferred: node visibility
    int m48; // inferred: only the "op_" nodes
    unsigned char* m4C; // inferred: material visibility
    short* m50; // inferred: nodes to draw
    unsigned int m54;
    int m58;
    void* m5C;
    LightSet_unk m60;
    DrawLens_unk* m84;
    DrawCallback_unk m88;
    int m98;
};

extern "C" {
void fn_8009B93C(DrawEntry_unk* entry);
void fn_8009B9E8(ShaderMaterial_unk* mat, DrawMesh_unk* mesh, DrawSubMesh_unk* sub, Mtx* mtx, int* cache);
void fn_8009BB9C(ShaderMaterial_unk* mat, DrawMesh_unk* mesh, Mtx* mtx);
void fn_8009BF3C(DrawMesh_unk* mesh, ShaderMaterial_unk* mat, DrawLens_unk* lens, DrawColor_unk* color);
void fn_8009C508(ShaderMaterial_unk* mat);
void fn_8009C688(ShaderMaterial_unk* mat);
void fn_8009C84C(ShaderMaterial_unk* mat);
void fn_8009CC28(DrawMesh_unk* mesh, ShaderMaterial_unk* mat, DrawShader_unk* shader, int flag, float dist);
void fn_8009D1A4(ShaderMaterial_unk* mat);
void fn_8009D58C(DrawEntry_unk* entry, DrawNode_unk* node, DrawPart_unk* part, int visible);
void fn_8009DCE0(ShaderMaterial_unk* mat);
void fn_8009DE60(DrawCmd_unk* cmd, float dist);
void fn_8009E448(DrawEntry_unk* entry, DrawNode_unk* root);
}

static float sFogStart_unk = 0.8f; // inferred: start of the fog (ratio of its distance)
// inferred: explicitly zero-initialised (they come before the guards of fn_8009D1A4 in .sbss)
static unsigned char sShadowRecv_unk = 0; // inferred: shadow receiver
static unsigned char sSecondPass_unk = 0; // inferred: second pass of the transparent parts
static ShaderMaterial_unk* sLastMat_unk = 0; // inferred: last material
static int sMipLoaded_unk = 0; // inferred: the mip test textures are loaded

// inferred: is the texture loaded (inline of the texture class)
static inline int TexReady_unk(atn::Texture_unk* t) {
    return t->mImage != 0 ||
           (((GimmickUvOwner_unk*)t)->fn_800D59B8() == 0 && t->m20.fn_80473EFC() == 3);
}

// inferred: texture matrix of a UV transform
static inline void UvMtx_unk(Mtx m, DrawUv_unk* uv) {
    float sx = uv != 0 ? uv->mScaleU : lbl_80880B88;
    float sy = uv != 0 ? uv->mScaleV : lbl_80880B8C;
    float tx = uv != 0 ? uv->mTransU : lbl_80880B94;
    float ty = uv != 0 ? uv->mTransV : lbl_80880B98;
    float rot = uv != 0 ? uv->mRot : lbl_80880B90;
    if (rot != 0.0f) {
        fn_800D5C84(m, sx, sy, (float)sin(rot), (float)cos(rot), tx, ty);
    } else {
        m[0][0] = sx;
        m[0][1] = 0.0f;
        m[0][2] = 0.0f;
        m[0][3] = sx * -tx;
        m[1][0] = 0.0f;
        m[1][1] = sy;
        m[1][2] = 0.0f;
        m[1][3] = sy * (ty - 1.0f) + 1.0f;
        m[2][0] = 0.0f;
        m[2][1] = 0.0f;
        m[2][2] = 1.0f;
        m[2][3] = 0.0f;
    }
}

static inline unsigned char SwapR_unk(GXColor c) { return c.r; }
static inline unsigned char SwapG_unk(GXColor c) { return c.g; }
static inline unsigned char SwapB_unk(GXColor c) { return c.b; }
static inline unsigned char SwapA_unk(GXColor c) { return c.a; }

// NONMATCHING: 98.37% - r4/r5 swapped in the dead copy of the camera matrix (__MtxCopy, as the __VecCopy family)
void fn_8009B93C(DrawEntry_unk* entry) {
    atn::Matrix3x4 view = lbl_8087EFB4->m15C;
    if ((entry->m04 & 0x20000) && lbl_8087EFB0 == 0) {
        sSecondPass_unk = 1;
        fn_8009E448(entry, entry->m0C->m48[0]);
        sSecondPass_unk = 0;
    }
    fn_8009E448(entry, entry->m0C->m48[0]);
}

// NONMATCHING: 98.30% - saved registers of the two loops shifted by one
void fn_8009B9E8(ShaderMaterial_unk* mat, DrawMesh_unk* mesh, DrawSubMesh_unk* sub, Mtx* mtx, int* cache) {
    MtxPtr m;
    if (mesh->m14 != -1) {
        mtx += mesh->m14;
        GXLoadPosMtxImm(*mtx, 0);
        Mtx n;
        fn_805F8DA0(*mtx, n);
        n[0][3] = 0.0f;
        n[1][3] = 0.0f;
        n[2][3] = 0.0f;
        GXLoadNrmMtxImm(n, 0);
        GXLoadPosMtxImm(n, 3);
        GXSetCurrentMtx(0);
    } else if ((sShadowRecv_unk == 0 && lbl_8087EFB0 != 0) || lbl_8087EFA8->m234 != 0) {
        for (int i = 0; i < 10; i++) {
            int idx = sub->m10[i];
            if (idx == -1) {
                break;
            }
            if (idx != cache[i]) {
                cache[i] = idx;
                GXLoadPosMtxImm(mtx[idx], i * 3);
            }
        }
    } else {
        for (int i = 0; i < 10; i++) {
            int idx = sub->m10[i];
            if (idx == -1) {
                break;
            }
            if (idx != cache[i]) {
                cache[i] = idx;
                m = mtx[idx];
                GXLoadPosMtxImm(m, i * 3);
                Mtx n;
                fn_805F8DA0(m, n);
                n[0][3] = 0.0f;
                n[1][3] = 0.0f;
                n[2][3] = 0.0f;
                GXLoadNrmMtxImm(n, i * 3);
                GXLoadTexMtxImm(n, i * 3 + 30, 0);
            }
        }
    }
}

// NONMATCHING: 82.64% - register allocation reversed (loop locals get the highest registers in the original), f30/f31 swapped
void fn_8009BB9C(ShaderMaterial_unk* mat, DrawMesh_unk* mesh, Mtx* mtx) {
    DrawDesc_unk* desc = mat->m08->mDesc;
    int texMtx = 0x40;
    for (int i = 0; i < desc->m0EC; i++) {
        DrawTexGen_unk* gen = desc->m0F0[i];
        DrawTexMtxSrc_unk* src = desc->m0F8[i];
        TexStage_unk* stage = mat->FindStage_unk(&mat->m08->mTex[mat->m08->mOrder[i]]);
        Mtx uvm;
        UvMtx_unk(uvm, stage->m08);
        int mode = src->m10;
        int skinned = mesh->m14 == -1;
        if (mode >= 1 && mode <= 4) {
            if (skinned == 0) {
                if (mode != 4) {
                    atn::Matrix3x4 m(mtx[mesh->m14]);
                    m.m[0][3] = 0.0f;
                    m.m[1][3] = 0.0f;
                    m.m[2][3] = 0.0f;
                    GXLoadTexMtxImm(m.m, gen->m18, 0);
                } else {
                    GXLoadTexMtxImm(mtx[mesh->m14], gen->m18, 0);
                }
            }
            Mtx proj;
            fn_800C0900(proj, lbl_8087EFB4, src->m10, src->m14, src->m18, 0);
            float t;
            t = uvm[0][2];
            uvm[0][2] = uvm[0][3];
            uvm[0][3] = t;
            t = uvm[1][2];
            uvm[1][2] = uvm[1][3];
            uvm[1][3] = t;
            Mtx out;
            PSMTXConcat(uvm, proj, out);
            GXLoadTexMtxImm(out, texMtx, 0);
            texMtx += 3;
        } else if (mode == 0) {
            if (skinned == 0) {
                GXLoadTexMtxImm(uvm, gen->m18, 1);
            } else {
                GXLoadTexMtxImm(uvm, texMtx, 0);
                texMtx += 3;
            }
        }
    }
}

// inferred: material and ambient colours of a material (written with paired singles in the
// original; written out twice there: each copy has its own white constant in .sdata2)
#define SET_MAT_COLOR_UNK(mat, lens, color)                    \
    {                                                                      \
    GXColor mc = mat->m18;                                                                          \
    GXColor ac;                                                                                     \
    GXColor src = mat->m1C;                                                                         \
    float amb[4];                                                                                   \
    amb[0] = (float)src.r / 255.0f;                                                                 \
    amb[1] = (float)src.g / 255.0f;                                                                 \
    amb[2] = (float)src.b / 255.0f;                                                                 \
    amb[3] = (float)src.a / 255.0f;                                                                 \
    mc.r = (unsigned char)(mc.r * (1.0f / 255.0f) * color->m[0] * 255.0f);                          \
    mc.g = (unsigned char)(mc.g * (1.0f / 255.0f) * color->m[1] * 255.0f);                          \
    mc.b = (unsigned char)(mc.b * (1.0f / 255.0f) * color->m[2] * 255.0f);                          \
    mc.a = (unsigned char)(mc.a * (1.0f / 255.0f) * color->m[3] * 255.0f);                          \
    ac.r = (unsigned char)(lens->m0C[0] * amb[0] * 255.0f);                                         \
    ac.g = (unsigned char)(lens->m0C[1] * amb[1] * 255.0f);                                         \
    ac.b = (unsigned char)(lens->m0C[2] * amb[2] * 255.0f);                                         \
    ac.a = (unsigned char)(lens->m0C[3] * amb[3] * 255.0f);                                         \
    GXSetChanMatColor(4, mc);                                                                       \
    if (mat->m4E == 1) {                                                                            \
        GXSetTevKColor(1, mc);                                                                      \
    } else {                                                                                        \
        GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};                                                   \
        GXSetTevKColor(1, white);                                                                   \
    }                                                                                               \
    GXColor k = {(unsigned char)(255.0f * lens->m1C[0]), (unsigned char)(255.0f * lens->m1C[1]),    \
                 (unsigned char)(255.0f * lens->m1C[2]), (unsigned char)(255.0f * lens->m1C[3])};   \
    GXSetTevKColor(2, k);                                                                           \
    GXSetChanAmbColor(4, ac);                                                                       \
    }

// NONMATCHING: 35.93% - the colours are scaled with hand-written paired-single code (GQR6, psq_l/psq_st
// quantized) that cannot be written without asm
void fn_8009BF3C(DrawMesh_unk* mesh, ShaderMaterial_unk* mat, DrawLens_unk* lens, DrawColor_unk* color) {
    if (mat == 0) {
        sLastMat_unk = 0;
        return;
    }
    if (mat->m14_20) {
        fn_800C06B0(lbl_8087EFB4, lens->m3C);
    } else {
        fn_800C08B0(lbl_8087EFB4);
    }
    if (mat == sLastMat_unk && lbl_8087EFA8->m3A0 != 0) {
        fn_8009DCE0(mat);
        SET_MAT_COLOR_UNK(mat, lens, color)
        if (mat->m4C) {
            DrawRender_unk* rs = lbl_8087EEE0;
            fn_8007642C(rs, 0, (void*)fn_800C24B4(lens, 0));
        }
        return;
    }
    DrawDesc_unk* desc = mat->m08->mDesc;
    sLastMat_unk = mat;
    GXSetChanMatColor(5, mat->m20);
    GXSetChanAmbColor(5, desc->m020);
    GXColor k = desc->m054;
    k.r = mat->m3C;
    k.g = mat->m40;
    GXSetTevKColor(0, k);
    GXSetTevColor(1, mat->m24);
    GXSetTevColor(2, mat->m28);
    GXSetTevColor(3, mat->m2C);
    SET_MAT_COLOR_UNK(mat, lens, color)
    if (mat->m4C) {
        DrawRender_unk* rs = lbl_8087EEE0;
        fn_8007642C(rs, 0, (void*)fn_800C24B4(lens, 0));
    }
    fn_8009C508(mat);
    fn_8009C688(mat);
    fn_8009C84C(mat);
    DrawRender_unk* rs = lbl_8087EEE0;
    fn_80076A28(rs, (unsigned char*)mat + 0x14);
    GXSetTevSwapModeTable(3, SwapR_unk(mat->m38), SwapG_unk(mat->m38), SwapB_unk(mat->m38), SwapA_unk(mat->m38));
    if (lbl_8087EFB4->m2F8 == 4 && mat->m14_19 == 1) {
        fn_80076760(rs, 6, 2);
    }
    fn_800761A8(lbl_8087EEE0);
}

// NONMATCHING: 90.72% - the original keeps the address of the light set in a saved register (one more saved register)
void fn_8009C508(ShaderMaterial_unk* mat) {
    DrawDesc_unk* desc = mat->m08->mDesc;
    DrawChan_unk* ch = desc->m084;
    for (int i = 0; i < desc->m080; i++, ch++) {
        int attn = 1;
        if (ch->m14 == 0) {
            attn = 0;
        }
        unsigned int mask = 1;
        int enable = 0;
        if (mat->m4C == -1 || (ch->m00 >= 2 && ch->m00 <= 3)) {
            enable = ch->m04 != 0;
        } else if (mat->m4C == 3) {
            enable = 1;
        } else if (attn == 0 && (mat->m4C & 2)) {
            enable = 1;
        }
        if (lbl_8087EFA8->m208 != 0) {
            enable = 0;
        }
        if (enable) {
            LightSet_unk lights = LightSet_unk::fn_8007A41C(lbl_8087EEF8);
            for (int j = 0; j < lights.mCount; j++) {
                mask |= 1 << (j + 2);
                fn_8007642C(lbl_8087EEE0, j + 2, lights.mLight[j]);
            }
        }
        int matSrc;
        int ambSrc;
        if ((ch->m00 & 1) == 0) {
            matSrc = mat->m4E;
            ambSrc = mat->m50;
        } else {
            matSrc = mat->m4F;
            ambSrc = mat->m51;
        }
        GXSetChanCtrl(ch->m00, enable, ambSrc, matSrc, attn ? mask : 2, ch->m10, attn);
    }
}

// NONMATCHING: 92.89% - register allocation of the loop
void fn_8009C688(ShaderMaterial_unk* mat) {
    DrawShader_unk* shader = mat->m08;
    int hint = 0;
    unsigned int n = shader->mTexCount;
    for (unsigned int i = 0; i < n; i++) {
        DrawTexEntry_unk* entry = &shader->mTex[i];
        int id = entry->mId;
        void* tex = 0;
        switch (id) {
        case 0x14:
            tex = fn_800C0A8C(lbl_8087EFB4);
            break;
        case 0x16:
            tex = fn_800C0A98(lbl_8087EFB4);
            break;
        case 0x15:
            tex = &lbl_8087EFB4->m74C;
            break;
        default: {
            TexStage_unk* stage;
            for (; hint < mat->m4A && mat->m0C[hint].m10 < id; hint++) {
            }
            if (hint < mat->m4A && id == mat->m0C[hint].m10) {
                stage = &mat->m0C[hint];
                hint++;
            } else {
                stage = entry->mBind;
            }
            if (stage != 0) {
                atn::Texture_unk* t = stage->m04;
                if (t != 0 && TexReady_unk(t)) {
                    tex = stage->m04;
                } else if (shader->mTex[i].mBind != 0) {
                    tex = shader->mTex[i].mBind->m04;
                }
            }
            break;
        }
        }
        if (id == 0xB && lbl_8087EFA8->m028 != 0) {
            tex = &lbl_8087EEB0->m40;
        }
        if (tex == 0) {
            tex = &lbl_8087EEB0->m10;
        }
        fn_800763C0(lbl_8087EEE0, shader->mTex[i].mIndex, tex);
    }
}

static inline const float& MaxRef_unk(const float& a, const float& b) { return a < b ? b : a; }

// NONMATCHING: 95.12% - register allocation and scheduling of the scale loop
void fn_8009C84C(ShaderMaterial_unk* mat) {
    DrawShader_unk* shader = mat->m08;
    DrawDesc_unk* desc = shader->mDesc;
    DrawIndMtx_unk* ind = desc->m140;
    for (int i = 0; i < desc->m0FC; i++, ind++) {
        DrawIndMtx_unk im = *ind;
        TexStage_unk* stage = mat->FindStage_unk(&mat->m08->mTex[shader->mOrder[im.m00]]);
        Mtx uvm;
        UvMtx_unk(uvm, stage->m0C);
        float m[2][3];
        m[0][0] = uvm[0][0];
        m[0][1] = uvm[0][1];
        m[0][2] = uvm[0][2];
        m[1][0] = uvm[1][0];
        m[1][1] = uvm[1][1];
        m[1][2] = uvm[1][2];
        int scale = 0;
        int first = 1;
        int again;
        do {
            again = 0;
            float mx = m[0][0];
            for (int r = 0; r < 2; r++) {
                for (int c = 0; c < 3; c++) {
                    mx = MaxRef_unk(mx, m[r][c]);
                }
            }
            if (mx >= 1.0f || mx < -1.0f) {
                again = 1;
                m[0][0] *= 0.5f;
                m[0][1] *= 0.5f;
                m[0][2] *= 0.5f;
                m[1][0] *= 0.5f;
                m[1][1] *= 0.5f;
                m[1][2] *= 0.5f;
                if (first == 0) {
                    scale++;
                }
                first = 0;
            }
        } while (again);
        GXSetIndTexMtx(i + 1, m, scale);
    }
}

// NONMATCHING: 99.12% - r3/r4 of the two globals swapped, f26-f31 shifted, colour temporary at 0x1c instead of 0x8
void fn_8009CC28(DrawMesh_unk* mesh, ShaderMaterial_unk* mat, DrawShader_unk* shader, int flag, float dist) {
    fn_800C08B0(lbl_8087EFB4);
    DrawLens_unk* lens = lbl_8087EFB4->m2FC;
    DrawParams_unk* p = lbl_8087EFA8;
    float c[4];
    c[0] = p->m110[0];
    c[1] = p->m110[1];
    c[2] = p->m110[2];
    c[3] = p->m110[3];
    int mode = p->m108;
    if (lens->m234 != 0) {
        c[0] = lens->m244[0];
        c[1] = lens->m244[1];
        c[2] = lens->m244[2];
        c[3] = lens->m244[3];
        mode = lens->m23C;
    }
    float t = 0.0f;
    float start = p->m10C * sFogStart_unk;
    if (dist > start) {
        t = (dist - start) / (p->m10C - start);
    }
    t = 1.0f - t;
    c[0] = (1.0f - c[0]) * t;
    c[1] = (1.0f - c[1]) * t;
    c[2] = (1.0f - c[2]) * t;
    if (mode == 1) {
        c[3] = 1.0f;
        c[0] = 1.0f - c[0];
        c[1] = 1.0f - c[1];
        c[2] = 1.0f - c[2];
    }
    GXColor col = {(unsigned char)(255.0f * c[0]), (unsigned char)(255.0f * c[1]), (unsigned char)(255.0f * c[2]),
                   (unsigned char)(255.0f * c[3])};
    int on = flag == 0;
    if (on) {
        on = mat->m14_0;
    }
    sShadowRecv_unk = on != 0;
    GXSetChanMatColor(4, col);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    DrawRender_unk* rs = lbl_8087EEE0;
    if (on) {
        fn_80076760(rs, 3, 1);
        fn_80076760(rs, 0, 1);
        fn_80076760(rs, 1, 4);
        fn_80076760(rs, 2, 0x40);
        fn_800761A8(rs);
        TexStage_unk* stage = fn_80080528(mat, 3);
        if (stage == 0) {
            stage = fn_80080528(mat, 0);
        }
        fn_800763C0(rs, 0, stage->m04);
        Mtx uvm;
        UvMtx_unk(uvm, stage->m08);
        int skinned = mesh->m14 == -1;
        GXSetNumTevStages(1);
        GXSetNumTexGens(1);
        GXSetTevSwapModeTable(3, SwapR_unk(mat->m38), SwapG_unk(mat->m38), SwapB_unk(mat->m38), SwapA_unk(mat->m38));
        GXSetTevSwapMode(0, 0, 3);
        GXSetTevOrder(0, 0, 0, 4);
        GXSetTevColorIn(0, 15, 15, 15, 10);
        GXSetTevColorOp(0, 0, 0, 0, 1, 0);
        GXSetTevAlphaIn(0, 7, 7, 7, 4);
        GXSetTevAlphaOp(0, 0, 0, 0, 1, 0);
        if (skinned == 0) {
            GXLoadTexMtxImm(uvm, 0x1E, 1);
            GXSetTexCoordGen2(0, 1, 4, 0x1E, 0, 0x7D);
        } else {
            GXLoadTexMtxImm(uvm, 0x40, 0);
            GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x40);
        }
    } else {
        fn_80076760(rs, 3, 0);
        fn_80076760(rs, 0, 0);
        fn_800761A8(rs);
        GXSetNumTevStages(1);
        GXSetNumTexGens(0);
        GXSetTevSwapMode(0, 0, 0);
        GXSetTevOp(0, 4);
        GXSetTevOrder(0, 0xFF, 0xFF, 4);
    }
}

// NONMATCHING: 99.84% - stage and texture registers swapped in the width loop
void fn_8009D1A4(ShaderMaterial_unk* mat) {
    GXColor white = {0xFF, 0xFF, 0xFF, 0xFF};
    GXSetChanMatColor(4, white);
    GXSetChanMatColor(5, white);
    GXSetNumChans(1);
    GXSetChanCtrl(4, 0, 0, 0, 0, 2, 2);
    GXSetNumTevStages(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTevOp(0, 3);
    GXSetNumTexGens(1);
    GXSetTevOrder(0, 0, 0, 0xFF);
    GXSetTexCoordGen2(0, 1, 4, 0x3C, 0, 0x7D);
    static atn::Texture_unk sMip64;
    static atn::Texture_unk sMip128;
    static atn::Texture_unk sMip256;
    static atn::Texture_unk sMip512;
    static atn::Texture_unk sMip1024;
    if (sMipLoaded_unk == 0) {
        sMipLoaded_unk = 1;
        fn_800D594C(&sMip64, "common/mip64.texture");
        fn_800D594C(&sMip128, "common/mip128.texture");
        fn_800D594C(&sMip256, "common/mip256.texture");
        fn_800D594C(&sMip512, "common/mip512.texture");
        fn_800D594C(&sMip1024, "common/mip1024.texture");
    }
    TexStage_unk* stage;
    int width = 0;
    for (int i = 0; i < mat->m08->mDesc->m0E4; i++) {
        stage = &mat->m0C[i];
        if (stage->m10 == 0) {
            if (TexReady_unk(stage->m04)) {
                width = GXGetTexObjWidth(&stage->m04->mObj);
            }
            break;
        }
    }
    atn::Texture_unk* tex;
    if (width > 512) {
        tex = &sMip1024;
    } else if (width > 256) {
        tex = &sMip512;
    } else if (width > 128) {
        tex = &sMip256;
    } else if (width > 64) {
        tex = &sMip128;
    } else {
        tex = &sMip64;
    }
    DrawParams_unk* p = lbl_8087EFA8;
    int aniso = p->m224;
    int edge = 0;
    if (aniso != 0) {
        edge = p->m228;
    }
    float bias = p->m22C;
    int levels = tex->mImage != 0 ? tex->m2F : tex->m20.fn_80478198();
    GXInitTexObjLOD(&tex->mObj, 5, 1, 0.0f, levels - 1, bias, 0, edge, aniso);
    fn_800763C0(lbl_8087EEE0, 0, tex);
}

static inline int IsShadowA_unk(DrawEntry_unk* entry, DrawLens_unk* lens) {
    return (lens->mC8 & 1) == 1 && (entry->m04 & 0x100) == 0x100;
}
static inline int IsShadowB_unk(DrawEntry_unk* entry, DrawLens_unk* lens) {
    return (lens->mC8 & 2) == 2 && (entry->m04 & 8) == 8;
}

// NONMATCHING: 83.57% - flag tests and the queue capacity test (orc/srwi idiom) not reproduced, register allocation
void fn_8009D58C(DrawEntry_unk* entry, DrawNode_unk* node, DrawPart_unk* part, int visible) {
    int index;
    DrawRemap_unk* remap = entry->m24;
    if (remap->mCount != 0) {
        index = remap->mData[part->m00];
    } else {
        index = part->m00;
    }
    DrawMesh_unk* mesh = entry->m0C->m40[part->m04];
    ShaderMaterial_unk* mat;
    if (index >= 0) {
        if (entry->m4C != 0 && entry->m4C[index] == 0) {
            return;
        }
        mat = entry->m20->mData[index];
    } else {
        mat = (ShaderMaterial_unk*)lbl_8087EFE0->fn_800C607C(index);
    }
    if (mat == 0) {
        return;
    }
    if (sSecondPass_unk != 0 && (mat->m48 & 2) != 2) {
        return;
    }
    DrawPass_unk* pass = lbl_8087EFB4->mPass[lbl_8087EFB4->m2F8];
    if (lbl_8087EFB0 == 0) {
        unsigned char m = mat->m49;
        if (m != (m & pass->m0C)) {
            return;
        }
    }
    unsigned int flags = 0;
    if (mesh->m14 == -1) {
        flags |= 1;
    }
    DrawParams_unk* p = lbl_8087EFA8;
    if (p->m148 != 0) {
        flags |= 4;
    } else {
        unsigned int ef = entry->m04;
        if (((ef & 8) == 8 && p->m104 != 0) || p->m13C != 0) {
            if (visible) {
                if (p->m184 != 0) {
                    flags |= 2;
                } else if (p->m188 != 0) {
                    flags |= 0x10;
                } else if ((ef & 0x8000) == 0x8000 || (mat->m48 & 1) == 1) {
                    flags |= 0x10;
                } else {
                    flags |= 2;
                }
            }
            if (p->m134 != 0) {
                flags &= ~2;
            }
        } else if ((ef & 0x100) == 0x100 && p->m144 != 0) {
            flags |= 4;
        }
    }
    if (p->m2F8 != 0) {
        DrawLens_unk* lens = entry->m84;
        int a = 1;
        int b = 1;
        if (!IsShadowA_unk(entry, lens) && !IsShadowB_unk(entry, lens)) {
            b = 0;
        }
        if (b == 0 && (entry->m04 & 0x100000) != 0x100000) {
            a = 0;
        }
        int c = 0;
        if (a != 0 && (entry->m04 & 0x200000) != 0x200000) {
            c = 1;
        }
        if (c != 0 && lens->GetShadow_unk() != 0) {
            flags |= 8;
        }
    }
    if ((entry->m04 & 0x80000) == 0x80000) {
        flags |= 0x20;
        if ((flags & 8) && (flags & 4)) {
            flags &= ~4;
        }
    }
    flags &= pass->m10;
    if (entry->m58 != 0) {
        flags |= 0x40;
    }
    DrawArray_unk* arrays;
    if (mesh->m04 >= 2) {
        arrays = mesh->m54;
    } else {
        arrays = (DrawArray_unk*)fn_8047614C(entry->m08, part->m04);
    }
    int sort;
    if (lbl_8087EFB0 == 0) {
        sort = mat->m44;
    } else {
        sort = 0;
    }
    DrawCmd_unk cmd(flags, mat, mesh, entry->m84, entry->m14, fn_80476170(entry->m08, part->m04), arrays,
                    *entry->m1C, (void*)entry->m98, &lbl_8087D81C, entry->m88);
    fn_800BC438(&cmd);
    cmd.m38 = entry->m0C->m50[part->m00];
    float dist = 0.0f;
    if (lbl_8087EFB0 != 0) {
        DrawPass_unk* view = lbl_8087EFB4->mPass[3];
        atn::Vector3 d(view->m7C.x - entry->m30.mCenter.x, view->m7C.y - entry->m30.mCenter.y,
                       view->m7C.z - entry->m30.mCenter.z);
        dist = PSVECMag(&d);
    }
    if (flags & 4) {
        cmd.m20 = entry->m5C != 0 ? entry->m5C : fn_800C258C(entry->m84, &entry->m30.mCenter);
    }
    DrawScene_unk* scene = lbl_8087EFB4;
    int queue = 0;
    if (scene->m954 != 0 && scene->mCmds.mSize < 0x1000) {
        queue = 1;
    }
    if (queue && (lbl_8087EFA8->m398 != 0 || scene->m2F8 == 8)) {
        lbl_8087EFB4->Push_unk(cmd);
    } else if (sort > 0) {
        lbl_8087EFB4->Push_unk(cmd);
    } else {
        fn_8009DE60(&cmd, dist);
    }
}

// NONMATCHING: 95.19% - register allocation (loop locals get the highest registers in the original)
void fn_8009DCE0(ShaderMaterial_unk* mat) {
    DrawShader_unk* shader = mat->m08;
    int hint = 0;
    for (unsigned int i = 0; i < shader->mTexCount; i++) {
        DrawTexEntry_unk* entry = &shader->mTex[i];
        int id = entry->mId;
        int index = entry->mIndex;
        if (id == 0xB) {
            TexStage_unk* stage;
            for (; hint < mat->m4A && mat->m0C[hint].m10 < id; hint++) {
            }
            if (hint < mat->m4A && id == mat->m0C[hint].m10) {
                stage = &mat->m0C[hint];
                hint++;
            } else {
                stage = entry->mBind;
            }
            void* tex = 0;
            if (stage != 0) {
                atn::Texture_unk* t = stage->m04;
                if (t != 0 && TexReady_unk(t)) {
                    tex = stage->m04;
                } else if (shader->mTex[i].mBind != 0) {
                    tex = shader->mTex[i].mBind->m04;
                }
            }
            if (id == 0xB && lbl_8087EFA8->m028 != 0) {
                tex = &lbl_8087EEB0->m40;
            }
            if (tex == 0) {
                tex = &lbl_8087EEB0->m10;
            }
            fn_800763C0(lbl_8087EEE0, index, tex);
        }
    }
}

// NONMATCHING: 97.48% - register allocation
void fn_8009DE60(DrawCmd_unk* cmd, float dist) {
    ShaderMaterial_unk* mat = cmd->m08;
    DrawLens_unk* lens = cmd->m10;
    unsigned int flags = cmd->m04;
    DrawMesh_unk* mesh = cmd->m0C;
    void* vtx = cmd->m18;
    DrawArray_unk* arrays = cmd->m1C;
    Mtx* mtx = cmd->m14;
    DrawShader_unk* shader = mat->m08;
    fn_800BDB40(lbl_8087EFB4, lens);
    if (cmd->m3C.mFunc1 != 0) {
        cmd->m3C.mFunc1(cmd->m3C.mArg1, cmd->m3C.mArg2);
    }
    if (lbl_8087EFB0 != 0) {
        fn_8009CC28(mesh, mat, shader, (flags >> 6) & 1, dist);
    } else if (lbl_8087EFA8->m234 != 0) {
        fn_8009D1A4(mat);
    } else {
        fn_8009BF3C(mesh, mat, lens, &cmd->m24);
        if ((flags & 0x20) == 0x20 && (*(unsigned int*)cmd->m34 & 2)) {
            DrawRender_unk* rs = lbl_8087EEE0;
            fn_80076760(rs, 0, 1);
            fn_80076760(rs, 1, 4);
            fn_80076760(rs, 2, 0xC0);
            fn_800761A8(rs);
        }
        if (mesh->m20 != -1) {
            lbl_8087EFA8->m18C = 0;
        } else {
            lbl_8087EFA8->m18C = 1;
        }
        fn_80074EC8(lbl_8087EEE0, shader->mDesc, flags, cmd->m20, lens->GetShadow_unk(), cmd->m34);
        DrawDesc_unk* desc = shader->mDesc;
        int last = desc->m190 - 1;
        DrawTevStage_unk* st = &desc->m1BC_pad[last];
        GXSetTevColorOp(last, st->m2C, st->m30, mat->m34, st->m38, st->m3C);
    }
    fn_80076BE4(lbl_8087EEE0, vtx);
    for (; arrays->m00 != 0xFF; arrays++) {
        GXSetArray(arrays->m00, arrays->m04, arrays->m08);
    }
    if (lbl_8087EFB0 == 0 && lbl_8087EFA8->m234 == 0) {
        fn_8009BB9C(mat, mesh, mtx);
    }
    int cache[10];
    for (int i = 0; i < 10; i++) {
        cache[i] = -1;
    }
    if (sSecondPass_unk != 0) {
        DrawRender_unk* rs = lbl_8087EEE0;
        fn_80076760(rs, 0xB, 0);
        fn_80076760(rs, 8, 1);
        fn_80076760(rs, 9, 2);
        fn_80076760(rs, 10, 1);
        fn_800761A8(rs);
        for (int i = 0; i < mesh->m4C; i++) {
            DrawSubMesh_unk* sub = mesh->m50[i];
            fn_8009B9E8(mat, mesh, sub, mtx, cache);
            GXCallDisplayList(sub->m38->m14, sub->m38->m10);
        }
        fn_80076A28(rs, (unsigned char*)mat + 0x14);
        fn_800761A8(rs);
    } else {
        for (int i = 0; i < mesh->m4C; i++) {
            DrawSubMesh_unk* sub = mesh->m50[i];
            fn_8009B9E8(mat, mesh, sub, mtx, cache);
            GXCallDisplayList(sub->m38->m14, sub->m38->m10);
        }
    }
    if (cmd->m3C.mFunc2 != 0) {
        cmd->m3C.mFunc2(cmd->m3C.mArg1, cmd->m3C.mArg2);
    }
    sShadowRecv_unk = 0;
}

static inline const float& MaxF_unk(const float& a, const float& b) { return a > b ? a : b; }

// NONMATCHING: 87.64% - order of the copies of the light set and of the scale vector
DrawEntry_unk::DrawEntry_unk(atn::LH_Model* model, Mtx* mtx, Mtx* nodes, int m18, DrawColor_unk* color,
                             DrawMaterials_unk* mats, DrawRemap_unk* remap, const atn::Sphere* sphere, int* m40,
                             unsigned int flags, DrawLens_unk* lens, int m98, const DrawCallback_unk* cb)
    : m00(-1), m04(flags), m08(model), m10(mtx), m14(nodes), m18(m18), m1C(color), m20(mats), m24(remap), m28(0),
      m30(*sphere), m40(m40), m44(0), m48(0), m4C(0), m50(0), m54(0), m58(0), m5C(0),
      m60(), m84(lens), m88(*cb), m98(m98) {
    m0C = (DrawModel_unk*)model->fn_80475DF8();
    Mtx& m = *m10;
    atn::Vector3 scale(atn::Vector3(m[0][0], m[1][0], m[2][0]).Length_unk(),
                       atn::Vector3(m[0][1], m[1][1], m[2][1]).Length_unk(),
                       atn::Vector3(m[0][2], m[1][2], m[2][2]).Length_unk());
    m2C = MaxF_unk(scale.x, MaxF_unk(scale.y, scale.z));
    if (m30.mRadius < 15.0f || (m04 & 4) == 4) {
        m28 = 1;
    }
    m60 = LightSet_unk::fn_8007A36C(lbl_8087EEF8, atn::Vector3(m30.mCenter), m30.mRadius, (int)m84);
}

void fn_8009E448(DrawEntry_unk* entry, DrawNode_unk* root) {
    int visible = 1;
    if ((((entry->m04 & 8) == 8 && lbl_8087EFA8->m104 != 0) || lbl_8087EFA8->m13C != 0) &&
        lbl_8087EFA8->m150 != 0) {
        if (fn_8004FF58(lbl_8087EFB4->m418, &entry->m30) == 0) {
            visible = 0;
        }
    }
    for (unsigned int i = 0; i < entry->m54; i++) {
        DrawNode_unk* node = entry->m0C->m48[entry->m50[i]];
        int index = entry->m40 != 0 ? entry->m40[node->m18] : node->m18;
        if (entry->m44[index] == 0) {
            continue;
        }
        if (entry->m48 != 0 && !(node->m10[0] == 'o' && node->m10[1] == 'p' && node->m10[2] == '_')) {
            continue;
        }
        for (int j = 0; j < node->m70; j++) {
            fn_8009D58C(entry, node, &node->m74[j], visible);
        }
    }
}
