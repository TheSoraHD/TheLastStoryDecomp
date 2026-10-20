#include "db/EventIdTable_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strstr(const char* s, const char* sub);
extern "C" int atoi(const char* s);
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: loads a whole file / releases it
extern "C" void* fn_8006BA8C(const char* path, unsigned long* size, int arg);
extern "C" void fn_8006BB6C(void* data, int arg);

static const char sFileNameStr[] = "/boot/event_id_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

EventIdTable_unk* EventIdTable_unk::fn_8020F130() {
    static EventIdTable_unk sInstance;
    return &sInstance;
}

// inferred: kind of a column ("CAPTURE" 1, "CAMERA" 2, "TV" 3, otherwise 0)
static inline int ParseKind_unk(const char* kind) {
    if (strcmp(kind, "CAPTURE") == 0) {
        return 1;
    }
    if (strcmp(kind, "CAMERA") == 0) {
        return 2;
    }
    return strcmp(kind, "TV") ? 0 : 3;
}

// inferred: "CAMERA" 1, "ADD" 2, otherwise 0
static inline int ParseMode_unk(const char* mode) {
    if (strcmp(mode, "CAMERA") == 0) {
        return 1;
    }
    return strcmp(mode, "ADD") ? 0 : 2;
}

// inferred: loads /boot/event_id_table.csv
// NONMATCHING: 97.86% - register allocation only: the original keeps `this` in r24 and the zero of
// size/i in r23 (the zero stored to mFlags is a copy of it), here the zero of the entry constructor
// and mFlags share a register (tried moving the declarations of count, i and entry)
void EventIdTable_unk::fn_8020F248() {
    unsigned long size = 0;
    void* data = fn_8006BA8C(sFileName, &size, 0);
    if (data == 0) {
        return;
    }
    atn::Csv csv;
    int count = 0;
    csv.vf08((const char*)data, size);
    while (csv.fn_8005BBF8() != 0) {
        const char* field = csv.fn_8005B9CC();
        if (*field == '\0' || *field == '#') {
            continue;
        }
        count++;
    }
    mEntries.Resize_unk(count, 4);
    EventIdEntry_unk entry;
    int i = 0;
    csv.vf08((const char*)data, size);
    while (csv.fn_8005BBF8() != 0) {
        const char* field = csv.fn_8005B9CC();
        if (*field == '\0' || *field == '#') {
            continue;
        }
        entry.mId = atoi(field);
        csv.fn_8005B9CC(); // inferred: a column that is not used
        entry.mName = csv.fn_8005B9CC();
        entry.m44 = ParseKind_unk(csv.fn_8005B9CC());
        entry.m48 = ParseKind_unk(csv.fn_8005B9CC());
        entry.mFlags = 0;
        entry.mFlags |= atoi(csv.fn_8005B9CC()) != 0;
        entry.mFlags |= atoi(csv.fn_8005B9CC()) != 0 ? 2 : 0;
        entry.mFlags |= atoi(csv.fn_8005B9CC()) != 0 ? 4 : 0;
        entry.mFlags |= atoi(csv.fn_8005B9CC()) != 0 ? 8 : 0;
        entry.m50 = ParseMode_unk(csv.fn_8005B9CC());
        entry.m54 = fn_800DC288(csv.fn_8005B9CC());
        entry.m58 = strcmp(csv.fn_8005B9CC(), "") == 0 ? 1.0f : fn_800DC288(csv.mField);
        entry.m5C = strcmp(csv.fn_8005B9CC(), "") == 0 ? 1.0f : fn_800DC288(csv.mField);
        entry.mFlags |= atoi(csv.fn_8005B9CC()) != 0 ? 0x10 : 0;
        mEntries.mData[i] = entry;
        i++;
    }
    fn_8006BB6C(data, 0);
}

EventIdEntry_unk* EventIdTable_unk::fn_8020F71C(int id) {
    for (int i = 0; i < mEntries.mSize; i++) {
        EventIdEntry_unk* entry = &mEntries.mData[i];
        if (id == entry->mId) {
            return entry;
        }
    }
    return 0;
}

// inferred: id of the first entry whose name appears in `name`, -1 if none
// NONMATCHING: 99.24% - the original increments i before the byte offset (tried for/while forms,
// declaration order, pointer arithmetic)
int EventIdTable_unk::fn_8020F75C(const char* name) {
    int count = mEntries.mSize;
    EventIdEntry_unk* entry;
    for (int i = 0; i < count; i++) {
        entry = &mEntries.mData[i];
        if (strstr(name, entry->mName.c_str()) != 0) {
            return entry->mId;
        }
    }
    return -1;
}
