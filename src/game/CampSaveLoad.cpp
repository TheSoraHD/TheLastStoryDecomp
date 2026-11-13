#include "game/CampSaveLoad.h"
#include "game/GameTask.h"
#include "macros.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "MP/SaveLoad.h"
#include "game/TutorialTask.h"
#include "game/GameLoadTask.h"
#include "db/MapIdTable_unk.h"
#include "game/GameSaveBackground.h"
#include "game/GameLoadBackground.h"
#include "atn/GraphicsDevice.h"
#include "atn/DrawHelper.h"

extern "C" unsigned long strlen(const char* s);
extern "C" void* memcpy(void* dst, const void* src, unsigned long n);

// inferred: creates a d2anime task playing `path` (returns the task)
// inferred: creates a d2anime task (other variant of fn_801F3FF8, no extra argument)
extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path);


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


// inferred: plays a system sound effect
extern "C" atn::SoundRef fn_80117228(int id);
// inferred: creates a message window task

extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
// inferred: 2D point of a d2anime pane (names invented)
// inferred: position of the pane `hash` of `layout`
extern "C" void fn_801F6E78(atn::Task* task, const char* name, const atn::D2A_DrawRect* pos);
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
    MenuLoadTask(atn::Task* parent, GameLoadTask* data);
    static MenuLoadTask* fn_80117914(atn::Task* parent, GameLoadTask* data, GameLoadBackground* bg); // inferred: bg unused
    virtual ~MenuLoadTask();

    static MenuLoadTask* sInstance; // 0x8087F06C
};

MenuSaveTask* MenuSaveTask::sInstance;
MenuLoadTask* MenuLoadTask::sInstance;

MenuSaveTask* MenuSaveTask::fn_8011770C(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) MenuSaveTask(parent);
    }
    return sInstance;
}

// inferred: not inline (its literals are in the pool); inlined into fn_8011770C
MenuSaveTask::MenuSaveTask(atn::Task* parent) : CampSaveLoad(parent) {
    mMode = 0;
    (m70 = fn_801F3FF8(this, "/data/d2anime/save_select.d2a", 0))->fn_800D246C(1);
    m4C = GameSaveTask::fn_803B7944(this, lbl_8087F430 != 0 ? (GameSaveBackground*)lbl_8087F430->m5634 : 0);
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

MenuLoadTask* MenuLoadTask::fn_80117914(atn::Task* parent, GameLoadTask* data, GameLoadBackground* bg) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) MenuLoadTask(parent, data);
    }
    return sInstance;
}

// inferred: not inline (its literals are in the pool); inlined into fn_80117914
MenuLoadTask::MenuLoadTask(atn::Task* parent, GameLoadTask* data) : CampSaveLoad(parent) {
    mMode = 1;
    (m70 = fn_801F3FF8(this, "/data/d2anime/load_select.d2a", 0))->fn_800D246C(1);
    m50 = data;
    if (data == 0) {
        // inferred: a 2-byte literal (0x80736B4D: 00 00), not merged with ""
        GameLoadTask::Option arg("\0", 1);
        m50 = GameLoadTask::fn_803B27F8(this, arg);
    } else if (data->mD60 == 0) {
        data->fn_803B8758(-1);
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
      m15C(0), m160(0), m164(0), m168(0), m16C(0), m170(0), m174(0), m4B0(0), m4B4(0) {
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
    (mArrows[0].mTask = fn_801F3FF8(this, "/data/d2anime/save_arrow_l.d2a", 0))->fn_800D246C(1);
    mArrows[0].mPressed = 0;
    (mArrows[1].mTask = fn_801F3FF8(this, "/data/d2anime/save_arrow_r.d2a", 0))->fn_800D246C(1);
    mArrows[1].mPressed = 0;
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

// inferred: map of a save slot (db/MapIdTable_unk)
extern "C" MapId_unk* fn_8021771C(int type, int map, int point);
extern "C" int strcmp(const char* a, const char* b);
// inferred: plays a d2anime (as in game/MenuCommonTask.cpp)
extern "C" void fn_804A3AF0(atn::Task* anime, int arg1, int repeat, float frame, float speed);
extern "C" void fn_804A3B60(atn::Task* layout, int arg1, int repeat, float frame, float speed);
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value); // inferred: sets a value of a pane
// inferred: sets the value of the pane `name` of the layout of `anime` (name invented)
inline void SetPane_unk(atn::D2A_Task* anime, const char* name, float value) {
    fn_801FECE0(&anime->mData.m10, fn_800DC6B4(name), value);
}
extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0; // inferred: graphics device (screen size)

// NONMATCHING: 92.83% - one more saved register (the string pool of the unit is not complete yet: the original
// keeps "" at @stringBase0 + 0x3D off the same base register)
int CampSaveLoad::vf0C() {
    if (m54 == 0) {
        m54 = 1;
    } else if (m54 == 1) {
        if (m50 == 0 || (m50->mFlags & 2) != 2 || m50->fn_803B87A8() == 0) {
            for (int i = 0; i < 17; i++) {
                GameSaveSlot_unk* slot = (GameSaveSlot_unk*)&GetData_unk()->mSlots[i];
                if (slot->fn_803BEAB4() != 0) {
                    MapId_unk* map = fn_8021771C(slot->m30, slot->m32, slot->m31);
                    if (map != 0 && strcmp(map->m70, "") != 0) {
                        std::string path = "data/texture/";
                        path += map->m70;
                        path += ".texture";
                        mTextures[i].fn_800D5908(path.c_str());
                    }
                }
            }
            m54 = 2;
        }
    } else if (fn_800D3FA4() != 0 && m4B8.fn_80473F50() == 0) {
        int length = (unsigned int)m4B8.fn_8047059C() >> 1;
        fn_8011B8D4((const wchar_t*)m4B8.fn_80470580(), length);
        fn_804A3AF0(m70, 1, 0, 0.0f, 1.0f);
        SetPane_unk(m70, "arrowR", 0.0f);
        SetPane_unk(m70, "arrowL", 0.0f);
        SetPane_unk(m70, "frameOn", 0.0f);
        for (int i = 0; i < 2; i++) {
            fn_804A3AF0(mArrows[i].mTask, 1, 0, 0.0f, 1.0f);
        }
        fn_804A3AF0(m58, 1, 1, 0.0f, 1.0f);
        fn_804A3AF0(m5C, 1, 0, 0.0f, -2.0f);
        fn_804A3AF0(m60, 1, 0, 0.0f, -2.0f);
        fn_804A3AF0(m64, 1, 0, 0.0f, -2.0f);
        fn_804A3AF0(m68, 1, 0, 0.0f, -2.0f);
        SetPane_unk(m64, "frameOn", 0.0f);
        SetPane_unk(m68, "frameOn", 0.0f);
        fn_804A3AF0(m6C, 1, 1, 0.0f, 1.0f);
        for (int i = 0; i < 8; i++) {
            fn_804A3AF0(m94[i], 1, 0, 0.0f, 0.0f);
            fn_804A3AF0(mB4[i], 1, 0, 0.0f, 0.0f);
            fn_804A3B60(mF4[i], 1, 0, 0.0f, 0.0f);
            m94[i]->mFlags |= 4;
            mB4[i]->mFlags |= 4;
            mF4[i]->mFlags |= 4;
        }
        fn_804A3AF0(m114, 1, 0, 0.0f, 0.0f);
        fn_80119030(0, 0);
        m138 = 1;
        m4C8 = 0.0f;
        if (m128 < 0) {
            m134 = 0;
            m12C = 0;
            m130 = 0;
        } else {
            m134 = m128 / 8;
            int pos = m128 % 8;
            m12C = pos % 4;
            m130 = pos / 4;
        }
        for (int i = 1; i < 9; i++) {
            int slot = i + m134 * 8;
            fn_8011B018(slot, &GetData_unk()->mSlots[slot]);
        }
        m4AC.fn_80470528();
        return 1;
    }
    if (lbl_8087F430 != 0 && ((GameTask*)lbl_8087F430)->fn_80370174(0x391) > 0) {
        lbl_8087EEB0->fn_80060D58(0.0f, 0.0f, 100.0f, (float)lbl_8087EEE0->mWidth, (float)lbl_8087EEE0->mHeight,
                                  0xFF000000);
    }
    return 0;
}

extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
// inferred: sets the text of the pane `name` of the layout of `anime` (name invented)
inline void SetPaneText_unk(atn::D2A_Task* anime, const char* name, const wchar_t* text) {
    fn_801FEE08(&anime->mData.m10, fn_800DC6B4(name), text);
}
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task

// inferred: holds an arrow on its last frames while it is pressed (name invented)
static inline void UpdateArrow_unk(CampArrow_unk& arrow) {
    atn::D2A_Task* task = arrow.mTask;
    if (task->mFrame >= 15.0f) {
        if (arrow.mPressed != 0) {
            task->mSpeed = 1.0f;
            if (task->IsEnd_unk()) {
                arrow.mPressed = 0;
            }
        } else {
            task->mFrame = 15.0f;
            task->mSpeed = 0.0f;
        }
    }
}

void CampSaveLoad::vf10() {
    int prev = m128;
    m128 = m134 * 8 + m130 * 4 + m12C + 1;
    if (m134 == 2) {
        m128 = 0;
    }
    if (prev != m128) {
        m4C8 = 0.0f;
    }
    if (lbl_8087EF70 == 0) {
        return;
    }
    float warning = 0.0f;
    if (m164 > 0) {
        if (m164 != 45) {
            warning = 1.0f;
        }
        m164--;
    }
    if (m70 != 0) {
        SetPane_unk(m70, "dispWarning", warning);
    }
    for (int i = 0; i < 2; i++) {
        UpdateArrow_unk(mArrows[i]);
    }
    switch (m13C) {
    case 0:
        if (m70->mFrame >= 15.0f) {
            fn_80119030(1, 0);
        }
        break;
    case 1:
        fn_801191A8();
        break;
    case 3:
        fn_8011979C();
        break;
    case 4:
        fn_8011968C();
        break;
    case 5:
        fn_8011999C();
        break;
    case 6:
        if (IsTriggered_unk(4) || IsTriggered_unk(5)) {
            fn_80119030(7, 0);
        }
        break;
    case 9:
        if (IsTriggered_unk(4) || IsTriggered_unk(5)) {
            fn_80117228(1);
            fn_80119030(1, 0);
        }
        break;
    }
    if (m70 != 0) {
        if (mMode == 0) {
            SetPaneText_unk(m70, "titleName", L"SAVE DATA");
        } else {
            SetPaneText_unk(m70, "titleName", L"LOAD DATA");
        }
    }
    fn_8011AD38();
    if (m178 != 0 && m178->IsFlag_unk(2)) {
        m178->fn_800D246C(0);
        if (m178->mState == 4) {
            fn_800D2338(m178);
            m178 = 0;
        }
    }
}

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
                (m178 = TutorialPopup::fn_805BC6C8(this, 0x1B5, 0))->fn_800D246C(1);
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
        if (mMode == 0 && lbl_8087F430 != 0 && m4B0 == 0) {
            ((GameTask*)lbl_8087F430)->fn_8036E964();
        }
        break;
    case 6:
    case 8:
        m150 = 1;
        break;
    }
}

void CampSaveLoad::vf1C() {}

void CampSaveLoad::vf20() {}

extern "C" const wchar_t* fn_80116FC0(int a, int id); // inferred: system text
extern "C" int fn_800A58D0(void* pad, int port);      // inferred: nonzero with the classic controller
// inferred: the "delete" button of the current pad (name invented)
inline int IsDeleteTriggered_unk() {
    if (lbl_8087EF70 == 0) {
        return 0;
    }
    return fn_800A58D0(lbl_8087EF70, 0) ? IsTriggered_unk(0xE) : IsTriggered_unk(0xF);
}

// inferred: text of the warning of the camp info panel (name invented)
static inline void SetWarningText_unk(CampSaveLoad* menu, const wchar_t* text) {
    SetPaneText_unk(menu->m60, "warning_msg", text);
}

// NONMATCHING: 98.58% - register of the m60 layout in the warning text, the loop index
// and frame pointer of the last loop in swapped registers, and the string pool
// inferred: state 1: choice of the slot
void CampSaveLoad::fn_801191A8() {
    if (IsTriggered_unk(4)) {
        if (mMode == 1) {
            if (((GameSaveSlot_unk*)&((CampSaveData_unk*)m50)->mSlots[m128])->fn_803BEAB4() == 0) {
                fn_80117228(11);
            } else {
                m16C = 0;
                fn_80117228(1);
                fn_80119030(3, 0);
            }
        } else {
            m16C = 0;
            fn_80117228(1);
            fn_80119030(3, 0);
        }
    } else if (IsTriggered_unk(5)) {
        if (lbl_8087F430 != 0) {
            if (((GameTask*)lbl_8087F430)->fn_80370174(0x391) > 0) {
                m16C = 2;
                fn_80117228(2);
                fn_80119030(3, 0);
            } else {
                fn_80117228(0x20);
                fn_80119030(7, 0);
            }
        } else {
            fn_80117228(0x20);
            fn_80119030(7, 0);
        }
    } else if (IsDeleteTriggered_unk()) {
        if (((GameSaveSlot_unk*)GetSlot_unk(m128))->fn_803BEAB4() == 0) {
            fn_80117228(11);
        } else if (m128 == 0) {
            fn_80119030(4, 1);
            fn_80117228(11);
            if (m60 != 0) {
                SetWarningText_unk(this, fn_80116FC0(1, 0x11A));
            }
            m170 = 1;
        } else {
            m16C = 1;
            fn_80117228(1);
            fn_80119030(3, 0);
        }
    } else {
        int page = m134;
        fn_8011B570();
        if (page != m134) {
            int prev = m128;
            m128 = m134 * 8 + m130 * 4 + m12C + 1;
            if (m134 == 2) {
                m128 = 0;
            }
            if (prev != m128) {
                m4C8 = 0.0f;
            }
            int d = m134 - page;
            if (__abs(d) > 1) {
                d = -d;
            }
            if (d > 0) {
                mArrows[1].mTask->mFrame = 15.0f;
                mArrows[1].mPressed = 1;
            } else {
                mArrows[0].mTask->mFrame = 15.0f;
                mArrows[0].mPressed = 1;
            }
            if (m134 == 2) {
                fn_8011B018(0, &GetData_unk()->mSlots[0]);
                GameSaveSlot_unk empty;
                for (int i = 2; i < 9; i++) {
                    fn_8011B018(i + m134 * 8, (SaveSlot_unk*)&empty);
                }
            } else {
                for (int i = 1; i < 9; i++) {
                    int slot = i + m134 * 8;
                    fn_8011B018(slot, &GetData_unk()->mSlots[slot]);
                    m74[i - 1]->mFrame = 15.0f;
                }
            }
        }
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
                    m4C->fn_803B7FC8(m128);
                } else {
                    m50->fn_803B3270(m128);
                }
                fn_80119030(5, 0);
                fn_80117228(1);
            } else if (m16C == 2) {
                fn_80119030(7, 0);
                fn_80117228(1);
            } else {
                if (mMode == 0) {
                    m4C->fn_803B807C(m128);
                } else {
                    m50->fn_803B3310(m128);
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
extern "C" MP::SaveData_unk* lbl_8087F460; // inferred: current save data (game/GameSaveData_unk)
extern "C" int fn_803BE854(MP::SaveData_unk* data);

// NONMATCHING: 92.34% - registers of the warning texts (the original takes the layout of m60 before hashing the
// pane name) and the string pool
// inferred: state 5: waits for the save, load or delete (m16C) of the slot m128
void CampSaveLoad::fn_8011999C() {
    m14C++;
    if (m16C == 0) {
        if (mMode == 0) {
            GameSaveLoadBackgroundCommon* bg = (GameSaveLoadBackgroundCommon*)m4C->mD70;
            if (bg->m154 != 0) {
                return;
            }
            if (bg->fn_803B8144() == 0) {
                fn_8011B018(m128, &((CampSaveData_unk*)m4C)->mSlots[m128]);
                fn_80119030(6, 0);
                fn_80117228(12);
            } else {
                fn_80119030(4, 1);
                fn_80117228(11);
                if (m60 != 0) {
                    SetWarningText_unk(this, fn_80116FC0(1, 0x11C));
                }
                m170 = 1;
            }
        } else {
            GameSaveLoadBackgroundCommon* bg = (GameSaveLoadBackgroundCommon*)m50->m14F60;
            if (bg->m154 != 0) {
                return;
            }
            if (bg->fn_803B8144() != 0) {
                fn_80119030(4, 1);
                fn_80117228(11);
                if (m60 != 0) {
                    SetWarningText_unk(this, fn_80116FC0(1, 0x11E));
                }
                m170 = 1;
            } else {
                m50->fn_803B2FBC(m128);
                if (fn_803BEB54(GetSlot_unk(m128)) != 0 || fn_803BE854(lbl_8087F460) != 0) {
                    fn_80119030(4, 1);
                    fn_80117228(11);
                    if (m64 != 0) {
                        SetPane_unk(m64, "ynOn", 0.0f);
                        SetPane_unk(m64, "checkSubType", 0.0f);
                        SetPaneText_unk(m64, "check_msg", fn_80116FC0(1, 0x11B));
                    }
                    if (lbl_8087F460 != 0) {
                        delete lbl_8087F460;
                        lbl_8087F460 = 0;
                    }
                    m170 = 2;
                } else {
                    fn_80119030(8, 0);
                    fn_80117228(12);
                }
            }
        }
    } else if (mMode == 0) {
        GameSaveLoadBackgroundCommon* bg = (GameSaveLoadBackgroundCommon*)m4C->mD70;
        if (bg->m154 != 0) {
            return;
        }
        if (bg->fn_803B8144() == 0) {
            fn_8011B018(m128, &((CampSaveData_unk*)m4C)->mSlots[m128]);
            fn_80119030(9, 0);
            fn_80117228(12);
        } else {
            fn_80119030(4, 1);
            fn_80117228(11);
            if (m60 != 0) {
                SetWarningText_unk(this, fn_80116FC0(1, 0x120));
            }
            m170 = 1;
        }
    } else {
        GameSaveLoadBackgroundCommon* bg = (GameSaveLoadBackgroundCommon*)m50->m14F60;
        if (bg->m154 != 0) {
            return;
        }
        if (bg->fn_803B8144() == 0) {
            fn_8011B018(m128, &((CampSaveData_unk*)m50)->mSlots[m128]);
            fn_80119030(9, 0);
            fn_80117228(12);
        } else {
            fn_80119030(4, 1);
            fn_80117228(11);
            if (m60 != 0) {
                SetWarningText_unk(this, fn_80116FC0(1, 0x120));
            }
            m170 = 1;
        }
    }
}

MP::SaveElemB_unk::~SaveElemB_unk() {}

// NONMATCHING: 88.8% - the original converts each pad result to a bool value (neg/or/srwi)
// before testing it
// NONMATCHING: 98.7% - floating-point register allocation of the two frame values (and the
// string pool offsets until all the functions of the file are written)
void CampSaveLoad::fn_8011AD38() {
    atn::D2A_Task* layout = m70;
    for (int i = 0; i < 8; i++) {
        char name[0x40];
        sprintf(name, "save_data%02d", i);
        atn::D2A_DrawRect pos = fn_801F4E8C(layout, fn_800DC6B4(name));
        pos.w = 1.0f;
        pos.h = 1.0f;
        fn_801F4728(m74[i], "basePoint", &pos.x);
        fn_801F6E78(mF4[i], "basePoint", &pos);
        atn::D2A_Task* frame = m74[i];
        float cur = frame->mFrame;
        float end = frame->mData.mEndFrame;
        if (m13C == 0) {
            frame->mFrame = MIN(15.0f, cur + 1.0f);
        } else if ((m128 - 1) % 8 == i) {
            frame->mFrame = MAX(15.0f, MIN(end, cur + 1.0f));
        } else if (m128 == 0 && i == 0) {
            frame->mFrame = MAX(15.0f, MIN(end, cur + 1.0f));
            SetPaneValue_unk(&m74[i]->mData.m10, "basePoint", 0, 270.0f);
            SetPaneValue_unk(&m74[i]->mData.m10, "basePoint", 1, 160.0f);
            SetPaneValue_unk(&m74[i]->mData.m10, "basePoint", 2, 1.5f);
            SetPaneValue_unk(&m74[i]->mData.m10, "basePoint", 3, 1.5f);
            fn_801F6D7C(mF4[i], "basePoint", 0, 270.0f);
            fn_801F6D7C(mF4[i], "basePoint", 1, 160.0f);
            fn_801F6D7C(mF4[i], "basePoint", 2, 1.5f);
            fn_801F6D7C(mF4[i], "basePoint", 3, 1.5f);
        } else {
            frame->mFrame = MAX(15.0f, cur - 1.0f);
        }
        mF4[i]->mFlags |= 4;
        if (mD4[i] != 0 && m74[i]->mFrame >= 15.0f) {
            mF4[i]->mFlags &= ~4;
        }
    }
}

extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: swprintf

// NONMATCHING: 99.33% - string pool offsets and the size/pointer registers of the two appends swapped (r23/r24)
// inferred: shows the save slot `index` in its frame (index - 1) % 8: icon, number, play time and map thumbnail,
// or the "no data" panel
void CampSaveLoad::fn_8011B018(int index, SaveSlot_unk* slot) {
    int i = index == 0 ? 0 : (index - 1) % 8;
    GameSaveSlot_unk* save = (GameSaveSlot_unk*)slot;
    if (save->fn_803BEAB4() != 0) {
        atn::D2A_Task* icon = index == 0 ? m114 : m94[i];
        icon->SetNumber_unk("indexStr", index, -2);
        wchar_t text[0x100];
        int sec = save->m70 / 30;
        int min = sec / 60;
        fn_800DD3FC(text, L"%d:%02d:%02d", min / 60, min % 60, sec % 60);
        SetPaneText_unk(icon, "dayStr", text);
        MapId_unk* map = fn_8021771C(save->m30, save->m32, save->m31);
        if (map != 0 && strcmp(map->m70, "") != 0) {
            std::string path = "data/texture/";
            path += map->m70;
            path += ".texture";
            icon->fn_801F4D80("thumbnail", (int)path.c_str());
        }
        m74[i] = icon;
        icon->MeltTask();
        mB4[i]->FreezeTask();
        mD4[i] = save->m78 != 0;
        if (i == 0) {
            if (index == 0) {
                m94[i]->FreezeTask();
            } else {
                m114->FreezeTask();
            }
        }
    } else {
        mB4[i]->SetNumber_unk("indexStr", index, -2);
        m74[i] = mB4[i];
        m94[i]->FreezeTask();
        mB4[i]->MeltTask();
        mD4[i] = 0;
        if (i == 0) {
            m114->FreezeTask();
        }
    }
    if (index >= 17) {
        m74[i]->FreezeTask();
    } else {
        m74[i]->MeltTask();
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

// inferred: the load menu shows a third page of a single row (name invented)
static inline bool IsSinglePage_unk(CampSaveLoad* menu) {
    return menu->mMode == 1 && menu->m134 == 2;
}

// NONMATCHING: 80.52% - the pad tests are materialized as bools (neg/or/srwi.) in the original, as in
// fn_8011B3D8
// inferred: moves the cursor on the slot grid: two columns (m130), four rows (m12C) per page (m134); the load
// menu has a third page with one row
void CampSaveLoad::fn_8011B570() {
    if (IsRepeated_unk(0) || IsRepeated_unk(0x1A)) {
        if (--m130 < 0) {
            m130 += 2;
        }
        fn_80117228(3);
    } else if (IsRepeated_unk(1) || IsRepeated_unk(0x19)) {
        if (++m130 >= 2) {
            m130 -= 2;
        }
        fn_80117228(3);
    } else if (IsRepeated_unk(3) || IsRepeated_unk(0x18)) {
        m12C++;
        if ((IsSinglePage_unk(this) ? 1 : 4) <= m12C) {
            m12C = 0;
            m134 = (m134 + 1) % ((mMode == 1) + 2);
        }
        fn_80117228(3);
    } else if (IsRepeated_unk(2) || IsRepeated_unk(0x17)) {
        if (--m12C < 0) {
            m134 = m134 > 0 ? m134 - 1 : (mMode == 1) + 1;
            m12C = IsSinglePage_unk(this) ? 0 : 3;
        }
        fn_80117228(3);
    }
    if (IsSinglePage_unk(this)) {
        m12C = 0;
        m130 = 0;
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
    csv.SetData(text, length);
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
