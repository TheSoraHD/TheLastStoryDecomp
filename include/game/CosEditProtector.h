#ifndef GAME_COSEDITPROTECTOR_H
#define GAME_COSEDITPROTECTOR_H

#include "game/CosEditBase.h"
#include "atn/FixedArray.h"

class CharaStatus;
namespace atn {
struct D2ATask_unk;
}

// inferred: cell of the palette and part grids (0x10 bytes); names invented
struct CosEditCell_unk {
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

    void fn_801D3DF4();                                         // game/CosEditProtector.cpp
    void fn_801D4E74(int member, atn::D2ATask_unk* task, int mode); // game/CosEditProtector.cpp

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

    unsigned char mACC[0xB94 - 0xACC];
    atn::D3AnimeInstance* mPalettes[9];       // 0xB94: inferred: palette marks (the last one is the cursor)
    unsigned char mBB8[0xCF8 - 0xBB8];
    CosEditCell_unk mColorCells[8][8];        // 0xCF8
    CosEditCell_unk mPartCells[8][4];         // 0x10F8
    atn::D3AnimeInstance* mColorMarks[32];    // 0x12F8: inferred: mark of the colour in use
    atn::D3AnimeInstance* m1378;              // 0x1378: inferred: lock mark
    atn::FixedArray_unk<CosEditPart_unk, 128> mPartLists[2]; // 0x137C
    unsigned char m1B84[0x1B90 - 0x1B84];
    int m1B90; // 0x1B90: inferred: a confirmation dialog is open
    int m1B94; // inferred: a message must be shown
    unsigned char m1B98[0x1BA0 - 0x1B98];
    int m1BA0; // inferred: cursor of the top page
    int m1BA4; // inferred: cursor of the costume sets
    int m1BA8; // inferred: cursor of the saved sets
    int m1BAC; // inferred: cursor of the set being saved
    int m1BB0; // inferred: 1: saved sets, 0: built-in sets
    int m1BB4; // inferred: cursor of the part options
    int m1BB8; // inferred: cursor of the colour slot
    unsigned char m1BBC[0x1BC0 - 0x1BBC];
    int m1BC0; // inferred: weapons shown (0..3)
    unsigned char m1BC4[0x1BD0 - 0x1BC4];
    int mSetCount;                // 0x1BD0: inferred: saved costume sets
    CosEditSet_unk mSets[26];     // 0x1BD4
    unsigned char m1CA4[0x1CA8 - 0x1CA4];
};

#endif
