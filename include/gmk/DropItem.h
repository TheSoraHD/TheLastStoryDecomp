#ifndef GMK_DROPITEM_H
#define GMK_DROPITEM_H

#include "atn/EfpHandle_unk.h"
#include "atn/FixedArray.h"
#include "game/GimmickCommon.h"
#include "gmk/ItemSlotAnime.h"

// inferred: layout anime task of the item prompt (made by fn_801F64D0); only the members used here
struct DropLayout_unk : public atn::Task {
    unsigned char m48[0x50 - 0x48];
    float m50; // inferred: frame
    float m54; // inferred: speed
};

// inferred: item data returned by fn_80211480 (only the members used here)
struct DropItemData_unk {
    unsigned char m00[0x04];
    int mCode; // 0x04: inferred: item code (weapon/armour tables)
    unsigned char m08[0xBC - 0x08];
    short mBC; // inferred: kind (5: accessory?)
};

namespace gmk {

// inferred: item dropped in the map (gimmick type 18, factory fn_804093B4, file gmk/DropItem.cpp): an
// item icon (gmk::ItemSlotAnime) with a prompt (ui_slot_ok.d2b / ui_slot_ok_rare.d2b) that the player
// picks up when touching it. Effects "EFP" (one per kind, see fn_8040B394) and "EFP_GET". The name is
// in the DOL (__vt__Q23gmk8DropItem, 0x8078D458).
class DropItem : public GimmickCommon {
public:
    DropItem(atn::Task* parent, int id, int arg); // 0x80409438
    virtual ~DropItem() {}                         // 0x8040B4B8 (weak copy at the end of the file)
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual int vf1C();
    virtual void vf20();
    virtual int vf40(GimmickMsg_unk* msg);
    // inferred: setters (names invented); they give the register order of gmk::Item constructor
    void SetPos_unk(const atn::Vector3& pos) { m6C = pos; }
    void SetRot_unk(const atn::Vector3& rot) { m78 = rot; }
    // inferred: in the states 3-5 (name invented)
    bool IsOut_unk() {
        bool ret = false;
        if (3 <= m54 && m54 <= 5) {
            ret = true;
        }
        return ret;
    }
    // inferred: nonzero in the states 3-5 while m168 is not negative (weak copy 0x803FB788, kept
    // from gmk::Cannonball)
    // NONMATCHING: 98.12% (in gmk/Cannonball) - registers of m54 and the result swapped (r4/r5)
    virtual int vf60() {
        int ret = 0;
        if (IsOut_unk() && m168 >= 0) {
            ret = 1;
        }
        return ret;
    }
    virtual void vf68(int arg1, int arg2); // 0x8040B1F8
    virtual void vf6C(void* menu);         // 0x8040B4B4: empty

    void fn_8040B200(); // inferred: the player takes the item

    atn::EfpFile mEfp[7];  // 0xF4: "EFP"
    atn::EfpFile mEfpGet;  // 0x148: "EFP_GET"
    int m154; // inferred: drop table (set by vf68)
    int mItem;  // 0x158
    int mCount; // 0x15C
    DropItemData_unk* mData; // 0x160
    float mAlpha;            // 0x164: inferred: alpha of the prompt
    int m168;                // inferred: effect of the item kind (fn_8040B394)
    float mColor[4];         // 0x16C
    int m17C;                // inferred: rare
    int m180;                // inferred: drops to the ground (ray test)
    atn::Vector3 m184;       // inferred: offset of the icon (0, 12, 0)
    int m190; // inferred: set for weapons in the arena
    int m194; // inferred: touched (gmk::Item::vf4C)
    int m198; // inferred: gmk::Item::fn_803FC870
    ItemSlotAnime mSlot;     // 0x19C
    atn::FixedArray_unk<DropLayout_unk*, 2> mLayouts; // 0x290
    unsigned char m29C;
    int m2A0;
    int m2A4; // inferred: the object is 0x2A8 bytes
};

} // namespace gmk

// inferred: factory of gmk::DropItem (also called by GimmickManager)
extern "C" GimmickCommon* fn_804093B4(atn::Task* parent, int id, int arg);

#endif
