#ifndef EVENT_DEBUGINFO_UNK_H
#define EVENT_DEBUGINFO_UNK_H

#include "std/vector_string.h"

namespace Event {

// inferred: column of the debug display: texts of the rows (0x14 bytes; destructor inlined in the
// destructor of Event::Manager). Names invented.
struct DebugInfoColumn_unk {
    int m00;
    std::vector<std::string> mTexts; // 0x04
    int m10;
};

// inferred: table of the debug display (0x18 bytes). Names invented.
struct DebugInfoTable_unk {
    int m00;
    int m04;
    std::vector<DebugInfoColumn_unk> mColumns; // 0x08
    int m14;
};

// inferred: debug display of the event (0x78 bytes, Event/DebugInfo_unk.cpp; constructor 0x80221E08).
// Names invented.
struct DebugInfo_unk {
    DebugInfo_unk();
    void fn_80221F54(int page); // inferred: draws the page `page` of the debug information (Manager::vf14)
    int mPage;                  // 0x00: inferred: page shown last
    unsigned char m04[0x14 - 0x04];
    DebugInfoTable_unk m14;
    DebugInfoTable_unk m2C;
    DebugInfoTable_unk m44;
    void (*mPages[6])(DebugInfo_unk* info); // 0x5C: inferred: draw function of each page (set by the constructor)
    int m74;
};

} // namespace Event

#endif
