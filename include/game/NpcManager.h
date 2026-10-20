#ifndef GAME_NPCMANAGER_H
#define GAME_NPCMANAGER_H

#include "game/CharaManager.h"
#include "game/ResHandle.h"

class NpcManager : public CharaManager<NpcTask> {
public:
    NpcManager(atn::Task* parent) : CharaManager<NpcTask>(parent), m58(0) {
        mNpcs[0] = 0;
        mNpcs[1] = 0;
        mNpcs[2] = 0;
        mNpcs[3] = 0;
        mNpcs[4] = 0;
        mNpcs[5] = 0;
    }
    virtual ~NpcManager();
    virtual int vf0C();

    // inferred: creates the singleton under `parent` if it does not exist yet
    static NpcManager* fn_805477C8(atn::Task* parent);
    void fn_80547980(); // inferred: creates the fixed NPCs 1-6 (except 4) in mNpcs
    void fn_80547A20(); // inferred: deletes the fixed NPCs
    void fn_80547A84(); // inferred: deletes every other NPC
    int fn_80547B54();  // inferred: unloads the fixed NPCs; nonzero while some is still loading

    ResHandle mRes; // 0x54
    void* m58;
    NpcTask* mNpcs[6]; // 0x5C: inferred: fixed NPCs (GameTask), slot i holds NPC i + 1
    int m74;
    static NpcManager* sInstance; // inferred name (0x8087F890)
};

#endif
