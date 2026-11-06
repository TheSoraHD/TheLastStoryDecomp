#ifndef MP_MPMESSAGE_UNK_H
#define MP_MPMESSAGE_UNK_H

// inferred: one message of the multiplayer message table (0x1C bytes)
struct MpMessage_unk {
    unsigned int mVoice;  // inferred: sound id played when it is selected (0 = none)
    unsigned int m04;     // inferred: bits 8+: categories of the lobby list where the message appears
    unsigned int m08;     // inferred: top byte: character of the message (19 = generic, 20+ = cannot be chosen)
    int m0C;
    int m10;              // inferred: character id of the speaker (0 = none)
    int m14;
    const wchar_t* mText; // 0x18
};
// inferred: categories of the message table (0xC bytes each)
struct MpMessageCategory_unk {
    unsigned int mCount;
    int m04;
    MpMessage_unk* mData;
};
// inferred: message table of the session (lbl_8087F610 + 0x2A50); names invented. fn_804C54FC is
// inline: MP/D2AMpMenuSetMessage expands it (and its recursive call) in place and
// MP/D2AMpMenuMessageList emits the out-of-line copy after its first user
struct MpMessageTable_unk {
    // inferred: message `index` of `category` (21 = every category in a row)
    MpMessage_unk* fn_804C54FC(int category, int index) {
        if (category < 0 || 21 < category) {
            return 0;
        }
        if (category < 21) {
            if (index < 0 || (int)mCategories[category].mCount <= index) {
                return 0;
            }
            return &mCategories[category].mData[index];
        }
        if (index < 0) {
            return 0;
        }
        int i;
        for (i = 0; i < 20 && mCategories[i].mCount <= (unsigned int)index; i++) {
            index -= mCategories[i].mCount;
        }
        if (i < 20) {
            return fn_804C54FC(i, index);
        }
        return 0;
    }
    // inferred: number of messages of `category` (21 = every category)
    int GetCount_unk(int category) {
        if (category < 0 || 21 < category) {
            return 0;
        }
        if (category < 21) {
            return mCategories[category].mCount;
        }
        int count = 0;
        for (int i = 0; i < 20; i++) {
            count += mCategories[i].mCount;
        }
        return count;
    }
    MpMessageCategory_unk mCategories[21];
};

#endif
