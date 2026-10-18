#ifndef GAME_NPCTASK_H
#define GAME_NPCTASK_H

#include "game/CharaTask.h"
#include "game/NpcManager.h"

// inferred from __vt__7NpcTask (0x80794780)
class NpcTask : public CharaTask {
public:
    NpcTask(atn::Task* parent, void* data) : CharaTask(parent, 1, data), mNext(0), m1428(0), m142C(-1) {
        if (NpcManager::sInstance != 0) {
            mId = NpcManager::sInstance->NextId_unk();
            NpcManager::sInstance->Add_unk(this);
        }
    }
    virtual ~NpcTask();
    virtual int vf0C();
    virtual void vf10();
    virtual void vf14();

    static NpcTask* fn_80547C28(atn::Task* parent, void* data); // inferred: create
    void fn_80547DF8(NpcTask* npc); // inferred: append to the list starting at this
    void fn_80547E14(NpcTask* npc); // inferred: unlink from the list starting at this
    NpcTask* fn_80547E44();         // inferred: previous NPC in the manager list (wraps to the last)

    NpcTask* mNext; // 0x1424
    int m1428;
    int m142C;
    unsigned char m1430[0x1438 - 0x1430];
};

#endif
