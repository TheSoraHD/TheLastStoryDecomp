#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" wchar_t* wcsncpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int fn_800DC12C(const char* s);    // inferred: string to int
extern "C" float fn_800DC288(const char* s);  // inferred: string to float
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int
extern "C" float fn_800DC3CC(const wchar_t* s); // inferred: wide string to float

// inferred: the font (lbl_8087EEC8) converts UTF-16 text to the game encoding
extern "C" void* lbl_8087EEC8;
extern "C" int fn_8006F420(void* font, char* dst, const wchar_t* src, int size);
// inferred: copy / conversion of a text to buf + pos, returning the next position (db/CharaIdTable_unk)
extern "C" int fn_80208B70(const char** out, const char* src, char* buf, int pos);
extern "C" int fn_80208A3C(const char** out, const wchar_t* src, char* buf, int pos);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one map of /boot/map_id_table_xx.u16 (0xCC bytes); the inline constructor is emitted
// out of line (fn_802174E8) for __construct_new_array
struct MapId_unk {
    MapId_unk() : mCharas(0) {}

    const char* mId; // inferred: "wd000_00", "tw00_00", "dg000_00", "mp000_00"
    wchar_t mName[0x20];
    int mType; // inferred: 0x44: from the id prefix (fn_80216AFC)
    int mMap;
    int mPoint;
    int m50;
    int m54;
    int m58;
    const char* m5C;
    const char* m60;
    float m64;
    const char* m68;
    const char* m6C;
    const char* m70;
    int m74[16];
    unsigned int mCharas; // inferred: 0xB4: bit per chara group (/boot/map_motion_table.csv)
    int* mMembers;        // inferred: 0xB8: -1 terminated list (/boot/map_chara_list.csv)
    int mBC;
    int mC0;
    float mC4;
    int mC8;
};

static const char sFileNameStr[] = "/boot/map_id_table_xx.u16";
static const char sMotionFileNameStr[] = "/boot/map_motion_table.csv";
static const char sCharaFileNameStr[] = "/boot/map_chara_list.csv";
static const char sSubIdFileNameStr[] = "/boot/map_sub_id_table.csv";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sFileName = sFileNameStr;
static const char* const sMotionFileName = sMotionFileNameStr;
static const char* const sCharaFileName = sCharaFileNameStr;
static const char* const sSubIdFileName = sSubIdFileNameStr;

static MapId_unk* sEntries;
static unsigned int sCount;
static int* sMembers;
static char* sTexts;

// inferred: splits a map id into its kind (from the prefix) and two numbers
extern "C" bool fn_80216AFC(const char* id, int* type, int* map, int* point) {
    if (id == 0) {
        return false;
    }
    char mapStr[4] = {0};
    char pointStr[4] = {0};
    if (strncmp(id, "wd", 2) == 0) {
        *type = 0;
        strncpy(mapStr, id + 2, 3);
        strncpy(pointStr, id + 6, 2);
        *map = fn_800DC12C(mapStr);
        *point = fn_800DC12C(pointStr);
    } else if (strncmp(id, "tw", 2) == 0) {
        *type = 1;
        strncpy(mapStr, id + 2, 2);
        strncpy(pointStr, id + 5, 2);
        *map = fn_800DC12C(mapStr);
        *point = fn_800DC12C(pointStr);
    } else if (strncmp(id, "dg", 2) == 0) {
        *type = 2;
        strncpy(mapStr, id + 2, 3);
        strncpy(pointStr, id + 6, 2);
        *map = fn_800DC12C(mapStr);
        *point = fn_800DC12C(pointStr);
    } else if (strncmp(id, "mp", 2) == 0) {
        *type = 3;
        strncpy(mapStr, id + 2, 3);
        strncpy(pointStr, id + 6, 2);
        *map = fn_800DC12C(mapStr);
        *point = fn_800DC12C(pointStr);
    } else {
        *type = 2;
        *map = 0;
        *point = 0;
    }
    return true;
}

static inline MapId_unk* Find_unk(const char* id) {
    unsigned int count = sCount;
    MapId_unk* entry = sEntries;
    for (unsigned int i = 0; i < count; entry++, i++) {
        if (strcmp(entry->mId, id) == 0) {
            return entry;
        }
    }
    return 0;
}

extern "C" void fn_802174F4();
extern "C" MapId_unk* fn_8021771C(int type, int map, int point);

// inferred: loads the map tables (/boot/map_id_table_xx.u16, map_sub_id_table.csv,
// map_motion_table.csv and map_chara_list.csv)
// NONMATCHING: 99.85% - registers only: the original reuses the register of the CSV length for the
// font of the first loop (r27) and the one of the CSV address for the index (r28); here both get
// new registers. Tried declaring font/i/pos earlier and passing the font global directly, an
// unsigned index, declaration orders in Find_unk (entry++, i++ there fixed the subtable loop).
extern "C" void fn_80216CD0() {
    void* data;
    unsigned long size = 0;
    int textSize = 0;
    data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    atn::CsvU wcsv((const wchar_t*)data, (int)size / 2);
    char id[0x40];
    while (wcsv.fn_8005BBF8() != 0) {
        if (wcscmp(wcsv.fn_8005B9CC(), L"") != 0) {
            continue;
        }
        void* font = lbl_8087EEC8;
        fn_8006F420(font, id, wcsv.fn_8005B9CC(), 0x20);
        if (strcmp(id, "") == 0) {
            continue;
        }
        textSize += strlen(id) + 2;
        // inferred: columns that are not texts
        wcsv.fn_8005B9CC();
        wcsv.fn_8005B9CC();
        wcsv.fn_8005B9CC();
        wcsv.fn_8005B9CC();
        wcsv.fn_8005B9CC();
        font = lbl_8087EEC8;
        fn_8006F420(font, id, wcsv.fn_8005B9CC(), 0x20);
        textSize += strlen(id) + 2;
        font = lbl_8087EEC8;
        fn_8006F420(font, id, wcsv.fn_8005B9CC(), 0x20);
        textSize += strlen(id) + 2;
        wcsv.fn_8005B9CC();
        font = lbl_8087EEC8;
        fn_8006F420(font, id, wcsv.fn_8005B9CC(), 0x20);
        textSize += strlen(id) + 2;
        font = lbl_8087EEC8;
        fn_8006F420(font, id, wcsv.fn_8005B9CC(), 0x20);
        textSize += strlen(id) + 2;
        font = lbl_8087EEC8;
        fn_8006F420(font, id, wcsv.fn_8005B9CC(), 0x40);
        textSize += strlen(id) + 2;
        sCount++;
    }
    sEntries = new (1, "", "", 0) MapId_unk[sCount];
    sTexts = new (3, "", "", 0) char[textSize];
    int i = 0;
    int pos = 0;
    wcsv.vf08((const wchar_t*)data, (int)size / 2);
    while (wcsv.fn_8005BBF8() != 0) {
        if (wcscmp(wcsv.fn_8005B9CC(), L"") != 0) {
            continue;
        }
        void* font = lbl_8087EEC8;
        fn_8006F420(font, id, wcsv.fn_8005B9CC(), 0x20);
        if (strcmp(id, "") == 0) {
            continue;
        }
        MapId_unk* entry = &sEntries[i];
        pos = fn_80208B70(&entry->mId, id, sTexts, pos);
        fn_80216AFC(entry->mId, &entry->mType, &entry->mMap, &entry->mPoint);
        wcsncpy(entry->mName, wcsv.fn_8005B9CC(), 0x20);
        wcsv.fn_8005B9CC(); // inferred: a column that is not used
        entry->m50 = fn_800DC1DC(wcsv.fn_8005B9CC());
        entry->m54 = fn_800DC1DC(wcsv.fn_8005B9CC());
        entry->m58 = fn_800DC1DC(wcsv.fn_8005B9CC());
        pos = fn_80208A3C(&entry->m5C, wcsv.fn_8005B9CC(), sTexts, pos);
        pos = fn_80208A3C(&entry->m60, wcsv.fn_8005B9CC(), sTexts, pos);
        entry->m64 = fn_800DC3CC(wcsv.fn_8005B9CC());
        pos = fn_80208A3C(&entry->m68, wcsv.fn_8005B9CC(), sTexts, pos);
        pos = fn_80208A3C(&entry->m6C, wcsv.fn_8005B9CC(), sTexts, pos);
        pos = fn_80208A3C(&entry->m70, wcsv.fn_8005B9CC(), sTexts, pos);
        for (int j = 0; j < 16; j++) {
            entry->m74[j] = fn_800DC1DC(wcsv.fn_8005B9CC());
        }
        entry->mMembers = 0;
        entry->mBC = 0;
        i++;
    }
    fn_8046DD20(lbl_8087F518, data);

    data = fn_8046DC5C(lbl_8087F518, sSubIdFileName, &size, 0x20);
    atn::Csv subCsv((const char*)data, size);
    char name[0x20];
    while (subCsv.fn_8005BBF8() != 0) {
        if (strcmp(subCsv.fn_8005B9CC(), "") != 0) {
            continue;
        }
        strncpy(name, subCsv.fn_8005B9CC(), 0x20);
        subCsv.fn_8005B9CC(); // inferred: a column that is not used
        MapId_unk* entry = Find_unk(name);
        if (entry == 0) {
            continue;
        }
        entry->mBC = fn_800DC12C(subCsv.fn_8005B9CC());
        entry->mC4 = fn_800DC288(subCsv.fn_8005B9CC());
        entry->mC0 = fn_800DC12C(subCsv.fn_8005B9CC());
        entry->mC8 = fn_800DC12C(subCsv.fn_8005B9CC());
    }
    fn_8046DD20(lbl_8087F518, data);

    data = fn_8046DC5C(lbl_8087F518, sMotionFileName, &size, 0x20);
    atn::Csv motionCsv((const char*)data, size);
    while (motionCsv.fn_8005BBF8() != 0) {
        const char* field = motionCsv.fn_8005B9CC();
        if (strcmp(field, "") == 0) {
            continue;
        }
        int type;
        int map;
        int point;
        fn_80216AFC(field, &type, &map, &point);
        MapId_unk* entry = fn_8021771C(type, map, point);
        if (entry == 0) {
            continue;
        }
        motionCsv.fn_8005B9CC(); // inferred: a column that is not used
        for (int j = 0; j < 32; j++) {
            if (*motionCsv.fn_8005B9CC() == '1') {
                entry->mCharas |= 1 << j;
            }
        }
    }
    fn_8046DD20(lbl_8087F518, data);
    fn_802174F4();
}

// inferred: loads /boot/map_chara_list.csv (the members of each map, -1 terminated)
// NONMATCHING: 97.22% - saved register numbering only (constant 0/-1, size, the inlined search and
// the member counter); tried the declaration order of the search locals
extern "C" void fn_802174F4() {
    unsigned long size = 0;
    void* data = fn_8046DC5C(lbl_8087F518, sCharaFileName, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    int total = 0;
    while (csv.fn_8005BBF8() != 0) {
        csv.fn_8005B9CC(); // inferred: the map id
        for (const char* field = csv.fn_8005B9CC(); strlen(field) != 0; field = csv.fn_8005B9CC()) {
            total++;
        }
        total++;
    }
    sMembers = new (1, "", "", 0) int[total];
    csv.vf08((const char*)data, size);
    int pos = 0;
    while (csv.fn_8005BBF8() != 0) {
        MapId_unk* entry = Find_unk(csv.fn_8005B9CC());
        if (entry == 0) {
            // inferred: the original returns without releasing the file
            return;
        }
        entry->mMembers = &sMembers[pos];
        int n = 0;
        for (const char* field = csv.fn_8005B9CC(); strlen(field) != 0; field = csv.fn_8005B9CC()) {
            entry->mMembers[n] = atoi(field);
            n++;
        }
        entry->mMembers[n++] = -1;
        pos += n;
    }
    fn_8046DD20(lbl_8087F518, data);
}

// inferred: map by kind and numbers (kind 4 is searched as 3)
extern "C" MapId_unk* fn_8021771C(int type, int map, int point) {
    if (type == 4) {
        type = 3;
    }
    MapId_unk* entry = sEntries;
    for (unsigned int i = 0; i < sCount; i++, entry++) {
        if (entry->mType == type && entry->mMap == map && entry->mPoint == point) {
            return entry;
        }
    }
    return 0;
}
