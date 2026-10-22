#ifndef GAME_INTVECTOR_UNK_H
#define GAME_INTVECTOR_UNK_H

#include "std/vector.h"

// inferred: list of ids, a class derived from std::vector<int> (its destructor, weak copy 0x8000D760 of
// Event/Puppeteer_CharaTask.cpp, tests the address four times). Built with __construct_array (constructor:
// weak copy 0x80288390 of game/Em203GreaterDaemon.cpp, which has an array of five); also used by
// Event/Sequence, EnemyQuarkSecond and EnemyQuarkStand, so the real class is shared. Name invented.
struct IntVector_unk : public std::vector<int> {};

#endif
