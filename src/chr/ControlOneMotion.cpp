#include "chr/ControlOneMotion.h"
#include "macros.h"

extern "C" void fn_80097CCC(Model_unk* model, int track, float weight);

namespace chr {

// inferred: common end of the constructors (stops track 3 and the movement); name invented
// NONMATCHING (in the three constructors, 99.2-99.5%): the address of lbl_807C7030 and the reloaded mChara
// get r4/r5 the other way round. Tried: plain global, reference/pointer locals, a parameter, __VecCopy.
inline void ResetMove_unk(ControlBase* control, const atn::Vector3& vel) {
    Chara_unk* c = control->mChara;
    c->m580 = 0.0f;
    c->m584 = 0.0f;
    Chara_unk* chara = control->mChara;
    fn_80097CCC(&chara->mMotion, 3, 0.0f);
    chara->m12BC = -1;
    control->mChara->m570 = 0.0f;
    control->mChara->m574 = vel;
}

ControlOneMotion::ControlOneMotion(Chara_unk* chara, int motion, int a, int b, float start, float end)
    : ControlBase(chara), mLoop(a), mCount(0), mFrames(0) {
    chara->m560 = 0x27;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, motion, a, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    if (start >= 0.0f) {
        m->mTracks[0].m08 = start;
    }
    fn_80097CCC(m, 3, 0.0f);
    mEnd = end >= 0.0f ? end : m->fn_80097D7C(0);
    if (a != 0 && b > 0) {
        mCount = b;
    }
    ResetMove_unk(this, lbl_807C7030);
}

int ControlOneMotion::vf0C() {
    Chara_unk* chara = mChara;
    if (mLoop == 0) {
        float frame = chara->mMotion.mTracks[0].m08;
        if (frame >= mEnd) {
            return 1;
        }
    } else if (mCount > 0 && mCount <= ++mFrames) {
        return 1;
    }
    return 0;
}

ControlOneMotionTarget::ControlOneMotionTarget(Chara_unk* chara, int motion, int target, int a)
    : ControlBase(chara), mTarget((MtxPtr)target) {
    chara->m560 = 0x27;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, motion, a, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    fn_80097CCC(m, 3, 0.0f);
    ResetMove_unk(this, lbl_807C7030);
}

int ControlOneMotionTarget::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    atn::Vector3 pos = mChara->mPos;
    atn::Vector3 target = ((atn::Matrix3x4*)mTarget)->GetTrans_unk();
    atn::Vector3 dir = target - pos;
    dir.y = 0.0f;
    PSVECNormalize(&dir, &dir);
    atn::Vector3 rot = mChara->mRot;
    float turn = atn::WrapAngle_unk(dir.ToRotation_unk().y - rot.y);
    float abs = atn::Abs_unk(turn);
    if (abs > 0.31415927f) {
        float sign = turn > 0.0f ? 1.0f : -1.0f;
        turn = sign * abs;
    }
    rot.y += turn;
    mChara->mRot = rot;
    if (motion->mTracks[0].m18 == 0) {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            return 1;
        }
    }
    return 0;
}

// inferred: progress of the motion of track 0 (name invented)
inline float GetRate_unk(Motion_unk* motion) {
    float frame = motion->mTracks[0].m08;
    return frame / motion->fn_80097D7C(0);
}

// inferred: linear interpolation with the sum in this order (name invented)
inline atn::Vector3 LerpB_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

ControlOneMotionTransition::ControlOneMotionTransition(Chara_unk* chara, int motion, const atn::Vector3& ofs)
    : ControlBase(chara), mOfs(ofs) {
    chara->m560 = 0x27;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    fn_80097CCC(m, 3, 0.0f);
    ResetMove_unk(this, lbl_807C7030);
    mStart = mChara->mPos;
}

int ControlOneMotionTransition::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float t = MIN(MAX(0.0f, GetRate_unk(motion)), 1.0f);
    mChara->mPos = LerpB_unk(mStart, mStart + mOfs, t);
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    return 0;
}

} // namespace chr
