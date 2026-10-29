#include "game/CostumeEditChara.h"
#include "game/CharaTask.h"
#include "game/EquipItemData.h"
#include "atn/Memory.h"
#include <boost/function.hpp>

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path); // inferred: d2anime task
extern "C" void fn_8008AD4C(atn::Model* model, const char* path, const char* texture); // inferred: loads a model
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" void PSMTXTrans(Mtx m, float x, float y, float z);
extern "C" void PSMTXScale(Mtx m, float x, float y, float z);
extern "C" void fn_80097CCC(atn::Model* model, int arg, float value);
extern "C" unsigned int fn_800119B8(); // inferred: number of equipment slots
extern "C" void fn_80565F34(chr::CharaEquip_unk* equip); // inferred: draws the equipment
extern "C" void fn_801F6D7C(atn::Task* task, const char* name, int index, float value);
extern "C" void fn_804A4CE4(atn::Task* task, int face, int a, int b);
// inferred: camera (lbl_8087EFB4)
struct CostumeEditCamera_unk {
    unsigned char m000[0x10C];
    atn::Vector3 mPos; // 0x10C
};
extern "C" CostumeEditCamera_unk* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, CostumeEditCamera_unk* camera, const atn::Vector3* pos); // inferred: to screen
// inferred: menu cursor (lbl_8087F580)
struct CostumeEditCursor_unk {
    unsigned char m000[0x1EC];
    atn::Vector2_unk mOffset; // 0x1EC
    atn::Vector2_unk mSize;   // 0x1F4
};
extern "C" CostumeEditCursor_unk* lbl_8087F580;

#define __MAX_unk(a, b) ((a) > (b) ? (a) : (b))

// inferred: linear interpolation written as (b - a) * t + a (name invented)
inline atn::Vector3 __Lerp_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

// inferred: length of the column `i` of a matrix (name invented)
inline float __ColumnLength_unk(const atn::Matrix3x4& m, int i) {
    atn::Vector3 v(m.m[0][i], m.m[1][i], m.m[2][i]);
    return PSVECMag(&v);
}

// inferred: largest scale axis of a matrix (name invented)
inline float __MaxScaleT_unk(const atn::Matrix3x4& m) {
    float sz = __ColumnLength_unk(m, 2);
    float sy = __ColumnLength_unk(m, 1);
    float sx = __ColumnLength_unk(m, 0);
    atn::Vector3 s(sx, sy, sz);
    return __MAX_unk(s.x, __MAX_unk(s.y, s.z));
}

// inferred: rotation matrix y * x * z from euler angles (name invented; as in CharaTask.cpp)
inline atn::Matrix3x4& __MakeRotationYXZ_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    m.SetIdentity();
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
    if (r.z != 0.0f) {
        Mtx t;
        Mtx a;
        PSMTXRotRad(a, 'z', r.z);
        PSMTXConcat(m.m, a, t);
        __MtxCopy(m.m, t);
    }
    return m;
}

// inferred: m = m * rotation(r) (name invented)
inline void __MtxRotateYXZ_unk(atn::Matrix3x4& m, const atn::Vector3& r) {
    Mtx t;
    atn::Matrix3x4 rot;
    PSMTXConcat(m.m, __MakeRotationYXZ_unk(rot, r).m, t);
    __MtxCopy(m.m, t);
}

// inferred: m = translation(p) (name invented)
inline void __MtxSetTrans_unk(atn::Matrix3x4& m, const atn::Vector3& p) {
    Mtx trans;
    PSMTXTrans(trans, p.x, p.y, p.z);
    __MtxCopy(m.m, trans);
}
// inferred: m = m * scale(s) (name invented)
inline void __MtxScale_unk(atn::Matrix3x4& m, const atn::Vector3& s) {
    Mtx t;
    Mtx scale;
    PSMTXScale(scale, s.x, s.y, s.z);
    PSMTXConcat(m.m, scale, t);
    __MtxCopy(m.m, t);
}

CostumeEditChara* CostumeEditChara::fn_801E7FF8(atn::Task* parent, CharaTask* chara, int face) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) CostumeEditChara(parent, chara, face);
    }
    return 0;
}

CostumeEditChara::CostumeEditChara(atn::Task* parent, CharaTask* chara, int face)
    : atn::Task(parent), mPos(0.0f, 0.0f, 0.0f), mRot(0.0f, 0.0f, 0.0f), mScale(1.0f, 1.0f, 1.0f), mShadow(1, 0x20),
      mFaceIcon(0) {
    mFace = face;
    mOffsetTarget = lbl_807C7030;
    mTarget = lbl_807C7030;
    mCur = lbl_807C7030;
    mOffset = mOffsetTarget;
    mChara = chara;
    mMtx.SetIdentity();
    if (chara == 0) {
        (mFaceIcon = (CostumeEditFace_unk*)fn_801F64D0(this, "data/d2anime/equip_face.d2a"))->fn_800D246C(1);
    }
    fn_8008AD4C(&mShadow, "/map/cosedit/hk_shadow1.model", 0);
}

CostumeEditChara::~CostumeEditChara() {}

int CostumeEditChara::vf0C() {
    if (fn_800D3FA4() != 0) {
        if (mChara != 0) {
            mChara->mMotion.m34C = 1;
            mChara->mMotion.mTracks[0].m20 = 1.0f;
            mChara->mMotion.mTracks[0].m0C = 1.0f;
            mChara->mMotion.fn_80097C08(0, 2, 1, 0, 1, 1.0f, 0.1f);
            atn::Model* motion = &mChara->mMotion;
            __MtxCopy(motion->mMtx, mMtx.m);
            motion->m54 = __MaxScaleT_unk(mMtx);
            mChara->mMotion.fn_80097E80(1);
            fn_805F9920(&(lbl_8087EFB4->mPos - mMtx.GetTrans_unk()));
            mChara->fn_80148B38();
            mChara->mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
            mChara->fn_80147A18(0);
            CharaTask* chara = mChara;
            fn_80097CCC(&chara->mMotion, 3, 0.0f);
            chara->m12BC = -1;
            chara = mChara;
            chara->m580 = 0.0f;
            chara->m584 = 0.0f;
            mChara->mLook.fn_80129A48(1.0f);
            mChara->mLook.fn_80129A6C();
        }
        if (mFaceIcon != 0) {
            mFaceIcon->fn_800D246C(0);
            mFaceIcon->m54 = -2.0f;
            mFaceIcon->m4D = 0;
            mFaceIcon->m50 = 0.0f;
            mFaceIcon->mFlags |= 4;
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.99% - the first store of the translation copy uses 0x50(r31) instead of the
// mMtx pointer kept in r30. Tried: the copy through a pointer/MtxPtr, operator=, one TRS helper.
void CostumeEditChara::vf10() {
    mOffset = __Lerp_unk(mOffset, mOffsetTarget, 0.3f);
    mCur = __Lerp_unk(mCur, mTarget, 0.3f);
    mPos = mCur + mOffset;
    if (mChara != 0) {
        __MtxSetTrans_unk(mMtx, mPos);
        __MtxRotateYXZ_unk(mMtx, mRot);
        __MtxScale_unk(mMtx, mScale);
        CharaTask* chara = mChara;
        __MtxCopy(chara->mMotion.mMtx, mMtx.m);
        chara->mMotion.m54 = __MaxScaleT_unk(mMtx);
        atn::Model* motion = &mChara->mMotion;
        motion->m20C_15_8 = motion->m50;
        motion->m50 = 0;
        mChara->mMotion.fn_80097E80(1);
        mChara->mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
        fn_805F9920(&(lbl_8087EFB4->mPos - mMtx.GetTrans_unk()));
        mChara->fn_80148B38();
        if (mShadow.fn_8008B140() == 0) {
            __MtxCopy(mShadow.mMtx, mMtx.m);
            mShadow.m54 = __MaxScaleT_unk(mMtx);
            mShadow.CalcMatrixStack(atn::ModelNodeCallback_unk());
        }
        if (!fn_801E91B4() || mVisible == 0) {
            mChara->fn_8016E800(0);
        }
    }
    if (mFaceIcon != 0) {
        if (fn_801E91B4() && mVisible != 0) {
            mFaceIcon->m54 = 2.0f;
        } else {
            mFaceIcon->m54 = -2.0f;
        }
        atn::Vector3 screen;
        fn_800BFAC8(&screen, lbl_8087EFB4, &mPos);
        screen.y = 220.0f;
        atn::Vector2_unk offset = lbl_8087F580->mOffset;
        atn::Vector2_unk size = lbl_8087F580->mSize;
        fn_801F6D7C(mFaceIcon, "BasePos", 0, screen.x + offset.x);
        fn_801F6D7C(mFaceIcon, "BasePos", 1, screen.y + offset.y);
        fn_801F6D7C(mFaceIcon, "BasePos", 2, size.x);
        fn_801F6D7C(mFaceIcon, "BasePos", 3, size.y);
        fn_804A4CE4(mFaceIcon, mFace, 4, 2);
        if (mShadow.fn_8008B140() == 0) {
            atn::Matrix3x4 mtx;
            mtx.SetIdentity();
            mtx.m[0][3] = mTarget.x;
            mtx.m[1][3] = mTarget.y;
            mtx.m[2][3] = mTarget.z;
            __MtxCopy(mShadow.mMtx, mtx.m);
            mShadow.m54 = __MaxScaleT_unk(mtx);
            mShadow.CalcMatrixStack(atn::ModelNodeCallback_unk());
        }
    }
}

// NONMATCHING: 98.9% - in the first loop the equipment slot is read with add + lwz 0x680 instead
// of addi 0x680 + lwzx. Tried: pointer to the array, *(p + i), inline accessors (int/unsigned index).
void CostumeEditChara::vf14() {
    if (mVisible != 0) {
        if (mChara != 0) {
            mChara->mMotion.mFlags &= ~0x80000;
            for (unsigned int i = 0; i < fn_800119B8(); i++) {
                chr::CharaEquip_unk* equip = mChara->m680[i];
                if (equip != 0) {
                    *(unsigned int*)((char*)equip + 0x28) &= ~0x80000;
                }
            }
            atn::Model::Float4_unk color;
            color.m00 = 1.0f;
            color.m04 = 1.0f;
            color.m08 = 1.0f;
            color.m0C = 1.0f;
            mChara->mMotion.m40 = color;
            mChara->mMotion.fn_8008CD60();
            chr::CharaEquip_unk** equips = mChara->m680;
            for (unsigned int i = 0; i < 8; i++) {
                if (equips[i] != 0 && ((1u << i) & mChara->m6A0) == (1u << i)) {
                    fn_80565F34(equips[i]);
                }
            }
            for (unsigned int i = 0; i < 4; i++) {
                EquipItemData* item = (int)i < (int)mChara->m6A4.mCount ? mChara->m6A4.mData[i] : 0;
                if (item != 0 && item->mFlags.mFlag) {
                    item->vf0C();
                }
            }
        }
        if (mShadow.fn_8008B140() == 0) {
            mShadow.fn_8008CD60();
        }
    }
    if (mFaceIcon != 0) {
        if (mVisible != 0) {
            mFaceIcon->mFlags &= ~4;
        } else {
            mFaceIcon->mFlags |= 4;
        }
    }
}

void CostumeEditChara::fn_801E9164() {}

// NONMATCHING: ~60% - the original stores z before x/y (psq_l, lfs, stfs z, psq_st); Vec.h has no
// copy helper with that order (__VecCopy stores x/y first, Assign_unk loads z first).
void CostumeEditChara::fn_801E9168(const atn::Vector3& pos) {
    mTarget = pos;
}

// NONMATCHING: ~60% - the original stores z before x/y (psq_l, lfs, stfs z, psq_st); Vec.h has no
// copy helper with that order (__VecCopy stores x/y first, Assign_unk loads z first).
void CostumeEditChara::fn_801E917C(const atn::Vector3& pos) {
    mCur = pos;
    mTarget = pos;
}

// NONMATCHING: ~60% - the original stores z before x/y (psq_l, lfs, stfs z, psq_st); Vec.h has no
// copy helper with that order (__VecCopy stores x/y first, Assign_unk loads z first).
void CostumeEditChara::fn_801E91A0(const atn::Vector3& rot) {
    mRot = rot;
}

bool CostumeEditChara::fn_801E91B4() {
    return fn_805F9920(&(mCur - mTarget)) < 900.0f;
}
