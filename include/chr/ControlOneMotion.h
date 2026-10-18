#ifndef CHR_CONTROLONEMOTION_H
#define CHR_CONTROLONEMOTION_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlOneMotion.cpp created by CharaTask (0x80161570-0x80161B70).
// Only the constructors are declared; sizes from the operator new calls, argument types from the
// constructors (argument order of the floats unknown).

// inferred: plays one motion (the second argument is passed to Model_unk::fn_80097C08)
class ControlOneMotion : public ControlBase {
public:
    ControlOneMotion(Chara_unk* chara, int motion, int a, int b, float c, float d); // 0x801C3DCC
    virtual int vf0C();
    unsigned char m08[0x18 - 0x08];
};

class ControlOneMotionTarget : public ControlBase {
public:
    ControlOneMotionTarget(Chara_unk* chara, int motion, int target, int a); // 0x801C3FB0
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

class ControlOneMotionTransition : public ControlBase {
public:
    ControlOneMotionTransition(Chara_unk* chara, int motion, const atn::Vector3& pos); // 0x801C43F0
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

} // namespace chr

#endif
