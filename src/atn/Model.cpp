// atn::Model / atn::ModelBase (0x800899DC-0x8009A5CC). Written in the order of the .dol; the
// functions not written yet are missing from this file (the unit is not linked).
#include "atn/Model_unk.h"
#include "atn/Material_unk.h"
#include "atn/LH_Motion.h"
#include "atn/ModelCamera_unk.h"
#include "atn/Csv.h"
#include "atn/Shader_unk.h"
#include "atn/MemoryManager.h"
#include "std/string.h"
#include <boost/bind.hpp>

extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: atof
extern "C" int fn_800DC12C(const char* s);   // inferred: atoi
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists
extern "C" std::string& fn_8006AD24(std::string& path, const std::string& ext); // inferred: changes the extension
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
std::string fn_8006B0C8(const std::string& path); // inferred: directory of a path
extern "C" void* lbl_8087EFB4; // inferred: render system
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int memcmp(const void* a, const void* b, unsigned long n);
extern "C" void PSMTXInverse(const Mtx src, Mtx inv);
extern "C" void PSMTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, unsigned int count);
// inferred: collects the warnings of the loaders (see game/main.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* msg);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;
extern "C" atn::Matrix3x4* fn_80476130(atn::LH_Model* model); // inferred: inverse joint matrices

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

namespace atn {

// inferred: motion command entry of a motion slot (16 bytes, atn::Model::m358); names invented
struct MotionCmd_unk {
    unsigned char m00[0x10];
};
} // namespace atn
extern "C" void fn_800A1060(atn::MotionCmd_unk* cmd);                                 // inferred: clears
extern "C" void fn_800A0A68(atn::MotionCmd_unk* cmd, atn::Model* model, const atn::LH_Motion* src); // inferred: copies
extern "C" float fn_800A08D4(atn::LH_Motion* motion); // inferred: length of the motion

namespace atn {

extern "C" void fn_800D58A4(Texture_unk* tex); // inferred: releases a texture
} // namespace atn

// inferred: owner of a loading texture (method of atn::Texture_unk named after its first user; see
// game/LibraryTask.cpp)
struct GimmickUvOwner_unk {
    int fn_800D59B8(); // inferred: nonzero while loading
};

MaterialSet_unk::~MaterialSet_unk() {
    for (unsigned int i = 0; i < mMaterials.size(); i++) {
        delete mMaterials.mData[i];
    }
}

// inferred: releases the textures of every material (CharaManager::vf14 calls it on atn::Model::m104)
void MaterialSet_unk::fn_80089AD4() {
    for (unsigned int i = 0; i < mMaterials.mSize; i++) {
        for (int j = 0; j < mMaterials.mData[i]->m4A; j++) {
            if (mMaterials.mData[i]->m0C[j].mTexture != 0) {
                atn::fn_800D58A4(mMaterials.mData[i]->m0C[j].mTexture);
            }
        }
    }
}

// inferred: the texture has an image or finished loading
inline int IsTextureReady_unk(atn::Texture_unk* tex) {
    int ready = 0;
    if (tex->mImage != 0 || (((GimmickUvOwner_unk*)tex)->fn_800D59B8() == 0 && tex->m20.fn_80473EFC() == 3)) {
        ready = 1;
    }
    return ready;
}

// inferred: a texture of the materials is ready
int MaterialSet_unk::fn_80089B5C() {
    for (unsigned int i = 0; i < mMaterials.mSize; i++) {
        for (int j = 0; j < mMaterials.mData[i]->m4A; j++) {
            atn::Texture_unk* tex = mMaterials.mData[i]->m0C[j].mTexture;
            if (tex != 0) {
                if (IsTextureReady_unk(tex)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

// inferred: fills a node edit (flags: which parts are not identity); called by atn::Model::fn_80093F1C
// NONMATCHING: 99.7% - the first fabs goes to f0 instead of f3
extern "C" void fn_80089D70(atn::Model::NodeEdit_unk* edit, const char* name, const atn::Vector3& offset,
                            const atn::Vector3& rot, const atn::Vector3& scale, int flags) {
    edit->m00 = fn_800DC6B4(name);
    edit->m08 = offset;
    edit->m14 = rot;
    edit->m20 = scale;
    edit->m04 = flags;
    if (!(atn::Abs_unk(offset.x) < 1.1920929e-7f && atn::Abs_unk(offset.y) < 1.1920929e-7f &&
          atn::Abs_unk(offset.z) < 1.1920929e-7f)) {
        edit->m04 |= 1;
    }
    if (!(atn::Abs_unk(rot.x) < 1.1920929e-7f && atn::Abs_unk(rot.y) < 1.1920929e-7f &&
          atn::Abs_unk(rot.z) < 1.1920929e-7f)) {
        edit->m04 |= 2;
    }
    if (!(atn::Abs_unk(scale.x - 1.0f) < 1.1920929e-7f && atn::Abs_unk(scale.y - 1.0f) < 1.1920929e-7f &&
          atn::Abs_unk(scale.z - 1.0f) < 1.1920929e-7f)) {
        edit->m04 |= 4;
    }
}

// inferred: reads a node edit from a "NODE_EDIT" line: node name, offset, rotation, scale and flags
extern "C" __declspec(noinline) void fn_80089EE4(atn::Model::NodeEdit_unk* edit, atn::CsvTab* csv) {
    char name[0x100];
    strcpy(name, csv->fn_8005B9CC());
    float x = fn_800DC288(csv->fn_8005B9CC());
    float y = fn_800DC288(csv->fn_8005B9CC());
    float z = fn_800DC288(csv->fn_8005B9CC());
    atn::Vector3 offset(x, y, z);
    x = fn_800DC288(csv->fn_8005B9CC());
    y = fn_800DC288(csv->fn_8005B9CC());
    z = fn_800DC288(csv->fn_8005B9CC());
    atn::Vector3 rot(x, y, z);
    x = fn_800DC288(csv->fn_8005B9CC());
    y = fn_800DC288(csv->fn_8005B9CC());
    z = fn_800DC288(csv->fn_8005B9CC());
    atn::Vector3 scale(x, y, z);
    fn_80089D70(edit, name, offset, rot, scale, fn_800DC12C(csv->fn_8005B9CC()));
}

// inferred: appends the node edits ("NODE_EDIT" lines) of a text file
extern "C" void fn_8008A00C(atn::SimpleVector<atn::Model::NodeEdit_unk>* edits, const void* text, int size) {
    atn::CsvTab csv(text, size);
    do {
        if (strcmp(csv.fn_8005B9CC(), "NODE_EDIT") == 0) {
            atn::Model::NodeEdit_unk edit;
            fn_80089EE4(&edit, &csv);
            edits->push_back(edit);
        }
    } while (csv.fn_8005BBF8() != 0);
}

namespace atn {

ModelBase::ModelBase(unsigned int flags)
    : mFlags(flags), m40(1.0f, 1.0f, 1.0f, 1.0f), m50(0), m54(1.0f), m58(0), m5C(0), m94(6), m98(0.0f), m134(0),
      m138(0), m14C(1.0f), m150(1.0f), m154(0.0f), m158(1.0f), m15C(0), m160(0), m1DC(0), m1E0(0), m1F0(0), m1F4(0),
      m1F8(0), m1FC(0), m208(0) {
    m20C_31_24 = 0;
    m20C_23_16 = 1;
    m20C_15_8 = 0;
    m20C_7 = 0;
    mFlag6_unk = 1;
    mFlag5_unk = 0;
    mFlag4_unk = 0;
    mFlag3_unk = 0;
    mFlag2_unk = 0;
    mFlag1_unk = 0;
    m20C_0 = 0;
    m9C.x = 0.0f;
    m9C.y = 0.0f;
    m9C.z = 0.0f;
    mA8 = 0.0f;
    mAC = 1.0f;
    mB0 = -1;
    mB4 = -1;
}

ModelBase::~ModelBase() {
    fn_8008AA90();
}

struct CompositeCacheEntry_unk;
// inferred: reference to an entry of the texture composite cache (its methods are compiled in
// atn/CompositeIntensityTintMulti.cpp: constructor 0x800D87A4, 0x800D87B0, Release_unk 0x800D87C4)
struct CompositeCacheHandle_unk {
    void Release_unk();
    CompositeCacheEntry_unk* mEntry;
};

// inferred: polymorphic texture object owned by a composite entry
struct ModelCompositeTex_unk {
    virtual ~ModelCompositeTex_unk();
};

// inferred: composite texture of a model (atn::ModelBase::m1E4); names invented
struct ModelComposite_unk {
    std::string mName;
    int m0C;
    ModelCompositeTex_unk* m10;
    CompositeCacheHandle_unk m14;
};

// NONMATCHING: 97.9% - registers of the composite loop (the zero stored to m10 is a copy of the counter)
void ModelBase::fn_8008AA90() {
    m20C_31_24 = 0;
    for (unsigned int i = 0; i < m144.size(); i++) {
        m144.mData[i]->m00--;
    }
    for (unsigned int i = 0; i < m13C.size(); i++) {
        delete m13C.mData[i]->mLODs.mData[0].mShader;
        delete m13C.mData[i];
    }
    for (unsigned int i = 0; i < m1E4.mSize; i++) {
        if (m1E4.mData[i]->m10 != 0) {
            delete m1E4.mData[i]->m10;
            m1E4.mData[i]->m10 = 0;
        }
        m1E4.mData[i]->m14.Release_unk();
        delete m1E4.mData[i];
    }
    if (m15C != 0) {
        delete m15C;
    }
    if (m160 != 0) {
        delete m160;
    }
    if (m58 != 0) {
        MemoryManager::Instance()->Free(m58);
        m58 = 0;
    }
}

// inferred: node index by name hash in the given LOD (inline of fn_800928B0)
inline int FindNodeRes_unk(const Model::Res_unk* res, unsigned int hash) {
    if (res == 0) {
        return -1;
    }
    for (int i = 0; i < res->m44; i++) {
        if (hash == res->m48[i]->m14) {
            return i;
        }
    }
    return -1;
}
inline int FindNode_unk(const ModelBase* model, unsigned int hash, int lod) {
    return FindNodeRes_unk((const Model::Res_unk*)model->mLods[lod].m08, hash);
}

// inferred: the material has a texture stage with the given id
inline bool HasStage_unk(Material_unk* m, int id) {
    return m->fn_80080528(id) != 0;
}

// inferred: loads the model, its materials ("material" next to the model, or `node`) and sets the
// draw priority from the hash of the file name
// NONMATCHING: 98.8% - registers (this/path in r30/r31 instead of r29/r30), and "material" follows a "" in the pool
void ModelBase::fn_8008AD4C(const char* path, const char* node) {
    m20C_31_24 = 1;
    mLods[0].mHandle.vf0C(path);
    std::string mat;
    if (node == 0) {
        mat.assign(path);
        mat = fn_8006AD24(mat, std::string("material"));
    } else {
        mat.assign(node);
    }
    if (fn_8006AA20(mat.c_str())) {
        mFC.vf0C(mat.c_str());
    }
    unsigned int lo = fn_800DC6B4(fn_8006B174(std::string(path)).c_str());
    unsigned int hi = fn_800DC6B4(fn_8006B174(std::string(path)).c_str());
    m98 = 1e-5f * (float)((hi & 0xFF00) | (lo & 0xFF));
}

const char* ModelBase::fn_8008B130() {
    return mLods[0].mHandle.fn_80473F18();
}

unsigned int ModelBase::fn_8008B138() {
    return mLods[0].mHandle.fn_80473F34();
}
} // namespace atn

// inferred: the render system (lbl_8087EFB4); called out of line by GameUI and others
extern "C" void* fn_8008B964() {
    return lbl_8087EFB4;
}

namespace atn {

int ModelBase::vf20() {
    return 0;
}

void ModelBase::vf24() {
}

// NONMATCHING: 92.3% - stack layout (the path strings are above the boost::function temporaries)
void ModelBase::fn_8008B978(int unbound) {
    mFlag3_unk = unbound;
    if (!mFlag3_unk) {
        std::string dir = fn_8006B0C8(std::string(fn_8008B130()));
        for (unsigned int i = 0; i < m104.mMaterials.size(); i++) {
            TexGetter_unk getter = boost::bind(&ModelBase::fn_80095B08, this, _1);
            m104.mMaterials.mData[i]->fn_80080C20(dir, getter);
        }
        m20C_31_24 = 4;
        mFlag2_unk = 1;
    }
}

// inferred: index of the material with the given name hash (the size is read once)
inline int FindMaterial_unk(const SimpleArray<Material_unk*>& mats, unsigned int hash) {
    unsigned int n = mats.mSize;
    for (unsigned int i = 0; i < n; i++) {
        if (hash == mats.mData[i]->m00) {
            return i;
        }
    }
    return -1;
}

// NONMATCHING: 91.6% - registers and frame (the original keeps two more saved registers around the string)
void ModelBase::fn_8008C62C(int lod) {
    Res_unk* main = (Res_unk*)mLods[0].m08;
    ModelLod_unk* l = &mLods[lod];
    int n = ((Res_unk*)l->m08)->m30;
    m104.m08[lod].Resize_unk(main->m30, 6);
    for (int i = 0; i < n; i++) {
        const char* name = ((Res_unk*)l->m08)->m50[i];
        int index = FindMaterial_unk(m104.mMaterials, fn_800DC6B4(name));
        if (index < 0) {
            char msg[0x100];
            sprintf(msg, "Lod \"%s\" material \"%s\" not matched\n",
                    fn_8006B174(std::string(l->mHandle.fn_80473F18())).c_str(), name);
            msg[0xFF] = 0;
            lbl_8087EEB8->fn_800697D8(msg);
            index = -4;
        }
        m104.m08[lod].mData[i] = index;
    }
}

void Model::fn_8008C828(MaterialSet_unk* list) {
    if (list == m138) {
        return;
    }
    m138 = list;
    if (list != 0) {
        fn_8008C850(list);
    } else {
        fn_8008C850(&m104);
    }
}

void Model::fn_8008C850(MaterialSet_unk* list) {
    mFlags &= ~0x2000;
    mFlags &= ~0x40000;
    mFlags &= ~0x30;
    mFlags &= ~0x20000;
    for (unsigned int i = 0; i < list->mMaterials.mSize; i++) {
        Material_unk* m = list->mMaterials.mData[i];
        if (m->fn_800816A8()) {
            mFlags |= 0x10;
        } else {
            mFlags |= 0x20;
        }
        if (HasStage_unk(m, 0x15)) {
            mFlags |= 0x2000;
        }
        if (HasStage_unk(m, 0x14)) {
            mFlags |= 0x40000;
        }
        if (m->m48 & 2) {
            mFlags |= 0x20000;
        }
    }
}

// NONMATCHING: 97.3% - the "" of the pool is at 0xA in the original, and the minimum z of the radius is rounded (frsp)
void ModelBase::fn_8008C950() {
    ModelLod_unk* lod = &mLods[0];
    Res_unk* res = (Res_unk*)lod->m08;
    if (res == 0) {
        return;
    }
    mMtx.mNodes.Resize_unk(res->m44, 6);
    if (((Res_unk*)lod->m08)->m44 * 0x30 > 0x1800) {
        char msg[0x100];
        sprintf(msg, "Model %s: too many nodes (%d, max %d)", fn_8008B130(), ((Res_unk*)lod->m08)->m44, 0x80);
        lbl_8087EEB8->fn_800697D8(msg);
    }
    unsigned int size = (((Res_unk*)lod->m08)->m38->m10 * 0x30 + 0x1F) & ~0x1F;
    m58 = MemoryManager::Instance()->fn_800838B8(size, 0x20, (MemoryTag)9, "", "", 0);
    mE4.Resize_unk(((Res_unk*)lod->m08)->m44, 6);
    mEC.Resize_unk(((Res_unk*)lod->m08)->m44, 6);
    for (int i = 0; i < ((Res_unk*)lod->m08)->m44; i++) {
        mE4.mData[i] = -1;
        mEC.mData[i] = 1;
    }
    m90 = fn_80476130(&lod->mHandle);
    res = (Res_unk*)lod->m08;
    m9C = (res->GetMax_unk() + res->GetMin_unk()) * 0.5f;
    mA8 = (res->GetMax_unk() - res->GetMin_unk()).Length_unk() * 0.5f;
    CalcMatrixStack(0, ModelNodeCallback_unk());
}

void ModelBase::vf18(RenderView_unk* view) {
    if (m20C_31_24 == 8) {
        fn_8008D78C();
        if (mFlag6_unk) {
            fn_80091684(*(const Mtx*)((char*)view + 0x15C), mLods[m50].m10.mData);
        }
        if (m20C_0) {
            fn_80095CD0();
        }
    }
}

void ModelBase::vf1C() {
}

// NONMATCHING: 97.9% - registers (the original keeps &mMtx in r29 and this in r30)
void ModelBase::CalcMatrixStack(const MotionBuffer* motion, ModelNodeCallback_unk callback) {
    Mtx_unk& mtx = mMtx;
    Res_unk* data = (Res_unk*)mLods[m50].m08;
    const int* remap = mLods[m50].m10.mData;
    if (data == 0) {
        return;
    }
    ModelPose_unk pose(data, mtx.mNodes.mData, remap, mB8, mCC.mData, mCC.mSize);
    if (!callback.empty()) {
        pose.fn_80090584(callback);
    }
    if (mD8.mSize != 0) {
        pose.SetEdits_unk(mE4.mData, mD8.mData, mD8.mSize);
    }
    if (motion == 0) {
        pose.fn_8009A584(mtx);
    } else {
        pose.fn_8009A5A8(mtx, motion);
    }
}
} // namespace atn

ModelPose_unk::ModelPose_unk(ModelData_unk* model, atn::Matrix3x4* matrices, const int* remap,
                             boost::function<void()> callback, const int* extra, int extraCount)
    : m00(model), m04(matrices), m08(remap), m0C(0), m10(0), m14(0), m18(callback), m40(extra), m44(extraCount),
      m48(0) {}

void ModelPose_unk::fn_80090584(atn::ModelNodeCallback_unk callback) {
    m2C = callback;
}

namespace atn {

// inferred: applies `m` to the world matrix and to every node matrix
void ModelBase::fn_8009076C(const Mtx m) {
    Mtx_unk* mtx = &mMtx;
    Res_unk* data;
    const int* remap = mLods[m50].m10.mData;
    data = (Res_unk*)mLods[m50].m08;
    Mtx t;
    PSMTXConcat(mtx->m, m, t);
    if (remap != 0) {
        for (int n = data->m44; n > 0; n--, remap++) {
            Matrix3x4* node = &mtx->mNodes.mData[*remap];
            PSMTXConcat(t, node->m, node->m);
        }
    } else {
        int n = data->m44;
        for (int i = 0; n > 0; n--, i++) {
            Matrix3x4* node = &mtx->mNodes.mData[i];
            PSMTXConcat(t, node->m, node->m);
        }
    }
}
} // namespace atn

// inferred: out[i] = root * b[remap[i]] * a[i]
extern "C" void fn_80090834(atn::Matrix3x4* out, const atn::Matrix3x4* a, const atn::Matrix3x4* b, const int* remap,
                            unsigned int n, const atn::Matrix3x4& root) {
    for (unsigned int i = 0; i < n; i++) {
        atn::Matrix3x4 r;
        atn::Matrix3x4 t;
        PSMTXConcat(b[remap[i]].m, a[i].m, t.m);
        PSMTXConcat(root.m, t.m, r.m);
        out[i] = r;
    }
}

// inferred: out[i] = root * b[i] * a[i]
extern "C" void fn_800908FC(atn::Matrix3x4* out, const atn::Matrix3x4* a, const atn::Matrix3x4* b, unsigned int n,
                            const atn::Matrix3x4& root) {
    for (unsigned int i = 0; i < n; i++) {
        atn::Matrix3x4 r;
        atn::Matrix3x4 t;
        PSMTXConcat(b[i].m, a[i].m, t.m);
        PSMTXConcat(root.m, t.m, r.m);
        out[i] = r;
    }
}

// inferred: skinning weights of a vertex block with two joints (0x14 bytes); names invented
struct SkinWeight2_unk {
    int m00;
    int mIndex0;
    float mWeight0;
    int mIndex1;
    float mWeight1;
};

// inferred: matrix scaled by a weight (name invented)
inline atn::Matrix3x4 ScaleMtx_unk(const atn::Matrix3x4& a, float w) {
    atn::Matrix3x4 r;
    r.m[0][0] = a.m[0][0] * w;
    r.m[0][1] = a.m[0][1] * w;
    r.m[0][2] = a.m[0][2] * w;
    r.m[0][3] = a.m[0][3] * w;
    r.m[1][0] = a.m[1][0] * w;
    r.m[1][1] = a.m[1][1] * w;
    r.m[1][2] = a.m[1][2] * w;
    r.m[1][3] = a.m[1][3] * w;
    r.m[2][0] = a.m[2][0] * w;
    r.m[2][1] = a.m[2][1] * w;
    r.m[2][2] = a.m[2][2] * w;
    r.m[2][3] = a.m[2][3] * w;
    return r;
}

// inferred: element-wise sum (name invented)
inline void AddMtx_unk(atn::Matrix3x4& d, const atn::Matrix3x4& a) {
    d.m[0][0] += a.m[0][0];
    d.m[0][1] += a.m[0][1];
    d.m[0][2] += a.m[0][2];
    d.m[0][3] += a.m[0][3];
    d.m[1][0] += a.m[1][0];
    d.m[1][1] += a.m[1][1];
    d.m[1][2] += a.m[1][2];
    d.m[1][3] += a.m[1][3];
    d.m[2][0] += a.m[2][0];
    d.m[2][1] += a.m[2][1];
    d.m[2][2] += a.m[2][2];
    d.m[2][3] += a.m[2][3];
}

// inferred: out[i] = mats[a] * wa + mats[b] * wb for each weight
// NONMATCHING: 45% - the original computes and stores the first matrix before loading the second one
// (8 saved float registers instead of 17)
extern "C" void fn_800909B0(atn::Matrix3x4* out, const atn::Matrix3x4* mats, const SkinWeight2_unk* w, int n) {
    for (int i = 0; i < n; i++) {
        *out = ScaleMtx_unk(mats[w->mIndex0], w->mWeight0);
        atn::Matrix3x4 t = ScaleMtx_unk(mats[w->mIndex1], w->mWeight1);
        AddMtx_unk(*out, t);
        w++;
        out++;
    }
}

namespace atn {

// inferred: the two matrices are equal (name invented)
inline bool SameMtx_unk(const Mtx a, const Mtx b) {
    return memcmp(a, b, 0x30) == 0;
}

// NONMATCHING: 99.6% - the store order of the copy into m60
void ModelBase::fn_80091A90(const Mtx m) {
    if (mFlag4_unk && mFlag5_unk && *(int*)((char*)lbl_8087EFB4 + 0x2F8) == 3) {
        fn_80091684(m, m1C4.m10.mData);
        return;
    }
    if (m5C != m50) {
        fn_80091684(m, mLods[m50].m10.mData);
        return;
    }
    if (SameMtx_unk(m60.m, m)) {
        return;
    }
    Matrix3x4 inv(m60);
    PSMTXInverse(inv.m, inv.m);
    PSMTXConcat(m, inv.m, inv.m);
    m60 = *(const Matrix3x4*)m;
    Res_unk* data = (Res_unk*)mLods[0].m08;
    Mtx* skin = (Mtx*)m58;
    if (data->m38->m10 > 1) {
        PSMTXConcatArray(inv.m, skin, skin, data->m38->m10);
    } else {
        PSMTXConcat(inv.m, *skin, *skin);
    }
}

// NONMATCHING: 98.1% - the joint pointer is kept in r12 in the original
void ModelBase::fn_80091C10(const ModelBase* src, const int* map, int reset) {
    Res_unk* data = (Res_unk*)mLods[0].m08;
    for (int i = 0; i < data->m44; i++) {
        int index = map[i];
        ModelJoint_unk* joint = data->m48[i];
        if (index == -1) {
            if (reset) {
                mMtx.mNodes.mData[joint->m18].SetIdentity();
            }
        } else {
            const Matrix3x4* s = (index < 0) ? 0 : &src->mMtx.mNodes.mData[index];
            mMtx.mNodes.mData[joint->m18] = *s;
        }
    }
}

// RSO: GetNodeNoByName__Q23atn9ModelBaseCFPCci
int ModelBase::GetNodeNoByName(const char* name, int lod) const {
    return FindNode_unk(this, fn_800DC6B4(name), lod);
}

int Model::fn_800928B0(unsigned int hash, int lod) {
    return FindNode_unk(this, hash, lod);
}

// inferred: index of the material with the given name hash (inline of fn_8009290C)
inline int FindMaterialIndex_unk(Model* model, unsigned int hash) {
    for (unsigned int i = 0; i < model->m104.mMaterials.mSize; i++) {
        if (hash == model->m104.mMaterials.mData[i]->m00) {
            return i;
        }
    }
    return -1;
}

int Model::fn_8009290C(unsigned int hash) {
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        if (hash == m104.mMaterials.mData[i]->m00) {
            return i;
        }
    }
    return -1;
}

// inferred: material by name hash (inline of fn_800929C0)
inline Material_unk* FindMaterial_unk(Model* model, unsigned int hash) {
    for (unsigned int i = 0; i < model->m104.mMaterials.mSize; i++) {
        Material_unk* m = model->m104.mMaterials.mData[i];
        if (hash == m->m00) {
            return m;
        }
    }
    return 0;
}

Material_unk* Model::fn_80092954(const char* name) {
    return FindMaterial_unk(this, fn_800DC6B4(name));
}

// inferred: material with the given name hash
Material_unk* Model::fn_800929C0(unsigned int hash) {
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        Material_unk* m = m104.mMaterials.mData[i];
        if (hash == m->m00) {
            return m;
        }
    }
    return 0;
}

// inferred: sets the alpha of every material
void Model::fn_80092A00(float alpha) {
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        m104.mMaterials.mData[i]->m3C = (int)(255.0f * alpha);
    }
}

void Model::fn_80093014(int on) {
    mFlag1_unk = on;
    if (mFlag1_unk) {
        m20C_31_24 = 5;
    }
}

void Model::fn_8009373C(const char* name, int show) {
    int i = FindNode_unk(this, fn_800DC6B4(name), 0);
    if (i >= 0) {
        mEC.mData[i] = show != 0;
    }
}

void Model::fn_800937E8(unsigned int hash, int show) {
    int i = FindNode_unk(this, hash, 0);
    if (i >= 0) {
        mEC.mData[i] = show != 0;
    }
}

void Model::fn_80093B88(const char* name) {
    int i = FindNode_unk(this, fn_800DC6B4(name), 0);
    if (i >= 0) {
        mEC.mData[i] = 1;
    }
}

void Model::fn_80093C20(unsigned int hash) {
    int i = FindNode_unk(this, hash, 0);
    if (i >= 0) {
        mEC.mData[i] = 1;
    }
}

int Model::fn_80093C8C(const char* name) {
    int i = FindNode_unk(this, fn_800DC6B4(name), 0);
    if (i >= 0) {
        return mEC.mData[i];
    }
    return 1;
}

int Model::fn_80093D28(unsigned int hash) {
    int i = FindNode_unk(this, hash, 0);
    if (i >= 0) {
        return mEC.mData[i];
    }
    return 1;
}

void Model::fn_80093D98(const char* name, int show) {
    int i = FindMaterialIndex_unk(this, fn_800DC6B4(name));
    if (i >= 0) {
        mF4.mData[i] = show != 0;
    }
}

void Model::fn_80093E30(unsigned int hash, int show) {
    int i = FindMaterialIndex_unk(this, hash);
    if (i >= 0) {
        mF4.mData[i] = show != 0;
    }
}

int Model::fn_80093E90(unsigned int hash) {
    int i = FindMaterialIndex_unk(this, hash);
    if (i >= 0) {
        return mF4.mData[i];
    }
    return 0;
}

void Model::fn_80093EEC(int show) {
    unsigned char on = show != 0;
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        mF4.mData[i] = on;
    }
}

void Model::fn_80093F1C(const char* name, const atn::Vector3& a, const atn::Vector3& b, const atn::Vector3& c,
                        int flags) {
    NodeEdit_unk edit;
    fn_80089D70(&edit, name, a, b, c, flags);
    fn_80093F58(&edit);
}

Model::NodeEdit_unk* Model::fn_800948F0(const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    for (unsigned int i = 0; i < mD8.mSize; i++) {
        NodeEdit_unk* e = &mD8.mData[i];
        if (hash == e->m00) {
            return e;
        }
    }
    return 0;
}

Material_unk* Model::fn_80094B6C(const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        Material_unk* m = m104.mMaterials.mData[i];
        if (m->m00 == hash) {
            return m;
        }
    }
    return 0;
}

void Model::fn_80094D88(int arg, int flag, int a, int b, int c) {
    unsigned int n = m104.mMaterials.mSize;
    for (unsigned int i = 0; i < n; i++) {
        Material_unk* m = m104.mMaterials.mData[i];
        m->fn_8007FAF0(arg);
        int mode = 3;
        if (flag != 0) {
            mode = 0;
        }
        m->m4C = mode;
        m->m14_20 = a == 0;
        m->m14_21 = b == 0;
        m->m14_25 = c == 0;
    }
    fn_8008C850(&m104);
}

void Model::fn_80094E2C(int value) {
    unsigned int n = m104.mMaterials.mSize;
    for (unsigned int i = 0; i < n; i++) {
        m104.mMaterials.mData[i]->m34 = value;
    }
}

void Model::fn_80094F14(float u, float v) {
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        m104.mMaterials.mData[i]->fn_80081514(u, v);
    }
}

void ModelBase::fn_80095CD0() {
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        if (m104.mMaterials.mData[i]->m4B != 0) {
            m104.mMaterials.mData[i]->fn_80080048(1.0f / 30.0f);
        }
    }
}

// NONMATCHING: 83% - the copy of dir is scheduled before the arguments
void Model::fn_80097750(const atn::Vector3& dir, float a, float b, int c) {
    atn::Vector3 n(dir);
    n.Normalize_unk();
    m3A8 = n;
    m3B4 = a;
    m3B8 = b;
    m3BC = c;
}

void Model::fn_80097A20(unsigned int motion) {
    LH_Motion* slot = &m224.mData[motion];
    slot->fn_80473F88();
    slot->m08 = 0;
    fn_800A1060(&m354.mData[motion]);
}

void Model::fn_80097A88(int motion, const char* path) {
    m224.mData[motion].fn_800A03E4(path);
}

void Model::fn_80097A9C(int motion, const LH_Motion* src) {
    if ((int)m224.mSize <= motion || motion < 0) {
        return;
    }
    LH_Motion* slot = &m224.mData[motion];
    *slot = *src;
    slot->m08 = src->m08;
    fn_800A0A68(&m354.mData[motion], this, src);
}

int Model::fn_80097B34() {
    int n = 0;
    for (int i = 0; i < (int)m224.mSize; i++) {
        if (m224.mData[i].m08 != 0) {
            n++;
        }
    }
    return n;
}

int Model::vf20() {
    int busy = 0;
    for (int i = 0; i < (int)m224.mSize; i++) {
        if (m224.mData[i].fn_800A0448()) {
            busy = 1;
        }
    }
    return busy;
}

void Model::fn_80097C08(int track, int motion, int loop, int force, int arg5, float blend, float speed) {
    if (force != 0 || motion != mTracks[track].m00) {
        Track_unk& t = mTracks[track];
        t.m24 = t.m04;
        t.m28 = t.m08;
        t.m2C = t.m10;
        t.m00 = motion;
        if (motion >= 0 && motion < (int)m224.mSize && m224.mData[motion].m08 != 0) {
            t.m04 = (int)&m224.mData[motion];
            t.m08 = 0.0f;
            t.m10 = blend;
            t.m18 = loop != 0;
            t.m14 = speed;
            t.m19[0] = arg5 != 0;
        } else {
            t.m04 = 0;
        }
    } else {
        mTracks[track].m18 = loop != 0;
    }
}

void Model::fn_80097CCC(int track, float blend) {
    Track_unk& t = mTracks[track];
    t.m24 = t.m04;
    t.m28 = t.m08;
    t.m2C = t.m10;
    if (track == 3) {
        t.m04 = 0;
        t.m10 = blend < t.m2C ? blend : t.m2C;
    } else {
        t.m00 = -1;
        t.m04 = 0;
        if (track >= 3) {
            t.m08 = 0.0f;
            t.m10 = blend;
            t.m18 = 0;
        }
    }
}

atn::LoadHandle* Model::fn_80097D40(int motion) {
    if (motion < 0 || motion >= (int)m224.mSize) {
        return 0;
    }
    LH_Motion* slot = &m224.mData[motion];
    if (slot->m08 != 0) {
        return slot;
    }
    return 0;
}

float Model::fn_80097D7C(int track) {
    if (mTracks[track].m04 != 0) {
        return fn_800A08D4((LH_Motion*)mTracks[track].m04);
    }
    return 0.0f;
}

void Model::fn_80099E9C(const char* node, const char* name, float value) {
    m3C0 = FindNode_unk(this, fn_800DC6B4(node), 0);
    m3C4 = value;
    if (name != m3C8) {
        memcpy(m3C8, name, strlen(name) + 1);
    }
}
} // namespace atn

// inferred: out[i] = root * b[i] * a[i]
// NONMATCHING: 97.6% - register allocation of the pointers
extern "C" void fn_8009A490(Mtx* out, const Mtx* a, const Mtx* b, int n, const Mtx root) {
    while (n != 0) {
        PSMTXConcat(*b, *a, *out);
        PSMTXConcat(root, *out, *out);
        out++;
        n--;
        a++;
        b++;
    }
}

// inferred: out[i] = root * mats[index[i]] * a[i]
// NONMATCHING: 97.5% - register allocation of the pointers
extern "C" void fn_8009A504(Mtx* out, const Mtx* a, const Mtx* mats, const int* index, int n, const Mtx root) {
    while (n != 0) {
        PSMTXConcat(mats[*index], *a, *out);
        PSMTXConcat(root, *out, *out);
        out++;
        a++;
        index++;
        n--;
    }
}

__declspec(noinline) void ModelPose_unk::fn_8009A584(const atn::Matrix3x4& root) {
    if (m00->m44 > 0) {
        fn_8009A5CC(this, 0, &root, 0);
    }
}

__declspec(noinline) void ModelPose_unk::fn_8009A5A8(const atn::Matrix3x4& root, const atn::MotionBuffer* motion) {
    if (m00->m44 > 0) {
        fn_8009ADA4(this, 0, motion, &root, 0);
    }
}
