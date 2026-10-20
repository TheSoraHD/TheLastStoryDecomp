#ifndef CHR_CONTROLSHOOT_H
#define CHR_CONTROLSHOOT_H

#include "chr/ControlBase.h"
#include "game/HitInfo_unk.h"

namespace chr {

// inferred: created by CharaTask::fn_80165C5C (control id 0x62). The id is passed to fn_80219E6C
// (bullet data); the bullet goes from mFrom to mTo.
class ControlShoot : public ControlBase {
public:
    ControlShoot(Chara_unk* chara, int id, const atn::Vector3& from, const atn::Vector3& to); // 0x801C7918
    virtual int vf0C();

    HitInfo_unk* mData; // 0x08
    atn::Vector3 mFrom; // 0x0C
    atn::Vector3 mTo;   // 0x18
    int mMotion;        // 0x24: inferred: motion played before the shot (restored by vf0C)
    int m28;
};

} // namespace chr

#endif
