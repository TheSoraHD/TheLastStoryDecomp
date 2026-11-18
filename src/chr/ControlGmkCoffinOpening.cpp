#include "chr/ControlGmkCoffinOpening.h"
#include "game/GameVoice.h"
#include "game/CharaTask.h"
#include <boost/bind.hpp>

extern "C" double atan2(double y, double x);

float lbl_8087DA08 = -0.5f; // inferred: ground height used by ControlGmkCoffinGetUp (.sdata, global)
extern "C" void fn_80547708(void* arg, chr::GmkTarget_unk* target);

// inferred: weak copy 0x8013C38C (address of the position), inlined here
inline atn::Vector3* CharaTask::fn_8013C38C() {
    return &mPos;
}

namespace chr {

ControlGmkCoffinOpening::ControlGmkCoffinOpening(Chara_unk* chara, const atn::Vector3& pos, float rotY)
    : ControlBase(chara), mState(0) {
    chara->m58C = 0;
    Chara_unk* c1 = mChara;
    c1->m560 = 0x5B;
    mChara->mPos = pos;
    mChara->mRot = atn::Vector3(0.0f, rotY, 0.0f);
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x231, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

int ControlGmkCoffinOpening::vf0C() {
    if (mState == 0) {
        Motion_unk* motion = &mChara->mMotion;
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            motion->m34C = 1;
            motion->mTracks[0].m20 = 1.0f;
            motion->fn_80097C08(0, 0x232, 1, 0, 1, 0.0f, 0.1f);
            motion->mTracks[0].m0C = 1.0f;
            mState = 1;
        }
    }
    return 0;
}

ControlGmkCoffinOpen::ControlGmkCoffinOpen(Chara_unk* chara, const atn::Vector3& pos, int alt, float rotY)
    : ControlBase(chara) {
    chara->m58C = 0;
    Chara_unk* c1 = mChara;
    c1->m560 = 0x5C;
    Chara_unk* c2 = mChara;
    c2->mPos = pos;
    mChara->mRot = atn::Vector3(0.0f, rotY, 0.0f);
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    if (alt) {
        motion->fn_80097C08(0, 0x234, 0, 0, 1, 0.0f, 0.1f);
    } else {
        motion->fn_80097C08(0, 0x233, 0, 0, 1, 0.0f, 0.1f);
    }
    motion->mTracks[0].m0C = 1.0f;
}

int ControlGmkCoffinOpen::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    return frame >= motion->fn_80097D7C(0);
}

ControlGmkCoffinGetUp::ControlGmkCoffinGetUp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& rot)
    : ControlBase(chara), mWait(0x3C), mState(0) {
    chara->m58C = 0;
    mChara->m560 = 0x5D;
    mChara->mColli.mFlags &= ~1;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    if (mChara->mType == 2) {
        motion->fn_80097C08(0, 0x230, 0, 0, 1, 1.0f, 0.1f);
    } else {
        motion->fn_80097C08(0, 0x22F, 0, 0, 1, 1.0f, 0.1f);
    }
    motion->mTracks[0].m0C = 0.0f;
    mChara->mPos = pos;
    mChara->mRot = rot;
    ((CharaTask*)mChara)->fn_80145334();
    if (lbl_8087F498 != 0) {
        lbl_8087F498->fn_803EA77C(mChara, 0xE, 0, 1.0f, 10.0f);
    }
}

int ControlGmkCoffinGetUp::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mState == 0) {
        if (--mWait < 0) {
            if (mChara->mType == 2) {
                motion->mTracks[0].m0C = 1.0f;
            } else {
                motion->mTracks[0].m0C = 0.7f;
            }
            mState = 1;
        }
    } else if (mState == 1) {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            mChara->mColli.mFlags |= 1;
            mChara->fn_8015495C(0, 1, 1, 1);
            Motion_unk* m = &mChara->mMotion;
            m->m34C = 1;
            m->mTracks[0].m20 = 1.0f;
            m->fn_80097C08(0, 2, 0, 0, 1, 1.0f, 0.1f);
            m->mTracks[0].m0C = 1.0f;
            atn::Vector3 ofs(-16.0f, 0.0f, -18.0f);
            Mtx mtx;
            PSMTXRotRad(mtx, 'y', mChara->mRot.y);
            PSMTXMultVec(mtx, &ofs, &ofs);
            Chara_unk* chara = mChara;
            atn::Vector3 np = chara->mPos + ofs;
            chara->mPos = np;
            ((CharaTask*)mChara)->fn_80145334();
            return 1;
        }
        if (motion->mTracks[0].m08 >= 3.0f) {
            atn::Vector3 pos = mChara->mPos;
            pos.y += (lbl_8087DA08 - pos.y) * 0.25f;
            mChara->mPos = pos;
        }
    }
    return 0;
}

ControlGmkTrapBat::ControlGmkTrapBat(Chara_unk* chara, float damage) : ControlBase(chara), mDamage(damage) {
    chara->m58C = 0;
    mChara->m560 = 0x5E;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x236, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

int ControlGmkTrapBat::vf0C() {
    mChara->mStatus.mParamB.m8C -= mDamage;
    if (mChara->mStatus.m04 <= 2.0f) {
        mChara->vf44();
        return 1;
    }
    return 0;
}

boost::function<void()> ControlGmkTrapBat::vf10() const {
    if ((mChara->mStatus.m0C & 0x20) == 0x20) {
        atn::Vector3 dir(0.0f, 0.0f, -0.01f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', mChara->mRot.y);
        PSMTXMultVec(mtx, &dir, &dir);
        return boost::bind(&CharaTask::fn_8015E7A0, (CharaTask*)mChara, dir, -1, false);
    }
    return ControlBase::vf10();
}

// (turns the character towards the target with Vector3::ToRotationZ_unk; the pitch is discarded)
ControlGmkBreakArmor::ControlGmkBreakArmor(Chara_unk* chara, GmkTarget_unk* target, void* arg)
    : ControlBase(chara), mTarget(target), m0C(arg) {
    chara->m58C = 0;
    mChara->m560 = 0x5F;
    if (mTarget != 0) {
        atn::Vector3 dir = mTarget->m6C - *mChara->fn_8013C38C();
        atn::Vector3 rot = mChara->mRot;
        rot.y = dir.ToRotationZ_unk().y;
        mChara->mRot = rot;
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->HasC48_unk()) {
        mChara->fn_80154344();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0xB, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

int ControlGmkBreakArmor::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    float cur = motion->mTracks[0].m08;
    if ((float)__fabs(cur - 18.0f) < 1.1920929e-7f) {
        if (m0C != 0) {
            fn_80547708(m0C, mTarget);
        }
    } else if ((float)__fabs(cur - 20.0f) < 1.1920929e-7f) {
        if (mTarget != 0) {
            mTarget->vf70(1);
        }
    }
    return 0;
}

} // namespace chr
