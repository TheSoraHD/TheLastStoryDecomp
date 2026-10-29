#ifndef GAME_INVENTORY_H
#define GAME_INVENTORY_H

#include "atn/FixedArray.h"

class CharaStatus;
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

    // inferred: one 0x43C-byte block per player (size of CharaStatus, see GetPlayerStatusInfo)
    struct PlayerStatus_unk {
        unsigned char m000[0x230];
        unsigned char mEquipFlags; // 0x230: inferred: one bit per equipment slot (game/CharaTask)
        unsigned char m231[0x43C - 0x231];
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

    // RSO: Order__9InventoryFv (weak copy 0x801D80B4)
    static Inventory* Order() { return instance_; }

    // RSO: AddItem__9InventoryFiiiiPiii (0x804439FC); inferred: (id, count, ...) adds items
    void AddItem(int id, int num, int a2, int a3, int* out, int a5, int a6);

    // RSO: AddMoney__9InventoryFi (0x8044D678); inferred: money += n, clamped to 0..999999
    void AddMoney(int n);
    // inferred: sets the money (0x8044D6E0, right after AddMoney; game/DamageCalc_unk.cpp)
    void fn_8044D6E0(int money);

    // RSO: GetPlayerStatusInfo__9InventoryFi (weak copy 0x805282A8); inferred: the block is a
    // CharaStatus (game/NewCampPartsStatus.cpp passes it as one)
    CharaStatus* GetPlayerStatusInfo(int i) { return (CharaStatus*)&mPlayers[i]; }

    unsigned char m0000[0x6000];
    int mMoney;                                     // 0x6000
    unsigned char m6004[0x601C - 0x6004];
    atn::FixedArray_unk<ExpEntry_unk, 7> mExpEntries; // 0x601C: inferred: party of the last battle
    unsigned char m6410[0x64A0 - 0x6410];
    int mOrder[10];                                 // 0x64A0: inferred: order of the members (RSO GetPlayerOrder)
    unsigned char m64C8[0x64EC - 0x64C8];
    PlayerStatus_unk mPlayers[7];                   // 0x64EC
    Equip_unk mEquip[7];                            // 0x8290: inferred: one per player
    unsigned char m8450[0xD380 - 0x8450];
    Equip_unk mEquipShown[7];                       // 0xD380: inferred: equipment shown by the costume menu
    Equip_unk mEquipSetA[16];                       // 0xD540: inferred: costume sets (game/CosEditBase.cpp)
    Equip_unk mEquipSetB[16];                       // 0xD940

    static Inventory* instance_;
};

#endif
