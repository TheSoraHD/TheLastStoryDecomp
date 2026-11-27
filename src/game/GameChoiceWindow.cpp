#include "atn/SoundManager.h"
#include "game/GameChoiceWindow.h"
#include "game/GameTask.h"
#include "game/GameScene.h"
#include "game/Input_unk.h"
#include "atn/Csv.h"
#include "atn/Memory.h"
#include "atn/SoundRef.h"
#include "game/GameUI.h"
#include "atn/D2A_TaskInstanceC.h"
#include "atn/D2A_Data.h"
#include "game/GameVoice.h"
#include "game/GameEventScript.h"
#include "game/CharaTask.h"
#include "game/GameConfig_unk.h"
#include "atn/WordWrap_unk.h"
#include "std/string_replace_unk.h"
extern "C" unsigned long wcslen(const wchar_t* s);
extern "C" wchar_t* wcscpy(wchar_t* dst, const wchar_t* src);
extern "C" int fn_800DC1DC(const wchar_t* s);                 // inferred: wide string to int
extern "C" unsigned int fn_80209120(const char* name);        // inferred: id of a character name
extern "C" int fn_8006F420(void* font, char* dst, const wchar_t* src, int size); // inferred: to the game encoding
namespace atn {
class Font_unk;
}
extern "C" atn::Font_unk* lbl_8087EEC8;                                // inferred: font
extern "C" const wchar_t lbl_8077A0A8[];                      // inferred: separators of the message CSV
extern "C" float fn_801F0544(void* key, int frame); // inferred: value of a key of a layout track
extern "C" CharaInfo_unk* fn_8020924C(int id);      // inferred: character table entry
// inferred: word wrap of atn/WordWrap_unk.cpp (called with its automatic name, as in game/LibraryTask)
extern "C" std::basic_string<wchar_t> fn_800DFB40(const wchar_t* text, float maxWidth, float size, float spacing,
                                                  int a, int b, MeasureChar_unk measure, int mode);

// inferred: layouts of the message windows (names invented)
static const char* const sMessageLayout_unk = "/data/d2anime/ui_talk.d2a";
static const char* const sTextLayout_unk = "/data/d2anime/ui_friend_mes.d2a";

// inferred: parses the message CSV: a first pass counts the messages and the text length, the second
// one fills them (the texts lose the "\r" that are not followed by a line feed)
// NONMATCHING: 98.8% - the BOM test of the CsvU constructor keeps size / 2 - 1 in another register (the
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
            void* font = lbl_8087EEC8;
            fn_8006F420(font, name, csv.fn_8005B9CC(), 0x10);
            msg->mTalker = fn_80209120(name);
            void* font2 = lbl_8087EEC8;
            fn_8006F420(font2, name, csv.fn_8005B9CC(), 0x10);
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

extern "C" GameTask* lbl_8087F430;

GameMessage* GameMessageList::fn_803B3B38(int id) const {
    if (lbl_8087F430 != 0) {
        const GameMessageAlias_unk* alias = sAliases_unk;
        for (unsigned int i = 0; i < 6; i++, alias++) {
            if (alias->mId == id && lbl_8087F430->fn_80370174(alias->mCounter) == 1) {
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

// inferred: item tables of fn_8021E444 (names invented; game/TreasureBox reads the same entries)
struct GameChoiceItem_unk {
    int mId;
    int mCount;
};
struct GameChoiceItemTable_unk {
    int m00;
    GameChoiceItem_unk* mEntries; // 0x04
    int mNumEntries;              // 0x08
};
extern "C" void itow(int value, wchar_t* buf, int radix);
extern "C" const wchar_t* const* fn_80211480(int id);          // inferred: item data (+0x08: name)
extern "C" GameChoiceItemTable_unk* fn_8021E444(int table);    // inferred: item table
extern "C" int fn_80444CF8(wchar_t* buf, int id, int count, int arg); // inferred: text of an item

// inferred: item data of fn_80211480: the name is the third word (name invented)
struct GameChoiceItemData_unk {
    int m00;
    int m04;
    const wchar_t* mName; // 0x08
};

// inferred: converts the tags of a message text (LogMessageTask calls it): "$X[n]" becomes the value
// `n` (X = V), the name of the item `n` (I) or the text of the first item of the table `n` (T); "[Ln]"
// and "[Gn]" read `n` from the counters of the field
// NONMATCHING: 99.5% - the original sets up text + 2 (the short data) before saving `text` (mr r20, r3)
extern "C" int fn_803B3D2C(std::basic_string<wchar_t>* text) {
    unsigned long pos = 0;
    for (;;) {
        unsigned long start = text->find(L'$', pos);
        if (start == std::basic_string<wchar_t>::npos) {
            break;
        }
        unsigned long open = text->find(L'[', start + 1);
        if (open == std::basic_string<wchar_t>::npos) {
            continue;
        }
        unsigned long close = text->find(L']', open + 1);
        if (close == std::basic_string<wchar_t>::npos) {
            continue;
        }
        int value;
        wchar_t kind = text->data()[open + 1];
        if (kind == L'L') {
            value = fn_800DC1DC(std::basic_string<wchar_t>(*text, open + 2, close).c_str());
            if (lbl_8087F430 != 0) {
                value = lbl_8087F430->fn_80370A78(value);
            }
        } else if (kind == L'G') {
            value = fn_800DC1DC(std::basic_string<wchar_t>(*text, open + 2, close).c_str());
            if (lbl_8087F430 != 0) {
                value = lbl_8087F430->fn_80370174(value);
            }
        } else {
            value = fn_800DC1DC(std::basic_string<wchar_t>(*text, open + 1, close).c_str());
        }
        // inferred: L"\0" (two wide zeros): the original wide pool has 4 bytes of zeros before L"\xFF1F..."
        std::basic_string<wchar_t> repl(L"\0");
        switch (text->data()[start + 1]) {
        case L'V': {
            wchar_t buf[0x20];
            itow(value, buf, 10);
            repl += buf;
            break;
        }
        case L'I': {
            GameChoiceItemData_unk* item = (GameChoiceItemData_unk*)fn_80211480(value);
            if (item != 0) {
                repl += item->mName;
            }
            break;
        }
        case L'T': {
            GameChoiceItemTable_unk* table = fn_8021E444(value);
            if (table != 0 && table->mNumEntries != 0) {
                GameChoiceItem_unk* entry = table->mEntries;
                wchar_t buf[0x100];
                memset(buf, 0, sizeof(buf));
                int count = 1;
                if (entry->mId == 0x2714 || entry->mId == 0x2719) {
                    count = entry->mCount;
                }
                if (fn_80444CF8(buf, entry->mId, count, 1) != 0) {
                    repl += buf;
                }
            }
            break;
        }
        }
        text->replace(start, close + 1 - start, repl);
        pos = start + repl.size() + 1;
    }
    return 1;
}

extern "C" atn::SoundManager* lbl_8087EFE8;
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

// inferred: sets a texture rectangle (D2A_TaskInstanceBase<32, 8>::SetValues2_unk; the rectangle is copied)
extern "C" int fn_800A555C(void* pad, int port, int button); // inferred: button triggered
struct Pad_unk;
extern "C" Pad_unk* lbl_8087EF70; // inferred: pad
extern "C" GameConfig_unk* lbl_8087F0A8; // inferred: game settings

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
    mMain = (GameLayout_unk*)D2A_TaskInstance32_unk::Create(this, "/data/d2anime/ui_select_4.d2a");
    mMain->mLoop = 0;
    mMain->fn_800D246C(1);
    mOptions[0] = (GameLayout_unk*)D2A_TaskInstance32_unk::Create(this, "/data/d2anime/ui_select_4_l.d2a");
    mOptions[0]->fn_800D246C(1);
    mOptions[1] = (GameLayout_unk*)D2A_TaskInstance32_unk::Create(this, "/data/d2anime/ui_select_4_r.d2a");
    mOptions[1]->fn_800D246C(1);
    mOptions[2] = (GameLayout_unk*)D2A_TaskInstance32_unk::Create(this, "/data/d2anime/ui_select_4_u.d2a");
    mOptions[2]->fn_800D246C(1);
    mOptions[3] = (GameLayout_unk*)D2A_TaskInstance32_unk::Create(this, "/data/d2anime/ui_select_4_d.d2a");
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
    mOptions[0]->SetValue0_unk("canSelect", 1.0f);
    mOptions[0]->SetValue0_unk("skipSelect", 0.0f);
    rect = mMain->GetRect_unk("select_right");
    mOptions[1]->SetValues1Rect_unk("basePoint", rect);
    mOptions[1]->SetValue0_unk("canSelect", 1.0f);
    mOptions[1]->SetValue0_unk("skipSelect", 0.0f);
    rect = mMain->GetRect_unk("select_up");
    mOptions[2]->SetValues1Rect_unk("basePoint", rect);
    mOptions[2]->SetValue0_unk("canSelect", 1.0f);
    mOptions[2]->SetValue0_unk("skipSelect", 0.0f);
    rect = mMain->GetRect_unk("select_down");
    mOptions[3]->SetValues1Rect_unk("basePoint", rect);
    mOptions[3]->SetValue0_unk("canSelect", 1.0f);
    mOptions[3]->SetValue0_unk("skipSelect", 0.0f);
}

int GameChoiceWindow::fn_803B4C24() {
    return mState == 3;
}

// inferred: MSL moves strings with memcpy when a vector reallocates (the old elements are cleared and destroyed
// as empty strings by the buffer, 0x803B5000 in fn_803B4C38)
template <>
inline void std::__vec_split_unk<std::basic_string<wchar_t> >::__push_front_unk(std::basic_string<wchar_t>* first,
                                                                              std::basic_string<wchar_t>* last) {
    // inferred: a byte difference, so that (data + size) - data is not folded (as std/vector_wstring.h)
    int n = ((char*)last - (char*)first) / (int)sizeof(std::basic_string<wchar_t>);
    mStart -= n;
    memcpy(begin(), first, n * sizeof(std::basic_string<wchar_t>));
    memset(first, 0, n * sizeof(std::basic_string<wchar_t>));
    this->mSize += n;
}

// inferred: splits the text of the message into the (up to four) choices, one per line, and sets their
// texts; the empty choices are disabled (all of them empty: the first one stays enabled)
// NONMATCHING: 99.93% - r18/r19 swap (the reallocation buffer and the moved count) and stack slots of the two
// char temporaries; tried an unsigned count, the size in a local
void GameChoiceWindow::fn_803B4C38() {
    mChoice = -1;
    mCount = 1;
    std::basic_string<wchar_t> text(mMessage->mText);
    fn_803B3D2C(&text);
    std::vector<std::basic_string<wchar_t> > lines;
    fn_800E0000(&lines, text.c_str(), 100000.0f, 13.0f, 0.0f, 1, 1, 0);
    mCount = lines.size();
    while (lines.size() < 4) {
        lines.push_back(std::basic_string<wchar_t>(L"\0"));
    }
    bool none = true;
    for (int i = 0; i < 4; i++) {
        mEnabled[i] = lines[i][0] != 0;
        if (mEnabled[i]) {
            none = false;
        }
    }
    if (none) {
        mEnabled[0] = 1;
    }
    mOptions[0]->SetText_unk("text", lines[0].c_str());
    mOptions[1]->SetText_unk("text", lines[1].c_str());
    mOptions[2]->SetText_unk("text", lines[2].c_str());
    mOptions[3]->SetText_unk("text", lines[3].c_str());
    mOptions[0]->SetText_unk("textSub", lines[0].c_str());
    mOptions[1]->SetText_unk("textSub", lines[1].c_str());
    mOptions[2]->SetText_unk("textSub", lines[2].c_str());
    mOptions[3]->SetText_unk("textSub", lines[3].c_str());
    mOptions[0]->SetValue0_unk("iconOn", 0.0f);
    mOptions[1]->SetValue0_unk("iconOn", 0.0f);
    mOptions[2]->SetValue0_unk("iconOn", 0.0f);
    mOptions[3]->SetValue0_unk("iconOn", 0.0f);
    mOptions[2]->SetValue0_unk("tensionOn", 0.0f);
}

GameMessageWindow* GameMessageWindow::fn_803B5470(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameMessageWindow(parent);
    }
    return 0;
}


GameMessageWindow::GameMessageWindow(atn::Task* parent) : atn::Task(parent), mMessage(0), mState(0), mTimer(0) {
    mLayout = (GameLayout_unk*)D2A_TaskInstance32_unk::Create(this, sMessageLayout_unk);
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
        if (frame >= mLayout->GetEndFrame()) {
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
    mLayout->SetValues2_unk("btnUv", uv);
    mLayout->SetValues2_unk("btnUvMask", uv);
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

// inferred: sets the talker name and the three lines of the message mMessage, wrapped to the width and
// font size of the layout (as GameTextMessageWindow::fn_803B710C, and shows the message)
void GameMessageWindow::fn_803B5974() {
    float width = 640.0f;
    float size = 20.0f;
    void* track = mLayout->GetTrack_unk("textWidth");
    if (track != 0) {
        width = fn_801F0544(((atn::D2A_Track_unk*)track)->mTables, 0);
    }
    track = mLayout->GetTrack_unk("fontSize_text");
    if (track != 0) {
        size = fn_801F0544(((atn::D2A_Track_unk*)track)->mTables, 0);
    }
    std::basic_string<wchar_t> text(mMessage->mText);
    fn_803B3D2C(&text);
    text = fn_800DFB40(text.c_str(), width, size, 0.0f, 1, 1, 0, 0);
    std::basic_string<wchar_t> line1;
    std::basic_string<wchar_t> line2;
    std::basic_string<wchar_t> line3;
    unsigned long end1 = text.find(L'\n');
    line1 = std::basic_string<wchar_t>(text, 0, end1);
    if (end1 != std::basic_string<wchar_t>::npos) {
        unsigned long end2 = text.find(L'\n', end1 + 1);
        line2 = std::basic_string<wchar_t>(text, end1 + 1, end2 - (end1 + 1));
        if (end2 != std::basic_string<wchar_t>::npos) {
            unsigned long end3 = text.find(L'\n', end2 + 1);
            line3 = std::basic_string<wchar_t>(text, end2 + 1, end3 - (end2 + 1));
        }
    }
    CharaInfo_unk* talker = fn_8020924C(mMessage->mTalker);
    mLayout->SetValue0_unk("showMsg", 1.0f);
    if (talker != 0 && mMessage->mTalker != 0) {
        mLayout->SetValue0_unk("talkerOn", 1.0f);
        mLayout->SetText_unk("nameText", talker->m04);
        mLayout->SetText_unk("messageText1", line1.c_str());
        mLayout->SetText_unk("messageText2", line2.c_str());
        mLayout->SetText_unk("messageText3", line3.c_str());
    } else {
        mLayout->SetValue0_unk("talkerOn", 0.0f);
        mLayout->SetText_unk("nameText", L"\xFF1F\xFF1F\xFF1F");
        mLayout->SetText_unk("messageText11", line1.c_str());
        mLayout->SetText_unk("messageText12", line2.c_str());
        mLayout->SetText_unk("messageText13", line3.c_str());
    }
}
GameTextMessageWindow* GameTextMessageWindow::fn_803B612C(atn::Task* parent) {
    if (parent != 0) {
        return new ((MemoryTag)1, "", "", 0) GameTextMessageWindow(parent);
    }
    return 0;
}

GameTextMessageWindow::GameTextMessageWindow(atn::Task* parent)
    : atn::Task(parent), mLayout(0), mMessage(0), mState(0), m54(0), m58(0), m5C(0), m60(0.0f), m64(15), m68(0.0f), m70(0),
      m71(0), m74(0), m78(0), m7C(0), m80(0), m84(0), m88(0), m8C(0), mAC(0), mB0(0), mCount(0), mHead(0) {
    mLayout = (GameLayout_unk*)D2A_TaskInstance32_unk::Create(this, sTextLayout_unk);
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

// inferred: the message has a talker name, the voice manager exists and has the voice (names invented)
// inferred: the three tests are kept as booleans (li 0 / li 1 of each, 0x803B6A24)
inline bool HasVoice_unk(GameMessage* msg) {
    bool name = msg != 0 && strlen(msg->mName.c_str()) != 0;
    bool can = name && lbl_8087F498 != 0;
    bool voice = can && lbl_8087F498->fn_803EBC04(msg->mName.c_str()) != 0;
    return voice;
}

// inferred: command being run by the event script, or 0 (name invented)
inline ScriptCmd_unk* RunningCmd_unk(GameEventScript* script) {
    return script->m8C != 0 ? script->m90 : 0;
}

// inferred: the choice window may open with the message (the game holds the input and the script does
// not run the commands 0x5C or 0x61; name invented)
inline bool CanChoose_unk() {
    bool r = false;
    if (lbl_8087F0A8->mInput.fn_80124B60() != 0) {
        r = true;
        if (lbl_8087F430 != 0) {
            GameScene* scene = (GameScene*)lbl_8087F430->m10D8;
            if (scene != 0) {
                GameEventScript* script = scene->m134;
                if (script != 0) {
                    ScriptCmd_unk* cmd = RunningCmd_unk(script);
                    if (cmd != 0 && (cmd->mCmd == 0x5C || cmd->mCmd == 0x61)) {
                        r = false;
                    }
                }
            }
        }
    }
    return r;
}

wchar_t lbl_8087F468[1]; // inferred: empty text of the message of the choice window

extern "C" int fn_8021F3E8(const char* name); // inferred: length of the voice `name` (db/VoiceLengthTable_unk)
extern "C" void fn_800CB5C8(atn::SoundRef* sound, int frames, int arg); // inferred: stops the sound

// inferred: time of the choice window and voice of the message (inlined in fn_803B6970 and fn_803B6E04;
// name invented)
inline void GameTextMessageWindow::Start_unk(GameMessage* msg, int arg, float time, int keepRequests) {
    mMessage = msg;
    m54 = msg->m30;
    m58 = arg;
    m60 = time;
    if (msg->m34 > 0) {
        m58 = msg->m34;
    }
    if (m58 <= 0) {
        const char* name = mMessage->mName.c_str();
        if (strlen(name) != 0) {
            int length = fn_8021F3E8(name);
            if (length > 0) {
                m58 = length;
            } else {
                m58 = 150;
            }
        }
    }
    if (HasVoice_unk(mMessage)) {
        if (mSound.mPtr != 0) {
            fn_800CB5C8(&mSound, 10, 0);
            m54 += 10;
        }
        if (m54 <= 0) {
            m54 = 1;
        }
        if (lbl_8087F498 != 0) {
            lbl_8087F498->fn_803EB4A8(0, keepRequests);
            lbl_8087F498->fn_803EB038(mMessage->mName.c_str(), 0);
        }
    }
}

// inferred: the message has no frames left: plays the layout and opens the choice window (name invented)
inline void GameTextMessageWindow::Choose_unk() {
    mLayout->mFrame = 0.0f;
    mLayout->mSpeed = 1.0f;
    mLayout->MeltTask();
    m70 = 0;
    if (lbl_8087F490 != 0 && CanChoose_unk()) {
        GameMessageWindow* window = lbl_8087F490->m263C;
        if (window != 0 && !window->IsShown_unk()) {
            m70 = 1;
            m84 = (int)lbl_8087F468;
            window->fn_803B5774((GameMessage*)&m7C, m58);
        }
    }
}

inline GameMessage* GameTextMessageWindow::Pop_unk() {
    GameMessage* msg = mQueue[mHead];
    mHead = (mHead + 1) & 0x3F;
    mCount--;
    return msg;
}

// inferred: message window of the HUD with choices, or 0 (name invented)
inline GameMessageWindow* GetMessageWindow_unk() {
    return lbl_8087F490 != 0 ? lbl_8087F490->m263C : 0;
}

// NONMATCHING: 94% - the original keeps the three booleans of HasVoice_unk in saved registers (li 0 /
// li 1 of each one, the three li 0 first); with `bool x = a && b` ours sets each one where it is tested and
// keeps the inner ones in volatile registers (bool r = false; if (...) r = true; in nested inlines, int flags,
// static or out-of-line helpers: MWCC threads the jumps over them)
void GameTextMessageWindow::vf10() {
    if (lbl_8087F430 != 0 && lbl_8087F430->m54E4 == 8) {
        mLayout->FreezeTask();
        return;
    }
    if (m70 != 0 && (mState == 1 || mState == 2) && !CanChoose_unk()) {
        GameMessageWindow* window = GetMessageWindow_unk();
        if (window != 0 && window->IsShown_unk()) {
            window->fn_803B57B0(0);
        }
    }
    switch (mState) {
    case 1: {
        if (m54 > 0 && --m54 <= 0) {
            if (HasVoice_unk(mMessage) && lbl_8087F498->fn_803EB234() != 0 && m5C < 90) {
                m54 = 1;
                m5C++;
            } else {
                if (m5C >= 90 && lbl_8087F498 != 0) {
                    lbl_8087F498->fn_803EB394(mMessage->mName.c_str());
                }
                Choose_unk();
            }
        }
        float frame = mLayout->mFrame;
        if (frame >= mLayout->GetEndFrame()) {
            mState = 2;
            if (m5C < 90 && HasVoice_unk(mMessage)) {
                fn_800CB5C8(&mSound, 0, 0);
                float volume = 1.0f;
                if (lbl_8087F0A8->mVoiceLimitLevel >= 2) {
                    volume = 0.0f;
                }
                mSound = lbl_8087F498->fn_803E9608(mMessage->mName.c_str(), 0, 1, volume);
            }
        }
        break;
    }
    case 3:
        if (mLayout->mFrame <= 1.0f) {
            if (mCount == 0) {
                mState = 0;
                mLayout->FreezeTask();
                fn_800CB5C8(&mSound, 15, 0);
                fn_803B78E8();
            } else {
                fn_803B6970(Pop_unk(), 0, m60);
            }
        }
        break;
    case 2:
        if (m58 > 0 && --m58 == 0) {
            fn_803B6C88(0);
        }
        if (CanChoose_unk() && fn_800A555C(lbl_8087EF70, 0, 4) != 0) {
            fn_803B4338(1, 0.8f);
            fn_803B6C88(0);
        }
        break;
    }
}

// NONMATCHING: 92.3% - same booleans of HasVoice_unk as vf10; noinline: vf10 calls it with bl and MWCC
// would inline it
__declspec(noinline) void GameTextMessageWindow::fn_803B6970(GameMessage* msg, int arg, float time) {
    if (IsFree_unk(time)) {
        mState = 1;
        Start_unk(msg, arg, time, 1);
        m5C = 0;
        fn_803B710C();
        mLayout->mFrame = 0.0f;
        mLayout->mSpeed = 0.0f;
        mLayout->MeltTask();
        if (m54 <= 0) {
            Choose_unk();
        }
    }
}

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

// NONMATCHING: 91% - same booleans of HasVoice_unk as vf10; the original stores m58 before m54
void GameTextMessageWindow::fn_803B6E04(GameMessage* msg, int arg, float time) {
    if (mState == 0 || !(m60 < time)) {
        Start_unk(msg, arg, time, 0);
        m5C = 0;
        mLayout->mFrame = 0.0f;
        mLayout->mSpeed = 0.0f;
        mLayout->MeltTask();
        mState = 1;
        fn_803B710C();
        if (m54 <= 0) {
            Choose_unk();
        }
    }
}

// inferred: sets the talker name and the three lines of the message mMessage, wrapped to the width and
// font size of the layout
void GameTextMessageWindow::fn_803B710C() {
    float width = 640.0f;
    float size = 20.0f;
    void* track = mLayout->GetTrack_unk("textWidth");
    if (track != 0) {
        width = fn_801F0544(((atn::D2A_Track_unk*)track)->mTables, 0);
    }
    track = mLayout->GetTrack_unk("fontSize_text");
    if (track != 0) {
        size = fn_801F0544(((atn::D2A_Track_unk*)track)->mTables, 0);
    }
    std::basic_string<wchar_t> text(mMessage->mText);
    fn_803B3D2C(&text);
    text = fn_800DFB40(text.c_str(), width, size, 0.0f, 1, 1, 0, 0);
    std::basic_string<wchar_t> line1;
    std::basic_string<wchar_t> line2;
    std::basic_string<wchar_t> line3;
    unsigned long end1 = text.find(L'\n');
    line1 = std::basic_string<wchar_t>(text, 0, end1);
    if (end1 != std::basic_string<wchar_t>::npos) {
        unsigned long end2 = text.find(L'\n', end1 + 1);
        line2 = std::basic_string<wchar_t>(text, end1 + 1, end2 - (end1 + 1));
        if (end2 != std::basic_string<wchar_t>::npos) {
            unsigned long end3 = text.find(L'\n', end2 + 1);
            line3 = std::basic_string<wchar_t>(text, end2 + 1, end3 - (end2 + 1));
        }
    }
    CharaInfo_unk* talker = fn_8020924C(mMessage->mTalker);
    if (talker != 0 && mMessage->mTalker != 0) {
        mLayout->SetValue0_unk("talkerOn", 1.0f);
        mLayout->SetText_unk("nameText", talker->m04);
        mLayout->SetText_unk("messageText1", line1.c_str());
        mLayout->SetText_unk("messageText2", line2.c_str());
        mLayout->SetText_unk("messageText3", line3.c_str());
    } else {
        mLayout->SetValue0_unk("talkerOn", 0.0f);
        mLayout->SetText_unk("nameText", L"\xFF1F\xFF1F\xFF1F");
        mLayout->SetText_unk("messageText11", line1.c_str());
        mLayout->SetText_unk("messageText12", line2.c_str());
        mLayout->SetText_unk("messageText13", line3.c_str());
    }
}

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
                lbl_8087F430->fn_80370AE4(m78, 1);
            } else if (m74 == 1) {
                lbl_8087F430->fn_80370320(m78, 1, 0);
            }
        }
    }
}
