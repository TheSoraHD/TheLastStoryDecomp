#ifndef CHR_CONTROLINTERACTIONBASE_H
#define CHR_CONTROLINTERACTIONBASE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlInteractionBase.cpp created by CharaTask (0x80161E58-0x80163B88).
// Only the constructors are declared; sizes from the operator new calls, argument types from the
// constructors (vectors read through pointers).

class ControlInteractionBase : public ControlBase {
public:
    ControlInteractionBase(Chara_unk* chara) : ControlBase(chara) {}
};

class ControlDuckUnderA : public ControlInteractionBase {
public:
    ControlDuckUnderA(Chara_unk* chara); // 0x801B7EB4
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlDuckUnderB : public ControlInteractionBase {
public:
    ControlDuckUnderB(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B805C
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlDuckJoist : public ControlInteractionBase {
public:
    ControlDuckJoist(Chara_unk* chara, const atn::Vector3& rot); // 0x801B8244
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlConstrict : public ControlInteractionBase {
public:
    ControlConstrict(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b, float f); // 0x801B8360
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlBumpHead : public ControlInteractionBase {
public:
    ControlBumpHead(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B86CC
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlStepOn : public ControlInteractionBase {
public:
    ControlStepOn(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B8AD0
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlThroughRope : public ControlInteractionBase {
public:
    ControlThroughRope(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B8DFC
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

class ControlDiveIntoWater : public ControlInteractionBase {
public:
    ControlDiveIntoWater(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B9128
    virtual int vf0C();
    unsigned char m08[0x40 - 0x08];
};

} // namespace chr

#endif
