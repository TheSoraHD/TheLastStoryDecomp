#ifndef CHR_CONTROLLADDER_H
#define CHR_CONTROLLADDER_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlLadder.cpp created by CharaTask (0x80162720-0x801638A0).
// Sizes from the operator new calls. These classes declare no destructor (implicit destructors at
// 0x801BC188-0x801BC248).

class ControlLadder : public ControlBase {
public:
    ControlLadder(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B9A54
    virtual int vf0C();                                                            // 0x801B9DE4
    virtual atn::Vector3 vf14();                                                   // 0x801BAC74
    virtual void vf40();                                                           // 0x801BB138
    unsigned char m08[0x2C - 0x08];
    int m2C; // inferred: state (1: climbing, the hips node is moved by m4C)
    unsigned char m30[0x4C - 0x30];
    float m4C; // inferred: vertical offset of the hips
};

class ControlAlleyWay : public ControlBase {
public:
    ControlAlleyWay(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801BB1C8
    virtual int vf0C();                                                              // 0x801BB514
    unsigned char m08[0x30 - 0x08];
};

class ControlCrawling : public ControlBase {
public:
    ControlCrawling(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801BBC48
    virtual int vf0C();                                                              // 0x801BBF84
    unsigned char m08[0x34 - 0x08];
};

// inferred: created by CharaTask::fn_80165F54 (control 0x6C)
class ControlClimbDeck : public ControlBase {
public:
    ControlClimbDeck(Chara_unk* chara, const atn::Vector3& rot); // 0x801BB8BC
    virtual int vf0C();                                           // 0x801BBBDC
    int m08;          // inferred: frame counter
    unsigned char m0C[0x18 - 0x0C];
    atn::Vector3 m18; // inferred: position on the deck
};

} // namespace chr

#endif
