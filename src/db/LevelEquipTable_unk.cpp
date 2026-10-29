#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of /boot/level_equip_table.csv (0x24 bytes, no constructor). The ids are
// written with a two letter prefix that is skipped.
struct LevelEquipEntry_unk {
    int mIds[2];
    int mValues[2][3];
    int m20;
};

// inferred: MIN/MAX macros (the clamped value is computed twice)
#define LE_MAX(a, b) ((a) > (b) ? (a) : (b))
#define LE_MIN(a, b) ((a) < (b) ? (a) : (b))

static const char sFileNameStr[] = "/boot/level_equip_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static LevelEquipEntry_unk* sEntries;
static int sCount;

// inferred: loads /boot/level_equip_table.csv (rows whose first column is empty)
extern "C" void fn_8021414C() {
    atn::Csv csv;
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.SetData((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() == '\0') {
                sCount++;
            }
        }
        sEntries = new (12, "", "", 0) LevelEquipEntry_unk[sCount];
        csv.SetData((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != '\0') {
                continue;
            }
            LevelEquipEntry_unk* entry = &sEntries[i];
            csv.fn_8005B9CC(); // inferred: a column that is not used
            const char* field;
            for (int k = 0; k < 2; k++) {
                field = csv.fn_8005B9CC();
                if (strlen(field) > 2) {
                    entry->mIds[k] = atoi(field + 2);
                } else {
                    entry->mIds[k] = 0;
                }
                for (int j = 0; j < 3; j++) {
                    entry->mValues[k][j] = atoi(csv.fn_8005B9CC());
                }
                // inferred: columns that are not used
                csv.fn_8005B9CC();
                csv.fn_8005B9CC();
                csv.fn_8005B9CC();
                csv.fn_8005B9CC();
                csv.fn_8005B9CC();
            }
            field = csv.fn_8005B9CC();
            if (strlen(field) > 2) {
                entry->m20 = atoi(field + 2);
            } else {
                entry->m20 = 0;
            }
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

extern "C" int fn_80214394(int index, int k) {
    if (0 > k || k >= 2) {
        return 0;
    }
    return sEntries[LE_MIN(LE_MAX(index, 0), sCount - 1)].mIds[k];
}

extern "C" int fn_802143EC(int index, int k, int j) {
    if (0 > k || k >= 2) {
        return 0;
    }
    return sEntries[LE_MIN(LE_MAX(index, 0), sCount - 1)].mValues[k][j % 3];
}

extern "C" int fn_8021446C(int index) {
    return sEntries[LE_MIN(LE_MAX(index, 0), sCount - 1)].m20;
}
