#include "game/BattleCharaTask.h"
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
#include "atn/SoundRef.h"

extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: field (lbl_8087F430); only its address is used here
// inferred: camera of the field (lbl_8087F430 + 0x6C, as in game/FinishCamera_unk.cpp)
struct CameraSub_unk {
    void fn_80389838(int arg);
    unsigned char m000[0x10D0 - 0x6C];
};
struct Field_unk {
    void fn_803750E4(int id); // inferred: sets an event (as in game/CharaTask.cpp)
    int fn_803761BC();
    unsigned char m0000[0x6C];
    CameraSub_unk m006C; // inferred: also the object of fn_8037EF30 (as in game/Em306GurgBeast.cpp)
    int m10D0; // inferred: story progress (as in game/GameBgm.cpp)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id); // inferred: event flag
extern "C" int fn_80375184(Field_unk* field);
extern "C" void fn_804776F4(atn::LH_MotionSet* set, atn::Model* model); // inferred: applies the motions
// inferred: map information of the field (as in gmk/FragileObject.cpp)
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
};
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);
// inferred: effect manager (lbl_8087F048, as in game/CharaTask.cpp)
struct Effect_unk;
extern "C" Effect_unk* lbl_8087F048;
// inferred: target in front of the character within radius/theta
extern "C" CharaTask* fn_800FFE68(Effect_unk* mgr, CharaTask* chara, const atn::Vector3* dir, int a, int b,
                                  float radius, float theta);
extern "C" void fn_80105BD8(Effect_unk* mgr, atn::Model* model); // inferred: death effect of a model
extern "C" atn::SoundRef fn_800C344C(const char* name, const void* pos, int arg2, int arg3, float volume);
// inferred: target searches of the effect manager (in front / around)
extern "C" int fn_801004FC(Effect_unk* mgr, CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range,
                           float theta);
extern "C" int fn_80100BB8(Effect_unk* mgr, CharaTask** out, CharaTask* chara, const atn::Vector3* dir, float range,
                           float theta);
extern "C" double cos(double x);
extern "C" float PSVECDotProduct(const atn::Vector3* a, const atn::Vector3* b);
extern "C" int fn_800F8548(Effect_unk* mgr);
extern "C" atn::Vector3 lbl_807C7030; // inferred: zero vector
extern "C" int fn_80041A28(int arg, CharaTask* chara, CharaTask* target); // inferred: kind of attack
extern "C" void fn_8037EF30(void* obj, int a, int b, float f);
extern "C" void fn_80370320(Field_unk* field, int event, int arg2, int arg3); // inferred: starts an event
// inferred: target found by fn_800E7214 with its position (only the character is filled here)
struct BattleTarget_unk {
    BattleTarget_unk() : mChara(0), m04(0), m08(0), mPos(0.0f, 0.0f, 0.0f) {}
    CharaTask* mChara;
    int m04;
    int m08;
    atn::Vector3 mPos;
};
extern "C" int fn_80219344(CharaTask* chara, int arg);
extern "C" int fn_8036DAA8(Field_unk* field, CharaTask* chara); // inferred: the special skill is allowed here
extern "C" void* lbl_8087EE68; // inferred: party manager (as in game/CharaTask.cpp)
// inferred: result of fn_800185B4 (as in game/CharaTask.cpp)
struct MagicTarget_unk;
extern "C" MagicTarget_unk* fn_800185B4(void* manager, CharaTask* chara, HitInfo_unk* hit);
extern "C" int fn_803754F0(Field_unk* field); // inferred: an event scene is running
extern "C" int fn_80375D0C(Field_unk* field);  // inferred: level cap of the field
// inferred: player manager (lbl_8087F8A0, as in game/CharaTask.cpp)
struct PlayerManager_unk {
    unsigned char m00[0x48];
    CharaTask* mHead;
};
extern "C" PlayerManager_unk* lbl_8087F8A0;
// inferred: experience of a kill (as in gmk/FlyObjManager.cpp)
extern "C" void fn_804491F4(int* a, int* b, int level, int exp, int arg4, float rate);
extern "C" void fn_80449448(int* a, int* b, int level, int cap);
extern "C" void fn_80449A18(void* mgr, int killer, int level, int exp, int arg4, float rate);
// inferred: popup texts (lbl_8087F8A8, as in gmk/FlyObjManager.cpp)
extern "C" void* lbl_8087F8A8;
extern "C" void fn_8054D794(void* obj, int arg1, const wchar_t* text, const atn::Vector3* pos, int arg4, int arg5,
                            int arg6, int arg7, int arg8, int arg9, int arg10, int arg11, int arg12);
extern "C" int swprintf(wchar_t* s, unsigned long n, const wchar_t* format, ...);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound
extern "C" void fn_80108A54(Effect_unk* mgr, CharaTask* chara); // inferred: lock-on cursor
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
      m1474(0), m1478(150), m147C(0.0f), m1480(0.0f), m1484(0.0f), m1488(0), m149C(0.0f), m14A0(0), m14A4(0) {
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

// inferred: the HUD (lbl_8087F490, game/GameUI.h); only passed to fn_803E2110 here
struct GameUI;
extern "C" GameUI* lbl_8087F490;
extern "C" void fn_803E2110(GameUI* ui, int arg, CharaTask* chara); // inferred: HUD notice (3: low hit points)
extern "C" int fn_80370A78(Field_unk* field, int id);
extern "C" void fn_80370AE4(Field_unk* field, int id, int value);

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
            } else if (m50 == 0x30ED4 && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x11B) > 0) {
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

void BattleCharaTask::vf14() {
    fn_80149A30();
    if (lbl_8087F0A8->mDrawCharaColli != 0) {
        lbl_8087EEB0->fn_80063484(20, 0xFFFF0000, mPos.x, 3.0f + mPos.y, mPos.z, mRot.y, 0.0f, 12.0f, 1.5707964f);
    }
}

// NONMATCHING: 99.96% - stack slots of the second and third rotation matrices swapped (~8 forms)
void BattleCharaTask::fn_800E610C() {
    atn::Vector3 input(0.0f, 0.0f, 1.0f);
    Mtx m;
    PSMTXRotRad(m, 'y', fn_80178018().y);
    PSMTXMultVec(m, &input, &input);
    atn::Vector3 front(0.0f, 0.0f, 1.0f);
    Mtx m2;
    PSMTXRotRad(m2, 'y', mRot.y);
    PSMTXMultVec(m2, &front, &front);
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

int BattleCharaTask::fn_800E70EC(int a, int b, int c) {
    if (m1424 == 0 || a != 0) {
        GameConfig_unk* config = lbl_8087F0A8;
        if (SetConfig::sInstance->mAttackType != 0 && !mFlagB16 && c == 0 && config->mInput.fn_801231D0(4) &&
            !mFlag31) {
            if (a == 0) {
                Effect_unk* mgr = lbl_8087F048;
                atn::Vector3 dir = fn_80178018();
                CharaTask* target =
                    fn_800FFE68(mgr, this, &dir, 0, 0, config->mBtnAtkCorrectRadius, config->mBtnAtkCorrectTheta);
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
        if (fn_80100BB8(lbl_8087F048, out, this, &dir, config->m248, config->m24C)) {
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
        fn_8037EF30(obj, 0, 1, -1.0f);
        mFC0 = 0;
    }
    if (lbl_8087F0A8->mInput.fn_801240B4(0x12) && !mFlagB9) {
        if (!mFlag31) {
            fn_80155DAC(1);
        }
    } else if ((lbl_8087F0A8->mInput.fn_801240B4(7) || mFlagB16) && (!mFlag31 || !mFlag30) && !mFlagB9) {
        fn_800C344C(sBattleSe_unk[0], &mPos, 0, -1, 1.0f);
        if (fn_80370174(lbl_8087F430, 0xEF) == 0 && fn_80370174(lbl_8087F430, 0xEE) == 1) {
            fn_80370320(lbl_8087F430, 0xEF, 1, 0);
        }
        if (mFC0 == 0) {
            if (mFC4 != 0) {
                mFC0 = mFC4;
            } else {
                Effect_unk* mgr = lbl_8087F048;
                atn::Vector3 dir = fn_80178018();
                mFC0 = fn_800FFE68(mgr, this, &dir, 0, 1, config->mLockOnLen, config->mLockOnTheta);
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

void BattleCharaTask::fn_800E7650(Field_unk* field) {
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
            fn_8037EF30(obj, 0, 1, -1.0f);
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
            fn_80108A54(lbl_8087F048, this);
        }
        mFlag4 = 1;
        if (!lbl_8087F0A8->mInput.fn_801240B4(7)) {
            mFlag4 = 0;
        }
    } else {
        mFC0 = 0;
        mFlag4 = 0;
        if (lbl_8087F0A8->mInput.fn_801231D0(0x11)) {
            fn_8037EF30(obj, 0, 1, -1.0f);
            mFC0 = 0;
        } else if (lbl_8087F0A8->mInput.fn_801240B4(7)) {
            fn_800C344C(sBattleSe_unk[0], &mPos, 0, -1, 1.0f);
            if (fn_80370174(lbl_8087F430, 0xEF) == 0 && fn_80370174(lbl_8087F430, 0xEE) == 1) {
                fn_80370320(lbl_8087F430, 0xEF, 1, 0);
            }
            if (mFC0 == 0) {
                Effect_unk* mgr = lbl_8087F048;
                atn::Vector3 dir = fn_80178018();
                mFC0 = fn_800FFE68(mgr, this, &dir, 0, 1, config->mLockOnLen, config->mLockOnTheta);
            }
            mFlag4 = 1;
            fn_8037EF30(obj, 0, 1, -1.0f);
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
                MagicTarget_unk* target = lbl_8087EE68 != 0 ? fn_800185B4(lbl_8087EE68, this, hit) : 0;
                if (lbl_8087F0A8->mAutoTensionSkillFriend != 0 || target != 0) {
                    ok = 1;
                }
            }
        } else {
            if (mType == 0 || (mC58.mC0 > 0 && mC58.mC4 != 0)) {
                HitInfo_unk* hit = fn_80219E6C(skill);
                MagicTarget_unk* target = lbl_8087EE68 != 0 ? fn_800185B4(lbl_8087EE68, this, hit) : 0;
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
            if (lbl_8087F430 != 0 && fn_8036DAA8(lbl_8087F430, this)) {
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
        fn_80375184(lbl_8087F430);
    }
    m1470 = 0;
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
        lbl_8087F430->m006C.fn_80389838(3);
    }
    fn_8015F568(pos, 0, hit);
    mFC0 = 0;
    mFlag4 = 0;
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
                if (lbl_8087F430 == 0 || fn_80370174(lbl_8087F430, 0xAC) == 0) {
                    m1450--;
                }
                if (m1450 == 0) {
                    HitInfo_unk* data = fn_80219E6C(0x2711);
                    if (!mStatus.IsState_unk(0x20)) {
                        mStatus.fn_8013310C(0x20, 0);
                    }
                    DamageResult_unk result;
                    fn_8003EA3C(&result, data, this, this, 0, &lbl_807C6B90, 0, 0);
                    if (mType == 3 && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 5) == 0) {
                        lbl_8087F430->fn_803750E4(0x1B7);
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
                    fn_80105BD8(lbl_8087F048, &mMotion);
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
            fn_803E2110(lbl_8087F490, 3, this);
        }
        if (fn_80370174(lbl_8087F430, 0x38A) == 1 && msg->mAttacker != 0 && msg->mAttacker->mType == 0 &&
            fn_80370A78(lbl_8087F430, 0xFA) == 0) {
            Field_unk* field = lbl_8087F430;
            fn_80370AE4(field, 0xFA, fn_80219558(m50));
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

// inferred: experience of the kill (popup text and the inventory)
void BattleCharaTask::fn_800EB7A0() {
    if (lbl_8087F430 != 0 && fn_803754F0(lbl_8087F430) != 0) {
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
            cap = fn_80375D0C(lbl_8087F430);
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
                lbl_8087F430->fn_803750E4(0xEB);
            }
        }
        if (fn_8017CB2C()) {
            kind = 2;
            rate *= lbl_8087F0A8->mSummonExpBounus;
        }
        if (lbl_8087F8A8 != 0) {
            wchar_t text[0x40];
            swprintf(text, 0x20, L"%d", a);
            fn_8054D794(lbl_8087F8A8, 2, text, &pos, kind, 0, 1, 0, 0, 0, 1, -1, -1);
        }
        fn_80449A18(Inventory::instance_, m13B0, level, exp, 0, rate);
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
    if (lbl_8087F430 != 0 && lbl_8087F430->fn_803761BC() == 0) {
        return 0;
    }
    return 1;
}

// NONMATCHING: 98.7% - the original reloads mItemCount for the slot address of AddItem_unk
void BattleCharaTask::fn_800EE360() {
    if (lbl_8087F610 != 0 && fn_804EB1AC(lbl_8087F610) == 0) {
        return;
    }
    if (mFlag22 != 1 && lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x399) > 0) {
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

int BattleCharaTask::fn_800EE640(CharaTask* chara) {
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

int BattleCharaTask::fn_800EEFDC() {
    if (m50 == 0x189C1 && lbl_8087F430 != 0 && fn_80373148(lbl_8087F430) != 0) {
        MapInfo_unk* info = fn_80373148(lbl_8087F430);
        if (info->m48 == 2 && info->m4C == 0xD) {
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
