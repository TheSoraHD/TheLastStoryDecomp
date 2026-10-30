#include "MP/D2AMpMenu_unk.h"
#include "game/SetConfig.h"
#include "game/Input_unk.h"
#include "atn/DrawHelper.h"

// inferred: one message of the multiplayer message table (0x1C bytes)
struct MpMessage_unk {
    unsigned char m00[0x18];
    const wchar_t* mText; // 0x18
};
// inferred: categories of the message table (0xC bytes each)
struct MpMessageCategory_unk {
    unsigned int mCount;
    int m04;
    MpMessage_unk* mData;
};
// inferred: message table of the session (lbl_8087F610 + 0x2A50); names invented
struct MpMessageTable_unk {
    MpMessage_unk* fn_804C54FC(int category, int index);
    MpMessageCategory_unk mCategories[21];
};

// inferred: multiplayer session manager (lbl_8087F610); only the members used here
struct MpSession_unk {
    unsigned char m0000[0x4FC];
    int m4FC;
    unsigned char m500[0x508 - 0x500];
    short m508;
    unsigned char m50A[0x2A50 - 0x50A];
    MpMessageTable_unk m2A50;
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" int fn_804E82C4(MpSession_unk* s, int message); // inferred: sends a message

// inferred: one entry of the message list (category, index)
struct MpMessageRef_unk {
    short mCategory;
    short mIndex;
};
// inferred: multiplayer data (lbl_8087F628); only the members used here
struct MpData_unk {
    unsigned char m0000[0x1F8];
    unsigned int m1F8_hi : 11;
    unsigned int m1F8_11 : 1;
    unsigned int m1F8_lo : 20;
    unsigned char m1FC[0xC38 - 0x1FC];
    MpMessageRef_unk mMessages[40]; // 0xC38
    unsigned char mCD8[0xC215 - 0xCD8];
    unsigned char mC215;
};
extern "C" MpData_unk* lbl_8087F628;
extern "C" int fn_806B0DE0();
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task
extern "C" int fn_806A8E70();

// inferred: lobby (lbl_8087F5C4)
struct MpLobby_unk {
    unsigned char m000[0xD98];
    int mD98;
};
extern "C" MpLobby_unk* lbl_8087F5C4;
extern "C" int fn_804C2C14(MpLobby_unk* lobby);

// inferred: game settings (lbl_8087F0A8): help/button display at 0x48C
struct MpSettings_unk {
    unsigned char m000[0x48C];
    unsigned char mPad[1]; // 0x48C
};
extern "C" MpSettings_unk* lbl_8087F0A8;
extern "C" void* lbl_8087F430;

// inferred: debug text drawing

inline bool IsRepeated_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A55AC(lbl_8087EF70, 0, button) : 0;
}
inline int IsTriggered_unk(int button) {
    return lbl_8087EF70 != 0 ? fn_800A555C(lbl_8087EF70, 0, button) : 0;
}

namespace MP {

// inferred from __vt__Q22MP20D2AMpMenuMessageList (0x80790DB8): list of the fixed chat messages
class D2AMpMenuMessageList : public atn::Task {
public:
    static D2AMpMenuMessageList* fn_804C2FF8(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuMessageList(atn::Task* parent);
    virtual ~D2AMpMenuMessageList();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void SetTexts_unk();
    void fn_804C35F0(int state);  // inferred: changes the state
    void fn_804C38C0(int show);   // inferred: shows the button marks
    int fn_804C4150(int count);   // inferred: list input
    void fn_804C4530();           // inferred: update of the open state
    void fn_804C50C0();           // inferred: draw of the list
    void fn_804C5638();           // inferred: closes the task

    void Add_unk(MpD2A_unk* task) {
        if (mTasks.size() < 0x10) {
            mTasks.push_back(task);
        }
    }
    void SetAll_unk(int arg) {
        for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(arg);
            }
        }
    }

    MpD2A_unk* m48; // inferred: online_message_base2.d2a
    MpD2A_unk* m4C; // inferred: camp_cursor.d2a
    atn::FixedArray_unk<MpD2A_unk*, 0x10> mTasks; // 0x50
    int m94;        // inferred: state
    int m98;        // inferred: previous state
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;        // inferred: cursor
    float mB0;      // inferred: scroll timer of the selected message
    int mB4;

    static D2AMpMenuMessageList* sInstance;
    static int sPage;
};

// NONMATCHING (__sinit): the store is scheduled before the reload of the return address in the original
static unsigned int sCustomData = fn_800DC6B4("CustomData");
D2AMpMenuMessageList* D2AMpMenuMessageList::sInstance;
int D2AMpMenuMessageList::sPage;

D2AMpMenuMessageList* D2AMpMenuMessageList::fn_804C2FF8(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) D2AMpMenuMessageList(parent);
    }
    return sInstance;
}

D2AMpMenuMessageList::D2AMpMenuMessageList(atn::Task* parent)
    : atn::Task(parent), m94(0), m98(0), mA0(0), mA4(0), mA8(0), mAC(0), mB0(0.0f) {
    m48 = fn_801F3FF8(this, "/data/d2anime/online_message_base2.d2a", 0);
    Add_unk(m48);
    m4C = fn_801F3FF8(this, "/data/d2anime/camp_cursor.d2a", 0);
    Add_unk(m4C);
    SetAll_unk(1);
}

D2AMpMenuMessageList::~D2AMpMenuMessageList() {
    m48 = 0;
    mTasks.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

// inferred: texts of the open state; not inline (its literals are in @stringBase0)
void D2AMpMenuMessageList::SetTexts_unk() {
    if (SetConfig::sInstance->mInTacviewType == 0) {
        fn_801FEE08(m48->mLayout, fn_800DC6B4("controllerType"), L"$Bu");
    } else {
        fn_801FEE08(m48->mLayout, fn_800DC6B4("controllerType"), L"$Bd");
    }
    fn_801FEE08(m48->mLayout, fn_800DC6B4("string0017(1)"), L"$Br");
    fn_801FEE08(m48->mLayout, fn_800DC6B4("string0030(1)"), L"$Bl");
    SetPaneText_unk(m48->mLayout, "string0031(1)", lbl_8087F86C->Get_unk(0x136));
    SetPaneText_unk(m48->mLayout, "string0032(1)", lbl_8087F86C->Get_unk(0x137));
    SetPaneText_unk(m48->mLayout, "string0033(1)", lbl_8087F86C->Get_unk(0x138));
    SetPaneText_unk(m48->mLayout, "string0034(1)", lbl_8087F86C->Get_unk(0x138));
}

// NONMATCHING: the controllerType/$Br/$Bl texts: the original computes task+0x58 before the hash and
// loads the text after it (no helper/direct call form found); also inlined in vf0C, vf10, fn_804C4530
void D2AMpMenuMessageList::fn_804C35F0(int state) {
    if (m94 != state && state >= 0) {
        int prev = m94;
        m98 = prev;
        m94 = state;
        if (prev == 2) {
            Play_unk(m48, 0, -1.0f);
        }
        switch (m94) {
        case 1:
            vf1C();
            Play_unk(m48, 0, 1.0f);
            SetTexts_unk();
            mB0 = 0.0f;
            break;
        case 3:
            vf20();
            break;
        }
    }
}

int D2AMpMenuMessageList::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetAll_unk(0);
        for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
            if (*it != 0) {
                (*it)->mFlags |= 4;
            }
        }
        for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
            if (*it != 0) {
                (*it)->mFrame = 0.0f;
            }
        }
        for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
            if (*it != 0) {
                (*it)->mSpeed = 1.0f;
            }
        }
        m4C->mRepeat = 1;
        fn_804C35F0(4);
        return 1;
    }
    return 0;
}

void D2AMpMenuMessageList::vf1C() {}

void D2AMpMenuMessageList::vf20() {}

// NONMATCHING: text/layout registers of the message texts
void D2AMpMenuMessageList::fn_804C38C0(int show) {
    if (SetConfig::sInstance->mInTacviewType == 0) {
        SetPaneText_unk(m48->mLayout, "controllerType", show != 0 ? L"$Bu" : L"");
    } else {
        SetPaneText_unk(m48->mLayout, "controllerType", show != 0 ? L"$Bd" : L"");
    }
    SetPaneText_unk(m48->mLayout, "string0017(1)", show != 0 ? L"$Br" : L"");
    SetPaneText_unk(m48->mLayout, "string0030(1)", show != 0 ? L"$Bl" : L"");
    {
        const wchar_t* text = show != 0 ? lbl_8087F86C->Get_unk(0x136) : L"";
        SetPaneText_unk(m48->mLayout, "string0031(1)", text);
    }
    {
        const wchar_t* text = show != 0 ? lbl_8087F86C->Get_unk(0x137) : L"";
        SetPaneText_unk(m48->mLayout, "string0032(1)", text);
    }
    {
        const wchar_t* text = show != 0 ? lbl_8087F86C->Get_unk(0x138) : L"";
        SetPaneText_unk(m48->mLayout, "string0033(1)", text);
    }
    {
        const wchar_t* text = show != 0 ? lbl_8087F86C->Get_unk(0x138) : L"";
        SetPaneText_unk(m48->mLayout, "string0034(1)", text);
    }
}

void D2AMpMenuMessageList::vf10() {
    if (mA0 != 0) {
        mA0 = 0;
        return;
    }
    switch (m94) {
    case 1:
        if (lbl_8087F5C4 != 0 && fn_804C2C14(lbl_8087F5C4) == 0) {
            lbl_8087F5C4->mD98 = 1;
        }
        if (m48->mFrame >= 10.0f) {
            fn_804C35F0(2);
        }
        break;
    case 2:
        if (lbl_8087F5C4 != 0 && fn_804C2C14(lbl_8087F5C4) == 0) {
            lbl_8087F5C4->mD98 = 1;
        }
        fn_804C4530();
        break;
    case 3:
        fn_804C35F0(4);
        break;
    }
}

// NONMATCHING: the original materialises the direction conditions as booleans (same open problem
// as CampSaveLoad::fn_8011B3D8)
int D2AMpMenuMessageList::fn_804C4150(int count) {
    int result = 0;
    if (lbl_8087EF70 == 0) {
        return 0;
    }
    int cursor = mAC;
    int page = sPage;
    if (IsRepeated_unk(0) || (lbl_8087F430 == 0 && IsRepeated_unk(0x1A))) {
        if (--mAC < 0) {
            mAC += 10;
        }
        ((Input_unk*)lbl_8087F0A8->mPad)->fn_80124BFC(0, 1);
    } else if (IsRepeated_unk(1) || (lbl_8087F430 == 0 && IsRepeated_unk(0x19))) {
        if (++mAC >= 10) {
            mAC -= 10;
        }
        ((Input_unk*)lbl_8087F0A8->mPad)->fn_80124BFC(1, 1);
    } else if (IsRepeated_unk(2) || (lbl_8087F430 == 0 && IsRepeated_unk(0x17))) {
        if (--sPage < 0) {
            sPage += 4;
        }
        ((Input_unk*)lbl_8087F0A8->mPad)->fn_80124BFC(2, 1);
    } else if (IsRepeated_unk(3) || (lbl_8087F430 == 0 && IsRepeated_unk(0x18))) {
        if (++sPage >= 4) {
            sPage -= 4;
        }
        ((Input_unk*)lbl_8087F0A8->mPad)->fn_80124BFC(3, 1);
    }
    if (cursor != mAC || page != sPage) {
        fn_80117228(3);
        mB0 = 0.0f;
    }
    if (IsTriggered_unk(4) != 0) {
        fn_80117228(1);
        ((Input_unk*)lbl_8087F0A8->mPad)->fn_80124BFC(4, 1);
        result = 1;
    } else if (IsTriggered_unk(5) != 0) {
        fn_80117228(2);
        ((Input_unk*)lbl_8087F0A8->mPad)->fn_80124BFC(5, 1);
        result = 2;
    }
    return result;
}

// NONMATCHING: inlined fn_804C35F0 copies (see there)
void D2AMpMenuMessageList::fn_804C4530() {
    MpData_unk* data = lbl_8087F628;
    if (data != 0) {
        int count;
        if (data->mC215 == 0) {
            count = data->m1F8_11 ? 1 : fn_806B0DE0();
        } else {
            count = data->m1F8_11 ? 1 : fn_806A8E70();
        }
        if (count > 1 && lbl_8087F610->m4FC != 30 && lbl_8087F610->m508 < 90) {
            fn_804C35F0(3);
            return;
        }
    }
    int input = fn_804C4150(10);
    if (lbl_8087EF70 == 0) {
        return;
    }
    if (input == 1 || input == 2) {
        if (input == 1) {
            if (fn_804E82C4(lbl_8087F610, mAC) != 0) {
                fn_804C35F0(3);
            }
        } else {
            fn_804C35F0(3);
        }
    }
    if (((Input_unk*)lbl_8087F0A8->mPad)->fn_801231D0(0x34) != 0) {
        fn_80117228(2);
        fn_804C35F0(3);
    }
}

void D2AMpMenuMessageList::vf14() {
    for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
        if (*it != 0) {
            (*it)->mFlags |= 4;
        }
    }
    switch (m94) {
    case 1:
    case 2:
        fn_804C50C0();
        break;
    }
}

// NONMATCHING: not checked in detail (text drawing with scrolling)
void D2AMpMenuMessageList::fn_804C50C0() {
    float scale = 0.05f;
    float speed = 1.5f;
    float width = 220.0f;
    m48->mFlags &= ~4;
    m48->SetNumber_unk("page", sPage + 1, 0);
    if (m48->mFrame < 2.0f) {
        return;
    }
    char name[0x40];
    sprintf(name, "message_list%02d", mAC + 1);
    MpRect_unk cursor = GetPaneRect_unk(m48, name);
    cursor.w = 2.56f;
    cursor.h = 0.24f;
    cursor.z = -19.0f;
    fn_801F4728(m4C, "cursor_pos", &cursor);
    m4C->mFlags &= ~4;
    float scroll;
    if ((mB0 += 1.0f) < 50.0f) {
        scroll = 0.0f;
    } else {
        scroll = (mB0 - 50.0f) * speed;
    }
    MpMessageRef_unk* refs = lbl_8087F628->mMessages;
    for (int i = 0; i < 10; i++) {
        sprintf(name, "message_list%02d", i + 1);
        MpRect_unk pos = GetPaneRect_unk(m48, name);
        pos.x += 4.0f;
        pos.y += 6.0f;
        MpMessageRef_unk* ref = &refs[i + sPage * 10];
        MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(ref->mCategory, ref->mIndex);
        if (msg != 0) {
            const wchar_t* text = msg->mText != 0 ? msg->mText : lbl_808813D0;
            if (text != 0) {
                float off = i == mAC ? scroll : 0.0f;
                lbl_8087EEB0->fn_80061AE4(msg->mText != 0 ? msg->mText : lbl_808813D0, -1, 1, 1, 0, 1, 0xFF000000, pos.x - off, pos.y, -100.0f, 17.0f, 17.0f, 0.0f, 0.0f, 0.0f, pos.x - width * scale, width, scale);
                if (i == mAC) {
                    float w = fn_8006EF48(lbl_8087EEC8, msg->mText != 0 ? msg->mText : lbl_808813D0, 17.0f, 0.0f, 1, 1);
                    if (scroll > w) {
                        mB0 = 0.0f;
                    }
                }
            }
        }
    }
}

} // namespace MP

// NONMATCHING: the original inlines its own recursive call once
MpMessage_unk* MpMessageTable_unk::fn_804C54FC(int category, int index) {
    if (category < 0 || category > 21) {
        return 0;
    }
    if (category < 21) {
        if (index < 0 || mCategories[category].mCount <= (unsigned int)index) {
            return 0;
        }
        return &mCategories[category].mData[index];
    }
    if (index < 0) {
        return 0;
    }
    int i;
    for (i = 0; i < 20 && mCategories[i].mCount <= (unsigned int)index; i++) {
        index -= mCategories[i].mCount;
    }
    if (i >= 20) {
        return 0;
    }
    return fn_804C54FC(i, index);
}

namespace MP {

void D2AMpMenuMessageList::fn_804C5638() {
    fn_800D2338(this);
}

} // namespace MP
