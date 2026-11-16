#include "chr/ControlSitting.h"

// inferred: linear interpolation with the product first (name invented)
inline atn::Vector3 LerpSit_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

namespace chr {

ControlSitting::ControlSitting(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& seat, const atn::Vector3& rot)
    : ControlBase(chara) {
    chara->m560 = 0x3D;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, 0x78, 0, 0, 1, 0.0f, 0.1f);
    mStart = start;
    mStartRot = mChara->mRot;
    mSeat = seat;
    mSeat.y = start.y;
    mRot = rot;
    mCount = 8;
    mChara->mPos = start;
    atn::Vector3 zero(0.0f, 0.0f, 0.0f);
    mChara->m574 = zero;
    mChara->m570 = 0.0f;
    Chara_unk* c = mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
}

// inferred: check of the stick direction that the game never calls (the linker removes it). Its
// constants are created before those of vf0C, which is the .sdata2 order of the original: pi/2, the
// WrapAngle_unk constants and 0.4f before 0.125f (the epsilon and -pi/2 of ToRotationZ_unk are removed
// with it). Name invented.
int SitCanStand_unk(Chara_unk* chara) {
    atn::Vector3 rot = chara->fn_80178018().ToRotationZ_unk();
    float angle = atn::WrapAngle_unk(rot.y - chara->mRot.y);
    return chara->fn_80178078() > 0.4f && (float)__fabs(angle) < 1.5707964f;
}

// `chara` declared first (this in r31, motion in r30), `rot` declared before the named lerp result
// (stack order rot/pos/dir of the original), the model by GetModel_unk() and the rotation written by
// SetRot_unk().
int ControlSitting::vf0C() {
    Chara_unk* chara;
    Motion_unk* motion = mChara->GetModel_unk();
    if (mCount > 0) {
        mCount--;
        float t = mCount / 8.0f;
        atn::Vector3 rot;
        atn::Vector3 pos = LerpSit_unk(mSeat, mStart, t);
        mChara->mPos = pos;
        rot = mChara->mRot;
        rot.y += atn::WrapAngle_unk(mRot.y - mStartRot.y) / 8.0f;
        mChara->SetRot_unk(rot);
    }
    if (motion->mTracks[0].m00 == 0x78) {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            motion->fn_80097C08(0, 0x79, 1, 0, 1, 0.0f, 0.1f);
            mChara->mRot = mRot;
        }
    } else if (motion->mTracks[0].m00 == 0x79) {
        if (mChara->mType == 0 && mChara->fn_80178078() > 0.4f) {
            chara = mChara;
            atn::Vector3 dir = chara->fn_80178018();
            float angle = atn::WrapAngle_unk(dir.y - chara->mRot.y);
            if ((float)__fabs(angle) < 1.5707964f) {
                motion->fn_80097C08(0, 0x7A, 0, 0, 1, 0.0f, 0.1f);
            }
        }
    } else {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            return 1;
        }
    }
    return 0;
}

} // namespace chr
