#include "game/ColliAttrManager.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/CharaTask.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int rand();

// inferred: row of the collision attribute table (names invented)
struct ColliAttrData_unk {
    char mSe[3][0x20]; // 0x00: sound names
    char mEffect[1];   // 0x60: effect file name
};
extern "C" ColliAttrData_unk* fn_8020B1E0(int id);
extern "C" int fn_8020B208(int kind); // inferred: sound index of a kind
extern "C" int fn_8020B27C(int kind); // inferred: effect index of a kind
// inferred: attribute settings of a map (names invented)
struct MapColliAttr_unk {
    unsigned char m00[0x6C];
    const char* mEfp; // 0x6C
    unsigned char m70[0x74 - 0x70];
    int mAttr[0x10];  // 0x74
};
extern "C" MapColliAttr_unk* fn_8021771C(int id, int arg1, int arg2);

// inferred: result of the ray test fn_8004ED34 (as in game/ChaserNpc.cpp)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 mPos; // 0x10: inferred: hit position
    unsigned char m1C[0x34 - 0x1C];
    int* m34;          // inferred: attribute of the hit face
    int m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" void* lbl_8087EE98;
extern "C" int fn_8004ED34(void* colli, CollResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" int fn_800928B0(Model_unk* model, int hash, int arg);   // inferred: node index by hash

// inferred: lens of a camera (as in game/CharaTask.cpp)
struct Lens_unk {
    unsigned char m000[0x158];
    float m158;
    float m15C;
    float m160;
};
struct CameraLens_unk {
    unsigned char m000[0x2FC];
    Lens_unk* m2FC;
};
struct Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
struct ScreenInfo_unk {
    unsigned char m00[0x2C];
    float mColor[4];
};
extern "C" ScreenInfo_unk* fn_800C122C(Camera_unk* camera, const Model_unk::Sphere_unk* sphere);

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 NodePos_unk(MtxPtr mtx) {
    return atn::Vector3(mtx[0][3], mtx[1][3], mtx[2][3]);
}

inline Lens_unk* CharaTask::GetLens_unk() {
    return (mMotion.m1DC == 0) ? ((CameraLens_unk*)lbl_8087EFB4)->m2FC : mMotion.m1DC;
}

struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
// inferred: float color built from an ARGB word (as in game/CharaTask.cpp)
struct ColorF_unk {
    ColorF_unk(unsigned int argb) {
        r = (1.0f / 255.0f) * (float)((argb >> 16) & 0xFF);
        g = (1.0f / 255.0f) * (float)((argb >> 8) & 0xFF);
        b = (1.0f / 255.0f) * (float)(argb & 0xFF);
        a = (1.0f / 255.0f) * (float)(argb >> 24);
    }
    float r, g, b, a;
};
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A680(EffectMgr_unk* mgr, ResLoader_unk* res, int a, int b, void* mtx, const atn::Vector3* pos,
                            const atn::Vector3* rot, const ColorF_unk& color, float scale, int e, int f, int g);
extern "C" void fn_80232B7C(void* owner, int arg);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_800CB688(atn::SoundRef* sound, int arg, float pitch); // inferred: sets the pitch
struct SoundCfg_unk {
    unsigned char m00[0x34CC];
    float m34CC; // inferred: pitch variation
};
extern "C" SoundCfg_unk* lbl_8087EFE8;

// inferred: converts a [0,1] float channel to 0..255 (as in game/ChaserNpc.cpp)
inline unsigned int ToU8_unk(float f) {
    return (f >= 1.0f) ? 255 : (f <= 0.0f) ? 0 : (unsigned int)(f * 255.0f + 0.5f);
}

// inferred: suffixes of the sound names (none, left, right)
static const char* const sSideSuffix[] = {"", "_L", "_R", 0};

ColliAttrManager* ColliAttrManager::sInstance;

ColliAttrManager* ColliAttrManager::fn_801839FC(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (12, "", "", 0) ColliAttrManager(parent);
    }
    return sInstance;
}

ColliAttrManager::~ColliAttrManager() {
    sInstance = 0;
    Release_unk();
    mEfp.fn_8023781C();
}

int ColliAttrManager::vf0C() {
    return fn_800D3FA4();
}

void ColliAttrManager::vf10() {
    if (mLoading) {
        int loading = 0;
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            if (mEntries.mData[i].mRes->fn_802376D0()) {
                loading = 1;
            }
        }
        if (mEfp.fn_80237874()) {
            loading = 1;
        }
        mLoading = loading;
    }
}

void ColliAttrManager::vf14() {}

// NONMATCHING: 0% (fnstat) - fn_80183FFC is inlined here (4 calls) but called in the original: our
// fn_80183FFC costs a few statements less than the original for the inlining budget (with ~5 more
// statements in it the calls appear and the rest of the function is as in the original)
void ColliAttrManager::fn_80183D00(int id, int arg1, int arg2) {
    if (id == -1) {
        Release_unk();
        mEntries.Resize_unk(0x21, 0);
        for (unsigned int i = 0; i < mEntries.mSize; i++) {
            fn_80183FFC(i, i);
        }
        mLoading = 1;
    } else {
        Release_unk();
        mEfp.fn_8023781C();
        mEntries.Resize_unk(0x10, 0);
        MapColliAttr_unk* attr = fn_8021771C(id, arg1, arg2);
        if (attr != 0) {
            for (unsigned int i = 0; i < mEntries.mSize; i++) {
                fn_80183FFC(i, attr->mAttr[i]);
            }
            const char* efp = attr->mEfp;
            if (strcmp("", efp) != 0) {
                mEfp.fn_8023780C(efp);
            }
        } else {
            fn_80183FFC(0, 0x12);
            for (unsigned int i = 1; i < 0x10; i++) {
                fn_80183FFC(i, 0);
            }
        }
        mLoading = 1;
    }
}

// NONMATCHING: 90.9% - register allocation: the original keeps `this` in r31 (here r26) and the
// other variables one register lower
void ColliAttrManager::fn_80183FFC(int index, int id) {
    ColliAttrData_unk* data = fn_8020B1E0(id);
    if (data != 0) {
        mEntries[index].mId = id;
        char path[0x20];
        sprintf(path, "/effect/%s", data->mEffect);
        if (data->mEffect[0] != 0) {
            mEntries[index].mRes->fn_80237654(path);
        }
        for (int i = 0; i < 3; i++) {
            mEntries[index].mSe[i] = data->mSe[i];
        }
    }
}

// inferred: sound of an attribute (name invented)
inline MotionCmdElemD_unk* ColliAttrManager::FindSe_unk(int id, int index) {
    for (unsigned int i = 0; i < mEntries.mSize; i++) {
        if (id == mEntries.mData[i].mId) {
            return &mEntries.mData[i].mSe[index];
        }
    }
    return 0;
}

ResLoader_unk* ColliAttrManager::fn_801840D8(int id, int index) {
    for (unsigned int i = 0; i < mEntries.mSize; i++) {
        if (id == mEntries.mData[i].mId) {
            return &mEntries.mData[i].mRes[index];
        }
    }
    return 0;
}

void ColliAttrManager::fn_8018412C(int kind, CharaTask* chara, int useNode, int node, const atn::Vector3* ofs) {
    fn_80184188(fn_8020B27C(kind), chara, useNode, node, ofs);
}

// inferred: plays an effect at a position (name invented)
inline void ColliAttrManager::PlayEffect_unk(ResLoader_unk* res, const atn::Vector3* pos, const atn::Vector3* rot,
                                             const ColorF_unk& color) {
    fn_80232B7C(this, 0);
    fn_8023A680(lbl_8087F3C0, res, -1, 0, 0, pos, rot, color, 1.0f, 0, -1, 1);
}

// NONMATCHING: 99.9% - the copy of the sum into `pos` uses r4/r5 the other way round
void ColliAttrManager::fn_80184188(int index, CharaTask* chara, int useNode, int node, const atn::Vector3* ofs) {
    atn::Vector3 pos;
    atn::Vector3 rot = chara->mRot;
    Camera_unk* camera = lbl_8087EFB4;
    Model_unk::Sphere_unk sphere = chara->mMotion.fn_80094F98();
    ScreenInfo_unk* info = fn_800C122C(camera, &sphere);
    unsigned int r = ToU8_unk(info->mColor[0]);
    unsigned int g = ToU8_unk(info->mColor[1]);
    unsigned int b = ToU8_unk(info->mColor[2]);
    unsigned int a = ToU8_unk(info->mColor[3]);
    unsigned int argb = (b | (g << 8)) | ((a << 24) | (r << 16));
    CollResult_unk result;
    int attr = fn_80184864(&result, chara, useNode, node);
    if (attr >= 0 && (unsigned int)(attr - 0x1C) > 1) {
        pos = result.mPos + *ofs;
        if ((unsigned int)(attr - 0x14) <= 1) {
            pos.y = -chara->GetLens_unk()->m160 * chara->GetLens_unk()->m158 + 0.01f;
        }
        if (attr == 0x1F) {
            pos.y += 1.0f;
        }
        if (mLoading == 0) {
            ResLoader_unk* res = fn_801840D8(attr, index);
            if (res != 0) {
                PlayEffect_unk(res, &pos, &rot, ColorF_unk(argb));
            }
        }
    }
}

void ColliAttrManager::fn_80184548(int kind, CharaTask* chara, int useNode, int node, const atn::Vector3* ofs,
                                   int side, float volume) {
    int index = fn_8020B208(kind);
    if (mEnabled == 0) {
        return;
    }
    CollResult_unk result;
    int attr = fn_80184864(&result, chara, useNode, node);
    if (attr < 0) {
        return;
    }
    atn::Vector3 hit = result.mPos + *ofs;
    atn::Vector3 pos = hit;
    if (mLoading != 0) {
        return;
    }
    MotionCmdElemD_unk* se = FindSe_unk(attr, index);
    if (se == 0) {
        se = FindSe_unk(0, index);
    }
    if (se == 0) {
        return;
    }
    const char* name = se->mStr;
    char buf[0x20];
    if (side != 0) {
        sprintf(buf, "%s%s", se->mStr, sSideSuffix[side]);
        name = buf;
    }
    atn::SoundRef sound = fn_800C344C(name, &pos, 0, -1, volume);
    SoundCfg_unk* cfg = lbl_8087EFE8;
    if (cfg != 0) {
        // inferred: written out (the inline Rand01_unk schedules the 0.5f load earlier)
        fn_800CB688(&sound, 0, 1.0f + (cfg->m34CC * ((float)(rand() % 1001) / 1000.0f) - 0.5f * cfg->m34CC));
    }
}

void ColliAttrManager::fn_801847C0(CharaTask* chara) {
    if (mLoading == 0 && chara != 0 && mEfp.mResource != 0) {
        Color4f_unk color;
        atn::Vector3 rot;
        atn::Vector3 pos;
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_8023A8B4(lbl_8087F3C0, &mEfp, &chara->mMotion, 0, pos, rot, color, -1, -1, 1, 1.0f);
    }
}

// NONMATCHING: 99.7% - the copy of the sum into `from` uses r3/r4 the other way round
int ColliAttrManager::fn_80184864(CollResult_unk* result, CharaTask* chara, int useNode, int node) {
    atn::Vector3 from;
    atn::Vector3 to;
    if (useNode) {
        Model_unk* model = &chara->mMotion;
        from = NodePos_unk(model->GetNodeMtx_unk(fn_800928B0(model, node, 0))) + atn::Vector3(0.0f, 5.0f, 0.0f);
    } else {
        from = chara->mPos + atn::Vector3(0.0f, 5.0f, 0.0f);
    }
    to = from + atn::Vector3(0.0f, -15.0f, 0.0f);
    if (mForceAttr >= 0) {
        result->mPos = from;
        result->mPos.y = 0.0f;
        return mForceAttr;
    }
    void* colli = lbl_8087EE98;
    if (fn_8004ED34(colli, result, &from, &to, chara->fn_80179D44(), &chara->mColli, 0)) {
        if (result->m34 != 0) {
            return *result->m34;
        }
        return -1;
    }
    return -1;
}
