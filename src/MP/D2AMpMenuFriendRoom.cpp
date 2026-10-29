#include "MP/D2AMpMenu_unk.h"
#include "MP/D2ACommonDialog.h"
#include "game/MenuCommonTask.h"
#include "std/vector.h"

// inferred: multiplayer session manager (lbl_8087F610); only the members used here
struct MpSession_unk {
    unsigned char m000[0x540];
    int m540; // inferred: game mode
    int m544;
    unsigned char m548[0x99EC - 0x548];
    int m99EC;
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" int fn_804DC114(MpSession_unk* s, int room); // inferred: state of a room (< 0: none)
extern "C" int fn_804DC7C0(MpSession_unk* s, int room); // inferred: number of members
extern "C" int fn_804DC80C(MpSession_unk* s, int room); // inferred: capacity
extern "C" void fn_804DC858(MpSession_unk* s);           // inferred: requests the room list
extern "C" void fn_804DB474(MpSession_unk* s);
extern "C" void fn_804DC450(MpSession_unk* s, int arg);

// inferred: list of the friend rooms (lbl_8087F628 + 0x430)
struct MpRoomList_unk {
    unsigned char m00[1];
};
extern "C" int fn_8050F5A8(MpRoomList_unk* list, int arg);         // inferred: first room
extern "C" int fn_8050F664(MpRoomList_unk* list, int arg);         // inferred: next room
extern "C" int fn_8050F868(MpRoomList_unk* list, int room);        // inferred: nonzero if valid
extern "C" int fn_8050F764(MpRoomList_unk* list, int room);
extern "C" const wchar_t* fn_8050F724(MpRoomList_unk* list, int room); // inferred: name

// inferred: multiplayer data (lbl_8087F628); only the members used here
struct MpData_unk {
    unsigned char m0000[0x90];
    unsigned char m90; // inferred: 1 when a new room is made
    unsigned char m91[0x430 - 0x91];
    MpRoomList_unk m430;
    unsigned char m431[0xC1F4 - 0x431];
    int mC1F4;
    int mC1F8;
    unsigned char mC1FC[0xC214 - 0xC1FC];
    unsigned char mC214; // inferred: room to join
};
extern "C" MpData_unk* lbl_8087F628;

// inferred: main multiplayer menu (lbl_8087F59C); only the member used here
struct MpMenu_unk {
    unsigned char m00[0xC4];
    int mC4;
};
extern "C" MpMenu_unk* lbl_8087F59C;
extern "C" void fn_804B4150(MpMenu_unk* menu, int state);

extern "C" void fn_804BBBA0(atn::Task* parent, int a, int b); // inferred: creates the lobby
extern "C" void fn_804C2FF8(atn::Task* parent);               // inferred: creates the message list
extern "C" void fn_804A436C(int* cursor, int count, int arg2, int wrap, int sound);
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c);
extern "C" void fn_804A4738(MP::MpD2A_unk* bar, int top, int count, int visible); // inferred: scroll bar
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);

inline void* FindPane_unk(void* layout, const char* name) { return fn_801FEC74(layout, name); }
inline void* GetPad_unk() { return lbl_8087EF70; }
inline void SetPaneValue3_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

namespace MP {

// inferred: ids of the rooms (a class derived from std::vector: its destructor tests `this` four times)
struct MpRoomIds_unk : public std::vector<int> {};

// inferred from __vt__Q22MP19D2AMpMenuFriendRoom (0x80790C08): list of the friend rooms
class D2AMpMenuFriendRoom : public atn::Task {
public:
    static D2AMpMenuFriendRoom* fn_804B9D44(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuFriendRoom(atn::Task* parent);
    virtual ~D2AMpMenuFriendRoom();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804BA350(int state); // inferred: changes the state
    void UpdateScrollBar_unk();  // inferred: not inline (its literals are in @stringBase0)
    void fn_804BA970();          // inferred: input of the list
    void fn_804BB10C();          // inferred: draw of the list
    void fn_804BB2CC(int refresh); // inferred: rebuilds the list

    // inferred: helpers of fn_804BA350 (the registers of their loops differ when written inline)
    void CloseList_unk() {
        Play_unk(m48, 0, -1.0f);
        Play_unk(m4C, 0, -1.0f);
        for (int i = 0; i < 10; i++) {
            Play_unk(m50[i], 0, -1.0f);
        }
        Play_unk(m80, 0, -1.0f);
    }
    void OpenList_unk() {
        vf1C();
        fn_804BB2CC(1);
        Play_unk(m48, 0, 1.0f);
        Play_unk(m4C, 0, 1.0f);
        for (int i = 0; i < 10; i++) {
            Play_unk(m50[i], 0, 1.0f);
        }
        Play_unk(m78, 0, 1.0f);
    }
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

    MpD2A_unk* m48;     // inferred: online_groupselect_logo.d2a
    MpD2A_unk* m4C;     // inferred: online_groupselect_base.d2a
    MpD2A_unk* m50[10]; // inferred: online_groupselect_list.d2a
    MpD2A_unk* m78;     // inferred: online_info_back.d2a
    MpD2A_unk* m7C;     // inferred: online_info_cancel.d2a
    MpD2A_unk* m80;     // inferred: scroll_bar.d2a
    MpD2A_unk* m84;     // inferred: online_connect_base.d2a
    int m88;            // inferred: state
    int m8C;            // inferred: previous state
    int m90;
    int m94;            // inferred: first row shown
    int m98;
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;            // inferred: cursor (0: new room)
    int mB0;
    int mB4;            // inferred: number of rooms
    atn::FixedArray_unk<MpD2A_unk*, 0x10> mTasks; // 0xB8
    MpRoomIds_unk mFC;  // inferred: ids of the rooms
    int m108;           // inferred: refresh timer
    int m10C;

    static D2AMpMenuFriendRoom* sInstance;
};

D2AMpMenuFriendRoom* D2AMpMenuFriendRoom::sInstance;

D2AMpMenuFriendRoom* D2AMpMenuFriendRoom::fn_804B9D44(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) D2AMpMenuFriendRoom(parent);
    }
    return sInstance;
}

D2AMpMenuFriendRoom::D2AMpMenuFriendRoom(atn::Task* parent)
    : atn::Task(parent), m88(0), m8C(0), m94(0), mA0(0), mA4(0), mA8(0), mAC(0), mB4(0), m108(0) {
    m48 = fn_801F3FF8(this, "/data/d2anime/online_groupselect_logo.d2a", 0);
    Add_unk(m48);
    m4C = fn_801F3FF8(this, "/data/d2anime/online_groupselect_base.d2a", 0);
    Add_unk(m4C);
    m84 = fn_801F3FF8(this, "/data/d2anime/online_connect_base.d2a", 0);
    Add_unk(m84);
    m78 = fn_801F3FF8(this, "/data/d2anime/online_info_back.d2a", 0);
    Add_unk(m78);
    m7C = fn_801F3FF8(this, "/data/d2anime/online_info_cancel.d2a", 0);
    Add_unk(m7C);
    for (int i = 0; i < 10; i++) {
        m50[i] = fn_801F3FF8(this, "/data/d2anime/online_groupselect_list.d2a", 0);
        Add_unk(m50[i]);
    }
    m80 = fn_801F3FF8(this, "/data/d2anime/scroll_bar.d2a", 0);
    Add_unk(m80);
    SetAll_unk(1);
}

D2AMpMenuFriendRoom::~D2AMpMenuFriendRoom() {
    mTasks.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

// NONMATCHING: 98.28% - 16.0f loaded before the data() branch (as in D2AMpMenuSetRule::vf0C) and
// text/layout registers of the L"" texts
int D2AMpMenuFriendRoom::vf0C() {
    if (fn_800D3FA4() != 0) {
        mA0 = 0;
        fn_804BA350(6);
        SetAll_unk(0);
        m80->mFlags |= 4;
        m78->mFlags |= 4;
        SetPaneValue_unk(m78->mLayout, "pos0007", 4, -110.0f);
        SetPaneValue_unk(m78->mLayout, "pos0017", 4, -110.0f);
        void* pane = FindPane_unk(m78->mLayout, "string0007(1)");
        if (pane != 0) {
            float w = fn_8006EF48(lbl_8087EEC8, fn_800A4450(pane, 0, 0).data(), 16.0f, 0.0f, 1, 1);
            SetPaneValue_unk(m78->mLayout, "pos0017", 0, 590.0f - w);
        }
        SetPaneText_unk(m4C->mLayout, "string0001(1)", L"");
        SetPaneText_unk(m4C->mLayout, "string0002(1)", L"");
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.63% - loop registers of OpenList_unk and the text/layout registers of case 8
void D2AMpMenuFriendRoom::fn_804BA350(int state) {
    if (m88 != state && state >= 0) {
        int prev = m88;
        mAC = 0;
        m8C = prev;
        m88 = state;
        if (prev == 8 || prev == 2) {
            CloseList_unk();
        }
        switch (m88) {
        case 1:
            OpenList_unk();
            break;
        case 2:
            Play_unk(m80, 0, 1.0f);
            break;
        case 4:
            vf20();
            Stop_unk(m78);
            break;
        case 7:
            fn_804BBBA0(this, 1, lbl_8087F610->m544);
            fn_804C2FF8(this);
            Stop_unk(m78);
            break;
        case 8: {
            int msg = 0x101;
            if (lbl_8087F628->m90 == 1) {
                msg = 0x100;
            }
            Play_unk(m84, 1, 1.0f);
            SetPaneText_unk(m84->mLayout, "cmd01", lbl_8087F86C->Get_unk(msg));
            SetPaneText_unk(m84->mLayout, "string0003(1)", lbl_8087F86C->Get_unk(msg));
            Stop_unk(m78);
            Play_unk(m7C, 0, 1.0f);
            break;
        }
        case 9:
            Stop_unk(m84);
            m84->fn_800D246C(1);
            Stop_unk(m78);
            Stop_unk(m7C);
            break;
        }
    }
}

void D2AMpMenuFriendRoom::vf1C() {}

void D2AMpMenuFriendRoom::vf20() {}

void D2AMpMenuFriendRoom::vf10() {
    switch (m88) {
    case 1:
        fn_804BB2CC(1);
        if (m4C->mFrame >= 10.0f) {
            fn_804BA350(2);
        }
        UpdateScrollBar_unk();
        break;
    case 2:
        fn_804BA970();
        break;
    case 4:
        if (m4C->mFrame <= 0.0f) {
            fn_804BA350(5);
        }
        break;
    }
}

void D2AMpMenuFriendRoom::UpdateScrollBar_unk() {
    MpRect_unk rect;
    MpRect_unk pos = GetPaneRect_unk(m4C, "sb_null_0001");
    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = pos.w;
    rect.z = pos.z;
    rect.h = 1.0f;
    fn_801F4728(m80, "basePos", &rect);
    void* layout = m80->mLayout;
    SetPaneValue3_unk(layout, "s_hightMax", 197.0f);
}

// NONMATCHING: 99.62% - cursor/top/decide/pad registers and the stack slots of the sound
// temporaries and of the inlined UpdateScrollBar_unk
void D2AMpMenuFriendRoom::fn_804BA970() {
    if (m108 > 0) {
        m108--;
    } else {
        fn_804DC858(lbl_8087F610);
        fn_804BB2CC(1);
        m108 = 150;
    }
    if (D2ACommonDialog::sInstance->m4C != 0) {
        if (D2ACommonDialog::sInstance->fn_804AC734() != 0) {
            D2ACommonDialog::sInstance->fn_804ACAF8();
            D2ACommonDialog::sInstance->fn_804ACDBC();
        }
        return;
    }
    int cursor = mAC;
    int top = m94;
    int decide = 0;
    {
        void* pad = GetPad_unk();
        fn_804A436C(&mAC, 0, 0, 1, 3);
        if (fn_800A555C(pad, 0, 4) != 0) {
            fn_80117228(1);
            decide = 1;
        } else if (fn_800A555C(pad, 0, 5) != 0) {
            fn_80117228(2);
            decide = 2;
        }
    }
    void* pad = GetPad_unk();
    if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
        int t = m94;
        if (--mAC, t != 0) {
            if (mAC < 1) {
                mAC = 1;
                m94 = t - 1;
                fn_804BB2CC(0);
            }
        } else if (mAC < 0) {
            mAC = 0;
        }
    }
    if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
        if (mAC > 0) {
            int t = m94;
            int n = t - 10;
            n = n > 0 ? n : 0;
            m94 = n;
            if (n != t) {
                int c = mAC - (10 - (t - n));
                mAC = c > 1 ? c : 1;
            } else {
                mAC = 1;
            }
            fn_804BB2CC(0);
        }
    } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
        int c = ++mAC;
        if (c > 10) {
            int t = m94;
            if (t + c <= mB4) {
                m94 = t + 1;
                fn_804BB2CC(0);
            }
            mAC = mB4 > 10 ? 10 : mB4;
        } else if (c >= mB4) {
            mAC = mB4;
        }
    }
    if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
        if (mAC > 0) {
            if (m94 >= mB4 - 10) {
                mAC = mB4 < 10 ? mB4 : 10;
            }
            if (mB4 > 10) {
                int t = m94;
                int n = mB4 - 10;
                if (t + 10 < n) {
                    n = t + 10;
                }
                m94 = n;
                if (n != t) {
                    int c = mAC + 10 - (n - t);
                    mAC = c < 10 ? c : 10;
                } else {
                    mAC = 10;
                }
            }
            fn_804BB2CC(0);
        }
    }
    if (mAC != cursor || top != m94) {
        fn_80117228(3);
    }
    if (decide == 1) {
        unsigned int ok = 0;
        if (mAC == 0) {
            lbl_8087F628->m90 = 1;
            ok = 1;
            if (lbl_8087F610->m540 != 2) {
                lbl_8087F59C->mC4 = 3;
            }
        } else if ((unsigned int)mAC <= mFC.size()) {
            const wchar_t* text = fn_800A4450(FindPane_unk(m50[mAC - 1]->mLayout, "string0027(1)"), 0, 0).data();
            int members = fn_804DC7C0(lbl_8087F610, mFC[m94 + mAC - 1]);
            int capacity = fn_804DC80C(lbl_8087F610, mFC[m94 + mAC - 1]);
            if (wcscmp(text, L"") != 0 && members < capacity) {
                lbl_8087F628->m90 = 0;
                ok = 1;
                lbl_8087F628->mC214 = mFC[m94 + mAC - 1];
                if (lbl_8087F610->m540 != 2) {
                    lbl_8087F59C->mC4 = 3;
                }
            }
        }
        if (ok == 1) {
            fn_804B4150(lbl_8087F59C, lbl_8087F59C->mC4);
            fn_804DB474(lbl_8087F610);
            fn_804BA350(8);
        }
    } else if (decide == 2) {
        mA0 = 1;
        fn_804BA350(4);
        fn_804DC450(lbl_8087F610, 0);
    }
    UpdateScrollBar_unk();
}

void D2AMpMenuFriendRoom::vf14() {
    for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
        if (*it != 0) {
            (*it)->mFlags |= 4;
        }
    }
    switch (m88) {
    case 1:
    case 2:
        fn_804BB10C();
        break;
    case 4:
    case 5:
        vf20();
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        m80->mFlags &= ~4;
        SetPaneValue2_unk(m4C->mLayout, "col0005", 0, 128.0f);
        break;
    case 8:
        m84->mFlags &= ~4;
        if (lbl_8087F628->mC1F8 == 8 || lbl_8087F628->mC1F4 == 1) {
            if (lbl_8087F610->m99EC <= 0) {
                m7C->mFlags &= ~4;
            }
        }
        break;
    }
}

void D2AMpMenuFriendRoom::fn_804BB10C() {
    if (MenuCommonTask::sInstance != 0) {
        if (D2ACommonDialog::sInstance->m4C == 0) {
            char name[0x40];
            sprintf(name, "cursor_%04d", mAC + 1);
            MpRect_unk rect = GetPaneRect_unk(m4C, name);
            fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, 0);
        }
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        for (int i = 0; i < 10; i++) {
            m50[i]->mFlags &= ~4;
        }
        m80->mFlags &= ~4;
        m78->mFlags &= ~4;
        SetPaneValue2_unk(m4C->mLayout, "col0005", 0, 128.0f);
        fn_804A4738(m80, m94, mFC.size(), 10);
    }
}

// NONMATCHING: 94.57% - inlined vector<int>::push_back and the loop registers
void D2AMpMenuFriendRoom::fn_804BB2CC(int refresh) {
    if (refresh != 0) {
        mFC.clear();
        MpRoomList_unk* list = &lbl_8087F628->m430;
        int room = fn_8050F5A8(list, 0);
        while (room != -1) {
            if (fn_8050F868(list, room) != 0 && fn_804DC114(lbl_8087F610, room) != -1) {
                mFC.push_back(room);
            }
            room = fn_8050F664(list, 0);
        }
        mB4 = 0;
        for (unsigned int i = 0; i < mFC.size(); i++) {
            if (fn_804DC114(lbl_8087F610, mFC[i]) >= 0) {
                mB4++;
            }
        }
        if (mB4 > 10) {
            if (m94 + 10 > mB4) {
                int n = mB4 - 10;
                m94 = n > 0 ? n : 0;
            }
        } else {
            m94 = 0;
        }
        if (mAC > 0 && m94 + mAC - 1 >= mB4) {
            int n = mB4 - m94;
            mAC = n > 0 ? n : 0;
        }
    }
    int nulls[10] = {12, 20, 25, 30, 35, 40, 45, 50, 55, 61};
    char name[0x40] = {0};
    MpRect_unk rect;
    MpRoomList_unk* list = &lbl_8087F628->m430;
    for (int i = 0; i < 10; i++) {
        unsigned int index = i + m94;
        sprintf(name, "null_00%02d(1)", nulls[i]);
        rect = GetPaneRect_unk(m4C, name);
        fn_801F4728(m50[i], "pos0030(1)(1)", &rect);
        if (index < mFC.size()) {
            int state = fn_804DC114(lbl_8087F610, mFC[index]);
            float frame = 10.0f;
            if (state >= 0) {
                fn_8050F764(list, mFC[index]);
                SetPaneText_unk(m50[i]->mLayout, "string0028(1)", fn_8050F724(list, mFC[index]));
                int msg;
                if (state == 1) {
                    if (fn_804DC7C0(lbl_8087F610, mFC[index]) >= fn_804DC80C(lbl_8087F610, mFC[index])) {
                        msg = 0x123;
                    } else {
                        frame = 12.0f;
                        msg = 0x11C;
                    }
                } else {
                    msg = state == 5 ? 0x123 : state + 0x11B;
                }
                SetPaneText_unk(m50[i]->mLayout, "string0006(1)", lbl_8087F86C->Get_unk(msg));
            } else {
                SetPaneText_unk(m50[i]->mLayout, "string0028(1)", L"");
                SetPaneText_unk(m50[i]->mLayout, "string0006(1)", L"");
            }
            m50[i]->mSpeed = 0.0f;
            m50[i]->mFrame = frame;
            if (state == 1) {
                wchar_t text[16];
                fn_800DD3FC(text, L"%d", fn_804DC7C0(lbl_8087F610, mFC[index]));
                SetPaneText_unk(m50[i]->mLayout, "string0027(1)", text);
                fn_800DD3FC(text, L"/");
                SetPaneText_unk(m50[i]->mLayout, "string0010(1)", text);
                fn_800DD3FC(text, L"%d", fn_804DC80C(lbl_8087F610, mFC[index]));
                SetPaneText_unk(m50[i]->mLayout, "string0011(1)", text);
            } else {
                SetPaneText_unk(m50[i]->mLayout, "string0027(1)", L"");
                SetPaneText_unk(m50[i]->mLayout, "string0010(1)", L"");
                SetPaneText_unk(m50[i]->mLayout, "string0011(1)", L"");
            }
        } else {
            SetPaneText_unk(m50[i]->mLayout, "string0028(1)", L"");
            // inferred: the original passes the address of "" as the text id
            m50[i]->SetText_unk("string0006(1)", (int)"");
            SetPaneText_unk(m50[i]->mLayout, "string0027(1)", L"");
            SetPaneText_unk(m50[i]->mLayout, "string0010(1)", L"");
            SetPaneText_unk(m50[i]->mLayout, "string0011(1)", L"");
            m50[i]->mSpeed = 0.0f;
            m50[i]->mFrame = 0.0f;
        }
    }
}

} // namespace MP
