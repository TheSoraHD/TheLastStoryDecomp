#ifndef CHR_CONTROLCAUGHT_H
#define CHR_CONTROLCAUGHT_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: these classes declare no destructor (implicit destructors at 0x801A2C6C, 0x801A2CAC)

// inferred: held by another character; mCount is decreased by button presses (0x801A2468)
class ControlCaught : public ControlBase {
public:
    ControlCaught(Chara_unk* chara, int arg, int count, int motion, int releaseMotion); // 0x801A2290
    virtual int vf0C();                     // 0x801A2414
    virtual boost::function<void()> vf10(); // 0x801A24E4
    virtual void vf1C();                    // 0x801A24E0

    int m08;            // 0x08: also stored in Chara_unk::mF1C
    int mCount;         // 0x0C
    int mMotion;        // 0x10
    int mReleaseMotion; // 0x14
    bool m18;           // 0x18: inferred: button presses count down mCount
    bool m19;           // 0x19: inferred: finishes when mCount reaches 0
    bool m1A;           // 0x1A
};

// inferred: released by the holder (motions 0x2E/0x31 or 0x68/0x69)
class ControlRelease : public ControlBase {
public:
    ControlRelease(Chara_unk* chara, int motion); // 0x801A2A48
    virtual int vf0C();                           // 0x801A2B58

    int m08; // 0x08: bit 0x20 of Chara_unk::m7E0
};

} // namespace chr

#endif
