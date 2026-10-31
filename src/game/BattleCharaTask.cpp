#include "game/BattleCharaTask.h"
#include "game/GameConfig_unk.h"
#include "atn/DrawHelper.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/DamageCalc_unk.h"
#include "chr/ControlBase.h"

extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: field (lbl_8087F430); only its address is used here
struct Field_unk {
    void fn_803750E4(int id); // inferred: sets an event (as in game/CharaTask.cpp)
    unsigned char m0000[0x10D0];
    int m10D0; // inferred: story progress (as in game/GameBgm.cpp)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id); // inferred: event flag
extern "C" int fn_80375184(Field_unk* field);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
// inferred: multiplayer session (lbl_8087F610); only tested for null here
extern "C" void* lbl_8087F610;
extern "C" int fn_8021446C(int index, int arg); // inferred: entry of the level equipment table

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

int BattleCharaTask::vfE4() {
    if (m55C == 6 && mControl && mControl->vf50()) {
        return 1;
    }
    return 0;
}

// inferred: defined last in the file (0x800EF3E8, after the weak copies)
void BattleCharaTask::vf10() {
    fn_800E2FE0();
}
