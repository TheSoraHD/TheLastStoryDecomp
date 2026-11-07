// Debug display of the events (Event::DebugInfo_unk, Event::Manager + 0x1DFC). Only part of the unit is
// written (the pages 0x80221FA4-0x80224A2C and their tables are missing), so it is not linked.
#include "Event/DebugInfo_unk.h"
#include "game/GameConfig_unk.h"

extern "C" GameConfig_unk* lbl_8087F0A8;

namespace Event {

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
