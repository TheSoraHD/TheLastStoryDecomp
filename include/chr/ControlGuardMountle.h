#ifndef CHR_CONTROLGUARDMOUNTLE_H
#define CHR_CONTROLGUARDMOUNTLE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: climbs onto a guarding character or a ledge (created by CharaTask::fn_801720AC and
// fn_801725EC with the horizontal direction). Only the constructor is declared; class name from the
// vtable stored by the constructor, size from the operator new call.
class ControlGuardMountle : public ControlBase {
public:
    ControlGuardMountle(Chara_unk* chara, const atn::Vector3& dir); // 0x801C0D80
    virtual int vf0C();
    unsigned char m08[0x28 - 0x08];
};

} // namespace chr

#endif
