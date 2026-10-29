#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" int sprintf(char* dst, const char* fmt, ...);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/bgm_id_table.csv (0x28 bytes, no constructor: __nwa without
// __construct_new_array)
struct BgmIdEntry_unk {
    int mId;
    char mName[0x20];
    int m24;
};

static const char sFileNameStr[] = "/boot/bgm_id_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer
// inferred: returned when the id is not found (the literal goes to .sdata2, 0x80882D74)
static const char* const sEmpty = "";

static BgmIdEntry_unk* sEntries;
static int sCount;

// inferred: loads /boot/bgm_id_table.csv (called from fn_80207F80)
extern "C" void fn_80207CC0() {
    atn::Csv csv;
    const char* field;
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strlen(field) == 0) {
                continue;
            }
            if (atoi(field) > 0) {
                sCount++;
            }
        }
        sEntries = new (12, "", "", 0) BgmIdEntry_unk[sCount];
        csv.SetData((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strlen(field) == 0) {
                continue;
            }
            if (atoi(field) <= 0) {
                continue;
            }
            BgmIdEntry_unk* entry = &sEntries[i];
            entry->mId = atoi(csv.mField);
            sprintf(entry->mName, "%s", csv.fn_8005B9CC());
            csv.fn_8005B9CC(); // inferred: a column that is not used
            entry->m24 = atoi(csv.fn_8005B9CC());
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: name of a BGM id (used by GameBgm)
extern "C" const char* fn_80207EE8(int id) {
    for (int i = 0; i < sCount; i++) {
        if (id == sEntries[i].mId) {
            return sEntries[i].mName;
        }
    }
    return sEmpty;
}

extern "C" int fn_80207F34(int id) {
    for (int i = 0; i < sCount; i++) {
        if (id == sEntries[i].mId) {
            return sEntries[i].m24;
        }
    }
    return 0;
}
