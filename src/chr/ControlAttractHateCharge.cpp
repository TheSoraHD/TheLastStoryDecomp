#include "chr/ControlAttractHateCharge.h"
#include "atn/SoundRef.h"
#include "game/GimmickCommon.h"
#include "game/GimmickManager.h"

extern "C" int sprintf(char* buf, const char* fmt, ...);
extern "C" unsigned long strlen(const char* s);

// inferred: effect/bullet manager (lbl_8087F048, as in game/CharaTask.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_80105C9C(Effect_unk* effect, CharaTask* chara, chr::Motion_unk* motion);
extern "C" void fn_80105DCC(Effect_unk* effect, CharaTask* chara, chr::Motion_unk* motion, int arg);
extern "C" void fn_8010607C(Effect_unk* effect, CharaTask* chara);
extern "C" void fn_80105F3C(Effect_unk* effect, CharaTask* chara);
extern "C" void fn_80102A40(Effect_unk* effect, int arg, CharaTask* chara, int a, int b);
// inferred: RGBA colour (names invented)
struct Color4f_unk {
    Color4f_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};
extern "C" void fn_801070C8(Effect_unk* effect, int id, int a, int b, chr::Motion_unk* motion,
                            const atn::Vector3* pos, const atn::Vector3* rot, const Color4f_unk* color, float scale);
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" void fn_800FB4B0(Effect_unk* mgr, CharaTask* chara, HitInfo_unk* data, int id, const atn::Vector3* pos,
                            int a, int time, int b, int c);
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int arg3, const atn::Vector3* pos,
                            const atn::Vector3* rot, int a, int time, int b, int c, float f1, float f2);
extern "C" const char* fn_8021FD7C(int table, int id); // inferred: text of `id` in a table
// inferred: effect manager behind lbl_8087F498 (as in chr/ControlGmkCoffinOpening.cpp)
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F498;
extern "C" void fn_803EA77C(EffectMgr_unk* mgr, CharaTask* chara, int id, int arg, float a, float b);
// inferred: party manager (as in game/CharaTask.cpp)
extern "C" void* lbl_8087EE68;
extern "C" void* fn_800185B4(void* manager, CharaTask* chara, HitInfo_unk* hit);
extern "C" void fn_80018608(void* manager, CharaTask* chara, HitInfo_unk* hit);
extern "C" void* lbl_8087F610; // inferred: arena mode (as in game/CharaTask.cpp)

// inferred: game settings (lbl_8087F0A8, as in game/CharaTask.cpp); names invented
struct Settings_unk {
    unsigned char m000[0x254];
    int m254;
    unsigned char m258[0x304 - 0x258];
    int m304;
    unsigned char m308[0x3CC - 0x308];
    int m3CC; // inferred: id passed to fn_80210220
};
extern "C" Settings_unk* lbl_8087F0A8;
// inferred: game state (lbl_8087EFA8, as in game/CharaTask.cpp); names invented
struct GameState_unk {
    unsigned char m000[0x3A4];
    float m3A4; // inferred: time of a frame
};
extern "C" GameState_unk* lbl_8087EFA8;
// inferred: record returned by fn_80210220 (as in game/CharaTask.cpp); names invented
struct Record80210220_unk {
    unsigned char m00[0x1C];
    int m1C;
    unsigned char m20[0x28 - 0x20];
    int m28;
    int m2C;
    unsigned char m30[0x48 - 0x30];
    int m48;
    int m4C;
};
extern "C" Record80210220_unk* fn_80210220(int id);
// inferred: argument of CharaTask::fn_80164B00 (defined in game/CharaTask.cpp)
struct CharaShade_unk {
    CharaShade_unk() {
        m04 = 0.0f;
        m08 = 0.0f;
        m0C = 0.0f;
        m10 = 0;
        m00 = 1.0f;
    }
    float m00;
    float m04;
    float m08;
    float m0C;
    int m10;
};

// inferred: sound effects of chr::ControlAttractHateCharge (start and end)
static const char* const sSe[] = {"SE_BTCOM_026", "SE_BTCOM_040"};

namespace chr {

ControlAttractHateCharge::ControlAttractHateCharge(Chara_unk* chara) : ControlBase(chara) {
    chara->m560 = 0x2E;
    Motion_unk* motion = &mChara->mMotion;
    mChara->fn_8015495C(0, 1, 1, 1);
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, 0x173, 0, 0, 1, 0.0f, 0.1f);
    mTime = motion->fn_80097D7C(0);
    fn_800C344C(sSe[0], &mChara->mPos, 0, -1, 1.0f);
    mChara->fn_801446F0();
    fn_80105C9C(lbl_8087F048, mChara, motion);
    fn_80105DCC(lbl_8087F048, mChara, motion, 1);
}

int ControlAttractHateCharge::vf0C() {
    int result = 0;
    float step = lbl_8087EFA8->m3A4;
    mTime -= step;
    if (mTime <= 0.0f) {
        if (mChara->fn_8016DCD4()) {
            Settings_unk* settings = lbl_8087F0A8;
            Record80210220_unk* rec = fn_80210220(settings->m3CC);
            const atn::Vector3* pos = &mChara->mPos;
            fn_800C344C(sSe[1], pos, 0, -1, 1.0f);
            mChara->mFlag27 = 0;
            if (rec->m1C > 0) {
                CharaShade_unk shade;
                shade.m04 = rec->m48;
                shade.m08 = rec->m4C;
                shade.m0C = settings->m254;
                mChara->fn_80164B00(shade);
            }
            fn_80102A40(lbl_8087F048, 0, mChara, rec->m28, rec->m2C);
            for (GimmickCommon* g = lbl_8087F4A0->mHead; g != 0; g = g->m5C) {
                switch (g->m50) {
                case 0x2B: {
                    GimmickMsg_unk msg(3);
                    msg.m10 = (int)mChara;
                    msg.mVec = mChara->mPos;
                    g->vf40(&msg);
                    break;
                }
                case 0x41:
                    if (g->vf60()) {
                        GimmickMsg_unk msg(3);
                        msg.m10 = (int)mChara;
                        msg.mVec = mChara->mPos;
                        g->vf40(&msg);
                    }
                    break;
                }
            }
        } else {
            mChara->fn_80164DCC(1);
        }
        result = 1;
    }
    return result;
}

void ControlAttractHateCharge::vf1C() {
    fn_8010607C(lbl_8087F048, mChara);
    fn_80105F3C(lbl_8087F048, mChara);
}

ControlOverdrive::ControlOverdrive(Chara_unk* chara, int skill)
    : ControlBase(chara), mSkill(skill), mFired(0), mEffectDone(0) {
    chara->m560 = 0x84;
    Motion_unk* motion = &mChara->mMotion;
    motion->m34C = 1;
    motion->mTracks[0].m20 = 1.0f;
    motion->mTracks[0].m0C = 1.0f;
    motion->fn_80097C08(0, 0x17C, 0, 0, 1, 0.0f, 0.1f);
    mChara->fn_801446F0();
    mChara->fn_8015495C(0, 1, 1, 1);
    mChara->mFlag5 = 0;
    mChara->mFC0 = 0;
    if (mEffectDone == 0) {
        mEffectDone = 1;
        HitInfo_unk* hit = fn_80219E6C(mSkill);
        if (lbl_8087F048 != 0) {
            Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
            fn_801070C8(lbl_8087F048, hit->mB0, 0, 0, &mChara->mMotion, &lbl_807C7030, &lbl_807C7030, &color, 1.0f);
        }
    }
    if (mChara->m7C != 0 && fn_8021FD7C(mChara->m7C, 0x24) != 0) {
        char buf[0x100];
        sprintf(buf, "%s", fn_8021FD7C(mChara->m7C, 0x24));
        buf[strlen(buf) - 1] = 0;
        fn_800C31F4(buf, 0, -1, 1.0f);
    }
    if (lbl_8087F498 != 0) {
        fn_803EA77C(lbl_8087F498, mChara, 0x24, 0, 1.0f, 10.0f);
    }
    mChara->SetHit_unk(fn_80219E6C(mSkill));
}

// NONMATCHING: 99.9% - the dead comparison (0.2 against the frame time) uses f0/f1 the other way round
int ControlOverdrive::vf0C() {
    Motion_unk* motion = &mChara->mMotion;
    int result = 0;
    if (mChara->mType == 0 && lbl_8087F610 == 0 && motion->mTracks[0].m08 >= 14.0f &&
        motion->mTracks[0].m08 <= 20.0f) {
        float d = lbl_8087EFA8->m3A4 - 0.1f;
        if (d > 0.2f) {
            d = 0.2f;
        }
    }
    if (motion->mTracks[0].m08 >= 50.0f && mFired == 0) {
        mFired = 1;
        HitInfo_unk* hit = fn_80219E6C(mSkill);
        mChara->SetHit_unk(hit);
        float rate = 1.0f;
        rate += mChara->mStatus.fn_80134168(0x36, -1) / 100.0f;
        int time = hit->m5C * rate;
        mChara->mStatus.m2F4 = time;
        mChara->mStatus.m2F8 = time;
        Effect_unk* mgr = lbl_8087F048;
        fn_800FB4B0(mgr, mChara, hit, fn_800F8548(mgr), &mChara->mPos, 0, 30, -1, 0);
        mChara->mStatus.m228 = 0.0f;
        if (lbl_8087EE68 != 0 && fn_800185B4(lbl_8087EE68, mChara, hit)) {
            fn_80018608(lbl_8087EE68, mChara, hit);
        }
        if (lbl_8087F0A8->m304 != 0) {
            Effect_unk* mgr2 = lbl_8087F048;
            fn_800FAB80(mgr2, mChara, fn_80219E6C(0x1F6), fn_800F8548(mgr2), &mChara->mPos, &mChara->mRot, 0, 30, -1, -1,
                        0.0f, 1.0f);
        }
    }
    float frame = motion->mTracks[0].m08;
    if (frame >= motion->fn_80097D7C(0)) {
        result = 1;
    }
    return result;
}

} // namespace chr
