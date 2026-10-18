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

} // namespace chr

#endif
