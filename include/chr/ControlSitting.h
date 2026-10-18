#ifndef CHR_CONTROLSITTING_H
#define CHR_CONTROLSITTING_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: moves the character onto a seat in 8 frames and plays the sitting motions
class ControlSitting : public ControlBase {
public:
    ControlSitting(Chara_unk* chara, const atn::Vector3& start, const atn::Vector3& seat, const atn::Vector3& rot); // 0x801C9B24
    virtual ~ControlSitting() {}
    virtual int vf0C();

    atn::Vector3 mStart;     // 0x08
    atn::Vector3 mSeat;      // 0x14: y taken from mStart
    atn::Vector3 mStartRot;  // 0x20
    atn::Vector3 mRot;       // 0x2C
    int mCount;     // 0x38
};

} // namespace chr

#endif
