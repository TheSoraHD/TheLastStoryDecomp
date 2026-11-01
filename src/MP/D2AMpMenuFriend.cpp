#include "MP/D2AMpMenu_unk.h"
#include "MP/D2ACommonDialog.h"
#include "game/MenuCommonTask.h"
#include "std/vector.h"
#include "game/Input_unk.h"

// inferred: multiplayer session manager (lbl_8087F610); only the members used here
struct MpSession_unk {
    unsigned char m000[0x4FC];
    int m4FC;
    unsigned char m500[0x514 - 0x500];
    void* m514; // inferred: friend manager
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" void fn_804DC858(MpSession_unk* s); // inferred: requests the list
extern "C" int fn_804DCA4C(MpSession_unk* s, int id, int arg);
extern "C" int fn_80502848(void* mgr);

// inferred: friend list (lbl_8087F628 + 0x430)
struct MpFriendList_unk {
    unsigned char m000[0x1C0];
    wchar_t m1C0[(0x9F8 - 0x1C0) / 2]; // inferred: own name
    unsigned long long m9F8;           // inferred: own friend code
    unsigned char mA00[0xA04 - 0xA00];
    int mA04;                          // inferred: result of the registration
};
extern "C" int fn_8050F5A8(MpFriendList_unk* list, int arg);  // inferred: first friend
extern "C" int fn_8050F664(MpFriendList_unk* list, int arg);  // inferred: next friend
extern "C" int fn_8050F868(MpFriendList_unk* list, int id);   // inferred: nonzero if registered
extern "C" unsigned long long fn_8050F764(MpFriendList_unk* list, int id); // inferred: friend code
extern "C" const wchar_t* fn_8050F724(MpFriendList_unk* list, int id);     // inferred: name
extern "C" int fn_8050F7D8(MpFriendList_unk* list, int id);   // inferred: state
extern "C" int fn_8050F8A0(MpFriendList_unk* list, int id);
extern "C" int fn_8050F404(MpFriendList_unk* list);           // inferred: free slot (-1: none)
extern "C" void fn_8050EBC8(MpFriendList_unk* list, unsigned long long code, int arg); // inferred: adds
extern "C" int fn_8050EF14(MpFriendList_unk* list, int id, int arg); // inferred: removes
extern "C" void fn_8050EAE8(MpFriendList_unk* list);
extern "C" void fn_804F7EF0(MpFriendList_unk* list, int size); // inferred: saves
extern "C" std::basic_string<wchar_t> fn_8050F470(unsigned long long code); // inferred: code as text

// inferred: multiplayer data (lbl_8087F628); only the members used here
struct MpData_unk {
    unsigned char m000[0xC8];
    int mC8; // inferred: number of friends
    unsigned char mCC[0x430 - 0xCC];
    MpFriendList_unk m430;
};
extern "C" MpData_unk* lbl_8087F628;

// inferred: game settings (lbl_8087F0A8): help/button display at 0x48C
struct MpSettings_unk {
    unsigned char m000[0x48C];
    unsigned char mPad[1]; // 0x48C
};
extern "C" MpSettings_unk* lbl_8087F0A8;

extern "C" void fn_804A436C(int* cursor, int count, int arg2, int wrap, int sound);
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c);
extern "C" void fn_804A4738(MP::MpD2A_unk* bar, int top, int count, int visible); // inferred: scroll bar
extern "C" void fn_801FECE0(void* layout, unsigned int hash, float value);
extern "C" void itow(int value, wchar_t* buf, int radix);
extern "C" void fn_800D2338(atn::Task* task);                    // inferred: removes the task

inline void* GetPad_unk() { return lbl_8087EF70; }
inline void SetPaneValue3_unk(void* layout, const char* name, float value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}

namespace MP {

// inferred: ids of the friends (a class derived from std::vector: its destructor tests `this` four times)
struct MpFriendIds_unk : public std::vector<int> {};

// inferred from __vt__Q22MP15D2AMpMenuFriend (0x80790BC8): friend list
class D2AMpMenuFriend : public atn::Task {
public:
    static D2AMpMenuFriend* fn_804B6988(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuFriend(atn::Task* parent);
    virtual ~D2AMpMenuFriend();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804B6F08(int state); // inferred: changes the state
    void fn_804B76F4();          // inferred: leaves the previous state
    void fn_804B79F4();          // inferred: input of the list
    void fn_804B7F88();          // inferred: deletion of a friend
    void fn_804B83F4();          // inferred: input of a friend code
    void fn_804B8C54();          // inferred: draw of the list
    void fn_804B8E14();          // inferred: draw of the closing list
    void fn_804B9014();          // inferred: sets the list texts
    void fn_804B9694();          // inferred: sets the friend states
    int fn_804B988C(int id);     // inferred: message id of the state of a friend
    void fn_804B9960();          // inferred: sets the digits of the code
    unsigned int fn_804B9B68(wchar_t* text); // inferred: deletes the selected friend
    void fn_804B9D40();          // inferred: closes the task
    void UpdateScrollBar_unk();  // inferred: not inline (its literals are in @stringBase0)

    void Add_unk(MpD2A_unk* task) {
        if (mTasks.size() < 0x20) {
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
    void Rebuild_unk() {
        mIds.__shrink_unk(0); // inferred: the original computes size - size (erase of everything)
        int id;
        MpFriendList_unk* list = &lbl_8087F628->m430;
        id = fn_8050F5A8(list, 0);
        while (id != -1) {
            mIds.push_back(id);
            id = fn_8050F664(list, 0);
        }
    }

    wchar_t m48[0x100];  // inferred: message of the deletion
    unsigned char m248;
    MpD2A_unk* m24C;     // inferred: online_friendlist_logo.d2a
    MpD2A_unk* m250;     // inferred: online_friendlist_base.d2a
    MpD2A_unk* m254;     // inferred: online_friendlist_list.d2a (own code)
    MpD2A_unk* m258[10]; // inferred: online_friendlist_list.d2a
    MpD2A_unk* m280;     // inferred: online_friendlist_input_base.d2a
    MpD2A_unk* m284;     // inferred: online_friendlist_input2.d2a
    MpD2A_unk* m288[12]; // inferred: online_friendlist_input1.d2a
    MpD2A_unk* m2B8;     // inferred: scroll_bar.d2a
    int m2BC;            // inferred: state
    int m2C0;            // inferred: previous state
    int m2C4;
    int m2C8;            // inferred: first row shown
    int m2CC[2];         // inferred: fade of the arrows
    int m2D4;
    int m2D8;
    int m2DC;
    int m2E0;            // inferred: cursor
    int m2E4;            // inferred: digit cursor
    int m2E8[12];        // inferred: digits of the code
    int m318;            // inferred: row to delete
    int m31C;            // inferred: refresh timer
    int m320;
    atn::FixedArray_unk<MpD2A_unk*, 0x20> mTasks; // 0x324
    MpFriendIds_unk mIds; // 0x3A8
    int m3B4;

    static D2AMpMenuFriend* sInstance;
};

D2AMpMenuFriend* D2AMpMenuFriend::sInstance;

D2AMpMenuFriend* D2AMpMenuFriend::fn_804B6988(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) D2AMpMenuFriend(parent);
    }
    return sInstance;
}

D2AMpMenuFriend::D2AMpMenuFriend(atn::Task* parent)
    : atn::Task(parent), m248(0), m2BC(0), m2C0(0), m2C8(0), m2D4(0), m2D8(0), m2DC(0), m2E0(0), m318(-1),
      m31C(0) {
    m2CC[0] = -1;
    m2CC[1] = -1;
    m24C = fn_801F3FF8(this, "/data/d2anime/online_friendlist_logo.d2a", 0);
    Add_unk(m24C);
    m250 = fn_801F3FF8(this, "/data/d2anime/online_friendlist_base.d2a", 0);
    Add_unk(m250);
    m254 = fn_801F3FF8(this, "/data/d2anime/online_friendlist_list.d2a", 0);
    Add_unk(m254);
    for (int i = 0; i < 10; i++) {
        m258[i] = fn_801F3FF8(this, "/data/d2anime/online_friendlist_list.d2a", 0);
        Add_unk(m258[i]);
    }
    m280 = fn_801F3FF8(this, "/data/d2anime/online_friendlist_input_base.d2a", 0);
    Add_unk(m280);
    m284 = fn_801F3FF8(this, "/data/d2anime/online_friendlist_input2.d2a", 0);
    Add_unk(m284);
    for (int i = 0; i < 12; i++) {
        m288[i] = fn_801F3FF8(this, "/data/d2anime/online_friendlist_input1.d2a", 0);
        Add_unk(m288[i]);
        m2E8[i] = 0;
    }
    m2B8 = fn_801F3FF8(this, "/data/d2anime/scroll_bar.d2a", 0);
    Add_unk(m2B8);
    SetAll_unk(1);
}

D2AMpMenuFriend::~D2AMpMenuFriend() {
    mTasks.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int D2AMpMenuFriend::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetAll_unk(0);
        m2B8->mFlags |= 4;
        fn_804B6F08(1);
        if (lbl_8087F628->mC8 >= 8) {
            SetTaskText_unk(m250, "string0001(1)", L"");
            SetPaneText_unk(m250->mLayout, "string0002(1)", L"");
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.88% - inlined vector<int>::push_back (the original loads the size before the
// capacity, open problem of std/vector.h)
void D2AMpMenuFriend::fn_804B6F08(int state) {
    if (m2BC != state && state >= 0) {
        int cursor = m2E0;
        m2C0 = m2BC;
        m2E0 = 0;
        m2BC = state;
        fn_804B76F4();
        switch (m2BC) {
        case 1:
            vf1C();
            Rebuild_unk();
            fn_804B9014();
            if (lbl_8087F628->m430.m9F8 == 0 && m2C0 != 4) {
                D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
                D2ACommonDialog::sInstance->fn_804ACD10();
                D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xF7));
            }
            if (lbl_8087F628->mC8 >= 8) {
                SetPaneValue2_unk(m250->mLayout, "col0068", 1, 153.0f);
                SetPaneValue2_unk(m250->mLayout, "col0068", 2, 153.0f);
                SetPaneValue2_unk(m250->mLayout, "col0068", 3, 153.0f);
            }
            Play_unk(m24C, 0, 1.0f);
            Play_unk(m250, 0, 1.0f);
            Play_unk(m254, 0, 1.0f);
            for (int i = 0; i < 10; i++) {
                Play_unk(m258[i], 0, 1.0f);
            }
            break;
        case 2:
            m2E0 = cursor;
            Play_unk(m2B8, 0, 1.0f);
            break;
        case 3: {
            m248 = 0;
            D2ACommonDialog::sInstance->fn_804AC96C(0, 1.0f, 1, 0);
            D2ACommonDialog::sInstance->fn_804ACD10();
            D2ACommonDialog::sInstance->mFlags |= 4;
            m2E0 = cursor;
            m318 = cursor;
            wchar_t text[0x100] = {0};
            int index = m318 + m2C8 - 1;
            MpFriendList_unk* list = &lbl_8087F628->m430;
            const wchar_t* name;
            if (fn_8050F868(list, mIds[index]) != 0) {
                const wchar_t* str = fn_8050F724(list, mIds[index]);
                name = str[0] == 0 ? lbl_8087F86C->Get_unk(0x122) : str;
            } else {
                name = lbl_8087F86C->Get_unk(0x120);
            }
            fn_800DD3FC(text, lbl_8087F86C->Get_unk(0xEE), name);
            D2ACommonDialog::sInstance->fn_804AD000(text, 0);
            break;
        }
        case 4:
            m248 = 0;
            m2E4 = 0;
            m2CC[0] = -1;
            m2CC[1] = -1;
            SetPaneValue2_unk(m284->mLayout, "col0002(1)", 0, 0.0f);
            SetPaneValue2_unk(m284->mLayout, "col0001(1)", 0, 0.0f);
            Play_unk(m24C, 0, 1.0f);
            Play_unk(m280, 0, 1.0f);
            Play_unk(m284, 0, 1.0f);
            for (int i = 0; i < 12; i++) {
                Play_unk(m288[i], 0, 1.0f);
                m2E8[i] = 0;
            }
            Stop_unk(m2B8);
            fn_804B9960();
            break;
        case 5:
            fn_804B9694();
            m320 = 10;
            vf20();
            break;
        }
    }
}

void D2AMpMenuFriend::vf1C() {}

void D2AMpMenuFriend::vf20() {}

void D2AMpMenuFriend::fn_804B76F4() {
    switch (m2C0) {
    case 2:
        if (m2BC != 3) {
            Play_unk(m24C, 0, -1.0f);
            Play_unk(m250, 0, -1.0f);
            Play_unk(m254, 0, -1.0f);
            for (int i = 0; i < 10; i++) {
                Play_unk(m258[i], 0, -1.0f);
            }
        }
        break;
    case 3:
        D2ACommonDialog::sInstance->fn_804ACAF8();
        D2ACommonDialog::sInstance->fn_804ACDBC();
        break;
    case 4:
        m2E4 = 0;
        Play_unk(m24C, 0, -1.0f);
        Play_unk(m280, 0, -1.0f);
        Play_unk(m284, 0, -1.0f);
        for (int i = 0; i < 12; i++) {
            Play_unk(m288[i], 0, -1.0f);
            m2E8[i] = 0;
        }
        break;
    }
}

void D2AMpMenuFriend::vf10() {
    if (m2D4 != 0) {
        m2D4 = 0;
        return;
    }
    switch (m2BC) {
    case 1:
        fn_804B6F08(2);
        m254->mSpeed = 0.0f;
        fn_804B9694();
        m254->mFrame = 10.0f;
        break;
    case 2:
        fn_804B79F4();
        break;
    case 3:
        fn_804B7F88();
        break;
    case 4:
        fn_804B83F4();
        break;
    case 5:
        if (m250->mFrame <= 0.0f) {
            fn_804B6F08(6);
        }
        break;
    }
}

void D2AMpMenuFriend::UpdateScrollBar_unk() {
    MpRect_unk rect;
    MpRect_unk pos = GetPaneRect_unk(m250, "sb_null_0001");
    rect.x = pos.x;
    rect.y = pos.y;
    rect.w = pos.w;
    rect.z = pos.z;
    rect.h = 1.0f;
    fn_801F4728(m2B8, "basePos", &rect);
    void* layout = m2B8->mLayout;
    SetPaneValue3_unk(layout, "s_hightMax", 197.0f);
}

// NONMATCHING: 98.71% - decide/pad registers and the stack slots of the sound temporaries and of the
// inlined UpdateScrollBar_unk (as D2AMpMenuFriendRoom::fn_804BA970)
void D2AMpMenuFriend::fn_804B79F4() {
    if (m31C > 0) {
        m31C--;
    } else {
        fn_804DC858(lbl_8087F610);
        fn_804B9014();
        m31C = 150;
    }
    if (D2ACommonDialog::sInstance->m4C != 0) {
        if (D2ACommonDialog::sInstance->fn_804AC734() != 0) {
            D2ACommonDialog::sInstance->fn_804ACAF8();
            D2ACommonDialog::sInstance->fn_804ACDBC();
        }
        return;
    }
    fn_804B9694();
    int decide = 0;
    void* pad;
    int cursor = m2E0;
    int top = m2C8;
    {
        pad = GetPad_unk();
        fn_804A436C(&m2E0, 0, 0, 1, 3);
        if (fn_800A555C(pad, 0, 4) != 0) {
            fn_80117228(1);
            decide = 1;
        } else if (fn_800A555C(pad, 0, 5) != 0) {
            fn_80117228(2);
            decide = 2;
        }
    }
    pad = GetPad_unk();
    if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
        int t = m2C8;
        if (--m2E0, t != 0) {
            if (m2E0 < 1) {
                m2E0 = 1;
                m2C8 = t - 1;
                fn_804B9014();
            }
        } else if (m2E0 < 0) {
            m2E0 = 0;
        }
    }
    if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
        if (m2E0 > 0) {
            int t = m2C8;
            int n = t - 10;
            n = n > 0 ? n : 0;
            m2C8 = n;
            if (n != t) {
                int c = m2E0 - 10 - (t - n);
                m2E0 = c > 1 ? c : 1;
            } else {
                m2E0 = 1;
            }
            fn_804B9014();
        }
    } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
        int c = ++m2E0;
        if (c > 10) {
            int t = m2C8;
            if (t + c < mIds.size() + 1) {
                m2C8 = t + 1;
                fn_804B9014();
            }
            m2E0 = 10;
        } else if (c >= mIds.size()) {
            m2E0 = mIds.size();
        }
    }
    if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
        if (m2E0 > 0) {
            if (m2C8 >= (int)(mIds.size() - 10)) {
                m2E0 = mIds.size() < 10 ? mIds.size() : 10;
            }
            if (mIds.size() > 10) {
                int t = m2C8;
                unsigned int n = mIds.size() - 10;
                if (t + 10 < n) {
                    n = t + 10;
                }
                m2C8 = n;
                if (n != t) {
                    int c = m2E0 + 10 - (n - t);
                    m2E0 = c < 10 ? c : 10;
                } else {
                    m2E0 = 10;
                }
            }
            fn_804B9014();
        }
    }
    if (m2E0 != cursor || top != m2C8) {
        fn_80117228(3);
    }
    if (decide == 1) {
        if (m2E0 == 0) {
            if (lbl_8087F628->mC8 >= 8) {
                D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
                D2ACommonDialog::sInstance->fn_804ACD10();
                D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xF6));
            } else if (fn_8050F404(&lbl_8087F628->m430) == -1) {
                D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
                D2ACommonDialog::sInstance->fn_804ACD10();
                D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xEA));
            } else {
                fn_804B6F08(4);
            }
        }
    } else if (decide == 2) {
        fn_804B6F08(5);
    }
    if (((Input_unk*)lbl_8087F0A8->mPad)->fn_801231D0(0x34) != 0 && m2E0 != 0 && lbl_8087F628->mC8 < 8) {
        fn_80117228(1);
        fn_804B6F08(3);
    }
    UpdateScrollBar_unk();
}

// NONMATCHING: 99.89% - inlined vector<int>::push_back (as fn_804B6F08)
void D2AMpMenuFriend::fn_804B7F88() {
    if (m248 == 0) {
        if (D2ACommonDialog::sInstance->m4C != 0) {
            if (D2ACommonDialog::sInstance->fn_804AC79C() != 0) {
                if (fn_804B9B68(m48) == 1) {
                    m248 = 1;
                } else {
                    fn_804B6F08(2);
                }
                Rebuild_unk();
                if (m2C8 > 0 && m2C8 + 10 >= mIds.size()) {
                    unsigned int n = mIds.size() - 10;
                    m2C8 = n > 0 ? n : 0;
                }
                fn_804B9014();
                fn_804A4738(m2B8, m2C8, mIds.size(), 10);
                if (mIds.size() == 0) {
                    m2E0 = 0;
                } else if (m2E0 > mIds.size()) {
                    m2E0 = mIds.size();
                }
            } else if (D2ACommonDialog::sInstance->fn_804AC7EC() != 0) {
                fn_804B6F08(2);
                m2E0 = m318;
                m318 = -1;
            }
        }
    } else if (m48[0] == 0 || lbl_8087F610->m514 == 0 || fn_80502848(lbl_8087F610->m514) == 0) {
        fn_80117228(12);
        fn_804B6F08(2);
        if (m48[0] != 0) {
            D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
            D2ACommonDialog::sInstance->fn_804ACD10();
            D2ACommonDialog::sInstance->fn_804AD1EC(m48);
        }
        m248 = 0;
    }
}

// NONMATCHING: 99.94% - register of `result` (r28 in the original; tried it declared at the top and
// without the list local)
void D2AMpMenuFriend::fn_804B83F4() {
    if (D2ACommonDialog::sInstance->m4C != 0) {
        if (D2ACommonDialog::sInstance->fn_804AC734() != 0) {
            D2ACommonDialog::sInstance->fn_804ACAF8();
            D2ACommonDialog::sInstance->fn_804ACDBC();
            if (lbl_8087F628->m430.mA04 == 0) {
                fn_804B6F08(1);
            }
        }
        return;
    }
    if (m248 == 0) {
        void* pad = GetPad_unk();
        int cursor = m2E4;
        if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
            m2E8[m2E4]++;
            if (m2E8[m2E4] > 9) {
                m2E8[m2E4] = 0;
            }
            m2CC[0] = 0;
            fn_80117228(3);
        } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
            m2E8[m2E4]--;
            if (m2E8[m2E4] < 0) {
                m2E8[m2E4] = 9;
            }
            m2CC[1] = 0;
            fn_80117228(3);
        } else if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
            if (--cursor < 0) {
                cursor = 11;
            }
        } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
            if (++cursor >= 12) {
                cursor = 0;
            }
        }
        if (cursor != m2E4) {
            fn_80117228(3);
            m2E4 = cursor;
        }
        if (fn_800A555C(pad, 0, 4) != 0) {
            unsigned long long code = 0;
            for (int i = 0; i < 12; i++) {
                code = code * 10 + m2E8[i];
            }
            fn_8050EBC8(&lbl_8087F628->m430, code, 0);
            MpFriendList_unk* list = &lbl_8087F628->m430;
            int result = list->mA04;
            if (result == 0) {
                fn_8050EAE8(list);
                fn_804F7EF0(&lbl_8087F628->m430, 0x9F0);
                m248 = 1;
                fn_804B9694();
                fn_80117228(1);
            } else {
                D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
                D2ACommonDialog::sInstance->fn_804ACD10();
                if (result == 3) {
                    D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xEC));
                    fn_80117228(11);
                } else if (result == 1) {
                    D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xED));
                    fn_80117228(11);
                } else if (result == 2) {
                    D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xF5));
                    fn_80117228(11);
                }
            }
        } else if (fn_800A555C(pad, 0, 5) != 0) {
            for (int i = 0; i < 12; i++) {
                m2E8[i] = 0;
            }
            fn_80117228(2);
            fn_804B6F08(1);
        }
        fn_804B9960();
    } else if (lbl_8087F610->m514 == 0 || fn_80502848(lbl_8087F610->m514) == 0) {
        D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
        D2ACommonDialog::sInstance->fn_804ACD10();
        D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xEB));
        fn_80117228(12);
        m248 = 0;
    }
    for (int i = 0; i < 2; i++) {
        if (m2CC[i] != -1) {
            int c;
            if (m2CC[i] <= 5) {
                c = m2CC[i] * 0x33;
            } else {
                c = 0xFF - (m2CC[i] - 5) * 0x33;
            }
            float value = c;
            if (i == 0) {
                SetPaneValue2_unk(m284->mLayout, "col0002(1)", 0, value);
            } else if (i == 1) {
                SetPaneValue2_unk(m284->mLayout, "col0001(1)", 0, value);
            }
            if (++m2CC[i] > 10) {
                m2CC[i] = -1;
            }
        }
    }
}

void D2AMpMenuFriend::vf14() {
    for (unsigned int i = 0; i < mTasks.size(); i++) {
        mTasks[i]->mFlags |= 4;
    }
    switch (m2BC) {
    case 1:
    case 2:
        fn_804B8C54();
        break;
    case 3:
        fn_804B8C54();
        D2ACommonDialog::sInstance->mFlags &= ~4;
        break;
    case 4:
        m24C->mFlags &= ~4;
        m280->mFlags &= ~4;
        for (int i = 0; i < 12; i++) {
            m288[i]->mFlags &= ~4;
        }
        if (D2ACommonDialog::sInstance->m4C == 0) {
            m284->mFlags &= ~4;
        }
        break;
    case 5:
    case 6:
        vf20();
        fn_804B8E14();
        break;
    }
}

void D2AMpMenuFriend::fn_804B8C54() {
    if (MenuCommonTask::sInstance != 0) {
        if (D2ACommonDialog::sInstance->m4C == 0) {
            char name[0x40];
            sprintf(name, "cursor_%04d", m2E0 + 1);
            MpRect_unk rect = GetPaneRect_unk(m250, name);
            fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, 0);
        }
        m24C->mFlags &= ~4;
        m250->mFlags &= ~4;
        m254->mFlags &= ~4;
        for (int i = 0; i < 10; i++) {
            m258[i]->mFlags &= ~4;
        }
        m2B8->mFlags &= ~4;
        SetPaneValue2_unk(m250->mLayout, "col0005", 0, 128.0f);
        fn_804A4738(m2B8, m2C8, mIds.size(), 10);
    }
}

void D2AMpMenuFriend::fn_804B8E14() {
    m24C->mFlags &= ~4;
    m250->mFlags &= ~4;
    m254->mFlags &= ~4;
    for (int i = 0; i < 10; i++) {
        m258[i]->mFlags &= ~4;
    }
    SetPaneValue2_unk(m250->mLayout, "col0005", 0, 128.0f);
    m320--;
    for (int i = 0; i < 10; i++) {
        int frame = m258[i]->mFrame;
        if (frame == 12) {
            frame = 10;
        }
        if (frame > m320 && frame > 0) {
            m258[i]->mFrame = frame - 1;
        }
    }
}

// NONMATCHING: 97.70% - register allocation (the original uses one register less before the loop:
// this in r17)
void D2AMpMenuFriend::fn_804B9014() {
    char name[0x40] = {0};
    int nulls[10] = {12, 20, 25, 30, 35, 40, 45, 50, 55, 61};
    MpRect_unk rect;
    MpFriendList_unk* list = &lbl_8087F628->m430;
    rect = GetPaneRect_unk(m250, "basePoint");
    fn_801F4728(m254, "pos0030(1)", &rect);
    SetPaneText_unk(m254->mLayout, "string0028(1)", list->m1C0);
    unsigned long long code = list->m9F8;
    if (code != 0) {
        m254->SetNumber_unk("code_first", code % 10000, -4);
        m254->SetNumber_unk("code_second", (code / 10000) % 10000, -4);
        m254->SetNumber_unk("code_third", (code / 10000 / 10000) % 10000, -4);
        SetPaneValue3_unk(m254->mLayout, "showError", 0.0f);
    } else {
        SetPaneValue3_unk(m254->mLayout, "showError", 1.0f);
    }
    const wchar_t* text;
    if (lbl_8087F610->m4FC == 7) {
        text = lbl_8087F86C->Get_unk(0x11B);
    } else {
        text = lbl_8087F86C->Get_unk(0x11F);
    }
    SetPaneText_unk(m254->mLayout, "string0027(1)", text);
    for (int i = 0; i < 10; i++) {
        unsigned int index = i + m2C8;
        sprintf(name, "null_00%02d(1)", nulls[i]);
        rect = GetPaneRect_unk(m250, name);
        fn_801F4728(m258[i], "pos0030(1)", &rect);
        if (index < mIds.size()) {
            unsigned long long fc = fn_8050F764(list, mIds[index]);
            if (fn_8050F868(list, mIds[index]) == 0) {
                SetPaneText_unk(m258[i]->mLayout, "string0028(1)", lbl_8087F86C->Get_unk(0x120));
            } else {
                const wchar_t* str = fn_8050F724(list, mIds[index]);
                SetPaneText_unk(m258[i]->mLayout, "string0028(1)", str[0] == 0 ? lbl_8087F86C->Get_unk(0x122) : str);
            }
            SetPaneValue3_unk(m258[i]->mLayout, "showList", 1.0f);
            m258[i]->SetNumber_unk("code_first", fc % 10000, -4);
            m258[i]->SetNumber_unk("code_second", (fc / 10000) % 10000, -4);
            m258[i]->SetNumber_unk("code_third", (fc / 10000 / 10000) % 10000, -4);
            SetPaneValue3_unk(m258[i]->mLayout, "showError", 0.0f);
            if (lbl_8087F628->mC8 >= 8) {
                SetPaneText_unk(m258[i]->mLayout, "string0027(1)", lbl_8087F86C->Get_unk(fn_804B988C(mIds[index])));
            } else {
                SetPaneText_unk(m258[i]->mLayout, "string0027(1)", L"");
            }
        } else {
            SetPaneValue3_unk(m258[i]->mLayout, "showList", 0.0f);
            SetPaneText_unk(m258[i]->mLayout, "code_first", L"");
            SetPaneText_unk(m258[i]->mLayout, "code_second", L"");
            SetPaneText_unk(m258[i]->mLayout, "code_third", L"");
            SetPaneText_unk(m258[i]->mLayout, "string0028(1)", L"");
            SetPaneText_unk(m258[i]->mLayout, "string0027(1)", L"");
            m258[i]->mSpeed = 0.0f;
            m258[i]->mFrame = 0.0f;
        }
    }
}

// NONMATCHING: 99.80% - index/layout registers of the L"" text
void D2AMpMenuFriend::fn_804B9694() {
    char name[0x40] = {0};
    MpFriendList_unk* list = &lbl_8087F628->m430;
    for (int i = 0; i < 10; i++) {
        unsigned int index = i + m2C8;
        if (index >= mIds.size()) {
            break;
        }
        fn_8050F7D8(list, mIds[index]);
        int msg = fn_804B988C(mIds[index]);
        Play_unk(m258[i], 1, 0.0f);
        if (lbl_8087F628->mC8 >= 8) {
            const wchar_t* text = lbl_8087F86C->Get_unk(msg);
            void* layout = m258[i]->mLayout;
            fn_801FEE08(layout, fn_800DC6B4("string0027(1)"), text);
        } else {
            SetPaneText_unk(m258[i]->mLayout, "string0027(1)", L"");
        }
        if (msg == 0x11B) {
            m258[i]->mFrame = 3.0f;
        } else if (msg == 0x11C) {
            m258[i]->mFrame = 12.0f;
        } else {
            m258[i]->mFrame = 10.0f;
        }
    }
}

// inferred: the original calls this function instead of inlining it; the empty statements (release
// asserts) put it over the auto-inline threshold
int D2AMpMenuFriend::fn_804B988C(int id) {
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    ((void)0);
    MpFriendList_unk* list = &lbl_8087F628->m430;
    int state = fn_8050F7D8(list, id);
    if (state == 0) {
        return 0x11B;
    }
    if (fn_8050F8A0(list, id) != 0) {
        return 0x11B;
    }
    if (state == 4 || fn_804DCA4C(lbl_8087F610, id, 1) != 0 || fn_804DCA4C(lbl_8087F610, id, 0) != 0 ||
        fn_804DCA4C(lbl_8087F610, id, 2) != 0) {
        return 0x11C;
    }
    return 0x121;
}

void D2AMpMenuFriend::fn_804B9960() {
    char name[0x40] = {0};
    wchar_t text[0x40] = {0};
    MpRect_unk rect;
    int nulls[12] = {3, 13, 18, 23, 28, 33, 38, 43, 48, 53, 58, 65};
    for (int i = 0; i < 12; i++) {
        sprintf(name, "null_00%02d(1)", nulls[i]);
        rect = GetPaneRect_unk(m280, name);
        fn_801F4728(m288[i], "pos0003(1)", &rect);
        itow(m2E8[i], text, 10);
        SetPaneText_unk(m288[i]->mLayout, "num", text);
        if (i == m2E4) {
            fn_801F4728(m284, "pos0003(1)", &rect);
            SetPaneText_unk(m284->mLayout, "num", text);
        }
    }
}

unsigned int D2AMpMenuFriend::fn_804B9B68(wchar_t* text) {
    text[0] = 0;
    if (mIds.size() == 0) {
        return 0;
    }
    if (m318 == -1) {
        return 0;
    }
    unsigned int index = m318 + m2C8 - 1;
    if (mIds.size() < index) {
        return 0;
    }
    const wchar_t* name;
    MpFriendList_unk* list = &lbl_8087F628->m430;
    unsigned long long code = fn_8050F764(list, mIds[index]);
    if (fn_8050F868(list, mIds[index]) != 0) {
        const wchar_t* str = fn_8050F724(list, mIds[index]);
        name = str[0] == 0 ? lbl_8087F86C->Get_unk(0x122) : str;
    } else {
        name = lbl_8087F86C->Get_unk(0x120);
    }
    int removed = fn_8050EF14(list, mIds[index], 0);
    fn_8050EAE8(list);
    fn_804F7EF0(list, 0x9F0);
    {
        std::basic_string<wchar_t> str = fn_8050F470(code);
        const wchar_t* codeText = str.data();
        if (removed != 0) {
            fn_800DD3FC(text, lbl_8087F86C->Get_unk(0xEF), codeText, name);
        } else {
            fn_800DD3FC(text, lbl_8087F86C->Get_unk(0xF0), codeText, name);
        }
    }
    m318 = -1;
    return 1;
}

void D2AMpMenuFriend::fn_804B9D40() {
    fn_800D2338(this);
}

} // namespace MP
