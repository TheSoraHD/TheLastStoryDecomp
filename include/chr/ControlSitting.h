#ifndef CHR_CONTROLSITTING_H
#define CHR_CONTROLSITTING_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: moves the character onto a seat in 8 frames and plays the sitting motions
class ControlSitting : public ControlBase {
public:
    ControlSitting(Chara_unk* chara, const Vec& start, const Vec& seat, const Vec& rot); // 0x801C9B24
    virtual ~ControlSitting() {}
    virtual int vf0C();

    Vec mStart;     // 0x08
    Vec mSeat;      // 0x14: y taken from mStart
    Vec mStartRot;  // 0x20
    Vec mRot;       // 0x2C
    int mCount;     // 0x38
};

} // namespace chr

#endif
