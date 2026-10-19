#ifndef CHR_CONTROLSTAGGER_H
#define CHR_CONTROLSTAGGER_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: slip control of chr/ControlStagger.cpp (0x18 bytes, created by CharaTask::fn_8013CB68);
// only the constructor is declared
class ControlSlip : public ControlBase {
public:
    ControlSlip(Chara_unk* chara, int a); // 0x801C8938
    virtual int vf0C();

    unsigned char m08[0x18 - 0x08];
};

} // namespace chr

#endif
