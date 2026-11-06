#ifndef GAME_NEWCAMPPARTSITEM_H
#define GAME_NEWCAMPPARTSITEM_H

#include "game/NewCampPartsBase.h"
#include "std/vector.h"

namespace atn {
class D3Anime;
class D3AnimeInstance;
} // namespace atn

// inferred: entry of the item list (0x18 bytes: fn_8051B164 indexes it with mulli 0x18); names invented
struct NewCampItemEntry_unk {
    int mId;              // 0x00: inferred: item id (fn_80211480)
    unsigned short m04;   // 0x04 (the swap of std::sort copies it with lhz)
    unsigned short m06;
    int m08;              // inferred: nonzero for an item with data
    int m0C;
    int m10;
    int m14;
};


// inferred: d2anime task of camp_sort.d2a (made by fn_801F3FF8); only the members used here
struct NewCampItemD2A_unk : public atn::Task {
    unsigned char m48[0x58 - 0x48];
    unsigned char mLayout[0x100 - 0x58]; // 0x58: inferred: layout
    float mFrame;                        // 0x100
};

// inferred from __vt__16NewCampPartsItem (0x80793450): item list of the camp
class NewCampPartsItem : public NewCampPartsBase {
public:
    static NewCampPartsItem* fn_805169F8(atn::Task* parent); // inferred: creates the parts
    NewCampPartsItem(atn::Task* parent);                       // 0x80516A5C
    virtual ~NewCampPartsItem();                               // 0x80516CA4
    virtual void vf10();                                       // 0x80516DF8
    virtual void vf14();                                       // 0x80516E08
    virtual void vf1C(NewCampScene_unk* scene);                // 0x80516D34
    virtual void vf20();                                       // 0x8051765C
    virtual void vf24();                                       // 0x8051799C
    virtual void vf28();                                       // 0x805179F0
    virtual void vf2C();                                       // 0x80517AD0
    virtual void vf30();                                       // 0x80517AF4
    virtual void vf3C(int show);                               // 0x80517A5C
    virtual void vf48();                                       // 0x8051B7C8
    virtual int vf4C();                                        // 0x80516D2C
    void fn_80517B3C();         // inferred: reads the list
    void fn_8051B164();         // inferred: draws the list
    void fn_8051B43C();         // inferred: input (state 0)
    void fn_8051B544();         // inferred: input (state 1)
    void SetState_unk(int state);
    void fn_8051B364(const NewCampItemEntry_unk& entry, int* cursor, int* top); // inferred: finds the entry again after sorting

    std::vector<NewCampItemEntry_unk> mList; // 0xD4
    int mE0;
    int mE4;
    int mE8;
    int mEC;
    NewCampItemD2A_unk* mSort;        // 0xF0: inferred: camp_sort.d2a
    atn::D3AnimeInstance* mItems[10]; // 0xF4
    atn::D3Anime* mBase;              // 0x11C
    atn::D3Anime* mNoItem;            // 0x120
    atn::D3AnimeInstance* mScroll;    // 0x124
    atn::Vector3 mMove;               // 0x128
    int m134;
};

#endif
