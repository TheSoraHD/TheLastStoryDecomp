#ifndef CHR_CONTROLCLIFFHANG_H
#define CHR_CONTROLCLIFFHANG_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: created by CharaTask::fn_8016939C (control 0x3A); only the constructor is declared
// (size from the operator new call). `flags` is stored at 0x80 (bit 1 tested), `a` and `f` are
// passed to 0x801AB6B8.
class ControlCliffhang : public ControlBase {
public:
    ControlCliffhang(Chara_unk* chara, int a, int flags, float f); // 0x801AAB64
    virtual int vf0C();
    unsigned char m08[0x9C - 0x08];
};

} // namespace chr

#endif
