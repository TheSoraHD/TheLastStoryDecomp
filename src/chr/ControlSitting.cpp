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

// NONMATCHING: 90% - the original reloads mChara after each Vec copy into the character and
// keeps this in r31; the operator temporaries are in mirrored stack slots (as in
// ControlTurnQuadruped::vf0C).
int ControlSitting::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mCount > 0) {
        mCount--;
        float t = mCount / 8.0f;
        mChara->mPos = mSeat + (mStart - mSeat) * t;
        Vec rot = mChara->mRot;
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
        if (mChara->m48 == 0 && mChara->fn_80178078() > 0.4f) {
            Chara_unk* chara = mChara;
            Vec dir = chara->fn_80178018();
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

// NONMATCHING: 96.9% - same mChara reload difference after the m574 copy (r4/r5 swapped).
ControlSitting::ControlSitting(Chara_unk* chara, const Vec& start, const Vec& seat, const Vec& rot)
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
    Vec zero(0.0f, 0.0f, 0.0f);
    mChara->m574 = zero;
    mChara->m570 = 0.0f;
    mChara->m580 = 0.0f;
    mChara->m584 = 0.0f;
}

} // namespace chr
