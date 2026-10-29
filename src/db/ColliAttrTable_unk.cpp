#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/colli_attr_table.csv (0x80 bytes, indexed by the attribute id)
struct ColliAttrEntry_unk {
    char mSe[3][0x20]; // inferred: names for the ATR_*_SE kinds
    char mEff[0x20];   // inferred: name for ATR_EFF
};

// inferred: kind names (the index is the column of ColliAttrEntry_unk::mSe)
static const char sSeKinds[3][0x20] = {"ATR_HUMAN_SE", "ATR_CREATURE_SE", "ATR_MACHINE_SE"};
static const char sEffKinds[1][0x20] = {"ATR_EFF"};
static const char sFileNameStr[] = "/boot/colli_attr_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static ColliAttrEntry_unk* sEntries;

// inferred: loads /boot/colli_attr_table.csv
extern "C" void fn_8020AFF4() {
    unsigned long size = 0;
    atn::Csv csv;
    void* data;
    if (sEntries == 0) {
        sEntries = new (1, "", "", 0) ColliAttrEntry_unk[0x21];
        for (int i = 0; i < 0x21; i++) {
            ColliAttrEntry_unk* entry = &sEntries[i];
            for (int j = 0; j < 3; j++) {
                memset(entry->mSe[j], 0, 0x20);
            }
            memset(entry->mSe[0], 0, 0x20);
        }
        data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            int id = fn_800DC12C(csv.fn_8005B9CC());
            csv.fn_8005B9CC(); // inferred: a column that is not used
            if (0 > id || id >= 0x21) {
                continue;
            }
            ColliAttrEntry_unk* entry = &sEntries[id];
            strncpy(entry->mEff, csv.fn_8005B9CC(), 0x20);
            strncpy(entry->mSe[0], csv.fn_8005B9CC(), 0x20);
            strncpy(entry->mSe[1], csv.fn_8005B9CC(), 0x20);
            strncpy(entry->mSe[2], csv.fn_8005B9CC(), 0x20);
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

extern "C" ColliAttrEntry_unk* fn_8020B1E0(int id) {
    if (0 > id || id >= 0x21) {
        return 0;
    }
    return &sEntries[id];
}

// inferred: index of a SE kind name
extern "C" int fn_8020B208(const char* name) {
    for (int i = 0; i < 3; i++) {
        if (strcmp(sSeKinds[i], name) == 0) {
            return i;
        }
    }
    return 0;
}

// inferred: index of an effect kind name (only one kind)
extern "C" int fn_8020B27C(const char* name) {
    for (int i = 0; i < 1; i++) {
        if (strcmp(sEffKinds[i], name) == 0) {
            return i;
        }
    }
    return 0;
}
