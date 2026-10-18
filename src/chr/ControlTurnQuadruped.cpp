#include "chr/ControlTurnQuadruped.h"


// inferred: globals of unknown classes
struct System_unk {
    unsigned char m00[0x30];
    int mFrameRate; // 0x30
};
struct Params_unk {
    unsigned char m000[0x3A4];
    float mTimeScale; // 0x3A4
};
// inferred: collision query result (0x44 bytes)
struct CollResult_unk {
    CollResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x10];
    atn::Vector3 mPos; // 0x10
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    int m38;
    int m3C;
    int m40;
    int m44;
};
extern "C" System_unk* lbl_8087F0A8;
extern "C" Params_unk* lbl_8087EFA8;
extern "C" void* lbl_8087EE98;
extern "C" void fn_8004D388(void* mgr, CollResult_unk* result, chr::Chara_unk::Sphere_unk* sphere, atn::Vector3* move,
                            unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test

namespace chr {

// NONMATCHING: 99.9% - the original stores the double argument directly and converts it in
// another register for the comparison; here both use the converted value.
ControlTurnQuadruped::ControlTurnQuadruped(Chara_unk* chara, double angle) : ControlBase(chara) {
    chara->m560 = 0x56;
    mAngle = angle;
    if ((float)angle < 0.0f) {
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

// NONMATCHING: 85% - the stack slots of the three rotation matrices are mirrored (the original
// keeps the matrix address in r30), plus some float register/operand order differences.
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
    __MtxRotate(m.m, 'z', mRot.z);
    __MtxRotate(m.m, 'y', mRot.y);
    __MtxRotate(m.m, 'x', mRot.x);
    PSMTXMultVec(m.m, &move, &move);
    float scale = lbl_8087EFA8->mTimeScale;
    int fps = lbl_8087F0A8->mFrameRate;
    move.x *= scale;
    move.y += -98.1f / (float)(fps * fps);
    move.y *= scale;
    move.z *= scale;
    CollResult_unk result;
    Chara_unk::Sphere_unk sphere = mChara->fn_80176548(mChara->mPos);
    fn_8004D388(lbl_8087EE98, &result, &sphere, &move, 0x80000080, &mChara->mColli, 0, sphere.mRadius);
    Chara_unk* chara = mChara;
    result.mPos.x -= chara->m5A4.x;
    result.mPos.y -= chara->m5A4.y;
    result.mPos.y -= sphere.mRadius;
    result.mPos.z -= chara->m5A4.z;
    chara->mPos = result.mPos;
    return 0;
}

} // namespace chr
