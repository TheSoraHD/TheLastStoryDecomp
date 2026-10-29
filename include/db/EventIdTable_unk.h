#ifndef DB_EVENTIDTABLE_UNK_H
#define DB_EVENTIDTABLE_UNK_H

#include "atn/Array_unk.h"
#include "gmk/SeTable.h"

// inferred: one row of /boot/event_id_table.csv (0x60 bytes)
struct EventIdEntry_unk {
    ~EventIdEntry_unk() {}

    int mId;
    atn::FixedString_unk<0x40> mName;
    int m44; // inferred: "CAPTURE" 1, "CAMERA" 2, "TV" 3, otherwise 0
    int m48; // inferred: same kinds as m44
    unsigned int mFlags; // inferred: bits 0-3 and 4 from numeric columns
    int m50; // inferred: "CAMERA" 1, "ADD" 2, otherwise 0
    float m54;
    float m58; // inferred: 1.0f for an empty field
    float m5C; // inferred: 1.0f for an empty field
};

// inferred: the event id table (singleton returned by fn_8020F130). The original string pool also
// holds the basic_string error messages (some std::string use that is not reconstructed here)
struct EventIdTable_unk {
    static EventIdTable_unk* fn_8020F130();
    void fn_8020F248();
    EventIdEntry_unk* fn_8020F71C(int id);
    int fn_8020F75C(const char* name);

    atn::SimpleArray<EventIdEntry_unk> mEntries;
};

#endif
