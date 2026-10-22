#include "game/Em101LittleZesya.h"
#include "chr/ControlBase.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "math/Vec.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: effect/bullet manager (lbl_8087F048, as in chr/ControlUseSkill.cpp); class unknown
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800F8C6C(Effect_unk* mgr, int a, int b, CharaTask* chara, int data, int id, int time, int c,
                            float f);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
extern "C" EffectMgr_unk* lbl_8087F3C0;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg); // inferred: owner of the next effects

// inferred: plays an effect on the model (locals in the order of game/Em003LesserDaemon.cpp)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
    Color4f_unk color;
    atn::Vector3 rot;
    atn::Vector3 pos;
    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = 0.0f;
    pos.y = 0.0f;
    pos.z = 0.0f;
    color.r = 1.0f;
    color.g = 1.0f;
    color.b = 1.0f;
    color.a = 1.0f;
    fn_8023A8B4(lbl_8087F3C0, efp, model, 0, pos, rot, color, -1, -1, 1, 1.0f);
}
extern "C" void fn_80239DAC(EffectMgr_unk* mgr, void* owner, int id, int arg); // inferred: stops an effect

Em101LittleZesya::~Em101LittleZesya() {}

// inferred: name invented
inline float Em101LittleZesya::HpRate_unk() {
    return (float)mStatus.m04 / (float)mStatus.mParamB.m84;
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

void Em101LittleZesya::fn_8025DE7C() {
    if (fn_80158BB4()) {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 0;
        fn_8016E970(3);
        fn_80239DAC(lbl_8087F3C0, this, 500, 1);
        fn_80239DAC(lbl_8087F3C0, this, 102, 1);
    } else {
        fn_80139560();
        if (lbl_8087F048 != 0 && fn_80158CA4() != 0) {
            if (mControl && mControl->vf30() > m598) {
                m598 = mControl->vf30();
                m594 = 1;
                m590 = fn_800F8548(lbl_8087F048);
            }
            fn_80144710();
            fn_800F8C6C(lbl_8087F048, 0, 0, this, (int)m638, m590, 30, -1, 0.0f);
        }
    }
}

void Em101LittleZesya::fn_8025E008() {
    m14D0 = 0;
    m14F4 = 0;
    m58C = 11;
    fn_8016E970(3);
    mMotion.m34C = 1;
    mMotion.mTracks[0].m20 = 1.0f;
    mMotion.mTracks[0].m0C = 1.0f;
    mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
    fn_80232B7C(this, 101);
    PlayEffect_unk(&m156C, &mMotion);
    m1548 = 2;
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
void Em101LittleZesya::fn_8025E834() {
    if (m14D0 > 10) {
        m14D0 = 0;
        m14F4 = 0;
        m58C = 13;
        fn_8016E970(3);
        mMotion.m34C = 1;
        mMotion.mTracks[0].m20 = 1.0f;
        mMotion.mTracks[0].m0C = 1.0f;
        mMotion.fn_80097C08(0, 0x1C4, 1, 0, 1, 0.0f, 0.1f);
        fn_80232B7C(this, 500);
        PlayEffect_unk(&m1578, &mMotion);
    }
}

// inferred: back to state 0 (name invented; the same block is inlined in fn_8025DE7C and fn_8025E9E4)
inline void ResetState_unk(Em101LittleZesya* z) {
    z->m14D0 = 0;
    z->m14F4 = 0;
    z->m58C = 0;
    z->fn_8016E970(3);
    fn_80239DAC(lbl_8087F3C0, z, 500, 1);
    fn_80239DAC(lbl_8087F3C0, z, 102, 1);
}

// NONMATCHING: ~95% - load order of the node column (0x2C/0x1C/0x0C) and the offset of "hips" in the string
// pool (the pool is in the auto split until all the strings of the unit exist); 4 variants
void Em101LittleZesya::fn_8025E9E4() {
    if (m14F4 == 0) {
        if (mMotion.mTracks[0].m08 > 60.0f) {
            m12CC = 1.0f;
            MtxPtr mtx = mMotion.GetNodeMtx_unk("hips");
            atn::Vector3 pos;
            pos.x = mtx[0][3];
            pos.y = mtx[1][3];
            pos.z = mtx[2][3];
            fn_8025EAE4();
            m14F4 = 1;
        }
    } else {
        float frame = mMotion.mTracks[0].m08;
        if (frame >= 100.0f) {
            ResetState_unk(this);
        }
    }
}

void Em101LittleZesya::fn_8025F35C() {
    switch (m55C) {
    case 6:
        if (m560 != 2) {
            ResetState_unk(this);
            m14D0 = m15C0 / 2;
        }
        break;
    case 7:
        break;
    default:
        ResetState_unk(this);
        m14D0 = m15C0 / 2;
        break;
    }
    fn_80139560();
}

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
