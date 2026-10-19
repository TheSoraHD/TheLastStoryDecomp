#ifndef CHR_CONTROLPUSHEDDOWN_H
#define CHR_CONTROLPUSHEDDOWN_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlPushedDown.cpp created by CharaTask::fn_8013CB68 when two
// characters bump into each other. Only the constructors are declared; sizes from the operator new
// calls, argument types from the constructors (ints stored as words, vectors read through pointers).
class ControlPushedDown : public ControlBase {
public:
    // inferred: sets the control id 0x44
    ControlPushedDown(Chara_unk* chara, const atn::Vector3& dir, int a, int b, int c); // 0x801C48B4
    virtual int vf0C();
    int fn_801C59D4(); // inferred: nonzero once the character can be pushed again (m2C or a frame test)

    unsigned char m08[0x3C - 0x08];
};

class ControlPushedAvoid : public ControlBase {
public:
    // inferred: sets the control id 0x45
    ControlPushedAvoid(Chara_unk* chara, const atn::Vector3& dir, Chara_unk* other); // 0x801C61EC
    virtual int vf0C();

    unsigned char m08[0x3C - 0x08];
};

} // namespace chr

#endif
