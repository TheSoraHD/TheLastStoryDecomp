#ifndef CHR_CONTROLCHARGE_H
#define CHR_CONTROLCHARGE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: control of chr/ControlCharge.cpp created by CharaTask::fn_801588A0. Only the
// constructor is declared; class name from the vtable stored by the constructor, size from the
// operator new call. The vector is passed to the base constructor (0x801A4B00); the order of the
// float relative to the int is unknown.
class ControlHighJumpAttack : public ControlBase {
public:
    ControlHighJumpAttack(Chara_unk* chara, const atn::Vector3& pos, float speed, int a); // 0x801A5E90
    virtual int vf0C();
    unsigned char m08[0x30 - 0x08];
};

// inferred: created by CharaTask::fn_8016D74C; class name from the vtable
class ControlCharge : public ControlBase {
public:
    ControlCharge(Chara_unk* chara, int a, int b, int c); // 0x801A2CEC
    virtual int vf0C();

    unsigned char m08[0x44 - 0x08];
};

} // namespace chr

#endif
