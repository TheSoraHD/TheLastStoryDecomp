#include "game/Np131LivingArmor.h"
#include "game/GameUI.h"
#include "game/GimmickCommon.h"
#include "std/vector.h"
#include "atn/DrawHelper.h"

extern "C" int rand();
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" void fn_800E2FE0(BattleCharaTask* chara); // inferred: BattleCharaTask per-frame update
// inferred: color of an effect (as in atn/EffectManager.cpp)
struct EffectColor_unk {
    EffectColor_unk() {}
    float r, g, b, a;
};
namespace atn {
// inferred: effect manager (atn/EffectManager.cpp); only the member called here is declared
class EffectManager {
public:
    void fn_8023A184(unsigned int owner, int arg, const EffectColor_unk* color); // inferred: sets the color
};
} // namespace atn
extern "C" atn::EffectManager* lbl_8087F3C0;
extern "C" void fn_80239DAC(atn::EffectManager* mgr, void* owner, int id, int arg); // inferred: stops an effect
extern "C" void fn_8023A680(atn::EffectManager* mgr, atn::EffFile* res, int arg2, int arg3, const void* mtx,
                            const atn::Vector3* pos, const atn::Vector3* rot, const EffectColor_unk* color,
                            int arg8, int arg9, int arg10, float scale); // inferred: plays an effect
extern "C" void fn_8023A8B4(atn::EffectManager* mgr, atn::EfpFile* efp, atn::Model* model, int arg3,
                            const atn::Vector3& pos, const atn::Vector3& rot, const EffectColor_unk& color,
                            int arg7, int arg8, int arg9, float scale);
extern "C" void fn_80232B7C(void* owner, int arg);
// inferred: bullets (as in gmk/LandMine.cpp)
struct BulletMgr_unk;
extern "C" BulletMgr_unk* lbl_8087F048;
extern "C" int fn_800F8548(BulletMgr_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(BulletMgr_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id,
                            const atn::Vector3* pos, const atn::Vector3* rot, int arg6, int arg7, int arg8,
                            int arg9, float arg10, float arg11);
extern "C" void fn_80105BD8(BulletMgr_unk* mgr, atn::Model* model);
// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78 and written with fn_80370AE4
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);
extern "C" void fn_80370AE4(Field_unk* field, int index, int value);
// inferred: game settings
struct Settings_unk {
    unsigned char m00[0x34];
    int mDrawDebug; // 0x34
};
extern "C" Settings_unk* lbl_8087F0A8;
struct Camera_unk;
extern "C" Camera_unk* lbl_8087EFB4;
extern "C" void fn_800BFAC8(atn::Vector3* out, Camera_unk* camera, const atn::Vector3* pos); // inferred: to screen
extern "C" void fn_803E2A30(void* draw, int kind, int alpha, atn::Vector3 pos, int a, int b);
// inferred: player manager
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
extern "C" GameUI* lbl_8087F490;
extern "C" void fn_803E3384(GameUI* ui);
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg, int arg2, float volume); // inferred: plays a sound

// inferred: colors of the fade of the death sequence (names invented)
static float sFadeColorFrom = 0.8f;
static float sFadeColorTo = 1.0f;
static float sFadeAlphaFrom = 0.5f;
static float sFadeAlphaTo = 1.0f;
// inferred: fn_80547468 runs once per frame for all the armors (cleared by vf14)
static int sChecked;

Np131LivingArmor::Np131LivingArmor(atn::Task* parent, const EnemyInfo_unk* info)
    : EnemyTask(parent, info->m20), m14B0(0), m14B4(0), m14E0(0), m14E4(0), m14E8(0), m14F8(0), m14FC(0) {
    m54C |= 0x02000000;
    m14B8.LoadRequest("ef_uc069.eff");
    m14C4.LoadRequest("gm005_009a_01.eff");
    m14D0.LoadRequest("ef_uc068.efp");
    mFlagB13 = 1;
}

Np131LivingArmor::~Np131LivingArmor() {}

int Np131LivingArmor::vf0C() {
    if (m14B8.IsLoading() != 0 || m14C4.IsLoading() != 0 || m14D0.IsLoading() != 0) {
        return 0;
    }
    if (BattleCharaTask::vf0C() != 0) {
        mStatus.m18 |= 0x180;
        m14EC = 0.8f * mStatus.mParamB.m84;
        mStatus.m18 |= 0x10041;
        return 1;
    }
    return 0;
}

// inferred: a random armor stand that is free (names invented)
inline GimmickCommon* PickArmorStand_unk() {
    std::vector<GimmickCommon*> list;
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (g->mId == 0x1391 && g->m54 == 2) {
            list.push_back(g);
        }
    }
    GimmickCommon* result;
    if (list.size() == 0) {
        result = 0;
    } else {
        result = list[rand() % list.size()];
    }
    return result;
}

// inferred: linear interpolation (name invented)
inline float LerpF_unk(float a, float b, float t) {
    return (b - a) * t + a;
}

// NONMATCHING: 96.8% - std::vector::push_back loads the capacity before the size (open case in
// std/vector.h) and the stack slots of the locals differ
void Np131LivingArmor::vf10() {
    m14A8 |= 0x04000000;
    mFlag26 = 1;
    switch (m58C) {
    case 6:
        fn_80546B84();
        m14A8 &= ~0x04000000;
        mFlag26 = 0;
        m54C |= 0x2000;
        break;
    case 7:
        fn_80139560();
        if (m55C != 6) {
            m58C = 0;
            if (mPos.y > 90.0f) {
                atn::Vector3 v(0.0f, 0.0f, 1.0f);
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, &v, &v);
                m6B8 = v * 3.0f;
            }
            fn_8016EB48(0, 0, 0);
            fn_8015495C(0, 1, 1, 1);
        }
        fn_80145334();
        fn_8014C540();
        m14A8 &= ~0x04000000;
        mFlag26 = 0;
        m54C &= ~0x2000;
        break;
    case 2:
        if (m14E0 == 1) {
            m14E8++;
            mPos.y += 0.2f;
            mRot.y += 0.31415927f;
            m540.x *= 0.99f;
            m540.y *= 0.99f;
            m540.z *= 0.99f;
            EffectColor_unk color;
            color.r = color.g = color.b = LerpF_unk(sFadeColorFrom, sFadeColorTo, m14E8 / 60.0f);
            color.a = LerpF_unk(sFadeAlphaFrom, sFadeAlphaTo, m14E8 / 60.0f);
            lbl_8087F3C0->fn_8023A184((unsigned int)&m14B8, 0, &color);
            fn_80145334();
            fn_8014C540();
            if (m14E8 > 60) {
                atn::Vector3 pos(mPos);
                pos.y += 8.0f;
                fn_80239DAC(lbl_8087F3C0, &m14B8, 0, 1);
                BulletMgr_unk* mgr = lbl_8087F048;
                fn_800FAB80(mgr, this, fn_80219E6C(0x7F1), fn_800F8548(mgr), &mPos, &mRot, 0, 30, -1, -1, 0.0f,
                            1.0f);
                vf44();
                m14E0 = 2;
            }
        } else if (m14E0 == 2) {
            vfF0();
            fn_80145334();
            fn_8014C540();
        } else if (fn_8015EB2C() == 0) {
            vfF0();
            fn_80145334();
            fn_8014C540();
        } else {
            fn_80105BD8(lbl_8087F048, &mMotion);
            GimmickCommon* stand = PickArmorStand_unk();
            fn_8016E4C4(0);
            m14DC = stand;
            m58C = 6;
            EffectColor_unk color;
            color.r = 0.8f;
            color.g = 0.8f;
            color.b = 0.8f;
            color.a = 0.5f;
            fn_80232B7C(&m14B8, 0);
            fn_8023A680(lbl_8087F3C0, &m14B8, -1, 5, &mMotion.mMtx, 0, 0, &color, 0, -1, 1, 1.0f);
            mPos.y += 4.0f;
            fn_80144710();
            mStatus.fn_8012D8B8();
            mStatus.m04 = m14EC;
            mC58.mC0 = 1;
            m14B4 = 0;
            m14F0 = 0;
            if (fn_80370A78(lbl_8087F430, 250) == 0) {
                fn_80370AE4(lbl_8087F430, 250, m58);
            }
        }
        m54C &= ~0x2000;
        break;
    default:
        fn_800E2FE0(this);
        m54C &= ~0x2000;
        break;
    }
    if (sChecked == 0) {
        fn_80547468();
        sChecked = 1;
    }
}

void Np131LivingArmor::vf14() {
    sChecked = 0;
    fn_80149A30();
    if (lbl_8087F0A8->mDrawDebug != 0) {
        lbl_8087EEB0->fn_80063484(20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

void Np131LivingArmor::vfC0(void* draw) {
    if (m58C == 2 && m14E0 == 1) {
        float alpha = 1.0f - m14E8 / 60.0f;
        alpha *= 90.0f;
        atn::Vector3 pos = mPos + atn::Vector3(0.0f, 10.0f, 0.0f);
        atn::Vector3 screen;
        fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
        if (0.0f < screen.z && screen.z < 1.0f) {
            int a = 0.999f + alpha;
            if (a < 0) {
                a = 0;
            }
            fn_803E2A30(draw, 2, a, screen, 1, 0);
        }
    }
}

// NONMATCHING: 96.4% - as vf10 (push_back), and `this` is kept in r18 in the original
void Np131LivingArmor::fn_80546B84() {
    if (m14DC != 0 && m14DC->m54 != 2) {
        m14DC = PickArmorStand_unk();
    }
    if (m14DC == 0) {
        m14E0 = 1;
        m14E8 = 0;
        vf44();
        return;
    }
    m14F0++;
    atn::Vector3 target;
    target.Assign_unk(m14DC->m6C);
    target.y += 10.0f;
    atn::Vector3 d(target.x - mPos.x, target.y - mPos.y, target.z - mPos.z);
    if (d.x * d.x + d.z * d.z > 625.0f) {
        d.y = 0.0f;
    }
    if (d.Length_unk() < 4.0f) {
        fn_80239DAC(lbl_8087F3C0, &m14B8, 0, 1);
        m14DC->vf70(1);
        mPos = m14DC->m6C;
        mRot = m14DC->m78;
        m58C = 7;
        mStatus.fn_8012D8B8();
        mColli.mFlags |= 1;
        mStatus.m224 = 0;
        fn_8015495C(0, 1, 1, 1);
        mC58.mC0 = 1;
        fn_80161570(2, 0, 0, -1.0f, -1.0f);
        mMotion.mTracks[0].m10 = 1.0f;
        fn_8016E4C4(1);
        mFlag25 = 0;
        fn_80145334();
        fn_80232B7C(0, 0);
        fn_8023A680(lbl_8087F3C0, &m14C4, -1, 0, 0, &mPos, &mRot, 0, 0, -1, 1, 1.0f);
        m14E4++;
        fn_80145334();
        return;
    }
    d.Normalize_unk();
    float speed = m14F0 > 30 ? 1.2f : 0.8f;
    mPos += d * (speed * mStatus.fn_8012DB04());
    mRot = d.ToRotation_unk();
    fn_80144710();
    fn_80145334();
    atn::Vector3 top(mPos);
    top.y += 0.1f;
    mColli.m3C = mPos;
    mColli.m48 = top;
    mColli.m54 = 3.0f;
    m624.mData[0].m04.mCenter = mPos;
    m624.mData[0].m04.mRadius = 3.0f;
}

void Np131LivingArmor::vf3C(HitInfo_unk* hit) {
    HitMsg_unk* msg = (HitMsg_unk*)hit;
    if (m58C == 6) {
        msg->m64 = 3;
        msg->m90 = 0;
        msg->m84 = 2;
        msg->m88 = msg->mData->mC4;
        msg->m68 = 0;
    } else {
        fn_80151448(msg);
        vf40(hit);
    }
}

void Np131LivingArmor::vf40(HitInfo_unk* hit) {
    BattleCharaTask::vf40(hit);
}

// NONMATCHING: 99.9% - the stores of `dir` are scheduled z, x, y in the original (y, x, z here)
void Np131LivingArmor::fn_80547468() {
    float best = 1e16f;
    CharaTask* player = lbl_8087F8A0->mHead;
    GimmickCommon* found = 0;
    for (GimmickCommon* g = GimmickManager::instance_->mHead; g != 0; g = g->m5C) {
        if (g->mId == 0x1391 && g->m54 == 2) {
            atn::Vector3 d = player->mPos - g->m6C;
            if (d.Length_unk() < best) {
                best = d.Length_unk();
                found = g;
            }
        }
    }
    if (found != 0) {
        int usable = 0;
        atn::Vector3 dir(found->m6C.x - player->mPos.x, 0.0f, found->m6C.z - player->mPos.z);
        if (best < 30.0f) {
            atn::Vector3 front(0.0f, 0.0f, 1.0f);
            Mtx m;
            PSMTXRotRad(m, 'y', player->mRot.y);
            PSMTXMultVec(m, &front, &front);
            float dot = PSVECDotProduct(&dir, &front);
            if (dot > (float)cos(0.7853982f)) {
                if (player->m55C == 6) {
                    if (player->m560 == 4) {
                        usable = 1;
                    }
                } else if (player->fn_8016DDB0(0) && m14F8 == 0 && !player->mFlag6 && !player->mFlag31) {
                    usable = 1;
                }
            }
        }
        if (m14FC != 0) {
            GameUI* ui = lbl_8087F490;
            if (ui != 0) {
                ui->m764.m00 = 6;
                ui->m764.m04 = 0x10;
                ui->m764.m08 = -1;
                ui->m764.m0C = 0;
                ui->m764.m10 = 0;
                ui->m764.m14 = 0;
                ui->m764.m18 = 0;
            }
            if (fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
                player->fn_8016AB0C((chr::GmkTarget_unk*)found, this);
                found->vf70(3);
                fn_800C31F4("SE_UICOM_011", 0, -1, 1.0f);
                if (lbl_8087F490 != 0) {
                    fn_803E3384(lbl_8087F490);
                }
            }
        }
        m14F8 = player->mFlag31;
        m14FC = usable;
    }
}

// inferred: called by chr::ControlGmkBreakArmor when the player breaks the armor stand `target`
// (declared there with C linkage)
extern "C" void fn_80547708(void* arg, chr::GmkTarget_unk* target) {
    Np131LivingArmor* armor = (Np131LivingArmor*)arg;
    if (target != 0) {
        GimmickCommon* stand = (GimmickCommon*)target;
        EffectColor_unk color;
        color.r = 1.0f;
        color.g = 1.0f;
        color.b = 1.0f;
        color.a = 1.0f;
        fn_80232B7C(0, 0);
        fn_8023A8B4(lbl_8087F3C0, &armor->m14D0, 0, 0, stand->m6C, stand->m78, color, -1, -1, 1, 1.0f);
        fn_800C31F4("SE_NP132_002", 0, -1, 1.0f);
    }
}
