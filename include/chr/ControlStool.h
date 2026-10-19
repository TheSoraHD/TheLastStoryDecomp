#ifndef CHR_CONTROLSTOOL_H
#define CHR_CONTROLSTOOL_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: control of the character that another one climbs on (chr/ControlDive.cpp; created by
// CharaTask::fn_801725EC). Only the constructor is declared; class name from the vtable stored by
// the constructor, size from the operator new call.
class ControlStool : public ControlBase {
public:
    ControlStool(Chara_unk* chara); // 0x801B192C
    virtual int vf0C();
    unsigned char m08[0x0C - 0x08];
};

} // namespace chr

#endif
