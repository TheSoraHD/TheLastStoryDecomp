// Debug display of the events (Event::DebugInfo_unk, Event::Manager + 0x1DFC). Only part of the unit is
// written (the pages 0x80221FA4-0x80224A2C and their tables are missing), so it is not linked.
#include "Event/DebugInfo_unk.h"
#include "game/GameConfig_unk.h"

extern "C" GameConfig_unk* lbl_8087F0A8;

namespace Event {

// inferred: draw functions of the pages (not written yet)
void fn_80221FA4(DebugInfo_unk* info);
void fn_80222AD0(DebugInfo_unk* info);
void fn_8022354C(DebugInfo_unk* info);
void fn_80223A10(DebugInfo_unk* info);
void fn_80224184(DebugInfo_unk* info);
void fn_80224638(DebugInfo_unk* info);

// inferred: columns of the three tables (6, 6 and 10 columns)
static DebugInfoColumnDef_unk sColumns0_unk[6] = {
    {0, 0, 0, 50.0f}, {0, 0, 0, 60.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 60.0f}, {0, 0, 0, 220.0f}, {0, 0, 0, 100.0f},
};
static DebugInfoColumnDef_unk sColumns1_unk[6] = {
    {0, 0, 0, 70.0f}, {0, 0, 0, 60.0f}, {0, 0, 0, 180.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 80.0f}, {0, 0, 0, 100.0f},
};
static DebugInfoColumnDef_unk sColumns2_unk[10] = {
    {0, 0, 0, 80.0f}, {0, 0, 0, 70.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 40.0f}, {0, 0, 0, 70.0f}, {0, 0, 0, 50.0f},
    {0, 0, 0, 40.0f}, {0, 0, 0, 70.0f}, {0, 0, 0, 50.0f}, {0, 0, 0, 40.0f},
};

// NONMATCHING: 90% - same instructions; the original keeps -1 in r31, 0 in r30 and the column table in r29
// (ours: table r31, -1 r30, 0 r29). Tried the table as a static/global array, a local pointer, decayed array.
DebugInfo_unk::DebugInfo_unk() : mPage(0), m04(30.0f), m08(30.0f), m0C(14.0f), m10(-1), m74(0) {
    m14.fn_80220168(sColumns0_unk, 6, 25);
    m2C.fn_80220168(sColumns1_unk, 6, 25);
    m44.fn_80220168(sColumns2_unk, 10, 25);
    m10 = -1;
    m74 = 0;
    mPages[0] = fn_80221FA4;
    mPages[1] = fn_80222AD0;
    mPages[2] = fn_8022354C;
    mPages[3] = fn_80223A10;
    mPages[4] = fn_80224184;
    mPages[5] = fn_80224638;
}

void DebugInfo_unk::fn_80221F54(int page) {
    if (page >= 6) {
        return;
    }
    if (mPages[page] == 0) {
        return;
    }
    if (mPage != page) {
        lbl_8087F0A8->mDrawSystemInfo = 0;
    }
    mPage = page;
    mPages[mPage](this);
}

} // namespace Event
