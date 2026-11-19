#ifndef ATN_D2A_MESSAGETABLE_UNK_H
#define ATN_D2A_MESSAGETABLE_UNK_H

#include "atn/LH_Binary.h"
#include "atn/Memory.h"

// Table of texts by id of the d2anime menus (atn/D2A_MessageTable_unk.cpp): a UTF-16 CSV (id, text)
// read with atn::CsvU into one text buffer. Names invented.

// inferred: text of an id (8 bytes)
struct D2A_TextEntry_unk {
    unsigned int mId;
    wchar_t* mText;
};

// inferred: array of entries, released by its owner
struct D2A_TextEntries_unk {
    D2A_TextEntries_unk() : mCount(0), mEntries(0) {}
    ~D2A_TextEntries_unk() { Clear(); }
    void Clear() {
        if (mEntries != 0) {
            delete[] mEntries;
        }
        mEntries = 0;
        mCount = 0;
    }
    void Alloc(unsigned int count) {
        if (mEntries != 0) {
            delete[] mEntries;
        }
        mCount = count;
        if (count != 0) {
            mEntries = new ((MemoryTag)8, "", "", 0) D2A_TextEntry_unk[count];
        } else {
            mEntries = 0;
        }
    }

    unsigned int mCount;          // 0x00
    D2A_TextEntry_unk* mEntries;  // 0x04
};

// inferred: the table (0x18 bytes; atn::D2A_Manager::mTexts)
struct D2A_MessageTable_unk {
    D2A_MessageTable_unk();
    ~D2A_MessageTable_unk();

    D2A_TextEntries_unk mEntries; // 0x00
    int mState;                   // 0x08: inferred: 1 loading, 2 loaded
    atn::LH_Binary mLoader;       // 0x0C
    wchar_t* mText;               // 0x14
};

#endif
