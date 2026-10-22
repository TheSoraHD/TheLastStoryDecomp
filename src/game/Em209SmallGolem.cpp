#include "game/Em209SmallGolem.h"
#include "math/Vec.h"

extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id

// inferred: colors of the golem (static array of the file, built by the __sinit 0x802CFFC8)
struct GolemColor_unk {
    GolemColor_unk(float r_, float g_, float b_, float a_) {
        r = r_;
        g = g_;
        b = b_;
        a = a_;
    }
    float r, g, b, a;
};
static GolemColor_unk sColor0_unk(0.03f, 0.08f, 0.04f, 1.0f);
static GolemColor_unk sColor1_unk(0.14f, 0.56f, 0.7f, 1.0f);
static GolemColor_unk sColor2_unk(0.8f, 0.28f, 0.085f, 1.0f);

Em209SmallGolem::~Em209SmallGolem() {}

void Em209SmallGolem::vf14() {
    fn_80149A30();
}

void Em209SmallGolem::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 7) {
        msg->m68 = 0;
        msg->m6C = 0;
        msg->m70 = 0;
        msg->m74 = 0;
        *(int*)&msg->m78[0] = -1;
        msg->mFlag31 = 0;
        msg->mFlag30 = 0;
        msg->mFlag29 = 0;
        msg->mFlag28 = 0;
        *(int*)&msg->m78[4] = -1;
        msg->m64 = 3;
        msg->m90 = 0;
        return;
    }
    if (m150C == 1) {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
}

void Em209SmallGolem::vf40(HitInfo_unk* hit) {
    if (m55C == 6 && m560 - 22U <= 1) {
        m58C = 0;
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        m14BC = 0;
        m14C0 = 0;
        m590 = fn_800F8548(lbl_8087F048);
        vf48();
        fn_80178A6C();
        fn_8015495C(0, 1, 1, 1);
        fn_8016DA4C();
        m58C = 2;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x2E, 0, 0, 1, 0.0f, 0.1f);
        m574 = lbl_807C7030;
        m1434 = 30;
        vf9C(1);
    }
}

// NONMATCHING: 99.9% - the original reloads m624.mData before the copy of the center (same open case
// as CharaTask::vf54)
void Em209SmallGolem::vf54() {
    if (m624.mSize != 0) {
        CharaTask::Item14_unk* item = m624.mData;
        item->m04.mRadius = m5B0;
        item->m04.mCenter = mPos + m5A4;
        m624.mData[0].m04.mCenter.y += m624.mData[0].m04.mRadius;
    }
}

void Em209SmallGolem::vf84(int motion, float blend) {
    m14BC = 0;
    m14C0 = 0;
    m590 = fn_800F8548(lbl_8087F048);
    m58C = 10;
    fn_8016E970(3);
}

extern "C" void fn_802CFAE4(Em209SmallGolem* golem) {
    golem->m14BC = 0;
    golem->m14C0 = 0;
    golem->m590 = fn_800F8548(lbl_8087F048);
}
