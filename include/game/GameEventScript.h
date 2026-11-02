#ifndef GAME_GAMEEVENTSCRIPT_H
#define GAME_GAMEEVENTSCRIPT_H

#include "atn/Task.h"

class CharaTask;

// inferred from __vt__15GameEventScript: event script of a scene (created by fn_80393610 for
// GameScene::m134); only the members used by game/GameScene.cpp, method names invented
class GameEventScript : public atn::Task {
public:
    int fn_803ABFD4(atn::Task* chara);
    int fn_803AC3D8(CharaTask* chara, int a, int b, float range);
    int fn_803AC708(CharaTask* chara);
    int fn_803AC978(CharaTask* chara);
    void fn_803ACD08();

    unsigned char m48[0x8C - 0x48];
    int m8C; // inferred: a script is running
    unsigned char m90[0xB4 - 0x90];
    int mB4;
};

#endif
