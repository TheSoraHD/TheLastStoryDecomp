#include "game/CharaStatus.h"
#include "atn/Model_unk.h"
#include "chr/ControlBase.h"
#include "game/CharaTask.h"
#include "game/ResHandle.h"

// inferred: debug menu entries; the last two arguments are a callback and its argument
typedef void (*MenuCallback_unk)(void* arg);
extern "C" void fn_800874C8(void* group, const char* name, int* value, int min, int max, int step,
                            MenuCallback_unk callback, void* arg);
extern "C" void fn_80087E9C(void* group, const char* name, atn::Vector3* value, MenuCallback_unk callback,
                            void* arg, float min, float max, float step);
extern "C" void fn_8008771C(void* group, const char* name, float* value, MenuCallback_unk callback, void* arg,
                            float min, float max, float step);

CharaLook_unk::CharaLook_unk()
    : mChara(0), mType(0), mTarget(0), mOffset(0.0f, 0.0f, 0.0f), mRot(0.0f, 0.0f, 0.0f),
      mLimit(0.62831855f, 0.7853982f, 0.0f), mNeck(0), m34(0), m35(0), m36(1), mSpeed(0.3f), mEyeType(0),
      mEyeTarget(0), mEyeOffset(0.0f, 0.0f, 0.0f), mEyeSpeed(0.3f) {
    m3C = 0.0f;
    m40 = 0.0f;
    m44 = 0.0f;
    m48 = 0.0f;
    mEyeRot[0].x = 0.0f;
    mEyeRot[0].y = 0.0f;
    mEyeRot[0].z = 0.0f;
    mEyeRot[1].x = 0.0f;
    mEyeRot[1].y = 0.0f;
    mEyeRot[1].z = 0.0f;
}

CharaLook_unk::~CharaLook_unk() {}

void CharaLook_unk::fn_801298EC(chr::Chara_unk* chara) {
    mChara = chara;
    mNeck = ((atn::Model*)&chara->mMotion)->GetNodeNoByName("neck", 0);
}

void CharaLook_unk::fn_80129930(const atn::Vector3& pos, float speed) {
    mOffset = pos;
    mType = 2;
    mSpeed = speed;
}

void CharaLook_unk::fn_80129954(const atn::Vector3& pos, float speed) {
    mOffset = pos;
    mType = 3;
    mSpeed = speed;
}

void CharaLook_unk::fn_80129978(atn::Model* model, const char* node, const atn::Vector3& offset, float speed) {
    if (node != 0) {
        // inferred: written without atn::Model::GetNodeMtx_unk, whose weak copy is emitted after
        // fn_80129A6C (the first function that refers to it)
        int index = model->GetNodeNoByName(node, 0);
        mTarget = (index < 0) ? 0 : (float*)model->mNodeMtx[index];
        mOffset = offset;
        mType = 1;
        mSpeed = speed;
    } else {
        mTarget = (float*)model->mMtx;
        mOffset = offset;
        mType = 1;
        mSpeed = speed;
    }
}

void CharaLook_unk::fn_80129A48(float speed) {
    mTarget = 0;
    mOffset.x = 0.0f;
    mOffset.y = 0.0f;
    mOffset.z = 0.0f;
    mType = 0;
    mSpeed = speed;
}

void CharaLook_unk::fn_80129A6C() {
    atn::Vector3 rot(0.0f, 0.0f, 0.0f);
    atn::Vector3 limit(mLimit);
    m3C = 0.0f;
    m40 = 0.0f;
    m44 = 0.0f;
    m48 = 0.0f;
    switch (m36) {
    case 0:
        m3C = 1.0f;
        break;
    case 1:
        m3C = 0.5f;
        m40 = 0.5f;
        break;
    case 2:
        m3C = 0.4f;
        m40 = 0.4f;
        m44 = 0.2f;
        limit *= 1.5f;
        break;
    case 3:
        m3C = 0.35f;
        m40 = 0.35f;
        m44 = 0.1f;
        limit *= 2.0f;
        break;
    }
    if (mType == 3) {
        rot = mOffset;
        if (atn::Abs_unk(rot.y) > mLimit.y) {
            rot.y = rot.y > 0.0f ? mLimit.y : -mLimit.y;
        }
        if (atn::Abs_unk(rot.x) > mLimit.x) {
            rot.x = rot.x > 0.0f ? mLimit.x : -mLimit.x;
        }
    } else if (mType != 0) {
        atn::Vector3 target(mOffset);
        if (mType == 1 && mTarget != 0) {
            target += ((atn::Matrix3x4*)mTarget)->GetTrans_unk();
        }
        atn::Vector3 dir;
        int node = mNeck;
        if (node < 0) {
            rot = *mChara->GetRot_unk();
        } else {
            dir = target - ((atn::Matrix3x4*)mChara->GetModel_unk()->GetNodeMtx_unk(node))->GetTrans_unk();
            if (dir.IsZero_unk()) {
                rot = *mChara->GetRot_unk();
            } else {
                dir.Normalize_unk();
                rot = dir.ToRotation_unk();
            }
        }
        rot.Apply_unk(atn::WrapAngle_unk);
        rot.y -= mChara->GetRot_unk()->y;
        rot.Apply_unk(atn::WrapAngle_unk);
        rot *= 0.7f;
        if (rot.x > 0.0f) {
            rot.x *= 0.8f;
        }
        if (atn::Abs_unk(rot.y) > limit.y) {
            rot.y = rot.y > 0.0f ? limit.y : -limit.y;
        }
        if (atn::Abs_unk(rot.x) > limit.x) {
            rot.x = rot.x > 0.0f ? limit.x : -limit.x;
        }
    }
    mRot = atn::Lerp_unk(mRot, rot, mSpeed);
    m34 = 0;
    if (mType == 3) {
        m34 = 1;
    } else if (atn::Abs_unk(mRot.x) > 0.001f || atn::Abs_unk(mRot.y) > 0.001f) {
        m34 = 1;
    }
    atn::Vector3 eyeMax(atn::DegToRad_unk(10.0f), atn::DegToRad_unk(25.0f), 0.0f);
    atn::Vector3 eyeMin(atn::DegToRad_unk(-15.0f), atn::DegToRad_unk(-25.0f), 0.0f);
    atn::Vector3 eyeRot[2];
    if (mEyeType == 3) {
        eyeRot[0] = mEyeOffset;
        eyeRot[1] = mEyeOffset;
    } else if (mEyeType != 0) {
        atn::Vector3 target(mEyeOffset);
        if (mEyeType == 1 && mEyeTarget != 0) {
            target += ((atn::Matrix3x4*)mEyeTarget)->GetTrans_unk();
        }
        int nodes[2];
        nodes[0] = mChara->GetModel_unk()->GetNodeNoByName("eye_L", 0);
        nodes[1] = mChara->GetModel_unk()->GetNodeNoByName("eye_R", 0);
        int node;
        for (int i = 0; i < 2; i++) {
            atn::Vector3 dir;
            node = nodes[i];
            if (node < 0) {
                eyeRot[i] = *mChara->GetRot_unk();
            } else {
                dir = target - ((atn::Matrix3x4*)mChara->GetModel_unk()->GetNodeMtx_unk(node))->GetTrans_unk();
                if (dir.IsZero_unk()) {
                    eyeRot[i] = *mChara->GetRot_unk();
                } else {
                    dir.Normalize_unk();
                    eyeRot[i] = dir.ToRotation_unk();
                }
            }
            eyeRot[i].Apply_unk(atn::WrapAngle_unk);
            atn::Vector3 front(0.0f, 0.0f, 10.0f);
            front.fn_80011410(
                ((atn::Matrix3x4*)mChara->GetModel_unk()->GetNodeMtx_unk("head"))->GetRotation_unk());
            atn::Vector3 head = front.ToRotation_unk();
            head.Apply_unk(atn::WrapAngle_unk);
            eyeRot[i].y -= head.y;
            eyeRot[i].Apply_unk(atn::WrapAngle_unk);
            eyeRot[i] *= 0.9f;
            eyeRot[i].y = (eyeMax.y < (eyeMin.y > eyeRot[i].y ? eyeMin.y : eyeRot[i].y))
                              ? eyeMax.y
                              : (eyeMin.y > eyeRot[i].y ? eyeMin.y : eyeRot[i].y);
            eyeRot[i].x = (eyeMax.x < (eyeMin.x > eyeRot[i].x ? eyeMin.x : eyeRot[i].x))
                              ? eyeMax.x
                              : (eyeMin.x > eyeRot[i].x ? eyeMin.x : eyeRot[i].x);
        }
    } else {
        eyeRot[0].Set_unk(0.0f, 0.0f, 0.0f);
        eyeRot[1].Set_unk(0.0f, 0.0f, 0.0f);
    }
    m35 = 0;
    for (int i = 0; i < 2; i++) {
        mEyeRot[i] = atn::Lerp_unk(mEyeRot[i], eyeRot[i], mEyeSpeed);
        if (mEyeType == 3) {
            m35 = 1;
        } else if (atn::Abs_unk(mEyeRot[i].x) > 0.001f || atn::Abs_unk(mEyeRot[i].y) > 0.001f) {
            m35 = 1;
        }
    }
}

extern "C" int strcmp(const char* a, const char* b);

// inferred: rotations used by the node hook; names invented
inline atn::Matrix3x4 RotMtx_unk(char axis, float rad) {
    atn::Matrix3x4 r;
    PSMTXRotRad(r.m, axis, rad);
    return r;
}

inline atn::Matrix3x4 MulMtx_unk(const atn::Matrix3x4& a, const atn::Matrix3x4& b) {
    atn::Matrix3x4 r;
    PSMTXConcat(a.m, b.m, r.m);
    return r;
}

inline void RotateMtx_unk(atn::Matrix3x4& m, char axis, float rad) {
    m = MulMtx_unk(m, RotMtx_unk(axis, rad));
}

// The pre-rotation that keeps the translation is written in place (its rotation matrix is a
// temporary of this function: it sits on the stack before the temporaries of RotateMtx_unk).
int CharaLook_unk::fn_8012A288(atn::Matrix3x4* mtx, ModelNodeDesc_unk* node) {
    if (m34 != 0) {
        const char* name = node->mName;
        if (strcmp(name, "head") == 0) {
            atn::Matrix3x4 m(*mtx);
            RotateMtx_unk(m, 'y', -mRot.x * m3C);
            if (mType == 3) {
                RotateMtx_unk(m, 'z', mRot.z * m3C);
            }
            *mtx = m;
            atn::Vector3 trans = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            PSMTXConcat(RotMtx_unk('y', mRot.y * m3C).m, mtx->m, mtx->m);
            mtx->m[0][3] = trans.x;
            mtx->m[1][3] = trans.y;
            mtx->m[2][3] = trans.z;
            return 1;
        }
        int count = m36;
        if (count >= 1 && strcmp(name, "neck") == 0) {
            atn::Matrix3x4 m(*mtx);
            RotateMtx_unk(m, 'y', -mRot.x * m40);
            if (mType == 3) {
                RotateMtx_unk(m, 'z', mRot.z * m40);
            }
            *mtx = m;
            atn::Vector3 trans = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            PSMTXConcat(RotMtx_unk('y', mRot.y * m40).m, mtx->m, mtx->m);
            mtx->m[0][3] = trans.x;
            mtx->m[1][3] = trans.y;
            mtx->m[2][3] = trans.z;
            return 1;
        }
        if (count >= 2 && strcmp(name, "spine1") == 0) {
            atn::Matrix3x4 m(*mtx);
            RotateMtx_unk(m, 'y', -mRot.x * m44);
            *mtx = m;
            atn::Vector3 trans = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            PSMTXConcat(RotMtx_unk('y', mRot.y * m44).m, mtx->m, mtx->m);
            mtx->m[0][3] = trans.x;
            mtx->m[1][3] = trans.y;
            mtx->m[2][3] = trans.z;
            return 1;
        }
        if (count >= 3 && strcmp(name, "spine") == 0) {
            atn::Matrix3x4 m(*mtx);
            RotateMtx_unk(m, 'y', -mRot.x * m48);
            *mtx = m;
            atn::Vector3 trans = mtx->GetTrans_unk();
            mtx->m[0][3] = 0.0f;
            mtx->m[1][3] = 0.0f;
            mtx->m[2][3] = 0.0f;
            PSMTXConcat(RotMtx_unk('y', mRot.y * m48).m, mtx->m, mtx->m);
            mtx->m[0][3] = trans.x;
            mtx->m[1][3] = trans.y;
            mtx->m[2][3] = trans.z;
            return 1;
        }
    }
    if (m35 != 0) {
        const char* name = node->mName;
        if (strcmp(name, "eye_L") == 0) {
            atn::Matrix3x4 m(*mtx);
            RotateMtx_unk(m, 'x', mEyeRot[0].x);
            if (mType == 3) {
                RotateMtx_unk(m, 'z', mEyeRot[0].z);
            }
            RotateMtx_unk(m, 'y', mEyeRot[0].y);
            *mtx = m;
            return 1;
        }
        if (strcmp(name, "eye_R") == 0) {
            atn::Matrix3x4 m(*mtx);
            RotateMtx_unk(m, 'x', mEyeRot[1].x);
            if (mType == 3) {
                RotateMtx_unk(m, 'z', mEyeRot[1].z);
            }
            RotateMtx_unk(m, 'y', mEyeRot[1].y);
            *mtx = m;
            return 1;
        }
    }
    return 0;
}

void CharaLook_unk::fn_8012ABD0(const atn::Vector3& pos, float speed) {
    mEyeOffset = pos;
    mEyeType = 3;
    mSpeed = speed;
}

void CharaLook_unk::fn_8012ABF4(const atn::Vector3& pos, float speed) {
    mEyeOffset = pos;
    mEyeType = 2;
    mEyeSpeed = speed;
}

void CharaLook_unk::fn_8012AC18(float* target, const atn::Vector3& pos, float speed) {
    mEyeTarget = target;
    mEyeOffset = pos;
    mEyeType = 1;
    mEyeSpeed = speed;
}

void CharaLook_unk::fn_8012AC40(float speed) {
    mEyeTarget = 0;
    mEyeOffset.x = 0.0f;
    mEyeOffset.y = 0.0f;
    mEyeOffset.z = 0.0f;
    mEyeType = 0;
    mEyeSpeed = speed;
}

// inferred: debug menu callback, applies the new values at once
void fn_8012AC64(CharaLook_unk* look) {
    float speed = look->mSpeed;
    float eyeSpeed = look->mEyeSpeed;
    look->mSpeed = 1.0f;
    look->mEyeSpeed = 1.0f;
    look->fn_80129A6C();
    ((atn::Model*)&look->mChara->mMotion)->CalcMatrixStack(atn::ModelNodeCallback_unk());
    look->mSpeed = speed;
    look->mEyeSpeed = eyeSpeed;
}

void CharaLook_unk::fn_8012AD20(void* menu) {
    void* group = fn_8008937C(menu, "Sight");
    fn_800874C8(group, "type", &mType, 0, 3, 1, (MenuCallback_unk)fn_8012AC64, this);
    fn_80087E9C(group, "offset", &mOffset, (MenuCallback_unk)fn_8012AC64, this, -100000.0f, 100000.0f, 1.0f);
    fn_8008771C(group, "speed", &mSpeed, 0, 0, 0.0f, 1.0f, 0.1f);
    fn_800874C8(group, "eyeType", &mEyeType, 0, 3, 1, (MenuCallback_unk)fn_8012AC64, this);
    fn_80087E9C(group, "eyeOffset", &mEyeOffset, (MenuCallback_unk)fn_8012AC64, this, -100000.0f, 100000.0f, 1.0f);
    fn_8008771C(group, "eyeSpeed", &mEyeSpeed, 0, 0, 0.0f, 1.0f, 0.1f);
}
