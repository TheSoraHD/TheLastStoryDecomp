#ifndef MP_D2AMPMENULOBBY_H
#define MP_D2AMPMENULOBBY_H

#include "MP/D2AMpMenu_unk.h"
#include "MP/D2AMpMenuFriendRoom.h"

namespace atn {
class FadeColor;
}

namespace MP {

// inferred from __vt__Q22MP14D2AMpMenuLobby (0x80790CCC): lobby of a multiplayer game (raid or
// versus); the factory allocates 0xF98 bytes
class D2AMpMenuLobby : public atn::Task {
public:
    // inferred: one player slot of the lobby (0x22C bytes)
    struct Player_unk {
        Player_unk() { Clear(); }
        ~Player_unk() { Clear(); }
        void Clear() {
            m18 = 1;
            m1C = 0;
            m20 = 0;
            m24 = 0;
            m28 = 0;
            m2C = 0;
        }
        MpD2A_unk* mTasks[6]; // inferred: profile, face mark, wait, two balloons, ok
        int m18;
        int m1C;
        int m20;
        int m24;
        int m28;
        short m2C;
        unsigned char m2E[0x22C - 0x2E];
    };

    static D2AMpMenuLobby* fn_804BBBA0(atn::Task* parent, int a, int b); // inferred: creates the instance
    D2AMpMenuLobby(atn::Task* parent, int a, int b);
    virtual ~D2AMpMenuLobby();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804BC628(int state); // inferred: changes the state
    void fn_804BD9B8(); // inferred: enters the state of mD8C
    int fn_804BE330(int count); // inferred: cursor and decide/cancel keys
    void fn_804BF550();
    void fn_804C0430(); // inferred: countdown before the start
    void fn_804C0548(); // inferred: changes the face
    void fn_804C0A40();
    void fn_804C0EA4(); // inferred: refreshes the help text
    void fn_804C1634();
    void fn_804C2260(int v);
    void fn_804C28A0(int count); // inferred: selects the next entry
    void fn_804C2994(int id); // inferred: setter of mDCC (called from atn/PacketPack)
    int fn_804C2C14(); // inferred: nonzero while busy
    void fn_804C2F98(); // inferred: removes the menu task
    bool fn_804C2F9C(); // inferred: true in states 7, 8 and 10

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
        for (unsigned int i = 0; i < mTasks.size(); i++) {
            mTasks[i]->mSpeed = speed;
        }
    }

    MpD2A_unk* m48;
    MpD2A_unk* m4C;           // inferred: online_raid_base.d2a
    MpD2A_unk* m50;           // inferred: online_raid_time.d2a
    MpD2A_unk* m54;           // inferred: online_raid_logo.d2a
    MpD2A_unk* m58;           // inferred: online_versus_logo.d2a
    MpD2A_unk* m5C;           // inferred: online_face_select.d2a
    MpD2A_unk* m60;
    MpD2A_unk* m64;           // inferred: online_info_start.d2a
    MpD2A_unk* m68;           // inferred: online_connect_base.d2a
    MpD2A_unk* m6C;           // inferred: online_map_logo.d2a
    MpD2A_unk* m70;           // inferred: online_map_base.d2a
    MpD2A_unk* m74;           // inferred: online_map_list9_base.d2a
    MpD2A_unk* m78;           // inferred: online_map_list5_base.d2a
    atn::FadeColor* m7C;
    Player_unk mPlayers[6];   // 0x80
    int mD88;
    int mD8C;
    int mD90;
    int mD94;
    int mD98;
    int mD9C;
    int mDA0;
    int mDA4;
    int mDA8;
    int mDAC;
    int mDB0;
    int mDB4;
    int mDB8;
    int mDBC[4];
    int mDCC;
    int mDD0;
    int mDD4[16];
    int mE14[16];             // inferred: nonzero for a selectable entry
    int mE54;
    int mE58;
    int mE5C;
    int mE60;
    int mE64;
    int mE68;
    int mE6C;
    int mE70;
    int mE74;
    int mE78;
    int mE7C;
    atn::FixedArray_unk<MpD2A_unk*, 0x40> mTasks; // 0xE80
    MpRoomIds_unk mF84;
    int mF90;
    int mF94;

    static D2AMpMenuLobby* sInstance; // lbl_8087F5C4
};

} // namespace MP

#endif
