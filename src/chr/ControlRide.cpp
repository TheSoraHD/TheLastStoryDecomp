#include "chr/ControlRide.h"
#include "game/CharaTask.h"
#include "game/MapPartsModel.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>

extern "C" float PSVECMag(const atn::Vector3* v);
extern "C" void PSVECNormalize(const atn::Vector3* src, atn::Vector3* dst);

namespace chr {

// inferred: translation column of a node matrix (name invented)
inline atn::Vector3 __GetTrans_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

int ControlRide::vf0C() {
    int ret = 0;
    switch (mState) {
    case 0:
        mState = 1;
        break;
    case 2:
        ret = 1;
        break;
    }
    if (mBeast != 0 && mBeast->mType == 0 && mBeast->mD18 == 0) {
        mChara->fn_8017AC3C(0);
        mChara->mF1C = 0;
        return 1;
    }
    return ret;
}

void ControlRide::vf1C() {
    mChara->fn_8017AC3C(0);
    mChara->mF1C = 0;
}

// NONMATCHING: 99.7% - mBeast and mRider are reloaded into swapped registers (r3/r6).
ControlRideBeast::ControlRideBeast(Chara_unk* chara, Chara_unk* rider)
    : ControlBase(chara), mBeast(chara), mRider(rider) {
    chara->m560 = 0x8A;
    mBeast->mMotion.m34C = 1;
    mBeast->mMotion.m24C = 1.0f;
    mBeast->mMotion.m238 = 1.0f;
    mBeast->mMotion.fn_80097C08(0, 0x1F2, 0, 0, 1, 0.0f, 0.1f);
    mStartPos = mBeast->mPos;
    // inferred: the motion object at +0xB0 is also the model (MapPartsModel.h)
    Model_unk* model = (Model_unk*)&mRider->mMotion;
    MtxPtr mtx = model->GetNodeMtx_unk(model->fn_80092814("seatpoint", 0));
    mSeatPos = __GetTrans_unk(mtx);
    mStartRot = mBeast->mRot;
    mRiderRot = mRider->mRot;
}

// NONMATCHING: 97.8% - the operator temporaries get other stack slots (0x80-byte frame instead of
// 0x70) and the products are emitted with swapped operands; the lerp of the rotation is written
// per component because a (b - a) * t temporary is stored by our compiler and not by the original.
int ControlRideBeast::vf0C() {
    float frame = mBeast->mMotion.m234;
    if (frame < mBeast->mMotion.fn_80097D7C(0)) {
        Motion_unk* motion = &mBeast->mMotion;
        float cur = motion->m234;
        float rate = (0.0f == cur) ? 0.0f : cur / motion->fn_80097D7C(0);
        atn::Vector3 dir = mSeatPos - mStartPos;
        float dist = PSVECMag(&dir) * rate;
        PSVECNormalize(&dir, &dir);
        mBeast->mPos = mStartPos + dir * dist;
        const atn::Vector3& s = mStartRot;
        mBeast->mRot = s + atn::Vector3((mRiderRot.x - s.x) * rate, (mRiderRot.y - s.y) * rate, (mRiderRot.z - s.z) * rate);
        return 0;
    }
    return 1;
}

void ControlRideBeast::vf1C() {
    mChara->fn_8017AC3C(0);
    mChara->mF1C = 0;
}

// NONMATCHING: 96.4% (only scheduling) - the original loads mRider at the very start and reads it
// after the ControlBase part of the inlined ControlRide constructor is stored.
boost::function<void()> ControlRideBeast::vf10() {
    if (mRider != 0 && mRider->mD18 == 0) {
        return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
    }
    ControlRide* next = new (0, "", "", 0) ControlRide(mChara, mRider);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

} // namespace chr
