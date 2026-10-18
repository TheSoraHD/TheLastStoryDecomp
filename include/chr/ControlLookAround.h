#ifndef CHR_CONTROLLOOKAROUND_H
#define CHR_CONTROLLOOKAROUND_H

#include "chr/ControlBase.h"

namespace chr {

class ControlLookAround : public ControlBase {
public:
    ControlLookAround(Chara_unk* chara, int time); // 0x801BC288
    virtual ~ControlLookAround() {}
    virtual int vf0C();

    int mTime; // 0x08
};

class ControlLookAroundEgg : public ControlBase {
public:
    ControlLookAroundEgg(Chara_unk* chara, int time); // 0x801BC410
    virtual ~ControlLookAroundEgg() {}
    virtual int vf0C();

    int mTime; // 0x08
};

} // namespace chr

#endif
