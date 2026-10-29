#ifndef GMK_TREASUREBOX_H
#define GMK_TREASUREBOX_H

#include "atn/ColliBase.h"
#include "atn/EfpHandle_unk.h"
#include "atn/Model_unk.h"
#include "game/GimmickCommon.h"
#include "gmk/ItemSlotAnime.h"

namespace gmk {

// inferred: treasure chest: opening (state 3) draws an item from its table and shows it with a
// gmk::ItemSlotAnime. The name is in the DOL (__vt__Q23gmk11TreasureBox, 0x658 bytes).
class TreasureBox : public GimmickCommon {
public:
    // inferred: chest data (fn_8021ECD0); names invented
    struct Data_unk {
        unsigned char m00[0x08];
        int mTable; // 0x08: inferred: item table id
        int mFlag;  // 0x0C: inferred: field flag (opened)
    };

    TreasureBox(atn::Task* parent, int id, int arg)
        : GimmickCommon(parent, id, arg), mModel(8, 32), mData(0), mItem(-1), mFirst(1) {
        m54 = 0;
        mAnime.fn_8044D9BC(this);
    }
    static GimmickCommon* fn_8040DC30(atn::Task* parent, int id, int arg); // inferred: factory
    virtual ~TreasureBox();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual void Start();
    virtual Model_unk* vf38();
    virtual int vf40(GimmickMsg_unk* msg);
    virtual int vf64(int arg);
    virtual void vf68(int arg1, int arg2);
    virtual void vf70(int state);

    Model_unk mModel;         // 0xF4
    atn::ColliObject mColli;  // 0x4C4
    atn::EfpHandle_unk mEfp;  // 0x54C: "EFP"
    Data_unk* mData;          // 0x558
    int mItem;                // 0x55C: inferred: index of the item drawn (vf64)
    ItemSlotAnime mAnime;     // 0x560
    int mFirst;               // 0x654
};

} // namespace gmk

#endif
