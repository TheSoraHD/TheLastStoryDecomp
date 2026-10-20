#include "MP/D2AMpMenu_unk.h"
#include "MP/D2ACommonDialog.h"
#include "game/MenuCommonTask.h"

struct MpSession_unk;
extern "C" int fn_804EB870(MpSession_unk* s);
extern "C" unsigned int fn_804D9C14(MpSession_unk* s, int a, int b);
extern "C" void fn_804D824C(MpSession_unk* s, int a, int b);

// inferred: multiplayer session manager (lbl_8087F610); only the members used here
struct MpSession_unk {
    unsigned int fn_804D9C14_unk() { return fn_804D9C14(this, fn_804EB870(this), 0); }
    void fn_804D824C_unk() { fn_804D824C(this, fn_804EB870(this), 0); }

    unsigned char m000[0x540];
    int m540; // inferred: game mode (2: special rule set)
    unsigned char m544[0x564 - 0x544];
    int m564; // inferred: time limit in seconds (-1: none)
    unsigned char m568[0x5A4 - 0x568];
    int m5A4; // inferred: number of lives (at most 6)
    unsigned char m5A8[0x5B0 - 0x5A8];
    int m5B0;
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" int fn_804EB1AC(MpSession_unk* s);
extern "C" void fn_804EA608(MpSession_unk* s, int time, int b);
extern "C" int fn_804FB620(MpSession_unk* s, int mode); // inferred: highest value of the first rule

// inferred: main multiplayer menu (MP::D2AMpMenu::sInstance, lbl_8087F59C); only the member used here
struct MpMenu_unk {
    unsigned char m00[0xC4];
    int mC4;
};
extern "C" MpMenu_unk* lbl_8087F59C;

// inferred: camp message table (lbl_8087F1E4, see game/MenuCommonTask.cpp)
struct MpCampMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : MP::lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" MpCampMsgTable_unk* lbl_8087F1E4;

extern "C" void fn_804A5CD8(MenuCommonTask* menu, float y);
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c);
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int arg);

inline void* FindPane_unk(void* layout, const char* name) { return fn_801FEC74(layout, name); }

// inferred: fn_804CE61C keeps the pad in a register numbered as an inline result
inline void* GetPad_unk() { return lbl_8087EF70; }

namespace MP {

// inferred from __vt__Q22MP16D2AMpMenuSetRule (0x80790F30): rule settings of the multiplayer
// lobby (three rows: lives, team mode and time limit)
class D2AMpMenuSetRule : public atn::Task {
public:
    static D2AMpMenuSetRule* fn_804CD97C(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuSetRule(atn::Task* parent);
    virtual ~D2AMpMenuSetRule();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804CDE1C(int state);        // inferred: changes the state
    int fn_804CE2EC(int count);         // inferred: cursor input, returns 1 (decide) or 2 (cancel)
    void fn_804CE61C();                 // inferred: input of the open state
    void fn_804CE82C(int row, int before, int after, int dir); // inferred: shows a value change
    void fn_804CECEC();                 // inferred: reads the current rules
    void fn_804CEF84();                 // inferred: default rules
    void fn_804CF078(int arg);          // inferred: writes the rules

    void Add_unk(MpD2A_unk* task) {
        if (m7C.size() < 8) {
            m7C.push_back(task);
        }
    }
    void SetAll_unk(int arg) {
        for (MpD2A_unk** it = &m7C[0]; it != &m7C[m7C.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(arg);
            }
        }
    }
    void UpdateListPos_unk(); // inferred: not inline (its literals are in @stringBase0)
    // inferred: vf10 uses this inline helper (the registers of the inlined loop differ otherwise)
    void UpdateOpen_unk() {
        if (m48->mFrame >= 10.0f) {
            fn_804CDE1C(2);
        }
        UpdateListPos_unk();
    }
    int GetMaxLives_unk() { return fn_804FB620(lbl_8087F610, lbl_8087F610->m540 == 2 ? 2 : m6C[1] != 0); }
    void ReadTime_unk() {
        m6C[2] = 1;
        int min = lbl_8087F610->m564 / 60;
        for (int i = 0; i < 4; i++) {
            if (min == sTimes[i]) {
                m6C[2] = i;
            }
        }
    }
    void RefreshRows_unk() {
        for (int i = 0; i < 3; i++) {
            fn_804CE82C(i, m6C[i], m6C[i], 0);
        }
    }

    MpD2A_unk* m48;    // inferred: online_rule_base.d2a
    MpD2A_unk* m4C[3]; // inferred: option_rule_list.d2a (one per row)
    MpD2A_unk* m58;    // inferred: online_info_back.d2a
    int m5C;           // inferred: state
    int m60;           // inferred: previous state
    int m64;
    int m68;           // inferred: selected row
    int m6C[3];        // inferred: values of the rows (lives, team mode, time limit index)
    int m78;           // inferred: game mode when opened
    atn::FixedArray_unk<MpD2A_unk*, 8> m7C;

    static D2AMpMenuSetRule* sInstance;
    static const int sTimes[4];
};

D2AMpMenuSetRule* D2AMpMenuSetRule::sInstance;
const int D2AMpMenuSetRule::sTimes[4] = {1, 3, 5, 10};

D2AMpMenuSetRule* D2AMpMenuSetRule::fn_804CD97C(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) D2AMpMenuSetRule(parent);
    }
    return sInstance;
}

D2AMpMenuSetRule::D2AMpMenuSetRule(atn::Task* parent) : atn::Task(parent), m5C(0), m60(0), m64(0), m68(0), m78(0) {
    m48 = fn_801F3FF8(this, "/data/d2anime/online_rule_base.d2a", 0);
    Add_unk(m48);
    m58 = fn_801F3FF8(this, "/data/d2anime/online_info_back.d2a", 0);
    Add_unk(m58);
    for (int i = 0; i < 3; i++) {
        m4C[i] = fn_801F3FF8(this, "/data/d2anime/option_rule_list.d2a", 0);
        Add_unk(m4C[i]);
        m6C[i] = 0;
    }
    SetAll_unk(1);
}

D2AMpMenuSetRule::~D2AMpMenuSetRule() {
    m7C.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

// inferred: places the three rows on the base anime (inlined by its four callers)
void D2AMpMenuSetRule::UpdateListPos_unk() {
    MpRect_unk rect;
    for (int i = 0; i < 3; i++) {
        char name[0x40];
        sprintf(name, "message_list%02d", i + 1);
        MpRect_unk pos = GetPaneRect_unk(m48, name);
        rect.x = pos.x;
        rect.y = pos.y;
        rect.w = pos.w;
        rect.h = pos.h;
        rect.z = -5.0f;
        fn_801F4728(m4C[i], "base_pos(1)", &rect);
    }
}

// NONMATCHING: 98.46% - the original loads the 16.0f size argument of fn_8006EF48 before the
// branch of data(); tried a temporary/named/scoped string, inline width helpers, local size/font
int D2AMpMenuSetRule::vf0C() {
    if (fn_800D3FA4() != 0) {
        for (unsigned int i = 0; i < m7C.size(); i++) {
            m7C[i]->mSpeed = 0.0f;
        }
        SetAll_unk(0);
        fn_804CECEC();
        UpdateListPos_unk();
        void* pane = FindPane_unk(m58->mLayout, "string0007(1)");
        if (pane != 0) {
            float w = fn_8006EF48(lbl_8087EEC8, fn_800A4450(pane, 0, 0).data(), 16.0f, 0.0f, 1, 1);
            SetPaneValue_unk(m58->mLayout, "pos0017", 0, 590.0f - w);
        }
        m5C = 5;
        m60 = 5;
        return 1;
    }
    return 0;
}

void D2AMpMenuSetRule::fn_804CDE1C(int state) {
    if (m5C != state && state >= 0) {
        int row = m68;
        int prev = m5C;
        m60 = prev;
        m68 = 0;
        m5C = state;
        if (prev == 3) {
            fn_804ACAF8(D2ACommonDialog::sInstance);
            fn_804ACDBC(D2ACommonDialog::sInstance);
        }
        switch (m5C) {
        case 1:
            vf1C();
            Play_unk(m48, 1.0f);
            Play_unk(m58, 1.0f);
            fn_804CECEC();
            UpdateListPos_unk();
            m68 = 0;
            SetPaneValue_unk(m58->mLayout, "pos0007", 4, -200.0f);
            SetPaneValue_unk(m58->mLayout, "pos0017", 4, -200.0f);
            fn_804A5CD8(MenuCommonTask::sInstance, -100.0f);
            break;
        case 2:
            m68 = row;
            break;
        case 3:
            fn_804AC96C(D2ACommonDialog::sInstance, 0, 1.0f, 1, 0);
            fn_804ACD10(D2ACommonDialog::sInstance);
            fn_804AD000(D2ACommonDialog::sInstance, lbl_8087F1E4->Get_unk(0x37D), 0);
            m68 = row;
            break;
        case 4:
            vf20();
            Play_unk(m48, -1.0f);
            Play_unk(m58, -1.0f);
            break;
        case 5:
            fn_804CF078(0);
            break;
        }
    }
}

void D2AMpMenuSetRule::vf1C() {}

void D2AMpMenuSetRule::vf20() {}

void D2AMpMenuSetRule::vf10() {
    if (m64 != 0) {
        m64 = 0;
        return;
    }
    switch (m5C) {
    case 1:
        UpdateOpen_unk();
        break;
    case 2:
        fn_804CE61C();
        break;
    case 6:
        fn_804CE61C();
        break;
    case 3:
        if (D2ACommonDialog::sInstance->m4C != 0) {
            if (fn_804AC79C(D2ACommonDialog::sInstance) != 0) {
                fn_804CEF84();
                fn_804CDE1C(2);
            } else if (fn_804AC7EC(D2ACommonDialog::sInstance) != 0) {
                fn_804CDE1C(2);
            }
        }
        break;
    case 4:
        float frame = m48->mFrame;
        if (0.0f == frame) {
            fn_804CDE1C(5);
        }
        break;
    }
}

int D2AMpMenuSetRule::fn_804CE2EC(int count) {
    void* pad = lbl_8087EF70;
    int row = m68;
    int result = 0;
    int value = m6C[m68];
    if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
        if (--m68 < 0) {
            m68 = 2;
        }
    } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
        if (++m68 >= 3) {
            m68 = 0;
        }
    } else if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
        m6C[m68]--;
        if (m6C[m68] < 0) {
            m6C[m68] = count - 1;
        }
        fn_804CE82C(m68, m6C[m68], value, -1);
    } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
        m6C[m68]++;
        if (m6C[m68] >= count) {
            m6C[m68] = 0;
        }
        fn_804CE82C(m68, value, m6C[m68], 1);
    }
    if (lbl_8087F610->m540 != 2 && m68 == 1 && m6C[0] >= GetMaxLives_unk() + 1) {
        m6C[0] = GetMaxLives_unk();
        fn_804CE82C(0, value, m6C[0], 1);
    }
    if (m68 != row || value != m6C[m68]) {
        fn_80117228(3);
    }
    if (fn_800A555C(pad, 0, 4) != 0) {
        fn_80117228(1);
        result = 1;
    } else if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80117228(2);
        result = 2;
    }
    return result;
}

void D2AMpMenuSetRule::fn_804CE61C() {
    void* pad = GetPad_unk();
    if (m5C == 6) {
        if (fn_804EB1AC(lbl_8087F610) == 0 ||
            lbl_8087F610->fn_804D9C14_unk() == 1) {
            fn_804CDE1C(4);
        }
    } else {
        int count = m68 == 1 ? 2 : m68 == 2 ? 4 : GetMaxLives_unk() + 1;
        if (fn_804CE2EC(count) == 2) {
            if (fn_804EB1AC(lbl_8087F610) == 1) {
                lbl_8087F610->fn_804D824C_unk();
            }
            fn_804CDE1C(6);
        } else if (fn_800A555C(pad, 0, 0xC) != 0) {
            fn_80117228(1);
            fn_804CDE1C(3);
        }
    }
    UpdateListPos_unk();
}

void D2AMpMenuSetRule::fn_804CE82C(int row, int before, int after, int dir) {
    wchar_t textBefore[0x100] = {0};
    wchar_t textAfter[0x100] = {0};
    if (row == 0) {
        fn_800DD3FC(textBefore, L"%d", before);
        fn_800DD3FC(textAfter, L"%d", after);
    } else if (row == 1) {
        fn_800DD3FC(textBefore, before == 0 ? lbl_8087F86C->Get_unk(0xD9) : lbl_8087F86C->Get_unk(0xD8));
        fn_800DD3FC(textAfter, after == 0 ? lbl_8087F86C->Get_unk(0xD9) : lbl_8087F86C->Get_unk(0xD8));
    } else if (m78 != 2) {
        fn_800DD3FC(textBefore, lbl_8087F86C->Get_unk(0xDA), sTimes[before]);
        fn_800DD3FC(textAfter, lbl_8087F86C->Get_unk(0xDA), sTimes[after]);
    }
    Play_unk(m4C[row], dir);
    SetPaneText_unk(m4C[row]->mLayout, "conf_before(1)", textBefore);
    SetPaneText_unk(m4C[row]->mLayout, "conf_after(1)", textAfter);
    if (dir > 0) {
        m4C[row]->mFrame = 0.0f;
    } else {
        m4C[row]->mFrame = 10.0f;
    }
}

void D2AMpMenuSetRule::vf14() {
    for (unsigned int i = 0; i < m7C.size(); i++) {
        m7C[i]->mFlags |= 4;
    }
    switch (m5C) {
    case 1:
    case 2:
    case 3:
    case 6:
        m48->mFlags &= ~4;
        m58->mFlags &= ~4;
        m4C[0]->mFlags &= ~4;
        m4C[1]->mFlags &= ~4;
        m4C[2]->mFlags &= ~4;
        if (MenuCommonTask::sInstance != 0 && D2ACommonDialog::sInstance->m4C == 0 && m5C == 2) {
            MpRect_unk rect;
            MpRect_unk pos = GetPaneRect_unk(m4C[m68], "cursor_point(1)");
            rect.x = pos.x;
            rect.y = pos.y;
            rect.w = pos.w;
            rect.h = pos.h;
            rect.z = -10.0f;
            fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, 0);
        }
        if (m78 == 2) {
            fn_804A3C24(MenuCommonTask::sInstance, lbl_8087F86C->Get_unk(m68 + 0xD6), 0);
        } else {
            fn_804A3C24(MenuCommonTask::sInstance, lbl_8087F86C->Get_unk(m68 + 0xD3), 0);
        }
        break;
    case 4:
        vf20();
        for (unsigned int i = 0; i < m7C.size(); i++) {
            m7C[i]->mFlags &= ~4;
        }
        break;
    }
}

// inferred: unused locals (only their partially removed initialisation is left in the original)
struct MpPoint_unk {
    MpPoint_unk() {}
    float m00[6];
};

// NONMATCHING: 99.82% - r29/r30 swapped for the first text/layout pair; tried the text as a
// local, direct call, layout local, wrapper inlines
void D2AMpMenuSetRule::fn_804CECEC() {
    wchar_t unused0[0x20] = {0};
    MpRect_unk unused1;
    MpPoint_unk unused2[4];
    m78 = lbl_8087F610->m540;
    const wchar_t* text = lbl_8087F86C->Get_unk(0xDB);
    SetPaneText_unk(m4C[0]->mLayout, "name(1)", text);
    m6C[0] = lbl_8087F610->m5A4;
    if (m78 == 2) {
        SetPaneText_unk(m4C[1]->mLayout, "name(1)", lbl_8087F86C->Get_unk(0xDC));
        m6C[1] = lbl_8087F610->m5B0 != 0;
    } else {
        SetPaneText_unk(m4C[1]->mLayout, "name(1)", lbl_8087F86C->Get_unk(0xDD));
        m6C[1] = m78 != 0;
        SetPaneText_unk(m4C[2]->mLayout, "name(1)", lbl_8087F86C->Get_unk(0xDE));
        m6C[2] = 1;
        int min = lbl_8087F610->m564 / 60;
        for (int i = 0; i < 4; i++) {
            if (min == sTimes[i]) {
                m6C[2] = i;
            }
        }
    }
    RefreshRows_unk();
}

// inferred: the original calls this function from vf10 instead of inlining it; seven empty
// statements (release asserts) put it over the auto-inline threshold (six are not enough)
void D2AMpMenuSetRule::fn_804CEF84() {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    m6C[0] = 0;
    m6C[1] = 0;
    ReadTime_unk();
    for (int i = 0; i < 3; i++) {
        fn_804CE82C(i, m6C[i], m6C[i], 0);
    }
}

void D2AMpMenuSetRule::fn_804CF078(int arg) {
    int time = sTimes[m6C[2]] * 60;
    if (m78 == 2) {
        time = -1;
    }
    if (time != lbl_8087F610->m564) {
        fn_804EA608(lbl_8087F610, time, 0);
    }
    int lives = m6C[0];
    MpSession_unk* session = lbl_8087F610;
    session->m5A4 = lives;
    if (lives > 6) {
        session->m5A4 = 6;
    }
    if (m78 == 2) {
        lbl_8087F610->m5B0 = m6C[1] == 1;
    } else if (arg == 0) {
        if (m78 == 0 && m6C[1] == 1) {
            lbl_8087F610->m540 = 1;
            lbl_8087F59C->mC4 = 1;
        } else if (m78 == 1 && m6C[1] == 0) {
            lbl_8087F610->m540 = 0;
            lbl_8087F59C->mC4 = 3;
        }
    }
}

} // namespace MP
