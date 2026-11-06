// MP/D2AMpMenuSetMessage.cpp (0x804C8778-0x804CD97C): message settings of the multiplayer lobby. Only the
// first functions are reconstructed so far.
#include "MP/D2AMpMenu_unk.h"
#include "game/MenuCommonTask.h"
#include "MP/D2ACommonDialog.h"
#include "game/CharaTask.h"
#include "game/Input_unk.h"
#include "MP/MpMessage_unk.h"
#include "MP/D2AMpMenuMessageList.h"

// inferred: friend list (lbl_8087F628 + 0x430, as in MP/D2AMpMenuFriend.cpp)
struct MpMsgFriendList_unk;
extern "C" void fn_8050EAE8(MpMsgFriendList_unk* list);
extern "C" void fn_804F7EF0(MpMsgFriendList_unk* list, int size); // inferred: saves

// inferred: one entry of the lobby message list (category, index), as in MP/D2AMpMenuMessageList.cpp
struct MpMsgRef_unk {
    short mCategory;
    short mIndex;
};
// inferred: multiplayer data (lbl_8087F628); only the members used here
struct MpMsgData_unk {
    unsigned char m000[0x430];
    unsigned char m430[0xC38 - 0x430]; // inferred: friend list (saved with the next 0x9F0 bytes)
    MpMsgRef_unk mMessages[40];         // 0xC38
    unsigned char mCD8;
    unsigned char mCD9;
    unsigned char mCDA;
    unsigned char mCDB;
    unsigned char mCDC;
};
extern "C" CharaInfo_unk* fn_8020924C(int id);

// inferred: multiplayer session manager (lbl_8087F610), as in MP/D2AMpMenuMessageList.cpp
struct MpMsgSession_unk {
    unsigned char m0000[0x2A50];
    MpMessageTable_unk m2A50;
};
extern "C" MpMsgSession_unk* lbl_8087F610;
// inferred: game settings (lbl_8087F0A8): input at 0x48C
struct MpMsgSettings_unk {
    unsigned char m000[0x48C];
    unsigned char mPad[1]; // 0x48C
};
extern "C" MpMsgSettings_unk* lbl_8087F0A8;
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int detach); // inferred: stops the sound
extern "C" atn::SoundRef fn_800C32E0(unsigned int id, int arg2, int arg3, float volume);
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
    void fn_804C9360();          // inferred: hides the windows of the previous state
    int fn_804C9C88(int a, int b); // inferred: moves the cursor (1 = decided, 2 = cancelled)
    void fn_804C9EF8();
    void fn_804CA250();
    void fn_804CAAE8();          // inferred: draws the message list
    void fn_804CAD90();
    void fn_804CC618();          // inferred: reads the current messages
    void fn_804CC7A4();
    void fn_804CCB54();

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

// NONMATCHING: 98.4% - r28/r29 swapped between the anime and the array pointer in the Play_unk loops of
// states 4 and 5 (tried a shared loop index)
void D2AMpMenuSetMessage::fn_804C8D80(int state) {
    if (mD8 != state && state >= 0) {
        mDC = mD8;
        mD8 = state;
        fn_804C9360();
        switch (mD8) {
        case 1:
            vf1C();
            Play_unk(m4C, 0, 1.0f);
            Play_unk(m48, 0, 1.0f);
            SetTaskText_unk(m50, "string0037(1)", L"");
            SetPaneText_unk(m50->mLayout, "string0038(1)", L"");
            SetPaneText_unk(m50->mLayout, "string0039(1)", L"");
            SetPaneText_unk(m50->mLayout, "string0035(1)", L"");
            SetPaneText_unk(m50->mLayout, "string0029(1)", L"");
            mE4 = 0;
            break;
        case 2:
            mE4 = m160;
            m170 = 0.0f;
            break;
        case 3:
            D2ACommonDialog::sInstance->fn_804AC96C(0, 1.0f, 1, 0);
            D2ACommonDialog::sInstance->fn_804ACD10();
            D2ACommonDialog::sInstance->fn_804AD000(lbl_8087F86C->Get_unk(248), 0);
            break;
        case 4:
            fn_804CC618();
            fn_804CAAE8();
            mE4 = m164;
            Play_unk(m50, 0, 1.0f);
            for (int i = 0; i < 3; i++) {
                Play_unk(m58[i], 0, 1.0f);
            }
            for (int i = 0; i < 2; i++) {
                Play_unk(m64[i], 0, 1.0f);
                Play_unk(m6C[i], 0, 1.0f);
            }
            for (int i = 0; i < 24; i++) {
                Play_unk(m74[i], 0, 1.0f);
            }
            break;
        case 5:
            fn_804CC7A4();
            fn_804CAAE8();
            mE4 = m168;
            Play_unk(m50, 0, 1.0f);
            for (int i = 0; i < 3; i++) {
                Play_unk(m58[i], 0, 1.0f);
            }
            for (int i = 0; i < 2; i++) {
                Play_unk(m64[i], 0, 1.0f);
                Play_unk(m6C[i], 0, 1.0f);
            }
            for (int i = 0; i < 24; i++) {
                Play_unk(m74[i], 0, 1.0f);
            }
            break;
        case 6:
            mE4 = m16C;
            m170 = 0.0f;
            fn_804CCB54();
            fn_804CAD90();
            Play_unk(m54, 0, 1.0f);
            Play_unk(mD4, 0, 1.0f);
            break;
        case 7:
            vf20();
            Play_unk(m4C, 0, -1.0f);
            Play_unk(m48, 0, -1.0f);
            for (int i = 0; i < 10; i++) {
                Play_unk(m74[i], 0, -1.0f);
            }
            break;
        }
    }
}

void D2AMpMenuSetMessage::vf1C() {}

void D2AMpMenuSetMessage::vf20() {}

void D2AMpMenuSetMessage::fn_804C9360() {
    switch (mDC) {
    case 2:
        if (mD8 != 7 && mD8 != 3) {
            Stop_unk(m4C);
        }
        m160 = mE4;
        break;
    case 3:
        D2ACommonDialog::sInstance->fn_804ACAF8();
        D2ACommonDialog::sInstance->fn_804ACDBC();
        break;
    case 4:
        if (mD8 != 5) {
            Stop_unk(m50);
            for (int i = 0; i < 3; i++) {
                Stop_unk(m58[i]);
            }
            for (int i = 0; i < 24; i++) {
                Stop_unk(m74[i]);
            }
            for (int i = 0; i < 2; i++) {
                Stop_unk(m64[i]);
                Stop_unk(m6C[i]);
            }
        }
        if (m164 != mE4 && mD8 == 5) {
            m168 = 0;
            m16C = 0;
            mEC = 0;
        }
        m164 = mE4;
        break;
    case 5:
        if (mD8 == 5) {
            for (int i = 0; i < 3; i++) {
                Stop_unk(m58[i]);
            }
        }
        if (m168 != mE4 && mD8 == 6) {
            m16C = 0;
            mEC = 0;
        }
        m168 = mE4;
        break;
    case 6:
        m16C = mE4;
        Stop_unk(m54);
        Stop_unk(mD4);
        SetTaskText_unk(m50, "string0037(1)", L"");
        SetPaneText_unk(m50->mLayout, "string0038(1)", L"");
        SetPaneText_unk(m50->mLayout, "string0039(1)", L"");
        SetPaneText_unk(m50->mLayout, "string0035(1)", L"");
        SetPaneText_unk(m50->mLayout, "string0029(1)", L"");
        break;
    }
}

// inferred: default message list of the lobby (ten indices of category 20)
struct MpMsgIds_unk {
    int mIds[10];
};
static const MpMsgIds_unk sDefaultIds0 = {{0, 14, 15, 12, 17, 24, 29, 27, 16, 23}};
static const MpMsgIds_unk sDefaultIds1 = {{24, 25, 26, 27, 28, 30, 41, 42, 16, 23}};

// inferred: character ids of the names (0 = message table)
static const int sCharaIds[22] = {
    1, 100101, 100201, 100301, 100401, 100501, 100601, 100701, 100801, 100901, 101001,
    200561, 200562, 300501, 101101, 201004, -1, -1, -1, -1, -1, -1,
};

// inferred: name of a character for the lobby messages
static const wchar_t* GetCharaName_unk(int idx) {
    if (idx < 0 || (unsigned int)idx >= 22) {
        return 0;
    }
    if (sCharaIds[idx] > 0) {
        CharaInfo_unk* info = fn_8020924C(sCharaIds[idx]);
        if (info != 0) {
            return info->m04;
        }
    } else if (lbl_8087F86C != 0) {
        return lbl_8087F86C->Get_unk(idx + 0x2E);
    }
    return 0;
}

// inferred: name of a message for the lobby
static const wchar_t* GetMsgName_unk(int idx) {
    return lbl_8087F86C->Get_unk(idx + 0x45);
}

// NONMATCHING: 99.75% - case 5: the text goes to r31 and the layout to r29 (r29/r30 in the original)
void D2AMpMenuSetMessage::vf10() {
    if (mE0 != 0) {
        mE0 = 0;
        return;
    }
    switch (mD8) {
    case 1:
        if (m48->mFrame >= 10.0f) {
            fn_804C8D80(2);
        }
        break;
    case 2:
        fn_804C9EF8();
        break;
    case 3:
        if (D2ACommonDialog::sInstance->m4C != 0) {
            if (D2ACommonDialog::sInstance->fn_804AC79C()) {
                MpMsgIds_unk ids1;
                MpMsgIds_unk ids0 = sDefaultIds0;
                MpMsgData_unk* data = lbl_8087F628;
                for (int i = 0; i < 10; i++) {
                    data->mMessages[i].mCategory = 20;
                    data->mMessages[i].mIndex = ids0.mIds[i];
                }
                ids1 = sDefaultIds1;
                for (int i = 0; i < 10; i++) {
                    data->mMessages[i + 10].mCategory = 20;
                    data->mMessages[i + 10].mIndex = ids1.mIds[i];
                }
                for (int i = 20; i < 40; i++) {
                    data->mMessages[i].mCategory = -1;
                    data->mMessages[i].mIndex = 0;
                }
                data->mCD8 = 0;
                data->mCDA = 1;
                data->mCD9 = 2;
                data->mCDB = 8;
                data->mCDC = 9;
                fn_804C8D80(2);
            } else if (D2ACommonDialog::sInstance->fn_804AC7EC()) {
                fn_804C8D80(2);
            }
        }
        break;
    case 4:
        fn_804CAAE8();
        if (!(m50->mFrame < 10.0f)) {
            int ret = fn_804C9C88(3, 8);
            if (ret == 1) {
                if (mE4 <= 21) {
                    fn_804C8D80(5);
                    const wchar_t* text = GetCharaName_unk(m164);
                    SetPaneText_unk(m6C[0]->mLayout, "string0029(1)", text);
                }
            } else if (ret == 2) {
                fn_804C8D80(1);
            }
        }
        break;
    case 5: {
        fn_804CAAE8();
        int ret = fn_804C9C88(3, 8);
        if (ret == 1) {
            if (mE4 < 20 && m110[mE4] != 0) {
                fn_804C8D80(6);
                const wchar_t* text = GetMsgName_unk(m168);
                SetPaneText_unk(m6C[1]->mLayout, "string0029(1)", text);
            }
        } else if (ret == 2) {
            fn_804C8D80(4);
        }
        break;
    }
    case 6:
        fn_804CA250();
        break;
    case 7:
        if (m4C->mFrame <= 0.0f) {
            fn_804C8D80(8);
        }
        break;
    }
}

int D2AMpMenuSetMessage::fn_804C9C88(int rows, int cols) {
    void* pad = lbl_8087EF70;
    int result = 0;
    int cursor = mE4;
    if (cols != 0) {
        if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
            if (mE4 % cols - 1 >= 0) {
                mE4--;
            } else {
                mE4 = cols + mE4 - 1;
            }
        } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
            if (mE4 % cols + 1 < cols) {
                mE4++;
            } else {
                mE4 = mE4 - cols + 1;
            }
        }
    }
    if (rows > 1) {
        if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
            mE4 -= cols;
            if (mE4 < 0) {
                mE4 += rows * cols;
            }
        } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
            int count = rows * cols;
            mE4 += cols;
            if (mE4 >= count) {
                mE4 -= count;
            }
        }
    }
    if (mE4 != cursor) {
        fn_80117228(3);
        m170 = 0.0f;
    }
    if (fn_800A555C(pad, 0, 4) != 0) {
        if (mD8 != 6 || mE8 != 0) {
            fn_80117228(1);
            result = 1;
        }
    } else if (fn_800A555C(pad, 0, 5) != 0) {
        fn_80117228(2);
        result = 2;
    }
    return result;
}

void D2AMpMenuSetMessage::fn_804C9EF8() {
    unsigned int id;
    int ret = fn_804C9C88(1, 10);
    int page = D2AMpMenuMessageList::sPage;
    int next = page;
    if (ret == 1) {
        fn_804C8D80(4);
    } else if (ret == 2) {
        fn_804C8D80(7);
    } else if (fn_800A555C(lbl_8087EF70, 0, 0xC) != 0) {
        fn_80117228(1);
        fn_804C8D80(3);
    } else if (fn_800A555C(lbl_8087EF70, 0, 2) != 0 || fn_800A555C(lbl_8087EF70, 0, 0x17) != 0) {
        next = page - 1;
        if (next < 0) {
            next += 4;
        }
        D2AMpMenuMessageList::sPage = next;
        fn_80117228(3);
    } else if (fn_800A555C(lbl_8087EF70, 0, 3) != 0 || fn_800A555C(lbl_8087EF70, 0, 0x18) != 0) {
        next = page + 1;
        if (next >= 4) {
            next -= 4;
        }
        D2AMpMenuMessageList::sPage = next;
        fn_80117228(3);
    }
    if (next != page) {
        m170 = 0.0f;
    }
    if (((Input_unk*)lbl_8087F0A8->mPad)->fn_801231D0(0x34) != 0) {
        MpMsgRef_unk* ref = &lbl_8087F628->mMessages[mE4 + D2AMpMenuMessageList::sPage * 10];
        MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(ref->mCategory, ref->mIndex);
        if (msg != 0) {
            id = *(unsigned int*)msg->m00;
            fn_800CB5C8(&m174, 0, 0);
            if (id != 0) {
                m174 = fn_800C32E0(id, 0, -1, 1.0f);
            }
        }
    }
}




} // namespace MP
