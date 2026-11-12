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

// inferred: hash of the "CustomData" pane (as in MP/D2AMpMenu.cpp; read by functions not written yet); its
// initializer is the __sinit of the file (0x804C2FC8)
extern "C" unsigned int fn_800DC6B4(const char* s);
static const unsigned int sCustomData = fn_800DC6B4("CustomData");

// inferred: player information returned by fn_804EB870 (local player); only the member used here
struct MpLobbyPlayerInfo_unk {
    unsigned char m00[0xB0];
    int mB0;
    unsigned char mB4[0xD0 - 0xB4];
    unsigned int mD0_hi : 6;
    unsigned int mD0_6 : 4;   // inferred: face
    unsigned int mD0_lo : 22;
};
// inferred: one player of the session (0xD5C bytes)
struct MpLobbyMember_unk {
    unsigned char m000[0xCC];
    unsigned char mCC;        // inferred: aid
    unsigned char mCD[0xD0 - 0xCD];
    unsigned int mD0_31 : 1;  // inferred: valid
    unsigned int mD0_1 : 19;
    unsigned int mD0_20 : 2;  // inferred: zero while still loading
    unsigned int mD0_lo : 10;
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
    unsigned char m560[0x5E4 - 0x560];
    MpLobbyMember_unk* m5E4; // inferred: players
    int m5E8;                // inferred: number of players
};
extern "C" MpLobbySession_unk* lbl_8087F610;
extern "C" MpLobbyPlayerInfo_unk* fn_804EB870(MpLobbySession_unk* s); // inferred: local player
// inferred: multiplayer data (lbl_8087F628); only the members used here (as MP/D2AMpMenuMessageList.cpp)
struct MpLobbyData_unk {
    unsigned char m0000[0x1F8];
    unsigned int m1F8_hi : 12;
    unsigned int m1F8_12 : 1; // inferred: nonzero when playing alone
    unsigned int m1F8_lo : 19;
    unsigned char m1FC[0x25C - 0x1FC];
    int m25C;                 // inferred: 1 while the face can be changed
    unsigned char m260[0xC215 - 0x260];
    unsigned char mC215;      // inferred: nonzero for a friend room
};
extern "C" MpLobbyData_unk* lbl_8087F628;
extern "C" int DWC_GetNumConnectionHost();
extern "C" int fn_806A8E70();
extern "C" void fn_804DDA04(MpLobbySession_unk* s); // inferred: sends the game mode
extern "C" int fn_804E4A20(); // inferred: play time of the player
// inferred: list of the connected aids
struct MpLobbyAids_unk {
    unsigned char* mAids;
};
extern "C" int fn_804EB1AC(MpLobbySession_unk* s); // inferred: nonzero for the host
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
    MpLobbyData_unk* data = lbl_8087F628;
    if (data->mC215 == 0) {
        return data->m1F8_12 ? 1 : DWC_GetNumConnectionHost();
    }
    return data->m1F8_12 ? 1 : fn_806A8E70();
}

// inferred: aid of the i-th connected host (0xFF: none)
static inline unsigned int GetAid_unk(int i) {
    if (lbl_8087F628->m1F8_12) {
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

void D2AMpMenuLobby::fn_804C2F98() { fn_800D2338(this); }

bool D2AMpMenuLobby::fn_804C2F9C() {
    int state = mD88;
    return state == 7 || state == 8 || state == 10;
}
} // namespace MP
