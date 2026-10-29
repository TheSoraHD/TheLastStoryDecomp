#ifndef CHR_CONTROLMOUNTLE_H
#define CHR_CONTROLMOUNTLE_H

#include "chr/ControlBase.h"

namespace chr {

// Controls of chr/ControlMountle.cpp (0x801C041C-0x801C3414): climbing onto a ledge, a guarding
// character or a wall, and the jump towards a ledge. Class names from the vtables; ControlMountle
// declares its destructor (0x801C0CC4), the others have implicit destructors at the end of the file
// (0x801C3314-0x801C33D4) in the order of the classes below.

// inferred: climbs onto a ledge (control id 3, created by CharaTask::vf74); the motion moves the
// character along m08 until frame m24
class ControlMountle : public ControlBase {
public:
    ControlMountle(Chara_unk* chara, const atn::Vector3& dir, float speed); // 0x801C041C
    virtual ~ControlMountle();
    virtual int vf0C();
    virtual atn::Vector3 vf14();
    virtual void vf60(); // inferred: hook run when the climb ends (empty here)

    atn::Vector3 m08; // inferred: direction of the ledge
    atn::Vector3 m14; // inferred: start position
    float m20;        // inferred: speed passed to CharaTask::fn_8013CB68 at the end
    float m24;        // inferred: frame where the rise ends
    float m28;        // inferred: start of the frames where the character moves forward at the end
    float m2C;        // inferred: end of those frames
};

// inferred: same climb for the four-legged characters (created by game/QuadrupedTask.cpp); vf60
// deals the hit of the climb
class ControlMountleQuadruped : public ControlMountle {
public:
    ControlMountleQuadruped(Chara_unk* chara, const atn::Vector3& dir, float speed); // 0x801C0C70
    virtual void vf60();
};

// inferred: climbs onto a guarding character or a ledge (control id 0x2D, created by
// CharaTask::fn_801720AC and fn_801725EC with the horizontal direction)
class ControlGuardMountle : public ControlBase {
public:
    ControlGuardMountle(Chara_unk* chara, const atn::Vector3& dir); // 0x801C0D80
    virtual int vf0C();
    virtual atn::Vector3 vf14();

    atn::Vector3 m08; // inferred: direction
    atn::Vector3 m14; // inferred: start position
    float m20;        // inferred: speed scale (40 frames over the length of the motion)
    float m24;        // inferred: vertical speed of the fall
};

// inferred: climbs a wall (control id 0x2D, created by CharaTask::fn_801725EC and ControlJump::vf10)
class ControlWallMountle : public ControlBase {
public:
    ControlWallMountle(Chara_unk* chara, const atn::Vector3& normal, const atn::Vector3& pos, int jump); // 0x801C1578
    virtual int vf0C();
    virtual atn::Vector3 vf14();

    atn::Vector3 m08; // inferred: direction towards the wall
    atn::Vector3 m14; // inferred: start position
    atn::Vector3 m20; // inferred: position on the wall
    atn::Vector3 m2C; // inferred: top of the wall
    float m38;        // inferred: time
};

// inferred: jumps towards a ledge (control id 0x2D, created by CharaTask with the position and the
// direction); vf10 climbs the wall (ControlWallMountle) or keeps walking (CharaTask::fn_801591E8)
class ControlJump : public ControlBase {
public:
    ControlJump(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& dir); // 0x801C1DCC
    bool fn_801C2198(atn::Vector3& out, const atn::Vector3& dir); // inferred: looks for the ledge
    virtual int vf0C();
    virtual boost::function<void()> vf10();
    virtual atn::Vector3 vf14();

    atn::Vector3 m08; // inferred: direction of the jump
    atn::Vector3 m14; // inferred: start position
    atn::Vector3 m20; // inferred: landing position
    float m2C;        // inferred: speed
    bool m30;         // inferred: a ledge was found
    bool m31;         // inferred: the ledge is a wall to climb
    float m34;        // inferred: time
    float m38;        // inferred: length of the motion
};

} // namespace chr

#endif
