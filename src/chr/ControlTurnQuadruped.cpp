#include "game/GameConfig_unk.h"
#include "atn/RenderSetting_unk.h"
#include "chr/ControlTurnQuadruped.h"
#include "atn/ColliBase.h"


extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" RenderSetting_unk* lbl_8087EFA8;
extern "C" void* lbl_8087EE98;
extern "C" void fn_8004D388(void* mgr, atn::ColliResult_unk* result, chr::Chara_unk::Sphere_unk* sphere, atn::Vector3* move,
                            unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test

namespace chr {

// inferred: float angle (the original stores the argument without frsp and compares the member read back)
ControlTurnQuadruped::ControlTurnQuadruped(Chara_unk* chara, float angle) : ControlBase(chara) {
    chara->m560 = 0x56;
    mAngle = angle;
    if (mAngle < 0.0f) {
        mLeft = 1;
    } else {
        mLeft = 0;
    }
    mRot = mChara->mRot;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    if (mLeft == 0) {
        motion->fn_80097C08(0, 0x1EE, 0, 1, 1, 0.0f, 0.1f);
    } else {
        motion->fn_80097C08(0, 0x1EF, 0, 1, 1, 0.0f, 0.1f);
    }
    motion->mTracks[0].m0C = 2.0f;
}

// inferred: rotation of a matrix about one axis, as __MtxRotate (math/Vec.h) but with the angle by
// reference: the original loads the 0.0f before the angle (name invented)
inline void MtxRotAxis_unk(Mtx m, char axis, const float& rad) {
    if (rad != 0.0f) {
        Mtx r;
        PSMTXRotRad(r, axis, rad);
        Mtx t;
        PSMTXConcat(m, r, t);
        __MtxCopy(m, t);
    }
}
// inferred: rotation z, y, x of a matrix; one inline level more than the three rotations gives the
// stack slots of their matrices in the order of the original (name invented)
inline void MtxRotZYX_unk(Mtx m, const atn::Vector3& r) {
    MtxRotAxis_unk(m, 'z', r.z);
    MtxRotAxis_unk(m, 'y', r.y);
    MtxRotAxis_unk(m, 'x', r.x);
}

int ControlTurnQuadruped::vf0C() {
    float frame = mChara->mMotion.mTracks[0].m08;
    float length = mChara->mMotion.fn_80097D7C(0);
    if (frame >= length) {
        return 1;
    }
    float rate = 2.0f / length;
    atn::Vector3 rot = mChara->mRot;
    rot.y += mAngle * rate;
    mChara->mRot = rot;
    atn::Vector3 move;
    if (mLeft == 1) {
        move.x = -20.0f;
        move.y = 0.0f;
        move.z = 1.0f;
    } else {
        move.x = 20.0f;
        move.y = 0.0f;
        move.z = 1.0f;
    }
    move.x *= rate;
    move.y *= rate;
    move.z *= rate;
    atn::Matrix3x4 m;
    m.SetIdentity();
    MtxRotZYX_unk(m.m, mRot);
    PSMTXMultVec(m.m, &move, &move);
    int fps = lbl_8087F0A8->m030;
    float scale = lbl_8087EFA8->m3A4;
    move.y += -98.1f / (float)(fps * fps);
    move *= scale;
    atn::ColliResult_unk result;
    Chara_unk::Sphere_unk sphere = mChara->fn_80176548(mChara->mPos);
    fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000080, mChara->GetColli_unk(), 0, sphere.mRadius);
    Chara_unk* chara = mChara;
    result.m10 -= chara->m5A4;
    result.m10.y -= sphere.mRadius;
    chara->mPos = result.m10;
    return 0;
}

} // namespace chr
