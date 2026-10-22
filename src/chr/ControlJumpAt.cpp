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
    unsigned char m000[0x30];
    int mFrameRate; // 0x30
    unsigned char m034[0x48C - 0x34];
    Input_unk mInput; // 0x48C
};

// inferred: collision query result (as in chr/ControlTurnQuadruped.cpp)
struct GrappleCollResult_unk {
    GrappleCollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 mPos; // 0x10
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    atn::Vector3 m44;
};
extern "C" void* lbl_8087EE98;
extern "C" void fn_8004D388(void* mgr, GrappleCollResult_unk* result, chr::Chara_unk::Sphere_unk* sphere,
                            atn::Vector3* move, unsigned int mask, void* ignore, int arg,
                            float radius); // inferred: sweep test

extern "C" Unk_8087F490* lbl_8087F490;
extern "C" Unk_8087F0A8* lbl_8087F0A8;

namespace chr {

ControlJumpAt::ControlJumpAt(Chara_unk* chara, const atn::Vector3& start, Chara_unk* target)
    : ControlBase(chara), mStart(start), mTarget(target) {
    chara->m560 = 0x7B;
    mChara->mColli.mFlags &= ~1;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x146, 0, 0, 1, 1.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    mChara->mPos = start;
}

// NONMATCHING: 98.4% - the two operator temporaries get swapped stack slots (and float registers).
int ControlJumpAt::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    float t = frame / motion->fn_80097D7C(0);
    atn::Vector3 pos = (mTarget->mPos - mStart) * t + mStart;
    mChara->mPos = pos;
    atn::Vector3 rot = mTarget->mRot;
    rot.y += 3.1415927f;
    mChara->mRot = rot;
    float cur = motion->mTracks[0].m08;
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
    if ((mChara->mStatus.m0C & 0x20) == 0x20) {
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
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1DA, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

int ControlGrappled::vf0C() {
    if (mChara->mType == 0) {
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
    mChara->mStatus.mParamB.m8C -= 0.1f;
    if (mChara->mStatus.m04 <= 1.0f) {
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

// NONMATCHING: 97.7% - stack layout: the original places the bind_t of the first branch right
// below the first matrix (0xD8) and the second matrix below it (0xA8); here both matrices come
// first. The third branch also loads mChara earlier.
// Over the inlining budget: the original calls every helper out of line (0x8000D114, 0x8013A13C,
// 0x80011410, the bind and boost::function constructors 0x801B5DF0, 0x801B5EC0, 0x801B66A4 ...).
// inferred: the matrix is a plain Mtx viewed as atn::Matrix3x4 (no constructor call); a local
// atn::Matrix3x4 calls its empty constructor out of line here.
boost::function<void()> ControlGrappled::vf10() {
    if (mChara->GetStatus_unk()->fn_80139F4C()) {
        atn::Vector3 dir(0.0f, 0.0f, -2.0f);
        float angle = mChara->GetRot_unk()->y;
        Mtx mtx;
        ((atn::Matrix3x4*)mtx)->fn_8013A13C(angle);
        dir.fn_80011410(*(atn::Matrix3x4*)mtx);
        return boost::bind(&CharaTask::fn_8015E7A0, mChara, dir, -1, false);
    }
    if (mCount > 0) {
        atn::Vector3 dir(0.0f, 0.0f, -2.0f);
        float angle = mChara->GetRot_unk()->y;
        Mtx mtx;
        ((atn::Matrix3x4*)mtx)->fn_8013A13C(angle);
        dir.fn_80011410(*(atn::Matrix3x4*)mtx);
        return boost::bind(&CharaTask::fn_8015E4B0, mChara, dir, -1);
    }
    ControlGrappledThrow* next = new (0, "", "", 0) ControlGrappledThrow(mChara, mHolder);
    return boost::bind(&CharaTask::fn_80178208, mChara, next);
}

// inferred: identity rotated by z, y and x (name invented)
inline void RotZYX_unk(atn::Matrix3x4& m, const atn::Vector3& rot) {
    m.SetIdentity();
    if (rot.z != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'z', rot.z);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.y != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'y', rot.y);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
    if (rot.x != 0.0f) {
        Mtx t;
        Mtx r;
        PSMTXRotRad(r, 'x', rot.x);
        PSMTXConcat(m.m, r, t);
        __MtxCopy(m.m, t);
    }
}

// NONMATCHING: 99.77% - r3/r4 swapped in the __VecCopy of the swept position (documented blocker)
// inferred: the character spins and falls (gravity -98.1) during the first 112 frames
int ControlGrappleThrown::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mMotion.mTracks[0].m08 < 112.0f) {
        atn::Vector3 rot = mChara->mRot;
        rot.y += 0.028049935f;
        mChara->mRot = rot;
        int fps = lbl_8087F0A8->mFrameRate;
        float g = -98.1f / (float)(fps * fps);
        atn::Vector3 move(0.0f, g, 0.125f);
        Chara_unk* chara = mChara;
        atn::Matrix3x4 m;
        RotZYX_unk(m, chara->mRot);
        PSMTXMultVec(m.m, &move, &move);
        GrappleCollResult_unk result;
        Chara_unk::Sphere_unk sphere = mChara->fn_80176548(mChara->mPos);
        fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000000, &mChara->mColli, 0, sphere.mRadius);
        atn::Vector3 pos = result.mPos;
        Chara_unk* c = mChara;
        pos -= c->m5A4;
        pos.y -= sphere.mRadius;
        c->mPos = pos;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    return 0;
}

ControlGrappledThrow::ControlGrappledThrow(Chara_unk* chara, Chara_unk* holder)
    : ControlBase(chara), mHolder(holder) {
    chara->m560 = 0x7F;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1DB, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

int ControlGrappledThrow::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mMotion.mTracks[0].m08 < 112.0f) {
        atn::Vector3 rot = mChara->mRot;
        rot.y += 0.028049935f;
        mChara->mRot = rot;
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    return 0;
}

} // namespace chr
