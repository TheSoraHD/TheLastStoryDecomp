#include "atn/D2A_Common.h"
#include "std/string.h"

// Keyed parameter tables of the D2A elements: {key, value} pairs sorted by key, with a setter and
// an interpolated getter for each value type (int, float, wide string). Name of the file invented
// (between atn_d2a_frame and atn_d2a_line by the order of the data).
// The int and string versions keep their automatic extern "C" names (callers in D2A_Common,
// D2A_Message, D2A_Data, D2A_Texture and MenuCommonTask use them that way).

// inferred: int table (D2A_Common 0xA4, D2A_Texture); names invented
struct D2A_IntParamTable_unk {
    struct Entry {
        int mKey;
        int mValue;
    };
    Entry* mEntries; // 0x00
    int mCount;      // 0x04
};

// inferred: wide string table (D2A_Message); names invented
struct D2A_StringParamTable_unk {
    struct Entry {
        int mKey;
        std::basic_string<wchar_t> mValue;
    };
    Entry* mEntries; // 0x00
    int mCount;      // 0x04
};

// inferred: sets the value of an existing key; `arg` is not used (callers pass 1)
extern "C" void fn_801F0364(D2A_IntParamTable_unk* table, int key, int value, int arg) {
    for (int i = 0; i < table->mCount; i++) {
        if (key == table->mEntries[i].mKey) {
            table->mEntries[i].mValue = value;
            return;
        }
    }
}

// inferred: value at `key`, linearly interpolated between the keys around it
extern "C" int fn_801F03AC(D2A_IntParamTable_unk* table, int key) {
    if (table->mCount == 0) {
        return 0;
    }
    if (table->mCount <= 1 || key <= table->mEntries[0].mKey) {
        return table->mEntries[0].mValue;
    }
    D2A_IntParamTable_unk::Entry* prev = 0;
    D2A_IntParamTable_unk::Entry* next = table->mEntries;
    bool found = false;
    for (int i = 0; i < table->mCount; i++) {
        prev = next;
        next = &table->mEntries[i];
        if (key <= next->mKey) {
            found = true;
            break;
        }
    }
    if (!found) {
        return next->mValue;
    }
    int result = 0;
    if (prev != 0 && next == 0) {
        result = prev->mValue;
    } else if (prev == 0 && next != 0) {
        result = next->mValue;
    } else if (prev != 0 && next != 0) {
        float t = (float)(key - prev->mKey) / (float)(next->mKey - prev->mKey);
        result = prev->mValue + (int)(t * (float)(next->mValue - prev->mValue));
    }
    return result;
}

namespace atn {

void D2A_ParamTable::fn_801F04FC(int key, int arg, float value) {
    for (int i = 0; i < mCount; i++) {
        if (key == mEntries[i].mKey) {
            mEntries[i].mValue = value;
            return;
        }
    }
}

} // namespace atn

// inferred: value at `key`, linearly interpolated between the keys around it
extern "C" float fn_801F0544(atn::D2A_ParamTable* table, int key) {
    if (table->mCount == 0) {
        return 0.0f;
    }
    if (table->mCount <= 1 || key <= table->mEntries[0].mKey) {
        return table->mEntries[0].mValue;
    }
    atn::D2A_ParamTable::Entry* prev = 0;
    atn::D2A_ParamTable::Entry* next = table->mEntries;
    bool found = false;
    for (int i = 0; i < table->mCount; i++) {
        prev = next;
        next = &table->mEntries[i];
        if (key <= next->mKey) {
            found = true;
            break;
        }
    }
    if (!found) {
        return next->mValue;
    }
    float result = 0.0f;
    if (prev != 0 && next == 0) {
        result = prev->mValue;
    } else if (prev == 0 && next != 0) {
        result = next->mValue;
    } else if (prev != 0 && next != 0) {
        float t = (float)(key - prev->mKey) / (float)(next->mKey - prev->mKey);
        result = prev->mValue + t * (next->mValue - prev->mValue);
    }
    return result;
}

// inferred: sets the string of an existing key; `arg` is not used
extern "C" void fn_801F0670(D2A_StringParamTable_unk* table, int key, const std::basic_string<wchar_t>& value, int arg) {
    for (int i = 0; i < table->mCount; i++) {
        if (key == table->mEntries[i].mKey) {
            table->mEntries[i].mValue = value;
            return;
        }
    }
}

// inferred: strings are not interpolated: the one of the previous key (inline, name invented; the
// order of its iterator-tag temporaries on the stack is the ascending one of an inlined function)
inline void fn_SelectValue_unk(std::basic_string<wchar_t>& result, D2A_StringParamTable_unk::Entry* prev,
                               D2A_StringParamTable_unk::Entry* next) {
    if (prev != 0) {
        result = prev->mValue;
    } else if (next != 0) {
        result = next->mValue;
    }
}

// inferred: string at `key` (the one of the previous key; strings are not interpolated)
extern "C" std::basic_string<wchar_t> fn_801F0758(D2A_StringParamTable_unk* table, int key) {
    if (table->mCount == 0) {
        return std::basic_string<wchar_t>();
    }
    if (table->mCount <= 1 || key <= table->mEntries[0].mKey) {
        return table->mEntries[0].mValue;
    }
    D2A_StringParamTable_unk::Entry* prev = 0;
    D2A_StringParamTable_unk::Entry* next = table->mEntries;
    bool found = false;
    for (int i = 0; i < table->mCount; i++) {
        prev = next;
        next = &table->mEntries[i];
        if (key <= next->mKey) {
            found = true;
            break;
        }
    }
    if (!found) {
        return next->mValue;
    }
    std::basic_string<wchar_t> result;
    result.clear();
    fn_SelectValue_unk(result, prev, next);
    return result;
}
