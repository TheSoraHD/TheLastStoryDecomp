#ifndef EVENT_DEBUGINFO_UNK_H
#define EVENT_DEBUGINFO_UNK_H

#include "std/vector_string.h"

namespace Event {

// inferred: column of the debug display: texts of the rows (0x14 bytes; destructor inlined in the
// destructor of Event::Manager). Names invented.
struct DebugInfoColumn_unk {
    DebugInfoColumn_unk() : m00(0), m10(-1) {} // inferred: weak copy 0x80223DDC (Event/DebugInfo_unk.cpp)
    int m00;
    std::vector<std::string> mTexts; // 0x04
    int m10;
};

// inferred: column definition of a table of the debug display (0x10 bytes, tables at 0x807835F0;
// the first three words are zero in the data). Names invented.
struct DebugInfoColumnDef_unk {
    int m00;
    int m04;
    int m08;
    float mWidth; // inferred: 50, 60, 220, 100...
};

// inferred: table of the debug display (0x18 bytes). Names invented.
struct DebugInfoTable_unk {
    DebugInfoTable_unk() : m00(0), m04(0), m14(0) {}
    void fn_80220168(const DebugInfoColumnDef_unk* defs, int count, int rows); // inferred: sets the columns
    void fn_8022051C(const DebugInfoColumn_unk& column); // inferred: adds a column of texts
    void fn_802219CC(float x, float y, float h, int arg); // inferred: draws the table
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
    float m04;                  // inferred: position of the display (30, 30)
    float m08;
    float m0C;                  // inferred: height of a row (14)
    int m10;
    DebugInfoTable_unk m14;
    DebugInfoTable_unk m2C;
    DebugInfoTable_unk m44;
    void (*mPages[6])(DebugInfo_unk* info); // 0x5C: inferred: draw function of each page (set by the constructor)
    int m74;
};

} // namespace Event

#endif
