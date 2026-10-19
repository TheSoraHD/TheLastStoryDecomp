#ifndef CHR_CONTROLSHOOT_H
#define CHR_CONTROLSHOOT_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: created by CharaTask::fn_80165C5C; only the constructor is declared (size from the
// operator new call). The id is passed to fn_80219E6C (bullet data), the vectors are copied to
// 0x0C and 0x18.
class ControlShoot : public ControlBase {
public:
    ControlShoot(Chara_unk* chara, int id, const atn::Vector3& a, const atn::Vector3& b); // 0x801C7918
    virtual int vf0C();
    unsigned char m08[0x2C - 0x08];
};

} // namespace chr

#endif
