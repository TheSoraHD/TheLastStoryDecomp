#include "game/BattleCharaTask.h"
#include "game/GameConfig_unk.h"
#include "atn/DrawHelper.h"
#include "game/GimmickManager.h"
#include "game/DamageCalc_unk.h"
#include "chr/ControlBase.h"

extern "C" GameConfig_unk* lbl_8087F0A8;
// inferred: field (lbl_8087F430); only its address is used here
struct Field_unk {
    void fn_803750E4(int id); // inferred: sets an event (as in game/CharaTask.cpp)
};
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370174(Field_unk* field, int id); // inferred: event flag
extern "C" int fn_80375184(Field_unk* field);
extern "C" void fn_800D2338(atn::Task* task); // inferred: deletes the task

BattleCharaTask::~BattleCharaTask() {
    if (m1438 != 0 && GimmickManager::instance_ != 0) {
        fn_800D2338(m1438);
    }
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

int BattleCharaTask::vfE4() {
    if (m55C == 6 && mControl && mControl->vf50()) {
        return 1;
    }
    return 0;
}
