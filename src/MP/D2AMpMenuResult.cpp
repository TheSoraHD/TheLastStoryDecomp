#include "MP/D2AMpMenu_unk.h"
#include "game/Inventory.h"

// inferred: character task of a multiplayer player; only the flags used here
struct MpCharaTask_unk : public atn::Task {
    unsigned char m48[0x54C - 0x48];
    unsigned int m54C;
};

// inferred: player of the multiplayer session (0xD5C bytes)
struct MpPlayer_unk {
    MpCharaTask_unk* m00;
    unsigned char m04[0xB0 - 0x04];
    int mB0;               // inferred: character id
    unsigned char mB4[0xCC - 0xB4];
    unsigned char mCC;     // inferred: slot
    unsigned char mCD[0xD0 - 0xCD];
    unsigned int mValid : 1;
    unsigned int mHost : 1; // inferred
    unsigned int mD0_2 : 4;
    unsigned int mTeam : 4; // inferred
    unsigned int mD0_lo : 22;
    unsigned char mD4[0xDC - 0xD4];
    int mDC;               // inferred: score
    int mE0;
    int mE4;
    unsigned char mE8[0xD5C - 0xE8];
};

// inferred: ranking list of the session: count, then the players
struct MpRanking_unk {
    int mCount;
    MpPlayer_unk mPlayers[8];
};

// inferred: result data of the session (lbl_8087F610 + 0x2B80)
struct MpResult_unk {
    int m00;
    int m04;               // inferred: result (win/lose/draw/...)
    MpRanking_unk m08;
    unsigned char m6AEC[0x6AF4 - 0x6AEC];
    int m6AF4;
    unsigned char m6AF8[0x6B00 - 0x6AF8];
    int m6B00;
    unsigned char m6B04[0x6B14 - 0x6B04];
    int m6B14[8];
};

// inferred: experience table of the session (lbl_8087F610 + 0x610)
struct MpLevel_unk {
    int m00;
    int m04;
    int m08;
};
struct MpLevelTable_unk;
extern "C" MpLevel_unk* fn_80507610(MpLevelTable_unk* table, int exp);
extern "C" MpLevel_unk* fn_805075C4(MpLevelTable_unk* table, int exp);
extern "C" int fn_80507764(MpLevelTable_unk* table, int exp);
struct MpLevelTable_unk {
    int GetNext_unk(int exp) { return fn_80507610(this, exp) != 0 ? fn_80507610(this, exp)->m08 : m04->m04; }
    int m00;
    MpLevel_unk* m04;
};

// inferred: slot data of the session (0x34 bytes from lbl_8087F610 + 0x368)
struct MpSlot_unk {
    wchar_t mName[0x10];
    int m20; // inferred: experience
    unsigned char m24[0x34 - 0x24];
};

// inferred: multiplayer session manager (lbl_8087F610); only the members used here
struct MpSession_unk {
    unsigned char m0000[0x368];
    MpSlot_unk mSlots[8];
    unsigned char m508[0x534 - 0x508];
    int m534;
    unsigned char m538[0x540 - 0x538];
    int m540; // inferred: game mode
    unsigned char m544[0x610 - 0x544];
    MpLevelTable_unk m610;
    unsigned char m618[0x2B80 - 0x618];
    MpResult_unk m2B80;
};
extern "C" MpSession_unk* lbl_8087F610;
extern "C" MpPlayer_unk* fn_804EB870(MpSession_unk* s); // inferred: local player
extern "C" int fn_804EB480(MpSession_unk* s, MpPlayer_unk* player);
extern "C" int fn_804EB400(MpSession_unk* s, MpCharaTask_unk* chara);
extern "C" int fn_804E4D70(MpSession_unk* s, MpPlayer_unk* player);
extern "C" int fn_804E4B44(MpSession_unk* s, MpPlayer_unk* player); // inferred: rank
extern "C" int fn_804E4C34(MpSession_unk* s);
extern "C" int fn_804DD280(MpSession_unk* s, int arg);
extern "C" void fn_804DA3F4(MpSession_unk* s);
extern "C" const wchar_t* fn_804FA88C(MpSession_unk* s, int value); // inferred: rank name

// inferred: character table entry (see game/CharaTask.h)
struct MpCharaInfo_unk {
    int m00;
    const wchar_t* mName;
};
extern "C" MpCharaInfo_unk* fn_8020924C(int id);

// inferred: item data
struct MpItem_unk {
    unsigned char m00[0x8];
    const wchar_t* mName;
};
struct MpUV_unk {
    float m[4];
};
extern "C" MpItem_unk* fn_80211480(int id);
extern "C" int fn_80444BE8(void* mgr, MpItem_unk* item);
extern "C" MpUV_unk fn_80444C50(void* mgr, int icon);

// inferred: field (lbl_8087F430)
struct MpField_unk {
    unsigned char m0000[0x54E4];
    int m54E4;
};
extern "C" MpField_unk* lbl_8087F430;
extern "C" void fn_803729B4(MpField_unk* field, MpCharaTask_unk* chara);

// inferred: main multiplayer menu (lbl_8087F59C); only the members used here
struct MpMenu_unk {
    unsigned char m0000[0x299C];
    int m299C;
    int m29A0;
    int m29A4;
    int m29A8;
};
extern "C" MpMenu_unk* lbl_8087F59C;

// inferred: rank display (lbl_8087F5A0)
struct MpRankIcon_unk;
extern "C" MpRankIcon_unk* lbl_8087F5A0;
extern "C" void fn_804B4678(MpRankIcon_unk* icon, int a, float f, int b);
extern "C" void fn_804B4700(MpRankIcon_unk* icon, int a);
extern "C" void fn_804B4770(MpRankIcon_unk* icon, int a);
extern "C" void fn_804B47E0(MpRankIcon_unk* icon, int value, int index);
extern "C" void fn_804B49C0(MpRankIcon_unk* icon, MP::MpRect_unk rect, int index);
extern "C" void fn_804B49E0(MpRankIcon_unk* icon, int value, int index);

// inferred: camp message table (lbl_8087F1E4, see game/MenuCommonTask.cpp)
struct MpCampMsgTable_unk {
    const wchar_t* Get_unk(int id) { return mText[id] != 0 ? mText[id] : MP::lbl_808813D0; }
    const wchar_t* mText[1];
};
extern "C" MpCampMsgTable_unk* lbl_8087F1E4;

extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume);
extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task
extern "C" void* lbl_8087EEB0;
extern "C" void fn_80061824(void* draw, const wchar_t* text, unsigned int color, int a, int b, int c, int d,
                            unsigned int color2, float x, float y, float z, float w, float h, float f6, float f7,
                            float f8);

// inferred: unused locals (only their partially removed initialisation is left in the original)
struct MpPoint_unk {
    MpPoint_unk() {}
    float m00[6];
};

namespace MP {

// inferred: declared in MP/D2AMpMenuCockpit.cpp; only the members used here
class D2AMpMenuCockpit {
public:
    void fn_804B50C0(int state);
    static D2AMpMenuCockpit* sInstance;
};

// inferred from __vt__Q22MP15D2AMpMenuResult (0x80790EB0): result screen of the multiplayer
class D2AMpMenuResult : public atn::Task {
public:
    static D2AMpMenuResult* fn_804C566C(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuResult(atn::Task* parent);
    virtual ~D2AMpMenuResult();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804C5E1C();          // inferred: opens the result
    void fn_804C5E9C(int state); // inferred: changes the state
    void fn_804C66D0();          // inferred: leaves the previous state
    void fn_804C6888();
    void fn_804C7408();          // inferred: places the rank icon
    void fn_804C76EC();          // inferred: sets the own results
    void fn_804C7C34(int value);
    void fn_804C7D7C();          // inferred: sets the ranking list
    bool fn_804C8204();
    void fn_804C822C();          // inferred: sets the item list
    static void fn_804C8740();   // inferred: closes the task

    void Add_unk(MpD2A_unk* task) {
        if (mTasks.size() < 0x24) {
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

    MpD2A_unk* m48;     // inferred: online_raid_result_base.d2a
    MpD2A_unk* m4C;     // inferred: online_friend_result_base.d2a
    MpD2A_unk* m50;     // inferred: online_battlerank_base.d2a
    MpD2A_unk* m54;     // inferred: online_battlerank2_base.d2a
    MpD2A_unk* m58;     // inferred: online_connect_base.d2a
    MpD2A_unk* m5C[3];  // inferred: online_result.d2a
    MpD2A_unk* m68;     // inferred: arena_timeup_xx.d2a
    MpD2A_unk* m6C[5];  // inferred: arena_win/lose/draw/clear/failed_xx.d2a
    MpD2A_unk* m80[8];  // inferred: online_battlerank_list.d2a
    MpD2A_unk* mA0[8];  // inferred: online_battlerank2_list.d2a
    MpD2A_unk* mC0;     // inferred: online_info_check.d2a
    MpD2A_unk* mC4;     // inferred: online_item_mark.d2a
    MpD2A_unk* mC8;     // inferred: online_rankup_info.d2a
    int mCC[3];
    float mD8[3];
    int mE4; // inferred: state
    int mE8; // inferred: previous state
    int mEC;
    int mF0; // inferred: result
    int mF4;
    int mF8[3];
    unsigned char m104;
    atn::FixedArray_unk<MpD2A_unk*, 0x24> mTasks; // 0x108
    int m19C;
    int m1A0; // inferred: timer
    int m1A4; // inferred: timer

    static D2AMpMenuResult* sInstance;
};

// NONMATCHING (__sinit): the store is scheduled before the reload of the return address in the original
static unsigned int sCustomData = fn_800DC6B4("CustomData");
D2AMpMenuResult* D2AMpMenuResult::sInstance;

inline void Show_unk(MpD2A_unk* task) {
    task->mFlags &= ~4;
}
D2AMpMenuResult* D2AMpMenuResult::fn_804C566C(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new (1, "", "", 0) D2AMpMenuResult(parent);
    }
    return sInstance;
}

D2AMpMenuResult::D2AMpMenuResult(atn::Task* parent)
    : atn::Task(parent), mE4(0), mE8(0), mEC(0), mF0(0), mF4(0), m104(0), m19C(0), m1A0(0) {
    m48 = fn_801F3FF8(this, "/data/d2anime/online_raid_result_base.d2a", 0);
    Add_unk(m48);
    m4C = fn_801F3FF8(this, "/data/d2anime/online_friend_result_base.d2a", 0);
    Add_unk(m4C);
    m50 = fn_801F3FF8(this, "/data/d2anime/online_battlerank_base.d2a", 0);
    Add_unk(m50);
    m54 = fn_801F3FF8(this, "/data/d2anime/online_battlerank2_base.d2a", 0);
    Add_unk(m54);
    m58 = fn_801F3FF8(this, "/data/d2anime/online_connect_base.d2a", 0);
    Add_unk(m58);
    m68 = fn_801F3FF8(this, "/data/d2anime/arena_timeup_xx.d2a", 0);
    Add_unk(m68);
    m6C[0] = fn_801F3FF8(this, "/data/d2anime/arena_win_xx.d2a", 0);
    Add_unk(m6C[0]);
    m6C[1] = fn_801F3FF8(this, "/data/d2anime/arena_lose_xx.d2a", 0);
    Add_unk(m6C[1]);
    m6C[2] = fn_801F3FF8(this, "/data/d2anime/arena_draw_xx.d2a", 0);
    Add_unk(m6C[2]);
    m6C[3] = fn_801F3FF8(this, "/data/d2anime/arena_clear_xx.d2a", 0);
    Add_unk(m6C[3]);
    m6C[4] = fn_801F3FF8(this, "/data/d2anime/arena_failed_xx.d2a", 0);
    Add_unk(m6C[4]);
    mC0 = fn_801F3FF8(this, "/data/d2anime/online_info_check.d2a", 0);
    Add_unk(mC0);
    mC4 = fn_801F3FF8(this, "/data/d2anime/online_item_mark.d2a", 0);
    Add_unk(mC4);
    mC8 = fn_801F3FF8(this, "/data/d2anime/online_rankup_info.d2a", 0);
    Add_unk(mC8);
    for (unsigned int i = 0; i < 3; i++) {
        m5C[i] = fn_801F3FF8(this, "/data/d2anime/online_result.d2a", 0);
        Add_unk(m5C[i]);
        mCC[i] = 0;
        mD8[i] = 1.0f;
    }
    for (int i = 0; i < 8; i++) {
        m80[i] = fn_801F3FF8(this, "/data/d2anime/online_battlerank_list.d2a", 0);
        Add_unk(m80[i]);
        mA0[i] = fn_801F3FF8(this, "/data/d2anime/online_battlerank2_list.d2a", 0);
        Add_unk(mA0[i]);
    }
    SetAll_unk(1);
    fn_804B4770(lbl_8087F5A0, -1);
}

D2AMpMenuResult::~D2AMpMenuResult() {
    fn_804B4770(lbl_8087F5A0, -1);
    mTasks.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int D2AMpMenuResult::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetTaskText_unk(mC0, "desc0", L"");
        SetPaneText_unk(mC0->mLayout, "string0001(1)", L"");
        return 1;
    }
    return 0;
}

void D2AMpMenuResult::fn_804C5E1C() {
    SetAll_unk(0);
    fn_804C5E9C(1);
    fn_800D246C(0);
}

// NONMATCHING: 99.71% - text/layout registers swapped in the SetPaneText pairs of states 7, 9 and 10
// (tried the text as local/argument and the SetText2-5 helper forms)
void D2AMpMenuResult::fn_804C5E9C(int state) {
    if (mE4 != state && state >= 0) {
        fn_804EB870(lbl_8087F610);
        mEC = 0;
        mE8 = mE4;
        mE4 = state;
        fn_804C66D0();
        switch (mE4) {
        case 1:
            m1A0 = 210;
            if (lbl_8087F610->m534 != 0) {
                Play_unk(m68, 0, 1.0f);
            } else {
                m1A0 >>= 1;
            }
            break;
        case 2:
            mF0 = lbl_8087F610->m2B80.m04;
            Play_unk(m6C[mF0], 0, 1.0f);
            if ((mF0 == 0 || mF0 == 1 || mF0 == 3) && lbl_8087F430 != 0 && fn_804EB870(lbl_8087F610) != 0 &&
                fn_804EB870(lbl_8087F610)->m00 != 0) {
                MpCharaTask_unk* chara = fn_804EB870(lbl_8087F610)->m00;
                chara->m54C |= 0x10;
                chara->fn_800D246C(1);
                fn_803729B4(lbl_8087F430, chara);
                m19C = 1;
            }
            D2AMpMenuCockpit::sInstance->fn_804B50C0(3);
            break;
        case 4:
        case 5:
        case 6:
            vf1C();
            fn_804B4678(lbl_8087F5A0, 0, 1.0f, 0);
            fn_804B4700(lbl_8087F5A0, 0);
            if (lbl_8087F610->m540 != 2) {
                Play_unk(m50, 0, 1.0f);
                fn_804C7D7C();
                const wchar_t* text = lbl_8087F86C->Get_unk(0x12B);
                SetPaneText_unk(mC0->mLayout, "string0003(1)", text);
            } else {
                Play_unk(m48, 0, 1.0f);
            }
            for (int i = 0; i < 3; i++) {
                Play_unk(m5C[i], 0, 1.0f);
                mF8[i] = 0;
            }
            Play_unk(mC0, 0, 1.0f);
            fn_800C31F4("SE_UI001_010", 0, -1, 1.0f);
            fn_804C76EC();
            fn_804C7408();
            break;
        case 7: {
            fn_804C822C();
            Play_unk(m54, 0, 1.0f);
            Play_unk(mC4, 0, 1.0f);
            const wchar_t* text = lbl_8087F86C->Get_unk(0x12C);
            SetTaskText_unk(mC0, "string0003(1)", text);
            if (lbl_8087F610->m2B80.m6AF4 > 0) {
                fn_800C31F4("SE_UI001_011", 0, -1, 1.0f);
                fn_804DA3F4(lbl_8087F610);
            } else {
                fn_800C31F4("SE_UI001_010", 0, -1, 1.0f);
            }
            break;
        }
        case 8:
            Play_unk(mC8, 0, 1.0f);
            fn_804B4700(lbl_8087F5A0, 0);
            fn_800C31F4("SE_UI001_017", 0, -1, 1.0f);
            m1A4 = 150;
            break;
        case 9: {
            Play_unk(m58, 1, 1.0f);
            const wchar_t* text = lbl_8087F86C->Get_unk(0xFD);
            SetTaskText_unk(m58, "cmd01", text);
            text = lbl_8087F86C->Get_unk(0xFD);
            SetTaskText_unk(m58, "string0003(1)", text);
            fn_80117228(10);
            break;
        }
        case 10: {
            Play_unk(m58, 0, 0.0f);
            Play_unk(mC0, 0, 1.0f);
            m58->mFrame = 30.0f;
            const wchar_t* text = lbl_8087F86C->Get_unk(0xFE);
            SetTaskText_unk(m58, "cmd01", text);
            text = lbl_8087F86C->Get_unk(0xFE);
            SetTaskText_unk(m58, "string0003(1)", text);
            fn_80117228(12);
            break;
        }
        case 11:
            vf20();
            Play_unk(m48, 0, -1.0f);
            if (lbl_8087F610->m540 != 2) {
                Play_unk(m50, 0, -1.0f);
                Play_unk(m54, 0, -1.0f);
                Play_unk(mC4, 0, -1.0f);
                for (int i = 0; i < 8; i++) {
                    Play_unk(m80[i], 0, -1.0f);
                    Play_unk(mA0[i], 0, -1.0f);
                }
            }
            for (int i = 0; i < 3; i++) {
                Play_unk(m5C[i], 0, -1.0f);
            }
            break;
        }
    }
}

void D2AMpMenuResult::vf1C() {}

void D2AMpMenuResult::vf20() {}

void D2AMpMenuResult::fn_804C66D0() {
    switch (mE8) {
    case 1:
        Stop_unk(m68);
        break;
    case 4:
    case 7:
        for (int i = 0; i < 3; i++) {
            Stop_unk(m5C[i]);
        }
        fn_804B4770(lbl_8087F5A0, -1);
        break;
    case 5:
    case 6:
        Stop_unk(m50);
        for (int i = 0; i < 8; i++) {
            Stop_unk(m80[i]);
        }
        fn_804B4770(lbl_8087F5A0, -1);
        break;
    case 8:
        fn_804B4770(lbl_8087F5A0, -1);
    case 10:
        Play_unk(mC0, 0, -1.0f);
        break;
    }
}

void D2AMpMenuResult::fn_804C6888() {
    SetTaskText_unk(m48, "string0002(1)(1)", L"");
}

// NONMATCHING: 98.26% - loop registers (index/pointer/task) and stack slots of the rect temporaries in
// states 5-7 (tried the index declared first, a task local, the direct call)
void D2AMpMenuResult::vf10() {
    if (m19C != 0 && lbl_8087F430 != 0 && lbl_8087F430->m54E4 != 10) {
        MpCharaTask_unk* chara = fn_804EB870(lbl_8087F610)->m00;
        chara->m54C &= ~0x10;
        chara->fn_800D246C(0);
        m19C = 0;
    }
    if (mF4 != 0) {
        mF4 = 0;
        return;
    }
    switch (mE4) {
    case 1:
        if (--m1A0 <= 0) {
            fn_804C5E9C(2);
        }
        break;
    case 2:
        if (m6C[mF0]->IsEnd_unk() && m19C == 0) {
            int mode = lbl_8087F610->m540;
            if (mode == 2) {
                fn_804C5E9C(7);
                Play_unk(mC0, 0, 1.0f);
            } else if (mode == 1) {
                fn_804C5E9C(5);
            } else if (mode == 0) {
                fn_804C5E9C(6);
            }
        }
        break;
    case 4:
        fn_804C7408();
        break;
    case 5: {
        char name[0x40] = {0};
        MpRect_unk rect;
        fn_804C7408();
        for (int i = 0; i < 8; i++) {
            sprintf(name, "ranking_list%02d", i + 1);
            rect = GetPaneRect_unk(m50, name);
            fn_801F4728(m80[i], "pos0030(1)(1)(1)", &rect);
        }
        break;
    }
    case 6: {
        char name[0x40] = {0};
        MpRect_unk rect;
        fn_804C7408();
        for (int i = 0; i < 8; i++) {
            sprintf(name, "ranking_list%02d", i + 1);
            rect = GetPaneRect_unk(m50, name);
            fn_801F4728(m80[i], "pos0030(1)(1)(1)", &rect);
        }
        break;
    }
    case 7: {
        char name[0x40] = {0};
        MpRect_unk rect;
        fn_804C7408();
        for (int i = 0; i < 8; i++) {
            sprintf(name, "ranking_list%02d", i + 1);
            rect = GetPaneRect_unk(m54, name);
            fn_801F4728(mA0[i], "pos0030(1)(1)(1)", &rect);
        }
        rect = GetPaneRect_unk(m54, "item_null");
        fn_801F4728(mC4, "pos0026(1)", &rect);
        break;
    }
    case 8:
        m1A4--;
        fn_804C7408();
        break;
    case 10:
        fn_804C5E9C(11);
        lbl_8087F59C->m299C = 2;
        lbl_8087F59C->m29A4 = 14;
        lbl_8087F59C->m29A0 = 0;
        lbl_8087F59C->m29A8 = 0;
        break;
    case 11:
        if (mC0->mFrame == 0.0f && lbl_8087F59C->m29A0 == lbl_8087F59C->m29A4) {
            fn_804C5E9C(12);
        }
        break;
    }
}

// NONMATCHING: 99.91% - the original loads x (319) before y (42) with the same registers (tried the six
// declaration orders and a precomputed shadow x)
void D2AMpMenuResult::vf14() {
    for (unsigned int i = 0; i < mTasks.size(); i++) {
        mTasks[i]->mFlags |= 4;
    }
    switch (mE4) {
    case 1:
        if (lbl_8087F610->m534 != 0) {
            Show_unk(m68);
        }
        break;
    case 2:
    case 3:
        Show_unk(m6C[mF0]);
        break;
    case 4:
        Show_unk(m5C[0]);
        fn_804B4700(lbl_8087F5A0, 0);
        if (m104 == 0) {
            Show_unk(mC0);
        }
        Show_unk(m48);
        break;
    case 5: {
        Show_unk(m5C[0]);
        fn_804B4700(lbl_8087F5A0, 0);
        if (m104 == 0) {
            Show_unk(mC0);
        }
        Show_unk(m50);
        for (int i = 0; i < 8; i++) {
            Show_unk(m80[i]);
        }
        SetPaneText_unk(m50->mLayout, "string0028(1)", L"");
        MpPlayer_unk* player = fn_804EB870(lbl_8087F610);
        const wchar_t* result;
        if (mF0 == 0) {
            result = lbl_8087F86C->Get_unk(0x132);
        } else if (mF0 == 1) {
            result = lbl_8087F86C->Get_unk(0x133);
        } else {
            result = lbl_8087F86C->Get_unk(0x134);
        }
        int id = 0x12E;
        if (player->mTeam == 2) {
            id = 0x12D;
        }
        wchar_t text[0x100];
        fn_800DD3FC(text, L"%s %s", lbl_8087F86C->Get_unk(id), result);
        float size = 24.0f;
        float y = 42.0f;
        float x = 319.0f;
        float w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 5);
        fn_80061824(lbl_8087EEB0, text, 0xFF000000, 1, 5, 0, 0, 0xFF000000, x + 1.0f - 0.5f * w, y, -10.0f, 24.0f,
                    24.0f, 0.0f, 0.0f, 0.0f);
        w = fn_8006EF48(lbl_8087EEC8, text, size, 0.0f, 1, 4);
        fn_80061824(lbl_8087EEB0, text, 0xFFFFFFFF, 1, 4, 0, 0, 0xFF000000, x - 0.5f * w, y, -10.0f, 24.0f, 24.0f,
                    0.0f, 0.0f, 0.0f);
        break;
    }
    case 6:
        Show_unk(m5C[0]);
        fn_804B4700(lbl_8087F5A0, 0);
        if (m104 == 0) {
            Show_unk(mC0);
        }
        Show_unk(m50);
        for (int i = 0; i < 8; i++) {
            Show_unk(m80[i]);
        }
        break;
    case 7:
        if (m104 == 0) {
            Show_unk(mC0);
        }
        Show_unk(m54);
        Show_unk(mC4);
        for (int i = 0; i < 8; i++) {
            Show_unk(mA0[i]);
        }
        break;
    case 8:
        Show_unk(mC8);
        break;
    case 9:
        Show_unk(m58);
        break;
    case 10:
        Show_unk(m58);
        Show_unk(mC0);
        break;
    case 11:
        vf20();
        Show_unk(m58);
        Show_unk(mC0);
        break;
    }
}

void D2AMpMenuResult::fn_804C7408() {
    MpRect_unk rect;
    if (mE4 == 8) {
        rect = GetPaneRect_unk(mC8, "rank_null");
        rect.z = -20.0f;
        fn_804B49C0(lbl_8087F5A0, rect, 0);
        fn_804B49E0(lbl_8087F5A0, lbl_8087F610->m2B80.m6B00, 0);
        return;
    }
    if (lbl_8087F610->m540 != 2) {
        rect = GetPaneRect_unk(m50, "profile_null(1)(1)");
    } else {
        rect = GetPaneRect_unk(m48, "profile_null01");
    }
    fn_801F4728(m5C[0], "pos0001(1)(1)(1)", &rect);
    rect = GetPaneRect_unk(m5C[0], "rank_null");
    rect.z = -1.0f;
    fn_804B49C0(lbl_8087F5A0, rect, 0);
    fn_804B49E0(lbl_8087F5A0, mCC[0], 0);
    rect = GetPaneRect_unk(m5C[0], "lv_b10(1)");
    if (rect.h != 0.0f) {
        rect.w *= mD8[0];
        SetPaneValue_unk(m5C[0]->mLayout, "hp_pos(1)", 2, rect.w);
        mD8[0] = 1.0f;
    }
}

// NONMATCHING: 93.99% - register allocation: the original indexes the ranking with a byte offset and keeps
// session + slot * 0x34 (folding 0x368 into the uses); also the wide string pool of the original is
// padded (L"%d" at +0x10, ours at +0xE)
void D2AMpMenuResult::fn_804C76EC() {
    char name[0x40] = {0};
    wchar_t text[0x100] = {0};
    MpRect_unk unused1;
    MpPoint_unk unused2[4];
    MpPlayer_unk* local = fn_804EB870(lbl_8087F610);
    MpResult_unk* result = &lbl_8087F610->m2B80;
    unsigned int n = 0;
    for (int i = 0; i < result->m08.mCount; i++) {
        MpPlayer_unk* player = &result->m08.mPlayers[i];
        if (local->mCC != player->mCC || local->mTeam != player->mTeam || player->mValid != 1) {
            continue;
        }
        int id1 = fn_804DD280(lbl_8087F610, 1);
        int id2 = fn_804DD280(lbl_8087F610, 2);
        if (id1 == player->mB0 || id2 == player->mB0) {
            continue;
        }
        MpSlot_unk* slot = &lbl_8087F610->mSlots[player->mCC];
        MpCharaInfo_unk* chara = fn_8020924C(player->mB0 + 700000);
        int gain = fn_804E4D70(lbl_8087F610, player);
        int exp;
        if (player->mHost) {
            exp = result->m6B00;
        } else {
            exp = slot->m20 + gain;
        }
        int max = fn_80507764(&lbl_8087F610->m610, exp);
        if (exp > max) {
            int over = exp - max;
            exp = max;
            gain -= over;
        }
        if (fn_804EB400(lbl_8087F610, player->m00) != 0) {
            exp = 0;
            gain = 0;
        }
        mF8[n] = 1;
        const wchar_t* str;
        if (fn_804EB480(lbl_8087F610, player) == 0) {
            str = slot->mName;
        } else {
            str = lbl_8087F86C->Get_unk(0x130);
        }
        SetPaneText_unk(m5C[n]->mLayout, "string0027(2)", str);
        if (fn_804EB480(lbl_8087F610, player) == 0) {
            str = fn_804FA88C(lbl_8087F610, exp - gain);
        } else {
            str = L"";
        }
        SetPaneText_unk(m5C[n]->mLayout, "string0001(1)(1)", str);
        if (chara != 0) {
            SetPaneText_unk(m5C[n]->mLayout, "string0007(1)(1)", chara->mName);
        } else {
            SetPaneText_unk(m5C[n]->mLayout, "string0007(1)(1)", L"");
        }
        m5C[n]->SetNumber_unk("string0017(1)", player->mE0, 0);
        m5C[n]->SetNumber_unk("string0018(1)", player->mE4, 0);
        fn_800DD3FC(text, L"%d", exp);
        SetPaneText_unk(m5C[n]->mLayout, "string0020(1)", fn_804EB480(lbl_8087F610, player) != 0 ? L"" : text);
        int prev = exp - gain;
        fn_804B47E0(lbl_8087F5A0, prev, n);
        mCC[n] = prev;
        if (lbl_8087F610->m610.GetNext_unk(prev) > 0) {
            int base = fn_805075C4(&lbl_8087F610->m610, prev)->m04;
            int next = lbl_8087F610->m610.GetNext_unk(prev);
#define MAX_UNK(a, b) ((a) > (b) ? (a) : (b))
#define MIN_UNK(a, b) ((a) < (b) ? (a) : (b))
            mD8[n] = MIN_UNK(MAX_UNK(1.0f - (float)(base - exp) / (float)next, 0.0f), 1.0f);
        }
        if (++n >= 3) {
            break;
        }
    }
    SetPaneText_unk(mC0->mLayout, "string0001(1)", L"");
}

// NONMATCHING: 99.63% - text/layout registers of the last SetPaneText
void D2AMpMenuResult::fn_804C7C34(int value) {
    char name[0x40] = {0};
    wchar_t text[0x100] = {0};
    fn_804B47E0(lbl_8087F5A0, value, 0);
    MpCampMsgTable_unk* table = lbl_8087F1E4;
    MpSession_unk* session = lbl_8087F610;
    const wchar_t* format = table->Get_unk(0x37B);
    fn_800DD3FC(text, format, fn_804FA88C(session, value));
    SetPaneText_unk(mC8->mLayout, "check_msg", text);
    const wchar_t* str = lbl_8087F86C->Get_unk(0x12F);
    SetTaskText_unk(mC8, "string0002(1)", str);
}

// NONMATCHING: 92.85% - same register/addressing differences as fn_804C76EC (L"\xFF0D%d" at +0x18 in the
// original pool)
void D2AMpMenuResult::fn_804C7D7C() {
    char name[0x40] = {0};
    wchar_t text[0x100];
    MpRect_unk rect;
    MpResult_unk* result = &lbl_8087F610->m2B80;
    for (int i = 0; i < 8; i++) {
        Stop_unk(m80[i]);
    }
    for (int i = 0; i < result->m08.mCount; i++) {
        MpPlayer_unk* player = &result->m08.mPlayers[i];
        if (player->mValid == 1) {
            MpSlot_unk* slot = &lbl_8087F610->mSlots[player->mCC];
            int gain = fn_804E4D70(lbl_8087F610, player);
            int exp;
            if (player->mHost) {
                exp = result->m6B00;
            } else {
                exp = slot->m20 + gain;
            }
            int max = fn_80507764(&lbl_8087F610->m610, exp);
            if (exp > max) {
                int over = exp - max;
                exp = max;
                gain -= over;
            }
            if (fn_804EB400(lbl_8087F610, player->m00) != 0) {
                exp = 0;
                gain = 0;
            }
            if (player != 0) {
                int rank = fn_804E4B44(lbl_8087F610, player);
                if (lbl_8087F610->m540 == 2 || fn_804E4C34(lbl_8087F610) == 0) {
                    SetPaneText_unk(m80[i]->mLayout, "string0006(1)", L"");
                } else {
                    m80[i]->SetNumber_unk("string0006(1)", rank + 1, 0);
                }
                const wchar_t* str;
                if (fn_804EB480(lbl_8087F610, player) == 0) {
                    str = slot->mName;
                } else {
                    str = lbl_8087F86C->Get_unk(0x130);
                }
                SetPaneText_unk(m80[i]->mLayout, "string0028(1)(1)", str);
                if (fn_804EB480(lbl_8087F610, player) == 0) {
                    str = fn_804FA88C(lbl_8087F610, exp - gain);
                } else {
                    str = L"";
                }
                SetPaneText_unk(m80[i]->mLayout, "string0029(1)", str);
                int score = player->mDC;
                if (score < 0) {
                    fn_800DD3FC(text, L"\xFF0D%d", -score);
                    SetPaneText_unk(m80[i]->mLayout, "string0027(1)", text);
                } else {
                    m80[i]->SetNumber_unk("string0027(1)", score, 0);
                }
                Play_unk(m80[i], 0, 0.0f);
                if (player->mHost == 1) {
                    m80[i]->mFrame = 12.0f;
                } else {
                    m80[i]->mFrame = 11.0f;
                }
                sprintf(name, "ranking_list%02d", i + 1);
                rect = GetPaneRect_unk(m50, name);
                fn_801F4728(m80[i], "pos0030(1)(1)(1)", &rect);
            }
        } else {
            SetPaneText_unk(m80[i]->mLayout, "string0006(1)", L"");
            SetPaneText_unk(m80[i]->mLayout, "string0028(1)", L"");
            SetPaneText_unk(m80[i]->mLayout, "string0029(1)", L"");
            SetPaneText_unk(m80[i]->mLayout, "string0027(1)", L"");
        }
    }
}

bool D2AMpMenuResult::fn_804C8204() {
    bool result = false;
    if (m19C == 0 && m1A0 == 0) {
        result = true;
    }
    return result;
}

// NONMATCHING: 89.46% - same register/addressing differences as fn_804C76EC
void D2AMpMenuResult::fn_804C822C() {
    char name[0x40] = {0};
    wchar_t text[0x100] = {0};
    MpRect_unk rect;
    MpResult_unk* result = &lbl_8087F610->m2B80;
    for (int i = 0; i < 8; i++) {
        Stop_unk(mA0[i]);
    }
    rect = GetPaneRect_unk(m54, "item_null");
    fn_801F4728(mC4, "pos0026(1)", &rect);
    const wchar_t* str = lbl_8087F86C->Get_unk(0x135);
    SetPaneText_unk(mC4->mLayout, "string0013(1)", str);
    for (int i = 0; i < result->m08.mCount; i++) {
        MpPlayer_unk* player = &result->m08.mPlayers[i];
        if (player->mValid == 1) {
            MpSlot_unk* slot = &lbl_8087F610->mSlots[player->mCC];
            if (player != 0) {
                int item = result->m6B14[player->mCC];
                if (player->mHost == 1) {
                    item = result->m6AF4;
                }
                int rank = fn_804E4B44(lbl_8087F610, player);
                if (lbl_8087F610->m540 == 2 || fn_804E4C34(lbl_8087F610) == 0) {
                    SetPaneText_unk(mA0[i]->mLayout, "string0006(1)", L"");
                } else {
                    mA0[i]->SetNumber_unk("string0006(1)", rank + 1, 0);
                }
                if (fn_804EB480(lbl_8087F610, player) == 0) {
                    str = slot->mName;
                } else {
                    str = lbl_8087F86C->Get_unk(0x130);
                }
                SetPaneText_unk(mA0[i]->mLayout, "string0028(1)(1)", str);
                mA0[i]->SetText_unk("string0029(1)", (int)"");
                if (fn_804EB480(lbl_8087F610, player) == 0 && item > 0) {
                    MpItem_unk* data = fn_80211480(item);
                    int icon = fn_80444BE8(Inventory::instance_, data);
                    if (data != 0) {
                        fn_800DD3FC(text, L"%s", data->mName);
                        SetPaneText_unk(mA0[i]->mLayout, "string0029(1)", text);
                        if (player->mHost == 1) {
                            SetPaneText_unk(mC4->mLayout, "string0013(1)", text);
                            MpUV_unk uv = fn_80444C50(Inventory::instance_, icon);
                            mC4->SetParam2_unk("uv0021(1)(1)(1)", uv.m);
                            MpUV_unk uv2 = fn_80444C50(Inventory::instance_, icon);
                            mC4->SetParam2_unk("uv0001(1)(1)", uv2.m);
                        }
                    }
                }
                Play_unk(mA0[i], 0, 0.0f);
                if (player->mHost == 1) {
                    mA0[i]->mFrame = 12.0f;
                } else {
                    mA0[i]->mFrame = 11.0f;
                }
                sprintf(name, "ranking_list%02d", i + 1);
                rect = GetPaneRect_unk(m54, name);
                fn_801F4728(mA0[i], "pos0030(1)(1)(1)", &rect);
            }
        } else {
            SetPaneText_unk(mA0[i]->mLayout, "string0006(1)", L"");
            SetPaneText_unk(mA0[i]->mLayout, "string0028(1)(1)", L"");
            SetPaneText_unk(mA0[i]->mLayout, "string0029(1)", L"");
        }
    }
}

void D2AMpMenuResult::fn_804C8740() {
    fn_800D2338(sInstance);
}

} // namespace MP
