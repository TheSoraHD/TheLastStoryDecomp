#include "MP/D2AMpMenu_unk.h"
#include "math/Vec.h"
#include "atn/Model_unk.h"

// inferred: character of a multiplayer player (only the virtual used here)
struct MpChara_unk {
    virtual void vf08();
    virtual void vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf18();
    virtual void vf1C();
    virtual void vf20();
    virtual void vf24();
    virtual void vf28();
    virtual void vf2C();
    virtual void vf30();
    virtual void vf34();
    virtual void vf38();
    virtual void vf3C();
    virtual void vf40();
    virtual void vf44();
    virtual void vf48();
    virtual void vf4C();
    virtual void vf50();
    virtual void vf54();
    virtual atn::Vector3 vf58(); // inferred: position

    unsigned char m04[0xB0 - 0x04];
    atn::Model mModel; // 0xB0: inferred: model (its node matrices at 0xEC)
};

// inferred: player of the multiplayer session (0xD5C bytes)
struct MpPlayer_unk {
    MpChara_unk* m00;
    unsigned char m04[0xD0 - 0x04];
    unsigned int mD0_hi : 6;
    unsigned int mTeam : 4; // inferred
    unsigned int mD0_lo : 22;
    unsigned char mD4[0xDC - 0xD4];
    int mDC; // inferred: score
    unsigned char mE0[0xD5C - 0xE0];
};

// inferred: ranking list of the session: count, then the players
struct MpRanking_unk {
    unsigned int mCount;
    MpPlayer_unk mPlayers[1];
};

// inferred: multiplayer session manager (lbl_8087F610); only the members used here
struct MpSession_unk {
    unsigned char m000[0x50C];
    int m50C;
    unsigned char m510[0x540 - 0x510];
    int m540; // inferred: game mode
    unsigned char m544[0x5E4 - 0x544];
    MpPlayer_unk* m5E4; // inferred: players
    int m5E8;           // inferred: number of players
    unsigned char m5EC[0x2B88 - 0x5EC];
    MpRanking_unk m2B88;

    MpPlayer_unk* GetPlayer_unk(int i) { return (i >= 0 && i < m5E8) ? &m5E4[i] : 0; }
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" MpPlayer_unk* fn_804EB870(MpSession_unk* s); // inferred: local player
extern "C" int fn_804EB480(MpSession_unk* s, MpPlayer_unk* player);
extern "C" int fn_804DB408(MpSession_unk* s); // inferred: remaining time in seconds

// inferred: names of the players (lbl_8087F628)
struct MpPlayerInfo_unk {
    unsigned char m00[0x10];
    wchar_t mName[(0x34 - 0x10) / 2];
};
struct MpPlayerInfos_unk {
    unsigned char m000[0x274];
    MpPlayerInfo_unk mInfos[8];
};
extern "C" MpPlayerInfos_unk* lbl_8087F628;

// inferred: main multiplayer menu (lbl_8087F59C); only the member used here
struct MpMenu_unk {
    unsigned char m00[0xC4];
    int mC4;
};
extern "C" MpMenu_unk* lbl_8087F59C;

// inferred: render manager (see game/MenuCommonTask.cpp)
struct MpRender_unk {
    unsigned char m00[0x3C];
    int mWidth;  // 0x3C
    int mHeight; // 0x40
};
extern "C" MpRender_unk* lbl_8087EEE0;
extern "C" void* lbl_8087EFB4; // inferred: camera
extern "C" void fn_800BFAC8(atn::Vector3* out, void* camera, const atn::Vector3* pos); // inferred: to screen

extern "C" void* lbl_8087F498; // inferred: voice manager
extern "C" atn::SoundRef fn_803E9608(void* mgr, const char* name, int a, int b, float volume);
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
struct MpArena_unk {
    unsigned char m0000[0x255C];
    int m255C;
};
extern "C" MpArena_unk* lbl_8087F490;

namespace MP {

// inferred from __vt__Q22MP16D2AMpMenuCockpit (0x80790B78): in-game display of the multiplayer
// mode (start, points, crowns and names over the characters, time)
class D2AMpMenuCockpit : public atn::Task {
public:
    static D2AMpMenuCockpit* fn_804B4C50(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuCockpit(atn::Task* parent);
    virtual ~D2AMpMenuCockpit();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804B50C0(int state);           // inferred: changes the state
    void fn_804B5360();                    // inferred: start animation
    void UpdatePointPos_unk();             // inferred: not inline (its literals are in @stringBase0)
    void fn_804B5B10();                    // inferred: marks over the characters
    void fn_804B6204(int time);            // inferred: shows the remaining time
    void fn_804B671C(int value, int which); // inferred: shows a number
    void fn_804B67E4(int message);         // inferred: shows a message
    void fn_804B691C();                    // inferred: ends the info animes
    void fn_804B6944();                    // inferred: hides everything

    void Add_unk(MpD2A_unk* task) {
        if (m10C_tasks.size() < 0x18) {
            m10C_tasks.push_back(task);
        }
    }
    void SetAll_unk(int arg) {
        for (MpD2A_unk** it = &m10C_tasks[0]; it != &m10C_tasks[m10C_tasks.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(arg);
            }
        }
    }

    MpD2A_unk* m48;    // inferred: arena_ready_go_xx.d2a
    MpD2A_unk* m4C;    // inferred: online_point.d2a
    MpD2A_unk* m50[8]; // inferred: icon_crown.d2a
    MpD2A_unk* m70[8]; // inferred: ui_player_name.d2a
    MpD2A_unk* m90;    // inferred: ui_healer_att.d2a
    MpD2A_unk* m94;    // inferred: ui_magician_att.d2a
    MpD2A_unk* m98;    // inferred: ui_mp_info_popup.d2a
    int m9C;           // inferred: state
    int mA0;           // inferred: previous state
    int mA4;
    atn::FixedArray_unk<MpD2A_unk*, 0x18> m10C_tasks; // 0xA8
    int m10C;

    static D2AMpMenuCockpit* sInstance;
    static const int sMessages[8];
};

D2AMpMenuCockpit* D2AMpMenuCockpit::sInstance;
// inferred: message ids of fn_804B67E4
const int D2AMpMenuCockpit::sMessages[8] = {0x124, 0x125, 0x126, 0x127, 0x128, 0x129, 0x12A, 0};

D2AMpMenuCockpit* D2AMpMenuCockpit::fn_804B4C50(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) D2AMpMenuCockpit(parent);
    }
    return sInstance;
}

D2AMpMenuCockpit::D2AMpMenuCockpit(atn::Task* parent) : atn::Task(parent), m9C(0), mA0(0), mA4(0), m10C(0) {
    m48 = fn_801F3FF8(this, "/data/d2anime/arena_ready_go_xx.d2a", 0);
    Add_unk(m48);
    m4C = fn_801F3FF8(this, "/data/d2anime/online_point.d2a", 0);
    Add_unk(m4C);
    for (int i = 0; i < 8; i++) {
        m50[i] = fn_801F3FF8(this, "/data/d2anime/icon_crown.d2a", 0);
        Add_unk(m50[i]);
        m70[i] = fn_801F3FF8(this, "/data/d2anime/ui_player_name.d2a", 0);
        Add_unk(m70[i]);
    }
    m90 = fn_801F3FF8(this, "/data/d2anime/ui_healer_att.d2a", 0);
    Add_unk(m90);
    m94 = fn_801F3FF8(this, "/data/d2anime/ui_magician_att.d2a", 0);
    Add_unk(m94);
    m98 = fn_801F3FF8(this, "/data/d2anime/ui_mp_info_popup.d2a", 0);
    Add_unk(m98);
    SetAll_unk(1);
}

D2AMpMenuCockpit::~D2AMpMenuCockpit() {
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int D2AMpMenuCockpit::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetAll_unk(0);
        m90->mRepeat = 0;
        m90->mFrame = m90->mLastFrame;
        m94->mRepeat = 0;
        m94->mFrame = m94->mLastFrame;
        m98->mRepeat = 0;
        m98->mFrame = m98->mLastFrame;
        m98->mSpeed = 0.75f;
        return 1;
    }
    return 0;
}

// NONMATCHING: 99.60% - the loop pointer and the task of Play_unk swap r27/r28 in the mark loop
// (tried an inline helper for the loop or its body and an outer counter; close-units qD: 10 more
// forms, a single reused task local is the closest)
void D2AMpMenuCockpit::fn_804B50C0(int state) {
    if (m9C != state && state >= 0) {
        int prev = m9C;
        mA4 = 0;
        mA0 = prev;
        m9C = state;
        switch (state) {
        case 1: {
            m48->mFrame = 0.0f;
            m4C->mFrame = 0.0f;
            Play_unk(m48, 0, 1.0f);
            Play_unk(m4C, 0, 1.0f);
            wchar_t text0[8] = {0};
            fn_800DD3FC(text0, L"%d", 0);
            SetPaneText_unk(m4C->mLayout, "string0036(1)", text0);
            wchar_t text1[8] = {0};
            fn_800DD3FC(text1, L"%d", 0);
            SetPaneText_unk(m4C->mLayout, "string0037(1)", text1);
            for (int i = 0; i < 8; i++) {
                MpD2A_unk* task = m50[i];
                Play_unk(task, 1, 1.0f);
                task = m70[i];
                Play_unk(task, 0, 1.0f);
            }
            m10C = 0;
            break;
        }
        case 3:
            vf20();
            Play_unk(m48, 0, -1.0f);
            Play_unk(m4C, 0, -1.0f);
            break;
        }
    }
}

void D2AMpMenuCockpit::vf20() {}

void D2AMpMenuCockpit::vf10() {
    switch (m9C) {
    case 1:
        fn_804B5360();
        break;
    case 3:
        if (m4C->mFrame <= 0.0f) {
            fn_804B50C0(4);
        }
        break;
    }
}

void D2AMpMenuCockpit::fn_804B5360() {
    int done = 0;
    if (m10C == 0 && m48->mFrame < 45.0f) {
        m10C++;
        if (lbl_8087F498 != 0) {
            fn_803E9608(lbl_8087F498, "VO_ARCOM_001", 0, 1, 1.0f);
        }
    } else if (m10C == 1 && m48->mFrame >= 45.0f) {
        m10C++;
        if (lbl_8087F498 != 0) {
            fn_803E9608(lbl_8087F498, "VO_ARCOM_002", 0, 1, 1.0f);
        }
    }
    if (m48->IsEnd_unk()) {
        done = 1;
    }
    if (done != 0) {
        fn_804B50C0(2);
    }
}

void D2AMpMenuCockpit::UpdatePointPos_unk() {
    if (lbl_8087F59C->mC4 != 1) {
        SetPaneValue_unk(m4C->mLayout, "pos0005", 0, 800.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0036", 0, 800.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0037", 0, 800.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0038", 0, 800.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0039", 0, 800.0f);
    } else {
        SetPaneValue_unk(m4C->mLayout, "pos0005", 0, 320.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0036", 0, 261.5f);
        SetPaneValue_unk(m4C->mLayout, "pos0037", 0, 378.5f);
        SetPaneValue_unk(m4C->mLayout, "pos0038", 0, 210.0f);
        // inferred: the layout of the last pane is a separate local (register order of the copies
        // inlined in vf14)
        void* layout = m4C->mLayout;
        SetPaneValue_unk(layout, "pos0039", 0, 430.0f);
    }
}

void D2AMpMenuCockpit::vf14() {
    m48->mFlags |= 4;
    m4C->mFlags |= 4;
    for (int i = 0; i < 8; i++) {
        m50[i]->mFlags |= 4;
        m70[i]->mFlags |= 4;
    }
    switch (m9C) {
    case 1:
        m48->mFlags &= ~4;
        m4C->mFlags &= ~4;
        UpdatePointPos_unk();
        fn_804B5B10();
        break;
    case 2:
        m4C->mFlags &= ~4;
        UpdatePointPos_unk();
        fn_804B5B10();
        break;
    case 3:
        vf20();
        UpdatePointPos_unk();
        fn_804B5B10();
        break;
    }
}

// inferred: position of the mark over a character (3 units above its neck); not inline (its
// literal is in @stringBase0)
static atn::Vector3 GetMarkPos_unk(MpChara_unk* chara) {
    int node = chara->mModel.GetNodeNoByName("neck", 0);
    float (*mtx)[4] = node < 0 ? 0 : chara->mModel.mNodeMtx[node];
    return mtx != 0 ? atn::Vector3(mtx[0][3], mtx[1][3], mtx[2][3]) + atn::Vector3(0.0f, 3.0f, 0.0f) : chara->vf58();
}

inline bool IsOnScreen_unk(const atn::Vector3& screen) {
    return 0.0f <= screen.z && screen.z <= 1.0f && -50.0f <= screen.x &&
           screen.x < 50.0f + (float)lbl_8087EEE0->mWidth && -50.0f <= screen.y &&
           screen.y < 50.0f + (float)lbl_8087EEE0->mHeight;
}

// NONMATCHING: 93.26% - loop registers and the vector temporaries of GetMarkPos_unk differ
void D2AMpMenuCockpit::fn_804B5B10() {
    if (fn_804EB870(lbl_8087F610) == 0) {
        return;
    }
    unsigned int team = fn_804EB870(lbl_8087F610)->mTeam;
    MpSession_unk* session = lbl_8087F610;
    if (session->m540 == 0 && session->m50C < 5) {
        MpRanking_unk* ranking = &session->m2B88;
        MpPlayer_unk* prev = 0;
        int rank = 1;
        for (unsigned int i = 0; i < ranking->mCount; i++) {
            MpPlayer_unk* player = &ranking->mPlayers[i];
            if (player->m00 == 0) {
                continue;
            }
            if (rank == 1 && player->mDC == 0) {
                break;
            }
            if (rank == 1 && player->mDC == 0) {
                break;
            }
            if (prev != 0 && prev->mDC > player->mDC) {
                rank++;
            }
            if (rank > 2) {
                break;
            }
            atn::Vector3 pos = GetMarkPos_unk(player->m00);
            atn::Vector3 screen;
            fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
            if (IsOnScreen_unk(screen)) {
                m50[i]->SetColor_unk("col0002(1)", rank == 1 ? 0xFFE59E36 : 0xFF969696);
                m50[i]->SetColor_unk("col0003(1)", rank == 1 ? 0xFFE59E36 : 0xFFCCCCCC);
                m50[i]->mFlags &= ~4;
                SetPaneValue_unk(m50[i]->mLayout, "BasePos", 0, screen.x - 12.0f);
                SetPaneValue_unk(m50[i]->mLayout, "BasePos", 1, screen.y - 24.0f);
            }
            prev = player;
        }
    }
    if (lbl_8087F610->m540 == 1) {
        for (int i = 0; i < lbl_8087F610->m5E8; i++) {
            MpPlayer_unk* player = lbl_8087F610->GetPlayer_unk(i);
            MpPlayerInfo_unk* info = &lbl_8087F628->mInfos[i];
            if (info != 0 && player != 0 && team == player->mTeam) {
                atn::Vector3 pos = GetMarkPos_unk(player->m00);
                atn::Vector3 screen;
                fn_800BFAC8(&screen, lbl_8087EFB4, &pos);
                if (IsOnScreen_unk(screen)) {
                    m70[i]->mFlags &= ~4;
                    SetPaneValue_unk(m70[i]->mLayout, "basePoint", 0, screen.x);
                    SetPaneValue_unk(m70[i]->mLayout, "basePoint", 1, screen.y);
                    SetPaneText_unk(m70[i]->mLayout, "name",
                                    fn_804EB480(lbl_8087F610, player) != 0 ? lbl_8087F86C->Get_unk(0x130)
                                                                           : info->mName);
                    SetPaneText_unk(m70[i]->mLayout, "nameSub",
                                    fn_804EB480(lbl_8087F610, player) != 0 ? lbl_8087F86C->Get_unk(0x130)
                                                                           : info->mName);
                    if (team == 1) {
                        m70[i]->SetColor_unk("nameColor", 255.0f, 120.0f, 120.0f);
                    } else {
                        m70[i]->SetColor_unk("nameColor", 120.0f, 140.0f, 255.0f);
                    }
                }
            }
        }
    }
}

void D2AMpMenuCockpit::fn_804B6204(int time) {
    if (time < 0) {
        if (lbl_8087F610->m540 == 2) {
            SetPaneValue2_unk(m4C->mLayout, "col0040", 0, 0.0f);
            SetPaneValue2_unk(m4C->mLayout, "col0041", 0, 0.0f);
            SetPaneValue2_unk(m4C->mLayout, "col0042", 0, 0.0f);
            SetPaneValue2_unk(m4C->mLayout, "col0007", 0, 0.0f);
            Stop_unk(m4C);
        }
        return;
    }
    SetPaneValue2_unk(m4C->mLayout, "col0007", 0, 255.0f);
    Play_unk(m4C, 0, 1.0f);
    wchar_t text[0x100] = {0};
    fn_800DD3FC(text, L"%02d", time % 60);
    m4C->SetNumber_unk("string0040(1)", time / 60, 0);
    SetPaneText_unk(m4C->mLayout, "string0041(1)", text);
    MpSession_unk* session = lbl_8087F610;
    int warning = 0;
    if (fn_804DB408(session) >= 0 && fn_804DB408(session) < 60) {
        warning = 1;
    }
    if (warning != 0) {
        m4C->SetColor_unk("col0040", 0xFFFF0000);
        m4C->SetColor_unk("col0041", 0xFFFF0000);
        m4C->SetColor_unk("col0042", 0xFFFF0000);
        m4C->SetColor_unk("col0002", 0xFFFF0000);
        m4C->SetColor_unk("col0006", 0xFFFF0000);
        m4C->SetColor_unk("col0003", 0xFFFF0000);
        SetPaneValue_unk(m4C->mLayout, "pos0040", 2, 18.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0041", 2, 18.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0042", 2, 18.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0040", 3, 18.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0041", 3, 18.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0042", 3, 18.0f);
        SetPaneValue_unk(m4C->mLayout, "col0007", 1, 95.0f);
    } else {
        m4C->SetColor_unk("col0040", 0xFFFFFFFF);
        m4C->SetColor_unk("col0041", 0xFFFFFFFF);
        m4C->SetColor_unk("col0042", 0xFFFFFFFF);
        m4C->SetColor_unk("col0002", 0xFFFFFFFF);
        m4C->SetColor_unk("col0006", 0xFFFFFFFF);
        m4C->SetColor_unk("col0003", 0xFFFFFFFF);
        SetPaneValue_unk(m4C->mLayout, "pos0040", 2, 16.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0041", 2, 16.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0042", 2, 16.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0040", 3, 16.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0041", 3, 16.0f);
        SetPaneValue_unk(m4C->mLayout, "pos0042", 3, 16.0f);
        SetPaneValue_unk(m4C->mLayout, "col0007", 1, 92.0f);
    }
}

void D2AMpMenuCockpit::fn_804B671C(int value, int which) {
    wchar_t text[8] = {0};
    fn_800DD3FC(text, L"%d", value);
    if (which == 0) {
        SetPaneText_unk(m4C->mLayout, "string0036(1)", text);
    } else if (which == 1) {
        SetPaneText_unk(m4C->mLayout, "string0037(1)", text);
    }
}

// NONMATCHING: 99.62% - r30/r31 swapped for the message text and the layout (same problem as
// D2ACommonDialog::fn_804ACAF8); tried a local text, an inline getter and other helper signatures
void D2AMpMenuCockpit::fn_804B67E4(int message) {
    if (lbl_8087F490 != 0 && lbl_8087F490->m255C >= 0) {
        return;
    }
    m98->mFlags &= ~4;
    m98->mFrame = 0.0f;
    if (0 <= message && message < 7) {
        SetPaneText_unk(m98->mLayout, "message", lbl_8087F86C->Get_unk(sMessages[message]));
    }
    if (message == 6) {
        fn_800C31F4("SE_UI001_023", 0, -1, 1.0f);
    } else {
        fn_800C31F4("SE_UI001_022", 0, -1, 1.0f);
    }
}

void D2AMpMenuCockpit::fn_804B691C() {
    m90->mFrame = m90->mLastFrame;
    m94->mFrame = m94->mLastFrame;
    m98->mFrame = m98->mLastFrame;
}

void D2AMpMenuCockpit::fn_804B6944() {
    for (unsigned int i = 0; i < m10C_tasks.size(); i++) {
        m10C_tasks[i]->mFlags |= 4;
    }
    m9C = 4;
}

void D2AMpMenuCockpit::vf1C() {}

} // namespace MP
