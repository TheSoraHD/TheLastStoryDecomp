#ifndef GAME_INVENTORY_H
#define GAME_INVENTORY_H

#include "atn/FixedArray.h"
#include "MP/SaveElemB_unk.h"
#include "game/CharaStatus.h"
#include "atn/LH_Binary.h"

class CharaTask;
struct ItemParamEntry_unk;

// inferred: character and status saved when a finishing skill starts (GameTask::fn_80370320); names invented
struct InventoryFinish_unk {
    CharaTask* mChara;
    CharaStatusData_unk mData; // 0x04: copy of CharaStatus::m1A8
    int m80;
    int m84;
    int m88;
    int m8C;
    int m90; // inferred: status bit of the skill
};

// Name from the RSO (Inventory::GetPlayerStatusInfo(int), AddItem, AddMoney, Order...; static
// instance_ = 0x8087F4F0, Order() = 0x801D80B4). The code is game/ItemManager_unk.cpp (between
// HomingBullet and LastCocoonChrysalis in the alphabetical order of the game files, i.e.
// Inventory.cpp). Only the members used so far are declared; offsets from the code that uses
// them (member names inferred).
class Inventory {
public:
    // inferred: experience won in a battle by one party member (0x90 bytes, game/BattleResult)
    struct ExpEntry_unk {
        CharaTask* mChara;
        LevelUpResult_unk mResult; // 0x04: inferred: stats won (CharaStatus::fn_8013354C)
        int mExpA; // 0x80: inferred: experience won
        int mExpB; // 0x84
        int mExp;  // 0x88: inferred: mExpA + mExpB, given to fn_8013354C
        int mLevelUp; // 0x8C: inferred: levels won
    };

    // inferred: item of the bag (0x10 bytes)
    struct Item_unk {
        int mCode;   // inferred: kind * 1000000 + id
        int mCount;  // 0x04
        unsigned short m08;
        int m0C;
    };

    // inferred: equipped item of one slot
    struct EquipSlot_unk {
        int mId;
        int m04;
    };
    // inferred: equipment of one player (0x40 bytes): protectors and weapons (RSO EquipProtector,
    // EquipWeapon)
    struct Equip_unk {
        EquipSlot_unk mProtectors[4]; // 0x00
        EquipSlot_unk mWeapons[2];    // 0x20
        unsigned char m30[0x40 - 0x30];
    };

    Inventory(); // 0x80442FF4
    // inferred: creates / deletes the instance (0x80442F10, 0x80442F68)
    static void fn_80442F10();
    static void fn_80442F68();
    int fn_8044386C(); // inferred: nonzero while the file of mLoader is loading
    void fn_804438E0(int code); // inferred: marks the item `code` as the newest one (game/Em201ZanGurgFinal)
    int fn_80448F9C(int code); // inferred: chance (0-100) of finding the item `code` (gmk/TreasureBox, gmk/DropItem)
    int fn_8044909C(); // inferred: flags of the items that can be found (gmk/TreasureBox, gmk/DropItem)
    void fn_804490E0(ItemParamEntry_unk* item, int member); // inferred: uses a stat item on a party member
    // inferred: rewards of a defeated enemy (game/BattleCharaTask, gmk/FlyObjManager)
    void fn_80449A18(int killer, int level, int exp, int arg4, float rate);
    void fn_80449ABC(int killer, int level); // inferred: shares the experience of fn_80449A18
    int fn_8044D208(int slot, int chara); // inferred: protector of a party member (debug settings, game/CharaTask)
    void fn_8044D404(const int& code); // inferred: adds an item to identify

    // RSO: Order__9InventoryFv (weak copy 0x801D80B4)
    static Inventory* Order() { return instance_; }

    // RSO: AddItem__9InventoryFiiiiPiii (0x804439FC); inferred: (id, count, ...) adds items
    void AddItem(int id, int num, int a2, int a3, int* out, int a5, int a6);

    // RSO: AddMoney__9InventoryFi (0x8044D678); inferred: money += n, clamped to 0..999999
    void AddMoney(int n);
    // inferred: sets the money (0x8044D6E0, right after AddMoney; game/DamageCalc_unk.cpp)
    void fn_8044D6E0(int money);
    Item_unk* fn_80444744(int code); // inferred: finds an item of the bag
    Item_unk* fn_80444804(int index); // inferred: item of the bag by index
    void fn_8044D028(); // inferred: clears the party of the last battle
    void fn_8044D060(int slot, int item, int unused, int chara); // inferred: sets a protector
    void fn_8044D0DC(int slot, int item, int unused, int index); // inferred: sets a protector
    void fn_8044D4F0(); // inferred: clears the items to identify
    int fn_8044D530(int index); // inferred: item to identify (same as fn_8044D500)

    // RSO: GetPlayerStatusInfo__9InventoryFi (weak copy 0x805282A8)
    CharaStatus* GetPlayerStatusInfo(int i) { return &mPlayers[i]; }

    Item_unk mItems[0x600];                         // 0x0000: inferred: the bag (code < 0: free slot)
    int mMoney;                                     // 0x6000
    unsigned char m6004[0x600C - 0x6004];
    int m600C; // inferred: totals of m6014/m6018 (fn_80449ABC)
    int m6010;
    int m6014; // inferred: totals added by fn_80449A18 (first output of fn_804491F4)
    int m6018; // inferred: second output of fn_804491F4

    atn::FixedArray_unk<ExpEntry_unk, 8> mExpEntries; // 0x601C: inferred: party of the last battle (fn_80449ABC)
    int mOrder[10];                                 // 0x64A0: inferred: order of the members (RSO GetPlayerOrder)
    unsigned char m64C8[0x64EC - 0x64C8];
    CharaStatus mPlayers[7];                        // 0x64EC (destroyed by fn_80442F68)
    Equip_unk mEquip[7];                            // 0x8290: inferred: one per player
    MP::SaveElemB_unk mUpgrades[28];                // 0x8450: inferred: upgraded equipment (fn_8044D034)
    Equip_unk mEquipShown[7];                       // 0xD380: inferred: equipment shown by the costume menu
    Equip_unk mEquipSetA[16];                       // 0xD540: inferred: costume sets (game/CosEditBase.cpp)
    Equip_unk mEquipSetB[7];                        // 0xD940 (at most 7: the list below starts at 0xDB2C)
    atn::LH_Binary mLoader;                         // 0xDB00
    int mLoaded;                                    // 0xDB08: inferred: mLoader done (fn_8044386C)
    int mDB0C;                                      // inferred: cleared by the shops around AddItem (0x805849E8)
    unsigned char mDB10[0xDB14 - 0xDB10];
    int mDB14;                                      // inferred: read by fn_8044909C; 1 while mDB18-mDB20 are set
    int mDB18;                                      // inferred: map of the item being added (type, map,
    int mDB1C;                                      // point of fn_80216AFC; -1 when unset, gmk::RandomItem)
    int mDB20;
    int mDB24;                                      // inferred: read by fn_8044909C
    int mDB28;
    atn::FixedArray_unk<int, 0x100> mIdentify;      // 0xDB2C: inferred: items to identify (game/ShopIdentify)
    InventoryFinish_unk mDF30;                      // (size from fn_80442F10)

    static Inventory* instance_;
};

#endif
