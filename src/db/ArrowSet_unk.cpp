#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/Array_unk.h"
#include "std/vector.h"

extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s); // inferred: string to int

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/arrow_set.csv (0xC bytes): an id and the list of numbers that
// follow it (constructor fn_802077F8 and destructor fn_80207808 are the implicit ones)
struct ArrowSetEntry_unk {
    int mId;
    atn::SimpleArray<int> mValues;
};

// inferred: the destructor of the local list tests `this` three times (one level more than a
// plain std::vector)
struct IntList_unk : public std::vector<int> {};

static const char sFileNameStr[] = "/boot/arrow_set.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static ArrowSetEntry_unk* sEntries;
static int sCount;

// inferred: loads /boot/arrow_set.csv
// NONMATCHING: 99.21% - fast path of the inlined std::vector::push_back: the original loads the
// size before the capacity (known problem of std/vector.h; tried 8 forms of the comparison)
extern "C" void fn_80207310() {
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        atn::Csv csv((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == '\0' || *field == ';') {
                continue;
            }
            sCount++;
        }
        sEntries = new ((MemoryTag)3, "", "", 0) ArrowSetEntry_unk[sCount];
        csv.SetData((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == '\0' || *field == ';') {
                continue;
            }
            ArrowSetEntry_unk* entry = &sEntries[i];
            entry->mId = fn_800DC12C(field);
            csv.fn_8005B9CC(); // inferred: a column that is not used
            IntList_unk values;
            for (;;) {
                const char* value = csv.fn_8005B9CC();
                if (*value == '\0') {
                    break;
                }
                values.push_back(atoi(value));
            }
            entry->mValues.Resize_unk(values.size(), 1);
            for (unsigned int j = 0; j < values.size(); j++) {
                entry->mValues.mData[j] = values[j];
            }
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

extern "C" ArrowSetEntry_unk* fn_8020787C(int id) {
    for (int i = 0; i < sCount; i++) {
        if (id == sEntries[i].mId) {
            return &sEntries[i];
        }
    }
    return 0;
}
