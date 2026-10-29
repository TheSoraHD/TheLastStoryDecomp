#ifndef DB_OPTIONPARTS_UNK_H
#define DB_OPTIONPARTS_UNK_H

#include "gmk/SeTable.h"
#include "std/msl_common.h"

extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);

// Types of db/OptionParts_unk.cpp (costume option parts and their level lists), shared with the
// costume menus (game/CosEditPalette_unk.cpp).

// inferred: colour of the costume editor palette (db/PaletteTable_unk.cpp)
struct Color4f_unk {
    float r, g, b, a;
};
struct OptionPalette_unk {
    int m00;
    Color4f_unk mColor;
};
extern "C" OptionPalette_unk* fn_8020BCEC(int index);
// inferred: "option" row of database/cosedit/option/*.csv (0x44 bytes)
struct OptionPart_unk {
    atn::FixedString_unk<0x20> mName;
    atn::FixedString_unk<0x20> mKey;
    int mLevel;
};

// inferred: "color" row (0x54 bytes)
struct OptionColor_unk {
    atn::FixedString_unk<0x20> mGroup; // inferred: texture group
    atn::FixedString_unk<0x20> mLayer;
    Color4f_unk mColor;
    signed char mChannel; // inferred: 'R' 0, 'G' 1, other 2
    signed char mSlot;    // inferred: index in sColorSlots, -1 if none
    signed char m52;
    bool mEnable;
};

// inferred: "hide"/"base_hide" row: list of option parts (0x64 bytes)
struct OptionHide_unk {
    atn::FixedString_unk<0x20> mName;
    int mParts[0x10];
    int mCount;
};

// inferred: option parts of a costume (0x10E0 bytes, member of the equipment at 0x80564340)
struct OptionParts_unk {
    OptionPart_unk mParts[16];     // 0x000
    OptionColor_unk mColors[24];   // 0x440
    OptionHide_unk mHides[8];      // 0xC20
    OptionHide_unk mBaseHides[4];  // 0xF40
    int mPartCount;                // 0x10D0
    int mColorCount;               // 0x10D4
    int mHideCount;                // 0x10D8
    int mBaseHideCount;            // 0x10DC

    void SetLevels_unk(const bool* levels) {
        for (int i = 0; i < mPartCount; i++) {
            mParts[i].mLevel = levels[i];
        }
    }
};

// inferred: index of the option part with the hash of a name, -1 if none
extern "C" int fn_8020D608(OptionParts_unk* parts, unsigned int hash);

// inferred: wide string of a fixed size
template <int N>
struct WFixedString_unk {
    WFixedString_unk() { mStr[0] = 0; }
    WFixedString_unk& operator=(const wchar_t* str) {
        if (str != mStr) {
            wmemcpy(mStr, str, wcslen(str) + 1);
        }
        return *this;
    }
    wchar_t mStr[N];
};

// inferred: option of a level list (0x54 bytes)
struct OptionLevel_unk {
    OptionLevel_unk() {}
    ~OptionLevel_unk() {}

    int m00;
    unsigned int mHashes[3];
    WFixedString_unk<0x20> mName;
    int mFree; // inferred: "FREE"
};

// inferred: options of one letter and slot (0x544 bytes)
struct OptionLevelList_unk {
    OptionLevelList_unk() {
        memset(mLevels, 0, sizeof(mLevels));
        mCount = 0;
    }
    ~OptionLevelList_unk() {}

    OptionLevel_unk mLevels[16];
    int mCount;
};

// inferred: level list of a letter and slot
extern "C" OptionLevelList_unk* fn_8020DB18(int letter, int slot);

#endif
