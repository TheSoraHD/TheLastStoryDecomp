#ifndef GAME_COLLRESULT_UNK_H
#define GAME_COLLRESULT_UNK_H

#include "atn/ColliBase.h"

// inferred: result of the collision queries of lbl_8087EE98 as the game files declare it. The layout
// is atn::ColliResult_unk (0x50 bytes); the only difference is the out-of-line constructor at
// 0x80140500 (game/CharaTask.cpp), called with bl by CharaTask, Em315Coffin, FlamingArrow and
// ControlReflectLaser while other files inline the zero stores of the atn constructor. (Name
// invented; a weak copy of the inline atn constructor would be emitted before its first caller, not
// between fn_801404F8 and fn_80140518.)
struct CollResult_unk : public atn::ColliResult_unk {
    CollResult_unk(); // 0x80140500
};

#endif
