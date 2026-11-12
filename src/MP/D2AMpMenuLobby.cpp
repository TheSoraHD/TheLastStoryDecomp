// MP/D2AMpMenuLobby.cpp (0x804BBBA0-0x804C2FF8): lobby of a multiplayer game. Only the first functions are
// reconstructed so far.
#include "MP/D2AMpMenuLobby.h"
#include "MP/D2AMpMenuSetRule.h"
#include "atn/FadeCommon.h"
#include "MP/D2ACommonDialog.h"
#include "game/MenuCommonTask.h"
#include "game/GameConfig_unk.h"
#include "MP/D2AMpMenuMessageList.h"
#include "MP/D2AMpMenu.h"
#include "atn/DrawHelper.h"
#include "atn/Color.h"
#include "atn/net/Manager.h"
#include "MP/MpLevel_unk.h"
#include "game/CharaTask.h"

// inferred: hash of the "CustomData" pane (as in MP/D2AMpMenu.cpp; read by functions not written yet); its
// initializer is the __sinit of the file (0x804C2FC8)
extern "C" unsigned int fn_800DC6B4(const char* s);
static const unsigned int sCustomData = fn_800DC6B4("CustomData");

// inferred: player information returned by fn_804EB870 (local player); only the member used here
struct MpLobbyPlayerInfo_unk {
    unsigned char m00[0xB0];
    int mB0;
    unsigned char mB4[0xD0 - 0xB4];
    unsigned int mD0_0 : 3;
    unsigned int mD0_3 : 1;   // inferred: ready
    unsigned int mD0_4 : 2;
    unsigned int mD0_6 : 4;   // inferred: face
    unsigned int mD0_10 : 12;
    unsigned int mD0_22 : 4;  // inferred: map voted by the player
    unsigned int mD0_26 : 4;  // inferred: entry of the map chosen by the lottery
    unsigned int mD0_30 : 2;
};
// inferred: one player of the session (0xD5C bytes)
struct MpLobbyMember_unk {
    unsigned char m000[0xB0];
    int mB0;                  // inferred: character
    unsigned char mB4[0xCC - 0xB4];
    unsigned char mCC;        // inferred: aid
    unsigned char mCD[0xD0 - 0xCD];
    unsigned int mD0_31 : 1;  // inferred: valid
    unsigned int mD0_1 : 1;   // inferred: the local player
    unsigned int mD0_2 : 1;
    unsigned int mD0_3 : 1;   // inferred: ready (shows the ok mark of the lobby)
    unsigned int mD0_4 : 2;
    unsigned int mD0_6 : 4;   // inferred: team (versus)
    unsigned int mD0_10 : 10;
    unsigned int mD0_20 : 2;  // inferred: zero while still loading
    unsigned int mD0_22 : 4;  // inferred: map voted by the player
    unsigned int mD0_26 : 4;  // inferred: entry of the map chosen by the lottery (host)
    unsigned int mD0_30 : 2;
    unsigned char mD4[0xD5C - 0xD4];
};
// inferred: multiplayer session manager (lbl_8087F610, a task); only what the lobby uses here
struct MpLobbySession_unk : public atn::Task {
    // inferred: player of the session with an aid
    MpLobbyMember_unk* FindMember_unk(unsigned int aid) {
        for (int j = 0; j < m5E8; j++) {
            MpLobbyMember_unk* m = &m5E4[j];
            if (m->mD0_31 == 1 && aid == m->mCC) {
                return m;
            }
        }
        return 0;
    }
    unsigned char m048[0x508 - 0x48];
    short m508;               // inferred: time left (-1: none)
    unsigned char m50A[0x514 - 0x50A];
    int m514;                 // inferred: nonzero once the play time is known (fn_804E4A20)
    unsigned char m518[0x540 - 0x518];
    int m540;                 // inferred: game mode (0: raid, 1: versus)
    unsigned char m544[0x55C - 0x544];
    int m55C;
    unsigned char m560[0x564 - 0x560];
    int m564;                 // inferred: time limit in seconds (-1: none)
    unsigned char m568[0x5A4 - 0x568];
    int m5A4;                 // inferred: shown in "string0007(1)" of the base
    unsigned char m5A8[0x5B0 - 0x5A8];
    int m5B0;                 // inferred: on/off rule shown in "string0011(1)" of the base
    unsigned char m5B4;       // inferred: debug flags (4: everybody warrior, 8: everybody mage)
    unsigned char m5B5[0x5E4 - 0x5B5];
    MpLobbyMember_unk* m5E4; // inferred: players
    int m5E8;                // inferred: number of players
    unsigned char m5EC[0x610 - 0x5EC];
    MpLevelTable_unk m610;   // inferred: experience table
    unsigned char m618[0x999C - 0x618];
    unsigned char m999C;     // inferred: set once the game starts
};
extern "C" MpLobbySession_unk* lbl_8087F610;
extern "C" MpLobbyPlayerInfo_unk* fn_804EB870(MpLobbySession_unk* s); // inferred: local player
extern "C" int DWC_GetNumConnectionHost();
extern "C" int fn_806A8E70();
extern "C" void fn_804DDA04(MpLobbySession_unk* s); // inferred: sends the game mode
extern "C" int fn_804E4A20(); // inferred: play time of the player
extern "C" int fn_804EB26C(MpLobbySession_unk* s, int* count); // inferred: nonzero when everybody answered
extern "C" int fn_804FC010(MpLobbySession_unk* s);              // inferred: nonzero while a request is pending
extern "C" int fn_804DE560(MpLobbySession_unk* s, int arg, unsigned char* ok); // inferred: ready state of the room
extern "C" int sprintf(char* s, const char* format, ...);
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c); // inferred: cursor
extern "C" const wchar_t* fn_804FA7C8(MpLobbySession_unk* s, int mode, int map); // inferred: name of a map
// inferred: list of the connected aids
struct MpLobbyAids_unk {
    unsigned char* mAids;
};
extern "C" int fn_804EB1AC(MpLobbySession_unk* s); // inferred: nonzero for the host
extern "C" void fn_804A5D34(MenuCommonTask* menu, int id, const wchar_t* desc0, const wchar_t* desc1,
                            const wchar_t* desc2); // inferred: help text (three lines)
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int arg); // inferred: help text
extern "C" void fn_804DD9CC(MpLobbySession_unk* s, MpLobbyPlayerInfo_unk* player, int face); // inferred: sets the face (host)
extern "C" unsigned int fn_804FAB40(MpLobbySession_unk* s, int face, MpLobbyPlayerInfo_unk* player, int* id, int arg); // inferred: requests the face
extern "C" int fn_804F5E38(MpLobbySession_unk* s, unsigned char id);  // inferred: state of a request
extern "C" int* fn_804F5B08(MpLobbySession_unk* s, unsigned char id); // inferred: result of a request
extern "C" void fn_804F5C9C(MpLobbySession_unk* s, unsigned char id); // inferred: frees a request
extern "C" GameConfig_unk* lbl_8087F0A8;
extern "C" void fn_804A436C(int* cursor, int count, int arg2, int wrap, int sound); // inferred: moves a cursor
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task
extern "C" int fn_8050BA68(MpLobbyAids_unk* aids); // inferred: number of aids
extern "C" int fn_804EA5DC(MpLobbySession_unk* s, int mode); // inferred: number of maps of a game mode
extern "C" void fn_804EA534(MpLobbySession_unk* s, int map, int host); // inferred: sets the map
extern "C" int rand();
extern "C" CharaInfo_unk* fn_8020924C(int id); // inferred: character table entry
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" const wchar_t* fn_804FA88C(MpLobbySession_unk* s, int value); // inferred: rank name
extern "C" int fn_804EB2D8(MpLobbySession_unk* s);
extern "C" int fn_804EAFD0(MpLobbySession_unk* s);              // inferred: number of players ready
extern "C" int fn_804DDE34(MpLobbySession_unk* s, int mode);    // inferred: sends the start of the game
extern "C" void fn_804E8A80(MpLobbySession_unk* s, int id);     // inferred: debug command              // inferred: nonzero once the game starts
extern "C" int fn_804DBD08(MpLobbySession_unk* s, int mode);
extern "C" void fn_804FB470(MpLobbySession_unk* s, int code, int arg); // inferred: error of the session
extern "C" void fn_804DD33C(MpLobbySession_unk* s, int arg, int* value); // inferred: next character
extern "C" void fn_804DD294(MpLobbySession_unk* s, int arg, int* value); // inferred: previous character
extern "C" void fn_804FA948(MpLobbySession_unk* s, int value, unsigned int flush); // inferred: requests the character

// inferred: float values of panes given as integers (converted after the hash of the name)
static inline void SetPaneInt_unk(void* layout, const char* name, int value) {
    fn_801FECE0(layout, fn_800DC6B4(name), value);
}
static inline void SetPaneValue2Int_unk(void* layout, const char* name, int index, int value) {
    fn_801FEDBC(layout, fn_800DC6B4(name), index, value);
}

namespace MP {

D2AMpMenuLobby* D2AMpMenuLobby::sInstance;

D2AMpMenuLobby* D2AMpMenuLobby::fn_804BBBA0(atn::Task* parent, int a, int b) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) D2AMpMenuLobby(parent, a, b);
    }
    return sInstance;
}

D2AMpMenuLobby::D2AMpMenuLobby(atn::Task* parent, int a, int b)
    : atn::Task(parent), mD88(0), mD8C(0), mD90(0), mD98(0), mD9C(0), mDA0(0), mDA4(0), mDA8(0), mDAC(0), mDB0(0),
      mDB4(0), mDCC(0), mDD0(0), mE58(-1), mE5C(0), mE60(0), mE64(-1), mE68(-1), mE78(0x7FFFFFFF), mE7C(0),
      mF90(0) {
    mE74 = b;
    m4C = fn_801F3FF8(this, "/data/d2anime/online_raid_base.d2a", 0);
    Add_unk(m4C);
    m50 = fn_801F3FF8(this, "/data/d2anime/online_raid_time.d2a", 0);
    Add_unk(m50);
    m54 = fn_801F3FF8(this, "/data/d2anime/online_raid_logo.d2a", 0);
    Add_unk(m54);
    m58 = fn_801F3FF8(this, "/data/d2anime/online_versus_logo.d2a", 0);
    Add_unk(m58);
    m64 = fn_801F3FF8(this, "/data/d2anime/online_info_start.d2a", 0);
    Add_unk(m64);
    m6C = fn_801F3FF8(this, "/data/d2anime/online_map_logo.d2a", 0);
    Add_unk(m6C);
    m70 = fn_801F3FF8(this, "/data/d2anime/online_map_base.d2a", 0);
    Add_unk(m70);
    m74 = fn_801F3FF8(this, "/data/d2anime/online_map_list9_base.d2a", 0);
    Add_unk(m74);
    m78 = fn_801F3FF8(this, "/data/d2anime/online_map_list5_base.d2a", 0);
    Add_unk(m78);
    m68 = fn_801F3FF8(this, "/data/d2anime/online_connect_base.d2a", 0);
    Add_unk(m68);
    for (int i = 0; i < 6; i++) {
        mPlayers[i].mTasks[0] = fn_801F3FF8(this, "/data/d2anime/online_match_profile.d2a", 0);
        Add_unk(mPlayers[i].mTasks[0]);
        mPlayers[i].mTasks[1] = fn_801F3FF8(this, "/data/d2anime/online_face_mark.d2a", 0);
        Add_unk(mPlayers[i].mTasks[1]);
        mPlayers[i].mTasks[2] = fn_801F3FF8(this, "/data/d2anime/online_match_wait.d2a", 0);
        Add_unk(mPlayers[i].mTasks[2]);
        mPlayers[i].mTasks[3] = fn_801F3FF8(this, "/data/d2anime/online_fukidasi.d2a", 0);
        Add_unk(mPlayers[i].mTasks[3]);
        mPlayers[i].mTasks[4] = fn_801F3FF8(this, "/data/d2anime/online_fukidasi2.d2a", 0);
        Add_unk(mPlayers[i].mTasks[4]);
        mPlayers[i].mTasks[5] = fn_801F3FF8(this, "/data/d2anime/online_player_ok.d2a", 0);
        Add_unk(mPlayers[i].mTasks[5]);
    }
    m5C = fn_801F3FF8(this, "/data/d2anime/online_face_select.d2a", 0);
    Add_unk(m5C);
    SetAll_unk(1);
    SetSpeedAll_unk(0.0f);
    if (a != 0) {
        mE6C = 0;
        mE70 = 0;
    } else {
        MpLobbyPlayerInfo_unk* player = fn_804EB870(lbl_8087F610);
        if (player != 0) {
            mE6C = player->mB0;
            mE70 = player->mB0;
        } else {
            mE6C = 0;
            mE70 = 0;
        }
    }
    for (int i = 0; i < 4; i++) {
        mDBC[i] = -1;
    }
    D2AMpMenuSetRule::fn_804CD97C(this);
    m7C = atn::FadeColor::Create(lbl_8087F610, 15, 0, 0xFF000000);
    m7C->mZ = -100.0f;
}

D2AMpMenuLobby::~D2AMpMenuLobby() {
    mTasks.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

// inferred: number of connected hosts
static inline int GetHostCount_unk() {
    atn::net::Manager* data = lbl_8087F628;
    if (data->mC215 == 0) {
        return data->mFlags.mB12 ? 1 : DWC_GetNumConnectionHost();
    }
    return data->mFlags.mB12 ? 1 : fn_806A8E70();
}

// inferred: index of a connected host by its aid (-1: none)
static inline int GetAidIndex_unk(unsigned int aid) {
    if (lbl_8087F628->mFlags.mB12) {
        return 0;
    }
    MpLobbyAids_unk aids;
    int count = fn_8050BA68(&aids);
    for (int i = 0; i < count; i++) {
        if (aid == aids.mAids[i]) {
            return i;
        }
    }
    return -1;
}

// inferred: aid of the i-th connected host (0xFF: none)
static inline unsigned int GetAid_unk(int i) {
    if (lbl_8087F628->mFlags.mB12) {
        return 0;
    }
    MpLobbyAids_unk aids;
    if (fn_8050BA68(&aids) > i) {
        return aids.mAids[i];
    }
    return 0xFF;
}

// NONMATCHING: volatile registers of the member search (aid r3/r4, offset r5/r3, member r4/r5)
int D2AMpMenuLobby::vf0C() {
    if (fn_800D3FA4() != 0) {
        int ready;
        for (int i = 0; i < GetHostCount_unk(); i++) {
            MpLobbyMember_unk* member = lbl_8087F610->FindMember_unk(GetAid_unk(i));
            if (member == 0) {
                ready = 0;
                goto done;
            }
            if (member->mD0_31 != 1) {
                ready = 0;
                goto done;
            }
        }
        ready = 1;
    done:
        if (ready != 0) {
            SetAll_unk(0);
            mDA0 = 1;
            SetPaneFloat_unk(m70->mLayout, "zoomFrame", 0.0f);
            fn_804BC628(1);
            return 1;
        }
    }
    return 0;
}

void D2AMpMenuLobby::vf1C() {}

void D2AMpMenuLobby::vf20() {}

void D2AMpMenuLobby::fn_804BD9B8() {
    int mode = D2AMpMenu::sInstance->mC4;
    switch (mD8C) {
    case 2:
        D2ACommonDialog::sInstance->fn_804ACAF8();
        D2ACommonDialog::sInstance->fn_804ACDBC();
        break;
    case 3:
        D2ACommonDialog::sInstance->fn_804ACAF8();
        D2ACommonDialog::sInstance->fn_804ACDBC();
        break;
    case 4:
        Stop_unk(m5C);
        break;
    case 5:
        vf1C();
        fn_804BF550();
        Play_unk(m4C, 0, 1.0f);
        Play_unk(m50, 0, 1.0f);
        if (mode == 2) {
            Play_unk(m54, 0, 1.0f);
        } else {
            Play_unk(m58, 0, 1.0f);
        }
        Play_unk(m64, 0, 1.0f);
        for (int i = 0; i < 6; i++) {
            if (mPlayers[i].m18 != 0) {
                Play_unk(mPlayers[i].mTasks[2], 0, 1.0f);
                Stop_unk(mPlayers[i].mTasks[3]);
                Stop_unk(mPlayers[i].mTasks[4]);
            }
        }
        for (int i = 0; i < 6; i++) {
            if (mPlayers[i].m20 > 0) {
                if (mPlayers[i].m28 == 0) {
                    Play_unk(mPlayers[i].mTasks[3], 0, 1.0f);
                } else {
                    Play_unk(mPlayers[i].mTasks[4], 0, 1.0f);
                }
            }
        }
        break;
    case 10:
        m68->mFlags |= 4;
        Stop_unk(m68);
        Stop_unk(m5C);
        break;
    case 7:
        if (D2AMpMenuMessageList::sInstance != 0 && (unsigned int)D2AMpMenuMessageList::sInstance->m94 <= 2) {
            D2AMpMenuMessageList::sInstance->fn_804C35F0(3);
        }
        mDCC = 30;
        break;
    }
}

int D2AMpMenuLobby::fn_804BE330(int count) {
    void* pad = lbl_8087EF70;
    int decide = 0;
    if (mD88 == 7) {
        fn_804A436C(&mDB4, count, count, 1, 3);
    }
    if (fn_800A555C(pad, 0, 4) != 0) {
        fn_80117228(1);
        decide = 1;
    } else if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80117228(2);
        decide = 2;
    }
    if (decide == 0 && D2AMpMenuMessageList::sInstance != 0) {
        short time = lbl_8087F610->m508;
        if (time == -1 || time > 90) {
            if (D2ACommonDialog::sInstance->m4C == 0 || D2ACommonDialog::sInstance->fn_804AC734() != 0) {
                if (lbl_8087F0A8->mInput.fn_801231D0(0x34) != 0 && (mD88 == 4 || mD88 == 6)) {
                    fn_80117228(10);
                    D2AMpMenuMessageList::sInstance->fn_804C35F0(1);
                    D2AMpMenuMessageList::sInstance->fn_804C38C0(0);
                }
            }
        }
    }
    return decide;
}

// inferred: answer of the common dialog: "yes" switches the game mode (raid/versus) and enters the state mD8C,
// "no" enters it as it is; while the time left is under 90 the dialog is closed when the other players left
// NONMATCHING: 99.9% - offset of "col0005" until the strings of the functions before it are written
void D2AMpMenuLobby::fn_804BE4A4() {
    D2ACommonDialog* dialog = D2ACommonDialog::sInstance;
    if (dialog->m4C == 0) {
        return;
    }
    if (dialog->fn_804AC79C()) {
        MpLobbySession_unk* session = lbl_8087F610;
        if (session->m540 == 0) {
            session->m540 = 1;
            D2AMpMenu::sInstance->mC4 = 1;
            if (lbl_8087F628->m25C == 1) {
                fn_804DDA04(lbl_8087F610);
            }
        } else if (session->m540 == 1) {
            session->m540 = 0;
            D2AMpMenu::sInstance->mC4 = 3;
            SetPaneValue2_unk(m64->mLayout, "col0005", 0, 0.0f);
        }
        fn_804BC628(mD8C);
    } else if (D2ACommonDialog::sInstance->fn_804AC7EC()) {
        fn_804BC628(mD8C);
    } else if (lbl_8087F610->m508 < 90 && GetHostCount_unk() != 1) {
        D2ACommonDialog::sInstance->fn_804AD738();
    }
}

// inferred: true while a connected player is still loading (guests only)
static inline bool IsLoading_unk() {
    if (fn_804EB1AC(lbl_8087F610) != 0) {
        return false;
    }
    for (int i = 0; i < GetHostCount_unk(); i++) {
        MpLobbyMember_unk* member = lbl_8087F610->FindMember_unk(GetAid_unk(i));
        if (member != 0 && member->mD0_31 == 1 && member->mD0_20 == 0) {
            return true;
        }
    }
    return false;
}

// inferred: a player is still loading: leaves the session when not ready yet, otherwise waits (state 10)
static inline void WaitLoading_unk(MP::D2AMpMenuLobby* lobby) {
    if (fn_804EB870(lbl_8087F610) == 0 || (fn_804EB870(lbl_8087F610)->mD0_3 == 0 && lobby->mE7C < 30)) {
        fn_804DBD08(lbl_8087F610, 0);
        fn_804FB470(lbl_8087F610, 0xA1, 9);
        return;
    }
    fn_80117228(6);
    lobby->fn_804BC628(10);
    if (MP::D2AMpMenuMessageList::sInstance != 0) {
        MP::D2AMpMenuMessageList::sInstance->fn_804C35F0(3);
    }
}

// inferred: update of the lobby: warns the guests when the time runs out, follows the rules of the host, places
// the parts, reads the cursor (decide: state 6, cancel: state 2), waits for the players still loading, and
// changes the character (left/right) or the voted map (up/down)
// NONMATCHING: volatile registers of the member search (offset r3/r5, member r5/r3), as vf0C
void D2AMpMenuLobby::fn_804BE610() {
    void* pad = lbl_8087EF70;
    MpLobbyPlayerInfo_unk* player = fn_804EB870(lbl_8087F610);
    if (fn_804EB1AC(lbl_8087F610) == 0) {
        short time = lbl_8087F610->m508;
        if (time > mE78 && time <= 900 && fn_804EB2D8(lbl_8087F610) == 0 &&
            (D2ACommonDialog::sInstance->m4C == 0 || D2ACommonDialog::sInstance->fn_804AC734() != 0)) {
            D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
            D2ACommonDialog::sInstance->fn_804ACD10();
            D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xF2));
            Play_unk(m64, 0, -1.0f);
            mDAC = 1;
        }
        mE78 = lbl_8087F610->m508;
    }
    if (fn_804EB1AC(lbl_8087F610) == 0) {
        fn_804C0094();
    } else {
        short time = lbl_8087F610->m508;
        if ((time == -1 || time > 900 || GetHostCount_unk() == 1) && lbl_8087F0A8->mInput.fn_801231D0(0x35) != 0) {
            if (lbl_8087F610->m540 != 2) {
                fn_804BC628(3);
                fn_80117228(10);
            }
            return;
        }
    }
    MpD2A_unk* cursor = m5C;
    if (cursor->mFrame >= 10.0f) {
        Play_unk(cursor, 0, 1.0f);
        m5C->mFrame = 5.0f;
    }
    fn_804C18A8();
    fn_804BFB54();
    fn_804BF550();
    fn_804BF9AC();
    if (mE68 != -1 && mE68 == player->mB0) {
        mE68 = -1;
    }
    if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() != 0) {
        D2ACommonDialog::sInstance->fn_804ACAF8();
        D2ACommonDialog::sInstance->fn_804ACDBC();
        Play_unk(m64, 0, 1.0f);
    }
    int decide = fn_804BE330(0);
    if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() == 0) {
        decide = 0;
    }
    if (decide == 1) {
        fn_804BC628(6);
    } else if (decide == 2) {
        short time = lbl_8087F610->m508;
        if (time == -1 || time > 90 || GetHostCount_unk() == 1) {
            fn_804BC628(2);
            return;
        }
    }
    if (IsLoading_unk()) {
        WaitLoading_unk(this);
        return;
    }
    if (lbl_8087F610->m540 == 1) {
        fn_804C0548();
    }
    if (player == 0) {
        return;
    }
    int* chara = &mE6C;
    int old = *chara;
    if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
        fn_804DD33C(lbl_8087F610, 0, chara);
        mDBC[0] = 0;
    } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
        fn_804DD294(lbl_8087F610, 0, chara);
        mDBC[1] = 0;
    } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
        int time = lbl_8087F610->m508;
        if ((unsigned short)time > 90) {
            mE5C++;
            if (mE5C > fn_804EA5DC(lbl_8087F610, lbl_8087F610->m540)) {
                mE5C = 0;
            }
            fn_804EB870(lbl_8087F610)->mD0_22 = mE5C;
            mDBC[3] = 0;
            fn_80117228(3);
        }
    } else if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
        int time = lbl_8087F610->m508;
        if ((unsigned short)time > 90) {
            if (--mE5C < 0) {
                mE5C = fn_804EA5DC(lbl_8087F610, lbl_8087F610->m540);
            }
            fn_804EB870(lbl_8087F610)->mD0_22 = mE5C;
            mDBC[2] = 0;
            fn_80117228(3);
        }
    }
    if (old != *chara) {
        if (fn_804EB1AC(lbl_8087F610) == 1) {
            player->mB0 = *chara;
        } else {
            mE68 = *chara;
            fn_804FA948(lbl_8087F610, *chara, 1);
        }
        fn_80117228(3);
    }
}

// inferred: the time is running out: tells the guests (message 0xF2) and remembers it (mDAC)
static inline void ShowTimeWarning_unk(MP::D2AMpMenuLobby* lobby) {
    MP::D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
    MP::D2ACommonDialog::sInstance->fn_804ACD10();
    MP::D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xF2));
    MP::Play_unk(lobby->m64, 0, -1.0f);
    lobby->mDAC = 1;
}

// inferred: the host starts the game
static inline bool StartGame_unk() {
    if (fn_804DDE34(lbl_8087F610, MP::D2AMpMenu::sInstance->mC4) == 0) {
        return false;
    }
    lbl_8087F610->m999C = 1;
    lbl_8087F628->fn_8050A4E0(1);
    return true;
}

// inferred: update of state 6 (waiting for the other players): the host starts the game once everybody is
// ready (or the time runs out); the guests are warned when the time runs out
// NONMATCHING: the original reloads lbl_8087F610 after the check of the time; volatile registers of the member
// search (as vf0C)
void D2AMpMenuLobby::fn_804BEDB8() {
    void* pad = lbl_8087EF70;
    if (fn_804EB1AC(lbl_8087F610) == 0) {
        fn_804C0094();
    }
    fn_804C18A8();
    fn_804BFB54();
    fn_804BF550();
    fn_804BF9AC();
    fn_804EB26C(lbl_8087F610, 0);
    if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() != 0) {
        D2ACommonDialog::sInstance->fn_804ACAF8();
        D2ACommonDialog::sInstance->fn_804ACDBC();
        Play_unk(m64, 0, 1.0f);
        if (mDAC != 0) {
            mDAC = 0;
            fn_804BC628(4);
            return;
        }
    }
    int decide = fn_804BE330(0);
    if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() == 0) {
        decide = 0;
    }
    if (lbl_8087F610->m508 == 0) {
        decide = 1;
    }
    if (fn_804FC010(lbl_8087F610) != 0 && decide == 1) {
        decide = 0;
    }
    if (fn_804EB1AC(lbl_8087F610) == 0) {
        short time = lbl_8087F610->m508;
        if (time > mE78 && time <= 900 && fn_804EB2D8(lbl_8087F610) == 0 &&
            (D2ACommonDialog::sInstance->m4C == 0 || D2ACommonDialog::sInstance->fn_804AC734() != 0)) {
            ShowTimeWarning_unk(this);
        }
        mE78 = lbl_8087F610->m508;
    }
    if (decide == 1) {
        if (fn_804EB1AC(lbl_8087F610) != 0) {
            bool start = false;
            if (GetHostCount_unk() > 1 || lbl_8087F628->mC215 != 0) {
                if (lbl_8087F610->m508 == 0) {
                    if (fn_804EB2D8(lbl_8087F610) == 0) {
                        lbl_8087F610->m508 = 900;
                        if (D2ACommonDialog::sInstance->m4C == 0 || D2ACommonDialog::sInstance->fn_804AC734() != 0) {
                            ShowTimeWarning_unk(this);
                        }
                    } else if (StartGame_unk() == true) {
                        start = true;
                    } else {
                        lbl_8087F610->m508 = 900;
                    }
                } else if (fn_804EB2D8(lbl_8087F610) == 1 && fn_804EB26C(lbl_8087F610, 0) == 1) {
                    int count = GetHostCount_unk();
                    if (count == fn_804EAFD0(lbl_8087F610) && StartGame_unk() == true) {
                        start = true;
                    }
                }
            }
            if (start == true) {
                fn_804BC628(10);
            }
        }
    } else if (decide == 2) {
        fn_804BC628(4);
    }
    if (D2AMpMenu::sInstance->mC4 != 2) {
        if (fn_800A555C(pad, 0, 2) != 0) {
            fn_804E8A80(lbl_8087F610, 0xDF);
        }
        if (fn_800A555C(pad, 0, 3) != 0) {
            fn_804E8A80(lbl_8087F610, 0xE0);
        }
    }
    if (IsLoading_unk()) {
        WaitLoading_unk(this);
        return;
    }
    if (D2AMpMenu::sInstance->mC4 == 1 && fn_804EB26C(lbl_8087F610, 0) != 0 && fn_804EB2D8(lbl_8087F610) == 0 &&
        (D2ACommonDialog::sInstance->m4C == 0 || D2ACommonDialog::sInstance->fn_804AC734() != 0)) {
        ShowTimeWarning_unk(this);
    }
}

// inferred: colors of the buttons of the face select: "start" (col0003) lit for the host when the players can
// start, col0004 always lit, the index of the local player, and col0005 while the face can be changed
// NONMATCHING: 99.7% - @stringBase0 offsets (pool order)
void D2AMpMenuLobby::fn_804BF550() {
    if (fn_804EB1AC(lbl_8087F610) != 0) {
        if (mD88 == 6) {
            if (fn_804FC010(lbl_8087F610)) {
                SetPaneValue2_unk(m64->mLayout, "col0003", 0, 0.0f);
                mDB0 = 5;
            } else {
                int count;
                if (fn_804EB26C(lbl_8087F610, &count) && mDB0 <= 0 && count > 1 &&
                    (GetHostCount_unk() > 1 || lbl_8087F628->mC215 != 0)) {
                    SetPaneValue2_unk(m64->mLayout, "col0003", 0, 255.0f);
                } else {
                    SetPaneValue2_unk(m64->mLayout, "col0003", 0, 0.0f);
                }
                if (mDB0 > 0) {
                    mDB0--;
                }
            }
        } else {
            unsigned char ok = 1;
            if (fn_804DE560(lbl_8087F610, 0, &ok) == 0 && ok == 0 && GetHostCount_unk() > 1 &&
                lbl_8087F610->m540 != 2) {
                SetPaneValue2_unk(m64->mLayout, "col0003", 0, 255.0f);
            } else if (lbl_8087F610->m540 != 2 &&
                       (lbl_8087F610->m508 == -1 || lbl_8087F610->m508 > 900 || GetHostCount_unk() == 1)) {
                SetPaneValue2_unk(m64->mLayout, "col0003", 0, 255.0f);
            } else {
                SetPaneValue2_unk(m64->mLayout, "col0003", 0, 0.0f);
            }
        }
    } else {
        SetPaneValue2_unk(m64->mLayout, "col0003", 0, 0.0f);
    }
    SetPaneValue2_unk(m64->mLayout, "col0004", 0, 255.0f);
    SetPaneFloat_unk(m64->mLayout, "index", (float)mE60);
    if (mD88 == 4 && D2AMpMenu::sInstance->mC4 == 1 && lbl_8087F628->m25C == 1) {
        SetPaneValue2_unk(m64->mLayout, "col0005", 0, 255.0f);
    } else {
        SetPaneValue2_unk(m64->mLayout, "col0005", 0, 0.0f);
    }
}

// inferred: flashes four parts of the lobby (face select colors 4 and 1, colors 3 and 4 of the profile of the
// local player) while mDBC[i] counts from 0 to 10 (up to 0xFF and back, 0x33 per frame)
// NONMATCHING: 99.8% - offsets of the strings until the functions before it are written
void D2AMpMenuLobby::fn_804BF9AC() {
    for (int i = 0; i < 4; i++) {
        if (mDBC[i] == -1) {
            continue;
        }
        float value = (float)(mDBC[i] <= 5 ? mDBC[i] * 0x33 : 0xFF - (mDBC[i] - 5) * 0x33);
        if (i == 0) {
            SetPaneValue2_unk(m5C->mLayout, "col0004(1)", 0, value);
        } else if (i == 1) {
            SetPaneValue2_unk(m5C->mLayout, "col0001(1)", 0, value);
        } else if (i == 2) {
            SetPaneValue2_unk(mPlayers[mE60].mTasks[0]->mLayout, "col0003(1)", 0, value);
        } else if (i == 3) {
            SetPaneValue2_unk(mPlayers[mE60].mTasks[0]->mLayout, "col0004(1)", 0, value);
        }
        if (++mDBC[i] > 10) {
            mDBC[i] = -1;
        }
    }
}

// inferred: places the parts of the slot on the pane rect `base` (the wait mark, or the profile with the face
// mark, the two balloons and the ok mark); an inline member, so its literals are in .data
inline void D2AMpMenuLobby::Player_unk::SetPos_unk(const MpRect_unk& base) {
    MpRect_unk rect = base;
    if (m18 != 0) {
        fn_801F4728(mTasks[2], "pos0111(1)", &rect);
    } else {
        fn_801F4728(mTasks[0], "pos0001(1)(1)", &rect);
        rect = GetPaneRect_unk(mTasks[0], "rank_null");
        fn_801F4728(mTasks[1], "pos0026(1)", &rect);
        rect = GetPaneRect_unk(mTasks[0], "fukidasi_null");
        fn_801F4728(mTasks[3], "basePos", &rect);
        fn_801F4728(mTasks[4], "basePos", &rect);
        rect = base;
        fn_801F4728(mTasks[5], "pos0001(1)(1)", &rect);
    }
}

// inferred: places the slots of the players on the "profile_null0N" panes of the base: first the players
// (with the cursor of the face selection in state 4 and the end of the balloons), then the waiting slots
// NONMATCHING: offset of "profile_null0%d" in @stringBase0 until the functions before it are written
void D2AMpMenuLobby::fn_804BFB54() {
    char name[0x40] = {0};
    MpRect_unk rect;
    int n = 0;
    for (int i = 0; i < 6; i++) {
        if (mPlayers[i].m18 != 0) {
            continue;
        }
        sprintf(name, "profile_null0%d", n + 1);
        n++;
        rect = GetPaneRect_unk(m4C, name);
        mPlayers[i].SetPos_unk(rect);
        if (mD88 == 4 && i == mE60) {
            rect = GetPaneRect_unk(mPlayers[i].mTasks[0], "rank_null");
            fn_801F4728(m5C, "pos0003(1)", &rect);
        }
        if (mPlayers[i].m20 > 0) {
            if (--mPlayers[i].m20 <= 0) {
                mPlayers[i].m20 = 0;
                mPlayers[i].m24 = 0;
                Play_unk(mPlayers[i].mTasks[3], 0, -1.0f);
                Play_unk(mPlayers[i].mTasks[4], 0, -1.0f);
            }
        }
    }
    for (int i = n; i < 6; i++) {
        if (mPlayers[i].m18 == 0) {
            continue;
        }
        sprintf(name, "profile_null0%d", n + 1);
        n++;
        rect = GetPaneRect_unk(m4C, name);
        mPlayers[i].SetPos_unk(rect);
    }
}

// inferred: for the guests, follows the rules of the host: when the game mode changes, sets the mode of the menu,
// shows the message of the new mode and goes to state 4; then shows the rules on the base (time limit, healer,
// the on/off rule and the minutes of the time limit)
// NONMATCHING: 98.6% - registers of the message id (r4/r5) and of the layout/mode temporaries (r26/r27)
void D2AMpMenuLobby::fn_804C0094() {
    if (fn_804EB1AC(lbl_8087F610) == 1) {
        return;
    }
    int mode = lbl_8087F610->m540;
    if (mE74 != mode) {
        if (mode == 0) {
            D2AMpMenu::sInstance->mC4 = 3;
            if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() == 0) {
                D2ACommonDialog::sInstance->fn_804AD738();
            }
            D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
            D2ACommonDialog::sInstance->fn_804ACD10();
            D2ACommonDialog::sInstance->fn_804AD1EC(lbl_8087F86C->Get_unk(0xF9));
            fn_804BC628(4);
            Play_unk(m64, 0, -1.0f);
        } else if (mode == 1) {
            D2AMpMenu::sInstance->mC4 = 1;
            if (D2ACommonDialog::sInstance->m4C != 0 && D2ACommonDialog::sInstance->fn_804AC734() == 0) {
                D2ACommonDialog::sInstance->fn_804AD738();
            }
            D2ACommonDialog::sInstance->fn_804AC83C(0, 1.0f, 1);
            D2ACommonDialog::sInstance->fn_804ACD10();
            int id = 0xF4;
            if (lbl_8087F628->m25C == 1) {
                id = 0xF3;
            }
            const wchar_t* text = lbl_8087F86C->Get_unk(id);
            D2ACommonDialog::sInstance->fn_804AD1EC(text);
            fn_804BC628(4);
            Play_unk(m64, 0, -1.0f);
        }
        mE74 = lbl_8087F610->m540;
    }
    wchar_t text[32] = {0};
    int menuMode = D2AMpMenu::sInstance->mC4;
    int timeAlpha = (unsigned char)(lbl_8087F610->m564 != -1 ? 0xFF : 0);
    SetPaneValue2_unk(m4C->mLayout, "time_color", 0, timeAlpha);
    int healerAlpha = (unsigned char)(menuMode == 2 ? 0xFF : 0);
    SetPaneValue2_unk(m4C->mLayout, "healer_color", 0, healerAlpha);
    m4C->SetNumber_unk("string0007(1)", lbl_8087F610->m5A4, 0);
    SetPaneText_unk(m4C->mLayout, "string0011(1)", lbl_8087F610->m5B0 != 0 ? L"ON" : L"OFF");
    if (lbl_8087F610->m564 != -1) {
        fn_800DD3FC(text, L"%02d", lbl_8087F610->m564 / 60);
        SetPaneText_unk(m4C->mLayout, "string0014(1)", text);
    }
}

// inferred: before the countdown, waits for the base animation to go back to its first frame and enters the
// next state; then shows the seconds left on the map (30 - mDCC), fading out at 15 and beeping from 29
void D2AMpMenuLobby::fn_804C0430() {
    if (mDA4 == 0) {
        if (m4C->mFrame <= 0.0f) {
            int next = mD90;
            mD90 = mD8C;
            fn_804BC628(next);
        }
    } else if (mDCC < 30) {
        SetPaneFloat_unk(m70->mLayout, "zoomFrame", (float)(30 - mDCC));
        if (mDCC < 29) {
            if (mDCC == 15) {
                m7C->mWait = 5;
                m7C->mActive = 1;
            }
        } else {
            fn_80117228(8);
        }
    }
}

void D2AMpMenuLobby::fn_804C0548() {
    void* pad = lbl_8087EF70;
    MpLobbyPlayerInfo_unk* player = fn_804EB870(lbl_8087F610);
    int face = player->mD0_6;
    if (mE64 == -1) {
        if (lbl_8087F628->m25C == 1 && fn_800A55AC(pad, 0, 6) != 0) {
            if (--face < 0) {
                face += 3;
            }
            player = fn_804EB870(lbl_8087F610);
            if (fn_804EB1AC(lbl_8087F610) == 1) {
                fn_804DD9CC(lbl_8087F610, player, face);
            } else {
                int id;
                if (fn_804FAB40(lbl_8087F610, face, player, &id, 1) == 1) {
                    mE64 = id;
                }
            }
        }
    } else {
        int state = fn_804F5E38(lbl_8087F610, mE64);
        if (state == -2) {
            mE64 = -1;
        } else if (state == -1) {
            fn_804F5C9C(lbl_8087F610, mE64);
            mE64 = -1;
        } else if (state == 1) {
            int* result = fn_804F5B08(lbl_8087F610, mE64);
            if (result != 0) {
                player->mD0_6 = *result;
            }
            fn_804F5C9C(lbl_8087F610, mE64);
            mE64 = -1;
        }
    }
}

// inferred: draw: freezes every task, then lets the ones of the state draw; with the debug flags of the
// session, draws "everybody warrior" or "everybody mage" (Japanese) with a shadow
// NONMATCHING: 97.9% - offsets in the wide string pool until the other wide strings of the file are written
void D2AMpMenuLobby::vf14() {
    for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
        if (*it != 0) {
            (*it)->FreezeTask();
        }
    }
    switch (mD88) {
    case 2:
        fn_804C0A40();
        fn_804C1634();
        break;
    case 3:
        fn_804C0A40();
        fn_804C1634();
        break;
    case 1:
        fn_804C0A40();
        SetPaneValue2_unk(m4C->mLayout, "col0005", 0, 128.0f);
        m64->MeltTask();
        break;
    case 4:
        fn_804C0EA4();
        break;
    case 5: {
        int mode = D2AMpMenu::sInstance->mC4;
        if (mode == 1 || mode == 3) {
            m58->MeltTask();
        } else if (mode == 2) {
            m54->MeltTask();
        }
        m4C->MeltTask();
        if (GetHostCount_unk() >= 2) {
            m50->MeltTask();
        }
        break;
    }
    case 6:
        fn_804C0F88();
        break;
    case 10:
        m68->MeltTask();
        break;
    case 7:
        fn_804C1370();
        break;
    case 8:
    case 9:
        if (mDA4 != 0) {
            fn_804C1370();
        } else {
            fn_804C0A40();
        }
        vf20();
        break;
    }
    unsigned char flags = lbl_8087F610->m5B4;
    if (flags & 4) {
        float size = 16.0f;
        lbl_8087EEB0->fn_80061824(181.0f, 81.0f, 0.0f, size, size, 0.0f, 0.0f, 0.0f, L"\x5168\x54E1\x6226\x58EB",
                                  0xFF000000, 1, 1, 0, 0, 0xFF000000);
        lbl_8087EEB0->fn_80061824(180.0f, 80.0f, 0.0f, size, size, 0.0f, 0.0f, 0.0f, L"\x5168\x54E1\x6226\x58EB",
                                  0xFFFFFF00, 1, 1, 0, 0, 0xFF000000);
    } else if (flags & 8) {
        float size = 16.0f;
        lbl_8087EEB0->fn_80061824(181.0f, 81.0f, 0.0f, size, size, 0.0f, 0.0f, 0.0f,
                                  L"\x5168\x54E1\x9B54\x6CD5\x4F7F\x3044", 0xFF000000, 1, 1, 0, 0, 0xFF000000);
        lbl_8087EEB0->fn_80061824(180.0f, 80.0f, 0.0f, size, size, 0.0f, 0.0f, 0.0f,
                                  L"\x5168\x54E1\x9B54\x6CD5\x4F7F\x3044", 0xFFFFFF00, 1, 1, 0, 0, 0xFF000000);
    }
}

// inferred: draw of the lobby: the base, the logo of the game mode (texts cleared), and for every player slot
// the profile, the face mark and the balloons (or the wait mark), with the ok mark in the color of the face
// once the player is ready
// NONMATCHING: 97.2% - wide and narrow string offsets; order of the shifts of the color
void D2AMpMenuLobby::fn_804C0A40() {
    int mode = D2AMpMenu::sInstance->mC4;
    m4C->MeltTask();
    if (GetHostCount_unk() >= 2) {
        m50->MeltTask();
    }
    if (mode == 1 || mode == 3) {
        m58->MeltTask();
    } else if (mode == 2) {
        m54->MeltTask();
        SetPaneText_unk(m4C->mLayout, "string0010(1)", L"");
        SetPaneText_unk(m4C->mLayout, "string0011(1)", L"");
    }
    SetPaneText_unk(m4C->mLayout, "string0006(1)", L"");
    SetPaneText_unk(m4C->mLayout, "string0007(1)", L"");
    if (mD88 == 5) {
        return;
    }
    for (int i = 0; i < 6; i++) {
        if (mPlayers[i].m18 != 0) {
            mPlayers[i].mTasks[2]->MeltTask();
            continue;
        }
        mPlayers[i].mTasks[0]->MeltTask();
        if (mD88 != 9 && mD88 != 8) {
            mPlayers[i].mTasks[1]->MeltTask();
        }
        if (mD88 != 8 && mD88 != 9) {
            mPlayers[i].mTasks[3]->MeltTask();
            mPlayers[i].mTasks[4]->MeltTask();
        }
        MpLobbyMember_unk* member = lbl_8087F610->FindMember_unk(GetAid_unk(i));
        if (member != 0 && member->mD0_31 == 1 && member->mD0_3 == 1) {
            mPlayers[i].mTasks[5]->MeltTask();
            MpD2A_unk* face = mPlayers[i].mTasks[1];
            atn::Color color = face->GetColor_unk(fn_800DC6B4("face1"));
            mPlayers[i].mTasks[5]->SetColor_unk("col0001", color.ToU32());
        }
    }
}

void D2AMpMenuLobby::fn_804C0EA4() {
    fn_804C0A40();
    fn_804C1634();
    m64->mFlags &= ~4;
    m5C->mFlags &= ~4;
    int chara = mE6C;
    if (chara > 21) {
        chara -= 2;
    } else if (chara > 11) {
        chara -= 1;
    }
    if (D2ACommonDialog::sInstance->m4C == 0) {
        if (chara >= 0) {
            fn_804A3C24(MenuCommonTask::sInstance, lbl_8087F86C->Get_unk(chara + 0xB3), 0);
        } else {
            fn_804A3C24(MenuCommonTask::sInstance, lbl_8087F86C->Get_unk(0xC9), 0);
        }
    }
}

// inferred: help text of state 6 (waiting for the other players): the host may start once everybody answered,
// the others see the name of the host; then the list and the players
// NONMATCHING: 99.6% - only the name of the wide string pool (lbl_80790D74 / @wstringBase0) differs
void D2AMpMenuLobby::fn_804C0F88() {
    bool host = fn_804EB1AC(lbl_8087F610) != 0;
    if (D2ACommonDialog::sInstance->m4C == 0) {
        bool ready = false;
        if (lbl_8087F628->mC215 != 0) {
            if (fn_804EB26C(lbl_8087F610, 0) == 1) {
                ready = true;
            }
        } else {
            int count = 0;
            if (fn_804EB26C(lbl_8087F610, &count) != 0 && GetHostCount_unk() > 1 && count == GetHostCount_unk()) {
                ready = true;
            }
        }
        if (mDD0 < 120 || D2AMpMenu::sInstance->mC4 == 2) {
            if (ready == true) {
                if (host) {
                    if (mDB0 > 0) {
                        fn_804A5D34(MenuCommonTask::sInstance, 0, lbl_8087F86C->Get_unk(0xD2), L"", L"");
                    } else {
                        fn_804A5D34(MenuCommonTask::sInstance, 0, lbl_8087F86C->Get_unk(0xCB), L"", L"");
                        m64->mFlags &= ~4;
                    }
                } else if (fn_804FC010(lbl_8087F610) != 0) {
                    fn_804A5D34(MenuCommonTask::sInstance, 0, lbl_8087F86C->Get_unk(0xD2), L"", L"");
                } else {
                    wchar_t text[256] = {0};
                    atn::net::NetPlayerInfo_unk* info = lbl_8087F628->vf14(lbl_8087F628->GetServerAid_unk());
                    if (info != 0) {
                        fn_800DD3FC(text, lbl_8087F86C->Get_unk(0xCC), info->mName);
                        fn_804A5D34(MenuCommonTask::sInstance, 0, text, L"", L"");
                    }
                }
            } else {
                fn_804A5D34(MenuCommonTask::sInstance, 0, lbl_8087F86C->Get_unk(0xCA), L"", L"");
            }
        } else {
            fn_804A3C24(MenuCommonTask::sInstance, lbl_8087F86C->Get_unk(0xE1), 0);
        }
        if (!host) {
            if (++mDD0 > 240) {
                mDD0 = 0;
            }
        }
    } else {
        mDD0 = 0;
    }
    fn_804C0A40();
    fn_804C1634();
    m64->mFlags &= ~4;
}

// inferred: draw of the map list (states 7-9): the list of the game mode, the cursor on the entry mDB4 and the
// names of the maps (grey when not selectable); updates the number of entries
// NONMATCHING: 99.2% - registers of the loop pointer and of the color; string offsets
void D2AMpMenuLobby::fn_804C1370() {
    MpRect_unk rect;
    char name[0x40] = {0};
    int count = 0;
    m6C->MeltTask();
    m70->MeltTask();
    MpD2A_unk* list;
    unsigned int color;
    atn::DrawHelper* helper;
    if (lbl_8087F610->m540 != 2) {
        list = m74;
    } else {
        list = m78;
    }
    list->MeltTask();
    SetPaneValue2_unk(m70->mLayout, "col0005", 0, 128.0f);
    if (MenuCommonTask::sInstance != 0 && !(list->mFrame < 2.0f)) {
        sprintf(name, "message_list%02d", mDB4 + 1);
        MpRect_unk cursor = GetPaneRect_unk(list, name);
        rect.x = cursor.x;
        rect.y = cursor.y;
        rect.w = 2.56f;
        rect.h = 0.24f;
        rect.z = -10.0f;
        fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, mD88 == 7);
        for (int i = 0; i < 16; i++) {
            if (mDD4[i] == -1) {
                continue;
            }
            sprintf(name, "message_list%02d", i + 1);
            MpRect_unk pos = GetPaneRect_unk(list, name);
            rect = pos;
            rect.x += 15.0f;
            rect.y += 6.0f;
            color = 0xFF888888;
            if (mE14[i] != 0) {
                color = 0xFFFFFFFF;
            }
            helper = lbl_8087EEB0;
            helper->fn_80061824(rect.x, rect.y, -10.0f, 17.0f, 17.0f, 0.0f, 0.0f, 0.0f,
                                      fn_804FA7C8(lbl_8087F610, lbl_8087F610->m540, mDD4[i]), color, 1, 1, 0, 0,
                                      0xFF000000);
            count++;
        }
        fn_804C28A0(count);
    }
}

// inferred: scrolls the names of the players in their balloons (after 30 frames, 1.5 per frame; back to the
// start once the whole name went past)
// NONMATCHING: 99.6% - offset of "message_0007" until the strings of the functions before it are written
void D2AMpMenuLobby::fn_804C1634() {
    MpRect_unk pos;
    float scroll;
    float scale = 0.05f;
    float speed = 1.5f;
    float width = 220.0f;
    for (int i = 0; i < 6; i++) {
        if (mPlayers[i].m20 == 0) {
            continue;
        }
        int frame = ++mPlayers[i].m24;
        if ((float)frame < 30.0f) {
            scroll = 0.0f;
        } else {
            scroll = speed * ((float)frame - 30.0f);
        }
        if (mPlayers[i].m28 == 0) {
            pos = GetPaneRect_unk(mPlayers[i].mTasks[3], "message_0007");
        } else {
            pos = GetPaneRect_unk(mPlayers[i].mTasks[4], "message_0007");
        }
        pos.x += 2.0f;
        const wchar_t* name = (const wchar_t*)&mPlayers[i].m2C;
        lbl_8087EEB0->fn_80061AE4(name, -1, 1, 1, 0, 1, 0xFF000000, pos.x - scroll, pos.y, -10.0f, 14.0f, 12.0f,
                                  0.0f, 1.0f, 1.0f, pos.x - width * scale, width, scale);
        if (scroll > fn_8006EF48(lbl_8087EEC8, 17.0f, name, 0.0f, 1, 1)) {
            mPlayers[i].m24 = 0;
        }
    }
}

// inferred: stops an anime on the frame `frame`
static inline void StopAt_unk(MP::MpD2A_unk* task, float frame) {
    if (task != 0) {
        task->fn_800D246C(1);
        task->mSpeed = -1.0f;
        task->mFrame = frame;
    }
}

// inferred: refreshes the player slots from the session: empty slots show the wait mark; the others the name,
// the character, the rank, the voted map, the face (with the team color in versus) and the mark of the host
// NONMATCHING: 95% - register allocation of the loop; the range check of the character is folded into one
// unsigned compare here; (mode == 1) of "drawFlg1" is computed after the hash here
void D2AMpMenuLobby::fn_804C18A8() {
    atn::net::NetPlayerInfo_unk* server = lbl_8087F628->vf14(lbl_8087F628->GetServerAid_unk());
    wchar_t text[64] = {0};
    int mode = D2AMpMenu::sInstance->mC4;
    for (int i = 0; i < 6; i++) {
        if (i >= GetHostCount_unk()) {
            mPlayers[i].m18 = 1;
            mPlayers[i].m1C = 0;
            mPlayers[i].m20 = 0;
            mPlayers[i].m24 = 0;
            Play_unk(mPlayers[i].mTasks[2], 0, 1.0f);
            Stop_unk(mPlayers[i].mTasks[0]);
            Stop_unk(mPlayers[i].mTasks[1]);
            Stop_unk(mPlayers[i].mTasks[3]);
            Stop_unk(mPlayers[i].mTasks[4]);
            Stop_unk(mPlayers[i].mTasks[5]);
            continue;
        }
        unsigned int aid = GetAid_unk(i);
        atn::net::NetPlayerInfo_unk* info = lbl_8087F628->vf14(aid);
        MpLobbyMember_unk* member = lbl_8087F610->FindMember_unk(aid);
        fn_805075C4(&lbl_8087F610->m610, info != 0 ? info->m30 : 0);
        if (member == 0) {
            mPlayers[i].m18 = 1;
            mPlayers[i].m1C = 0;
            Play_unk(mPlayers[i].mTasks[2], 0, 1.0f);
            Stop_unk(mPlayers[i].mTasks[0]);
            Stop_unk(mPlayers[i].mTasks[1]);
            Stop_unk(mPlayers[i].mTasks[3]);
            Stop_unk(mPlayers[i].mTasks[4]);
            Stop_unk(mPlayers[i].mTasks[5]);
            continue;
        }
        if ((unsigned int)(mode - 2) <= 1) {
            mPlayers[i].m1C = 2;
        } else {
            mPlayers[i].m1C = member->mD0_6;
        }
        mPlayers[i].m18 = 0;
        int chara = member->mB0;
        int map = member->mD0_22;
        if (map >= fn_804EA5DC(lbl_8087F610, lbl_8087F610->m540)) {
            map = fn_804EA5DC(lbl_8087F610, lbl_8087F610->m540);
        }
        if (member->mD0_1 == 1) {
            mE60 = i;
            chara = mE6C;
            map = mE5C;
            if (mD88 == 4) {
                SetPaneValue_unk(mPlayers[mE60].mTasks[0]->mLayout, "pos0010_01(1)(1)", 4, 0.0f);
                SetPaneValue_unk(mPlayers[mE60].mTasks[0]->mLayout, "pos0011_01(1)", 4, 0.0f);
            }
        } else {
            SetPaneValue_unk(mPlayers[i].mTasks[0]->mLayout, "pos0010_01(1)(1)", 4, 200.0f);
            SetPaneValue_unk(mPlayers[i].mTasks[0]->mLayout, "pos0011_01(1)", 4, 200.0f);
            SetPaneValue2_unk(mPlayers[i].mTasks[0]->mLayout, "col0003(1)", 0, 0.0f);
            SetPaneValue2_unk(mPlayers[i].mTasks[0]->mLayout, "col0004(1)", 0, 0.0f);
        }
        if (wcscmp(info->mName, L"") == 0) {
            const wchar_t* none = lbl_8087F86C->Get_unk(0xCD);
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0027(2)", none);
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0007(1)(1)", L"");
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0001(1)(1)", L"");
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0001(1)", L"");
            SetPaneFloat_unk(mPlayers[i].mTasks[1]->mLayout, "patternIndex", 0.0f);
        } else {
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0027(2)", info->mName);
            CharaInfo_unk* ci = fn_8020924C(chara + 0xAAE60);
            if (chara == -1) {
                fn_800DD3FC(text, lbl_8087F86C->Get_unk(0x2A));
            } else if (chara >= 0 && chara < 26) {
                fn_800DD3FC(text, ci->m04);
            } else {
                fn_800DD3FC(text, ci != 0 ? ci->m04 : L"");
            }
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0007(1)(1)", text);
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0001(1)(1)", fn_804FA88C(lbl_8087F610, info->m30));
            SetPaneText_unk(mPlayers[i].mTasks[0]->mLayout, "string0001(1)", fn_804FA7C8(lbl_8087F610, lbl_8087F610->m540, map));
            int pattern = ci != 0 ? ci->m2C : 0;
            SetPaneInt_unk(mPlayers[i].mTasks[1]->mLayout, "patternIndex", pattern);
            SetPaneValue2Int_unk(mPlayers[i].mTasks[1]->mLayout, "col0006", 0, (unsigned char)(pattern == 0 ? 0xFF : 0));
        }
        if (info == server) {
            SetPaneValue2_unk(mPlayers[i].mTasks[0]->mLayout, "col0001", 3, 180.0f);
            SetPaneValue2_unk(mPlayers[i].mTasks[0]->mLayout, "col0027", 3, 180.0f);
        } else {
            SetPaneValue2_unk(mPlayers[i].mTasks[0]->mLayout, "col0001", 3, 255.0f);
            SetPaneValue2_unk(mPlayers[i].mTasks[0]->mLayout, "col0027", 3, 255.0f);
        }
        SetPaneInt_unk(mPlayers[i].mTasks[1]->mLayout, "drawFlg1", mode == 1);
        if (mode == 1) {
            unsigned int color = 0xFFFFFFFF;
            if (mPlayers[i].m1C == 2) {
                color = 0xFF0096FF;
            } else if (mPlayers[i].m1C == 1) {
                color = 0xFFFF0000;
            }
            mPlayers[i].mTasks[1]->SetColor_unk("col0002(1)", color);
        }
        Play_unk(mPlayers[i].mTasks[0], 0, 1.0f);
        Play_unk(mPlayers[i].mTasks[1], 0, 1.0f);
        Play_unk(mPlayers[i].mTasks[5], 0, 1.0f);
        mPlayers[i].mTasks[0]->mFrame = mPlayers[i].mTasks[0]->mLastFrame;
        StopAt_unk(mPlayers[i].mTasks[2], mPlayers[i].mTasks[2]->mLastFrame);
    }
}

void D2AMpMenuLobby::fn_804C2260(int v) { mDCC = v; }

// inferred: shows the time left of the lobby (color index 1 is 255, 2 and 3 depend on the time: the original
// computes them with a mask, (t > 30) ? 0 : 0xFF or the reverse) and, while the lobby runs, plays the clock
// animation forwards or backwards
// NONMATCHING: 85% - the branchless color value and the string offsets
void D2AMpMenuLobby::fn_804C2268(int time) {
    m50->SetNumber_unk("string0100(1)", time, 0);
    SetPaneValue2_unk(m50->mLayout, "col0100", 1, 255.0f);
    int full = 0xFF;
    SetPaneValue2_unk(m50->mLayout, "col0100", 2, (float)(time <= 30 ? 0 : full));
    SetPaneValue2_unk(m50->mLayout, "col0100", 3, (float)(time <= 30 ? 0 : full));
    if (mD88 != 0) {
        MpD2A_unk* task = m50;
        if (task != 0) {
            task->fn_800D246C(0);
            task->mSpeed = (float)(time == 0 ? -1 : 1);
            task->mRepeat = 0;
        }
    }
}

// inferred: lottery of the map: the entries voted by the players (other than "random", the last one) take
// part, or every entry when nobody voted; with a single entry it is set at once, otherwise the host draws one
// (the guests take the one of the host) and the roulette runs for 60 frames
// NONMATCHING: the fill of mE14 is unrolled 8x2 in the original (16 stores here); volatile registers of the
// member search (as vf0C)
void D2AMpMenuLobby::fn_804C23E4() {
    int count = 0;
    for (unsigned int i = 0; i < 16; i++) {
        if ((int)i < fn_804EA5DC(lbl_8087F610, lbl_8087F610->m540)) {
            mDD4[i] = i;
        } else {
            mDD4[i] = -1;
        }
        mE14[i] = 0;
    }
    for (int i = 0; i < GetHostCount_unk(); i++) {
        MpLobbyMember_unk* member = lbl_8087F610->FindMember_unk(GetAid_unk(i));
        if (member != 0 && member->mD0_31 == 1) {
            unsigned int map = member->mD0_22;
            if (map != fn_804EA5DC(lbl_8087F610, lbl_8087F610->m540) && mE14[map] == 0) {
                mE14[map] = 1;
                count++;
            }
        }
    }
    if (count <= 0) {
        for (int i = 0; i < 16; i++) {
            mE14[i] = 1;
        }
        count = fn_804EA5DC(lbl_8087F610, lbl_8087F610->m540);
    }
    if (count == 1) {
        mE54 = 1;
        for (int i = 0; i < 16; i++) {
            if (mE14[i] != 0) {
                MpLobbySession_unk* s = lbl_8087F610;
                fn_804EA534(s, mDD4[i], fn_804EB1AC(s));
                fn_804EB870(lbl_8087F610)->mD0_26 = mDD4[i];
                fn_804C2994(mDD4[i]);
                return;
            }
        }
        return;
    }
    mE54 = 60;
    int sel = 0;
    if (fn_804EB1AC(lbl_8087F610) != 0) {
        int r = rand() % count;
        for (int i = 0; i < 16; i++) {
            if (mE14[i] != 0) {
                if (r <= 0) {
                    sel = i;
                    break;
                }
                r--;
            }
        }
        fn_804EB870(lbl_8087F610)->mD0_26 = sel;
    } else {
        MpLobbyMember_unk* member = lbl_8087F610->FindMember_unk(lbl_8087F628->GetServerAid_unk());
        if (member != 0) {
            sel = member->mD0_26;
        }
    }
    MpLobbySession_unk* s = lbl_8087F610;
    fn_804EA534(s, mDD4[sel], fn_804EB1AC(s));
    fn_804C2994(mDD4[sel]);
}

void D2AMpMenuLobby::fn_804C28A0(int count) {
    if (mD88 == 7) {
        int start = mDB4;
        do {
            mDB4 = (mDB4 + 1) % count;
        } while (mE14[mDB4] == 0 && start != mDB4);
        if (mE54 <= mDCC) {
            mDB4 = lbl_8087F610->m55C;
            fn_804C2994(mDD4[mDB4]);
            mDA4 = 1;
            mD90 = 9;
            fn_80117228(1);
            fn_804BC628(8);
        } else {
            mDCC++;
            fn_80117228(3);
        }
    }
}

// inferred: selects the map drawing of the entry `id` (tables per game mode) and, once the play time is known,
// colors the logo and moves Ruri for the players under 407000 (0x635D8) frames
// NONMATCHING: 96.5% - the tables are one .rodata object with the int-to-float constant in the original; order
// of two instructions after fn_804E4A20
void D2AMpMenuLobby::fn_804C2994(int id) {
    int versus[9] = {0, 0, 1, 0, 0, 0, 2, 0, 0};
    int raid[5] = {0, 0, 0, 0, 1};
    mE58 = D2AMpMenu::sInstance->mC4 == 2 ? raid[id] : versus[id];
    const char* names[3] = {"draw1", "draw2", "draw3"};
    for (int i = 0; i < 3; i++) {
        if (i == mE58) {
            SetPaneFloat_unk(m70->mLayout, names[i], 1.0f);
        } else {
            SetPaneFloat_unk(m70->mLayout, names[i], 0.0f);
        }
    }
    if (lbl_8087F610->m514 != 0) {
        int time = fn_804E4A20();
        int full = 0xFF;
        SetPaneValue2_unk(m70->mLayout, "col0002(1)", 0, (float)(time <= 407000 ? 0 : full));
        SetPaneValue2_unk(m70->mLayout, "col0009(1)", 0, (float)(time <= 407000 ? 0 : full));
        SetPaneFloat_unk(m70->mLayout, "ruriXposCorrection", (float)(time <= 407000 ? -100 : 0));
    }
}

// NONMATCHING: same volatile registers of the member search as vf0C (aid r3/r4)
int D2AMpMenuLobby::fn_804C2C14() {
    if (fn_804EB1AC(lbl_8087F610) != 0) {
        return 0;
    }
    for (int i = 0; i < GetHostCount_unk(); i++) {
        MpLobbyMember_unk* member = lbl_8087F610->FindMember_unk(GetAid_unk(i));
        if (member != 0 && member->mD0_31 == 1 && member->mD0_20 == 0) {
            return 1;
        }
    }
    return 0;
}

// inferred: shows `text` in the balloon of the player `member` (kind 0: first balloon, 1: second one) for
// frames + 30 frames (90 when no time is given), clearing the old text and opening the balloon
// NONMATCHING: 90.8% - L"" is in the .sdata until the other wide strings of the file are written (the original
// pools them in .data); the bounds test of the index
void D2AMpMenuLobby::fn_804C2D5C(MpLobbyMember_unk* member, const wchar_t* text, int frames, int kind) {
    int index = GetAidIndex_unk(member->mCC);
    if (index < 0) {
        return;
    }
    if (index < 6) {
        if (kind == 0) {
            SetPaneText_unk(mPlayers[index].mTasks[3]->mLayout, "string0007(1)", L"");
            mPlayers[index].mTasks[3]->mFrame = 0.0f;
        } else {
            SetPaneText_unk(mPlayers[index].mTasks[4]->mLayout, "string0007(1)", L"");
            mPlayers[index].mTasks[4]->mFrame = 0.0f;
        }
        mPlayers[index].m20 = frames > 0 ? frames + 30 : 90;
        mPlayers[index].m24 = 0;
        mPlayers[index].m28 = kind;
        fn_800DD3FC((wchar_t*)&mPlayers[index].m2C, text);
        if (kind == 0) {
            MpD2A_unk* task = mPlayers[index].mTasks[3];
            if (task != 0) {
                task->fn_800D246C(0);
                task->mSpeed = 1.0f;
                task->mRepeat = 0;
            }
            task = mPlayers[index].mTasks[4];
            if (task != 0) {
                task->fn_800D246C(1);
                task->mSpeed = -1.0f;
                task->mFrame = 0.0f;
            }
        } else {
            MpD2A_unk* task = mPlayers[index].mTasks[4];
            if (task != 0) {
                task->fn_800D246C(0);
                task->mSpeed = 1.0f;
                task->mRepeat = 0;
            }
            task = mPlayers[index].mTasks[3];
            if (task != 0) {
                task->fn_800D246C(1);
                task->mSpeed = -1.0f;
                task->mFrame = 0.0f;
            }
        }
    }
}

void D2AMpMenuLobby::fn_804C2F98() { fn_800D2338(this); }

bool D2AMpMenuLobby::fn_804C2F9C() {
    int state = mD88;
    return state == 7 || state == 8 || state == 10;
}
} // namespace MP
