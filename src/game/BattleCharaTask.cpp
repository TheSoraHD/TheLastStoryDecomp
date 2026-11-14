#include "Event/Manager.h"
#include "game/PopupTask.h"
#include "atn/EffectSystem_unk.h"
#include "game/GameCamera.h"
#include "game/AiTarget.h"
#include "game/GameTask.h"
#include "game/BattleCharaTask.h"
#include "game/GameVoice.h"
#include "game/GameScene.h"
#include "game/GameUI.h"
#include "game/EnemyManager.h"
#include "game/EnemyTask.h"
#include "game/HomingBullet.h"
#include "game/GameConfig_unk.h"
#include "atn/DrawHelper.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/DamageCalc_unk.h"
#include "chr/ControlBase.h"
#include "chr/ControlAttractHateCharge.h"
#include "game/Arms.h"
#include "game/EquipItemData.h"
#include "game/BattleResult.h"
#include "game/SetConfig.h"
#include "game/CrossbowMenuTask.h"
#include "game/Inventory.h"
#include "std/string.h"
#include "std/string_members.h"
#include "atn/Csv.h"
#include <boost/tokenizer.hpp>
#include "atn/SoundRef.h"
#include "chr/ControlCharge.h"
#include "chr/ControlAttackBase.h"
#include "game/TacView.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/SkillEffectManager.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include "atn/RenderManager.h"
#include "game/MapTask.h"
#include "chr/CharaEquip_unk.h"
#include "game/BattleTask.h"
#include "gmk/Item.h"
#include "math/VecAssign_unk.h"

extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: field (lbl_8087F430); only its address is used here
// inferred: camera of the field (lbl_8087F430 + 0x6C, as in game/FinishCamera_unk.cpp)
struct CameraSub_unk {
    unsigned char m000[0x08];
    atn::Vector3 m08; // inferred: eye (field + 0x74)
    atn::Vector3 m14; // inferred: target point (field + 0x80)
    unsigned char m020[0x1FC - 0x20];
    atn::Vector3 m1FC; // inferred: eye of the aim (field + 0x268)
    atn::Vector3 m208; // inferred: target of the aim (field + 0x274)
    unsigned char m214[0x868 - 0x280];
};
// inferred: battle area of the field (lbl_8087F430->m10D8); names invented
struct BattleAreaInfo_unk {
    unsigned char m00[0x48];
    int m48;
};
struct BattleArea_unk {
    unsigned char m00[0x64];
    BattleAreaInfo_unk* m64;
};
struct Field_unk {
    unsigned char m0000[0x6C];
    CameraSub_unk m006C; // inferred: also the object of fn_8037EF30 (as in game/Em306GurgBeast.cpp)
    int m868; // inferred: mode (3 and 4: battle, as in game/CharaTask.cpp)
    unsigned char m86C[0x94C - 0x86C];
    int m94C;
    unsigned char m950[0x10D0 - 0x950];
    int m10D0; // inferred: story progress (as in game/GameBgm.cpp)
    int m10D4;
    BattleArea_unk* m10D8;
    unsigned char m10DC[0x54E4 - 0x10DC];
    int m54E4; // inferred: nonzero stops the combined attack
    unsigned char m54E8[0x5674 - 0x54E8];
    int m5674; // inferred: nonzero allows the specials (fn_800E62AC)
    unsigned char m5678[0x5744 - 0x5678];
    unsigned int m5744; // inferred: frame counter of the field
};
extern "C" Field_unk* lbl_8087F430;
extern "C" void fn_804776F4(atn::LH_MotionSet* set, atn::Model* model); // inferred: applies the motions
// inferred: effect manager (lbl_8087F048, as in game/CharaTask.cpp)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
extern "C" int fn_800F8548(Effect_unk* mgr); // inferred: new bullet id
extern "C" void fn_800FAB80(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* bullet, int id, const atn::Vector3* pos,
                            const atn::Vector3* rot, int arg6, int arg7, int arg8, int arg9, float arg10,
                            float arg11); // inferred: fires a bullet (as in gmk/Coffin.cpp)
extern "C" void fn_800F8574(Effect_unk* mgr, CharaTask* owner, HitInfo_unk* data, const atn::Vector3* pos,
                            const atn::Vector3* dir, HomingParam_unk* param, int arg6, int arg7, float a,
                            float b); // inferred: fires a bullet (as in game/Em003LesserDaemon.cpp)
// inferred: colour as four floats (as in game/CharaTask.cpp)
typedef EffectColor_unk Color4f_unk;
extern "C" void* fn_80210220(int id); // inferred: record of the parameters (result unused here)
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
// inferred: state checks of CharaTask written with a result flag (the original materializes each bool);
// names invented
static inline bool IsActiveB_unk(CharaTask* c) {
    bool r = false;
    if ((c->mFlags & 4) != 4 && (c->mFlags & 1) != 1) {
        r = true;
    }
    return r;
}
static inline bool IsFreeB_unk(CharaTask* c) {
    bool r = false;
    if (IsActiveB_unk(c) && (c->mStatus.m0C & 0x20) != 0x20) {
        r = true;
    }
    return r;
}
static inline bool IsGrappledB_unk(CharaTask* c) {
    bool r = false;
    if (c->m55C == 6 && c->m560 == 28) {
        r = true;
    }
    return r;
}
static inline bool CanActB_unk(CharaTask* c) {
    bool r = false;
    if (IsFreeB_unk(c) && !IsGrappledB_unk(c)) {
        r = true;
    }
    return r;
}
// inferred: front direction of the character (as in game/Em206EggMonster.cpp; name invented)
static inline atn::Vector3 FrontDir_unk(const atn::Vector3& rot) {
    atn::Vector3 v(0.0f, 0.0f, 1.0f);
    atn::Matrix3x4 m;
    m.fn_8013A13C(rot.y);
    PSMTXMultVec(m.m, &v, &v);
    return v;
}
extern "C" void fn_8010220C(Effect_unk* mgr, int arg);
// inferred: target in front of the character within radius/theta
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: target searches of the effect manager (in front / around)
extern "C" int fn_801004FC(Effect_unk* mgr, CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range,
                           float theta);
extern "C" double cos(double x);
extern "C" double sin(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" int fn_80041A28(int arg, CharaTask* chara, CharaTask* target); // inferred: kind of attack
// inferred: target found by fn_800E7214 with its position (only the character is filled here)
struct BattleTarget_unk {
    BattleTarget_unk() : mChara(0), m04(0), m08(0), mPos(0.0f, 0.0f, 0.0f) {}
    CharaTask* mChara;
    int m04;
    int m08;
    atn::Vector3 mPos;
};
extern "C" HitInfo_unk* fn_80219344(CharaTask* chara, int arg);
extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager
// inferred: experience of a kill (as in gmk/FlyObjManager.cpp)
extern "C" void fn_804491F4(int* a, int* b, int level, int exp, int arg4, float rate);
extern "C" void fn_80449448(int* a, int* b, int level, int cap);
// inferred: popup texts (lbl_8087F8A8, as in gmk/FlyObjManager.cpp)
extern "C" int swprintf(wchar_t* s, unsigned long n, const wchar_t* format, ...);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound
extern "C" float fn_800DC288(const char* s); // inferred: string to float
extern "C" int atoi(const char* s);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" bool* fn_802185F4(); // inferred: motions in use
extern "C" void fn_80477694(atn::LH_MotionSet* set, const char* path);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
// inferred: multiplayer session (lbl_8087F610)
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" int fn_804EB1AC(MpSession_unk* session);
extern "C" int fn_8021446C(int index, int arg); // inferred: entry of the level equipment table

// inferred: sound effects of the battle (names invented; 0 lock-on, 2 death, 4 crossbow selection)
static const char sSe009_unk[] = "SE_BTCOM_009";
static const char sSe023_unk[] = "SE_BTCOM_023";
static const char sSe078_unk[] = "SE_BTCOM_078";
static const char sSe101_unk[] = "SE_BTCOM_101";
static const char* const sBattleSe_unk[] = {sSe009_unk, sSe009_unk, sSe023_unk, sSe078_unk, sSe101_unk, 0};

BattleCharaTask::BattleCharaTask(atn::Task* parent, int type, void* data)
    : CharaTask(parent, type, data), m1424(0), m1428(0), m142C(0), m1430(0), m1434(0), m1438(0), m143C(0),
      m1440(0), m1450(0), m1454(1), m1458(1.0f), m145C(0), m1460(0), m1464(0), m146C(0), m1470(0), m1472(0),
      m1474(0), m1478(150), m147C(0.0f, 0.0f, 0.0f), m1488(0), m149C(0.0f), m14A0(0), m14A4(0) {
    mFlagC31 = 0;
    mFlagC30 = 0;
    mFlagC29 = 0;
    mFlagC27 = 0;
    mFlagC26 = 1;
    mFlagC25 = 0;
    mFlagC24 = 0;
    if (m5C != 0) {
        for (int i = 0; i < 4; i++) {
            fn_8014F2B4(m5C->mD4[i], m5C->mD8[i], i);
        }
        // inferred: data is the character id
        if ((unsigned int)data == 0x18705 && lbl_8087F430 != 0 && lbl_8087F430->m10D0 == 0x62E08) {
            mFlagB17 = 1;
            fn_8014F2B4(1, fn_8021446C(4, 1), -1);
        }
    }
    m148C = mColli.m3C;
    m1498 = mColli.m54;
    if (lbl_8087F610 != 0) {
        int free = 1;
        for (unsigned int i = 0; i < mParts.mCount; i++) {
            if (mParts.mData[i]->m04 == 1) {
                free = 0;
            }
        }
        if (free) {
            if (m50 == 0xAAE77) {
                fn_8014F2B4(1, 0x76, -1);
            } else {
                fn_8014F2B4(1, 0x245, -1);
            }
            mFlagB17 = 1;
        }
    }
    if (m50 == 0xAAE71 || m50 == 0xAAE72) {
        mFlagB5 = 1;
    }
}

BattleCharaTask::~BattleCharaTask() {
    if (m1438 != 0 && GimmickManager::instance_ != 0) {
        fn_800D2338(m1438);
    }
}


int BattleCharaTask::vf0C() {
    int ok = 1;
    if (fn_8013655C() != 0) {
        ok = 0;
    }
    if (m1438 != 0 && (m1438->mFlags & 2) != 2) {
        ok = 0;
    }
    if (ok) {
        int flag = 0;
        if (mType == 2 && m146C == 0) {
            flag = 1;
        }
        mFlagC24 = flag;
        if (m1438 != 0) {
            m1438->fn_800D246C(0);
            m1438->mFlags |= 4;
            // inferred: m1438 is a gimmick (GimmickCommon::vf68)
            if ((m137C & 0x400) == 0x400 && lbl_8087F610 == 0) {
                ((GimmickCommon*)m1438)->vf68(0x2724, 1);
            } else if (m50 == 0x30ED4 && lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_80370174(0x11B) > 0) {
                ((GimmickCommon*)m1438)->vf68(0x2747, 1);
            }
        }
        if (m50 == 0x1895D || m50 == 0x189C1 || m50 == 0x31289) {
            mStatus.m2D0[3] = CharaStatusPair_unk(0x1785);
            mStatus.m2FC = 2.0f;
        }
        if (fn_80179AB4() == 1 && fn_80179A34() / 10000 == 305) {
            m54C |= 4;
            for (unsigned int i = 0; i < m624.mSize; i++) {
                m624[i].m00 |= 8;
            }
        }
    }
    return ok;
}

// inferred: ray result (0x50 bytes, as CollResult_unk in game/CharaTask.cpp)
struct RayResult_unk {
    RayResult_unk() : m34(0), m38(0), m3C(0), m40(0) {}
    unsigned char m00[0x04];
    atn::Vector3 m04; // inferred: hit position
    atn::Vector3 m10; // inferred: position of the sweep
    unsigned char m1C[0x34 - 0x1C];
    int m34;
    RayObj_unk* m38;
    int m3C;
    int m40;
    unsigned char m44[0x50 - 0x44];
};
extern "C" int fn_8004ED34(void* colli, RayResult_unk* result, const atn::Vector3* from, const atn::Vector3* to,
                           unsigned int mask, void* ignore, int arg7); // inferred: ray test
extern "C" void* lbl_8087EE98;
atn::Vector3 fn_8017C9AC(CharaTask* chara); // inferred: position of the character's eyes

// inferred: record of the parameters of fn_80210220 (as Record80210220_unk in chr/ControlAttractHateCharge.cpp)
struct ParamRecord_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
};
// inferred: skill effect manager (lbl_8087F9E8, as in gmk/HealPoint.cpp)
extern "C" SkillEffect* fn_8059B66C(void* mgr, HitInfo_unk* skill, CharaTask* user, CharaTask* target,
                                    const atn::Vector3* pos, const atn::Vector3* rot,
                                    boost::function<void(SkillEffect*)> callback, float rate);

// NONMATCHING: 99.2% - `mType == 3 || mType == 2` becomes a range compare; stack slots 4 bytes apart
// inferred: per-frame update of the player characters: runs the pending state m1470 (skills, combined
// attack, charge...) when the timer m1474 ends, then the weapons, the movement and the falls of the guests
void BattleCharaTask::fn_800E2FE0() {
    m14A0--;
    m14A4 = m14A4 - 1 < 0 ? 0 : m14A4 - 1;
    if (m1470 != 0 && --m1474 < 0) {
        if ((mType == 0 && m55C == 1) ||
            ((mType == 3 || mType == 2) && (m55C == 2 || (m55C == 7 && mC58.m11C.fn_8011D1FC() && m1470 == 12)))) {
            switch (m1470) {
            case 1:
                fn_80155A88(m147C, m1472 == 8);
                m1470 = 0;
                if (m1472 > 0) {
                    PlayerState_unk state(m1472);
                    state.m0C = m147C;
                    state.m18 = m1488;
                    SetState_unk(state);
                }
                break;
            case 2:
                fn_80161570(0x153, 1, 0x78, -1.0f, -1.0f);
                m1470 = 0;
                break;
            case 4:
                m1470 = 0;
                break;
            case 5:
                if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 0) {
                    GameConfig_unk* config = lbl_8087F0A8;
                    ParamRecord_unk* rec = (ParamRecord_unk*)fn_80210220(config->mParamIndex);
                    CharaShade_unk shade;
                    shade.m04 = rec->m48;
                    shade.m08 = rec->m4C;
                    shade.m0C = config->mHateAttackedDelayFrame;
                    fn_80164B00(shade);
                    m1428 = this;
                    mFlagC27 = 1;
                    m1430 = config->mCoolDownFrame;
                    if (lbl_8087F048 != 0) {
                        ((BattleTask*)lbl_8087F048)->fn_8010AB94();
                    }
                    ((BattleTask*)lbl_8087F048)->fn_80105DCC(this, &mMotion, 1);
                    m1470 = 0;
                }
                break;
            case 6: {
                HitInfo_unk* skill = fn_80219344(this, 1);
                m63C = m638;
                m638 = skill;
                fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlChargeAttack(this, m147C, 1));
                m1470 = 0;
                break;
            }
            case 7: {
                if (lbl_8087F9E8 != 0) {
                    HitInfo_unk* skill = fn_80219E6C(m1488);
                    fn_8059B66C(lbl_8087F9E8, skill, this, this, &lbl_807C7030, &mRot,
                                boost::function<void(SkillEffect*)>(), 1.0f);
                }
                m1470 = 0;
                break;
            }
            case 8: {
                if (lbl_8087F9E8 != 0) {
                    HitInfo_unk* skill = fn_80219E6C(m1488);
                    fn_8059B66C(lbl_8087F9E8, skill, this, this, &lbl_807C7030, &mRot,
                                boost::bind(&TacView::fn_805AA4C8, lbl_8087F9F8, _1), 1.0f);
                    if (lbl_8087F498 != 0) {
                        lbl_8087F498->fn_803EA77C(this, 0x20, 0, 1.0f, 10.0f);
                    }
                }
                mFlagB16 = 1;
                m138C = lbl_8087F430 != 0 ? lbl_8087F430->m5744 : 0;
                m1470 = 0;
                break;
            }
            case 9:
                if (m55C == 2 && !mFlag31) {
                    fn_80170A20(lbl_8087F8A0->mHead, 30.0f, 0);
                    m1470 = 0;
                }
                break;
            case 10:
                if (((GameTask*)lbl_8087F430)->fn_8036DAA8(this)) {
                    HitInfo_unk* skill = fn_80219344(this, 5);
                    m63C = m638;
                    m638 = skill;
                    fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlQuarkSeirenAttack(this, m147C, 150.0f, 1));
                    m1470 = 0;
                }
                break;
            case 11:
                if (((GameTask*)lbl_8087F430)->fn_8036DAA8(this)) {
                    HitInfo_unk* skill = fn_80219344(this, 5);
                    m63C = m638;
                    m638 = skill;
                    mF6C = m147C;
                    mF7C = this;
                    mPos.y += 18.0f;
                    atn::Vector3 d = mF6C - mPos;
                    if (d.x * d.x + d.z * d.z > 0.01f) {
                        mRot.y = (mF6C - mPos).ToRotationZ_unk().y;
                    }
                    fn_801603CC(1, 1.0f);
                    m1470 = 0;
                }
                break;
            case 12:
                if (mC58.m11C.fn_8011D1FC()) {
                    if (!mC58.m11C.fn_8011D21C() || m1488 == 1) {
                        mC58.m11C.fn_8011CD84(this, m1488);
                    }
                }
                m1470 = 0;
                break;
            }
        } else if ((unsigned int)(m1470 - 3) > 2 && m1470 != 12) {
            if (--m1478 < 0) {
                m1470 = 0;
            }
        }
    }
    fn_8014C540();
    if (lbl_8087F540 == 0 || lbl_8087F540->fn_80481668() == 0) {
        fn_800EE5AC((Arms*)m648);
        if (m64C != 0) {
            fn_800EE5AC((Arms*)m64C);
        }
    }
    if (lbl_8087F610 != 0 && m58C != 2) {
        m1450 = 0;
    }
    if (mType == 0 && (mStatus.m0C & 0x100) != 0x100) {
        if ((mStatus.m0C & 0x20) == 0x20 && m58C != 2) {
            vf44();
        }
        if (m648 != 0) {
            fn_800E589C();
        } else if (m58C == 2) {
            vfEC();
        } else {
            fn_80139560();
        }
        atn::Vector3 from;
        atn::Vector3 to;
        fn_800E932C(&from, &to, 0);
        mFA4 = to;
    } else {
        if ((mStatus.m0C & 0x20) == 0x20 && m58C != 2) {
            vf44();
        }
        if (mFlag2) {
            if (m58C == 2) {
                vfF0();
            }
        } else if (m648 != 0) {
            fn_800E9ED8();
        } else if (m58C == 2) {
            vfF0();
        } else {
            fn_80139560();
        }
    }
    if (mType != 2) {
        if ((mFlags & 1) != 1) {
            fn_80145334();
        }
    } else if ((mFlags & 1) != 1 && !mFlag25) {
        fn_80145334();
    } else {
        atn::Vector3 d = atn::RenderManager::instance_->mCamera.mPos - mPos;
        fn_80148B38(PSVECSquareMag(&d));
    }
    if (mType == 2 && CanAct_unk() && (m54C & 0x2000) != 0x2000 && mC58.mC0 != 0) {
        CharaTask* player = lbl_8087F8A0 != 0 ? lbl_8087F8A0->mHead : 0;
        if (player != 0) {
            atn::Vector3 d = mPos - player->mPos;
            if (atn::Abs_unk(d.y) >= 400.0f) {
                vf44();
                atn::Vector3 front = FrontDir_unk(mRot);
                fn_8015E7A0(atn::Vector3(-front.x, -front.y, -front.z), -1, 0);
            }
        }
    }
}

// inferred: looks for the character under the camera ray
// NONMATCHING: 99.96% - stack slots of the two vector temporaries swapped
// of the start crossed (~12 forms)
void BattleCharaTask::fn_800E3F5C() {
    Field_unk* field = lbl_8087F430;
    if (field == 0 || lbl_8087F048 == 0) {
        return;
    }
    RayObj_unk* obj = 0;
    int enemy = 0;
    int b = 0;
    BattleArea_unk* area = field->m10D8;
    int mode = field->m868;
    if (!(mode != 3 && mode != 4)) {
        atn::RenderManager* camera = atn::RenderManager::instance_;
        atn::Vector3 from = fn_8017C9AC(this);
        atn::Vector3 dir;
        dir = camera->mCamera.mTarget - camera->mCamera.mPos;
        dir.Normalize_unk();
        float range = 800.0f > mStatus.mParamB.m28 ? 800.0f : mStatus.mParamB.m28;
        atn::Vector3 start;
        atn::VecAssign_unk(start, from);
        atn::Vector3 to = from + dir * range;
        RayResult_unk result;
        unsigned int mask = 4;
        if (area != 0 && area->m64 != 0 && area->m64->m48 == 1) {
            mask = 0x10;
        }
        int found = 0;
        if (fn_8004ED34(lbl_8087EE98, &result, &start, &to, mask, &mColli, 0) != 0 && result.m38 != 0 &&
            (result.m38->m08 & 2) == 2) {
            CharaTask* chara = result.m38->m0C;
            enemy = 0;
            if (!chara->fn_8016F3D0(this, 1) && !chara->IsPartyMember_unk()) {
                enemy = 1;
            }
            if ((chara->m54C & 0x400000) == 0x400000) {
                enemy = !enemy;
            }
            found = 1;
            if (area != 0 && ((GameScene*)area)->fn_803CCA4C(chara) && !((GameScene*)area)->fn_803CCA64(chara, 0, 80.0f)) {
                found = 0;
                enemy = 0;
            }
        }
        if (!found && area != 0) {
            b = ((GameScene*)area)->fn_803CCA34(this);
        }
        obj = result.m38;
    }
    ((BattleTask*)lbl_8087F048)->fn_801021B4(obj, enemy, b);
}

void BattleCharaTask::vf14() {
    fn_80149A30();
    if (lbl_8087F0A8->mDrawCharaColli != 0) {
        lbl_8087EEB0->fn_80063484(20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

void BattleCharaTask::fn_800E426C() {
    fn_8010220C(lbl_8087F048, 0);
}

// inferred: translation of a node matrix (name invented)
inline atn::Vector3 NodePos_unk(MtxPtr m) {
    return atn::Vector3(m[0][3], m[1][3], m[2][3]);
}

extern "C" float fn_800A56A8(void* pad, int port, int axis, int arg); // inferred: stick axis (as in game/GameTask.cpp)
extern "C" void* lbl_8087EF70; // inferred: pad of the stick
// inferred: target in front of the character for the cover actions (game/BattleTask.cpp, BattleAim_unk)
extern "C" int fn_8010089C(Effect_unk* mgr, BattleTarget_unk* out, CharaTask* chara, const atn::Vector3* rot,
                           float range, float angle);
extern "C" void fn_801092C8(Effect_unk* mgr, int kind, CharaTask* chara, int item, int num); // inferred: battle message
extern "C" int fn_80211734(HitInfo_unk* item);                 // inferred: infinite item
extern "C" void fn_8044441C(Inventory* inv, int id, int num);  // inferred: removes items
extern "C" int fn_804444E8(Inventory* inv, int id);            // inferred: number of items
extern "C" HitInfo_unk* fn_80211480(int code);                 // inferred: entry of an item code (db/ItemParam_unk.h)
extern "C" void fn_80108C10(Effect_unk* mgr, DamageResult_unk* damage, const atn::Vector3& pos, CharaTask* attacker,
                            CharaTask* chara, unsigned int flags, int skipEmpty); // inferred: shows the damage
extern "C" EnemyTask* fn_8036DDD4(GameTask* task); // inferred: enemy in reach of the leader (game/GameTask.cpp)

// inferred: button of the cover attack (2 with the attack type A); name invented
// inferred: drops the lock-on target when it can no longer be targeted (name invented)
static inline void ReleaseTarget_unk(CharaTask* self) {
    CharaTask* target = self->mFC4;
    if (target != 0) {
        if (!CanActB_unk(target) || self->mFlag31 || self->mFlag6 || (target->m54C & 0x2000) == 0x2000) {
            self->fn_8016DC14(0, 0);
        } else if (!self->mFlag5 && lbl_8087F0A8->mInput.fn_801240B4(0x11)) {
            self->fn_8016DC14(0, 0);
        } else if (self->m55C == 6 && self->m560 == 0x1D) {
            self->fn_8016DC14(0, 0);
        }
    }
}
// inferred: the camera kept its sub mode (name invented)
static inline bool IsSameMode_unk(const GameCamera* camera) {
    return camera->m7FC == camera->m800;
}
// inferred: dot product of two vectors passed by reference (name invented)
static inline float Dot_unk(const atn::Vector3& a, const atn::Vector3& b) {
    return PSVECDotProduct(&a, &b);
}
// inferred: a x b and -v returned by value (names invented)
static inline atn::Vector3 Cross_unk(const atn::Vector3& a, const atn::Vector3& b) {
    atn::Vector3 r;
    PSVECCrossProduct(&a, &b, &r);
    return r;
}
static inline atn::Vector3 Neg_unk(const atn::Vector3& v) {
    return atn::Vector3(-v.x, -v.y, -v.z);
}
static inline bool IsAttackTypeA_unk() {
    return SetConfig::sInstance->mAttackType == 0;
}
static inline int AttackButton_unk() {
    bool typeA = IsAttackTypeA_unk();
    int button = 4;
    if (typeA) {
        button = 2;
    }
    return button;
}

// NONMATCHING: 99.5% - r0/r3 swapped in the target checks, the lock-on input reloads lbl_8087F0A8 (here config)
// and the flag word reload before mFlag5 = 0
// inferred: input of the leader (called by GameTask::fn_8036CC90): lock-on, cover actions, gimmicks and items
// in reach, the button hint of the HUD (GameUI::m764) and the input of the weapon (fn_800E62AC/fn_800E7650)
void BattleCharaTask::fn_800E4278(GameTask* task) {
    int lockOn = 1;
    if (m55C == 1) {
        lockOn = 0;
    }
    if (m55C == 6 && ((unsigned int)(m560 - 0x3C) <= 1 || m560 == 0x62)) {
        lockOn = 0;
    }
    if (m55C == 5 && mFlag6 && lbl_8087F0A8->mInput.fn_801240B4(0x15)) {
        lockOn = 0;
    }
    lbl_8087F0A8->mInput.fn_80122330(lockOn);
    GameUI* ui = lbl_8087F490;
    GameUIElem1C_unk action;
    action.m00 = ui->m764.m00;
    GameCamera* camera = &task->mCamera;
    GameConfig_unk* config = lbl_8087F0A8;
    int a = ui->m748.m00;
    int b = ui->m748.m14;
    int done = 0;
    ReleaseTarget_unk(this);
    if (m55C == 1 && !mFlagB9) {
        GameScene* scene;
        if (mFlag6) {
            if (IsSameMode_unk(camera)) {
                atn::Vector3 rot = mRot;
                rot.y = 3.1415927f + camera->mRot.y;
                mRot = rot;
            }
            if (mFlag3) {
                fn_80155DAC(0);
            }
            if (m648 != 0 && m648->m04 == 1) {
                action.m00 = 0xB;
            }
        } else if (m1208 == 0 && !mFlag3 && mF54 == 0) {
            if (mFlag31) {
                if (mC48 != 0) {
                    if (!lbl_8087F0A8->mInput.fn_801240B4(0x15)) {
                        fn_80154344();
                    }
                    if (m648 != 0 && m648->m04 == 1) {
                        action.m00 = 0xB;
                    }
                } else {
                    m143C--;
                    if (!mFlag5) {
                        int shown = 0;
                        int side = fn_80153B44();
                        atn::Vector3 stick;
                        stick.x = -fn_800A56A8(lbl_8087EF70, 0, 0, 0);
                        stick.y = 0.0f;
                        stick.z = -fn_800A56A8(lbl_8087EF70, 0, 1, 0);
                        float len = PSVECMag(&stick);
                        atn::Vector3 dir = fn_80178018();
                        float angle = atn::WrapAngle_unk(dir.y - mRot.y);
                        if (len > 0.4f) {
                            if (side != 0 && (side != 3 || mC40 == 1) && fn_80179FA8() && m139C == 0) {
                                BattleTarget_unk target;
                                int found = fn_8010089C(lbl_8087F048, &target, this, &dir, 80.0f, 1.5707964f);
                                if (found) {
                                    action.m00 = 10;
                                    shown = 1;
                                    action.m0C = target.mChara;
                                }
                                if (found && a == 10 && lbl_8087F0A8->mInput.fn_801231D0(AttackButton_unk())) {
                                    int mode = 0;
                                    if (side == 3) {
                                        mode = 2;
                                    }
                                    atn::Vector3 to = target.mPos - mPos;
                                    to.y = 0.0f;
                                    if (PSVECSquareMag(&to) > 0.001f) {
                                        PSVECNormalize(&to, &to);
                                    } else {
                                        atn::VecAssign_unk(to, FrontDir_unk(mRot));
                                    }
                                    if (side == 3) {
                                        if (Dot_unk(to, FrontDir_unk(mRot)) < 0.0f) {
                                            mode = 0;
                                        }
                                    } else if (side == 2) {
                                        if (Dot_unk(to, FrontDir_unk(mRot)) > 0.0f) {
                                            mPos += mC14 * 5.0f;
                                        }
                                    } else if (side == 1) {
                                        if (Dot_unk(to, FrontDir_unk(mRot)) > 0.0f) {
                                            mPos += mC14 * -5.0f;
                                        }
                                    }
                                    fn_801539E0();
                                    fn_800E8688(0xCB, 0, target.mChara, &target.mPos, mode, 0);
                                    done = 1;
                                }
                            }
                            int kind = -1;
                            atn::Vector3 pos;
                            int link;
                            if ((side == 2 && angle > 0.7853982f && angle < 2.3561945f) ||
                                (side == 1 && angle < -0.7853982f && angle > -2.3561945f)) {
                                if (task->fn_8036DB3C(&pos, &link, this)) {
                                    kind = 3;
                                } else if (m139C == 0) {
                                    kind = 2;
                                }
                            }
                            if (kind == -1 && m139C == 0 && (unsigned int)(side - 1) <= 1 &&
                                atn::Abs_unk(angle) < 0.7853982f) {
                                kind = 5;
                            }
                            if (kind == -1 && m139C == 0 && side == 3 && atn::Abs_unk(angle) < 0.7853982f &&
                                mC40 == 1) {
                                kind = 4;
                            }
                            if (kind != -1) {
                                if (!shown) {
                                    action.m00 = kind;
                                    if (kind == 5) {
                                        action.m00 = 2;
                                    }
                                    shown = 1;
                                }
                                if (!done && lbl_8087F0A8->mInput.fn_801231D0(2)) {
                                    if (kind == 4) {
                                        vf74(Cross_unk(mC14, atn::Vector3(0.0f, 1.0f, 0.0f)));
                                    } else if (kind == 5) {
                                        fn_801551AC();
                                    } else if (kind == 2) {
                                        fn_80155790(fn_80153B44() == 1 ? Neg_unk(mC14) : mC14);
                                    } else if (kind == 3) {
                                        fn_80154EC4(pos, link);
                                        m143C = 8;
                                    }
                                    done = 1;
                                }
                            }
                        }
                        if (!shown) {
                            action.m00 = 1;
                            if (lbl_8087F0A8->mInput.fn_801231D0(0x13)) {
                                fn_801539E0();
                                m14A0 = 2;
                                done = 1;
                            }
                        }
                    }
                    if (mFlag31 && m139C == 0 && lbl_8087F0A8->mInput.fn_801240B4(0x15) && m143C <= 0) {
                        fn_80153FA0();
                    }
                    if (!mFlag6 && !HasC48_unk() && m139C == 0 && fn_80153B44() != 0 &&
                        !lbl_8087F0A8->mInput.fn_80124B60() && ((GameTask*)lbl_8087F430)->fn_80370174(0xD0) == 0) {
                        GameUI* hud = lbl_8087F490;
                        GameUIElem1C_unk hint;
                        hint.m00 = 0xD;
                        hud->m7C0 = hint;
                    }
                }
            } else {
                int shown = 0;
                if (action.m00 == -1 && m139C == 0) {
                    GimmickCommon* gimmick = task->fn_8036E098();
                    if (gimmick != 0) {
                        int press = 0;
                        int type = gimmick->m50;
                        if (type == 0x10) {
                            shown = 1;
                            press = 0;
                            action.m00 = gimmick->mE8;
                            action.m04 = gimmick->mEC;
                            if (lbl_8087F0A8->mInput.fn_801231D0(2) || lbl_8087F0A8->mInput.fn_801231D0(4)) {
                                press = 1;
                            }
                        } else if (type == 0x3F || type == 0x3B || type == 0x3C || type == 0x27) {
                            shown = 1;
                            press = 0;
                            action.m00 = gimmick->mE8;
                            action.m04 = gimmick->mEC;
                            if (lbl_8087F0A8->mInput.fn_801231D0(2) || lbl_8087F0A8->mInput.fn_801231D0(0)) {
                                press = 1;
                            }
                        } else if (type == 8) {
                            int ok = !mFlag5;
                            if (config->mNewDiveMode == 2 && m648 != 0 && lbl_8087F0A8->mInput.fn_801248DC(0) > 0.4f) {
                                ok = 0;
                            }
                            if (ok) {
                                ((gmk::Item*)gimmick)->fn_803FC870();
                                shown = 1;
                                press = lbl_8087F0A8->mInput.fn_801231D0(2);
                            }
                        } else {
                            int ok = !mFlag5;
                            if (config->mNewDiveMode == 2 && m648 != 0 && lbl_8087F0A8->mInput.fn_801248DC(0) > 0.4f) {
                                ok = 0;
                            }
                            if (ok) {
                                shown = 1;
                                action.m00 = gimmick->mE8;
                                action.m04 = gimmick->mEC;
                                press = lbl_8087F0A8->mInput.fn_801231D0(2);
                            }
                        }
                        if (shown && press) {
                            if (gimmick->m50 == 0x10) {
                                fn_800E8688(200, 0, 0, &gimmick->m6C, 0, 0);
                            } else {
                                gimmick->vf4C(this);
                            }
                            done = 1;
                        }
                    }
                }
                if (!shown && b == 0) {
                    if (!mFlag5) {
                        GameSceneRoute_unk route;
                        if (task->fn_8036DAF4(&route, this, 1, 0)) {
                            action.m00 = 0;
                            shown = 1;
                            if (lbl_8087F0A8->mInput.fn_801231D0(0x13)) {
                                fn_80153698(&route, 0);
                                m143C = 8;
                                done = 1;
                            }
                        }
                    }
                    if (m14A4 <= 0 && (action.m00 == -1 || lbl_8087F0A8->mTouchHelp != 0) && m139C == 0) {
                        EnemyTask* enemy = fn_8036DDD4(task);
                        if (enemy != 0) {
                            FixedArray_unk<HitInfo_unk*, 4> items;
                            if (enemy->mStatus.IsState_unk(0x20) && enemy->mStatus.m224 > 0) {
                                if (task->fn_803761BC() && fn_80219558(m50) == 0 && mFlag28) {
                                    action.m00 = 9;
                                }
                            } else if (enemy->mStatus.IsState_unk(0x40)) {
                                int num = fn_804444E8(Inventory::instance_, 400);
                                if (num > 0) {
                                    HitInfo_unk* item = fn_80211480(400);
                                    items.PushBack_unk(item);
                                    if (!shown) {
                                        action.m08 = num;
                                        action.m10 = (int)item;
                                        action.m00 = 0xC;
                                    }
                                }
                            }
                            if (lbl_8087F0A8->mTouchHelp != 0 || lbl_8087F0A8->mInput.fn_801231D0(2)) {
                                if (action.m00 == 9) {
                                    HitInfo_unk* data = fn_80219E6C(0x2711);
                                    if (!enemy->mStatus.IsState_unk(0x20)) {
                                        enemy->mStatus.fn_8013310C(0x20, 0);
                                    }
                                    DamageResult_unk result;
                                    fn_8003EA3C(&result, data, this, enemy, 0, &lbl_807C6B90, 0, 0);
                                    fn_801092C8(lbl_8087F048, 0x16, enemy, 0, 0);
                                    HitInfo_unk* hit = fn_80219E6C(0x4FBF);
                                    if (hit != 0) {
                                        enemy->fn_80178668(hit, 0, hit->m5C, this, 0);
                                        if ((enemy->mType == 0 || enemy->mType == 3) && lbl_8087F8A8 != 0) {
                                            wchar_t text[2] = {0, 0};
                                            atn::Vector3 pos(0.0f, 2.0f, 0.0f);
                                            if (enemy->GetModel_unk()->GetNodeMtx_unk("hips") != 0) {
                                                pos += NodePos_unk(enemy->GetModel_unk()->GetNodeMtx_unk("hips"));
                                            }
                                            lbl_8087F8A8->fn_8054D794(7, text, &pos, 0, 0, 1, 0, 0, 0, 1, -1, -1);
                                        }
                                    }
                                    done = 1;
                                } else if (items.mCount != 0) {
                                    unsigned int count = items.mCount;
                                    for (unsigned int i = 0; i < count; i++) {
                                        if (!fn_80211734(items.mData[i])) {
                                            fn_8044441C(Inventory::instance_, items.mData[i]->m04, 1);
                                        }
                                        DamageResult_unk result;
                                        fn_8003F1E4(&result, items.mData[i], this, enemy, 0);
                                        if (items.mData[i]->m04 != 400) {
                                            fn_80108C10(lbl_8087F048, &result, enemy->mPos + atn::Vector3(0.0f, 15.0f, 0.0f),
                                                        this, 0, 1, 0);
                                        }
                                    }
                                    m14A4 = 60;
                                    done = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
        GameUI* hud = lbl_8087F490;
        if (hud->m7C0.m00 == 0xD ||
            (hud->m7C0.m00 == -1 && !mFlag6 && !HasC48_unk() && !mFlag31 && !lbl_8087F0A8->mInput.fn_80124B60())) {
            if (lbl_8087F430 != 0 && lbl_8087F490 != 0) {
                scene = ((GameTask*)lbl_8087F430)->m10D8;
                if (scene != 0 && !scene->fn_803CCC4C() && scene->fn_803CC77C() > 0 && !mFlag3) {
                    GameUIElem1C_unk& hint = hud->m7C0;
                    hint.m00 = 0x11;
                    lbl_8087F490->m7C0 = hint;
                }
            }
        }
        if (task->fn_80370174(0x8F) == 1 && task->fn_80370174(0xA9) == 0 && (mFlag6 || HasC48_unk()) &&
            mMotion.mTracks[0].m10 >= 0.99f) {
            ((GameTask*)lbl_8087F430)->fn_80370320(0xA9, 1, 0);
        }
    } else if (m55C == 6) {
        switch (m560) {
        case 0xF:
            action.m00 = 10;
            break;
        case 0x62:
            if (mFlag6) {
                if (IsSameMode_unk(camera)) {
                    atn::Vector3 rot = mRot;
                    rot.y = 3.1415927f + camera->mRot.y;
                    mRot = rot;
                }
            }
            action.m00 = 0xB;
            break;
        case 0x79:
            action.m00 = 0x10;
            break;
        }
    } else if (m55C == 2) {
        done = 1;
    }
    if (!lbl_8087F0A8->mInput.fn_80124B60()) {
        lbl_8087F490->m764 = action;
    }
    if (!lbl_8087F0A8->mInput.fn_80124B60() && (m55C == 1 || m55C == 6)) {
        if (mStatus.IsState_unk(0x80) || mStatus.IsState_unk(0x100) || (mStatus.IsState_unk(0x40) && mF94 == 0) ||
            mStatus.IsState_unk(0x4000) || mStatus.IsState_unk(0x20000)) {
            lbl_8087F490->m728 = 1;
        }
    }
    if (!done) {
        switch (m58C) {
        default:
            if (m648 != 0) {
                if (m648->m04 == 0) {
                    fn_800E62AC((Field_unk*)task, a, b);
                } else if (m648->m04 == 1) {
                    fn_800E7650((Field_unk*)task, a, b);
                }
            } else if (mFlag6) {
                if (!lbl_8087F0A8->mInput.fn_801240B4(0x12)) {
                    fn_80155DAC(0);
                }
            } else if (!mFlag31) {
                mFlag5 = 0;
                if (lbl_8087F0A8->mInput.fn_801231D0(0x11)) {
                    camera->fn_8037EF30(0, 1, -1.0f);
                    mFC0 = 0;
                }
                if (lbl_8087F0A8->mInput.fn_801240B4(0x12)) {
                    if (m55C == 6) {
                        if ((unsigned int)(m560 - 0x3C) <= 1) {
                            fn_8016E970(m564);
                            fn_80155DAC(1);
                        }
                    } else if (!mFlag3) {
                        fn_80155DAC(1);
                    }
                }
            }
            break;
        case 3:
            if (m55C != 6) {
                fn_80166544();
            }
            break;
        case 4:
            if (m55C != 6) {
                fn_80166A54();
            }
            break;
        }
    }
}

// inferred: one hit of fn_800F8C6C (names invented)
struct HitResult_unk {
    CharaTask* mChara;
    unsigned int mFlags; // inferred: 8 = critical
};
extern "C" int fn_800F8C6C(Effect_unk* mgr, HitResult_unk* results, int count, CharaTask* chara, HitInfo_unk* data,
                           int id, int time, int hits, float angle); // inferred: deals a hit, returns the hits

// inferred: frame of the attack motions where the hit starts (name invented)
inline float HitFrame_unk(int motion) {
    float frame;
    switch (motion) {
    case 0x158:
        frame = 15.0f;
        break;
    case 0x159:
        frame = 14.0f;
        break;
    case 0x15A:
        frame = 16.0f;
        break;
    case 0x15B:
        frame = 16.0f;
        break;
    case 0x16A:
        frame = 32.0f;
        break;
    case 0x161:
        frame = 23.0f;
        break;
    default:
        frame = 14.0f;
        break;
    }
    return frame;
}

// NONMATCHING: 97% - `m560 >= 4 && m560 < 12` becomes a range compare; this/config in r29/r30 instead of
// r30/r31
// inferred: update of the player characters with a weapon (states 0-4 of m58C: idle, attack, ...)
void BattleCharaTask::fn_800E589C() {
    GameConfig_unk* config = lbl_8087F0A8;
    int grappled = 0;
    if (m55C == 6 && m560 == 28) {
        grappled = 1;
    }
    switch (m58C) {
    case 0:
        if (m55C == 1 && mMotion.fn_80097D40(200)) {
            m1188.SetMotion(200, false, false, false, 1.0f, 0.0f);
        }
        fn_80139560();
        if (grappled && (m55C != 6 || m560 != 28) && lbl_8087F430 != 0 && lbl_8087F430->m868 == 6) {
            ((GameTask*)lbl_8087F430)->fn_803748E0(1);
            lbl_8087F430->m94C = 1;
        }
        break;
    case 1: {
        atn::Vector3 rot = mRot;
        if (fn_80158BB4()) {
            if (m55C != 6 || (m560 != 13 && m560 != 29)) {
                fn_8016DA4C();
            }
            m1424 = 0;
            mFlagC30 = 0;
            m58C = 0;
            int attack = 0;
            if (m55C == 6) {
                if (m560 >= 4) {
                    if (m560 < 12) {
                        attack = 1;
                    }
                }
            }
            if (attack) {
                fn_8016E970(1);
            }
        } else if ((unsigned int)(m560 - 4) <= 1) {
            float end = 12.0f;
            if ((unsigned int)m50 == 0xAAE76 || (unsigned int)m50 == 0xAAE77) {
                end = mMotion.fn_80097D7C(0) - 5.0f;
            }
            if (m1424 > 0 && mMotion.mTracks[0].m08 >= end) {
                BattleTarget_unk target;
                m1424--;
                float frame = HitFrame_unk(mMotion.mTracks[0].m00);
                if (m560 == 4 && config->mAttackEndWait != 0 && mMotion.mTracks[0].m08 >= frame) {
                    float speed = 4.0f / m638->m68;
                    if (((unsigned int)mStatus.m14 & 0x2000) == 0x2000) {
                        speed *= ((BattleTask*)lbl_8087F048)->fn_8010CA34();
                    }
                    mMotion.mTracks[0].m0C = speed;
                }
                int pressed = fn_800E70EC(1, -1, 0);
                if (mMotion.mTracks[0].m08 > 13.0f && !mFlag31) {
                    int count = m1424;
                    m1424 = 0;
                    if (SetConfig::sInstance->mAttackType == 0 || pressed != 0) {
                        if (fn_800E7214(&target.mChara, 1)) {
                            mFlagC30 = 1;
                            m1444 = target.mPos;
                        } else if (pressed) {
                            mFlagC30 = 1;
                            Effect_unk* mgr = lbl_8087F048;
                            atn::Vector3 dir = fn_80178018();
                            CharaTask* chara = ((BattleTask*)mgr)->fn_800FFE68(this, &dir, 0, 0, config->mBtnAtkCorrectRadius,
                                                           0.5f * config->mBtnAtkCorrectTheta);
                            if (chara != 0) {
                                target.mChara = chara;
                                m1444 = chara->mPos;
                            } else {
                                target.mChara = 0;
                                m1444.x = 0.0f;
                                m1444.y = 0.0f;
                                m1444.z = 10.0f;
                                Mtx m;
                                PSMTXRotRad(m, 'y', fn_80178018().y);
                                PSMTXMultVec(m, &m1444, &m1444);
                                m1444 += mPos;
                            }
                        }
                    }
                    m1424 = count;
                }
                if (mFlagC29 && config->mAttackGuardWait > 0.0f) {
                    if (mMotion.mTracks[0].m00 == 0x162) {
                        frame = mMotion.fn_80097D7C(0) - 5.0f;
                    } else {
                        frame = mMotion.fn_80097D7C(0) - 2.0f;
                    }
                }
                if (SetConfig::sInstance->mAttackType != 0 && lbl_8087F610 != 0) {
                    if (mMotion.mTracks[0].m00 == 0x162) {
                        frame = 18.0f;
                    } else if (mMotion.mTracks[0].m00 == 0x163) {
                        frame = 18.0f;
                    }
                }
                if (mFlagC30 && mMotion.mTracks[0].m08 >= frame &&
                    (pressed || !lbl_8087F0A8->mInput.fn_801240B4(7))) {
                    fn_800E8688(200, 0, target.mChara, &m1444, 0, 0);
                }
            } else {
                m13B4 = 0.0f;
            }
        }
        float frame = HitFrame_unk(mMotion.mTracks[0].m00);
        config = lbl_8087F0A8;
        if (mFlagC29 && config->mAttackGuardWait > 0.0f && mMotion.mTracks[0].m08 >= frame) {
            float rest = mMotion.fn_80097D7C(0) - frame;
            float speed = rest / (rest + config->mAttackGuardWait);
            if (((unsigned int)mStatus.m14 & 0x2000) == 0x2000) {
                speed *= ((BattleTask*)lbl_8087F048)->fn_8010CA34();
            }
            mMotion.mTracks[0].m0C = speed;
        }
        fn_80139560();
        if (lbl_8087F048 != 0 && fn_80158CA4() && m560 != 11) {
            if (mControl && mControl->vf30() > m598) {
                m598 = mControl->vf30();
                m594 = 1;
                m590 = fn_800F8548(lbl_8087F048);
            }
            if (m594 != 0) {
                fn_80144710();
                HitResult_unk results[8];
                int n = fn_800F8C6C(lbl_8087F048, results, 8, this, m638, m590, 30, m594,
                                    mFlag31 ? 3.1415927f : 0.0f);
                if (n > 0) {
                    if (m594 > 0) {
                        m594 -= n;
                    }
                    int critical = 0;
                    for (int i = 0; i < n; i++) {
                        if ((results[i].mFlags & 8) == 8) {
                            critical = 1;
                        }
                    }
                    if (critical && (unsigned int)(m560 - 4) <= 1) {
                        mFlagC29 = 1;
                    }
                }
            }
        }
        break;
    }
    case 2:
        vfEC();
        break;
    case 3:
    case 4:
        fn_80139560();
        break;
    }
}

// inferred: rotates v around the y axis by the angle a
static inline void RotateY_unk(atn::Vector3& v, float a) {
    Mtx m;
    PSMTXRotRad(m, 'y', a);
    PSMTXMultVec(m, &v, &v);
}

void BattleCharaTask::fn_800E610C() {
    atn::Vector3 input(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', fn_80178018().y);
    PSMTXMultVec(m, &input, &input);
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    RotateY_unk(front, mRot.y);
    if (PSVECDotProduct(&front, &input) < (float)cos(0.5235987901687622) || !fn_8016AFE8(front)) {
        int mode = lbl_8087F0A8->mSurroundMode;
        if (!mFlag10 || mode != 1) {
            atn::Vector3 dir(0.0f, 0.0f, 1.0f);
            Mtx m3;
            PSMTXRotRad(m3, 'y', fn_80178018().y);
            PSMTXMultVec(m3, &dir, &dir);
            fn_80155790(dir);
            if (mFlag10 && mode == 2) {
                if (mFlag10) {
                    for (CharaPair_unk* it = m125C.begin(); it != m125C.end(); it++) {
                        it->m04->fn_80155790(dir);
                    }
                }
            }
        }
    }
}

extern "C" int fn_8021921C(CharaTask* chara, int slot); // inferred: magic of the slot (db/PlayerParam_unk.cpp)
extern "C" int fn_8021AF98(int id); // inferred: skill id of a skill growth entry (db/SkillGrowth_unk)

// inferred: control with a timer (ids 10 and 12 of m560; class unknown, names invented)
class ControlTimer_unk : public chr::ControlBase {
public:
    bool IsDone_unk() const { return m08 <= 0.0f; }
    float m08;
    unsigned char m0C[0x1C - 0x0C];
    int m1C; // inferred: set to end the control early
};

// inferred: hit point rate of a character (name invented)
inline float HpRate_unk(CharaTask* c) {
    return c->mStatus.mParamB.m88 > 0 ? c->mStatus.m08 / c->mStatus.mParamB.m88 : 0.0f;
}

// NONMATCHING: 98.7% - the flag word is not reloaded before clearing mFlag5; registers of the magic slot
// inferred: input of the player in the field: cursor of the field at full life, lock-on, crossbow,
// attacks (fn_800E73EC) and the skills of the special button
void BattleCharaTask::fn_800E62AC(Field_unk* field, int a, int b) {
    GameConfig_unk* config = lbl_8087F0A8;
    int attack = 1;
    if (lbl_8087F610 == 0 && ((GameTask*)lbl_8087F430)->fn_80375184() && atn::Abs_unk(HpRate_unk(this) - 1.0f) < 1.1920929e-7f &&
        ((GameTask*)lbl_8087F430)->fn_80370174(0x88) == 0 && lbl_8087F0A8->mInput.fn_801231D0(0x31)) {
        int cursor = 0;
        if (m55C == 1) {
            cursor = 1;
        } else if (m55C == 6 && (m560 == 4 || m560 == 2 || m560 == 0x61)) {
            if (mControl) {
                mControl->vf1C();
                boost::function<void()> next = mControl->vf10();
                mControl.reset();
                if (next) {
                    next();
                }
            }
            cursor = 1;
        }
        if (mFlag3 || m139C != 0 || m1208 != 0) {
            cursor = 0;
        }
        if (cursor) {
            ((GameTask*)lbl_8087F430)->fn_80373FC4();
            return;
        }
    }
    if (m55C != 1) {
        int ending = 0;
        if (m55C == 6) {
            if (m560 == 0x14) {
                ending = 1;
            } else if (m560 == 10) {
                float frame = mMotion.mTracks[0].m08;
                ending = 1;
                if (frame > 9.0f && frame < 12.0f) {
                    ((ControlTimer_unk*)mControl.get())->m1C = 1;
                    if (fn_8016E484()) {
                        fn_800E8688(fn_8021AF98(2), 0, 0, 0, 0, 0);
                    } else {
                        fn_800E8688(0xD0, 0, 0, 0, 0, 0);
                    }
                }
            }
        }
        mFlag5 = ending;
        int release = !lbl_8087F0A8->mInput.fn_801240B4(7);
        if (m55C == 6 && ((unsigned int)(m560 - 0x14) <= 3 || (unsigned int)(m560 - 0x1C) <= 1 || m560 == 0x3B ||
                          m560 == 0xF || m560 == 10)) {
            release = 1;
        }
        if (release) {
            mFC0 = 0;
        }
        if (m55C != 6) {
            return;
        }
        if ((unsigned int)(m560 - 4) > 1 && m560 != 9) {
            return;
        }
        attack = mMotion.mTracks[0].m08 >= config->mBtnAtkChargeStart;
    } else {
        if (m1424 > 0) {
            m1424--;
        }
        CameraSub_unk* camera = &field->m006C;
        if (mFlag27) {
            mFC0 = 0;
            attack = 0;
            mFlag5 = 0;
        } else {
            int cover = 0;
            if (mFlag31 && mC48 != 0) {
                cover = 1;
            }
            if (cover) {
                mFC0 = 0;
                mFlag5 = 0;
                if (fn_800E70EC(0, a, b)) {
                    attack = 0;
                }
            } else if (mFlag6) {
                mFC0 = 0;
                mFlag5 = 0;
                if (!lbl_8087F0A8->mInput.fn_801240B4(0x12)) {
                    fn_80155DAC(0);
                } else if ((unsigned int)(m50 - 0xAAE76) <= 1) {
                    if (lbl_8087F0A8->mInput.fn_801237F0(0x17)) {
                        CrossbowMenuTask::sInstance->fn_801E9ECC(-1, 1);
                        fn_800C31F4(sBattleSe_unk[4], 1.0f, 0, -1);
                    } else if (lbl_8087F0A8->mInput.fn_801237F0(0x18)) {
                        CrossbowMenuTask::sInstance->fn_801E9ECC(1, 1);
                        fn_800C31F4(sBattleSe_unk[4], 1.0f, 0, -1);
                    } else if (lbl_8087F0A8->mInput.fn_801237F0(0x1A)) {
                        CrossbowMenuTask::sInstance->fn_801E97DC();
                        fn_800C31F4(sBattleSe_unk[4], 1.0f, 0, -1);
                    } else if (lbl_8087F0A8->mInput.fn_801237F0(0x1B)) {
                    }
                    if (lbl_8087F0A8->mInput.fn_801231D0(6) && m640 <= 0 && mFB0 <= 0) {
                        mFlagB8 = 0;
                        fn_800E8FB8();
                    }
                }
            } else if (m1208 != 0 || mFlag3 || mF54 != 0 || m139C != 0) {
                mFC0 = 0;
                attack = 0;
                mFlag5 = 0;
                if (lbl_8087F0A8->mInput.fn_801231D0(0x11)) {
                    ((GameCamera*)camera)->fn_8037EF30(0, 1, -1.0f);
                }
            } else if (mFlag5) {
                if (lbl_8087F0A8->mInput.fn_801231D0(0x16)) {
                    fn_800E610C();
                }
                if (mFlag31) {
                    mFC0 = 0;
                } else if (lbl_8087F0A8->mInput.fn_801231D0(0x1E)) {
                    if (mFC4 != 0) {
                        Effect_unk* mgr = lbl_8087F048;
                        atn::Vector3 dir = fn_80178018();
                        fn_8016DC14(((BattleTask*)mgr)->fn_800FFE68(this, &dir, 0, 1, config->mLockOnLen, config->mLockOnTheta), 0);
                        mFC0 = mFC4;
                    } else {
                        fn_8016DC14(mFC0, 0);
                    }
                } else {
                    ((BattleTask*)lbl_8087F048)->fn_80108A54(this);
                }
                mFlag5 = 1;
                if (fn_800E70EC(0, a, b)) {
                    attack = 0;
                }
                if (!lbl_8087F0A8->mInput.fn_801240B4(7) && !mFlagB16) {
                    mFlag5 = 0;
                }
            } else if (lbl_8087F0A8->mInput.fn_801231D0(0x16) &&
                       ((b == 0 && a == -1) || !lbl_8087F0A8->mInput.fn_801231D0(2))) {
                fn_800E610C();
                attack = 0;
            } else {
                attack = fn_800E73EC(field, a, b);
            }
        }
    }
    int special = 0;
    if (attack && fn_80178078() < 0.3f) {
        special = 1;
    }
    switch (m5C->m11C) {
    case 0:
    case 1:
    case 2:
        special = special && !mFlag6;
        break;
    case 3:
    case 4:
        special = special && (mStatus.m0C & 4) != 4;
        break;
    }
    if (special) {
        int wait = 6;
        if (mFlag31) {
            wait = 12;
        }
        if (lbl_8087F0A8->mInput.fn_801240B4(9) && ((GameTask*)field)->fn_803761AC() && fn_8016DCD4() && !mStatus.fn_80134134(0x2E) &&
            field->m5674 != 0) {
            if (mFB8 >= wait) {
                switch (fn_80219558(m50)) {
                case 0:
                    if (fn_80219344(this, 1) != 0 && ((GameTask*)field)->fn_80375184() && HpRate_unk(this) >= 0.25f &&
                        (mStatus.m0C & 4) != 4) {
                        if (lbl_8087F610 == 0) {
                            fn_8016D74C(fn_80219344(this, 1), 0, 0);
                        } else {
                            fn_8016D74C(fn_80219E6C(mStatus.m2D0[m145C].m[0]), 0, 0);
                        }
                    }
                    break;
                case 2:
                case 3:
                    if (lbl_8087F610 != 0 && mStatus.m2D0[m145C].m[0] > 0 && HpRate_unk(this) >= 0.25f &&
                        (mStatus.m0C & 4) != 4) {
                        fn_8016D74C(fn_80219E6C(mStatus.m2D0[m145C].m[0]), 0, 0);
                    }
                    break;
                case 6: {
                    CharaStatusPair_unk& magic = mStatus.m2D0[m145C];
                    if (magic.m[0] != fn_8021921C(this, 4)) {
                        fn_800E9110();
                    }
                    break;
                }
                case 1:
                case 4:
                case 5:
                    fn_800E9110();
                    break;
                default: {
                    CharaStatusPair_unk& magic = mStatus.m2D0[m145C];
                    if (magic.m[0] > 0) {
                        if ((unsigned int)m5C->m11C <= 2) {
                            if ((unsigned int)m50 == 0xAAE72) {
                                if ((mStatus.m0C & 4) != 4) {
                                    fn_800E9110();
                                }
                            } else if ((unsigned int)(m50 - 0xAAE76) <= 1) {
                                fn_80178208(new ((MemoryTag)3, "", "", 0) chr::ControlZanTackle(this));
                            } else if (HpRate_unk(this) >= 0.25f && (mStatus.m0C & 4) != 4) {
                                fn_8016D74C(fn_80219E6C(magic.m[0]), 0, 0);
                            }
                        } else {
                            fn_800E9110();
                        }
                    }
                    break;
                }
                }
            }
            mFB8 += 1.0f;
        } else {
            special = 0;
        }
    }
    if (!special && mFB8 > 0.0f) {
        fn_8016DA4C();
    }
    fn_800E7BB0();
}

int BattleCharaTask::fn_800E70EC(int a, int b, int c) {
    if (m1424 == 0 || a != 0) {
        GameConfig_unk* config = lbl_8087F0A8;
        if (SetConfig::sInstance->mAttackType != 0 && !mFlagB16 && c == 0 && config->mInput.fn_801231D0(4) &&
            !mFlag31) {
            if (a == 0) {
                Effect_unk* mgr = lbl_8087F048;
                atn::Vector3 dir = fn_80178018();
                CharaTask* target =
                    ((BattleTask*)mgr)->fn_800FFE68(this, &dir, 0, 0, config->mBtnAtkCorrectRadius, config->mBtnAtkCorrectTheta);
                if (target != 0) {
                    fn_800E8688(0xC8, 1, target, &target->mPos, 0, 0);
                } else {
                    fn_800E8688(0xC8, 1, 0, 0, 0, 0);
                }
            }
            return 1;
        }
    }
    return 0;
}

int BattleCharaTask::fn_800E7214(CharaTask** out, int keep) {
    GameConfig_unk* config = lbl_8087F0A8;
    if (mFlag31 || mFlagB16) {
        return 0;
    }
    if (SetConfig::sInstance->mAttackType == 0) {
        if (fn_80178078() < 0.4f || m1424 > 0) {
            return 0;
        }
    } else {
        return 0;
    }
    float range = config->m248;
    atn::Vector3 dir = fn_80178018();
    int ok = 1;
    if (mFlag31) {
        ok = atn::Abs_unk(atn::WrapAngle_unk(dir.y - mRot.y)) > 1.5707964f;
    }
    if (keep) {
        range *= 1.2f;
    }
    if (ok) {
        if (fn_801004FC(lbl_8087F048, out, this, &dir, range, config->m24C)) {
            if (keep == 0 || (CharaTask*)m1440 != *out || SetConfig::sInstance->mAttackType != 0) {
                m1440 = (int)*out;
                return 1;
            }
        }
        if (((BattleTask*)lbl_8087F048)->fn_80100BB8(out, this, &dir, config->m248, config->m24C)) {
            return 1;
        }
    }
    return 0;
}

int BattleCharaTask::fn_800E73EC(Field_unk* field, int b, int c) {
    GameConfig_unk* config = lbl_8087F0A8;
    void* obj = &field->m006C;
    int ret = 1;
    if (config->mInput.fn_801231D0(0x11)) {
        ((GameCamera*)obj)->fn_8037EF30(0, 1, -1.0f);
        mFC0 = 0;
    }
    if (lbl_8087F0A8->mInput.fn_801240B4(0x12) && !mFlagB9) {
        if (!mFlag31) {
            fn_80155DAC(1);
        }
    } else if ((lbl_8087F0A8->mInput.fn_801240B4(7) || mFlagB16) && (!mFlag31 || !mFlag30) && !mFlagB9) {
        fn_800C344C(sBattleSe_unk[0], &mPos, 0, -1, 1.0f);
        if (((GameTask*)lbl_8087F430)->fn_80370174(0xEF) == 0 && ((GameTask*)lbl_8087F430)->fn_80370174(0xEE) == 1) {
            ((GameTask*)lbl_8087F430)->fn_80370320(0xEF, 1, 0);
        }
        if (mFC0 == 0) {
            if (mFC4 != 0) {
                mFC0 = mFC4;
            } else {
                Effect_unk* mgr = lbl_8087F048;
                atn::Vector3 dir = fn_80178018();
                mFC0 = ((BattleTask*)mgr)->fn_800FFE68(this, &dir, 0, 1, config->mLockOnLen, config->mLockOnTheta);
            }
        }
        mFlag5 = 1;
    } else if (fn_800E70EC(0, b, c)) {
        ret = 0;
    } else {
        mFC0 = 0;
        mFlag5 = 0;
        BattleTarget_unk target;
        if (fn_800E7214(&target.mChara, 0)) {
            fn_800E8688(200, 0, target.mChara, &target.mPos, 0, fn_80041A28(0, this, target.mChara));
        }
    }
    return ret;
}

void BattleCharaTask::fn_800E7650(Field_unk* field, int, int) {
    GameConfig_unk* config = lbl_8087F0A8;
    void* obj = &field->m006C;
    mFlag5 = 0;
    if (m55C != 1) {
        mFlag4 = 0;
        mFC0 = 0;
        return;
    }
    if (mFlag27) {
        mFC0 = 0;
        mFlag4 = 0;
    } else if (m1208 != 0 || mFlag3 || mF54 != 0 || m139C != 0) {
        mFC0 = 0;
        mFlag5 = 0;
        mFlag4 = 0;
        if (lbl_8087F0A8->mInput.fn_801231D0(0x11)) {
            ((GameCamera*)obj)->fn_8037EF30(0, 1, -1.0f);
        }
    } else if (mFlag31) {
        mFC0 = 0;
        mFlag4 = 0;
    } else if (mFlag6) {
        mFC0 = 0;
        mFlag4 = 0;
        if (!lbl_8087F0A8->mInput.fn_801240B4(0x12)) {
            fn_80155DAC(0);
        }
    } else if (mFlag4) {
        if (lbl_8087F0A8->mInput.fn_801231D0(0x16)) {
            int mode = lbl_8087F0A8->mSurroundMode;
            if (!mFlag10 || mode != 1) {
                atn::Vector3 dir(0.0f, 0.0f, 1.0f);
                atn::Vector3 rot = fn_80178018();
                Mtx m;
                PSMTXRotRad(m, 'y', rot.y);
                PSMTXMultVec(m, &dir, &dir);
                fn_80155790(dir);
                if (mFlag10 && mode == 2) {
                    if (mFlag10) {
                        for (CharaPair_unk* it = m125C.begin(); it != m125C.end(); it++) {
                            it->m04->fn_80155790(dir);
                        }
                    }
                }
            }
        }
        if (mFlag31) {
            mFC0 = 0;
        } else if (!lbl_8087F0A8->mInput.fn_801231D0(0x1E)) {
            ((BattleTask*)lbl_8087F048)->fn_80108A54(this);
        }
        mFlag4 = 1;
        if (!lbl_8087F0A8->mInput.fn_801240B4(7)) {
            mFlag4 = 0;
        }
    } else {
        mFC0 = 0;
        mFlag4 = 0;
        if (lbl_8087F0A8->mInput.fn_801231D0(0x11)) {
            ((GameCamera*)obj)->fn_8037EF30(0, 1, -1.0f);
            mFC0 = 0;
        } else if (lbl_8087F0A8->mInput.fn_801240B4(7)) {
            fn_800C344C(sBattleSe_unk[0], &mPos, 0, -1, 1.0f);
            if (((GameTask*)lbl_8087F430)->fn_80370174(0xEF) == 0 && ((GameTask*)lbl_8087F430)->fn_80370174(0xEE) == 1) {
                ((GameTask*)lbl_8087F430)->fn_80370320(0xEF, 1, 0);
            }
            if (mFC0 == 0) {
                Effect_unk* mgr = lbl_8087F048;
                atn::Vector3 dir = fn_80178018();
                mFC0 = ((BattleTask*)mgr)->fn_800FFE68(this, &dir, 0, 1, config->mLockOnLen, config->mLockOnTheta);
            }
            mFlag4 = 1;
            ((GameCamera*)obj)->fn_8037EF30(0, 1, -1.0f);
        } else if (lbl_8087F0A8->mInput.fn_801240B4(0x12)) {
            fn_80155DAC(1);
        }
    }
    if (lbl_8087F0A8->mInput.fn_801237F0(0x17)) {
        CrossbowMenuTask::sInstance->fn_801E9ECC(-1, 1);
        fn_800C31F4(sBattleSe_unk[4], 0, -1, 1.0f);
    } else if (lbl_8087F0A8->mInput.fn_801237F0(0x18)) {
        CrossbowMenuTask::sInstance->fn_801E9ECC(1, 1);
        fn_800C31F4(sBattleSe_unk[4], 0, -1, 1.0f);
    } else if (lbl_8087F0A8->mInput.fn_801237F0(0x1A)) {
        CrossbowMenuTask::sInstance->fn_801E97DC();
        fn_800C31F4(sBattleSe_unk[4], 0, -1, 1.0f);
    } else if (lbl_8087F0A8->mInput.fn_801237F0(0x1B)) {
    }
    if (lbl_8087F0A8->mInput.fn_801231D0(6) && (!mFlag31 || mC48 != 0) && m640 <= 0 && mFB0 <= 0) {
        mFlagB8 = 0;
        fn_800E8FB8();
    }
    fn_800E7BB0();
}

void BattleCharaTask::fn_800E7BB0() {
    if (m648 == 0) {
        return;
    }
    int skill = 0;
    if (mType == 0 || mType == 3) {
        if (mStatus.fn_80134134(0xD) != 0) {
            skill = 0x179B;
        } else if (mStatus.fn_80134134(0x29) != 0) {
            skill = 0x177B;
        } else if (mStatus.m228 >= 1.0f && fn_80219344(this, 5) != 0) {
            skill = mStatus.m2D0[3].m[0];
        }
    }
    int ok = 0;
    if (fn_8016DDB0(0) && skill > 0) {
        if ((unsigned int)(skill - 0x1774) <= 1) {
            if (mType == 0 || (mC58.mC0 > 0 && mC58.mC4 != 0)) {
                HitInfo_unk* hit = fn_80219E6C(skill);
                AiTarget* target = lbl_8087EE68 != 0 ? lbl_8087EE68->fn_800185B4(this, hit) : 0;
                if (lbl_8087F0A8->mAutoTensionSkillFriend != 0 || target != 0) {
                    ok = 1;
                }
            }
        } else {
            if (mType == 0 || (mC58.mC0 > 0 && mC58.mC4 != 0)) {
                HitInfo_unk* hit = fn_80219E6C(skill);
                AiTarget* target = lbl_8087EE68 != 0 ? lbl_8087EE68->fn_800185B4(this, hit) : 0;
                if (lbl_8087F0A8->mAutoTensionSkillFriend != 0 || target != 0) {
                    ok = 1;
                }
            }
        }
    }
    if (ok && skill > 0) {
        switch (skill) {
        case 0x1774:
        case 0x1775:
            if (lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_8036DAA8(this)) {
                fn_80178208(new ((MemoryTag)0, "", "", 0) chr::ControlOverdrive(this, skill));
            }
            break;
        case 0x1785:
        case 0x1786:
        case 0x1787:
        case 0x1788:
        case 0x1791:
        case 0x1792:
            if (mType != 0) {
                fn_8016D74C(fn_80219E6C(skill), 0, 0);
            }
            break;
        case 0x178F:
        case 0x1790:
            // inferred: the skill pair m2D0[3] is passed as the skill reference
            fn_8016125C((const SkillRef_unk*)&mStatus.m2D0[3], this, 0x173, 0, 10.0f);
            break;
        default:
            if ((mStatus.m0C & 4) != 4) {
                HitInfo_unk* hit = fn_80219E6C(skill);
                m63C = m638;
                m638 = hit;
                mF7C = this;
                mF6C = mPos;
                if (mType != 0) {
                    fn_8015F568(mF6C, this, hit);
                }
            }
            break;
        }
    } else if (mType == 0) {
        int index = fn_80219558(m50);
        if (index == 0 || index == 2) {
            fn_800E7F04();
        }
    }
}

// NONMATCHING: 97.4% - stack layout: the original frame is 0x10 bigger (0x14 bytes between the direction and the
// position of the loop, 8 more above the homing parameters) and keeps the addresses of the loop vectors in
// r24-r26; the code otherwise matches (state checks with result flags, homing declared outside the loop)
// inferred: combined attack: the button starts it on this character and, when it is ready (mFlag28 of the
// partner), fires the bullets of the explosion skill (level from fn_8012F3D8) at up to 7 enemies around
void BattleCharaTask::fn_800E7F04() {
    GameConfig_unk* config = lbl_8087F0A8;
    fn_80210220(config->mParamIndex);
    if (m1428 != 0 && m1428->mFlag28) {
        if (lbl_8087F0A8->mInput.fn_801231D0(8) && m1430 <= 0 && config->mButtonReset != 0) {
            HitInfo_unk* skill = 0;
            int index = fn_80219558(m50);
            if (config->mExplosion != 0 && mStatus.fn_801346C8(0x3D) && mStatus.fn_8012F3D8() > 0 &&
                lbl_8087F048 != 0) {
                int level = mStatus.fn_8012F3D8();
                if (level == 1) {
                    skill = fn_80219E6C(index == 2 ? 0x201 : 0x1F7);
                } else if (level == 2) {
                    skill = fn_80219E6C(index == 2 ? 0x202 : 0x1F8);
                } else {
                    skill = fn_80219E6C(index == 2 ? 0x203 : 0x1F9);
                }
            }
            if (skill != 0) {
                Effect_unk* mgr = lbl_8087F048;
                fn_800FAB80(mgr, this, skill, fn_800F8548(mgr), &mPos, &mRot, 0, 30, -1, -1, 0.0f, 1.0f);
                Color4f_unk color(1.0f, 1.0f, 1.0f, 1.0f);
                ((BattleTask*)lbl_8087F048)->fn_80107168(skill->m04, 0, 0, m1428->GetModel_unk(), &lbl_807C7030, &lbl_807C7030,
                            &color, 1.0f);
                ((BattleTask*)lbl_8087F048)->fn_80107208(skill->m04, &m1428->mPos, 1.0f);
                HitInfo_unk* sub = skill->m6C > 0 ? fn_80219E6C(skill->m6C) : 0;
                if (sub != 0) {
                    FixedArray_unk<CharaTask*, 32> targets;
                    HomingParam_unk homing;
                    for (EnemyTask* e = EnemyManager::sInstance->mHead; e != 0; e = e->mNext) {
                        if (CanActB_unk(e) && (e->m54C & 0x2000) != 0x2000 && !fn_8016F3D0(e, 1) &&
                            (e->mStatus.m0C & 0x800) == 0x800) {
                            targets.PushBack_unk(e);
                        }
                    }
                    int count = targets.mCount;
                    if (targets.mCount < 7) {
                        count = 7;
                    }
                    int id = fn_800F8548(lbl_8087F048);
                    for (int i = 0; i < count; i++) {
                        atn::Matrix3x4 rot;
                        PSMTXRotRad(rot.m, 'y', i * (6.2831855f / count));
                        atn::Vector3 pos;
                        pos = FrontDir_unk(mRot);
                        PSMTXMultVec(rot.m, &pos, &pos);
                        pos.y += 0.4f;
                        if (targets.mCount != 0) {
                            homing.mAccel = 0.0f;
                            homing.m08 = -1.0f;
                            homing.mMaxSpeed = 10.0f;
                            homing.mTime = 300.0f;
                            homing.mNode = 0;
                            homing.mTurn = 1.2566371f;
                            homing.mTarget = targets[i % targets.mCount];
                            homing.mDelay = 0.0f;
                            homing.mCount = id;
                            Effect_unk* mgr = lbl_8087F048;
                            fn_800F8574(mgr, this, sub, &fn_801781B0(), &pos, &homing, 0x2006, 0, 0.0f, 1.0f);
                        } else {
                            Effect_unk* mgr = lbl_8087F048;
                            fn_800F8574(mgr, this, sub, &fn_801781B0(), &pos, 0, 2, 0, 0.0f, 1.0f);
                        }
                    }
                }
            }
            m1428->fn_80164DCC(skill == 0);
            m1428 = 0;
            m14A8 |= 0x8000000;
        }
    } else if ((m1428 == 0 || !m1428->mFlag27) && fn_8016DDB0(1) && m1430 <= 0 && mStatus.fn_801346C8(10) &&
               lbl_8087F610 == 0) {
        bool pressed = false;
        if (lbl_8087F0A8->mInput.fn_801231D0(8)) {
            pressed = true;
        }
        if (pressed) {
            m1428 = this;
            if (this != 0) {
                fn_801647BC();
                m14A8 |= 0x8000000;
                m1430 = config->mCoolDownFrame;
            }
            ((BattleTask*)lbl_8087F048)->fn_8010AB94();
        }
    }
    if (m1430 > 0) {
        m1430--;
    }
}

void BattleCharaTask::vf40(HitInfo_unk* hit) {
    if (mType == 0) {
        if (mStatus.IsState_unk(0x20)) {
            fn_80164DCC(1);
            HitMsg_unk* msg = (HitMsg_unk*)hit;
            if (m55C != 6 || (m560 != 0x17 && m560 != 0x3B && m560 != 0x8E)) {
                fn_8015E7A0(msg->m28 * 5.0f, -1, 0);
            }
            vf44();
        }
    } else {
        vfE8((HitMsg_unk*)hit);
    }
}

void BattleCharaTask::vf44() {
    CharaTask::vf44();
    if (!mFlag16) {
        m1434 = 30;
    }
    if (lbl_8087F430 != 0) {
        ((GameTask*)lbl_8087F430)->fn_80375184();
    }
    m1470 = 0;
}

// NONMATCHING: 98.1% - cases 1 and 2 of mC48 become a range compare; the original keeps f1 = radius for
// fn_80157444/fn_80157C34
// inferred: starts the attack `id` (200: normal attack) at `target`/`pos`; type and mode pass through to
// CharaTask::fn_801562A0
void BattleCharaTask::fn_800E8688(int id, int b, CharaTask* target, const atn::Vector3* point, int type, int mode) {
    if (mFlag6) {
        return;
    }
    m1460 = 0;
    if (target != 0 && id == 200) {
        atn::Vector3 d = target->mPos - mPos;
        float dist = PSVECMag(&d) - target->m5B0;
        if (dist < target->vf70()) {
            int kind = target->vf68(this);
            if (kind != 0) {
                target->vf6C(kind, this);
                switch (kind) {
                case 8: {
                    atn::Vector3 dir(target->mPos.x - mPos.x, 0.0f, target->mPos.z - mPos.z);
                    PSVECNormalize(&dir, &dir);
                    m590 = fn_800F8548(lbl_8087F048);
                    m594 = 1;
                    m598 = 0;
                    m59C = 0;
                    m59D = 0;
                    m59E = 1;
                    m59F = 0;
                    m5A0 = 1.0f;
                    fn_80178208(new ((MemoryTag)3, "", "", 0)
                                    chr::ControlAttackLastOne(this, dir, target, 9, fn_80219E6C(0xE2)));
                    m58C = 1;
                    mFlag5 = 0;
                    return;
                }
                case 9:
                case 10:
                case 11:
                case 12: {
                    atn::Vector3 dir(target->mPos.x - mPos.x, 0.0f, target->mPos.z - mPos.z);
                    PSVECNormalize(&dir, &dir);
                    m590 = fn_800F8548(lbl_8087F048);
                    m594 = 1;
                    m598 = 0;
                    m59C = 0;
                    m59D = 0;
                    m59E = 1;
                    m59F = 0;
                    m5A0 = 1.0f;
                    fn_80178208(new ((MemoryTag)3, "", "", 0)
                                    chr::ControlAttackLastOne(this, dir, target, kind, fn_80219E6C(0xE2)));
                    m58C = 1;
                    mFlag5 = 0;
                    return;
                }
                }
            }
        }
    }
    if (mode == 1) {
        m59E = 1;
    } else {
        m59E = 0;
    }
    GameConfig_unk* config = lbl_8087F0A8;
    float radius = 0.0f;
    int far = 0;
    if (id == 0xCB || id == 0xCD) {
        far = 1;
    }
    int found = 0;
    atn::Vector3 pos;
    if (point != 0) {
        found = 1;
        pos = *point;
    }
    if (!found) {
        CharaTask* lock = mFC4 != 0 ? mFC4 : mFC0;
        if (lock != 0) {
            found = 1;
            pos = lock->m614;
            pos.y = lock->mPos.y;
            radius = lock->m620;
        }
    }
    if (!found && mFlag31) {
        switch (mC48) {
        case 1:
        case 2: {
            mPos += mC48 == 1 ? mC14 * -5.0f : mC14 * 5.0f;
            atn::Vector3 dir;
            dir = lbl_8087F430->m006C.m14 - lbl_8087F430->m006C.m08;
            PSVECNormalize(&dir, &dir);
            dir *= 80.0f;
            found = 1;
            far = 1;
            pos = mPos + dir;
            fn_801539E0();
            break;
        }
        case 3: {
            atn::Vector3 dir;
            dir = lbl_8087F430->m006C.m14 - lbl_8087F430->m006C.m08;
            PSVECNormalize(&dir, &dir);
            dir *= 80.0f;
            found = 1;
            type = 2;
            far = 1;
            pos = mPos + dir;
            fn_801539E0();
            break;
        }
        default:
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 1.0f;
            Mtx m;
            PSMTXRotRad(m, 'y', mRot.y);
            PSMTXMultVec(m, &pos, &pos);
            found = 1;
            pos += mPos;
            break;
        }
    }
    atn::Vector3 rot = fn_80178018();
    if (mFlag5) {
        rot = mRot;
    }
    if (!found) {
        if (far) {
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 80.0f;
        } else {
            pos.x = 0.0f;
            pos.y = 0.0f;
            pos.z = 1.0f;
        }
        Mtx m;
        PSMTXRotRad(m, 'y', rot.y);
        PSMTXMultVec(m, &pos, &pos);
        pos += mPos;
    }
    if (mFlag31) {
        id = 0xC9;
    }
    HitInfo_unk* hit = fn_80219E6C(id);
    if (id == 200 && m648 != 0) {
        hit = ((Arms*)m648)->fn_80044C30(0);
    }
    m58C = 1;
    if (lbl_8087F048 != 0) {
        m590 = fn_800F8548(lbl_8087F048);
    }
    mFlag5 = 0;
    if (id == 0xD0 || id == 0xE5) {
        m594 = -1;
        m59D = 0;
        m59C = 0;
        m59F = 1;
        if (m648 != 0) {
            m648->vf18(lbl_807C7030, lbl_807C7030, 0, 200);
        }
        m63C = m638;
        m638 = hit;
    } else if (mFlagC30) {
        m594 = config->m274;
        fn_80157444(hit, pos);
    } else {
        m594 = id == 200 ? config->m270 : -1;
        if (mode == 2) {
            fn_8015783C(hit, pos, radius);
        } else if (mode == 4) {
            fn_80157C34(hit, pos);
        } else if (target != 0 && mode == 5) {
            fn_8015802C(hit, target);
        } else {
            fn_801562A0(hit, pos, type, far, b, 0, radius);
        }
    }
    m598 = 0;
    int manual = 0;
    if (config->m318 != 0) {
        if (mType == 0 && SetConfig::sInstance->mAttackType != 0) {
            manual = 1;
        }
        if (manual && id == 200) {
            m5A0 = config->mManualPower;
        } else {
            m5A0 = 1.0f;
        }
    } else {
        m5A0 = 1.0f;
    }
    mFlagC30 = 0;
    mFlagC29 = 0;
    m1424 = hit != 0 ? hit->m68 : fn_80219E6C(200)->m68;
}

void BattleCharaTask::fn_800E8FB8() {
    if (m648 == 0) {
        return;
    }
    atn::Vector3 from;
    atn::Vector3 to;
    fn_800E932C(&from, &to, 1);
    int bullet = CrossbowMenuTask::sInstance->fn_801EA01C();
    CharaStatusRef_unk* ref = mStatus.fn_80134134(0x3D);
    if (ref != 0 && fn_80219E6C(ref->mEntry->mValue) != 0 &&
        CrossbowMenuTask::sInstance->m4C[CrossbowMenuTask::sInstance->m150] == 0x65) {
        bullet = ref->mEntry->mValue;
    }
    if (lbl_8087F610 != 0 && CrossbowMenuTask::sInstance->m4C[CrossbowMenuTask::sInstance->m150] == 0x65) {
        if (m5C->m11C == 3 || m50 == 0xAAE72) {
            HitInfo_unk* hit = fn_80219E6C(mStatus.m2D0[m145C].m[0]);
            if (hit != 0 && hit->mC8 > 0) {
                bullet = hit->mC8;
            }
        } else if (m50 == 0xAAE76 || m50 == 0xAAE77) {
            bullet = mStatus.m2D0[1].m[0];
        }
    }
    if (bullet > 0) {
        fn_80165C5C(bullet, from, to);
    }
}

// NONMATCHING: 98.7% - the m2D0 entry is addressed from 0xAA0 (a FixedArray of CharaStatus?) and stack slots of the
// scaled direction temporaries
void BattleCharaTask::fn_800E9110() {
    HitInfo_unk* hit = fn_80219E6C(mStatus.m2D0[m145C].m[0]);
    atn::Vector3 pos = mPos;
    atn::Vector3 dir(0.0f, 0.0f, 50.0f);
    if (mFlag31 && mC48 != 0) {
        atn::Vector3 from;
        atn::Vector3 to;
        fn_800E932C(&from, &to, 1);
        dir = to * 50.0f;
    } else {
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &dir, &dir);
    }
    pos += dir;
    if ((unsigned char)(hit->m02 - 3) <= 1) {
        atn::Vector3 front(0.0f, 0.0f, 1.0f);
        Mtx m2;
        PSMTXRotRad(m2, 'y', mRot.y);
        PSMTXMultVec(m2, &front, &front);
        pos = mPos + front * 0.1f;
    }
    if (mFlag6) {
        ((GameCamera*)&lbl_8087F430->m006C)->fn_80389838(3);
    }
    fn_8015F568(pos, 0, hit);
    mFC0 = 0;
    mFlag4 = 0;
}

// inferred: direction of the aim camera (name invented)
inline atn::Vector3 AimDir_unk(Field_unk* field) {
    atn::Vector3 dir;
    dir = field->m006C.m208 - field->m006C.m1FC;
    PSVECNormalize(&dir, &dir);
    return dir;
}
// inferred: translation of a node matrix (name invented)

// NONMATCHING: 99.86% - stack slots (the locals sit 0x30 lower than in the original)
// inferred: start point and direction of a shot: in front of the character, or along the aim camera (crossbow,
// mFlag31 with a cover side, or mFlag6) corrected towards the point it hits
void BattleCharaTask::fn_800E932C(atn::Vector3* pos, atn::Vector3* dir, int fromShot) {
    atn::Vector3 shot = mPos;
    if (m648 != 0 && m648->m04 != 0) {
        shot = ((Arms*)m648)->fn_80044BD4();
    }
    if (mFlag6) {
        if ((unsigned int)m50 == 0xAAE76) {
            atn::VecAssign_unk(shot, NodePos_unk(mMotion.GetNodeMtx_unk("ef_locater01")));
        } else if ((unsigned int)m50 == 0xAAE77) {
            atn::VecAssign_unk(shot, NodePos_unk(mMotion.GetNodeMtx_unk("rightwepoint")));
        }
    }
    if (mFlag31) {
        if (mC48 == 0) {
            int side = fn_80153B44();
            if (side == 0) {
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                dir->x = 0.0f;
                dir->y = 0.0f;
                dir->z = 1.0f;
                PSMTXMultVec(m, dir, dir);
                atn::Vector3 off(-3.0f, 15.0f, 4.0f);
                PSMTXMultVec(m, &off, &off);
                *pos = mPos;
                *pos += off;
            } else {
                dir->x = 0.0f;
                dir->y = 0.0f;
                dir->z = -1.0f;
                Mtx m;
                PSMTXRotRad(m, 'y', mRot.y);
                PSMTXMultVec(m, dir, dir);
                float shift = 0.0f;
                switch (side) {
                case 1:
                    shift = 5.0f;
                    break;
                case 2:
                    shift = -5.0f;
                    break;
                }
                *pos = mPos;
                *pos += *dir * (0.1f + m5B0);
                pos->y += m5B4;
                pos->x += shift * (float)cos(mRot.y);
                pos->z += -shift * (float)sin(mRot.y);
            }
        } else {
            Field_unk* field = lbl_8087F430;
            *dir = AimDir_unk(field);
            *pos = fn_8017C9AC(this);
            if (m648 != 0 && m648->m04 != 0) {
                RayResult_unk result;
                atn::Vector3 to = *pos + *dir * 800.0f;
                if (fn_8004ED34(lbl_8087EE98, &result, pos, &to, 4, &mColli, 0)) {
                    if (fromShot) {
                        *dir = result.m04 - shot;
                    } else {
                        MtxPtr node = mMotion.GetNodeMtx_unk("spine1");
                        *dir = result.m04 - (node != 0 ? NodePos_unk(node) : ((Arms*)m648)->fn_80044BD4());
                    }
                    if (PSVECSquareMag(dir) < 0.0001f) {
                        *dir = AimDir_unk(field);
                    } else {
                        PSVECNormalize(dir, dir);
                    }
                    atn::Vector3 aim = AimDir_unk(field);
                    if (PSVECDotProduct(dir, &aim) < 0.25881904f) {
                        *dir = AimDir_unk(field);
                    }
                }
            }
        }
    } else if (mFlag6) {
        Field_unk* field = lbl_8087F430;
        *dir = AimDir_unk(field);
        *pos = fn_8017C9AC(this);
        if (m648 != 0) {
            RayResult_unk result;
            atn::Vector3 to = *pos + *dir * 800.0f;
            if (fn_8004ED34(lbl_8087EE98, &result, pos, &to, 4, &mColli, 0)) {
                if (fromShot) {
                    *dir = result.m04 - shot;
                } else if (mFlagB19) {
                    MtxPtr node = mMotion.GetNodeMtx_unk("leftshoulder");
                    *dir = result.m04 - (node != 0 ? NodePos_unk(node) : ((Arms*)m648)->fn_80044BD4());
                } else {
                    MtxPtr node = mMotion.GetNodeMtx_unk("spine1");
                    *dir = result.m04 - (node != 0 ? NodePos_unk(node) : ((Arms*)m648)->fn_80044BD4());
                }
                if (PSVECSquareMag(dir) < 0.0001f) {
                    *dir = AimDir_unk(field);
                } else {
                    PSVECNormalize(dir, dir);
                }
                atn::Vector3 aim = AimDir_unk(field);
                if (PSVECDotProduct(dir, &aim) < 0.25881904f) {
                    *dir = AimDir_unk(field);
                }
            }
        }
    } else {
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        dir->x = 0.0f;
        dir->y = 0.0f;
        dir->z = 1.0f;
        PSMTXMultVec(m, dir, dir);
        atn::Vector3 off(-3.0f, 15.0f, 4.0f);
        PSMTXMultVec(m, &off, &off);
        *pos = mPos;
        *pos += off;
    }
    if ((m648 != 0 && m648->m04 != 0) || (unsigned int)(m50 - 0xAAE76) <= 1) {
        *pos = shot;
    }
}

// inferred: first skill of the weapon (name invented)
inline int WeaponSkill_unk(Arms* arms) {
    int id = 0;
    if (arms != 0 && arms->m274 != 0) {
        id = arms->m274->mAC[0];
    }
    return id;
}

// inferred: controls (m560) that let the character move freely (name invented)
inline bool IsFreeControl_unk(int id) {
    switch (id) {
    case 0x14:
    case 0x42:
        return false;
    case 2:
    case 3:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x27:
    case 0x34:
    case 0x35:
    case 0x41:
    case 0x43:
    case 0x47:
    case 0x63:
    case 0x6E:
    case 0x6F:
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
    case 0x76:
    case 0x77:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
    case 0x8E:
        return true;
    }
    return false;
}

// NONMATCHING: 97.8% - the result of IsFreeControl_unk is materialized (the original branches from its switch);
// registers of the fn_800F8C6C arguments
// inferred: update of the party members and enemies with a weapon (types 1-3; fn_800E589C for the player)
void BattleCharaTask::fn_800E9ED8() {
    switch (m58C) {
    case 0:
        if (m55C == 6 && IsFreeControl_unk(m560)) {
            mC58.m2B0 = 0;
            fn_80139560();
        } else if (m55C == 6 && m560 == 10) {
            float frame = mMotion.mTracks[0].m08;
            if (frame > 9.0f && frame < 12.0f && mControl) {
                ControlTimer_unk* control = (ControlTimer_unk*)mControl.get();
                if (control != 0) {
                    control->m1C = 1;
                    if (fn_8016E484()) {
                        fn_800EB270(fn_8021AF98(2));
                    } else {
                        int id = 0xD0;
                        if (mType == 2) {
                            id = 0xE5;
                        }
                        fn_800EB270(id);
                    }
                }
            }
        } else if (m55C == 7) {
            atn::Vector3 pos = mPos;
            fn_80139560();
            pos.x -= mPos.x;
            pos.y -= mPos.y;
            pos.z -= mPos.z;
            if (m570 > 0.4f && atn::Abs_unk(pos.x) < 0.2f && atn::Abs_unk(pos.y) < 0.2f && atn::Abs_unk(pos.z) < 0.2f) {
                if (++mC58.m2B0 > 1) {
                    if (fn_8016F67C()) {
                        atn::Vector3 front = FrontDir_unk(mRot);
                        vf74(front);
                        mC58.m2B0 = 0;
                    } else if (fn_801725EC(&mPos, &m574, 0)) {
                        mC58.m2B0 = 0;
                    }
                }
            } else {
                mC58.m2B0 = 0;
            }
        } else if (mC58.mMode == 5 && mC58.mStep == 2) {
            if (mType == 2 && mMotion.fn_80097D40(0x173) && lbl_8087F0A8->m080 > 0 &&
                ((unsigned int)mStatus.mParamB.m9C & 0x1000) != 0x1000) {
                m58C = 5;
                fn_80158EF0(lbl_8087F0A8->m080);
            } else {
                fn_800EB270(WeaponSkill_unk((Arms*)m648));
            }
        } else if (mC58.mMode == 6 && mC58.mStep >= 1) {
            fn_800EB4AC();
        } else {
            atn::Vector3 pos = mPos;
            if (lbl_8087F610 == 0) {
                fn_80139560();
            } else if (mType == 3 && lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead == this) {
                int grappled = 0;
                if (m55C == 6 && m560 == 28) {
                    grappled = 1;
                }
                fn_80139560();
                if (grappled && (m55C != 6 || m560 != 28) && lbl_8087F430 != 0 && lbl_8087F430->m868 == 6) {
                    ((GameTask*)lbl_8087F430)->fn_803748E0(1);
                    lbl_8087F430->m94C = 1;
                }
            } else {
                fn_80139560();
            }
            pos.x -= mPos.x;
            pos.y -= mPos.y;
            pos.z -= mPos.z;
            if (m570 > 0.4f && atn::Abs_unk(pos.x) < 0.2f && atn::Abs_unk(pos.y) < 0.2f && atn::Abs_unk(pos.z) < 0.2f) {
                if (++mC58.m2B0 > 1) {
                    CharaTask* other = 0;
                    if (fn_8016F67C()) {
                        atn::Vector3 front = FrontDir_unk(mRot);
                        vf74(front);
                        mC58.m2B0 = 0;
                    } else if (!mFlagC25 && fn_801725EC(&mPos, &m574, &other)) {
                        if (mC58.mMode == 5 && mC58.mStep == 1 && mC58.mD0 == 0 && mC58.mC4 != 0) {
                            atn::Vector3 up(0.0f, 1.0f, 0.0f);
                            atn::Vector3 front = FrontDir_unk(mRot);
                            atn::Vector3 side;
                            PSVECCrossProduct(&front, &up, &side);
                            mC14 = side;
                            m58C = 1;
                            if (lbl_8087F048 != 0) {
                                m590 = fn_800F8548(lbl_8087F048);
                            }
                            m594 = -1;
                            CharaTask* target = mC58.mC4;
                            float radius = target->m5B0;
                            fn_801562A0(fn_80219E6C(200), target->mPos, 1, 1, 0, other, radius);
                        }
                        mC58.m2B0 = 0;
                    }
                }
            } else {
                mC58.m2B0 = 0;
            }
        }
        break;
    case 1: {
        if (fn_80158BB4()) {
            m58C = 0;
            if ((unsigned int)(m560 - 0x15) > 2 && m560 != 0x47 && m560 != 0x8E && m55C != 7) {
                fn_8016EB48(1, 1, 1);
            }
        }
        GameConfig_unk* config = lbl_8087F0A8;
        float frame = HitFrame_unk(mMotion.mTracks[0].m00);
        if (mFlagC29 && config->mAttackGuardWait > 0.0f && mMotion.mTracks[0].m08 >= frame) {
            float rest = mMotion.fn_80097D7C(0) - frame;
            float speed = rest / (rest + config->mAttackGuardWait);
            if (((unsigned int)mStatus.m14 & 0x2000) == 0x2000) {
                speed *= ((BattleTask*)lbl_8087F048)->fn_8010CA34();
            }
            mMotion.mTracks[0].m0C = speed;
        }
        fn_80139560();
        if (lbl_8087F048 != 0 && fn_80158CA4()) {
            if (mControl && mControl->vf30() > m598) {
                m598 = mControl->vf30();
                m594 = 1;
                m590 = fn_800F8548(lbl_8087F048);
            }
            if (m594 != 0) {
                fn_80144710();
                HitResult_unk results[8];
                int n = fn_800F8C6C(lbl_8087F048, results, 8, this, m638, m590, 30, m594, 0.0f);
                if (n > 0) {
                    if (m594 > 0) {
                        m594 -= n;
                    }
                    int critical = 0;
                    for (int i = 0; i < n; i++) {
                        if ((results[i].mFlags & 8) == 8) {
                            critical = 1;
                        }
                    }
                    if (critical && (unsigned int)(m560 - 4) <= 1) {
                        mFlagC29 = 1;
                    }
                }
            }
        }
        break;
    }
    case 5:
        if (m55C != 6 || m560 != 12) {
            m58C = 0;
            fn_80139560();
        } else {
            fn_80139560();
            if (((ControlTimer_unk*)mControl.get())->IsDone_unk()) {
                fn_800EB270(WeaponSkill_unk((Arms*)m648));
            }
        }
        break;
    case 2:
        if (lbl_8087F610 == 0) {
            vfF0();
        } else if (mType == 3 && lbl_8087F8A0 != 0 && lbl_8087F8A0->mHead == this) {
            vfEC();
        } else {
            vfF0();
        }
        break;
    case 3:
    case 4:
        fn_80139560();
        break;
    }
    fn_800E7BB0();
}

// inferred: listeners of the character events (inline in the original, as in gmk/ReinforcePoint.cpp)
inline CharaListeners_unk* fn_8014F8F4() {
    static CharaListeners_unk sListeners;
    return &sListeners;
}

// inferred: calls the listeners of `id` (as in gmk/ReinforcePoint.cpp; name invented)
inline void NotifyListeners_unk(CharaListeners_unk* listeners, int id, void* chara, void* arg) {
    if (listeners->mEnabled != 0) {
        for (int i = 0; i < (int)listeners->mList.size(); i++) {
            CharaListener_unk& listener = listeners->mList[i];
            if (listener.mId == -1 || id == listener.mId) {
                listener.mFunc(id, (CharaTask*)chara, arg);
            }
        }
    }
}

// inferred: end of the knock-down of the head of the party in multiplayer (type 3): event 5 to the
// listeners; offline (no session) the character dies after m1450 frames
void BattleCharaTask::vfEC() {
    if (fn_8015EB2C() == 0) {
        fn_80139560();
        m1450 = 0;
        return;
    }
    if (mF50 == 0 && !fn_80175AEC() && !(m55C == 6 && m560 == 0x65)) {
        if (lbl_8087F610 != 0) {
            if (m1450 == 0) {
                NotifyListeners_unk(fn_8014F8F4(), 5, this, 0);
                mColli.mFlags &= ~1;
                fn_800D246C(1);
                m1450 = 1;
            }
        } else if (m1450 > 0) {
            if (--m1450 == 0) {
                HitInfo_unk* data = fn_80219E6C(0x2711);
                if (!mStatus.IsState_unk(0x20)) {
                    mStatus.fn_8013310C(0x20, 0);
                }
                DamageResult_unk result;
                fn_8003EA3C(&result, data, this, this, 0, &lbl_807C6B90, 0, 0);
            }
        } else {
            NotifyListeners_unk(fn_8014F8F4(), 5, this, 0);
            mColli.mFlags &= ~1;
            if (mStatus.m224 <= 0) {
                fn_800D246C(1);
                if (((GameTask*)lbl_8087F430)->fn_80370174(0x66) == 0) {
                    ((GameTask*)lbl_8087F430)->fn_80370C6C();
                }
            } else {
                m1450 = 30;
            }
        }
    } else {
        fn_80139560();
    }
}

// NONMATCHING: 97.3% - the original reloads the flag word for mFlag25 after the mFlag16 test (as CharaTask::vf34)
// inferred: end of the knock-down: after m1450 frames down the character dies (event 0x1B7 for type 3)
void BattleCharaTask::vfF0() {
    if (fn_8015EB2C() == 0) {
        fn_80139560();
        m1450 = 0;
        return;
    }
    mColli.mFlags &= ~1;
    if (mFlag16) {
        if (mF50 == 0 && !fn_80175AEC() && !(m55C == 6 && m560 == 0x65)) {
            if (m1450 > 0) {
                if (lbl_8087F430 == 0 || ((GameTask*)lbl_8087F430)->fn_80370174(0xAC) == 0) {
                    m1450--;
                }
                if (m1450 == 0) {
                    HitInfo_unk* data = fn_80219E6C(0x2711);
                    if (!mStatus.IsState_unk(0x20)) {
                        mStatus.fn_8013310C(0x20, 0);
                    }
                    DamageResult_unk result;
                    fn_8003EA3C(&result, data, this, this, 0, &lbl_807C6B90, 0, 0);
                    if (mType == 3 && lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_80370174(5) == 0) {
                        ((GameTask*)lbl_8087F430)->fn_803750E4(0x1B7);
                    }
                }
            } else if (mStatus.m224 <= 0) {
                fn_800D246C(1);
            } else {
                m1450 = 600;
            }
        } else {
            fn_80139560();
        }
    } else {
        mFlag25 = 1;
        fn_800EAECC();
    }
}

void BattleCharaTask::fn_800EAECC() {
    if (m1434 > 0) {
        if (!mFlag2) {
            if (m1434 == 30) {
                fn_800EB7A0();
            } else if (m1434 == 15) {
                if (m146C != 0x2C) {
                    ((BattleTask*)lbl_8087F048)->fn_80105BD8(&mMotion);
                    fn_800C344C(sBattleSe_unk[2], &mPos, 0, -1, 1.0f);
                }
                vf9C(1);
            }
        }
        m1434--;
    } else {
        if (!mFlag15 && !(m55C == 6 && m560 == 0x3B)) {
            fn_800EE360();
        }
        mFlag15 = 1;
        fn_801765D8();
    }
}

// inferred: a hit was received
void BattleCharaTask::vfE8(HitMsg_unk* msg) {
    if (mFlag2) {
        return;
    }
    if (m55C != 2 && !(m55C == 6 && m560 == 0x1D)) {
        fn_8015495C(0, 1, 1, 1);
    }
    if (mStatus.IsState_unk(0x20)) {
        if (m55C != 6 || (m560 != 0x17 && m560 != 0x3B && m560 != 0x8E)) {
            fn_8015E7A0(msg->m28 * 5.0f, -1, 0);
        }
        vf44();
        return;
    }
    if (mType == 3 && msg->m68 > 0) {
        float before = (mStatus.m04 + (float)msg->m68) / (float)mStatus.mParamB.m84;
        float after = mStatus.m04 / (float)mStatus.mParamB.m84;
        if (before > 0.3f && after < 0.3f) {
            lbl_8087F490->fn_803E2110(3, this); // 3: low hit points
        }
        if (((GameTask*)lbl_8087F430)->fn_80370174(0x38A) == 1 && msg->mAttacker != 0 && msg->mAttacker->mType == 0 &&
            ((GameTask*)lbl_8087F430)->fn_80370A78(0xFA) == 0) {
            Field_unk* field = lbl_8087F430;
            ((GameTask*)field)->fn_80370AE4(0xFA, fn_80219558(m50));
        }
    }
    if (mType == 2 && (mStatus.m0C & 0x40) != 0x40 && mC58.m11C.m00 >= 2) {
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

void BattleCharaTask::fn_800EB270(int id) {
    GameConfig_unk* config = lbl_8087F0A8;
    atn::Vector3 pos;
    m58C = 1;
    if (lbl_8087F048 != 0) {
        m590 = fn_800F8548(lbl_8087F048);
    }
    m594 = id == 200 ? config->m270 : -1;
    m598 = 0;
    mFlagC29 = 0;
    if (id == 0xD0 || id == 0xE5) {
        m594 = -1;
        m59D = 0;
        m59C = 0;
        m59F = 1;
        if (m648 != 0) {
            m648->vf18(lbl_807C7030, lbl_807C7030, 0, 200);
        }
        HitInfo_unk* hit = fn_80219E6C(id);
        m63C = m638;
        m638 = hit;
    } else if (mC58.mD0 != 0) {
        pos = mC58.mD0->vf54();
        fn_801562A0(fn_80219E6C(id), pos, 0, 0, 0, 0, 0.0f);
    } else if (mC58.mC4 != 0) {
        pos = mC58.mC4->mPos;
        if (fn_80041A28(0, this, mC58.mC4) == 2 && mMotion.fn_80097D40(0x161)) {
            fn_8015783C(fn_80219E6C(id), pos, mC58.mC4->m620);
        } else {
            fn_801562A0(fn_80219E6C(id), pos, 0, 0, 0, 0, mC58.mC4->m620);
        }
    } else {
        fn_801561E4(fn_80219E6C(id));
    }
}

void BattleCharaTask::fn_800EB49C() {
    vf44();
}

// NONMATCHING: 98.3% - this stays in r3 for the first test, and dir/aim get r30/r29 the other way round
void BattleCharaTask::fn_800EB4AC() {
    if (m55C != 6 && m648 != 0 && m648->m04 == 1) {
        atn::Vector3 dir(0.0f, 0.0f, 1.0f);
        Mtx m;
        PSMTXRotRad(m, 'y', mRot.y);
        PSMTXMultVec(m, &dir, &dir);
        atn::Vector3 from;
        atn::Vector3 to;
        atn::Vector3 aim = dir;
        // inferred: the part m648 is the weapon (Arms; fn_800EE5AC receives it as one)
        from = ((Arms*)m648)->fn_80044BD4();
        if (mC58.mD0 != 0) {
            to = mC58.mD0->vf54();
            dir = (to - from).GetNormalize();
            aim = dir;
        } else if (mC58.mC4 != 0) {
            CharaTask* target = mC58.mC4;
            to = target->mColli.m48;
            dir = (to - from).GetNormalize();
            aim = (target->mColli.m3C - mColli.m3C).GetNormalize();
        }
        mFA4 = aim;
        if (mC58.mStep == 2 && m640 <= 0 && mFB0 <= 0) {
            HitInfo_unk* hit = ((Arms*)m648)->fn_80044C30(0);
            int id = hit != 0 ? hit->m04 : 0;
            CharaStatusRef_unk* ref = mStatus.fn_80134134(0x3D);
            if (ref != 0 && fn_80219E6C(ref->mEntry->mValue) != 0) {
                id = ref->mEntry->mValue;
            }
            fn_80165C5C(id, from, dir);
        }
    }
    fn_80139560();
}

// inferred: experience of the kill (popup text and the inventory)
void BattleCharaTask::fn_800EB7A0() {
    if (lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_803754F0() != 0) {
        return;
    }
    if (lbl_8087F0A8->mAddExpImmediately != 0 && lbl_8087F610 == 0 && !mFlag16 && mType == 2 &&
        (mStatus.m0C & 0x100) != 0x100 && (mStatus.m0C & 0x10000) != 0x10000) {
        CharaTask* killer = (CharaTask*)m13B0;
        if (killer != 0 && killer->mType == 2 && (killer->mStatus.m0C & 0x10000) != 0x10000) {
            return;
        }
        int level = mStatus.fn_80133BD8(0);
        int exp = mStatus.mParamB.m80;
        CharaTask* player = lbl_8087F8A0->mHead;
        int a = 0;
        int b = 0;
        int cap = 4;
        fn_804491F4(&a, &b, level, exp, 0, 1.0f);
        if (lbl_8087F430 != 0) {
            cap = ((GameTask*)lbl_8087F430)->fn_80375D0C();
        }
        if (level > cap && lbl_8087F0A8->mCurrentLevel == 3) {
            cap = level;
        }
        fn_80449448(&a, &b, player->mStatus.mParamA.m78, cap);
        a += b;
        int kind = 0;
        atn::Vector3 pos = mColli.m3C + atn::Vector3(0.0f, 2.0f * m5B0, 0.0f);
        float rate = 1.0f;
        if (mC58.m11C.IsFlag_unk(4) || (!mC58.m11C.fn_8011D21C() && mC58.m11C.IsFlag_unk(2))) {
            kind = 2;
            rate *= lbl_8087F0A8->mPickSucceedBounus;
            if (lbl_8087F430 != 0) {
                ((GameTask*)lbl_8087F430)->fn_803750E4(0xEB);
            }
        }
        if (fn_8017CB2C()) {
            kind = 2;
            rate *= lbl_8087F0A8->mSummonExpBounus;
        }
        if (lbl_8087F8A8 != 0) {
            wchar_t text[0x40];
            swprintf(text, 0x20, L"%d", a);
            lbl_8087F8A8->fn_8054D794(2, text, &pos, kind, 0, 1, 0, 0, 0, 1, -1, -1);
        }
        Inventory::instance_->fn_80449A18(m13B0, level, exp, 0, rate);
    }
}

void BattleCharaTask::fn_800EBA5C() {
    fn_801765D8();
    fn_801446F0();
    if (mFlag31) {
        fn_801539E0();
    }
    if (mFlag6) {
        fn_80155DAC(0);
    }
    if (IsFlag54C_unk(0x2000)) {
        m54C &= ~0x2000;
    }
    if (IsFlag3_unk()) {
        mFlag3 = 0;
    }
    fn_80164DCC(0);
    m54C &= ~0x200000;
    m58C = 0;
    m568 = 1.0f;
    mStatus.fn_80133B30(0);
    mStatus.fn_8012D8B8();
    mStatus.m224 = mStatus.mParamB.m98;
    mStatus.m2F4 = 0.0f;
    fn_8016F634();
    mC58.fn_8011BF94();
    mRoute.fn_8012539C();
    mMotion.m1DC = 0;
    mMotion.CalcMatrixStack(atn::ModelNodeCallback_unk());
    mMotion.fn_80097510();
    for (unsigned int i = 0; i < mParts.mCount; i++) {
        ((atn::Model*)mParts.mData[i]->m010)->m1DC = 0;
    }
    chr::CharaEquip_unk** equips = m680;
    for (unsigned int i = 0; i < 8; i++, equips++) {
        if (*equips != 0) {
            (*equips)->GetModel_unk()->m1DC = 0;
        }
    }
    for (unsigned int i = 0; i < m6A4.mCount; i++) {
        m6A4.mData[i]->mModel.m1DC = 0;
    }
    m1428 = 0;
    m142C = 0;
    mFlagC27 = 0;
    mFC0 = 0;
    fn_8016DC14(0, 1);
    fn_8017A33C(0, 0);
    mFlagB9 = 0;
    fn_8017AC44(1);
    m1208 = 0;
    m610 = 0;
    mFlagB18 = 0;
    mF1C = 0;
}

// NONMATCHING: 99.7% - code matches; string and float pool offsets wait for earlier pool users
void BattleCharaTask::fn_800EBD00(int mode, int prev) {
    if (mode != prev) {
        if (mode == 1) {
            lbl_8087F8A0->mHead->fn_8014EEC4(0);
            mMotion.m34C = 1;
            mMotion.fn_80097C08(0, 2, 1, 0, 1, 0.0f, 0.1f);
            for (int i = 0; i < 0x23A; i++) {
                mMotion.fn_80097A20(i);
            }
            m8C.vf0C("/database/chara/motion/na000_00_town.mchr");
        } else {
            lbl_8087F8A0->mHead->fn_8014DEE4(0, 0, 0);
            for (int i = 0; i < 0x23A; i++) {
                mMotion.fn_80097A20(i);
            }
            m8C.vf0C("/database/chara/motion/na000_00.mchr");
        }
        fn_80137B78();
        mAC = 2;
    } else if (mode != 1) {
        bool* used = fn_802185F4();
        for (int i = 0; i < 0x23A; i++) {
            if (!used[i]) {
                mMotion.fn_80097A20(i);
            }
        }
        fn_80477694(&m8C, "/database/chara/motion/na000_00.mchr");
        mAC = 2;
    }
}

int BattleCharaTask::fn_800EBE84() {
    if (mAC == 2) {
        if (mMotion.fn_8008B140()) {
            return 1;
        }
        if (m8C.fn_80473F50()) {
            return 1;
        }
        fn_804776F4(&m8C, &mMotion);
        if ((m54C & 0x4000) != 0x4000) {
            fn_804776F4(&m98, &mMotion);
        }
        mAC = 3;
    }
    return 0;
}

// inferred: parameters of the placement ("pick=", "magicPointId=", "movePointType="); arg is the route table
void BattleCharaTask::vf2C(const char* params, int arg) {
    CharaTask::vf2C(params, arg);
    std::string str(params);
    typedef boost::tokenizer<boost::char_separator<char> > Tokenizer;
    Tokenizer tok(str, boost::char_separator<char>(";"));
    for (Tokenizer::iterator it = tok.begin(); it != tok.end(); ++it) {
        if (strncmp(it->c_str(), "pick=", 5) == 0) {
            std::string s(it->c_str());
            atn::Csv csv(s.c_str() + 5, s.size());
            mC58.m11C.m00 = 2;
            mC58.m11C.m14 = fn_800DC288(csv.fn_8005B9CC());
            mC58.m11C.m18 = fn_800DC288(csv.fn_8005B9CC());
            mC58.m11C.m08 = atoi(csv.fn_8005B9CC());
            mC58.m11C.m0C = atoi(csv.fn_8005B9CC());
            mC58.m11C.m1C = fn_800DC288(csv.fn_8005B9CC());
            mC58.m11C.m10 = atoi(csv.fn_8005B9CC());
            SetFlagC26_unk(0);
        } else if (strncmp(it->c_str(), "magicPointId=", 13) == 0) {
            std::string s(it->c_str());
            atn::Csv csv(s.c_str() + 13, s.size());
            m1464 = ((RouteTable_unk*)arg)->FindPlace_unk(atoi(csv.fn_8005B9CC()));
        } else if (strncmp(it->c_str(), "movePointType=", 14) == 0) {
            std::string s(it->c_str());
            atn::Csv csv(s.c_str() + 14, s.size());
            mRoute.SetA0_unk(atoi(csv.fn_8005B9CC()));
        }
    }
}

int BattleCharaTask::fn_800EE180() {
    if (!mFlagC26) {
        return 0;
    }
    if (!CanAct_unk() || (mType != 0 && mC58.mC0 == 0)) {
        return 0;
    }
    if ((m54C & 0x2000) == 0x2000 || mFlag15) {
        return 0;
    }
    if (mC58.mC4 == 0 || !mC58.mC4->CanAct_unk()) {
        return 0;
    }
    if (mC58.m00 == 14) {
        return 0;
    }
    if ((mStatus.m0C & 0x2000) == 0x2000) {
        return 0;
    }
    if (lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_803761BC() == 0) {
        return 0;
    }
    return 1;
}

// NONMATCHING: 98.7% - the original reloads mItemCount for the slot address of AddItem_unk
void BattleCharaTask::fn_800EE360() {
    if (lbl_8087F610 != 0 && fn_804EB1AC(lbl_8087F610) == 0) {
        return;
    }
    if (mFlag22 != 1 && lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_80370174(0x399) > 0) {
        return;
    }
    if (((unsigned int)m5C->mParam.m9C & 0x4000) == 0x4000) {
        if (BattleResult::sInstance != 0) {
            BattleResult::sInstance->AddItem_unk(m5C->mD0);
        }
    } else if (m1438 != 0 && m1438->IsFlag_unk(4)) {
        m1438->mFlags &= ~4;
        GimmickMsg_unk msg;
        msg.mType = 3;
        msg.m10 = (int)this;
        ((GimmickCommon*)m1438)->vf40(&msg);
    }
}

int BattleCharaTask::vfA4(atn::Vector3* pos, atn::Vector3* rot) {
    if (m1464 != 0) {
        *pos = m1464->mPos;
        rot->Set_unk(0.0f, m1464->m14, 0.0f);
        return 1;
    }
    return 0;
}

int BattleCharaTask::vfE4() {
    if (m55C == 6 && mControl && mControl->vf50()) {
        return 1;
    }
    return 0;
}

void BattleCharaTask::fn_800EE5AC(Arms* arms) {
    if (arms == 0 || arms->m274 == 0) {
        return;
    }
    int ok = 0;
    if (fn_8014FB60()) {
        ok = arms->fn_80044C50(this);
    }
    if (ok) {
        arms->fn_80044CCC();
    } else {
        fn_80044E0C(arms, 1);
    }
}

// inferred: self and chara are on the same side of the same group (self may be null; name invented)
inline bool IsSameGroup_unk(CharaTask* chara, CharaTask* self) {
    if (self != 0) {
        if (!chara->fn_8016F3D0(self, 1)) {
            return false;
        }
        if (self->mC58.mB4 > 0 && chara->mC58.mB4 > 0 && self->mC58.mB4 != chara->mC58.mB4) {
            return false;
        }
    }
    return true;
}

__declspec(noinline) int BattleCharaTask::fn_800EE640(CharaTask* chara) {
    if (chara == 0) {
        return 0;
    }
    if ((chara->mFlags & 4) == 4 || (chara->m54C & 0x2000) == 0x2000 ||
        (chara->mStatus.mParamB.m98 > 0 && chara->mStatus.m224 <= 0) || chara->m5C == 0) {
        return 0;
    }
    if (chara->m55C == 6) {
        switch (chara->m560) {
        case 0x11:
        case 0x12:
        case 0x52:
        case 0x53:
        case 0x58:
        case 0x59:
        case 0x6D:
        case 0x6E:
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x7A:
            return 0;
        }
    }
    return IsSameGroup_unk(chara, this);
}

extern "C" int fn_8021A984(HitInfo_unk* skill); // inferred: the skill heals
extern "C" int fn_8004D388(void* world, RayResult_unk* result, const atn::Vector3* center, const atn::Vector3* move,
                           unsigned int mask, void* ignore, int arg, float radius); // inferred: sweep test

// NONMATCHING: 98.9% - registers and stack slots
// inferred: sends the character to the healing magic `effect` (or to the nearest one, or to a player who can
// heal): state 1 of m1470 with a point 0.7 of the way there put on the ground; returns 1 when it has to move
int BattleCharaTask::fn_800EE794(SkillEffect* effect) {
    if (!CanAct_unk()) {
        return 0;
    }
    atn::Vector3 target(0.0f, 0.0f, 0.0f);
    float radius;
    if (effect == 0 && lbl_8087F9E8 != 0) {
        float best = 1e16f;
        for (int i = 0; i < 32; i++) {
            SkillEffect* bullet = ((SkillEffectManager*)lbl_8087F9E8)->GetBullet_unk(i);
            if (fn_800EE640(bullet->m08)) {
                HitInfo_unk* hit = bullet->m04;
                if (hit != 0 && fn_8021A984(hit) && (hit->mAC & 0x1000000) != 0x1000000) {
                    atn::Vector3 d = bullet->m10 - mPos;
                    float dist = PSVECSquareMag(&d);
                    if (dist < best) {
                        best = dist;
                        effect = bullet;
                    }
                }
            }
        }
        for (int i = 0; i < 32; i++) {
            SkillEffect* bullet = ((SkillEffectManager*)lbl_8087F9E8)->GetBullet_unk(i);
            if (fn_800EE640(bullet->m08)) {
                HitInfo_unk* hit = bullet->m04;
                if (hit != 0 && fn_8021A984(hit) && (hit->mAC & 0x400000) == 0x400000 &&
                    (hit->mAC & 0x1000000) != 0x1000000) {
                    effect = bullet;
                    break;
                }
            }
        }
    }
    if (effect != 0) {
        radius = effect->m5C * effect->m28;
        target = effect->m10;
    } else {
        CharaTask* healer = 0;
        if (lbl_8087F8A0 != 0) {
            float best = 1e16f;
            for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                if (fn_800EE640(p)) {
                    HitInfo_unk* hit = p->mStatus.m2D0[0].m[0] > 0 ? fn_80219E6C(p->mStatus.m2D0[0].m[0]) : 0;
                    if (hit != 0 && fn_8021A984(hit)) {
                        atn::Vector3 d = mPos - p->mPos;
                        float dist = PSVECSquareMag(&d);
                        if (dist < best) {
                            best = dist;
                            healer = p;
                        }
                    }
                }
            }
            if (healer == 0) {
                int ids[4] = {5, 1, 4, 6};
                for (unsigned int i = 0; i < 4; i++) {
                    PlayerTask* p = lbl_8087F8A0->FindByName_unk(ids[i]);
                    if (fn_800EE640(p)) {
                        healer = p;
                        break;
                    }
                }
            }
            if (healer == 0) {
                float best = 1e16f;
                for (PlayerTask* p = lbl_8087F8A0->mHead; p != 0; p = p->mNext) {
                    if (fn_800EE640(p) && (unsigned char)(p->m5C->m122 - 2) <= 1) {
                        atn::Vector3 d = mPos - p->mPos;
                        float dist = PSVECSquareMag(&d);
                        if (dist < best) {
                            best = dist;
                            healer = p;
                        }
                    }
                }
            }
            if (healer == 0) {
                healer = lbl_8087F8A0->mHead;
            }
        }
        if (healer != 0 && healer != this) {
            target = healer->mPos;
            radius = 25.0f;
        } else {
            return 0;
        }
    }
    atn::Vector3 dir(mPos.x - target.x, 0.0f, mPos.z - target.z);
    if (PSVECMag(&dir) > radius) {
        PSVECNormalize(&dir, &dir);
        PlayerState_unk state(1);
        state.m04 = rand() % 10;
        state.m0C = target + dir * radius * 0.7f;
        atn::Vector3 from = state.m0C;
        atn::Vector3 to = from + atn::Vector3(0.0f, -400.0f, 0.0f);
        from.y += 10.0f;
        void* world = lbl_8087EE98;
        RayResult_unk result;
        if (fn_8004ED34(world, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0)) {
            state.m0C = result.m04;
        }
        from = target;
        to = state.m0C;
        from.y += 10.0f;
        to.y += 10.0f;
        world = lbl_8087EE98;
        if (fn_8004ED34(world, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0)) {
            state.m0C = result.m04;
        }
        from = state.m0C;
        to = state.m0C;
        from.y += 10.0f;
        to.y -= 10.0f;
        world = lbl_8087EE98;
        if (fn_8004ED34(world, &result, &from, &to, fn_80179D44() | 0x80000000, 0, 0)) {
            state.m0C = result.m04;
        }
        from = state.m0C;
        from.y += 5.0f;
        atn::Vector3 move(0.0f, 0.0f, 0.0f);
        world = lbl_8087EE98;
        if (fn_8004D388(world, &result, &from, &move, fn_80179D44() | 0x80000000, 0, 0, 5.0f)) {
            state.m0C = result.m10;
            state.m0C.y -= 5.0f;
        }
        SetState_unk(state);
        mC58.m110 = (CharaD68_unk*)effect;
        mC58.m2B8 = 150;
        return 1;
    }
    return 0;
}

int BattleCharaTask::fn_800EEFDC() {
    if (m50 == 0x189C1 && lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->GetCurrentMap() != 0) {
        MapTask* info = ((GameTask*)lbl_8087F430)->GetCurrentMap();
        if (info->mType == 2 && info->mId == 0xD) {
            return 0;
        }
    }
    int ok = 0;
    if (!mFlag28 && !mFlagB16 && lbl_8087F610 == 0) {
        ok = 1;
    }
    return ok;
}

// inferred: defined last in the file (0x800EF3E8, after the weak copies)
void BattleCharaTask::vf10() {
    fn_800E2FE0();
}
