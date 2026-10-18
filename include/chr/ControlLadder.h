#ifndef CHR_CONTROLLADDER_H
#define CHR_CONTROLLADDER_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlLadder.cpp created by CharaTask (0x80162720-0x801638A0).
// Only the constructors are declared; sizes from the operator new calls. The two vectors are
// copied to 0x08 and 0x14.

class ControlLadder : public ControlBase {
public:
    ControlLadder(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801B9A54
    virtual int vf0C();
    unsigned char m08[0x50 - 0x08];
};

class ControlAlleyWay : public ControlBase {
public:
    ControlAlleyWay(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801BB1C8
    virtual int vf0C();
    unsigned char m08[0x30 - 0x08];
};

class ControlCrawling : public ControlBase {
public:
    ControlCrawling(Chara_unk* chara, const atn::Vector3& a, const atn::Vector3& b); // 0x801BBC48
    virtual int vf0C();
    unsigned char m08[0x34 - 0x08];
};

} // namespace chr

#endif
