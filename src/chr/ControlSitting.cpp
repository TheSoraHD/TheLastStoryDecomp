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

// NONMATCHING: 97.9% - with the lerp helper the arithmetic matches; the original keeps this in r31
// (motion in r30) and puts `dir` below the lerp result on the stack. Tried: motion taken after the
// lerp or split/as a reference, named locals, dir/chara declared elsewhere, pre-decrement, engine
// Lerp_unk/Abs_unk, by-value or static helper.
int ControlSitting::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mCount > 0) {
        mCount--;
        float t = mCount / 8.0f;
        mChara->mPos = LerpSit_unk(mSeat, mStart, t);
        atn::Vector3 rot = mChara->mRot;
        rot.y += atn::WrapAngle_unk(mRot.y - mStartRot.y) / 8.0f;
        mChara->mRot = rot;
    }
    if (motion->mTracks[0].m00 == 0x78) {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            motion->fn_80097C08(0, 0x79, 1, 0, 1, 0.0f, 0.1f);
            mChara->mRot = mRot;
        }
    } else if (motion->mTracks[0].m00 == 0x79) {
        if (mChara->mType == 0 && mChara->fn_80178078() > 0.4f) {
            Chara_unk* chara = mChara;
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
