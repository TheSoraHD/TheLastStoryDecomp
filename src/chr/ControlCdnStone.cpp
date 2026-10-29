// Status-condition controls (stone, paralysis, betrayal, sleep, dizziness "piyo", confusion
// "psycho") and their restore controls; the out-of-line virtuals of ControlBase are at the end.
#include "chr/ControlCdn.h"
#include "chr/ControlDamage.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include <boost/bind.hpp>

// inferred: game settings (the pad state is at 0x48C)
struct Input_unk {
    int fn_8012476C(int button); // inferred: nonzero while `button` is pressed
};
struct Settings_unk {
    unsigned char m000[0x68];
    int m68; // inferred: side of the player (restored after a betrayal)
    unsigned char m06C[0x48C - 0x6C];
    Input_unk mInput; // 0x48C
};
extern "C" Settings_unk* lbl_8087F0A8;

struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_80107860(Effect_unk* mgr, atn::Model* model, atn::Model* model2); // inferred: stone break effect
extern "C" void fn_80101524(Effect_unk* mgr, const atn::Vector3* pos, const atn::Vector3* rot, int id, int arg);
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" void fn_801076D0(Effect_unk* mgr, atn::Model* model, atn::Model* model2); // inferred: paralysis effect
extern "C" void fn_80107798(Effect_unk* mgr, atn::Model* model, int arg);           // inferred: stops it
extern "C" void fn_80107850(Effect_unk* mgr, atn::Model* model);                    // inferred: stops the betrayal effect
extern "C" void fn_80107A78(Effect_unk* mgr, atn::Model* model, atn::Model* model2);
extern "C" void fn_801077A8(Effect_unk* mgr, atn::Model* model, atn::Model* model2);
extern "C" void fn_801028E4(Effect_unk* mgr, int a, CharaTask* chara, int b);
extern "C" void fn_80109828(Effect_unk* mgr, const wchar_t* text); // inferred: shows a message
extern "C" int fn_800DD3FC(wchar_t* dst, const wchar_t* fmt, ...); // inferred: swprintf
// inferred: message table (lbl_8087F1E4); mDC is the betrayal message
struct CdnMessages_unk {
    unsigned char m00[0xDC];
    const wchar_t* mDC;
};
extern "C" CdnMessages_unk* lbl_8087F1E4;
extern "C" void* lbl_8087FA20; // inferred: nonzero in the arena (the betrayal of an enemy has no time limit)

// inferred: vector rotated by a matrix (by value; name invented, as in chr/ControlDropDown.cpp)
inline atn::Vector3 RotY_unk(Mtx m, const atn::Vector3& v) {
    atn::Vector3 r(v);
    PSMTXMultVec(m, &r, &r);
    return r;
}

namespace chr {

ControlCdnStone::ControlCdnStone(Chara_unk* chara, int a) : ControlBase(chara), m08(0.0f, 0.0f, 0.0f), m24(a) {
    chara->m58C = 3;
    mChara->m560 = 0x30;
    mChara->mStatus.fn_8013310C(0x40, 0);
    mChara->mF94 = 0;
    mChara->mFlag5 = 0;
    mChara->fn_8016DA4C();
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->HasC48_unk()) {
        mChara->fn_80154344();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    mChara->fn_80164DCC(1);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', mChara->mRot.y - 1.5707964f);
    m14 = RotY_unk(mtx, atn::Vector3(0.0f, 0.0f, 0.25f));
    m14.y = 0.0f;
    mChara->mMotion.mTracks[0].m0C = 0.0f;
    m20 = 450;
    if (a == 0) {
        fn_800C344C("SE_BTCOM_071", &mChara->mPos, 0, -1, 1.0f);
    }
}

// inferred: the player shakes the stone off with button 0x22
int ControlCdnStone::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mType == 0) {
        if (lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
            m20 -= 14;
            if (m14.y <= 0.0f) {
                Mtx mtx;
                PSMTXRotRad(mtx, 'y', 3.1415927f);
                PSMTXMultVec(mtx, &m14, &m14);
                m14.y = 6.0f;
                fn_80107860(lbl_8087F048, motion, motion);
            }
        }
        if (m14.y > 0.0f) {
            m14.y -= 1.0f;
            float t;
            if (m14.y > 3.0f) {
                t = (3.0f - m14.y) / 3.0f;
            } else {
                t = m14.y / 3.0f;
            }
            m08.x = m14.x * t;
            m08.y = 0.0f;
            m08.z = m14.z * t;
        } else {
            m08 = atn::Vector3(0.0f, 0.0f, 0.0f);
        }
        mChara->mF88 = m08;
    }
    if (mChara->mType != 2 || (mChara->m54C & 0x2000) != 0x2000) {
        mChara->fn_8013CB68(&mChara->mRot, 0, 0.0f, 1.0f);
        if ((mChara->mStatus.m0C & 0x20) == 0x20) {
            return 1;
        }
    }
    if (m24 == 0) {
        m20--;
    }
    if (m20 <= 0) {
        return 1;
    }
    return 0;
}

inline ControlCdnRestoreStone::ControlCdnRestoreStone(Chara_unk* chara) : ControlBase(chara) {
    chara->m58C = 0;
    mChara->m560 = 0x31;
    mChara->mStatus.fn_8013322C(0x40);
    mChara->mF94 = -1;
    mChara->mMotion.mTracks[0].m0C = 0.0f;
    if (lbl_8087F048 != 0) {
        fn_80101524(lbl_8087F048, &mChara->mPos, &mChara->mRot, 0x40, 0);
    }
}

boost::function<void()> ControlCdnStone::vf10() {
    if (mChara != 0 && (mChara->mStatus.m0C & 0x20) == 0x20) {
        mChara->vf44();
        atn::Vector3 dir(0.0f, 0.0f, -0.01f);
        Mtx mtx;
        PSMTXRotRad(mtx, 'y', mChara->mRot.y);
        PSMTXMultVec(mtx, &dir, &dir);
        ControlKnockdown* next = new ((MemoryTag)0, "", "", 0) ControlKnockdown(mChara, dir, -1, 0);
        return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
    }
    ControlCdnRestoreStone* next = new ((MemoryTag)0, "", "", 0) ControlCdnRestoreStone(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

int ControlCdnRestoreStone::vf0C() {
    return 1;
}

ControlCdnParalyzed::ControlCdnParalyzed(Chara_unk* chara) : ControlBase(chara) {
    chara->m58C = 4;
    mChara->m560 = 0x32;
    mChara->mStatus.fn_8013310C(0x80, 0);
    mChara->mFlag5 = 0;
    mChara->fn_8016DA4C();
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->HasC48_unk()) {
        mChara->fn_80154344();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1DC, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    fn_801076D0(lbl_8087F048, motion, motion);
}

int ControlCdnParalyzed::vf0C() {
    return 0;
}

void ControlCdnParalyzed::vf1C() {
    if (mChara != 0 && lbl_8087F048 != 0) {
        mChara->mStatus.fn_8013322C(0x80);
        fn_80107798(lbl_8087F048, &mChara->mMotion, 1);
    }
}

boost::function<void()> ControlCdnParalyzed::vf10() {
    ControlCdnRestoreParalyzed* next = new ((MemoryTag)0, "", "", 0) ControlCdnRestoreParalyzed(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

ControlCdnRestoreParalyzed::ControlCdnRestoreParalyzed(Chara_unk* chara) : ControlBase(chara) {
    chara->m58C = 0;
    mChara->m560 = 0x33;
    mChara->mStatus.fn_8013322C(0x80);
    fn_80107798(lbl_8087F048, &mChara->mMotion, 1);
    fn_800C344C("SE_BTCOM_074", &mChara->mPos, 0, -1, 1.0f);
}

int ControlCdnRestoreParalyzed::vf0C() {
    return 1;
}

ControlCdnBetray::ControlCdnBetray(Chara_unk* chara, int a, int b) : ControlBase(chara), m08(a), m0C(b) {
    chara->m58C = 0;
    if (m0C != 0) {
        mChara->m560 = 0x35;
        if (mChara->mType == 2 && lbl_8087FA20 == 0) {
            mChara->mStatus.fn_80133130(0x10000, 450, 0);
        } else {
            mChara->mStatus.fn_8013310C(0x10000, 0);
        }
    } else {
        mChara->m560 = 0x34;
        mChara->mStatus.fn_8013310C(0x100, 0);
    }
    mChara->fn_8016DA4C();
    wchar_t text[64];
    fn_800DD3FC(text, lbl_8087F1E4->mDC != 0 ? lbl_8087F1E4->mDC : L"", mChara->m60->m04);
    fn_80109828(lbl_8087F048, text);
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, m0C != 0 ? 0x72 : 0x40, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    if (m0C != 0) {
        fn_80107A78(lbl_8087F048, motion, motion);
    } else {
        fn_801077A8(lbl_8087F048, motion, motion);
    }
    fn_800C344C("SE_BTCOM_075", &mChara->mPos, 0, -1, 1.0f);
    fn_801028E4(lbl_8087F048, 0, mChara, 0);
}

int ControlCdnBetray::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    if (m0C != 0) {
        if ((mChara->mStatus.m0C & 0x10000) != 0x10000) {
            return 1;
        }
    } else if ((mChara->mStatus.m0C & 0x100) != 0x100) {
        fn_80107850(lbl_8087F048, motion);
        return 1;
    }
    if (motion->mTracks[0].m08 >= motion->fn_80097D7C(0)) {
        if (mChara->m50 == 1) {
            mChara->fn_80154654(mChara->mC50, 0x238D);
        }
        mChara->fn_8015495C(0, 1, 1, 1);
        mChara->mC58.mC4 = 0;
        if (mChara->mType == 0) {
            mChara->mF84 = lbl_8087F0A8->m68;
        }
        mChara->mF18 = mChara->mF14;
        mChara->mF14 = m08;
        return 1;
    }
    return 0;
}

boost::function<void()> ControlCdnBetray::vf10() {
    if (mChara->mType == 0) {
        return boost::bind(&CharaTask::fn_8016E970, (CharaTask*)mChara, 2);
    }
    return boost::bind(&CharaTask::fn_8016E970, (CharaTask*)mChara, mChara->m564);
}

void ControlCdnBetray::vf1C() {
    if (mChara != 0) {
        mChara->fn_8015495C(0, 1, 1, 1);
        if ((mChara->mStatus.m0C & 0x20) != 0x20) {
            if ((m0C != 0 && (mChara->mStatus.m0C & 0x10000) == 0x10000) ||
                (m0C == 0 && (mChara->mStatus.m0C & 0x100) == 0x100)) {
                mChara->mC58.mC4 = 0;
                mChara->mF18 = mChara->mF14;
                mChara->mF14 = m08;
            }
        }
    }
}

} // namespace chr
