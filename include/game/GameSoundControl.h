#ifndef GAME_GAMESOUNDCONTROL_H
#define GAME_GAMESOUNDCONTROL_H

#include "atn/Task.h"
#include "game/ResHandle.h"

// inferred from __vt__16GameSoundControl (0x8078BDD0): singleton task that picks the BGM and the
// volume of the sound groups (created by GameTask)
class GameSoundControl : public atn::Task {
public:
    GameSoundControl(atn::Task* parent)
        : atn::Task(parent), m4C(0), m50(0), m54(0), m58(0), m5C(0.5f), m60(15), m64(0) {}
    virtual ~GameSoundControl();
    virtual void vf10();

    static GameSoundControl* fn_803CD9A8(atn::Task* parent); // inferred: creates the singleton
    // inferred: play a BGM according to the battle state (called from the field code)
    static void fn_803CDAE4();
    static void fn_803CDB64();
    static void fn_803CDB68();
    static void fn_803CDC48();
    static void fn_803CDF84();

    ResHandle m48;
    int m4C;
    int m50;
    int m54;
    int m58;
    float m5C;
    int m60;
    int m64;
    unsigned char m68[0x88 - 0x68];

    static GameSoundControl* sInstance; // 0x8087F480
};

#endif
