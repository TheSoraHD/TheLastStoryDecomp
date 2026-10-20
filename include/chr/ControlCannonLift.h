#ifndef CHR_CONTROLCANNONLIFT_H
#define CHR_CONTROLCANNONLIFT_H

#include "chr/ControlMoveCursor.h"

class GimmickCommon;

namespace chr {

// inferred: these classes declare no destructor (implicit destructors at 0x801A21AC, 0x801A21EC,
// 0x801A222C)

// inferred: the character is lifted into a cannon (created by CharaTask::fn_80174E04)
class ControlCannonLift : public ControlBase {
public:
    ControlCannonLift(Chara_unk* chara, GimmickCommon* cannon); // 0x801A0BA8
    virtual int vf0C();                                         // 0x801A0C54
    virtual boost::function<void()> vf10();                     // 0x801A10CC
    virtual void vf1C();                                        // 0x801A1548

    GimmickCommon* mCannon; // 0x08
    bool m0C;               // 0x0C: inferred: go on to chr::ControlCannonTarget when finished
    bool m0D;               // 0x0D: inferred: the arena request was sent
    bool m0E;               // 0x0E: inferred: the arena slot is held
};

// inferred: shot by the cannon (created by CharaTask::fn_8017518C with mF6C and mF78)
class ControlCannonThrow : public ControlBase {
public:
    ControlCannonThrow(Chara_unk* chara, const atn::Vector3& dir, float speed); // 0x801A17A4
    virtual int vf0C();                                                        // 0x801A1854

    atn::Vector3 m08; // inferred: target position of the shot
    float m14;        // inferred: hit radius
};

// inferred: aims the cannon (created by CharaTask::fn_80139560 and ControlCannonLift::vf10)
class ControlCannonTarget : public ControlMoveCursor {
public:
    ControlCannonTarget(Chara_unk* chara); // 0x801A1BE4
    virtual int vf0C();                     // 0x801A1D9C
    virtual boost::function<void()> vf10(); // 0x801A2058

    // inferred: passes the aim to the character (read back by CharaTask::fn_8017518C); name invented
    void SetAim_unk() {
        mChara->SetHit_unk(0);
        mChara->mF6C = m08;
        mChara->mF78 = m28;
    }

    float m28; // inferred: speed/radius given by the cannon (fn_803FB830)
    int m2C;   // inferred: fire
};

} // namespace chr

#endif
