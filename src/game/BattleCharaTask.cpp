#include "game/BattleCharaTask.h"
#include "game/GameConfig_unk.h"
#include "atn/DrawHelper.h"
#include "game/GimmickManager.h"
#include "game/GimmickCommon.h"
#include "game/DamageCalc_unk.h"
#include "chr/ControlBase.h"
#include "game/Arms.h"
#include "game/BattleResult.h"

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
extern "C" void fn_804776F4(atn::LH_MotionSet* set, atn::Model* model); // inferred: applies the motions
// inferred: map information of the field (as in gmk/FragileObject.cpp)
struct MapInfo_unk {
    unsigned char m00[0x48];
    int m48;
    int m4C;
};
extern "C" MapInfo_unk* fn_80373148(Field_unk* field);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task
// inferred: multiplayer session (lbl_8087F610)
struct MpSession_unk;
extern "C" MpSession_unk* lbl_8087F610;
extern "C" int fn_804EB1AC(MpSession_unk* session);
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
