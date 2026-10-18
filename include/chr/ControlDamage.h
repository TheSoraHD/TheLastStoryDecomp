#ifndef CHR_CONTROLDAMAGE_H
#define CHR_CONTROLDAMAGE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: knocked-down control (chr/ControlDamage.cpp, 0x40 bytes). Only what other units use is
// declared; the parameters of the constructor are inferred from chr::ControlCaught::vf10.
class ControlKnockdown : public ControlBase {
public:
    ControlKnockdown(Chara_unk* chara, const Vec& dir, int arg3, int arg4); // 0x801AD34C
    virtual int vf0C();

    unsigned char m08[0x40 - 0x08];
};

} // namespace chr

#endif
