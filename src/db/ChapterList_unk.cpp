#include "db/EventIdTable_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int sprintf(char* dst, const char* fmt, ...);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: loaders of the other /boot tables (db/*_unk.cpp)
extern "C" void fn_80204958();
extern "C" void fn_80204E54();
extern "C" void fn_80206F44();
extern "C" void fn_80207310();
extern "C" void fn_802078C4();
extern "C" void fn_80207CC0();
extern "C" void fn_80208144();
extern "C" void fn_80208BE0();
extern "C" void fn_80209300();
extern "C" void fn_80209960();
extern "C" void fn_8020A90C();
extern "C" void fn_8020AFF4();
extern "C" void fn_8020B2BC();
extern "C" void fn_8020BB0C();
extern "C" void fn_8020D688();
extern "C" void fn_8020DB58();
extern "C" void fn_8020DF88();
extern "C" void fn_8020F84C();
extern "C" void fn_8020FDEC();
extern "C" void fn_80210244();
extern "C" void fn_80210ACC();
extern "C" void fn_802130BC();
extern "C" void fn_8021414C();
extern "C" void fn_802144A8();
extern "C" void fn_802150F0();
extern "C" void fn_80216CD0();
extern "C" void fn_80217778();
extern "C" void fn_802179FC();
extern "C" void fn_80217E18();
extern "C" void fn_802189EC();
extern "C" void fn_80218BD4();
extern "C" void fn_802196D0();
extern "C" void fn_8021996C();
extern "C" void fn_8021AAB8();
extern "C" void fn_8021B060();
extern "C" void fn_8021B2F8();
extern "C" void fn_8021C3F4();
extern "C" void fn_8021CCC4();
extern "C" void fn_8021CF8C();
extern "C" void fn_8021DBEC();
extern "C" void fn_8021E0B0();
extern "C" void fn_8021EB04();
extern "C" void fn_8021ED08();
extern "C" void fn_8021F100();
extern "C" void fn_8021F47C();

// inferred: table of the game texts (defined in atn/ptr_string_base_wchar_t_0.cpp)
class TextTable_unk {
public:
    static void fn_8020C028(); // inferred: creates the singleton
};

// inferred: one row of /boot/chapter_list.csv (0x64 bytes); the rows are linked in order
struct ChapterEntry_unk {
    ChapterEntry_unk() : m04(0), mType(-1), mMap(0), mPoint(0), m54(0), m58(0), mPrev(0), mNext(0) {}

    int m00;
    int m04;
    int mType;  // inferred: from the map id prefix ("tw" 1, "dg" 2, otherwise 7)
    int mMap;   // inferred: number after the prefix
    int mPoint; // inferred: second number of a "dg" id
    char mName[0x40];
    int m54;
    int m58;
    ChapterEntry_unk* mPrev;
    ChapterEntry_unk* mNext;
};

static const char sFileNameStr[] = "/boot/chapter_list.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static ChapterEntry_unk* sEntries;
static int sCount;

// inferred: loads all the /boot tables
extern "C" void fn_80207F80() {
    fn_80208BE0();
    fn_80209960();
    fn_80204E54();
    fn_80206F44();
    fn_8020DF88();
    fn_80204958();
    fn_8020FDEC();
    fn_80217E18();
    fn_8021996C();
    fn_80210ACC();
    fn_8020AFF4();
    fn_80216CD0();
    EventIdTable_unk::fn_8020F130()->fn_8020F248();
    fn_80209300();
    fn_802196D0();
    fn_80207CC0();
    fn_80210244();
    fn_8020F84C();
    fn_802078C4();
    fn_8021F47C();
    fn_8020A90C();
    fn_8021CCC4();
    fn_80208144();
    fn_8021E0B0();
    fn_8021EB04();
    TextTable_unk::fn_8020C028();
    fn_8021F100();
    fn_80218BD4();
    fn_8021414C();
    fn_8020B2BC();
    fn_8020BB0C();
    fn_80207310();
    fn_8021CF8C();
    fn_8021DBEC();
    fn_802179FC();
    fn_8021AAB8();
    fn_8021ED08();
    fn_8020D688();
    fn_8020DB58();
    fn_8021B060();
    fn_8021B2F8();
    fn_8021C3F4();
    fn_802150F0();
    fn_802189EC();
    fn_80217778();
    fn_802144A8();
    fn_802130BC();
}

// inferred: splits a map id ("dg012_03", "tw005") into its kind and numbers
extern "C" void fn_8020805C(const char* id, int* type, int* map, int* point) {
    if (id == 0 || strlen(id) < 2 || type == 0 || map == 0 || point == 0) {
        return;
    }
    *type = 7;
    *map = 0;
    *point = 0;
    if (strncmp(id, "dg", 2) == 0) {
        *type = 2;
        *map = atoi(id + 2);
        *point = atoi(id + 6);
    } else if (strncmp(id, "tw", 2) == 0) {
        *type = 1;
        *map = atoi(id + 2);
    }
}

// inferred: loads /boot/chapter_list.csv (the first row is a fixed "debug" entry)
extern "C" void fn_80208144() {
    if (sEntries == 0) {
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        atn::Csv csv((const char*)data, size);
        for (int i = 0; i < 7; i++) {
            csv.fn_8005BBF8();
        }
        do {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "") != 0) {
                continue;
            }
            if (strcmp(csv.fn_8005B9CC(), "") == 0) {
                continue;
            }
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
            if (atoi(csv.fn_8005B9CC()) > 0) {
                sCount++;
            }
        } while (csv.fn_8005BBF8() != 0);
        sCount++;
        sEntries = new (1, "", "", 0) ChapterEntry_unk[sCount];
        ChapterEntry_unk* first = sEntries;
        first->m00 = 0;
        first->m04 = 0;
        first->mType = 0;
        first->mMap = 0;
        first->mPoint = 0;
        sprintf(first->mName, "debug");
        first->m54 = 0x14;
        first->m58 = 0x10;
        csv.SetData((const char*)data, size);
        for (int i = 0; i < 7; i++) {
            csv.fn_8005BBF8();
        }
        int i = 1;
        do {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "") != 0) {
                continue;
            }
            if (strcmp(csv.fn_8005B9CC(), "") == 0) {
                continue;
            }
            ChapterEntry_unk entry;
            entry.m00 = atoi(csv.mField);
            entry.m04 = atoi(csv.fn_8005B9CC());
            fn_8020805C(csv.fn_8005B9CC(), &entry.mType, &entry.mMap, &entry.mPoint);
            strncpy(entry.mName, csv.fn_8005B9CC(), 0x40);
            entry.m54 = atoi(csv.fn_8005B9CC());
            entry.m58 = atoi(csv.fn_8005B9CC());
            if (entry.m54 > 0) {
                sEntries[i] = entry;
                i++;
            }
        } while (csv.fn_8005BBF8() != 0);
        fn_8046DD20(lbl_8087F518, data);
        for (int j = 1; j < sCount; j++) {
            ChapterEntry_unk* entry = &sEntries[j];
            if (j > 1) {
                entry->mPrev = &sEntries[j - 1];
            }
            if (j < sCount - 1) {
                entry->mNext = &sEntries[j + 1];
            }
        }
    }
}

extern "C" ChapterEntry_unk* fn_802085E0(int type, int map) {
    ChapterEntry_unk* entries = sEntries;
    for (int i = 0; i < sCount; i++) {
        ChapterEntry_unk* entry = &entries[i];
        if (entry->mType == type && entry->mMap == map) {
            return &entries[i];
        }
    }
    return entries;
}

extern "C" ChapterEntry_unk* fn_80208634(int type, int map, int point) {
    ChapterEntry_unk* entries = sEntries;
    for (int i = 0; i < sCount; i++) {
        ChapterEntry_unk* entry = &entries[i];
        if (entry->mType == type && entry->mMap == map && entry->mPoint == point) {
            return &entries[i];
        }
    }
    return entries;
}

// inferred: entry for a value of m04 (the last entry whose m04 is not above it)
extern "C" ChapterEntry_unk* fn_80208694(int value) {
    int count = sCount;
    ChapterEntry_unk* entries = sEntries;
    ChapterEntry_unk* last = &entries[count - 1];
    int lastValue = last->m04;
    if (value <= entries->m04) {
        return entries;
    }
    if (lastValue <= value) {
        return last;
    }
    for (int i = 1; i < count; i++) {
        if (entries[i].m04 > value) {
            return &entries[i - 1];
        }
    }
    return entries;
}

extern "C" ChapterEntry_unk* fn_80208714(int index) {
    if (index < 0 || sCount <= index) {
        return 0;
    }
    return &sEntries[index];
}

extern "C" int fn_80208740() {
    return sCount;
}
