#include "game/CampSaveLoad.h"
#include "macros.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "MP/SaveLoad.h"

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

extern "C" int fn_803BEB54(SaveSlot_unk* slot); // inferred: nonzero if the slot is empty

// inferred: pad input (lbl_8087EF70); names invented
extern "C" void* lbl_8087EF70;
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
inline int IsTriggered_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A555C(lbl_8087EF70, 0, button) : 0;
}
inline bool IsRepeated_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A55AC(lbl_8087EF70, 0, button) : 0;
}

// inferred: save and load of a slot (save menu: fn_803B7FC8/fn_803B807C, load menu:
// fn_803B3270/fn_803B3310)
extern "C" void fn_803B7FC8(CampSaveData_unk* data, int slot);
extern "C" void fn_803B807C(CampSaveData_unk* data, int slot);
extern "C" void fn_803B3270(CampSaveData_unk* data, int slot);
extern "C" void fn_803B3310(CampSaveData_unk* data, int slot);

// inferred: plays a system sound effect
extern "C" atn::SoundRef fn_80117228(int id);
// inferred: creates a message window task
extern "C" atn::Task* fn_805BC6C8(atn::Task* parent, int msg, int arg);
extern "C" void fn_8036E964(Field_unk* field);

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
// inferred: 2D point of a d2anime pane (names invented)
struct D2APoint_unk {
    D2APoint_unk() {}
    float x;
    float y;
    float mScaleX;
    float mScaleY;
    float m10;
    float m14;
};
// inferred: position of the pane `hash` of `layout`
extern "C" D2APoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801F4728(D2ATask_unk* task, const char* name, const D2APoint_unk* pos);
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const D2APoint_unk* pos);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
// inferred: sets the value `index` of the pane `name` (name invented)
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
extern "C" void fn_801F6D7C(atn::Task* task, const char* name, int index, float value);
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

void CampSaveLoad::fn_80119030(int state, int arg) {
    m140 = m13C;
    m13C = state;
    m144 = arg;
    switch (state) {
    case 0:
        fn_80117228(0x1F);
        vf1C();
        break;
    case 3:
        if (m16C == 0) {
            m148 = 0;
            if (mMode == 1 && m174 == 0 && fn_8011BBD0(m128)) {
                (m178 = fn_805BC6C8(this, 0x1B5, 0))->fn_800D246C(1);
                if (lbl_8087F0A8 != 0) {
                    lbl_8087F0A8->m1140 = 1;
                }
                m174 = 1;
            }
        } else {
            m148 = 1;
        }
        break;
    case 5:
        m14C = 0;
        break;
    case 7:
        vf20();
        if (mMode == 0 && lbl_8087F430 != 0 && m4AC.m04 == 0) {
            fn_8036E964(lbl_8087F430);
        }
        break;
    case 6:
    case 8:
        m150 = 1;
        break;
    }
}

void CampSaveLoad::fn_8011968C() {
    if (IsTriggered_unk(4) || IsTriggered_unk(5)) {
        if (m128 == 0 || mMode != 1 || m16C != 0 || m170 != 2) {
            fn_80119030(1, 0);
            fn_80117228(2);
        } else {
            m16C = 1;
            fn_80117228(1);
            fn_80119030(3, 0);
        }
        m170 = 0;
    }
}

void CampSaveLoad::fn_8011979C() {
    if (IsTriggered_unk(4)) {
        if (m148 == 0) {
            if (m16C == 0) {
                if (lbl_8087F0A8 != 0) {
                    lbl_8087F0A8->m113C = m128;
                }
                if (mMode == 0) {
                    fn_803B7FC8(m4C, m128);
                } else {
                    fn_803B3270(m50, m128);
                }
                fn_80119030(5, 0);
                fn_80117228(1);
            } else if (m16C == 2) {
                fn_80119030(7, 0);
                fn_80117228(1);
            } else {
                if (mMode == 0) {
                    fn_803B807C(m4C, m128);
                } else {
                    fn_803B3310(m50, m128);
                }
                fn_80119030(5, 0);
                fn_80117228(1);
            }
        } else {
            fn_80119030(1, 0);
            fn_80117228(2);
        }
    } else if (IsTriggered_unk(5)) {
        fn_80119030(1, 0);
        fn_80117228(2);
    } else {
        m154 = m148;
        m158 = 2;
        fn_8011B3D8(1);
        m148 = m154;
    }
}

// inferred: out-of-line destructor of the save elements (weak copy, emitted here)
MP::SaveElemB_unk::~SaveElemB_unk() {}

// NONMATCHING: 88.8% - the original converts each pad result to a bool value (neg/or/srwi)
// before testing it
// NONMATCHING: 98.7% - floating-point register allocation of the two frame values (and the
// string pool offsets until all the functions of the file are written)
void CampSaveLoad::fn_8011AD38() {
    atn::Task* layout = m70;
    for (int i = 0; i < 8; i++) {
        char name[0x40];
        sprintf(name, "save_data%02d", i);
        D2APoint_unk pos = fn_801F4E8C(layout, fn_800DC6B4(name));
        pos.mScaleX = 1.0f;
        pos.mScaleY = 1.0f;
        fn_801F4728(m74[i], "basePoint", &pos);
        fn_801F6E78(mF4[i], "basePoint", &pos);
        D2ATask_unk* frame = m74[i];
        float cur = frame->m100;
        float end = frame->mA0;
        if (m13C == 0) {
            frame->m100 = MIN(15.0f, cur + 1.0f);
        } else if ((m128 - 1) % 8 == i) {
            frame->m100 = MAX(15.0f, MIN(end, cur + 1.0f));
        } else if (m128 == 0 && i == 0) {
            frame->m100 = MAX(15.0f, MIN(end, cur + 1.0f));
            SetPaneValue_unk(m74[i]->m58, "basePoint", 0, 270.0f);
            SetPaneValue_unk(m74[i]->m58, "basePoint", 1, 160.0f);
            SetPaneValue_unk(m74[i]->m58, "basePoint", 2, 1.5f);
            SetPaneValue_unk(m74[i]->m58, "basePoint", 3, 1.5f);
            fn_801F6D7C(mF4[i], "basePoint", 0, 270.0f);
            fn_801F6D7C(mF4[i], "basePoint", 1, 160.0f);
            fn_801F6D7C(mF4[i], "basePoint", 2, 1.5f);
            fn_801F6D7C(mF4[i], "basePoint", 3, 1.5f);
        } else {
            frame->m100 = MAX(15.0f, cur - 1.0f);
        }
        mF4[i]->mFlags |= 4;
        if (mD4[i] != 0 && m74[i]->m100 >= 15.0f) {
            mF4[i]->mFlags &= ~4;
        }
    }
}

void CampSaveLoad::fn_8011B3D8(int wrap) {
    if (IsRepeated_unk(0) || IsRepeated_unk(0x1A)) {
        if (--m154 < 0) {
            if (wrap) {
                m154 = m158 - 1;
            } else {
                m154 = 0;
            }
        }
        fn_80117228(3);
    } else if (IsRepeated_unk(1) || IsRepeated_unk(0x19)) {
        m154++;
        if (m154 >= m158) {
            if (wrap) {
                m154 = 0;
            } else {
                m154 = m158 - 1;
            }
        }
        fn_80117228(3);
    }
}

// NONMATCHING: 94.6% - the original keeps L"" (offset 0x90 of @wstringBase0) in a saved register and
// the wide string pool of the unit is not reproduced yet (the asm functions own most of it); with the
// empty ~CsvBase() the csv address is now in a saved register as in the original
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

int CampSaveLoad::fn_8011BBD0(int slot) {
    if (m4B4 != 0) {
        return 0;
    }
    int result = 0;
    SaveSlot_unk* s = &GetData_unk()->mSlots[slot];
    SaveSlot_unk* cur = &GetData_unk()->mSlots[0];
    SaveDate_unk* date = &s->m08;
    if (fn_803BEB54(cur)) {
        result = 0;
    } else if (slot == 0) {
        result = 0;
    } else if (date->m14 < cur->m08.m14) {
        result = 1;
    } else if (cur->m08.m14 == date->m14) {
        if (date->mSec + (date->mMin + (date->mHour + (date->mDay + 1) * 24) * 60) * 60 <=
            cur->m08.mSec + (cur->m08.mMin + (cur->m08.mHour + (cur->m08.mDay + 1) * 24) * 60) * 60) {
            result = 1;
        }
    }
    return result;
}
