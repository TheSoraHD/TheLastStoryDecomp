// inferred file name (splits-auto): shared menu helpers (texts of the message manager, system sounds,
// item category table, colors). The real name is unknown.
#include "std/string.h"
#include "atn/SoundRef.h"

extern "C" void* memset(void* dst, int c, unsigned long n);

// inferred: message manager (lbl_8087F138); the text lookup object is at +0x168
struct MenuMsg_unk;
struct MenuMsgOwner_unk {
    unsigned char m000[0x168];
    unsigned char m168[1];
};
extern "C" MenuMsgOwner_unk* lbl_8087F138;
extern "C" void fn_801F3E20(void* msg, std::basic_string<wchar_t>* out, int id); // inferred: text by id
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
// inferred: message table (lbl_8087F1E4) with 8-byte entries
struct MenuMsgEntry_unk {
    int m00;
    const wchar_t* mText;
};
struct MenuMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mEntries[id].mText != 0 ? mEntries[id].mText : lbl_808813D0; }
    MenuMsgEntry_unk mEntries[1];
};
extern "C" MenuMsgTable_unk* lbl_8087F1E4;
extern "C" int fn_800DD3FC(wchar_t* dst, const wchar_t* fmt, ...); // inferred: swprintf
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: plays a sound
// inferred: sound manager (lbl_8087EFE8)
struct MenuSound_unk {
    unsigned char m0000[0x34D0];
    int m34D0; // inferred: sound category of the next sounds
};
extern "C" MenuSound_unk* lbl_8087EFE8;

// inferred: item data read by fn_8011728C/fn_801173A8 (class unknown)
struct MenuItem_unk {
    int m00;
    int m04; // inferred: item id
    unsigned char m08[0xBC - 0x08];
    short mBC; // inferred: item kind
};

// inferred: color with a constructor (initialized by the __sinit)
struct MenuColor_unk {
    MenuColor_unk(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {}
    float r, g, b, a;
};

// inferred: menu category (7 entries)
struct MenuCategory_unk {
    int mText;    // inferred: message id of the name
    int mAll;     // inferred: accepts every item
    int mOrder;   // inferred: value returned by fn_801173A8
    int mKind[4]; // inferred: item kinds (mBC) of the category, -1 = unused
};

const wchar_t* const lbl_8088171C = L""; // inferred: text returned for an unknown source

// inferred: system sound names, indexed by fn_80117228
const char* const lbl_807368F8[] = {
    "SE_UICOM_009", "SE_UICOM_001", "SE_UICOM_002", "SE_UICOM_003", "SE_UICOM_003", "SE_UICOM_008",
    "SE_UICOM_013", "SE_UICOM_014", "SE_UICOM_015", "SE_UICOM_016", "SE_UICOM_017", "SE_UICOM_018",
    "SE_UICOM_025", "SE_UICOM_028", "SE_UICOM_048", "SE_UICOM_028", "SE_UICOM_042", "SE_UICOM_017",
    "SE_UICOM_028", "SE_UICOM_005", "SE_UI001_101", "SE_UI001_102", "SE_UI001_103", "SE_UI001_104",
    "SE_UI001_105", "SE_UI001_106", "SE_UI001_107", "SE_UI001_108", "SE_UI001_109", "SE_UI001_108",
    "SE_UICOM_017", "SE_UICOM_043", "SE_UICOM_044", "SE_UICOM_016", 0,
};

// inferred: prefixes of the stat names
const char* const lbl_807369AC[] = {"atk_", "def_", "agi_", "mdef_", "matk_"};

const int lbl_807369C0[] = {5, 7, 2, 4, 6, 0, 1, 3};

const MenuCategory_unk lbl_807369E0[] = {
    {800, 0, 0, {5, -1, -1, -1}}, {801, 0, 1, {2, -1, -1, -1}}, {802, 0, 2, {3, -1, -1, -1}},
    {803, 0, 3, {9, -1, -1, -1}}, {804, 0, 4, {8, 7, 1, -1}},   {805, 0, 5, {0, -1, -1, -1}},
    {806, 1, 6, {-1, -1, -1, -1}},
};

MenuColor_unk lbl_807C7898(20.0f, 100.0f, 200.0f, 255.0f);
MenuColor_unk lbl_807C78A8(205.0f, 90.0f, 90.0f, 255.0f);
MenuColor_unk lbl_807C78B8(50.0f, 50.0f, 50.0f, 255.0f);
MenuColor_unk lbl_807C78C8(220.0f, 220.0f, 220.0f, 255.0f);
static std::basic_string<wchar_t> lbl_807C78E4(L""); // inferred: text returned by the getters
static wchar_t lbl_807C78F0[0x100];                  // inferred: formatted text (fn_801170C8)

// inferred: text `id` of source `kind` (0: message manager, 1: message table)
extern "C" const wchar_t* fn_80116FC0(int kind, int id);

struct MenuTextRef_unk {
    int mKind;
    int mId;
};

// NONMATCHING: 99.7% - same as fn_80116FC0 (inlined)
extern "C" const wchar_t* fn_80116EB8(const MenuTextRef_unk* ref) {
    return fn_80116FC0(ref->mKind, ref->mId);
}

// NONMATCHING: 99.7% - in the message table lookup the original keeps the table in r0 and the index
// in r3 (`add r3, r0, r3`); here they are swapped (tried: local entry pointer, pointer arithmetic,
// index on the left, an inline wrapper)
extern "C" const wchar_t* fn_80116FC0(int kind, int id) {
    lbl_807C78E4 = L"";
    if (kind == 0) {
        void* msg = lbl_8087F138->m168;
        if (msg != 0) {
            fn_801F3E20(msg, &lbl_807C78E4, id);
        }
        return lbl_807C78E4.c_str();
    }
    if (kind == 1) {
        const wchar_t* text = lbl_8087F1E4->Get_unk(id);
        if (text != 0) {
            return text;
        }
    }
    return lbl_8088171C;
}

// NONMATCHING: 99.7% - same as fn_80116FC0 (inlined)
// inferred: formats the text `id` with one argument
extern "C" const wchar_t* fn_801170C8(int kind, int id, int arg) {
    memset(lbl_807C78F0, 0, sizeof(lbl_807C78F0));
    fn_800DD3FC(lbl_807C78F0, fn_80116FC0(kind, id), arg);
    return lbl_807C78F0;
}

extern "C" const char* fn_80117200(int stat) {
    return lbl_807369AC[stat];
}

extern "C" int fn_80117214(int index) {
    return lbl_807369C0[index];
}

// inferred: plays the system sound `id`
extern "C" atn::SoundRef fn_80117228(int id) {
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 5;
    }
    atn::SoundRef ref = fn_800C31F4(lbl_807368F8[id], 0, -1, 1.0f);
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 0;
    }
    return ref;
}

// inferred: whether `item` belongs to category `category` (99 and 98 are special)
extern "C" int fn_8011728C(int category, MenuItem_unk* item) {
    if (item == 0) {
        return 0;
    }
    if (category == 99) {
        if (0x26D <= item->m04 && item->m04 <= 0x270) {
            return 1;
        }
    } else if (category == 98) {
        return item->mBC != 9;
    } else {
        if (category < 0 || 7 <= category) {
            return 0;
        }
        const MenuCategory_unk* entry = &lbl_807369E0[category];
        if (entry->mAll != 0) {
            return 1;
        }
        for (int i = 0; i < 4; i++) {
            if (entry->mKind[i] >= 0 && item->mBC == entry->mKind[i]) {
                return 1;
            }
        }
    }
    return 0;
}

// inferred: order of the category of `item` (100 if none)
extern "C" int fn_801173A8(MenuItem_unk* item) {
    if (item == 0) {
        return 100;
    }
    for (int i = 0; i < 6; i++) {
        const MenuCategory_unk* entry = &lbl_807369E0[i];
        for (int j = 0; j < 4; j++) {
            if (entry->mKind[j] >= 0 && item->mBC == entry->mKind[j]) {
                return entry->mOrder;
            }
        }
    }
    return 100;
}

// inferred: name of category `category`
extern "C" const wchar_t* fn_8011745C(int category) {
    if (category < 0 || 7 <= category) {
        return fn_80116FC0(0, 800);
    }
    return fn_80116FC0(0, lbl_807369E0[category].mText);
}
