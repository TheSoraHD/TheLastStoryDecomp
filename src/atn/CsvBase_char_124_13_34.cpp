#include "atn/Csv.h"
#include "atn/Memory.h"

// Table of the battle effects (/boot/battle_eff_se_table.csv, used by game/BattleEfp_unk). The unit
// keeps its automatic name: the file also emits the weak copies of atn::CsvPipe (SetData, vtable and
// RTTI of atn::CsvBase<char, '|', '\r', '"'>) used for the level column.

extern "C" unsigned long strlen(const char* s);
extern "C" int atoi(const char* s);
extern "C" char* strncpy(char* dst, const char* src, unsigned long n);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: one row of the table (0x12C bytes, no constructor; same view as BattleEfpData_unk of
// game/BattleEfp_unk.cpp)
struct BattleEffSeEntry_unk {
    int mId;
    char mNames[9][0x20];          // 0x04: effects, resources and camera animations
    int m124;
    unsigned char mLevels[4];      // 0x128: levels where the row is used (0: any)
};

static const char sFileNameStr[] = "/boot/battle_eff_se_table.csv";
static const char* const sFileName = sFileNameStr; // inferred: .rodata array + .sdata2 pointer

extern "C" {
BattleEffSeEntry_unk* lbl_8087F180;
int lbl_8087F184;
}

// inferred: loads the table (called from db/ChapterList_unk)
// NONMATCHING: 99.48% - only the row pointer and the level string/loop counter swap r24/r26 (the
// declarations at the top fix the other registers; ~10 orders and scopes tried)
extern "C" void fn_802078C4() {
    int offset;
    BattleEffSeEntry_unk* entry;
    int count;
    void* data;
    unsigned long size;
    size = 0;
    data = fn_8046DC5C(lbl_8087F518, sFileName, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    count = 0;
    while (csv.fn_8005BBF8() != 0) {
        char c = *csv.fn_8005B9CC();
        if (c != 0 && c != ';') {
            count++;
        }
    }
    lbl_8087F180 = new (1, "", "", 0) BattleEffSeEntry_unk[count];
    lbl_8087F184 = count;
    csv.SetData((const char*)data, size);
    offset = 0;
    while (csv.fn_8005BBF8() != 0) {
        const char* field = csv.fn_8005B9CC();
        char c = *field;
        if (c == 0 || c == ';') {
            continue;
        }
        entry = (BattleEffSeEntry_unk*)((char*)lbl_8087F180 + offset);
        entry->mId = atoi(field);
        strncpy(entry->mNames[0], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[1], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[2], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[3], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[4], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[5], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[6], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[7], csv.fn_8005B9CC(), 0x20);
        strncpy(entry->mNames[8], csv.fn_8005B9CC(), 0x20);
        entry->m124 = atoi(csv.fn_8005B9CC());
        memset(entry->mLevels, 0, sizeof(entry->mLevels));
        const char* levels = csv.fn_8005B9CC();
        atn::CsvPipe pipe(levels, strlen(levels));
        for (int i = 0; i < 4; i++) {
            entry->mLevels[i] = atoi(pipe.fn_8005B9CC());
        }
        offset += sizeof(BattleEffSeEntry_unk);
    }
    fn_8046DD20(lbl_8087F518, data);
}

// inferred: number of rows
extern "C" int fn_80207C00() {
    return lbl_8087F184;
}

// inferred: row `index` (0 when out of range)
extern "C" BattleEffSeEntry_unk* fn_80207C08(int index) {
    if (index < 0 || index >= lbl_8087F184) {
        return 0;
    }
    return &lbl_8087F180[index];
}

// inferred: nonzero when the row is used at `level` (a row without levels is used at every level)
extern "C" int fn_80207C34(BattleEffSeEntry_unk* entry, int level) {
    int ret = 1;
    for (int i = 0; i < 4; i++) {
        if (level == entry->mLevels[i]) {
            ret = 1;
            break;
        }
        if (entry->mLevels[i] != 0) {
            ret = 0;
        }
    }
    return ret;
}
