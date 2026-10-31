#include "game/GameUI.h"
#include "game/BattleResult.h"
#include "game/Inventory.h"
#include "game/CharaStatus.h"
#include "game/CharaTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "string.h"

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" ResultD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int loop); // inferred: d2anime task
extern "C" ResultLayout_unk* fn_801F64D0(atn::Task* parent, const char* path);      // inferred: layout anime
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value);      // inferred: sets a value
extern "C" void fn_801F6D7C(atn::Task* layout, const char* name, int index, float value);
extern "C" void fn_801F837C(atn::Task* layout, const char* name, const char* text); // inferred: sets a text
extern "C" void fn_801F8598(atn::Task* layout, const char* name, int value, int arg); // inferred: sets a number
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
extern "C" char lbl_8087D970[4]; // inferred: empty text
// inferred: sets the value `index` of the pane `name` (name invented, as in game/MenuCommonTask.cpp)
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}

extern "C" int fn_80444020(Inventory* party, int item, int a, int b, int c, int d, int* outA, int* outB,
                           int* outC); // inferred: adds an item
extern "C" void fn_80444CF8(char* text, int a, int b, int c); // inferred: item name
// inferred: field (lbl_8087F430)
struct ResultField_unk {
    unsigned char m0000[0x54E4];
    int m54E4; // inferred: state
};
extern "C" ResultField_unk* lbl_8087F430;
extern "C" int fn_80370174(ResultField_unk* field, int id); // inferred: event flag
// inferred: LogMessageTask (game/LogMessageTask.h); only the entry count
struct ResultLog_unk {
    unsigned char m00[0x48];
    int mCount;
};
class LogMessageTask {
public:
    static ResultLog_unk* sInstance;
};

// inferred: wrapper of fn_801F64D0 (the paths given to it are hoisted out of the loops); name invented
inline ResultLayout_unk* CreateLayout_unk(atn::Task* parent, const char* path) {
    return fn_801F64D0(parent, path);
}

// inferred: plays `anime`; at its end it shows `next` instead (name invented)
inline void Play_unk(ResultLayout_unk* anime, ResultLayout_unk* next, float speed) {
    anime->mSpeed = speed;
    if (anime->IsEnd_unk() && next != 0) {
        next->mFlags &= ~4;
    } else {
        anime->mFlags &= ~4;
    }
}

// inferred: linear interpolation (name invented)
inline float Lerp_unk(float a, float b, float t) {
    return t * (b - a) + a;
}

BattleResult* BattleResult::sInstance;

BattleResult* BattleResult::fn_800EFF68(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) BattleResult(parent);
    }
    return sInstance;
}

BattleResult::BattleResult(atn::Task* parent)
    : atn::Task(parent), mState(0), mCount(0), mTimer(0), mRate(0.0f), mItemCount(0) {
    (mBase = fn_801F3FF8(this, "/data/d2anime/ui_result_name_op.d2a", 0))->fn_800D246C(1);
    for (unsigned int i = 0; i < 7; i++) {
        (mExpOp[i] = CreateLayout_unk(this, "/data/d2anime/ui_result_exp_op.d2a"))->fn_800D246C(1);
        (mExpLoop[i] = CreateLayout_unk(this, "/data/d2anime/ui_result_exp_loop.d2a"))->fn_800D246C(1);
        (mLvUpOp[i] = CreateLayout_unk(this, "/data/d2anime/ui_result_LvUP_op_xx.d2a"))->fn_800D246C(1);
        (mLvUpLoop[i] = CreateLayout_unk(this, "/data/d2anime/ui_result_LvUP_loop_xx.d2a"))->fn_800D246C(1);
    }
    (mItemOp = fn_801F64D0(this, "/data/d2anime/ui_result_item_op.d2a"))->fn_800D246C(1);
    (mItemLoop = fn_801F64D0(this, "/data/d2anime/ui_result_item_loop.d2a"))->fn_800D246C(1);
    memset(mLvUp, 0, sizeof(mLvUp));
    mExp.mSize = 0;
    mItemA = -1;
    mItemB = 0;
    mItemC = 1;
}

BattleResult::~BattleResult() {
    sInstance = 0;
}

int BattleResult::vf0C() {
    if (fn_800D3FA4()) {
        mFlags |= 4;
        mBase->fn_800D246C(0);
        mBase->mRepeat = 0;
        mBase->mFlags |= 4;
        for (unsigned int i = 0; i < 7; i++) {
            fn_801F6C80(mExpOp[i], "line", i);
            mExpOp[i]->fn_800D246C(0);
            mExpOp[i]->mRepeat = 0;
            mExpOp[i]->mFlags |= 4;
            fn_801F6C80(mExpLoop[i], "line", i);
            mExpLoop[i]->fn_800D246C(0);
            mExpLoop[i]->mRepeat = 1;
            mExpLoop[i]->mFlags |= 4;
            fn_801F6C80(mLvUpOp[i], "line", i);
            mLvUpOp[i]->fn_800D246C(0);
            mLvUpOp[i]->mRepeat = 0;
            mLvUpOp[i]->mFlags |= 4;
            fn_801F6C80(mLvUpLoop[i], "line", i);
            mLvUpLoop[i]->fn_800D246C(0);
            mLvUpLoop[i]->mFlags |= 4;
        }
        mItemOp->fn_800D246C(0);
        mItemOp->mRepeat = 0;
        mItemOp->mFlags |= 4;
        mItemLoop->fn_800D246C(0);
        mItemLoop->mRepeat = 1;
        mItemLoop->mFlags |= 4;
        mItemCount = 0;
        return 1;
    }
    return 0;
}

void BattleResult::vf10() {
    mBase->mFlags |= 4;
    mItemOp->mFlags |= 4;
    mItemLoop->mFlags |= 4;
    for (unsigned int i = 0; i < 7; i++) {
        mExpOp[i]->mFlags |= 4;
        mExpLoop[i]->mFlags |= 4;
        mLvUpOp[i]->mFlags |= 4;
        mLvUpLoop[i]->mFlags |= 4;
    }
    if (lbl_8087F430 != 0) {
        int state = lbl_8087F430->m54E4;
        if (state == 8 || state == 1 || state == 13 || state == 7 || state == 14) {
            return;
        }
    }
    switch (mState) {
    case 0:
        mBase->mSpeed = 1.0f;
        mBase->mFlags &= ~4;
        if (mBase->IsEnd_unk()) {
            mState = 1;
            mTimer = 300;
            mRate = 0.0f;
        }
        for (unsigned int i = 0; i < mExp.size(); i++) {
            Play_unk(mExpOp[i], mLvUp[i] ? mExpLoop[i] : 0, 1.0f);
        }
        if (mHasItems) {
            Play_unk(mItemOp, mItemLoop, 1.0f);
        }
        break;
    case 1:
        mBase->mSpeed = 1.0f;
        mBase->mFlags &= ~4;
        for (unsigned int i = 0; i < mExp.size(); i++) {
            Play_unk(mExpOp[i], mLvUp[i] ? mExpLoop[i] : 0, 1.0f);
            if (mLvUp[i]) {
                Play_unk(mLvUpOp[i], mLvUpLoop[i], 1.0f);
            }
        }
        if (mHasItems) {
            Play_unk(mItemOp, mItemLoop, 1.0f);
            if (mItemA >= 0 && mTimer < 270) {
                lbl_8087F490->fn_803E5E64(mItemA, mItemB, mItemC);
                mItemA = -1;
                mItemB = 0;
                mItemC = 1;
            }
        }
        {
            float rate = 1.0f + mRate;
            mRate = 15.0f < rate ? 15.0f : rate;
        }
        if (--mTimer < 0) {
            int state = lbl_8087F430->m54E4;
            if (state != 9 && state != 10) {
                if (mMode != 1) {
                    mState = 2;
                    mBase->mFrame = mBase->mLastFrame;
                    mBase->mSpeed = -1.0f;
                    for (unsigned int i = 0; i < mCount; i++) {
                        mExpOp[i]->mFrame = fn_801F6C2C(mExpOp[i]) - 1.0f;
                        mExpOp[i]->mSpeed = -1.0f;
                    }
                }
                mItemOp->mFrame = fn_801F6C2C(mItemOp) - 1.0f;
                mItemOp->mSpeed = -1.0f;
            }
        }
        break;
    case 2:
        mBase->mSpeed = -1.0f;
        mBase->mFlags &= ~4;
        for (unsigned int i = 0; i < mExp.size(); i++) {
            Play_unk(mExpOp[i], mLvUp[i] ? mExpLoop[i] : 0, -1.0f);
            if (mLvUp[i]) {
                Play_unk(mLvUpOp[i], mLvUpLoop[i], -1.0f);
            }
        }
        if (mHasItems) {
            Play_unk(mItemOp, mItemLoop, -1.0f);
        }
        if (mBase->mFrame <= 0.0f) {
            fn_800F1378();
        }
        break;
    }
}

void BattleResult::vf14() {
    float t = mRate / 15.0f;
    for (unsigned int i = 0; i < mExp.size(); i++) {
        ResultExp_unk* exp = &mExp.mData[i];
        int value = Lerp_unk((float)exp->mFrom, (float)exp->mTo, t);
        float rate = t * (exp->mRateTo - exp->mRateFrom) + exp->mRateFrom;

        if (mExpOp[i] != 0) {
            fn_801F8598(mExpOp[i], "nowExp", value, 0);
            fn_801F6C80(mExpOp[i], "expRate", rate);
        }
        if (mExpLoop[i] != 0) {
            fn_801F8598(mExpLoop[i], "nowExp", value, 0);
            fn_801F6C80(mExpLoop[i], "expRate", rate);
        }
    }
}

// NONMATCHING: 99.33% - register allocation only (loop counters/iterators of the base_point, member and
// item loops, chara/won pair swapped)
void BattleResult::fn_800F0D14(int mode) {
    LogMessageTask::sInstance->mCount = 0;
    mExp.mSize = 0;
    fn_800C31F4("SE_UI001_010", 0, -1, 1.0f);
    if (IsFlag_unk(4)) {
        mFlags &= ~4;
    }
    mState = 0;
    mBase->mFlags &= ~4;
    mBase->mFrame = 0.0f;
    mBase->mSpeed = 1.0f;
    memset(mLvUp, 0, sizeof(mLvUp));
    mMode = mode;
    float base = 0.0f;
    float line = -2.0f;
    if (mode == 1) {
        line = base = -20.0f;
    }
    SetPaneValue_unk(mBase->m58, "base_point", 4, base);
    for (unsigned int i = 0; i < 7; i++) {
        fn_801F6D7C(mExpOp[i], "base_point", 4, line);
        fn_801F6D7C(mExpLoop[i], "base_point", 4, line);
        fn_801F6D7C(mLvUpOp[i], "base_point", 4, line);
    }
    atn::FixedArray_unk<Inventory::ExpEntry_unk, 7>* members = &Inventory::instance_->mExpEntries;
    mCount = members->size();
    for (int i = 0; i < members->size(); i++) {
        mExpOp[i]->mFrame = 0.0f;
        mExpLoop[i]->mFrame = 0.0f;
        if (members->mData[i].mLevelUp > 0) {
            mLvUpOp[i]->mFrame = 0.0f;
            mLvUpLoop[i]->mFrame = 0.0f;
            mLvUp[i] = 1;
        }
        CharaTask* chara = members->mData[i].mChara;
        CharaStatus* status = &chara->mStatus;
        int won = members->mData[i].mExpA + members->mData[i].mExpB;
        ResultExp_unk exp;
        if (members->mData[i].mLevelUp > 0) {
            int need = status->fn_801334CC(status->mParamA.m78 - 1);
            if (members->mData[i].mLevelUp == 1) {
                int prev = status->m1A8.mExp - won;
                exp.mRateTo = 1.0f;
                exp.mFrom = prev;
                exp.mTo = 0;
                exp.mRateFrom = (float)(need + prev) / (float)need;
            } else {
                exp.mFrom = need;
                exp.mTo = 0;
                exp.mRateFrom = 0.0f;
                exp.mRateTo = 1.0f;
            }
        } else if (status->mParamA.m78 >= 99) {
            exp.mFrom = 0;
            exp.mTo = 0;
            exp.mRateFrom = 0.0f;
            exp.mRateTo = 0.0f;
        } else {
            int need = status->fn_801334CC(status->mParamA.m78);
            int now = status->m1A8.mExp;
            int left = need - now;
            exp.mRateFrom = (float)(now - won) / (float)need;
            exp.mRateTo = (float)now / (float)need;
            exp.mFrom = won + left;
            exp.mTo = left;
        }
        mExp.push_back(exp);
        fn_801F837C(mExpOp[i], "name", chara->m60->m08);
        fn_801F837C(mExpLoop[i], "name", chara->m60->m08);
        fn_801F837C(mExpLoop[i], "name_sdw", chara->m60->m08);
    }
    float count = members->size();
    mHasItems = 0;
    mItemOp->mFrame = 0.0f;
    fn_801F6C80(mItemOp, "line", count);
    fn_801F837C(mItemOp, "item00", lbl_8087D970);
    fn_801F837C(mItemOp, "item01", lbl_8087D970);
    mItemLoop->mFrame = 0.0f;
    fn_801F6C80(mItemLoop, "line", count);
    fn_801F837C(mItemLoop, "item00", lbl_8087D970);
    fn_801F837C(mItemLoop, "item01", lbl_8087D970);
    mItemA = -1;
    mItemB = 0;
    mItemC = 1;
    int shown = 0;
    for (int j = 0; j < mItemCount; j++) {
        if (Inventory::instance_ != 0) {
            int a;
            int b;
            int c = 1;
            if (fn_80444020(Inventory::instance_, mItems[j], 1, -1, 0, 0, &a, &b, &c)) {
                char name[16];
                sprintf(name, "item%02d", j);
                char text[0x80];
                memset(text, 0, sizeof(text));
                fn_80444CF8(text, a, b, c);
                fn_801F837C(mItemOp, name, text);
                fn_801F837C(mItemLoop, name, text);
                if (shown >= 1) {
                    mItemA = a;
                    mItemB = b;
                    mItemC = c;
                } else {
                    lbl_8087F490->fn_803E5E64(a, b, c);
                }
                mHasItems = 1;
                shown++;
            }
        }
    }
    if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x11B) > 0 && mItemCount == 1) {
        Inventory::instance_->AddItem(0x143, 5, 1, 0, 0, 1, 1);
        char text[0x80];
        memset(text, 0, sizeof(text));
        fn_80444CF8(text, 0x143, 5, 1);
        fn_801F837C(mItemOp, "item01", text);
        fn_801F837C(mItemLoop, "item01", text);
        shown++;
    }
    fn_801F6C80(mItemOp, "multiItem", shown >= 2 ? 1.0f : 0.0f);
    fn_801F6C80(mItemLoop, "multiItem", shown >= 2 ? 1.0f : 0.0f);
    mItemCount = 0;
}

void BattleResult::fn_800F1378() {
    mState = 3;
    mRate = 0.0f;
    mHasItems = 0;
    if (!IsFlag_unk(4)) {
        mFlags |= 4;
    }
}

void BattleResult::fn_800F13B0() {
    mItemCount = 0;
}
