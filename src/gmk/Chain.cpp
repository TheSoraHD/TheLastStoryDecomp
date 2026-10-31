#include "gmk/Chain.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/EnemyManager.h"
#include "game/NpcManager.h"

extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void C_QUATMtx(atn::Quaternion_unk* q, const Mtx m);
extern "C" void C_QUATRotAxisRad(atn::Quaternion_unk* q, const atn::Vector3* axis, float rad); // inferred: C_QUATRotAxisRad
extern "C" double acos(double x);
// inferred: camera (only the position)
struct CameraPos_unk {
    unsigned char m00[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" CameraPos_unk* lbl_8087EFB4;
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;

// inferred: node names of the characters the chain hangs from (indexed by mDesc->m28)
static const char* sNodes[] = {"const_foot_l", "const_foot_r", "left_chainpoint", "right_chainpoint",
                               "leftfoot",     "rightfoot",    "leftshoulder",    "rightshoulder"};

static inline float SqLength_unk(const atn::Vector3& v) {
    return PSVECSquareMag(&v);
}

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))
inline atn::Vector3 GetCol_unk(const atn::Matrix3x4& m, int i) {
    return atn::Vector3(m.m[0][i], m.m[1][i], m.m[2][i]);
}
// inferred: largest scale axis of a matrix (as in gmk/Bat.cpp)
inline float MaxScale_unk(const atn::Matrix3x4& mtx) {
    atn::Vector3 s;
    atn::Vector3 az = GetCol_unk(mtx, 2);
    float sz = PSVECMag(&az);
    atn::Vector3 ay = GetCol_unk(mtx, 1);
    float sy = PSVECMag(&ay);
    atn::Vector3 ax = GetCol_unk(mtx, 0);
    float sx = PSVECMag(&ax);
    s.x = sx;
    s.y = sy;
    s.z = sz;
    return __MAX_unk(s.x, __MAX_unk(s.y, s.z));
}
// inferred: m = m * translation(v) (name invented)
inline void Translate_unk(atn::Matrix3x4& m, const atn::Vector3& v) {
    Mtx t;
    Mtx r;
    PSMTXTrans(t, v.x, v.y, v.z);
    PSMTXConcat(m.m, t, r);
    __MtxCopy(m.m, r);
}

namespace gmk {

GimmickCommon* Chain::fn_8043B778(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        new ((MemoryTag)12, "", "", 0) Chain(parent, desc);
    }
    return 0;
}

Chain::Chain(atn::Task* parent, GimmickDesc_unk* desc)
    : GimmickCommon(parent, desc->mId, desc->mArg), mDesc(desc), mModel(1), mTarget(0), m340(0.98f) {
    m54 = 0;
    m68 = 1;
    m6C = desc->mPos;
    m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
    m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
    fn_8008AD4C(mModel.GetModel_unk(), "it087_00.model", 0);
    mModel.GetModel_unk()->mAC = 5.0f;
}

Chain::~Chain() {}

int Chain::vf0C() {
    if (fn_803EC91C() == 0 && mModel.GetModel_unk()->fn_8008B140() == 0) {
        m54 = 1;
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.95% - only the place of 90000.0f in .sdata2 (fn_8043B9E0, not decompiled yet,
// creates 1.4f before it).
void Chain::vf10() {
    if (SqLength_unk(lbl_8087EFB4->mPos - m6C) > 90000.0f) {
        return;
    }
    if (mTarget == 0) {
        if (mDesc->m20 == 0) {
            mTarget = (CharaTask*)lbl_8087F8A0->mHead;
        } else if (mDesc->m20 == 1) {
            mTarget = (CharaTask*)NpcManager::sInstance->Find_unk(mDesc->m24);
        } else if (mDesc->m20 == 2) {
            mTarget = (CharaTask*)EnemyManager::sInstance->Find_unk(mDesc->m24);
        } else if (mDesc->m20 == 3) {
            mTarget = (CharaTask*)lbl_8087F8A0->Find_unk(mDesc->m24);
        }
        if (mTarget == 0 || !mTarget->IsFlag_unk(2) || mTarget->IsFlag_unk(4)) {
            mTarget = 0;
            return;
        }
        mNode = mTarget->mMotion.GetNodeNoByName(sNodes[mDesc->m28], 0);
        fn_8043B9E0();
    } else if (mTarget->IsFlag_unk(4)) {
        mTarget = 0;
        return;
    }
    fn_8043C508();
}

void Chain::vf14() {
    if (mTarget != 0 && lbl_8087F0A8->m18 != 0) {
        fn_803ED610(mModel.GetModel_unk());
    }
}

// NONMATCHING: 90.6% - stack layout of the temporaries (16 bytes smaller frame) and the order of
// the node copies of the joints.
void Chain::fn_8043C508() {
    m314 = m6C;
    atn::Matrix3x4 mtx = *(atn::Matrix3x4*)mTarget->mMotion.GetNodeMtx_unk(mNode);
    Translate_unk(mtx, atn::Vector3(0.75f, 0.0f, -0.85f));
    m320 = mtx.GetTrans_unk();
    atn::Vector3 from = m6C;
    m33C = 1.0f;
    Translate_unk(mtx, atn::Vector3(0.0f, 0.0f, 10.0f));
    atn::Vector3 to = mtx.GetTrans_unk();
    atn::Vector3 d = to - from;
    atn::Vector3 dir = d;
    if (PSVECMag(&dir) > 0.00001f) {
        float a = 1.5707964f + -(float)atan2(dir.x, dir.z);
        atn::Matrix3x4 m;
        m.SetIdentity();
        Translate_unk(m, atn::Vector3(m6C.x, m6C.y - 1.4f, m6C.z));
        Mtx r;
        Mtx t;
        PSMTXRotRad(r, 'y', a);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
        __MtxCopy(mModel.GetModel_unk()->mMtx, m.m);
        mModel.GetModel_unk()->m54 = MaxScale_unk(m);
        mModel.GetModel_unk()->CalcMatrixStack(0, atn::ModelNodeCallback_unk());
        for (int i = 0; i < mNumJoints; i++) {
            ChainJoint_unk& j = mJoints2.mData[i];
            j.m1C = *(atn::Matrix3x4*)mModel.GetModel_unk()->GetNodeMtx_unk(j.mNode);
            mJoints2.mData[i].m04 = atn::Vector3(mJoints2.mData[i].m1C.m[0][3], mJoints2.mData[i].m1C.m[1][3],
                                                 mJoints2.mData[i].m1C.m[2][3]);
        }
    }
    fn_8043CA58();
}

// NONMATCHING: 98.8% - saved registers rotated by one (tmp/pass counters); a frsp of the copy of
// the last delta is missing here.
void Chain::fn_8043CA58() {
    ChainJoint_unk* tmp = new ((MemoryTag)3, "", "", 0) ChainJoint_unk[mNumJoints];
    for (int i = 0; i < mNumJoints; i++) {
        tmp[i] = mJoints.mData[i];
    }
    for (int i = 0; i < mNumJoints; i++) {
        mJoints.mData[i].m4C.y -= m340 * m33C;
        tmp[i].m04 += mJoints.mData[i].m4C;
    }
    int passes = 20;
    for (int it = 0; it < passes; it++) {
        tmp[0].m04 = m314;
        tmp[mNumJoints - 1].m04 = m320;
        for (int i = 0; i < mNumJoints - 1; i++) {
            ChainJoint_unk& a = tmp[i];
            ChainJoint_unk& b = tmp[i + 1];
            atn::Vector3 n;
            atn::Vector3 d = b.m04 - a.m04;
            n = d;
            float len = PSVECMag(&n);
            float k = 0.9f * (a.m58 * m33C - len);
            PSVECNormalize(&n, &n);
            n *= k;
            a.m04 -= n;
            b.m04 += n;
            a.m04.y = a.m04.y > mTarget->mPos.y ? a.m04.y : mTarget->mPos.y;
            b.m04.y = b.m04.y > mTarget->mPos.y ? b.m04.y : mTarget->mPos.y;
        }
    }
    tmp[0].m04 = m314;
    tmp[mNumJoints - 1].m04 = m320;
    for (int i = 0; i < mNumJoints; i++) {
        atn::Vector3 d = tmp[i].m04 - mJoints.mData[i].m04;
        tmp[i].m4C = d;
        mJoints.mData[i].m4C = d;
        mJoints.mData[i].m04 += tmp[i].m4C;
    }
    fn_8043CF58();
    for (int i = 0; i < mNumJoints; i++) {
        mJoints.mData[i].m4C = mJoints.mData[i].m04 - mJoints.mData[i].m10;
        mJoints.mData[i].m4C *= 0.9f;
        mJoints.mData[i].m10 = mJoints.mData[i].m04;
    }
    delete[] tmp;
}

// NONMATCHING: 91.0% - stack layout of the vector/quaternion temporaries (declaration order).
void Chain::fn_8043CF58() {
    atn::Matrix3x4 rot;
    for (int i = 0; i < mNumJoints - 1; i++) {
        atn::Vector3 d = mJoints.mData[i + 1].m04 - mJoints.mData[i].m04;
        atn::Vector3 dir = d;
        if (PSVECMag(&dir) > 0.00001f) {
            atn::Vector3 cur = dir;
            atn::Vector3 prev;
            if (i == 0) {
                atn::Vector3 p = mJoints2.mData[i + 1].m04 - mJoints2.mData[i].m04;
                prev = p;
            } else {
                atn::Vector3 p = mJoints.mData[i].m04 - mJoints.mData[i - 1].m04;
                prev = p;
            }
            PSVECNormalize(&prev, &prev);
            PSVECNormalize(&cur, &cur);
            atn::Vector3 c;
            PSVECCrossProduct(&prev, &cur, &c);
            atn::Vector3 axis = c;
            PSVECNormalize(&axis, &axis);
            float dot = PSVECDotProduct(&prev, &cur);
            dot = dot > 1.0f ? 1.0f : (dot < -1.0f ? -1.0f : dot);
            if (dot < 1.0f) {
                float a = acos(dot);
                rot.SetIdentity();
                if (i == 0) {
                    rot = mJoints2.mData[i].m1C;
                } else {
                    rot = mJoints.mData[i - 1].m1C;
                }
                rot.m[0][3] = 0.0f;
                rot.m[1][3] = 0.0f;
                rot.m[2][3] = 0.0f;
                atn::Quaternion_unk q;
                C_QUATMtx(&q, rot.m);
                atn::Quaternion_unk q0 = q;
                atn::Quaternion_unk r;
                if (PSVECMag(&axis) > 0.0f) {
                    atn::Quaternion_unk t;
                    C_QUATRotAxisRad(&t, &axis, a);
                    r = t;
                } else {
                    r = atn::Quaternion_unk(0.0f, 0.0f, 0.0f, 1.0f);
                }
                atn::Quaternion_unk res;
                PSQUATMultiply(&r, &q0, &res);
                atn::Quaternion_unk qr = res;
                Mtx m;
                PSMTXQuat(m, &qr);
                __MtxCopy(rot.m, m);
                rot.m[0][3] = mJoints.mData[i].m04.x;
                rot.m[1][3] = mJoints.mData[i].m04.y;
                rot.m[2][3] = mJoints.mData[i].m04.z;
                mJoints.mData[i].m1C = rot;
            } else {
                if (i != 0) {
                    mJoints.mData[i].m1C = mJoints.mData[i - 1].m1C;
                }
                mJoints.mData[i].m1C.m[0][3] = mJoints.mData[i].m04.x;
                mJoints.mData[i].m1C.m[1][3] = mJoints.mData[i].m04.y;
                mJoints.mData[i].m1C.m[2][3] = mJoints.mData[i].m04.z;
            }
        } else {
            if (i != 0) {
                mJoints.mData[i].m1C = mJoints.mData[i - 1].m1C;
            }
            mJoints.mData[i].m1C.m[0][3] = mJoints.mData[i].m04.x;
            mJoints.mData[i].m1C.m[1][3] = mJoints.mData[i].m04.y;
            mJoints.mData[i].m1C.m[2][3] = mJoints.mData[i].m04.z;
        }
    }
    mJoints.mData[mNumJoints - 1].m1C = rot;
    mJoints.mData[mNumJoints - 1].m1C.m[0][3] = mJoints.mData[mNumJoints - 1].m04.x;
    mJoints.mData[mNumJoints - 1].m1C.m[1][3] = mJoints.mData[mNumJoints - 1].m04.y;
    mJoints.mData[mNumJoints - 1].m1C.m[2][3] = mJoints.mData[mNumJoints - 1].m04.z;
    for (int i = 0; i < mNumJoints; i++) {
        MtxPtr node = mModel.GetModel_unk()->GetNodeMtx_unk(mJoints.mData[i].mNode);
        *(atn::Matrix3x4*)node = mJoints.mData[i].m1C;
    }
}

int Chain::vf40(GimmickMsg_unk* msg) {
    if (msg != 0) {
        m54 = msg->mType;
    }
    return 0;
}

void Chain::vf3C(void* lens) {
    mModel.GetModel_unk()->m1DC = (Lens_unk*)lens;
    mModel.GetModel_unk()->mFlags |= 0x10000;
}
} // namespace gmk
