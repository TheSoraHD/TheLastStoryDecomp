#ifndef GAME_GAMESCENEFIND_UNK_H
#define GAME_GAMESCENEFIND_UNK_H

#include "game/GameScene.h"

// Inline lookups of GameScene, outside the class so that the units that do not use them keep their
// numbering (an inline body in a header consumes @N in every unit that includes it).

// inferred: weak copy 0x800EC204 (game/BattleCharaTask), formerly RouteTable_unk::FindPlace_unk
inline gs::WarpPoint* GameScene::FindPlace_unk(int id) {
    for (unsigned int i = 0; i < (unsigned int)m78.mSize; i++) {
        if (id == m78.mData[i].mId) {
            return &m78.mData[i];
        }
    }
    return 0;
}

// inferred: hide point `link` (1-based), formerly RouteTable_unk::GetLink_unk
inline gs::HidePoint* GameScene::GetLink_unk(int link) { return &mAC.mData[link - 1]; }

#endif
