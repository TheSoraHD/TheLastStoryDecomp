#include "game/NpcManager.h"
#include "atn/Memory.h"

int NpcManager::vf0C() {
    if (fn_800D3FA4()) {
        m58 = mRes.Load("NpcManager");
        return 1;
    }
    return 0;
}

NpcManager::~NpcManager() {
    sInstance = 0;
}

NpcManager* NpcManager::fn_805477C8(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (3, "", "", 0) NpcManager(parent);
    }
    return sInstance;
}
