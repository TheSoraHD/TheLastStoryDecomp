// inferred: file boundaries from the per-file constants (.sdata2 0x80885F50-0x80885F88) and the
// gimmick factory jump tables (0x8078C6C8); the file also holds the factories at 0x803EC16C/0x803EC374.

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
