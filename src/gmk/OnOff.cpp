#include "gmk/OnOff.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/CharaTask.h"
#include "game/GimmickManager.h"
#include "game/PlayerTask.h"
#include "gmk/Lemon.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" char lbl_8087D710[2]; // inferred: field separators of the tab CSV
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" void fn_8008AD4C(Model_unk* model, const char* path, const char* texture); // inferred: loads a model
extern "C" void fn_80092F1C(Model_unk* model, const char* path, int arg, float lod); // inferred: adds a LOD model
extern "C" int fn_802180A8(const char* s); // inferred: motion id from its name
extern "C" void fn_80097A88(Model_unk* model, int id, const char* path); // inferred: loads a motion
extern "C" int fn_80097B34(Model_unk* model); // inferred: number of motions
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" atn::Matrix3x4 lbl_807C7060; // inferred: identity matrix
extern "C" void fn_800577D8(atn::ColliBox* box); // inferred: updates the box

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct OnOffEffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" OnOffEffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(OnOffEffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_8023A680(OnOffEffectMgr_unk* mgr, ResLoader_unk* res, int arg2, int arg3, Model_unk* model,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg7, int arg8, int arg9,
                            int arg10, float scale);
extern "C" void fn_8023A02C(OnOffEffectMgr_unk* mgr, void* owner, int arg2, int arg3);
extern "C" void fn_80239DAC(OnOffEffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: debug drawing (lbl_8087EEB0)
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80063D3C(void* dbg, const atn::Vector3* pos, unsigned int color, float radius, float arg);
// inferred: game settings
struct OnOffSettings_unk {
    unsigned char m00[0x08];
    int m08; // inferred: debug draw of the gimmicks
    unsigned char m0C[0x18 - 0x0C];
    int m18; // inferred: debug draw of the models
};
extern "C" OnOffSettings_unk* lbl_8087F0A8;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

// inferred: map part (type at 0x48, next at 0x4C; see gmk/ArenaFloor.cpp)
struct OnOffPart_unk {
    unsigned char m00[0x48];
    int mType;
    OnOffPart_unk* mNext;
    unsigned char m50[0x344 - 0x50]; // inferred: a model (type 0)
    int mObjCount;                   // 0x344: inferred: objects of a part of type 4
    atn::ModelInstance** mObjs;      // 0x348
};
struct OnOffMap_unk {
    unsigned char m00[0x38];
    unsigned int mFlags; // 0x38: inferred: bit 1 set while loading, bit 2 hidden
    unsigned char m3C[0x70 - 0x3C];
    OnOffPart_unk* mParts; // 0x70
    unsigned char m74[0xF4 - 0x74];
    int mF4;
};
extern "C" OnOffMap_unk* fn_8049D68C(void* owner, const char* name);
extern "C" void fn_8049994C(OnOffPart_unk* part, atn::Vector3* pos, atn::Vector3* rot); // inferred: places the gimmicks of a part
extern "C" void fn_80499B9C(OnOffPart_unk* part);
extern "C" void fn_80499C48(OnOffPart_unk* part);

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))

// inferred: rotation matrix z * y * x (same as GimmickCommon.cpp)
inline void __MakeRotation_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.y != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'y', r.y);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    if (r.x != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'x', r.x);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
}
inline void __SetTrans_unk(atn::Matrix3x4& m, const atn::Vector3& v) {
    m.m[0][3] = v.x;
    m.m[1][3] = v.y;
    m.m[2][3] = v.z;
}
// inferred: sets the translation column (the arguments are read z first, see GimmickCommon.cpp)
inline void __SetTrans3_unk(atn::Matrix3x4& m, float x, float y, float z) {
    m.m[0][3] = x;
    m.m[1][3] = y;
    m.m[2][3] = z;
}
// inferred: flag test through an accessor (the flags are loaded again before they are changed)
inline bool IsColliOn_unk(const atn::ColliBase* colli) {
    return (colli->mFlags & 1) == 1;
}
// inferred: oriented box assigned to atn::ColliBox::m3C (size, then matrix)
struct OnOffBox_unk {
    OnOffBox_unk() {}
    OnOffBox_unk(const atn::Vector3& size, const atn::Matrix3x4& mtx) {
        mMtx = mtx;
        mSize = size;
    }
    atn::Vector3 mSize;
    atn::Matrix3x4 mMtx;
};
// inferred: names invented
inline void SetBox_unk(atn::ColliBox* colli, const OnOffBox_unk& box) {
    *(OnOffBox_unk*)colli->m3C = box;
    fn_800577D8(colli);
}
inline void SetSphere_unk(atn::ColliSphere* colli, const atn::Vector3& center, float radius) {
    colli->mSphere.mCenter = center;
    colli->mSphere.mRadius = radius;
}
// inferred: plays an effect at the model (name invented)
inline void PlayEfp_unk(OnOffEffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
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
    fn_8023A8B4(mgr, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}

// inferred: plays an effect at a position (name invented)
inline void PlayEfpAt_unk(OnOffEffectMgr_unk* mgr, atn::EfpHandle_unk* efp, const atn::Vector3& pos,
                          const atn::Vector3& rot, float scale) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, 0, 0, pos, rot, color, -1, -1, 1, scale);
}

namespace gmk {

GimmickCommon* OnOff::fn_803F744C(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) OnOff(parent, id, arg);
    }
    return 0;
}

GimmickCommon* OnOff::fn_803F74D0(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        OnOff* onoff = new (12, "", "", 0) OnOff(parent, desc->mId + desc->m20, desc->mArg);
        if (onoff != 0) {
            onoff->m6C = desc->mPos;
            onoff->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            onoff->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
            onoff->mDesc = desc;
        }
        return onoff;
    }
    return 0;
}

OnOff::OnOff(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mHasModel(0), mHasColli(0), mHasBox(0), mHasSphere(0),
      mMap(0), mOn(0), mMotion(0), mBoxPos(0.0f, 0.0f, 0.0f), mBoxRot(0.0f, 0.0f, 0.0f), mBoxSize(0.0f, 0.0f, 0.0f),
      mSpherePos(0.0f, 0.0f, 0.0f), mSphereRadius(0.0f), mInit(1), mFlags(0), mDesc(0) {
    m54 = 0;
}

OnOff::~OnOff() {}

// NONMATCHING: 99.86% - stack slots of the temporaries of the case 5 loop and the order of the
// hoisted stack addresses (r19-r30)
int OnOff::vf0C() {
    if (fn_803EC91C() == 0) {
    if (mHasModel != 0) {
        fn_803EDB18(&mModel);
    }
    if (mMap != 0) {
        for (OnOffPart_unk* part = mMap->mParts; part != 0; part = part->mNext) {
            switch (part->mType) {
            case 0: {
                Model_unk* model = (Model_unk*)part->m50;
                atn::Matrix3x4 mtx = *(atn::Matrix3x4*)model->mMtx;
                atn::Matrix3x4 rot;
                __MakeRotation_unk(rot, m78);
                PSMTXConcat(rot.m, mtx.m, mtx.m);
                __SetTrans_unk(mtx, mtx.GetTrans_unk() + m6C);
                __MtxCopy(model->mMtx, mtx.m);
                atn::Vector3 az(mtx.m[0][2], mtx.m[1][2], mtx.m[2][2]);
                float sz = PSVECMag(&az);
                atn::Vector3 ay(mtx.m[0][1], mtx.m[1][1], mtx.m[2][1]);
                float sy = PSVECMag(&ay);
                atn::Vector3 ax(mtx.m[0][0], mtx.m[1][0], mtx.m[2][0]);
                float sx = PSVECMag(&ax);
                atn::Vector3 s(sx, sy, sz);
                model->m54 = __MAX_unk(s.x, __MAX_unk(s.y, s.z));
                fn_8000D430(model, boost::function<void()>());
                break;
            }
            case 4:
                for (int i = 0; i < part->mObjCount; i++) {
                    atn::ModelInstance* obj = part->mObjs[i];
                    atn::Matrix3x4 mtx = obj->mMtx;
                    atn::Matrix3x4 rot;
                    __MakeRotation_unk(rot, m78);
                    PSMTXConcat(rot.m, mtx.m, mtx.m);
                    __SetTrans_unk(mtx, mtx.GetTrans_unk() + m6C);
                    obj->fn_8009EE30(mtx);
                }
                break;
            case 5:
                fn_8049994C(part, &m6C, &m78);
                for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
                    if (g->m20 == part) {
                        g->m6C = g->m6C + m6C;
                        g->m78 = g->m78 + m78;
                    }
                }
                break;
            }
        }
    }
    return 1;
    }
    return 0;
}

void OnOff::vf28() {
    vf2C();
    if (mHasModel != 0) {
        fn_803ED0D4(&mModel, 1);
    }
    fn_803F8324();
    GimmickMsg_unk msg(mInit);
    if (m58 != 0) {
        msg.mType = 2;
    }
    vf40(&msg);
}

void OnOff::vf10() {
    if (mDesc != 0 && mDesc->m7C == 0) {
        if (IsColliOn_unk(&mBox)) {
            mBox.mFlags &= ~1;
        }
        if (IsColliOn_unk(&mSphere)) {
            mSphere.mFlags &= ~1;
        }
        if (IsColliOn_unk(&mColli)) {
            mColli.mFlags &= ~1;
        }
        return;
    }
    if (m54 == 0) {
        GimmickMsg_unk msg(mInit);
        vf40(&msg);
    }
    if (mOn != 0 || (mFlags & 0x80) == 0x80) {
        if (vf38() != 0) {
            fn_803ED5D0(vf38());
        }
    }
    if (mHasModel != 0) {
        if (vf38() != 0) {
            fn_803ED0D4(vf38(), 1);
        }
    }
    fn_803F8324();
    if ((m54 == 1 && mOn != 0) || (m54 == 2 && mOn == 0)) {
        GimmickMsg_unk msg(m54);
        vf40(&msg);
    }
    if (mOn == 0) {
        return;
    }
    mMtx = lbl_807C7060;
    atn::Matrix3x4 rot;
    __MakeRotation_unk(rot, m78);
    Mtx scale;
    PSMTXScale(scale, m90.x, m90.y, m90.z);
    PSMTXConcat(scale, mMtx.m, mMtx.m);
    PSMTXConcat(rot.m, mMtx.m, mMtx.m);
    __SetTrans3_unk(mMtx, m6C.x, m6C.y, m6C.z);
}

// NONMATCHING: 99.16% - the copy of mMtx uses r6/r3 (r7/r6 here) and the copy of mBoxPos reads z
// through r31 in the original
void OnOff::fn_803F8324() {
    if (mHasBox != 0) {
        atn::Matrix3x4 mtx;
        OnOffBox_unk box;
        __MtxCopy(mtx.m, mMtx.m);
        atn::Vector3 pos = mtx.GetTrans_unk();
        mtx.m[0][3] = 0.0f;
        mtx.m[1][3] = 0.0f;
        mtx.m[2][3] = 0.0f;
        atn::Vector3 off(mBoxPos);
        PSMTXMultVec(mtx.m, &off, &off);
        pos += off;
        atn::Matrix3x4 rot;
        __MakeRotation_unk(rot, mBoxRot);
        PSMTXConcat(rot.m, mtx.m, mtx.m);
        __SetTrans3_unk(mtx, pos.x, pos.y, pos.z);
        box.mMtx = mtx;
        __VecCopy(&box.mSize.x, &mBoxSize.x);
        SetBox_unk(&mBox, box);
    }
    if (mHasSphere != 0) {
        SetSphere_unk(&mSphere, mMtx.GetTrans_unk(), mSphereRadius);
    }
    if (mHasColli != 0) {
        if (mHasModel != 0) {
            mColli.SetMtx_unk(*(atn::Matrix3x4*)mModel.mMtx);
        } else {
            mColli.SetMtx_unk(mMtx);
        }
    }
}

void OnOff::vf14() {
    if (mDesc != 0 && mDesc->m7C == 0) {
        return;
    }
    if (lbl_8087F0A8->m18 != 0 && mHasModel != 0 && (mOn != 0 || (mFlags & 1) == 1)) {
        if (vf38() != 0) {
            fn_803ED774(vf38());
        }
    }
    if (lbl_8087F0A8->m08 != 0 && mHasBox == 0 && mHasSphere == 0 && mHasColli == 0) {
        fn_80063D3C(lbl_8087EEB0, &m6C, 0xFFFFFF00, 3.0f, 0.0f);
    }
}

int OnOff::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli.fn_800580BC() != 0) {
        return 1;
    }
    if (mEff.fn_802376D0() != 0) {
        return 1;
    }
    if (mEfp.fn_80237874() != 0) {
        return 1;
    }
    if (mMap != 0 && (mMap->mFlags & 2) != 2) {
        return 1;
    }
    return 0;
}

void OnOff::vf20() {
    char path[256];
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(&mModel, csv.fn_8005B9CC(), 0);
            fn_803EC7A0(&mModel, &csv);
            mHasModel = 1;
        } else if (strcmp(field, "MODEL_LOD") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            fn_80092F1C(&mModel, path, 0, fn_800DC288(csv.fn_8005B9CC()));
        } else if (strcmp(field, "MOTION") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            mMotion = fn_802180A8(csv.fn_8005B9CC());
            fn_80097A88(&mModel, mMotion, path);
        }
        if (strcmp(field, "COLLI_OBJECT") == 0) {
            mColli.fn_80058078(csv.fn_8005B9CC());
            mHasColli = 1;
        } else if (strcmp(field, "COLLI_BOX") == 0) {
            mBoxPos.x = fn_800DC288(csv.fn_8005B9CC());
            mBoxPos.y = fn_800DC288(csv.fn_8005B9CC());
            mBoxPos.z = fn_800DC288(csv.fn_8005B9CC());
            mBoxRot.x = fn_800DC288(csv.fn_8005B9CC());
            mBoxRot.y = fn_800DC288(csv.fn_8005B9CC());
            mBoxRot.z = fn_800DC288(csv.fn_8005B9CC());
            mBoxSize.x = fn_800DC288(csv.fn_8005B9CC());
            mBoxSize.y = fn_800DC288(csv.fn_8005B9CC());
            mBoxSize.z = fn_800DC288(csv.fn_8005B9CC());
            mHasBox = 1;
        } else if (strcmp(field, "COLLI_SPHERE") == 0) {
            mSpherePos.x = fn_800DC288(csv.fn_8005B9CC());
            mSpherePos.y = fn_800DC288(csv.fn_8005B9CC());
            mSpherePos.z = fn_800DC288(csv.fn_8005B9CC());
            mSphereRadius = fn_800DC288(csv.fn_8005B9CC());
            mHasSphere = 1;
        } else if (strcmp(field, "EFF") == 0) {
            mEff.fn_80237654(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            mEfp.fn_8023780C(csv.fn_8005B9CC());
        } else if (strcmp(field, "MAP") == 0) {
            mMap = fn_8049D68C(this, csv.fn_8005B9CC());
            if (mMap != 0) {
                mMap->mF4 = 1;
            }
        } else if (strcmp(field, "INIT") == 0) {
            strcpy(path, csv.fn_8005B9CC());
            int cmp = strcmp(path, "ON");
            int init = 1;
            if (cmp == 0) {
                init = 2;
            }
            mInit = init;
        } else if (strcmp(field, "FLAG") == 0) {
            fn_803F8C98(&csv);
        }
    } while (csv.fn_8005BBF8() != 0);
}

void OnOff::fn_803F8C98(atn::CsvTab* csv) {
    const char* name = csv->fn_8005B9CC();
    while (name != 0 && strcmp("", name) != 0) {
        if (strcmp(name, "FORCE_MODEL_ENABLE") == 0) {
            mFlags |= 1;
        } else if (strcmp(name, "NO_MOTION_LOOP") == 0) {
            mFlags |= 2;
        } else if (strcmp(name, "EFF_CLEAR_IMMEDIATE") == 0) {
            mFlags |= 4;
        } else if (strcmp(name, "EFF_NO_OFFSET") == 0) {
            mFlags |= 8;
        } else if (strcmp(name, "EFF_PARENT_PLAYER") == 0) {
            mFlags |= 0x10;
        } else if (strcmp(name, "ONOFF_REVERSE_COLLI") == 0) {
            mFlags |= 0x20;
        } else if (strcmp(name, "EFF_REFLECT") == 0) {
            mFlags |= 0x40;
        } else if (strcmp(name, "FORCE_MOTION_ENABLE") == 0) {
            mFlags |= 0x80;
        } else if (strcmp(name, "FORCE_EFF_ENABLE") == 0) {
            mFlags |= 0x100;
        } else if (strcmp(name, "MOTION_RESET") == 0) {
            mFlags |= 0x200;
        }
        name = csv->fn_8005B9CC();
    }
}

int OnOff::vf40(GimmickMsg_unk* msg) {
    if (msg == 0) {
        return 0;
    }
    switch (msg->mType) {
    case 2:
        fn_803F8F30(1);
        break;
    case 1:
        fn_803F8F30(0);
        break;
    case 3:
        fn_803F8F30(0);
        fn_80232B7C(0, 0);
        fn_8023A680(lbl_8087F3C0, &mEff, -1, 0, 0, &m6C, &m78, 0, 0, -1, 1, 1.0f);
        break;
    }
    return m54;
}

void OnOff::fn_803F8F30(int on) {
    mOn = on;
    int state = 1;
    if (on != 0) {
        state = 2;
    }
    m54 = state;
    if (mHasModel != 0 && (mOn != 0 || (mFlags & 0x80) == 0x80) && fn_80097B34(&mModel) > 0) {
        int loop = ((mFlags >> 1) & 1) ^ 1;
        int reset = 0;
        if (loop == 0 || (mFlags & 0x200) == 0x200) {
            reset = 1;
        }
        mModel.fn_80097C08(0, mMotion, loop, reset, 1, reset != 0 ? 1.0f : 0.0f, 0.1f);
    }
    int colli = mOn;
    if ((mFlags & 0x20) == 0x20) {
        colli = mOn == 0;
    } else if ((mFlags & 1) == 1) {
        colli = 1;
    }
    if (mHasColli != 0) {
        if (colli != 0) {
            mColli.mFlags |= 1;
        } else {
            mColli.mFlags &= ~1;
        }
    }
    if (mHasBox != 0) {
        if (colli != 0) {
            mBox.mFlags |= 1;
        } else {
            mBox.mFlags &= ~1;
        }
    }
    if (mHasSphere != 0) {
        if (colli != 0) {
            mSphere.mFlags |= 1;
        } else {
            mSphere.mFlags &= ~1;
        }
    }
    if (mMap != 0) {
        if (mOn != 0) {
            mMap->mFlags &= ~4;
        } else {
            mMap->mFlags |= 4;
        }
        for (OnOffPart_unk* part = mMap->mParts; part != 0; part = part->mNext) {
            if (part->mType == 5) {
                if (mOn != 0) {
                    fn_80499B9C(part);
                } else {
                    fn_80499C48(part);
                }
            }
        }
    }
    fn_80239DAC(lbl_8087F3C0, this, 0, ((mFlags >> 2) & 1) ^ 1);
    if (mOn != 0 || (mFlags & 0x100) == 0x100) {
        fn_80232B7C(this, 0);
        lbl_8087F3C0->mB8 = 1;
        if (mId == 5049) {
            lbl_8087F3C0->mB8 = 7;
        }
        OnOffEffectMgr_unk* mgr = lbl_8087F3C0;
        fn_8023A680(mgr, &mEff, -1, 5, (Model_unk*)&mMtx, 0, 0, 0, 0, -1, 1, 1.0f);
        CharaTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if ((mFlags & 0x10) == 0x10 && player != 0) {
            PlayEfp_unk(lbl_8087F3C0, &mEfp, player->GetModel_unk());
        } else if (vf38() != 0) {
            PlayEfp_unk(lbl_8087F3C0, &mEfp, vf38());
        } else {
            atn::Vector3 pos(0.0f, 0.0f, 0.0f);
            atn::Vector3 rot(0.0f, 0.0f, 0.0f);
            if ((mFlags & 8) != 8) {
                pos = m6C;
                rot = m78;
            }
            PlayEfpAt_unk(lbl_8087F3C0, &mEfp, pos, rot, m90.x);
        }
        if ((mFlags & 0x40) == 0x40) {
            fn_8023A02C(lbl_8087F3C0, this, 0, 8);
        }
        lbl_8087F3C0->mB8 = 0;
    }
}

Model_unk* OnOff::vf38() {
    if (mHasModel != 0) {
        return &mModel;
    }
    return 0;
}

int OnOff::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    return 0;
}

void OnOff::vf4C() {}

// NOTE: vf3C (0x803F940C) is not decompiled: it takes an argument (stored in the model at 0x1DC,
// then mModel flag 0x10000 is set) while GimmickCommon::vf3C is declared without arguments.

} // namespace gmk
