#ifndef MP_D2AMPMENUSETMESSAGE_H
#define MP_D2AMPMENUSETMESSAGE_H

#include "MP/D2AMpMenu_unk.h"
#include "MP/MpMessage_unk.h"
#include "MP/MpLevel_unk.h"

// inferred: multiplayer session manager (lbl_8087F610), as in MP/D2AMpMenuMessageList.cpp
struct MpMsgSession_unk {
    unsigned char m0000[0x4FC];
    int m4FC;  // inferred: error code
    unsigned char m0500[0x50C - 0x500];
    int m50C;  // inferred: state of the match (tested with m4FC == 30)
    unsigned char m0510[0x540 - 0x510];
    int m540;  // inferred: game mode
    unsigned char m0544[0x610 - 0x544];
    MpLevelTable_unk m610; // inferred: experience table
    unsigned char m0618[0x2A50 - 0x618];
    MpMessageTable_unk m2A50;
};
extern "C" MpMsgSession_unk* lbl_8087F610;
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach); // inferred: stops the sound
extern "C" atn::SoundRef fn_800C32E0(unsigned int id, int arg2, int arg3, float volume);

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
    void fn_804C9360();          // inferred: hides the windows of the previous state
    int fn_804C9C88(int a, int b); // inferred: moves the cursor (1 = decided, 2 = cancelled)
    void fn_804C9EF8();
    void fn_804CA250();          // inferred: input of the message list of a category
    void fn_804CD2F8(int move);  // inferred: scrolls the message list
    void fn_804CAAE8();          // inferred: draws the message list
    void fn_804CAD90();          // inferred: places the windows of a category
    void fn_804CB0B8();
    void fn_804CB2F0();
    void fn_804CB74C();
    void fn_804CBA1C();
    void fn_804CBBD8();
    void fn_804CBCD0();          // inferred: draws the messages of a category
    void fn_804CC618();          // inferred: resets the texts of the lists (names of the categories)
    void fn_804CC7A4();
    void fn_804CCB54();

    // inferred: plays the voice of a message
    void PlayVoice_unk(int category, int index) {
        MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(category, index);
        if (msg != 0) {
            unsigned int id = msg->mVoice;
            fn_800CB5C8(&m174, 0, 0);
            if (id != 0) {
                m174 = fn_800C32E0(id, 0, -1, 1.0f);
            }
        }
    }
    // inferred: whether a message of `category` belongs to the lobby list `list`
    static int HasMessage_unk(int category, int list) {
        int found = 0;
        int count = lbl_8087F610->m2A50.GetCount_unk(category);
        for (int j = 0; j < count; j++) {
            MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(category, j);
            if (msg != 0 && ((msg->m04 >> 8) & (1 << list)) != 0) {
                found = 1;
                break;
            }
        }
        return found;
    }
    // inferred: number of messages of a category
    static int GetMsgCount_unk(int category) {
        return lbl_8087F610->m2A50.GetCount_unk(category);
    }
    // inferred: counts the messages of the current category in the current list
    void CountMessages_unk(unsigned int count) {
        mE8 = 0;
        for (unsigned int i = 0; i < count; i++) {
            MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(m164, i);
            if (msg != 0 && ((msg->m04 >> 8) & (1 << m168)) != 0) {
                mE8++;
            }
        }
    }
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

} // namespace MP

#endif
