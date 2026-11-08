#ifndef GAME_GAMEMESSAGELIST_H
#define GAME_GAMEMESSAGELIST_H

#include "atn/Array_unk.h"
#include "atn/LH_Binary.h"
#include "atn/D2A_TaskInstance.h"
#include "gmk/SeTable.h"

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

// GameMessageList (name from its destructor __dt__15GameMessageListFv, 0x803663A4): message table of a scene
// (game/GameScene m11C and the field table lbl_8087F430 + 0x54F4);
// (member names invented)
struct GameMessageList {
    void fn_803B35FC(const wchar_t* data, unsigned int size); // inferred: parses
    void fn_803B3AC4();                    // inferred: releases the messages
    GameMessage* fn_803B3B38(int id) const;      // inferred: message `id`
    void fn_803B3C10(const char* path);    // inferred: loads the messages
    int fn_803B3CA8();                     // inferred: nonzero while loading

    atn::SimpleArray<wchar_t> mTexts;      // 0x00: inferred: buffer of all the texts
    atn::SimpleArray<GameMessage> mMessages; // 0x08
    atn::LH_Binary mFile;                  // 0x10
};

#endif
