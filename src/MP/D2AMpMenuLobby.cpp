// MP/D2AMpMenuLobby.cpp (0x804BBBA0-0x804C2FF8): lobby of a multiplayer game. Only the first functions are
// reconstructed so far.
#include "MP/D2AMpMenuLobby.h"
#include "MP/D2AMpMenuSetRule.h"
#include "atn/FadeCommon.h"

// inferred: player information returned by fn_804EB870 (local player); only the member used here
struct MpLobbyPlayerInfo_unk {
    unsigned char m00[0xB0];
    int mB0;
};
// inferred: one player of the session (0xD5C bytes)
struct MpLobbyMember_unk {
    unsigned char m000[0xCC];
    unsigned char mCC;        // inferred: aid
    unsigned char mCD[0xD0 - 0xCD];
    unsigned int mD0_31 : 1;  // inferred: valid
    unsigned int mD0_lo : 31;
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
    unsigned char m048[0x5E4 - 0x48];
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
    unsigned char m1FC[0xC215 - 0x1FC];
    unsigned char mC215;      // inferred: nonzero for a friend room
};
extern "C" MpLobbyData_unk* lbl_8087F628;
extern "C" int DWC_GetNumConnectionHost();
extern "C" int fn_806A8E70();
// inferred: list of the connected aids
struct MpLobbyAids_unk {
    unsigned char* mAids;
};
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

} // namespace MP
