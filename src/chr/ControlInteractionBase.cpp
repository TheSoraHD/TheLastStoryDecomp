#include "chr/ControlInteractionBase.h"
#include "game/CharaTask.h"

extern "C" int rand();
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: squared length

// inferred: closest point to `pos` on the segment a-b (written to `out`)
extern "C" void fn_80050A1C(const atn::Vector3* pos, atn::Vector3 a, atn::Vector3* out, atn::Vector3 b);

// inferred: voice/sound player (lbl_8087F498); fn_803EA77C plays voice `id` of a character
extern "C" void* lbl_8087F498;
extern "C" void fn_803EA77C(void* player, CharaTask* chara, int id, int arg, float a, float b);

// inferred: camera/character manager (lbl_8087F048, class unknown)
extern "C" void* lbl_8087F048;
extern "C" void fn_8010A308(void* obj, atn::Vector3* from, atn::Vector3* to, float radius);

// inferred: field object (lbl_8087F430) and its counters (same calls as in gmk/AttractHatePoint.cpp)
extern "C" void* lbl_8087F430;
extern "C" int fn_80370174(void* field, int id);
extern "C" void fn_80370320(void* field, int id, int value, int arg);

// inferred: flag test through an accessor (the flag word is loaded again before it is changed);
// name invented (same as in gmk/ReinforcePoint.cpp)
inline bool HasCharaFlag_unk(const CharaTask* chara, unsigned int flag) {
    return (chara->m54C & flag) == flag;
}

// inferred: front direction of the character (name invented)
inline atn::Vector3 GetFrontDir_unk(CharaTask* chara) {
    atn::Vector3 dir(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', chara->mRot.y);
    PSMTXMultVec(mtx, &dir, &dir);
    return dir;
}

// inferred: linear interpolation with the product first (name invented)
inline atn::Vector3 LerpInteraction_unk(const atn::Vector3& a, const atn::Vector3& b, float t) {
    return (b - a) * t + a;
}

namespace chr {

ControlInteractionBase::ControlInteractionBase(Chara_unk* chara, int id, int motion) : ControlBase(chara) {
    chara->m560 = id;
    Motion_unk* m = &mChara->mMotion;
    mChara->mMotion.m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
}

// NONMATCHING: 99.4% - r6/r7 swapped (character pointer / temporary vector address).
int ControlInteractionBase::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (m08 > 0) {
        m08--;
        float t = m08 / 3.0f;
        mChara->mPos = LerpInteraction_unk(m0C, m18, t);
        m0C += m24 * m3C;
        m18 += m24 * m3C;
    }
    mChara->mPos = mChara->mPos + m24 * m3C;
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        return 1;
    }
    return 0;
}

void ControlInteractionBase::Setup_unk(const atn::Vector3& a, const atn::Vector3& b, float speed) {
    atn::Vector3 dir = b - a;
    PSVECMag(&dir);
    PSVECNormalize(&dir, &dir);
    m18 = mChara->mPos;
    m0C = a;
    m30 = b;
    m24 = dir;
    m3C = speed;
    m08 = 3;
    mChara->mRot = m24.ToRotationZ_unk();
}

ControlDuckUnderA::ControlDuckUnderA(Chara_unk* chara) : ControlInteractionBase(chara, 0x18, 0x49) {}

int ControlDuckUnderA::vf0C() {
    int ret = 0;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0)) {
        ret = 1;
    }
    if (HasCharaFlag_unk(mChara, 0x08000000)) {
        mChara->m54C |= 0x200;
    }
    mChara->fn_8013CB68(&mChara->mRot, 1, 1.0f, 1.0f);
    if (HasCharaFlag_unk(mChara, 0x08000000)) {
        mChara->m54C &= ~0x200;
    }
    return ret;
}

// NONMATCHING: 91.9% - saved register allocation (this/a/b/motion) and stack slots of the
// by-value vectors passed to fn_80050A1C.
ControlDuckUnderB::ControlDuckUnderB(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b)
    : ControlInteractionBase(chara, 0x19, 0x4A) {
    m24 = GetFrontDir_unk(mChara);
    m3C = 0.5f;
    m08 = 3;
    m18 = mChara->mPos;
    fn_80050A1C(&m18, a, &m0C, b);
    m0C += m24 * -5.0f;
    mChara->fn_801446F0();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0xC, 0, 1.0f, 10.0f);
    }
}

ControlDuckJoist::ControlDuckJoist(Chara_unk* chara, const atn::Vector3& rot) : ControlInteractionBase(chara, 0x1A, 0x200) {
    mChara->mRot = rot;
    m24 = GetFrontDir_unk(mChara);
    m3C = 0.5f;
    m08 = 0;
    mChara->fn_801446F0();
}

ControlConstrict::ControlConstrict(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b, float f)
    : ControlInteractionBase(chara, 0x29, 0x4E) {
    Setup_unk(a, b, f);
    mChara->fn_801446F0();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0xC, 0, 1.0f, 10.0f);
    }
}

ControlBumpHead::ControlBumpHead(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b)
    : ControlInteractionBase(chara, 0x2A, 0x4F) {
    Setup_unk(a, b, 0.52f);
    mChara->fn_801446F0();
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x11, 0, 1.0f, 10.0f);
    }
    if (lbl_8087F498 != 0 && rand() % 2 == 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x12, 0x2D, 1.0f, 10.0f);
    }
    if (mChara->mType == 0 && lbl_8087F430 != 0) {
        // inferred: counts the bumps (flag 0xF8, at most 999)
        fn_80370320(lbl_8087F430, 0xF8,
                    fn_80370174(lbl_8087F430, 0xF8) + 1 < 999 ? fn_80370174(lbl_8087F430, 0xF8) + 1 : 999, 0);
    }
}

ControlStepOn::ControlStepOn(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b)
    : ControlInteractionBase(chara, 0x2B, 0x50) {
    Setup_unk(a, b, 0.52f);
    mChara->fn_801446F0();
}

ControlThroughRope::ControlThroughRope(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b)
    : ControlInteractionBase(chara, 0x2C, 0x201) {
    Setup_unk(a, b, 0.467f);
    mChara->fn_801446F0();
}

void ControlDiveIntoWater::GetBodyPos_unk(atn::Vector3& out) {
    MtxPtr node = mChara->mMotion.GetNodeMtx_unk("neck");
    if (node == 0) {
        node = mChara->mMotion.GetNodeMtx_unk("spine1");
    }
    if (node == 0) {
        node = mChara->mMotion.GetNodeMtx_unk("hips");
    }
    if (node == 0) {
        node = mChara->mMotion.GetNodeMtx_unk("reference");
    }
    if (node != 0) {
        out.Set_unk(node[0][3], node[1][3], node[2][3]);
    } else {
        out = mChara->mPos;
    }
}

// NONMATCHING: 97.6% - register allocation and stack slots of the rotation vectors.
ControlDiveIntoWater::ControlDiveIntoWater(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b)
    : ControlInteractionBase(chara, 2, 0x5F) {
    m24 = b - a;
    m24.y = 0.0f;
    if (!((float)__fabs(fn_805F9920(&m24)) < 1.1920929e-7f)) {
        PSVECNormalize(&m24, &m24);
    }
    atn::Vector3 rot = m24.ToRotation_unk();
    mChara->mRot = atn::Vector3(mChara->mRot.x, rot.y, mChara->mRot.z);
    atn::Vector3 pos;
    GetBodyPos_unk(pos);
    m18 = pos;
}

// NONMATCHING: 97.7% - registers r3/r5/r9 in the node position copy.
int ControlDiveIntoWater::vf0C() {
    int ret = 0;
    float frame = mChara->mMotion.mTracks[0].m08;
    if (frame >= mChara->mMotion.fn_80097D7C(0)) {
        mChara->mPos = ((atn::Matrix3x4*)mChara->mMotion.GetNodeMtx_unk(mChara->m524))->GetTrans_unk();
        mChara->mMotion.fn_80097C08(0, 0x5D, 1, 0, 1, 1.0f, 0.1f);
        ret = 1;
    }
    atn::Vector3 pos;
    GetBodyPos_unk(pos);
    if (mChara->mMotion.mTracks[0].m08 < 30.0f && lbl_8087F048 != 0) {
        fn_8010A308(lbl_8087F048, &m18, &pos, mChara->m5B0);
    }
    m18 = pos;
    return ret;
}

atn::Vector3 ControlDiveIntoWater::vf14() {
    return ((atn::Matrix3x4*)mChara->mMotion.GetNodeMtx_unk(mChara->m524))->GetTrans_unk();
}

} // namespace chr
