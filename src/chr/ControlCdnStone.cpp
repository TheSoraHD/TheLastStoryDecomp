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
extern "C" void fn_80107FD8(Effect_unk* mgr, atn::Model* model, atn::Model* model2); // inferred: sleep effect
extern "C" void fn_80108080(Effect_unk* mgr, atn::Model* model);                    // inferred: stops it
extern "C" void fn_801079C0(Effect_unk* mgr, atn::Model* model, atn::Model* model2); // inferred: dizzy effect
extern "C" void fn_80107A68(Effect_unk* mgr, atn::Model* model);                    // inferred: stops it
extern "C" float fn_805F9920(const atn::Vector3* v); // inferred: length
extern "C" int fn_800DD3FC(wchar_t* dst, const wchar_t* fmt, ...); // inferred: swprintf
// inferred: message table (lbl_8087F1E4); mDC is the betrayal message
struct CdnMessages_unk {
    unsigned char m00[0xDC];
    const wchar_t* mDC;
};
extern "C" CdnMessages_unk* lbl_8087F1E4;
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text (shared by the text tables, as in MP/Text.cpp)
extern "C" void* lbl_8087FA20;
// inferred: game state (lbl_8087EFA8, as in game/CharaTask.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8; // inferred: nonzero in the arena (the betrayal of an enemy has no time limit)

// inferred: vector rotated by a matrix (by value; name invented, as in chr/ControlDropDown.cpp)
inline atn::Vector3 RotY_unk(Mtx m, const atn::Vector3& v) {
    atn::Vector3 r(v);
    PSMTXMultVec(m, &r, &r);
    return r;
}

// inferred: inline in the original (weak copy 0x8013C3B4, defined out of line in game/CharaTask.cpp)
inline atn::Vector3 atn::Vector3::operator-() const {
    return atn::Vector3(-x, -y, -z);
}

// inferred: facing direction of the rotation `rot` (name invented)
inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    Mtx mtx;
    PSMTXRotRad(mtx, 'y', rot.y);
    PSMTXMultVec(mtx, &v, &v);
    return v;
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
        mChara->fn_8013CB68(mChara->mRot, 0, 0.0f, 1.0f);
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

// NONMATCHING: 97.9% - the original loads mChara before saving r31 (prologue scheduling) and keeps
// it in r8 for the sound position.
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
    CharaInfo_unk* info = mChara->m60;
    fn_800DD3FC(text, lbl_8087F1E4->mDC != 0 ? lbl_8087F1E4->mDC : lbl_808813D0, info->m04);
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
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        if (mChara->m50 == 1) {
            mChara->fn_80154654(mChara->mC50, 0x238D);
        }
        mChara->fn_8015495C(0, 1, 1, 1);
        mChara->mC58.mC4 = 0;
        if (mChara->mType == 0) {
            mChara->mF84 = lbl_8087F0A8->m68;
        }
        mChara->SetSide_unk(m08);
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.0% - the original loads mChara before saving r31 (prologue scheduling).
boost::function<void()> ControlCdnBetray::vf10() {
    if (mChara->mType == 0) {
        return boost::bind(&CharaTask::fn_8016E970, (CharaTask*)mChara, 2);
    }
    return boost::bind(&CharaTask::fn_8016E970, (CharaTask*)mChara, mChara->m564);
}

// NONMATCHING: 99.2% - r5/r6 swapped between the status flags and m0C.
void ControlCdnBetray::vf1C() {
    if (mChara != 0) {
        mChara->fn_8015495C(0, 1, 1, 1);
        if (!mChara->mStatus.IsState_unk(0x20)) {
            if ((m0C != 0 && mChara->mStatus.IsState_unk(0x10000)) || (m0C == 0 && mChara->mStatus.IsState_unk(0x100))) {
                mChara->mC58.mC4 = 0;
                mChara->SetSide_unk(m08);
            }
        }
    }
}

ControlCdnSleep::ControlCdnSleep(Chara_unk* chara) : ControlBase(chara), m08(0), m0C(0) {
    chara->m560 = 0x6A;
    mChara->mStatus.fn_8013310C(0x10, 0);
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
    motion->fn_80097C08(0, 7, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    if (lbl_8087F048 != 0) {
        fn_80107FD8(lbl_8087F048, motion, motion);
    }
}

ControlCdnSleep::~ControlCdnSleep() {
    mChara->mStatus.fn_8013322C(0x10);
    if (lbl_8087F048 != 0) {
        fn_80108080(lbl_8087F048, &mChara->mMotion);
    }
}

int ControlCdnSleep::vf0C() {
    return 0;
}

// NONMATCHING: 99.8% - r29/r30 swapped for the functor allocated by boost::function.
boost::function<void()> ControlCdnSleep::vf10() {
    ControlCdnRestoreSleep* next = new ((MemoryTag)0, "", "", 0) ControlCdnRestoreSleep(mChara);
    return boost::bind(&CharaTask::fn_80178208, (CharaTask*)mChara, next);
}

ControlCdnRestoreSleep::ControlCdnRestoreSleep(Chara_unk* chara) : ControlBase(chara) {
    chara->m58C = 0;
    mChara->m560 = 0x6B;
    mChara->mStatus.fn_8013322C(0x10);
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 8, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    if (lbl_8087F048 != 0) {
        fn_80108080(lbl_8087F048, motion);
    }
}

int ControlCdnRestoreSleep::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    return frame >= motion->fn_80097D7C(0);
}

boost::function<void()> ControlCdnRestoreSleep::vf10() {
    if (mChara->m564 == 7) {
        return ControlBase::vf10();
    }
    return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
}

ControlCdnPiyo::ControlCdnPiyo(Chara_unk* chara, const atn::Vector3& dir, int time, int flag)
    : ControlBase(chara), m08(0), m0C(time), m10(flag) {
    chara->m58C = 0;
    mChara->m560 = 0x36;
    mChara->mStatus.fn_8013310C(0x4000, 0);
    mChara->mFlag5 = 0;
    mChara->fn_8016DA4C();
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, flag != 0 ? 0x1E1 : 0x35, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    m14 = dir;
    if (!(atn::Abs_unk(fn_805F9920(&dir)) < 1.1920929e-7f)) {
        m20 = (-dir).ToRotation_unk();
        m14.Normalize_unk();
    } else {
        m20 = mChara->mRot;
        m14 = -FrontDir_unk(mChara->mRot);
    }
    if (m0C <= 0.0f) {
        m0C = m10 != 0 ? 450.0f : 150.0f;
    }
    mChara->fn_801446F0();
    if (lbl_8087F048 != 0) {
        fn_801079C0(lbl_8087F048, motion, motion);
    }
}

ControlCdnPiyo::~ControlCdnPiyo() {
    mChara->mStatus.fn_8013322C(0x4000);
    Motion_unk* motion = &mChara->mMotion;
    if (lbl_8087F048 != 0) {
        fn_80107A68(lbl_8087F048, motion);
    }
}

// inferred: the player shakes the dizziness off faster with button 0x22
int ControlCdnPiyo::vf0C() {
    int done = 0;
    float dt = lbl_8087EFA8->m3A4;
    m0C -= dt;
    Motion_unk* motion = &mChara->mMotion;
    if (mChara->mType == 0 && lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
        float dt2 = lbl_8087EFA8->m3A4;
        m0C -= 14.0f * dt2;
    }
    switch (m08) {
    case 0: {
        if (m10 != 0) {
            mChara->m6B8 = m14 * 0.2888889f;
        } else {
            mChara->m6B8 = m14;
        }
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            m08 = 1;
            motion->m34C = 1;
            motion->mTracks[0].m20 = 1.0f;
            motion->fn_80097C08(0, 0x1E2, 1, 0, 1, 0.0f, 0.1f);
            motion->mTracks[0].m0C = 1.0f;
        }
        break;
    }
    case 1:
        mChara->m6B8 = atn::Vector3(0.0f, 0.0f, 0.0f);
        if (m0C < 0.0f || (mChara->mStatus.m0C & 0x4000) != 0x4000) {
            done = 1;
        }
        break;
    }
    mChara->vf34(&m20, 0, 0.0f, 1.0f);
    return done;
}

// NONMATCHING: 94.0% - the original loads mChara before saving r31 (prologue scheduling, as in
// chr/ControlCliffhang.cpp vf10).
boost::function<void()> ControlCdnPiyo::vf10() {
    return boost::bind(&CharaTask::fn_801684A0, (CharaTask*)mChara);
}

ControlCdnRestorePiyo::ControlCdnRestorePiyo(Chara_unk* chara) : ControlBase(chara) {
    chara->m58C = 0;
    mChara->m560 = 0x37;
    mChara->mStatus.fn_8013322C(0x4000);
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->fn_80097C08(0, 0x1E3, 0, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
}

int ControlCdnRestorePiyo::vf0C() {
    int done = 0;
    Motion_unk* motion = &mChara->mMotion;
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        done = 1;
    }
    return done;
}

boost::function<void()> ControlCdnRestorePiyo::vf10() {
    if (mChara->m564 == 7) {
        return ControlBase::vf10();
    }
    return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
}

ControlCdnPsycho::ControlCdnPsycho(Chara_unk* chara, int flag) : ControlBase(chara), m08(0), m0C(210.0f), m10(flag) {
    chara->m560 = 0x38;
    mChara->mStatus.fn_8013310C(0x20000, 0);
    mChara->mFlag5 = 0;
    mChara->fn_8016DA4C();
    if (mChara->mFlag31) {
        mChara->fn_801539E0();
    }
    if (mChara->mFlag6) {
        mChara->fn_80155DAC(0);
    }
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    m08 = 1;
    motion->fn_80097C08(0, 0x1E4, 1, 0, 1, 0.0f, 0.1f);
    motion->mTracks[0].m0C = 1.0f;
    mChara->fn_801446F0();
}

ControlCdnPsycho::~ControlCdnPsycho() {
    mChara->mStatus.fn_8013322C(0x20000);
}

// inferred: the player recovers faster with button 0x22
int ControlCdnPsycho::vf0C() {
    float dt = lbl_8087EFA8->m3A4;
    m0C -= dt;
    Motion_unk* motion = &mChara->mMotion;
    int done = 0;
    if (mChara->mType == 0 && lbl_8087F0A8->mInput.fn_8012476C(0x22)) {
        float dt2 = lbl_8087EFA8->m3A4;
        m0C -= 9.0f * dt2;
    }
    switch (m08) {
    case 0: {
        float frame = motion->mTracks[0].m08;
        if (frame >= motion->fn_80097D7C(0)) {
            m08 = 1;
            motion->m34C = 1;
            motion->mTracks[0].m20 = 1.0f;
            motion->fn_80097C08(0, 0x1E2, 1, 0, 1, 0.0f, 0.1f);
            motion->mTracks[0].m0C = 1.0f;
        }
        break;
    }
    case 1:
        mChara->m6B8 = atn::Vector3(0.0f, 0.0f, 0.0f);
        if (m0C < 0.0f) {
            done = 1;
        }
        break;
    }
    return done;
}

// NONMATCHING: 94.0% - the original loads mChara before saving r31 (as in ControlCdnPiyo::vf10).
boost::function<void()> ControlCdnPsycho::vf10() {
    return boost::bind(&CharaTask::fn_80168B1C, (CharaTask*)mChara);
}

ControlCdnRestorePsycho::ControlCdnRestorePsycho(Chara_unk* chara) : ControlBase(chara) {
    chara->m58C = 0;
    mChara->m560 = 0x39;
    mChara->mStatus.fn_8013322C(0x20000);
}

int ControlCdnRestorePsycho::vf0C() {
    return 1;
}

boost::function<void()> ControlCdnRestorePsycho::vf10() {
    if (mChara->m564 == 7) {
        return ControlBase::vf10();
    }
    return boost::bind(&CharaTask::fn_8016EB48, (CharaTask*)mChara, false, false, false);
}

} // namespace chr
