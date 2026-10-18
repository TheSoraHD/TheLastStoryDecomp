#ifndef CHR_CONTROLDAMAGE_H
#define CHR_CONTROLDAMAGE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: knocked-down control (chr/ControlDamage.cpp, 0x40 bytes). Only what other units use is
// declared; the parameters of the constructor are inferred from chr::ControlCaught::vf10.
class ControlKnockdown : public ControlBase {
public:
    ControlKnockdown(Chara_unk* chara, const atn::Vector3& dir, int arg3, int arg4); // 0x801AD34C
    virtual int vf0C();

    unsigned char m08[0x40 - 0x08];
};

// inferred: other controls of chr/ControlDamage.cpp created by CharaTask; sizes from the operator
// new calls, argument types from the constructors.
class ControlGuard : public ControlBase {
public:
    ControlGuard(Chara_unk* chara, float f, const atn::Vector3& dir, int a); // 0x801ACB38
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

class ControlGuarded : public ControlBase {
public:
    ControlGuarded(Chara_unk* chara, int a); // 0x801AF888
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

class ControlAvoid : public ControlBase {
public:
    ControlAvoid(Chara_unk* chara, const atn::Vector3& dir); // 0x801ACF4C
    virtual int vf0C();
    unsigned char m08[0x14 - 0x08];
};

class ControlDamage : public ControlBase {
public:
    ControlDamage(Chara_unk* chara, const atn::Vector3& dir, int a); // 0x801AC434
    virtual int vf0C();
    unsigned char m08[0x18 - 0x08];
};

} // namespace chr

#endif
