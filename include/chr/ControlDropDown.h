#ifndef CHR_CONTROLDROPDOWN_H
#define CHR_CONTROLDROPDOWN_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: falls from a height (motion 0x68, landing motion 0x69). When the character falls out of
// the map it is put back at the start position (or next to the leader, or on the ground found by
// rays around the start). Names invented.
class ControlDropDown : public ControlBase {
public:
    ControlDropDown(Chara_unk* chara, const atn::Vector3& vel); // 0x801B2EDC
    virtual int vf0C();
    void End_unk();  // inferred: enemies drop their attack (name invented)
    void Land_unk(Motion_unk* motion); // inferred: landing motion and a voice now and then (name invented)

    float m08;           // inferred: chara->m570 at the start
    atn::Vector3 mVel;   // 0x0C: inferred: velocity added every frame while falling
    int mFrames;         // 0x18
    atn::Vector3 mStart; // 0x1C: inferred: position before the fall
    float mFallSpeed;    // 0x28: inferred: smoothed fall per frame
    unsigned char m2C;   // inferred: passed to CharaTask::vf34
    unsigned char m2D;   // inferred: landed
    int mSlowFrames;     // 0x30
};

// inferred: same for the quadrupeds (created by QuadrupedTask)
class ControlDropDownQuadruped : public ControlDropDown {
public:
    ControlDropDownQuadruped(Chara_unk* chara, const atn::Vector3& vel); // 0x801B3E70
    virtual int vf0C();
};

} // namespace chr

#endif
