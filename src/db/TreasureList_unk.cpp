#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int atoi(const char* s);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/treasure_list.csv (0x10 bytes, no constructor)
struct TreasureListEntry_unk {
    int mId;
    int m04;
    int m08;
    int m0C;
};

static const char sFileNameStr[] = "/boot/treasure_list.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static TreasureListEntry_unk* sEntries;
static int sCount;

// inferred: loads /boot/treasure_list.csv (rows whose first column is empty)
extern "C" void fn_8021EB04() {
    atn::Csv csv;
    if (sEntries == 0) {
        unsigned long size;
        int ofs;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() == '\0') {
                sCount++;
            }
        }
        sEntries = new (12, "", "", 0) TreasureListEntry_unk[sCount];
        ofs = 0;
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != '\0') {
                continue;
            }
            TreasureListEntry_unk* entry = (TreasureListEntry_unk*)((char*)sEntries + ofs);
            entry->mId = atoi(csv.fn_8005B9CC());
            csv.fn_8005B9CC(); // inferred: a column that is not used
            entry->m04 = atoi(csv.fn_8005B9CC());
            entry->m08 = atoi(csv.fn_8005B9CC());
            entry->m0C = atoi(csv.fn_8005B9CC());
            ofs += sizeof(TreasureListEntry_unk);
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: entry by id (0 if there is none)
extern "C" TreasureListEntry_unk* fn_8021ECD0(int id) {
    TreasureListEntry_unk* entry = sEntries;
    for (int i = 0; i < sCount; i++, entry++) {
        if (entry->mId == id) {
            return entry;
        }
    }
    return 0;
}
