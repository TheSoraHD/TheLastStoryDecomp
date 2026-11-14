#ifndef GAME_INVENTORYEXP_UNK_H
#define GAME_INVENTORYEXP_UNK_H

#include <string.h>
#include "game/Inventory.h"

// inferred: inline constructors of LevelUpResult_unk and Inventory::ExpEntry_unk (the original builds the
// entries with them: the Inventory constructor 0x80442FF4, Inventory::fn_80449ABC and GameTask::fn_80371790
// clear mChara, memset the result and clear the four counters). Kept out of game/CharaStatus.h and
// game/Inventory.h: an inline body there takes @N labels in every unit that includes them; name invented.
inline LevelUpResult_unk::LevelUpResult_unk() {
    memset(this, 0, sizeof(*this));
}

inline Inventory::ExpEntry_unk::ExpEntry_unk() : mChara(0), mExpA(0), mExpB(0), mExp(0), mLevelUp(0) {}

#endif
