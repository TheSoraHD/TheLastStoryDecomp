#ifndef GAME_BATTLERESULT_H
#define GAME_BATTLERESULT_H

#include "atn/Task.h"
#include "atn/FixedArray.h"

// inferred: d2anime task (made by fn_801F3FF8); only the members used here
struct ResultD2A_unk : public atn::Task {
    // inferred: as MenuD2A_unk::IsEnd_unk (name invented)
    bool IsEnd_unk() {
        float last = mLastFrame;
        return mFrame >= last;
    }
    unsigned char m48[0x58 - 0x48];
    unsigned char m58[0xA0 - 0x58]; // 0x58: inferred: layout (fn_801FED24)
    float mLastFrame;               // 0xA0
    unsigned char mA4[0xFC - 0xA4];
    unsigned int mLoop : 1; // 0xFC (as atn::D2A_Task)
    unsigned int mVisible : 1;
    unsigned int mPlaying : 1;
    unsigned int mRepeat : 1;
    unsigned int mFC_rest : 28;
    float mFrame; // 0x100
    float mSpeed; // 0x104
};

// inferred: layout anime task (made by fn_801F64D0); only the members used here
struct ResultLayout_unk : public atn::Task {
    bool IsEnd_unk(); // inferred: the frame reached the last one (name invented)
    unsigned char m48[0x4D - 0x48];
    bool mRepeat; // 0x4D
    unsigned char m4E[0x50 - 0x4E];
    float mFrame; // 0x50
    float mSpeed; // 0x54
};

extern "C" float fn_801F6C2C(atn::Task* layout); // inferred: last frame

inline bool ResultLayout_unk::IsEnd_unk() {
    return mFrame >= fn_801F6C2C(this);
}

// inferred: experience bar of one member (0x10 bytes); names invented
struct ResultExp_unk {
    int mFrom;     // inferred: value shown at the start
    int mTo;       // inferred: value shown at the end
    float mRateFrom;
    float mRateTo;
};

// inferred from __vt__12BattleResult (0x80779CA8): window with the experience and the items won after
// a battle (singleton, 0x8087F040)
class BattleResult : public atn::Task {
public:
    static BattleResult* fn_800EFF68(atn::Task* parent); // inferred: creates the instance
    BattleResult(atn::Task* parent);                    // 0x800EFFD8
    virtual ~BattleResult();                            // 0x800F015C
    virtual int vf0C();                                 // 0x800F01BC
    virtual void vf10();                                // 0x800F03C4
    virtual void vf14();                                // 0x800F0BC0
    void fn_800F0D14(int mode); // inferred: opens the window
    void fn_800F1378();         // inferred: closes the window
    void fn_800F13B0();         // inferred: clears the items

    int mState;               // 0x48
    int mMode;                // 0x4C
    ResultD2A_unk* mBase;     // 0x50
    ResultLayout_unk* mExpOp[7];    // 0x54
    ResultLayout_unk* mExpLoop[7];  // 0x70
    ResultLayout_unk* mLvUpOp[7];   // 0x8C
    ResultLayout_unk* mLvUpLoop[7]; // 0xA8
    int mLvUp[7];             // 0xC4
    unsigned int mCount;      // 0xE0: inferred: number of members
    int mTimer;               // 0xE4
    atn::FixedArray_unk<ResultExp_unk, 7> mExp; // 0xE8
    float mRate;              // 0x15C
    int mHasItems;            // 0x160
    int mItemA;               // 0x164: inferred: item shown later (fn_803E5E64)
    int mItemB;               // 0x168
    int mItemC;               // 0x16C
    unsigned int mItemCount;  // 0x170
    int mItems[2];            // 0x174
    ResultLayout_unk* mItemOp;   // 0x17C
    ResultLayout_unk* mItemLoop; // 0x180
    int m184;

    // inferred: adds an item to the result (at most two; name invented)
    void AddItem_unk(int id) {
        if (mItemCount < 2) {
            PushItem_unk(id);
        }
    }
    void PushItem_unk(int id) {
        new (&mItems[mItemCount]) int(id);
        mItemCount++;
    }

    static BattleResult* sInstance;
};

#endif
