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

// inferred: releases the textures of every material (CharaManager::vf14 passes atn::Model::m104)
extern "C" void fn_80089AD4(atn::Model::MaterialList_unk* list) {
    for (unsigned int i = 0; i < list->mSize; i++) {
        for (int j = 0; j < list->mData[i]->m4A; j++) {
            if (list->mData[i]->m0C[j].mTexture != 0) {
                atn::fn_800D58A4(list->mData[i]->m0C[j].mTexture);
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
extern "C" int fn_80089B5C(atn::Model::MaterialList_unk* list) {
    for (unsigned int i = 0; i < list->mSize; i++) {
        for (int j = 0; j < list->mData[i]->m4A; j++) {
            atn::Texture_unk* tex = list->mData[i]->m0C[j].mTexture;
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
inline int FindNode_unk(const Model* model, unsigned int hash, int lod) {
    return FindNodeRes_unk(model->mLods[lod].mRes, hash);
}

// RSO: GetNodeNoByName__Q23atn9ModelBaseCFPCci; the members are the ones of atn::Model (the class
// layout lives in Model in include/atn/Model_unk.h)
int ModelBase::GetNodeNoByName(const char* name, int lod) const {
    return FindNode_unk((const Model*)this, fn_800DC6B4(name), lod);
}

int Model::fn_800928B0(unsigned int hash, int lod) {
    return FindNode_unk(this, hash, lod);
}

// inferred: index of the material with the given name hash (inline of fn_8009290C)
inline int FindMaterialIndex_unk(Model* model, unsigned int hash) {
    for (unsigned int i = 0; i < model->m104.mSize; i++) {
        if (hash == model->m104.mData[i]->m00) {
            return i;
        }
    }
    return -1;
}

int Model::fn_8009290C(unsigned int hash) {
    for (unsigned int i = 0; i < m104.mSize; i++) {
        if (hash == m104.mData[i]->m00) {
            return i;
        }
    }
    return -1;
}

// inferred: material by name hash (inline of fn_800929C0)
inline Material_unk* FindMaterial_unk(Model* model, unsigned int hash) {
    for (unsigned int i = 0; i < model->m104.mSize; i++) {
        Material_unk* m = model->m104.mData[i];
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
    for (unsigned int i = 0; i < m104.mSize; i++) {
        Material_unk* m = m104.mData[i];
        if (hash == m->m00) {
            return m;
        }
    }
    return 0;
}

// inferred: sets the alpha of every material
void Model::fn_80092A00(float alpha) {
    for (unsigned int i = 0; i < m104.mSize; i++) {
        m104.mData[i]->m3C = (int)(255.0f * alpha);
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
        mF0[i] = show != 0;
    }
}

void Model::fn_800937E8(unsigned int hash, int show) {
    int i = FindNode_unk(this, hash, 0);
    if (i >= 0) {
        mF0[i] = show != 0;
    }
}

void Model::fn_80093B88(const char* name) {
    int i = FindNode_unk(this, fn_800DC6B4(name), 0);
    if (i >= 0) {
        mF0[i] = 1;
    }
}

void Model::fn_80093C20(unsigned int hash) {
    int i = FindNode_unk(this, hash, 0);
    if (i >= 0) {
        mF0[i] = 1;
    }
}

int Model::fn_80093C8C(const char* name) {
    int i = FindNode_unk(this, fn_800DC6B4(name), 0);
    if (i >= 0) {
        return mF0[i];
    }
    return 1;
}

int Model::fn_80093D28(unsigned int hash) {
    int i = FindNode_unk(this, hash, 0);
    if (i >= 0) {
        return mF0[i];
    }
    return 1;
}

void Model::fn_80093D98(const char* name, int show) {
    int i = FindMaterialIndex_unk(this, fn_800DC6B4(name));
    if (i >= 0) {
        mF8[i] = show != 0;
    }
}

void Model::fn_80093E30(unsigned int hash, int show) {
    int i = FindMaterialIndex_unk(this, hash);
    if (i >= 0) {
        mF8[i] = show != 0;
    }
}

int Model::fn_80093E90(unsigned int hash) {
    int i = FindMaterialIndex_unk(this, hash);
    if (i >= 0) {
        return mF8[i];
    }
    return 0;
}

void Model::fn_80093EEC(int show) {
    unsigned char on = show != 0;
    for (unsigned int i = 0; i < m104.mSize; i++) {
        mF8[i] = on;
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
    for (unsigned int i = 0; i < mD8; i++) {
        NodeEdit_unk* e = &mE0[i];
        if (hash == e->mHash) {
            return e;
        }
    }
    return 0;
}

Material_unk* Model::fn_80094B6C(const char* name) {
    unsigned int hash = fn_800DC6B4(name);
    for (unsigned int i = 0; i < m104.mSize; i++) {
        Material_unk* m = m104.mData[i];
        if (m->m00 == hash) {
            return m;
        }
    }
    return 0;
}

void Model::fn_80094D88(int arg, int flag, int a, int b, int c) {
    unsigned int n = m104.mSize;
    for (unsigned int i = 0; i < n; i++) {
        Material_unk* m = m104.mData[i];
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
    unsigned int n = m104.mSize;
    for (unsigned int i = 0; i < n; i++) {
        m104.mData[i]->m34 = value;
    }
}

void Model::fn_80094F14(float u, float v) {
    for (unsigned int i = 0; i < m104.mSize; i++) {
        m104.mData[i]->fn_80081514(u, v);
    }
}

void Model::fn_80095CD0() {
    for (unsigned int i = 0; i < m104.mSize; i++) {
        if (m104.mData[i]->m4B != 0) {
            m104.mData[i]->fn_80080048(1.0f / 30.0f);
        }
    }
}

void Model::fn_80097A20(unsigned int motion) {
    LH_Motion* slot = &m228[motion];
    slot->fn_80473F88();
    slot->m08 = 0;
    fn_800A1060(&m358[motion]);
}

void Model::fn_80097A88(int motion, const char* path) {
    m228[motion].fn_800A03E4(path);
}

void Model::fn_80097A9C(int motion, const LH_Motion* src) {
    if ((int)mMotionCount <= motion || motion < 0) {
        return;
    }
    LH_Motion* slot = &m228[motion];
    *slot = *src;
    slot->m08 = src->m08;
    fn_800A0A68(&m358[motion], this, src);
}

int Model::fn_80097B34() {
    int n = 0;
    for (int i = 0; i < (int)mMotionCount; i++) {
        if (m228[i].m08 != 0) {
            n++;
        }
    }
    return n;
}

int Model::fn_80097B74() {
    int busy = 0;
    for (int i = 0; i < (int)mMotionCount; i++) {
        if (m228[i].fn_800A0448()) {
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
        if (motion >= 0 && motion < (int)mMotionCount && m228[motion].m08 != 0) {
            t.m04 = (int)&m228[motion];
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
    if (motion < 0 || motion >= (int)mMotionCount) {
        return 0;
    }
    LH_Motion* slot = &m228[motion];
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
