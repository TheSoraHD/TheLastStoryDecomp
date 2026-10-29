#include "atn/Csv.h"
#include "atn/Memory.h"
#include "gmk/SeTable.h"
#include "std/string.h"
#include "std/string_members.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" int strncmp(const char* a, const char* b, unsigned long n);
extern "C" int atoi(const char* s);
extern "C" int sprintf(char* buf, const char* format, ...);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" float fn_800DC288(const char* s);        // inferred: string to float
extern "C" int fn_800DC12C(const char* s);          // inferred: string to number
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: hash of a name

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: index of an equipment letter (db/EquipParam_unk.cpp)
extern "C" int fn_8020F064(const char* letter);

// inferred: colour of the costume editor palette (db/PaletteTable_unk.cpp)
struct Color4f_unk {
    float r, g, b, a;
};
struct OptionPalette_unk {
    int m00;
    Color4f_unk mColor;
};
extern "C" OptionPalette_unk* fn_8020BCEC(int index);

namespace atn {
// inferred: model (only the name is read here, see include/atn/Model_unk.h)
struct Model {
    const char* fn_8008B130(); // inferred: model name
};
}

namespace atn {
// inferred: see src/atn/CompositeIntensityTintMulti.cpp (0x40 bytes)
struct CompositeLayerDesc_unk {
    const char* mPath;
    Color4f_unk mColor[3];
    int mUse[3];
};
class CompositeIntensityTintMulti {
public:
    CompositeIntensityTintMulti(const char* base, const CompositeLayerDesc_unk* layers, int count);
    char m00[0x38C];
};
} // namespace atn

// inferred: sets a texture compositor on a model (name of the texture, compositor)
extern "C" void fn_80095750(atn::Model* model, const char* name, atn::CompositeIntensityTintMulti* compositor);

// inferred: helpers of the path names (directory, name without extension, joined path)
std::string fn_8006B0C8(const std::string& path);
extern "C" std::string fn_8006B2D8(const std::string& path);
extern "C" std::string fn_8006AFF8(const std::string& dir, const std::string& name);
namespace std {
string operator+(const string& lhs, const char* rhs); // 0x8006D008
}

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

// inferred: colour chosen for a "color" row (0x14 bytes)
struct OptionColorOut_unk {
    int m00;
    short mPalette; // inferred: palette index, < 0 if none
    short mId;
    float mColor[3];
};

// inferred: unused tables (as in db/WeaponParam_unk.cpp and db/EquipParam_unk.cpp); the linker
// strips them, but their literals shared with the tables below come first in .sdata2
static const char* const sRanks[5] = {"\x81\x7C", "S", "A", "B", "C"};
static const char* const sKinds[5] = {"qu", "ql", "qg", "qw", "qs"};
static const char* const sSlots[5] = {"up", "lo", "ga", "wa", "qs"};
static const char sLetters[] = "abdfgjlmwxy";
static const char* const sColorSlots[8] = {"B", "F", "M", "L", "A", "J", "P", "S"};
static const char* const sSlotNames[4] = {"UPPER", "LOWER", "GAUNTLET", "WAIST"};

// inferred: std::equal
static inline bool Equal_unk(const char* first2, const char* first, const char* last) {
    for (; first != last; ++first, ++first2) {
        if (!(*first == *first2)) {
            return false;
        }
    }
    return true;
}

// inferred: whether a name is equal to a key
static inline bool IsKey_unk(const atn::FixedString_unk<0x20>& key, const char* name) {
    unsigned long n = strlen(key.c_str());
    return name[n] == 0 && Equal_unk(name, key.c_str(), key.c_str() + n);
}

// inferred: copies the colours of the palette slots to the "color" rows
extern "C" void fn_8020C4C8(int* palettes, OptionParts_unk* parts, OptionColorOut_unk* out) {
    if (out == 0) {
        return;
    }
    for (int k = 0; k < 8; k++) {
        for (int i = 0; i < parts->mColorCount; i++) {
            if (k == parts->mColors[i].mSlot) {
                OptionPalette_unk* palette = fn_8020BCEC(palettes[k]);
                Color4f_unk c;
                int id;
                if (palette == 0) {
                    c = parts->mColors[i].mColor;
                    id = 0;
                } else {
                    c = palette->mColor;
                    id = palette->m00;
                }
                out[i].mPalette = palettes[k];
                out[i].mId = id;
                out[i].mColor[0] = c.r;
                out[i].mColor[1] = c.g;
                out[i].mColor[2] = c.b;
            }
        }
    }
}

// inferred: index of the option part with a key, -1 if none
static inline int FindPart_unk(OptionParts_unk* parts, const char* name) {
    int result = -1;
    int count = parts->mPartCount;
    for (int i = 0; i < count; i++) {
        if (IsKey_unk(parts->mParts[i].mKey, name)) {
            result = i;
            break;
        }
    }
    return result;
}

// inferred: reads database/cosedit/option/*.csv
// NONMATCHING: 99.26% - register numbering in the "hide"/"base_hide" searches (the original gives the
// two identical branches different registers) and the operands of the inlined std::equal
extern "C" void fn_8020C5D4(OptionParts_unk* parts, const char* data, int size) {
    parts->mColorCount = 0;
    parts->mPartCount = 0;
    parts->mHideCount = 0;
    parts->mBaseHideCount = 0;
    if (data == 0 || size == 0) {
        return;
    }
    atn::Csv csv(data, size);
    do {
        const char* field = csv.fn_8005B9CC();
        if (*field == ';' || *field == 0) {
            continue;
        }
        if (strcmp(field, "option") == 0) {
            parts->mParts[parts->mPartCount].mName = csv.fn_8005B9CC();
            parts->mParts[parts->mPartCount].mKey = csv.fn_8005B9CC();
            parts->mParts[parts->mPartCount].mLevel = atoi(csv.fn_8005B9CC());
            parts->mPartCount++;
        } else if (strcmp(field, "color") == 0) {
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
            parts->mColors[parts->mColorCount].mGroup = csv.fn_8005B9CC();
            parts->mColors[parts->mColorCount].mLayer = csv.fn_8005B9CC();
            const char* channel = csv.fn_8005B9CC();
            if (*channel == 'R') {
                parts->mColors[parts->mColorCount].mChannel = 0;
            } else if (*channel == 'G') {
                parts->mColors[parts->mColorCount].mChannel = 1;
            } else {
                parts->mColors[parts->mColorCount].mChannel = 2;
            }
            parts->mColors[parts->mColorCount].mColor.r = fn_800DC288(csv.fn_8005B9CC());
            parts->mColors[parts->mColorCount].mColor.g = fn_800DC288(csv.fn_8005B9CC());
            parts->mColors[parts->mColorCount].mColor.b = fn_800DC288(csv.fn_8005B9CC());
            parts->mColors[parts->mColorCount].mColor.a = 0.0f;
            const char* slot = csv.fn_8005B9CC();
            parts->mColors[parts->mColorCount].mSlot = -1;
            for (int k = 0; k < 8; k++) {
                if (*slot == *sColorSlots[k]) {
                    parts->mColors[parts->mColorCount].mSlot = k;
                    break;
                }
            }
            parts->mColors[parts->mColorCount].mEnable = atoi(csv.fn_8005B9CC()) != 0;
            const char* mode = csv.fn_8005B9CC();
            if (*mode == '1') {
                parts->mColors[parts->mColorCount].m52 = 1;
            } else if (*mode == '2') {
                parts->mColors[parts->mColorCount].m52 = 2;
            } else {
                parts->mColors[parts->mColorCount].m52 = 0;
            }
            parts->mColorCount++;
        } else if (strcmp(field, "hide") == 0) {
            OptionHide_unk* hide = &parts->mHides[parts->mHideCount];
            hide->mName = csv.fn_8005B9CC();
            hide->mCount = 0;
            for (;;) {
                int index = FindPart_unk(parts, csv.fn_8005B9CC());
                if (index < 0) {
                    break;
                }
                hide->mParts[hide->mCount++] = index;
            }
            parts->mHideCount++;
        } else if (strcmp(field, "base_hide") == 0) {
            OptionHide_unk* hide = &parts->mBaseHides[parts->mBaseHideCount];
            hide->mName = csv.fn_8005B9CC();
            hide->mCount = 0;
            for (;;) {
                int index = FindPart_unk(parts, csv.fn_8005B9CC());
                if (index < 0) {
                    break;
                }
                hide->mParts[hide->mCount++] = index;
            }
            parts->mBaseHideCount++;
        }
    } while (csv.fn_8005BBF8() != 0);
}

// inferred: one tinted layer of a texture group (0x5C bytes)
struct OptionLayer_unk {
    OptionLayer_unk() {
        mUse[0] = 0;
        mUse[1] = 0;
        mUse[2] = 0;
    }
    ~OptionLayer_unk() {}

    atn::FixedString_unk<0x20> mName;
    int mUse[3];
    Color4f_unk mColor[3];
};

// inferred: texture group with its layers (0x24C bytes)
struct OptionGroup_unk {
    OptionGroup_unk() : mCount(0) {}
    ~OptionGroup_unk() {}
    bool IsUsed_unk() {
        for (int i = 0; i < mCount; i++) {
            if (mLayers[i].mUse[0] != 0 || mLayers[i].mUse[1] != 0 || mLayers[i].mUse[2] != 0) {
                return true;
            }
        }
        return false;
    }

    atn::FixedString_unk<0x20> mName;
    OptionLayer_unk mLayers[6];
    int mCount;
};

// inferred: builds the tinted textures of the option colours and sets them on the model
// NONMATCHING: 99.61% - register numbering of the group/layer searches
extern "C" void fn_8020CC64(atn::Model* model, OptionParts_unk* parts, OptionColorOut_unk* out) {
    OptionGroup_unk groups[0x20];
    unsigned int groupCount = 0;
    OptionLayer_unk* layer;
    OptionGroup_unk* group;
    int count;
    for (int i = 0; i < parts->mColorCount; i++) {
        group = 0;
        for (unsigned int j = 0; j < 6; j++) {
            if (IsKey_unk(parts->mColors[i].mGroup, groups[j].mName.c_str())) {
                group = &groups[j];
                break;
            }
        }
        if (group == 0) {
            group = groups + groupCount;
            groupCount++;
            group->mName = parts->mColors[i].mGroup;
        }
        layer = 0;
        count = group->mCount;
        for (int k = 0; k < count; k++) {
            if (IsKey_unk(group->mLayers[k].mName, parts->mColors[i].mLayer.c_str())) {
                layer = &group->mLayers[k];
                break;
            }
        }
        if (layer == 0) {
            layer = &group->mLayers[count];
            group->mCount++;
            layer->mName = parts->mColors[i].mLayer;
        }
        if (out == 0 || out[i].mPalette < 0) {
            layer->mUse[parts->mColors[i].mChannel] = parts->mColors[i].mEnable;
            layer->mColor[parts->mColors[i].mChannel] = parts->mColors[i].mColor;
        } else {
            layer->mUse[parts->mColors[i].mChannel] = out[i].mPalette >= 0;
            layer->mColor[parts->mColors[i].mChannel] = *(Color4f_unk*)out[i].mColor;
        }
    }
    for (unsigned int j = 0; j < groupCount; j++) {
        OptionGroup_unk* group = &groups[j];
        if (!group->IsUsed_unk()) {
            continue;
        }
        std::string dir = fn_8006B0C8(std::string(model->fn_8008B130()));
        std::string base(group->mName.c_str());
        std::string name = fn_8006B2D8(std::string(group->mName.c_str()));
        base = fn_8006AFF8(dir + "/textures/", base);
        std::string paths[6];
        atn::CompositeLayerDesc_unk layers[6];
        for (int k = 0; k < group->mCount; k++) {
            paths[k] = group->mLayers[k].mName.c_str();
            paths[k] = fn_8006AFF8(dir + "/mask/", paths[k]);
            layers[k].mPath = paths[k].c_str();
            memcpy(layers[k].mUse, group->mLayers[k].mUse, sizeof(layers[k].mUse));
            memcpy(layers[k].mColor, group->mLayers[k].mColor, sizeof(layers[k].mColor));
        }
        atn::CompositeIntensityTintMulti* compositor =
            new (6, "", "", 0) atn::CompositeIntensityTintMulti(base.c_str(), layers, group->mCount);
        fn_80095750(model, name.c_str(), compositor);
    }
}

// inferred: index of the option part with the hash of a name, -1 if none
extern "C" int fn_8020D608(OptionParts_unk* parts, unsigned int hash) {
    for (int i = 0; i < parts->mPartCount; i++) {
        unsigned int h = fn_800DC6B4(parts->mParts[i].mName.c_str());
        if (h == hash) {
            return i;
        }
    }
    return -1;
}

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

// inferred: lists of the 11 letters and 4 slots (0xE7B0 bytes)
struct OptionLevelTable_unk {
    OptionLevelList_unk mLists[11][4];
};

// inferred: per level (10), letter (11) and slot (4), whether each option part is set (0x1B80 bytes)
struct OptionSetTable_unk {
    OptionSetTable_unk() { memset(this, 0, sizeof(OptionSetTable_unk)); }
    bool mSets[10][11][4][16];
};

static OptionLevelTable_unk* sLevels;
static OptionSetTable_unk* sSets;

extern "C" OptionLevelList_unk* fn_8020DB18(int letter, int slot);

// inferred: index of a slot name ("UPPER", ...), 0 if none
static inline int FindSlotName_unk(const char* name) {
    const char* slot;
    for (int i = 0; i < 4; i++) {
        slot = sSlotNames[i];
        if (strncmp(slot, name, strlen(slot)) == 0) {
            return i;
        }
    }
    return 0;
}

// inferred: loads database/cosedit/option/option_parts.csv and its names
// NONMATCHING: 97.57% - the original keeps the address of the atn::CsvU in a saved register while
// its inlined constructor runs (known, see db/EquipParam_unk.cpp) and numbers the registers of the
// slot search differently
extern "C" void fn_8020D688() {
    if (sLevels == 0) {
        sLevels = new (1, "", "", 0) OptionLevelTable_unk;
    }
    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, "database/cosedit/option/option_parts.csv", &size, 0x20);
    int textSize;
    void* text =
        fn_8046DC5C(lbl_8087F518, "database/cosedit/option/option_parts_name_xx.u16", (unsigned long*)&textSize, 0x20);
    atn::Csv csv((const char*)data, size);
    atn::CsvU csvu((const wchar_t*)text, textSize / 2);
    while (csv.fn_8005BBF8() != 0) {
        csvu.fn_8005BBF8();
        const char* field = csv.fn_8005B9CC();
        if (*field == ';' || *field == '#') {
            continue;
        }
        if (strcmp(field, "") == 0) {
            continue;
        }
        int letter = fn_8020F064(field);
        int slot = FindSlotName_unk(csv.fn_8005B9CC());
        if (!(0 <= letter && letter < 11 && 0 <= slot && slot < 4)) {
            continue;
        }
        OptionLevelList_unk* list = &sLevels->mLists[letter][slot];
        OptionLevel_unk* level = &list->mLevels[list->mCount];
        level->m00 = fn_800DC12C(csv.fn_8005B9CC());
        for (int i = 0; i < 3; i++) {
            level->mHashes[i] = fn_800DC6B4(csv.fn_8005B9CC());
        }
        csv.fn_8005B9CC();
        level->mName = csvu.fn_8005B9CC();
        if (strncmp(csv.fn_8005B9CC(), "FREE", 4) == 0) {
            level->mFree = 1;
        }
        list->mCount++;
    }
    fn_8046DD20(lbl_8087F518, data);
    fn_8046DD20(lbl_8087F518, text);
}

extern "C" OptionLevelList_unk* fn_8020DB18(int letter, int slot) {
    if (0 <= letter && letter < 11 && 0 <= slot && slot < 4) {
        return &sLevels->mLists[letter][slot];
    }
    return 0;
}

// inferred: loads database/cosedit/option/pc_set_<letter>_level.csv
extern "C" void fn_8020DB58() {
    if (sSets == 0) {
        sSets = new (1, "", "", 0) OptionSetTable_unk;
    }
    int section = -1;
    int row = 0;
    for (int i = 0; i < 11; i++) {
        char path[0x100];
        sprintf(path, "database/cosedit/option/pc_set_%c_level.csv", sLetters[i]);
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
        atn::Csv csv((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (strcmp(field, "\0") == 0) {
                continue;
            }
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "option") == 0) {
                csv.fn_8005B9CC();
                csv.fn_8005B9CC();
                for (int level = 0; level < 10; level++) {
                    bool* sets = sSets->mSets[level][i][section];
                    sets[row] = strncmp(csv.fn_8005B9CC(), "1", 1) == 0;
                }
                row++;
            } else if (strcmp(field, "<up>") == 0) {
                section = 0;
                row = 0;
            } else if (strcmp(field, "<ga>") == 0) {
                section = 2;
                row = 0;
            } else if (strcmp(field, "<wa>") == 0) {
                section = 3;
                row = 0;
            } else if (strcmp(field, "<lo>") == 0) {
                section = 1;
                row = 0;
            } else if (strncmp(field, "</", 2) == 0) {
                section = -1;
                row = 0;
            }
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: options set for a level, letter and slot
static inline bool* GetSets_unk(int level, int letter, int slot) {
    if (0 > level || level >= 10) {
        return 0;
    }
    if (0 > letter || letter >= 11) {
        return 0;
    }
    if (0 > slot || slot >= 4) {
        return 0;
    }
    return sSets->mSets[level][letter][slot];
}

// inferred: sets the level of the option parts from the name of a costume model
// NONMATCHING: 99.79% - the original increments the level pointer before the loop counter
// ("pcXXXX_YY<slot>_<letter>..._<level>")
extern "C" void fn_8020DE0C(OptionParts_unk* parts, const char* name) {
    int length = strlen(name);
    int level = -1;
    int letter = -1;
    int slot = -1;
    if (length >= 9) {
        for (int i = 0; i < 4; i++) {
            if (strncmp(sSlots[i], name + 8, 2) == 0) {
                slot = i;
                break;
            }
        }
    }
    if (length >= 11) {
        for (int i = 0; i < 11; i++) {
            if (sLetters[i] == name[11]) {
                letter = i;
                break;
            }
        }
    }
    if (length >= 15) {
        level = fn_800DC12C(name + 15);
    }
    bool* sets = GetSets_unk(level, letter, slot);
    if (sets != 0) {
        parts->SetLevels_unk(sets);
    }
}
