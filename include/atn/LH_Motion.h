#ifndef ATN_LH_MOTION_H
#define ATN_LH_MOTION_H

#include "atn/LoadHandle.h"
#include "math/Vec.h"

namespace atn {

// inferred: no destructor declared (the implicit one is emitted first at 0x80097128). The
// constructor and two members are compiled out of line in another file (0x800A03A0-0x800A0448).
class LH_Motion : public LoadHandle {
public:
    LH_Motion();                         // 0x800A03A0
    virtual void vf0C(const char* path); // 0x80476664

    void fn_800A03E4(const char* path); // inferred: releases m08 and starts loading `path`
    int fn_800A0448();                  // inferred: nonzero while loading; fetches m08 when done
    void* fn_80476884();                // inferred: returns the loaded motion
    int fn_804768B4();                  // inferred: LoadMotion::m5C (motion name id)
    int fn_804768D0();                  // inferred: LoadMotion::m60

    void* m08; // inferred: loaded motion (from fn_80476884)
};

} // namespace atn

// inferred: node pose filled by fn_800A04A0 from a motion (0x2C bytes); constructor 0x800A08E0
struct MotionPose_unk {
    MotionPose_unk(); // 0x800A08E0
    int m00;
    atn::Vector3 mPos; // 0x04
    atn::Quaternion_unk m10; // inferred: rotation
    atn::Vector3 mScale; // 0x20
};

#endif
