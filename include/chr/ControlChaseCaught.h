#ifndef CHR_CONTROLCHASECAUGHT_H
#define CHR_CONTROLCHASECAUGHT_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlChaseCaught.cpp created by CharaTask (0x801598B4-0x8015A184).
// Only the constructors are declared; class names from the vtables stored by the constructors,
// sizes from the operator new calls, argument types from the constructors.

class ControlChaseCaught : public ControlBase {
public:
    ControlChaseCaught(Chara_unk* chara, int a, int b, int c); // 0x801A77BC
    virtual int vf0C();
    unsigned char m08[0x14 - 0x08];
};

class ControlChaseCatch : public ControlBase {
public:
    ControlChaseCatch(Chara_unk* chara, int a); // 0x801A9A94
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

class ControlChaseAssist : public ControlBase {
public:
    ControlChaseAssist(Chara_unk* chara); // 0x801A880C
    virtual int vf0C();
    unsigned char m08[0x10 - 0x08];
};

class ControlChaseFollow : public ControlBase {
public:
    ControlChaseFollow(Chara_unk* chara); // 0x801A90BC
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

} // namespace chr

#endif
