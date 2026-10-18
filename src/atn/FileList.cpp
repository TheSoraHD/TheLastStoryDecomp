#include "atn/FileList.h"

// NOTE: both functions match 100%, but the unit is not linked yet: its .rodata starts at a
// 4-aligned address (0x8073147C) and the data right after it (0x8073148C) is used by the
// functions around 0x8006AC08-0x8006BF00, so the real file is larger than this split.

namespace atn {

FileList::~FileList() {
    m04 = 0;
    mEntries.clear();
}

FileList::FileList() {
    m04 = 0;
    mEntries.__shrink_unk(0); // inferred: some vector call that destroys the (zero) elements
}

} // namespace atn
