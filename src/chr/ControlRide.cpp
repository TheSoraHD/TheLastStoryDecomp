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
    if (mBeast != 0 && mBeast->mType == 0 && mBeast->mC58.mC0 == 0) {
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

ControlRideBeast::ControlRideBeast(Chara_unk* chara, Chara_unk* rider)
    : ControlBase(chara), mBeast(chara), mRider(rider) {
    chara->m560 = 0x8A;
    mBeast->mMotion.m34C = 1;
    mBeast->mMotion.mTracks[0].m20 = 1.0f;
    mBeast->mMotion.mTracks[0].m0C = 1.0f;
    mBeast->mMotion.fn_80097C08(0, 0x1F2, 0, 0, 1, 0.0f, 0.1f);
    CharaTask* beast = mBeast;
    mStartPos = beast->mPos;
    atn::Model* model = mRider->GetModel_unk();
    MtxPtr mtx = model->GetNodeMtx_unk(model->GetNodeNoByName("seatpoint", 0));
    mSeatPos = __GetTrans_unk(mtx);
    mStartRot = mBeast->mRot;
    mRiderRot = mRider->mRot;
}

int ControlRideBeast::vf0C() {
    float frame = mBeast->mMotion.mTracks[0].m08;
    if (frame < mBeast->mMotion.fn_80097D7C(0)) {
        Motion_unk* motion = &mBeast->mMotion;
        float cur = motion->mTracks[0].m08;
        float rate = (0.0f == cur) ? 0.0f : cur / motion->fn_80097D7C(0);
        atn::Vector3 dir = mSeatPos - mStartPos;
        float dist = PSVECMag(&dir);
        dist *= rate;
        PSVECNormalize(&dir, &dir);
        CharaTask* beast = mBeast;
        beast->mPos = mStartPos + dir * dist;
        const atn::Vector3& s = mStartRot;
        atn::Vector3 d = mRiderRot - s;
        d *= rate;
        mBeast->mRot = s + d;
        return 0;
    }
    return 1;
}

void ControlRideBeast::vf1C() {
    mChara->fn_8017AC3C(0);
    mChara->mF1C = 0;
}

boost::function<void()> ControlRideBeast::vf10() const {
    if (mRider != 0 && mRider->mC58.mC0 == 0) {
        return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
    }
    ControlRide* next = new ((MemoryTag)0, "", "", 0) ControlRide(mChara, mRider);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

} // namespace chr
