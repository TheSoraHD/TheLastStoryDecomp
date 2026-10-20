#include "atn/Array_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int atoi(const char* s);
extern "C" int strcmp(const char* a, const char* b);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: element of the global buffers of db/SpText_unk.cpp (its destructor, fn_8021BE40, is
// defined there)
struct SpTextEntry_unk {
    ~SpTextEntry_unk();
};

// inferred: the destructor at 0x8021CC50 comes after the __sinit of db/SpText_unk.cpp (which
// registers it for six global buffers), so it is the weak copy of a destructor of that file
// (body of atn::Buffer_unk<T>::~Buffer_unk); the split puts it at the start of this unit and an
// explicit instantiation of the template destructor emits nothing, so it is written here as a
// non-template class (name invented)
struct SpTextBuffer_unk {
    ~SpTextBuffer_unk();

    int mSize;
    SpTextEntry_unk* mData;
};

SpTextBuffer_unk::~SpTextBuffer_unk() {
    if (mData != 0) {
        delete[] mData;
    }
    mData = 0;
    mSize = 0;
}

// inferred: one row of /boot/strategy_ai_table.csv (0x20 bytes, no constructor)
struct StrategyAiEntry_unk {
    int mId;
    int mKind; // inferred: index in sKinds (2 if not found)
    int m08[6];
};

static const char sFileNameStr[] = "/boot/strategy_ai_table.csv";
static const char* const sKinds[] = {"ATTACK", "GUARD", 0};
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

static StrategyAiEntry_unk* sEntries;
static int sCount;

static inline int GetKind_unk(const char* name) {
    for (int i = 0; sKinds[i] != 0; i++) {
        if (strcmp(name, sKinds[i]) == 0) {
            return i;
        }
    }
    return 2;
}

// inferred: loads /boot/strategy_ai_table.csv (rows whose first column is empty and whose second
// column is not a comment)
extern "C" void fn_8021CCC4() {
    atn::Csv csv;
    if (sEntries == 0) {
        unsigned long size;
        StrategyAiEntry_unk* entry;
        void* data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
        csv.vf08((const char*)data, size);
        sCount = 0;
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != '\0') {
                continue;
            }
            char c = *csv.fn_8005B9CC();
            if (c == '\0' || c == ';' || c == '#') {
                continue;
            }
            sCount++;
        }
        sEntries = new (1, "", "", 0) StrategyAiEntry_unk[sCount];
        csv.vf08((const char*)data, size);
        int i = 0;
        while (csv.fn_8005BBF8() != 0) {
            if (*csv.fn_8005B9CC() != '\0') {
                continue;
            }
            char c = *csv.fn_8005B9CC();
            if (c == '\0' || c == ';' || c == '#') {
                continue;
            }
            entry = &sEntries[i];
            entry->mId = atoi(csv.mField);
            csv.fn_8005B9CC(); // inferred: a column that is not used
            entry->mKind = GetKind_unk(csv.fn_8005B9CC());
            for (int j = 0; j < 6; j++) {
                entry->m08[j] = atoi(csv.fn_8005B9CC());
            }
            i++;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: entry by id and kind (0 if there is none)
extern "C" StrategyAiEntry_unk* fn_8021CF08(int id, int kind) {
    StrategyAiEntry_unk* entry = sEntries;
    for (int i = 0; i < sCount; i++, entry++) {
        if (entry->mId == id && entry->mKind == kind) {
            return entry;
        }
    }
    return 0;
}
