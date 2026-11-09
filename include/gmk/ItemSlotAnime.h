#ifndef GMK_ITEMSLOTANIME_H
#define GMK_ITEMSLOTANIME_H

#include "math/Vec.h"
#include "atn/static_string.h"

struct RecycleBufferEntry_unk;

namespace gmk {

// inferred: item data shown by the animation (the item table entry; names invented)
struct ItemSlotItem_unk {
    int m00;
    int mId;                  // 0x04: inferred: item id (301: always shown)
    unsigned char m08[0xC2 - 0x08];
    signed char mC2;          // inferred: flag: always shown
};

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
    int fn_80450A78() const;
    // inferred: starts the animation of an item at `pos`
    void fn_8044E418(const atn::Vector3* pos, int arg2, int arg3, void* item, int count, int arg6, int arg7);
    int fn_8044E610();
    int fn_80450A5C();
    void fn_8044E644();
    void fn_8044F2D4();
    void fn_8044F434(); // inferred: per-frame update
    void fn_80450778();

    inline void Release_unk(); // inferred: gives the entry m00 back to the buffer (name invented)

    RecycleBufferEntry_unk* m00; // inferred: entry of the ItemSlotAnimeBuffer in use
    // inferred: names invented
    bool IsState_unk(int lo, int hi) const { return lo <= mState && mState < hi; }
    bool IsBusy_unk() const { return IsState_unk(3, 5) || mState == 7; }
    bool IsState56_unk() const { return mState == 5 || mState == 6; } // inferred (gmk::DropItem::vf14)

    int mState; // 0x04
    atn::Vector3 mPos;  // 0x08
    float mScale;       // 0x14
    int m18;
    int m1C;
    ItemSlotItem_unk* m20; // inferred: current item selected by the animation task
    int m24; // inferred: item count checked by fn_8044E610
    unsigned char m28[0x2C - 0x28];
    atn::static_string<32, wchar_t, std::char_traits<wchar_t> > mTexts[3]; // 0x2C: inferred: texts of the item
    // inferred: flags set by gmk::DropItem (names invented)
    unsigned int mFlag31 : 1;  // inferred: rare item (fn_80450B84/fn_80450B60)
    unsigned int mFlag30 : 1;
    unsigned int mBits29 : 2;
    unsigned int mFlag27 : 1;
    unsigned int mFlag26 : 1;  // inferred: the "take" prompt is shown
    unsigned int mFlag25 : 1;
    unsigned int mFlag24 : 1;
    unsigned int mBits23 : 24;
    int mF0;
};

} // namespace gmk

#endif
