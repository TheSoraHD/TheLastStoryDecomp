#include "game/GameChoiceWindow.h"
#include "game/GameTask.h"
#include "game/GameScene.h"
#include "game/Input_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/GameUI.h"

extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_800DC1DC(const wchar_t* s);                 // inferred: wide string to int
extern "C" unsigned int fn_80209120(const char* name);        // inferred: id of a character name
extern "C" int fn_8006F420(void* font, char* dst, const wchar_t* src, int size); // inferred: to the game encoding
extern "C" void* lbl_8087EEC8;                                // inferred: font
extern "C" const wchar_t lbl_8077A0A8[];                      // inferred: separators of the message CSV

// inferred: layouts of the message windows (names invented)
static const char* const sMessageLayout_unk = "/data/d2anime/ui_talk.d2a";
static const char* const sTextLayout_unk = "/data/d2anime/ui_friend_mes.d2a";

// inferred: parses the message CSV: a first pass counts the messages and the text length, the second
// one fills them (the texts lose the "\r" that are not followed by a line feed)
// NONMATCHING: 96.6% - the BOM test of the CsvU constructor keeps size / 2 - 1 in another register (the
// original computes size / 2 - 2 from size / 2); the rest are the registers that follow
void GameMessageList::fn_803B35FC(const wchar_t* data, unsigned int size) {
    if (data == 0) {
        return;
    }
    atn::CsvU csv(data, size / 2 - 1);
    unsigned int count = 0;
    unsigned int length = 0;
    for (unsigned int i = 0; i < 4; i++) {
        csv.fn_8005BBF8();
    }
    do {
        if (fn_800DC1DC(csv.fn_8005B9CC()) != 1) {
            count++;
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
            csv.fn_8005B9CC();
            length += wcslen(csv.fn_8005B9CC()) + 1;
        }
    } while (csv.fn_8005BBF8() != 0);
    mTexts.Resize_unk(length, 1);
    mMessages.Resize_unk(count, 1);
    csv.SetData(data, size / 2 - 1);
    unsigned int pos = 0;
    unsigned int index = 0;
    for (unsigned int i = 0; i < 4; i++) {
        csv.fn_8005BBF8();
    }
    char name[0x10];
    do {
        if (fn_800DC1DC(csv.fn_8005B9CC()) != 1) {
            GameMessage* msg = &mMessages.mData[index];
            csv.fn_8005B9CC();
            msg->mId = fn_800DC1DC(csv.fn_8005B9CC());
            msg->m04 = fn_800DC1DC(csv.fn_8005B9CC());
            msg->mText = &mTexts.mData[pos];
            wcscpy(msg->mText, csv.fn_8005B9CC());
            unsigned int n = wcslen(msg->mText);
            for (unsigned int j = 0; j < n; j++) {
                if (msg->mText[j] == L'\r' && msg->mText[j + 1] != L'\n') {
                    for (unsigned int k = j; k < n; k++) {
                        msg->mText[k] = msg->mText[k + 1];
                    }
                    if (--n == 0) {
                        break;
                    }
                }
            }
            fn_8006F420(lbl_8087EEC8, name, csv.fn_8005B9CC(), 0x10);
            msg->mTalker = fn_80209120(name);
            fn_8006F420(lbl_8087EEC8, name, csv.fn_8005B9CC(), 0x10);
            msg->mName = name;
            msg->m30 = fn_800DC1DC(csv.fn_8005B9CC());
            msg->m34 = fn_800DC1DC(csv.fn_8005B9CC());
            index++;
            pos += wcslen(msg->mText) + 1;
        }
    } while (csv.fn_8005BBF8() != 0);
}

void GameMessageList::fn_803B3AC4() {
    mMessages.Free_unk();
    mTexts.Free_unk();
    mFile.fn_80473F88();
}

// inferred: messages replaced while an event counter is set (id, replacement, counter)
struct GameMessageAlias_unk {
    int mId;
    int mAlias;
    int mCounter;
};
static const GameMessageAlias_unk sAliases_unk[6] = {
    {0x3B10, 0x3B13, 0xC8}, {0x3B11, 0x3B14, 0xC9}, {0x3B12, 0x3B15, 0xCA},
    {0x3B06, 0x3B09, 0xC8}, {0x3B07, 0x3B0A, 0xC9}, {0x3B08, 0x3B0B, 0xCA},
};

// inferred: field (lbl_8087F430); only the members used here
struct GameChoiceField_unk {
    unsigned char m0000[0x10D8];
    void* m10D8; // inferred: the scene (game/GameScene)
};
extern "C" GameChoiceField_unk* lbl_8087F430;

GameMessage* GameMessageList::fn_803B3B38(int id) const {
    if (lbl_8087F430 != 0) {
        const GameMessageAlias_unk* alias = sAliases_unk;
        for (unsigned int i = 0; i < 6; i++, alias++) {
            if (alias->mId == id && ((GameTask*)lbl_8087F430)->fn_80370174(alias->mCounter) == 1) {
                id = alias->mAlias;
                break;
            }
        }
    }
    for (unsigned int i = 0; i < (unsigned int)mMessages.mSize; i++) {
        if (id == mMessages.mData[i].mId) {
            return &mMessages.mData[i];
        }
    }
    return 0;
}

void GameMessageList::fn_803B3C10(const char* path) {
    fn_803B3AC4();
    mFile.vf0C(path);
}

int GameMessageList::fn_803B3CA8() {
    if (mFile.fn_80473F50() != 0) {
        return 1;
    }
    if (mMessages.mSize == 0) {
        int size = mFile.fn_8047059C();
        fn_803B35FC((const wchar_t*)mFile.fn_80470580(), size);
        mFile.fn_80473F88();
    }
    return 0;
}

// inferred: converts the tags of a message text (LogMessageTask calls it)
// NONMATCHING: stub (the tag replacement is not written yet)
extern "C" void fn_803B3D2C(std::basic_string<wchar_t>* text) {
    (void)text;
}

// inferred: sound manager (lbl_8087EFE8); only the member used here
struct GameChoiceSound_unk {
    unsigned char m0000[0x34D0];
    int m34D0; // inferred: category of the next sound
};
extern "C" GameChoiceSound_unk* lbl_8087EFE8;
extern "C" atn::SoundRef fn_800C31F4(const char* name, int arg2, int arg3, float volume); // inferred: 2D sound

// inferred: plays a sound of the interface
extern "C" atn::SoundRef fn_803B42E8(const char* name, float volume) {
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 5;
    }
    atn::SoundRef ref = fn_800C31F4(name, 0, -1, volume);
    if (lbl_8087EFE8 != 0) {
        lbl_8087EFE8->m34D0 = 0;
    }
    return ref;
}

static const char* const sSounds_unk[2] = {"SE_UICOM_003", "SE_UICOM_027"};

extern "C" atn::SoundRef fn_803B4338(int index, float volume) {
    return fn_803B42E8(sSounds_unk[index], volume);
}

extern "C" GameLayout_unk* fn_801F64D0(atn::Task* parent, const char* path); // inferred: creates a layout
extern "C" void fn_801F6C80(GameLayout_unk* layout, const char* name, float value);
extern "C" float fn_801F6C2C(GameLayout_unk* layout); // inferred: end frame
// inferred: sets a texture rectangle (D2A_TaskInstanceBase<32, 8>::SetValues2_unk; the rectangle is copied)
extern "C" void fn_801F7590(GameLayout_unk* layout, const char* name, ButtonUv_unk uv);
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
extern "C" void* lbl_8087EF70; // inferred: pad
// inferred: settings (lbl_8087F0A8); only the member used here
struct GameChoiceSettings_unk {
    unsigned char m000[0x48C];
    Input_unk mInput; // 0x48C
};
extern "C" GameChoiceSettings_unk* lbl_8087F0A8;

// inferred: creates the window, as fn_803B5470 and fn_803B612C for the other windows; unused, so removed
// by the linker (not in the DOL), but its literal is the first one of @stringBase0
GameChoiceWindow* GameChoiceWindow::Create_unk(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameChoiceWindow(parent);
    }
    return 0;
}

GameChoiceWindow::GameChoiceWindow(atn::Task* parent)
    : atn::Task(parent), mMessage(0), mTimer(0), mState(0), mChoice(0), mCount(0) {
    mMain = fn_801F64D0(this, "/data/d2anime/ui_select_4.d2a");
    mMain->mLoop = 0;
    mMain->fn_800D246C(1);
    mOptions[0] = fn_801F64D0(this, "/data/d2anime/ui_select_4_l.d2a");
    mOptions[0]->fn_800D246C(1);
    mOptions[1] = fn_801F64D0(this, "/data/d2anime/ui_select_4_r.d2a");
    mOptions[1]->fn_800D246C(1);
    mOptions[2] = fn_801F64D0(this, "/data/d2anime/ui_select_4_u.d2a");
    mOptions[2]->fn_800D246C(1);
    mOptions[3] = fn_801F64D0(this, "/data/d2anime/ui_select_4_d.d2a");
    mOptions[3]->fn_800D246C(1);
    mSelected = 0;
}

GameChoiceWindow::~GameChoiceWindow() {}

int GameChoiceWindow::vf0C() {
    if (fn_800D3FA4()) {
        mMain->fn_800D246C(0);
        mMain->FreezeTask();
        mMain->mLoop = 0;
        mSelected = 0;
        for (int i = 0; i < 4; i++) {
            mOptions[i]->fn_800D246C(0);
            mOptions[i]->FreezeTask();
            mOptions[i]->mLoop = 0;
            mOptions[i]->mSpeed = 0.0f;
        }
        return 1;
    }
    return 0;
}

// NONMATCHING: first version
void GameChoiceWindow::vf10() {
    switch (mState) {
    case 1:
        mTimer++;
        mOptions[0]->mFrame = mTimer;
        mOptions[1]->mFrame = mTimer;
        mOptions[2]->mFrame = mTimer;
        mOptions[3]->mFrame = mTimer;
        if (mTimer >= 15.0f) {
            mState = 2;
            mTimer = 0;
            mStick = lbl_8087F0A8->mInput.fn_801248DC(0) < 0.4f;
        }
        break;
    case 4:
        mOptions[0]->mFrame = 31.0f + mTimer;
        mOptions[1]->mFrame = 31.0f + mTimer;
        mOptions[2]->mFrame = 31.0f + mTimer;
        mOptions[3]->mFrame = 31.0f + mTimer;
        if (mSelected != 0) {
            mSelected->mFrame = 16.0f + mTimer;
        }
        mTimer++;
        if (mSelected != 0) {
            if (mTimer >= 15.0f) {
                mState = 0;
                mSelected = 0;
            }
        } else {
            mState = 0;
        }
        break;
    case 2:
        mOptions[0]->mFrame = 15.0f;
        mOptions[1]->mFrame = 15.0f;
        mOptions[2]->mFrame = 15.0f;
        mOptions[3]->mFrame = 15.0f;
        if (mStick) {
            if (fn_800A555C(lbl_8087EF70, 0, 0x17)) {
                if (mEnabled[0]) {
                    Select_unk(0);
                }
            } else if (fn_800A555C(lbl_8087EF70, 0, 0x18)) {
                if (mEnabled[1]) {
                    Select_unk(1);
                }
            } else if (fn_800A555C(lbl_8087EF70, 0, 0x1A)) {
                if (mEnabled[2]) {
                    Select_unk(2);
                }
            } else if (fn_800A555C(lbl_8087EF70, 0, 0x19)) {
                if (mEnabled[3]) {
                    Select_unk(3);
                }
            }
        } else {
            mStick = lbl_8087F0A8->mInput.fn_801248DC(0) < 0.4f;
        }
        break;
    case 3:
        mOptions[0]->mFrame = 15.0f;
        mOptions[1]->mFrame = 15.0f;
        mOptions[2]->mFrame = 15.0f;
        mOptions[3]->mFrame = 15.0f;
        break;
    }
    if (mState == 0) {
        mMain->FreezeTask();
        mOptions[0]->FreezeTask();
        mOptions[1]->FreezeTask();
        mOptions[2]->FreezeTask();
        mOptions[3]->FreezeTask();
    } else {
        mMain->MeltTask();
        for (int i = 0; i < 4; i++) {
            if (mEnabled[i]) {
                mOptions[i]->MeltTask();
            } else {
                mOptions[i]->FreezeTask();
            }
        }
    }
    atn::D2A_DrawRect rect;
    rect = mMain->GetRect_unk("select_left");
    mOptions[0]->SetValues1Rect_unk("basePoint", rect);
    fn_801F6C80(mOptions[0], "canSelect", 1.0f);
    fn_801F6C80(mOptions[0], "skipSelect", 0.0f);
    rect = mMain->GetRect_unk("select_right");
    mOptions[1]->SetValues1Rect_unk("basePoint", rect);
    fn_801F6C80(mOptions[1], "canSelect", 1.0f);
    fn_801F6C80(mOptions[1], "skipSelect", 0.0f);
    rect = mMain->GetRect_unk("select_up");
    mOptions[2]->SetValues1Rect_unk("basePoint", rect);
    fn_801F6C80(mOptions[2], "canSelect", 1.0f);
    fn_801F6C80(mOptions[2], "skipSelect", 0.0f);
    rect = mMain->GetRect_unk("select_down");
    mOptions[3]->SetValues1Rect_unk("basePoint", rect);
    fn_801F6C80(mOptions[3], "canSelect", 1.0f);
    fn_801F6C80(mOptions[3], "skipSelect", 0.0f);
}

int GameChoiceWindow::fn_803B4C24() {
    return mState == 3;
}

// NONMATCHING: stub (splits the text into the choices, a std::vector of wide strings); noinline until
// it is written (the original, 2104 bytes, is always called with bl)
__declspec(noinline) void GameChoiceWindow::fn_803B4C38() {
    mChoice = -1;
    mCount = 1;
}

GameMessageWindow* GameMessageWindow::fn_803B5470(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameMessageWindow(parent);
    }
    return 0;
}


GameMessageWindow::GameMessageWindow(atn::Task* parent) : atn::Task(parent), mMessage(0), mState(0), mTimer(0) {
    mLayout = fn_801F64D0(this, sMessageLayout_unk);
    mLayout->mLoop = 0;
    mLayout->fn_800D246C(1);
    mChoices = this != 0 ? new ((MemoryTag)1, "", "", 0) GameChoiceWindow(this) : 0;
}

GameMessageWindow::~GameMessageWindow() {}

int GameMessageWindow::vf0C() {
    if (fn_800D3FA4()) {
        mLayout->fn_800D246C(0);
        mLayout->FreezeTask();
        return 1;
    }
    return 0;
}

void GameMessageWindow::vf10() {
    switch (mState) {
    case 1: {
        float frame = mLayout->mFrame;
        if (frame >= fn_801F6C2C(mLayout)) {
            mState = 2;
        }
        break;
    }
    case 3:
        if (mLayout->IsFlag_unk(4) || mLayout->mFrame <= 1.0f) {
            mState = 0;
            mLayout->FreezeTask();
        }
        break;
    case 2:
        if (mTimer > 0 && --mTimer == 0) {
            mState = 3;
            mLayout->mSpeed = -1.5f;
        }
        break;
    }
    ButtonUv_unk uv = lbl_8087F0A8->mInput.fn_80124C6C(0);
    fn_801F7590(mLayout, "btnUv", uv);
    fn_801F7590(mLayout, "btnUvMask", uv);
}

void GameMessageWindow::fn_803B5774(GameMessage* msg, int time) {
    mMessage = msg;
    mState = 1;
    mLayout->mFrame = 0.0f;
    mLayout->mSpeed = 1.5f;
    mLayout->MeltTask();
    mTimer = time;
    fn_803B5974();
}

void GameMessageWindow::fn_803B57B0(int enabled) {
    if (enabled == 0) {
        mState = 3;
        mLayout->mSpeed = -1.5f;
    } else {
        mState = 0;
        mLayout->FreezeTask();
    }
}

void GameMessageWindow::fn_803B57EC() {
    mChoices->Close_unk();
    if (IsShown_unk()) {
        mState = 2;
    }
}

void GameMessageWindow::fn_803B5830(GameMessage* msg, int time) {
    mMessage = msg;
    mTimer = time;
    fn_803B5974();
}

void GameMessageWindow::fn_803B583C(int id) {
    if (lbl_8087F430 != 0 && lbl_8087F430->m10D8 != 0) {
        GameMessage* msg = (GameMessage*)((GameScene*)lbl_8087F430->m10D8)->fn_803CC6B4(id);
        if (msg != 0) {
            mChoices->Open_unk(msg);
            if (IsShown_unk()) {
                mState = 4;
            }
        }
    }
}

void GameMessageWindow::fn_803B58D8(GameMessage* msg) {
    mChoices->Open_unk(msg);
    if (IsShown_unk()) {
        mState = 4;
    }
}

int GameMessageWindow::fn_803B5950() {
    if (mState == 2) {
        return fn_800A555C(lbl_8087EF70, 0, 4);
    }
    return 0;
}

// NONMATCHING: stub (sets the texts of the window); noinline until it is written (the original, 1976
// bytes, is always called out of line)
__declspec(noinline) void GameMessageWindow::fn_803B5974() {}
GameTextMessageWindow* GameTextMessageWindow::fn_803B612C(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameTextMessageWindow(parent);
    }
    return 0;
}

GameTextMessageWindow::GameTextMessageWindow(atn::Task* parent)
    : atn::Task(parent), mLayout(0), m4C(0), mState(0), m54(0), m58(0), m5C(0), m60(0.0f), m64(15), m68(0.0f), m70(0),
      m71(0), m74(0), m78(0), m7C(0), m80(0), m84(0), m88(0), m8C(0), mAC(0), mB0(0), mCount(0), mHead(0) {
    mLayout = fn_801F64D0(this, sTextLayout_unk);
    mLayout->mLoop = 0;
    mLayout->fn_800D246C(1);
}

GameTextMessageWindow::~GameTextMessageWindow() {}

int GameTextMessageWindow::vf0C() {
    if (fn_800D3FA4()) {
        mLayout->fn_800D246C(0);
        mLayout->FreezeTask();
        return 1;
    }
    return 0;
}

// NONMATCHING: stub
void GameTextMessageWindow::vf10() {}

// NONMATCHING: stub
void GameTextMessageWindow::fn_803B6970(GameMessage* msg, int arg, float time) {
    (void)msg;
    (void)arg;
    (void)time;
}

extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound

void GameTextMessageWindow::fn_803B6C88(int enabled) {
    if (enabled == 0) {
        mState = 3;
        mLayout->mSpeed = -1.0f;
    } else {
        mState = 0;
        mLayout->mFrame = 0.0f;
        mLayout->mSpeed = 0.0f;
        mLayout->MeltTask();
        mCount = 0;
        mLayout->FreezeTask();
        fn_800CB5C8(&mSound, 15, 0);
        fn_803B78E8();
    }
    if (m70 != 0 && lbl_8087F490 != 0 && lbl_8087F490->m263C != 0) {
        GameMessageWindow* window = (GameMessageWindow*)lbl_8087F490->m263C;
        if (window->IsShown_unk()) {
            window->fn_803B57B0(enabled);
        }
    }
}

// NONMATCHING: stub
void GameTextMessageWindow::fn_803B6E04() {}

// NONMATCHING: stub
void GameTextMessageWindow::fn_803B710C() {}

void GameTextMessageWindow::fn_803B78B4(GameMessage* msg) {
    if (GetCount_unk() < 0x40) {
        Push_unk(msg);
    }
}

void GameTextMessageWindow::fn_803B78E8() {
    if (m71 != 0) {
        m71 = 0;
        if (lbl_8087F430 != 0) {
            if (m74 == 0) {
                ((GameTask*)lbl_8087F430)->fn_80370AE4(m78, 1);
            } else if (m74 == 1) {
                ((GameTask*)lbl_8087F430)->fn_80370320(m78, 1, 0);
            }
        }
    }
}
