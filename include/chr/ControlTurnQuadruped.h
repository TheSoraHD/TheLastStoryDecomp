#ifndef CHR_CONTROLTURNQUADRUPED_H
#define CHR_CONTROLTURNQUADRUPED_H

#include "chr/ControlBase.h"

namespace chr {

class ControlTurnQuadruped : public ControlBase {
public:
    ControlTurnQuadruped(Chara_unk* chara, double angle); // 0x801C8F10
    virtual ~ControlTurnQuadruped() {}
    virtual int vf0C();

    float mAngle; // 0x08
    int mLeft;    // 0x0C: inferred: 1 when turning with a negative angle
    Vec mRot;     // 0x10: rotation of the character at the start
};

} // namespace chr

#endif
