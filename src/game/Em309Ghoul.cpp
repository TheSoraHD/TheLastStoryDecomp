#include "game/Em309Ghoul.h"
#include "math/Vec.h"

// inferred: field (lbl_8087F430); the event flags are read with fn_80370A78
struct Field_unk;
extern "C" Field_unk* lbl_8087F430;
extern "C" int fn_80370A78(Field_unk* field, int index);

Em309Ghoul::~Em309Ghoul() {}

atn::Vector3 Em309Ghoul::vf64(int center) {
    atn::Vector3 result = mPos;
    if (center == 0) {
        result = mPos;
    } else if (fn_80370A78(lbl_8087F430, 0xA1) == 0) {
        result = mPos;
    } else {
        result = m624.mData[1].m04.mCenter;
    }
    return result;
}
