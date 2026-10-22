#include "game/Em101LittleZesya.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, int data, int id, int time, int c,
                            float f);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect

Em101LittleZesya::~Em101LittleZesya() {}

// inferred: name invented
inline float Em101LittleZesya::HpRate_unk() {
    return mStatus.m04 / (float)mStatus.mParamB.m84;
}

void Em101LittleZesya::vf10() {
    m14B8 = mC58.mC4;
    if (m14B8 == 0) {
        m14B8 = lbl_8087F8A0->mHead;
    }
    if (mC58.mC0 != 0 && m14B8 != 0) {
        m14D0++;
        fn_8025D7B0();
    }
    m14F0 = m14EC;
    fn_8014C540();
    fn_80145334();
}

extern "C" void fn_8025D6D0(Em101LittleZesya* zesya) {
    zesya->m14FC = zesya->mPos;
    zesya->m14F8 = 1;
    zesya->m1508 = zesya->mRot;
}

inline void Em101LittleZesya::Idle_unk() {
    m14D0 = 0;
    m14F4 = 0;
    m58C = 0;
    fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, this, 500, 1);
    fn_80239DAC(lbl_8087F3C0, this, 102, 1);
}

extern "C" void fn_8025D704(Em101LittleZesya* zesya) {
    zesya->mColli.mFlags |= 1;
    zesya->fn_8016DA4C();
    zesya->Idle_unk();
    zesya->fn_8016E970(3);
    zesya->fn_8015495C(0, 1, 1, 1);
    zesya->m14F8 = 0;
}

void Em101LittleZesya::fn_8025DE54() {
    if (m55C - 6U > 1) {
        mFlag5 = 1;
        mFC0 = m14B8;
    }
    fn_80139560();
}

void Em101LittleZesya::fn_8025E748() {
    float frame = mMotion.mTracks[0].m08;
    if (17.0f < frame && frame <= 19.0f) {
        fn_800F8C6C(lbl_8087F048, 0, 0, this, m1528, m590, 30, -1, 0.0f);
    } else {
        frame = mMotion.mTracks[0].m08;
        if (frame >= mMotion.fn_80097D7C(0)) {
            Idle_unk();
        }
    }
    m1540 = mMotion.mTracks[0].m08;
}

// NONMATCHING: ~95% - f1/f2 of the int->float conversion of the maximum HP (4 variants)
void Em101LittleZesya::fn_8025E930() {
    mStatus.mParamB.m8C += m1568;
    if (HpRate_unk() >= 0.99f) {
        Idle_unk();
    }
}

void Em101LittleZesya::fn_8025F754() {
    if (m55C != 6) {
        Idle_unk();
    }
    fn_80139560();
}

void Em101LittleZesya::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C - 11U > 2) {
        if (m58C == 10) {
            msg->m10 *= 0.0f;
            msg->m64 = 3;
            msg->m90 = 0;
            msg->m84 = 2;
            msg->m88 = msg->mData->mC4;
            msg->m68 = 0;
            return;
        }
    } else {
        msg->m10 *= 0.0f;
    }
    fn_80151448(msg);
    vf40(hit);
    m14D0 += 30;
}

extern "C" void fn_8025FE30(Em101LittleZesya* zesya, const ZesyaWarp_unk* warp) {
    zesya->m15BC = 1;
    zesya->m15A8 = *warp;
}
