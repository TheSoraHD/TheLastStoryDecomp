// MP/D2AMpMenu.cpp (0x804AE820-0x804B4C50): main multiplayer menu (title, match select, rankings) and
// the rank mark task. Only part of the functions are reconstructed so far.
#include "MP/D2AMpMenu.h"
#include "MP/D2AMpMenuFriendRoom.h"
#include "MP/D2AMpMenuFriend.h"
#include "MP/D2ACommonDialog.h"
#include "MP/D2AMpMenuSetMessage.h"
#include "game/MenuCommonTask.h"

// inferred: setters of the session rules (the last argument: 1 = default)
extern "C" void fn_804EA75C(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EA534(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EAA68(MpMsgSession_unk* s, int value);
extern "C" void fn_804EA6B0(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EA810(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EA608(MpMsgSession_unk* s, int time, int b);
extern "C" void fn_804A5CD8(MenuCommonTask* menu, float y); // inferred: places the help window
extern "C" int fn_804EAA50(MpMsgSession_unk* s); // inferred: nonzero for the host of the room
extern "C" void fn_800D2338(atn::Task* task);                 // inferred: removes the task
extern "C" void fn_804E4E74(MpMsgSession_unk* s);
extern "C" void fn_804A436C(int* cursor, int count, int arg2, int wrap, int sound); // inferred: moves a cursor
// inferred: software keyboard (lbl_8087F860, MP::SoftKeyboard); only the members used here
struct MpMenuKeyboard_unk : public atn::Task {
    unsigned char m048[0xFC - 0x48];
    wchar_t mFC[0x10];  // inferred: text entered
    unsigned char m11C[0x25B8 - 0x11C];
    int m25B8;          // inferred: nonzero when the text was decided
};
extern "C" MpMenuKeyboard_unk* lbl_8087F860;

namespace MP {

// NONMATCHING (__sinit): 99.5% - offset of "CustomData" in @stringBase0 (the unit is not complete)
static const unsigned int sCustomData = fn_800DC6B4("CustomData");
D2AMpMenu* D2AMpMenu::sInstance;
D2AMpMenuRankMark* D2AMpMenuRankMark::sInstance;
const char* D2AMpMenuRankMark::sTablePath = "/database/online/rank_mark_table.csv";

D2AMpMenu* D2AMpMenu::fn_804AE820(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) D2AMpMenu(parent);
    }
    return sInstance;
}

// NONMATCHING: 98.9% - the loop counters get r26 and the offsets r27/r28 (r28 and r26/r27 in the original)
D2AMpMenu::D2AMpMenu(atn::Task* parent)
    : atn::Task(parent), mBC(0), mC4(0), mC8(0), mCC(-1), mD0(0), mD4(0), mD8(0), mDC(0), mE4(0), mE8(-1),
      m154(3) {
    int i;
    MpMenuColor_unk color(0.0f, 0.0f, 0.0f, 0.0f);
    lbl_8087EFA8->m3C = color;
    mLists[0] = &mNear;
    mLists[1] = &mTop;
    mLists[2] = &mFriend;
    m48 = fn_801F3FF8(this, "/data/d2anime/online_title_top_base.d2a", 0);
    Add_unk(m48);
    m4C = fn_801F3FF8(this, "/data/d2anime/online_title_logo.d2a", 0);
    Add_unk(m4C);
    m50 = fn_801F3FF8(this, "/data/d2anime/online_match_select_base.d2a", 0);
    Add_unk(m50);
    m54 = fn_801F3FF8(this, "/data/d2anime/online_match_select_logo.d2a", 0);
    Add_unk(m54);
    for (i = 0; i < sizeof(m58) / sizeof(m58[0]); i++) {
        m58[i] = fn_801F3FF8(this, "/data/d2anime/online_friend_on.d2a", 0);
        Add_unk(m58[i]);
    }
    m64 = fn_801F3FF8(this, "/data/d2anime/online_profile.d2a", 0);
    Add_unk(m64);
    m68 = fn_801F3FF8(this, "/data/d2anime/online_game_select_base.d2a", 0);
    Add_unk(m68);
    m6C = fn_801F3FF8(this, "/data/d2anime/online_game_select_logo.d2a", 0);
    Add_unk(m6C);
    mB8 = fn_801F3FF8(this, "/data/d2anime/online_connect_base.d2a", 0);
    Add_unk(mB8);
    m70 = fn_801F3FF8(this, "/data/d2anime/online_info_back.d2a", 0);
    Add_unk(m70);
    m74 = fn_801F3FF8(this, "/data/d2anime/online_info_cancel.d2a", 0);
    Add_unk(m74);
    m78 = fn_801F3FF8(this, "/data/d2anime/online_side_select.d2a", 0);
    Add_unk(m78);
    m7C = fn_801F3FF8(this, "/data/d2anime/online_load_ranking.d2a", 0);
    Add_unk(m7C);
    m80 = fn_801F3FF8(this, "/data/d2anime/online_ranking_base.d2a", 0);
    Add_unk(m80);
    m84 = fn_801F3FF8(this, "/data/d2anime/online_ranking_logo.d2a", 0);
    Add_unk(m84);
    m88 = fn_801F3FF8(this, "/data/d2anime/online_ranking_select.d2a", 0);
    Add_unk(m88);
    mB4 = fn_801F3FF8(this, "/data/d2anime/scroll_bar.d2a", 0);
    Add_unk(mB4);
    for (i = 0; i < 10; i++) {
        m8C[i] = fn_801F3FF8(this, "/data/d2anime/online_ranking_list.d2a", 0);
        Add_unk(m8C[i]);
    }
    SetAll_unk(1);
    SetSpeedAll_unk(0.0f);
    MenuCommonTask::fn_804A29C4(this);
    if (D2AMpMenuRankMark::sInstance == 0) {
        D2AMpMenuRankMark::sInstance = new ((MemoryTag)1, "", "", 0) D2AMpMenuRankMark(this);
    }
    D2AMpMenuFriendRoom::fn_804B9D44(this);
    for (i = 0; i < 3; i++) {
        RANKING_DATA_LIST* list = mLists[i];
        for (int j = 0; j < list->GetCount(); j++) {
            list->Get(j)->Clear();
        }
    }
    for (i = 0; i < 25; i++) {
        mEC[i] = -1;
    }
    m299C.Clear();
}

// inferred: auto-inlined in the constructor of the menu (the out-of-line copy is dead-stripped)
D2AMpMenuRankMark::D2AMpMenuRankMark(atn::Task* parent) : atn::Task(parent) {
    for (int i = 0; i < 3; i++) {
        mMarks[i] = fn_801F3FF8(this, "/data/d2anime/online_rank_mark.d2a", 0);
        mMarks[i]->fn_800D246C(1);
    }
    mTable.vf0C(sTablePath);
}

D2AMpMenu::~D2AMpMenu() {
    mTasks.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

// NONMATCHING: the original loads the 16.0f of fn_8006EF48 before the data() branch (as D2AMpMenuSetRule::vf0C)
int D2AMpMenu::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetAll_unk(0);
        mD0 = 1;
        fn_804AF520(1);
        SetPaneValue_unk(m70->mLayout, "pos0007", 4, -110.0f);
        SetPaneValue_unk(m70->mLayout, "pos0017", 4, -110.0f);
        void* pane = fn_801FEC74(m70->mLayout, "string0007(1)");
        if (pane != 0) {
            fn_8006EF48(lbl_8087EEC8, fn_800A4450(pane, 0, 0).data(), 16.0f, 0.0f, 1, 1);
        }
        fn_804A5CD8(MenuCommonTask::sInstance, -100.0f);
        D2AMpMenuRankMark::sInstance->fn_804B4770(-1);
        return 1;
    }
    return 0;
}

void D2AMpMenu::vf1C() {}

void D2AMpMenu::vf20() {}

// inferred: closes the windows of the previous state
// NONMATCHING: 99.86% - case 5: the name gets r31 (r29 in the original, as if `this` were still live)
void D2AMpMenu::fn_804B0A78() {
    switch (mC0) {
    case 3:
    case 8:
        if (lbl_8087F610->m4FC != 43 && lbl_8087F610->m4FC != -1) {
            D2ACommonDialog::sInstance->fn_804ACAF8();
            D2ACommonDialog::sInstance->fn_804ACDBC();
        }
        D2AMpMenuRankMark::sInstance->fn_804B4770(-1);
        break;
    case 17:
        Stop_unk(mB8);
        break;
    case 9:
        if (mBC != 8) {
            Stop_unk(m50);
            Stop_unk(m54);
            for (int i = 0; i < sizeof(m58) / sizeof(m58[0]); i++) {
                Stop_unk(m58[i]);
            }
            D2AMpMenuRankMark::sInstance->fn_804B4770(0);
        }
        mC8 = 0;
        break;
    case 12:
        Play_unk(mB8, 1, 1.0f);
        Play_unk(m74, 0, -1.0f);
        break;
    case 4:
        if (D2AMpMenuSetMessage::sInstance != 0) {
            fn_800D2338(D2AMpMenuSetMessage::sInstance);
        }
        break;
    case 5:
        if (lbl_8087F860 != 0) {
            if (lbl_8087F860->m25B8 != 0) {
                lbl_8087F628->m430.SetOwnName_unk(lbl_8087F860->mFC);

            }
            fn_800D2338(lbl_8087F860);
        }
        break;
    case 7:
        mB8->fn_800D246C(1);
        mDC = 1;
        break;
    case 10:
        Stop_unk(m68);
        Stop_unk(m6C);
        for (int i = 0; i < sizeof(m58) / sizeof(m58[0]); i++) {
            Stop_unk(m58[i]);
        }
        break;
    case 6:
        fn_800D2338(D2AMpMenuFriend::sInstance);
        Stop_unk(m70);
        break;
    case 18:
        if (mBC != 17) {
            Stop_unk(m80);
            Stop_unk(m84);
            Stop_unk(mB4);
            Stop_unk(m88);
            Stop_unk(m70);
        }
        for (int i = 0; i < 10; i++) {
            Stop_unk(m8C[i]);
        }
        mDC = 0;
        fn_804E4E74(lbl_8087F610);
        break;
    case 16:
        MenuCommonTask::fn_804A29C4(this);
        MenuCommonTask::sInstance->fn_800D246C(0);
        MenuCommonTask::sInstance->fn_804A2F98();
        break;
    }
}

// inferred: input of the game selection (state 2)
// inferred: moves the cursor and reads the decide/cancel keys (0: none, 1: decide, 2: cancel)
static inline int Decide_unk(int* cursor, int count) {
    void* pad = lbl_8087EF70;
    int decide = 0;
    fn_804A436C(cursor, count, count, 1, 3);
    if (fn_800A555C(pad, 0, 4) != 0) {
        fn_80117228(1);
        decide = 1;
    } else if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80117228(2);
        decide = 2;
    }
    return decide;
}

void D2AMpMenu::fn_804B12F8() {
    if (m299C.m00 != 0) {
        if (m299C.m04 == m299C.m08) {
            m299C.Clear();
        }
        return;
    }
    if (D2ACommonDialog::sInstance->m4C != 0) {
        if (D2ACommonDialog::sInstance->fn_804AC734()) {
            D2ACommonDialog::sInstance->fn_804ACAF8();
            D2ACommonDialog::sInstance->fn_804ACDBC();
            Play_unk(m48, 0, 1.0f);
            Play_unk(m4C, 0, 1.0f);
        } else {
            return;
        }
    }
    if (lbl_8087F628->mC1F4 == 1) {
        int decide = Decide_unk(&mE4, 5);
        if (decide == 1) {
            if (mE4 == 0) {
                fn_804AF520(7);
            } else if (mE4 == 1) {
                fn_804AF520(4);
            } else if (mE4 == 2) {
                fn_804AF520(5);
            } else if (mE4 == 3) {
                fn_804AF520(6);
            } else if (mE4 == 4) {
                fn_804AF520(3);
            }
        } else if (decide == 2) {
            fn_804AF520(3);
        }
    }
}

// inferred: sends the room settings
static inline void SendRoomInfo_unk() {
    if (lbl_8087F628 != 0) {
        MpRoomInfo_unk* info = &lbl_8087F628->mC1;
        if (info != 0) {
            info->mHost = fn_804EAA50(lbl_8087F610);
            info->mMode = lbl_8087F610->m540;
            memcpy(&lbl_8087F628->mC1, info, sizeof(MpRoomInfo_unk));
        }
    }
}

void D2AMpMenu::fn_804B274C() {
    int decide = Decide_unk(&mE4, 3);
    if (decide == 1) {
        int next = 12;
        if (fn_804EAA50(lbl_8087F610) == 1) {
            next = 11;
        }
        if (mE4 == 0) {
            mC4 = 3;
            lbl_8087F610->m540 = 0;
            SendRoomInfo_unk();
            fn_804AF520(next);
        } else if (mE4 == 1) {
            mC4 = 2;
            lbl_8087F610->m540 = 2;
            SendRoomInfo_unk();
            fn_804AF520(next);
        } else if (mE4 == 2) {
            fn_804AF520(9);
        }
    } else if (decide == 2) {
        fn_804AF520(9);
    }
}

// inferred: sets the default rules of a game mode
void D2AMpMenu::fn_804B4150(int state) {
    fn_804EA75C(lbl_8087F610, -1, 1);
    if (state == 3) {
        lbl_8087F610->m540 = 0;
        fn_804EA534(lbl_8087F610, 9, 1);
        fn_804EAA68(lbl_8087F610, 8);
        fn_804EA6B0(lbl_8087F610, 8, 1);
        fn_804EA810(lbl_8087F610, 0, 1);
        fn_804EA608(lbl_8087F610, 300, 1);
    } else if (state == 2) {
        lbl_8087F610->m540 = 2;
        fn_804EA534(lbl_8087F610, 5, 1);
        fn_804EAA68(lbl_8087F610, 8);
        fn_804EA6B0(lbl_8087F610, 8, 1);
        fn_804EA810(lbl_8087F610, 1, 1);
        fn_804EA608(lbl_8087F610, -1, 1);
    }
}

// inferred: text of a rule value
// NONMATCHING: the L"" is the second string of the .data wide pool (after L"%d" of an unwritten function)
const wchar_t* D2AMpMenu::fn_804B424C(int rule, int value) {
    if (rule == 2 && value == 4) {
        return L"";
    }
    if (rule == 9 && value == 4) {
        return L"";
    }
    if (rule == 10 && value == 2) {
        return L"";
    }
    int base = 0;
    if (rule == 2) {
        base = 0;
    } else if (rule == 9) {
        base = 4;
    } else if (rule == 10) {
        base = 8;
    }
    return lbl_8087F86C->Get_unk(base + value + 0xA3);
}

D2AMpMenuRankMark::~D2AMpMenuRankMark() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int D2AMpMenuRankMark::vf0C() {
    if (fn_800D3FA4() != 0 && mTable.fn_80473F50() == 0) {
        fn_804B4400();
        return 1;
    }
    return 0;
}

void D2AMpMenuRankMark::fn_804B4678(int index, float speed, int repeat) {
    for (int i = 0; i < 3; i++) {
        if (index == -1 || i == index) {
            Play_unk(mMarks[i], repeat, speed);
        }
    }
}

void D2AMpMenuRankMark::fn_804B4700(int index) {
    for (int i = 0; i < 3; i++) {
        if (index == -1 || i == index) {
            mMarks[i]->mFlags &= ~4;
        }
    }
}

void D2AMpMenuRankMark::fn_804B4770(int index) {
    for (int i = 0; i < 3; i++) {
        if (index == -1 || i == index) {
            mMarks[i]->mFlags |= 4;
        }
    }
}

// NONMATCHING: 99.25% - offset of "pos0026(1)" in @stringBase0 (the functions before it are not written)
void D2AMpMenuRankMark::fn_804B49C0(MpRect_unk rect, int index) {
    fn_801F4728(mMarks[index], "pos0026(1)", &rect);
}

} // namespace MP
