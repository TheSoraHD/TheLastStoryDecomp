#ifndef CHR_CONTROLATTRACTHATECHARGE_H
#define CHR_CONTROLATTRACTHATECHARGE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: created by CharaTask::fn_801647BC (size from the operator new call)
class ControlAttractHateCharge : public ControlBase {
public:
    ControlAttractHateCharge(Chara_unk* chara); // 0x801B73F0
    virtual int vf0C();
    virtual void vf1C();
    float mTime; // 0x08: inferred: counted down by GameState_unk::m3A4 (starts at the motion length)
};

// inferred: created by BattleCharaTask (0x800E7E00) with a skill id (control id 0x84)
class ControlOverdrive : public ControlBase {
public:
    ControlOverdrive(Chara_unk* chara, int skill); // 0x801B77DC
    virtual int vf0C();
    int mSkill;                // 0x08: inferred: id passed to fn_80219E6C
    unsigned char mFired;      // 0x0C
    unsigned char mEffectDone; // 0x0D
};

} // namespace chr

#endif
