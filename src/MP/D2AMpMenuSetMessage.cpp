// MP/D2AMpMenuSetMessage.cpp (0x804C8778-0x804CD97C): message settings of the multiplayer lobby. Only the
// first functions are reconstructed so far.
#include "MP/D2AMpMenu_unk.h"
#include "game/MenuCommonTask.h"

// inferred: friend list (lbl_8087F628 + 0x430, as in MP/D2AMpMenuFriend.cpp)
struct MpMsgFriendList_unk;
extern "C" void fn_8050EAE8(MpMsgFriendList_unk* list);
extern "C" void fn_804F7EF0(MpMsgFriendList_unk* list, int size); // inferred: saves

// inferred: multiplayer data (lbl_8087F628); only the member used here
struct MpMsgData_unk {
    unsigned char m000[0x430];
    unsigned char m430[0x9F0]; // inferred: friend list
};
extern "C" MpMsgData_unk* lbl_8087F628;

namespace MP {

// inferred from __vt__Q22MP19D2AMpMenuSetMessage (0x80790EF0): selection of the lobby messages; the
// factory allocates 0x218 bytes
class D2AMpMenuSetMessage : public atn::Task {
public:
    static D2AMpMenuSetMessage* fn_804C8778(atn::Task* parent); // inferred: creates the instance
    D2AMpMenuSetMessage(atn::Task* parent);
    virtual ~D2AMpMenuSetMessage();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();
    virtual void vf1C();
    virtual void vf20();

    void fn_804C8D80(int state); // inferred: changes the state
    void fn_804CC618();          // inferred: reads the current messages

    void Add_unk(MpD2A_unk* task) {
        if (m178.size() < 37) {
            m178.push_back(task);
        }
    }
    void SetAll_unk(int arg) {
        for (MpD2A_unk** it = &m178[0]; it != &m178[m178.size()]; it++) {
            if (*it != 0) {
                (*it)->fn_800D246C(arg);
            }
        }
    }

    MpD2A_unk* m48;      // inferred: online_message_logo.d2a
    MpD2A_unk* m4C;      // inferred: online_message_base.d2a
    MpD2A_unk* m50;      // inferred: online_message_select_base.d2a
    MpD2A_unk* m54;      // inferred: online_message_select3.d2a
    MpD2A_unk* m58[3];   // inferred: online_message_select1.d2a
    MpD2A_unk* m64[2];   // inferred: online_message_select2.d2a
    MpD2A_unk* m6C[2];   // inferred: online_message_list.d2a
    MpD2A_unk* m74[24];  // inferred: online_message_list.d2a
    MpD2A_unk* mD4;      // inferred: scroll_bar.d2a
    int mD8;
    int mDC;
    int mE0;
    int mE4;
    int mE8;
    int mEC;
    int mF0[8];          // inferred: -1 = empty
    int m110[20];
    int m160;
    int m164;
    int m168;
    int m16C;
    float m170;
    atn::SoundRef m174;
    atn::FixedArray_unk<MpD2A_unk*, 37> m178;
    int m210;            // inferred: nonzero when the friend list has to be saved

    static D2AMpMenuSetMessage* sInstance; // lbl_8087F5EC
};

D2AMpMenuSetMessage* D2AMpMenuSetMessage::sInstance;

D2AMpMenuSetMessage* D2AMpMenuSetMessage::fn_804C8778(atn::Task* parent) {
    if (sInstance == 0) {
        sInstance = new ((MemoryTag)1, "", "", 0) D2AMpMenuSetMessage(parent);
    }
    return sInstance;
}

D2AMpMenuSetMessage::D2AMpMenuSetMessage(atn::Task* parent)
    : atn::Task(parent), mD8(0), mDC(0), mE0(0), mE4(0), mE8(0), mEC(0), m160(0), m164(0), m168(0), m16C(0),
      m170(0.0f), m210(0) {
    m4C = fn_801F3FF8(this, "/data/d2anime/online_message_base.d2a", 0);
    Add_unk(m4C);
    m48 = fn_801F3FF8(this, "/data/d2anime/online_message_logo.d2a", 0);
    Add_unk(m48);
    m50 = fn_801F3FF8(this, "/data/d2anime/online_message_select_base.d2a", 0);
    Add_unk(m50);
    for (int i = 0; i < 24; i++) {
        m74[i] = fn_801F3FF8(this, "/data/d2anime/online_message_list.d2a", 0);
        Add_unk(m74[i]);
    }
    for (int i = 0; i < 3; i++) {
        m58[i] = fn_801F3FF8(this, "/data/d2anime/online_message_select1.d2a", 0);
        Add_unk(m58[i]);
    }
    for (int i = 0; i < 2; i++) {
        m64[i] = fn_801F3FF8(this, "/data/d2anime/online_message_select2.d2a", 0);
        Add_unk(m64[i]);
        m6C[i] = fn_801F3FF8(this, "/data/d2anime/online_message_list.d2a", 0);
        Add_unk(m6C[i]);
    }
    m54 = fn_801F3FF8(this, "/data/d2anime/online_message_select3.d2a", 0);
    Add_unk(m54);
    mD4 = fn_801F3FF8(this, "/data/d2anime/scroll_bar.d2a", 0);
    Add_unk(mD4);
    for (int i = 0; i < 8; i++) {
        mF0[i] = -1;
    }
    for (int i = 0; i < 20; i++) {
        m110[i] = 0;
    }
    fn_804CC618();
    SetAll_unk(1);
    if (MenuCommonTask::sInstance == 0) {
        MenuCommonTask::fn_804A29C4(this);
    }
}

D2AMpMenuSetMessage::~D2AMpMenuSetMessage() {
    MpMsgFriendList_unk* list = (MpMsgFriendList_unk*)lbl_8087F628->m430;
    if (m210 != 0) {
        m210 = 0;
        fn_8050EAE8(list);
        fn_804F7EF0(list, 0x9F0);
    }
    m178.clear();
    if (sInstance != 0) {
        sInstance = 0;
    }
}

int D2AMpMenuSetMessage::vf0C() {
    if (fn_800D3FA4() != 0) {
        SetAll_unk(0);
        fn_804C8D80(1);
        return 1;
    }
    return 0;
}

void D2AMpMenuSetMessage::vf1C() {}

void D2AMpMenuSetMessage::vf20() {}

} // namespace MP
