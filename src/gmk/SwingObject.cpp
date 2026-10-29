#include "gmk/SwingObject.h"
#include "atn/Csv.h"
#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "gmk/Lemon.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int fn_800DC6B4(const char* s);   // inferred: hash of a name
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" int fn_800928B0(atn::Model* model, int hash, int arg); // inferred: node index by hash
extern "C" void fn_800920B8(atn::Model* model, int hash); // inferred: enables the node callback of a node
extern "C" void fn_800874C8(void* group, const char* name, int* value, int arg3, int max, int step, int arg6,
                            int arg7);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, int arg3, int arg4, float min,
                            float max, float step);
extern "C" void fn_80087994(void* group, const char* name, int* value, int arg3, int arg4);
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

// inferred: node callback of a model (as in game/CharaTask.cpp)
typedef boost::function<void(atn::Matrix3x4*, const atn::NodeChunk&, const atn::ModelNodeCallbackContext&),
                        std::allocator<boost::function_base> >
    NodeCallback_unk;
extern "C" void fn_800F29D0(atn::Model* model, NodeCallback_unk callback);

// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk {
    unsigned char m00[0xB8];
    int mB8;
};
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model, void* owner,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: camera (class unknown)
struct SwingCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" SwingCamera_unk* lbl_8087EFB4;
extern "C" void* lbl_8087F430; // inferred: field
extern "C" int fn_80376224(void* field); // inferred: nonzero in the second state (night?)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: node chunk passed to the node callback (only the hash is read)
namespace atn {
struct NodeChunk {
    unsigned char m00[0x14];
    int mHash; // 0x14
};
}

// inferred: path of a gimmick file. @stringBase0 starts with this literal (followed by an extra
// zero byte) before the literals of LoadSwing_unk, so a function placed before it used it and was
// removed by the linker; the swinging map parts (fn_803FFD10) use the same literal. Name invented.
void SwingPath_unk(char* buf, int id) {
    sprintf(buf, "/database/gimmick/gm%03d_%03d.gmk\0", id / 1000, id % 1000);
}

// inferred: reads "NODE", "PARAM", "AXIS" and "EFP" (shared by gmk::SwingObject and the swinging map
// parts; name invented)
// NONMATCHING: 90.09% - register allocation and the AXIS letter parsing (X/Y/Z) compiles to a branchless form
static void LoadSwing_unk(const char* data, int size, gmk::SwingParam_unk* params, int* nodes,
                          atn::EfpFile* efp) {
    atn::CsvTab csv(data, size);
    csv.fn_8005BCE8(lbl_8087D710);
    gmk::SwingParam_unk* param = params;
    int count = 0;
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] == ';') {
            continue;
        }
        if (strcmp(field, "NODE") == 0) {
            nodes[0] = fn_800DC6B4(csv.fn_8005B9CC());
            nodes[1] = fn_800DC6B4(csv.fn_8005B9CC());
            nodes[2] = fn_800DC6B4(csv.fn_8005B9CC());
        } else if (strcmp(field, "PARAM") == 0) {
            if (count < 2) {
                param->mWindPower = fn_800DC288(csv.fn_8005B9CC());
                param->mWindVec = fn_800DC288(csv.fn_8005B9CC());
                param->mDegMin = fn_800DC288(csv.fn_8005B9CC());
                param->mDegMax = fn_800DC288(csv.fn_8005B9CC());
                param->mMiddle = fn_800DC288(csv.fn_8005B9CC());
                param->mLag = fn_800DC288(csv.fn_8005B9CC());
                param->mSpeed = fn_800DC288(csv.fn_8005B9CC());
                param->mTwist = fn_800DC288(csv.fn_8005B9CC());
                param->mWindRand = fn_800DC288(csv.fn_8005B9CC());
                param++;
                count++;
            }
        } else if (strcmp(field, "AXIS") == 0) {
            for (int i = 0; i < 2; i++) {
                const char* axis = csv.fn_8005B9CC();
                int value = 0;
                if (*axis == '-') {
                    value = 3;
                    axis++;
                }
                if (*axis != 'X') {
                    value += (*axis == 'Y') ? 1 : 2;
                }
                nodes[10 + i] = value;
            }
        } else if (strcmp(field, "EFP") == 0) {
            efp->LoadRequest(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
    for (; count < 2; count++) {
        params[count] = params[count - 1];
    }
}

// inferred: the swing update shared with the map parts (name invented)
inline void UpdateSwing_unk(gmk::SwingCore_unk* core) {
    gmk::SwingParam_unk* param = core->mCur;
    float range = param->mDegMax - param->mDegMin;
    float wind = param->mWindPower + core->mWind;
    float amp = range / core->mMaxWind * wind;
    if (wind > core->mMaxWind) {
        amp = range;
    }
    core->mAngle1 = 0.017453292f * ((param->mDegMin + amp) / (100.0f / param->mMiddle)) *
                    (float)sin(core->mTime + param->mLag);
    core->mAngle2 = 0.017453292f * ((param->mDegMin + amp) * (float)sin(core->mTime));
    core->mTwistAngle =
        param->mTwist * (0.017453292f * ((param->mDegMin + amp) * (float)sin(core->mTime + core->mPhase)));
    core->mAngle0 = (param->mDegMax / 20.0f) * 0.017453292f * (param->mWindPower + core->mWind);
    if (core->mAngle0 >= 1.5707964f) {
        core->mAngle0 = core->mMaxWind * 0.034906585f;
    }
    if (param->mWindPower > 0.0f) {
        core->mWind = 0.5f * param->mWindPower +
                      param->mWindRand * (1.0f - (float)cos(core->mTime / (6.0f - 0.03f * param->mWindPower)));
    } else {
        core->mWind = 0.0f;
    }
    core->mTime += param->mSpeed + (param->mWindPower + core->mWind) / (300.0f - 2.0f * param->mWindPower);
}

// inferred: rotation of a matrix around an axis (0-2 x/y/z, 3-5 negated); names invented
inline void RotateAxis_unk(atn::Matrix3x4* mtx, int axis, float angle) {
    switch (axis) {
    case 0: {
        Mtx r;
        atn::Matrix3x4 t;
        PSMTXRotRad(r, 'x', angle);
        PSMTXConcat(mtx->m, r, t.m);
        *mtx = t;
        break;
    }
    case 1: {
        Mtx r;
        atn::Matrix3x4 t;
        PSMTXRotRad(r, 'y', angle);
        PSMTXConcat(mtx->m, r, t.m);
        *mtx = t;
        break;
    }
    case 2: {
        Mtx r;
        atn::Matrix3x4 t;
        PSMTXRotRad(r, 'z', angle);
        PSMTXConcat(mtx->m, r, t.m);
        *mtx = t;
        break;
    }
    case 3: {
        Mtx r;
        atn::Matrix3x4 t;
        PSMTXRotRad(r, 'x', -angle);
        PSMTXConcat(mtx->m, r, t.m);
        *mtx = t;
        break;
    }
    case 4: {
        Mtx r;
        atn::Matrix3x4 t;
        PSMTXRotRad(r, 'y', -angle);
        PSMTXConcat(mtx->m, r, t.m);
        *mtx = t;
        break;
    }
    case 5: {
        Mtx r;
        atn::Matrix3x4 t;
        PSMTXRotRad(r, 'z', -angle);
        PSMTXConcat(mtx->m, r, t.m);
        *mtx = t;
        break;
    }
    }
}
inline void AxisMtx_unk(atn::Matrix3x4& r, int axis, float angle) {
    switch (axis) {
    case 0:
        PSMTXRotRad(r.m, 'x', angle);
        break;
    case 1:
        PSMTXRotRad(r.m, 'y', angle);
        break;
    case 2:
        PSMTXRotRad(r.m, 'z', angle);
        break;
    case 3:
        PSMTXRotRad(r.m, 'x', -angle);
        break;
    case 4:
        PSMTXRotRad(r.m, 'y', -angle);
        break;
    case 5:
        PSMTXRotRad(r.m, 'z', -angle);
        break;
    default:
        r.SetIdentity();
        break;
    }
}
inline void RotateAxis2_unk(atn::Matrix3x4* mtx, int axis, float angle) {
    atn::Matrix3x4 r;
    AxisMtx_unk(r, axis, angle);
    atn::Matrix3x4 t;
    PSMTXConcat(mtx->m, r.m, t.m);
    *mtx = t;
}
inline float CameraDistSq_unk(const atn::Vector3& pos) {
    atn::Vector3 d = lbl_8087EFB4->mPos - pos;
    return fn_805F9920(&d);
}

inline void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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

namespace gmk {

// NONMATCHING: 99.88% - .sdata2 constant order (inlined SwingCore_unk ctor)
GimmickCommon* SwingObject::fn_803FE390(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) SwingObject(parent, id, arg);
    }
    return 0;
}

// NONMATCHING: 94.04% - .sdata2 constant order and float load scheduling
GimmickCommon* SwingObject::fn_803FE570(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        SwingObject* obj = new (12, "", "", 0) SwingObject(parent, desc->mId, desc->mArg);
        if (obj != 0) {
            obj->m6C = desc->mPos;
            obj->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            obj->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
        }
    }
    return 0;
}

SwingObject::~SwingObject() {}

// NONMATCHING: 99.98% - .sdata2 constant order
int SwingObject::vf0C() {
    if (fn_803EC91C() == 0 && mModel.GetModel_unk()->fn_8008B140() == 0 && mEfp.IsLoading() == 0) {
        mNodeMtx[0] =
            mModel.GetModel_unk()->GetNodeMtx_unk(fn_800928B0(mModel.GetModel_unk(), mCore.mNodes[0], 0));
        mNodeMtx[1] =
            mModel.GetModel_unk()->GetNodeMtx_unk(fn_800928B0(mModel.GetModel_unk(), mCore.mNodes[1], 0));
        mNodeMtx[2] =
            mModel.GetModel_unk()->GetNodeMtx_unk(fn_800928B0(mModel.GetModel_unk(), mCore.mNodes[2], 0));
        if (mNodeMtx[0] != 0 && mNodeMtx[1] != 0 && mNodeMtx[2] != 0) {
            fn_800F29D0(mModel.GetModel_unk(), boost::bind(&SwingObject::fn_803FEF78, this, _1, _2));
            fn_800920B8(mModel.GetModel_unk(), mCore.mNodes[0]);
            fn_800920B8(mModel.GetModel_unk(), mCore.mNodes[1]);
            fn_800920B8(mModel.GetModel_unk(), mCore.mNodes[2]);
        }
        if ((m58 & 2) != 2) {
            fn_80232B7C(this, 1);
            lbl_8087F3C0->mB8 = 1;
            PlayEfp_unk(lbl_8087F3C0, &mEfp, mModel.GetModel_unk());
            lbl_8087F3C0->mB8 = 0;
        }
        m54 = 1;
        mCore.mCur = &mCore.mParams[0];
        fn_803EDB18(mModel.GetModel_unk());
        fn_803ECBD4(mModel.GetModel_unk(), 1);
        return 1;
    }
    return 0;
}

// NONMATCHING: 91.55% - .sdata2 constant order and register allocation
void SwingObject::vf10() {
    if (mId != 10042) {
        if (CameraDistSq_unk(m6C) > 90000.0f) {
            atn::Model* model = mModel.GetModel_unk();
            int lod = model->m20C_23_16 - 1;
            model->m20C_15_8 = model->m50;
            model->m50 = lod < 0 ? 0 : lod;
            if (!mHidden) {
                fn_803ECBD4(mModel.GetModel_unk(), 1);
                mHidden = true;
            }
            return;
        }
    }
    fn_803FFC50();
    UpdateSwing_unk(&mCore);
    if (mNodeMtx[0] == 0 || mNodeMtx[1] == 0) {
        return;
    }
    if (mNodeMtx[2] == 0) {
        return;
    }
    fn_803ECBD4(mModel.GetModel_unk(), 0);
    if (mModel.GetModel_unk()->mFlag6_unk) {
        mModel.GetModel_unk()->CalcMatrixStack(0, atn::ModelNodeCallback_unk());
        mHidden = true;
    }
}

// NONMATCHING: 95.29% - .sdata2 constant order and register allocation in the inlined matrix code
void SwingObject::fn_803FEF78(atn::Matrix3x4* mtx, const atn::NodeChunk& node) {
    if (node.mHash == mCore.mNodes[0]) {
        SwingParam_unk* param = mCore.mCur;
        if (param->mWindVec > 0.0f) {
            RotateAxis_unk(mtx, mCore.mAxis1, -mCore.mAngle0);
        } else if (param->mWindVec < 0.0f) {
            RotateAxis_unk(mtx, mCore.mAxis1, mCore.mAngle0);
        }
        RotateAxis2_unk(mtx, mCore.mAxis1, -mCore.mAngle1);
    } else {
        RotateAxis2_unk(mtx, mCore.mAxis1, -mCore.mAngle2);
        RotateAxis2_unk(mtx, mCore.mAxis2, -mCore.mTwistAngle);
    }
}

void SwingObject::vf14() {
    if (lbl_8087F0A8->m18 != 0) {
        fn_803ED610(mModel.GetModel_unk());
    }
}

void SwingObject::vf20() {
    atn::CsvTab csv((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C());
    csv.fn_8005BCE8(lbl_8087D710);
    do {
        const char* field = csv.fn_8005B9CC();
        if (field[0] != ';' && strcmp(field, "MODEL") == 0) {
            fn_8008AD4C(mModel.GetModel_unk(), csv.fn_8005B9CC(), 0);
        }
    } while (csv.fn_8005BBF8() != 0);
}

void SwingObject::vf24() {
    LoadSwing_unk((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C(), mCore.mParams, mCore.mNodes, &mEfp);
}

// NONMATCHING: 96.65% - .sdata2 constant order
void SwingObject::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    void* group;
    if (mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        group = mF0.mRes = fn_8008937C(menu, name);
    } else {
        group = 0;
    }
    fn_800874C8(group, "state", &m54, 0, 9999, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    SwingParam_unk* param = mCore.mParams;
    for (int i = 0; i < 2; i++) {
        char text[0x100];
        sprintf(text, "Param%d", i + 1);
        void* sub = fn_8008937C(group, text);
        fn_8008771C(sub, "wind_power", &param->mWindPower, 0, 0, 0.0f, 100.0f, 1.0f);
        fn_8008771C(sub, "wind_vec", &param->mWindVec, 0, 0, -1.0f, 1.0f, 1.0f);
        fn_8008771C(sub, "degree_min", &param->mDegMin, 0, 0, 0.0f, 90.0f, 1.0f);
        fn_8008771C(sub, "degree_max", &param->mDegMax, 0, 0, 0.0f, 90.0f, 1.0f);
        fn_8008771C(sub, "middle/bottom (%)", &param->mMiddle, 0, 0, 0.0f, 100.0f, 1.0f);
        fn_8008771C(sub, "node_time_lag", &param->mLag, 0, 0, 0.0f, 99.0f, 1.0f);
        fn_8008771C(sub, "time_speed", &param->mSpeed, 0, 0, 0.0f, 99.0f, 1.0f);
        fn_8008771C(sub, "twist_power", &param->mTwist, 0, 0, 0.0f, 99.0f, 0.1f);
        fn_8008771C(sub, "wind_power_rand", &param->mWindRand, 0, 0, 0.0f, 100.0f, 1.0f);
        param++;
    }
    fn_800874C8(group, "axis1", &mCore.mAxis1, 0, 5, 1, 0, 0);
    fn_800874C8(group, "axis2", &mCore.mAxis2, 0, 5, 1, 0, 0);
}

void SwingObject::fn_803FFC50() {
    if ((m58 & 1) == 1 && lbl_8087F430 != 0) {
        if (fn_80376224(lbl_8087F430) == 0) {
            m54 = 1;
        } else {
            m54 = 2;
        }
    }
    SwingParam_unk* param = 0;
    if (m54 == 2 && mCore.mCur == &mCore.mParams[0]) {
        param = &mCore.mParams[1];
    } else if (m54 == 1 && mCore.mCur == &mCore.mParams[1]) {
        param = &mCore.mParams[0];
    }
    if (param != 0 && mCore.mCur != param) {
        mCore.mCur = param;
    }
}

int SwingObject::vf40(GimmickMsg_unk* msg) {
    if (msg != 0) {
        m54 = msg->mType;
    }
    return 0;
}

void SwingObject::vf3C(void* lens) {
    mModel.GetModel_unk()->m1DC = (Lens_unk*)lens;
    mModel.GetModel_unk()->mFlags |= 0x10000;
}

} // namespace gmk

// inferred: swinging map part (a model instance with the swing state; built by fn_803FFD10)
struct SwingPartData_unk {
    unsigned char m00[0x44];
    int mNumNodes; // 0x44
};
struct SwingPartInst_unk {
    unsigned char m00[0x4C];
    Mtx* mNodes; // 0x4C: inferred: local node matrices
    unsigned char m50[0x17C - 0x50];
    SwingPartData_unk* m17C;
};
struct SwingPartPos_unk {
    int m00;
    atn::Vector3 mPos; // 0x04
};
struct SwingPart_unk : public atn::ModelInstance {
    unsigned char m88[0x94 - 0x88];
    SwingPartPos_unk* m94;
    Mtx* m98; // inferred: world node matrices
    int m9C;
    int mA0;
    int mA4;
    gmk::SwingCore_unk mCore; // 0xA8
};

// inferred: update of a swinging map part (called by MapPartsGimmickLoc; name unknown)
// NONMATCHING: 96.9% - float registers of the inlined UpdateSwing_unk (as in SwingObject::vf10) and
// the registers of the node pointer and the loop counter swapped.
extern "C" void fn_804003E0(SwingPart_unk* part) {
    if (CameraDistSq_unk(part->m94->mPos) > 90000.0f) {
        return;
    }
    UpdateSwing_unk(&part->mCore);
    if (part->m9C == 0 || part->mA0 == 0 || part->mA4 == 0) {
        return;
    }
    part->fn_8009EB20();
    Mtx* nodes = part->m98;
    if (nodes == 0) {
        return;
    }
    SwingPartInst_unk* data = (SwingPartInst_unk*)part->mData;
    int count = data->m17C->mNumNodes;
    Mtx* local = data->mNodes;
    atn::Matrix3x4 t;
    atn::Matrix3x4 mtx = part->mMtx;
    for (int i = 0; i < count; i++) {
        PSMTXConcat(mtx.m, local[i], t.m);
        *(atn::Matrix3x4*)nodes[i] = t;
    }
}

// inferred: draw of a swinging map part (its model instance comes first); called by
// MapPartsGimmickLoc (name unknown)
extern "C" void fn_804006E0(atn::ModelInstance* inst) {
    if (lbl_8087F0A8->m18 != 0) {
        inst->fn_8009E6EC(0);
    }
}
