#ifndef GAME_INVENTORY_H
#define GAME_INVENTORY_H

#include "atn/FixedArray.h"
#include "MP/SaveElemB_unk.h"
#include "game/CharaStatus.h"
#include "atn/LH_Binary.h"

class CharaTask;

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
        unsigned char m04[0x80 - 0x04];
        int mExpA; // 0x80: inferred: experience won
        int mExpB; // 0x84
        unsigned char m88[0x8C - 0x88];
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
    int fn_8044909C(); // inferred: flags of the items that can be found (gmk/TreasureBox, gmk/DropItem)

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
    unsigned char m6004[0x601C - 0x6004];
    atn::FixedArray_unk<ExpEntry_unk, 7> mExpEntries; // 0x601C: inferred: party of the last battle
    unsigned char m6410[0x64A0 - 0x6410];
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
    unsigned char mDB0C[0xDB14 - 0xDB0C];
    int mDB14;                                      // inferred: read by fn_8044909C
    unsigned char mDB18[0xDB24 - 0xDB18];
    int mDB24;                                      // inferred: read by fn_8044909C
    unsigned char mDB28[0xDB2C - 0xDB28];
    atn::FixedArray_unk<int, 0x100> mIdentify;      // 0xDB2C: inferred: items to identify (game/ShopIdentify)
    unsigned char mDF30[0xDFC4 - 0xDF30];           // (size from fn_80442F10)

    static Inventory* instance_;
};

#endif
