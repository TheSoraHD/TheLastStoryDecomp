#ifndef GAME_GAMELOADBACKGROUND_H
#define GAME_GAMELOADBACKGROUND_H

#include "game/GameSaveLoadBackgroundCommon.h"

class GameLoadBackground : public GameSaveLoadBackgroundCommon {
public:
    GameLoadBackground(atn::Task* parent) : GameSaveLoadBackgroundCommon(parent), m158(0) {}
    virtual ~GameLoadBackground() {}
    virtual void vf10();

    // inferred: factory
    static GameLoadBackground* fn_803B33F8(atn::Task* parent);
    void fn_803B3530(const char* name, int arg1, int arg2, int arg3); // inferred: starts loading

    int m158;
    int m15C;
};

#endif
