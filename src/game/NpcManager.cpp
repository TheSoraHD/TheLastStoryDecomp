#include "game/NpcManager.h"
#include "atn/Memory.h"
#include "game/NpcTask.h"

extern "C" void fn_800D2338(atn::Task* task); // inferred: removes the task

NpcManager* NpcManager::fn_805477C8(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (3, "", "", 0) NpcManager(parent);
    }
    return sInstance;
}

NpcManager::~NpcManager() {
    sInstance = 0;
}

int NpcManager::vf0C() {
    if (fn_800D3FA4()) {
        m58 = mRes.Load("NpcManager");
        return 1;
    }
    return 0;
}

void NpcManager::fn_80547980() {
    for (int i = 1; i < 7; i++) {
        NpcTask** slot = &mNpcs[i - 1];
        if (*slot == 0 && i != 4) {
            NpcTask* npc = NpcTask::fn_80547C28(this, (void*)(100001 + i * 100));
            npc->fn_800D246C(1);
            npc->m1428 = 1;
            npc->m58 = -i * 100;
            npc->mC58.mHeld_unk = 1;
            *slot = npc;
        }
    }
}

void NpcManager::fn_80547A20() {
    NpcTask** npc = mNpcs;
    for (unsigned int i = 0; i < 6; i++, npc++) {
        if (*npc != 0) {
            fn_800D2338(*npc);
        }
        *npc = 0;
    }
}

void NpcManager::fn_80547A84() {
    for (NpcTask* npc = mHead; npc != 0; npc = npc->mNext) {
        NpcTask** p = mNpcs;
        bool found = false;
        for (unsigned int i = 0; i < 6; i++, p++) {
            if (*p == npc) {
                found = true;
                break;
            }
        }
        if (!found) {
            fn_800D2338(npc);
        }
    }
}

int NpcManager::fn_80547B54() {
    NpcTask** npc = mNpcs;
    int loading = 0;
    for (unsigned int i = 0; i < 6; i++, npc++) {
        if (*npc != 0 && ((*npc)->mFlags & 4) != 4) {
            if (((*npc)->mFlags & 2) == 2) {
                (*npc)->fn_800D246C(0);
                (*npc)->mFlags |= 4;
                for (unsigned int j = 0; j < (*npc)->GetModel_unk()->mMotionCount; j++) {
                    (*npc)->GetModel_unk()->fn_80097A20(j);
                }
                (*npc)->m8C.fn_80473F88();
            } else {
                loading = 1;
            }
        }
    }
    return loading;
}
