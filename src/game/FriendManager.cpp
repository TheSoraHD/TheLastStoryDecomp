#include "game/FriendManager.h"
#include "atn/Memory.h"

FriendManager::~FriendManager() {
    sInstance = 0;
}

FriendManager* FriendManager::fn_80365464(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (3, "", "", 0) FriendManager(parent);
    }
    return sInstance;
}
