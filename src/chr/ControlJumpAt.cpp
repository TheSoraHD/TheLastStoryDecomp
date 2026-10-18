#include "chr/ControlJumpAt.h"
#include "game/CharaTask.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>

extern "C" int rand();

// inferred: object behind lbl_8087F490 (class unknown, also used by chr::ControlCaught)
struct Unk_8087F490 {
    unsigned char m000[0x728];
    int m728;
};

// inferred: input state embedded at +0x48C of the object behind lbl_8087F0A8
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero when `button` was pressed
};

struct Unk_8087F0A8 {
    unsigned char m000[0x48C];
    Input_unk mInput; // 0x48C
};

extern "C" Unk_8087F490* lbl_8087F490;
extern "C" Unk_8087F0A8* lbl_8087F0A8;

namespace chr {

ControlJumpAt::ControlJumpAt(Chara_unk* chara, const atn::Vector3& start, Chara_unk* target)
    : ControlBase(chara), mStart(start), mTarget(target) {
    chara->m560 = 0x7B;
    mChara->m5C0 &= ~1;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->fn_80097C08(0, 0x146, 0, 0, 1, 1.0f, 0.1f);
    motion->m238 = 1.0f;
    mChara->mPos = start;
}

// NONMATCHING: 98.4% - the two operator temporaries get swapped stack slots (and float registers).
int ControlJumpAt::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->m234;
    float t = frame / motion->fn_80097D7C(0);
    atn::Vector3 pos = (mTarget->mPos - mStart) * t + mStart;
    mChara->mPos = pos;
    atn::Vector3 rot = mTarget->mRot;
    rot.y += 3.1415927f;
    mChara->mRot = rot;
    float cur = motion->m234;
    if (cur >= motion->fn_80097D7C(0)) {
        ControlGrappled* next = new (0, "", "", 0) ControlGrappled(mTarget, mChara);
        ((CharaTask*)mTarget)->fn_80178208(next);
        return 1;
    }
    return 0;
}

// NONMATCHING: 95.0% (scheduling only) - in the inlined ControlGrapple constructor the original
// reads mTarget after storing the ControlBase part, as in chr::ControlRideBeast::vf10.
boost::function<void()> ControlJumpAt::vf10() {
    ControlGrapple* next = new (0, "", "", 0) ControlGrapple(mChara, mTarget);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

int ControlGrapple::vf0C() {
    atn::Vector3 pos = mTarget->mPos;
    atn::Vector3 dir(0.0f, 0.0f, 0.5f);
    float angle = mTarget->mRot.y;
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', angle);
    PSMTXMultVec(mtx, &dir, &dir);
    pos.x += dir.x;
    pos.y += dir.y;
    pos.z += dir.z;
    Chara_unk* chara = mChara;
    chara->mPos = pos;
    atn::Vector3 rot = mTarget->mRot;
    rot.y += 3.1415927f;
    mChara->mRot = rot;
    if ((mChara->m7E0 & 0x20) == 0x20) {
        mChara->vf44();
        atn::Vector3 back(0.0f, 0.0f, -2.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mTarget->mRot.y);
        PSMTXMultVec(m, &back, &back);
        ((CharaTask*)mTarget)->fn_8015E4B0(back, -1);
        return 1;
    }
    if (mTarget->m55C != 6 || mTarget->m560 != 0x7D) {
        return 1;
    }
    return 0;
}

// NONMATCHING: 98.8% (scheduling only) - the original loads mTarget before storing `dir`.
boost::function<void()> ControlGrapple::vf10() {
    atn::Vector3 dir(0.0f, 0.0f, 2.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', mTarget->mRot.y);
    PSMTXMultVec(mtx, &dir, &dir);
    return boost::bind(&CharaTask::fn_8015E7A0, (CharaTask*)mChara, dir, -1, false);
}

// NONMATCHING: 97.8% - the original reloads the flag word to clear mFlag5 (character in r3); ours
// keeps the word loaded for the test, like chr::ControlCaught::ControlCaught. Declaring the
// 0x12A4 bitfields volatile fixes both but changes chr::ControlLookAround::ControlLookAround.
ControlGrappled::ControlGrappled(Chara_unk* chara, Chara_unk* holder)
    : ControlBase(chara), mHolder(holder), mCount(0x3C) {
    chara->m560 = 0x7D;
    mChara->fn_8016DA4C();
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag5) {
        mChara->mFlag5 = 0;
    }
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->fn_80097C08(0, 0x1DA, 1, 0, 1, 0.0f, 0.1f);
    motion->m238 = 1.0f;
}

int ControlGrappled::vf0C() {
    if (mChara->m48 == 0) {
        lbl_8087F490->m728 = 1;
        if (mCount > 0 && lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
            mCount--;
        }
    } else if (rand() % 3 == 0) {
        mCount--;
    }
    if (mCount <= 0) {
        ControlGrappleThrown* next = new (0, "", "", 0) ControlGrappleThrown(mHolder, mChara);
        ((CharaTask*)mHolder)->fn_80178208(next);
        return 1;
    }
    mChara->m948 -= 0.1f;
    if (mChara->m7D8 <= 1.0f) {
        mChara->vf44();
        atn::Vector3 dir(0.0f, 0.0f, 2.0f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', mChara->mRot.y);
        PSMTXMultVec(mtx, &dir, &dir);
        ((CharaTask*)mHolder)->fn_8015E4B0(dir, -1);
        return 1;
    }
    if (mHolder->m55C != 6 || mHolder->m560 != 0x7C) {
        return 1;
    }
    return 0;
}

// NONMATCHING: draft - the original builds the vectors with out-of-line atn::Vector3 and
// atn::Matrix3x4 functions (0x8000D114, 0x8013A13C, 0x80011410) and calls the bind_t and
// boost::function constructors out of line (0x801B5DF0, 0x801B5EC0, 0x801B66A4 ...).
boost::function<void()> ControlGrappled::vf10() {
    if (mCount > 0) {
        atn::Vector3 dir(0.0f, 0.0f, -2.0f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', mChara->mRot.y);
        PSMTXMultVec(mtx, &dir, &dir);
        return boost::bind(&CharaTask::fn_8015E4B0, (CharaTask*)mChara, dir, -1);
    }
    ControlGrappledThrow* next = new (0, "", "", 0) ControlGrappledThrow(mChara, mHolder);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

// NONMATCHING: draft (0x801B6E70 throws the target along a parabola: gravity -98.1, 112 frames)
int ControlGrappleThrown::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->m234;
    return frame >= motion->fn_80097D7C(0);
}

ControlGrappledThrow::ControlGrappledThrow(Chara_unk* chara, Chara_unk* holder)
    : ControlBase(chara), mHolder(holder) {
    chara->m560 = 0x7F;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->m24C = 1.0f;
    motion->fn_80097C08(0, 0x1DB, 0, 0, 1, 0.0f, 0.1f);
    motion->m238 = 1.0f;
}

int ControlGrappledThrow::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mMotion.m234 < 112.0f) {
        atn::Vector3 rot = mChara->mRot;
        rot.y += 0.028049935f;
        mChara->mRot = rot;
    }
    float frame = motion->m234;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    return 0;
}

} // namespace chr
