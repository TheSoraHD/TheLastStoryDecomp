#ifndef CHR_CONTROLDIVE_H
#define CHR_CONTROLDIVE_H

#include "chr/ControlBase.h"

namespace chr {

// Controls of chr/ControlDive.cpp (class names from the vtables stored by the constructors). The
// classes declare no destructor (implicit destructors at 0x801B26C8-0x801B2848).

// inferred: dive (CharaTask::fn_80155790)
class ControlDive : public ControlBase {
public:
    ControlDive(Chara_unk* chara, const atn::Vector3& dir); // 0x801B0D88
    virtual int vf0C();                                     // 0x801B0ED0
    virtual void vf1C();                                    // 0x801B11A8

    atn::Vector3 m08; // inferred: direction
    atn::Vector3 m14; // inferred: rotation when the dive started
};

// inferred: turns toward a ledge and grabs it (CharaTask::fn_80154EC4)
class ControlSwatTurn : public ControlBase {
public:
    ControlSwatTurn(Chara_unk* chara, const atn::Vector3& pos, int link); // 0x801B11BC
    virtual int vf0C();                                                  // 0x801B1230
    virtual int vf3C() { return 1; } // 0x801B26C0

    atn::Vector3 m08; // inferred: target position
    int m14;          // inferred: route link of the ledge
    int m18;          // inferred: frame counter
};

// inferred: slips off the cover (CharaTask::fn_801551AC)
class ControlCoverSlip : public ControlBase {
public:
    ControlCoverSlip(Chara_unk* chara); // 0x801B151C
    virtual int vf0C();                  // 0x801B1624

    atn::Vector3 m08; // inferred: direction
    int m14;          // inferred: frame counter
};

// inferred: control of the character that another one climbs on (created by
// CharaTask::fn_801725EC)
class ControlStool : public ControlBase {
public:
    ControlStool(Chara_unk* chara); // 0x801B192C
    virtual int vf0C();              // 0x801B19C8

    int m08; // inferred: frames left
};

// inferred: swaps the position with another character (control id 0x3F, CharaTask 0x8016939C-0x8016AFE8)
class ControlSwapPos : public ControlBase {
public:
    ControlSwapPos(Chara_unk* chara, const atn::Vector3& dir); // 0x801B19E4
    virtual int vf0C();                                        // 0x801B1AAC

    atn::Vector3 m08; // inferred: direction
    int m14;          // inferred: state (1: turning around)
};

// inferred: steps back (CharaTask::fn_80155A88)
class ControlBackStep : public ControlBase {
public:
    ControlBackStep(Chara_unk* chara, const atn::Vector3& dir, int a); // 0x801B1F4C
    virtual int vf0C();                                                // 0x801B22AC
    virtual void vf1C();                                               // 0x801B242C

    atn::Vector3 m08; // inferred: destination
    atn::Vector3 m14; // inferred: start position
    float m20;        // inferred: time left
};

// inferred: warps to a position (0x801B2480)
class ControlWarp : public ControlBase {
public:
    ControlWarp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& rot); // 0x801B2480
    virtual int vf0C();                                                             // 0x801B2560
    virtual void vf1C();                                                            // 0x801B26A4

    atn::Vector3 m08; // inferred: position
    atn::Vector3 m14; // inferred: rotation
    int m20;          // inferred: state
};

} // namespace chr

#endif
