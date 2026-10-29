#include "MP/D2ACommonDialog.h"
#include "game/MenuCommonTask.h"
#include "std/vector.h"
#include "std/string_members.h"

namespace std {
// inferred: wstring elements are moved as raw bytes when push_back reallocates and the buffer
// destroys its elements before releasing them (same as in game/MenuCommonTask.cpp); local to
// this file
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

// inferred: splits a text in lines (see game/MenuCommonTask.cpp)
extern "C" void fn_800E0000(std::vector<std::basic_string<wchar_t> >* lines, const wchar_t* text, float width,
                            float size, int a, int b, int c, float d);
// inferred: moves a list cursor (game/MenuCommonTask.cpp)
extern "C" void fn_804A436C(int* cursor, int count, int arg2, int wrap, int sound);
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c);

namespace MP {

inline float GetFrame_unk(MpD2A_unk* task) { return task->mFrame; }

D2ACommonDialog* D2ACommonDialog::sInstance;

D2ACommonDialog* D2ACommonDialog::fn_804AC430(atn::Task* parent) {
    return sInstance = new ((MemoryTag)1, "", "", 0) D2ACommonDialog(parent);
}

D2ACommonDialog::D2ACommonDialog(atn::Task* parent) : atn::Task(parent), m48(0), m4C(0) {
    m54 = fn_801F3FF8(this, "/data/d2anime/camp_info.d2a", 0);
    m54->fn_800D246C(1);
    m58 = fn_801F3FF8(this, "/data/d2anime/camp_info3.d2a", 0);
    m58->fn_800D246C(1);
    m5C = fn_801F3FF8(this, "/data/d2anime/camp_info4.d2a", 0);
    m5C->fn_800D246C(1);
    m60 = fn_801F3FF8(this, "/data/d2anime/camp_info2.d2a", 0);
    m60->fn_800D246C(1);
    m64 = fn_801F3FF8(this, "/data/d2anime/camp_info7.d2a", 0);
    m64->fn_800D246C(1);
    m68 = fn_801F3FF8(this, "/data/d2anime/online_info_check.d2a", 0);
    m68->fn_800D246C(1);
}

D2ACommonDialog::~D2ACommonDialog() {}

int D2ACommonDialog::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetPaneValue_unk(m54->mLayout, "pos0001(1)", 4, -500.0f);
        SetPaneValue_unk(m58->mLayout, "pos0001(1)", 4, -800.0f);
        SetPaneValue_unk(m5C->mLayout, "pos0001(1)", 4, -1200.0f);
        SetPaneValue_unk(m60->mLayout, "pos0001(1)", 4, -300.0f);
        SetPaneValue_unk(m64->mLayout, "pos0001(1)", 4, -300.0f);
        SetPaneValue_unk(m68->mLayout, "pos0017", 4, -200.0f);
        SetPaneValue_unk(m68->mLayout, "pos0007", 4, -200.0f);
        return 1;
    }
    return 0;
}

int D2ACommonDialog::fn_804AC734() {
    if (0.0f != GetFrame_unk(m68) || 0.0f != GetFrame_unk(m60) || 0.0f != GetFrame_unk(m64)) {
        return 0;
    }
    return m4C == 2 || m4C == 4 || m4C == 5;
}

int D2ACommonDialog::fn_804AC79C() {
    if (0.0f != GetFrame_unk(m68) || 0.0f != GetFrame_unk(m60) || 0.0f != GetFrame_unk(m64)) {
        return 0;
    }
    return m4C == 4;
}

int D2ACommonDialog::fn_804AC7EC() {
    if (0.0f != GetFrame_unk(m68) || 0.0f != GetFrame_unk(m60) || 0.0f != GetFrame_unk(m64)) {
        return 0;
    }
    return m4C == 5;
}

void D2ACommonDialog::fn_804AC83C(int repeat, float speed, int type) {
    if (type == 0) {
        Play_unk(m54, repeat, speed);
    } else if (type == 1) {
        Play_unk(m58, repeat, speed);
    } else if (type == 2) {
        Play_unk(m5C, repeat, speed);
    }
    Play_unk(m68, repeat, speed);
    m4C = 1;
    m50 = type;
    m70 = 1;
    m6C = 1;
    m48 = 0;
}

// NONMATCHING: 99.70% - the task of the last Play_unk gets r26 instead of r27 (tried a helper for
// the pane values, a block and a local)
void D2ACommonDialog::fn_804AC96C(int repeat, float speed, int cursor, int type) {
    if (type == 0) {
        Play_unk(m60, repeat, speed);
    } else if (type == 1) {
        SetPaneValue_unk(m64->mLayout, "cursor_pos(1)", 1, 9.0f);
        SetPaneValue_unk(m64->mLayout, "pos0009(1)", 1, 27.5f);
        SetPaneValue_unk(m64->mLayout, "pos0002", 1, 9.0f);
        SetPaneValue_unk(m64->mLayout, "pos0004", 1, 30.0f);
        SetPaneValue_unk(m64->mLayout, "pos0003", 1, -40.0f);
        Play_unk(m64, repeat, speed);
    }
    m4C = 3;
    m48 = cursor;
    m50 = type;
    m70 = 2;
    m6C = 2;
}

// NONMATCHING: 98.84% - first text call: the original computes task+0x58 after the pool address
// (tried text/layout locals, direct call, helpers with other parameter orders)
void D2ACommonDialog::fn_804ACAF8() {
    Stop_unk(m54);
    Stop_unk(m58);
    Stop_unk(m5C);
    Stop_unk(m60);
    Stop_unk(m64);
    Stop_unk(m68);
    {
        void* layout = m58->mLayout;
        SetPaneText_unk(layout, "select00_text", L"");
    }
    {
        void* layout = m58->mLayout;
        SetPaneText_unk(layout, "select01_text", L"");
    }
    {
        void* layout = m58->mLayout;
        SetPaneText_unk(layout, "select02_text", L"");
    }
    {
        void* layout = m5C->mLayout;
        SetPaneText_unk(layout, "select00_text", L"");
    }
    {
        void* layout = m5C->mLayout;
        SetPaneText_unk(layout, "select01_text", L"");
    }
    {
        void* layout = m5C->mLayout;
        SetPaneText_unk(layout, "select02_text", L"");
    }
    m4C = 0;
}

void D2ACommonDialog::fn_804ACD10() {
    if (m4C == 1 || m4C == 2) {
        if (m50 == 0) {
            m54->mFlags &= ~4;
        } else if (m50 == 1) {
            m58->mFlags &= ~4;
        } else {
            m5C->mFlags &= ~4;
        }
        m68->mFlags &= ~4;
    } else if (m4C != 0) {
        if (m50 == 0) {
            m60->mFlags &= ~4;
        } else {
            m64->mFlags &= ~4;
        }
    }
}

void D2ACommonDialog::fn_804ACDBC() {
    if (m4C == 1 || m4C == 2) {
        if (m50 == 0) {
            m54->mFlags |= 4;
        } else if (m50 == 1) {
            m58->mFlags |= 4;
        } else {
            m5C->mFlags |= 4;
        }
        m68->mFlags |= 4;
    } else if (m4C != 0) {
        if (m50 == 0) {
            m60->mFlags |= 4;
        } else {
            m64->mFlags |= 4;
        }
    }
}

// NONMATCHING: 98.63% - text/layout registers of the calls before the loop (r29/r26 in the original)
void D2ACommonDialog::fn_804ACE68() {
    SetPaneText_unk(m54->mLayout, "warning_msg", L"");
    SetPaneText_unk(m60->mLayout, "check_msg", L"");
    SetPaneText_unk(m64->mLayout, "select00_text", L"");
    for (int i = 0; i < 3; i++) {
        char name[0x100] = {0};
        sprintf(name, "select%02d_text", i);
        SetPaneText_unk(m58->mLayout, name, L"");
        SetPaneText_unk(m5C->mLayout, name, L"");
    }
    SetPaneText_unk(m5C->mLayout, "string0007(1)", L"");
    SetPaneText_unk(m5C->mLayout, "string0009(1)", L"");
    SetPaneText_unk(m5C->mLayout, "string0010(1)", L"");
}

void D2ACommonDialog::fn_804AD000(const wchar_t* text, int line) {
    char name[0x100] = {0};
    if (m4C == 1 || m4C == 2) {
        if (m50 == 0) {
            SetPaneText_unk(m54->mLayout, "warning_msg", text);
        } else if (m50 == 1) {
            sprintf(name, "select%02d_text", line);
            SetPaneText_unk(m58->mLayout, name, text);
        } else {
            if (line == 3) {
                sprintf(name, "string0007(1)");
            } else if (line == 4) {
                sprintf(name, "string0009(1)");
            } else if (line == 5) {
                sprintf(name, "string0010(1)");
            } else {
                sprintf(name, "select%02d_text", line);
            }
            SetPaneText_unk(m5C->mLayout, name, text);
        }
    } else if (m4C != 0) {
        if (m50 == 0) {
            SetPaneText_unk(m60->mLayout, "check_msg", text);
        } else {
            SetPaneText_unk(m64->mLayout, "select00_text", text);
        }
    }
}

// NONMATCHING: 93.06% - the inlined vector<wstring>::push_back differs in registers/scheduling; the
// L"" of the original is a second .sdata literal (lbl_8087E0E8 + 2), not the file pool one
void D2ACommonDialog::fn_804AD1EC(const wchar_t* text) {
    std::vector<std::basic_string<wchar_t> > lines;
    fn_800E0000(&lines, text, 100000.0f, 16.0f, 1, 1, 0, 0.0f);
    while (lines.size() < 6) {
        lines.push_back(std::basic_string<wchar_t>(L""));
    }
    for (int i = 0; i < 6; i++) {
        fn_804AD000(lines[i].data(), i);
    }
}

void D2ACommonDialog::fn_804AD738() {
    switch (m4C) {
    default:
        return;
    case 1:
        m4C = 2;
        fn_80117228(1);
        break;
    case 3:
        m4C = 5;
        fn_80117228(2);
        break;
    }
    Play_unk(m54, 0, -1.0f);
    Play_unk(m58, 0, -1.0f);
    Play_unk(m5C, 0, -1.0f);
    Play_unk(m60, 0, -1.0f);
    Play_unk(m68, 0, -1.0f);
    Play_unk(m64, 0, -1.0f);
}

void D2ACommonDialog::vf10() {
    switch (m4C) {
    case 1:
        if (m68->mFrame < 5.0f) {
            break;
        }
    case 3:
        if (m4C == 3 && GetFrame_unk(m60) < 5.0f && m4C == 3 && GetFrame_unk(m64) < 5.0f) {
            break;
        }
        void* pad = lbl_8087EF70;
        fn_804A436C(&m48, m6C, m70, 1, 3);
        int done = 0;
        if (fn_800A555C(pad, 0, 4) != 0) {
            if (m4C == 1) {
                m4C = 2;
            } else if (m4C == 3) {
                m4C = m48 + 4;
            }
            done = 1;
        } else if (fn_800A555C(pad, 0, 5) != 0) {
            if (m4C == 1) {
                m4C = 2;
            } else if (m4C == 3) {
                m4C = 5;
            }
            done = 1;
        }
        if (done != 0) {
            if (m4C == 5) {
                fn_80117228(2);
            } else {
                fn_80117228(1);
            }
            Play_unk(m54, 0, -1.0f);
            Play_unk(m58, 0, -1.0f);
            Play_unk(m5C, 0, -1.0f);
            Play_unk(m60, 0, -1.0f);
            Play_unk(m64, 0, -1.0f);
            Play_unk(m68, 0, -1.0f);
        }
        break;
    }
}

void D2ACommonDialog::vf14() {
    if (MenuCommonTask::sInstance != 0 && m4C == 3) {
        char name[0x40];
        MpRect_unk rect;
        if (m50 == 0) {
            sprintf(name, "cursor_point%d", m48 + 1);
            rect = GetPaneRect_unk(m60, name);
        } else if (m50 == 1) {
            sprintf(name, "cursor_point%d", m48);
            rect = GetPaneRect_unk(m64, name);
        }
        fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, 0);
    }
}

} // namespace MP
