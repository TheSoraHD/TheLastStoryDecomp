#ifndef CHR_CONTROLATTACKBASE_H
#define CHR_CONTROLATTACKBASE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: guard counter control (chr/ControlAttackBase.cpp, 0x20 bytes); only the constructor
// used by CharaTask is declared.
class ControlGuardCounter : public ControlBase {
public:
    ControlGuardCounter(Chara_unk* chara, int a); // 0x8018CEF8
    virtual int vf0C();
    unsigned char m08[0x20 - 0x08];
};

// inferred: other controls of chr/ControlAttackBase.cpp created by CharaTask (class names from
// the vtables their constructors store); sizes from the operator new calls.
class ControlAttackSetup : public ControlBase {
public:
    ControlAttackSetup(Chara_unk* chara, int a); // 0x8018B48C
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

// inferred: the next ones derive from ControlAttackBase (their constructors store its vtable
// first); only the constructors are declared, so they are declared on ControlBase here.
class ControlAttack : public ControlBase {
public:
    ControlAttack(Chara_unk* chara, const atn::Vector3& dir); // 0x80189CB4
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

class ControlDashAttack : public ControlBase {
public:
    ControlDashAttack(Chara_unk* chara, const atn::Vector3& pos); // 0x8018B6A0
    virtual int vf0C();
    unsigned char m08[0x34 - 0x08];
};

class ControlJumpAttack : public ControlBase {
public:
    ControlJumpAttack(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& offset); // 0x8018BB7C
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlJumpAttack2 : public ControlBase {
public:
    ControlJumpAttack2(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& offset); // 0x8018C0EC
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlAttackNext : public ControlBase {
public:
    ControlAttackNext(Chara_unk* chara, const atn::Vector3& dir); // 0x8018A8CC
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

class ControlMultiAttack : public ControlBase {
public:
    ControlMultiAttack(Chara_unk* chara, const atn::Vector3& dir); // 0x8018C6B8
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

class ControlSideWallAttack : public ControlBase {
public:
    ControlSideWallAttack(Chara_unk* chara, const atn::Vector3& dir); // 0x8018CAEC
    virtual int vf0C();
    unsigned char m08[0x1C - 0x08];
};

class ControlSwapAttack : public ControlBase {
public:
    ControlSwapAttack(Chara_unk* chara, Chara_unk* target); // 0x8018DAE4
    virtual int vf0C();
    unsigned char m08[0x3C - 0x08];
};

} // namespace chr

#endif
