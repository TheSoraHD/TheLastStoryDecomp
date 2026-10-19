#include "game/CampSaveLoad.h"
#include "atn/Csv.h"
#include "atn/Memory.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: creates a d2anime task playing `path` (returns the task)
extern "C" atn::Task* fn_801F3FF8(atn::Task* parent, const char* path, int arg);
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
