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
    // inferred: starts the animation of an item at `pos`
    void fn_8044E418(const atn::Vector3* pos, int arg2, int arg3, void* item, int count, int arg6, int arg7);
    int fn_8044E610();
    void fn_8044E644();
    void fn_8044F2D4();
    void fn_8044F434(); // inferred: per-frame update
    void fn_80450778();

    int m00;
    // inferred: names invented
    bool IsState_unk(int lo, int hi) const { return lo <= mState && mState < hi; }
    bool IsBusy_unk() const { return IsState_unk(3, 5) || mState == 7; }

    int mState; // 0x04
    unsigned char m08[0xF4 - 0x08];
};

} // namespace gmk

#endif
