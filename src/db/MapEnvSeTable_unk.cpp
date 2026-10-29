#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "gmk/SeTable.h"

extern "C" int atoi(const char* s);
extern "C" int sprintf(char* dst, const char* fmt, ...);
// inferred: parses a map id ("xx000_000") into its three numbers (db/MapIdTable_unk)
extern "C" void fn_80216AFC(const char* name, int* a, int* b, int* c);

// inferred: sound engine (lbl_8087EE90); m04 must be set for the tables to be loaded
struct EnvSeSound_unk {
    int m00;
    int m04;
};
extern "C" EnvSeSound_unk* lbl_8087EE90;
extern "C" unsigned int fn_80049B74(EnvSeSound_unk* engine, const char* name); // inferred: sound id of a name

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one environment sound of a map: key and sound id (8 bytes); inline constructor emitted
// out of line (fn_80214E04) for __construct_new_array
struct EnvSe_unk {
    EnvSe_unk() : mKey(0), mSe(0) {}

    int mKey;
    unsigned int mSe;
};

// inferred: atn::SimpleVector::reserve without the capacity check, that also empties the list (as
// InitItems_unk of db/ItemRare_unk.cpp; its own "" literals in .sdata). Name invented.
inline void InitList_unk(atn::SimpleVector<EnvSe_unk>& list, unsigned int n) {
    EnvSe_unk* data = new ((MemoryTag)0, "", "", 0) EnvSe_unk[n];
    if (list.mData != 0) {
        unsigned int count = (n > list.mSize) ? list.mSize : n;
        for (unsigned int i = 0; i < count; i++) {
            data[i] = list.mData[i];
        }
        delete[] list.mData;
    }
    list.mData = data;
    list.mCapacity = n;
    list.mSize = 0;
}

// inferred: one row of /boot/map_envse_table.csv (0x18 bytes): a map id and its sounds; inline
// constructor and destructor emitted out of line (fn_80214BF4, fn_80214E24)
struct MapEnvSe_unk {
    MapEnvSe_unk() : m00(-1), m04(0), m08(0) { InitList_unk(mList, 2); }
    ~MapEnvSe_unk() {}

    int m00;
    int m04;
    int m08;
    atn::SimpleVector<EnvSe_unk> mList;
};

// inferred: one row of /boot/loading_envse_table.csv (0x14 bytes); inline constructor and empty
// destructor emitted out of line (fn_80214E14, fn_80214E94)
struct LoadingEnvSe_unk {
    LoadingEnvSe_unk() : mSe(0) {}
    ~LoadingEnvSe_unk() {}

    unsigned int mSe;
    atn::FixedString_unk<0x10> mName;
};

static const char sFileNameStr[] = "/boot/map_envse_table.csv";
static const char sLoadingFileNameStr[] = "/boot/loading_envse_table.csv";
// inferred: .rodata arrays + .sdata2 pointers
static const char* const sFileName = sFileNameStr;
static const char* const sLoadingFileName = sLoadingFileNameStr;

static MapEnvSe_unk* sEntries;
static int sCount;
static LoadingEnvSe_unk* sLoading;
static int sLoadingCount;

// inferred: loads /boot/map_envse_table.csv and /boot/loading_envse_table.csv
extern "C" void fn_802144A8() {
    if (lbl_8087EE90 == 0 || lbl_8087EE90->m04 == 0) {
        return;
    }
    EnvSeSound_unk* engine;
    atn::Csv csv;
    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    int count = 0;
    csv.SetData((const char*)data, size);
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() == '\0') {
            count++;
        }
    }
    sEntries = new ((MemoryTag)1, "", "", 0) MapEnvSe_unk[count];
    sCount = count;
    csv.SetData((const char*)data, size);
    count = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != '\0') {
            continue;
        }
        const char* name = csv.fn_8005B9CC();
        MapEnvSe_unk* entry = &sEntries[count];
        fn_80216AFC(name, &entry->m00, &entry->m04, &entry->m08);
        for (;;) {
            const char* field = csv.fn_8005B9CC();
            if (*field == '\0') {
                break;
            }
            EnvSe_unk se;
            se.mKey = atoi(field);
            engine = lbl_8087EE90;
            se.mSe = fn_80049B74(engine, csv.fn_8005B9CC());
            entry->mList.push_back(se);
        }
        count++;
    }
    fn_8046DD20(lbl_8087F518, data);

    void* loadingData = fn_8046DC5C(lbl_8087F518, sLoadingFileName, &size, 0x20);
    int loadingCount = 0;
    csv.SetData((const char*)loadingData, size);
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != '\0') {
            continue;
        }
        engine = lbl_8087EE90;
        if (fn_80049B74(engine, csv.fn_8005B9CC()) != 0) {
            loadingCount++;
        }
    }
    sLoading = new ((MemoryTag)1, "", "", 0) LoadingEnvSe_unk[loadingCount];
    sLoadingCount = loadingCount;
    csv.SetData((const char*)loadingData, size);
    loadingCount = 0;
    while (csv.fn_8005BBF8() != 0) {
        if (*csv.fn_8005B9CC() != '\0') {
            continue;
        }
        const char* name;
        engine = lbl_8087EE90;
        unsigned int se = fn_80049B74(engine, csv.fn_8005B9CC());
        if (se == 0) {
            continue;
        }
        LoadingEnvSe_unk* entry = (LoadingEnvSe_unk*)((char*)sLoading + loadingCount);
        entry->mSe = se;
        name = csv.fn_8005B9CC();
        if (strlen(name) > 9) {
            entry->mName = name + 9;
        }
        loadingCount += sizeof(LoadingEnvSe_unk);
    }
    fn_8046DD20(lbl_8087F518, loadingData);
}

extern "C" unsigned int fn_80214F80(int a, int b, int c, int key);
extern "C" LoadingEnvSe_unk* fn_80214F38(unsigned int se);

// inferred: loading sound of a map and key
extern "C" LoadingEnvSe_unk* fn_80214ED4(int a, int b, int c, int key) {
    return fn_80214F38(fn_80214F80(a, b, c, key));
}

// inferred: loading sound entry of a sound id (0 if none)
extern "C" LoadingEnvSe_unk* fn_80214F38(unsigned int se) {
    if (se == 0) {
        return 0;
    }
    LoadingEnvSe_unk* entry = sLoading;
    for (int i = 0; i < sLoadingCount; i++, entry++) {
        if (entry->mSe == se) {
            return entry;
        }
    }
    return 0;
}

// inferred: environment sound of a map for a key (the first one if the key is not listed)
// NONMATCHING: 99.21% - volatile register allocation only: the original gives r12 to the counter i
// and r8 to the element address of the inner loop (here swapped). The counter declared first and
// the increments in the order entry++, i++ fixed the rest. Tried: index/pointer forms of both
// loops, while loops, declaration orders, pre-increments, reference/pointer to the element,
// continue on size == 0.
extern "C" unsigned int fn_80214F80(int a, int b, int c, int key) {
    int i = 0;
    MapEnvSe_unk* entry = sEntries;
    int count = sCount;
    for (; i < count; entry++, i++) {
        if (entry->m00 == a && entry->m04 == b && entry->m08 == c) {
            unsigned int size = entry->mList.mSize;
            for (unsigned int j = 0; j < size; j++) {
                EnvSe_unk* se = &entry->mList.mData[j];
                if (se->mKey == key) {
                    return se->mSe;
                }
            }
            if (size != 0) {
                return entry->mList.mData[0].mSe;
            }
        }
    }
    return 0;
}

extern "C" bool fn_80215018(LoadingEnvSe_unk* entry, char* buf) {
    if (strlen(entry->mName.c_str()) != 0) {
        sprintf(buf, "GROUP_VRC%s\0", entry->mName.c_str());
        return true;
    }
    return false;
}

extern "C" bool fn_80215084(LoadingEnvSe_unk* entry, char* buf) {
    if (strlen(entry->mName.c_str()) != 0) {
        sprintf(buf, "SE_VRCOM_%s", entry->mName.c_str());
        return true;
    }
    return false;
}
