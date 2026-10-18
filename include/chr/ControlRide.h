#ifndef CHR_CONTROLRIDE_H
#define CHR_CONTROLRIDE_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: these classes declare no destructor (implicit destructors at 0x801C7898, 0x801C78D8)

// inferred: rider sitting on a beast; ends when the beast stops (m48 == 0 and mD18 == 0)
class ControlRide : public ControlBase {
public:
    ControlRide(Chara_unk* chara, Chara_unk* beast) : ControlBase(chara), mBeast(beast), mState(0) {
        chara->m560 = 0x79;
    }
    virtual int vf0C();  // 0x801C6F90
    virtual void vf1C(); // 0x801C702C

    Chara_unk* mBeast; // 0x08
    int mState;        // 0x0C
};

// inferred: beast moving under its rider to the rider's "seatpoint" node (motion 0x1F2)
class ControlRideBeast : public ControlBase {
public:
    ControlRideBeast(Chara_unk* chara, Chara_unk* rider); // 0x801C706C
    virtual int vf0C();                          // 0x801C71A8
    virtual boost::function<void()> vf10();      // 0x801C7390
    virtual void vf1C();                         // 0x801C7350

    Chara_unk* mBeast;  // 0x08: inferred: the same character as mChara
    Chara_unk* mRider;  // 0x0C
    atn::Vector3 mStartPos;      // 0x10
    atn::Vector3 mSeatPos;       // 0x1C
    atn::Vector3 mStartRot;      // 0x28
    atn::Vector3 mRiderRot;      // 0x34
};

} // namespace chr

#endif
