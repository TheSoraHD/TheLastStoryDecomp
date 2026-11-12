#ifndef GAME_COSEDITPROTECTOR_H
#define GAME_COSEDITPROTECTOR_H

#include "game/CosEditBase.h"
#include "atn/FixedArray.h"
#include "std/vector.h"

class CharaStatus;
namespace atn {
struct D2ATask_unk;
}

// inferred: cell of the palette and part grids (0x10 bytes); names invented
struct CosEditCell_unk {
    CosEditCell_unk() : mIcon(0), mFrame(0), mId(0), mEnabled(0) {}
    atn::D3AnimeInstance* mIcon;  // 0x00
    atn::D3AnimeInstance* mFrame; // 0x04
    int mId;                      // 0x08: inferred: palette or part id
    int mEnabled;                 // 0x0C: inferred: owned
};

// inferred: option of a costume part shown by the part list (slot and index in the level list)
struct CosEditPart_unk {
    int mSlot;
    int mIndex;
};

// inferred: costume set (8 bytes, list at 0x1BD0)
struct CosEditSet_unk {
    int mId;
    int m04;
};

// inferred from __vt__16CosEditProtector (0x807827A8): costume menu (equipment, colours and parts).
// Created by fn_801CFD68 (0x1CA8 bytes). The code is split in two files: game/CosEditProtector.cpp
// (0x801CFD68) and game/CosEditPalette_unk.cpp (0x801D54C0, the palette and part pages). Member
// names invented.
class CosEditProtector : public CosEditBase {
public:
    static CosEditProtector* fn_801CFD68(atn::Task* parent); // inferred: creates the menu
    CosEditProtector(atn::Task* parent); // 0x801CFDCC
    virtual ~CosEditProtector(); // 0x801D09C0
    virtual void vf1C(NewCampScene_unk* scene); // 0x801D0C10
    virtual void vf24();         // 0x801D1324
    virtual void vf28();         // 0x801D14D4
    virtual void vf2C();         // 0x801D4700
    virtual void vf30();         // 0x801D156C
    virtual void vf34(int state); // 0x801D4A0C
    virtual void vf48();         // 0x801D162C
    virtual int vf4C();          // 0x801D0B3C
    virtual void vf64();         // 0x801D19F0
    virtual void vf6C();         // 0x801D52F8
    virtual void vf74(int a, int b); // 0x801D5410

    int fn_801D0BAC(); // inferred: busy (loading, or state 13)
    int fn_801D5420(int a, int b); // inferred: (called by game/CosEditItemList_unk.cpp)

    void fn_801D3DF4();                                         // game/CosEditProtector.cpp
    void fn_801D463C(atn::D3Anime** out, CosEditList_unk* lists, int state, int a, int b);
    void fn_801D4E74(int member, atn::D2ATask_unk* task, int mode); // game/CosEditProtector.cpp

    // game/CosEditItemList_unk.cpp (0x801D80A0)
    void fn_801D80BC(); // inferred: state 0
    void fn_801D8560(); // inferred: state 2
    void fn_801D8CC4(); // inferred: state 3
    void fn_801D924C(); // inferred: state 11
    void fn_801D9388(); // inferred: state 12
    void fn_801D94D0(); // inferred: state 13
    // inferred: equips the entry on the member (nonzero when it changed; name invented)
    int fn_801DC42C(int member, CharaTask* chara, CosEditItem_unk* entry, int a, int b);
    CosEditItem_unk* fn_801DCE88(int member, int slot, int a); // inferred: empty entry of an equipment slot
    void fn_801DCF60(int member, CharaTask* chara, int slot, int a); // inferred: takes the protector off
    void fn_801DD26C(int member);
    // inferred: puts the cursor of the protector list on `item` (name invented)
    void fn_801E7098(CosEditItem_unk* item, int list, int* cursor, int* top);
    void fn_801E07A4(int member, int list);
    void fn_801E6714();
    void fn_801E6B10();
    void fn_801E6EF0(int member, int list, int* cursor, int* top, int a);
    void fn_801E6FC8(int member, int list, int* cursor, int* top, int a);

    // game/CosEditPalette_unk.cpp
    void fn_801D54C0(); // inferred: state 0: top page
    void fn_801D59A8(); // inferred: state 8 or 9 opened from the top page
    void fn_801D5AB0(); // inferred: costume sets
    void fn_801D5F04(); // inferred: saving a costume set
    void fn_801D6188(); // inferred: palette grid
    void fn_801D6390(); // inferred: part grid
    void fn_801D6560(); // inferred: part options list
    void fn_801D6754(); // inferred: show or hide the weapons
    void fn_801D694C(int member, CharaTask* chara, int mode);
    int fn_801D6A88(); // inferred: the state shows the costume pages
    void fn_801D6AB0(); // inferred: builds the part options list
    void fn_801D6C1C(int cursor); // inferred: draws the part options list
    void fn_801D6F84(); // inferred: draws the list of costume sets
    void fn_801D70EC(atn::D3Anime* base); // inferred: places the palette grid
    void fn_801D7354(atn::D3Anime* base); // inferred: places the part grid
    void fn_801D7560(int member, CharaStatus* status, NewCampPartsEntry_unk* entry, atn::D3Anime* base);
    void fn_801D77A0(int* ids, atn::D3AnimeInstance** items, NewCampPartsEntry_unk* entry,
                     atn::D3AnimeInstance* base, int item);
    void fn_801D77A4(int* ids, atn::D3AnimeInstance** items, NewCampPartsEntry_unk* entry,
                     atn::D3AnimeInstance* base, int item);
    void fn_801D7AC0(int* ids, atn::D3AnimeInstance** items, NewCampPartsEntry_unk* entry, atn::D3Anime* base,
                     int item);
    void fn_801D7DE8(int* x, int* y, int cols, int rows, int count); // inferred: moves a grid cursor
    CosEditCell_unk* fn_801D8040(); // inferred: selected palette cell
    CosEditCell_unk* fn_801D8060(); // inferred: selected part cell
    void SetEquip_unk(int member, CharaTask* chara, int color, short id, int set, int saved);
    void ToggleOption_unk(int slot, int index);
    // inferred: selected cell of the current grid (inlined in fn_801D6188 and fn_801D6390)
    CosEditCell_unk* GetCell_unk() {
        if (m7C == 8) {
            return fn_801D8040();
        } else if (m7C == 9) {
            return fn_801D8060();
        }
        return 0;
    }

    atn::D3Anime* mSelect;        // 0xACC: equip_select.d2a
    atn::D3Anime* mListParts;     // 0xAD0: protector_list_parts.d2a
    atn::D3Anime* mListBase;      // 0xAD4: protector_list_base.d2a
    atn::D3Anime* mColorSelect;   // 0xAD8: equip_color_select.d2a
    atn::D3Anime* mColorPalette;  // 0xADC: equip_color_palette.d2a
    atn::D3Anime* mEffectPalette; // 0xAE0: equip_effect_palette.d2a
    atn::D3Anime* mStatusUp[5];   // 0xAE4: weapon_status_up_loop.d2a
    atn::D3Anime* mStatusDown[5]; // 0xAF8: weapon_status_down_loop.d2a
    atn::D3Anime* mEquipStatus;   // 0xB0C: equip_status.d2a
    atn::D3Anime* mCampStatus;    // 0xB10: camp_status.d2a
    int mB14;
    atn::D3Anime* mSortBase;      // 0xB18: camp_sort.d2a
    atn::D2ATask_unk* mCharaSelect;  // 0xB1C: camp_chara_select.d2a
    atn::D2ATask_unk* mCharaSelect2; // 0xB20: camp_chara_select2.d2a
    atn::D3Anime* mBest;          // 0xB24: equip_best.d2a
    float mB28;
    atn::D2ATask_unk* mBestTask;  // 0xB2C: equip_best.d2a
    atn::D2ATask_unk* mNaked;     // 0xB30: equip_naked.d2a
    std::vector<CosEditItem_unk> mB34[2]; // inferred: lists of the protectors and of the equipments
    std::vector<CosEditItem_unk> mB4C;
    std::vector<CosEditItem_unk> mB58;
    unsigned char mB64[0xB7C - 0xB64];
    int mB7C;
    int mB80;
    atn::D2ATask_unk* mSort;      // 0xB84: camp_sort.d2a
    atn::Task* mB88;              // inferred: deleted by vf28 (fn_800D2338)
    int mB8C;
    atn::D3Anime* mIconCursor;                // 0xB90: equip_color_icon_cursor.d2a
    atn::D3AnimeInstance* mPalettes[9];       // 0xB94: inferred: palette marks (the last one is the cursor)
    atn::D3AnimeInstance* mBB8[10][8];        // 0xBB8: equip_color_icon.d2a
    CosEditCell_unk mColorCells[8][8];        // 0xCF8
    CosEditCell_unk mPartCells[8][4];         // 0x10F8
    atn::D3AnimeInstance* mColorMarks[32];    // 0x12F8: inferred: mark of the colour in use
    atn::D3AnimeInstance* m1378;              // 0x1378: inferred: lock mark
    atn::FixedArray_unk<CosEditPart_unk, 128> mPartLists[2]; // 0x137C
    atn::Vector3 m1B84;
    int m1B90; // 0x1B90: inferred: a confirmation dialog is open
    int m1B94; // inferred: a message must be shown
    int m1B98;
    int m1B9C;
    int m1BA0; // inferred: cursor of the top page
    int m1BA4; // inferred: cursor of the costume sets
    int m1BA8; // inferred: cursor of the saved sets
    int m1BAC; // inferred: cursor of the set being saved
    int m1BB0; // inferred: 1: saved sets, 0: built-in sets
    int m1BB4; // inferred: cursor of the part options
    int m1BB8; // inferred: cursor of the colour slot
    int m1BBC;
    int m1BC0; // inferred: weapons shown (0..3)
    unsigned char m1BC4[0x1BC8 - 0x1BC4];
    int m1BC8;
    int m1BCC;
    int mSetCount;                // 0x1BD0: inferred: saved costume sets
    CosEditSet_unk mSets[7];      // 0x1BD4
    atn::FixedArray_unk<CosEditSet_unk, 4> m1C0C;
    atn::FixedArray_unk<CosEditSet_unk, 4> m1C30;
    atn::FixedArray_unk<CosEditSet_unk, 4> m1C54;
    std::vector<CosEditItem_unk> m1C78;
    std::vector<CosEditItem_unk> m1C84;
    std::vector<CosEditItem_unk> m1C90;
    std::vector<CosEditItem_unk> m1C9C;
};

#endif
