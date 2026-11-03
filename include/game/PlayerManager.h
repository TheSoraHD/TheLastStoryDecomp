#ifndef GAME_PLAYERMANAGER_H
#define GAME_PLAYERMANAGER_H

#include "game/CharaManager.h"
#include "game/PlayerTask.h"
#include "game/ResHandle.h"

// inferred from __vt__13PlayerManager (0x80794920): manager of the party (lbl_8087F8A0)
class PlayerManager : public CharaManager<PlayerTask> {
public:
    // inferred: creates the singleton under `parent` (0x80549D8C)
    static PlayerManager* fn_80549D8C(atn::Task* parent);
    PlayerManager(atn::Task* parent);
    virtual ~PlayerManager();  // 0x80549E58
    virtual int vf0C();        // 0x80549EE8
    virtual void vf1C();       // 0x8054A000
    void fn_80549F14(int mode); // inferred: creates the player (multiplayer modes 3 and 4)
    void fn_8054A09C(PlayerTask* player, int swap); // inferred: makes `player` the leader
    PlayerTask* fn_8054A33C(int member);           // inferred: player of a party member (character id via fn_80219544)
    void fn_8054A398();                              // inferred: fn_800D2338 on all but the leader

    // inferred: accessors called out of line by Event::Actor (weak copies 0x8000D9F0 and 0x8000DCF4); defined
    // in Event/Puppeteer_CharaTask.cpp (an inline body here renumbers the @N labels of the files that include
    // this header); names invented
    inline PlayerTask* GetLeader_unk();
    inline PlayerTask* GetHead_unk();

    PlayerTask* m54;  // inferred: leader when the party was created
    ResHandle mMenu;  // 0x58: debug menu group
    void* m5C;
};

#endif
