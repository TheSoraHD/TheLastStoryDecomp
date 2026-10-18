#include "chr/ControlBreakOpen.h"

// inferred: camera settings block copied into the camera manager (class names unknown)
struct CameraParam_unk {
    CameraParam_unk(float px, float py, float pz, float tx, float ty, float tz, float fov, int flag)
        : mPos(px, py, pz), mTarget(tx, ty, tz), mFov(fov), mFlag(flag) {}

    atn::Vector3 mPos;
    atn::Vector3 mTarget;
    float mFov;
    int mFlag;
};
struct CameraSub_unk {
    void fn_80389838(int mode);
};
struct CameraMgr_unk {
    void SetParam_unk(const CameraParam_unk& param) {
        // inferred: the two vectors are copied through a atn::Vector3 pointer (second one at src + 0xC)
        const atn::Vector3* src = &param.mPos;
        atn::Vector3* dst = &mParam.mPos;
        dst[0] = src[0];
        dst[1] = src[1];
        mParam.mFov = param.mFov;
        mParam.mFlag = param.mFlag;
    }

    unsigned char m000[0x6C];
    CameraSub_unk m06C;
    unsigned char m06D[0x568 - 0x6D];
    CameraParam_unk mParam; // 0x568
};
struct Flags_unk {
    unsigned char m000[0x738];
    int m738;
};
extern "C" CameraMgr_unk* lbl_8087F430;
extern "C" Flags_unk* lbl_8087F490;
extern "C" void* lbl_8087F0A8;
extern "C" int fn_801231D0(void* pad, int arg); // inferred: button pressed

namespace chr {

// NOTE: every function matches 100%, but the unit is not linked yet: the original emits the
// weak ControlBase::vf5C/vf4C/vf48/vf44 and the destructor in reverse vtable order
// (0x80188060-0x80188090); our compiler emits them in vtable order.

ControlBreakOpen::ControlBreakOpen(Chara_unk* chara, BreakTarget_unk* target)
    : ControlBase(chara), mState(0), mTarget(target), mWait(20) {
    chara->m560 = 0x81;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x237, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    lbl_8087F430->SetParam_unk(CameraParam_unk(-20.0f, 3.0f, -15.0f, 0.0f, 10.0f, 5.0f, 0.7853982f, 1));
    lbl_8087F430->m06C.fn_80389838(0x12);
}

int ControlBreakOpen::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    switch (mState) {
    case 0: {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            motion->fn_80097C08(0, 0x238, 1, 0, 1, 0.0f, 0.1f);
            BreakMsg_unk msg;
            msg.mType = 2;
            mTarget->vf40(&msg);
            mState = 1;
        }
        break;
    }
    case 1:
        lbl_8087F490->m738 = 1;
        if (fn_801231D0((char*)lbl_8087F0A8 + 0x48C, 0)) {
            if (--mWait <= 0) {
                motion->fn_80097C08(0, 0x239, 0, 0, 1, 0.0f, 0.1f);
                BreakMsg_unk msg;
                msg.mType = 3;
                mTarget->vf40(&msg);
                mState = 2;
            }
        }
        break;
    case 2: {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            return 1;
        }
        break;
    }
    }
    return 0;
}

void ControlBreakOpen::vf1C() {
    if (mState < 2) {
        BreakMsg_unk msg;
        msg.mType = 1;
        mTarget->vf40(&msg);
    }
}

} // namespace chr
