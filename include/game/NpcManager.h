#ifndef GAME_NPCMANAGER_H
#define GAME_NPCMANAGER_H

#include "game/CharaManager.h"
#include "game/ResHandle.h"

class NpcManager : public CharaManager<NpcTask> {
public:
    NpcManager(atn::Task* parent) : CharaManager<NpcTask>(parent), m58(0), m5C(0), m60(0), m64(0), m68(0), m6C(0), m70(0) {}
    virtual ~NpcManager();
    virtual int vf0C();

    // inferred: creates the singleton under `parent` if it does not exist yet
    static NpcManager* fn_805477C8(atn::Task* parent);

    ResHandle mRes; // 0x54
    void* m58;
    int m5C; int m60; int m64; int m68; int m6C; int m70;
    int m74;
    static NpcManager* sInstance; // inferred name (0x8087F890)
};

#endif
