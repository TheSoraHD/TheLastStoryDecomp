#ifndef ATN_MOTIONSLOT_UNK_H
#define ATN_MOTIONSLOT_UNK_H

#include "atn/LH_Motion.h"

namespace atn {

// inferred: wrapper of a motion loader (non-virtual destructor 0x800970CC; constructor 0x800A03A0 out of
// line in atn/Motion_unk.cpp): slots of atn::Model and gmk::MapLift, Event::MotionData, the entries of
// LH_MotionSet and NewCampPartsBase, TutorialTask. Name invented
struct MotionSlot_unk {
    MotionSlot_unk();
    LH_Motion& GetMotion_unk() { return mMotion; }
    LH_Motion mMotion;
};

} // namespace atn

#endif
