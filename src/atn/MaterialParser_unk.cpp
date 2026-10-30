// atn/MaterialParser_unk.cpp: materials of the models (file name invented). A material is read
// from the text of a material file (fn_8007C46C, called by LoadMaterial of atn/LH_Material.cpp)
// or built from a shader (fn_800C4A90 of atn/Shader_unk.cpp); its texture stages are sorted by
// the id of their texture. Types in include/atn/Material_unk.h; all names are invented.
#include "atn/Material_unk.h"
#include "atn/Array_unk.h"
#include "atn/RenderSetting_unk.h"
#include "atn/Shader_unk.h"
#include "game/GimmickCommon.h"
#include "std/string.h"

extern "C" RenderSetting_unk* lbl_8087EFA8;

extern "C" char* strchr(const char* s, int c);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

extern "C" char* strcat(char* dst, const char* src);
extern "C" double fmod(double x, double y);
extern "C" double sin(double x);
extern "C" int fn_800D1C60(int mode); // inferred: converts a blend mode (atn/Shader_unk.cpp)
extern "C" int fn_800D1CD4(int mode); // inferred: converts a blend factor
extern "C" ShaderManager_unk* lbl_8087EFE0;

// inferred: debug parameter menu (as in atn/Shader_unk.cpp)
extern "C" void* fn_8008937C(void* parent, const char* name);
extern "C" void fn_800874C8(void* group, const char* name, void* value, int min, int max, int step,
                            void (*callback)(void*), void* arg);
extern "C" void fn_800875F8(void* group, const char* name, GXColor* value, int min, int max, int step,
                            void (*callback)(void*), void* arg);
extern "C" void fn_8008771C(void* group, const char* name, float* value, void (*callback)(void*), void* arg,
                            float min, float max, float step);

// inferred: debug message output (lbl_8087EEB8, as in atn/Shader_unk.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* message);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;
extern "C" int strncmp(const char* a, const char* b, unsigned long n);

extern "C" {

void GXInitTexObjWrapMode(GXTexObj* obj, int wrapS, int wrapT);
}

extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" std::string fn_8006AC08(const std::string& path); // inferred: extension of a path (atn/FileList)

// inferred: loads the texture of `path` (atn/Texture_unk; the second one keeps the image)
extern "C" void fn_800D5908(atn::Texture_unk* texture, const char* path);
extern "C" void fn_800D594C(atn::Texture_unk* texture, const char* path);

// inferred: keys of a material block
static const char* const sMaterialKeys[] = {
    "Name",     "Shader",   "MatColor",   "AmbColor",  "MatSrc",    "SpecSrc",   "AmbSrc",    "SpecColor",
    "SpecPower", "Fog",     "Culling",    "ShadowGen", "ShadowRe",  "PE",        "Lighting",  "Brightness",
    "RegColor1", "RegColor2", "RegColor3", "Animation", "Shadow",   "SortPriority", 0,
};

// inferred: values of MatSrc/SpecSrc/AmbSrc
static const char* const sColorSrcNames[] = {"VtxColor", "Material"};

// inferred: values of PE (draw modes of fn_8007FAF0)
static const char* sDrawModeNames[] = {
    "Custom",  "Opacity",  "OpacityNoZ", "Semi",     "SemiZPrePass", "SemiZ", "SemiPickOut", "SemiPickOutZPrePass",
    "PickOut", "Multiply", "Screen",     "Subtract", "Subtract2",    "Add",   0,
};

// inferred: keys of a texture stage block
static const char* const sStageKeys[] = {
    "\t\tname",     "\t\tscale",    "\t\trot",      "\t\ttrans",   "\t\twrapU",
    "\t\twrapV",    "\t\tmirrorU",  "\t\tmirrorV",  "\t\tindScale", "\t\tindRot",
    "\t\tintTrans", "\t\tindTrans", "\t\tuvset",    "\t\treflectivity", "\t\tchannel",
};

// inferred: texture names; the index is the id of a texture stage
static const char* sTexNames[] = {
    "TexColor1",       "TexColor2",         "TexColor3",         "TexAlpha1",     "TexAlpha2",
    "TexSpec1",        "TexSpecProj1",      "TexDiffuseProj1",   "TexEmboss1",    "TexEmboss2",
    "TexAmbientMap1",  "TexLightMap1",      "TexEnvironment1",   "TexLightSurround1", "TexLightSurround2",
    "TexWaterProj1",   "TexWaterProj2",     "TexWaterEmboss1",   "TexWaterEmboss2", "TexSlimeEdge1",
    "TexReflect1",     "TexFrameBuffer1",   "TexOutlineMask1",   0,
};

static const char* const sLineDelims = " \n\r";
static const char* const sTokenDelims = " ,=\n\r";

char String_unk::sEmpty_unk; // lbl_8087EF00 (.sbss)

const float TexUv_unk::sScaleU = 1.0f;
const float TexUv_unk::sScaleV = 1.0f;
const float TexUv_unk::sRot = 0.0f;
const float TexUv_unk::sTransU = 0.0f;
const float TexUv_unk::sTransV = 0.0f;

// inferred: deletes an object and clears the pointer (the destructor and the copies of the stages
// are written with it: small enough for MWCC to inline them, as in the original)
template <class T>
inline void SafeDelete_unk(T*& p) {
    if (p != 0) {
        delete p;
        p = 0;
    }
}

// inferred: same lookup written as a macro in the material parser (its own code keeps the inlining
// budget of the function, as in the original)
#define FIND_NAME(result, table, s, len)                                                                      \
    do {                                                                                                     \
        const char* const* p_ = table;                                                                       \
        const char* name_;                                                                                   \
        for (result = 0; (name_ = *p_) != 0; p_++, result++) {                                               \
            int n_ = strlen(name_);                                                                          \
            if (n_ == (len) && strncmp(s, name_, len) == 0) {                                                \
                break;                                                                                       \
            }                                                                                                \
        }                                                                                                    \
        if (name_ == 0) {                                                                                    \
            result = -1;                                                                                     \
        }                                                                                                    \
    } while (0)

// inferred: index of the string `s` (`len` characters) in a null-terminated table, -1 if missing
static inline int FindName_unk(const char* const* table, const char* s, int len) {
    const char* const* p = table;
    for (int i = 0; *p != 0; p++, i++) {
        const char* name = *p;
        int n = strlen(name);
        if (n == len && strncmp(s, name, len) == 0) {
            return i;
        }
    }
    return -1;
}

// inferred: skips the delimiters and returns the next token of `s` and its length (0 at the end).
// The original calls it with bl everywhere (noinline: CLAUDE.md rule 2)
// NONMATCHING: 98.1% - registers of s/delims/len (s in r27 instead of r30)
__declspec(noinline) const char* fn_8007A994(const char* s, const char* delims, int* len) {
    int n = 0;
    while (*s != 0 && strchr(delims, *s) != 0) {
        s++;
    }
    const char* p = s;
    while (*p != 0 && strchr(delims, *p) == 0) {
        p++;
        n++;
    }
    *len = n;
    if (n == 0) {
        return 0;
    }
    return s;
}

TexStage_unk::TexStage_unk(int id)
    : mTexture(0), mUv(0), mIndUv(0), mId(id), mOwnsTexture(0), mWrapS(0), mWrapT(0), mMinFilter(1),
      mMagFilter(1) {}

TexStage_unk::~TexStage_unk() {
    SafeDelete_unk(mUv);
    SafeDelete_unk(mIndUv);
    if (mOwnsTexture && mTexture != 0) {
        delete mTexture;
        mOwnsTexture = 0;
        mTexture = 0;
    }
}

// inferred: not in the .dol (the linker strips it); inlined in the copies of the stages
TexStage_unk& TexStage_unk::operator=(const TexStage_unk& o) {
    mMagFilter = o.mMagFilter;
    mMinFilter = o.mMinFilter;
    mName = o.mName;
    mOwnsTexture = o.mOwnsTexture;
    mTexture = o.mTexture;
    mId = o.mId;
    mWrapS = o.mWrapS;
    mWrapT = o.mWrapT;
    SafeDelete_unk(mUv);
    SafeDelete_unk(mIndUv);
    if (o.mUv != 0) {
        mUv = new ((MemoryTag)6, "", "", 0) TexUv_unk(*o.mUv);
    }
    if (o.mIndUv != 0) {
        mIndUv = new ((MemoryTag)6, "", "", 0) TexUv_unk(*o.mIndUv);
    }
    return *this;
}

// NONMATCHING: 91.7% - float registers and load order of the UV setters (the original keeps the next value in f27 before the store; tried direct Uv()-> stores, const& setters)
void TexStage_unk::fn_8007AB38(atn::ShaderDesc_unk* desc, int index) {
    const char* name = desc->mA4C[index];
    mId = FindName_unk(sTexNames, name, strlen(name));
    SetScaleU(desc->mTexMtxs[index]->mScaleU);
    SetScaleV(desc->mTexMtxs[index]->mScaleV);
    SetRot(desc->mTexMtxs[index]->mRot);
    SetTransU(desc->mTexMtxs[index]->mTransU);
    SetTransV(desc->mTexMtxs[index]->mTransV);
    for (int i = 0; i < desc->mIndStageCount; i++) {
        if (index == desc->mIndStages[i].mTexMap) {
            SetIndScaleU(desc->mIndUv[i][0]);
            SetIndScaleV(desc->mIndUv[i][1]);
            SetIndRot(desc->mIndUv[i][2]);
            SetIndTransU(desc->mIndUv[i][3]);
            SetIndTransV(desc->mIndUv[i][4]);
        }
    }
    mWrapS = desc->mTexInfos[index]->mWrapS;
    mWrapT = desc->mTexInfos[index]->mWrapT;
    mMinFilter = desc->mTexInfos[index]->mMinFilter;
    mMagFilter = desc->mTexInfos[index]->mMagFilter;
}

// inferred: id of the texture `name`, -1 if unknown
extern "C" int fn_8007B078(const char* name) {
    return FindName_unk(sTexNames, name, strlen(name));
}

// inferred: the texture is usable (as in atn/ModelDraw_unk.cpp)
static inline bool TexReady_unk(atn::Texture_unk* tex) {
    return tex->mImage != 0 || (((GimmickUvOwner_unk*)tex)->fn_800D59B8() == 0 && tex->m20.fn_80473EFC() == 3);
}

// NONMATCHING: 95.2% - obj and minFilter swap r30/r31 (tried declaration orders, a local texture pointer)
void TexStage_unk::fn_8007B100() {
    if (mTexture != 0 && TexReady_unk(mTexture)) {
        GXTexObj* obj = &mTexture->mObj;
        GXInitTexObjWrapMode(obj, mWrapS, mWrapT);
        int minFilter = mMinFilter;
        int magFilter = mMagFilter;
        int lods = (mTexture->mImage != 0 ? mTexture->m2F : mTexture->m20.fn_80478198()) - 1;
        if (lods > 0) {
            switch (minFilter) {
            case 0:
                minFilter = 4;
                break;
            case 1:
                minFilter = 5;
                break;
            }
        }
        int aniso = 0;
        unsigned char edge = 0;
        if (lods > 0) {
            aniso = lbl_8087EFA8->m224;
            if (aniso != 0) {
                edge = lbl_8087EFA8->m228;
            }
        }
        GXInitTexObjLOD(obj, minFilter, magFilter, 0.0f, lods, lbl_8087EFA8->m22C, 0, edge, aniso);
    }
}

// inferred: channel of a swap table ('R', 'G', 'B' or 'A')
static inline int SwapChannel_unk(char c) {
    switch (c) {
    case 'R':
    case 'r':
        return 0;
    case 'G':
    case 'g':
        return 1;
    case 'B':
    case 'b':
        return 2;
    case 'A':
    case 'a':
        return 3;
    }
    return 0;
}

// inferred: copies of the swap table go byte by byte (names invented)
static inline GXColor CopySwap_unk(GXColor c) {
    GXColor s;
    s.r = c.r;
    s.g = c.g;
    s.b = c.b;
    s.a = c.a;
    return s;
}
static inline void SetSwap_unk(GXColor& dst, GXColor c) {
    dst.r = c.r;
    dst.g = c.g;
    dst.b = c.b;
    dst.a = c.a;
}

// inferred: reads the texture stage block of a material ("\t\tkey value" lines)
// NONMATCHING: 99.5% - register allocation
const char* TexStage_unk::fn_8007B260(const char* path, Material_unk* mat, const char* p) {
    int n;
    int len = 0;
    bool wrapU = false;
    bool wrapV = false;
    bool mirrorU = false;
    bool mirrorV = false;
    p = fn_8007A994(p, sTokenDelims, &len);
    while (p[0] == '\t' && p[1] == '\t') {
        if (p[2] == 'M' && p[3] == 'B' && p[4] == '_') {
            p = fn_8007A994(p + len, sTokenDelims, &len);
        } else {
            n = len;
            switch (FindName_unk(sStageKeys, p, n)) {
            case 0:
                if (p[n + 1] == '\n' || p[n + 1] == '\r') {
                    mName = "";
                } else {
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    mName.assign(p, len);
                    std::string ext = fn_8006AC08(std::string(mName.c_str()));
                    if (ext.compare("") != 0) {
                        mName.assign(p, len - (ext.size() + 1));
                    }
                }
                break;
            case 1:
                SetScaleU(fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len)));
                SetScaleV(fn_800DC288(p = fn_8007A994(p + len, sTokenDelims, &len)));
                break;
            case 2:
                SetRot(fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len)));
                break;
            case 3:
                SetTransU(fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len)));
                SetTransV(fn_800DC288(p = fn_8007A994(p + len, sTokenDelims, &len)));
                break;
            case 4:
                wrapU = fn_800DC12C(p = fn_8007A994(p + n, sTokenDelims, &len)) == 1;
                break;
            case 5:
                wrapV = fn_800DC12C(p = fn_8007A994(p + n, sTokenDelims, &len)) == 1;
                break;
            case 6:
                mirrorU = fn_800DC12C(p = fn_8007A994(p + n, sTokenDelims, &len)) == 1;
                break;
            case 7:
                mirrorV = fn_800DC12C(p = fn_8007A994(p + n, sTokenDelims, &len)) == 1;
                break;
            case 8:
                SetIndScaleU(fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len)));
                SetIndScaleV(fn_800DC288(p = fn_8007A994(p + len, sTokenDelims, &len)));
                break;
            case 9:
                SetIndRot(fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len)));
                break;
            case 10:
            case 11:
                SetIndTransU(fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len)));
                SetIndTransV(fn_800DC288(p = fn_8007A994(p + len, sTokenDelims, &len)));
                break;
            case 12:
                p = fn_8007A994(p + n, sTokenDelims, &len);
                break;
            case 13:
                mat->m3C = 255.0f * fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len));
                break;
            case 14: {
                p = fn_8007A994(p + len, sTokenDelims, &len);
                GXColor swap = CopySwap_unk(mat->m38);
                if (len == 1) {
                    int c = SwapChannel_unk(p[0]);
                    if (mId == 3) {
                        swap.a = c;
                    } else {
                        swap.b = c;
                        swap.g = c;
                        swap.r = c;
                    }
                } else if (len == 3) {
                    swap.r = SwapChannel_unk(p[0]);
                    swap.g = SwapChannel_unk(p[1]);
                    swap.b = SwapChannel_unk(p[2]);
                } else if (len == 4) {
                    swap.r = SwapChannel_unk(p[0]);
                    swap.g = SwapChannel_unk(p[1]);
                    swap.b = SwapChannel_unk(p[2]);
                    swap.a = SwapChannel_unk(p[3]);
                }
                SetSwap_unk(mat->m38, swap);
                break;
            }
            }
        }
        p = fn_8007A994(p + len, sTokenDelims, &len);
    }
    mWrapS = 0;
    if (wrapU) {
        mWrapS = 1;
    } else if (mirrorU) {
        mWrapS = 2;
    }
    mWrapT = 0;
    if (wrapV) {
        mWrapT = 1;
    } else if (mirrorV) {
        mWrapT = 2;
    }
    return p;
}

void fn_8007C0D4(void* stage);

void TexStage_unk::fn_8007C018(const char* path, int keep) {
    if (mOwnsTexture && mTexture != 0) {
        delete mTexture;
        mOwnsTexture = 0;
        mTexture = 0;
    }
    mOwnsTexture = 1;
    mTexture = new ((MemoryTag)6, "", "", 0) atn::Texture_unk;
    if (keep != 0) {
        fn_800D594C(mTexture, path);
    } else {
        fn_800D5908(mTexture, path);
    }
}

// inferred: debug menu of a texture stage (static: inlined in fn_800816B4, its literals come first)
static void TexStageMenu_unk(void* parent, TexStage_unk* stage) {
    char buf[0x100];
    sprintf(buf, "%s", sTexNames[stage->mId]);
    void* g = fn_8008937C(parent, buf);
    fn_8008771C(g, "ScaleX", &stage->mUv->mScaleU, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "ScaleY", &stage->mUv->mScaleV, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "Rot", &stage->mUv->mRot, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "TranxX", &stage->mUv->mTransU, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "TransY", &stage->mUv->mTransV, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "IndScaleX", &stage->mIndUv->mScaleU, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "IndScaleY", &stage->mIndUv->mScaleV, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "IndRot", &stage->mIndUv->mRot, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "IndTranxX", &stage->mIndUv->mTransU, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_8008771C(g, "IndTransY", &stage->mIndUv->mTransV, fn_8007C0D4, stage, -1000.0f, 1000.0f, 0.01f);
    fn_800874C8(g, "WrapU", &stage->mWrapS, 0, 2, 1, fn_8007C0D4, stage);
    fn_800874C8(g, "WrapV", &stage->mWrapT, 0, 2, 1, fn_8007C0D4, stage);
}

// inferred: callback of the debug menu (applies the modes of a stage)
void fn_8007C0D4(void* stage) {
    ((TexStage_unk*)stage)->fn_8007B100();
}

TexAnim_unk::TexAnim_unk()
    : mStageId(-1), mStage(0), m08(0.0f), m0C(0.0f), m10(0.0f), mScrollU(0.0f), mScrollV(0.0f), m1C(0.0f),
      m20(0.0f), m24(0.0f), mIndScrollU(0.0f), mIndScrollV(0.0f), mWaveU(0.0f, 0.0f, 0.0f),
      mWaveV(0.0f, 0.0f, 0.0f), mWaveTimeU(0.0f), mWaveTimeV(0.0f), mInd(0), mWave(0) {}

// inferred: float colour converted to a GXColor (as in game/GimmickCommon.cpp)
struct ColorF_unk {
    ColorF_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    GXColor ToGX() const {
        GXColor c;
        c.r = (unsigned char)(255.0f * r);
        c.g = (unsigned char)(255.0f * g);
        c.b = (unsigned char)(255.0f * b);
        c.a = (unsigned char)(255.0f * a);
        return c;
    }
    float r, g, b, a;
};

// NONMATCHING: 87.5% - store order: the original sets the swap table m38 in the member initialisation (a type with a constructor gives 90.6% here but lowers fn_8007B260)
Material_unk::Material_unk(ShaderTex_unk* shader)
    : m00(0), m04(0), m08(shader), m0C(0), m10(0), mColor(ColorF_unk(1.0f, 1.0f, 1.0f, 1.0f).ToGX()),
      m1C(ColorF_unk(1.0f, 1.0f, 1.0f, 1.0f).ToGX()), m20(ColorF_unk(1.0f, 1.0f, 1.0f, 1.0f).ToGX()), m30(0.0f),
      m34(0), m3C(0), m40(0), m44(0), m48(0), m49(0), m4A(0), m4B(0), m4C(-1), m4D(0), m4E(0), m4F(0), m50(0),
      m51(0) {
    m38.r = 0;
    m38.g = 1;
    m38.b = 2;
    m38.a = 3;
    mTevColor[0].r = mColor.r;
    mTevColor[0].g = mColor.g;
    mTevColor[0].b = mColor.b;
    mTevColor[0].a = 0;
    mTevColor[1].r = mColor.r;
    mTevColor[1].g = mColor.g;
    mTevColor[1].b = mColor.b;
    mTevColor[1].a = 0;
    mTevColor[2].r = mColor.r;
    mTevColor[2].g = mColor.g;
    mTevColor[2].b = mColor.b;
    mTevColor[2].a = 0;
    m14_0 = 0;
    m14_1 = 7;
    m14_4 = 0;
    m14_12 = 1;
    m14_13 = 4;
    m14_16 = 5;
    m14_19 = 1;
    m14_20 = 0;
    m14_21 = 1;
    m14_22 = 2;
    m14_25 = 1;
    m14_26 = 1;
    m14_27 = 1;
    fn_80081D00();
}

Material_unk::~Material_unk() {
    delete[] m0C;
    delete[] m10;
}

// inferred: texture stage of id `id` (linear search)
inline TexStage_unk* Material_unk::FindStage_unk(int id) {
    for (int i = 0; i < m4A; i++) {
        if (m0C[i].mId == id) {
            return &m0C[i];
        }
    }
    return 0;
}

// inferred: order of the texture stages (by id)
struct TexStageLess_unk {
    bool operator()(const TexStage_unk& a, const TexStage_unk& b) const { return a.mId < b.mId; }
};

// NONMATCHING: 99.4% - register allocation (0x8007ED30)
template <class T>
void Swap_unk(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

// inferred: median of three of the quicksort below (MSL __sort; see atn/Shader_unk.cpp)
template <class T, class Compare>
void Median3_unk(T* a, T* b, T* c, Compare& comp) {
    bool ca = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ca && bc) {
        return;
    }
    if (!ca && !bc) {
        Swap_unk(*a, *b);
    } else {
        if (comp(*b, *a)) {
            Swap_unk(*a, *b);
        }
        if (ca) {
            Swap_unk(*b, *c);
        } else {
            Swap_unk(*a, *c);
        }
    }
}

// NONMATCHING: 98.2% - register allocation (0x8007F464)
template <class T, class Compare>
void SelectionSort_unk(T* first, T* last, Compare& comp) {
    if (first != last) {
        for (T* end = last - 1; first != end; first++) {
            T* min = first;
            for (T* it = first + 1; it != last; it++) {
                if (comp(*it, *min)) {
                    min = it;
                }
            }
            if (min != first) {
                Swap_unk(*min, *first);
            }
        }
    }
}

static int sSortOffset = -4; // inferred: pivot offset of the quicksort, -4 to 4

// NONMATCHING: 95.9% - register allocation of the partition loops (0x8007EA24)
template <class T, class Compare>
void Sort_unk(T* first, T* last, Compare& comp) {
    while (true) {
        int n = last - first;
        if (n <= 1) {
            return;
        }
        if (n <= 20) {
            SelectionSort_unk(first, last, comp);
            return;
        }
        T* a = first + (n / 4 + sSortOffset % 5);
        if (++sSortOffset >= 5) {
            sSortOffset = -4;
        }
        T* b = first + (n * 3 / 4 + sSortOffset % 5);
        if (++sSortOffset >= 5) {
            sSortOffset = -4;
        }
        T* j;
        T* pivot = last - 1;
        Median3_unk(a, b, pivot, comp);
        T* i = first;
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
            Sort_unk(first, i, comp);
            first = i;
        } else {
            Sort_unk(i, last, comp);
            last = i;
        }
    }
}

// inferred: reads one material block of a material file and returns the end of what it read
// NONMATCHING: 97.0% - FIND_NAME leaves a cmpwi/bne after each loop (the original has the shape of a lookup with return, but the inline function puts the parser over the inlining budget) and register allocation
const char* Material_unk::fn_8007C46C(const char* path, const char* data, int size) {
    int len = 0;
    atn::SimpleVector<TexStage_unk> stages;
    atn::SimpleVector<TexAnim_unk> anims;
    char name[0x100];
    char shader[0x100];
    const char* p = fn_8007A994(data, sLineDelims, &len);
    p = fn_8007A994(p + len, sTokenDelims, &len);
    int n;
    while (p != 0 && p - data < size && p[0] == '\t') {
        if (p[1] == 'M' && p[2] == 'B' && p[3] == '_') {
            while (p[len] != '\n') {
                len++;
            }
            while (p[len] == '\n' || p[len] == '\r') {
                len++;
            }
            p += len;
            len = 0;
        } else {
            n = len;
            int key;
            FIND_NAME(key, sMaterialKeys, p + 1, n - 1);
            if (key == -1) {
                int tex;
                FIND_NAME(tex, sTexNames, p + 1, n - 1);
                TexStage_unk stage(tex);
                p = stage.fn_8007B260(path, this, p + len);
                len = 0;
                stages.push_back(stage);
            } else {
                switch (key) {
                case 0:
                    if (p[n + 1] == '\n' || p[n + 1] == '\r') {
                        strcpy(name, "");
                    } else {
                        p = fn_8007A994(p + n, sTokenDelims, &len);
                        memcpy(name, p, len);
                        name[len] = 0;
                        m00 = fn_800DC6B4(name);
                    }
                    break;
                case 1:
                    if (p[n + 1] == '\n' || p[n + 1] == '\r') {
                        strcpy(shader, "");
                    } else {
                        p = fn_8007A994(p + n, sTokenDelims, &len);
                        memcpy(shader, p, len);
                        shader[len] = 0;
                        m04 = fn_800DC6B4(shader);
                    }
                    break;
                case 4: {
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    int src;
                    FIND_NAME(src, sColorSrcNames, p, len);
                    if (src == 0) {
                        m4E = 1;
                    } else if (src == 1) {
                        m4E = 0;
                    }
                    break;
                }
                case 5: {
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    int src;
                    FIND_NAME(src, sColorSrcNames, p, len);
                    if (src == 0) {
                        m4F = 1;
                    } else if (src == 1) {
                        m4F = 0;
                    }
                    break;
                }
                case 6: {
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    int src;
                    FIND_NAME(src, sColorSrcNames, p, len);
                    if (src == 0) {
                        m50 = 1;
                        m51 = 1;
                    } else if (src == 1) {
                        m50 = 0;
                        m51 = 0;
                    }
                    break;
                }
                case 2: {
                    const char* delims = sTokenDelims;
                    mColor.r = 255.0f * fn_800DC288(p = fn_8007A994(p + n, delims, &len));
                    mColor.g = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    mColor.b = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    mColor.a = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    break;
                }
                case 3: {
                    const char* delims = sTokenDelims;
                    m1C.r = 255.0f * fn_800DC288(p = fn_8007A994(p + n, delims, &len));
                    m1C.g = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    m1C.b = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    break;
                }
                case 7: {
                    const char* delims = sTokenDelims;
                    m20.r = 255.0f * fn_800DC288(p = fn_8007A994(p + n, delims, &len));
                    m20.g = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    m20.b = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    break;
                }
                case 16:
                case 17:
                case 18: {
                    const char* delims = sTokenDelims;
                    int reg = key - 16;
                    mTevColor[reg].r = 255.0f * fn_800DC288(p = fn_8007A994(p + n, delims, &len));
                    mTevColor[reg].g = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    mTevColor[reg].b = 255.0f * fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    break;
                }
                case 19: {
                    const char* tex = fn_8007A994(p + n, sTokenDelims, &len);
                    TexAnim_unk anim;
                    int stageId;
                    FIND_NAME(stageId, sTexNames, tex, len);
                    anim.mStageId = stageId;
                    const char* delims = sTokenDelims;
                    anim.m08 = fn_800DC288(p = fn_8007A994(tex + len, delims, &len));
                    anim.m0C = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.m10 = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.mScrollU = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.mScrollV = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.m1C = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.m20 = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.m24 = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.mIndScrollU = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.mIndScrollV = fn_800DC288(p = fn_8007A994(p + len, delims, &len));
                    anim.mInd = anim.m1C != 0.0f || anim.m20 != 0.0f || anim.m24 != 0.0f ||
                                anim.mIndScrollU != 0.0f || anim.mIndScrollV != 0.0f;
                    if (p[len] == ',') {
                        const char* delims2 = sTokenDelims;
                        anim.mWaveU.x = fn_800DC288(p = fn_8007A994(p + len, delims2, &len));
                        anim.mWaveU.y = fn_800DC288(p = fn_8007A994(p + len, delims2, &len));
                        anim.mWaveU.z = fn_800DC288(p = fn_8007A994(p + len, delims2, &len));
                        anim.mWaveV.x = fn_800DC288(p = fn_8007A994(p + len, delims2, &len));
                        anim.mWaveV.y = fn_800DC288(p = fn_8007A994(p + len, delims2, &len));
                        anim.mWaveV.z = fn_800DC288(p = fn_8007A994(p + len, delims2, &len));
                        anim.mWave = anim.mWaveU.x != 0.0f || anim.mWaveU.y != 0.0f || anim.mWaveU.z != 0.0f ||
                                     anim.mWaveV.x != 0.0f || anim.mWaveV.y != 0.0f || anim.mWaveV.z != 0.0f;
                    }
                    anims.push_back(anim);
                    break;
                }
                case 8:
                    m30 = fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len));
                    break;
                case 15: {
                    float brightness = fn_800DC288(p = fn_8007A994(p + n, sTokenDelims, &len));
                    m34 = 0;
                    if (brightness < 1.0f) {
                        m34 = 3;
                    } else if (brightness > 1.0f) {
                        if (brightness < 4.0f) {
                            m34 = 1;
                        } else {
                            m34 = 2;
                        }
                    }
                    break;
                }
                case 9:
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    m14_20 = std::string("ON") == std::string(p, len);
                    break;
                case 10:
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    m14_19 = (std::string("front") == std::string(p, len)) != 0;
                    break;
                case 11:
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    break;
                case 12:
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    break;
                case 13: {
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    int mode;
                    FIND_NAME(mode, sDrawModeNames, p, len);
                    if (mode == 4 || mode == 7) {
                        m48 |= 2;
                    }
                    if (mode == -1) {
                        char text[0x100];
                        char buf[0x100];
                        mode = 1;
                        memcpy(text, p, len);
                        sprintf(buf, "Unknown PE mode '%s' in '%s'\n", text, path);
                        buf[0xFF] = 0;
                        lbl_8087EEB8->fn_800697D8(buf);
                    }
                    m4D = mode;
                    fn_8007FAF0(m4D);
                    break;
                }
                case 14: {
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    std::string s(p, len);
                    if (s.compare("OFF") == 0) {
                        m4C = 0;
                    } else {
                        m4C = s.compare("ON") != 0 ? 2 : 3;
                    }
                    break;
                }
                case 20:
                    p = fn_8007A994(p + n, sTokenDelims, &len);
                    if (std::string("Project") == std::string(p, len)) {
                        m48 |= 1;
                    }
                    break;
                case 21:
                    m44 = fn_800DC12C(p = fn_8007A994(p + n, sTokenDelims, &len));
                    break;
                }
            }
        }
        p = fn_8007A994(p + len, sTokenDelims, &len);
    }
    TexStageLess_unk comp;
    Sort_unk(stages.mData, stages.mData + stages.mSize, comp);
    m4A = stages.mSize;
    m0C = m4A != 0 ? new ((MemoryTag)6, "", "", 0) TexStage_unk[m4A] : 0;
    for (int i = 0; i < m4A; i++) {
        m0C[i] = stages[i];
    }
    m4B = anims.mSize;
    m10 = m4B != 0 ? new ((MemoryTag)6, "", "", 0) TexAnim_unk[m4B] : 0;
    for (int i = 0; i < m4B; i++) {
        m10[i] = anims[i];
        m10[i].mStage = FindStage_unk(m10[i].mStageId);
    }
    fn_80081080(shader);
    fn_80081D00();
    return p;
}

void Material_unk::fn_8007FAF0(int mode) {
    switch (mode) {
    case 1:
        m14_0 = 0;
        m14_12 = 0;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 1;
        break;
    case 2:
        m14_0 = 0;
        m14_12 = 0;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    case 3:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 4;
        m14_16 = 5;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    case 4:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 4;
        m14_16 = 5;
        m14_21 = 1;
        m14_22 = 3;
        m14_25 = 0;
        break;
    case 5:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 4;
        m14_16 = 5;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 1;
        break;
    case 6:
        m14_0 = 1;
        m14_4 = 0;
        m14_1 = 4;
        m14_12 = 1;
        m14_13 = 4;
        m14_16 = 5;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    case 7:
        m14_0 = 1;
        m14_4 = 0;
        m14_1 = 4;
        m14_12 = 1;
        m14_13 = 4;
        m14_16 = 5;
        m14_21 = 1;
        m14_22 = 3;
        m14_25 = 0;
        break;
    case 8:
        m14_12 = 0;
        m14_0 = 1;
        m14_4 = 0x80;
        m14_1 = 4;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 1;
        break;
    case 13:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 4;
        m14_16 = 1;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    case 9:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 0;
        m14_16 = 2;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    case 10:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 9;
        m14_16 = 1;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    case 11:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 0;
        m14_16 = 5;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    case 12:
        m14_0 = 0;
        m14_12 = 1;
        m14_13 = 0;
        m14_16 = 3;
        m14_21 = 1;
        m14_22 = 2;
        m14_25 = 0;
        break;
    }
    m4D = mode;
    fn_80081D00();
}

// inferred: blend modes from a shader description
// NONMATCHING: 91.8% - the original writes the three bit fields with one store (no alias assumed with desc) and keeps desc/this in r29/r31
void Material_unk::fn_8007FE5C(atn::ShaderDesc_unk* desc) {
    int blend = fn_800D1C60(desc->mA2C);
    m14_21 = desc->mA28;
    m14_22 = blend;
    m14_25 = desc->mA30;
    int src = fn_800D1CD4(desc->mA3C);
    int dst = fn_800D1CD4(desc->mA40);
    if (desc->m074 != 0 || desc->mA38 == 1) {
        m14_12 = 1;
        m14_13 = src;
        m14_16 = dst;
    } else {
        m14_12 = 0;
    }
    int op = 7;
    switch (desc->mA14) {
    case 0:
        op = 0;
        break;
    case 1:
        op = 1;
        break;
    case 2:
        op = 3;
        break;
    case 3:
        op = 2;
        break;
    case 4:
        op = 4;
        break;
    case 5:
        op = 6;
        break;
    case 6:
        op = 5;
        break;
    case 7:
        op = 7;
        break;
    }
    m14_0 = 1;
    m14_4 = desc->mA18;
    m14_1 = op;
    fn_80081D00();
}

// inferred: scrolls the UV transforms of the animated stages
// NONMATCHING: 90.0% - allocation of the float constants hoisted out of the loop
void Material_unk::fn_80080048(float time) {
    for (int i = 0; i < m4B; i++) {
        TexAnim_unk* anim = &m10[i];
        TexStage_unk* stage = anim->mStage;
        if (stage == 0) {
            continue;
        }
        float u = anim->mScrollU * lbl_8087EFA8->m3A4 + stage->GetTransU();
        float scale = stage->GetScaleU();
        if (anim->mWave) {
            anim->mWaveTimeU = fmod(anim->mWaveTimeU + time, 6.2831855f / anim->mWaveU.y);
            u += anim->mWaveU.x * (float)sin(anim->mWaveU.y * anim->mWaveTimeU + anim->mWaveU.z);
        }
        if (u < 0.0f) {
            u += scale;
        }
        if (u > scale) {
            u -= scale;
        }
        stage->SetTransU(u);
        float v = anim->mScrollV * lbl_8087EFA8->m3A4 + stage->GetTransV();
        scale = stage->GetScaleV();
        if (anim->mWave) {
            anim->mWaveTimeV = fmod(anim->mWaveTimeV + time, 6.2831855f / anim->mWaveV.y);
            v += anim->mWaveV.x * (float)sin(anim->mWaveV.y * anim->mWaveTimeV + anim->mWaveV.z);
        }
        if (v < 0.0f) {
            v += scale;
        }
        if (v > scale) {
            v -= scale;
        }
        stage->SetTransV(v);
        if (anim->mInd) {
            float iu = anim->mIndScrollU * lbl_8087EFA8->m3A4 + stage->GetIndTransU();
            if (iu < 0.0f) {
                iu += stage->GetIndScaleU();
            }
            if (iu > stage->GetIndScaleU()) {
                iu -= stage->GetIndScaleU();
            }
            stage->SetIndTransU(iu);
            float iv = anim->mIndScrollV * lbl_8087EFA8->m3A4 + stage->GetIndTransV();
            if (iv < 0.0f) {
                iv += stage->GetIndScaleV();
            }
            if (iv > stage->GetIndScaleV()) {
                iv -= stage->GetIndScaleV();
            }
            stage->SetIndTransV(iv);
        }
    }
}

int Material_unk::fn_800804D0(int id) {
    for (int i = 0; i < m4A; i++) {
        int stageId = m0C[i].mId;
        if (stageId == id) {
            return i;
        }
        if (stageId > id) {
            return -1;
        }
    }
    return -1;
}

// NONMATCHING: 98.5% - r6/r7 swapped (stage base and offset)
TexStage_unk* Material_unk::fn_80080528(int id) {
    for (int i = 0; i < m4A; i++) {
        int stageId = m0C[i].mId;
        if (stageId == id) {
            return &m0C[i];
        }
        if (stageId > id) {
            return 0;
        }
    }
    return 0;
}

// inferred: copy of another material (the stages get their own UV transforms)
// NONMATCHING: 94.8% - order of the byte copies and register allocation
void Material_unk::fn_80080584(const Material_unk& o) {
    mTevColor[0].r = o.mTevColor[0].r;
    mTevColor[0].b = o.mTevColor[0].b;
    mTevColor[0].a = o.mTevColor[0].a;
    mTevColor[1].r = o.mTevColor[1].r;
    mTevColor[1].g = o.mTevColor[1].g;
    mTevColor[1].b = o.mTevColor[1].b;
    mTevColor[1].a = o.mTevColor[1].a;
    mTevColor[2].r = o.mTevColor[2].r;
    mTevColor[2].g = o.mTevColor[2].g;
    mTevColor[2].b = o.mTevColor[2].b;
    mTevColor[2].a = o.mTevColor[2].a;
    m4D = o.m4D;
    m34 = o.m34;
    m00 = o.m00;
    m4E = o.m4E;
    m4F = o.m4F;
    m50 = o.m50;
    m51 = o.m51;
    mColor.r = o.mColor.r;
    mColor.g = o.mColor.g;
    mColor.b = o.mColor.b;
    mColor.a = o.mColor.a;
    m1C.r = o.m1C.r;
    m1C.g = o.m1C.g;
    m1C.b = o.m1C.b;
    m1C.a = o.m1C.a;
    m20.r = o.m20.r;
    m20.g = o.m20.g;
    m20.b = o.m20.b;
    m20.a = o.m20.a;
    mTevColor[0].g = o.mTevColor[0].g;
    m30 = o.m30;
    m4C = o.m4C;
    m4A = o.m4A;
    m0C = new ((MemoryTag)6, "", "", 0) TexStage_unk[m4A];
    for (int i = 0; i < m4A; i++) {
        TexStage_unk& dst = m0C[i];
        const TexStage_unk& src = o.m0C[i];
        dst.mName = src.mName;
        dst.mId = src.mId;
        SafeDelete_unk(dst.mUv);
        SafeDelete_unk(dst.mIndUv);
        if (src.mUv != 0) {
            dst.mUv = new ((MemoryTag)6, "", "", 0) TexUv_unk(*src.mUv);
        }
        if (src.mIndUv != 0) {
            dst.mIndUv = new ((MemoryTag)6, "", "", 0) TexUv_unk(*src.mIndUv);
        }
        dst.mWrapS = src.mWrapS;
        dst.mWrapT = src.mWrapT;
        dst.mMinFilter = src.mMinFilter;
        dst.mMagFilter = src.mMagFilter;
    }
    m4B = o.m4B;
    m10 = m4B != 0 ? new ((MemoryTag)6, "", "", 0) TexAnim_unk[m4B] : 0;
    for (int i = 0; i < m4B; i++) {
        m10[i] = o.m10[i];
        m10[i].mStage = FindStage_unk(m10[i].mStageId);
    }
    m3C = o.m3C;
    m38.r = o.m38.r;
    m38.g = o.m38.g;
    m38.b = o.m38.b;
    m38.a = o.m38.a;
    *(unsigned int*)&((unsigned char*)this)[0x14] = *(const unsigned int*)&((const unsigned char*)&o)[0x14];
    m48 = o.m48;
    m49 = o.m49;
    m44 = o.m44;
    if (m4D == 0) {
        fn_8007FE5C(m08->mDesc);
    }
}

// inferred: names of the stages from a shader description
// NONMATCHING: 98.3% - registers of the inlined String_unk assignment
void Material_unk::fn_80080B34(atn::ShaderDesc_unk* desc) {
    for (int i = 0; i < m4A; i++) {
        m0C[i].mName = desc->mA4C[i];
    }
}

// inferred: textures of the stages: from `getter`, or loaded from `dir`
// NONMATCHING: 96.6% - register allocation
void Material_unk::fn_80080C20(const std::string& dir, TexGetter_unk& getter) {
    for (int i = 0; i < m4A; i++) {
        std::string name(m0C[i].mName.c_str());
        atn::Texture_unk* tex = getter.empty() ? 0 : getter(name);
        if (tex != 0) {
            TexStage_unk& stage = m0C[i];
            if (stage.mOwnsTexture && stage.mTexture != 0) {
                delete stage.mTexture;
                stage.mOwnsTexture = 0;
                stage.mTexture = 0;
            }
            stage.mTexture = tex;
        } else if (name.compare("") != 0) {
            char path[0x100];
            if (name.compare("bake_tex") == 0) {
                sprintf(path, "data/texture/sys_gray.texture");
            } else {
                sprintf(path, "%s/textures/%s.texture", dir.c_str(), name.c_str());
            }
            m0C[i].fn_8007C018(path, 0);
        }
    }
}

// inferred: name of the shader from the stages of the material (suffix per texture)
// NONMATCHING: 99.8% - register allocation
void Material_unk::fn_80081080(char* shader) {
    if (FindStage_unk(5) != 0) {
        strcat(shader, "_Spec");
    }
    if (FindStage_unk(1) != 0) {
        strcat(shader, "_MT");
    }
    if (FindStage_unk(10) != 0) {
        strcat(shader, "_AMB");
    }
    if (FindStage_unk(3) != 0) {
        strcat(shader, "_Alpha");
    }
    if (FindStage_unk(11) != 0) {
        strcat(shader, "_LM");
    }
    if (FindStage_unk(12) != 0) {
        strcat(shader, "_RF");
    }
    if (FindStage_unk(13) != 0) {
        strcat(shader, "_LS");
    }
    if (FindStage_unk(20) != 0) {
        strcat(shader, "_RFX");
    }
    if (FindStage_unk(21) != 0) {
        strcat(shader, "_FB");
    }
    if (lbl_8087EFE0->fn_800C5F5C(shader) == 0) {
        char buf[0x100];
        sprintf(buf, "WARNING Shader was not found '%s'\n", shader);
        buf[0xFF] = 0;
        lbl_8087EEB8->fn_800697D8(buf);
    }
    m04 = fn_800DC6B4(shader);
    if (lbl_8087EFA8->m050 != 0) {
        if (FindStage_unk(0) != 0) {
            m04 = fn_800DC6B4("default");
        } else {
            m04 = fn_800DC6B4("color");
        }
    }
}

// inferred: translation of all the UV transforms
void Material_unk::fn_80081514(float u, float v) {
    for (int i = 0; i < m4A; i++) {
        m0C[i].SetTransU(u);
        m0C[i].SetTransV(v);
    }
}

int Material_unk::fn_800816A8() {
    return m14_12;
}

void fn_80081BCC(void* arg);

// inferred: debug menu of the material
// NONMATCHING: 99.0% - register allocation
void Material_unk::fn_800816B4(void* parent, const char* name) {
    char title[0x20];
    if (name != 0) {
        sprintf(title, "%s", name);
    } else {
        sprintf(title, "%s:%08x", m08->mDesc->m010, m00);
    }
    title[0x1F] = 0;
    void* group = fn_8008937C(parent, title);
    fn_800875F8(group, "Mat Color", &mColor, 0, -1, 1, fn_80081BCC, this);
    fn_800875F8(group, "Spec Color", &m20, 0, -1, 1, fn_80081BCC, this);
    fn_800875F8(group, "Amb Color", &m1C, 0, -1, 1, fn_80081BCC, this);
    fn_800875F8(group, "Reg0 Color", &mTevColor[0], 0, -1, 1, fn_80081BCC, this);
    fn_800875F8(group, "Reg1 Color", &mTevColor[1], 0, -1, 1, fn_80081BCC, this);
    fn_800875F8(group, "Reg2 Color", &mTevColor[2], 0, -1, 1, fn_80081BCC, this);
    fn_800874C8(group, "Brightness", &m34, 0, 3, 1, fn_80081BCC, this);
    fn_800874C8(group, "Reflectivity", &m3C, 0, 0xFF, 1, fn_80081BCC, this);
    for (int i = 0; i < m4A; i++) {
        TexStageMenu_unk(group, &m0C[i]);
    }
    if (m4B != 0) {
        void* anims = fn_8008937C(group, "Animations");
        for (int i = 0; i < m4B; i++) {
            char buf[0x100];
            sprintf(buf, "%s", sTexNames[m10[i].mStageId]);
            void* g = fn_8008937C(anims, buf);
            fn_8008771C(g, "TransX", &m10[i].mScrollU, 0, 0, -100.0f, 100.0f, 0.01f);
            fn_8008771C(g, "TransY", &m10[i].mScrollV, 0, 0, -100.0f, 100.0f, 0.01f);
            fn_8008771C(g, "IndTransX", &m10[i].mIndScrollU, 0, 0, -100.0f, 100.0f, 0.01f);
            fn_8008771C(g, "IndTransY", &m10[i].mIndScrollV, 0, 0, -100.0f, 100.0f, 0.01f);
        }
    }
}

// inferred: callback of the debug menu (nothing to update)
void fn_80081BCC(void* arg) {}

// inferred: applies the modes of all the stages
void Material_unk::fn_80081BD0() {
    for (int i = 0; i < m4A; i++) {
        m0C[i].fn_8007B100();
    }
}

// inferred: path of the default texture of an id (0 if none)
extern "C" const char* fn_80081C34(int id) {
    switch (id) {
    case 6:
        return "data/texture/specular.texture";
    case 7:
        return "data/texture/diffuse.texture";
    case 10:
        return "data/texture/sys_white.texture";
    case 12:
        return "data/texture/sys_white.texture";
    case 13:
        return "data/texture/lightsurround.texture";
    case 14:
        return "data/texture/lightsurround2.texture";
    case 15:
    case 16:
        return "data/texture/waterproj.texture";
    case 8:
    case 9:
    case 17:
    case 18:
        return "data/texture/wateremboss.texture";
    case 19:
        return "data/texture/slime_edge.texture";
    case 20:
    case 21:
    case 22:
        return "data/texture/sys_white.texture";
    }
    return 0;
}

void Material_unk::fn_80081D00() {
    int mask = 0;
    if (m14_12) {
        mask |= 2;
    } else {
        mask |= 1;
    }
    if (FindStage_unk(0x15) != 0) {
        mask |= 4;
    }
    if (FindStage_unk(0x14) != 0) {
        mask |= 8;
    }
    m49 = mask;
}
