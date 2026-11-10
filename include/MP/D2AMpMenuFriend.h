#ifndef MP_D2AMPMENUFRIEND_H
#define MP_D2AMPMENUFRIEND_H

#include "MP/D2AMpMenu_unk.h"
#include "std/vector.h"

struct MpFriendList_unk;
extern "C" int wcsncmp(const wchar_t* a, const wchar_t* b, int n);
extern "C" void fn_8050EAE8(MpFriendList_unk* list);
extern "C" void fn_8050EFCC(MpFriendList_unk* list, const wchar_t* name, int save); // inferred: sets the own name
extern "C" void fn_804F7EF0(MpFriendList_unk* list, int size); // inferred: saves
// inferred: friend list (lbl_8087F628 + 0x430)
struct MpFriendList_unk {
    // inferred: changes the own name (first 8 characters) and saves the list
    void SetOwnName_unk(const wchar_t* name) {
        if (wcsncmp(m1C0, name, 8) != 0) {
            fn_8050EFCC(this, name, 0);
            m7F8 = 0;
            fn_8050EAE8(this);
            fn_804F7EF0(this, 0x9F0);
        }
    }
    unsigned char m000[0x1C0];
    wchar_t m1C0[16];                  // inferred: own name (as atn/NetFriend_unk.cpp)
    unsigned char m1E0[0x7F8 - 0x1E0];
    int m7F8;
    unsigned char m7FC[0x9F8 - 0x7FC];
    unsigned long long m9F8;           // inferred: own friend code
    unsigned char mA00[0xA04 - 0xA00];
    int mA04;                          // inferred: result of the registration
};
extern "C" int fn_8050F5A8(MpFriendList_unk* list, int arg);  // inferred: first friend
extern "C" int fn_8050F664(MpFriendList_unk* list, int arg);  // inferred: next friend
extern "C" int fn_8050F868(MpFriendList_unk* list, int id);   // inferred: nonzero if registered
extern "C" unsigned long long fn_8050F764(MpFriendList_unk* list, int id); // inferred: friend code
extern "C" const wchar_t* fn_8050F724(MpFriendList_unk* list, int id);     // inferred: name
extern "C" int fn_8050F7D8(MpFriendList_unk* list, int id);   // inferred: state
extern "C" int fn_8050F8A0(MpFriendList_unk* list, int id);
extern "C" int fn_8050F404(MpFriendList_unk* list);           // inferred: free slot (-1: none)
extern "C" void fn_8050EBC8(MpFriendList_unk* list, unsigned long long code, int arg); // inferred: adds
extern "C" int fn_8050EF14(MpFriendList_unk* list, int id, int arg); // inferred: removes
extern "C" std::basic_string<wchar_t> fn_8050F470(unsigned long long code); // inferred: code as text

// inferred: multiplayer data (lbl_8087F628); only the members used here
// inferred: room settings sent to the other players (4 bytes at lbl_8087F628 + 0xC1)
struct MpRoomInfo_unk {
    unsigned char mHost; // inferred: fn_804EAA50
    unsigned char mMode; // inferred: game mode (lbl_8087F610 + 0x540)
    unsigned char m2[2];
};
struct MpData_unk {
    unsigned char m000[0xC1];
    MpRoomInfo_unk mC1;
    unsigned char mC5[0xC8 - 0xC5];
    int mC8; // inferred: number of friends
    unsigned char mCC[0x430 - 0xCC];
    MpFriendList_unk m430;
    int mE38; // inferred: own player id (compared with RANKING_DATA::m4C)
    unsigned char mE3C[0xC1F4 - 0xE3C];
    int mC1F4; // inferred: 1 when connected (as MP/D2AMpMenuFriendRoom.cpp)
};
extern "C" MpData_unk* lbl_8087F628;

namespace MP {

// inferred: ids of the friends (a class derived from std::vector: its destructor tests `this` four times)
struct MpFriendIds_unk : public std::vector<int> {};

// inferred from __vt__Q22MP15D2AMpMenuFriend (0x80790BC8): friend list
class D2AMpMenuFriend : public atn::Task {
public:
    static D2AMpMenuFriend* fn_804B6988(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuFriend(atn::Task* parent);
    virtual ~D2AMpMenuFriend();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804B6F08(int state); // inferred: changes the state
    void fn_804B76F4();          // inferred: leaves the previous state
    void fn_804B79F4();          // inferred: input of the list
    void fn_804B7F88();          // inferred: deletion of a friend
    void fn_804B83F4();          // inferred: input of a friend code
    void fn_804B8C54();          // inferred: draw of the list
    void fn_804B8E14();          // inferred: draw of the closing list
    void fn_804B9014();          // inferred: sets the list texts
    void fn_804B9694();          // inferred: sets the friend states
    int fn_804B988C(int id);     // inferred: message id of the state of a friend
    void fn_804B9960();          // inferred: sets the digits of the code
    unsigned int fn_804B9B68(wchar_t* text); // inferred: deletes the selected friend
    void fn_804B9D40();          // inferred: closes the task
    void UpdateScrollBar_unk();  // inferred: not inline (its literals are in @stringBase0)

    void Add_unk(MpD2A_unk* task) {
        if (mTasks.size() < 0x20) {
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
    void Rebuild_unk() {
        mIds.__shrink_unk(0); // inferred: the original computes size - size (erase of everything)
        int id;
        MpFriendList_unk* list = &lbl_8087F628->m430;
        id = fn_8050F5A8(list, 0);
        while (id != -1) {
            mIds.push_back(id);
            id = fn_8050F664(list, 0);
        }
    }

    wchar_t m48[0x100];  // inferred: message of the deletion
    unsigned char m248;
    MpD2A_unk* m24C;     // inferred: online_friendlist_logo.d2a
    MpD2A_unk* m250;     // inferred: online_friendlist_base.d2a
    MpD2A_unk* m254;     // inferred: online_friendlist_list.d2a (own code)
    MpD2A_unk* m258[10]; // inferred: online_friendlist_list.d2a
    MpD2A_unk* m280;     // inferred: online_friendlist_input_base.d2a
    MpD2A_unk* m284;     // inferred: online_friendlist_input2.d2a
    MpD2A_unk* m288[12]; // inferred: online_friendlist_input1.d2a
    MpD2A_unk* m2B8;     // inferred: scroll_bar.d2a
    int m2BC;            // inferred: state
    int m2C0;            // inferred: previous state
    int m2C4;
    int m2C8;            // inferred: first row shown
    int m2CC[2];         // inferred: fade of the arrows
    int m2D4;
    int m2D8;
    int m2DC;
    int m2E0;            // inferred: cursor
    int m2E4;            // inferred: digit cursor
    int m2E8[12];        // inferred: digits of the code
    int m318;            // inferred: row to delete
    int m31C;            // inferred: refresh timer
    int m320;
    atn::FixedArray_unk<MpD2A_unk*, 0x20> mTasks; // 0x324
    MpFriendIds_unk mIds; // 0x3A8
    int m3B4;

    static D2AMpMenuFriend* sInstance;
};

} // namespace MP

#endif
