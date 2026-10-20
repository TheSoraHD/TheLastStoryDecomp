#include "chr/ControlShoot.h"
#include "macros.h"

// inferred: effect/bullet manager (lbl_8087F048, as in game/CharaTask.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, int a, int b, int c, float f1, float f2);
// inferred: field (lbl_8087F430, as in game/CharaTask.cpp); names invented
struct Field_unk {
    unsigned char m000[0x96C];
    int m96C;
    int m970;
    float m974;
    float m978;
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id);                    // inferred: event flag
extern "C" void fn_80370320(Field_unk* field, int id, int value, int arg); // inferred: sets an event flag
extern "C" void* lbl_8087F610; // inferred: arena mode (as in game/CharaTask.cpp)
extern "C" void* lbl_8087F120;
extern "C" void fn_801EA0B4(void* arg);
// inferred: shot speed ranges (arena and normal)
extern "C" float lbl_8087DA48;
extern "C" float lbl_8087DA4C;
extern "C" float lbl_8087DA50;
extern "C" float lbl_8087DA54;

namespace chr {

// NONMATCHING: 98.8% - the case 0x25/0x26 schedules the motion selection before the call arguments, and
// the zero stored to mFB0/mFB4 gets r6 instead of r4
ControlShoot::ControlShoot(Chara_unk* chara, int id, const atn::Vector3& from, const atn::Vector3& to)
    : ControlBase(chara), mData(fn_80219E6C(id)), mFrom(from), mTo(to), m28(1) {
    mChara->m560 = 0x62;
    HitInfo_unk* data = mData;
    Motion_unk* motion = &mChara->mMotion;
    int flag = data != 0 ? (data->mAC >> 5) & 1 : 0;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    mMotion = motion->mTracks[0].m00;
    switch (mMotion) {
    case 0x25:
    case 0x26:
        {
            int id = 0x17E;
            if (mMotion == 0x25) {
                id = 0x17D;
            }
            motion->fn_80097C08(0, id, 0, 1, 1, 1.0f, 0.1f);
        }
        if (flag) {
            atn::Vector3 ofs(0.0f, 0.0f, -2.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', mChara->mRot.y);
            PSMTXMultVec(m, &ofs, &ofs);
            mChara->m6B8.Assign_unk(ofs);
        }
        break;
    case 0x1A9:
        motion->fn_80097C08(0, 0x17F, 0, 1, 1, 1.0f, 0.1f);
        break;
    case 0x1AA:
        motion->fn_80097C08(0, 0x180, 0, 1, 1, 1.0f, 0.1f);
        break;
    case 0x189:
        motion->fn_80097C08(0, 0x181, 0, 1, 1, 1.0f, 0.1f);
        break;
    case 0x18A:
        motion->fn_80097C08(0, 0x182, 0, 1, 1, 1.0f, 0.1f);
        break;
    case 0x18B:
        motion->fn_80097C08(0, 0x183, 0, 1, 1, 1.0f, 0.1f);
        break;
    case 0x18C:
        motion->fn_80097C08(0, 0x184, 0, 1, 1, 1.0f, 0.1f);
        break;
    default:
        motion->fn_80097C08(0, 0x17D, 0, 1, 1, 1.0f, 0.1f);
        break;
    }
    int scale = 1;
    if (mChara->mFlag6) {
        switch (mChara->m50) {
        case 700022:
            motion->fn_80097C08(0, 0x25, 0, 1, 1, 1.0f, 0.1f);
            scale = 0;
            motion->mTracks[0].m08 = 50.0f;
            motion->mTracks[0].m0C = 1.0f;
            break;
        case 700023:
            motion->fn_80097C08(0, 0x25, 0, 1, 1, 1.0f, 0.1f);
            scale = 0;
            motion->mTracks[0].m08 = 34.0f;
            motion->mTracks[0].m0C = 1.0f;
            break;
        }
    }
    if ((unsigned int)(mData->m04 - 0x148) <= 2) {
        fn_800F8574(lbl_8087F048, mChara, mData, &mFrom, &mTo, 0, 0xA00000, 0, 0.0f, 1.0f);
        Field_unk* field = lbl_8087F430;
        fn_80370320(field, 0xCF7, fn_80370174(field, 0xCF7) + 1, 0);
    } else if (mData->m04 == 0x144) {
        fn_800F8574(lbl_8087F048, mChara, mData, &mFrom, &mTo, 0, 0x40, 0, 0.0f, 1.0f);
    } else if (mChara->m50 == 700023) {
        fn_800F8574(lbl_8087F048, mChara, mData, &mFrom, &mTo, 0, 0, 0, 0.0f, 1.0f);
        {
            Mtx m;
            PSMTXRotRad(m, 'y', 0.47123894f);
            atn::Vector3 to = mTo;
            PSMTXMultVec(m, &to, &to);
            fn_800F8574(lbl_8087F048, mChara, mData, &mFrom, &to, 0, 0, 0, 0.0f, 1.0f);
        }
        {
            Mtx m;
            PSMTXRotRad(m, 'y', -0.47123894f);
            atn::Vector3 to = mTo;
            PSMTXMultVec(m, &to, &to);
            fn_800F8574(lbl_8087F048, mChara, mData, &mFrom, &to, 0, 0, 0, 0.0f, 1.0f);
        }
    } else {
        fn_800F8574(lbl_8087F048, mChara, mData, &mFrom, &mTo, 0, 0, 0, 0.0f, 1.0f);
    }
    int level = mChara->mStatus.mParamB.m94;
    CharaStatus* status = &mChara->mStatus;
    if (lbl_8087F610 != 0) {
        level -= 100;
    }
    float rate = MAX(0.0f, MIN(1.0f, 1.0f - 0.001f * level));
    float speed = 1.0f;
    if (scale) {
        if (lbl_8087F610 != 0) {
            speed = (speed - rate) * (lbl_8087DA4C - lbl_8087DA48) + lbl_8087DA48;
        } else {
            speed = (speed - rate) * (lbl_8087DA54 - lbl_8087DA50) + lbl_8087DA50;
        }
    }
    if (status->fn_80134168(0x43, -1) > 0) {
        speed *= mChara->mStatus.fn_80134168(0x43, -1) / 10.0f;
    }
    motion->mTracks[0].m0C = speed;
    int wait = MAX(0, (int)(mData->m68 * rate));
    if (mChara->mType == 0) {
        wait = MIN(40, wait);
    }
    if (mChara->mStatus.fn_80134168(0x43, -1) > 0) {
        wait = wait / (mChara->mStatus.fn_80134168(0x43, -1) / 10.0f);
    }
    Chara_unk* c = mChara;
    c->m644 = wait;
    c->m640 = wait;
    mChara->mFlagB8 = 0;
    if (mChara->mType == 0) {
        fn_801EA0B4(lbl_8087F120);
    }
    mChara->mFB0 = 0;
    mChara->mFB4 += mData->m48;
    if (mChara->mFB4 >= mChara->mStatus.mParamB.m20) {
        mChara->mFB0 = rate * mChara->mStatus.mParamB.m24;
        mChara->mFB4 = 0;
    }
    if (flag && mChara->mType == 0 && lbl_8087F430 != 0) {
        Field_unk* field = lbl_8087F430;
        field->m96C %= 2;
        field->m970 = 2;
        field->m974 = 0.0f;
        field->m978 = 1.0f;
    }
}

int ControlShoot::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if ((unsigned int)(motion->mTracks[0].m00 - 0x17D) <= 1) {
        mChara->vf34(&mChara->mRot, 0, 0.0f, 1.0f);
    }
    float frame = motion->mTracks[0].m08;
    bool done = frame >= motion->fn_80097D7C(0);
    if (mChara->m50 == 700022 && mChara->mFlag6) {
        done = motion->mTracks[0].m08 >= 60.0f;
    }
    if (done) {
        motion->fn_80097C08(0, mMotion, 0, 0, 1, 0.0f, 0.1f);
        if (mChara->mFlag6) {
            switch (mChara->m50) {
            case 700022:
                motion->mTracks[0].m08 = 50.0f;
                motion->mTracks[0].m0C = 0.0f;
                break;
            case 700023:
                motion->mTracks[0].m08 = 34.0f;
                motion->mTracks[0].m0C = 0.0f;
                break;
            }
        }
        return 1;
    }
    return 0;
}

} // namespace chr
