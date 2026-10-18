// GimmickCommon (0x803EC568-0x803EDF7C, inferred). Not a unit yet: this code is still inside the
// auto split auto_03_803E836C, whose file boundaries are not known. The constructor and the
// destructor compile to the same instructions as the DOL in a standalone compile (only the
// relocation names differ).

#include "game/GimmickCommon.h"

GimmickCommon::GimmickCommon(atn::Task* parent, int id, int arg)
    : atn::Task(parent), mId(id), m4C(arg), m50(id / 1000), m54(0), m58(0), m5C(0), m68(0),
      m6C(0.0f, 0.0f, 0.0f), m78(0.0f, 0.0f, 0.0f), m84(0.0f, 0.0f, 0.0f), m90(1.0f, 1.0f, 1.0f),
      m9C(1), mA0(0), mA4(0), mA8(0), mAC(0), mE4(0), mE8(-1), mEC(-1) {
    if (mId < 1000) {
        m50 = mId;
        mId *= 1000;
    }
    if (lbl_8087F4A0 != 0) {
        lbl_8087F4A0->fn_803EE3F0(this);
    }
}

GimmickCommon::~GimmickCommon() {
    if (lbl_8087F4A0 != 0) {
        lbl_8087F4A0->fn_803EE41C(this);
    }
}
