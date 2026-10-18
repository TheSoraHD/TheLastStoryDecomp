#ifndef CHR_CONTROLATTRACTHATECHARGE_H
#define CHR_CONTROLATTRACTHATECHARGE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: created by CharaTask::fn_801647BC; only the constructor is declared (size from the
// operator new call)
class ControlAttractHateCharge : public ControlBase {
public:
    ControlAttractHateCharge(Chara_unk* chara); // 0x801B73F0
    virtual int vf0C();
    unsigned char m08[0xC - 0x08];
};

} // namespace chr

#endif
