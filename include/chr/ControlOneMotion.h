#ifndef CHR_CONTROLONEMOTION_H
#define CHR_CONTROLONEMOTION_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlOneMotion.cpp created by CharaTask (0x80161570-0x80161B70)
// (control id 0x27). Implicit destructors at the end of the unit in class order.

// inferred: plays one motion (the second argument is passed to atn::Model::fn_80097C08)
class ControlOneMotion : public ControlBase {
public:
    // inferred: `start` >= 0 sets the first frame, `end` >= 0 the last one (motion length otherwise)
    ControlOneMotion(Chara_unk* chara, int motion, int a, int b, float start, float end); // 0x801C3DCC
    virtual int vf0C();
    float mEnd;  // 0x08
    int mLoop;   // 0x0C: inferred: nonzero plays the motion mCount frames instead of up to mEnd
    int mCount;  // 0x10
    int mFrames; // 0x14
};

// inferred: plays a motion turning towards a node matrix
class ControlOneMotionTarget : public ControlBase {
public:
    ControlOneMotionTarget(Chara_unk* chara, int motion, int target, int a); // 0x801C3FB0
    virtual int vf0C();
    MtxPtr mTarget; // 0x08
};

// inferred: plays a motion moving the character from its position by an offset
class ControlOneMotionTransition : public ControlBase {
public:
    ControlOneMotionTransition(Chara_unk* chara, int motion, const atn::Vector3& ofs); // 0x801C43F0
    virtual int vf0C();
    atn::Vector3 mStart; // 0x08
    atn::Vector3 mOfs;   // 0x14
};

} // namespace chr

#endif
