#include "game/EnemyManager.h"
#include "atn/Memory.h"

int EnemyManager::vf0C() {
    if (fn_800D3FA4()) {
        m58 = mRes.Load("EnemyManager");
        return 1;
    }
    return 0;
}

EnemyManager::~EnemyManager() {
    sInstance = 0;
}

EnemyManager* EnemyManager::fn_80334B38(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (3, "", "", 0) EnemyManager(parent);
    }
    return sInstance;
}
