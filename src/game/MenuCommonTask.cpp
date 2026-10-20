#include "game/MenuCommonTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "math/Vec.h"
#include "macros.h"

extern "C" MenuD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: creates a D2A anime
namespace atn {
// inferred: the d2anime task, declared here with only the member used (atn/D2A_Task.h declares other
// functions with the types of atn/ files, which conflict with the menu's own views of the animes)
class D2A_Task : public Task {
public:
    void SetParam2_unk(const char* name, const float* v);
};
} // namespace atn
extern "C" MenuCursor_unk* fn_80202A6C(atn::Task* parent, const char* path, int arg);
extern "C" MenuLayout_unk* fn_801F64D0(atn::Task* parent, const char* path);
extern "C" void fn_805115D0(MenuD2A_unk* anime, int arg1, int arg2, float a, float b);
extern "C" void fn_805114D4(atn::Task* anime, int arg1, int arg2, float a, float b);
extern "C" void fn_80087BB4(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step); // inferred: two floats
extern "C" void* lbl_8087EF70; // inferred: pad
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" int fn_800A55AC(void* pad, int port, int button); // inferred: button repeated
extern "C" atn::SoundRef fn_80117228(int id);
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" unsigned int fn_800DC6B4(const char* s); // inferred: string hash
// inferred: 2D point of a d2anime pane (names invented, as in CampSaveLoad.cpp)
struct D2APoint_unk {
    D2APoint_unk() {}
    float x;
    float y;
    float mScaleX;
    float mScaleY;
    float m10;
    float m14;
};
extern "C" D2APoint_unk fn_801F4E8C(atn::Task* layout, unsigned int hash);
extern "C" void fn_801FED24(void* layout, unsigned int hash, int index, float value);
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
// inferred: sets the value `index` of the pane `name` (name invented)
inline void SetPaneValue_unk(void* layout, const char* name, int index, float value) {
    fn_801FED24(layout, fn_800DC6B4(name), index, value);
}
inline D2APoint_unk GetPanePos_unk(atn::Task* layout, const char* name) {
    return fn_801F4E8C(layout, fn_800DC6B4(name));
}
inline void SetPaneValue2_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
extern "C" void* lbl_8087EEE0;
extern "C" float fn_80076FF8(void* obj); // inferred: screen aspect ratio
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

MenuCommonTask* MenuCommonTask::sInstance;

MenuCommonTask* MenuCommonTask::fn_804A29C4(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (1, "", "", 0) MenuCommonTask(parent);
    }
    return sInstance;
}

// NONMATCHING: 99.6% - registers of the mA0 array construction loop differ (zero/pointer/end in r29/r30/r28 in the target);
// tried element ctor init list/body, nested text struct, counted list struct
MenuCommonTask::MenuCommonTask(atn::Task* parent)
    : atn::Task(parent), m9C(0), m1EC(0.0f), m1F0(-15.0f), m1F4(0.9f), m1F8(0.9f), m1FC(-7.0f) {
    m4C = 0;
    m50 = fn_801F3FF8(this, "/data/d2anime/camp_info.d2a", 0);
    m50->fn_800D246C(1);
    m54 = 0;
    m58 = fn_801F3FF8(this, "/data/d2anime/camp_info6.d2a", 0);
    m58->fn_800D246C(1);
    m5C = 0;
    m60 = fn_801F3FF8(this, "/data/d2anime/camp_info.d2a", 0);
    m60->fn_800D246C(1);
    m64 = 0;
    m6C = fn_801F3FF8(this, "/data/d2anime/camp_info2.d2a", 1);
    m6C->fn_800D246C(1);
    m70 = fn_801F3FF8(this, "/data/d2anime/camp_cursor.d2a", 1);
    m70->fn_800D246C(1);
    m68 = 0;
    m74 = -1;
    for (int i = 0; i < 4; i++) {
        m78[i] = fn_80202A6C(this, "/data/d2anime/camp_cursor.d2a", 1);
        m78[i]->fn_800D246C(1);
    }
    int i;
    const char* cursor = "/data/d2anime/camp_cursor.d2a";
    for (i = 0; i < 4; i++) {
        MenuLayout_unk* task = fn_801F64D0(this, cursor);
        m8C[i] = task;
        task->fn_800D246C(1);
    }
    m88 = -1;
    m1C0 = -1;
    m1C4 = 0;
    m1C8 = fn_801F3FF8(this, "/data/d2anime/camp_info_bar.d2a", 1);
    m1C8->fn_800D246C(1);
    m9C = 0;
    m1CC = 0;
    for (int i = 0; i < 2; i++) {
        m1D0[i] = 0;
        m1D8[i] = fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction_opcl.d2a", 0);
        m1D8[i]->fn_800D246C(1);
        m1E0[i] = fn_801F3FF8(this, "/data/d2anime/ui_abtn_reaction_loop.d2a", 0);
        m1E0[i]->fn_800D246C(1);
    }
}

MenuCommonTask::~MenuCommonTask() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int MenuCommonTask::vf0C() {
    if (fn_800D3FA4() != 0) {
        fn_805115D0(m50, 1, 0, 0.0f, -3.0f);
        fn_805115D0(m58, 1, 0, 0.0f, -3.0f);
        fn_805115D0(m60, 1, 0, 0.0f, -3.0f);
        fn_805115D0(m6C, 0, 0, 0.0f, 0.0f);
        fn_805115D0(m70, 0, 1, 0.0f, 1.0f);
        for (int i = 0; i < 4; i++) {
            fn_805114D4(m78[i], 0, 1, 0.0f, 1.0f);
        }
        for (int i = 0; i < 4; i++) {
            m8C[i]->fn_800D246C(0);
            m8C[i]->mFlags |= 4;
        }
        fn_805115D0(m1C8, 1, 0, 0.0f, -1.0f);
        for (int i = 0; i < 2; i++) {
            fn_805115D0(m1D8[i], 1, 0, 0.0f, 1.0f);
            fn_805115D0(m1E0[i], 0, 1, 0.0f, 1.0f);
            m1D8[i]->mFlags |= 4;
            m1E0[i]->mFlags |= 4;
            m1D0[i] = 0;
        }
        void* group = m1E8.Load("MenuCommon");
        fn_80087BB4(group, "EquipFace Ofs", &m1EC, 0, 0, -1000.0f, 1000.0f, 1.0f);
        fn_80087BB4(group, "EquipFace Scl", &m1F4, 0, 0, -1000.0f, 1000.0f, 1.0f);
        fn_8008771C(group, "4:3 Desc Ofs", &m1FC, 0, 0, -1000.0f, 1000.0f, 1.0f);
        return 1;
    }
    return 0;
}

void MenuCommonTask::fn_804A2F98() {
    m4C = 0;
    m54 = 0;
    m64 = 0;
    m68 = 0;
    mFlags &= ~4;
}

inline void MenuCommonTask::ResetInfo_unk() {
    m4C = 0;
    m54 = 0;
    m50->mFrame = 0.0f;
    m50->mSpeed = -6.0f;
    m58->mFrame = 0.0f;
    m58->mSpeed = -6.0f;
    m5C = 0;
    m60->mFrame = 0.0f;
    m60->mSpeed = -6.0f;
    m64 = 0;
    m68 = 0;
    m6C->mFrame = 0.0f;
    m6C->mSpeed = -1.0f;
    m1C0 = -1;
    m1C4 = 0;
    m1C8->mFrame = 0.0f;
    m1C8->mSpeed = -1.0f;
    for (int i = 0; i < 2; i++) {
        m1D8[i]->mFrame = 0.0f;
        m1D8[i]->mFlags |= 4;
        m1E0[i]->mFrame = 0.0f;
        m1E0[i]->mFlags |= 4;
        m1D0[i] = 0;
    }
}

void MenuCommonTask::fn_804A2FBC() {
    for (int i = 0; i < 4; i++) {
        m78[i]->mActive = 0;
    }
    m74 = -1;
    for (int i = 0; i < 4; i++) {
        m8C[i]->mFlags |= 4;
    }
    m88 = -1;
    ResetInfo_unk();
    mFlags |= 4;
}

void MenuCommonTask::fn_804A313C() {
    ResetInfo_unk();
}

void MenuCommonTask::vf10() {
    m9C = 0;
    m48 = 0;
    void* pad = lbl_8087EF70;
    if (m4C != 0) {
        if (m50->IsEnd_unk()) {
            if (fn_800A555C(pad, 0, 4) != 0 || fn_800A555C(pad, 0, 5) != 0) {
                m48 = 1;
            }
        }
    } else if (m54 != 0) {
        if (m58->IsEnd_unk()) {
            if (fn_800A555C(pad, 0, 4) != 0 || fn_800A555C(pad, 0, 5) != 0) {
                m48 = 1;
            }
        }
    } else if (m64 != 0) {
        if (m6C->IsEnd_unk()) {
            if (fn_800A555C(pad, 0, 4) != 0) {
                m48 = 1;
            } else if (fn_800A555C(pad, 0, 5) != 0) {
                m68 = 1;
                m48 = 1;
            } else if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0 ||
                       fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
                m68 = m68 == 0;
                fn_80117228(3);
            }
        }
    }
}

void MenuCommonTask::vf14() {
    if (m48 != 0) {
        m64 = 0;
        m4C = 0;
        m54 = 0;
    }
    if (m4C != 0) {
        m50->mSpeed = 3.0f;
    } else {
        m50->mSpeed = -3.0f;
    }
    if (m54 != 0) {
        m58->mSpeed = 3.0f;
    } else {
        m58->mSpeed = -3.0f;
    }
    if (m5C != 0) {
        m60->mSpeed = 3.0f;
    } else {
        m60->mSpeed = -3.0f;
    }
    m5C = 0;
    m70->mFlags |= 4;
    if (m64 != 0) {
        m6C->mSpeed = 1.0f;
        if (m6C->IsEnd_unk() && m70 != 0) {
            m70->mFlags &= ~4;
            char name[0x40];
            sprintf(name, "cursor_point%d", m68 + 1);
            D2APoint_unk pos = GetPanePos_unk(m6C, name);
            SetPaneValue_unk(m70->m58, "cursor_pos", 0, pos.x);
            SetPaneValue_unk(m70->m58, "cursor_pos", 1, pos.y);
            SetPaneValue_unk(m70->m58, "cursor_pos", 4, pos.m10);
            SetPaneValue2_unk(m70->m58, "wRate", pos.mScaleX);
            SetPaneValue2_unk(m70->m58, "hRate", pos.mScaleY);
        }
    } else {
        m6C->mSpeed = -1.0f;
    }
    for (int i = 0; i < 4; i++) {
        m78[i]->mActive = i <= m74;
    }
    m74 = -1;
    for (int i = 0; i < 4; i++) {
        if (i <= m88) {
            m8C[i]->mFlags &= ~4;
        } else {
            m8C[i]->mFlags |= 4;
        }
    }
    m88 = -1;
    m1C8->mSpeed = -1.0f;
    if (m1C0 >= 0) {
        m1C8->mSpeed = 1.0f;
    }
    if (m1C4 != 0) {
        if (m1C8->mFrame >= 9.0f) {
            m1C8->mFrame = 9.0f;
        }
    } else {
        if (m1C8->mFrame >= 10.0f) {
            m1C8->mFrame = 10.0f;
        }
    }
    m1C0 = -1;
    m1C4 = 0;
    if (fn_80076FF8(lbl_8087EEE0) < 1.5f) {
        SetPaneValue2_unk(m1C8->m58, "ofs", m1FC);
    } else {
        SetPaneValue2_unk(m1C8->m58, "ofs", 0.0f);
    }
    for (int i = 0; i < 2; i++) {
        bool open = i < m1CC;
        m1D8[i]->mFlags |= 4;
        m1E0[i]->mFlags |= 4;
        if (open) {
            if (m1D8[i]->mFrame >= 8.0f) {
                m1D8[i]->mFrame = 8.0f;
                if (m1D0[i] != 0) {
                    m1D8[i]->mFlags &= ~4;
                } else {
                    m1E0[i]->mFlags &= ~4;
                }
            } else {
                m1D8[i]->mFlags &= ~4;
            }
        } else {
            if (m1D8[i]->IsEnd_unk() || m1D8[i]->mFrame <= 0.0f) {
                m1D8[i]->mFrame = 0.0f;
                m1E0[i]->mFrame = 0.0f;
            } else {
                m1D8[i]->mFlags &= ~4;
            }
        }
        m1D0[i] = 0;
    }
    m1CC = 0;
}


extern "C" MenuLayoutData_unk* fn_801F6C10(MenuLayout_unk* layout); // inferred: data of the layout anime
extern "C" void fn_804A3BD4(int a, int b, int c);

// inferred: plays an anime with the menu callback (weak copies of a header helper; names keep the address)
extern "C" void fn_804A39EC(MenuD2A_unk* anime, int arg1, int repeat, float frame, float speed) {
    if (anime != 0) {
        anime->fn_800D246C(0);
        anime->mRepeat = repeat;
        anime->mSpeed = speed;
        anime->mFrame = frame;
        anime->mCallback = fn_804A3BD4;
    }
}

extern "C" void fn_804A3A68(MenuLayout_unk* layout, int arg1, int repeat, float frame, float speed) {
    if (layout != 0) {
        layout->fn_800D246C(0);
        layout->mSpeed = speed;
        layout->mRepeat = repeat != 0;
        layout->mFrame = frame;
        MenuLayoutData_unk* data = fn_801F6C10(layout);
        data->mCallback = fn_804A3BD4;
    }
}

extern "C" void fn_804A3AF0(MenuD2A_unk* anime, int arg1, int repeat, float frame, float speed) {
    if (anime != 0) {
        anime->fn_800D246C(0);
        anime->mRepeat = repeat;
        anime->mSpeed = speed;
        anime->mFrame = frame;
    }
}

extern "C" void fn_804A3B60(MenuLayout_unk* layout, int arg1, int repeat, float frame, float speed) {
    if (layout != 0) {
        layout->fn_800D246C(0);
        layout->mSpeed = speed;
        layout->mRepeat = repeat != 0;
        layout->mFrame = frame;
    }
}

// inferred: d2anime manager (see atn/D2A_Task.cpp); the menu of the current anime
struct MenuD2AManager_unk {
    unsigned char m000[0x1A8];
    void* m1A8; // inferred: current d2anime task
    void* m1AC; // inferred: current layout task
};
extern "C" MenuD2AManager_unk* lbl_8087F138;

// inferred: event callback of the menu animes: forwards to the menu task (names keep the address)
extern "C" void fn_804A3BD4(int a, int b, int c) {
    if (lbl_8087F138 != 0 && MenuCommonTask::sInstance != 0) {
        if (lbl_8087F138->m1AC != 0) {
            MenuCommonTask::sInstance->fn_804A71F0(lbl_8087F138->m1AC, a, b, c);
        } else if (lbl_8087F138->m1A8 != 0) {
            MenuCommonTask::sInstance->fn_804A660C(lbl_8087F138->m1A8, a, b, c);
        }
    }
}

// NONMATCHING: fn_804A3C24 (0x640) is not written yet

// inferred: info bar helpers (names keep the address)
extern "C" float fn_804A4264(MenuCommonTask* menu) {
    float frame = menu->m1C8->mFrame;
    return -10.4f * (10.0f - frame);
}

extern "C" float fn_804A4280(MenuCommonTask* menu) {
    return menu->m1C8->mFrame / 10.0f;
}

extern "C" void fn_801FEE08(void* layout, unsigned int hash, const wchar_t* text);
inline void SetPaneText_unk(void* layout, const char* name, const wchar_t* text) {
    fn_801FEE08(layout, fn_800DC6B4(name), text);
}

// inferred: shows a warning in the info window (camp_info.d2a)
extern "C" void fn_804A4294(MenuCommonTask* menu, const wchar_t* text) {
    SetPaneText_unk(menu->m50->m58, "warning_msg", text);
    menu->m4C = 1;
}

// inferred: shows a warning in the info window (camp_info6.d2a)
extern "C" void fn_804A4300(MenuCommonTask* menu, const wchar_t* text) {
    SetPaneText_unk(menu->m58->m58, "warning_msg", text);
    menu->m54 = 1;
}

// inferred: moves a list cursor with up/down (wrapping or clamping) and plays `sound` when it moved
extern "C" void fn_804A436C(int* cursor, int count, int arg2, int wrap, int sound) {
    if (count > 0) {
        void* pad = lbl_8087EF70;
        int old = *cursor;
        if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
            if (--*cursor < 0) {
                if (wrap != 0) {
                    *cursor = count - 1;
                } else {
                    *cursor = 0;
                }
            }
        } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
            if (++*cursor >= count) {
                if (wrap != 0) {
                    *cursor = 0;
                } else {
                    *cursor = count - 1;
                }
            }
        }
        if (*cursor != old) {
            fn_80117228(sound);
        }
    }
}

// inferred: moves a scrolled list cursor (up/down and page up/down) and plays `sound` when it moved
extern "C" void fn_804A4494(int* cursor, int* top, int count, int page, int arg4, int sound) {
    if (count > 0) {
        int old = *cursor;
        int maxTop = count - page < 0 ? 0 : count - page;
        bool first = old == *top;
        bool last;
        if (page >= count) {
            last = *top + count - 1 == old;
        } else {
            last = *top + page - 1 == old;
        }
        void* pad = lbl_8087EF70;
        if (fn_800A55AC(pad, 0, 0) != 0 || fn_800A55AC(pad, 0, 0x1A) != 0) {
            (*cursor)--;
            if (first) {
                (*top)--;
            }
            if (*cursor < 0) {
                *cursor = count - 1;
                *top = maxTop;
            }
        } else if (fn_800A55AC(pad, 0, 1) != 0 || fn_800A55AC(pad, 0, 0x19) != 0) {
            (*cursor)++;
            if (last) {
                (*top)++;
            }
            if (*cursor >= count) {
                *cursor = 0;
                *top = 0;
            }
        } else if (fn_800A55AC(pad, 0, 2) != 0 || fn_800A55AC(pad, 0, 0x17) != 0) {
            if (*cursor == 0) {
                *cursor = count - 1;
                *top = maxTop;
            } else {
                *cursor -= page;
                if ((*top -= page) < 0) {
                    *top = 0;
                }
                if (*cursor < 0) {
                    *cursor = 0;
                    *top = 0;
                }
            }
        } else if (fn_800A55AC(pad, 0, 3) != 0 || fn_800A55AC(pad, 0, 0x18) != 0) {
            if (*cursor == count - 1) {
                *cursor = 0;
                *top = 0;
            } else {
                *cursor += page;
                *top += page;
                if (*top > maxTop) {
                    *top = maxTop;
                }
                if (*cursor >= count) {
                    *cursor = count - 1;
                    *top = maxTop;
                }
            }
        }
        if (*cursor != old) {
            fn_80117228(sound);
        }
    }
}

extern "C" void fn_801FEDBC(void* layout, unsigned int hash, int index, float value); // inferred: sets a value
extern "C" void* fn_801FEB9C(void* layout, const char* name); // inferred: track of a pane value
extern "C" float fn_801F0544(void* table, int key);            // inferred: value at key
extern "C" void fn_801F791C(MenuLayout_unk* layout, const char* name, int index, float value);
extern "C" void* fn_801F6C38(MenuLayout_unk* layout, const char* name);
extern "C" void fn_801F6C80(MenuLayout_unk* layout, const char* name, float value);

extern "C" void fn_804A473C(MenuD2A_unk* anime, int pos, int count, int page);
// inferred: sets the scroll bar of a d2anime list (wrapper; names keep the address)
extern "C" void fn_804A4738(MenuD2A_unk* anime, int pos, int count, int page) {
    fn_804A473C(anime, pos, count, page);
}

// inferred: scroll bar of a d2anime list: hidden when everything fits, else sized to the visible part
extern "C" void fn_804A473C(MenuD2A_unk* anime, int pos, int count, int page) {
    if (anime != 0) {
        fn_801FEDBC(anime->m58, fn_800DC6B4("col0001"), 0, 0.0f);
        if (count > page) {
            fn_801FEDBC(anime->m58, fn_800DC6B4("col0001"), 0, 255.0f);
            void* track = fn_801FEB9C(anime->m58, "s_hightMax");
            if (track != 0) {
                float max = fn_801F0544((char*)track + 4, 0);
                float unit = max / count;
                float ratio = MIN(max, (float)count / page);
                float bar = MIN(max, max / ratio);
                float height = bar;
                if (bar < 12.0f) {
                    height = 12.0f;
                    unit = (max - (12.0f - bar)) / count;
                }
                SetPaneValue2_unk(anime->m58, "hight", height);
                SetPaneValue2_unk(anime->m58, "hightUnit", unit);
                SetPaneValue2_unk(anime->m58, "index", pos);
            }
        }
    }
}

extern "C" void fn_804A4934(MenuLayout_unk* layout, int pos, int count, int page);
// inferred: sets the scroll bar of a layout list (wrapper; names keep the address)
extern "C" void fn_804A4930(MenuLayout_unk* layout, int pos, int count, int page) {
    fn_804A4934(layout, pos, count, page);
}

// inferred: same as fn_804A473C for a layout anime
extern "C" void fn_804A4934(MenuLayout_unk* layout, int pos, int count, int page) {
    if (layout != 0) {
        fn_801F791C(layout, "col0001", 0, 0.0f);
        if (count > page) {
            fn_801F791C(layout, "col0001", 0, 255.0f);
            void* track = fn_801F6C38(layout, "s_hightMax");
            if (track != 0) {
                float max = fn_801F0544((char*)track + 4, 0);
                float unit = max / count;
                float ratio = MIN(max, (float)count / page);
                float bar = MIN(max, max / ratio);
                float height = bar;
                if (bar < 12.0f) {
                    height = 12.0f;
                    unit = (max - (12.0f - bar)) / count;
                }
                fn_801F6C80(layout, "hight", height);
                fn_801F6C80(layout, "hightUnit", unit);
                fn_801F6C80(layout, "index", pos);
            }
        }
    }
}

// inferred: item data (only the icon is used here)
struct MenuItem_unk {
    unsigned char m00[0x08];
    const wchar_t* mName; // 0x08
    unsigned char m0C[0x2C - 0x0C];
    int mIcon; // 0x2C: inferred: face icon (1-based, 4 per row)
};
extern "C" MenuItem_unk* fn_802091E8(int id); // inferred: item data
extern "C" MenuItem_unk* fn_80211480(int id); // inferred: item data
extern "C" void fn_801F7590(MenuLayout_unk* layout, const char* name, const float* uv);

// inferred: texture rectangle (u0, v0, u1, v1); passed by value to the setters (names invented)
struct MenuUV_unk {
    MenuUV_unk() {}
    MenuUV_unk(float u0_, float v0_, float u1_, float v1_) : u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
    MenuUV_unk(const MenuUV_unk& o) {
        __Vec4Copy(&u0, &o.u0);
    }
    MenuUV_unk& operator=(const MenuUV_unk& o) {
        __Vec4Copy(&u0, &o.u0);
        return *this;
    }
    float u0, v0, u1, v1;
};
inline MenuUV_unk GetIconUV_unk(MenuItem_unk* item, int cols, int rows) {
    MenuUV_unk uv(0.0f, 0.0f, 0.0f, 0.0f);
    if (item != 0 && item->mIcon > 0 && item->mIcon < cols * rows) {
        int index = item->mIcon - 1;
        int x = index % 4;
        int y = index / 4;
        uv = MenuUV_unk(1.0f / cols * x, 1.0f / rows * y, 1.0f / cols * (x + 1), 1.0f / rows * (y + 1));
    }
    return uv;
}
inline void SetUV_unk(atn::D2A_Task* anime, const char* name, MenuUV_unk uv) {
    anime->SetParam2_unk(name, &uv.u0);
}
inline void SetUV_unk(MenuLayout_unk* layout, const char* name, MenuUV_unk uv) {
    fn_801F7590(layout, name, &uv.u0);
}

// inferred: sets the face icon of an item on a d2anime
extern "C" void fn_804A4AEC(MenuD2A_unk* menuAnime, MenuItem_unk* item, int cols, int rows) {
    atn::D2A_Task* anime = (atn::D2A_Task*)menuAnime;
    MenuUV_unk uv = GetIconUV_unk(item, cols, rows);
    SetUV_unk(anime, "faceUV", uv);
    SetUV_unk(anime, "faceMUV", uv);
    SetUV_unk(anime, "faceSubUV", uv);
    SetUV_unk(anime, "faceSubMUV", uv);
}

// inferred: sets the face icon of an item on a layout anime
extern "C" void fn_804A4CE4(MenuLayout_unk* layout, int id, int cols, int rows) {
    MenuUV_unk uv = GetIconUV_unk(fn_802091E8(id), cols, rows);
    SetUV_unk(layout, "faceUV", uv);
    SetUV_unk(layout, "faceMUV", uv);
    SetUV_unk(layout, "faceSubUV", uv);
    SetUV_unk(layout, "faceSubMUV", uv);
}

// NONMATCHING: fn_804A4EEC .. fn_804A5B9C are not written yet

// inferred: position of the info bar
extern "C" void fn_804A5CD8(MenuCommonTask* menu, float value) {
    SetPaneValue_unk(menu->m1C8->m58, "pos0015", 4, value);
}

// NONMATCHING: fn_804A5D34 .. fn_804A62C8 are not written yet

// inferred: shows a warning in the info window (third camp_info.d2a)
extern "C" void fn_804A65A0(MenuCommonTask* menu, const wchar_t* text) {
    SetPaneText_unk(menu->m60->m58, "warning_msg", text);
    menu->m5C = 1;
}

// NONMATCHING: fn_804A660C and fn_804A71F0 (events of the animes, 0xBE4 each) are not written yet

// inferred: message table (lbl_8087F1E4, see game/CharaTask.cpp)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct MenuMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" MenuMsgTable_unk* lbl_8087F1E4;
extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf
extern "C" void* lbl_8087EEB0; // inferred: debug drawing
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width
extern "C" void fn_80061824(void* draw, float x, float y, float z, float w, float h, float f6, float f7, float f8,
                            const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2); // inferred: text

// inferred: draws the name of an item at the top right
extern "C" void fn_804A7DD4(MenuCommonTask* menu, int id) {
    MenuItem_unk* item = fn_80211480(id);
    if (item != 0) {
        wchar_t text[0x80];
        float y = 120.0f;
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x36F), item->mName);
        float w = fn_8006EF48(lbl_8087EEC8, text, 14.0f, 0.0f, 1, 1);
        fn_80061824(lbl_8087EEB0, 590.0f - w, y, 0.0f, 14.0f, 14.0f, 0.0f, 0.0f, 0.0f, text, -1, 1, 1, 0, 0,
                    0xFF000000);
    }
}

// fn_804A7EA4 (0x804A7EA4) is the creator of MiniMapTask (it uses the MiniMapTask string pool): see notes_G.md
