#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" int strcmp(const char* a, const char* b);
extern "C" char* strcpy(char* dst, const char* src);
extern "C" int sprintf(char* dst, const char* fmt, ...);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wmemcpy(wchar_t* dst, const wchar_t* src, unsigned long n);
extern "C" int fn_800DC12C(const char* s);   // inferred: string to int
extern "C" float fn_800DC288(const char* s); // inferred: string to float

// inferred: the font (lbl_8087EEC8) converts UTF-16 text to the game encoding
extern "C" void* lbl_8087EEC8;
extern "C" int fn_8006F420(void* font, char* dst, const wchar_t* src, int size);

// inferred: resource manager (lbl_8087F518): loads a whole file / releases it
struct Unk_8087F518;
extern "C" Unk_8087F518* lbl_8087F518;
extern "C" void* fn_8046DC5C(Unk_8087F518* mgr, const char* path, unsigned long* size, int align);
extern "C" void fn_8046DD20(Unk_8087F518* mgr, void* data);

// inferred: colour
struct PaletteColor_unk {
    float r;
    float g;
    float b;
    float a;
};

// inferred: colour read from the CSV (alpha 1 by default)
struct PaletteColorInit_unk : public PaletteColor_unk {
    PaletteColorInit_unk() { a = 1.0f; }
};

// inferred: one colour of the costume editor palette (0x54 bytes)
struct Palette_unk {
    Palette_unk() { mName[0] = 0; }
    ~Palette_unk() {}
    void SetName_unk(const wchar_t* name) {
        if (name != mName) {
            wmemcpy(mName, name, wcslen(name) + 1);
        }
    }

    int m00;
    PaletteColor_unk mColor;
    wchar_t mName[0x20];
};

struct PaletteSet_unk {
    Palette_unk mPalettes[64];
};

struct EffPaletteSet_unk {
    Palette_unk mPalettes[32];
};

struct PaletteIds_unk {
    int mIds[64];
};

struct EffPaletteIds_unk {
    int mIds[32];
};

// inferred: 7 colour slots of 8 presets
struct PaletteSlots_unk {
    int mSlots[7][8];
};

// inferred: colour names (four shades per group) and the name of each group
static const char* const sGroupNames[16] = {
    "sv00", "rd00", "pk00", "pu00", "bl00", "bk00", "gr00", "kh00",
    "yl00", "or00", "sk00", "br00", "yy00", "gg00", "rr00", "bb00",
};
static const char* const sColorNames[65] = {
    "sv01", "sv02", "sv03", "sv04", "rd01", "rd02", "rd03", "rd04",
    "pk01", "pk02", "pk03", "pk04", "pu01", "pu02", "pu03", "pu04",
    "bl01", "bl02", "bl03", "bl04", "bk01", "bk02", "bk03", "bk04",
    "gr01", "gr02", "gr03", "gr04", "kh01", "kh02", "kh03", "kh04",
    "yl01", "yl02", "yl03", "yl04", "or01", "or02", "or03", "or04",
    "sk01", "sk02", "sk03", "sk04", "br01", "br02", "br03", "br04",
    "yy01", "yy02", "yy03", "yy04", "gg01", "gg02", "gg03", "gg04",
    "rr01", "rr02", "rr03", "rr04", "bb01", "bb02", "bb03", "bb04",
    "",
};
static const char* const sEffNames[32] = {
    "ef001", "ef002", "ef003", "ef011", "ef021", "ef022", "ef031", "ef041",
    "ef051", "ef052", "ef053", "ef061", "ef062", "ef071", "ef072", "ef073",
    "ef081", "ef082", "ef083", "ef091", "ef092", "ef093", "ef101", "ef102",
    "ef111", "ef112", "ef113", "ef121", "ef122", "ef123", "ef124",
    "",
};
static const char* sInvNames[2] = {"inv_up", "inv_low"};

static Palette_unk* sPalettes;      // inferred: PaletteSet_unk
static Palette_unk* sEffPalettes;   // inferred: EffPaletteSet_unk
static PaletteSlots_unk* sPresets;
static PaletteIds_unk* sPaletteIds;
static EffPaletteIds_unk* sEffPaletteIds;

// inferred: index of a colour name, -1 if unknown
static inline int FindColor_unk(const char* name) {
    int index = -1;
    for (int i = 0; i < 64; i++) {
        if (strcmp(name, sColorNames[i]) == 0) {
            index = i;
            break;
        }
    }
    return index;
}

// inferred: index of an effect colour name, -1 if unknown
static inline int FindEffColor_unk(const char* name) {
    int index = -1;
    for (int i = 0; i < 32; i++) {
        if (strcmp(name, sEffNames[i]) == 0) {
            index = i;
            break;
        }
    }
    return index;
}

// inferred: loads the colour palettes of the costume editor (ids, colours and names)
// NONMATCHING: 98.03% - register allocation (zero constant, data pointer) and one byte less in the
// string pool; tried static helpers per block (not inlined then). The UTF-16 CSV address is now in
// a saved register as in the original (empty ~CsvBase())
extern "C" void fn_8020B2BC() {
    if (sPaletteIds == 0) {
        sPaletteIds = new (1, "", "", 0) PaletteIds_unk;
    }
    if (sPalettes == 0) {
        sPalettes = (new (1, "", "", 0) PaletteSet_unk)->mPalettes;
    }
    {
        char path[0x100];
        sprintf(path, "database/cosedit/palette/db_palette_id_table.csv");
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
        atn::Csv csv((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "") == 0) {
                continue;
            }
            char name[8];
            strcpy(name, field);
            int id = fn_800DC12C(csv.fn_8005B9CC());
            int index = FindColor_unk(name);
            if (index < 0) {
                continue;
            }
            sPaletteIds->mIds[id] = index;
            PaletteColorInit_unk color;
            color.r = fn_800DC288(csv.fn_8005B9CC());
            color.g = fn_800DC288(csv.fn_8005B9CC());
            color.b = fn_800DC288(csv.fn_8005B9CC());
            sPalettes[index].mColor = color;
            sPalettes[index].m00 = 0;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
    {
        char path[0x100];
        sprintf(path, "database/cosedit/palette/db_palette_name_table_xx.u16");
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
        atn::CsvU csv((const wchar_t*)data, (int)size / 2);
        while (csv.fn_8005BBF8() != 0) {
            const wchar_t* field = csv.fn_8005B9CC();
            if (*field == L';' || *field == L'#') {
                continue;
            }
            if (wcscmp(field, L"") == 0) {
                continue;
            }
            char name[8];
            fn_8006F420(lbl_8087EEC8, name, field, 8);
            int index = FindColor_unk(name);
            if (index < 0) {
                continue;
            }
            sPalettes[index].SetName_unk(csv.fn_8005B9CC());
        }
        fn_8046DD20(lbl_8087F518, data);
    }
    if (sEffPaletteIds == 0) {
        sEffPaletteIds = new (1, "", "", 0) EffPaletteIds_unk;
    }
    if (sEffPalettes == 0) {
        sEffPalettes = (new (1, "", "", 0) EffPaletteSet_unk)->mPalettes;
    }
    {
        char path[0x100];
        sprintf(path, "database/cosedit/palette/db_eff_palette_id_table.csv");
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
        atn::Csv csv((const char*)data, size);
        while (csv.fn_8005BBF8() != 0) {
            const char* field = csv.fn_8005B9CC();
            if (*field == ';' || *field == '#') {
                continue;
            }
            if (strcmp(field, "") == 0) {
                continue;
            }
            char name[8];
            strcpy(name, field);
            int id = fn_800DC12C(csv.fn_8005B9CC());
            int index = FindEffColor_unk(name);
            if (index < 0) {
                continue;
            }
            sEffPaletteIds->mIds[id] = index;
            sEffPalettes[index].m00 = index;
        }
        fn_8046DD20(lbl_8087F518, data);
    }
    {
        char path[0x100];
        sprintf(path, "database/cosedit/palette/db_eff_palette_name_table_xx.u16");
        unsigned long size;
        void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
        atn::CsvU csv((const wchar_t*)data, (int)size / 2);
        while (csv.fn_8005BBF8() != 0) {
            const wchar_t* field = csv.fn_8005B9CC();
            if (*field == L';' || *field == L'#') {
                continue;
            }
            if (wcscmp(field, L"") == 0) {
                continue;
            }
            char name[8];
            fn_8006F420(lbl_8087EEC8, name, field, 8);
            int index = FindEffColor_unk(name);
            if (index < 0) {
                continue;
            }
            sEffPalettes[index].SetName_unk(csv.fn_8005B9CC());
        }
        fn_8046DD20(lbl_8087F518, data);
    }
}

// inferred: loads the colour presets (7 colour names per row)
// NONMATCHING: 99.41% - the original has unused locals below the path (floats 1.0f stored at
// 0x14, 0x28, 0x3c and 0x40 of the frame, stack 0x7B0); their type is unknown
extern "C" void fn_8020BB0C() {
    if (sPresets == 0) {
        sPresets = new (1, "", "", 0) PaletteSlots_unk;
    }
    char path[0x100];
    sprintf(path, "database/cosedit/preset/edit_preset.csv");
    unsigned long size;
    void* data = fn_8046DC5C(lbl_8087F518, path, &size, 0x20);
    atn::Csv csv((const char*)data, size);
    PaletteColorInit_unk colors[4]; // inferred: not used
    int preset = 0;
    while (csv.fn_8005BBF8() != 0) {
        const char* field = csv.fn_8005B9CC();
        if (*field == ';' || *field == '#') {
            continue;
        }
        if (strcmp(field, "") == 0) {
            continue;
        }
        for (int slot = 0; slot < 7; slot++) {
            const char* name = csv.fn_8005B9CC();
            sPresets->mSlots[slot][preset] = -1;
            for (int i = 0; i < 64; i++) {
                if (strcmp(name, sColorNames[i]) == 0) {
                    sPresets->mSlots[slot][preset] = i;
                    break;
                }
            }
        }
        preset++;
    }
    fn_8046DD20(lbl_8087F518, data);
}

extern "C" Palette_unk* fn_8020BCEC(int index) {
    if (0 > index || index >= 64) {
        return 0;
    }
    return &sPalettes[index];
}

extern "C" Palette_unk* fn_8020BD14(int index) {
    if (0 > index || index >= 32) {
        return 0;
    }
    return &sEffPalettes[index];
}

// inferred: name of the group of a colour
extern "C" const char* fn_8020BD3C(int index) {
    if (0 <= index && index < 64) {
        return sGroupNames[index / 4];
    }
    return sColorNames[64];
}

extern "C" const char* fn_8020BD78(int index) {
    if (0 <= index && index < 32) {
        return sEffNames[index];
    }
    return sEffNames[32]; // inferred: one past the end in the original (the table has 32 entries)
}

extern "C" const char* fn_8020BDAC(int index) {
    if (0 <= index && index < 2) {
        return sInvNames[index];
    }
    return sEffNames[32];
}

extern "C" int fn_8020BDE0(int id) {
    if (sPaletteIds != 0 && 0 <= id && id < 64) {
        return sPaletteIds->mIds[id];
    }
    return 0;
}

extern "C" int fn_8020BE10(int id) {
    if (sEffPaletteIds != 0 && 0 <= id && id < 32) {
        return sEffPaletteIds->mIds[id];
    }
    return 0;
}

// inferred: id of a colour index
extern "C" int fn_8020BE40(int index) {
    for (int i = 0; i < 64; i++) {
        if (index == sPaletteIds->mIds[i]) {
            return i;
        }
    }
    return 0;
}

extern "C" int fn_8020BF20(int index) {
    for (int i = 0; i < 32; i++) {
        if (index == sEffPaletteIds->mIds[i]) {
            return i;
        }
    }
    return 0x1F;
}

extern "C" int* fn_8020C000(int slot) {
    if (0 <= slot && slot < 7) {
        return sPresets->mSlots[slot];
    }
    return 0;
}
