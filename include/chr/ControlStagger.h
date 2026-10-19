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

// inferred: created by CharaTask::fn_8016CDB8 (control id 0x55); class name from the vtable
class ControlStagger : public ControlBase {
public:
    ControlStagger(Chara_unk* chara, int a, int b); // 0x801C82F8
    virtual int vf0C();

    unsigned char m08[0x1C - 0x08];
};

} // namespace chr

#endif
