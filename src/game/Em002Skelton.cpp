#include "game/Em002Skelton.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" void fn_800EAECC(CharaTask* chara); // inferred: BattleCharaTask
extern "C" void fn_800EB49C(CharaTask* chara); // inferred: BattleCharaTask (knocked back)
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: effect manager (class unknown), global instance at 0x8087F3C0
struct EffectMgr_unk;
struct Color4f_unk {
    Color4f_unk() {}
    float r, g, b, a;
};
extern "C" EffectMgr_unk* lbl_8087F3C0;
extern "C" void fn_8023A8B4(EffectMgr_unk* mgr, atn::EfpHandle_unk* efp, Model_unk* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const Color4f_unk& color, int arg7,
                            int arg8, int arg9, float scale);
extern "C" void fn_800EE360(CharaTask* chara); // inferred: BattleCharaTask (enemies only)
// inferred: parameters of fn_8003EA3C (defined in CharaTask.cpp)
struct HitParam_unk;
extern "C" HitParam_unk lbl_807C6B90; // inferred: default parameters
// inferred: result of fn_8003EA3C (same fields as HitMsg_unk::m64-m80); names invented
struct DamageResult_unk {
    DamageResult_unk() : m00(0), m04(0), m08(0), m0C(0), m10(0), m14(-1) {
        mFlags31_28 = 0;
        m18 = -1;
    }
    int m00;
    int m04;
    int m08;
    int m0C;
    int m10;
    int m14;
    int m18;
    unsigned int mFlags31_28 : 4;
    unsigned int mFlags27_0 : 28;
};
extern "C" void fn_8003EA3C(DamageResult_unk* result, HitInfo_unk* data, CharaTask* attacker, CharaTask* owner,
                            unsigned int flags, const HitParam_unk* param, int arg, int arg2); // inferred: computes the damage

Em002Skelton::Em002Skelton(atn::Task* parent, void* data) : EnemyTask(parent, data), m14B0(0), m14B4(0), m14D0(0) {
    m14B8.fn_8023780C("/effect/ef_uc001.efp");
    m14C4.fn_8023780C("/effect/ef_cm009.efp");
}

Em002Skelton::~Em002Skelton() {}

int Em002Skelton::vf0C() {
    int result = 1;
    if (fn_8013655C() != 0) {
        result = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        result = 0;
    }
    if (result) {
        m14B0 = m5C->mParam.m98;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
        }
        m14A8 |= 0x01000000;
    }
    return result;
}

// inferred: plays an effect on the model (name invented)
inline void PlayEffect_unk(atn::EfpHandle_unk* efp, Model_unk* model) {
    atn::Vector3 pos;
    atn::Vector3 rot;
    Color4f_unk color;
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

void Em002Skelton::vfE8(HitMsg_unk* msg) {
    if (msg->mAttacker != 0 && strcmp(msg->mAttacker->fn_8017C904(), "ch3004_01") == 0) {
        fn_80232B7C(0, 0);
        PlayEffect_unk(&m14C4, &mMotion);
        vf44();
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        mFlag15 = 1;
        m14B0 = m5C->mParam.m98;
        fn_801765D8();
        return;
    }
    if (m55C != 2) {
        fn_8015495C(0, 1, 1, 1);
    }
    if ((mStatus.m0C & 0x20) == 0x20) {
        if (!(m55C == 6 && m560 == 0x3B)) {
            fn_8015E7A0(msg->m28 * 5.0f, 0x2E, 0);
        }
        fn_800EB49C(this);
    }
    if (CanAct_unk() && (mStatus.m0C & 0x40) != 0x40 && mC58.m11C.m00 >= 2) {
        if (msg->mAttacker != 0 && msg->mAttacker->mFlag28) {
            mC58.m11C.fn_8011CD84(this, 1);
        } else if (mC58.m11C.m08 >= 0 && mC58.m11C.m00 >= 2) {
            if (msg->m44 == 1) {
                mC58.m11C.fn_8011CD84(this, 4);
            } else {
                mC58.m11C.fn_8011CD84(this, 1);
            }
        }
    }
}

void Em002Skelton::vfF0() {
    if (fn_8015EB2C() == 0) {
        fn_80139560();
        if (m560 == 0x3B) {
            m14B0 = 0;
        }
    } else if (m14B0 > 0) {
        mMotion.mTracks[0].m0C = 0.0f;
        mMotion.mTracks[0].m08 = mMotion.fn_80097D7C(0);
        if (m14B4 < 0) {
            DamageResult_unk result;
            fn_8003EA3C(&result, fn_80219E6C(0x2712), this, this, 0, &lbl_807C6B90, 0, 0);
            m14B0--;
        }
        m14B4--;
    } else {
        mFlag25 = 1;
        mColli.mFlags &= ~1;
        if (m1434 > 0) {
            mFlag25 = 1;
            fn_800EAECC(this);
        } else {
            mFlag15 = 1;
            m14B0 = m5C->mParam.m98;
            fn_801765D8();
            fn_800EE360(this);
        }
        mColli.mFlags &= ~1;
    }
}

void Em002Skelton::vf44() {
    CharaTask::vf44();
    if (mType == 2) {
        m1434 = 30;
    }
    m1470 = 0;
}
