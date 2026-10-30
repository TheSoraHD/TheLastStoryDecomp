#include "game/MenuCommonTask.h"
#include "atn/Memory.h"
#include "atn/D3AnimeInstance.h"
#include "atn/D2A_Message.h"
#include "std/vector.h"
#include "atn/SoundRef.h"
#include "math/Vec.h"
#include "macros.h"
#include "atn/DrawHelper.h"
#include "std/string_members.h"

extern "C" MenuD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: creates a D2A anime
namespace atn {
// inferred: the d2anime task, declared here with only the member used (atn/D2A_Task.h declares other
// functions with the types of atn/ files, which conflict with the menu's own views of the animes)
class D2A_Task : public Task {
public:
    void SetParam2_unk(const char* name, const float* v);

    D2A_Data mData; // 0x48
};
} // namespace atn
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
// inferred: render manager (see game/CharaTask.cpp); only the member used here
struct MenuRender_unk {
    unsigned char m00[0x44];
    int mWide; // 0x44: inferred: 16:9 screen
};
extern "C" MenuRender_unk* lbl_8087EEE0;
extern "C" float fn_80076FF8(void* obj); // inferred: screen aspect ratio
extern "C" void fn_8008771C(void* group, const char* name, float* value, int arg3, int arg4, float min, float max,
                            float step);

MenuCommonTask* MenuCommonTask::sInstance;

MenuCommonTask* MenuCommonTask::fn_804A29C4(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) MenuCommonTask(parent);
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
        m78[i] = (MenuCursor_unk*)atn::D3AnimeInstance::Create(this, "/data/d2anime/camp_cursor.d2a", 1);
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

namespace std {
// inferred: wstring elements are moved as raw bytes when push_back reallocates and the buffer
// destroys its elements before releasing them (fn_804A3C24); local to this file
template <>
struct __vec_split_unk<basic_string<wchar_t> > : public __vec_storage_unk<basic_string<wchar_t> > {
    typedef basic_string<wchar_t> T;
    __vec_split_unk(void* alloc) : mAlloc(alloc), mStart(0) {}
    ~__vec_split_unk() {
        T* b = begin();
        T* p = b + this->mSize;
        while (p > b) {
            (--p)->~T();
        }
        this->mSize = 0;
    }
    T* begin() { return this->mData + mStart; }
    void push_back(const T& x) {
        new (begin() + this->mSize) T(x);
        this->mSize++;
    }
    void __push_front_unk(T* first, T* last) {
        int n = last - first;
        mStart -= n;
        memcpy(this->mData + mStart, first, n * sizeof(T));
        memset(first, 0, n * sizeof(T));
        this->mSize += n;
    }
    void* mAlloc;
    unsigned int mStart;
};
} // namespace std

extern "C" void fn_800E0000(std::vector<std::basic_string<wchar_t> >* lines, const wchar_t* text, float width,
                            float size, int a, int b, int c, float d); // inferred: splits a text in lines
extern "C" void fn_804A5D34(MenuCommonTask* menu, int id, const wchar_t* desc0, const wchar_t* desc1,
                            const wchar_t* desc2);
extern "C" wchar_t lbl_8087E0E0[2]; // inferred: empty text

// inferred: shows a description (up to three lines) in the info bar
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int id) {
    if (text == 0) {
        return;
    }
    std::vector<std::basic_string<wchar_t> > lines;
    fn_800E0000(&lines, text, 100000.0f, 16.0f, 1, 1, 0, 0.0f);
    while (lines.size() < 3) {
        lines.push_back(std::basic_string<wchar_t>(lbl_8087E0E0));
    }
    fn_804A5D34(menu, 0, lines[0].data(), lines[1].data(), lines[2].data());
    menu->m1C4 = id;
}

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
void MenuCommonTask::fn_804A4294(const wchar_t* text) {
    SetPaneText_unk(m50->m58, "warning_msg", text);
    m4C = 1;
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
extern "C" void fn_801F6C80(atn::Task* layout, const char* name, float value);

extern "C" void fn_804A473C(MenuD2A_unk* anime, int pos, int count, int page);
// inferred: sets the scroll bar of a d2anime list (wrapper; names keep the address)
extern "C" void fn_804A4738(MenuD2A_unk* anime, int pos, int count, int page) {
    fn_804A473C(anime, pos, count, page);
}

// NONMATCHING: 99.0% - the layout address and the "s_hightMax" literal of the track lookup are computed in the
// other order
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
                    max -= height - bar;
                    unit = max / count;
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
                    max -= height - bar;
                    unit = max / count;
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

extern "C" void fn_801F6D7C(atn::Task* layout, const char* name, int index, float value); // inferred: sets a value
extern "C" float fn_801F6C2C(atn::Task* layout); // inferred: last frame

inline atn::D3AnimeInstance* GetCursor_unk(MenuCursor_unk* cursor) {
    return (atn::D3AnimeInstance*)cursor;
}

// inferred: places the next 3D cursor and adds an entry (id in m00)
void MenuCommonTask::fn_804A4EEC(atn::Task* id, const char* name, const atn::Matrix3x4& mtx, int end, float x, float y,
                                 float w, float h, float z) {
    int i = ++m74;
    if (0 <= i && i < 4 && m78[i] != 0) {
        GetCursor_unk(m78[i])->SetMatrix(mtx);
        GetCursor_unk(m78[i])->UpdateMtx();
        fn_801F6D7C(GetCursor_unk(m78[i])->GetTask(), "cursor_pos", 0, x);
        fn_801F6D7C(GetCursor_unk(m78[i])->GetTask(), "cursor_pos", 1, y);
        fn_801F6D7C(GetCursor_unk(m78[i])->GetTask(), "cursor_pos", 4, z);
        fn_801F6C80(GetCursor_unk(m78[i])->GetTask(), "wRate", w);
        fn_801F6C80(GetCursor_unk(m78[i])->GetTask(), "hRate", h);
        if (end) {
            float frame = fn_801F6C2C(GetCursor_unk(m78[i])->GetTask()) - 2.0f;
            ((MenuLayout_unk*)GetCursor_unk(m78[i])->GetTask())->mFrame = frame;
        }
    }
    if (id != 0 && (unsigned int)m9C < 4) {
        MenuCommonEntry_unk entry;
        entry.m00 = (int)id;
        if (name != 0) {
            strcpy(entry.m08.mText, name);
        }
        new (&mA0[m9C]) MenuCommonEntry_unk(entry);
        m9C++;
    }
}

// inferred: same as fn_804A4EEC with the id in m04
void MenuCommonTask::fn_804A5160(atn::Task* id, const char* name, const atn::Matrix3x4& mtx, int end, float x, float y,
                                 float w, float h, float z) {
    int i = ++m74;
    if (0 <= i && i < 4 && m78[i] != 0) {
        GetCursor_unk(m78[i])->SetMatrix(mtx);
        GetCursor_unk(m78[i])->UpdateMtx();
        fn_801F6D7C(GetCursor_unk(m78[i])->GetTask(), "cursor_pos", 0, x);
        fn_801F6D7C(GetCursor_unk(m78[i])->GetTask(), "cursor_pos", 1, y);
        fn_801F6D7C(GetCursor_unk(m78[i])->GetTask(), "cursor_pos", 4, z);
        fn_801F6C80(GetCursor_unk(m78[i])->GetTask(), "wRate", w);
        fn_801F6C80(GetCursor_unk(m78[i])->GetTask(), "hRate", h);
        if (end) {
            float frame = fn_801F6C2C(GetCursor_unk(m78[i])->GetTask()) - 2.0f;
            ((MenuLayout_unk*)GetCursor_unk(m78[i])->GetTask())->mFrame = frame;
        }
    }
    if (id != 0 && (unsigned int)m9C < 4) {
        MenuCommonEntry_unk entry;
        entry.m04 = (int)id;
        if (name != 0) {
            strcpy(entry.m08.mText, name);
        }
        new (&mA0[m9C]) MenuCommonEntry_unk(entry);
        m9C++;
    }
}

// inferred: places the next 2D cursor and adds an entry (id in m00)
extern "C" void fn_804A53D4(MenuCommonTask* menu, int id, const char* name, const D2APoint_unk* rect, int end) {
    if (menu->m88 + 1 < 4) {
        menu->m88++;
        if (menu->m8C[menu->m88] != 0) {
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 0, rect->x);
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 1, rect->y);
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 4, rect->m10);
            fn_801F6C80(menu->m8C[menu->m88], "wRate", rect->mScaleX);
            fn_801F6C80(menu->m8C[menu->m88], "hRate", rect->mScaleY);
            if (end) {
                menu->m8C[menu->m88]->mFrame = fn_801F6C2C(menu->m8C[menu->m88]) - 2.0f;
            }
        }
    }
    if (id != 0 && (unsigned int)menu->m9C < 4) {
        MenuCommonEntry_unk entry;
        entry.m00 = (int)id;
        if (name != 0) {
            strcpy(entry.m08.mText, name);
        }
        new (&menu->mA0[menu->m9C]) MenuCommonEntry_unk(entry);
        menu->m9C++;
    }
}

// inferred: same as fn_804A53D4 with the id in m04
extern "C" void fn_804A55FC(MenuCommonTask* menu, int id, const char* name, const D2APoint_unk* rect, int end) {
    if (menu->m88 + 1 < 4) {
        menu->m88++;
        if (menu->m8C[menu->m88] != 0) {
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 0, rect->x);
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 1, rect->y);
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 4, rect->m10);
            fn_801F6C80(menu->m8C[menu->m88], "wRate", rect->mScaleX);
            fn_801F6C80(menu->m8C[menu->m88], "hRate", rect->mScaleY);
            if (end) {
                menu->m8C[menu->m88]->mFrame = fn_801F6C2C(menu->m8C[menu->m88]) - 2.0f;
            }
        }
    }
    if (id != 0 && (unsigned int)menu->m9C < 4) {
        MenuCommonEntry_unk entry;
        entry.m04 = (int)id;
        if (name != 0) {
            strcpy(entry.m08.mText, name);
        }
        new (&menu->mA0[menu->m9C]) MenuCommonEntry_unk(entry);
        menu->m9C++;
    }
}

extern "C" std::basic_string<wchar_t> fn_800A4450(atn::D2A_TextParam_unk* text, int index, int table); // inferred
extern "C" void* lbl_8087EEC8; // inferred: font
extern "C" float fn_8006EF48(void* font, const wchar_t* text, float size, float f2, int a, int b); // inferred: width

// NONMATCHING: 98.8% - scheduling of the size argument of fn_8006EF48
// inferred: places the next 2D cursor on the text pane `name` of a d2anime (the anime is the id of the entry)
extern "C" void fn_804A5824(MenuCommonTask* self, atn::D2A_Task* anime, const char* name, int end) {
    if (anime == 0 || name == 0) {
        return;
    }
    atn::D2A_Common* element = anime->mData.fn_801EFA78(fn_800DC6B4(name));
    if (element == 0 || element->mType != 1) {
        return;
    }
    atn::D2A_Message* msg = (atn::D2A_Message*)element;
    D2APoint_unk rect = GetPanePos_unk(anime, name);
    float width;
    {
        std::basic_string<wchar_t> text = fn_800A4450(&msg->mText, 0, 0);
        width = fn_8006EF48(lbl_8087EEC8, text.data(), rect.mScaleX, 0.0f, 1, msg->mFont);
    }
    if (msg->mAlign == 1) {
        rect.x -= 0.5f * width + 1.0f;
    } else if (msg->mAlign == 2) {
        rect.x -= 1.0f + width;
    } else {
        rect.x -= 1.0f;
    }
    rect.mScaleX = 0.01f * (1.0f + (1.0f + width));
    rect.mScaleY = 0.01f * rect.mScaleY;
    rect.m10 = rect.m10 - 10.0f;
    // inferred: same code as fn_804A53D4 (written again in the original: its strings are in @stringBase0)
    MenuCommonTask* menu = MenuCommonTask::sInstance;
    int id = (int)anime;
    const D2APoint_unk* r = &rect;
    if (menu->m88 + 1 < 4) {
        menu->m88++;
        if (menu->m8C[menu->m88] != 0) {
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 0, r->x);
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 1, r->y);
            fn_801F6D7C(menu->m8C[menu->m88], "cursor_pos", 4, r->m10);
            fn_801F6C80(menu->m8C[menu->m88], "wRate", r->mScaleX);
            fn_801F6C80(menu->m8C[menu->m88], "hRate", r->mScaleY);
            if (end) {
                menu->m8C[menu->m88]->mFrame = fn_801F6C2C(menu->m8C[menu->m88]) - 2.0f;
            }
        }
    }
    if (id != 0 && (unsigned int)menu->m9C < 4) {
        MenuCommonEntry_unk entry;
        entry.m00 = (int)id;
        if (name != 0) {
            strcpy(entry.m08.mText, name);
        }
        new (&menu->mA0[menu->m9C]) MenuCommonEntry_unk(entry);
        menu->m9C++;
    }
}

// inferred: adds an entry (id + name) to the array at 0xA0
extern "C" void fn_804A5B9C(MenuCommonTask* menu, int id, const char* name) {
    if (!(id != 0 && (unsigned int)menu->m9C < 4)) {
        return;
    }
    MenuCommonEntry_unk entry;
    entry.m00 = id;
    if (name != 0) {
        strcpy(entry.m08.mText, name);
    }
    new (&menu->mA0[menu->m9C]) MenuCommonEntry_unk(entry);
    menu->m9C++;
}

// inferred: position of the info bar
extern "C" void fn_804A5CD8(MenuCommonTask* menu, float value) {
    SetPaneValue_unk(menu->m1C8->m58, "pos0015", 4, value);
}

// NONMATCHING: 92.9% - register allocation of the layout pointers (r30/r31 swapped)
// inferred: sets the descriptions of the info bar
extern "C" void fn_804A5D34(MenuCommonTask* menu, int id, const wchar_t* desc0, const wchar_t* desc1,
                            const wchar_t* desc2) {
    menu->m1C0 = id;
    SetPaneText_unk(menu->m1C8->m58, "desc0", desc0 != 0 ? desc0 : lbl_8087E0E0);
    SetPaneText_unk(menu->m1C8->m58, "desc1", desc1 != 0 ? desc1 : lbl_8087E0E0);
    SetPaneText_unk(menu->m1C8->m58, "desc2", desc2 != 0 ? desc2 : lbl_8087E0E0);
    const wchar_t* desc3 = 0;
    SetPaneText_unk(menu->m1C8->m58, "desc3", desc3 != 0 ? desc3 : lbl_8087E0E0);
}

// inferred: placement and contents of the A button prompts (17 types)
struct MenuButtonInfo_unk {
    float mX;     // 16:9
    float mY;
    float mX43;   // 4:3
    float mY43;
    float mZ;
    float mCross; // "crossOn"
    wchar_t mButton[4];
    int mUV;      // inferred: button icon
    int mMsg;     // inferred: action name
};
static const MenuButtonInfo_unk lbl_80757110[17] = {
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"c", 50, 10070},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"c", 50, 10071},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"c", 50, 10072},
    {70.0f, 407.0f, 80.0f, 395.0f, 0.0f, 0.0f, L"c", 50, 10073},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 0, 10076},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"b", 1, 10074},
    {570.0f, 407.0f, 560.0f, 395.0f, -1000.0f, 0.0f, L"a", 0, 107},
    {570.0f, 407.0f, 560.0f, 395.0f, -1000.0f, 0.0f, L"+", 40, 107},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 0, 10036},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 0, 10075},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 0, 107},
    {70.0f, 407.0f, 80.0f, 395.0f, 0.0f, 0.0f, L"b", 1, 10074},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 0, 10077},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 0, 10078},
    {570.0f, 407.0f, 560.0f, 395.0f, -1000.0f, 0.0f, L"a", 0, 107},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 0, 23},
    {570.0f, 407.0f, 560.0f, 395.0f, 0.0f, 0.0f, L"a", 51, 10079},
};

// inferred: game settings (lbl_8087F0A8, see game/CampSaveLoad.cpp); only the member used here
struct MenuSettings_unk {
    unsigned char m00[0x48C];
    unsigned char m48C[4]; // 0x48C: inferred: button icon table
};
extern "C" MenuSettings_unk* lbl_8087F0A8;
// inferred: pad input of the settings (game/Input_unk.h; declared here with the uv type of this file)
struct Input_unk {
    MenuUV_unk fn_80124C6C(int action); // inferred: UV of a button icon
};
extern "C" const wchar_t* fn_80116FC0(int group, int id); // inferred: message text

// inferred: shows an A button prompt (ui_abtn_reaction_*.d2a)
extern "C" void fn_804A5E40(MenuCommonTask* menu, int type, const float* ofs, int id) {
    if (!(0 <= type && type < 17)) {
        return;
    }
    if (menu->m1CC >= 2) {
        return;
    }
    int wide = lbl_8087EEE0->mWide;
    const MenuButtonInfo_unk* info = &lbl_80757110[type];
    float x = wide != 0 ? info->mX : info->mX43;
    float y = wide != 0 ? info->mY : info->mY43;
    menu->m1D0[menu->m1CC] = id;
    SetPaneValue_unk(menu->m1D8[menu->m1CC]->m58, "basePos", 0, x + ofs[0]);
    SetPaneValue_unk(menu->m1D8[menu->m1CC]->m58, "basePos", 1, y + ofs[1]);
    SetPaneValue_unk(menu->m1D8[menu->m1CC]->m58, "basePos", 4, info->mZ);
    SetPaneValue2_unk(menu->m1D8[menu->m1CC]->m58, "crossOn", info->mCross);
    SetUV_unk((atn::D2A_Task*)menu->m1D8[menu->m1CC], "btnUv", ((Input_unk*)lbl_8087F0A8->m48C)->fn_80124C6C(info->mUV));
    SetUV_unk((atn::D2A_Task*)menu->m1D8[menu->m1CC], "btnUvMask", ((Input_unk*)lbl_8087F0A8->m48C)->fn_80124C6C(info->mUV));
    SetPaneText_unk(menu->m1D8[menu->m1CC]->m58, "buttonStr", info->mButton);
    SetPaneText_unk(menu->m1D8[menu->m1CC]->m58, "action_name", fn_80116FC0(0, info->mMsg));
    SetPaneValue_unk(menu->m1E0[menu->m1CC]->m58, "basePos", 0, x + ofs[0]);
    SetPaneValue_unk(menu->m1E0[menu->m1CC]->m58, "basePos", 1, y + ofs[1]);
    SetPaneValue_unk(menu->m1E0[menu->m1CC]->m58, "basePos", 4, info->mZ);
    SetPaneValue2_unk(menu->m1E0[menu->m1CC]->m58, "crossOn", info->mCross);
    SetUV_unk((atn::D2A_Task*)menu->m1E0[menu->m1CC], "btnUv", ((Input_unk*)lbl_8087F0A8->m48C)->fn_80124C6C(info->mUV));
    SetUV_unk((atn::D2A_Task*)menu->m1E0[menu->m1CC], "btnUvMask", ((Input_unk*)lbl_8087F0A8->m48C)->fn_80124C6C(info->mUV));
    SetPaneText_unk(menu->m1E0[menu->m1CC]->m58, "buttonStr", info->mButton);
    SetPaneText_unk(menu->m1E0[menu->m1CC]->m58, "action_name", fn_80116FC0(0, info->mMsg));
    if (type == 6 || type == 7) {
        menu->m1D8[menu->m1CC]->m108 = 0x13;
        menu->m1E0[menu->m1CC]->m108 = 0x13;
    } else {
        menu->m1D8[menu->m1CC]->m108 = 0x12;
        menu->m1E0[menu->m1CC]->m108 = 0x12;
    }
    menu->m1CC++;
}

// inferred: message table (lbl_8087F1E4, see game/CharaTask.cpp)
extern "C" const wchar_t lbl_808813D0[2]; // inferred: empty text
struct MenuMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" MenuMsgTable_unk* lbl_8087F1E4;

// inferred: shows a confirmation message in camp_info2.d2a
extern "C" void fn_804A62C8(MenuCommonTask* menu, int type, int noCancel) {
    MenuD2A_unk* anime = menu->m6C;
    if (anime == 0) {
        return;
    }
    switch (type) {
    case 0:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x104));
        break;
    case 1:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x108));
        break;
    case 2:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x109));
        break;
    case 3:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x10A));
        break;
    case 4:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x10B));
        break;
    case 7:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x10C));
        break;
    case 8:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x10E));
        break;
    case 9:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x10F));
        break;
    case 10:
        SetPaneText_unk(menu->m6C->m58, "check_msg", fn_80116FC0(1, 0x110));
        break;
    default:
        SetPaneText_unk(anime->m58, "check_msg", lbl_8087F1E4->Get_unk(0x36D));
        break;
    }
    menu->m64 = 1;
    menu->m68 = noCancel == 0;
}

// inferred: shows a warning in the info window (third camp_info.d2a)
extern "C" void fn_804A65A0(MenuCommonTask* menu, const wchar_t* text) {
    SetPaneText_unk(menu->m60->m58, "warning_msg", text);
    menu->m5C = 1;
}

// inferred: event of an anime element (only the members used here)
struct MenuAnimeEvent_unk {
    int m00;
    unsigned int mHash;  // 0x04: inferred: hash of the element name
    int m08;
    int m0C;
    unsigned int mFlags; // 0x10
};
// inferred: object with the menu colors (lbl_8087F490); only the members used here
struct MenuColors_unk {
    unsigned char m00[0x288C];
    float mText[4];   // 0x288C: inferred: text color
    float mShadow[4]; // 0x289C: inferred: second color
};
extern "C" MenuColors_unk* lbl_8087F490;

// inferred: 8-bit color packed as ARGB (names invented)
inline unsigned int ColorByte_unk(float f) {
    return f >= 1.0f ? 0xFF : (f <= 0.0f ? 0 : (unsigned int)(255.0f * f + 0.5f));
}
struct MenuColor_unk {
    MenuColor_unk(const float* v, unsigned int alpha = 0) {
        unsigned int r = ColorByte_unk(v[0]);
        unsigned int g = ColorByte_unk(v[1]);
        unsigned int b = ColorByte_unk(v[2]);
        unsigned int a = ColorByte_unk(v[3]);
        mColor = alpha | ((a << 24) | (r << 16) | (g << 8) | b);
    }
    void Get_unk(float* v) const {
        v[0] = 0.003921569f * ((mColor >> 16) & 0xFF);
        v[1] = 0.003921569f * ((mColor >> 8) & 0xFF);
        v[2] = 0.003921569f * (mColor & 0xFF);
        v[3] = 0.003921569f * (mColor >> 24);
    }
    unsigned int mColor;
};
struct MenuColor4_unk {
    MenuColor4_unk(float r, float g, float b, float a) {
        v[0] = r;
        v[1] = g;
        v[2] = b;
        v[3] = a;
    }
    float v[4];
};

// inferred: replaces the default text colors of the registered elements by the colors of the settings
inline void ApplyColors_unk(const float* color, float* outColor, float* outColor2) {
    unsigned int alpha = MenuColor_unk(color).mColor & 0xFF000000;
    unsigned int rgb = MenuColor_unk(color).mColor & 0x00FFFFFF;
    if (rgb == MenuColor_unk(MenuColor4_unk(0.11764706f, 0.11764706f, 0.11764706f, 0.0f).v).mColor ||
        rgb == MenuColor_unk(MenuColor4_unk(0.78431374f, 0.78431374f, 0.78431374f, 0.0f).v).mColor ||
        rgb == MenuColor_unk(MenuColor4_unk(1.0f, 1.0f, 1.0f, 0.0f).v).mColor ||
        rgb == MenuColor_unk(MenuColor4_unk(0.84313726f, 0.8784314f, 0.91764706f, 0.0f).v).mColor) {
        if (lbl_8087F490 != 0) {
            MenuColor_unk c(lbl_8087F490->mText, alpha);
            c.Get_unk(outColor);
        }
        if (outColor2 != 0 && lbl_8087F490 != 0) {
            MenuColor_unk(lbl_8087F490->mShadow).Get_unk(outColor2);
        }
    }
}

// NONMATCHING: 98.9% - scheduling of the color conversion back to floats
// inferred: event of a d2anime: recolors the registered elements (entries with the anime in m00)
void MenuCommonTask::fn_804A660C(void* anime, int a, int b, int c) {
    const MenuAnimeEvent_unk* event = (const MenuAnimeEvent_unk*)a;
    float* color = (float*)b;
    float* color2 = (float*)c;
    if (anime == 0 || event == 0) {
        return;
    }
    unsigned int hash;
    MenuCommonEntry_unk* entry = mA0;
    int found = 0;
    for (unsigned int i = 0; i < m9C; entry++, i++) {
        if ((void*)entry->m00 == anime) {
            if (entry->m08.mText[0] == 0) {
                if ((event->mFlags & 2) == 2) {
                    found = 1;
                }
            } else {
                hash = event->mHash;
                if (hash == fn_800DC6B4(entry->m08.mText)) {
                    found = 1;
                }
            }
        }
    }
    if (found == 0) {
        return;
    }
    if (color == 0) {
        return;
    }
    ApplyColors_unk(color, color, color2);
}

// NONMATCHING: 98.9% - as fn_804A660C
// inferred: same as fn_804A660C for a layout anime (entries with the layout in m04)
void MenuCommonTask::fn_804A71F0(void* layout, int a, int b, int c) {
    const MenuAnimeEvent_unk* event = (const MenuAnimeEvent_unk*)a;
    float* color = (float*)b;
    float* color2 = (float*)c;
    if (layout == 0 || event == 0) {
        return;
    }
    unsigned int hash;
    MenuCommonEntry_unk* entry = mA0;
    int found = 0;
    for (unsigned int i = 0; i < m9C; entry++, i++) {
        if ((void*)entry->m04 == layout) {
            if (entry->m08.mText[0] == 0) {
                if ((event->mFlags & 2) == 2) {
                    found = 1;
                }
            } else {
                hash = event->mHash;
                if (hash == fn_800DC6B4(entry->m08.mText)) {
                    found = 1;
                }
            }
        }
    }
    if (found == 0) {
        return;
    }
    if (color == 0) {
        return;
    }
    ApplyColors_unk(color, color, color2);
}

extern "C" int fn_800DD3FC(wchar_t* s, const wchar_t* format, ...); // inferred: wide sprintf

// inferred: draws the name of an item at the top right
extern "C" void fn_804A7DD4(MenuCommonTask* menu, int id) {
    MenuItem_unk* item = fn_80211480(id);
    if (item != 0) {
        wchar_t text[0x80];
        float y = 120.0f;
        fn_800DD3FC(text, lbl_8087F1E4->Get_unk(0x36F), item->mName);
        float w = fn_8006EF48(lbl_8087EEC8, text, 14.0f, 0.0f, 1, 1);
        lbl_8087EEB0->fn_80061824(590.0f - w, y, 0.0f, 14.0f, 14.0f, 0.0f, 0.0f, 0.0f, text, -1, 1, 1, 0, 0, 0xFF000000);
    }
}

// fn_804A7EA4 (0x804A7EA4) is the creator of MiniMapTask (it uses the MiniMapTask string pool): the .text
// split of this file ends before it (see notes_G.md)
