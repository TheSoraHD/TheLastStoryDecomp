// atn::Model / atn::ModelBase (0x800899DC-0x8009A5CC). Written in the order of the .dol; the
// functions not written yet are missing from this file (the unit is not linked).
#include "atn/MotionSlot_unk.h"
#include "atn/Model_unk.h"
#include "atn/Material_unk.h"
#include "atn/LH_Motion.h"
#include "atn/ModelCamera_unk.h"
#include "atn/Csv.h"
#include "atn/Shader_unk.h"
#include "atn/MemoryManager.h"
#include "std/string.h"
#include "atn/AABB_unk.h"
#include "atn/PBone.h"
#include "atn/RenderManager.h"
#include "atn/RenderSetting_unk.h"
#include "std/new.h"
#include <boost/bind.hpp>
#include "nw4r/math/triangular.h"

extern "C" char* strcpy(char* dst, const char* src);
extern "C" int strcmp(const char* a, const char* b);
extern "C" float fn_800DC288(const char* s); // inferred: atof
extern "C" int fn_800DC12C(const char* s);   // inferred: atoi
extern "C" int fn_8006AA20(const char* path); // inferred: the file exists
extern "C" std::string& fn_8006AD24(std::string& path, const std::string& ext); // inferred: changes the extension
extern "C" std::string fn_8006B174(const std::string& path); // inferred: file name of a path
std::string fn_8006B0C8(const std::string& path); // inferred: directory of a path
extern "C" RenderSetting_unk* lbl_8087EFA8; // inferred: render settings (frame speed at 0x3A4)
// inferred: base of the locked cache (scratch for the node matrices); name invented
#define LOCKED_CACHE_UNK ((atn::Matrix3x4*)0xE0000000)
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int memcmp(const void* a, const void* b, unsigned long n);
extern "C" int rand();
extern "C" void PSMTXMultVecArray(const Mtx m, const atn::Vector3* srcBase, atn::Vector3* dstBase, unsigned int count);
extern "C" void PSMTXInverse(const Mtx src, Mtx inv);
extern "C" void PSMTXConcatArray(const Mtx a, const Mtx* srcBase, Mtx* dstBase, unsigned int count);
// inferred: collects the warnings of the loaders (see game/main.cpp)
struct DebugMessages_unk {
    void fn_800697D8(const char* msg);
};
extern "C" DebugMessages_unk* lbl_8087EEB8;
extern "C" atn::Matrix3x4* fn_80476130(atn::LH_Model* model); // inferred: inverse joint matrices
extern "C" int fn_80476194(atn::LH_Model* model); // inferred: the model has no occlusion data
extern "C" int fn_804761B0(atn::LH_Model* model); // inferred: the occlusion data is ready
namespace atn { struct ModelBase; }
extern "C" void fn_8008BD10(atn::ModelBase* model); // inferred: creates the materials (a ModelBase member in the original)
extern "C" ShaderManager_unk* lbl_8087EFE0; // inferred: shader manager
extern "C" int fn_80476204(atn::LH_Model* model); // inferred: the model keeps the material flags
extern "C" atn::AABB_unk fn_80070B60(const atn::AABB_unk* box, float scale); // inferred: scales a box

// inferred: empty asserts of the release build (they keep fn_80091684 and fn_80095300 over the inline
// budget, as in the original); name invented
#define MTX_ASSERT_UNK(x) ((void)0)
// inferred: ten of them (about 400 are needed)
#define MTX_ASSERT10_UNK(x) \
    MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x); \
    MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x); MTX_ASSERT_UNK(x)

// inferred: b * a by value (weak copy 0x8008CD50); name invented
inline atn::Matrix3x4 MulMtx_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0); MTX_ASSERT10_UNK(&a != 0);
    atn::Matrix3x4 r;
    PSMTXConcat(b.m, a.m, r.m);
    return r;
}

extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash

// inferred: skin matrices of the weighted vertex blocks (the paired-single versions 0x8009A0F8/0x8009A2C0
// work on the locked cache; 0x80090C58/0x800910D4 on main memory and advance the output pointer)
extern "C" void C_QUATSlerp(const atn::Quaternion_unk* p, const atn::Quaternion_unk* q, atn::Quaternion_unk* r, float t);
extern "C" unsigned long LCLoadData(void* dst, void* src, unsigned long size);
extern "C" void LCQueueWait(unsigned long len);
extern "C" void fn_80099F74(const SkinWeight2_unk* w, const atn::Matrix3x4* mats, atn::Matrix3x4* out, int n);
extern "C" void fn_8009A018(const SkinWeight3_unk* w, const atn::Matrix3x4* mats, atn::Matrix3x4* out, int n);
extern "C" void fn_800A05FC(atn::LH_Motion* motion, MotionPose_unk* pose, int key, atn::NodeWork_unk* work, float frame); // inferred: samples a motion key
extern "C" void fn_80098948(MotionPose_unk* out, unsigned int n, const MotionPose_unk* a, const MotionPose_unk* b, float t);
extern "C" void DCZeroRange(void* addr, unsigned long size);
extern "C" void fn_8009A490(Mtx* out, const Mtx* a, const Mtx* b, int n, const Mtx root);
extern "C" void fn_8009A504(Mtx* out, const Mtx* a, const Mtx* mats, const int* index, int n, const Mtx root);
extern "C" void fn_8009A0F8(atn::Matrix3x4* out, const atn::Matrix3x4* mats, const SkinWeight2_unk* w, int n);
extern "C" void fn_8009A2C0(atn::Matrix3x4** out, const atn::Matrix3x4* mats, const SkinWeight3_unk* w, int n);
extern "C" void fn_80090C58(atn::Matrix3x4** out, const atn::Matrix3x4* mats, const SkinWeight3_unk* w, int n);
extern "C" void fn_800910D4(atn::Matrix3x4** out, const atn::Matrix3x4* mats, const SkinWeight4_unk* w, int n);

#include "atn/MotionCmd_unk.h"
extern "C" void fn_800A0A68(atn::MotionCmd_unk* cmd, atn::Model* model, const atn::LH_Motion* src); // inferred: copies
extern "C" float fn_800A08D4(atn::LH_Motion* motion); // inferred: length of the motion

namespace atn {

inline Model::Node_unk::Node_unk() {
}

inline atn::Vector3 ModelJoint_unk::GetScale_unk() const {
    return m34;
}

inline atn::Vector3 ModelJoint_unk::GetRot_unk() const {
    return m40;
}

inline atn::Vector3 ModelJoint_unk::GetTrans_unk() const {
    return m4C;
}

// inferred: rotation from euler angles (same as Quaternion_unk::fn_801498F0 of game/CharaTask.cpp); name
// invented
inline Quaternion_unk EulerToQuat_unk(const Vector3& rot) {
    Quaternion_unk q;
    float rx = rot.x;
    float ry = rot.y;
    float rz = rot.z;
    float cx, cy, cz;
    float sx, sy, sz;
    nw4r::math::SinCosRad(&sx, &cx, rx / 2.0f);
    nw4r::math::SinCosRad(&sy, &cy, ry / 2.0f);
    nw4r::math::SinCosRad(&sz, &cz, rz / 2.0f);
    float cycz = cy * cz;
    float sysz = sy * sz;
    q.w = cx * cycz + sx * sysz;
    q.x = sx * cycz - cx * sysz;
    q.y = cz * (cx * sy) + sz * (sx * cy);
    q.z = sz * (cx * cy) - cz * (sx * sy);
    return q;
}

inline NodeWork_unk::NodeWork_unk() {
    memset(this, 0, sizeof(NodeWork_unk));
}

inline int RenderManager::GetPass_unk() const {
    return m2F8;
}

inline ModelBase::Mtx_unk* Model::GetMtx_unk() {
    return &mMtx;
}

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
    atn::Texture_unk* tex;
    for (unsigned int i = 0; i < mMaterials.mSize; i++) {
        for (int j = 0; j < mMaterials.mData[i]->m4A; j++) {
            tex = mMaterials.mData[i]->m0C[j].mTexture;
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
    CompositeCacheHandle_unk();    // 0x800D87A4
    void AddRef_unk() const;       // 0x800D87B0
    void Release_unk();
    Texture_unk* fn_800D8808(); // inferred: texture of the entry
    CompositeCacheEntry_unk* mEntry;
};

// inferred: polymorphic texture object owned by a composite entry (a texture composite with its cache
// reference at 0x04)
struct ModelCompositeTex_unk {
    virtual ~ModelCompositeTex_unk();
    virtual int vf0C();  // inferred: still building (fn_8008B140)
    virtual int vf10();  // inferred: finished
    virtual void vf14(); // inferred: starts building
    CompositeCacheHandle_unk m04;
    int m08; // inferred: nonzero once started
};

// inferred: composite texture of a model (atn::ModelBase::m1E4); names invented
struct ModelComposite_unk {
    std::string mName;
    unsigned int m0C; // inferred: hash of the name
    ModelCompositeTex_unk* m10;
    CompositeCacheHandle_unk m14;
};

void ModelBase::fn_8008AA90() {
    m20C_31_24 = 0;
    for (unsigned int i = 0; i < m144.size(); i++) {
        m144[i]->m00--;
    }
    for (unsigned int i = 0; i < m13C.size(); i++) {
        delete m13C[i]->mLODs[0].mShader;
        delete m13C[i];
    }
    for (unsigned int i = 0; i < m1E4.mSize; i++) {
        if (m1E4[i]->m10 != 0) {
            delete m1E4[i]->m10;
            m1E4[i]->m10 = 0;
        }
        m1E4[i]->m14.Release_unk();
        delete m1E4[i];
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

// inferred: load state machine (m20C_31_24); returns nonzero while the model is still loading
// NONMATCHING: 99.2% - loop registers (counter/offset swapped, as in fn_8008AA90), the "" of the pool at 0xA and
// the stack slots of the path strings (below the boost::function temporaries in the original, as fn_8008B978)
int ModelBase::fn_8008B140() {
    switch (m20C_31_24) {
    case 1:
        if (mLods[0].mHandle.fn_80473F50()) {
            return 1;
        }
        if (mLods[0].mHandle.fn_80473EFC() == 3) {
            m20C_31_24 = 5;
        } else {
            m20C_31_24 = 9;
            break;
        }
    case 5: {
        int busy = 0;
        for (int i = 1; i < m20C_23_16; i++) {
            if (mLods[i].mHandle.fn_80473F50()) {
                busy = 1;
            }
        }
        if (mFlag5_unk) {
            int shadow = m1C4.mHandle.fn_80473F50();
            busy = busy || shadow;
            if (!shadow && m1C4.mHandle.fn_80473EFC() == 4) {
                mFlag5_unk = 0;
            }
        }
        if (busy) {
            return 1;
        }
        fn_8009303C();
        if (mFlag1_unk) {
            for (int i = 1; i < m20C_23_16; i++) {
                fn_8008C62C(i);
            }
            m20C_31_24 = 8;
            mFlag1_unk = 0;
            break;
        }
        fn_8008C950();
        int done = 1;
        int plain = 1;
        if ((mFlags & 2) != 2 && fn_80476194(&mLods[0].mHandle) == 0) {
            plain = 0;
        }
        if (!plain && fn_804761B0(&mLods[0].mHandle) == 0) {
            done = 0;
        }
        if (done) {
            Res_unk* res = GetRes_unk();
            if (res != 0 && res->m38->m10 != 1) {
                char msg[0x100];
                sprintf(msg, "WARNING Occlusion '%s' was build with motion\n", mLods[0].mHandle.fn_80473F18());
                msg[0xFF] = 0;
                lbl_8087EEB8->fn_800697D8(msg);
            }
            m20C_31_24 = 8;
            break;
        }
        m20C_31_24 = 2;
    }
    case 2:
        if (m1E4.mSize != 0) {
            int busy = 0;
            for (unsigned int i = 0; i < m1E4.mSize; i++) {
                ModelCompositeTex_unk* tex = m1E4.mData[i]->m10;
                if (tex != 0 && tex->vf0C()) {
                    busy = 1;
                }
            }
            if (busy || atn::RenderManager::instance_->mPassHeads[1] != 0) {
                return 1;
            }
            for (unsigned int i = 0; i < m1E4.mSize; i++) {
                ModelCompositeTex_unk* tex = m1E4.mData[i]->m10;
                if (tex != 0) {
                    if (tex->m08 == 0) {
                        tex->vf14();
                        return 1;
                    }
                    if (!tex->vf10()) {
                        return 1;
                    }
                    if (m1E4.mData[i]->m10 != 0) {
                        delete m1E4.mData[i]->m10;
                        m1E4.mData[i]->m10 = 0;
                    }
                }
            }
        }
        m20C_31_24 = 3;
    case 3:
        if (m1E4.mSize != 0) {
            int ready = 1;
            for (unsigned int i = 0; i < m1E4.mSize; i++) {
                ModelCompositeTex_unk* tex = m1E4.mData[i]->m10;
                if (tex != 0 && !tex->vf10()) {
                    ready = 0;
                }
            }
            if (!ready) {
                return 1;
            }
            for (unsigned int i = 0; i < m1E4.mSize; i++) {
                if (m1E4.mData[i]->m10 != 0) {
                    delete m1E4.mData[i]->m10;
                }
                m1E4.mData[i]->m10 = 0;
            }
        }
        m20C_31_24 = 7;
    case 7:
        if (mFC.fn_80473F50()) {
            return 1;
        }
        fn_8008BD10(this);
        GetModel_unk()->fn_8008C850(&m104);
        if (!mFlag3_unk) {
            std::string dir = fn_8006B0C8(std::string(fn_8008B130()));
            for (unsigned int i = 0; i < m104.mMaterials.size(); i++) {
                TexGetter_unk getter = boost::bind(&ModelBase::fn_80095B08, this, _1);
                m104.mMaterials.mData[i]->fn_80080C20(dir, getter);
            }
        }
        m20C_31_24 = 4;
    case 4: {
        int loading = 0;
        for (unsigned int i = 0; i < m104.mMaterials.size(); i++) {
            for (int j = 0; j < m104.mMaterials.mData[i]->m4A; j++) {
                Texture_unk* tex = m104.mMaterials.mData[i]->m0C[j].mTexture;
                if (tex != 0 && ((GimmickUvOwner_unk*)tex)->fn_800D59B8()) {
                    loading = 1;
                }
            }
        }
        if (loading) {
            return 1;
        }
        for (unsigned int i = 0; i < m104.mMaterials.size(); i++) {
            m104.mMaterials.mData[i]->fn_80081BD0();
        }
        if (mFlag2_unk) {
            mFlag2_unk = 0;
            m20C_31_24 = 8;
        } else {
            m20C_31_24 = 6;
        }
    }
    case 6:
        if (vf20()) {
            return 1;
        }
        m20C_31_24 = 8;
        vf24();
        break;
    }
    return 0;
}
} // namespace atn

// inferred: the render system (atn::RenderManager::instance_); called out of line by GameUI and others
extern "C" atn::RenderManager* fn_8008B964() {
    return atn::RenderManager::instance_;
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

// inferred: loaded material `i` of the material file (LoadMaterial::mMaterials)
inline Material_unk* LoadedMaterial_unk(LH_Material& file, int i) {
    return (*(std::vector<Material_unk*>*)file.fn_80474860())[i];
}

// inferred: debug menu folder of the model (created on first use when the debug menu is on)
inline void* ModelFolder_unk(ResHandle& handle, const char* name) {
    void* parent = *(void**)((char*)lbl_8087EFA8 + 0x4C); // inferred: debug menu folder of the models
    if (handle.mRes == 0 && parent != 0 && lbl_8087EF10 != 0) {
        return handle.mRes = fn_8008937C(parent, name);
    }
    return 0;
}

} // namespace atn

// NONMATCHING: 97.1% - registers (the original saves r20 too and keeps the zero of the first loop as a copy of
// the counter), and the "" of the pool at 0xA
// inferred: creates the materials and binds their shaders after the material file is loaded. A member of
// ModelBase in the original (declared here until Model_unk.h is changed in a batch re-pairing).
extern "C" void fn_8008BD10(atn::ModelBase* model) {
    using namespace atn;
    int count = model->GetRes_unk()->m30;
    if (count > 0xFE) {
        char msg[0x100];
        sprintf(msg, "WARNING Too many materials (%d) in '%s'\n", count, model->mLods[0].mHandle.fn_80473F18());
        msg[0xFF] = 0;
        lbl_8087EEB8->fn_800697D8(msg);
    }
    model->m144.Resize_unk(count, 6);
    model->m104.mMaterials.Resize_unk(count, 6);
    if ((model->mFlags & 0x800000) == 0x800000) {
        model->mF4.Resize_unk(count, 6);
    }
    for (int i = 0; i < count; i++) {
        model->m144.mData[i] = 0;
        model->m104.mMaterials.mData[i] = 0;
        if (model->mF4.mSize != 0) {
            model->mF4.mData[i] = 1;
        }
    }
    if (model->mFC.fn_80473EFC() == 3) {
        if ((int)model->mFC.fn_8047486C() > 0xFE) {
            char msg[0x100];
            sprintf(msg, "WARNING Too many materials (%d) in '%s'\n", model->mFC.fn_8047486C(), model->mFC.fn_80473F18());
            msg[0xFF] = 0;
            lbl_8087EEB8->fn_800697D8(msg);
        }
        if (fn_80476204(&model->mLods[0].mHandle) == 0) {
            for (int i = 0; i < (int)model->mFC.fn_8047486C(); i++) {
                Material_unk* m = LoadedMaterial_unk(model->mFC, i);
                unsigned char* flags = (unsigned char*)&m->m4E; // inferred: four tristate flags (1 = model default)
                for (int k = 0; k < 4; k++) {
                    if ((int)flags[k] == 1) {
                        flags[k] = 0;
                    }
                }
            }
        }
        for (int i = 0; i < count; i++) {
            Material_unk* found = 0;
            const char* name = model->GetRes_unk()->m50[i];
            unsigned int hash = fn_800DC6B4(name);
            for (int j = 0; j < (int)model->mFC.fn_8047486C(); j++) {
                if (hash == LoadedMaterial_unk(model->mFC, j)->m00) {
                    found = LoadedMaterial_unk(model->mFC, j);
                    break;
                }
            }
            ShaderLOD_unk* lod;
            if (found == 0) {
                char msg[0x100];
                sprintf(msg, "WARNING Material '%s' was not matched in model '%s'\n", name, model->mLods[0].mHandle.fn_80473F18());
                msg[0xFF] = 0;
                lbl_8087EEB8->fn_800697D8(msg);
                lod = lbl_8087EFE0->fn_800C5F5C("error_unmatched_material");
            } else {
                lod = lbl_8087EFE0->fn_800C5FF0(found->m04);
            }
            if (lod == 0) {
                char msg[0x100];
                sprintf(msg, "WARNING Shader was not found for model '%s'\n", model->mLods[0].mHandle.fn_80473F18());
                msg[0xFF] = 0;
                lbl_8087EEB8->fn_800697D8(msg);
                lod = lbl_8087EFE0->fn_800C5F5C("error_missing_shader");
            }
            model->m144.mData[i] = lod;
            lod->m00++;
            if (found != 0) {
                model->m104.mMaterials.mData[i] = new ((MemoryTag)6, "", "", 0) Material_unk(lod->mLODs[0].mShader);
                model->m104.mMaterials.mData[i]->fn_80080584(*found);
            } else {
                model->m104.mMaterials.mData[i] = new ((MemoryTag)6, "", "", 0) Material_unk(lod->mLODs[0].mShader);
                model->m104.mMaterials.mData[i]->fn_80080B34(lod->mLODs[0].mShader->mDesc);
            }
        }
    } else {
        char msg[0x100];
        sprintf(msg, "WARNING no material file found: '%s'\n", model->mLods[0].mHandle.fn_80473F18());
        msg[0xFF] = 0;
        lbl_8087EEB8->fn_800697D8(msg);
        ModelLod_unk* main = &model->mLods[0];
        model->m13C.Resize_unk(((ModelBase::Res_unk*)main->m08)->m30, 6);
        if (((ModelBase::Res_unk*)main->m08)->m34 != 0) {
            for (int i = 0; i < ((ModelBase::Res_unk*)main->m08)->m30; i++) {
                ShaderTex_unk* tex = new ((MemoryTag)6, "", "", 0) ShaderTex_unk(main, ((ModelBase::Res_unk*)main->m08)->m34[i]);
                ShaderLOD_unk* lod = new ((MemoryTag)6, "", "", 0) ShaderLOD_unk();
                lod->fn_800C4ECC(tex, 0.0f);
                model->m13C.mData[i] = lod;
                model->m144.mData[i] = lod;
                lod->m00++;
                lod->fn_800C4F38(&model->m104.mMaterials.mData[i]);
                model->m104.mMaterials.mData[i]->fn_80080B34(model->GetRes_unk()->m34[i]);
            }
        } else {
            ShaderLOD_unk* lod = lbl_8087EFE0->fn_800C5F5C("error_missing_material");
            for (int i = 0; i < ((ModelBase::Res_unk*)main->m08)->m30; i++) {
                lod->m00++;
                lod->fn_800C4F38(&model->m104.mMaterials.mData[i]);
            }
        }
    }
    model->m104.m08[0].Resize_unk(count, 6);
    model->m104.m28.Resize_unk(count, 6);
    for (unsigned int i = 0; i < model->m104.mMaterials.size(); i++) {
        model->m104.m08[0].mData[i] = i;
        model->m104.m28.mData[i] = model->m104.mMaterials.mData[i]->fn_800804D0(0xB);
        if (model->m104.mMaterials.mData[i]->m4B != 0) {
            model->m20C_0 = 1;
        }
    }
    for (int i = 1; i < model->m20C_23_16; i++) {
        model->fn_8008C62C(i);
    }
    if ((model->mFlags & 0x200) == 0x200) {
        void* folder = ModelFolder_unk(model->m210, fn_8006B174(std::string(model->fn_8008B130())).c_str());
        for (unsigned int i = 0; i < model->m104.mMaterials.size(); i++) {
            const char* matched = 0;
            if (model->mFC.fn_80473EFC() == 3) {
                const char* name = model->GetRes_unk()->m50[i];
                unsigned int hash = fn_800DC6B4(name);
                for (int j = 0; j < (int)model->mFC.fn_8047486C(); j++) {
                    if (hash == LoadedMaterial_unk(model->mFC, j)->m00) {
                        matched = name;
                        break;
                    }
                }
            }
            model->m104.mMaterials.mData[i]->fn_800816B4(folder, matched);
        }
    }
}

namespace atn {

// inferred: index of the material with the given name hash (the size is read once)
inline int FindMaterial_unk(const SimpleArray<Material_unk*>& mats, unsigned int hash) {
    int index = -1;
    unsigned int n = mats.mSize;
    for (unsigned int i = 0; i < n; i++) {
        if (hash == mats.mData[i]->m00) {
            index = i;
            break;
        }
    }
    return index;
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

void ModelBase::vf18(RenderManager* view) {
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
} // namespace atn

// inferred: bounding box of `box` transformed by `m` (its eight corners)
atn::AABB_unk fn_8008F5F8(const atn::AABB_unk& box, const atn::Matrix3x4& m) {
    float maxZ = box.mMax.z;
    float maxY = box.mMax.y;
    float maxX = box.mMax.x;
    float minX = box.mMin.x;
    float minY = box.mMin.y;
    float minZ = box.mMin.z;
    atn::Vector3 corners[8];
    corners[0] = atn::Vector3(maxX, maxY, maxZ);
    corners[1] = atn::Vector3(minX, maxY, maxZ);
    corners[2] = atn::Vector3(maxX, minY, maxZ);
    corners[3] = atn::Vector3(minX, minY, maxZ);
    corners[4] = atn::Vector3(maxX, maxY, minZ);
    corners[5] = atn::Vector3(minX, maxY, minZ);
    corners[6] = atn::Vector3(maxX, minY, minZ);
    corners[7] = atn::Vector3(minX, minY, minZ);
    PSMTXMultVecArray(m.m, corners, corners, 8);
    atn::Vector3 max(corners[0]);
    atn::Vector3 min(corners[0]);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[1].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[2].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[3].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[4].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[5].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[6].x);
    __VecMinMax(&min.x, &max.x, &min.x, &max.x, &corners[7].x);
    return atn::AABB_unk(min, max);
}

namespace atn {

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
                             boost::function<void()> callback, const unsigned int* extra, int extraCount)
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

// inferred: skin matrices of the model for the world matrix m; the node matrices are multiplied in the
// locked cache when they fit in it (0x1800 bytes = 0x80 nodes)
// NONMATCHING: 97.5% - the original reads m50 through the pointer returned by GetMtx_unk (0x48 from the
// block at 0x08): the matrix block probably extends to m5C
void ModelBase::fn_80091684(const Mtx m, const int* remap) {
    Mtx_unk* mtx = GetMtx_unk();
    m60 = *(const Matrix3x4*)m;
    m5C = m50;
    ModelLod_unk* lod = &mLods[m50];
    if (mFlag4_unk && mFlag5_unk && fn_8008B964()->GetPass_unk() == 3) {
        m5C = -1;
        lod = &m1C4;
    }
    bool cached = mtx->mNodes.size() * sizeof(Matrix3x4) <= 0x1800;
    ResMtx_unk* block = ((Res_unk*)lod->m08)->m38;
    Matrix3x4* nodes = mtx->mNodes.GetData_unk();
    Matrix3x4* inv = fn_80476130(&lod->mHandle);
    Matrix3x4* skin = (Matrix3x4*)m58;
    DCZeroRange(skin, block->m10 * sizeof(Matrix3x4));
    if (cached) {
        if (remap != 0) {
            fn_8009A504((Mtx*)LOCKED_CACHE_UNK, (const Mtx*)inv, (const Mtx*)nodes, remap, ((Res_unk*)lod->m08)->m44, m60.m);
            for (int i = 0; i < block->mRigidCount; i++) {
                *skin = MulMtx_unk(nodes[remap[block->mRigid[i].mNode]], m60);
                skin++;
            }
        } else {
            fn_8009A490((Mtx*)LOCKED_CACHE_UNK, (const Mtx*)inv, (const Mtx*)nodes, mtx->mNodes.size(), m60.m);
            for (int i = 0; i < block->mRigidCount; i++) {
                *skin = MulMtx_unk(nodes[block->mRigid[i].mNode], m60);
                skin++;
            }
        }
        if (block->mSkin2Count != 0 || block->mSkin3Count != 0 || block->mSkin4Count != 0) {
            fn_8009A0F8(skin, LOCKED_CACHE_UNK, block->mSkin2, block->mSkin2Count);
            skin += block->mSkin2Count;
            fn_8009A2C0(&skin, LOCKED_CACHE_UNK, block->mSkin3, block->mSkin3Count);
            skin += block->mSkin3Count;
            fn_800910D4(&skin, LOCKED_CACHE_UNK, block->mSkin4, block->mSkin4Count);
        }
    } else {
        if (remap != 0) {
            fn_80090834(LOCKED_CACHE_UNK, inv, nodes, remap, ((Res_unk*)lod->m08)->m44, m60);
            for (int i = 0; i < block->mRigidCount; i++) {
                *skin = MulMtx_unk(nodes[remap[block->mRigid[i].mNode]], m60);
                skin++;
            }
        } else {
            fn_800908FC(LOCKED_CACHE_UNK, inv, nodes, mtx->mNodes.size(), m60);
            for (int i = 0; i < block->mRigidCount; i++) {
                *skin = MulMtx_unk(nodes[block->mRigid[i].mNode], m60);
                skin++;
            }
        }
        if (block->mSkin2Count != 0 || block->mSkin3Count != 0 || block->mSkin4Count != 0) {
            fn_800909B0(skin, LOCKED_CACHE_UNK, block->mSkin2, block->mSkin2Count);
            skin += block->mSkin2Count;
            fn_80090C58(&skin, LOCKED_CACHE_UNK, block->mSkin3, block->mSkin3Count);
            skin += block->mSkin3Count;
            fn_800910D4(&skin, LOCKED_CACHE_UNK, block->mSkin4, block->mSkin4Count);
        }
    }
}

// NONMATCHING: 99.6% - the store order of the copy into m60
void ModelBase::fn_80091A90(const Mtx m) {
    if (mFlag4_unk && mFlag5_unk && atn::RenderManager::instance_->m2F8 == 3) {
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

// inferred: index of the material with the given name hash (inline of fn_8009290C)
inline int FindMaterialIndex_unk(Model* model, unsigned int hash) {
    for (unsigned int i = 0; i < model->m104.mMaterials.mSize; i++) {
        if (hash == model->m104.mMaterials.mData[i]->m00) {
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

// inferred: index of the node edit of a node (the size is read once)
inline int FindEdit_unk(const SimpleVector<Model::NodeEdit_unk>& v, unsigned int hash) {
    int index = -1;
    unsigned int n = v.mSize;
    for (unsigned int i = 0; i < n; i++) {
        if (hash == v.mData[i].m00) {
            index = i;
            break;
        }
    }
    return index;
}

// inferred: removes an element, moving the next ones down (name invented)
inline void EraseEdit_unk(SimpleVector<Model::NodeEdit_unk>& v, Model::NodeEdit_unk* it) {
    if (it != v.mData + v.mSize) {
        for (unsigned int k = it - v.mData; k < v.mSize - 1; k++) {
            v.mData[k] = v.mData[k + 1];
        }
        v.mSize = v.mSize - 1;
    }
}

// inferred: float colour written to a material as bytes (name invented)
struct ModelColor_unk {
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

// inferred: appends a bone to the fixed array (name invented)
inline void PushBone_unk(Model* model, PBoneInterface* bone) {
    new (&model->m388[model->m384]) PBoneInterface*(bone);
    model->m384++;
}

// NONMATCHING: 83% - the copy of dir is scheduled before the arguments
// inferred: node pose of the shared pose buffer (implicit constructor 0x8008A4B0); name invented
struct NodePose_unk : public MotionPose_unk {};

// inferred: pose buffer shared by every model (0x400 nodes, allocated by the first model)
static NodePose_unk* sNodePoses;
} // namespace atn

// inferred: out[i] = root * b[i] * a[i]
// NONMATCHING: 97.6% - register allocation of the pointers
extern "C" void DCZeroRange(void* addr, unsigned long size);
extern "C" asm void fn_80099F74(register const SkinWeight2_unk* w, register const atn::Matrix3x4* mats, register atn::Matrix3x4* out, register int n);
extern "C" asm void fn_8009A018(register const SkinWeight3_unk* w, register const atn::Matrix3x4* mats, register atn::Matrix3x4* out, register int n);

// inferred: skinning weights of a vertex block with three joints (0x1C bytes); names invented
struct SkinWeight3_unk {
    int m00;
    int mIndex0;
    float mWeight0;
    int mIndex1;
    float mWeight1;
    int mIndex2;
    float mWeight2;
};

// inferred: double-buffered transfer of an array to the locked cache, 32 entries a block (buffers at
// 0xE0001800 and right after the first one); names invented
template <class T>
struct LCStream_unk {
    LCStream_unk(const T* src, int count) {
        mSrc = src;
        mCount = count;
        mIndex = 0;
        mBuf[0] = (T*)0xE0001800;
        mBuf[1] = (T*)(0xE0001800 + 32 * sizeof(T) + 32);
    }
    unsigned long Load_unk(int count) {
        mOffset[mIndex] = (unsigned long)mSrc & 31;
        unsigned long n = LCLoadData(mBuf[mIndex], (char*)mSrc - mOffset[mIndex], count * sizeof(T));
        mSrc += count;
        mCount -= count;
        return n;
    }

    const T* mSrc;
    int mCount;
    int mIndex;
    T* mBuf[2];
    int mOffset[2];
};
extern "C" void fn_80099010(MotionPose_unk* pose, const ModelJoint_unk* joint);

// inferred: quicksort of the engine for the node lists (selection sort up to 20 elements, pivot from the
// median of two positions moved by a rotating offset and the last element); names invented
int lbl_8087D7A8 = -4;

template <class T>
inline void Swap_unk(T& a, T& b) {
    T t = a;
    a = b;
    b = t;
}

struct LessU_unk {
    bool operator()(const unsigned int& a, const unsigned int& b) const { return a < b; }
};

template <class T, class C>
void Median3_unk(T* a, T* b, T* c, C comp) {
    bool ca = !comp(*c, *a);
    bool bc = !comp(*b, *c);
    if (ca && bc) {
        return;
    }
    if (!ca && !bc) {
        Swap_unk(*a, *b);
        return;
    }
    if (comp(*b, *a)) {
        Swap_unk(*a, *b);
    }
    if (ca) {
        Swap_unk(*b, *c);
    } else {
        Swap_unk(*a, *c);
    }
}

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
void Sort_unk(T* first, T* last, Compare comp) {
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
        T* a = first + (n / 4 + lbl_8087D7A8 % 5);
        if (++lbl_8087D7A8 >= 5) {
            lbl_8087D7A8 = -4;
        }
        int n3 = n * 3 / 4;
        T* b = first + (n3 + lbl_8087D7A8 % 5);
        if (++lbl_8087D7A8 >= 5) {
            lbl_8087D7A8 = -4;
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
            Sort_unk<T, Compare>(first, i, comp);
            first = i;
        } else {
            Sort_unk<T, Compare>(i, last, comp);
            last = i;
        }
    }
}


namespace atn {

// NONMATCHING: 98% - same as fn_800920B8 (inlined)
void Model::fn_80091CFC(const char* node) {
    fn_800920B8(fn_800DC6B4(node));
}

// inferred: adds a node to the sorted list of the nodes with the node callback enabled
// NONMATCHING: 98% - registers of the inlined reserve copy loops
void Model::fn_800920B8(unsigned int hash) {
    int node = FindNodeRes_unk(GetRes_unk(), hash);
    if (node < 0) {
        return;
    }
    for (unsigned int i = 0; i < mCC.size(); i++) {
        if (node == mCC[i]) {
            return;
        }
    }
    mCC.push_back(node);
    Sort_unk(mCC.mData, mCC.mData + mCC.mSize, LessU_unk());
}

// RSO: GetNodeNoByName__Q23atn9ModelBaseCFPCci
int ModelBase::GetNodeNoByName(const char* name, int lod) const {
    return FindNode_unk(this, fn_800DC6B4(name), lod);
}

int Model::fn_800928B0(unsigned int hash, int lod) {
    return FindNode_unk(this, hash, lod);
}

int Model::fn_8009290C(unsigned int hash) {
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        if (hash == m104.mMaterials.mData[i]->m00) {
            return i;
        }
    }
    return -1;
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

void ModelBase::fn_80092EC8(const char* path) {
    mFlag4_unk = 1;
    mFlag5_unk = 1;
    m1C4.mHandle.vf0C(path);
    m1C4.m0C = 0.0f;
    m1C4.m08 = 0;
}

void ModelBase::fn_80092F1C(const char* path, const char* node, float distance) {
    int n = m20C_23_16;
    m20C_23_16 = n + 1;
    ModelLod_unk* lod = &mLods[n];
    lod->mHandle.vf0C(path);
    lod->m0C = distance;
    lod->m08 = 0;
}

void ModelBase::fn_80092F90() {
    ModelLod_unk* lod = &mLods[1];
    for (int i = 1; i < m20C_23_16; lod++, i++) {
        lod->mHandle.fn_80473F88();
        lod->m08 = 0;
    }
    m20C_23_16 = 1;
}

void Model::fn_80093014(int on) {
    mFlag1_unk = on;
    if (mFlag1_unk) {
        m20C_31_24 = 5;
    }
}

// NONMATCHING: 99.6% - register of the level pointer of the second loop (r30 in the original)
void ModelBase::fn_8009303C() {
    for (int j = mFlag1_unk != 0; j < m20C_23_16; j++) {
        mLods[j].m08 = mLods[j].mHandle.fn_80475DF8();
    }
    int i = mFlag1_unk != 0;
    for (ModelLod_unk* lod = &mLods[i]; i < m20C_23_16; lod++, i++) {
        if (lod->m08 != 0) {
            fn_80093158(lod);
        }
    }
    if (mFlag5_unk && !mFlag1_unk) {
        m1C4.m08 = m1C4.mHandle.fn_80475DF8();
        fn_80093158(&m1C4);
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

// NONMATCHING: 96.6% - the inlined node search reloads the joint array (the original reuses it)
void Model::fn_8009385C(unsigned int hash, bool show, int children, int siblings) {
    fn_80093A1C(fn_800928B0(hash, 0), show, children, siblings);
}

// NONMATCHING: 95.8% - same as fn_8009385C (the recursion is inlined twice, as in the original)
void Model::fn_80093A1C(int node, bool show, int children, int siblings) {
    while (node >= 0) {
        ModelJoint_unk* joint = ((Res_unk*)mLods[0].m08)->m48[node];
        fn_800937E8(joint->m14, show);
        if (children && joint->m20 >= 0) {
            fn_80093A1C(joint->m20, show, 1, 1);
        }
        if (!siblings) {
            break;
        }
        node = joint->m24;
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

// inferred: sets the node edit of a node: replaces the one with the same node name or adds it and points
// the node at it
void Model::fn_80093F58(const void* p) {
    const NodeEdit_unk* edit = (const NodeEdit_unk*)p;
    for (unsigned int i = 0; i < mD8.size(); i++) {
        NodeEdit_unk* e = &mD8[i];
        if (edit->m00 == e->m00) {
            *e = *edit;
            return;
        }
    }
    mD8.push_back(*edit);
    Res_unk* res = GetRes_unk();
    for (int j = 0; j < res->m44; j++) {
        if (edit->m00 == res->m48[j]->m14) {
            mE4.mData[j] = mD8.mSize - 1;
            return;
        }
    }
}

// inferred: replaces the node edits and rebuilds the node -> edit table
// NONMATCHING: 98.1% - registers of the -1 fill loop and of the copy in the inlined Assign_unk
void Model::fn_80094420(const SimpleVector<NodeEdit_unk>& edits) {
    mD8.clear();
    for (int i = 0; i < GetRes_unk()->m44; i++) {
        mE4.mData[i] = -1;
    }
    mD8 = edits;
    for (unsigned int i = 0; i < mD8.mSize; i++) {
        Res_unk* res = GetRes_unk();
        for (int j = 0; j < res->m44; j++) {
            if (res->m48[j]->m14 == mD8.mData[i].m00) {
                mE4.mData[j] = i;
                break;
            }
        }
    }
}

// NONMATCHING: 97.7% - registers of the search and of the index fix-up loop
void Model::fn_80094778(const char* name) {
    int index = FindEdit_unk(mD8, fn_800DC6B4(name));
    if (index >= 0) {
        EraseEdit_unk(mD8, &mD8.mData[index]);
        for (int i = 0; i < ((Res_unk*)mLods[0].m08)->m44; i++) {
            short v = mE4.mData[i];
            if (index == v) {
                mE4.mData[i] = -1;
            } else if (v > index) {
                mE4.mData[i] = v - 1;
            }
        }
    }
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

void Model::fn_80094958(const char* name, const void* a, const void* b, const void* c) {
    unsigned int hash = fn_800DC6B4(name);
    for (unsigned int i = 0; i < m104.mMaterials.size(); i++) {
        Material_unk* m = m104.mMaterials.mData[i];
        if (m->m00 == hash) {
            m->mColor = ((const ModelColor_unk*)a)->ToGX();
            m->m20 = ((const ModelColor_unk*)b)->ToGX();
            m->m1C = ((const ModelColor_unk*)c)->ToGX();
        }
    }
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

void Model::fn_80094BD8(int mode) {
    if (mode == 4) {
        unsigned int n = m104.mMaterials.mSize;
        for (unsigned int i = 0; i < n; i++) {
            Material_unk* m = m104.mMaterials.mData[i];
            if ((int)m->m14_0 == 1) {
                m->fn_8007FAF0(7);
                m->m48 |= 2;
            } else if ((int)m->m14_12 == 1) {
                m->m44 = -1;
            } else {
                m->fn_8007FAF0(mode);
                m->m48 |= 2;
            }
        }
    } else if (mode == 3) {
        unsigned int n = m104.mMaterials.mSize;
        for (unsigned int i = 0; i < n; i++) {
            Material_unk* m = m104.mMaterials.mData[i];
            if ((int)m->m14_0 == 1) {
                m->fn_8007FAF0(8);
                m->m48 = m->m48 & 0xFFFFFFFD;
            }
        }
    } else if (mode == 1) {
        unsigned int n = m104.mMaterials.mSize;
        for (unsigned int i = 0; i < n; i++) {
            Material_unk* m = m104.mMaterials.mData[i];
            if ((int)m->m14_0 == 1) {
                m->fn_8007FAF0(8);
                m->m48 = m->m48 & 0xFFFFFFFD;
            } else if ((m->m48 & ~2) == 0) {
                m->m44 = -1;
            } else {
                m->fn_8007FAF0(1);
                m->m48 = m->m48 & 0xFFFFFFFD;
            }
        }
    }
    fn_8008C850(&m104);
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

// inferred: over the inline budget (R3) as the original: every accessor, matrix product and box
// assignment is called out of line (weak copies 0x8008CD50, 0x8008E2E0, 0x80091C00, 0x800952F8...)
AABB_unk Model::fn_80095300() {
    AABB_unk box;
    box.mMin = fn_8000DD04()->GetMin_unk();
    box.mMax = fn_8000DD04()->GetMax_unk();
    int n;
    Mtx_unk* mtx = GetMtx_unk();
    Matrix3x4 world(mtx->m);
    if (mB0 >= 0) {
        Matrix3x4* inv = fn_80476130(&mLods[0].mHandle);
        if (mB4 >= 0) {
            n = mB0;
            Matrix3x4 m0 = MulMtx_unk(inv[n], mtx->mNodes[n]);
            AABB_unk a = fn_80070B60(&box, mAC);
            a = fn_8008F5F8(a, m0);
            n = mB4;
            Matrix3x4 m1 = MulMtx_unk(inv[n], mtx->mNodes[n]);
            AABB_unk b = fn_80070B60(&box, mAC);
            b = fn_8008F5F8(b, m1);
            box = a.Merge_unk(b);
            return box;
        }
        n = mB0;
        world = MulMtx_unk(inv[n], mtx->mNodes[n]);
    }
    box = fn_80070B60(&box, mAC);
    box = fn_8008F5F8(box, world);
    return box;
}

// inferred: sets an external texture with a UV scale and offset on the materials that take one
void Model::fn_800954DC(void* tex, Vector2_unk uv, float scale) {
    if (tex == 0) {
        return;
    }
    unsigned int n = m104.mMaterials.mSize;
    for (unsigned int i = 0; i < n; i++) {
        Material_unk* m = m104.mMaterials.mData[i];
        int stage = m104.m28.mData[i];
        if (stage > -1) {
            TexStage_unk* st = &m->m0C[stage];
            st->SetScaleU(scale);
            st->SetScaleV(scale);
            st->SetTransU(uv.x);
            st->SetTransV(uv.y);
            if (st->mOwnsTexture && st->mTexture != 0) {
                delete st->mTexture;
                st->mOwnsTexture = 0;
                st->mTexture = 0;
            }
            st->mTexture = (Texture_unk*)tex;
        }
    }
}

void ModelBase::fn_80095750(const char* name, ModelCompositeTex_unk* tex) {
    ModelComposite_unk* e = new ((MemoryTag)6, "", "", 0) ModelComposite_unk;
    e->mName.assign(name);
    e->m0C = fn_800DC6B4(name);
    e->m10 = tex;
    tex->m04.AddRef_unk();
    e->m14.mEntry = tex->m04.mEntry;
    m1E4.push_back(e);
}

Texture_unk* ModelBase::fn_80095B08(const std::string& name) {
    if (m1E4.mSize != 0) {
        unsigned int hash = fn_800DC6B4(name.c_str());
        for (unsigned int i = 0; i < m1E4.mSize; i++) {
            if (hash == m1E4[i]->m0C && name == m1E4[i]->mName) {
                return m1E4[i]->m14.fn_800D8808();
            }
        }
    }
    return 0;
}

void ModelBase::fn_80095CD0() {
    for (unsigned int i = 0; i < m104.mMaterials.mSize; i++) {
        if (m104.mMaterials.mData[i]->m4B != 0) {
            m104.mMaterials.mData[i]->fn_80080048(1.0f / 30.0f);
        }
    }
}

// inferred: bounds follow nodes `node` and `parent` (mB0/mB4); moves the bounding center to their midpoint
void Model::fn_80095D44(int node, int parent) {
    mB0 = node;
    mB4 = parent;
    if (node >= 0) {
        if (parent >= 0) {
            m9C -= (((const Matrix3x4*)GetNodeMtx_unk(node))->GetTrans_unk() +
                    ((const Matrix3x4*)GetNodeMtx_unk(parent))->GetTrans_unk()) *
                       0.5f -
                   mMtx.GetTrans_unk();
        } else {
            m9C -= ((const Matrix3x4*)GetNodeMtx_unk(node))->GetTrans_unk() - mMtx.GetTrans_unk();
        }
    }
}

// inferred: copies a camera into the model's own camera (if it has one) and flags it (0x80000). Declared
// extern "C" in atn/Camera_unk.h, the callers (game/CharaTask.cpp, gmk/AutomaticDoor.cpp, Event/DialogHead.cpp) use
// that name. The test is the boolean conversion of boost::scoped_ptr (__ptmf_test of &scoped_ptr::get).
// The copy goes through ModelCamera_unk (the implicit operator= of the parameter type copies in another order).
extern "C" void fn_80095F10(void* model, ModelCameraParam_unk* param) {
    ModelBase* base = (ModelBase*)model;
    if (base->m208) {
        *base->m208 = *(const ModelCamera_unk*)param;
        base->mFlags |= 0x80000;
    }
}

// NONMATCHING: 99.3% - the "" of the pool is at 0xA in the original, and the motion slots are built with
// the constructor of LH_Motion (here the implicit one of MotionSlot_unk)
Model::Model(int flags, int motions)
    : ModelBase(flags), m34C(1), m350(1.0f), m384(0), m3A8(0.0f, 0.0f, 0.0f), m3B4(0.0f), m3B8(0.0f), m3BC(0),
      m3C0(-1), m3C4(0.0f) {
    m3C8[0] = 0;
    if (sNodePoses == 0) {
        sNodePoses = new ((MemoryTag)6, "", "", 0) NodePose_unk[0x400];
    }
    for (int i = 0; i < 6; i++) {
        mTracks[i].m00 = -1;
        mTracks[i].m08 = 0.0f;
        mTracks[i].m0C = 1.0f;
        mTracks[i].m18 = 1;
        mTracks[i].m10 = 0.0f;
        mTracks[i].m14 = 0.1f;
        mTracks[i].m1C = 1.0f;
        mTracks[i].m20 = 1.0f;
        mTracks[i].m04 = 0;
        mTracks[i].m24 = 0;
        mTracks[i].m28 = 0.0f;
        mTracks[i].m2C = 0.0f;
    }
    if ((mFlags & 1) != 1) {
        m224.Resize_unk(motions, 11);
    }
}

Model::~Model() {
    for (unsigned int i = 0; i < m384; i++) {
        delete m388[i];
    }
}

void Model::fn_800973C0(CsvTab* csv) {
    if (m384 < 8) {
        int type = fn_800DC12C(csv->fn_8005B9CC());
        if (type == 0) {
            PBoneInterface* bone = new ((MemoryTag)6, "", "", 0) PBoneBasic;
            bone->vf1C((PBoneModel_unk*)this, csv);
            PushBone_unk(this, bone);
        } else if (type == 1) {
            PBoneInterface* bone = new ((MemoryTag)6, "", "", 0) PBoneOutForce;
            bone->vf1C((PBoneModel_unk*)this, csv);
            PushBone_unk(this, bone);
        }
    }
}

void Model::fn_80097510() {
    for (unsigned int i = 0; i < m384; i++) {
        m388[i]->vf0C();
    }
}

// inferred: pushes the physics bones in the wind direction (random strength and angle) and updates them
void Model::fn_8009757C() {
    for (unsigned int i = 0; i < m384; i++) {
        if (m3BC > 0) {
            Vector3 dir(m3A8);
            float b;
            float a = m3B4;
            b = m3B8;
            dir *= (a - b) * (rand() / 32767.0f) + b;
            Mtx rot;
            PSMTXRotRad(rot, 'y', 0.08726646f * ((rand() % 1001) / 1000.0f));
            PSMTXMultVec(rot, &dir, &dir);
            m388[i]->vf28(dir);
        }
        m388[i]->vf10();
    }
    m3BC--;
}

void Model::fn_80097750(atn::Vector3& dir, float a, float b, int c) {
    atn::Vector3 n(dir);
    n.Normalize_unk();
    m3A8 = n;
    m3B4 = a;
    m3B8 = b;
    m3BC = c;
}

// inferred: rebuilds the per-node arrays from the loaded data and the motion commands of the slots
// NONMATCHING: 99.2% - loop counter allocated after the two offsets (r28 instead of r30)
void Model::vf24() {
    Res_unk* res = (Res_unk*)mLods[0].m08;
    if (res == 0) {
        return;
    }
    if ((mFlags & 1) != 1) {
        m214.Resize_unk(res->m44, 0xb);
        m21C.Resize_unk(((Res_unk*)mLods[0].m08)->m44, 0xb);
    }
    if ((mFlags & 1) != 1) {
        m354.Resize_unk(m224.mSize, 6);
        for (unsigned int i = 0; i < m224.size(); i++) {
            fn_800A0A68(&m354.mData[i], this, &m224.mData[i].mMotion);
        }
    }
}

void Model::fn_80097A20(unsigned int motion) {
    LH_Motion* slot = &m224.mData[motion].GetMotion_unk();
    slot->fn_80473F88();
    slot->m08 = 0;
    fn_800A1060(&m354.mData[motion]);
}

void Model::fn_80097A88(int motion, const char* path) {
    m224.mData[motion].GetMotion_unk().fn_800A03E4(path);
}

void Model::fn_80097A9C(int motion, const LH_Motion* src) {
    if ((int)m224.mSize <= motion || motion < 0) {
        return;
    }
    LH_Motion* slot = &m224.mData[motion].GetMotion_unk();
    *slot = *src;
    slot->m08 = src->m08;
    fn_800A0A68(&m354.mData[motion], this, src);
}

int Model::fn_80097B34() {
    int n = 0;
    for (int i = 0; i < (int)m224.mSize; i++) {
        if (m224[i].GetMotion_unk().m08 != 0) {
            n++;
        }
    }
    return n;
}

int Model::vf20() {
    int busy = 0;
    for (int i = 0; i < (int)m224.mSize; i++) {
        if (m224[i].GetMotion_unk().fn_800A0448()) {
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
        if (motion >= 0 && motion < (int)m224.mSize && m224.mData[motion].GetMotion_unk().m08 != 0) {
            t.m04 = &m224.mData[motion].GetMotion_unk();
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
    LH_Motion* slot = &m224.mData[motion].GetMotion_unk();
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

// NONMATCHING: 97% - registers of the inlined node search
void Model::fn_80097D9C(Model* src) {
    for (int i = 0; i < ((Res_unk*)mLods[0].m08)->m44; i++) {
        int j = FindNodeRes_unk((Res_unk*)src->mLods[0].m08, ((Res_unk*)mLods[0].m08)->m48[i]->m14);
        if (j >= 0) {
            m21C.mData[i] = src->m21C.mData[j];
        }
    }
}

// inferred: advances the motion tracks without sampling them (models that are not drawn); track 3 is the
// fading one, tracks 4 and 5 only loop forwards
void Model::fn_80098698() {
    LH_Motion* motion;
    float speed = lbl_8087EFA8->m3A4;
    float dt = m350 * speed;
    for (int i = 0; i < m34C; i++) {
        Track_unk* t = &mTracks[i];
        motion = t->m04;
        if (motion != 0) {
            t->m10 += t->m14 * dt;
            if (t->m10 > t->m1C) {
                t->m10 = t->m1C;
            }
            t->m08 += t->m0C * dt;
            if (t->m18 && t->m08 >= fn_800A08D4(motion) && t->m0C > 0.0f) {
                t->m08 -= fn_800A08D4(motion);
            }
            if (t->m18 && t->m08 < 0.0f && t->m0C < 0.0f) {
                t->m08 += fn_800A08D4(motion);
            }
        }
    }
    if (mTracks[3].m04 != 0 || mTracks[3].m24 != 0) {
        LH_Motion* motion = (mTracks[3].m04 != 0) ? mTracks[3].m04 : mTracks[3].m24;
        if (mTracks[3].m04 == 0) {
            mTracks[3].m10 -= mTracks[3].m14 * dt;
            if (mTracks[3].m10 <= 0.0f) {
                mTracks[3].m10 = 0.0f;
                mTracks[3].m24 = 0;
                mTracks[3].m00 = -1;
            }
        } else {
            mTracks[3].m10 += mTracks[3].m14 * dt;
        }
        if (mTracks[3].m10 > mTracks[3].m1C) {
            mTracks[3].m10 = mTracks[3].m1C;
        }
        mTracks[3].m08 += mTracks[3].m0C * dt;
        if (mTracks[3].m18 && mTracks[3].m08 >= fn_800A08D4(motion)) {
            mTracks[3].m08 -= fn_800A08D4(motion);
        }
    }
    for (int i = 4; i <= 5; i++) {
        Track_unk* t = &mTracks[i];
        LH_Motion* motion = t->m04;
        if (motion != 0) {
            t->m10 += t->m14 * dt;
            if (t->m10 > t->m1C) {
                t->m10 = t->m1C;
            }
            t->m08 += t->m0C * dt;
            if (t->m18 && t->m08 >= fn_800A08D4(motion)) {
                t->m08 -= fn_800A08D4(motion);
            }
        }
    }
}
} // namespace atn

// inferred: blends n node poses: out = a * (1 - t) + b * t for the parts set in both (slerp for the
// rotation); a part set in only one of them is copied, a part set in none is reset
extern "C" void fn_80098948(MotionPose_unk* out, unsigned int n, const MotionPose_unk* a, const MotionPose_unk* b, float t) {
    float s = 1.0f - t;
    for (unsigned int i = 0; i < n; i++) {
        if (a->m00 & 1) {
            if (b->m00 & 1) {
                if (atn::Abs_unk(t) < 1.1920929e-7f) {
                    out->mPos = a->mPos;
                } else if (1.0f == t) {
                    out->mPos = b->mPos;
                } else {
                    out->mPos.x = a->mPos.x * s + b->mPos.x * t;
                    out->mPos.y = a->mPos.y * s + b->mPos.y * t;
                    out->mPos.z = a->mPos.z * s + b->mPos.z * t;
                }
            } else {
                out->mPos = a->mPos;
            }
        } else if (b->m00 & 1) {
            out->mPos = b->mPos;
        } else {
            out->mPos.x = 0.0f;
            out->mPos.y = 0.0f;
            out->mPos.z = 0.0f;
        }
        if (a->m00 & 2) {
            if (b->m00 & 2) {
                if (atn::Abs_unk(t) < 1.1920929e-7f) {
                    out->m10 = a->m10;
                } else if (1.0f == t) {
                    out->m10 = b->m10;
                } else {
                    atn::Quaternion_unk q;
                    C_QUATSlerp(&a->m10, &b->m10, &q, t);
                    out->m10 = q;
                }
            } else {
                out->m10 = a->m10;
            }
        } else if (b->m00 & 2) {
            out->m10 = b->m10;
        } else {
            out->m10.x = 0.0f;
            out->m10.y = 0.0f;
            out->m10.z = 0.0f;
            out->m10.w = 1.0f;
        }
        if (a->m00 & 4) {
            if (b->m00 & 4) {
                if (atn::Abs_unk(t) < 1.1920929e-7f) {
                    out->mScale = a->mScale;
                } else if (1.0f == t) {
                    out->mScale = b->mScale;
                } else {
                    out->mScale.x = a->mScale.x * s + b->mScale.x * t;
                    out->mScale.y = a->mScale.y * s + b->mScale.y * t;
                    out->mScale.z = a->mScale.z * s + b->mScale.z * t;
                }
            } else {
                out->mScale = a->mScale;
            }
        } else if (b->m00 & 4) {
            out->mScale = b->mScale;
        } else {
            out->mScale.x = 1.0f;
            out->mScale.y = 1.0f;
            out->mScale.z = 1.0f;
        }
        out->m00 = a->m00 | b->m00;
        a++;
        b++;
        out++;
    }
}

namespace atn {

// inferred: samples the motion of a track on the node and its children (and its next siblings when
// `siblings`); without `replace` the sample is blended into the current pose with the track weight
void Model::fn_80098C90(int node, int siblings, int replace, Res_unk* res, Track_unk* track, const int* remap) {
    MotionPose_unk pose;
    while (node >= 0) {
        NodeRes_unk* joint = res->m48[node];
        int index = (remap != 0) ? remap[joint->m18] : joint->m18;
        int key = m354.mData[track->m00].mData[index];
        if (key >= 0) {
            if (replace) {
                fn_800A05FC(track->m04, &m21C.mData[index], key, &m214.mData[index], track->m08);
            } else {
                fn_800A05FC(track->m04, &pose, key, &m214.mData[index], track->m08);
                fn_80098948(&m21C.mData[index], 1, &m21C.mData[index], &pose, track->m10);
            }
        }
        if (joint->m20 >= 0) {
            fn_80098C90(joint->m20, 1, replace, res, track, remap);
        }
        if (!siblings) {
            break;
        }
        node = joint->m24;
    }
}

// inferred: samples the fading track 3 from the "spine" node (an upper body motion); while it fades out
// the previous motion is sampled
// NONMATCHING: 99.5% - the remap pointer is kept in r27 instead of r24, and "spine" is at 0x5E of the pool
// instead of 0x26C
void Model::fn_80098DD8(int advance) {
    Track_unk* t = &mTracks[3];
    Res_unk* res;
    LH_Motion* motion;
    bool fading;
    const int* remap;
    if (t->m04 == 0 && t->m24 == 0) {
        return;
    }
    res = (Res_unk*)mLods[m50].m08;
    remap = mLods[m50].m10.mData;
    motion = (t->m04 != 0) ? t->m04 : t->m24;
    float speed = lbl_8087EFA8->m3A4;
    float dt = m350 * speed;
    fading = false;
    if (t->m04 == 0) {
        if (advance) {
            t->m10 -= t->m14;
        }
        if (t->m10 <= 0.0f) {
            t->m10 = 0.0f;
            t->m24 = 0;
            t->m00 = -1;
            return;
        }
        fading = true;
    } else if (advance) {
        t->m10 += t->m14;
    }
    if (t->m10 > t->m1C) {
        t->m10 = t->m1C;
    }
    if (fading) {
        t->m04 = t->m24;
    }
    int node = FindNode_unk(this, fn_800DC6B4("spine"), m50);
    fn_80098C90(node, 0, Abs_unk(t->m10 - 1.0f) < 1.1920929e-7f, res, t, remap);
    if (fading) {
        t->m04 = 0;
    }
    if (advance) {
        t->m08 += t->m0C * dt;
    }
    if (t->m18 && t->m08 >= fn_800A08D4(motion)) {
        t->m08 -= fn_800A08D4(motion);
    }
}
} // namespace atn

// inferred: completes a pose with the bind pose of the joint (the parts the motion did not set)
extern "C" void fn_80099010(MotionPose_unk* pose, const ModelJoint_unk* joint) {
    if ((pose->m00 & 1) != 1) {
        pose->mPos = joint->GetTrans_unk();
        pose->m00 |= 1;
    }
    if ((pose->m00 & 2) != 2) {
        pose->m10 = EulerToQuat_unk(joint->GetRot_unk());
        pose->m00 |= 2;
    }
    if ((pose->m00 & 4) != 4) {
        pose->mScale = joint->GetScale_unk();
        pose->m00 |= 4;
    }
}

namespace atn {

void Model::fn_80099E9C(const char* node, const char* name, float value) {
    m3C0 = FindNode_unk(this, fn_800DC6B4(node), 0);
    m3C4 = value;
    if (name != m3C8) {
        memcpy(m3C8, name, strlen(name) + 1);
    }
}

int ModelBase::GetRenderType_unk() {
    return 0;
}
} // namespace atn

// inferred: skin matrices of the two-joint weights: out[i] = mats[w.mIndex0] * w.mWeight0 +
// mats[w.mIndex1] * w.mWeight1 (paired singles on the locked cache; asm approved for this function)
extern "C" asm void fn_80099F74(register const SkinWeight2_unk* w, register const atn::Matrix3x4* mats,
                     register atn::Matrix3x4* out, register int n) {
    nofralloc
    mtctr r6
    subi r5, r5, 0x8
L_80099F7C:
    lwz r7, 0x4(r3)
    lwz r8, 0xc(r3)
    lfs f0, 0x8(r3)
    lfs f1, 0x10(r3)
    mulli r7, r7, 0x30
    mulli r8, r8, 0x30
    psq_lux f2, r7, r4, 0, 0
    psq_lu f3, 0x8(r7), 0, 0
    psq_lu f4, 0x8(r7), 0, 0
    psq_lu f5, 0x8(r7), 0, 0
    psq_lu f6, 0x8(r7), 0, 0
    psq_lu f7, 0x8(r7), 0, 0
    psq_lux f8, r8, r4, 0, 0
    psq_lu f9, 0x8(r8), 0, 0
    psq_lu f10, 0x8(r8), 0, 0
    psq_lu f11, 0x8(r8), 0, 0
    psq_lu f12, 0x8(r8), 0, 0
    psq_lu f13, 0x8(r8), 0, 0
    ps_muls0 f2, f2, f0
    ps_muls0 f3, f3, f0
    ps_muls0 f4, f4, f0
    ps_muls0 f5, f5, f0
    ps_muls0 f6, f6, f0
    ps_muls0 f7, f7, f0
    ps_madds0 f2, f8, f1, f2
    ps_madds0 f3, f9, f1, f3
    ps_madds0 f4, f10, f1, f4
    ps_madds0 f5, f11, f1, f5
    ps_madds0 f6, f12, f1, f6
    ps_madds0 f7, f13, f1, f7
    psq_stu f2, 0x8(r5), 0, 0
    psq_stu f3, 0x8(r5), 0, 0
    psq_stu f4, 0x8(r5), 0, 0
    psq_stu f5, 0x8(r5), 0, 0
    psq_stu f6, 0x8(r5), 0, 0
    psq_stu f7, 0x8(r5), 0, 0
    addi r3, r3, 0x14
    bdnz L_80099F7C
    blr
}

// inferred: same with three joints (SkinWeight3_unk, 0x1C bytes; asm approved for this function)
extern "C" asm void fn_8009A018(register const SkinWeight3_unk* w, register const atn::Matrix3x4* mats,
                     register atn::Matrix3x4* out, register int n) {
    nofralloc
    mtctr r6
    subi r5, r5, 0x8
L_8009A020:
    lwz r7, 0x4(r3)
    lwz r8, 0xc(r3)
    lfs f0, 0x8(r3)
    lfs f1, 0x10(r3)
    mulli r7, r7, 0x30
    mulli r8, r8, 0x30
    psq_lux f2, r7, r4, 0, 0
    psq_lu f3, 0x8(r7), 0, 0
    psq_lu f4, 0x8(r7), 0, 0
    psq_lu f5, 0x8(r7), 0, 0
    psq_lu f6, 0x8(r7), 0, 0
    psq_lu f7, 0x8(r7), 0, 0
    psq_lux f8, r8, r4, 0, 0
    psq_lu f9, 0x8(r8), 0, 0
    psq_lu f10, 0x8(r8), 0, 0
    psq_lu f11, 0x8(r8), 0, 0
    psq_lu f12, 0x8(r8), 0, 0
    psq_lu f13, 0x8(r8), 0, 0
    ps_muls0 f2, f2, f0
    ps_muls0 f3, f3, f0
    ps_muls0 f4, f4, f0
    ps_muls0 f5, f5, f0
    ps_muls0 f6, f6, f0
    ps_muls0 f7, f7, f0
    ps_madds0 f2, f8, f1, f2
    ps_madds0 f3, f9, f1, f3
    ps_madds0 f4, f10, f1, f4
    ps_madds0 f5, f11, f1, f5
    ps_madds0 f6, f12, f1, f6
    ps_madds0 f7, f13, f1, f7
    lwz r8, 0x14(r3)
    lfs f1, 0x18(r3)
    mulli r8, r8, 0x30
    psq_lux f8, r8, r4, 0, 0
    psq_lu f9, 0x8(r8), 0, 0
    psq_lu f10, 0x8(r8), 0, 0
    psq_lu f11, 0x8(r8), 0, 0
    psq_lu f12, 0x8(r8), 0, 0
    psq_lu f13, 0x8(r8), 0, 0
    ps_madds0 f2, f8, f1, f2
    ps_madds0 f3, f9, f1, f3
    ps_madds0 f4, f10, f1, f4
    ps_madds0 f5, f11, f1, f5
    ps_madds0 f6, f12, f1, f6
    ps_madds0 f7, f13, f1, f7
    psq_stu f2, 0x8(r5), 0, 0
    psq_stu f3, 0x8(r5), 0, 0
    psq_stu f4, 0x8(r5), 0, 0
    psq_stu f5, 0x8(r5), 0, 0
    psq_stu f6, 0x8(r5), 0, 0
    psq_stu f7, 0x8(r5), 0, 0
    addi r3, r3, 0x1c
    bdnz L_8009A020
    blr
}

// inferred: skin matrices of the two-joint weights, streamed through the locked cache
// NONMATCHING: 98% - the original keeps the queue count in r27 and the block pointer in r28
extern "C" void fn_8009A0F8(atn::Matrix3x4* out, const atn::Matrix3x4* mats, const SkinWeight2_unk* w, int n) {
    if (n == 0) {
        return;
    }
    LCStream_unk<SkinWeight2_unk> stream(w, n);
    int first = (n >= 32) ? 32 : n;
    if (first != 0) {
        stream.Load_unk(first);
    }
    unsigned long pending = 0;
    do {
        int count = (n > 32) ? 32 : n;
        SkinWeight2_unk* cur = stream.mBuf[stream.mIndex] + stream.mOffset[stream.mIndex];
        stream.mIndex = (stream.mIndex + 1) % 2;
        int next = (stream.mCount >= 32) ? 32 : stream.mCount;
        unsigned long loads;
        if (next != 0) {
            loads = stream.Load_unk(next);
        } else {
            loads = 0;
        }
        LCQueueWait(pending + loads);
        fn_80099F74(cur, mats, out, count);
        pending = 0;
        out += count;
        n -= count;
    } while (n > 0);
}

// inferred: same with three joints; the output pointer is advanced
// NONMATCHING: 98% - same registers as fn_8009A0F8
extern "C" void fn_8009A2C0(atn::Matrix3x4** out, const atn::Matrix3x4* mats, const SkinWeight3_unk* w, int n) {
    if (n == 0) {
        return;
    }
    LCStream_unk<SkinWeight3_unk> stream(w, n);
    int first = (n >= 32) ? 32 : n;
    if (first != 0) {
        stream.Load_unk(first);
    }
    unsigned long pending = 0;
    do {
        int count = (n > 32) ? 32 : n;
        SkinWeight3_unk* cur = stream.mBuf[stream.mIndex] + stream.mOffset[stream.mIndex];
        stream.mIndex = (stream.mIndex + 1) % 2;
        int next = (stream.mCount >= 32) ? 32 : stream.mCount;
        unsigned long loads;
        if (next != 0) {
            loads = stream.Load_unk(next);
        } else {
            loads = 0;
        }
        LCQueueWait(pending + loads);
        fn_8009A018(cur, mats, *out, count);
        pending = 0;
        *out += count;
        n -= count;
    } while (n > 0);
}

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
