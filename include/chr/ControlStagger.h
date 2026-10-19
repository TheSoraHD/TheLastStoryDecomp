#ifndef CHR_CONTROLSTAGGER_H
#define CHR_CONTROLSTAGGER_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: created by CharaTask::fn_8016CDB8 (control id 0x55); class name from the vtable
class ControlStagger : public ControlBase {
public:
    ControlStagger(Chara_unk* chara, int time, int kind); // 0x801C82F8
    virtual int vf0C();
    inline void SetPushBack_unk();

    int mTime;         // 0x08
    atn::Vector3 mDir; // 0x0C: inferred: pushed back along it while motion 0x51 plays
    float mSpeed;      // 0x18
};

// inferred: slip control of chr/ControlStagger.cpp (0x18 bytes, created by CharaTask::fn_8013CB68,
// control id 0x63)
class ControlSlip : public ControlBase {
public:
    ControlSlip(Chara_unk* chara, int stop); // 0x801C8938
    virtual int vf0C();

    atn::Vector3 mDir; // 0x08
    int mStop;         // 0x14: inferred: nonzero stops the movement at once
};

} // namespace chr

#endif
