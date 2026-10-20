#ifndef CHR_CONTROLDETECT_H
#define CHR_CONTROLDETECT_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: controls of chr/ControlDetect.cpp created by CharaTask (fn_80164F54, fn_80165284).
// Only the constructors are declared; sizes from the operator new calls. Both set CharaTask::m560
// to 0x2F.

class ControlDetect : public ControlBase {
public:
    ControlDetect(Chara_unk* chara, int arg); // 0x801B0710
    virtual int vf0C();
    virtual int vf3C() { return 1; } // inferred: weak copy 0x801B0D00
    int m08; // inferred: the constructor's second argument (frames left)
};

class ControlDetectHateAtt : public ControlBase {
public:
    ControlDetectHateAtt(Chara_unk* chara); // 0x801B0854
    virtual int vf0C();
};

} // namespace chr

#endif
