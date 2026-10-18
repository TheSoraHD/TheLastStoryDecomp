#ifndef CHR_CONTROLCHANT_H
#define CHR_CONTROLCHANT_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlChant.cpp created by CharaTask::fn_8015F568. Sizes from the
// operator new calls; argument types from the call sites and the constructors (the bool is stored
// as a byte by ControlMagic).
class ControlMagic : public ControlBase {
public:
    ControlMagic(Chara_unk* chara, CharaTask* target, float f, const atn::Vector3& pos, bool b); // 0x801BE10C
    virtual int vf0C();
    unsigned char m08[0x24 - 0x08];
};

class ControlChant : public ControlBase {
public:
    ControlChant(Chara_unk* chara, CharaTask* target, const atn::Vector3& pos, int time, float f); // 0x801BC5B8
    virtual int vf0C();
    unsigned char m08[0x34 - 0x08];
    CharaTask* m34; // inferred: target (CharaTask::fn_80160170)
    int m38;        // inferred: chant time
    unsigned char m3C[0x4C - 0x3C];
};

} // namespace chr

#endif
