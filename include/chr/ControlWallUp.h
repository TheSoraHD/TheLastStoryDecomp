#ifndef CHR_CONTROLWALLUP_H
#define CHR_CONTROLWALLUP_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: climbs a wall (chr/ControlWallUp.cpp), created by CharaTask::fn_801591E8. Only the
// constructor is declared; class name from the vtable stored by the constructor, size from the
// operator new call.
class ControlWallUp : public ControlBase {
public:
    ControlWallUp(Chara_unk* chara, const atn::Vector3& pos, const atn::Vector3& dir); // 0x801C9F6C
    virtual int vf0C();
    unsigned char m08[0x54 - 0x08];
};

} // namespace chr

#endif
