// atn::Model / atn::ModelBase (0x800899DC-0x8009A5CC). Written in the order of the .dol; the
// functions not written yet are missing from this file (the unit is not linked).
#include "atn/Model_unk.h"
#include "atn/Material_unk.h"
#include "atn/LH_Motion.h"

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
    edit->mHash = fn_800DC6B4(name);
    edit->m08 = offset;
    edit->m14 = rot;
    edit->m20 = scale;
    edit->mFlags = flags;
    if (!(atn::Abs_unk(offset.x) < 1.1920929e-7f && atn::Abs_unk(offset.y) < 1.1920929e-7f &&
          atn::Abs_unk(offset.z) < 1.1920929e-7f)) {
        edit->mFlags |= 1;
    }
    if (!(atn::Abs_unk(rot.x) < 1.1920929e-7f && atn::Abs_unk(rot.y) < 1.1920929e-7f &&
          atn::Abs_unk(rot.z) < 1.1920929e-7f)) {
        edit->mFlags |= 2;
    }
    if (!(atn::Abs_unk(scale.x - 1.0f) < 1.1920929e-7f && atn::Abs_unk(scale.y - 1.0f) < 1.1920929e-7f &&
          atn::Abs_unk(scale.z - 1.0f) < 1.1920929e-7f)) {
        edit->mFlags |= 4;
    }
}

namespace atn {

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
        if (hash == e->mHash) {
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

void Model::fn_80095CD0() {
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
