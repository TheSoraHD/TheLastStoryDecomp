#include "chr/ControlAppear.h"
#include "game/CharaTask.h"
#include <boost/bind.hpp>

// inferred: collision objects behind lbl_8087EE98 and lbl_8087F048 (classes unknown)
struct Colli_unk {};
extern "C" Colli_unk* lbl_8087EE98;
extern "C" Colli_unk* lbl_8087F048;
extern "C" int fn_8004ED34(Colli_unk* colli, int arg2, atn::Vector3* from, atn::Vector3* to, int mask, int arg6, int arg7); // inferred: ray test
extern "C" void fn_8010A308(Colli_unk* colli, atn::Vector3* from, atn::Vector3* to, float radius); // inferred: sphere sweep

// inferred: motion ids local to this file (the bound enum type is MOTION_ID::@enum...control_appear_cpp)
namespace MOTION_ID {
enum {
    MOTION_2C = 0x2C
};
}

namespace chr {

ControlAppear::ControlAppear(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& offset)
    : ControlBase(chara), mStart(start), mOffset(offset) {
    chara->m560 = 0x23;
    mChara->mColli.mFlags &= ~1;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x2B, 0, 0, 1, 1.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    mChara->mPos = start;
    m08 = mChara->mColli.m48;
}

int ControlAppear::vf0C() {
    atn::Vector3 rot = mChara->mRot;
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    float a = 0.0f;
    float b = 1.0f;
    if (frame >= motion->fn_80097D7C(0)) {
        mChara->mColli.mFlags |= 1;
        return 1;
    }
    atn::Vector3 from = m08;
    atn::Vector3 to = mChara->mColli.m48;
    if (lbl_8087F048 != 0) {
        fn_8010A308(lbl_8087F048, &from, &to, mChara->m5B0);
    }
    m08 = to;
    mChara->vf34(&rot, 0, a, b);
    return 0;
}

// NONMATCHING: 98.8% - the character, the entry and the result temporary get r5/r3/r4 instead of
// r4/r5/r3.
void ControlAppear::vf40() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    float t = frame / motion->fn_80097D7C(0);
    Chara_unk* chara = mChara;
    Motion_unk::Entry_unk* e = &motion->m220[chara->m524];
    chara->mPos = e->m04 + mStart + mOffset * t;
    e->m04.x = 0.0f;
    e->m04.y = 0.0f;
    e->m04.z = 0.0f;
}

boost::function<void()> ControlAppear::vf10() {
    atn::Vector3 from = mChara->mPos;
    atn::Vector3 to = from;
    from.y += 3.0f;
    to.y -= 9.0f;
    // inferred: the collision object is read before the mask is computed (0x80188440)
    Colli_unk* colli = lbl_8087EE98;
    if (fn_8004ED34(colli, 0, &from, &to, mChara->fn_80179D44() | 0x80000000, 0, 0) == 0) {
        return boost::bind(&CharaTask::vf8C, (CharaTask*)mChara, lbl_807C7030);
    }
    return boost::bind(&CharaTask::vf84, (CharaTask*)mChara, MOTION_ID::MOTION_2C, 0.0f);
}

void ControlAppear::vf1C() {
    mChara->mColli.mFlags |= 1;
}

ControlAppearEnd::ControlAppearEnd(Chara_unk* chara, int motion, float blend) : ControlBase(chara) {
    chara->m560 = 0x24;
    mChara->mColli.mFlags |= 1;
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, motion, 0, 0, 1, blend, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

int ControlAppearEnd::vf0C() {
    atn::Vector3 rot = mChara->mRot;
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    // inferred: the vf34 arguments are locals (loaded into f30/f29 before the first call)
    float a = 0.0f;
    float b = 1.0f;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    mChara->vf34(&rot, 0, a, b);
    return 0;
}

// NONMATCHING: 94.1% (scheduling only) - the original loads mChara before the member function
// pointer constant, as in chr::ControlRideBeast::vf10.
boost::function<void()> ControlAppearEnd::vf10() {
    return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
}

ControlAppearSkelton::ControlAppearSkelton(Chara_unk* chara, const atn::Vector3& pos)
    : ControlBase(chara), mState(0), mWait(0x37) {
    chara->m560 = 0x23;
    mChara->mColli.mFlags &= ~1;
    mChara->fn_8016E4C4(0);
    mChara->mPos = pos;
    mChara->mF60 = pos;
}

int ControlAppearSkelton::vf0C() {
    if (mState == 0) {
        if (mWait <= 0) {
            mChara->fn_8016E4C4(1);
            Motion_unk* motion = &mChara->mMotion;
            motion->m34C = 1;
            motion->mTracks[0].m20 = 1.0f;
            motion->fn_80097C08(0, 0x2E, 0, 0, 1, 1.0f, 0.1f);
            motion->mTracks[0].m0C = -1.0f;
            motion->mTracks[0].m08 = motion->fn_80097D7C(0);
            mState++;
        } else {
            mWait--;
        }
    } else if (mChara->mMotion.mTracks[0].m08 <= 0.0f) {
        mChara->mColli.mFlags |= 1;
        return 1;
    }
    return 0;
}

// NONMATCHING: 96.8% (scheduling only) - mChara is loaded before the stack frame is set up.
boost::function<void()> ControlAppearSkelton::vf10() {
    if (mChara != 0) {
        mChara->fn_8016E4C4(1);
        mChara->mColli.mFlags |= 1;
    }
    return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
}

void ControlAppearSkelton::vf1C() {
    if (mChara != 0) {
        mChara->fn_8016E4C4(1);
        mChara->mColli.mFlags |= 1;
    }
}

ControlAppearCoffin::ControlAppearCoffin(Chara_unk* chara, const atn::Vector3& pos) : ControlBase(chara), mWait(0x3C) {
    chara->m560 = 0x23;
    mChara->mColli.mFlags &= ~1;
    mChara->fn_8016E4C4(0);
    mChara->mPos = pos;
    mChara->mF60 = pos;
}

int ControlAppearCoffin::vf0C() {
    if (mWait <= 0) {
        mChara->mColli.mFlags |= 1;
        mChara->fn_8016E4C4(1);
        return 1;
    }
    mWait--;
    return 0;
}

} // namespace chr
