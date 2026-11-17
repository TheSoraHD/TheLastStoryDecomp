#include "gmk/SwingObject.h"
#include "game/GameTask.h"
#include "atn/Csv.h"
#include "atn/LH_Binary.h"
#include "atn/Memory.h"
#include "gmk/Lemon.h"
#include "gmk/SwingPart_unk.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "atn/RenderManager.h"
#include "atn/NodeChunk.h"
#include "atn/MapModelCache_unk.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" double sin(double x);
extern "C" double cos(double x);
extern "C" InstData_unk* fn_8009F788(const char* name); // inferred: finds or loads a building model
extern "C" int fn_8009FFCC(InstData_unk* data);         // inferred: nonzero while loading
extern "C" char lbl_8087D710[2]; // inferred: " " (field separators of the tab CSV)
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: hash of a name (as atn/MapModelCache_unk.h)
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
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
extern "C" void* lbl_8087F430; // inferred: field
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: node chunk passed to the node callback (only the hash is read)


// inferred: path of a gimmick file. @stringBase0 starts with this literal (followed by an extra
// zero byte) before the literals of LoadSwing_unk, so a function placed before it used it and was
// removed by the linker; the swinging map parts (fn_803FFD10) use the same literal. Name invented.
void SwingPath_unk(char* buf, int id) {
    sprintf(buf, "/database/gimmick/gm%03d_%03d.gmk\0", id / 1000, id % 1000);
}

// inferred: axis of a swing ("X", "Y", "Z"; 3-5 with a leading '-'); name invented
static inline int ParseAxis_unk(const char* axis) {
    int value = 0;
    if (*axis == '-') {
        value = 3;
        axis++;
    }
    if (*axis != 'X') {
        if (*axis == 'Y') {
            value += 1;
        } else {
            value += 2;
        }
    }
    return value;
}

// inferred: reads "NODE", "PARAM", "AXIS" and "EFP" (shared by gmk::SwingObject and the swinging map
// parts; name invented)
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
            nodes[10] = ParseAxis_unk(csv.fn_8005B9CC());
            nodes[11] = ParseAxis_unk(csv.fn_8005B9CC());
        } else if (strcmp(field, "EFP") == 0) {
            efp->LoadRequest(csv.fn_8005B9CC());
        }
    } while (csv.fn_8005BBF8() != 0);
    // inferred: fewer than two PARAM rows: the missing ones copy the last one
    if (count < 2) {
        for (; count < 2; count++) {
            params[count] = params[count - 1];
        }
    }
}

// inferred: not inline: MWCC inlines it into vf0C and the linker removes its own copy; compiled here, its
// 0.0f and 1.0f are the first constants of the .sdata2 of the unit, as in the original (name invented)
void PlayEfp_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, atn::Model* model) {
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

// inferred: degrees to radians (constant first; name invented)
inline float ToRad_unk(float deg) {
    return 0.017453292f * deg;
}

// inferred: sine of a float angle (name invented)
inline float SinF_unk(float x) {
    return (float)sin(x);
}

// inferred: the swing update shared with the map parts (name invented). The wind sum is written out at
// each use and the sines go through SinF_unk (float registers and scheduling of the original); the limit
// of mAngle0 is mMaxWind * ToRad_unk(2.0f) (mMaxWind loaded before the constant)
inline void UpdateSwing_unk(gmk::SwingCore_unk* core) {
    gmk::SwingParam_unk* param = core->mCur;
    float range = param->mDegMax - param->mDegMin;
    float amp = range / core->mMaxWind * (param->mWindPower + core->mWind);
    if (param->mWindPower + core->mWind > core->mMaxWind) {
        amp = range;
    }
    core->mAngle1 =
        ToRad_unk((param->mDegMin + amp) / (100.0f / param->mMiddle)) * SinF_unk(core->mTime + param->mLag);
    core->mAngle2 = ToRad_unk((param->mDegMin + amp) * SinF_unk(core->mTime));
    core->mTwistAngle = param->mTwist * ToRad_unk((param->mDegMin + amp) * SinF_unk(core->mTime + core->mPhase));
    core->mAngle0 = ToRad_unk(param->mDegMax / 20.0f) * (param->mWindPower + core->mWind);
    if (core->mAngle0 >= 1.5707964f) {
        core->mAngle0 = core->mMaxWind * ToRad_unk(2.0f);
    }
    if (param->mWindPower > 0.0f) {
        float c = (float)cos(core->mTime / (6.0f - 0.03f * param->mWindPower));
        core->mWind = 0.5f * param->mWindPower + param->mWindRand * (1.0f - c);
    } else {
        core->mWind = 0.0f;
    }
    core->mTime += param->mSpeed + (param->mWindPower + core->mWind) / (300.0f - 2.0f * param->mWindPower);
}

// inferred: update of a swing that the game never calls (the linker removes it); its constants are
// created before those of the callers of UpdateSwing_unk, which is the .sdata2 order of the original.
// Name invented.
void SwingUpdate_unk(gmk::SwingCore_unk* core) {
    UpdateSwing_unk(core);
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
// inferred: rotation matrix around an axis (0-2 x/y/z, 3-5 negated, else identity); returns the matrix
// on every path (the original sets the PSMTXConcat argument in each case). Name invented.
inline MtxPtr AxisMtx_unk(atn::Matrix3x4& r, int axis, float angle) {
    switch (axis) {
    case 0:
        PSMTXRotRad(r.m, 'x', angle);
        return r.m;
    case 1:
        PSMTXRotRad(r.m, 'y', angle);
        return r.m;
    case 2:
        PSMTXRotRad(r.m, 'z', angle);
        return r.m;
    case 3:
        PSMTXRotRad(r.m, 'x', -angle);
        return r.m;
    case 4:
        PSMTXRotRad(r.m, 'y', -angle);
        return r.m;
    case 5:
        PSMTXRotRad(r.m, 'z', -angle);
        return r.m;
    default:
        r.SetIdentity();
        return r.m;
    }
}
// inferred: level of detail of a model, keeping the previous one (name invented)
inline void SetLod_unk(atn::Model* model, int lod) {
    model->m20C_15_8 = model->m50;
    model->m50 = lod;
}
inline float CameraDistSq_unk(const atn::Vector3& pos) {
    atn::Vector3 d = atn::RenderManager::instance_->mCamera.mPos - pos;
    return PSVECSquareMag(&d);
}


namespace gmk {

GimmickCommon* SwingObject::fn_803FE390(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new ((MemoryTag)12, "", "", 0) SwingObject(parent, id, arg);
    }
    return 0;
}

GimmickCommon* SwingObject::fn_803FE570(atn::Task* parent, const GimmickDesc_unk* desc) {
    if (parent != 0) {
        SwingObject* obj = new ((MemoryTag)12, "", "", 0) SwingObject(parent, desc->mId, desc->mArg);
        if (obj != 0) {
            obj->m6C = desc->mPos;
            obj->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            obj->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
        }
    }
    return 0;
}

SwingObject::~SwingObject() {}

int SwingObject::vf0C() {
    if (fn_803EC91C() == 0 && mModel.GetModel_unk()->fn_8008B140() == 0 && mEfp.IsLoading() == 0) {
        mNodeMtx[0] =
            mModel.GetModel_unk()->GetNodeMtx_unk(mModel.GetModel_unk()->fn_800928B0(mCore.mNodes[0], 0));
        mNodeMtx[1] =
            mModel.GetModel_unk()->GetNodeMtx_unk(mModel.GetModel_unk()->fn_800928B0(mCore.mNodes[1], 0));
        mNodeMtx[2] =
            mModel.GetModel_unk()->GetNodeMtx_unk(mModel.GetModel_unk()->fn_800928B0(mCore.mNodes[2], 0));
        if (mNodeMtx[0] != 0 && mNodeMtx[1] != 0 && mNodeMtx[2] != 0) {
            fn_800F29D0(mModel.GetModel_unk(), boost::bind(&SwingObject::fn_803FEF78, this, _1, _2));
            mModel.GetModel_unk()->fn_800920B8(mCore.mNodes[0]);
            mModel.GetModel_unk()->fn_800920B8(mCore.mNodes[1]);
            mModel.GetModel_unk()->fn_800920B8(mCore.mNodes[2]);
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

// NONMATCHING: 98.36% - the model address kept in r4 across the LOD branch
void SwingObject::vf10() {
    if (mId != 10042) {
        if (CameraDistSq_unk(m6C) > 90000.0f) {
            int lod = mModel.GetModel_unk()->m20C_23_16 - 1;
            SetLod_unk(mModel.GetModel_unk(), lod < 0 ? 0 : lod);
            if (!mHidden) {
                fn_803ECBD4(mModel.GetModel_unk(), 1);
                mHidden = true;
            }
            return;
        }
    }
    fn_803FFC50();
    UpdateSwing_unk(&mCore);
    if (mNodeMtx[0] == 0 || mNodeMtx[1] == 0 || mNodeMtx[2] == 0) {
        return;
    }
    fn_803ECBD4(mModel.GetModel_unk(), 0);
    if (mModel.GetModel_unk()->mFlag6_unk) {
        mModel.GetModel_unk()->CalcMatrixStack(0, atn::ModelNodeCallback_unk());
        mHidden = true;
    }
}

void SwingObject::fn_803FEF78(atn::Matrix3x4* mtx, const atn::NodeChunk& node) {
    if (node.mHash == mCore.mNodes[0]) {
        SwingParam_unk* param = mCore.mCur;
        if (param->mWindVec > 0.0f) {
            RotateAxis_unk(mtx, mCore.mAxis1, -mCore.mAngle0);
        } else if (param->mWindVec < 0.0f) {
            RotateAxis_unk(mtx, mCore.mAxis1, mCore.mAngle0);
        }
        // inferred: the matrices of the three rotations are locals of this function (an inline places
        // them below those of RotateAxis_unk), t declared before r
        atn::Matrix3x4 t;
        atn::Matrix3x4 r;
        PSMTXConcat(mtx->m, AxisMtx_unk(r, mCore.mAxis1, -mCore.mAngle1), t.m);
        *mtx = t;
    } else {
        {
            atn::Matrix3x4 t;
            atn::Matrix3x4 r;
            PSMTXConcat(mtx->m, AxisMtx_unk(r, mCore.mAxis1, -mCore.mAngle2), t.m);
            *mtx = t;
        }
        {
            atn::Matrix3x4 t;
            atn::Matrix3x4 r;
            PSMTXConcat(mtx->m, AxisMtx_unk(r, mCore.mAxis2, -mCore.mTwistAngle), t.m);
            *mtx = t;
        }
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
            mModel.GetModel_unk()->fn_8008AD4C(csv.fn_8005B9CC(), 0);
        }
    } while (csv.fn_8005BBF8() != 0);
}

void SwingObject::vf24() {
    LoadSwing_unk((const char*)mBinary.fn_80470580(), mBinary.fn_8047059C(), mCore.mParams, mCore.mNodes, &mEfp);
}

// inferred: debug menu group of the gimmick in an inline (the pool address is then built before the
// group is moved to r3, as in the original); name invented
static inline void* MenuGroup_unk(SwingObject* self, void* menu, char* name) {
    if (self->mF0.mRes == 0 && menu != 0 && lbl_8087EF10 != 0) {
        return self->mF0.mRes = fn_8008937C(menu, name);
    }
    return 0;
}

void SwingObject::vf6C(void* menu) {
    char name[0x20];
    fn_803EC758(name);
    SwingParam_unk* param;
    void* group = MenuGroup_unk(this, menu, name);
    fn_800874C8(group, "state", &m54, 0, 9999, 1, 0, 0);
    fn_80087E9C(group, "pos", &m6C, 0, 0, -9999.0f, 9999.0f, 0.25f);
    fn_80087994(group, "isShow", &m9C, 0, 0);
    param = mCore.mParams;
    for (int i = 0; i < 2; i++) {
        char text[0x100];
        sprintf(text, "Param%d\0", i + 1);
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
        if (((GameTask*)lbl_8087F430)->fn_80376224() == 0) {
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
} // namespace gmk

// inferred: model of the building data (InstData_unk is an entry of the map model cache)
inline atn::Model* GetInstModel_unk(InstData_unk* data) {
    return ((MapModelCache_unk*)data)->mModel.GetModel_unk();
}

static char sSwingPath_unk[0x100]; // inferred: path buffer of the swinging map parts (0x807C8770)

// inferred: model of a swinging map part: the .gmk path is also the name of its building model
static InstData_unk* GetSwingData_unk(int id) {
    SwingPath_unk(sSwingPath_unk, id);
    return fn_8009F788(sSwingPath_unk);
}

// NONMATCHING: 99.36% - registers of the inlined SwingCore_unk array constructor and the LH_Binary vtable
SwingPart_unk::SwingPart_unk(int id, void* arg, SwingPartPos_unk* pos, unsigned int flags)
    : mInst(GetSwingData_unk(id)), m90(0), m94(pos), m98(0), m9C(0), mA0(0), mA4(0), m134(flags) {
    SwingPath_unk(sSwingPath_unk, id);
    mLoader.vf0C(sSwingPath_unk);
}

// inferred: plays the effect of a swinging map part (the manager is evaluated first, as in the
// original; name invented)
inline void PlayPartEfp_unk(EffectMgr_unk* mgr, atn::EfpFile* efp, void* owner) {
    Color4f_unk color;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(mgr, efp, 0, owner, lbl_807C7030, lbl_807C7030, color, -1, -1, 1, 1.0f);
}

// inferred: world matrices of the nodes of a swinging map part (shared by its loading and update; name
// invented)
inline void CalcNodes_unk(SwingPart_unk* part) {
    part->mInst.fn_8009EB20();
    Mtx* nodes = (Mtx*)part->m98.get();
    if (nodes == 0) {
        return;
    }
    SwingPartInst_unk* data = (SwingPartInst_unk*)part->mInst.mData;
    Mtx* local = data->mNodes;
    int count = data->m17C->mNumNodes;
    atn::Matrix3x4 mtx = part->mInst.mMtx;
    atn::Matrix3x4 t;
    for (int i = 0; i < count; i++) {
        PSMTXConcat(mtx.m, local[i], t.m);
        *(atn::Matrix3x4*)nodes[i] = t;
    }
}

// NONMATCHING: 99.90% - the three loads of m98 for m9C/mA0/mA4 take r26/r27 (r28/r29 in the original)
// inferred: loading of a swinging map part (1 when ready)
int SwingPart_unk::fn_803FFF40() {
    switch (m90) {
    case 0:
        if (fn_8009FFCC(mInst.mData) != 0 || mLoader.fn_80473F50() != 0) {
            return 0;
        }
        LoadSwing_unk((const char*)mLoader.fn_80470580(), mLoader.fn_8047059C(), mCore.mParams, mCore.mNodes, &mEfp);
        mLoader.fn_80473F88();
        m90 = 1;
    case 1: {
        if (mEfp.IsLoading()) {
            return 0;
        }
        m90 = 2;
        mInst.fn_8009EE30(mInst.mMtx);
        if ((m134 & 1) != 1 || (m134 & 2) != 2) {
            int count = ((SwingPartInst_unk*)mInst.mData)->m17C->mNumNodes;
            Mtx* nodes = (Mtx*)new ((MemoryTag)6, "", "", 0) atn::Matrix3x4[count];
            m98.reset((atn::Matrix3x4*)nodes);
            Mtx* base = (Mtx*)m98.get();
            m9C = base + GetInstModel_unk(mInst.mData)->fn_800928B0(mCore.mNodes[0], 0);
            base = (Mtx*)m98.get();
            mA0 = base + GetInstModel_unk(mInst.mData)->fn_800928B0(mCore.mNodes[1], 0);
            base = (Mtx*)m98.get();
            mA4 = base + GetInstModel_unk(mInst.mData)->fn_800928B0(mCore.mNodes[2], 0);
            mInst.m80 = (Mtx*)m98.get();
        }
        if ((m134 & 2) != 2) {
            fn_80232B7C(this, 1);
            lbl_8087F3C0->mB8 = 1;
            PlayPartEfp_unk(lbl_8087F3C0, &mEfp, this);
            lbl_8087F3C0->mB8 = 0;
        }
        mCore.mCur = &mCore.mParams[0];
        UpdateSwing_unk(&mCore);
        CalcNodes_unk(this);
    }
    }
    return 1;
}


// inferred: update of a swinging map part (called by MapPartsGimmickLoc)
void SwingPart_unk::fn_804003E0() {
    SwingPart_unk* part = this;
    if (CameraDistSq_unk(part->m94->mPos) > 90000.0f) {
        return;
    }
    UpdateSwing_unk(&part->mCore);
    if (part->m9C == 0 || part->mA0 == 0 || part->mA4 == 0) {
        return;
    }
    CalcNodes_unk(part);
}

// inferred: draw of a swinging map part (called by MapPartsGimmickLoc::vf14)
void SwingPart_unk::fn_804006E0() {
    if (lbl_8087F0A8->m18 != 0) {
        mInst.fn_8009E6EC(0);
    }
}

namespace gmk {

int SwingObject::vf40(GimmickMsg_unk* msg) {
    if (msg != 0) {
        m54 = msg->mType;
    }
    return 0;
}

void SwingObject::vf3C(atn::Scene* scene) {
    mModel.GetModel_unk()->m1DC = scene;
    mModel.GetModel_unk()->mFlags |= 0x10000;
}
} // namespace gmk
