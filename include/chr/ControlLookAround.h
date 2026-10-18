#ifndef CHR_CONTROLLOOKAROUND_H
#define CHR_CONTROLLOOKAROUND_H

#include "chr/ControlBase.h"

namespace chr {

// inferred: these classes declare no destructor; implicit destructors are emitted at the end of the
// unit in class order (0x801BC538), inline ones in reverse order

class ControlLookAround : public ControlBase {
public:
    ControlLookAround(Chara_unk* chara, int time); // 0x801BC288
    virtual int vf0C();

    int mTime; // 0x08
};

class ControlLookAroundEgg : public ControlBase {
public:
    ControlLookAroundEgg(Chara_unk* chara, int time); // 0x801BC410
    virtual int vf0C();

    int mTime; // 0x08
};

} // namespace chr

#endif
