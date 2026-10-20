#include "gmk/WaitObject.h"
#include "atn/Memory.h"

extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x18];
    int m18;
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: collision target of a state (the collision of mStates[]: position at 0x3C, radius at
// 0x48)
struct WaitTarget_unk {
    unsigned char m00[0x3C];
    atn::Vector3 mPos; // 0x3C
    float mRadius;     // 0x48
};

namespace gmk {

GimmickCommon* WaitObject::fn_80418EAC(atn::Task* parent, int id, int arg) {
    if (parent != 0) {
        return new (12, "", "", 0) WaitObject(parent, id, arg);
    }
    return 0;
}

GimmickCommon* WaitObject::fn_80418F30(atn::Task* parent, GimmickDesc_unk* desc) {
    if (parent != 0) {
        WaitObject* obj = new (12, "", "", 0) WaitObject(parent, desc->mId, desc->mArg);
        if (obj != 0) {
            obj->m6C = desc->mPos;
            obj->m78 = atn::Vector3(0.0f, desc->mRotY, 0.0f);
            obj->m90 = atn::Vector3(1.0f, 1.0f, 1.0f);
            obj->mDesc = desc;
        }
        return obj;
    }
    return 0;
}

// NONMATCHING: 98.84% - the zero is kept in r31 instead of r30 (register numbers only).
WaitObject::WaitObject(atn::Task* parent, int id, int arg)
    : GimmickCommon(parent, id, arg), mModel(8, 32), mNumTriggers(0), m7F8(0), m7FC(0.0f, 0.0f, 0.0f),
      m808(0.0f, 0.0f, 0.0f), mDesc(0) {
    m54 = 0;
    mE8 = 6;
    mEC = 0;
}

WaitObject::~WaitObject() {
    for (unsigned int i = 0; i < 7; i++) {
        if (mStates[i].mColli != 0) {
            delete mStates[i].mColli;
        }
    }
}

int WaitObject::vf0C() {
    if (fn_803EC91C() == 0) {
        fn_803EDB18(&mModel);
        for (int i = 0; i < 7; i++) {
            if (mStates[i].mColli != 0) {
                mStates[i].mColli->mFlags |= 8;
                mStates[i].mColli->m0C = (int)this;
                mStates[i].mColli->m20 = 0x808F8;
            }
        }
        m7FC = m6C;
        m808 = m78;
        return 1;
    }
    return 0;
}

// NONMATCHING: 97.56% - the original branches over a `b` for m54 == 0 (bne + b); tried if/else,
// nested ifs and a switch.
void WaitObject::vf14() {
    if (mDesc != 0 && mDesc->m7C == 0) {
        return;
    }
    if (m54 == 0) {
        return;
    }
    if (lbl_8087F0A8->m18 != 0 && (mStates[m54].mFlags & 2) != 2 && vf38() != 0) {
        fn_803ED774(vf38());
    }
}

// NONMATCHING: 99.85% - the safe-bool member pointer of boost::scoped_ptr is lbl_8078DC64 in the
// original (a local object here, @NNNN).
int WaitObject::vf1C() {
    if (mModel.fn_8008B140() != 0) {
        return 1;
    }
    if (mColli && mColli->fn_800580BC() != 0) {
        return 1;
    }
    for (int i = 0; i < 7; i++) {
        if (mEfp[i].fn_80237874() != 0) {
            return 1;
        }
    }
    return 0;
}

void WaitObject::vf70(int state) {
    GimmickMsg_unk msg(state);
    vf40(&msg);
    if (mE4 != 0) {
        mE4->m58 = 1;
    }
}

// NONMATCHING: 93.33% - the original reloads m54 in the loop and uses other registers.
int WaitObject::vf60() {
    if (m54 == 0) {
        return 0;
    }
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk& t = mTriggers[i];
        if (t.mState == m54 && t.mType == 4) {
            return 1;
        }
    }
    return 0;
}

// NONMATCHING: 99.94% - only .sdata2 order (vf10 and others are not decompiled yet).
int WaitObject::vf48(const atn::Vector3* pos, const atn::Vector3* dir, int flag) {
    if (vf60() != 0 && flag == 0) {
        atn::Vector3 d = m6C - *pos;
        float dist = fn_805F9920(&d);
        if (dist > 0.0f) {
            PSVECNormalize(&d, &d);
        }
        for (unsigned int i = 0; i < mNumTriggers; i++) {
            WaitTrigger_unk& t = mTriggers[i];
            if (t.mState == m54 && t.mType == 4) {
                float radius = *(float*)t.m08;
                if (dist < radius * radius) {
                    return PSVECDotProduct(&d, dir) > 0.5f;
                }
            }
        }
    }
    return 0;
}

void WaitObject::vf4C(CharaTask* chara) {
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk& t = mTriggers[i];
        if (t.mState == m54 && t.mType == 4) {
            GimmickMsg_unk msg(t.mNext);
            vf40(&msg);
        }
    }
}

// NONMATCHING: 97.50% - the original reloads the collision pointer for the return.
int WaitObject::vf50() {
    for (unsigned int i = 0; i < mNumTriggers; i++) {
        WaitTrigger_unk& t = mTriggers[i];
        if (t.mState == m54 && t.mType == 5 && GetColli_unk(m54) != 0) {
            return (int)GetColli_unk(m54);
        }
    }
    return 0;
}

atn::Vector3 WaitObject::vf54() {
    WaitTarget_unk* target = (WaitTarget_unk*)vf50();
    if (target != 0) {
        return target->mPos;
    }
    return lbl_807C7030;
}

float WaitObject::vf58() {
    WaitTarget_unk* target = (WaitTarget_unk*)vf50();
    return target != 0 ? target->mRadius : 0.0f;
}

int WaitObject::vf5C() {
    return vf58() > 0.0f;
}

Model_unk* WaitObject::vf38() {
    return &mModel;
}

} // namespace gmk
