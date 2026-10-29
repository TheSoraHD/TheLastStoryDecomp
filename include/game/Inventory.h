#ifndef GAME_INVENTORY_H
#define GAME_INVENTORY_H

class CharaStatus;

// Name from the RSO (Inventory::GetPlayerStatusInfo(int), AddItem, AddMoney, Order...; static
// instance_). inferred: the object behind lbl_8087F4F0 (returned by fn_801D80B4); one 0x43C-byte
// status block per player from 0x64EC. Only the members used so far are declared.
class Inventory {
public:
    // inferred: one 0x43C-byte block per player (size of CharaStatus)
    struct PlayerStatus_unk {
        unsigned char m000[0x43C];
    };

    // RSO: GetPlayerStatusInfo__9InventoryFi (weak copy 0x805282A8); inferred: the block is a
    // CharaStatus (game/NewCampPartsStatus.cpp passes it as one)
    CharaStatus* GetPlayerStatusInfo(int i) { return (CharaStatus*)&mPlayers[i]; }

    unsigned char m0000[0x64EC];
    PlayerStatus_unk mPlayers[1]; // 0x64EC: inferred: party members
};

#endif
