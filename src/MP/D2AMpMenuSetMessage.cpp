// MP/D2AMpMenuSetMessage.cpp (0x804C8778-0x804CD97C): message settings of the multiplayer lobby. Only the
// first functions are reconstructed so far.
#include "MP/D2AMpMenu_unk.h"
#include "game/MenuCommonTask.h"
#include "MP/D2ACommonDialog.h"
#include "game/CharaTask.h"
#include "game/Input_unk.h"
#include "game/SetConfig.h"
#include "atn/DrawHelper.h"
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
// inferred: lobby settings saved at lbl_8087F628 + 0xC20 (message list at 0x18)
struct MpMsgSave_unk {
    unsigned char m00[0x18];
    MpMsgRef_unk mMessages[40]; // 0x18 (0xC38)
    unsigned char mB8;          // 0xCD8
    unsigned char mB9;
    unsigned char mBA;
    unsigned char mBB;
    unsigned char mBC;
};
// inferred: multiplayer data (lbl_8087F628); only the members used here
struct MpMsgData_unk {
    unsigned char m000[0x430];
    unsigned char m430[0xC20 - 0x430]; // inferred: friend list (saved with the next 0x9F0 bytes)
    MpMsgSave_unk mC20;
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
extern "C" void fn_804A4738(MP::MpD2A_unk* bar, int top, int count, int visible); // inferred: scroll bar
extern "C" int wcscmp(const wchar_t* a, const wchar_t* b);
extern "C" void fn_804A3C24(MenuCommonTask* menu, const wchar_t* text, int arg); // inferred: help text
extern "C" void fn_804A53D4(MenuCommonTask* menu, int a, int b, const MP::MpRect_unk* rect, int c); // inferred: cursor

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
    // inferred: name of the character that says a message (0 = none)
    static const wchar_t* GetSpeaker_unk(int category, int index) {
        if (category < 0) {
            return 0;
        }
        MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(category, index);
        if (msg == 0 || msg->m10 == 0) {
            return 0;
        }
        CharaInfo_unk* info = fn_8020924C(msg->m10);
        return info != 0 ? info->m04 : L"";
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
                    data->mC20.mMessages[i].mCategory = 20;
                    data->mC20.mMessages[i].mIndex = ids0.mIds[i];
                }
                ids1 = sDefaultIds1;
                for (int i = 0; i < 10; i++) {
                    data->mC20.mMessages[i + 10].mCategory = 20;
                    data->mC20.mMessages[i + 10].mIndex = ids1.mIds[i];
                }
                for (int i = 20; i < 40; i++) {
                    data->mC20.mMessages[i].mCategory = -1;
                    data->mC20.mMessages[i].mIndex = 0;
                }
                data->mC20.mB8 = 0;
                data->mC20.mBA = 1;
                data->mC20.mB9 = 2;
                data->mC20.mBB = 8;
                data->mC20.mBC = 9;
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
        MpMsgRef_unk* ref = &lbl_8087F628->mC20.mMessages[mE4 + D2AMpMenuMessageList::sPage * 10];
        MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(ref->mCategory, ref->mIndex);
        if (msg != 0) {
            id = msg->mVoice;
            fn_800CB5C8(&m174, 0, 0);
            if (id != 0) {
                m174 = fn_800C32E0(id, 0, -1, 1.0f);
            }
        }
    }
}

void D2AMpMenuSetMessage::fn_804CA250() {
    void* pad = lbl_8087EF70;
    int cursor = mE4;
    int top = mEC;
    fn_804CAD90();
    int ret = fn_804C9C88(0, 0);
    if (ret == 1) {
        MpMsgRef_unk* ref = &lbl_8087F628->mC20.mMessages[m160 + D2AMpMenuMessageList::sPage * 10];
        ref->mCategory = m164;
        ref->mIndex = mF0[mE4];
        fn_804C8D80(1);
        m210 = 1;
        return;
    }
    if (ret == 2) {
        fn_804C8D80(5);
        return;
    }
    if (mE8 != 0) {
        if (((Input_unk*)lbl_8087F0A8->mPad)->fn_801231D0(0x34) != 0) {
            PlayVoice_unk(m164, mF0[mE4]);
        } else if (fn_800A55AC(pad, 0, 0x1A) != 0 || fn_800A55AC(pad, 0, 0) != 0) {
            if (--mE4 < 0) {
                mE4 = 0;
                if (mEC != 0) {
                    mEC--;
                } else if (mE8 > 8) {
                    mE4 = 7;
                    mEC = mE8 - 8;
                }
            }
        } else if (fn_800A55AC(pad, 0, 0x19) != 0 || fn_800A55AC(pad, 0, 1) != 0) {
            if (++mE4 > 7) {
                if (mEC + mE4 < mE8) {
                    mEC = mEC + 1;
                    mE4 = 7;
                } else {
                    mE4 = 0;
                    mEC = 0;
                }
            }
        } else if (fn_800A55AC(pad, 0, 0x17) != 0 || fn_800A55AC(pad, 0, 2) != 0) {
            if (mEC == 0 && mE4 == 0 && mE8 > 8) {
                mE4 = 7;
                mEC = mE8 - 8;
            } else if ((mEC -= 8) < 0) {
                mE4 = 0;
                mEC = 0;
            }
        } else if (fn_800A55AC(pad, 0, 0x18) != 0 || fn_800A55AC(pad, 0, 3) != 0) {
            if (mE8 <= 8) {
                mE4 = mE8 - 1;
                mEC = 0;
            } else if (mEC == mE8 - 8 && mE4 == 7) {
                mE4 = 0;
                mEC = 0;
            } else {
                mEC += 8;
                if (mEC > mE8 - 8) {
                    mE4 = 7;
                    mEC = mE8 - 8;
                }
            }
        }
        if (mEC + mE4 >= mE8) {
            mE4 = cursor;
            mEC = top;
        }
    }
    if (cursor != mE4 || top != mEC) {
        m170 = 0.0f;
        fn_80117228(3);
        m50->SetNumber_unk("string0038(1)", mEC + mE4 + 1, 0);
        if (top != mEC) {
            fn_804CD2F8(mEC - top);
        }
    }
    MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(m164, mF0[mE4]);
    if (msg != 0 && (msg->m08 >> 24) < 20) {
        SetPaneValue2_unk(m50->mLayout, "col0035", 1, 255.0f);
        SetPaneValue2_unk(m50->mLayout, "col0035", 2, 255.0f);
        SetPaneValue2_unk(m50->mLayout, "col0035", 3, 255.0f);
        SetPaneValue2_unk(m50->mLayout, "col0029", 1, 255.0f);
        SetPaneValue2_unk(m50->mLayout, "col0029", 2, 255.0f);
        SetPaneValue2_unk(m50->mLayout, "col0029", 3, 255.0f);
    } else {
        SetPaneValue2_unk(m50->mLayout, "col0035", 1, 127.0f);
        SetPaneValue2_unk(m50->mLayout, "col0035", 2, 127.0f);
        SetPaneValue2_unk(m50->mLayout, "col0035", 3, 127.0f);
        SetPaneValue2_unk(m50->mLayout, "col0029", 1, 127.0f);
        SetPaneValue2_unk(m50->mLayout, "col0029", 2, 127.0f);
        SetPaneValue2_unk(m50->mLayout, "col0029", 3, 127.0f);
    }
}

void D2AMpMenuSetMessage::fn_804CAAE8() {
    MpRect_unk rect;
    char name[0x40];
    for (int i = 0; i < 3; i++) {
        sprintf(name, "online_message_select1_%02d", i + 1);
        rect = GetPaneRect_unk(m50, name);
        fn_801F4728(m58[i], "pos0114(1)", &rect);
    }
    for (int i = 0; i < 24; i++) {
        int page = (int)(i / 8.0f);
        sprintf(name, "message_list%02d", i % 8 + 1);
        rect = GetPaneRect_unk(m58[page], name);
        fn_801F4728(m74[i], "pos0030(1)(1)(1)", &rect);
    }
    for (int i = 0; i < 2; i++) {
        sprintf(name, "online_message_select2_%02d", i + 1);
        rect = GetPaneRect_unk(m50, name);
        fn_801F4728(m64[i], "pos0114(1)", &rect);
        rect = GetPaneRect_unk(m64[i], "message_list01");
        fn_801F4728(m6C[i], "pos0030(1)(1)(1)", &rect);
    }
}

void D2AMpMenuSetMessage::fn_804CAD90() {
    MpRect_unk rect;
    char name[0x40];
    for (int i = 0; i < 2; i++) {
        sprintf(name, "online_message_select2_%02d", i + 1);
        rect = GetPaneRect_unk(m50, name);
        fn_801F4728(m64[i], "pos0114(1)", &rect);
        rect = GetPaneRect_unk(m64[i], "message_list01");
        fn_801F4728(m6C[i], "pos0030(1)(1)(1)", &rect);
    }
    rect = GetPaneRect_unk(m50, "online_message_select1_01");
    fn_801F4728(m54, "pos0114(1)", &rect);
    rect = GetPaneRect_unk(m54, "sb_null_0003(1)");
    rect.h = 1.0f;
    fn_801F4728(mD4, "basePos", &rect);
    SetPaneFloat_unk(mD4->mLayout, "s_hightMax", 192.0f);
}

void D2AMpMenuSetMessage::vf14() {
    for (unsigned int i = 0; i < m178.size(); i++) {
        m178[i]->mFlags |= 4;
    }
    switch (mD8) {
    case 1:
    case 2:
    case 3:
        m4C->mFlags &= ~4;
        m48->mFlags &= ~4;
        if (!(m48->mFrame < 2.0f)) {
            fn_804CB0B8();
            fn_804CB2F0();
        }
        break;
    case 4:
        fn_804CB74C();
        break;
    case 5:
        fn_804CBA1C();
        break;
    case 6:
        fn_804CBBD8();
        break;
    case 7:
        vf20();
        m4C->mFlags &= ~4;
        m48->mFlags &= ~4;
        break;
    }
}

// inferred: places the cursor of the common menu on the current item
void D2AMpMenuSetMessage::fn_804CB0B8() {
    if (MenuCommonTask::sInstance != 0 && D2ACommonDialog::sInstance->m4C == 0 && (mD8 != 6 || mE8 != 0)) {
        MpRect_unk rect;
        char name[0x40];
        if (mD8 == 2) {
            sprintf(name, "message_list%02d", mE4 + 1);
            MpRect_unk pos = GetPaneRect_unk(m4C, name);
            rect.x = pos.x;
            rect.y = pos.y;
            rect.w = 2.56f;
            rect.h = 0.24f;
            rect.z = -10.0f;
        } else if (mD8 == 4 || mD8 == 5) {
            int page = (int)(mE4 / 8.0f);
            sprintf(name, "message_list%02d", mE4 % 8 + 1);
            MpRect_unk pos = GetPaneRect_unk(m58[page], name);
            rect.x = pos.x;
            rect.y = pos.y;
            rect.w = 1.28f;
            rect.h = 0.24f;
            rect.z = -10.0f;
        } else if (mD8 == 6) {
            sprintf(name, "message_list%02d", mE4 + 9);
            MpRect_unk pos = GetPaneRect_unk(m54, name);
            rect.x = pos.x;
            rect.y = pos.y;
            rect.w = 3.18f;
            rect.h = 0.24f;
            rect.z = -10.0f;
        }
        fn_804A53D4(MenuCommonTask::sInstance, 0, 0, &rect, 0);
    }
}

// inferred: draws the messages of the current page
// NONMATCHING: 99.5% - the layout of the controllerType text goes to r28 (the string base) instead of r27
void D2AMpMenuSetMessage::fn_804CB2F0() {
    char name[0x40] = {0};
    MpRect_unk pos;
    float scroll = 0.0f;
    float scale = 0.05f;
    float speed = 1.5f;
    float width = 220.0f;
    MpMsgSave_unk* save = &lbl_8087F628->mC20;
    if (D2ACommonDialog::sInstance->m4C == 0) {
        if ((m170 += 1.0f) < 50.0f) {
            scroll = 0.0f;
        } else {
            scroll = (m170 - 50.0f) * speed;
        }
    }
    m4C->SetNumber_unk("page", D2AMpMenuMessageList::sPage + 1, 0);
    if (SetConfig::sInstance->mInTacviewType == 0) {
        SetPaneText_unk(m4C->mLayout, "controllerType", L"$Bu");
    } else {
        SetPaneText_unk(m4C->mLayout, "controllerType", L"$Bd");
    }
    for (int i = 0; i < 10; i++) {
        MpMsgRef_unk* ref = &save->mMessages[i + D2AMpMenuMessageList::sPage * 10];
        MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(ref->mCategory, ref->mIndex);
        const wchar_t* text = msg == 0 ? 0 : (msg->mText != 0 ? msg->mText : lbl_808813D0);
        sprintf(name, "message_list%02d", i + 1);
        pos = GetPaneRect_unk(m4C, name);
        pos.x += 4.0f;
        pos.y += 4.0f;
        if (text != 0) {
            float off = i == mE4 ? scroll : 0.0f;
            lbl_8087EEB0->fn_80061AE4(text, -1, 1, 1, 0, 1, 0xFF000000, pos.x - off, pos.y, -5.0f, 17.0f, 17.0f, 0.0f, 0.0f, 0.0f, pos.x - width * scale, width, scale);
            if (i == mE4) {
                float w = fn_8006EF48(lbl_8087EEC8, text, 17.0f, 0.0f, 1, 1);
                if (scroll > w) {
                    m170 = 0.0f;
                }
            }
        }
    }
}

// inferred: unused locals (only their partially removed initialisation is left in the original)
struct MpMsgPoint_unk {
    MpMsgPoint_unk() {}
    float m00[6];
};

// inferred: shows the category list
void D2AMpMenuSetMessage::fn_804CB74C() {
    char name[0x40] = {0};
    wchar_t text[0x100] = {0};
    MpRect_unk unused1;
    MpMsgPoint_unk unused2[4];
    m48->mFlags &= ~4;
    m50->mFlags &= ~4;
    for (int i = 0; i < 3; i++) {
        m58[i]->mFlags &= ~4;
    }
    for (int i = 0; i < 2; i++) {
        m64[i]->mFlags &= ~4;
        m6C[i]->mFlags &= ~4;
    }
    for (int i = 0; i < 24; i++) {
        m74[i]->mFlags &= ~4;
    }
    if (!(m50->mFrame < 10.0f)) {
        fn_804CB0B8();
        if (mE4 < 19) {
            fn_800DD3FC(text, lbl_8087F86C->Get_unk(0x103), GetCharaName_unk(mE4));
            fn_804A3C24(MenuCommonTask::sInstance, text, 0);
        } else if (mE4 < 22) {
            fn_804A3C24(MenuCommonTask::sInstance, lbl_8087F86C->Get_unk(mE4 + 0xF1), 0);
        }
    }
}

// inferred: shows the message list of a category
void D2AMpMenuSetMessage::fn_804CBA1C() {
    m48->mFlags &= ~4;
    m50->mFlags &= ~4;
    for (int i = 0; i < 3; i++) {
        m58[i]->mFlags &= ~4;
    }
    for (int i = 0; i < 2; i++) {
        m64[i]->mFlags &= ~4;
        m6C[i]->mFlags &= ~4;
    }
    for (int i = 0; i < 24; i++) {
        m74[i]->mFlags &= ~4;
    }
    if (!(m50->mFrame < 10.0f)) {
        fn_804CB0B8();
        if (mE4 < 20) {
            const wchar_t* text = lbl_8087F86C->Get_unk(mE4 + 0x107);
            if (wcscmp(text, L"") != 0) {
                fn_804A3C24(MenuCommonTask::sInstance, text, 0);
            }
        }
    }
}

// inferred: shows the messages of a category
void D2AMpMenuSetMessage::fn_804CBBD8() {
    MpRect_unk unused1;
    MpMsgPoint_unk unused2[4];
    m48->mFlags &= ~4;
    m50->mFlags &= ~4;
    m54->mFlags &= ~4;
    mD4->mFlags &= ~4;
    for (int i = 0; i < 2; i++) {
        m64[i]->mFlags &= ~4;
        m6C[i]->mFlags &= ~4;
    }
    if (!(m54->mFrame < 2.0f)) {
        fn_804A4738(mD4, mEC, mE8, 8);
        fn_804CB0B8();
        fn_804CBCD0();
    }
}

void D2AMpMenuSetMessage::fn_804CBCD0() {
    char name[0x40] = {0};
    MpRect_unk pos;
    float scroll = 0.0f;
    float scale = 0.05f;
    float speed = 1.5f;
    float width = 310.0f;
    float nameWidth = 120.0f;
    if (D2ACommonDialog::sInstance->m4C == 0) {
        if ((m170 += 1.0f) < 50.0f) {
            scroll = 0.0f;
        } else {
            scroll = (m170 - 50.0f) * speed;
        }
    }
    for (unsigned int i = 0; i < 8; i++) {
        int index = mF0[i];
        MpMessage_unk* msg = lbl_8087F610->m2A50.fn_804C54FC(m164, index);
        if (msg == 0) {
            continue;
        }
        sprintf(name, "message_list%02d", i + 9);
        pos = GetPaneRect_unk(m54, name);
        pos.x += 15.0f;
        pos.y += 4.0f;
        const wchar_t* text = msg->mText != 0 ? msg->mText : lbl_808813D0;
        if (text != 0) {
            float off = i == mE4 ? scroll : 0.0f;
            lbl_8087EEB0->fn_80061AE4(text, -1, 1, 1, 0, 1, 0xFF000000, pos.x - off, pos.y, -10.0f, 17.0f, 17.0f, 0.0f, 0.0f, 0.0f, pos.x - width * scale, width, scale);
            if (i == mE4) {
                float w = fn_8006EF48(lbl_8087EEC8, text, 17.0f, 0.0f, 1, 1);
                if (scroll > w) {
                    m170 = 0.0f;
                }
            }
        }
        sprintf(name, "message_list%02d", i + 1);
        pos = GetPaneRect_unk(m54, name);
        pos.x += 8.0f;
        pos.y += 4.0f;
        const wchar_t* who;
        if (GetSpeaker_unk(m164, index) == 0) {
            int kind = msg->m08 >> 24;
            if (kind == 19) {
                who = lbl_8087F86C->Get_unk(0x44);
            } else {
                who = GetCharaName_unk(kind);
            }
        } else {
            who = GetSpeaker_unk(m164, index);
        }
        if (who != 0) {
            int w = fn_8006EF48(lbl_8087EEC8, who, 17.0f, 0.0f, 1, 1);
            if (w < 119) {
                lbl_8087EEB0->fn_80061824(pos.x, pos.y, -10.0f, 17.0f, 17.0f, 0.0f, 0.0f, 0.0f, who, -1, 1, 1, 0, 0, 0xFF000000);
            } else {
                float off = 0.0f;
                if (i == mE4 && scroll < w) {
                    off = scroll;
                }
                lbl_8087EEB0->fn_80061AE4(who, -1, 1, 1, 0, 1, 0xFF000000, pos.x - off, pos.y, -10.0f, 17.0f, 17.0f, 0.0f, 0.0f, 0.0f, pos.x - nameWidth * scale, nameWidth, scale);
            }
        }
    }
}

// NONMATCHING: 99% - r28/r29 swapped (hoisted "col0030" and the text/layout)
void D2AMpMenuSetMessage::fn_804CC618() {
    MpRect_unk unused1;
    MpMsgPoint_unk unused2[4];
    for (int i = 0; i < 24; i++) {
        m74[i]->SetText_unk("string0029(1)", (int)"");
        SetPaneValue2_unk(m74[i]->mLayout, "col0030", 0, 255.0f);
        if (i <= 21) {
            const wchar_t* text = GetCharaName_unk(i);
            if (text != 0) {
                SetPaneText_unk(m74[i]->mLayout, "string0029(1)", text);
            }
        }
    }
    for (int i = 0; i < 2; i++) {
        SetPaneText_unk(m6C[i]->mLayout, "string0029(1)", L"");
    }
}

// inferred: names of the categories, greyed out when no message of the current list is in them
// NONMATCHING: 99.4% - r22/r23 swapped for the first text and its layout
void D2AMpMenuSetMessage::fn_804CC7A4() {
    MpRect_unk unused1;
    MpMsgPoint_unk unused2[4];
    for (int i = 0; i < 24; i++) {
        m74[i]->SetText_unk("string0029(1)", (int)"");
        if (i < 20) {
            const wchar_t* text = lbl_8087F86C->Get_unk(i + 0x45);
            if (text != 0) {
                SetTaskText_unk(m74[i], "string0029(1)", text);
            }
            if (HasMessage_unk(m164, i)) {
                m110[i] = 1;
                SetPaneValue2_unk(m74[i]->mLayout, "col0030", 0, 255.0f);
            } else {
                m110[i] = 0;
                SetPaneValue2_unk(m74[i]->mLayout, "col0030", 0, 100.0f);
            }
        }
    }
    SetTaskText_unk(m6C[1], "string0029(1)", L"");
}




} // namespace MP
