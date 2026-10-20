#include "game/BattleResult.h"
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

// inferred: CharaStatus (game/CharaStatus.h); only the members used here
class CharaStatus {
public:
    int fn_801334CC(int level); // inferred: experience needed for `level`
    unsigned char m00[0xA0];
    int mLevel; // 0xA0 (CharaParam_unk::m78 of mParamA)
    unsigned char mA4[0x1AC - 0xA4];
    int mExp; // 0x1AC
};
// inferred: character table entry (CharaTask::m60)
struct ResultCharaInfo_unk {
    unsigned char m00[0x08];
    const char* mName; // 0x08
};
// inferred: CharaTask; only the members used here
struct ResultChara_unk {
    unsigned char m00[0x60];
    ResultCharaInfo_unk* mInfo; // 0x60
    unsigned char m64[0x7D4 - 0x64];
    CharaStatus mStatus; // 0x7D4
};
// inferred: one party member of the battle (0x90 bytes)
struct ResultMember_unk {
    ResultChara_unk* mChara;
    unsigned char m04[0x80 - 0x04];
    int mExpA; // 0x80: inferred: experience won
    int mExpB; // 0x84
    unsigned char m88[0x8C - 0x88];
    int mLevelUp; // 0x8C: inferred: levels won
};
// inferred: party data (lbl_8087F4F0); only the members used here
struct ResultParty_unk {
    unsigned char m0000[0x601C];
    atn::FixedArray_unk<ResultMember_unk, 7> mMembers; // 0x601C
};
extern "C" ResultParty_unk* lbl_8087F4F0;
extern "C" int fn_80444020(ResultParty_unk* party, int item, int a, int b, int c, int d, int* outA, int* outB,
                           int* outC); // inferred: adds an item
extern "C" void fn_804439FC(ResultParty_unk* party, int a, int b, int c, int d, int e, int f, int g);
extern "C" void fn_80444CF8(char* text, int a, int b, int c); // inferred: item name
// inferred: field (lbl_8087F430)
struct ResultField_unk {
    unsigned char m0000[0x54E4];
    int m54E4; // inferred: state
};
extern "C" ResultField_unk* lbl_8087F430;
extern "C" int fn_80370174(ResultField_unk* field, int id); // inferred: event flag
extern "C" void* lbl_8087F490;                               // inferred: GameUI
extern "C" void fn_803E5E64(void* ui, int a, int b, int c);  // inferred: shows an item
// inferred: LogMessageTask (game/LogMessageTask.h); only the entry count
struct ResultLog_unk {
    unsigned char m00[0x48];
    int mCount;
};
class LogMessageTask {
public:
    static ResultLog_unk* sInstance;
};

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
        sInstance = new (1, "", "", 0) BattleResult(parent);
    }
    return sInstance;
}

// NONMATCHING: 99.54% - register allocation only: the original keeps the last path in the register of the
// string pool (r30) and the loop offset in r26; with named locals the two are swapped, without them the
// paths are not hoisted out of the loop
BattleResult::BattleResult(atn::Task* parent)
    : atn::Task(parent), mState(0), mCount(0), mTimer(0), mRate(0.0f), mItemCount(0) {
    (mBase = fn_801F3FF8(this, "/data/d2anime/ui_result_name_op.d2a", 0))->fn_800D246C(1);
    const char* expOp = "/data/d2anime/ui_result_exp_op.d2a";
    const char* expLoop = "/data/d2anime/ui_result_exp_loop.d2a";
    const char* lvUpOp = "/data/d2anime/ui_result_LvUP_op_xx.d2a";
    const char* lvUpLoop = "/data/d2anime/ui_result_LvUP_loop_xx.d2a";
    for (unsigned int i = 0; i < 7; i++) {
        (mExpOp[i] = fn_801F64D0(this, expOp))->fn_800D246C(1);
        (mExpLoop[i] = fn_801F64D0(this, expLoop))->fn_800D246C(1);
        (mLvUpOp[i] = fn_801F64D0(this, lvUpOp))->fn_800D246C(1);
        (mLvUpLoop[i] = fn_801F64D0(this, lvUpLoop))->fn_800D246C(1);
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
                fn_803E5E64(lbl_8087F490, mItemA, mItemB, mItemC);
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
    atn::FixedArray_unk<ResultMember_unk, 7>* members = &lbl_8087F4F0->mMembers;
    mCount = members->size();
    for (int i = 0; i < members->size(); i++) {
        mExpOp[i]->mFrame = 0.0f;
        mExpLoop[i]->mFrame = 0.0f;
        if (members->mData[i].mLevelUp > 0) {
            mLvUpOp[i]->mFrame = 0.0f;
            mLvUpLoop[i]->mFrame = 0.0f;
            mLvUp[i] = 1;
        }
        ResultChara_unk* chara = members->mData[i].mChara;
        CharaStatus* status = &chara->mStatus;
        int won = members->mData[i].mExpA + members->mData[i].mExpB;
        ResultExp_unk exp;
        if (members->mData[i].mLevelUp > 0) {
            int need = status->fn_801334CC(status->mLevel - 1);
            if (members->mData[i].mLevelUp == 1) {
                int prev = status->mExp - won;
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
        } else if (status->mLevel >= 99) {
            exp.mFrom = 0;
            exp.mTo = 0;
            exp.mRateFrom = 0.0f;
            exp.mRateTo = 0.0f;
        } else {
            int need = status->fn_801334CC(status->mLevel);
            int now = status->mExp;
            int left = need - now;
            exp.mRateFrom = (float)(now - won) / (float)need;
            exp.mRateTo = (float)now / (float)need;
            exp.mFrom = won + left;
            exp.mTo = left;
        }
        mExp.push_back(exp);
        fn_801F837C(mExpOp[i], "name", chara->mInfo->mName);
        fn_801F837C(mExpLoop[i], "name", chara->mInfo->mName);
        fn_801F837C(mExpLoop[i], "name_sdw", chara->mInfo->mName);
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
        if (lbl_8087F4F0 != 0) {
            int a;
            int b;
            int c = 1;
            if (fn_80444020(lbl_8087F4F0, mItems[j], 1, -1, 0, 0, &a, &b, &c)) {
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
                    fn_803E5E64(lbl_8087F490, a, b, c);
                }
                mHasItems = 1;
                shown++;
            }
        }
    }
    if (lbl_8087F430 != 0 && fn_80370174(lbl_8087F430, 0x11B) > 0 && mItemCount == 1) {
        fn_804439FC(lbl_8087F4F0, 0x143, 5, 1, 0, 0, 1, 1);
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
