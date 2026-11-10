// MP/D2AMpMenu.cpp (0x804AE820-0x804B4C50): main multiplayer menu (title, match select, rankings) and
// the rank mark task. Only part of the functions are reconstructed so far.
#include "MP/D2AMpMenu.h"
#include "MP/MpCampMsgTable_unk.h"
#include "MP/D2AMpMenuFriendRoom.h"
#include "MP/D2AMpMenuFriend.h"
#include "MP/D2ACommonDialog.h"
#include "MP/D2AMpMenuSetMessage.h"
#include "game/MenuCommonTask.h"
#include "atn/Csv.h"
#include "MP/SoftKeyboard.h"
#include "atn/GraphicsDevice.h"
#include "atn/DrawHelper.h"
#include "MP/D2AMpMenuLobby.h"
#include "MP/D2AMpMenuMessageList.h"
#include "game/GameConfig_unk.h"
#include "atn/net/NetRanking.h"

// inferred: view of atn::net::Manager (lbl_8087F628) with the only method used here: atn/net/Manager.h cannot be
// included with MP/D2AMpMenuFriend.h (both define MpFriendList_unk and declare lbl_8087F628). Rule 10 debt.
namespace atn {
namespace net {
class Manager {
public:
    int fn_80509AF8(); // inferred: resets the connection
};
} // namespace net
} // namespace atn

extern "C" int atoi(const char* s);
extern "C" int strcmp(const char* a, const char* b);

// inferred: setters of the session rules (the last argument: 1 = default)
extern "C" void fn_804EA75C(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EA534(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EAA68(MpMsgSession_unk* s, int value);
extern "C" void fn_804EA6B0(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EA810(MpMsgSession_unk* s, int value, int b);
extern "C" void fn_804EA608(MpMsgSession_unk* s, int time, int b);
extern "C" void fn_804A5CD8(MenuCommonTask* menu, float y); // inferred: places the help window
extern "C" int fn_804EAA50(MpMsgSession_unk* s); // inferred: nonzero for the host of the room
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int arg); // inferred: help text
extern "C" void fn_804A4738(MP::MpD2A_unk* bar, int top, int count, int visible); // inferred: scroll bar
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c);
extern "C" int fn_804DCA4C(MpMsgSession_unk* s, int id, int arg); // inferred: state of a friend
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" int fn_804E4DD4(MpMsgSession_unk* s, int mode, int arg); // inferred: starts the ranking request
extern "C" void fn_804FB220(int* state, int value); // inferred: changes the state of the session (as atn/PacketPack.cpp)
extern "C" atn::GraphicsDevice_unk* lbl_8087EEE0; // inferred: graphics device (screen size)
extern "C" const wchar_t* fn_804FA8E8(MpMsgSession_unk* s, int value); // inferred: text of a score
extern "C" void fn_800D2338(atn::Task* task);                 // inferred: removes the task
extern "C" void fn_804E4E74(MpMsgSession_unk* s);
extern "C" void fn_804A436C(int* cursor, int count, int arg2, int wrap, int sound); // inferred: moves a cursor
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void* fn_801FEC08(void* layout, const char* name); // inferred: track of a pane value
extern "C" float fn_801F0544(void* key, int frame);           // inferred: value of a key
extern "C" void fn_804EAA5C(MpMsgSession_unk* s, int value);  // inferred: sets the kind of match
extern "C" int SCCheckPCMessageRestriction();
extern "C" const wchar_t* fn_804FA88C(MpMsgSession_unk* s, int exp); // inferred: rank name of an experience
extern "C" void fn_804DB474(MpMsgSession_unk* s);
extern "C" void fn_804DC450(MpMsgSession_unk* s, int state);
extern "C" int fn_804EA520(MpMsgSession_unk* s); // inferred: nonzero when the session is idle (atn/PacketPack.cpp)

namespace MP {

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

// NONMATCHING: 99.5% - the loop counters get r26 and the offsets r27/r28 (r28 and r26/r27 in the original)
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
    for (i = 0; i < 2; i++) {
        mEC[i] = -1;
    }
    for (i = 0; i < 23; i++) {
        mF4[i] = -1;
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

// inferred: widens the cursor `pos` of `task` to the text of the pane `str`
static inline float TextWidth_unk(const std::basic_string<wchar_t>& text, float size) {
    return fn_8006EF48(lbl_8087EEC8, size, text.data(), 0.0f, 1, 1);
}
static inline void FitCursor_unk(MpD2A_unk* task, const char* str, const char* pos) {
    float w = TextWidth_unk(fn_800A4450(fn_801FEC74(task->mLayout, str), 0, 0), 21.0f);
    SetPaneValue_unk(task->mLayout, pos, 2, (7.0f + w) / 100.0f);
}

// inferred: fills the screen with `color` at depth `z`
static inline void FillScreen_unk(float z, unsigned int color) {
    lbl_8087EEB0->fn_80060D58(0.0f, 0.0f, z, (float)lbl_8087EEE0->mWidth, (float)lbl_8087EEE0->mHeight, color);
}

int D2AMpMenu::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetAll_unk(0);
        mD0 = 1;
        fn_804AF520(1);
        SetPaneValue_unk(m70->mLayout, "pos0007", 4, -110.0f);
        SetPaneValue_unk(m70->mLayout, "pos0017", 4, -110.0f);
        void* pane = fn_801FEC74(m70->mLayout, "string0007(1)");
        if (pane != 0) {
            fn_8006EF48(lbl_8087EEC8, 16.0f, fn_800A4450(pane, 0, 0).data(), 0.0f, 1, 1);
        }
        fn_804A5CD8(MenuCommonTask::sInstance, -100.0f);
        D2AMpMenuRankMark::sInstance->fn_804B4770(-1);
        return 1;
    }
    return 0;
}

// NONMATCHING: 98.6% - copy of the ranking rows (RefreshRanking_unk): the original keeps the row base and the
// offset in two registers (one more saved register)
// inferred: changes the state of the menu: saves the cursor of the old state and opens the windows of the new one
void D2AMpMenu::fn_804AF520(int state) {
    if (mD0 == 0) {
        return;
    }
    if (mBC == state) {
        return;
    }
    if (state < 0) {
        return;
    }
    if ((unsigned int)mBC <= 22) {
        mF4[mBC] = mE4;
    }
    int cursor = mE4;
    mE4 = 0;
    if ((unsigned int)state <= 22 && mF4[state] >= 0) {
        mE4 = mF4[state];
    }
    mC0 = mBC;
    mBC = state;
    fn_804B0A78();
    switch (mBC) {
    case 1:
        vf1C();
        break;
    case 2:
        MenuCommonTask::fn_804A29C4(this);
        MenuCommonTask::sInstance->fn_800D246C(0);
        MenuCommonTask::sInstance->fn_804A2F98();
        if (mC0 == 8) {
            ((atn::net::Manager*)lbl_8087F628)->fn_80509AF8();
            lbl_8087F610->m9750 = -1;
            fn_804FB220(&lbl_8087F610->m4FC, -2);
            D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 0);
            D2ACommonDialog::sInstance->fn_804ACD10();
            D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F86C->Get_unk(0xE6), 0);
        } else {
            FitCursor_unk(m48, "cmd01", "cursorPos1(1)");
            FitCursor_unk(m48, "cmd01(1)", "cursorPos1(1)(1)");
            FitCursor_unk(m48, "string0006(1)(1)", "pos0003(1)(1)");
            FitCursor_unk(m48, "string0003(1)", "pos0006(1)");
            FitCursor_unk(m48, "string0004", "cursorPos3");
            Play_unk(m48, 0, 1.0f);
            Play_unk(m4C, 0, 1.0f);
            if (mC0 == 3) {
                mE4 = cursor;
            }
        }
        if (mE8 >= 0) {
            mE4 = mE8;
            mE8 = -1;
        }
        break;
    case 3:
        D2ACommonDialog::sInstance->fn_804AC96C(0, 1.0f, 1, 0);
        D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F86C->Get_unk(0xE7), 0);
        D2ACommonDialog::sInstance->fn_804ACD10();
        D2ACommonDialog::sInstance->mFlags |= 4;
        mE4 = cursor;
        break;
    case 4:
        D2AMpMenuSetMessage::fn_804C8778(this);
        break;
    case 5: {
        mE4 = cursor;
        MpData_unk* data = lbl_8087F628;
        if (wcslen(data->m430.m1C0) != 0) {
            if (data->m430.m7F8 > 0) {
                m154 = 1;
                D2ACommonDialog::sInstance->fn_804AC96C(0, 1.0f, 1, 1);
                D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F1E4->Get_unk(0x39B), 0);
                D2ACommonDialog::sInstance->fn_804ACD10();
            } else {
                m154 = 3;
                if (lbl_8087F860 == 0) {
                    SoftKeyboard::fn_80502974(this);
                }
                lbl_8087F860->SetText_unk(lbl_8087F628->m430.m1C0);
                lbl_8087F860->fn_805053F8();
            }
        } else {
            m154 = 0;
            D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 0);
            D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F1E4->Get_unk(0x399), 0);
            D2ACommonDialog::sInstance->fn_804ACD10();
        }
        break;
    }
    case 6:
        D2AMpMenuFriend::fn_804B6988(this);
        Play_unk(m70, 0, 1.0f);
        break;
    case 7: {
        int ok = 0;
        if (lbl_8087F628 != 0 && SCCheckPCMessageRestriction() == 0) {
            ok = 1;
        }
        mDC = ok;
        Play_unk(mB8, 1, 1.0f);
        SetPaneText_unk(&mB8->mLayout, "cmd01", lbl_8087F86C->Get_unk(0xFA));
        SetPaneText_unk(&mB8->mLayout, "string0003(1)", lbl_8087F86C->Get_unk(0xFA));
        fn_804FB220(&lbl_8087F610->m4FC, 8);
        break;
    }
    case 8:
        D2ACommonDialog::sInstance->fn_804AC96C(0, 1.0f, 1, 0);
        D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F86C->Get_unk(0xE8), 0);
        D2ACommonDialog::sInstance->fn_804ACD10();
        D2ACommonDialog::sInstance->mFlags |= 4;
        mE4 = cursor;
        break;
    case 9: {
        mE0 = 1;
        MenuCommonTask::sInstance->fn_800D246C(0);
        MenuCommonTask::sInstance->fn_804A2F98();
        if (mC0 == 7) {
            D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 0);
            D2ACommonDialog::sInstance->fn_804ACD10();
            D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F86C->Get_unk(0xE9), 0);
        } else {
            Play_unk(m50, 0, 1.0f);
            Play_unk(m54, 0, 1.0f);
            for (int i = 0; i < sizeof(m58) / sizeof(m58[0]); i++) {
                Play_unk(m58[i], 1, 1.0f);
            }
            Play_unk(m64, 0, 1.0f);
            D2AMpMenuRankMark::sInstance->fn_804B4678(0, 1.0f, 0);
            D2AMpMenuRankMark::sInstance->fn_804B4700(0);
            if (mD4 = fn_8050F5A8(&lbl_8087F628->m430, 0) != -1) {
                m50->SetColor_unk("col0006", 255.0f, 255.0f, 255.0f);
            } else {
                m50->SetColor_unk("col0006", 100.0f, 100.0f, 100.0f);
            }
            if (mC0 == 8) {
                if (cursor == 1 && mD4 == 0) {
                    mE4 = 0;
                } else {
                    mE4 = cursor;
                }
            }
        }
        wchar_t text[64] = {0};
        MpData_unk* data = lbl_8087F628;
        int exp = data->m430.m7F8;
        SetPaneText_unk(&m64->mLayout, "string0027(2)(1)", data->m430.m1C0);
        fn_800DD3FC(text, L"%d", exp);
        SetPaneText_unk(&m64->mLayout, "string0008(1)(1)", text);
        SetPaneText_unk(&m64->mLayout, "string0007(1)(1)", fn_804FA88C(lbl_8087F610, exp));
        D2AMpMenuRankMark::sInstance->fn_804B47E0(-1, 0);
        if (D2AMpMenuLobby::sInstance != 0) {
            fn_800D2338(D2AMpMenuLobby::sInstance);
        }
        if (D2AMpMenuMessageList::sInstance != 0) {
            fn_800D2338(D2AMpMenuMessageList::sInstance);
        }
        break;
    }
    case 10:
        FitCursor_unk(m68, "cmd01", "cursorPos1(1)");
        FitCursor_unk(m68, "string0006(1)", "pos0003(1)");
        FitCursor_unk(m68, "string0004", "cursorPos3");
        Play_unk(m68, 0, 1.0f);
        Play_unk(m6C, 0, 1.0f);
        for (int i = 0; i < sizeof(m58) / sizeof(m58[0]); i++) {
            Play_unk(m58[i], 1, 1.0f);
        }
        break;
    case 12:
        Play_unk(mB8, 1, 1.0f);
        SetPaneText_unk(&mB8->mLayout, "cmd01", lbl_8087F86C->Get_unk(0xFB));
        SetPaneText_unk(&mB8->mLayout, "string0003(1)", lbl_8087F86C->Get_unk(0xFB));
        Play_unk(m74, 0, 1.0f);
        fn_804B4150(mC4);
        fn_804DB474(lbl_8087F610);
        m150 = 0;
        break;
    case 11:
        D2AMpMenuFriendRoom::sInstance->fn_804BA350(1);
        fn_804DC450(lbl_8087F610, 4);
        break;
    case 13:
        if (lbl_8087F610 != 0) {
            D2AMpMenuLobby::fn_804BBBA0(this, mC0 != 16, lbl_8087F610->m544);
        } else {
            D2AMpMenuLobby::fn_804BBBA0(this, mC0 != 16, 0);
        }
        D2AMpMenuMessageList::fn_804C2FF8(this);
        break;
    case 17:
        mDC = 0;
        Play_unk(m7C, 1, 1.0f);
        Play_unk(m80, 0, 1.0f);
        Play_unk(m84, 0, 1.0f);
        Play_unk(mB4, 0, 1.0f);
        m88->mFrame = 0.0f;
        SetPaneText_unk(&m88->mLayout, "conf_before", fn_80116FC0(0, 0x89547B));
        fn_804B3EFC(-1, 0, 10);
        if (mC0 != 18) {
            m78->mFrame = 0.0f;
            Play_unk(m78, 0, 1.0f);
        }
        break;
    case 18:
        Play_unk(m70, 0, 1.0f);
        for (int i = 0; i < 10; i++) {
            Play_unk(m8C[i], 0, 1.0f);
        }
        RefreshRanking_unk();
        break;
    case 15:
        Play_unk(mB8, 1, 1.0f);
        SetPaneText_unk(&mB8->mLayout, "cmd01", lbl_8087F86C->Get_unk(0xFC));
        SetPaneText_unk(&mB8->mLayout, "string0003(1)", lbl_8087F86C->Get_unk(0xFC));
        break;
    case 16:
    case 21:
        if (D2AMpMenuLobby::sInstance != 0) {
            fn_800D2338(D2AMpMenuLobby::sInstance);
        }
        m299C.Clear();
        break;
    case 19:
        vf20();
        m299C.m00 = 2;
        m299C.m08 = 14;
        m299C.m04 = 0;
        m299C.m0C = 0;
        break;
    case 22:
        if (D2AMpMenuLobby::sInstance != 0) {
            fn_800D2338(D2AMpMenuLobby::sInstance);
        }
        if (D2AMpMenuMessageList::sInstance != 0) {
            fn_800D2338(D2AMpMenuMessageList::sInstance);
        }
        for (int i = 0; i < 23; i++) {
            mF4[i] = -1;
        }
        m299C.Clear();
        FillScreen_unk(-10000.0f, 0xFFFFFF);
        break;
    }
}

void D2AMpMenu::vf1C() {}

void D2AMpMenu::vf20() {}

// inferred: closes the windows of the previous state
// NONMATCHING: 99.95% - case 5: the name gets r31 (r29 in the original, as if `this` were still live)
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

// inferred: update of the menu, runs the input of the current state
void D2AMpMenu::vf10() {
    if (mD8 != 0) {
        mD8 = 0;
        return;
    }
    switch (mBC) {
    case 2:
        fn_804B12F8();
        break;
    case 3:
        if (D2ACommonDialog::sInstance->m4C != 0) {
            if (D2ACommonDialog::sInstance->fn_804AC79C()) {
                fn_804AF520(19);
            } else if (D2ACommonDialog::sInstance->fn_804AC7EC()) {
                fn_804AF520(2);
            }
        }
        break;
    case 4: {
        int state = D2AMpMenuSetMessage::sInstance->mD8;
        if (state == 8) {
            fn_804AF520(2);
        } else if (state == 1) {
            Play_unk(m70, 0, 1.0f);
        } else if (state == 7) {
            Play_unk(m70, 0, -1.0f);
        }
        break;
    }
    case 5:
        fn_804B2934();
        break;
    case 6:
        if (D2AMpMenuFriend::sInstance->m2BC == 6) {
            fn_804AF520(mC0);
        }
        break;
    case 8:
        if (D2ACommonDialog::sInstance->m4C != 0) {
            if (D2ACommonDialog::sInstance->fn_804AC79C()) {
                for (int i = 0; i < 23; i++) {
                    mF4[i] = -1;
                }
                fn_804AF520(2);
            } else if (D2ACommonDialog::sInstance->fn_804AC7EC()) {
                fn_804AF520(9);
            }
        }
        break;
    case 9:
        fn_804B1528();
        break;
    case 16:
        if (lbl_8087F0A8->mInput.fn_801231D0(0x34) != 0 && fn_804EA520(lbl_8087F610) != 0 &&
            D2AMpMenuMessageList::sInstance != 0 && D2AMpMenuMessageList::sInstance->m94 == 4) {
            if (lbl_8087F610->m4FC != 30 || lbl_8087F610->m50C == 0) {
                fn_80117228(10);
                D2AMpMenuMessageList::sInstance->fn_804C35F0(1);
            }
        }
        break;
    case 17:
        fn_804B1C88();
        break;
    case 18:
        fn_804B1E48();
        break;
    case 10:
        fn_804B274C();
        break;
    case 11:
        if (D2AMpMenuFriendRoom::sInstance->m88 == 5 && D2AMpMenuFriendRoom::sInstance->mA0 != 0) {
            D2AMpMenuFriendRoom::sInstance->fn_804BA350(6);
            fn_804AF520(10);
        }
        break;
    case 13:
        if (D2AMpMenuLobby::sInstance->mD88 == 9 && D2AMpMenuLobby::sInstance->mDA4 == 0) {
            if (D2AMpMenuLobby::sInstance != 0) {
                fn_800D2338(D2AMpMenuLobby::sInstance);
            }
            if (fn_804EAA50(lbl_8087F610) == 1) {
                fn_804AF520(11);
            } else {
                fn_804AF520(10);
            }
        }
        break;
    case 19:
        if (m299C.m04 == m299C.m08) {
            fn_804AF520(20);
        }
        break;
    }
    if (m299C.m00 != 0 && m299C.m0C == 0) {
        m299C.m04++;
        if (m299C.m04 >= m299C.m08) {
            m299C.m04 = m299C.m08;
            m299C.m0C = 1;
        }
    }
}

// inferred: input of the game selection (state 2)
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

// NONMATCHING: 99.5% - the pane name of m64 is computed before its layout in the original; the pad gets r28
// (r29 in the original)
// inferred: input of the match selection (state 9)
void D2AMpMenu::fn_804B1528() {
    if (D2ACommonDialog::sInstance->m4C != 0) {
        if (D2ACommonDialog::sInstance->fn_804AC734()) {
            D2ACommonDialog::sInstance->fn_804ACAF8();
            D2ACommonDialog::sInstance->fn_804ACDBC();
            FitCursor_unk(m50, "cmd01", "cursorPos1(1)");
            FitCursor_unk(m50, "string0006(1)", "pos0003(1)");
            FitCursor_unk(m50, "string0001(1)", "pos0007(1)");
            FitCursor_unk(m50, "string0010(1)", "pos0008(1)");
            FitCursor_unk(m50, "string0004", "cursorPos3");
            Play_unk(m50, 0, 1.0f);
            Play_unk(m54, 0, 1.0f);
            for (int i = 0; i < sizeof(m58) / sizeof(m58[0]); i++) {
                Play_unk(m58[i], 1, 1.0f);
            }
            float w = fn_8006EF48(lbl_8087EEC8, 16.0f, fn_800A4450(fn_801FEC74(m64->mLayout, "string0001(1)(1)"), 0, 0).data(), 0.0f, 1, 1);
            w += fn_801F0544((char*)fn_801FEC08(m64->mLayout, "pos0001") + 4, 0);
            SetPaneValue_unk(m64->mLayout, "pos0008(2)", 0, w);
            Play_unk(m64, 0, 1.0f);
            D2AMpMenuRankMark::sInstance->fn_804B4678(0, 1.0f, 0);
            D2AMpMenuRankMark::sInstance->fn_804B4700(0);
            if (mD4 = fn_8050F5A8(&lbl_8087F628->m430, 0) != -1) {
                m50->SetColor_unk("col0006", 255.0f, 255.0f, 255.0f);
            } else {
                m50->SetColor_unk("col0006", 100.0f, 100.0f, 100.0f);
            }
        } else {
            return;
        }
    }
    if (mE4 == 1 && mD4 == 0) {
        mE4 = 0;
    }
    void* pad = lbl_8087EF70;
    do {
        fn_804A436C(&mE4, 5, 5, 1, 3);
    } while (mE4 == 1 && mD4 == 0);
    if (fn_800A555C(pad, 0, 4) != 0) {
        fn_80117228(1);
        if (mE4 == 0) {
            fn_804EAA5C(lbl_8087F610, 0);
            fn_804AF520(10);
        } else if (mE4 == 1) {
            fn_804EAA5C(lbl_8087F610, 1);
            fn_804AF520(10);
        } else if (mE4 == 2) {
            fn_804AF520(17);
        } else if (mE4 == 3) {
            fn_804AF520(6);
        } else if (mE4 == 4) {
            fn_804AF520(8);
        }
    } else if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80117228(2);
        fn_804AF520(8);
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

// inferred: blinks the side marks and starts the ranking request
void D2AMpMenu::fn_804B1C88() {
    for (int i = 0; i < 2; i++) {
        if (mEC[i] != -1) {
            int v;
            if (mEC[i] <= 5) {
                v = mEC[i] * 30;
            } else {
                v = 150 - (mEC[i] - 5) * 30;
            }
            unsigned int color = 0xFF000000 | (v & 0xFF) << 16 | (v & 0xFF) << 8 | (v & 0xFF);
            if (i == 0) {
                m78->SetColor_unk("col0101(1)", color);
            } else if (i == 1) {
                m78->SetColor_unk("col0102(1)", color);
            }
            mEC[i]++;
            if (mEC[i] > 10) {
                mEC[i] = -1;
            }
        } else if (i == 0) {
            m78->SetColor_unk("col0101(1)", 0xFF000000);
        } else if (i == 1) {
            m78->SetColor_unk("col0102(1)", 0xFF000000);
        }
    }
    if (mDC == 0) {
        mDC = fn_804E4DD4(lbl_8087F610, mC8, mE0 != 0);
        mE0 = 0;
        if (mDC == 0) {
            fn_804AF520(0x12);
            fn_804FB220(&lbl_8087F610->m4FC, 0x2A);
        }
        MpRect_unk rect = GetPaneRect_unk(m80, "sb_null_0003(1)");
        rect.h = 1.0f;
        fn_801F4728(mB4, "basePos", &rect);
        SetPaneFloat_unk(&mB4->mLayout, "s_hightMax", 170.0f);
    }
}

// inferred: copies the received ranking into the current list and shows it (the unused locals are in the
// original: they give its stack frame)
inline void D2AMpMenu::RefreshRanking_unk() {
    wchar_t text[32] = {0};
    MpRect_unk rect;
    if (mDC != 0) {
        RANKING_DATA_LIST* list = mLists[mC8];
        for (int i = 0; i < list->GetCount(); i++) {
            list->Get(i)->Clear();
        }
        list = mLists[mC8];
        atn::FixedArray_unk<atn::net::NetRankData_unk, 65>* rows = &lbl_8087F5A4->mRows;
        for (int i = 0; i < list->GetCount(); i++) {
            if (i < rows->size()) {
                list->Get(i)->m00 = (*rows)[i].mOrder;
                list->Get(i)->m04 = (*rows)[i].mScore;
                fn_800DD3FC(list->Get(i)->mName, (const wchar_t*)(*rows)[i].mUserData);
                list->Get(i)->m48 = 1;
                list->Get(i)->m4C = (*rows)[i].mPid;
            } else {
                list->Get(i)->m48 = 0;
            }
        }
        list->m04 = rows->size();
        mLists[mC8]->m00 = 0;
    }
    fn_804B3EFC(mC8, mLists[mC8]->m00, 10);
    mDC = 1;
}

// NONMATCHING: 98.6% - in the copy of the rows (RefreshRanking_unk) the original keeps the row base and the
// offset in two registers (one more saved register: r22-r31)
// inferred: input of the ranking (state 18)
void D2AMpMenu::fn_804B1E48() {
    char name[64] = "";
    MpRect_unk rect;
    void* pad = lbl_8087EF70;
    for (int i = 0; i < 10; i++) {
        sprintf(name, "ranking_list%02d", i + 1);
        rect = GetPaneRect_unk(m80, name);
        fn_801F4728(m8C[i], "pos0030(1)(1)(1)", &rect);
    }
    int decide = Decide_unk(&mE4, 1);
    if (decide == 1) {
        fn_804AF520(9);
    } else if (decide == 2) {
        fn_804AF520(9);
    }
    for (int i = 0; i < 2; i++) {
        if (mEC[i] != -1) {
            int v;
            if (mEC[i] <= 5) {
                v = mEC[i] * 30;
            } else {
                v = 150 - (mEC[i] - 5) * 30;
            }
            unsigned int color = 0xFF000000 | (v & 0xFF) << 16 | (v & 0xFF) << 8 | (v & 0xFF);
            if (i == 0) {
                m78->SetColor_unk("col0101(1)", color);
            } else if (i == 1) {
                m78->SetColor_unk("col0102(1)", color);
            }
            mEC[i]++;
            if (mEC[i] > 10) {
                mEC[i] = -1;
            }
        } else if (i == 0) {
            m78->SetColor_unk("col0101(1)", 0xFF000000);
        } else if (i == 1) {
            m78->SetColor_unk("col0102(1)", 0xFF000000);
        }
    }
    int prev = mC8;
    int changed = 0;
    if (fn_800A555C(pad, 0, 0x17) != 0 || fn_800A555C(pad, 0, 2) != 0) {
        mEC[0] = 0;
        if (--mC8 < 0) {
            mC8 = 2;
        }
        changed = 1;
    } else if (fn_800A555C(pad, 0, 0x18) != 0 || fn_800A555C(pad, 0, 3) != 0) {
        mEC[1] = 0;
        if (++mC8 > 2) {
            mC8 = 0;
        }
        changed = 1;
    }
    if (changed) {
        fn_80117228(3);
        if (lbl_8087F5A4->fn_8050C840(mC8) != 0) {
            fn_804AF520(17);
        } else {
            mDC = 0;
            fn_804B3EFC(-1, 0, 10);
            RefreshRanking_unk();
        }
        fn_804B3C78(prev, mC8, 1);
    } else {
        int top = mLists[mC8]->m00;
        if (fn_800A55AC(pad, 0, 0) != 0 || fn_800A55AC(pad, 0, 0x1A) != 0) {
            if (--mLists[mC8]->m00 < 0) {
                mLists[mC8]->m00 = 0;
            }
        } else if (fn_800A55AC(pad, 0, 1) != 0 || fn_800A55AC(pad, 0, 0x19) != 0) {
            RANKING_DATA* data = mLists[mC8]->Get(mLists[mC8]->m00 + 10);
            bool more = data == 0 ? false : data->m48 != 0;
            if (more == true) {
                mLists[mC8]->m00++;
            }
        }
        if (top != mLists[mC8]->m00) {
            fn_804B3EFC(mC8, mLists[mC8]->m00, 10);
            fn_80117228(3);
        }
    }
    for (int i = 0; i < 10; i++) {
        if (i != mCC && m8C[i]->mFrame >= 11.0f) {
            m8C[i]->mSpeed = 0.0f;
            m8C[i]->mFrame = 11.0f;
        }
    }
    rect = GetPaneRect_unk(m80, "sb_null_0003(1)");
    rect.h = 1.0f;
    fn_801F4728(mB4, "basePos", &rect);
    SetPaneFloat_unk(&mB4->mLayout, "s_hightMax", 170.0f);
}

// inferred: input of the room type (state 10)
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

// inferred: places the cursor on the selected item of `base` and shows its help (a static function:
// with it the rectangle gets the lowest stack slot, as in the original)
static void SetTopCursor_unk(D2AMpMenu* menu, MpD2A_unk* base) {
    if (MenuCommonTask::sInstance != 0 && D2ACommonDialog::sInstance->m4C == 0) {
        char name[64];
        sprintf(name, "top_cursor%d", menu->mE4 + 1);
        MpRect_unk rect = GetPaneRect_unk(base, name);
        fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, 0);
        const wchar_t* text = menu->fn_804B424C(menu->mBC, menu->mE4);
        if (wcscmp(text, L"") != 0) {
            fn_804A3C24(MenuCommonTask::sInstance, text, 0);
        }
    }
}

// inferred: opens the keyboard with the own name
inline void D2AMpMenu::OpenKeyboard_unk() {
    for (int i = 0; i < 23; i++) {
        mF4[i] = -1;
    }
    if (lbl_8087F860 == 0) {
        SoftKeyboard::fn_80502974(this);
    }
    lbl_8087F860->SetText_unk(lbl_8087F628->m430.m1C0);
    lbl_8087F860->fn_805053F8();
    mDC = 0;
    m154 = 3;
}

// inferred: input of the own name
void D2AMpMenu::fn_804B2934() {
    switch (m154) {
    case 0:
        if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() != 0) {
            OpenKeyboard_unk();
        }
        break;
    case 1:
        if (D2ACommonDialog::sInstance->m4C != 0) {
            if (D2ACommonDialog::sInstance->fn_804AC79C() != 0) {
                D2ACommonDialog::sInstance->fn_804AC96C(0, 1.0f, 1, 0);
                D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F1E4->Get_unk(0x39D), 0);
                D2ACommonDialog::sInstance->fn_804ACD10();
                m154 = 2;
            } else if (D2ACommonDialog::sInstance->fn_804AC7EC() != 0) {
                fn_804AF520(2);
            }
        }
        break;
    case 2:
        if (D2ACommonDialog::sInstance->m4C != 0) {
            if (D2ACommonDialog::sInstance->fn_804AC79C() != 0) {
                OpenKeyboard_unk();
            } else if (D2ACommonDialog::sInstance->fn_804AC7EC() != 0) {
                fn_804AF520(2);
            }
        }
        break;
    case 3:
        if (lbl_8087F860->m25AC == 0) {
            m154 = 1;
            fn_804AF520(2);
        }
        break;
    }
}

// NONMATCHING: 99.7% - case 12 reloads lbl_8087F628 in the original; alpha and the device pointer of the fade
// get r7/r8 swapped
// inferred: draw of the menu: shows the windows of the current state, then the fade
void D2AMpMenu::vf14() {
    for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
        if (*it != 0) {
            (*it)->mFlags |= 4;
        }
    }
    switch (mBC) {
    case 2:
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        if (D2ACommonDialog::sInstance->m4C == 0) {
            SetTopCursor_unk(this, m48);
        }
        break;
    case 3:
        D2ACommonDialog::sInstance->mFlags &= ~4;
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        if (D2ACommonDialog::sInstance->m4C == 0) {
            SetTopCursor_unk(this, m48);
        }
        break;
    case 4:
        if (D2AMpMenuSetMessage::sInstance != 0) {
            m70->mFlags &= ~4;
            int state = D2AMpMenuSetMessage::sInstance->mD8;
            if (state < 1 || state == 8) {
                FillScreen_unk(100.0f, 0x80000000);
            }
        }
        break;
    case 5:
        D2ACommonDialog::sInstance->mFlags &= ~4;
        if (m154 == 3) {
            if (lbl_8087F860->m25AC == 1) {
                FillScreen_unk(100.0f, 0x80000000);
            }
        } else {
            m48->mFlags &= ~4;
            m4C->mFlags &= ~4;
            if (D2ACommonDialog::sInstance->m4C == 0) {
                SetTopCursor_unk(this, m48);
            }
        }
        break;
    case 6:
        if (D2AMpMenuFriend::sInstance != 0 && D2AMpMenuFriend::sInstance->m2BC < 1) {
            FillScreen_unk(100.0f, 0x80000000);
        }
        if (D2ACommonDialog::sInstance->m4C == 0) {
            m70->mFlags &= ~4;
        }
        break;
    case 8:
        D2ACommonDialog::sInstance->mFlags &= ~4;
        fn_804B3494();
        break;
    case 9:
        fn_804B3494();
        break;
    case 18:
        fn_804B3838();
        break;
    case 17:
        m78->mFlags &= ~4;
        m7C->mFlags &= ~4;
        m80->mFlags &= ~4;
        m84->mFlags &= ~4;
        m88->mFlags &= ~4;
        fn_804A4738(mB4, 0, 0, 10);
        SetPaneValue2_unk(&m80->mLayout, "col0005", 0, 125.0f);
        break;
    case 12:
        mB8->mFlags &= ~4;
        if (lbl_8087F628->mC1F8 == 8 || lbl_8087F628->mC1F4 == 1) {
            if (lbl_8087F610->m99EC <= 0) {
                if (lbl_8087F628->mC0 == 1) {
                    m150++;
                } else {
                    m150 = 0;
                }
                if (m150 < 30) {
                    m74->mFlags &= ~4;
                }
            }
        }
        break;
    case 10:
        fn_804B39F8();
        break;
    case 13:
        if (D2AMpMenuLobby::sInstance != 0 && D2AMpMenuLobby::sInstance->mD88 < 1) {
            FillScreen_unk(100.0f, 0x80000000);
        }
        break;
    case 15:
        mB8->mFlags &= ~4;
        break;
    case 7:
        if (mDC != 0) {
            mB8->mFlags &= ~4;
        } else {
            FillScreen_unk(100.0f, 0x80000000);
        }
        break;
    case 19:
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        vf20();
        break;
    case 20:
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        break;
    case 22:
        FillScreen_unk(100.0f, 0x80000000);
        break;
    }
    int mode = m299C.m00;
    if (mode != 0) {
        unsigned int color;
        int alpha = 255;
        if (m299C.m08 != 0) {
            alpha = 255 / m299C.m08 * m299C.m04;
        }
        bool in = mode == 1 || mode == 3;
        if (in) {
            alpha = 255 - alpha;
        }
        bool black = mode == 1 || mode == 2;
        color = black ? 0 : 0xFFFFFF;
        FillScreen_unk(-10000.0f, color + (alpha << 24));
    }
}

// NONMATCHING: 98.4% - the original reads D2ACommonDialog::sInstance->m4C at the end without using it (tried
// an empty if and an empty loop: removed)
// inferred: shows the main page of the menu
void D2AMpMenu::fn_804B3494() {
    if (D2ACommonDialog::sInstance->m4C != 0 && mBC != 8) {
        lbl_8087EEB0->fn_80060D58(0.0f, 0.0f, 100.0f, (float)lbl_8087EEE0->mWidth,
                                  (float)lbl_8087EEE0->mHeight, 0x80000000);
        return;
    }
    MpData_unk* data = lbl_8087F628;
    m50->mFlags &= ~4;
    SetTopCursor_unk(this, m50);
    m54->mFlags &= ~4;
    m64->mFlags &= ~4;
    D2AMpMenuRankMark::sInstance->fn_804B4700(0);
    if (lbl_8087F628->mC8 >= 8) {
        int id = fn_8050F5A8(&lbl_8087F628->m430, 0);
        while (id != -1) {
            if (fn_804DCA4C(lbl_8087F610, id, 1) != 0 || fn_804DCA4C(lbl_8087F610, id, 0) != 0 ||
                fn_804DCA4C(lbl_8087F610, id, 2) != 0) {
                m58[0]->mFlags &= ~4;
                MpRect_unk rect = GetPaneRect_unk(m50, "friend_online");
                fn_801F4728(m58[0], "pos0009(1)", &rect);
                break;
            }
            id = fn_8050F664(&lbl_8087F628->m430, 0);
        }
    }
    MpRect_unk rect = GetPaneRect_unk(m64, "rank_null");
    D2AMpMenuRankMark::sInstance->fn_804B49C0(rect, 0);
    D2AMpMenuRankMark::sInstance->fn_804B49E0(data->m430.m7F8, 0);
}

// inferred: shows the windows of the ranking
void D2AMpMenu::fn_804B3838() {
    if (mDC != 0) {
        m78->mFlags &= ~4;
        m80->mFlags &= ~4;
        m84->mFlags &= ~4;
        m88->mFlags &= ~4;
        mB4->mFlags &= ~4;
        m70->mFlags &= ~4;
        SetPaneValue2_unk(&m80->mLayout, "col0005", 0, 125.0f);
        for (int i = 0; i < 10; i++) {
            m8C[i]->mFlags &= ~4;
        }
        fn_804A3C24(MenuCommonTask::sInstance, lbl_8087F86C->Get_unk(mC8 + 0xAD), 0);
        fn_804A4738(mB4, mLists[mC8]->m00, mLists[mC8]->m04, 10);
    }
}

// inferred: shows the game selection
void D2AMpMenu::fn_804B39F8() {
    m68->mFlags &= ~4;
    m6C->mFlags &= ~4;
    SetTopCursor_unk(this, m68);
    if (fn_804EAA50(lbl_8087F610) == 1) {
        if (lbl_8087F628->mC8 >= 8) {
            int id = fn_8050F5A8(&lbl_8087F628->m430, 0);
            while (id != -1) {
                if (fn_804DCA4C(lbl_8087F610, id, 2) != 0) {
                    m58[0]->mFlags &= ~4;
                    MpRect_unk rect = GetPaneRect_unk(m68, "friend_online1");
                    fn_801F4728(m58[0], "pos0009(1)", &rect);
                }
                if (fn_804DCA4C(lbl_8087F610, id, 0) != 0) {
                    m58[1]->mFlags &= ~4;
                    MpRect_unk rect = GetPaneRect_unk(m68, "friend_online2");
                    fn_801F4728(m58[1], "pos0009(1)", &rect);
                }
                id = fn_8050F664(&lbl_8087F628->m430, 0);
            }
        }
        SetTaskText_unk(m6C, "title", fn_80116FC0(0, 0x166));
    } else {
        SetTaskText_unk(m6C, "title", fn_80116FC0(0, 0x164));
    }
}

// inferred: name of a side
static inline void SideName_unk(wchar_t* text, int side) {
    if (side == 0) {
        fn_800DD3FC(text, fn_80116FC0(0, 0x89547B));
    } else if (side == 2) {
        fn_800DD3FC(text, fn_80116FC0(0, 0x89547C));
    } else if (side == 1) {
        fn_800DD3FC(text, fn_80116FC0(0, 0x8954D7));
    }
}

// NONMATCHING: 99.6% - the task of Play_unk in r29 instead of r30
// inferred: shows the change of side from `before` to `after` (animated backwards if `back`)
__declspec(noinline) void D2AMpMenu::fn_804B3C78(int before, int after, int back) {
    wchar_t textBefore[256] = {0};
    wchar_t textAfter[256] = {0};
    SideName_unk(textBefore, before);
    SideName_unk(textAfter, after);
    if (back == 0) {
        SetPaneText_unk(&m88->mLayout, "conf_before", textBefore);
        SetPaneText_unk(&m88->mLayout, "conf_after(1)", textAfter);
        m88->mFrame = 0.0f;
        Play_unk(m88, 0, 1.0f);
    } else {
        SetPaneText_unk(&m88->mLayout, "conf_before", textAfter);
        SetPaneText_unk(&m88->mLayout, "conf_after(1)", textBefore);
        m88->mFrame = 10.0f;
        Play_unk(m88, 0, -1.0f);
    }
}

// NONMATCHING: 99.6% - the task of Play_unk in r20 instead of r21
// inferred: fills the rows of the ranking with `count` entries of the list `list` from `first`
void D2AMpMenu::fn_804B3EFC(int list, int first, int count) {
    mCC = -1;
    int row = 0;
    for (int i = first; i < first + count; i++, row++) {
        if (list >= 0 && mLists[list]->Get(i)->m48 != 0) {
            RANKING_DATA* data = mLists[list]->Get(i);
            m8C[row]->SetNumber_unk("string0006(1)", data->m00, 0);
            SetPaneText_unk(&m8C[row]->mLayout, "string0028(1)", data->mName);
            SetTaskText_unk(m8C[row], "string0029(1)", fn_804FA8E8(lbl_8087F610, data->m04));
            m8C[row]->SetNumber_unk("string0027(1)", data->m04, 0);
            Play_unk(m8C[row], 0, 1.0f);
            if (data->m4C == lbl_8087F628->mE38) {
                mCC = row;
            }
        } else {
            m8C[row]->SetNumber_unk("string0006(1)", i + 1, 0);
            const wchar_t* empty = L"";
            SetPaneText_unk(&m8C[row]->mLayout, "string0028(1)", empty);
            SetPaneText_unk(&m8C[row]->mLayout, "string0029(1)", empty);
            SetPaneText_unk(&m8C[row]->mLayout, "string0027(1)", empty);
            Stop_unk(m8C[row]);
        }
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
const wchar_t* D2AMpMenu::fn_804B424C(int rule, int value) {
    int base = 0;
    if (rule == 2 && value == 4) {
        return L"";
    }
    if (rule == 9 && value == 4) {
        return L"";
    }
    if (rule == 10 && value == 2) {
        return L"";
    }
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

// NONMATCHING: 98.3% - the original sets the row offset (r30) before reading the count, and register of the
// count
// inferred: reads the colours of the marks and the rows of the rank table
void D2AMpMenuRankMark::fn_804B4400() {
    int count;
    int i;
    int numColors = 0;
    atn::Csv csv((const char*)mTable.fn_80470580(), mTable.fn_8047059C());
    const char* field = csv.fn_8005B9CC();
    do {
        int r = atoi(field);
        int g = atoi(csv.fn_8005B9CC());
        int b = atoi(csv.fn_8005B9CC());
        const char* kind = csv.fn_8005B9CC();
        if (strcmp(kind, "base") == 0) {
            mColors[numColors][0] = (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF);
        } else if (strcmp(kind, "pattern") == 0) {
            mColors[numColors][1] = (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF);
        } else if (strcmp(kind, "sword") == 0) {
            mColors[numColors][2] = (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF);
            numColors++;
        }
        csv.fn_8005BBF8();
    } while (numColors < 8 && strcmp(field = csv.fn_8005B9CC(), "rank_num") != 0);

    i = 0;
    count = atoi(csv.fn_8005B9CC());
    mRows.Resize_unk(count);
    while (csv.fn_8005BBF8() != 0) {
        Entry* e = &mRows.mEntries[i];
        e->mColor = atoi(csv.fn_8005B9CC());
        e->m04 = atoi(csv.fn_8005B9CC());
        e->m08 = atoi(csv.fn_8005B9CC());
        e->m0C = atoi(csv.fn_8005B9CC());
        i++;
        if (i >= count) {
            break;
        }
    }
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

// inferred: shows the pattern, the sword and the stars of the rank of the experience `exp` (own
// experience if negative) in the mark `index`
void D2AMpMenuRankMark::fn_804B47E0(int exp, int index) {
    if (exp < 0) {
        exp = lbl_8087F628->m430.m7F8;
    }
    unsigned int row = fn_805075C4(&lbl_8087F610->m610, exp)->m00;
    char name[64] = {0};
    if (index > 3) {
        index = 3;
    }
    if (row >= mRows.mCount) {
        row = mRows.mCount - 1;
    }
    SetPaneFloat_unk(&mMarks[index]->mLayout, "patternIndex", (float)(unsigned int)mRows.mEntries[row].m04);
    SetPaneFloat_unk(&mMarks[index]->mLayout, "swordIndex", (float)(unsigned int)(mRows.mEntries[row].m08 + 3));
    for (int i = 0; i < 4; i++) {
        sprintf(name, "drawFlg%d", i + 1);
        if ((unsigned int)i < mRows.mEntries[row].m0C) {
            SetPaneFloat_unk(&mMarks[index]->mLayout, name, 1.0f);
        } else {
            SetPaneFloat_unk(&mMarks[index]->mLayout, name, 0.0f);
        }
    }
}

void D2AMpMenuRankMark::fn_804B49C0(MpRect_unk rect, int index) {
    fn_801F4728(mMarks[index], "pos0026(1)", &rect);
}

// inferred: colours the mark `index` with the colours of the rank of the experience `exp`
void D2AMpMenuRankMark::fn_804B49E0(int exp, int index) {
    unsigned int row = fn_805075C4(&lbl_8087F610->m610, exp)->m00;
    if (row >= mRows.mCount) {
        row = mRows.mCount - 1;
    }
    int color = mRows.mEntries[row].mColor;
    mMarks[index]->SetColor_unk("col0009(1)(1)", mColors[color][0]);
    mMarks[index]->SetColor_unk("col0021(1)(1)", mColors[color][1]);
    mMarks[index]->SetColor_unk("col1", mColors[color][2]);
    mMarks[index]->SetColor_unk("col2", mColors[color][2]);
    mMarks[index]->SetColor_unk("col3", mColors[color][2]);
    mMarks[index]->SetColor_unk("col4", mColors[color][2]);
}
} // namespace MP
