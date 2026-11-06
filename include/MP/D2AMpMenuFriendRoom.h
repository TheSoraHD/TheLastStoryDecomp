#ifndef MP_D2AMPMENUFRIENDROOM_H
#define MP_D2AMPMENUFRIENDROOM_H

#include "MP/D2AMpMenu_unk.h"
#include "std/vector.h"

namespace MP {

// inferred: ids of the rooms (a class derived from std::vector: its destructor tests `this` four times)
struct MpRoomIds_unk : public std::vector<int> {};

// inferred from __vt__Q22MP19D2AMpMenuFriendRoom (0x80790C08): list of the friend rooms
class D2AMpMenuFriendRoom : public atn::Task {
public:
    static D2AMpMenuFriendRoom* fn_804B9D44(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuFriendRoom(atn::Task* parent);
    virtual ~D2AMpMenuFriendRoom();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804BA350(int state); // inferred: changes the state
    void UpdateScrollBar_unk();  // inferred: not inline (its literals are in @stringBase0)
    void fn_804BA970();          // inferred: input of the list
    void fn_804BB10C();          // inferred: draw of the list
    void fn_804BB2CC(int refresh); // inferred: rebuilds the list

    // inferred: helpers of fn_804BA350 (the registers of their loops differ when written inline)
    void CloseList_unk() {
        Play_unk(m48, 0, -1.0f);
        Play_unk(m4C, 0, -1.0f);
        for (int i = 0; i < 10; i++) {
            Play_unk(m50[i], 0, -1.0f);
        }
        Play_unk(m80, 0, -1.0f);
    }
    void OpenList_unk() {
        vf1C();
        fn_804BB2CC(1);
        Play_unk(m48, 0, 1.0f);
        Play_unk(m4C, 0, 1.0f);
        for (int i = 0; i < 10; i++) {
            Play_unk(m50[i], 0, 1.0f);
        }
        Play_unk(m78, 0, 1.0f);
    }
    void Add_unk(MpD2A_unk* task) {
        if (mTasks.size() < 0x10) {
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

    MpD2A_unk* m48;     // inferred: online_groupselect_logo.d2a
    MpD2A_unk* m4C;     // inferred: online_groupselect_base.d2a
    MpD2A_unk* m50[10]; // inferred: online_groupselect_list.d2a
    MpD2A_unk* m78;     // inferred: online_info_back.d2a
    MpD2A_unk* m7C;     // inferred: online_info_cancel.d2a
    MpD2A_unk* m80;     // inferred: scroll_bar.d2a
    MpD2A_unk* m84;     // inferred: online_connect_base.d2a
    int m88;            // inferred: state
    int m8C;            // inferred: previous state
    int m90;
    int m94;            // inferred: first row shown
    int m98;
    int m9C;
    int mA0;
    int mA4;
    int mA8;
    int mAC;            // inferred: cursor (0: new room)
    int mB0;
    int mB4;            // inferred: number of rooms
    atn::FixedArray_unk<MpD2A_unk*, 0x10> mTasks; // 0xB8
    MpRoomIds_unk mFC;  // inferred: ids of the rooms
    int m108;           // inferred: refresh timer
    int m10C;

    static D2AMpMenuFriendRoom* sInstance;
};

} // namespace MP

#endif
