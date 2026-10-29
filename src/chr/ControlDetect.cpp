#include "chr/ControlDetect.h"

extern "C" float PSVECSquareMag(const atn::Vector3* v); // inferred: squared length

namespace chr {

ControlDetect::ControlDetect(Chara_unk* chara, int arg) : ControlBase(chara), m08(arg) {
    chara->m560 = 0x2F;
    if (mChara->mC58.mC4 != 0 && mChara->mC58.mC4->GetModel_unk()->GetNodeMtx_unk("neck") != 0) {
        mChara->mLook.fn_80129978(mChara->mC58.mC4->GetModel_unk(), "neck", lbl_807C7030, 0.3f);
    }
}

int ControlDetect::vf0C() {
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mChara->mRot;
    if (--m08 <= 0) {
        mChara->mLook.fn_80129A48(0.3f);
        return 1;
    }
    mChara->vf34(&rot, 0, speed, rate);
    return 0;
}

ControlDetectHateAtt::ControlDetectHateAtt(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 0x2F;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1ED, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    mChara->fn_8016DA4C();
    if (mChara->IsOnLedge_unk()) {
        mChara->mFlag31 = 0;
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
}

// NONMATCHING: 99.9% - copying the moved position back into mPos swaps r3/r4 (source temporary and
// mChara)
int ControlDetectHateAtt::vf0C() {
    float spd;
    float speed = 0.0f;
    float rate = 1.0f;
    atn::Vector3 rot = mChara->mRot;
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mC58.mC4 != 0) {
        atn::Vector3 dir = mChara->mC58.mC4->mPos - mChara->mPos;
        dir.y = 0.0f;
        if (!(atn::Abs_unk(PSVECSquareMag(&dir)) < 1.1920929e-7f)) {
            dir.Normalize_unk();
        }
        if (motion->fn_80097D7C(0) > 0.0f) {
            spd = motion->mTracks[0].m0C;
            mChara->mPos = mChara->mPos - dir * 7.0f * (1.0f / motion->fn_80097D7C(0)) * spd;
        }
        atn::Vector3 newRot = mChara->mRot;
        newRot.y = dir.ToRotation_unk().y;
        mChara->mRot = newRot;
        float frame = motion->mTracks[0].m08;
        if (frame >= 30.0f) {
            return 1;
        }
    }
    mChara->vf34(&rot, 0, speed, rate);
    return 0;
}

} // namespace chr
