#ifndef GAME_NEWCAMPPAIRS_UNK_H
#define GAME_NEWCAMPPAIRS_UNK_H

#include "game/NewCampTask.h"

// inferred: inline bodies of the pair accessors of NewCampTask (weak copies 0x801DA2F0 and 0x801DA2F8 of
// game/CosEditItemList_unk, which reads m1224 through them). Kept out of game/NewCampTask.h: an inline body
// there takes @N labels in every unit that includes it.
inline unsigned int NewCampTask::GetPairCount_unk() const {
    return m1224.size();
}

inline NewCampPair_unk& NewCampTask::GetPair_unk(int i) {
    return m1224[i];
}

#endif
