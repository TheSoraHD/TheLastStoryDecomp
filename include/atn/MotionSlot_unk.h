#ifndef ATN_MOTIONSLOT_UNK_H
#define ATN_MOTIONSLOT_UNK_H

#include "atn/LH_Motion.h"

namespace atn {

// inferred: motion slot of a model (the array is built with the constructor of LH_Motion and destroyed
// with its own destructor 0x800970CC, also used by gmk::MapLift); name invented
struct MotionSlot_unk {
    LH_Motion& GetMotion_unk() { return mMotion; }
    LH_Motion mMotion;
};

} // namespace atn

#endif
