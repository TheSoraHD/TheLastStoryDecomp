#include "chr/ControlSitting.h"

extern "C" double fmod(double x, double y);

namespace chr {

// inferred: wraps an angle into [-pi, pi] (name invented)
inline float __WrapAngle_unk(float angle) {
    float a = fmod(angle, 6.283185307179586);
    if (a > 3.1415927f) {
        a -= 6.2831855f;
    }
    if (a < -3.1415927f) {
        a += 6.2831855f;
    }
    return a;
}

ControlSitting::ControlSitting(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& seat, const atn::Vector3& rot)
    : ControlBase(chara) {
    chara->m560 = 0x3D;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->m238 = 1.0f;
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

// NONMATCHING: 95.6% - the original keeps this in r31 (motion in r30) and places the operator
// temporaries in other stack slots (as in ControlTurnQuadruped::vf0C).
int ControlSitting::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mCount > 0) {
        mCount--;
        float t = mCount / 8.0f;
        mChara->mPos = mSeat + (mStart - mSeat) * t;
        atn::Vector3 rot = mChara->mRot;
        rot.y += __WrapAngle_unk(mRot.y - mStartRot.y) / 8.0f;
        mChara->mRot = rot;
    }
    if (motion->m22C == 0x78) {
        float frame = motion->m234;
        if (frame >= motion->fn_80097D7C(0)) {
            motion->fn_80097C08(0, 0x79, 1, 0, 1, 0.0f, 0.1f);
            mChara->mRot = mRot;
        }
    } else if (motion->m22C == 0x79) {
        if (mChara->mType == 0 && mChara->fn_80178078() > 0.4f) {
            Chara_unk* chara = mChara;
            atn::Vector3 dir = chara->fn_80178018();
            float angle = __WrapAngle_unk(dir.y - chara->mRot.y);
            if ((float)__fabs(angle) < 1.5707964f) {
                motion->fn_80097C08(0, 0x7A, 0, 0, 1, 0.0f, 0.1f);
            }
        }
    } else {
        float frame = motion->m234;
        if (frame >= motion->fn_80097D7C(0)) {
            return 1;
        }
    }
    return 0;
}

} // namespace chr
