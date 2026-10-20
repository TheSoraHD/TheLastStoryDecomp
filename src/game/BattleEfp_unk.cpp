// inferred file name (splits-auto): preloading of the battle effects listed in battle_eff_se_table
// (atn/CsvBase_char_124_13_34.cpp); used by GameTask.
#include "atn/Memory.h"
#include "atn/EfpHandle_unk.h"
#include "atn/LH_Xsca.h"
#include "atn/ResLoader_unk.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int sprintf(char* dst, const char* fmt, ...);

// inferred: row of the battle effect table (atn/CsvBase_char_124_13_34.cpp); names invented
struct BattleEfpData_unk {
    int mId;
    char mEfp1[0x20];  // 0x04
    char mEfp2[0x20];  // 0x24
    char mRes[0x20];   // 0x44
    char mEfp3[0x20];  // 0x64
    char m84[0x20];
    char mA4[0x20];
    char mC4[0x20];
    char mXsca1[0x20]; // 0xE4: camera animation (without path)
    char mXsca2[0x20]; // 0x104
    int m124;          // inferred: always loaded (mode 1)
};
extern "C" unsigned int fn_80207C00();                               // inferred: number of rows
extern "C" BattleEfpData_unk* fn_80207C08(unsigned int index);       // inferred: row `index`
extern "C" int fn_80207C34(BattleEfpData_unk* data, int level);     // inferred: row used at `level`
extern "C" const char* fn_80473F18(atn::LoadHandle* handle);        // inferred: name of the loaded resource

// inferred: loaded resources of one row (0x48 bytes)
struct BattleEfpEntry_unk {
    BattleEfpEntry_unk() : mData(0), mLoaded(0) {}

    // inferred: nonzero while one of the resources is loading
    bool IsLoading_unk() {
        if (m04.fn_80237874()) {
            return true;
        }
        if (m10.fn_80237874()) {
            return true;
        }
        if (m1C.fn_802376D0()) {
            return true;
        }
        if (m28.fn_80237874()) {
            return true;
        }
        if (m34.fn_80473F50()) {
            return true;
        }
        return m3C.fn_80473F50();
    }
    void Unload_unk() {
        if (mLoaded != 0) {
            mLoaded = 0;
            m04.fn_8023781C();
            m10.fn_8023781C();
            m1C.fn_8023772C();
            m28.fn_8023781C();
            m34.fn_80473F88();
            m3C.fn_80473F88();
        }
    }

    BattleEfpData_unk* mData;
    atn::EfpHandle_unk m04;
    atn::EfpHandle_unk m10;
    ResLoader_unk m1C;
    atn::EfpHandle_unk m28;
    atn::LH_Xsca m34;
    atn::LH_Xsca m3C;
    int mLoaded; // 0x44
};

int lbl_8087D968 = 2;                 // inferred: load mode (1: flagged rows, 2: rows of the level)
BattleEfpEntry_unk* lbl_8087F030;     // inferred: entries
unsigned int lbl_8087F034;            // inferred: number of entries
int lbl_8087F038;                     // inferred: level (255: everything)

// inferred: starts loading the resources of `entry`
extern "C" void fn_800EF3EC(BattleEfpEntry_unk* entry) {
    if (entry->mLoaded != 0) {
        return;
    }
    entry->mLoaded = 1;
    {
        const char* name = entry->mData->mEfp1;
        if (strcmp(name, "") != 0) {
            entry->m04.fn_8023780C(name);
        }
    }
    {
        const char* name = entry->mData->mEfp2;
        if (strcmp(name, "") != 0) {
            entry->m10.fn_8023780C(name);
        }
    }
    {
        const char* name = entry->mData->mRes;
        if (strcmp(name, "") != 0) {
            entry->m1C.fn_80237654(name);
        }
    }
    {
        const char* name = entry->mData->mEfp3;
        if (strcmp(name, "") != 0) {
            entry->m28.fn_8023780C(name);
        }
    }
    {
        const char* name = entry->mData->mXsca1;
        if (strcmp(name, "") != 0) {
            char path[0x100];
            sprintf(path, "/camera/battle/%s.xsca", name);
            entry->m34.vf0C(path);
        }
    }
    {
        const char* name = entry->mData->mXsca2;
        if (strcmp(name, "") != 0) {
            char path[0x100];
            sprintf(path, "/camera/battle/%s.xsca", name);
            entry->m3C.vf0C(path);
        }
    }
}

// inferred: releases every entry (inlined in fn_800EF588)
extern "C" void fn_800EF8DC();

// inferred: creates the entries and loads the ones of `mode`/`level`
extern "C" void fn_800EF588(int mode, int level) {
    fn_800EF8DC();
    lbl_8087D968 = mode;
    lbl_8087F038 = level;
    lbl_8087F034 = fn_80207C00();
    lbl_8087F030 = new (1, "", "", 0) BattleEfpEntry_unk[lbl_8087F034];
    for (unsigned int i = 0; i < lbl_8087F034; i++) {
        BattleEfpEntry_unk* entry = &lbl_8087F030[i];
        entry->mData = fn_80207C08(i);
        if (lbl_8087D968 == 1 && entry->mData->m124 == 0) {
            continue;
        }
        if (lbl_8087D968 == 2 && lbl_8087F038 < 255 && fn_80207C34(entry->mData, lbl_8087F038) == 0) {
            continue;
        }
        fn_800EF3EC(entry);
    }
}

extern "C" void fn_800EF8DC() {
    if (lbl_8087F030 != 0) {
        delete[] lbl_8087F030;
        lbl_8087F030 = 0;
        lbl_8087F034 = 0;
    }
}

// inferred: nonzero while an entry is loading
extern "C" int fn_800EF91C() {
    for (unsigned int i = 0; i < lbl_8087F034; i++) {
        BattleEfpEntry_unk* entry = &lbl_8087F030[i];
        if (entry->IsLoading_unk()) {
            return 1;
        }
    }
    return 0;
}

// inferred: changes the mode/level, unloading and loading the entries
extern "C" void fn_800EFA18(int mode, int level) {
    if (lbl_8087D968 == mode && lbl_8087F038 == level) {
        return;
    }
    if (lbl_8087D968 == mode && lbl_8087D968 == 1) {
        return;
    }
    lbl_8087D968 = mode;
    lbl_8087F038 = level;
    for (unsigned int i = 0; i < lbl_8087F034; i++) {
        BattleEfpEntry_unk* entry = &lbl_8087F030[i];
        if (lbl_8087D968 == 1) {
            if (entry->mData->m124 == 0) {
                entry->Unload_unk();
            }
        } else if (lbl_8087F038 < 255 && fn_80207C34(entry->mData, lbl_8087F038) == 0) {
            entry->Unload_unk();
        } else {
            fn_800EF3EC(entry);
        }
    }
}

// inferred: entry of row `id`
extern "C" BattleEfpEntry_unk* fn_800EFB78(int id) {
    for (unsigned int i = 0; i < lbl_8087F034; i++) {
        if (id == lbl_8087F030[i].mData->mId) {
            return &lbl_8087F030[i];
        }
    }
    return 0;
}

extern "C" atn::EfpHandle_unk* fn_800EFBC4(int id) {
    BattleEfpEntry_unk* entry = fn_800EFB78(id);
    if (entry != 0 && !entry->m04.fn_80237874() && fn_80473F18(&entry->m04.mLoader) != 0) {
        return &entry->m04;
    }
    return 0;
}

extern "C" atn::EfpHandle_unk* fn_800EFC64(int id) {
    BattleEfpEntry_unk* entry = fn_800EFB78(id);
    if (entry != 0 && !entry->m10.fn_80237874() && fn_80473F18(&entry->m10.mLoader) != 0) {
        return &entry->m10;
    }
    return 0;
}

extern "C" ResLoader_unk* fn_800EFD04(int id) {
    BattleEfpEntry_unk* entry = fn_800EFB78(id);
    if (entry != 0 && !entry->m1C.fn_802376D0() && entry->m1C.mResource != 0) {
        return &entry->m1C;
    }
    return 0;
}

extern "C" atn::EfpHandle_unk* fn_800EFDA0(int id) {
    BattleEfpEntry_unk* entry = fn_800EFB78(id);
    if (entry != 0 && !entry->m28.fn_80237874() && entry->m28.mResource != 0) {
        return &entry->m28;
    }
    return 0;
}

extern "C" const char* fn_800EFE3C(int id) {
    BattleEfpEntry_unk* entry = fn_800EFB78(id);
    if (entry != 0) {
        return entry->mData->m84;
    }
    return 0;
}

extern "C" const char* fn_800EFEA0(int id) {
    BattleEfpEntry_unk* entry = fn_800EFB78(id);
    if (entry != 0) {
        return entry->mData->mA4;
    }
    return 0;
}

extern "C" const char* fn_800EFF04(int id) {
    BattleEfpEntry_unk* entry = fn_800EFB78(id);
    if (entry != 0) {
        return entry->mData->mC4;
    }
    return 0;
}
