#ifndef MP_D2AMPMENU_H
#define MP_D2AMPMENU_H

#include "MP/D2AMpMenu_unk.h"
#include "atn/LH_Binary.h"

// inferred: render parameters (lbl_8087EFA8); only the colour cleared by the menu
struct MpMenuColor_unk {
    MpMenuColor_unk(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    float r, g, b, a;
};
struct MpMenuRender_unk {
    unsigned char m00[0x3C];
    MpMenuColor_unk m3C;
};
extern "C" MpMenuRender_unk* lbl_8087EFA8;

namespace MP {

// inferred from __vt__Q22MP17D2AMpMenuRankMark (0x80790A78): rank marks of the ranking lists, read
// from /database/online/rank_mark_table.csv; the factory and the constructor are inlined in the menu
class D2AMpMenuRankMark : public atn::Task {
public:
    D2AMpMenuRankMark(atn::Task* parent);
    virtual ~D2AMpMenuRankMark();
    virtual int vf0C();

    void fn_804B4400();                                  // inferred: reads the table
    void fn_804B4678(int index, float speed, int repeat); // inferred: plays the marks
    void fn_804B4700(int index);                          // inferred: shows the marks
    void fn_804B4770(int index);                          // inferred: hides the marks
    void fn_804B47E0(int value, int index);
    void fn_804B49C0(MpRect_unk rect, int index);         // inferred: places a mark
    void fn_804B49E0(int value, int index);               // inferred: colours a mark

    // inferred: one row of the table (0x10 bytes)
    struct Entry {
        int mColor; // inferred: index of the colours
        int m04;
        int m08;
        int m0C;
    };

    // inferred: rows of the table
    struct Table_unk {
        Table_unk() : mCount(0), mEntries(0) {}
        ~Table_unk() {
            if (mEntries != 0) {
                delete[] mEntries;
            }
            mEntries = 0;
            mCount = 0;
        }
        // inferred: reallocates the rows (inline: its "" literals are in @STRING@ pools of .sdata)
        void Resize_unk(unsigned int count) {
            if (mEntries != 0) {
                delete[] mEntries;
            }
            mCount = count;
            if (count != 0) {
                mEntries = new ((MemoryTag)0, "", "", 0) Entry[count];
            } else {
                mEntries = 0;
            }
        }
        unsigned int mCount;
        Entry* mEntries;
    };

    atn::LH_Binary mTable;       // 0x48
    unsigned int mColors[8][3];  // 0x50: inferred: colours of the marks
    Table_unk mRows;             // 0xB0
    MpD2A_unk* mMarks[3];        // 0xB8: inferred: online_rank_mark.d2a

    static D2AMpMenuRankMark* sInstance; // lbl_8087F5A0
    static const char* sTablePath;       // lbl_8087E0F0
};

// inferred from __vt__Q22MP9D2AMpMenu (0x80790AB0): main multiplayer menu; the factory allocates
// 0x29B0 bytes
class D2AMpMenu : public atn::Task {
public:
    // inferred: one row of a ranking list (0x50 bytes)
    struct RANKING_DATA {
        RANKING_DATA() { Clear(); }
        ~RANKING_DATA() { Clear(); }
        void Clear() {
            m00 = 0;
            m04 = 0;
            m4C = -1;
            m48 = 0;
            fn_800DD3FC(mName, L"");
        }
        int m00;
        int m04;
        wchar_t mName[0x20]; // 0x08
        int m48;
        int m4C;
    };
    // inferred from the RTTI: ranking list (near the player, top, friends)
    class RANKING_DATA_LIST {
    public:
        RANKING_DATA_LIST() : m00(0), m04(0) {}
        int m00;
        int m04;
        virtual int GetCount() = 0;
        virtual RANKING_DATA* Get(int index) = 0;
    };
    class RANKING_DATA_LIST_IMPL_NEAR : public RANKING_DATA_LIST {
    public:
        virtual int GetCount() { return 30; }
        virtual RANKING_DATA* Get(int index) {
            if (index < 0 || index >= GetCount()) {
                return 0;
            }
            return &mData[index];
        }
        RANKING_DATA mData[30];
    };
    class RANKING_DATA_LIST_IMPL_TOP : public RANKING_DATA_LIST {
    public:
        virtual int GetCount() { return 30; }
        virtual RANKING_DATA* Get(int index) {
            if (index < 0 || index >= GetCount()) {
                return 0;
            }
            return &mData[index];
        }
        RANKING_DATA mData[30];
    };
    class RANKING_DATA_LIST_IMPL_FRIEND : public RANKING_DATA_LIST {
    public:
        virtual int GetCount() { return 65; }
        virtual RANKING_DATA* Get(int index) {
            if (index < 0 || index >= GetCount()) {
                return 0;
            }
            return &mData[index];
        }
        RANKING_DATA mData[65];
    };
    // inferred: cleared by the constructor and the destructor
    struct Work_unk {
        Work_unk() { Clear(); }
        ~Work_unk() { Clear(); }
        void Clear() {
            m00 = 0;
            m08 = 0;
            m04 = 0;
            m0C = 0;
        }
        int m00;
        int m04;
        int m08;
        int m0C;
    };

    static D2AMpMenu* fn_804AE820(atn::Task* parent); // inferred: creates the instance
    D2AMpMenu(atn::Task* parent);
    virtual ~D2AMpMenu();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804AF520(int state); // inferred: changes the state
    void fn_804B0A78();          // inferred: closes the windows of the previous state
    void fn_804B12F8();          // inferred: input of the game selection
    void fn_804B1528();          // inferred: input of the match selection (state 9)
    void fn_804B1C88();          // inferred: blinks the side marks, starts the ranking request
    void fn_804B1E48();          // inferred: input of the ranking (state 18)
    void RefreshRanking_unk();   // inferred: copies the received ranking (inline)
    void fn_804B274C();          // inferred: input of the room type
    void fn_804B2934();          // inferred: input of the own name
    void OpenKeyboard_unk();     // inferred: opens the keyboard with the own name
    void fn_804B3494();          // inferred: shows the main page of the menu
    void fn_804B3838();          // inferred: shows the windows of the ranking
    void fn_804B39F8();          // inferred: shows the game selection
    void fn_804B3C78(int before, int after, int back); // inferred: shows the change of side
    void fn_804B3EFC(int list, int first, int count);  // inferred: fills the rows of the ranking
    void fn_804B4150(int state); // inferred: sets the default rules of a game mode
    const wchar_t* fn_804B424C(int rule, int value); // inferred: text of a rule value

    void Add_unk(MpD2A_unk* task) {
        if (mTasks.size() < 0x40) {
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
    void SetSpeedAll_unk(float speed) {
        for (MpD2A_unk** it = &mTasks[0]; it != &mTasks[mTasks.size()]; it++) {
            if (*it != 0) {
                (*it)->mSpeed = speed;
            }
        }
    }

    MpD2A_unk* m48;          // inferred: online_title_top_base.d2a
    MpD2A_unk* m4C;          // inferred: online_title_logo.d2a
    MpD2A_unk* m50;          // inferred: online_match_select_base.d2a
    MpD2A_unk* m54;          // inferred: online_match_select_logo.d2a
    MpD2A_unk* m58[3];       // inferred: online_friend_on.d2a
    MpD2A_unk* m64;          // inferred: online_profile.d2a
    MpD2A_unk* m68;          // inferred: online_game_select_base.d2a
    MpD2A_unk* m6C;          // inferred: online_game_select_logo.d2a
    MpD2A_unk* m70;          // inferred: online_info_back.d2a
    MpD2A_unk* m74;          // inferred: online_info_cancel.d2a
    MpD2A_unk* m78;          // inferred: online_side_select.d2a
    MpD2A_unk* m7C;          // inferred: online_load_ranking.d2a
    MpD2A_unk* m80;          // inferred: online_ranking_base.d2a
    MpD2A_unk* m84;          // inferred: online_ranking_logo.d2a
    MpD2A_unk* m88;          // inferred: online_ranking_select.d2a
    MpD2A_unk* m8C[10];      // inferred: online_ranking_list.d2a
    MpD2A_unk* mB4;          // inferred: scroll_bar.d2a
    MpD2A_unk* mB8;          // inferred: online_connect_base.d2a
    int mBC;
    int mC0;
    int mC4;                 // inferred: state
    int mC8;
    int mCC;
    int mD0;
    int mD4;
    int mD8;
    int mDC;
    int mE0;
    int mE4;
    int mE8;
    int mEC[2];              // inferred: blink counters of the side marks
    int mF4[23];             // inferred: cursor saved for each state (-1: none)
    int m150;
    int m154;
    atn::FixedArray_unk<MpD2A_unk*, 0x40> mTasks; // 0x158
    RANKING_DATA_LIST* mLists[3];                // 0x25C
    RANKING_DATA_LIST_IMPL_NEAR mNear;           // 0x268
    RANKING_DATA_LIST_IMPL_TOP mTop;             // 0xBD4
    RANKING_DATA_LIST_IMPL_FRIEND mFriend;       // 0x1540
    Work_unk m299C;
    int m29AC;

    static D2AMpMenu* sInstance; // lbl_8087F59C
};

} // namespace MP

#endif
