#ifndef GAME_GAMEMAP_UNK_H
#define GAME_GAMEMAP_UNK_H

#include "atn/Task.h"

// inferred: map of the field (a task: Event::Manager tests its flags and removes it with fn_800D2338).
// Only the base and the area/map/part at 0x48 are known (set by Event::Manager::fn_804805F0;
// probably a MapTask, created by fn_8049D68C/fn_8049D704). Name invented.
struct GameMap_unk : public atn::Task {
    int m48; // inferred: area
    int m4C; // inferred: map
    int m50; // inferred: part
};

#endif
