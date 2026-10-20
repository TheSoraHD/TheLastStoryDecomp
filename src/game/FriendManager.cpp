#include "game/FriendManager.h"
#include "atn/Memory.h"
#include "game/CharaManager.h"
#include "game/PlayerTask.h"

extern "C" CharaManager<PlayerTask>* lbl_8087F8A0; // inferred: player manager

FriendManager* FriendManager::fn_80365464(atn::Task* parent) {
    if (parent != 0 && sInstance == 0) {
        sInstance = new (3, "", "", 0) FriendManager(parent);
    }
    return sInstance;
}

FriendManager::~FriendManager() {
    sInstance = 0;
}

// inferred: second player of the list (the first friend after the leader); `this` is unused
CharaTask* FriendManager::fn_8036554C() {
    return lbl_8087F8A0->mHead->mNext;
}
