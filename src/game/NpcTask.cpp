#include "game/NpcTask.h"
#include "atn/Memory.h"

NpcTask* NpcTask::fn_80547C28(atn::Task* parent, void* data) {
    if (parent != 0) {
        return new ((MemoryTag)3, "", "", 0) NpcTask(parent, data);
    }
    return 0;
}

NpcTask::~NpcTask() {
    if (NpcManager::sInstance != 0) {
        NpcManager::sInstance->Remove_unk(this);
    }
}

int NpcTask::vf0C() {
    return fn_8013655C() == 0;
}

void NpcTask::vf10() {
    fn_80139560();
    fn_80145334();
}

void NpcTask::vf14() {
    fn_80149A30();
}

void NpcTask::fn_80547DF8(NpcTask* npc) {
    NpcTask* cur = this;
    while (cur->mNext != 0) {
        cur = cur->mNext;
    }
    cur->mNext = npc;
}

void NpcTask::fn_80547E14(NpcTask* npc) {
    NpcTask* prev = 0;
    NpcTask* cur = this;
    while (cur != npc) {
        prev = cur;
        cur = cur->mNext;
    }
    if (cur == npc && prev != 0) {
        prev->mNext = cur->mNext;
    }
}

NpcTask* NpcTask::fn_80547E44() {
    NpcManager* manager = NpcManager::sInstance;
    if (manager != 0) {
        NpcTask* npc = manager->mHead;
        if (npc != 0) {
            if (this == npc) {
                while (npc->mNext != 0) {
                    npc = npc->mNext;
                }
            } else {
                while (npc->mNext != this) {
                    npc = npc->mNext;
                }
            }
            return npc;
        }
    }
    return 0;
}
