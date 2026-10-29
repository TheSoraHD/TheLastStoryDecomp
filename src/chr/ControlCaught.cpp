#include "chr/ControlCaught.h"
#include "chr/ControlDamage.h"
#include "game/CharaTask.h"
#include "atn/Memory.h"
#include <boost/bind.hpp>

// inferred: object behind lbl_8087F490 (class unknown)
struct Unk_8087F490 {
    unsigned char m000[0x728];
    int m728;
};

// inferred: input state embedded at +0x48C of the object behind lbl_8087F0A8 (class unknown)
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

// inferred: inline Chara_unk helper (name invented)
inline bool __IsUnk_80154344(Chara_unk* chara) {
    return chara->mFlag31 && chara->mC48 != 0;
}

ControlCaught::ControlCaught(Chara_unk* chara, int arg, int count, int motion, int releaseMotion)
    : ControlBase(chara), mCount(count), mMotion(motion), mReleaseMotion(releaseMotion), m18(true),
      m19(true), m1A(true) {
    if (mMotion < 0) {
        mMotion = 0x20C;
    }
    if (mReleaseMotion < 0) {
        mReleaseMotion = 0x68;
    }
    mChara->m58C = 0;
    mChara->m560 = 0x11;
    mChara->fn_8016DA4C();
    mChara->fn_80164DCC(1);
    if (mChara->IsOnLedge_unk()) {
        mChara->mFlag31 = 0;
    }
    if (mChara->m1208 != 0) {
        mChara->fn_801750FC();
    }
    if (__IsUnk_80154344(mChara)) {
        mChara->fn_80154344();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* m = &mChara->mMotion;
    m->m34C = 1;
    m->mTracks[0].m20 = 1.0f;
    m->fn_80097C08(0, mMotion, 1, 0, 1, 0.0f, 0.1f);
    m->mTracks[0].m0C = 1.0f;
    m08 = arg;
    mChara->mF1C = arg;
}

int ControlCaught::vf0C() {
    if (mChara->mType == 0) {
        if (m1A) {
            lbl_8087F490->m728 = 1;
        }
        if (mCount > 0 && m18 && lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
            mCount--;
        }
    }
    if ((mChara->mStatus.m0C & 0x20) == 0x20) {
        return 1;
    }
    if (mCount <= 0 && m19) {
        return 1;
    }
    return 0;
}

void ControlCaught::vf1C() {}

boost::function<void()> ControlCaught::vf10() {
    if (mCount <= 0) {
        ControlRelease* next = new ((MemoryTag)0, "", "", 0) ControlRelease(mChara, mReleaseMotion);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    if (mChara != 0 && (mChara->mStatus.m0C & 0x20) == 0x20) {
        mChara->vf44();
        atn::Vector3 dir(0.0f, 0.0f, -0.01f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', mChara->mRot.y);
        PSMTXMultVec(mtx, &dir, &dir);
        ControlKnockdown* next = new ((MemoryTag)0, "", "", 0) ControlKnockdown(mChara, dir, -1, 0);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    return ControlBase::vf10();
}

ControlRelease::ControlRelease(Chara_unk* chara, int motion) : ControlBase(chara), m08(0) {
    m08 = (chara->mStatus.m0C & 0x20) != 0;
    if (m08) {
        if (motion < 0) {
            motion = 0x2E;
        }
        Motion_unk* m = &mChara->mMotion;
        m->m34C = 1;
        m->mTracks[0].m20 = 1.0f;
        m->fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
        m->mTracks[0].m0C = 1.0f;
    } else {
        if (motion < 0) {
            motion = 0x68;
        }
        Motion_unk* m = &mChara->mMotion;
        m->m34C = 1;
        m->mTracks[0].m20 = 1.0f;
        m->fn_80097C08(0, motion, 0, 0, 1, 0.0f, 0.1f);
        m->mTracks[0].m0C = 1.0f;
    }
    mChara->m560 = 0x13;
    mChara->mF1C = 0;
}

int ControlRelease::vf0C() {
    int ret = 0;
    Motion_unk* motion = &mChara->mMotion;
    if (m08) {
        if (motion->mTracks[0].m00 == 0x2E && mChara->m574.y > -0.01f) {
            motion->fn_80097C08(0, 0x31, 1, 0, 1, 0.0f, 0.1f);
            ret = 1;
        }
    } else if (motion->mTracks[0].m00 == 0x68) {
        if (mChara->m574.y > -0.01f) {
            motion->fn_80097C08(0, 0x69, 0, 0, 1, 0.0f, 0.1f);
        }
    } else {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            ret = 1;
        }
    }
    mChara->fn_8013CB68(&mChara->mRot, 0, 0.0f, 1.0f);
    return ret;
}

} // namespace chr
