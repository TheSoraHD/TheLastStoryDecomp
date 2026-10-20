#ifndef CHR_CONTROLINTERACTIONBASE_H
#define CHR_CONTROLINTERACTIONBASE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlInteractionBase.cpp created by CharaTask (0x80161E58-0x80163B88).
// The classes declare no destructor (implicit destructors 0x801B7F50 and 0x801B9854-0x801B9A14).

// inferred: moves the character from m0C to m18 in m08 frames, both points sliding by m24 * m3C
// every frame. The constructor and Setup_unk are defined in the .cpp and inlined in every caller
// (their out-of-line copies were stripped by the linker; their .sdata2 constants come first).
class ControlInteractionBase : public ControlBase {
public:
    ControlInteractionBase(Chara_unk* chara, int id, int motion);
    // inferred: moves along a-b (common part of Constrict, BumpHead, StepOn and ThroughRope); name invented
    void Setup_unk(const atn::Vector3& a, const atn::Vector3& b, float speed);
    virtual int vf0C(); // 0x801B7CD0

    int m08;          // 0x08: inferred: frames left to reach m18
    atn::Vector3 m0C; // 0x0C: inferred: start
    atn::Vector3 m18; // 0x18: inferred: end
    atn::Vector3 m24; // 0x24: inferred: direction
    atn::Vector3 m30; // 0x30
    float m3C;        // 0x3C: inferred: speed
};

class ControlDuckUnderA : public ControlInteractionBase {
public:
    ControlDuckUnderA(Chara_unk* chara); // 0x801B7EB4
    virtual int vf0C();                  // 0x801B7F90
};

class ControlDuckUnderB : public ControlInteractionBase {
public:
    ControlDuckUnderB(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B805C
};

class ControlDuckJoist : public ControlInteractionBase {
public:
    ControlDuckJoist(Chara_unk* chara, const atn::Vector3& rot); // 0x801B8244
};

class ControlConstrict : public ControlInteractionBase {
public:
    ControlConstrict(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b, float f); // 0x801B8360
};

class ControlBumpHead : public ControlInteractionBase {
public:
    ControlBumpHead(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B86CC
};

class ControlStepOn : public ControlInteractionBase {
public:
    ControlStepOn(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B8AD0
};

class ControlThroughRope : public ControlInteractionBase {
public:
    ControlThroughRope(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B8DFC
};

class ControlDiveIntoWater : public ControlInteractionBase {
public:
    ControlDiveIntoWater(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B9128
    virtual int vf0C();          // 0x801B9598
    virtual atn::Vector3 vf14(); // 0x801B9814
    // inferred: position of the first node found among neck, spine1, hips and reference (or the
    // character position); defined in the .cpp and inlined in the constructor and vf0C; name invented
    void GetBodyPos_unk(atn::Vector3& out);
};

} // namespace chr

#endif
