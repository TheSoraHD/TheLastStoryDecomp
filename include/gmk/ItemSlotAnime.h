#ifndef GMK_ITEMSLOTANIME_H
#define GMK_ITEMSLOTANIME_H

#include "math/Vec.h"

namespace gmk {

// inferred: item that pops out of a gimmick (gmk::TreasureBox); the name comes from
// RecycleBuffer<gmk::ItemSlotAnime> (RecycleBuffer_gmk/ItemSlotAnime.cpp, not decompiled). Only the
// members used by the gimmicks are known; 0xF4 bytes.
class ItemSlotAnime {
public:
    ItemSlotAnime();  // 0x8044D710
    ~ItemSlotAnime(); // 0x8044D884
    void fn_8044D9BC(void* owner);
    int fn_8044E1BC(); // inferred: nonzero while loading
    void fn_8044EA34(int arg);
    int fn_8044EEC0();
    void fn_8044F14C();
    void fn_8044F96C();
    int fn_80450A78();
    // inferred: starts the animation of an item at `pos`
    void fn_8044E418(const atn::Vector3* pos, int arg2, int arg3, void* item, int count, int arg6, int arg7);
    int fn_8044E610();
    int fn_80450A5C();
    void fn_8044E644();
    void fn_8044F2D4();
    void fn_8044F434(); // inferred: per-frame update
    void fn_80450778();

    int m00;
    // inferred: names invented
    bool IsState_unk(int lo, int hi) const { return lo <= mState && mState < hi; }
    bool IsBusy_unk() const { return IsState_unk(3, 5) || mState == 7; }
    bool IsState56_unk() const { return mState == 5 || mState == 6; } // inferred (gmk::DropItem::vf14)

    int mState; // 0x04
    unsigned char m08[0x18 - 0x08];
    int m18;
    int m1C;
    void* m20; // inferred: current item selected by the animation task
    int m24; // inferred: item count checked by fn_8044E610
    unsigned char m28[0xEC - 0x28];
    // inferred: flags set by gmk::DropItem (names invented)
    unsigned int mFlag31 : 1;  // inferred: rare item (fn_80450B84/fn_80450B60)
    unsigned int mFlag30 : 1;
    unsigned int mBits29 : 2;
    unsigned int mFlag27 : 1;
    unsigned int mFlag26 : 1;  // inferred: the "take" prompt is shown
    unsigned int mFlag25 : 1;
    unsigned int mFlag24 : 1;
    unsigned int mBits23 : 24;
    unsigned char mF0[0xF4 - 0xF0];
};

} // namespace gmk

#endif
