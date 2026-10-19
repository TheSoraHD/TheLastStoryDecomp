#include "game/MenuCommonTask.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"

extern "C" MenuD2A_unk* fn_801F3FF8(atn::Task* parent, const char* path, int arg); // inferred: creates a D2A anime
extern "C" MenuCursor_unk* fn_80202A6C(atn::Task* parent, const char* path, int arg);
extern "C" atn::Task* fn_801F64D0(atn::Task* parent, const char* path);
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
        atn::Task* task = fn_801F64D0(this, cursor);
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

// NONMATCHING: the methods 0x804A39EC-0x804A7F18 are not written yet
