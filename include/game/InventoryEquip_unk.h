#ifndef GAME_INVENTORYEQUIP_UNK_H
#define GAME_INVENTORYEQUIP_UNK_H

#include "game/Inventory.h"

// inferred: inline body of Inventory::GetEquip_unk (weak copy 0x801D80A0, called out of line by
// game/CosEditItemList_unk and game/NewCampPartsStatus). Kept out of game/Inventory.h: an inline body there
// takes @N labels in every unit that includes it; name invented (as GetPlayerStatusInfo of the RSO).
inline Inventory::Equip_unk* Inventory::GetEquip_unk(int i) {
    return &mEquip[i];
}

#endif
