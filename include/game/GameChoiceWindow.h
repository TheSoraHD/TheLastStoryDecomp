#ifndef GAME_GAMECHOICEWINDOW_H
#define GAME_GAMECHOICEWINDOW_H

#include "atn/Task.h"
#include "atn/D2A_TaskInstance.h"
#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "gmk/SeTable.h"
#include "atn/SoundRef.h"

// game/GameChoiceWindow.cpp (0x803B35FC-0x803B7944): the message table of a scene and the message,
// choice and text windows (class names from the RTTI; member names invented).

// inferred: layout anime task of the windows (made by fn_801F64D0)
typedef atn::D2A_TaskInstanceBase<32, 8> GameLayout_unk;

// inferred: message of the table (0x38 bytes; constructor 0x803B3AA0, destructor 0x80366364 in
// game/GameTask.cpp)
struct GameMessage {
    GameMessage() : mId(0), m04(0), mText(0), mTalker(0), m30(0), m34(0) {}
    ~GameMessage();

    int mId;            // 0x00
    int m04;            // 0x04: inferred: number of choices (game/GameChoiceWindow fn_803B4C38)
    wchar_t* mText;     // 0x08: inferred: text, inside the buffer of the table
    unsigned int mTalker; // 0x0C: inferred: id of the talker (fn_80209120 of the name)
    atn::FixedString_unk<0x20> mName; // 0x10
    int m30;
    int m34;
};

// inferred: message table of a scene (game/GameScene m11C and the field table lbl_8087F430 + 0x54F4);
// the functions are called with this view by the other files (names invented)
struct GameMessageTable_unk {
    atn::SimpleArray<wchar_t> mTexts;      // 0x00: inferred: buffer of all the texts
    atn::SimpleArray<GameMessage> mMessages; // 0x08
    atn::LH_Binary mFile;                  // 0x10
};
extern "C" void fn_803B35FC(GameMessageTable_unk* table, const wchar_t* data, unsigned int size); // inferred: parses
extern "C" void fn_803B3AC4(GameMessageTable_unk* table); // inferred: releases the messages
extern "C" GameMessage* fn_803B3B38(GameMessageTable_unk* table, int id); // inferred: message `id`
extern "C" void fn_803B3C10(GameMessageTable_unk* table, const char* path); // inferred: loads the messages
extern "C" int fn_803B3CA8(GameMessageTable_unk* table); // inferred: nonzero while loading

// inferred from __vt__16GameChoiceWindow (0x8078B4D0): the four choices of a message (0x88 bytes)
class GameChoiceWindow : public atn::Task {
public:
    static GameChoiceWindow* Create_unk(atn::Task* parent); // inferred: removed by the linker
    GameChoiceWindow(atn::Task* parent); // 0x803B434C
    virtual ~GameChoiceWindow();         // 0x803B443C
    virtual int vf0C();                  // 0x803B4494
    virtual void vf10();                 // 0x803B4564

    int fn_803B4C24(); // inferred: a choice was made
    void fn_803B4C38(); // inferred: reads the choices of the message mMessage
    // inferred: names invented
    void Open_unk(GameMessage* msg) {
        mMessage = msg;
        mState = 1;
        mTimer = 0;
        mSelected = 0;
        fn_803B4C38();
    }
    void Select_unk(int i) {
        mChoice = i;
        mState = 3;
        mSelected = mOptions[mChoice];
    }
    void Close_unk() {
        mState = 4;
        mTimer = 0;
    }

    GameMessage* mMessage;          // 0x48
    GameLayout_unk* mMain;          // 0x4C
    GameLayout_unk* mOptions[4];    // 0x50: inferred: left, right, up, down
    GameLayout_unk* mSelected;      // 0x60
    int mEnabled[4];                // 0x64
    int mTimer;                     // 0x74
    int mState;                     // 0x78: 1 opening, 2 waiting, 3 chosen, 4 closing
    int mChoice;                    // 0x7C
    int mCount;                     // 0x80
    int mStick;                     // 0x84: inferred: the stick was released
};

// inferred from __vt__17GameMessageWindow (0x8078B498): message window with its choices (0x60 bytes)
class GameMessageWindow : public atn::Task {
public:
    static GameMessageWindow* fn_803B5470(atn::Task* parent); // inferred: creates the window
    GameMessageWindow(atn::Task* parent); // inferred: inlined in fn_803B5470, removed by the linker
    virtual ~GameMessageWindow(); // 0x803B5564
    virtual int vf0C();           // 0x803B55BC
    virtual void vf10();          // 0x803B5618

    void fn_803B5774(int arg, int time); // inferred: opens the window
    void fn_803B57B0(int enabled);
    void fn_803B57EC();
    void fn_803B5830(int arg, int time);
    void fn_803B583C(int id); // inferred: shows the choices of the message `id` of the scene
    void fn_803B58D8(GameMessage* msg);
    int fn_803B5950();
    void fn_803B5974(); // inferred: sets the texts of the window from m4C
    bool IsShown_unk() const { return mState == 2 || mState == 4; } // inferred: name invented

    GameLayout_unk* mLayout;     // 0x48
    int m4C;
    int mState;                  // 0x50
    GameChoiceWindow* mChoices;  // 0x54
    int mTimer;                  // 0x58
    int m5C;
};

// inferred from __vt__21GameTextMessageWindow (0x8078B460): message window of the HUD (GameUI::m2640, 0x1C0
// bytes); the messages wait in a queue of 64 entries
class GameTextMessageWindow : public atn::Task {
public:
    static GameTextMessageWindow* fn_803B612C(atn::Task* parent); // inferred: creates the window
    GameTextMessageWindow(atn::Task* parent); // 0x803B6190
    virtual ~GameTextMessageWindow();         // 0x803B6260
    virtual int vf0C();                       // 0x803B62C8
    virtual void vf10();                      // 0x803B6324

    void fn_803B6970(GameMessage* msg, int arg, float time); // inferred: shows a message
    void fn_803B6C88(int enabled);
    void fn_803B6E04();
    void fn_803B710C();
    void fn_803B78B4(int value); // inferred: queues a value
    void fn_803B78E8();          // inferred: sets the event counter of the message
    unsigned int GetCount_unk() const { return mCount; } // inferred: name invented
    void Push_unk(int value) {
        mQueue[(mHead + mCount) & 0x3F] = value;
        mCount++;
    }

    GameLayout_unk* mLayout; // 0x48
    int m4C;
    int mState;              // 0x50
    int m54;
    int m58;
    int m5C;
    float m60;
    int m64;
    float m68;
    atn::SoundRef mSound;    // 0x6C
    unsigned char m70;
    unsigned char m71;       // inferred: the event counter m78 must be set
    int m74;                 // inferred: 0 sets the counter, 1 adds to it
    int m78;                 // inferred: event counter
    int m7C;
    int m80;
    int m84;
    int m88;
    unsigned char m8C;
    unsigned char m8D[0xAC - 0x8D];
    int mAC;
    int mB0;
    unsigned int mCount;     // 0xB4
    int mQueue[0x40];        // 0xB8
    unsigned int mHead;      // 0x1B8
    int m1BC;
};

#endif
