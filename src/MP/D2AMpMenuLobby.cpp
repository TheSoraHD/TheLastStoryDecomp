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
    unsigned char m50A[0x55C - 0x50A];
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

void D2AMpMenuLobby::fn_804C2260(int v) { mDCC = v; }

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
