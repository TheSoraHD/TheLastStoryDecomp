#include "game/CampSaveLoad.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: creates a d2anime task playing `path` (returns the task)
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int arg);
// inferred: creates a d2anime task (other variant of fn_801F3FF8, no extra argument)
extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path);
// inferred: makes the save data of the save menu
extern "C" CampSaveData_unk* fn_803B7944(atn::Task* parent, void* arg);
extern "C" void fn_803B8758(CampSaveData_unk* data, int arg);

// inferred: copies a string unless it is the destination itself (name invented)
inline void CopyString_unk(char* dst, const char* src) {
    if (src != dst) {
        memcpy(dst, src, strlen(src) + 1);
    }
}

// inferred: name and flag given to fn_803B27F8 (see UnkArg in src/auto_03_803B27AC_text.cpp)
struct CampSaveArg_unk {
    char mName[0x40];
    int mFlag; // 0x40
};
extern "C" CampSaveData_unk* fn_803B27F8(atn::Task* parent, const CampSaveArg_unk* arg);

// inferred: field data (lbl_8087F430)
struct Field_unk {
    unsigned char m0000[0x5634];
    void* m5634;
};
extern "C" Field_unk* lbl_8087F430;

// inferred: game settings (lbl_8087F0A8); only the members used here
struct Settings_unk {
    unsigned char m0000[0x113C];
    int m113C;           // inferred: chapter (1-based)
    unsigned char m1140;
};
extern "C" Settings_unk* lbl_8087F0A8;
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" int fn_800DC1DC(const wchar_t* s); // inferred: wide string to int

// inferred: slot chosen in the save menu (-1: none); read by 0x8039BDB4
extern "C" int lbl_8087D990;
int lbl_8087D990 = -1;

// inferred: separators of atn::CsvU (declared in atn/Csv.h)
wchar_t lbl_8077A0A8[3] = L" \t";

class MenuSaveTask : public CampSaveLoad {
public:
    MenuSaveTask(atn::Task* parent);
    static MenuSaveTask* fn_8011770C(atn::Task* parent);
    virtual ~MenuSaveTask();
    virtual void vf20();

    static MenuSaveTask* sInstance; // 0x8087F068
};

class MenuLoadTask : public CampSaveLoad {
public:
    MenuLoadTask(atn::Task* parent, CampSaveData_unk* data);
    static MenuLoadTask* fn_80117914(atn::Task* parent, CampSaveData_unk* data);
    virtual ~MenuLoadTask();

    static MenuLoadTask* sInstance; // 0x8087F06C
};

MenuSaveTask* MenuSaveTask::sInstance;
MenuLoadTask* MenuLoadTask::sInstance;

MenuSaveTask* MenuSaveTask::fn_8011770C(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) MenuSaveTask(parent);
    }
    return sInstance;
}

// inferred: not inline (its literals are in the pool); inlined into fn_8011770C
MenuSaveTask::MenuSaveTask(atn::Task* parent) : CampSaveLoad(parent) {
    mMode = 0;
    (m70 = fn_801F3FF8(this, "/data/d2anime/save_select.d2a", 0))->fn_800D246C(1);
    m4C = fn_803B7944(this, lbl_8087F430 != 0 ? lbl_8087F430->m5634 : 0);
    m4C->mD64 = 0;
}

MenuSaveTask::~MenuSaveTask() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

void MenuSaveTask::vf20() {
    lbl_8087D990 = m150 != 0 ? m128 : -1;
}

MenuLoadTask* MenuLoadTask::fn_80117914(atn::Task* parent, CampSaveData_unk* data) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) MenuLoadTask(parent, data);
    }
    return sInstance;
}

// inferred: not inline (its literals are in the pool); inlined into fn_80117914
MenuLoadTask::MenuLoadTask(atn::Task* parent, CampSaveData_unk* data) : CampSaveLoad(parent) {
    mMode = 1;
    (m70 = fn_801F3FF8(this, "/data/d2anime/load_select.d2a", 0))->fn_800D246C(1);
    m50 = data;
    if (data == 0) {
        CampSaveArg_unk arg;
        // inferred: a 2-byte literal (0x80736B4D: 00 00), not merged with ""
        CopyString_unk(arg.mName, "\0");
        arg.mFlag = 1;
        m50 = fn_803B27F8(this, &arg);
    } else if (data->mD60 == 0) {
        fn_803B8758(data, -1);
    }
    m50->mD64 = 0;
}

MenuLoadTask::~MenuLoadTask() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

CampSaveLoad::CampSaveLoad(atn::Task* parent)
    : atn::Task(parent), m4C(0), m50(0), m54(0), m128(1), m138(0), m13C(0), m148(0), m150(0), m154(0), m158(0),
      m15C(0), m160(0), m164(0), m168(0), m16C(0), m170(0), m174(0), m4B4(0) {
    m4AC.fn_80470364("/preload/saveload.pk", 0, 0, 1);
    m130 = 0;
    m12C = 0;
    m134 = 0;
    if (lbl_8087F0A8 != 0) {
        m174 = lbl_8087F0A8->m1140;
        m128 = lbl_8087F0A8->m113C - 1;
    }
    m178 = 0;
    (m58 = fn_801F3FF8(this, "/data/d2anime/ui_event_icon_new_map_loop.d2a", 0))->fn_800D246C(1);
    (m5C = fn_801F3FF8(this, "/data/d2anime/save_info_bar.d2a", 0))->fn_800D246C(1);
    (m60 = fn_801F3FF8(this, "/data/d2anime/camp_info.d2a", 0))->fn_800D246C(1);
    (m64 = fn_801F3FF8(this, "/data/d2anime/camp_info2.d2a", 0))->fn_800D246C(1);
    (m68 = fn_801F3FF8(this, "/data/d2anime/camp_info5.d2a", 0))->fn_800D246C(1);
    (m118 = fn_801F3FF8(this, "/data/d2anime/save_arrow_l.d2a", 0))->fn_800D246C(1);
    m11C = 0;
    (m120 = fn_801F3FF8(this, "/data/d2anime/save_arrow_r.d2a", 0))->fn_800D246C(1);
    m124 = 0;
    (m6C = fn_801F3FF8(this, "/data/d2anime/camp_cursor.d2a", 0))->fn_800D246C(1);
    const char* clearPath = "/data/d2anime/save_select_clear.d2a";
    for (int i = 0; i < 8; i++) {
        m74[i] = 0;
        (m94[i] = fn_801F3FF8(this, "/data/d2anime/save_select_icon.d2a", 0))->fn_800D246C(1);
        (mB4[i] = fn_801F3FF8(this, "/data/d2anime/save_select_nodata.d2a", 0))->fn_800D246C(1);
        (mF4[i] = fn_801F64D0(this, clearPath))->fn_800D246C(1);
        mD4[i] = 0;
    }
    (m114 = fn_801F3FF8(this, "/data/d2anime/save_select_icon2.d2a", 0))->fn_800D246C(1);
    m4B8.vf0C("database/saveload/story_log_message_xx.u16");
    m4C8 = 0.0f;
}

CampSaveLoad::~CampSaveLoad() {
    mEntries.Free_unk();
}

void CampSaveLoad::vf1C() {}

void CampSaveLoad::vf20() {}

// NONMATCHING: the original keeps the address of csv in a saved register while it is
// constructed (one more saved register and a 0x10 bytes larger frame)
void CampSaveLoad::fn_8011B8D4(const wchar_t* text, int length) {
    atn::CsvU csv(text, length);
    int count = 0;
    while (csv.fn_8005BBF8()) {
        if (wcscmp(csv.fn_8005B9CC(), L"") == 0) {
            int a = fn_800DC1DC(csv.fn_8005B9CC());
            int b = fn_800DC1DC(csv.fn_8005B9CC());
            if (a > 0 && b > 0) {
                count++;
            }
        }
    }
    mEntries.Resize_unk(count, 0);
    int i = 0;
    csv.vf08(text, length);
    while (csv.fn_8005BBF8()) {
        if (wcscmp(csv.fn_8005B9CC(), L"") == 0) {
            int a = fn_800DC1DC(csv.fn_8005B9CC());
            int b = fn_800DC1DC(csv.fn_8005B9CC());
            int c = fn_800DC1DC(csv.fn_8005B9CC());
            if (a > 0 && b > 0) {
                CampSaveLoadEntry_unk& entry = mEntries[i];
                entry.m00 = a;
                entry.m04 = b;
                entry.m08 = c;
                entry.m0C = csv.fn_8005B9CC();
                i++;
            }
        }
    }
}
