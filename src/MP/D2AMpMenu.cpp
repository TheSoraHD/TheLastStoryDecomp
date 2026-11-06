// MP/D2AMpMenu.cpp (0x804AE820-0x804B4C50): main multiplayer menu (title, match select, rankings) and
// the rank mark task. Only part of the functions are reconstructed so far.
#include "MP/D2AMpMenu.h"
#include "MP/D2AMpMenuFriendRoom.h"
#include "game/MenuCommonTask.h"

// inferred: multiplayer session manager (lbl_8087F610); only the member used here
struct MpMenuSession_unk {
    unsigned char m000[0x540];
    int m540; // inferred: game mode
};
extern "C" MpMenuSession_unk* lbl_8087F610;
// inferred: setters of the session rules (the last argument: 1 = default)
extern "C" void fn_804EA75C(MpMenuSession_unk* s, int value, int b);
extern "C" void fn_804EA534(MpMenuSession_unk* s, int value, int b);
extern "C" void fn_804EAA68(MpMenuSession_unk* s, int value);
extern "C" void fn_804EA6B0(MpMenuSession_unk* s, int value, int b);
extern "C" void fn_804EA810(MpMenuSession_unk* s, int value, int b);
extern "C" void fn_804EA608(MpMenuSession_unk* s, int time, int b);
extern "C" void fn_804A5CD8(MenuCommonTask* menu, float y); // inferred: places the help window

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
