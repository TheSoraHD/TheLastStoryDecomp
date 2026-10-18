#ifndef GAME_GAMESAVEBACKGROUND_H
#define GAME_GAMESAVEBACKGROUND_H

#include "game/GameSaveLoadBackgroundCommon.h"

// inferred: 0x20-byte save header filled from global state (class name unknown)
class Unk_803CE3B8 {
public:
    Unk_803CE3B8(); // 0x803CE3B8
    void fn_803CE408();
    void fn_803CE448();

    int m00;
    int m04;
    short m08;
    unsigned short m0A;
    unsigned char m0C[0x14];
};

class GameSaveBackground : public GameSaveLoadBackgroundCommon {
public:
    GameSaveBackground(atn::Task* parent) : GameSaveLoadBackgroundCommon(parent), m158(0), m15C(-1), m160(0) {
        m148 = 1;
    }
    virtual ~GameSaveBackground() {}
    virtual int vf0C();
    virtual void vf10();

    // inferred: factory
    static GameSaveBackground* fn_803B8C6C(atn::Task* parent);
    void fn_803B8E60(void* data, int arg1, int arg2);                                // inferred
    void fn_803B8F50(const char* name, void* data, int size, int arg, int flag);    // inferred

    void* m158;
    int m15C;
    unsigned char m160;
    unsigned char m164[0x180 - 0x164];
    Unk_803CE3B8 mHeader; // 0x180
};

#endif
