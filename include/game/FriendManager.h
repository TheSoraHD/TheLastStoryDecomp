#ifndef GAME_FRIENDMANAGER_H
#define GAME_FRIENDMANAGER_H

#include "atn/Task.h"

class FriendManager : public atn::Task {
public:
    FriendManager(atn::Task* parent) : atn::Task(parent) {}
    virtual ~FriendManager();

    // inferred: creates the singleton under `parent` if it does not exist yet
    static FriendManager* fn_80365464(atn::Task* parent);

    static FriendManager* sInstance; // inferred name (0x8087F428)
};

#endif
