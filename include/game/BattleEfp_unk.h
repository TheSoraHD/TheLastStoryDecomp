#ifndef GAME_BATTLEEFP_UNK_H
#define GAME_BATTLEEFP_UNK_H

#include "atn/EfpHandle_unk.h"
#include "atn/LH_Xsca.h"

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
// inferred: loaded resources of one row (0x48 bytes)
struct BattleEfpEntry_unk {
    BattleEfpEntry_unk() : mData(0), mLoaded(0) {}

    // inferred: nonzero while one of the resources is loading
    bool IsLoading_unk() {
        if (m04.IsLoading()) {
            return true;
        }
        if (m10.IsLoading()) {
            return true;
        }
        if (m1C.IsLoading()) {
            return true;
        }
        if (m28.IsLoading()) {
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
            m04.Destroy();
            m10.Destroy();
            m1C.Destroy();
            m28.Destroy();
            m34.fn_80473F88();
            m3C.fn_80473F88();
        }
    }

    BattleEfpData_unk* mData;
    atn::EfpFile m04;
    atn::EfpFile m10;
    atn::EffFile m1C;
    atn::EfpFile m28;
    atn::LH_Xsca m34;
    atn::LH_Xsca m3C;
    int mLoaded; // 0x44
};

// inferred: entry of row `id` (0x800EFB78), null if missing
extern "C" BattleEfpEntry_unk* fn_800EFB78(int id);

#endif
