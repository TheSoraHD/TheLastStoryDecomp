#ifndef ATN_COLLIFLAGS_UNK_H
#define ATN_COLLIFLAGS_UNK_H

#include "atn/ColliBase.h"

namespace atn {

// inferred: bit 0 of ColliBase::mFlags = the shape takes part in the tests (names invented). Kept
// out of ColliBase.h: every inline member defined there consumes @N labels in each unit that
// includes it. As members, the original reloads mFlags before each write (gmk/RotaryRoom vf10).
inline bool ColliBase::IsOn_unk() const { return (mFlags & 1) == 1; }
inline void ColliBase::SetOn_unk() { mFlags |= 1; }
inline void ColliBase::SetOff_unk() { mFlags &= ~1; }

} // namespace atn

#endif
